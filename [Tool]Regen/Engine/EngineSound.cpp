// EngineSound.cpp: implementation of the CEngineSound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineSound.h"
#include "SoundEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngineSound::CEngineSound()
{
	m_hSound = NULL;
	m_SoundFileName[0] = 0;
}

CEngineSound::~CEngineSound()
{
	Release();
}

BOOL CEngineSound::Init(char* SoundFileName)
{
	if(strcmp(SoundFileName,m_SoundFileName) == 0)
		return TRUE;

	DIRECTORYMGR->SetLoadMode(eLM_Sound);
	m_hSound = g_pAudio->CreateSoundObject(SoundFileName,MHSoundProc,this,0);
	
		DIRECTORYMGR->SetLoadMode(eLM_Root);
	if(m_hSound == NULL)
	{
		return FALSE;
	}
	return TRUE;
}

void CEngineSound::Release()
{
	if(m_hSound && g_pAudio)
	{
		g_pAudio->DeleteSoundObject(m_hSound);
		m_hSound = NULL;
	}
}

void CEngineSound::Play()
{
	ASSERT(m_hSound);
	g_pAudio->Play(m_hSound,0);
}

void CEngineSound::Stop()
{
	ASSERT(m_hSound);
	g_pAudio->Stop(m_hSound);
}

void CEngineSound::SetVolume(long Volume)
{
	ASSERT(m_hSound);
	g_pAudio->SetVolume(m_hSound,Volume,0);
}

DWORD MHSoundProc(I4DyuchiAudio* pAudio,GXSOUND_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData)
{
	return 0;
}
