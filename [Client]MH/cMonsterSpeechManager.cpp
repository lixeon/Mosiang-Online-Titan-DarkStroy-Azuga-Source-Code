// cMonsterSpeechManager.cpp: implementation of the cMonsterSpeechManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cMonsterSpeechManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(cMonsterSpeechManager)

cMonsterSpeechManager::cMonsterSpeechManager()
{

}

cMonsterSpeechManager::~cMonsterSpeechManager()
{
	Release();
}

void cMonsterSpeechManager::Init()
{
	m_MonSpeechTable.Initialize(100);

	LoadSpeechList();
}

void cMonsterSpeechManager::Release()
{
	ReleaseSpeechList();
}

bool cMonsterSpeechManager::LoadSpeechList()
{
	CMHFile fp;
	char szBuf[256] = {0,};
	char line[512] = {0,};

	char filename[256];

#ifdef _FILE_BIN_
//	sprintf(filename,"Resource/MonsterSpeechList.bin");
	sprintf(filename,"Resource/Client/MonsterSpeechList.bin");
	if( !fp.Init( filename, "rb" ) )
		return FALSE;
#else
	sprintf(filename,"Resource/MonsterSpeechList.txt");
	if( !fp.Init( filename, "rt" ) )
		return FALSE;
#endif

	int nLen = 0;
	MonSpeechStr* pSpeechStr = NULL;
	while( TRUE )
	{
		fp.GetString(szBuf);

		if(fp.IsEOF())
			break;

		if(szBuf[0] == '@')
		{
			fp.GetLineX(szBuf, 256);
			continue;
		}

		fp.GetStringInQuotation( line );
		nLen = strlen( line );

		if( nLen )
		{
			pSpeechStr = new MonSpeechStr;
			pSpeechStr->idx = atoi(szBuf);
			pSpeechStr->pStr = new char[ nLen + 1 ];
			memcpy( pSpeechStr->pStr, line, nLen + 1 );


			ASSERT(!m_MonSpeechTable.GetData(pSpeechStr->idx));
			m_MonSpeechTable.Add(pSpeechStr, pSpeechStr->idx);

			fp.GetLine( line, 256 );
		}

	}//while

	fp.Release();
	return TRUE;
}
	
void cMonsterSpeechManager::ReleaseSpeechList()
{
	MonSpeechStr* pList = NULL;
	m_MonSpeechTable.SetPositionHead();
	while( pList = m_MonSpeechTable.GetData() )
	{
		if(pList->pStr)
		{
			delete [] pList->pStr;
			pList->pStr = NULL;
		}
		if(pList)
		{
			delete pList;
			pList = NULL;
		}
	}
	m_MonSpeechTable.RemoveAll();
}

char* cMonsterSpeechManager::GetMonsterSpeech(DWORD Idx)
{
	MonSpeechStr* pSpeech = m_MonSpeechTable.GetData(Idx);

	if(!pSpeech)
		return NULL;

	return pSpeech->pStr;
}
