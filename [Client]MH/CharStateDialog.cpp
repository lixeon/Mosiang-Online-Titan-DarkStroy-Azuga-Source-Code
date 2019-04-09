#include "stdafx.h"
#include "CharStateDialog.h"
#include "./Interface/cPushupButton.h"
#include "WindowIDEnum.h"
#include "MacroManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cResourceManager.h"
#include "GameIn.h"
#include "PKManager.h"


#include "MacroDialog.h"


CCharStateDialog::CCharStateDialog()
{
	m_pBtnPK		= NULL;
	m_pBtnMove		= NULL;
	m_pBtnKyungGong	= NULL;
	m_pBtnPeaceWar	= NULL;
	m_pBtnUngi		= NULL;
}


CCharStateDialog::~CCharStateDialog()
{
}

void CCharStateDialog::Linking()
{
	m_pBtnPK		= (cPushupButton*)GetWindowForID( CSS_BTN_PK );
	m_pBtnMove		= (cPushupButton*)GetWindowForID( CSS_BTN_MOVE );
	m_pBtnKyungGong	= (cPushupButton*)GetWindowForID( CSS_BTN_KYUNGGONG );
	m_pBtnPeaceWar	= (cPushupButton*)GetWindowForID( CSS_BTN_PEACEWAR );
	m_pBtnUngi		= (cPushupButton*)GetWindowForID( CSS_BTN_UNGI );

	m_pBtnPK->SetPassive( TRUE );
	m_pBtnMove->SetPassive( TRUE );
	m_pBtnKyungGong->SetPassive( TRUE );
	m_pBtnPeaceWar->SetPassive( TRUE );
	m_pBtnUngi->SetPassive( TRUE );
}


void CCharStateDialog::Refresh()
{
	cImage ToolTipImg;
	SCRIPTMGR->GetImage( 63, &ToolTipImg, PFT_HARDPATH );	

	char imagePath[128];
	char strMacro[32];
	sMACRO* pMacro;

	strcpy( imagePath, RESRCMGR->GetMsg(358) );
	pMacro = MACROMGR->GetCurMacroKey( ME_TOGGLE_MOVEMODE );
	GAMEIN->GetMacroDialog()->ConvertMacroToText( strMacro, pMacro );
	wsprintf( imagePath, "%s(%s)", imagePath, strMacro );
	m_pBtnMove->SetToolTip( imagePath, RGB_HALF( 255, 255, 255 ), &ToolTipImg );

//	strcpy( imagePath, RESRCMGR->GetMsg(359) );
//	pMacro = MACROMGR->GetCurMacroKey( ME_TOGGLE_KYUNGGONG );
//	GAMEIN->GetMacroDialog()->ConvertMacroToText( strMacro, pMacro );
//	wsprintf( imagePath, "%s(%s)", imagePath, strMacro );
//	m_pBtnKyungGong->SetToolTip( imagePath, RGB_HALF( 255, 255, 255 ), &ToolTipImg );

	strcpy( imagePath, RESRCMGR->GetMsg(360) );
	pMacro = MACROMGR->GetCurMacroKey( ME_TOGGLE_PEACEWARMODE );
	GAMEIN->GetMacroDialog()->ConvertMacroToText( strMacro, pMacro );
	wsprintf( imagePath, "%s(%s)", imagePath, strMacro );
	m_pBtnPeaceWar->SetToolTip( imagePath, RGB_HALF( 255, 255, 255 ), &ToolTipImg );

//	strcpy( imagePath, RESRCMGR->GetMsg(361) );
//	pMacro = MACROMGR->GetCurMacroKey( ME_TOGGLE_UNGIMODE );
//	GAMEIN->GetMacroDialog()->ConvertMacroToText( strMacro, pMacro );
//	wsprintf( imagePath, "%s(%s)", imagePath, strMacro );
//	m_pBtnUngi->SetToolTip( imagePath, RGB_HALF( 255, 255, 255 ), &ToolTipImg );	
}


void CCharStateDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{

	if( ( we & WE_PUSHUP ) || ( we & WE_PUSHDOWN ) )
	{
		switch( lId )
		{
		case CSS_BTN_MOVE:
			{
				MACROMGR->PlayMacro( ME_TOGGLE_MOVEMODE );
			}
			break;
		case CSS_BTN_KYUNGGONG:
			{
//				MACROMGR->PlayMacro( ME_TOGGLE_KYUNGGONG );
			}
			break;
		case CSS_BTN_PEACEWAR:
			{
				MACROMGR->PlayMacro( ME_TOGGLE_PEACEWARMODE );
			}
			break;
		case CSS_BTN_UNGI:
			{
//				MACROMGR->PlayMacro( ME_TOGGLE_UNGIMODE );
			}
			break;
		case CSS_BTN_PK:
			{
				PKMGR->ToggleHeroPKMode();
			}
			break;
		}
	}
}

void CCharStateDialog::SetPKMode( BOOL bPKMode )
{
	m_pBtnPK->SetPush( bPKMode );
}

void CCharStateDialog::SetMoveMode( BOOL bRun )
{
	m_pBtnMove->SetPush( bRun );
}

void CCharStateDialog::SetKyungGongMode( BOOL bKyungGong )
{
	m_pBtnKyungGong->SetPush( bKyungGong );
}

void CCharStateDialog::SetPeaceWarMode( BOOL bPeace )
{
	m_pBtnPeaceWar->SetPush( !bPeace );
}

void CCharStateDialog::SetUngiMode( BOOL bUngi )
{
	m_pBtnUngi->SetPush( bUngi );
}

