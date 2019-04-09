// PartyWarDialog.cpp: implementation of the CPartyWarDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyWarDialog.h"
#include "WindowIDEnum.h"
#include "./interface/cStatic.h"
#include "./interface/cButton.h"
#include "./interface/cCheckBox.h"
#include "./interface/cTextArea.h"
#include "./Interface/cScriptManager.h"
#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPartyWarDialog::CPartyWarDialog()
{
	m_type = WT_PARTYWAR_DLG;
	m_bPartyLock = m_bEnemyLock = FALSE;
}

CPartyWarDialog::~CPartyWarDialog()
{

}

void CPartyWarDialog::Render()
{
	cDialog::Render();
	
	if( !m_LockImage.IsNull() )
	{
		VECTOR2 vPos;
		VECTOR2 vScale;
		vScale.x = 4.15f;	vScale.y = 4.76f;	
		if( m_bPartyLock )
		{
			vPos.x = this->m_absPos.x + 20;	vPos.y = this->m_absPos.y + 120;			
			m_LockImage.RenderSprite( &vScale, NULL, 0.0f, &vPos,
									  RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );
		}
		if( m_bEnemyLock )
		{
			vPos.x = this->m_absPos.x + 205; vPos.y = this->m_absPos.y + 120;
			m_LockImage.RenderSprite( &vScale, NULL, 0.0f, &vPos,
									  RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );
		}
	}
}

void CPartyWarDialog::Linking()
{
	m_pcbName[0] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER0 );
	m_pcbName[1] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER1 );
	m_pcbName[2] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER2 );
	m_pcbName[3] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER3 );
	m_pcbName[4] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER4 );
	m_pcbName[5] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER5 );
	m_pcbName[6] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER6 );
	m_pcbName[7] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER7 );
	m_pcbName[8] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER8 );
	m_pcbName[9] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER9 );
	m_pcbName[10] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER10 );
	m_pcbName[11] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER11 );
	m_pcbName[12] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER12 );
	m_pcbName[13] = (cCheckBox*)GetWindowForID( PW_CB_MEMBER13 );

	m_psName[0] = (cStatic*)GetWindowForID( PW_MEMBER0 );
	m_psName[1] = (cStatic*)GetWindowForID( PW_MEMBER1 );
	m_psName[2] = (cStatic*)GetWindowForID( PW_MEMBER2 );
	m_psName[3] = (cStatic*)GetWindowForID( PW_MEMBER3 );
	m_psName[4] = (cStatic*)GetWindowForID( PW_MEMBER4 );
	m_psName[5] = (cStatic*)GetWindowForID( PW_MEMBER5 );
	m_psName[6] = (cStatic*)GetWindowForID( PW_MEMBER6 );
	m_psName[7] = (cStatic*)GetWindowForID( PW_MEMBER7 );
	m_psName[8] = (cStatic*)GetWindowForID( PW_MEMBER8 );
	m_psName[9] = (cStatic*)GetWindowForID( PW_MEMBER9 );
	m_psName[10] = (cStatic*)GetWindowForID( PW_MEMBER10 );
	m_psName[11] = (cStatic*)GetWindowForID( PW_MEMBER11 );
	m_psName[12] = (cStatic*)GetWindowForID( PW_MEMBER12 );
	m_psName[13] = (cStatic*)GetWindowForID( PW_MEMBER13 );

	m_pbtnLock = (cButton*)GetWindowForID( PW_BTN_LOCK );
	m_pbtnUnLock = (cButton*)GetWindowForID( PW_BTN_UNLOCK );
	m_pbtnStart = (cButton*)GetWindowForID( PW_BTN_START );
	m_pbtnCancel = (cButton*)GetWindowForID( PW_BTN_CANCEL );
	
	m_pTitle = (cTextArea*)GetWindowForID( PW_TITLE );
	m_pTeam1 = (cStatic*)GetWindowForID( PW_TEAM1 );
	m_pTeam2 = (cStatic*)GetWindowForID( PW_TEAM2 );
	m_pTime = (cStatic*)GetWindowForID( PW_TIME );

	SCRIPTMGR->GetImage( 65, &m_LockImage, PFT_HARDPATH );
	
	for( int i = 0; i < 14; ++i )
		m_psName[i]->SetAlign( TXT_LEFT );
}

void CPartyWarDialog::ShowPartyWarDlg( BOOL bMaster )
{
	cDialog::SetActive( TRUE );

	m_pbtnLock->SetActive( bMaster );
	m_pbtnUnLock->SetActive( bMaster );
	m_pbtnStart->SetActive( FALSE );
	m_pbtnCancel->SetActive( bMaster );

	for( int i = 0; i < 14; ++i )
	{
		m_pcbName[i]->SetActive( FALSE );		
		m_psName[i]->SetActive( FALSE );
	}
}

void CPartyWarDialog::HidePartyWarDlg()
{
	SetActive( FALSE );

	m_bPartyLock = m_bEnemyLock = FALSE;	
}

BOOL CPartyWarDialog::NoChangeCheckBoxState( int nIndex )
{
	if( m_pcbName[nIndex]->IsChecked() )
	{
		m_pcbName[nIndex]->SetChecked( FALSE );
		return TRUE;
	}
	else
	{
		m_pcbName[nIndex]->SetChecked( TRUE );
		return FALSE;
	}
}

void CPartyWarDialog::SetPartyMemberName( char* pName, int nIndex, BOOL bLogged )
{
	m_pcbName[nIndex]->SetActive( TRUE );
	m_pcbName[nIndex]->SetChecked( FALSE );

	m_psName[nIndex]->SetActive( TRUE );
	m_psName[nIndex]->SetStaticText( pName );	
	if( !bLogged )
		m_psName[nIndex]->SetFGColor( RGBA_MAKE(172,182,199,255) );

	char temp[256];
	if( nIndex == 0 )
	{
		m_pTeam1->SetStaticText( pName );
		sprintf( temp, CHATMGR->GetChatMsg( 867 ), m_psName[0]->GetStaticText(), m_psName[7]->GetStaticText() );
		m_pTitle->SetScriptText( temp );
	}	
	if( nIndex == 7 )
	{
		m_pTeam2->SetStaticText( pName );
		sprintf( temp, CHATMGR->GetChatMsg( 867 ), m_psName[0]->GetStaticText(), m_psName[7]->GetStaticText() );
		m_pTitle->SetScriptText( temp );
	}
}

void CPartyWarDialog::AddPartyWarMember( int nIndex )
{
	m_pcbName[nIndex]->SetChecked( TRUE );
}

void CPartyWarDialog::RemovePartyWarMember( int nIndex )
{
	m_pcbName[nIndex]->SetChecked( FALSE );
}

void CPartyWarDialog::SetLock( BOOL bParty, BOOL bMaster )
{
	m_pbtnStart->SetActive( FALSE );
	m_pbtnCancel->SetActive( FALSE );

	if( bParty )	m_bPartyLock = TRUE;
	else			m_bEnemyLock = TRUE;

	if( bMaster )
	{
		if( m_bPartyLock && m_bEnemyLock )
			m_pbtnStart->SetActive( TRUE );
		else
			m_pbtnCancel->SetActive( TRUE );
	}
}

void CPartyWarDialog::SetUnLock( BOOL bMaster )
{
	m_pbtnStart->SetActive( FALSE );
	m_pbtnCancel->SetActive( FALSE );

	m_bPartyLock = m_bEnemyLock = FALSE;

	if( bMaster )
		m_pbtnCancel->SetActive( TRUE );
}

void CPartyWarDialog::SetTime( DWORD dwTime )
{
	DWORD min = dwTime / 60;
	DWORD sec = dwTime % 60;
	char temp[256];
	sprintf( temp, CHATMGR->GetChatMsg( 868 ), min, sec );
	m_pTime->SetStaticText( temp );
}

void CPartyWarDialog::TestPartyWarDialog()
{
	cDialog::SetActive( TRUE );

	m_pbtnLock->SetActive( TRUE );
	m_pbtnUnLock->SetActive( TRUE );
	m_pbtnStart->SetActive( TRUE );
	m_pbtnCancel->SetActive( TRUE );

	for( int i = 0; i < 7; ++i )
	{
		m_pcbName[i]->SetActive( TRUE );
		m_psName[i]->SetActive( TRUE );
		m_psName[i]->SetStaticText( "aaaaaa" );
		m_pcbName[i+1]->SetActive( TRUE );
		m_psName[i+1]->SetActive( TRUE );
		m_psName[i+1]->SetStaticText( "ȣȣȣȣ" );
	}

	char temp[256];
	m_pTeam1->SetStaticText( "aaaaaa" );
	m_pTeam2->SetStaticText( "aaaaaa" );
	sprintf( temp, CHATMGR->GetChatMsg( 867 ), "aaaaaa", "aaaaaa" );
	m_pTitle->SetScriptText( temp );
	
	sprintf( temp, CHATMGR->GetChatMsg( 868 ), 1, 2 );
	m_pTime->SetStaticText( temp );
}
