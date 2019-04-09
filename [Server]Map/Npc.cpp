// Npc.cpp: implementation of the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Npc.h"
#include "ObjectStateManager.h"
#include "AISystem.h"
#include "MHError.h"
#include "UserTable.h"
#include "Player.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "CharMove.h"
#include "MapDBMsgParser.h"
#include "PackedData.h"
#include "BobusangManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpc::CNpc()
{

}

CNpc::~CNpc()
{

}


void CNpc::InitNpc(NPC_TOTALINFO* pTotalInfo)
{
	memcpy(&m_NpcInfo,pTotalInfo,sizeof(NPC_TOTALINFO));
	m_FirstTalkCharacterID = 0;
	m_DieTime = gCurTime + 60000*5;
}

void CNpc::GetNpcTotalInfo(NPC_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_NpcInfo,sizeof(NPC_TOTALINFO));
}


void CNpc::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
{
	SEND_NPC_TOTALINFO* pMsg = (SEND_NPC_TOTALINFO*)pAddMsg;
	pMsg->AddableInfo.Init();

	pMsg->Category = MP_USERCONN;
	pMsg->Protocol = MP_USERCONN_NPC_ADD;
	pMsg->dwObjectID = dwReceiverID;
	pMsg->Angle = m_Angle;
	GetSendMoveInfo(&pMsg->MoveInfo,&pMsg->AddableInfo);
	GetBaseObjectInfo(&pMsg->BaseObjectInfo);
	GetNpcTotalInfo(&pMsg->TotalInfo);
	pMsg->bLogin = bLogin;

	*pMsgLen = pMsg->GetMsgLength();
}

void CNpc::DoDie(CObject* pAttacker)
{
	//send msg
	

	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,m_FirstTalkCharacterID);
	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die,2000);
}

void CNpc::SetFirstTalkCharacterID(DWORD CharacterIdx)
{
	m_FirstTalkCharacterID = CharacterIdx;
}

DWORD CNpc::GetFirstTalkCharacterID()
{
	return m_FirstTalkCharacterID;
}

void CNpc::SetDieTime(DWORD time)
{
	m_DieTime = time;
}

void CNpc::Process()
{
	if( m_NpcInfo.NpcKind != BOBUSANG_NPCIDX )
	if(m_DieTime)
	{
		if(m_DieTime < gCurTime)
		{			
			DoDie(NULL);
			m_DieTime = 0;
		}
	}
}

void CNpc::OnEndObjectState(BYTE State)
{
	switch(State)
	{
	case eObjectState_Die:
		{
			MSG_DWORD msg;
			msg.Category = MP_NPC;
			msg.Protocol = MP_NPC_DIE_ACK;
			msg.dwData = GetID();
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(this, &msg, sizeof(msg));
			
			g_pServerSystem->RemoveNpc(GetID());
		}
		break;
	}
}

void CNpc::Release()
{
	m_DieTime = 0;
	m_FirstTalkCharacterID = 0;
//	g_pAISystem->ReleaseMonsterID(GetID());
	CObject::Release();
}

WORD CNpc::GetNpcJob()
{
	return m_NpcInfo.NpcJob;
}

void CNpc::DoJob(CPlayer* pPlayer, DWORD ConnectIdx)
{
	WORD job = GetNpcJob();
	switch(job)
	{
	case MAPCHANGE_ROLE://É½ºÓÆ×Àà,µØÍ¼×ª»»npc
		MapChangeRole(pPlayer, ConnectIdx);
		break;
	case BOMUL_ROLE://»ÊÁê±¦Îï,Ïä×ÓÀànpc
		BomulRole(pPlayer); 
		break;
	default:
		ASSERT(0); break;
	}
}

void CNpc::MapChangeRole(CPlayer* pPlayer, DWORD dwConnectionIndex)
{
	if(GetState() == eObjectState_Die)
	{
		SendNackMsg(pPlayer, 2); 
		return;
	}
	MAPCHANGE_INFO*	pChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum(12); //12 : Jang Ahn
	if( !pChangeInfo ) 
	{
		SendNackMsg(pPlayer, 3); return;
	}

	VECTOR3 ObjectPos	= *CCharMove::GetPosition(pPlayer);
	VECTOR3 TObjectPos	= *CCharMove::GetPosition(this);
	DWORD	Distance	= (DWORD)CalcDistanceXZ( &ObjectPos, &TObjectPos );
	if(Distance > 3000.0f)
	{
		SendNackMsg(pPlayer, 1); return;
	}

	pPlayer->UpdateLogoutToDB();
	
	LEVELTYPE lvl = pPlayer->GetLevel();
	EXPTYPE exp = pPlayer->GetPlayerExpPoint();
	g_pServerSystem->RemovePlayer(pPlayer->GetID());
	
	// ¸®ÅÏ ¹ÞÀ¸¸é ¸Ê ¾Æ¿ô ¸Þ½ÃÁö º¸³¿.
	SaveMapChangePointReturn(pPlayer->GetID(), pChangeInfo->Kind, dwConnectionIndex, lvl, exp, pPlayer->GetMoney(eItemTable_Pyoguk));	
}

void CNpc::SendNackMsg(CPlayer* pPlayer, BYTE err)
{
	MSG_DWORDBYTE msg;
	msg.Category = MP_NPC;
	msg.Protocol = MP_NPC_DOJOB_NACK;
	msg.bData = err;
	msg.dwData = GetUniqueIdx();
	pPlayer->SendMsg(&msg, sizeof(msg));
}

WORD CNpc::GetUniqueIdx()
{
	return m_NpcInfo.NpcUniqueIdx;
}

void CNpc::BomulRole(CPlayer* pPlayer)
{
	if(GetFirstTalkCharacterID() != pPlayer->GetID())
	{
		ASSERT(0);
		return;
	}
	DoDie(pPlayer);
}

void CNpc::AddChat(char * str)
{
	TESTMSG pmsg;
	pmsg.Category = MP_CHAT;
	pmsg.Protocol = MP_CHAT_FROMMONSTER_ALL;
	pmsg.dwObjectID = GetID();
	//strcpy(pmsg.Msg, str);
	SafeStrCpy( pmsg.Msg, str, MAX_CHAT_LENGTH+1 );
	//g_Console.LOG(4, pmsg->Msg);

	//CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	//if(!pPlayer) return;

	//PACKEDDATA_OBJ->QuickSend(this,&pmsg,sizeof(pmsg));
	PACKEDDATA_OBJ->QuickSend(this,&pmsg,pmsg.GetMsgLength());	//CHATMSG 040324
}
