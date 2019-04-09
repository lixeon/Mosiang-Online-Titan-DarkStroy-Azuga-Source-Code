// SoundItem.cpp: implementation of the CSoundItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoundItem.h"

extern IMilesSoundLib*		g_pSoundLib;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSoundItem::CSoundItem()
{
	idx				= 0;
	m_hSoundFile	= NULL;
	m_MinDist		= 10;
	m_MaxDist		= 100;
	m_fVolume		= 1;
}

CSoundItem::~CSoundItem()
{
	if(m_hSoundFile)
	{
		g_pSoundLib->ReleaseSoundEffectFile(m_hSoundFile);
		m_hSoundFile = NULL;
	}
}
void CSoundItem::Init( SNDIDX idx, char * fileName, BOOL bLoop, BOOL bStreaming, float MinDist, float MaxDist, float fVolume )
{
	idx				= idx;

	char fullpath[1024] = {0,};
	sprintf( fullpath, "%s\\%s", DIRECTORYMGR->GetFullDirectoryName(eLM_Sound), fileName );
	m_szFileName = fullpath;
	m_bLoop = bLoop;
	m_bStream = bStreaming;

	m_MinDist = MinDist;
	m_MaxDist = MaxDist;
	m_fVolume = fVolume;
	if(m_fVolume > 0)
		m_fVolume = 1;
	if(m_fVolume < 0)
		m_fVolume = 0;

#ifdef _DEBUG
	FILE* fp = fopen(fullpath,"rb");
	if(fp == NULL)
	{
		char temp[1024];
		sprintf(temp,"There's No Sound File (%d)%s",idx,fullpath);
//		MessageBox(NULL,temp,NULL,NULL);
	}
	else
		fclose(fp);
#endif
}

CSound* CSoundItem::CreateSound(CObject* pOwnerObject)
{
	if(((LPCTSTR)m_szFileName) == NULL)
	{
//		ASSERTMSG(0,"Null.wav Referenced");
		return NULL;
	}

	if(m_hSoundFile == NULL)
	{
		SOUND_ERROR_CODE herr = g_pSoundLib->CreateSoundEffectFile(&m_hSoundFile,m_szFileName);
		if(herr != SOUND_ERROR_NOERROR)
		{
//			_asm int 3;
			return NULL;
		}
	}

	CSound* pSound = new CSound(this,pOwnerObject);
	return pSound;
}