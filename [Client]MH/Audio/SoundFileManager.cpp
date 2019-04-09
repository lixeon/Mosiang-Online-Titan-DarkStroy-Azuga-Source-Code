// SoundFileManager.cpp: implementation of the CSoundFileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoundFileManager.h"
#include "..\MHFile.h"
#include "SoundItem.h"
#include "..\DirectoryManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSoundFileManager::CSoundFileManager()
{
}

CSoundFileManager::~CSoundFileManager()
{

}
void CSoundFileManager::Release()
{

}
void CSoundFileManager::Init( CSoundItem ** ppSoudItemList, __int32 & nMaxSound )
{
	DIRECTORYMGR->SetLoadMode(eLM_Sound);

	CMHFile file;
#ifdef _FILE_BIN_
	file.Init( "SoundList.bin", "rb", MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT );
#else
	file.Init( "SoundList.txt", "r", MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT );
#endif

	DIRECTORYMGR->SetLoadMode(eLM_Root);

	nMaxSound = file.GetInt();
	(*ppSoudItemList) = new CSoundItem[nMaxSound];

	WORD idxline = 0;
	char fileName[256];

	char nullwav[] = "NULL.WAV";
	int nulllength = strlen(nullwav);
	for( int i = 0 ; i < nMaxSound ; ++i )
	{
//		if(i == 397)
//		{
//			int a=0;
//		}
		idxline			= file.GetWord();
		strcpy( fileName, file.GetString() );
		BOOL loop		= file.GetBool();
		BOOL streaming	= file.GetBool();

		float mind	= file.GetFloat();
		float maxd	= file.GetFloat();
		float vol	= file.GetFloat();

		char* pTemp = &fileName[strlen(fileName) - nulllength];
		if(strcmp(strupr(pTemp),nullwav) == 0)
		{
			continue;
		}

		ASSERT( idxline == i );
		(*ppSoudItemList)[idxline].Init( i, fileName, loop, streaming, mind, maxd, vol );
	}

	file.Release();

}


