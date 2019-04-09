// TacticManager.cpp: implementation of the CTacticManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TacticManager.h"

#include "Gamein.h"

#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "PartyManager.h"
#include "MoveManager.h"

#include "Player.h"

#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CTacticManager);
CTacticManager::CTacticManager()
{
	m_TacticInfoTable.Initialize(64);
}

CTacticManager::~CTacticManager()
{
	m_TacticInfoTable.SetPositionHead();
	while(CTacticStartInfo* pInfo = m_TacticInfoTable.GetData())
	{
		delete pInfo;
	}
	m_TacticInfoTable.RemoveAll();
}

void CTacticManager::AddTacticStartInfo(TACTICSTART_INFO* pList)
{
	ASSERT(m_TacticInfoTable.GetData(pList->TacticID) == NULL);

	CTacticStartInfo* pInfo = new CTacticStartInfo(pList);
	m_TacticInfoTable.Add(pInfo,pList->TacticID);
}

CTacticStartInfo* CTacticManager::GetTacticStartInfo(WORD TacticID)
{
	CTacticStartInfo* tacticinfo = m_TacticInfoTable.GetData(TacticID);
	return tacticinfo;
}

void CTacticManager::HeroTacticStart(WORD TacticID)
{
	MOVEMGR->HeroMoveStop();
		
	if(HERO->GetPartyIdx() == 0)
	{
//		GAMEIN->GetChatDialog()->AddChatMsg(WHOLE,RGBA_MAKE(255,0,0,255),"파티원이 아니어서 진법을 시전할 수 없습니다.");//pjslocal
		CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(101) );
		return;
	}

	DWORD TacticObjectID = PARTYMGR->GetTacticObjectID();
	if(TacticObjectID != 0)
	{	
		CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(104) );
		return;
	}

	CTacticStartInfo* TacticInfo = GetTacticStartInfo(TacticID);
	if(TacticInfo->IsTacticableState(HERO) == FALSE)
	{
//		GAMEIN->GetChatDialog()->AddChatMsg(WHOLE,RGBA_MAKE(255,0,0,255),"진법을 시전할 수 없습니다.");//pjslocal
		CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(102) );
		return;
	}
	if(TacticInfo->IsTacticablePosition(HERO) == FALSE)
	{
//		GAMEIN->GetChatDialog()->AddChatMsg(WHOLE,RGBA_MAKE(255,0,0,255),"진법을 시전할 수 있는 위치가 아닙니다..");//pjslocal
		CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(103) );
		return;
	}	

	SEND_TACTIC_START msg;
	msg.Category = MP_TACTIC;
	msg.Protocol = MP_TACTIC_START_SYN;
	msg.dwObjectID = HEROID;
	msg.OperatorID = HEROID;
	msg.TacticID = TacticID;
	msg.Direction = DEGTODIR(HERO->GetAngleDeg());
	NETWORK->Send(&msg, sizeof(msg));

	OBJECTSTATEMGR->StartObjectState(HERO,eObjectState_Tactic);

//	GAMEIN->GetChatDialog()->AddChatMsg(WHOLE,RGBA_MAKE(255,0,0,255), "TACTICSTART SYN SEND");
//	CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(235) );
	
}

void CTacticManager::HeroTacticJoin()
{
	MOVEMGR->HeroMoveStop();

	DWORD TacticObjectID = PARTYMGR->GetTacticObjectID();
	if(TacticObjectID == 0) return;
	CTacticObject* pTacticObj = GetTacticObject(TacticObjectID);
	if(pTacticObj == 0)
	{		
		return;
	}

	int pos = pTacticObj->CheckJoin(HERO);		// 조인할수 있다면 position[0~6]값이 아니면 -1이 넘어온다
	if(pos == -1)
		return;
	
	SEND_TACTIC_JOIN msg;
	msg.Category = MP_TACTIC;
	msg.Protocol = MP_TACTIC_JOIN_SYN;
	msg.dwObjectID = HEROID;
	msg.TacticObjectID = TacticObjectID;
	msg.JoinedPlayerID = HEROID;
	msg.Position = pos;
	NETWORK->Send(&msg, sizeof(msg));

	OBJECTSTATEMGR->StartObjectState(HERO,eObjectState_Tactic);

//	GAMEIN->GetChatDialog()->AddChatMsg(WHOLE,RGBA_MAKE(255,0,0,255), "TACTICJOIN SYN SEND");
//	CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(235) );
}

void CTacticManager::OnFailedTacticStart(DWORD FailedReason)
{
	ySWITCH(FailedReason)
		yCASE(eTSEC_NotTacticableState)
			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(102) );
		yCASE(eTSEC_NotTacticablePosition)
			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(103) );
		yCASE(eTSEC_AlreadyPartyDoingTactic)
			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(104) );
		yCASE(eTSEC_NoParty)
			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(101) );
	yENDSWITCH

	OBJECTSTATEMGR->EndObjectState(HERO,eObjectState_Tactic);
}

void CTacticManager::OnFailedTacticJoin()
{
	OBJECTSTATEMGR->EndObjectState(HERO,eObjectState_Tactic);
}

void CTacticManager::OnTacticJoinAcked(SEND_TACTIC_JOIN* pmsg)
{
	CTacticObject* pTacticObj = GetTacticObject(pmsg->TacticObjectID);
	CObject* pJoinedObj = OBJECTMGR->GetObject(pmsg->JoinedPlayerID);
	if(pTacticObj == NULL || pJoinedObj == NULL)
		return;
	
	pTacticObj->StartJoinTacticEffect(pJoinedObj,pmsg->Position);

//	GAMEIN->GetChatDialog()->AddChatMsg(WHOLE,RGBA_MAKE(255,0,0,255), "TACTICJOIN ACKED");
//	CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(236) );
}

void CTacticManager::RegistTacticObject(SEND_TACTIC_TOTALINFO* sendtacticinfo)
{
	CTacticObject* pObj = OBJECTMGR->AddTacticObject(&sendtacticinfo->baseinfo,&sendtacticinfo->tacticinfo);
	pObj->TacticStartEffect();

//	GAMEIN->GetChatDialog()->AddChatMsg(WHOLE,RGBA_MAKE(255,0,0,255), "REGIST TACTICOBJECT");
//	CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(237) );
}

CTacticObject* CTacticManager::GetTacticObject(DWORD TacticObjectID)
{
	return (CTacticObject*)OBJECTMGR->GetObject(TacticObjectID);
}

void CTacticManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol) 
	{
	case MP_TACTIC_START_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			PARTYMGR->SetTacticObjectID(pmsg->dwData);

//			GAMEIN->GetChatDialog()->AddChatMsg(WHOLE,RGBA_MAKE(255,0,0,255), "TACTICSTART_ ACK");
//			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(238) );
		}
		break;
	case MP_TACTIC_START_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			OnFailedTacticStart(pmsg->dwData);
		}
		break;
	case MP_TACTIC_JOIN_ACK:
		{
			OnTacticJoinAcked((SEND_TACTIC_JOIN*)pMsg);
		}
		break;
	case MP_TACTIC_JOIN_NACK:
		{
			OnFailedTacticJoin();
		}
		break;
	case MP_TACTIC_OBJECT_ADD:
		{
			RegistTacticObject((SEND_TACTIC_TOTALINFO*)pMsg);
			
			HeroTacticJoin();
		}
		break;
	case MP_TACTIC_FAIL:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CTacticObject* pTacObj = GetTacticObject(pmsg->dwData1);
			
			BOOL bMsg = FALSE;
			if(PARTYMGR->GetTacticObjectID() == pmsg->dwData1)
			{
				PARTYMGR->SetTacticObjectID(0);
				bMsg = TRUE;
			}

			if(pTacObj == NULL)
				return;
			pTacObj->OnFailedTactic(pmsg->dwData2, bMsg);

//			GAMEIN->GetChatDialog()->AddChatMsg(WHOLE,RGBA_MAKE(255,0,0,255), "TACTIC_FAIL");
//			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(239) );
		}
		break;
	case MP_TACTIC_EXECUTE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CTacticObject* pTacObj = GetTacticObject(pmsg->dwData);
						
			if(PARTYMGR->GetTacticObjectID() == pmsg->dwData)
				PARTYMGR->SetTacticObjectID(0);

			if(pTacObj == NULL)
				return;
			
			pTacObj->ExcuteTactic();
		}
		break;
	} //SWITCH
}

