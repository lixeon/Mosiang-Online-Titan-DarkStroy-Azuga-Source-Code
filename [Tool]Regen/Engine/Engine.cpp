// Engine.cpp: implementation of the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "Engine.h"
#include "mmsystem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MAX_AUDIO_OBJECT_NUM 100

CEngine::CEngine()
:	m_pGraphic(NULL)
,	m_pSound(NULL)
{

}

CEngine::~CEngine()
{
	Release();
}

void CEngine::Init(HWND hMainWnd,DISPLAY_INFO* pDispInfo,DWORD MaxShadowNum,DWORD ShadowMapDetail,float SightDistance,DWORD FPS)
{
	m_hMainWnd = hMainWnd;
	CoInitialize(NULL);

	m_pGraphic = new CGraphicEngine(hMainWnd,pDispInfo,MaxShadowNum,ShadowMapDetail,SightDistance,FPS);
	m_pSound = new CSoundEngine(hMainWnd,MAX_AUDIO_OBJECT_NUM);
}

void CEngine::Release()
{	
	if(m_pSound)
	{
		delete m_pSound;
		m_pSound = NULL;
	}
	if(m_pGraphic)
	{
		delete m_pGraphic;
		m_pGraphic = NULL;
	}


	CoUninitialize();
	CoFreeUnusedLibraries();
}

BOOL CEngine::BeginProcess(GX_FUNC pBeforeRenderFunc,GX_FUNC pAfterRenderFunc)
{
	return m_pGraphic->BeginProcess(pBeforeRenderFunc,pAfterRenderFunc);
}

void CEngine::EndProcess()
{
	m_pGraphic->EndProcess();
}
