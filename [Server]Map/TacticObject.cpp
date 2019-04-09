// TacticObject.cpp: implementation of the CTacticObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TacticObject.h"
#include "TacticStartInfo.h"
#include "PackedData.h"
#include "UserTable.h"
#include "CharMove.h"
#include "PartyManager.h"
#include "Player.h"
#include "objectstatemanager.h"
#include "Party.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DURATION_TACTIC_NAERYUKREDUCE	1000

CTacticObject::CTacticObject()
{

}

CTacticObject::~CTacticObject()
{

}

void CTacticObject::InitTacticObject(CTacticStartInfo* pInfo,CPlayer* pOperator,DIRINDEX Direction,BYTE TacticAbilityLevel)
{
	m_JoinLvl = pOperator->GetLevel();	
	m_OperatorAbilityLevel = TacticAbilityLevel;
	m_bExecute = FALSE;

	SetBattle(pOperator->GetBattleID(),pOperator->GetBattleTeam());
	SetGridID(pOperator->GetGridID());
	
	m_pTacticInfo = pInfo;
	memset(m_TotalInfo.JoinedPlayerIDs,0,sizeof(DWORD)*MAX_PARTY_LISTNUM);
	m_TotalInfo.TacticId = pInfo->GetTacticID();
	m_TotalInfo.JoinedPlayerIDs[0] = pOperator->GetID();
	m_TotalInfo.Direction = Direction;
	m_TotalInfo.StartTime = gCurTime;

	m_TacticLevel = pOperator->GetMugongLevel(pInfo->GetTacticID());
	ASSERT(m_TacticLevel > 0);

	m_LastNaeryukReduceTime = gCurTime;

	VECTOR3* pPos = CCharMove::GetPosition(pOperator);
	m_TotalInfo.Pos.x = pPos->x;
	m_TotalInfo.Pos.y = 0;
	m_TotalInfo.Pos.z = pPos->z;

	m_PartyID = pOperator->GetPartyIdx();
	PARTYMGR->SetTacticObjectID(pOperator->GetPartyIdx(),GetID());
	
	MSG_DWORD msg;
	msg.Category = MP_TACTIC;
	msg.Protocol = MP_TACTIC_START_ACK;
	msg.dwObjectID = pOperator->GetID();
	msg.dwData = GetID();

	PARTYMGR->SendPartyMsg(pOperator->GetPartyIdx(),&msg,sizeof(msg));
}

BOOL CTacticObject::TacticJoin(CPlayer* pObject,int Position)
{
	SEND_TACTIC_JOIN msg;

	if(Position >= m_pTacticInfo->GetTacticPlayerNum())
		goto tacticjoin_failed;
	if(m_TotalInfo.JoinedPlayerIDs[Position] != 0)
		goto tacticjoin_failed;

	m_TotalInfo.JoinedPlayerIDs[Position] = pObject->GetID();

	// 주변 클라이언트에게 메세지 보낸다.
	msg.Category = MP_TACTIC;
	msg.Protocol = MP_TACTIC_JOIN_ACK;
	msg.TacticObjectID = GetID();
	msg.JoinedPlayerID = pObject->GetID();
	msg.Position = Position;
	PACKEDDATA_OBJ->QuickSend(pObject,&msg,sizeof(msg));

	AddJoinLevel(pObject->GetLevel());

	return TRUE;

tacticjoin_failed:
	msg.Category = MP_TACTIC;
	msg.Protocol = MP_TACTIC_JOIN_NACK;
	msg.dwObjectID = pObject->GetID();
	msg.TacticObjectID = GetID();
	msg.JoinedPlayerID = pObject->GetID();
	msg.Position = Position;
	pObject->SendMsg(&msg,sizeof(msg));
	return FALSE;
}
BOOL CTacticObject::CheckFail()
{
	CPlayer* pObject;
	DWORD ElapsedTime;
		
	// 총시간이 지났는지 검사
	ElapsedTime = gCurTime - m_TotalInfo.StartTime;
	if(ElapsedTime > m_pTacticInfo->GetLimitTime())
	{  
		FailTactic(eTFR_TimeOver);
		return TRUE;
	}

	// 1초마다 내력다는 검사
	ElapsedTime = gCurTime - m_LastNaeryukReduceTime;
	if(ElapsedTime > DURATION_TACTIC_NAERYUKREDUCE)
	{
		m_LastNaeryukReduceTime += DURATION_TACTIC_NAERYUKREDUCE;
		for(int n=0;n<m_pTacticInfo->GetTacticPlayerNum() ;++n)
		{
			if(m_TotalInfo.JoinedPlayerIDs[n] == 0)
				continue;
			
			// 자리를 차지했던 유저가 나가면 Fail
			if((pObject = (CPlayer*)g_pUserTable->FindUser(m_TotalInfo.JoinedPlayerIDs[n])) == NULL)
			{
				FailTactic(eTFR_MemberDie);
				return TRUE;
			}
			
			// 내력 다는 처리
			pObject->ReduceNaeRyuk(m_pTacticInfo->GetReduceNaeRyuk());
			
			//내력이 0이 되면 Fail			
			if(pObject->GetNaeRyuk() < m_pTacticInfo->GetSuccessNaeRyuk(m_TacticLevel))
			{
				FailTactic(eTFR_Naeryuk);
				return TRUE;
			}
		}
	}
	
	return FALSE;	
}
BOOL CTacticObject::CheckExecute()
{
	if(m_bExecute)
		return TRUE;
	BOOL bFull = TRUE;

	// 자리가 전부 찼는지 검사
	for(int n=0;n<m_pTacticInfo->GetTacticPlayerNum() ;++n)
	{
		if(m_TotalInfo.JoinedPlayerIDs[n] == 0)
			bFull = FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// for test	
	/*
	DWORD ElapsedTime;
		
	// 총시간이 지났는지 검사
	ElapsedTime = gCurTime - m_TotalInfo.StartTime;
	
	if(ElapsedTime > 1000)
	{
		ExecuteTactic();
		return TRUE;
	}
	*/
	//////////////////////////////////////////////////////////////////////////
	
	if(bFull == TRUE)
	{
		if(ExecuteTactic() == FALSE)
		{
			FailTactic(eTFR_Naeryuk);
			m_bExecute = FALSE;
		}
		else
			m_bExecute = TRUE;
		
		return TRUE;
	}

	return FALSE;
}

void CTacticObject::FailTactic(int tfr)
{
	MSG_DWORD2 msg;
	msg.Category = MP_TACTIC;
	msg.Protocol = MP_TACTIC_FAIL;
	msg.dwData1 = GetID();
	msg.dwData2 = tfr;
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(this,&msg,sizeof(msg));
	PARTYMGR->SendPartyMsg(m_PartyID,&msg,sizeof(msg));
	
	PARTYMGR->SetTacticObjectID(m_PartyID,0);
	
	// 참여 인원의 상태 변경
	CObject* pObject;

	for(int n=0;n<m_pTacticInfo->GetTacticPlayerNum() ;++n)
	{
		if(m_TotalInfo.JoinedPlayerIDs[n] == 0)
			continue;

		pObject = g_pUserTable->FindUser(m_TotalInfo.JoinedPlayerIDs[n]);
		if(pObject == NULL)
			continue;


		OBJECTSTATEMGR_OBJ->EndObjectState(pObject, eObjectState_Tactic, 0);
	}
}
BOOL CTacticObject::ExecuteTactic()
{
	// 참여 인원의 상태 변경
	CObject* pObject;

	for(int n=0;n<m_pTacticInfo->GetTacticPlayerNum() ;++n)
	{
		if(m_TotalInfo.JoinedPlayerIDs[n] == 0)
			continue;

		pObject = g_pUserTable->FindUser(m_TotalInfo.JoinedPlayerIDs[n]);
		if(pObject == NULL)
			continue;

		OBJECTSTATEMGR_OBJ->EndObjectState(pObject, eObjectState_Tactic, 0);		

		if(n != 0)	// 시전자는 스킬에서 깍는다.
			pObject->ReduceNaeRyuk(m_pTacticInfo->GetSuccessNaeRyuk(m_TacticLevel));
	}

	// 메세지 보낸다.
	MSG_DWORD msg;
	msg.Category = MP_TACTIC;
	msg.Protocol = MP_TACTIC_EXECUTE;
	msg.dwData = GetID();
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(this,&msg,sizeof(msg));
	PARTYMGR->SendPartyMsg(m_PartyID,&msg,sizeof(msg));

/* hs
	CParty* pParty = PARTYMGR->GetParty(m_PartyID);
	if(pParty)
	{
		pParty->SetTacticObjectID(0);
	}
	*/


	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// Overrided Func
void CTacticObject::DoSetInited()
{
	
}
void CTacticObject::SetAddMsg(char* pAddMsg, WORD* pMsgLen, DWORD dwReceiverID,BOOL bLogin)
{
	SEND_TACTIC_TOTALINFO* msg = (SEND_TACTIC_TOTALINFO*)pAddMsg;

	msg->Category = MP_TACTIC;
	msg->Protocol = MP_TACTIC_OBJECT_ADD;
	msg->dwObjectID = dwReceiverID;
	GetBaseObjectInfo(&msg->baseinfo);
	GetTacticTotalInfo(&msg->tacticinfo);

	*pMsgLen = sizeof(SEND_TACTIC_TOTALINFO);
}
void CTacticObject::OnStartObjectState(BYTE State, DWORD dwParam)
{
	
}
void CTacticObject::OnEndObjectState(BYTE State)
{
	
}

void CTacticObject::GetTacticTotalInfo(TACTIC_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_TotalInfo,sizeof(TACTIC_TOTALINFO));
	pRtInfo->StartTime = gCurTime - m_TotalInfo.StartTime;
}

DWORD CTacticObject::GetJoinLevel()
{
	return m_JoinLvl;
}

void CTacticObject::AddJoinLevel(LEVELTYPE JoinLevel)
{	
	m_JoinLvl += JoinLevel;
}

BYTE CTacticObject::GetOperatorAbilityLevel()
{
	return m_OperatorAbilityLevel;
}

DWORD CTacticObject::GetTacticStartTime()
{
	return m_TotalInfo.StartTime;
}