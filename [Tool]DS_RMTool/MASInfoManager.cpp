// MASInfoManager.cpp: implementation of the CMASInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DS_RMTool.h"
#include "MASInfoManager.h"
#include "MHFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CMASInfoManager)

CMASInfoManager::CMASInfoManager()
{
	memset( m_MASInfo, 0, sizeof(MASINFO)*10 );
	m_nCurMasInfo = -1;
	m_nMaxMasInfo = 0;
}

CMASInfoManager::~CMASInfoManager()
{
}

BOOL CMASInfoManager::ReadMasInfo( char* filename )
{
	CMHFile file;
	char temp[256];

	if( !file.Init( filename, "rt" ) )
		return FALSE;

	while( !file.IsEOF() )
	{
		strcpy( temp, strupr(file.GetString()) );

		if( temp[0] == '/' ) 
		{
			file.GetLine( temp, 256 );
			continue;
		}
		
		if( strcmp( temp, "*MASINFO" ) == 0 )
		{
			int index = file.GetInt();
			file.GetString( m_MASInfo[index].strIP );
			m_MASInfo[index].nPort = file.GetInt();
			m_MASInfo[index].nServerSet = file.GetInt();
			file.GetStringInQuotation( m_MASInfo[index].strServerName );
			++m_nMaxMasInfo;
		}
	}

	file.Release();

	return TRUE;	
}