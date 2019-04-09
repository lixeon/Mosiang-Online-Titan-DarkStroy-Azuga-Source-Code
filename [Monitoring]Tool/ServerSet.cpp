// ServerSet.cpp: implementation of the CServerSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerTool.h"
#include "ServerSet.h"
#include "..\[CC]Header\CommonGameFunc.h"
#include "..\[CC]ServerModule\ServerListManager.h"

#include "MHNetwork.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerSet::CServerSet()
{
	memset( m_sMASIP, 0, 16 );
	m_nMASPort = 0;

	memset( m_sServerInfoFileName, 0, 256 );
	memset( m_sClientExeVersion, 0, 256 );

	m_nServerSet = 0;
	memset( m_sServerSetName, 0, 256 );

	m_dwTotalUserCount = 0;
	m_nUserLevel = eUSERLEVEL_SUPERUSER;
	strcpy( m_sUserLevel, "SuperUser" );
	
	m_bInit = FALSE;
	
	m_dwMsgCheckTime = 0;

	m_dwMaxUser = 5000;
}

CServerSet::~CServerSet()
{	
	Release();
}

BOOL CServerSet::Init(int ServerSetNum,char* setName,char* setFileName)
{
	m_nServerSet = ServerSetNum;
	strcpy(m_sServerSetName,setName);
	return InitFromFile(setFileName);
}

BOOL CServerSet::InitFromFile(char* setFileName)
{
	char temp[256];
	strcpy( temp, GetDataDirectory() );
	strcat( temp, setFileName );
	SERVERLISTMGR->Load(temp);
	SERVERLISTMGR->SetPositionHead();
	SERVERINFO* pInfo;
	
	CServer* pServer;
	while(pInfo = SERVERLISTMGR->GetNextServerInfo())
	{
		pServer = new CServer(m_nServerSet,pInfo);

		if( pInfo->wServerKind == MONITOR_AGENT_SERVER )
		{
			strcpy(m_sMASIP,pInfo->szIPForUser);
			m_nMASPort = pInfo->wPortForUser;

			m_vecMSServer.push_back( pServer );
		}
		else if( pInfo->wServerKind == MONITOR_SERVER )
		{
			m_vecMSServer.push_back( pServer );
		}
		else
		{
			m_vecServer.push_back(pServer);
		}
	}

	m_bInit = TRUE;
	
	ReadClientExeVersion();
	CreateUserCountFile();	

	SERVERLISTMGR->Clear();

	return TRUE;
}

void CServerSet::Release()
{
	memset( m_sMASIP, 0, 16 );
	m_nMASPort = 0;

	memset( m_sServerInfoFileName, 0, 256 );
	memset( m_sClientExeVersion, 0, 256 );

	m_nServerSet = 0;
	memset( m_sServerSetName, 0, 256 );

	m_dwTotalUserCount = 0;
	m_nUserLevel = eUSERLEVEL_SUPERUSER;
	strcpy( m_sUserLevel, "SuperUser" );
	
	m_bInit = FALSE;

	m_dwMsgCheckTime = 0;

	m_dwMaxUser = 5000;

	vector<CServer*>::iterator iter = m_vecServer.begin();
	for(;iter != m_vecServer.end();++iter)
		delete (*iter);
	m_vecServer.clear();

	iter = m_vecMSServer.begin();
	for(;iter != m_vecMSServer.end();++iter)
		delete (*iter);
	m_vecMSServer.clear();
}

CServer* CServerSet::GetServer( WORD serverkind, WORD servernum )
{	
	vector<CServer*>::iterator iter = m_vecServer.begin();
	for(;iter != m_vecServer.end();++iter)
	{
		if( (*iter)->m_nServerKind == serverkind &&
			(*iter)->m_nServerNum == servernum )
			return (*iter);
	}

	return NULL;
}

CServer* CServerSet::GetServer( SERVERSTATE* pState )
{
	CServer* pServer = GetServer(pState->ServerType,pState->ServerNum);
	if(pServer == NULL)
		return NULL;

//	ASSERT(strcmp( pState->ServerIP, pServer->m_sIP ) == 0);
//	ASSERT(pState->ServerPort == pServer->m_nPort);

	return pServer;
}

void CServerSet::SetUserLevel( int userlevel )
{
	m_nUserLevel = userlevel;

	switch( userlevel )
	{
	case eUSERLEVEL_GOD:		strcpy( m_sUserLevel, "God");			break;
	case eUSERLEVEL_PROGRAMMER:	strcpy( m_sUserLevel, "Programmer");	break;
	case eUSERLEVEL_DEVELOPER:	strcpy( m_sUserLevel, "Developer");		break;
	case eUSERLEVEL_GM:			strcpy( m_sUserLevel, "GM");			break;
	case eUSERLEVEL_SUPERUSER:	strcpy( m_sUserLevel, "SuperUser");		break;
	case eUSERLEVEL_USER:		strcpy( m_sUserLevel, "User");			break;
	case eUSERLEVEL_SUBUSER:	strcpy( m_sUserLevel, "SubUser");		break;
	}	
}

void CServerSet::ReadClientExeVersion()
{
	char temp[256];
	strcpy( temp, GetDataDirectory() );
	strcat( temp, m_sServerSetName );
	strcat( temp, "_ClientExeVersion.ver" );
	
	FILE* fp = fopen( temp, "r" );
	if( fp )
	{
		fscanf( fp, "%s", temp );
		fclose( fp );
		strcpy( m_sClientExeVersion, temp );
		return;
	}

	strcpy( m_sClientExeVersion, "None!!" );
}

void CServerSet::WriteClientExeVersion( char* version )
{
	if( m_bInit )
	{
		strcpy( m_sClientExeVersion, version );

		char temp[256];
		strcpy( temp, GetDataDirectory() );		
		strcat( temp, m_sServerSetName );
		strcat( temp, "_ClientExeVersion.ver" );

		FILE* fp = fopen( temp, "w" );
		if( fp )
		{
			fwrite( version, strlen(version), 1, fp );
			fclose( fp );
		}
	}
}

void CServerSet::CreateUserCountFile()
{
	SYSTEMTIME time;
	GetLocalTime( &time );
	char temp[256];
	char filename[256];
	strcpy( temp, GetUserCountDirectory() );
	sprintf( filename, "%s_%04d%02d%02d_UserCount.txt", m_sServerSetName, time.wYear, time.wMonth, time.wDay );
	strcat( temp, filename );

	FILE* fp = fopen( temp, "a+" );
	if( fp == NULL )
		return;

	fprintf( fp, "Time\tTotal\tDist\tAgent\tMuim\t" );

	// map
	
	vector<CServer*>::iterator iter = m_vecServer.begin();
	for(;iter != m_vecServer.end();++iter)
	{
		if((*iter)->m_nServerKind != MAP_SERVER)
			continue;

		fprintf( fp, "%s\t", (*iter)->m_sName );
	}

	fprintf( fp, "\n" );

	fclose( fp );
}

void CServerSet::WriteUserCount( SYSTEMTIME time )
{
	vector<CServer*>::iterator iter = m_vecServer.begin();
	if( m_bInit )
	if( IsThereOnServer() )
	{
		char temp[256];
		char filename[256];	
		strcpy( temp, GetUserCountDirectory() );
		sprintf( filename, "%s_%04d%02d%02d_UserCount.txt", m_sServerSetName, time.wYear, time.wMonth, time.wDay );
		strcat( temp, filename );

		FILE* fp = fopen( temp, "a+" );
		if( fp == NULL )
			return;

		fprintf( fp, "%02d:%02d:%02d\t", time.wHour, time.wMinute, time.wSecond );

		DWORD total = 0;
		DWORD dist = 0;
		DWORD agent = 0;
		DWORD murim = 0;

		// distribute
		iter = m_vecServer.begin();
		for(;iter != m_vecServer.end();++iter)
		{
			if((*iter)->m_nServerKind != DISTRIBUTE_SERVER)
				continue;
			
			dist += (*iter)->m_dwUserCount;
		}
		total += dist;

		// agent
		iter = m_vecServer.begin();
		for(;iter != m_vecServer.end();++iter)
		{
			if((*iter)->m_nServerKind != AGENT_SERVER)
				continue;
			
			agent += (*iter)->m_dwUserCount;
		}
		total += agent;

		// murim
		iter = m_vecServer.begin();
		for(;iter != m_vecServer.end();++iter)
		{
			if((*iter)->m_nServerKind != MURIM_SERVER)
				continue;
			
			agent += (*iter)->m_dwUserCount;
		}

		fprintf( fp, "%d\t%d\t%d\t%d\t", total, dist, agent, murim );
		// map
		iter = m_vecServer.begin();
		for(;iter != m_vecServer.end();++iter)
		{
			if((*iter)->m_nServerKind != MAP_SERVER)
				continue;
			
			fprintf( fp, "%d\t", (*iter)->m_dwUserCount );
		}
		fprintf( fp, "\n" );

		fclose(fp);
	}
}

BOOL CServerSet::IsThereOnServer()
{	
	vector<CServer*>::iterator iter = m_vecServer.begin();
	for(;iter != m_vecServer.end();++iter)
	{
		if((*iter)->m_nState == eServerState_On)
			return TRUE;
	}
	return FALSE;
}

DWORD CServerSet::GetTotalUserCount()
{
	DWORD total = 0;
	DWORD dist = 0;
	DWORD agent = 0;
	
	vector<CServer*>::iterator iter = m_vecServer.begin();
	// distribute
	for(;iter != m_vecServer.end();++iter)
	{
		if((*iter)->m_nServerKind != DISTRIBUTE_SERVER)
			continue;
		
		dist += (*iter)->m_dwUserCount;
	}
	total += dist;
	
	// agent
	iter = m_vecServer.begin();
	for(;iter != m_vecServer.end();++iter)
	{
		if((*iter)->m_nServerKind != AGENT_SERVER)
			continue;
		
		agent += (*iter)->m_dwUserCount;
	}
	total += agent;

	m_dwTotalUserCount = total;

	return m_dwTotalUserCount;
}

BOOL CServerSet::CheckDisconnectUser()
{
	vector<CServer*>::iterator iter = m_vecServer.begin();
	for(;iter != m_vecServer.end();++iter)
	{
		if((*iter)->m_bDisconnectUser)
			return FALSE;
	}

	return TRUE;
}

// operate fn----------------------------------------------------------------------------
void CServerSet::DoServerSetOn() 
{
	if( m_bInit )
	{
		// on 순서 : map, murim -> agent -> distribute
		vector<CServer*>::iterator iter = m_vecServer.begin();
		for(;iter != m_vecServer.end();++iter)
		{
			if((*iter)->m_nServerKind != MAP_SERVER)
				continue;
			
			(*iter)->DoServerOn();
		}
		iter = m_vecServer.begin();
		for(;iter != m_vecServer.end();++iter)
		{
			if((*iter)->m_nServerKind != MURIM_SERVER)
				continue;
			
			(*iter)->DoServerOn();
		}
		iter = m_vecServer.begin();
		for(;iter != m_vecServer.end();++iter)
		{
			if((*iter)->m_nServerKind != AGENT_SERVER)
				continue;
			
			(*iter)->DoServerOn();
		}
		iter = m_vecServer.begin();
		for(;iter != m_vecServer.end();++iter)
		{
			if((*iter)->m_nServerKind != DISTRIBUTE_SERVER)
				continue;
			
			(*iter)->DoServerOn();
		}
	}
}

void CServerSet::DoServerSetOff()
{
	if( m_bInit )
	{
		// on 순서 : map, murim -> agent -> distribute
		vector<CServer*>::iterator iter = m_vecServer.begin();
		for(;iter != m_vecServer.end();++iter)
		{
			if((*iter)->m_nServerKind != MAP_SERVER)
				continue;
			
			(*iter)->DoServerOff();
		}
		iter = m_vecServer.begin();
		for(;iter != m_vecServer.end();++iter)
		{
			if((*iter)->m_nServerKind != MURIM_SERVER)
				continue;
			
			(*iter)->DoServerOff();
		}
		iter = m_vecServer.begin();
		for(;iter != m_vecServer.end();++iter)
		{
			if((*iter)->m_nServerKind != AGENT_SERVER)
				continue;
			
			(*iter)->DoServerOff();
		}
		iter = m_vecServer.begin();
		for(;iter != m_vecServer.end();++iter)
		{
			if((*iter)->m_nServerKind != DISTRIBUTE_SERVER)
				continue;
			
			(*iter)->DoServerOff();
		}
	}
}

void CServerSet::DoServerSetRefresh()
{
	if( m_bInit )
	{
		MSGROOT msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_MAPSERVERSTATE_SYN;

		NETWORK->Send( m_nServerSet, &msg, sizeof(msg) );
	}
}

void CServerSet::DoServerOn( WORD serverkind, WORD servernum )
{
	if( m_bInit )
	{
		CServer* pServer = GetServer(serverkind,servernum);
		if(pServer)
			pServer->DoServerOn();
	}
}

void CServerSet::DoServerOff( WORD serverkind, WORD servernum )
{
	if( m_bInit )
	{
		CServer* pServer = GetServer(serverkind,servernum);
		if(pServer)
			pServer->DoServerOff();
	}
}

void CServerSet::DoQueryClientExeVersion()
{
	if( m_bInit )
	{
		MSGROOT msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_QUERY_VERSION_SYN;
		
		NETWORK->Send( m_nServerSet, (MSGROOT*)&msg, sizeof(msg) );
	}
}

void CServerSet::DoChangeClientExeVersion( char* version )
{
	if( m_bInit )
	{
		MSGNOTICE msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_CHANGE_VERSION_SYN;
		strcpy( msg.Msg, version );
		
		NETWORK->Send( m_nServerSet, (MSGROOT*)&msg, sizeof(msg) );
	}
}

void CServerSet::DoQueryMaxUser()
{
	if( m_bInit )
	{
		MSGROOT msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_QUERY_MAXUSER_SYN;
		
		NETWORK->Send( m_nServerSet, (MSGROOT*)&msg, sizeof(msg) );
	}
}

void CServerSet::DoChangeMaxUser( DWORD maxuser )
{
	if( m_bInit )
	{
		MSG_DWORD msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_CHANGE_MAXUSER_SYN;
		msg.dwData = maxuser;
		
		NETWORK->Send( m_nServerSet, (MSGROOT*)&msg, sizeof(msg) );
	}
}

void CServerSet::DoQueryUserLevel()
{
	if( m_bInit )
	{
		MSG_DWORD msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_CHANGE_USERLEVEL_SYN;
		msg.dwData = 0;
	
		NETWORK->Send( m_nServerSet, (MSGROOT*)&msg, sizeof(msg) );
	}
}

void CServerSet::DoChangeUserLevel( int userlevel )
{
	if( m_bInit )
	{
		MSG_DWORD msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_CHANGE_USERLEVEL_SYN;
		msg.dwData = userlevel;

		NETWORK->Send( m_nServerSet, (MSGROOT*)&msg, sizeof(msg) );
	}
}

void CServerSet::DoQueryUserCount()
{
	if( m_bInit )
	{
		MSGROOT msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_QUERYUSERCOUNT_SYN;

		NETWORK->Send( m_nServerSet, &msg, sizeof(msg) );
	}
}

void CServerSet::DoSendMessage( char* pmsg )
{
	if( m_bInit )
	{
		MSGNOTICE msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_NOTICESEND_SYN;
		strcpy( msg.Msg, pmsg );
		
		NETWORK->Send( m_nServerSet, (MSGROOT*)&msg, sizeof(msg) );
	}
}

void CServerSet::DoAutoPatch()
{
	if( m_bInit )
	{
		MSGROOT msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_EXECUTE_AUTOPATCH_SYN;
		
		NETWORK->Send( m_nServerSet, &msg, sizeof(msg) );
	}
}

void CServerSet::DoAssertMsgBox( BOOL bState )
{
	if( m_bInit )
	{
		MSG_DWORD msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_ASSERTMSGBOX_SYN;
		msg.dwData = (DWORD)bState;
		
		NETWORK->Send( m_nServerSet, &msg, sizeof(msg) );
	}
}

void CServerSet::DoAllUserDisconnect()
{
	if( m_bInit )
	{
		MSGROOT msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_DISCONNECT_ALLUSER_SYN;

		NETWORK->Send( m_nServerSet, &msg, sizeof(msg) );
	}
}

void CServerSet::DoServerSetOffAfterUserDiscon()
{
	if( CheckDisconnectUser() )
		DoServerSetOff();
}

void CServerSet::DoMSStateAll()
{
	if( m_bInit )
	{
		MSGROOT msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_MSSTATEALL_SYN;

		NETWORK->Send( m_nServerSet, &msg, sizeof(msg) );	
	}
}

void CServerSet::DoQueryChannelInfo()
{
	if( m_bInit )
	{
		MSGROOT msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_QUERY_CHANNELINFO_SYN;

		NETWORK->Send( m_nServerSet, &msg, sizeof(msg) );	
	}
}

CServer* CServerSet::GetMSServer( MSSTATE* pState )
{
	vector<CServer*>::iterator iter = m_vecMSServer.begin();
	for(;iter != m_vecMSServer.end();++iter)
	{
		if( ( (*iter)->m_nPort == pState->wServerPort ) &&
			( strcmp((*iter)->m_sIP, pState->sServerIP) == 0 ) )
			return (*iter);
	}

	return NULL;
}

CServer* CServerSet::GetServer( char* pServerIp, WORD wServerPort )
{
	vector<CServer*>::iterator iter = m_vecServer.begin();
	for(;iter != m_vecServer.end();++iter)
	{
		if( ( (*iter)->m_nPort == wServerPort ) &&
			( strcmp((*iter)->m_sIP, pServerIp) == 0 ) )
			return (*iter);
	}

	return NULL;
}

void CServerSet::ClearMSStateAll()
{
	vector<CServer*>::iterator iter = m_vecMSServer.begin();
	for(;iter != m_vecMSServer.end();++iter)
	{
		(*iter)->SetState( eServerState_Disconnect );
	}
}

void CServerSet::DoQueryAutoPatchVersion()
{
	if( m_bInit )
	{
		MSGROOT msg;
		msg.Category = MP_MORNITORTOOL;
		msg.Protocol = MP_MORNITORTOOL_QUERY_AUTOPATCHVERSION_SYN;

		NETWORK->Send( m_nServerSet, &msg, sizeof(msg) );	
	}
}