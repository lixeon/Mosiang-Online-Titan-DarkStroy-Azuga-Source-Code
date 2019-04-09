// Engine.h: interface for the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINE_H__27B662DC_9848_4737_A9A4_3BA7CB894C62__INCLUDED_)
#define AFX_ENGINE_H__27B662DC_9848_4737_A9A4_3BA7CB894C62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphicEngine.h"
#include "SoundEngine.h"

class CEngine
{
	HWND m_hMainWnd;
	CGraphicEngine* m_pGraphic;
	CSoundEngine* m_pSound;

public:	
	CEngine();
	virtual ~CEngine();	

	void Init(HWND hMainWnd,DISPLAY_INFO* pDispInfo,DWORD MaxShadowNum,DWORD ShadowMapDetail,float SightDistance,DWORD FPS);

	void Release();

	BOOL BeginProcess(GX_FUNC pBeforeRenderFunc,GX_FUNC pAfterRenderFunc);
	void EndProcess();

	CGraphicEngine* GetGraphicsEngine()	{	return m_pGraphic;	}
};

#endif // !defined(AFX_ENGINE_H__27B662DC_9848_4737_A9A4_3BA7CB894C62__INCLUDED_)
