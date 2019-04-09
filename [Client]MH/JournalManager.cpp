// JournalManager.cpp: implementation of the CJournalManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JournalManager.h"
#include "mhnetwork.h"
#include "ObjectManager.h"
#include "JournalDialog.h"
#include "Gamein.h"
#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJournalManager::CJournalManager()
{

}

CJournalManager::~CJournalManager()
{

}

void CJournalManager::Init()
{
	m_bListReceived = FALSE;
	m_RefreshTime = 0;
}

void CJournalManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_JOURNAL_GETLIST_ACK:
		{
			m_bListReceived = TRUE;
			SEND_JOURNALLIST* pmsg = (SEND_JOURNALLIST*)pMsg;
			
			// 기존 목록 삭제
			GAMEIN->GetJournalDialog()->JournalReset();

			// 새로 받아온 목록을 추가
			for(int i=0; i<pmsg->wCount; i++)
				GAMEIN->GetJournalDialog()->JournalItemAdd(&pmsg->m_Info[i]);
			
			m_bListReceived = FALSE;
		}
		break;
	case MP_JOURNAL_ADD:
		{
			SEND_JOURNALINFO* pmsg = (SEND_JOURNALINFO*)pMsg;

			GAMEIN->GetJournalDialog()->JournalItemAdd(&pmsg->m_Info);
		}
		break;		
	default:
		ASSERT(0);
	}
}

void CJournalManager::GetListSyn()
{
	if((gCurTime-m_RefreshTime) < 500)
		return;

	if(!m_bListReceived)
	{
		MSGBASE msg;
		msg.Category = MP_JOURNAL;
		msg.Protocol = MP_JOURNAL_GETLIST_SYN;
		msg.dwObjectID = HEROID;
		
		NETWORK->Send(&msg, sizeof(msg));

		m_RefreshTime = gCurTime;		
	}
}