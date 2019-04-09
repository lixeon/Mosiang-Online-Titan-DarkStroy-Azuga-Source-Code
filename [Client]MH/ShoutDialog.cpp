// ShoutDialog.cpp: implementation of the CShoutDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShoutDialog.h"
#include "./Interface/cEditBox.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "WindowIDEnum.h"
#include "FilteringTable.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShoutDialog::CShoutDialog()
{
	m_type = WT_SHOUT_DLG;
	m_dwItemIdx = m_dwItemPos = 0;
}

CShoutDialog::~CShoutDialog()
{
	
}


void CShoutDialog::Linking()
{
	m_pMsgBox = (cEditBox*)GetWindowForID( CHA_MSG );
}


BOOL CShoutDialog::SendShoutMsgSyn()
{
	char buf[MAX_SHOUT_LENGTH+1] = { 0, };
	strcpy( buf, m_pMsgBox->GetEditText() );
	if( strlen( buf ) == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(903) );
		return FALSE;
	}
	m_pMsgBox->SetEditText("");

	if( FILTERTABLE->FilterChat( buf ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(27) );
		return FALSE;
	}
	
	SEND_SHOUTBASE_ITEMINFO msg;	
	sprintf( msg.Msg, "%s : %s", HERO->GetObjectName(), buf );	
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_SHOUT_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.ItemIdx = (WORD)m_dwItemIdx;
	msg.ItemPos = (WORD)m_dwItemPos;
	
	NETWORK->Send( &msg, sizeof(msg) );

	SetActive( FALSE );
	m_dwItemIdx = m_dwItemPos = 0;
	
	return TRUE;
}
