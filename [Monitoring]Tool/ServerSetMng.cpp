// ServerSetMng.cpp: implementation of the CServerSetMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerTool.h"
#include "ServerSetMng.h"

#include "MHNetwork.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CServerSetMng)

CServerSetMng::CServerSetMng()
{
	m_nMaxServerSet = 1;
	m_dwTotalUserCount = 0;
}

CServerSetMng::~CServerSetMng()
{
	Release();
}

BOOL CServerSetMng::Init( char* filename )
{
	return ReadMASList(filename);
}

BOOL CServerSetMng::ReadMASList( char* filename )
{
	Release();

	char temp[256];
	strcpy( temp, GetDataDirectory() );
	strcat( temp, filename );

	CMHFile file;
	
	if( !file.Init( temp, "rt" ) )
		return FALSE;

	char setName[256];
	char setFilename[256];

	m_nMaxServerSet = file.GetInt();
	for(int n=0;n<m_nMaxServerSet;++n)
	{
		file.GetString( setFilename );
		file.GetString( setName );

		m_ServerSet[n].Init(n,setName,setFilename);		
	}

	file.Release();

	return TRUE;
}


void CServerSetMng::Release()
{
	m_nMaxServerSet = 1;
	m_dwTotalUserCount = 0;

	for( int i = 0; i < MAX_SERVERSET; ++i )
		m_ServerSet[i].Release();
}

CServerSet* CServerSetMng::GetServerSet( int setindex )
{
	if( setindex < m_nMaxServerSet )
		return &m_ServerSet[setindex];

	return NULL;
}

CServerSet* CServerSetMng::GetServerSet( char* setname )
{
	for( int i = 0; i < m_nMaxServerSet; ++i )
	{
		if( m_ServerSet[i].m_bInit )
		if( strcmp( m_ServerSet[i].m_sServerSetName, setname ) == 0 )
			return &m_ServerSet[i];
	}

	return NULL;
}

DWORD CServerSetMng::GetTotalUserCount()
{
	m_dwTotalUserCount = 0;
	for( int i = 0; i < m_nMaxServerSet; ++i )
	{
		if( m_ServerSet[i].m_bInit )
			m_dwTotalUserCount += m_ServerSet[i].GetTotalUserCount();
	}

	return m_dwTotalUserCount;
}

void CServerSetMng::WriteClientExeVersion( char* setname, char* version )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].WriteClientExeVersion( version );
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->WriteClientExeVersion( version );
	}
}

void CServerSetMng::WriteUserCount( SYSTEMTIME time )
{
	m_dwTotalUserCount = 0;
	for( int i = 0; i < m_nMaxServerSet; ++i )
	{
		if( m_ServerSet[i].m_bInit )
		{
			m_ServerSet[i].WriteUserCount( time );
			m_dwTotalUserCount += m_ServerSet[i].m_dwTotalUserCount;
		}
	}
}

// operate func
void CServerSetMng::DoServerSetOn( char* setname )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoServerSetOn();
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoServerSetOn();
	}
}

void CServerSetMng::DoServerSetOff( char* setname )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoServerSetOff();
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoServerSetOff();
	}
}

void CServerSetMng::DoServerSetRefresh( char* setname )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoServerSetRefresh();
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoServerSetRefresh();
	}
}

void CServerSetMng::DoQueryClientExeVersion()
{
	for( int i = 0; i < m_nMaxServerSet; ++i )
		m_ServerSet[i].DoQueryClientExeVersion();
}

void CServerSetMng::DoChangeClientExeVersion( char* setname, char* version )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoChangeClientExeVersion( version );
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoChangeClientExeVersion( version );
	}
}

void CServerSetMng::DoQueryMaxUser()
{
	for( int i = 0; i < m_nMaxServerSet; ++i )
		m_ServerSet[i].DoQueryMaxUser();
}

void CServerSetMng::DoChangeMaxUser( char* setname, DWORD maxuser )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoChangeMaxUser( maxuser );
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoChangeMaxUser( maxuser );
	}
}

void CServerSetMng::DoQueryUserLevel()
{
	for( int i = 0; i < m_nMaxServerSet; ++i )
		m_ServerSet[i].DoQueryUserLevel();
}

void CServerSetMng::DoChangeUserLevel( char* setname, int userlevel )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoChangeUserLevel( userlevel );
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoChangeUserLevel( userlevel );
	}
}

void CServerSetMng::DoQueryUserCount()
{
	for( int i = 0; i < m_nMaxServerSet; ++i )
		m_ServerSet[i].DoQueryUserCount();	
}

void CServerSetMng::DoSendMessage( char* setname, char* pmsg )
{
	int len = strlen( pmsg );
	if( pmsg == NULL || len > 255 )
		return;

	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoSendMessage( pmsg );
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoSendMessage( pmsg );
	}
}

void CServerSetMng::DoAutoPatch( char* setname )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoAutoPatch();
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoAutoPatch();
	}
}

void CServerSetMng::DoAssertMsgBox( char* setname, BOOL bState )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoAssertMsgBox( bState );
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoAssertMsgBox( bState );
	}
}

void CServerSetMng::DoAllUserDisconnect( char* setname )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoAllUserDisconnect();
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoAllUserDisconnect();
	}
}

void CServerSetMng::DoServerSetOffAfterUserDiscon( char* setname )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoServerSetOffAfterUserDiscon();
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoServerSetOffAfterUserDiscon();
	}
}


void CServerSetMng::DoMSStateAll( char* setname )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoMSStateAll();
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoMSStateAll();
	}
}

void CServerSetMng::DoQueryAutoPatchVersion( char* setname )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoQueryAutoPatchVersion();
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoQueryAutoPatchVersion();
	}
}

void CServerSetMng::DoQueryChannelInfo( char* setname )
{
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
			m_ServerSet[i].DoQueryChannelInfo();
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			pServerSet->DoQueryChannelInfo();
	}
}

BOOL CServerSetMng::IsThereOnServer( char* setname )
{
	BOOL ret = FALSE;
	if( strcmp( "All Server", setname ) == 0 )
	{
		for( int i = 0; i < m_nMaxServerSet; ++i )
		{
			ret = m_ServerSet[i].IsThereOnServer();
			if( ret )	
				return ret;
		}
	}
	else
	{
		CServerSet* pServerSet = GetServerSet( setname );
		if( pServerSet )
			ret = pServerSet->IsThereOnServer();
	}

	return ret;
}
