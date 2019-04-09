// ChaseinputDialog.cpp: implementation of the CChaseinputDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChaseinputDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cEditBox.h"
#include "ObjectManager.h"
#include "../cIMEex.h"
#include "ChatManager.h"
#include "FilteringTable.h"
#include "WantedManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChaseinputDialog::CChaseinputDialog()
{
	m_type = WT_CHASEINPUT_DLG;
	m_LastChktime = 0;
}

CChaseinputDialog::~CChaseinputDialog()
{

}


void CChaseinputDialog::Linking()
{
	m_pEditName = (cEditBox*)GetWindowForID( CHASE_EDITBOX );
	m_pEditName->SetValidCheck(VCM_CHARNAME);
}


void CChaseinputDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( val )
	{
		m_pEditName->SetEditText("");
		m_dwItemIdx = 0;
	}
}


void CChaseinputDialog::WantedChaseSyn()
{
	if( (gCurTime - m_LastChktime) < 30000 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(909) );
		return;
	}

	char buf[MAX_NAME_LENGTH+1];
	char buftmp[MAX_NAME_LENGTH+1] = { 0, };
	strncpy( buf, m_pEditName->GetEditText(), MAX_NAME_LENGTH );
	strncpy( buftmp, buf, MAX_NAME_LENGTH );
	
	if( strlen( buf ) == 0 )		return;
	if( strcmp( buf, HERO->GetObjectName() ) == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(911) );
		return;
	}
	strupr( buftmp );
	if( FILTERTABLE->FilterWordInString( buftmp, eFilter_GM, eFM_INCLUDE ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(919) );
		return;
	}
	// 척살목록에 있는 사람만 추적가능
	if( m_dwItemIdx == eIncantation_Tracking_Jin )
	{
		if( WANTEDMGR->IsInList( buf ) == FALSE )
		{
			// ChatMsg 추가
			return;
		}
	}

	SEND_CHASEBASE msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_CHASE_SYN;
	SafeStrCpy( msg.WantedName, buf, MAX_NAME_LENGTH+1);
	msg.dwItemIdx = m_dwItemIdx;
	msg.dwObjectID = HERO->GetID();
	NETWORK->Send( &msg, sizeof(msg) );

	SetActive( FALSE );
	
	m_LastChktime = gCurTime;
}