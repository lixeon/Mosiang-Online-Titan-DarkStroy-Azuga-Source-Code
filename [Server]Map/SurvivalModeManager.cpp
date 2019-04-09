#include "stdafx.h"
#include "UserTable.h"
#include "Player.h"
#include "Network.h"
#include "ObjectStateManager.h"
#include "MapDBMsgParser.h"
#include "QuestManager.h"
#include "PackedData.h"

#include ".\survivalmodemanager.h"

GLOBALTON(CSurvivalModeManager);

CSurvivalModeManager::CSurvivalModeManager(void)
{
	m_SVModeUserTable.Initialize(50);
	m_SVItemUsingCounter.Initialize(50);

	Init();
}

CSurvivalModeManager::~CSurvivalModeManager(void)
{
	Release();
}

void CSurvivalModeManager::Init()
{
	m_wModeState = eSVVMode_None;
	m_dwStateRemainTime = 0;
	m_nUserAlive = 0;

	m_dwUsingCountLimit = 5;	//기본 5
}

void CSurvivalModeManager::Release()
{
	m_SVModeUserTable.RemoveAll();
	m_SVItemUsingCounter.RemoveAll();

	DWORD* pCounter = NULL;
	m_SVItemUsingCounter.SetPositionHead();
	while( pCounter = m_SVItemUsingCounter.GetData() )
	{
		delete pCounter;
	}
	m_SVModeAliveUserList.RemoveAll();
}

void CSurvivalModeManager::Process()
{

	switch(m_wModeState)
	{
	case eSVVMode_None:
		{
			//GM 에 의해 Ready 로 변화
		}
		break;
	case eSVVMode_Ready:
		{
			//10초 카운트 후 Fight 로 변화
			if( CheckRemainTime() )
			{
				ChangeStateTo(eSVVMode_Fight);
			}
		}
		break;
	case eSVVMode_Fight:
		{
			//1명 살아남을 시 End 로 변화
		}
		break;
	case eSVVMode_End:
		{
			//10초 카운트 후 Ready 로 변화
			if( CheckRemainTime() )
			{
				ChangeStateTo(eSVVMode_None);
			}
		}
		break;
	default:
		break;
	}
}

void CSurvivalModeManager::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pTempMsg->dwObjectID );
	if(pSender)
	{
		if( pSender->GetUserLevel() > eUSERLEVEL_GM && Protocol != MP_SURVIVAL_LEAVE_SYN )
		{
			char buf[64];
			sprintf(buf, "User Lvl Chk! Character_idx : %d", pTempMsg->dwObjectID);
			ASSERTMSG(0, buf);
			return;
		}
	}

	switch( Protocol )
	{
	case MP_SURVIVAL_READY_SYN:
		{
			if( m_wModeState != eSVVMode_None )
				SendNackMsg(pSender, MP_SURVIVAL_READY_NACK, 1);
			else
				ChangeStateTo(eSVVMode_Ready);
		}
		break;
	case MP_SURVIVAL_STOP_SYN:
		{
			if( m_wModeState != eSVVMode_Fight )
				SendNackMsg(pSender, MP_SURVIVAL_STOP_NACK, 1);
			else
				ChangeStateTo(eSVVMode_None);
		}
		break;
	case MP_SURVIVAL_MAPOFF_SYN:
		{
			//유저들 먼저 맵으로 되돌리기.
			ReturnToMap();
		}
		break;
	case MP_SURVIVAL_LEAVE_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			CObject* pObject = g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pObject != NULL )
			{
				if( pObject->GetObjectKind() == eObjectKind_Player )
				{
					MSG_DWORD msg;
					SetProtocol( &msg, MP_SURVIVAL, MP_USERCONN_GAMEIN_NACK );
					msg.dwData = pmsg->dwObjectID;
					g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
					return;
				}
			}

			CPlayer* pPlayer = g_pServerSystem->AddPlayer( pmsg->dwObjectID, dwConnectionIndex, pmsg->dwData1, pmsg->dwData3 );
			if(!pPlayer) return;

			pPlayer->SetUserLevel( pmsg->dwData2 );
			CharacterNumSendAndCharacterInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterMugongInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterItemOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			//SW050920 Rare
			CharacterItemRareOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterAbilityInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);

			QuestTotalInfo(pmsg->dwObjectID);

			QUESTMGR->CreateQuestForPlayer( pPlayer );
			// 항상 서브퀘스트 정보부터 읽어야 함...
//			QuestMainQuestLoad(pmsg->dwObjectID);
			QuestSubQuestLoad(pmsg->dwObjectID);
//			QuestItemload(pmsg->dwObjectID);			
		}
		break;
	case MP_SURVIVAL_ITEMUSINGCOUNT_SET:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			SetUsingCountLimit(pmsg->dwData);
		}
		break;
	default:
		break;
	}
}

void CSurvivalModeManager::SendMsgToAllSVModeUser(MSGBASE* pMsg, int msgsize)
{
	/*CObject* pObj = NULL;
	m_SVModeUserTable.SetPositionHead();
	while( pObj = m_SVModeUserTable.GetData() )
	{
		if(pObj->GetObjectKind() == eObjectKind_Player)
		{
			((CPlayer*)pObj)->SendMsg(pMsg,msgsize);
		}
	}*/
	g_pUserTable->SetPositionUserHead();
	while( CObject* pObject = g_pUserTable->GetUserData() )
	{
		if( pObject->GetObjectKind() != eObjectKind_Player ) continue;

		CPlayer* pSVUser = (CPlayer*)pObject;
		pSVUser->SendMsg(pMsg, msgsize);
	}
}

void CSurvivalModeManager::SendAliveUserCount()
{
	MSG_DWORD msg;
	msg.Category = MP_SURVIVAL;
	msg.Protocol = MP_SURVIVAL_ALIVEUSER_COUNT;
	msg.dwData = m_SVModeAliveUserList.GetCount();
	SendMsgToAllSVModeUser(&msg, sizeof(msg));
}

void CSurvivalModeManager::SendNackMsg(CPlayer* pGM, BYTE Protocol, BYTE errstate)
{
	MSG_BYTE nmsg;
	nmsg.Category = MP_SURVIVAL;
	nmsg.Protocol = Protocol;
	nmsg.bData = errstate;
	pGM->SendMsg(&nmsg, sizeof(nmsg));
}

BOOL CSurvivalModeManager::CheckRemainTime()
{// 상태별 유효 시간을 체크한다.
	if(m_dwStateRemainTime)
	{
		if(gCurTime > m_dwStateRemainTime)
		{
			m_dwStateRemainTime = 0;
			return TRUE;
		}
		else
			return FALSE;
	}

	return FALSE;
}

void CSurvivalModeManager::SetUsingCountLimit( DWORD limit )
{
	m_dwUsingCountLimit = limit;

	MSG_DWORD msg;
	msg.Category = MP_SURVIVAL;
	msg.Protocol = MP_SURVIVAL_ITEMUSINGCOUNT_LIMIT;
	msg.dwData = limit;

	SendMsgToAllSVModeUser( &msg, sizeof(msg) );
}

BOOL CSurvivalModeManager::AddItemUsingCount( CPlayer* pPlayer )
{
	DWORD* pCounter = NULL;
	pCounter = m_SVItemUsingCounter.GetData( pPlayer->GetID() );
	if( pCounter )
	{
		if( *pCounter >= m_dwUsingCountLimit )
		{
			return FALSE;
		}
		else
		{
			(*pCounter)++;
			return TRUE;
		}
	}
	return FALSE;
}

void CSurvivalModeManager::ChangeStateTo( WORD nextState )
{// 상태 변화와 해당 처리
	if(m_wModeState == nextState)
		return;
	
	m_wModeState = nextState;

	switch(nextState)
	{
	case eSVVMode_None:
		{
			m_dwStateRemainTime = 0;

			//사람 다 살리고 초기화.
			ReadyToSurvivalMode();

			SendAliveUserCount();

			MSGBASE msg;
			msg.Category = MP_SURVIVAL;
			msg.Protocol = MP_SURVIVAL_NONE_ACK;
			
			SendMsgToAllSVModeUser(&msg, sizeof(msg));
		}
		break;
	case eSVVMode_Ready:
		{
			m_dwStateRemainTime = gCurTime + eSVVMD_TIME_READY;

			//m_nUserAlive = m_SVModeUserTable.GetDataNum();

			MSGBASE msg;
			msg.Category = MP_SURVIVAL;
			msg.Protocol = MP_SURVIVAL_READY_ACK;

			SendMsgToAllSVModeUser(&msg, sizeof(msg));
		}
		break;
	case eSVVMode_Fight:
		{
			m_dwStateRemainTime = 0;
			MSGBASE msg;
			msg.Category = MP_SURVIVAL;
			msg.Protocol = MP_SURVIVAL_FIGHT_ACK;

			SendMsgToAllSVModeUser(&msg, sizeof(msg));
		}
		break;
	case eSVVMode_End:
		{
			m_dwStateRemainTime = gCurTime + eSVVMD_TIME_END;

			//살아있는 한명을 찾아 만피 세팅해주고
			PTRLISTPOS pos = m_SVModeAliveUserList.GetHeadPosition();

			CPlayer* pTheWinner = (CPlayer*)m_SVModeAliveUserList.GetAt(pos);

			if(pTheWinner)
			{
				//지속스킬로 죽는것 막자. -_-;
				pTheWinner->SetLife(pTheWinner->GetMaxLife());

				////이펙트 처리 -> MSG_SVVMODE_ENDINFO 하나로 처리하자.
				//MSG_DWORD Msg;
				//Msg.Category = MP_SURVIVAL;
				//Msg.Protocol = MP_SURVIVAL_WINNER_EFFECT;
				//Msg.dwObjectID = pTheWinner->GetID();
				//Msg.dwData = pTheWinner->GetID();

				//PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pTheWinner, &Msg, sizeof(MSG_DWORD));

				MSG_SVVMODE_ENDINFO msg;
				msg.Category = MP_SURVIVAL;
				msg.Protocol = MP_SURVIVAL_END_ACK;
				msg.WinnerID = pTheWinner->GetID();
				//ID 처리 안하는 이유는 다른 유저의 그리드 밖일 수 있기 때문!
				SafeStrCpy( msg.WinnerName, pTheWinner->GetObjectName(), MAX_NAME_LENGTH+1);

				SendMsgToAllSVModeUser(&msg, sizeof(msg));
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}
}

void CSurvivalModeManager::ReadyToSurvivalMode()
{// 모든 유저를 살리고 준비 상태에 들어간다.
	m_SVModeAliveUserList.RemoveAll();

	DWORD* pCounter = NULL;

	CObject* pObj = NULL;
	m_SVModeUserTable.SetPositionHead();
	while( pObj = m_SVModeUserTable.GetData() )
	{
		if(pObj->GetObjectKind() == eObjectKind_Player)
		{
			//살리기
			CPlayer* pPlayer = ((CPlayer*)pObj);

			if( OBJECTSTATEMGR_OBJ->GetObjectState(pObj) == eObjectState_Die )
				pPlayer->RevivePresentSpot();

			pPlayer->SetLife(pPlayer->GetMaxLife());
			pPlayer->SetNaeRyuk(pPlayer->GetMaxNaeRyuk());
			pPlayer->SetShield(pPlayer->GetMaxShield());

			m_SVModeAliveUserList.AddTail(pObj);

			//SW061129 홍콩추가요청작업 - 사용갯수제한
			pCounter = m_SVItemUsingCounter.GetData(pObj->GetID());
			if(pCounter)
				*pCounter = 0;
		}
	}
}

void CSurvivalModeManager::ReturnToMap()
{
	MSG_DWORD msg;
	msg.Category = MP_SURVIVAL;
	msg.Protocol = MP_SURVIVAL_RETURNTOMAP;

	CObject* pObj = NULL;
	m_SVModeUserTable.SetPositionHead();
	while( pObj = m_SVModeUserTable.GetData() )
	{
		if(pObj->GetObjectKind() == eObjectKind_Player )
		{
			CPlayer* pPlayer = ((CPlayer*)pObj);

			msg.dwData = pPlayer->GetReturnMapNum();
			pPlayer->SendMsg(&msg, sizeof(msg));

			g_pServerSystem->RemovePlayer( pPlayer->GetID() );
		}
	}
}

void CSurvivalModeManager::AddSVModeUser( CObject* pObject )
{
	if( pObject->GetObjectKind() == eObjectKind_Player )
	if( ((CPlayer*)pObject)->GetUserLevel() <= eUSERLEVEL_GM )
		return;

	if( m_SVModeUserTable.GetData( pObject->GetID() ) )
		return;

	CObject* pObj = m_SVModeUserTable.GetData( pObject->GetID() );
	if( pObj )
	{
		char buf[128] = {0,};
		sprintf(buf, "User Duplication! Name: %s ID: %d", pObj->GetObjectName(), pObj->GetID() );
		ASSERTMSG(0, buf);
		return;
	}
	m_SVModeUserTable.Add( pObject, pObject->GetID() );

	AddAliveUser(pObject);

	//SW061129 홍콩추가요청작업 - 사용갯수제한
	DWORD* pCounter = new DWORD;
	*pCounter = 0;
	m_SVItemUsingCounter.Add( pCounter, pObject->GetID() );

	MSG_DWORD msg;
	msg.Category = MP_SURVIVAL;
	msg.Protocol = MP_SURVIVAL_ITEMUSINGCOUNT_LIMIT;
	msg.dwData = m_dwUsingCountLimit;
	((CPlayer*)pObject)->SendMsg(&msg, sizeof(msg));

	//if( pObject->GetLife() )	//DB세팅이전이라 0값.	//Inited 이후로 변경?
		//AddAliveUserCount(TRUE);
}

void CSurvivalModeManager::RemoveSVModeUser( CObject* pObject )
{
	if( pObject->GetObjectKind() == eObjectKind_Player )
	if( ((CPlayer*)pObject)->GetUserLevel() <= eUSERLEVEL_GM )
		return;

	CObject* pObj = m_SVModeUserTable.GetData( pObject->GetID() );

	if(pObj)
	{
		if(pObj->GetObjectKind() != eObjectKind_Player )
		{
			char buf[128] = {0,};
			sprintf(buf, "SVVMODE_ObjectTable ERROR!! OBJ_ID: %d OBJ_NAME: %s", pObj->GetID(), pObj->GetObjectName() );
			ASSERTMSG(0, buf);
		}

		m_SVModeUserTable.Remove( pObject->GetID() );

		//if( pObj->GetLife() )	//이거 뺄가..
		RemoveAliveUser(pObj);

		//SW061129 홍콩추가요청작업 - 사용갯수제한
		DWORD* pCounter = m_SVItemUsingCounter.GetData( pObj->GetID() );
		if(pCounter)
			delete pCounter;
		m_SVItemUsingCounter.Remove( pObj->GetID() );

	}
	else
		return;

}

void CSurvivalModeManager::AddAliveUser( CObject* pObject )
{	//생존자 리스트
	m_SVModeAliveUserList.AddTail(pObject);

	SendAliveUserCount();
}

void CSurvivalModeManager::RemoveAliveUser( CObject* pObject )
{
	m_SVModeAliveUserList.Remove(pObject);

	int AliveCount = m_SVModeAliveUserList.GetCount();

	SendAliveUserCount();

#define THE_ONE	1
	if( THE_ONE == AliveCount && eSVVMode_Fight == GetCurModeState())
	{
		ChangeStateTo(eSVVMode_End);
	}
}

//int CSurvivalModeManager::AddAliveUserCount( BOOL bPlus )
//{
//	int before = m_nUserAlive;
//
//	if(bPlus)	//+
//		m_nUserAlive++;
//	else		//-
//		m_nUserAlive--;
//
//	//모든 유저에게 카운트 정보 보내기
//	MSG_DWORD msg;
//	msg.Category = MP_SURVIVAL;
//	msg.Protocol = MP_SURVIVAL_ALIVEUSER_COUNT;
//	msg.dwData = m_nUserAlive;
//	SendMsgToAllSVModeUser(&msg, sizeof(msg));
//		
//	return m_nUserAlive;
//
//	//if( GetCurModeState() == eSVVMode_Fight )
//	if( before > 1 && m_nUserAlive == 1 )
//	{
//		ChangeStateTo(eSVVMode_End);
//	}
//}
