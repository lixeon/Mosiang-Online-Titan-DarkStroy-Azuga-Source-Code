// ResourceMng.cpp: implementation of the CResourceMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResourceMng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CResourceMng)

CResourceMng::CResourceMng()
{
	memset( &m_DisIPPort, 0, sizeof(IPPORT) );
	memset( &m_AgentIPPort, 0, sizeof(IPPORT) );
	m_pMapDesc = NULL;
}

CResourceMng::~CResourceMng()
{
	Exit();
}

void CResourceMng::Init()
{
	LoadGameDesc( "GameDesc.ini", "rt" );
	LoadMapDesc( "MapDesc.txt", "rt" );
}

void CResourceMng::Exit()
{
	if( m_pMapDesc )
	{
		delete [] m_pMapDesc;
		m_pMapDesc = NULL;
	}
}

void CResourceMng::LoadGameDesc( char* filename, char* mode )
{
	CMHFile fp;
	char value[256] = {0,};
	
	if( !fp.Init( filename, mode ) )
		return;
	
	while( 1 )
	{
		if( fp.IsEOF() ) break;

		strcpy( value, strupr(fp.GetString()) );

		if( value[0] == '/')
		{
			fp.GetLine( value, 256 );
			continue;
		}

		if( strcmp( value, "*DISTRIBUTESERVERIP" ) == 0 )
		{
			fp.GetString( m_DisIPPort.Ip );
		}
		else if( strcmp( value, "*DISTRIBUTESERVERPORT" ) == 0 )
		{
			m_DisIPPort.Port = fp.GetWord();
		}
		else if( strcmp( value, "*AGENTSERVERIP" ) == 0 )
		{
			fp.GetString( m_AgentIPPort.Ip );
		}
		else if( strcmp( value, "*AGENTSERVERPORT" ) == 0 )
		{
			m_AgentIPPort.Port = fp.GetWord();
		}
	}

	fp.Release();
}

void CResourceMng::LoadMapDesc( char* filename, char* mode )
{
	CMHFile fp;
	char buff[256] = {0,};
	
	if( !fp.Init( filename, mode ) )
		return;
	
	while( 1 )
	{
		fp.GetString( buff );
		if( fp.IsEOF() ) break;
		if( buff[0] == '@')
		{
			fp.GetLine( buff, 256 );
			continue;
		}

		strcpy( buff, strupr( buff ) );
		if( strcmp( buff, "*TOTALMAPDESC" ) == 0 )
		{
			m_nMaxMapDesc = fp.GetInt();
			if( m_nMaxMapDesc > 0 )
				m_pMapDesc = new MAPDESC[ m_nMaxMapDesc ];
		}
		else if( strcmp( buff, "*MAPDESC" ) == 0 )
		{
			int index = fp.GetInt();
			while( 1 )
			{
				fp.GetString( buff );
				if( fp.IsEOF() ) break;
				if( buff[0] == '@' || buff[0] == '{' ) 
				{
					fp.GetLine( buff, 256 );
					continue;
				}
				if( buff[0] == '}' ) break;
				
				strcpy( buff, strupr( buff ) );
				if( strcmp( buff, "*MAPNUM" ) == 0 )
				{
					m_pMapDesc[index].nMapNum = fp.GetInt();
				}
				else if( strcmp( buff, "*LOGINPOINT" ) == 0 )
				{
					m_pMapDesc[index].vLoginPoint.x = fp.GetFloat();
					m_pMapDesc[index].vLoginPoint.y = 0;
					m_pMapDesc[index].vLoginPoint.z = fp.GetFloat();

				}
				else if( strcmp( buff, "*MAPCHANGENUM" ) == 0 )
				{
					m_pMapDesc[index].nMaxMapChange = fp.GetInt();

					while( 1 )
					{
						fp.GetString( buff );
						if( fp.IsEOF() ) break;
						if( buff[0] == '@' || buff[0] == '{' ) 
						{
							fp.GetLine( buff, 256 );
							continue;
						}
						if( buff[0] == '}' ) break;

						strcpy( buff, strupr( buff ) );
						if( strcmp( buff, "*MAPCHANGEPOINT" ) == 0 )
						{
							int i = fp.GetInt();
							m_pMapDesc[index].MapChangePoint[i].nToMapNum = fp.GetInt();
							m_pMapDesc[index].MapChangePoint[i].nChangeNum = fp.GetInt();
							m_pMapDesc[index].MapChangePoint[i].vMapChange.x = fp.GetFloat();
							m_pMapDesc[index].MapChangePoint[i].vMapChange.y = 0;
							m_pMapDesc[index].MapChangePoint[i].vMapChange.z = fp.GetFloat();							
						}
					}
				}
				else if( strcmp( buff, "*MOVEPOINT" ) == 0 )
				{
					m_pMapDesc[index].nMaxMovePoint = fp.GetInt();

					while( 1 )
					{
						fp.GetString( buff );
						if( fp.IsEOF() ) break;
						if( buff[0] == '@' || buff[0] == '{' ) 
						{
							fp.GetLine( buff, 256 );
							continue;
						}
						if( buff[0] == '}' ) break;

						strcpy( buff, strupr( buff ) );
						if( strcmp( buff, "*POINT" ) == 0 )
						{
							int i = fp.GetInt();
							m_pMapDesc[index].vMovePoint[i].x = fp.GetFloat();
							m_pMapDesc[index].vMovePoint[i].y = 0.0f;
							m_pMapDesc[index].vMovePoint[i].z = fp.GetFloat();
						}
					}
				}
			}
		}
	}

	fp.Release();
}

MAPDESC CResourceMng::GetMapDesc( int mapnum )
{
	for( int i = 0; i < m_nMaxMapDesc; ++i )
	{
		if( m_pMapDesc[i].nMapNum == mapnum )
			break;
	}

	return m_pMapDesc[i];
}