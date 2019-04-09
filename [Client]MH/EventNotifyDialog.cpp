// EventNotifyDialog.cpp: implementation of the CEventNotifyDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventNotifyDialog.h"

#include "WindowIDEnum.h"
#include "./Interface/cStatic.h"
#include "./Interface/cTextArea.h"
#include "./Input/Mouse.h"
#include "./Interface/cScriptManager.h"
#include "ChatManager.h"

#include "GMNotifyManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEventNotifyDialog::CEventNotifyDialog()
{
	m_pStcTitle		= NULL;
	m_pTAContext	= NULL;
}

CEventNotifyDialog::~CEventNotifyDialog()
{

}

void CEventNotifyDialog::Linking()
{
	m_pStcTitle = (cStatic*)GetWindowForID( GMOT_STC_TITLE );
	m_pTAContext = (cTextArea*)GetWindowForID( GMOT_TA_CONTEXT );

	cImage ToolTipImg;
	SCRIPTMGR->GetImage( 63, &ToolTipImg, PFT_HARDPATH );	
	// m_pTAContext->SetToolTip( CHATMGR->GetChatMsg(782), RGB_HALF( 255, 255, 255 ), &ToolTipImg ); // 컨텍스트 도움말 제거
}

void CEventNotifyDialog::SetActive( BOOL val )
{
//	NOTIFYMGR->SetNotifyActive( val );
	cDialog::SetActive( val );

	if( m_pTAContext && !val )
		m_pTAContext->SetScriptText("");
}

DWORD CEventNotifyDialog::ActionEvent( CMouse* mouseInfo )
{
	DWORD we = cDialog::ActionEvent( mouseInfo );

	//if( m_pTAContext->PtInWindow( mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY() ) )
	//if( mouseInfo->LButtonDown() )
	//{
	//	SetActive( FALSE );
	//}

	return we;
}


void CEventNotifyDialog::SetTitle( char* pStrTitle )
{
	if( m_pStcTitle )
		m_pStcTitle->SetStaticText( pStrTitle );
}

void CEventNotifyDialog::SetContext( char* pStrContext )
{
	if( m_pTAContext )
		m_pTAContext->SetScriptText( pStrContext );
}
