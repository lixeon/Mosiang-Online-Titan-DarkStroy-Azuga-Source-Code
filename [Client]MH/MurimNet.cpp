// MurimNet.cpp: implementation of the CMurimNet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MurimNet.h"
#include "MNStgFront.h"
#include "MNStgChannel.h"
#include "MNStgPlayRoom.h"
#include "MNPlayerManager.h"
#include "MainGame.h"

#include "MHTimeManager.h"
#include "./Interface/cWindowManager.h"

//#include "./Interface/cResourceManager.h"
#include "cMsgBox.h"

GLOBALTON(CMurimNet)
char gStrTemp128[128];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMurimNet::CMurimNet()
{
	m_pMNPlayerManager = NULL;
}

CMurimNet::~CMurimNet()
{

}

void CMurimNet::InitStages()
{
	m_apMNStage[MNSTG_FRONT] = new CMNStgFront;
	m_apMNStage[MNSTG_CHANNEL] = new CMNStgChannel;
	m_apMNStage[MNSTG_PLAYROOM] = new CMNStgPlayRoom;

	m_apMNStage[MNSTG_FRONT]->Init();
	m_apMNStage[MNSTG_CHANNEL]->Init();
	m_apMNStage[MNSTG_PLAYROOM]->Init();


	m_nCurMNStage = MNSTG_CHANNEL;
//---TestPlayRoom
//	m_nCurMNStage = MNSTG_PLAYROOM;
}

void CMurimNet::ReleaseStages()
{
	m_apMNStage[MNSTG_FRONT]->Release();
	m_apMNStage[MNSTG_CHANNEL]->Release();
	m_apMNStage[MNSTG_PLAYROOM]->Release();

	SAFE_DELETE( m_apMNStage[MNSTG_FRONT] );
	SAFE_DELETE( m_apMNStage[MNSTG_CHANNEL] );
	SAFE_DELETE( m_apMNStage[MNSTG_PLAYROOM] );
}

BOOL CMurimNet::Init( void* pInitParam )
{
	m_pMNPlayerManager = new CMNPlayerManager;
	m_pMNPlayerManager->Init();
	
	NETWORK->SetCurState(this);
//load images and create windows for murim-net interface
	InitStages();

	if(pInitParam != NULL)
		SendConnectMsg( *(DWORD*)pInitParam );
	else
		SendReConnectMsg();	// pInitParam 이 NULL 일경우는 무림서버에 갔다가 돌아오는 경우.
	
	m_apMNStage[m_nCurMNStage]->Open();

	return TRUE;
}


void CMurimNet::Release(CGameState* pNextGameState)
{
	m_apMNStage[m_nCurMNStage]->Close();

//release images and windows for murim-net
	ReleaseStages();

	m_pMNPlayerManager->Release();
	SAFE_DELETE( m_pMNPlayerManager );
}


void CMurimNet::Process()
{
///	MHTIMEMGR->Process();	//maingame에서 process하는 편이 낫지 않은가?
	m_apMNStage[m_nCurMNStage]->Process();
}


void CMurimNet::ChangeStage( int nStage )
{
	if( m_nCurMNStage == nStage ) return;

	m_apMNStage[m_nCurMNStage]->Close();
	m_nCurMNStage = nStage;
	m_apMNStage[m_nCurMNStage]->Open();
}


void CMurimNet::BeforeRender()
{
	g_pExecutive->GetRenderer()->BeginRender(0,0,0);
	g_pExecutive->GetRenderer()->EndRender();
}


void CMurimNet::AfterRender()
{
	m_apMNStage[m_nCurMNStage]->Render();
}


void CMurimNet::NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_MURIMNET_CONNECT_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CMNPlayer* pMNPlayer = m_pMNPlayerManager->GetMNPlayer( pmsg->dwObjectID );
			ASSERT( pMNPlayer );
			m_pMNPlayerManager->SetMNHero( pMNPlayer );

			//---접속성공처리
		}
		break;

	case MP_MURIMNET_RECONNECT_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CMNPlayer* pMNPlayer = m_pMNPlayerManager->GetMNPlayer( pmsg->dwObjectID );
			ASSERT( pMNPlayer );
			m_pMNPlayerManager->SetMNHero( pMNPlayer );
		}
		break;
		
	case MP_MURIMNET_DISCONNECT_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			MAINGAME->SetGameState( eGAMESTATE_MAPCHANGE, &pmsg->dwData, 4 );
		}
		break;

	case MP_MURIMNET_CHAT_ALL:
		{
			TESTMSGID* pmsg = (TESTMSGID*)pMsg;
					
			CMNPlayer* pMNPlayer = MURIMNET->GetMNPlayerManager()->GetMNPlayer( pmsg->dwSenderID );
			ASSERT( pMNPlayer );
			if( pMNPlayer == NULL ) break;

			GetCurMNStage()->ChatMsg( PRCTC_WHOLE, pMNPlayer, pmsg->Msg );
		}
		break;

	default:
		{
			m_apMNStage[m_nCurMNStage]->NetworkMsgParse( Category, Protocol, pMsg );
		}
		break;
	}

}

void CMurimNet::SendConnectMsg( DWORD dwBackMapNum )
{
	MSG_DWORD msg;

	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_CONNECT_SYN;
	msg.dwData		= dwBackMapNum;
	
	NETWORK->Send( &msg, sizeof(msg) );
}

void CMurimNet::SendReConnectMsg()
{
	MSGBASE msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_RECONNECT_SYN;
	

	NETWORK->Send( &msg, sizeof(msg) );
}