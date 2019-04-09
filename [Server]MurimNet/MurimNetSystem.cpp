// MurimNetSystem.cpp: implementation of the CMurimNetSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MurimNetSystem.h"

#include "MNServerManager.h"
#include "MNPlayerManager.h"
#include "ChannelManager.h"
#include "PlayRoomManager.h"
#include "Front.h"


CMurimNetSystem g_MNSystem;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMurimNetSystem::CMurimNetSystem()
{
	m_pMNServerMgr	= NULL;
	m_pMNPlayerMgr	= NULL;
	m_pChannelMgr	= NULL;
	m_pPlayRoomMgr	= NULL;
//	m_pFront		= NULL;
}

CMurimNetSystem::~CMurimNetSystem()
{

}

void CMurimNetSystem::Init()
{
	m_pMNServerMgr		= new CMNServerManager;
	m_pMNPlayerMgr		= new CMNPlayerManager;
	m_pChannelMgr		= new CChannelManager;
	m_pPlayRoomMgr		= new CPlayRoomManager;
//	m_pFront			= new CFront;

//	m_pMNServerMgr->Init();	//
	m_pMNPlayerMgr->Init( MAXPLAYER_IN_MURIMNET );
	m_pChannelMgr->Init( MAXCHANNEL_IN_MURIMNET );
	m_pPlayRoomMgr->Init( MAXPLAYROOM_IN_MURIMNET );
//	m_pFront->Init( MAXPLAYER_IN_FRONT );	


/*///////////
//---for test
	PLAYROOM_INFO PlayRoomInfo;

	PlayRoomInfo.dwPlayRoomIndex	= 1;
	PlayRoomInfo.nPlayRoomKind		= 0;
	PlayRoomInfo.dwMaxPlayerNum		= 50;
	PlayRoomInfo.dwMaxPlayerPerTeam	= 20;
	맵넘버 인자 추가 HS
	
	strcpy( PlayRoomInfo.strPlayRoomName, "무림넷 테스트 게임" );

	m_pPlayRoomMgr->CreatePlayRoom( &PlayRoomInfo );
*///////////

}


void CMurimNetSystem::Release()
{
	m_pMNPlayerMgr->Release();
//	m_pChannelMgr->Release();
	m_pPlayRoomMgr->Release();

	SAFE_DELETE( m_pMNServerMgr );
	SAFE_DELETE( m_pMNPlayerMgr );
	SAFE_DELETE( m_pChannelMgr );
	SAFE_DELETE( m_pPlayRoomMgr );
//	SAFE_DELETE( m_pFront );
}



void CMurimNetSystem::SelectChannelMode( CMNPlayer* pMNPlayer, int nChannelMode )
{
	switch( nChannelMode )
	{
	case eCNL_MODE_ID:
		{

		}
		break;
	case eCNL_MODE_CHANNEL:
		{
			m_pChannelMgr->SendMsg_ChannelList( pMNPlayer );
		}
		break;
	case eCNL_MODE_PLAYROOM:
		{
			m_pPlayRoomMgr->SendMsg_PlayRoomList( pMNPlayer );
		}
		break;
	}
}