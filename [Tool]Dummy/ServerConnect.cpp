// ServerConnect.cpp: implementation of the CServerConnect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerConnect.h"
#include "StateMng.h"
#include "DummyObjMng.h"

#include "Console.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerConnect::CServerConnect()
{
}

CServerConnect::~CServerConnect()
{
}

BOOL CServerConnect::Init( int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr )
{
	CGameState::Init( id, pMgr, pNet, pDOMgr );
		
	return FALSE;
}

void CServerConnect::Open( void* pInitParam )
{
	m_pNetwork->SetCurState( this );

//	m_pNetwork->Disconnect();
//	m_pStateMng->m_bStart = FALSE;

	g_Console.Log( eLogDisplay, 4, "Dummy%d(UserIDx:%d) State : Waiting to be Started!!", m_nID, m_pStateMng->m_dwUserIDx );
}

void CServerConnect::Close()
{
}

void CServerConnect::Exit()
{
}

void CServerConnect::Process()
{
	m_pStateMng->ChangeState( eGAMESTATE_TITLE, NULL, 0 );
}

void CServerConnect::NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
	CGameState::CommonNetworkMsgParse( Category, Protocol, pMsg );
}

