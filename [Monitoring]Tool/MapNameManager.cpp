// MapNameManager.cpp: implementation of the CMapNameManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapNameManager.h"
#include "mhfile.h"
#include "FileBin.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMapNameManager)

CMapNameManager::CMapNameManager()
{
	m_MapNameTable.Initialize( 100 );
}

CMapNameManager::~CMapNameManager()
{
	char* pData = NULL;
	m_MapNameTable.SetPositionHead();
	while( pData = m_MapNameTable.GetData() )
	{
		delete pData;
	}
	m_MapNameTable.RemoveAll();
}

void CMapNameManager::ReadMapName()
{
	CMHFile file;
	if( !file.Init( "./Data/MapName.txt", "rt" ) )
		return;

	DWORD dwMapNum = 0;
	DWORD dwMax = file.GetDword();
	for( DWORD i = 0; i < dwMax; ++i )
	{
		dwMapNum = file.GetInt();
		char* pMapName = new char[128];
		file.GetStringInQuotation( pMapName );

		m_MapNameTable.Add( pMapName, dwMapNum );
	}

	file.Release();
/*
	CFileBin file;
	if( !file.OpenBin( "./Data/MapName.bin" ) )
		return;

	DWORD dwMapNum = 0;
	while( !file.IsEOF() )
	{
		dwMapNum = file.GetInt();
		if( dwMapNum > 0 )
		{
			char* pMapName = new char[128];
			file.GetStringInQuotation( pMapName );

			m_MapNameTable.Add( pMapName, dwMapNum );
		}		
	}

	file.Clear();
	*/
}

char* CMapNameManager::GetMapName( DWORD dwMapNum )
{
	static char temp[128] = "";
	char* pData = m_MapNameTable.GetData( dwMapNum );
	if( pData )
		return pData;
	else
		return temp;
}