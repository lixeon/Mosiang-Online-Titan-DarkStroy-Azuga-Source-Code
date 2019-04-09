// ShoutchatDialog.cpp: implementation of the CShoutchatDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShoutchatDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cListDialog.h"
#include "GameIn.h"
#include "ChatDialog.h"
#include "GameResourceManager.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShoutchatDialog::CShoutchatDialog()
{
	m_type = WT_SHOUTCHAT_DLG;
	m_LastMsgTime = 0;
}

CShoutchatDialog::~CShoutchatDialog()
{

}


void CShoutchatDialog::Linking()
{
	m_pMsgListDlg = (cListDialog*)GetWindowForID( CHA_LIST );

	//SW061111 저해상도옵션
	if( GAMERESRCMNGR->IsLowResolution() )
	{
		GAMEIN->GetShoutchatDlg()->RefreshPosition();
	}
}


void CShoutchatDialog::Process()
{
	if( gCurTime - m_LastMsgTime < 5000 )
		return;
	
	m_LastMsgTime = gCurTime;
}

void CShoutchatDialog::SetActive( BOOL val )
{
	if( val )
		RefreshPosition();

	cDialog::SetActive( val );
}

void CShoutchatDialog::AddMsg( char* pstr )
{
	char buf[61] = { 0, };
	strncpy( buf, pstr, 60 );
	
	if( m_pMsgListDlg )
		m_pMsgListDlg->AddItem( buf, RGBA_MAKE(217, 206, 247, 255) );
}


void CShoutchatDialog::RefreshPosition()
{
	float absX = GAMEIN->GetChatDialog()->GetAbsX();
	float absY = GAMEIN->GetChatDialog()->GetSheetPosY();

	SetAbsXY( (LONG)absX, (LONG)absY-GetHeight() );
}
