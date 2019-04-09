// NoteManager.cpp: implementation of the CNoteManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NoteManager.h"
#include "mhnetwork.h"
#include "MiniNoteDialog.h"
#include "NoteDialog.h"
#include "GameIn.h"
#include "MainBarDialog.h"
#include "ObjectManager.h"
#include "ChatManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CNoteManager)
CNoteManager::CNoteManager()
{

}

CNoteManager::~CNoteManager()
{

}

void CNoteManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
		case MP_NOTE_RECEIVENOTE:
		{
			MSGBASE * pmsg = (MSGBASE *) pMsg;
			if(GAMEIN->GetNoteDialog()->IsActive() == TRUE)
				NoteListSyn(1);
			else
				GAMEIN->GetMainInterfaceDialog()->SetAlram( OPT_NOTEDLGICON, TRUE );
			
			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(105));						
		}
		break;
	case MP_NOTE_NEW_NOTE:
		{
			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(95));
			if(GAMEIN->GetNoteDialog()->IsActive() == TRUE)
				NoteListSyn(1);
			else
				GAMEIN->GetMainInterfaceDialog()->SetAlram( OPT_NOTEDLGICON, TRUE );
		}
		break;
	case MP_NOTE_SENDNOTE_ACK:
		{
			MSG_NAME *pmsg = (MSG_NAME*) pMsg;
			GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);
			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(93), pmsg->Name );
		}
		break;
	case MP_NOTE_NOTELIST_ACK:
		{
			MSG_FRIEND_NOTE_LIST* pmsg = (MSG_FRIEND_NOTE_LIST*) pMsg;
			GAMEIN->GetNoteDialog()->SetNoteList(pmsg);
		}
		break;
	case MP_NOTE_DELNOTE_ACK:
		{
			MSG_FRIEND_DEL_NOTE* pmsg = (MSG_FRIEND_DEL_NOTE*)pMsg;
			if(pmsg->bLast)
			{
				NoteListSyn( 1, GAMEIN->GetNoteDialog()->GetMode() );
			}
			GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);
		}
		break;
	case MP_NOTE_READNOTE_ACK:
		{
			MSG_FRIEND_READ_NOTE * pmsg = (MSG_FRIEND_READ_NOTE*)pMsg;
			GAMEIN->GetMiniNoteDialog()->SetMiniNote(pmsg->FromName, pmsg->Note, pmsg->ItemIdx);
			GAMEIN->GetMiniNoteDialog()->SetNoteID(pmsg->NoteID);
			GAMEIN->GetMiniNoteDialog()->SetActive(TRUE);
			GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Read);
		}
		break;
		
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//nack 	
	case MP_NOTE_SENDNOTE_NACK:
		{
			MSG_NAME_WORD * pmsg = (MSG_NAME_WORD *)pMsg;
			if(pmsg->wData == 2) //invalid user
				CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(94) );
			else if(pmsg->wData == 3) //full list
				CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(129), pmsg->Name );
		}
		break;
	}

}

void CNoteManager::WriteNote(char* Note, char* ToName)
{
	MSG_FRIEND_SEND_NOTE msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_SENDNOTE_SYN;
	msg.FromId = HEROID;
	SafeStrCpy(msg.ToName, ToName, MAX_NAME_LENGTH+1);
	SafeStrCpy(msg.FromName, HERO->GetObjectName(), MAX_NAME_LENGTH+1);
	SafeStrCpy(msg.Note, Note, MAX_NOTE_LENGTH+1);	
	
	
	NETWORK->Send(&msg, msg.GetMsgLength());
}

void CNoteManager::WriteNoteID(char* Note, DWORD TargetID)
{
	MSG_FRIEND_SEND_NOTE_ID msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_SENDNOTEID_SYN;
	msg.dwObjectID = HEROID;
	SafeStrCpy(msg.FromName, HERO->GetObjectName(), MAX_NAME_LENGTH+1);
	msg.TargetID = TargetID;
	SafeStrCpy(msg.Note, Note, MAX_NOTE_LENGTH+1);

	NETWORK->Send(&msg, msg.GetMsgLength());
}

void CNoteManager::DelNoteSyn(DWORD NoteID, BOOL bLast)
{
	MSG_FRIEND_DEL_NOTE msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_DELNOTE_SYN;
	msg.NoteID = NoteID;
	msg.dwObjectID = HEROID;
	msg.bLast = bLast;

	NETWORK->Send(&msg, sizeof(msg));
}

void CNoteManager::DelAllNoteSyn()
{
	MSGBASE msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_DELALLNOTE_SYN;
	msg.dwObjectID = HEROID;
	NETWORK->Send(&msg, sizeof(msg));
}

void CNoteManager::NoteListSyn(WORD SelectedPage, WORD Mode)
{
	if(HERO->IsInited() == FALSE)
		return;
	if(HEROID > 10000000)
	{
		char buf[256];
		sprintf(buf, "HeroId: %d, HeroName: %s, HeroSim: %d", HEROID, HERO->GetObjectName(), HERO->GetSimMek());
		ASSERTMSG(0, buf);
	}
	MSG_WORD2 msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_NOTELIST_SYN;
	msg.wData1 = SelectedPage;
	msg.wData2 = Mode;
	msg.dwObjectID = HEROID;

	NETWORK->Send(&msg, sizeof(msg));

	GAMEIN->GetNoteDialog()->SetSelectedNotePge(SelectedPage);
}

void CNoteManager::ReadNote(DWORD NoteID, BOOL IsFront)
{
	MSG_DWORD2 msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_READNOTE_SYN;
	msg.dwData1 = NoteID;
	msg.dwData2 = IsFront;
	msg.dwObjectID = HEROID;

	NETWORK->Send(&msg, sizeof(msg));
}