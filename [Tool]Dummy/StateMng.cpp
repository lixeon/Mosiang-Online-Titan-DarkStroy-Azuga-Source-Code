// StateMng.cpp: implementation of the CStateMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StateMng.h"
#include "GameState.h"

#include "ServerConnect.h"
#include "MainTitle.h"
#include "CharSelect.h"
#include "CharMake.h"
#include "GameLoading.h"
#include "GameIn.h"
#include "MapChange.h"
//#include "MurimNet.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CStateMng)

CStateMng::CStateMng()
{
	memset(	m_GameState, 0, sizeof(CGameState*)*eGAMESTATE_MAX );
	m_nCurState = eGAMESTATE_END;	

	m_bStart = FALSE;
	m_bEnterGame = FALSE;

	memset( &m_AgentIpPort, 0, sizeof(m_AgentIpPort) );
	
	m_dwUserIDx = 0;
	m_dwChannel = 0;
	
	m_bEnterAgent = FALSE;
	
	m_dwLoginTime = 0;
}

CStateMng::~CStateMng()
{
}

BOOL CStateMng::Init( int id, int channel )
{
	m_nID = id;
	m_dwChannel = channel;
	m_DummyObjMgr.Init( id, &m_Network);

	m_GameState[eGAMESTATE_END] = &m_GameEnd;
	m_GameState[eGAMESTATE_CONNECT] = &m_ServerConnect;
	m_GameState[eGAMESTATE_TITLE] = &m_MainTitle;	
	m_GameState[eGAMESTATE_CHARSELECT] = &m_CharSelect;	
	m_GameState[eGAMESTATE_CHARMAKE] = &m_CharMake;	
	m_GameState[eGAMESTATE_GAMELOADING] = &m_GameLoading;	
	m_GameState[eGAMESTATE_GAMEIN] = &m_GameIn;	
	m_GameState[eGAMESTATE_MAPCHANGE] = &m_MapChange;	

	for( int i = eGAMESTATE_END; i < eGAMESTATE_MAX; ++i )
		m_GameState[i]->Init( m_nID, this, &m_Network, &m_DummyObjMgr );

	m_nCurState = eGAMESTATE_CONNECT;		

	m_GameState[m_nCurState]->Open( NULL );

	return TRUE;
}

void CStateMng::Exit()
{
	// close
	m_GameState[m_nCurState]->Close();

	// exit
	for( int i = eGAMESTATE_END; i < eGAMESTATE_MAX; ++i )
		m_GameState[i]->Exit();

	m_nCurState = eGAMESTATE_END;
}

int CStateMng::Process()
{
//	if( GetTickCount()-time >= 60000 )
//	{
//		ChangeState( eGAMESTATE_END, "TimeOver", 0 );
//		return 1;
//	}
	if( m_bStart == FALSE )
		return 1;
//	if( m_nCurState != eGAMESTATE_END )
	{
		m_GameState[m_nCurState]->Process();
		return 1;
	}
//	else
//		return -1;
}

void CStateMng::NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
	if( m_GameState[m_nCurState] )
		m_GameState[m_nCurState]->NetworkMsgParse( Category, Protocol, pMsg );
}

void CStateMng::ChangeState( int state, void* pParam, int len )
{
//	if( m_nCurState == state )
//		return;

	m_GameState[m_nCurState]->Close();
	m_nCurState = state;
	m_GameState[m_nCurState]->Open( pParam );	
}

void LOGDUMMY( char* Log, ... )
{
	FILE* fp = fopen( "DummyLog.txt", "a+" );
	if( fp == NULL )
	{
		ASSERT(0);
		return;
	}

	static char va_Temp_Buff[1024];
	
	va_list vl;
	va_start( vl, Log );
	vsprintf( va_Temp_Buff, Log, vl );
	va_end( vl );

	fprintf( fp, va_Temp_Buff );
	fprintf( fp, "\n" );
	fclose(fp);
}

void CStateMng::ForcedMapChange( int mapnum )
{
	if( m_nCurState == eGAMESTATE_GAMEIN )
		m_DummyObjMgr.ForcedToMapChange( mapnum );
}

void CStateMng::SaveLoginPoint() 
{
	if( m_nCurState == eGAMESTATE_GAMEIN )
		m_DummyObjMgr.ActionSaveLogin();
}
