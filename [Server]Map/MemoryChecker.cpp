// MemoryChecker.cpp: implementation of the CMemoryChecker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MemoryChecker.h"


//

char keyname[eMemoryChecker_Max][128] = 
{
	"eSkillFactory_MakeSkillObject",
	"eObjectFactory_Player",
	"eObjectFactory_Monster",
	"eObjectFactory_Npc",
	"eObjectFactory_Tactic",
	"eObjectFactory_BossMonster",
	"ePackedData_Init",
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoryChecker::CMemoryChecker()
{
	m_MemoryPool = new CMemoryPoolTempl<stMemoryInfo>;
	m_MemoryPool->Init( 100, 10, "MemoryCheckerPool" );
	m_MemoryTable.Initialize( 100 );
	m_LastSaveTime = 0;	
}


CMemoryChecker::~CMemoryChecker()
{
	m_MemoryTable.SetPositionHead();
	stMemoryInfo* pTemp = NULL;
	while( pTemp = m_MemoryTable.GetData() )
	{
		m_MemoryPool->Free( pTemp );
	}
	m_MemoryTable.RemoveAll();
	SAFE_DELETE( m_MemoryPool );	
}


stMemoryInfo* CMemoryChecker::GetMemoryInfo( DWORD Key )
{
	stMemoryInfo* pInfo = m_MemoryTable.GetData( Key );
	if( !pInfo )
	{
		pInfo = m_MemoryPool->Alloc();
		pInfo->type = Key;
		m_MemoryTable.Add( pInfo, Key );
	}

	return pInfo;
}
/*
void CMemoryChecker::AddTemplete( CMemoryPoolTempl* pTplete )
{
	if( !pTplete )		return;

	m_TempleteList.AddTail( pTplete );
}
*/
void CMemoryChecker::Process()
{
#ifndef _IGNORE_ASSERT_
	if( (gCurTime-m_LastSaveTime) < 600000 )		return;

	SYSTEMTIME time;
	char szFile[256] = {0,};
	GetLocalTime( &time );
	sprintf( szFile, "./Log/MemoryCheker_%02d_%4d%02d%02d.txt", g_pServerSystem->GetMapNum(), time.wYear, time.wMonth, time.wDay );

	FILE* fp = fopen(szFile,"a+");
	if(!fp)		return;	

	char buf[256] = { 0, };
	stMemoryInfo* pInfo = NULL;
	m_MemoryTable.SetPositionHead();
	while( pInfo = m_MemoryTable.GetData() )
	{
		sprintf( buf, "%d	%s	%d	%d	%d",
			pInfo->type, keyname[pInfo->type], pInfo->Created, pInfo->Released, pInfo->MemorySize );

		fprintf(fp,"%s\n",buf);
	}

	fclose(fp);

	m_LastSaveTime = gCurTime;	
#endif
}
