// GameEnd.cpp: implementation of the CGameEnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameEnd.h"
#include "StateMng.h"
#include "DummyObjMng.h"

#include "Console.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameEnd::CGameEnd()
{
	m_dwConnectTime = 0;
}

CGameEnd::~CGameEnd()
{
}

BOOL CGameEnd::Init( int id, CStateMng* pMgr, CDummyNetwork* pNet, CDummyObjMng* pDOMgr )
{
	CGameState::Init( id, pMgr, pNet, pDOMgr );
		
	return FALSE;
}

void CGameEnd::Open( void* pInitParam )
{
	m_pNetwork->SetCurState( this );

	if( pInitParam )
	{
		SYSTEMTIME time;
		GetLocalTime( &time );
		char* pmsg = (char*)pInitParam;
		g_Console.Log( eLogDisplay, 4, "Dummy%d(UserIDx:%d) : Error!! - %s - %d.%d %d:%d:%d", m_nID, m_pStateMng->m_dwUserIDx, pmsg,
					   time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond );
//		LOGDUMMY( "Dummy%d(UserIDx:%d) : Error!! - %s - %d.%d %d:%d:%d", m_nID, m_pStateMng->m_dwUserIDx, pmsg,
//				  time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond );

//		if( strcmp( "LOGIN_ERROR_OVERLAPPEDLOGIN", (const char*)pInitParam ) == 0 )
//		{
//			MSGBASE msg;
//			msg.Category = MP_USERCONN;
//			msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
//			m_pNetwork->Send(&msg,sizeof(msg));

//			m_pStateMng->ChangeState( eGAMESTATE_CONNECT, NULL, 0 );
//		}
//		else
	}
	
	m_pNetwork->Disconnect();
//	}
//	else 
//	{
//		OnDisconnect();
//	}

	m_dwConnectTime = 0;
}

void CGameEnd::Close()
{
}

void CGameEnd::Exit()
{
}

void CGameEnd::Process()
{
/*	++m_dwConnectTime;

	if( m_dwConnectTime > 150 )
	{
		m_pStateMng->ChangeState( eGAMESTATE_TITLE, NULL, 0 );

		m_dwConnectTime = 0;

		m_pStateMng->m_bEnterGame = TRUE;
	}
	*/
}

void CGameEnd::NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
}

void CGameEnd::OnDisconnect()
{
//	MSGBASE data;
//	data.Category	= MP_USERCONN;
//	data.Protocol	= MP_USERCONN_DISCONNECTED;
//	data.dwObjectID	= m_nID;
//	m_pNetwork->Send( &data,sizeof(data) );

	m_pNetwork->Disconnect();	
}