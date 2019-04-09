// SoundEngine.cpp: implementation of the CSoundEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoundEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

I4DyuchiAudio* g_pAudio = NULL;

CSoundEngine::CSoundEngine(HWND hWnd,int MaxAudioObject)
{
	CreateAudio(hWnd,MaxAudioObject);
	g_pAudio = m_pAudio;
}

CSoundEngine::~CSoundEngine()
{
	if(m_pAudio)
	{
		m_pAudio->Release();
		m_pAudio = NULL;
	}
	g_pAudio = NULL;
}

BOOL CSoundEngine::CreateAudio(HWND hWnd,int MaxAudioObject)
{		
	HRESULT hr;

	hr = CoCreateInstance(
           CLSID_4DyuchiAudio,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_4DyuchiAudio,
           (void**)&m_pAudio);

	if (hr != S_OK)
	{
		MessageBox(NULL,"Fail to init audio","Error",MB_OK);
		return FALSE;
	}

	m_pAudio->Initialize(hWnd,MaxAudioObject);


	return TRUE;
}

