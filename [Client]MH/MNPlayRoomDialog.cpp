// MNPlayRoomDialog.cpp: implementation of the CMNPlayRoomDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MNPlayRoomDialog.h"
#include "WindowIdEnum.h"
#include "./Interface/cListDialog.h"
#include "./Interface/cButton.h"
#include "./Interface/cStatic.h"
#include "./Interface/cEditBox.h"

#include "MurimNet.h"
#include "MNPlayerManager.h"
#include "MNPlayer.h"
#include "GlobalEventFunc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMNPlayRoomDialog::CMNPlayRoomDialog()
{
	for( int i = 0 ; i < eTEAM_MAX ; ++i )
		m_pPlayerListDlg[i]	= NULL;

	m_pBtnStart			= NULL;
	m_pEdtChat			= NULL;
	m_pLstChat			= NULL;
}

CMNPlayRoomDialog::~CMNPlayRoomDialog()
{

}

void CMNPlayRoomDialog::Linking()
{
	m_pPlayerListDlg[eTEAM_LEFT]		= (cListDialog*)GetWindowForID( MNPRI_LSTDLG_TEAMA );
	m_pPlayerListDlg[eTEAM_RIGHT]		= (cListDialog*)GetWindowForID( MNPRI_LSTDLG_TEAMB );
	m_pPlayerListDlg[eTEAM_OBSERVER]	= (cListDialog*)GetWindowForID( MNPRI_LSTDLG_OB );

	m_pBtnStart	= (cButton*)GetWindowForID( MNPRI_BTN_START );
	
	m_pEdtChat = (cEditBox*)GetWindowForID( MNPRI_EB_CHAT );
	m_pEdtChat->SetEditFunc( MNPRI_ChatFunc );

	m_pLstChat = (cListDialog*)GetWindowForID( MNPRI_LSTDLG_CHAT );

//	MNPRI_BTN_EXIT
//	MNPRI_BTN_BAN
//	MNPRI_BTN_INVITE
}

DWORD CMNPlayRoomDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = cDialog::ActionEvent( mouseInfo );

	if( m_pEdtChat )
	if( !m_pEdtChat->IsFocus() )
		m_pEdtChat->SetFocusEdit( TRUE );

	return we;
}

void CMNPlayRoomDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we == WE_BTNCLICK )
	{
		if( lId == MNPRI_BTN_MOVETOA )
		{
			SendMsgTeamChange( 0 );
			//답이 올때까지 멈추기
		}
		else if( lId == MNPRI_BTN_MOVETOB )
		{
			SendMsgTeamChange( 1 );
		}
		else if( lId == MNPRI_BTN_MOVETOOB )
		{
			//SendMsgTeamChange( 2 );
		}
		else if( lId == MNPRI_BTN_EXIT )
		{
			SendMsgExit();
		}
		else if( lId == MNPRI_BTN_START )
		{
			SendMsgStart();
		}
	}	
}

void CMNPlayRoomDialog::SetPlayRoomInfo( PLAYROOM_BASEINFO* pPlayRoomInfo )
{
	cStatic* pTitle = (cStatic*)GetWindowForID( MNPRI_STC_TITLE );
	pTitle->SetStaticText( pPlayRoomInfo->strPlayRoomTitle );
}

void CMNPlayRoomDialog::AddPlayer( MNPLAYER_BASEINFO* pPlayerInfo )
{
	m_pPlayerListDlg[pPlayerInfo->cbTeam]->AddItem( pPlayerInfo->strPlayerName, 0xffffffff );
}

void CMNPlayRoomDialog::RemovePlayer( MNPLAYER_BASEINFO* pPlayerInfo )
{
	m_pPlayerListDlg[pPlayerInfo->cbTeam]->RemoveItem( pPlayerInfo->strPlayerName );
}

void CMNPlayRoomDialog::RemoveAllPlayer()
{
	for( int i = 0 ; i < eTEAM_MAX ; ++i )
		m_pPlayerListDlg[i]->RemoveAll();
}

void CMNPlayRoomDialog::SendMsgTeamChange( BYTE cbTeam )
{
	MSG_BYTE msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_PR_TEAMCHANGE_SYN;
	msg.dwObjectID	= MURIMNET->GetMNPlayerManager()->GetMNHeroID();
	msg.bData		= cbTeam;

	NETWORK->Send( &msg, sizeof( msg ) );
}

//server로부터 메세지 받고 실제 팀을 변경
void CMNPlayRoomDialog::TeamChange( char* strName, BYTE cbFromTeam, BYTE cbToTeam )
{
	m_pPlayerListDlg[cbFromTeam]->RemoveItem( strName );
	m_pPlayerListDlg[cbToTeam]->AddItem( strName, 0xffffffff );
}

void CMNPlayRoomDialog::SendMsgExit()
{
	MSGBASE msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_DISCONNECT_SYN;
	msg.dwObjectID	= MURIMNET->GetMNPlayerManager()->GetMNHeroID();
	
	NETWORK->Send( &msg, sizeof(msg) );
}

void CMNPlayRoomDialog::SendMsgStart()
{
	MSGBASE msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_PR_START_SYN;
	msg.dwObjectID	= MURIMNET->GetMNPlayerManager()->GetMNHeroID();

	NETWORK->Send( &msg, sizeof(msg) );
}

void CMNPlayRoomDialog::SetCaptain( BOOL bCaptain )
{
	if( bCaptain )
	{
		m_pBtnStart->SetActive( TRUE );
	}
	else
	{
		m_pBtnStart->SetActive( FALSE );
	}
}

void CMNPlayRoomDialog::ChatMsg( int nClass, char* strName, char* strMsg )
{
	static char msg[256] = {0,};
	switch( nClass )
	{
	case PRCTC_WHOLE:
		{
			wsprintf( msg, "[%s]: %s", strName, strMsg );
			m_pLstChat->AddItem( msg, 0xffffffff );
		}
		break;
	}
}

void CMNPlayRoomDialog::PrintMsg( int nClass, char* strMsg )
{
	m_pLstChat->AddItem( strMsg, 0xffffffff );
}