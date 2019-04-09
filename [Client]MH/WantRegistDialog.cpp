// WantRegistDialog.cpp: implementation of the CWantRegistDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WantRegistDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cStatic.h"
#include "./Interface/cEditBox.h"
#include "cImeEx.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWantRegistDialog::CWantRegistDialog()
{
	m_type = WT_WANTREGISTDIALOG;
}

CWantRegistDialog::~CWantRegistDialog()
{

}

void CWantRegistDialog::Linking()
{
	m_WantedName = (cStatic*)GetWindowForID(WANTREG_WANTEDNAME);
	m_PrizeEdit =  (cEditBox*)GetWindowForID(WANTREG_PRIZEEDIT);
	m_PrizeEdit->SetValidCheck(VCM_NUMBER);

	m_bShow = FALSE;
	m_dwStartShowTime = 0;
}

void CWantRegistDialog::SetWantedName(char* pName)
{	
//	char Name[32];
//	sprintf(Name, CHATMGR->GetChatMsg(546), pName);
	m_WantedName->SetStaticText(pName);
	m_PrizeEdit->SetEditText("");
}

void CWantRegistDialog::SetActive( BOOL val )
{
	if( val == m_bActive )
		return;
	if( val == TRUE )
	{
		m_dwStartShowTime = gCurTime;
	}
	else
	{
		m_PrizeEdit->SetFocusEdit(FALSE);
		MSGBASE msg;
		msg.Category = MP_WANTED;
		msg.Protocol = MP_WANTED_REGIST_CANCEL;
		msg.dwObjectID = HEROID;
		NETWORK->Send( &msg, sizeof(msg) );
	}

	m_bShow = FALSE;

	cDialog::SetActive( val );
}

DWORD CWantRegistDialog::ActionEvent( CMouse * mouseInfo )
{
	DWORD we = WE_NULL;
	if( m_bDisable || !m_bActive ) return we;

	if( !m_bShow )
	{
		if( gCurTime - m_dwStartShowTime >= 3000 )	//3√ 
		{
			m_bShow = TRUE;
		}
		else
		{
			return we;
		}
	}
	
	we = cDialog::ActionEvent( mouseInfo );
	
	return we;
}


void CWantRegistDialog::Render()
{
	if( m_bShow )
	{
		cDialog::Render();
	}
}