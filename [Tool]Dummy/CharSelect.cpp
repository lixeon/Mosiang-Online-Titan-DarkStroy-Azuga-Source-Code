// CharSelect.cpp: implementation of the CCharSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharSelect.h"
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

CCharSelect::CCharSelect()
{
}

CCharSelect::~CCharSelect()
{
}

BOOL CCharSelect::Init( int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr )
{
	CGameState::Init( id, pMgr, pNet, pDOMgr );

	return TRUE;
}

void CCharSelect::Open( void* pInitParam )
{
	m_pNetwork->SetCurState( this );

	if( pInitParam )
	{
		SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO *)pInitParam;
		NetworkMsgParse( pmsg->Category, pmsg->Protocol, pInitParam );
	}

}

void CCharSelect::Close()
{
}

void CCharSelect::Exit()
{
}

void CCharSelect::Process()
{
}

void CCharSelect::NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
	CGameState::CommonNetworkMsgParse( Category, Protocol, pMsg );

	switch( Category ) 
	{
	case MP_USERCONN:
		{
			switch( Protocol ) 
			{
			case MP_USERCONN_CHARACTERLIST_ACK:
				{
					SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO*)pMsg;
					if( pmsg->CharNum )
					{
						m_pDummyObjMgr->SetObjectNum( pmsg->CharNum );

						m_pStateMng->ChangeState( eGAMESTATE_GAMELOADING, pMsg, sizeof(SEND_CHARSELECT_INFO) );
					}
					else
					{
						m_pStateMng->ChangeState( eGAMESTATE_CHARMAKE, "NULL", 0 );
					}
				}				
				return;
			case MP_USERCONN_CHARACTERLIST_NACK:
				{
					g_DisconnectAgent++;
					
					m_pNetwork->Disconnect();

					m_pStateMng->ChangeState( eGAMESTATE_END, "CharSelect - MP_USERCONN_CHARACTERLIST_NACK", 0 );
				}
				return;
			}
		}
		return;
	}
}

void CCharSelect::OnDisconnect()
{
	m_pStateMng->ChangeState( eGAMESTATE_END, "CCharSelect-OnDisconnect()", 0 );

	g_DisconnectAgent++;
}
