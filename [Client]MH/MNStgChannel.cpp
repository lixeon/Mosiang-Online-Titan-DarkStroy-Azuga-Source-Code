// MNStgChannel.cpp: implementation of the CMNStgChannel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MNStgChannel.h"
#include "MNChannelDialog.h"
#include "./Interface/cWindowManager.h"
#include "MurimNet.h"
#include "MNPlayerManager.h"
#include "MNPlayer.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMNChannelDialog*	CMNStgChannel::m_pInterface	= NULL;
//CMNJoinDialog*	CMNStgChannel::m_pInterfaceJoin		= NULL;
//CMNCreateDialog*	CMNStgChannel::m_pInterfaceCreate	= NULL;


CMNStgChannel::CMNStgChannel()
{
//	m_nCurChannelStg	= CNLSTG_CHANNEL;
//	m_pInterface[0]		= NULL;
//	m_pInterface[1]		= NULL;
//	m_pInterface[2]		= NULL;
	
	m_pInterface		= NULL;
}

CMNStgChannel::~CMNStgChannel()
{

}

void CMNStgChannel::Init()
{
//---create window interfaces

#ifdef _FILE_BIN_
	m_pInterface = (CMNChannelDialog*)WINDOWMGR->GetDlgInfoFromFile( "./Image/MurimNet/Channel.bin", "rb" );
#else
	m_pInterface = (CMNChannelDialog*)WINDOWMGR->GetDlgInfoFromFile( "./Image/MurimNet/Channel.txt" );
#endif

	WINDOWMGR->AddWindow( m_pInterface );

	m_pInterface->Linking();

/////
	m_dwChannelIndex	= 0;
	m_nChannelMode		= eCNL_MODE_MAX;
}

void CMNStgChannel::Release()
{
//---destroy window interfaces
	WINDOWMGR->AddListDestroyWindow( m_pInterface );
	m_pInterface = NULL;
}

void CMNStgChannel::Open()
{
	if( m_pInterface )
		m_pInterface->SetActive( TRUE );

	m_nChannelMode = eCNL_MODE_ID;
	m_pInterface->SetChannelMode( eCNL_MODE_ID );
}

void CMNStgChannel::Close()
{
	if( m_pInterface )
		m_pInterface->SetActive( FALSE );
}

void CMNStgChannel::Process()
{

}

void CMNStgChannel::Render()
{
//---render window interfaces
	WINDOWMGR->Render();
}

void CMNStgChannel::SetChannelMode( int nChannelMode )
{
	if( m_nChannelMode == nChannelMode )
		return;

	m_nChannelMode = nChannelMode;
	m_pInterface->SetChannelMode( nChannelMode );

	MSG_BYTE msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_CHNL_MODECHANGE;
	msg.dwObjectID	= MURIMNET->GetMNPlayerManager()->GetMNHeroID();
	msg.bData		= nChannelMode;
	
	NETWORK->Send( &msg, sizeof(msg) );
}

void CMNStgChannel::ChatMsg( int nClass, CMNPlayer* pMNPlayer, char* strMsg )
{
	m_pInterface->ChatMsg( nClass, pMNPlayer, strMsg );
}

void CMNStgChannel::NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
	switch( Category )
	{
	case MP_MURIMNET:
		{
			switch( Protocol )
			{
			case MP_MURIMNET_CHNL_CHANNELINFO:
				{
					MSG_CHANNEL_BASEINFO* pMsgChannelInfo = (MSG_CHANNEL_BASEINFO*)pMsg;
					m_dwChannelIndex = pMsgChannelInfo->Info.dwChannelIndex;
					m_pInterface->SetChannelInfo( &pMsgChannelInfo->Info );					
				}
				break;

			case MP_MURIMNET_CHNL_PLAYERIN:
				{
					MSG_MNPLAYER_BASEINFO* pMsgPlayerInfo = (MSG_MNPLAYER_BASEINFO*)pMsg;

					MNPLAYER_BASEINFO* pInfo = &pMsgPlayerInfo->Info;
					MURIMNET->GetMNPlayerManager()->AddMNPlayer( pInfo );
					m_pInterface->AddPlayer( pInfo );
				}
				break;

			case MP_MURIMNET_CHNL_PLAYERLIST:
				{
					MSG_MNPLAYER_BASEINFOLIST* pMsgPlayerInfoList = (MSG_MNPLAYER_BASEINFOLIST*)pMsg;

					for( DWORD i = 0 ; i < pMsgPlayerInfoList->dwTotalPlayerNum ; ++i )
					{
						MURIMNET->GetMNPlayerManager()->AddMNPlayer( &pMsgPlayerInfoList->PlayerInfo[i] );
						m_pInterface->AddPlayer( &pMsgPlayerInfoList->PlayerInfo[i] );
					}
				}
				break;

			case MP_MURIMNET_CHNL_PLAYEROUT:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

					CMNPlayer* pMNPlayer = MURIMNET->GetMNPlayerManager()->GetMNPlayer( pmsg->dwData );
					ASSERT( pMNPlayer );
					if( pMNPlayer == NULL ) break;

					m_pInterface->RemovePlayer( pMNPlayer->GetBaseInfo() );
					MURIMNET->GetMNPlayerManager()->DeleteMNPlayer( pmsg->dwData );
				}
				break;

			case MP_MURIMNET_CHNL_CAPTAIN:
				{
				}
				break;

			case MP_MURIMNET_CHNL_CHANNELINFOLIST:
				{
					MSG_CHANNEL_BASEINFOLIST* pMsgChannelInfoList = (MSG_CHANNEL_BASEINFOLIST*)pMsg;

					for( DWORD i = 0 ; i < pMsgChannelInfoList->dwTotalChannelNum ; ++i )
					{
						//MURIMNET->GetChannelManager()->AddChannel( &pMsgChannelInfoList->ChannelInfo[i] );
						m_pInterface->AddChannel( &pMsgChannelInfoList->ChannelInfo[i] );
					}
				}
				break;

			case MP_MURIMNET_CHNL_PLAYROOMINFOLIST:
				{
					MSG_PLAYROOM_BASEINFOLIST* pMsgPlayRoomInfoList = (MSG_PLAYROOM_BASEINFOLIST*)pMsg;

					for( DWORD i = 0 ; i < pMsgPlayRoomInfoList->dwTotalPlayRoomNum ; ++i )
					{
						//MURIMNET->GetChannelManager()->AddPlayRoom( &pMsgPlayRoomInfoList->PlayRoomInfo[i] );
						m_pInterface->AddPlayRoom( &pMsgPlayRoomInfoList->PlayRoomInfo[i] );
					}
				}
				break;
			}
		}
		break;
	}
}


