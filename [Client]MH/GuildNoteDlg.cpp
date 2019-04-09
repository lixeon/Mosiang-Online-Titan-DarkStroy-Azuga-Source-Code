#include "stdafx.h"
#include "GuildNoteDlg.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "Hero.h"
#include "Item.h"
#include "./Interface\cTextArea.h"
#include "ChatManager.h"
#include "ItemManager.h"

CGuildNoteDlg::CGuildNoteDlg(void)
{
	m_bUse = FALSE;
}

CGuildNoteDlg::~CGuildNoteDlg(void)
{
}

void CGuildNoteDlg::Linking()
{
	m_pNoteText = (cTextArea*)GetWindowForID(GN_TEXTREA);

	m_pNoteText->SetEnterAllow(FALSE);
	m_pNoteText->SetScriptText("");
}

void CGuildNoteDlg::Show(CItem* pItem)
{
	if( !HERO->GetGuildIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(35) );
		return;
	}
    if(pItem == NULL)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );
		//786 "아이템 사용에 실패하였습니다."
		return;
	}
	if(m_bUse)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(752) );
		//752 "이미 사용중인 아이템 입니다."
		return;
	}

	m_pItem = pItem;

	SetActive(TRUE);
}

void CGuildNoteDlg::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch(lId)
		{
		case GN_SENDOKBTN:
			{
				MSG_GUILD_SEND_NOTE msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_NOTE_SYN;
				msg.FromId = HERO->GetGuildIdx();
				msg.dwObjectID = HEROID;

				SafeStrCpy(msg.FromName, HERO->GetObjectName(), MAX_NAME_LENGTH+1);
				m_pNoteText->GetScriptText(msg.Note);

				NETWORK->Send(&msg, msg.GetMsgLength());

				SetActive(FALSE);
			}
		case GN_CANCELBTN:
			{
				SetActive(FALSE);
			}
		}
	}
}

void CGuildNoteDlg::Use()
{
	m_bUse = FALSE;
	m_pNoteText->SetScriptText("");

	MSG_ITEM_USE_SYN msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_SYN;
	msg.dwObjectID = HEROID;
	msg.wItemIdx = m_pItem->GetItemIdx();
	msg.TargetPos = m_pItem->GetPosition();

	NETWORK->Send(&msg,sizeof(msg));

	// debug용
	ITEMMGR->m_nItemUseCount++;
}

