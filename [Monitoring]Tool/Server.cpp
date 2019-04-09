// Server.cpp: implementation of the CServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerTool.h"
#include "Server.h"
#include "MapNameManager.h"

#include "MHNetwork.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern sMSGOPTION	gMsgOption;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServer::CServer(int ServerSetNum,SERVERINFO* pInfo)
{
	m_nServerSet = ServerSetNum;
	m_nServerKind = pInfo->wServerKind;
	m_nState = eServerState_Off;
	strcpy( m_sState, "OFF" );
	m_nServerNum = pInfo->wServerNum;
	strcpy( m_sIP, pInfo->szIPForServer );	
	m_nPort = pInfo->wPortForServer;
	m_dwUserCount = 0;
	m_bDisconnectUser = FALSE;
	m_nAutoPatchState = AUTOPATCHSTATE_END;
	strcpy( m_sAutoPatchState, "End" );
	strcpy( m_sAutoPatchVersion, "Null" );

	memset( m_nChannelUserCount, 0, sizeof(int)*10 );
	m_nMaxChannel = 0;

	switch(m_nServerKind)
	{
	case DISTRIBUTE_SERVER:
		strcpy( m_sName, "Distribute" );
		break;
	case AGENT_SERVER:
		strcpy( m_sName, "Agent" );
		break;
	case MURIM_SERVER:
		strcpy( m_sName, "Murim" );
		break;
	case MAP_SERVER:
//		strcpy( m_sName, GetMapName(m_nServerNum) );
		strcpy( m_sName, MAPNAME->GetMapName(m_nServerNum) );
		break;
	case MONITOR_AGENT_SERVER:
		{
			strcpy( m_sName, "MAS" );
			strcpy( m_sState, "Disconnect" );
		}
		break;
	case MONITOR_SERVER:
		{
			strcpy( m_sName, "MS" );
			strcpy( m_sState, "Disconnect" );
		}
		break;
	default:
		strcpy( m_sName, "ETC" );
	}
	
}

CServer::~CServer()
{
	Release();
}

void CServer::Release()
{
	m_nServerSet = -1;
	m_nServerKind = 0;
	m_nState = eServerState_Off;
	strcpy( m_sState, "OFF" );
	m_nServerNum = 0;
	memset( m_sIP, 0, 16 );	
	m_nPort = 0;
	memset( m_sName, 0, 64 );
	m_dwUserCount = 0;	
	m_bDisconnectUser = FALSE;
	m_nAutoPatchState = AUTOPATCHSTATE_END;
	strcpy( m_sAutoPatchState, "End" );
	strcpy( m_sAutoPatchVersion, "Null" );

	memset( m_nChannelUserCount, 0, sizeof(int)*10 );
	m_nMaxChannel = 0;
}

int CServer::SetState( int state )
{
	m_nState = state;

	switch( state )
	{
	case eServerState_Off:
		{
			strcpy( m_sState, "OFF" );
			m_dwUserCount = 0;
			m_bDisconnectUser = FALSE;
			
			memset( m_nChannelUserCount, 0, sizeof(int)*10 );
			m_nMaxChannel = 0;
		}
		break;
		
	case eServerState_On:
		{
			strcpy( m_sState, "ON" );
			if( m_nServerKind == AGENT_SERVER && atoi(m_sState) == eServerState_Off )
				m_bDisconnectUser = TRUE;
		}
		break;
		
	case eServerState_AbNormal:
		{
			strcpy( m_sState, "AbNormal" );

			CString str;
			char temp[256];
			strcpy( temp, GetDataDirectory() );
			strcat( temp, "ringin.wav" );
			sndPlaySound( temp, SND_LOOP | SND_NODEFAULT | SND_ASYNC );
			str.Format( "%s Server AbNormal!!", m_sName );
			if( gMsgOption.bServer )
			{				
				if( MessageBox( NULL, str, "Error!!", MB_OK ) == IDOK )
				{
					sndPlaySound( NULL, SND_LOOP | SND_NODEFAULT | SND_ASYNC );
					return m_nServerSet;
				}
			}
			else
				return m_nServerSet;

		}
		break;

	case eServerState_Disconnect:
		{
			strcpy( m_sState, "Disconnect" );
			m_nState = eServerState_Off;
		}
		break;
	case eServerState_Connect:
		{
			strcpy( m_sState, "Connect" );
			m_nState = eServerState_On;
		}
		break;
	}

	return -1;
}

void CServer::DoServerOn()
{
	if( m_nState == eServerState_Off )
	{
		MSGEXECUTESERVER msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_EXECUTE_MAPSERVER_SYN;
		msg.ServerType = m_nServerKind;
		msg.ServerNum = m_nServerNum;
		strcpy( msg.ServerIP, m_sIP );
		msg.ServerPort = m_nPort;		

		NETWORK->Send( m_nServerSet, (MSGROOT*)&msg, sizeof(msg) );
	}
}

void CServer::DoServerOff()
{
	if( m_nState == eServerState_On || m_nState == eServerState_AbNormal )
	{
		MSGEXECUTESERVER msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_SHUTDOWN_MAPSERVER_SYN;
		msg.ServerType = m_nServerKind;
		msg.ServerNum = m_nServerNum;
		strcpy( msg.ServerIP, m_sIP );
		msg.ServerPort = m_nPort;
		
		NETWORK->Send( m_nServerSet, (MSGROOT*)&msg, sizeof(msg) );
	}
}

void CServer::DoKillServer()
{
	if( m_nState == eServerState_On || m_nState == eServerState_AbNormal )
	{
		MSGEXECUTESERVER msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_KILL_SERVER_SYN;
		msg.ServerType = m_nServerKind;
		msg.ServerNum = m_nServerNum;
		strcpy( msg.ServerIP, m_sIP );
		msg.ServerPort = m_nPort;
		
		NETWORK->Send( m_nServerSet, (MSGROOT*)&msg, sizeof(msg) );
	}
}

void CServer::SetAutoPatchState( int nState, char* sVersion )
{
	if( m_nAutoPatchState == AUTOPATCHSTATE_END && nState == AUTOPATCHSTATE_PROC )
		return;

	m_nAutoPatchState = nState;
	switch( m_nAutoPatchState )
	{
	case AUTOPATCHSTATE_START:	strcpy( m_sAutoPatchState, "Start" );	break;
	case AUTOPATCHSTATE_PROC:	strcpy( m_sAutoPatchState, "Proc" );	break;
	case AUTOPATCHSTATE_END:	strcpy( m_sAutoPatchState, "End" );		break;			
	}

	strcpy( m_sAutoPatchVersion, sVersion );
}

void CServer::SetAutoPatchVersion( char* sVersion )
{
	strcpy( m_sAutoPatchVersion, sVersion );
}

void CServer::SetChannelUserCount( MSG_CHANNEL_INFO* pChannelInfo )
{
	memset( m_nChannelUserCount, 0, sizeof(int)*10 );
	m_nMaxChannel = 0;

	m_nMaxChannel = pChannelInfo->Count;
	for( int i = 0; i < m_nMaxChannel; ++i )
		m_nChannelUserCount[i] = pChannelInfo->PlayerNum[i];
}