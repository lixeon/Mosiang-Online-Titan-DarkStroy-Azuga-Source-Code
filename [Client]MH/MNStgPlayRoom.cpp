// MNStgPlayRoom.cpp: implementation of the CMNStgPlayRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MNStgPlayRoom.h"
#include "MainGame.h"
#include "./Interface/cWindowManager.h"
#include "MNPlayRoomDialog.h"
#include "MurimNet.h"
#include "MNPlayerManager.h"
#include "MNPlayer.h"

#include "./Input/UserInput.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMNPlayRoomDialog* CMNStgPlayRoom::m_pInterface = NULL;

CMNStgPlayRoom::CMNStgPlayRoom()
{

}

CMNStgPlayRoom::~CMNStgPlayRoom()
{

}

void CMNStgPlayRoom::Init()
{
//---Create Window Interface

#ifdef _FILE_BIN_
	m_pInterface = (CMNPlayRoomDialog*)WINDOWMGR->GetDlgInfoFromFile( "./Image/MurimNet/PlayRoom.bin", "rb" );
#else
	m_pInterface = (CMNPlayRoomDialog*)WINDOWMGR->GetDlgInfoFromFile( "./Image/MurimNet/PlayRoom.txt" );
#endif

	WINDOWMGR->AddWindow( m_pInterface );

	m_pInterface->Linking();
}

void CMNStgPlayRoom::Release()
{
//---destroy window interfaces
	WINDOWMGR->AddListDestroyWindow( m_pInterface );
	m_pInterface = NULL;
}

void CMNStgPlayRoom::Open()
{
	if( m_pInterface )
		m_pInterface->SetActive( TRUE );

	m_dwStartTime	= 0;
	m_bGameStart	= FALSE;
	m_dwMapNum		= 0;

}

void CMNStgPlayRoom::Close()
{
	if( m_pInterface )
		m_pInterface->SetActive( FALSE );
}

void CMNStgPlayRoom::Process()
{
	if( m_bGameStart )
	{
		DWORD dwElapsedSecond = ( gCurTime - m_dwStartTime ) / 1000;
		
		if( m_dwCountDown != dwElapsedSecond )
		{
			if( m_dwCountDown == 6 )
			{
				MAINGAME->SetGameState( eGAMESTATE_MAPCHANGE, &m_dwMapNum, 4 );
				m_bGameStart = FALSE;
			}
			else
			{
				//display count;
				char str[32];
				wsprintf( str, "%d......", 5 - m_dwCountDown );
				m_pInterface->PrintMsg( PRCTC_SYSTEM, str );
				//add sound
				++m_dwCountDown;
			}
		}
	}
}

void CMNStgPlayRoom::Render()
{
//---render window interfaces
	WINDOWMGR->Render();
}

void CMNStgPlayRoom::NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
	switch(Category)
	{
	case MP_MURIMNET:
		{
			switch(Protocol)
			{
			case MP_MURIMNET_PR_PLAYROOMINFO:
				{
					MSG_PLAYROOM_BASEINFO* pMsgPlayRoomInfo = (MSG_PLAYROOM_BASEINFO*)pMsg;
					m_dwPlayRoomIndex = pMsgPlayRoomInfo->Info.dwPlayRoomIndex;
					m_pInterface->SetPlayRoomInfo( &pMsgPlayRoomInfo->Info );
					//로딩할 맵넘도..?
				}
				break;

			case MP_MURIMNET_PR_PLAYERIN:
				{
					MSG_MNPLAYER_BASEINFO* pMsgPlayerInfo = (MSG_MNPLAYER_BASEINFO*)pMsg;

					MNPLAYER_BASEINFO* pInfo = &pMsgPlayerInfo->Info;
					MURIMNET->GetMNPlayerManager()->AddMNPlayer( pInfo );
					m_pInterface->AddPlayer( pInfo );					
				}
				break;

			case MP_MURIMNET_PR_PLAYERLIST:
				{
					MSG_MNPLAYER_BASEINFOLIST* pMsgPlayerInfoList = (MSG_MNPLAYER_BASEINFOLIST*)pMsg;

					for( DWORD i = 0 ; i < pMsgPlayerInfoList->dwTotalPlayerNum ; ++i )
					{
						MURIMNET->GetMNPlayerManager()->AddMNPlayer( &pMsgPlayerInfoList->PlayerInfo[i] );
						m_pInterface->AddPlayer( &pMsgPlayerInfoList->PlayerInfo[i] );
					}
				}
				break;

			case MP_MURIMNET_PR_PLAYEROUT:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

					CMNPlayer* pMNPlayer = MURIMNET->GetMNPlayerManager()->GetMNPlayer( pmsg->dwData );
					ASSERT( pMNPlayer );

					m_pInterface->RemovePlayer( pMNPlayer->GetBaseInfo() );
					MURIMNET->GetMNPlayerManager()->DeleteMNPlayer( pmsg->dwData );
				}
				break;
					
			case MP_MURIMNET_PR_CAPTAIN:
				{
					MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

					CMNPlayer* pMNPlayer = MURIMNET->GetMNPlayerManager()->GetMNPlayer(pmsg->dwObjectID);

					ASSERT( pMNPlayer );
					if( pMNPlayer == NULL ) break;

					pMNPlayer->SetCaptain( pmsg->bData );
					m_pInterface->SetCaptain( pmsg->bData );
				}
				break;

			case MP_MURIMNET_PR_TEAMCHANGE_ACK:
				{
					MSG_MNTEAMCHANGE* pmsg = (MSG_MNTEAMCHANGE*)pMsg;

					CMNPlayer* pMNPlayer = MURIMNET->GetMNPlayerManager()->GetMNPlayer( pmsg->dwMoverID );
					ASSERT( pMNPlayer );
					if( pMNPlayer == NULL ) break;

					pMNPlayer->SetTeam( pmsg->cbToTeam );
					m_pInterface->TeamChange( pMNPlayer->GetName(),
											  pmsg->cbFromTeam, pmsg->cbToTeam );
				}
				break;

			case MP_MURIMNET_PR_TEAMCHANGE_NACK:
				{
				}
				break;

			case MP_MURIMNET_PR_START:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
					
					//----
					m_dwMapNum		= 99;
					m_dwStartTime	= gCurTime;
					m_bGameStart	= TRUE;
					m_dwCountDown	= 0;
					//----
				}
				break;

			case MP_MURIMNET_PR_PLAYERREIN:
				{
					MSG_DWORD* pMsgPlayer = (MSG_DWORD*)pMsg;
					
					CMNPlayer* pPlayer = MURIMNET->GetMNPlayerManager()->GetMNPlayer( pMsgPlayer->dwData );

//					m_pInterface->PlayerReIn( pPlayer->GetTeam(), pPlayer->GetName() );
				}
				break;				

			}
		}
		break;
	}
}
