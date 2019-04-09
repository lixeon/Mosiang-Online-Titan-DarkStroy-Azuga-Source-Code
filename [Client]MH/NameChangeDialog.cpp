// NameChangeDialog.cpp: implementation of the CNameChangeDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NameChangeDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cEditBox.h"
#include "ObjectManager.h"
#include "../cIMEex.h"
#include "FilteringTable.h"
#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNameChangeDialog::CNameChangeDialog()
{
	m_type = WT_NAMECHANGE_DLG;
	m_dwDBIdx = 0;
}

CNameChangeDialog::~CNameChangeDialog()
{

}

void CNameChangeDialog::Linking()
{
	m_pNameBox = (cEditBox*)GetWindowForID( CH_NAME_CHANGE_EDITBOX );
	m_pNameBox->SetValidCheck(VCM_CHARNAME);
}


void CNameChangeDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( val )
		m_pNameBox->SetEditText("");
}


void CNameChangeDialog::NameChangeSyn()
{
	DWORD len = 0;
	char buf[20];
	strcpy( buf, m_pNameBox->GetEditText() );

	len = strlen(buf);
	if( len == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(11) );
		return;		
	}
	else if( len < 4 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(19) );
		return;
	}
	else if( len > MAX_NAME_LENGTH )
		return;
	
	if( strcmp(buf, HERO->GetObjectName()) == 0 )
		return;
	if( FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf) )
//	if( FILTERTABLE->IsInvalidCharacterName((unsigned char*)buf) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(14) );		
		return;
	}
	if( !FILTERTABLE->IsUsableName(buf) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(14) );		
		return;
	}
	if( m_dwDBIdx == 0 )		return;	


	
	SEND_CHANGENAMEBASE msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_NCHANGE_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.DBIdx = m_dwDBIdx;
	strncpy( msg.Name, buf, MAX_NAME_LENGTH+1 );
	NETWORK->Send( &msg, sizeof(msg) );

	SetActive( FALSE );
}
