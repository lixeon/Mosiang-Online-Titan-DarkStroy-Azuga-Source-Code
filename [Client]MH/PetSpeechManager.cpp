#include "stdafx.h"
#include ".\petspeechmanager.h"

GLOBALTON(CPetSpeechManager)

CPetSpeechManager::CPetSpeechManager(void)
{
}

CPetSpeechManager::~CPetSpeechManager(void)
{
	Release();
}

void CPetSpeechManager::Init()
{
	m_PetSpeechTable.Initialize(100);

	LoadPetSpeechList();
}

void CPetSpeechManager::Release()
{
	ReleasePetSpeechList();
}

BOOL CPetSpeechManager::LoadPetSpeechList()
{
	CMHFile fp;
	char szBuf[256] = {0,};
	char line[512] = {0,};

	char filename[256];

#ifdef _FILE_BIN_
//	sprintf(filename, "Resource/PetSpeechList.bin");
	sprintf(filename, "Resource/Client/PetSpeechList.bin");
	if( !fp.Init( filename, "rb" ) )
		return FALSE;
#else
	sprintf(filename, "Resource/PetSpeechList.txt");
	if( !fp.Init( filename, "rt) ) )
		return FALSE;
#endif

	int nLen = 0;
	PetSpeechStr* pSpeechStr = NULL;
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
			pSpeechStr = new PetSpeechStr;
			pSpeechStr->Idx = atoi(szBuf);
			pSpeechStr->pStr = new char[ nLen + 1 ];
			memcpy( pSpeechStr->pStr, line, nLen + 1 );

			ASSERT(!m_PetSpeechTable.GetData(pSpeechStr->Idx));
			m_PetSpeechTable.Add(pSpeechStr, pSpeechStr->Idx);

			fp.GetLine( line, 256 );
		}
	}//while

	fp.Release();

	return TRUE;
}

void CPetSpeechManager::ReleasePetSpeechList()
{
	PetSpeechStr* pList = NULL;
	m_PetSpeechTable.SetPositionHead();
	while( pList = m_PetSpeechTable.GetData() )
	{
		if( pList->pStr )
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
	m_PetSpeechTable.RemoveAll();
}

char* CPetSpeechManager::GetPetSpeech(DWORD strIdx)
{
	PetSpeechStr* pSpeech = m_PetSpeechTable.GetData(strIdx);

	if(!pSpeech)
		return NULL;

	return pSpeech->pStr;
}