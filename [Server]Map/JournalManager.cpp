// JournalManager.cpp: implementation of the CJournalManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JournalManager.h"
#include "Player.h"
#include "UserTable.h"
#include "MapDBMsgParser.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJournalManager::CJournalManager()
{

}

CJournalManager::~CJournalManager()
{

}

void CJournalManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_JOURNAL_GETLIST_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;
			JournalGetList(pmsg->dwObjectID);
		}
		break;
	case MP_JOURNAL_UPDATE:
		{
			SEND_JOURNAL_DWORD* pmsg = (SEND_JOURNAL_DWORD*)pMsg;

			JournalSaveUpdate(pmsg->dwObjectID, pmsg->Param);			
		}
		break;
	case MP_JOURNAL_DELETE:
		{
			SEND_JOURNAL_DWORD* pmsg = (SEND_JOURNAL_DWORD*)pMsg;

			JournalDelete(pmsg->dwObjectID, pmsg->Param);
		}
		break;
	default:
		ASSERT(0);
	}
}
