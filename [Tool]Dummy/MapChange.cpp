// MapChange.cpp: implementation of the CMapChange class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapChange.h"
#include "StateMng.h"
#include "MoveMng.h"

#include "Console.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMapChange)

CMapChange::CMapChange()
{
}

CMapChange::~CMapChange()
{
}

BOOL CMapChange::Init( int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr )
{
	CGameState::Init( id, pMgr, pNet, pDOMgr );
	
	return TRUE;
}

void CMapChange::Open( void* pInitParam )
{
	m_pNetwork->SetCurState( this );

//	g_Console.Log( eLogDisplay, 4, "Dummy%d State : CMapChange", m_nID );
}

void CMapChange::Close()
{
}

void CMapChange::Exit()
{
}

void CMapChange::Process()
{
	MOVEMNG->Reset();

	m_pStateMng->ChangeState( eGAMESTATE_GAMEIN, NULL, 0 );
}

void CMapChange::NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
	CGameState::CommonNetworkMsgParse( Category, Protocol, pMsg );
}

void CMapChange::OnDisconnect()
{
	m_pStateMng->ChangeState( eGAMESTATE_END, "CMapChange-OnDisconnect()", 0 );
}
