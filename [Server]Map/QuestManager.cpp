// QuestManager.cpp: implementation of the CQuestManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestManager.h"

#include "MHFile.h"
#include "UserTable.h"
#include "..\[CC]Quest\QuestScriptLoader.h"
#include "..\[CC]Quest\QuestString.h"
#include "..\[CC]Quest\QuestInfo.h"
#include "QuestGroup.h"
#include "QuestUpdater.h"
#include "Player.h"
#include "Quest.h"
#include "MapDBMsgParser.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CQuestManager);

CQuestManager::CQuestManager()
{
	m_QuestInfoTable.Initialize(MAX_QUEST);
	memset( m_QuestEvent, 0, sizeof(MQUESTEVENT)*MAX_QUESTEVENT_MGR );
	m_dwQuestEventCount = 0;

	m_QuestUpdater = new CQuestUpdater;
}

CQuestManager::~CQuestManager()
{
	Release();
}

void CQuestManager::Release()
{
	CQuestInfo* pQuestInfo = NULL;
	m_QuestInfoTable.SetPositionHead();
	while( pQuestInfo = m_QuestInfoTable.GetData() )
	{
		delete pQuestInfo;
	}
	m_QuestInfoTable.RemoveAll();

	if( m_QuestUpdater )
	{
		delete m_QuestUpdater;
		m_QuestUpdater =NULL;
	}
}

BOOL CQuestManager::LoadQuestScript()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if( !file.Init( "./Resource/QuestScript/QuestScript.bin", "rb") )
		return FALSE;
#else
	if( !file.Init( "./Resource/QuestScript/QuestScript.txt", "rt") )
		return FALSE;
#endif
	/*
#ifdef _FILE_BIN_
	if( !file.Init( "./Quest/QuestScript.bin", "rb") )
		return FALSE;
#else
	if( !file.Init( "./Quest/QuestScript.txt", "rt") )
		return FALSE;
#endif
	*/

	char Token[256];
	DWORD dwQuestNum = 0;
	while( !file.IsEOF() )
	{
		file.GetString( Token );
		if( strcmp(Token,"$QUEST") == 0 )
		{
			dwQuestNum = file.GetDword();			
			m_QuestInfoTable.Add( CQuestScriptLoader::LoadQuestInfo( &file, dwQuestNum ), dwQuestNum );
		}
	}
	file.Release();
	
	return TRUE;
}

void CQuestManager::CreateQuestForPlayer( PLAYERTYPE* pPlayer )
{
	CQuestGroup* pQuestGroup = pPlayer->GetQuestGroup();
	if( pQuestGroup )
	{
		CQuestInfo* pQuestInfo = NULL;
		m_QuestInfoTable.SetPositionHead();
		for( DWORD i = 0; i < m_QuestInfoTable.GetDataNum(); ++i )
		{
			pQuestInfo = m_QuestInfoTable.GetData();
			if( pQuestInfo )
				pQuestGroup->CreateQuest( pQuestInfo->GetQuestIdx(), pQuestInfo );
		}
	}
}

BOOL CQuestManager::SetMainQuestData( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime )
{
	CQuestGroup* pQuestGroup = pPlayer->GetQuestGroup();
	if(pQuestGroup)
		return pQuestGroup->SetMainQuestData( dwQuestIdx, dwSubQuestFlag, dwData, dwTime, bCheckType, dwCheckTime );
	return TRUE;
}

void CQuestManager::SetSubQuestData( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwData, DWORD dwTime )
{
	CQuestGroup* pQuestGroup = pPlayer->GetQuestGroup();
	if(pQuestGroup)
		pQuestGroup->SetSubQuestData( dwQuestIdx, dwSubQuestIdx, dwData, dwTime );
}

void CQuestManager::SetQuestItem( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwItemIdx, int nItemNum )
{
	// 일단 item갯수가 0보다 작으면...
	if( nItemNum < 0 )
		return;
	CQuestGroup* pQuestGroup = pPlayer->GetQuestGroup();
	if(pQuestGroup)
		pQuestGroup->SetQuestItem( dwQuestIdx, dwItemIdx, nItemNum );
}

void CQuestManager::DeleteQuest( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, BOOL bTime )
{
	CQuestGroup* pQuestGroup = pPlayer->GetQuestGroup();
	if( !pQuestGroup )	return;

	if( pQuestGroup->IsQuestComplete( dwQuestIdx ) )
	{
		return;
	}

	pQuestGroup->DeleteQuest( dwQuestIdx );
	m_QuestUpdater->DeleteQuest( pPlayer, pQuestGroup->GetQuest(dwQuestIdx) );

	SEND_QUEST_IDX msg;
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_DELETE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.MainQuestIdx = (WORD)dwQuestIdx;
	msg.dwFlag = (DWORD)bTime;
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CQuestManager::StartQuest( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuest* pQuest = pPlayer->GetQuestGroup()->GetQuest( dwQuestIdx );
	if( !pQuest )	return;

	// DB 
	m_QuestUpdater->StartQuest( pPlayer, pQuest );
	JouranlInsertQuest(pPlayer->GetID(), eJournal_Quest, dwQuestIdx, dwSubQuestIdx, 0);

	// Network Send
	SEND_QUEST_IDX Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUEST_START_ACK;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.MainQuestIdx = (WORD)pQuest->GetQuestIdx();
	Msg.SubQuestIdx = 0;
	Msg.dwFlag = pQuest->GetSubQuestFlag();
	Msg.dwData = pQuest->IsQuestComplete();
	Msg.dwTime = pQuest->GetQuestTime();
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

void CQuestManager::EndQuest( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuest* pQuest = pPlayer->GetQuestGroup()->GetQuest( dwQuestIdx );
	if( !pQuest )	return;


	// DB
	if( dwSubQuestIdx )	// 반복
		m_QuestUpdater->DeleteQuest( pPlayer, pQuest );
	else				// 1번
		m_QuestUpdater->EndQuest( pPlayer, pQuest );

	JouranlInsertQuest(pPlayer->GetID(), eJournal_Quest, pQuest->GetQuestIdx(), dwSubQuestIdx, 1);
	
	// network send
	SEND_QUEST_IDX Msg;

	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUEST_END_ACK;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.MainQuestIdx = (WORD)pQuest->GetQuestIdx();
	Msg.SubQuestIdx = (WORD)dwSubQuestIdx;
	Msg.dwFlag = pQuest->GetSubQuestFlag();
	Msg.dwData = 1;
	Msg.dwTime = pQuest->GetQuestTime();
	pPlayer->SendMsg(&Msg, sizeof(Msg));

	
}

void CQuestManager::StartSubQuest( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuest* pQuest = pPlayer->GetQuestGroup()->GetQuest( dwQuestIdx );
	if( !pQuest )	return;

	// DB
	m_QuestUpdater->StartSubQuest( pPlayer, pQuest, dwSubQuestIdx );
//	JouranlInsertQuest(pPlayer->GetID(), eJournal_Quest, dwQuestIdx, dwSubQuestIdx, FALSE);

	// network send
	SEND_QUEST_IDX Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_SUBQUEST_START_ACK;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.MainQuestIdx = (WORD)pQuest->GetQuestIdx();
	Msg.SubQuestIdx = (WORD)dwSubQuestIdx;
	Msg.dwFlag = pQuest->GetSubQuestFlag();
	Msg.dwData = pQuest->GetSubQuestData( dwSubQuestIdx );
	Msg.dwTime = pQuest->GetSubQuestTime( dwSubQuestIdx );
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

void CQuestManager::EndSubQuest( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuest* pQuest = pPlayer->GetQuestGroup()->GetQuest( dwQuestIdx );
	if( !pQuest )	return;

	// DB
	m_QuestUpdater->EndSubQuest( pPlayer, pQuest, dwSubQuestIdx );
//	JouranlInsertQuest(pPlayer->GetID(), eJournal_Quest, dwQuestIdx, dwSubQuestIdx, TRUE);

	// network send
	SEND_QUEST_IDX Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_SUBQUEST_END_ACK;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.MainQuestIdx = (WORD)pQuest->GetQuestIdx();
	Msg.SubQuestIdx = (WORD)dwSubQuestIdx;
	Msg.dwFlag = pQuest->GetSubQuestFlag();
	Msg.dwData = pQuest->GetSubQuestData( dwSubQuestIdx );
	Msg.dwTime = pQuest->GetSubQuestTime( dwSubQuestIdx );
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

void CQuestManager::UpdateSubQuestData( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount )
{
	CQuest* pQuest = pPlayer->GetQuestGroup()->GetQuest( dwQuestIdx );
	if( pQuest == NULL )	return;

	// DB
	m_QuestUpdater->UpdateSubQuest( pPlayer, pQuest, dwSubQuestIdx );

	// network send
	SEND_SUBQUEST_UPDATE Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_SUBQUEST_UPDATE;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.wQuestIdx = (WORD)pQuest->GetQuestIdx();
	Msg.wSubQuestIdx = (WORD)dwSubQuestIdx;
	Msg.dwMaxCount = dwMaxCount;
	Msg.dwData = pQuest->GetSubQuestData( dwSubQuestIdx );
	Msg.dwTime = pQuest->GetSubQuestTime( dwSubQuestIdx );
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

void CQuestManager::GiveQuestItem( PLAYERTYPE* pPlayer, DWORD dwItemIdx, DWORD dwItemNum )
{
	// DB
	m_QuestUpdater->GiveQuestItem( pPlayer, dwItemIdx, dwItemNum );

	// network send
	SEND_QUESTITEM_IDX Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUESTITEM_GIVE;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.dwItemIdx = dwItemIdx;
	Msg.dwItemNum = dwItemNum;
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

void CQuestManager::TakeQuestItem( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwItemIdx, DWORD dwItemNum )
{
	// DB
	m_QuestUpdater->TakeQuestItem( pPlayer, dwQuestIdx, dwItemIdx, dwItemNum );

	// network send
	SEND_QUESTITEM_IDX Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUESTITEM_TAKE;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.dwItemIdx = dwItemIdx;
	Msg.dwItemNum = dwItemNum;
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

void CQuestManager::UpdateQuestItem( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwItemIdx, DWORD dwItemNum )
{
	// DB
	m_QuestUpdater->UpdateQuestItem( pPlayer, dwQuestIdx, dwItemIdx, dwItemNum );

	// network send
	SEND_QUESTITEM_IDX Msg;
	Msg.Category = MP_QUEST;
	Msg.Protocol = MP_QUESTITEM_UPDATE;
	Msg.dwObjectID = pPlayer->GetID();
	Msg.dwItemIdx = dwItemIdx;
	Msg.dwItemNum = dwItemNum;
	pPlayer->SendMsg(&Msg, sizeof(Msg));
}

void CQuestManager::UpdateQuestCheckTime( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwCheckType, DWORD dwCheckTime )
{
	// DB
	m_QuestUpdater->UpdateQuestCheckTime( pPlayer, dwQuestIdx, dwCheckType, dwCheckTime );
}

void CQuestManager::AddQuestEvent( PLAYERTYPE* pPlayer, CQuestEvent* pQuestEvent, DWORD dwQuestIdx )
{
	if( m_dwQuestEventCount < MAX_QUESTEVENT_MGR )
	{
		m_QuestEvent[m_dwQuestEventCount].pPlayer = pPlayer;
		m_QuestEvent[m_dwQuestEventCount].QuestEvent = *pQuestEvent;
		m_QuestEvent[m_dwQuestEventCount].dwQuestIdx = dwQuestIdx;
		++m_dwQuestEventCount;
	}
}

void CQuestManager::Process()
{
	if( m_dwQuestEventCount <= 0 ) return;

	CQuestGroup* pQuestGroup = NULL;
	for( DWORD i = 0; i < m_dwQuestEventCount; ++i )
	{
		if( m_QuestEvent[i].pPlayer )
		{
			pQuestGroup = m_QuestEvent[i].pPlayer->GetQuestGroup();
			if( pQuestGroup )
				pQuestGroup->AddQuestEvent( &m_QuestEvent[i].QuestEvent, m_QuestEvent[i].dwQuestIdx );		
		}
	}

	memset( m_QuestEvent, 0, sizeof(MQUESTEVENT)*MAX_QUESTEVENT_MGR );
	m_dwQuestEventCount = 0;
}

void CQuestManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_QUESTEVENT_NPCTALK:
		{			
			SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL )	return;
			else
			{
				CQuest * pQuest = pPlayer->GetQuestGroup()->GetQuest( pmsg->SubQuestIdx );
				if( pQuest )
				{
					// 첫 퀘스트이면서 퀘스트 개수가 제한개수 이상일때, 퀘스트 풀 메세지 클라이언트에 전송.
					if( pQuest->IsFirstStart() && ( pPlayer->GetQuestGroup()->GetProcessQuestCount() >= LIMIT_PROCESS_QUEST ) )
					{
						SEND_QUEST_IDX msg;
						msg.Category = MP_QUEST;
						msg.Protocol = MP_QUEST_FULL;
						msg.dwObjectID = pPlayer->GetID();

						pPlayer->SendMsg(&msg, sizeof(msg));
					}
					else
					{
						// quest event
						CQuestEvent qe;
						qe.SetValue( eQuestEvent_NpcTalk, pmsg->MainQuestIdx, pmsg->SubQuestIdx );
						AddQuestEvent( pPlayer, &qe );
					}
				}
			}
		}
		break;
	case MP_QUESTEVENT_NPCTALK_SYN:
		{
			SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL )	return;

			stTIME time;
			time.value = GetCurTime();
			char temp[256] = {0,};
			sprintf( temp, "%d-%d-%d %d:%d", time.GetYear()+2000, time.GetMonth(), time.GetDay(), 0, 0 );//time.GetHour(), time.GetMinute() );
			QuestEventCheck( pPlayer->GetUserID(), pmsg->dwObjectID, pmsg->MainQuestIdx, pmsg->SubQuestIdx, temp );			
		}
		break;
	case MP_QUEST_CHANGESTATE:
		{
			SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL )	return;

			m_QuestUpdater->UpdateQuest( pPlayer, pmsg->dwFlag, pmsg->dwData, pmsg->dwTime );
		}
		break;
	case MP_QUEST_DELETE_SYN:
		{
			SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL )	return;


			DeleteQuest( pPlayer, pmsg->MainQuestIdx );			
		}
		break;
	case MP_QUEST_DELETE_CONFIRM_SYN:
		{
			// RaMa 04.10.16
			SEND_QUEST_IDX_SIMPLE* pmsg = (SEND_QUEST_IDX_SIMPLE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL)	return;

			// Assert
			CQuestGroup* pQuestGroup = pPlayer->GetQuestGroup();
			if( pQuestGroup == NULL )
				return;

			CQuest* pQuest = pQuestGroup->GetQuest( pmsg->MainQuestIdx );
			if( pQuest == NULL )	
				return;			

			// 모든서브퀘스트가 끝났는지 확인. -RaMa 04.10.25
			for(DWORD i=0; i<pQuest->GetSubQuestCount(); i++)
			{
				if( pQuest->IsSubQuestComplete(i) == FALSE)
				{
					char buf[128];
					sprintf(buf, "MP_QUEST_DELETE_CONFIRM_SYN User:%d QuestIdx:%d NotComplete:%d", 
						pPlayer->GetID(), pmsg->MainQuestIdx, i);
					ASSERTMSG(0, buf);

					return;			
				}
			}

			SEND_QUEST_IDX_SIMPLE msg;
			msg.Category = MP_QUEST;
			msg.Protocol = MP_QUEST_DELETE_CONFIRM_ACK;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.MainQuestIdx = pmsg->MainQuestIdx;
			pPlayer->SendMsg(&msg, sizeof(msg));

///			char buf[128];
//			sprintf(buf, "MP_QUEST_DELETE_CONFIRM_SYN User:%d QuestIdx:%d", 
//				pPlayer->GetID(), pmsg->MainQuestIdx);
//			ASSERTMSG(0, buf);

			CQuestInfo* pQuestInfo = GetQuestInfo( pmsg->MainQuestIdx );
			if(pQuestInfo)
				EndQuest( pPlayer, pmsg->MainQuestIdx, pQuestInfo->GetEndParam() );
			else
				EndQuest( pPlayer, pmsg->MainQuestIdx, 0 );

			// 서버에 저장중인 퀘스트도 처리해준다. -RaMa 04.10.28
			pQuest->EndQuest( pmsg->MainQuestIdx, pQuestInfo->GetEndParam(), FALSE );
		}
	}	
}

