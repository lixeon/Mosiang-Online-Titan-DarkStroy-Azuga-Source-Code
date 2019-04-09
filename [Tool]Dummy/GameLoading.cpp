// GameLoading.cpp: implementation of the CGameLoading class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameLoading.h"
#include "StateMng.h"
#include "DummyObjMng.h"

#include "Console.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern DWORD g_ConnectDist;
extern DWORD g_DisconnectDist;
extern DWORD g_TryToConnectAgent;
extern DWORD g_ConnectAgent;
extern DWORD g_DisconnectAgent;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameLoading::CGameLoading()
{
}

CGameLoading::~CGameLoading()
{
}

BOOL CGameLoading::Init( int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr )
{
	CGameState::Init( id, pMgr, pNet, pDOMgr );

	return TRUE;
}

void CGameLoading::Open( void* pInitParam )
{
	m_pNetwork->SetCurState( this );

//	g_Console.Log( eLogDisplay, 4, "Dummy%d State : CServerConnect", m_nID );

	if( pInitParam )
	{
		int objnum = m_pDummyObjMgr->GetObjectNum();
		SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO*)pInitParam;

		m_Msg.Category = MP_USERCONN;
		m_Msg.Protocol = MP_USERCONN_CHARACTERSELECT_SYN;
		m_Msg.dwObjectID = pmsg->BaseObjectInfo[0].dwObjectID;
//		m_pNetwork->Send( &m_Msg, sizeof(m_Msg) );
	}	
}

void CGameLoading::Close()
{
}

void CGameLoading::Exit()
{
}

void CGameLoading::Process()
{
	if( m_pStateMng->m_bEnterGame )
	{
		m_pNetwork->Send( &m_Msg, sizeof(m_Msg) );

		m_pStateMng->m_bEnterGame = FALSE;
	}
}

void CGameLoading::NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
	CGameState::CommonNetworkMsgParse( Category, Protocol, pMsg );
	
	switch( Category )
	{
	case MP_USERCONN:
		{
			switch( Protocol ) 
			{
			case MP_USERCONN_CHARACTERSELECT_ACK:
				{
					MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
					m_pDummyObjMgr->SetMapNum( pmsg->bData );

					m_pStateMng->ChangeState( eGAMESTATE_GAMEIN, NULL, 0 );					
				}
				return;
				
			case MP_USERCONN_CHARACTERSELECT_NACK:
				{
					m_pNetwork->Disconnect();

					m_pStateMng->ChangeState( eGAMESTATE_END, "GameLoading - MP_USERCONN_CHARACTERSELECT_NACK", 0 );					

					g_DisconnectAgent++;
				}
				return;
			}
		}
		break;
	}
}

void CGameLoading::OnDisconnect()
{
	m_pStateMng->ChangeState( eGAMESTATE_END, "CGameLoading-OnDisconnect()", 0 );
	g_DisconnectAgent++;
}