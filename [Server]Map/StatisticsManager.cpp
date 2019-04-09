// StatisticsManager.cpp: implementation of the CStatisticsManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatisticsManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatisticsManager::CStatisticsManager()
{
	m_dwLastSaveTime = 0;
}

CStatisticsManager::~CStatisticsManager()
{

}

void CStatisticsManager::Init()
{
	ZeroMemory( m_dwMonsterDieCount, sizeof( m_dwMonsterDieCount ) );
	m_dwLastSaveTime = gCurTime;
}

void CStatisticsManager::Process()
{
	DWORD dwElapsedTime = gCurTime - m_dwLastSaveTime;

	if( dwElapsedTime >= 1000*60*60 )
	{
		SaveMonsterStatistics();

		m_dwLastSaveTime = gCurTime;
	}

}

void CStatisticsManager::SaveMonsterStatistics()
{
	SYSTEMTIME ti;
	GetLocalTime( &ti );

	char fname[256];
	sprintf(fname,"./Log/MobDie_%02d_%02d%02d%02d.txt", g_pServerSystem->GetMapNum(),
														ti.wYear, ti.wMonth, ti.wDay );
	FILE* fp = NULL;
	fp = fopen( fname, "a+" );
	if(!fp) return;
	
	fprintf( fp, "%02d\t%02d\t%02d\n", ti.wHour, ti.wMinute, ti.wSecond );

	for( int i = 0 ; i < MAX_MONSTER_NUM ; ++i )
	{
		if( m_dwMonsterDieCount[i] > 0 )
		{
			fprintf( fp, "\t%d\t%d\n", i,  m_dwMonsterDieCount[i] );
		}
	}

	fclose(fp);
	
	ZeroMemory( m_dwMonsterDieCount, sizeof( m_dwMonsterDieCount ) );
}

void CStatisticsManager::MonsterDieCount( WORD wMonsterKind )
{
	if( wMonsterKind >= MAX_MONSTER_NUM ) return;

	++m_dwMonsterDieCount[wMonsterKind];
}

