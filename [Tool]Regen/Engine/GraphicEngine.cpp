// GraphicEngine.cpp: implementation of the CGraphicEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphicEngine.h"

#include "EngineObject.h"

#define LIGHT_CONFIG_FILE "light.cfg"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

float gTickPerFrame;
I4DyuchiGXExecutive* g_pExecutive = NULL;
cPtrList GarbageObjectList;
cPtrList EffectPool;
BOOL bRenderSky = FALSE;
BOOL g_bDisplayFPS = FALSE;
DWORD g_bColor;
BOOL g_bFixHeight = FALSE;
float g_fFixHeight = 0;
DISPLAY_INFO g_ScreenInfo;

DWORD __stdcall MHErrorHandleProc(ERROR_TYPE type,DWORD dwErrorPriority,void* pCodeAddress,char* szStr)
{
	return 0;
}

CGraphicEngine::CGraphicEngine(HWND hWnd,DISPLAY_INFO* pDispInfo,DWORD MaxShadowNum,DWORD ShadowMapDetail,float SightDistance,DWORD FPS)
{
	m_BackGroungColor = 0;
	ASSERT(!g_pExecutive);
	CreateExecutive(hWnd,pDispInfo,MaxShadowNum,ShadowMapDetail,SightDistance,FPS);
	g_pExecutive = m_pExecutive;
	g_ScreenInfo = *pDispInfo;
}

CGraphicEngine::~CGraphicEngine()
{
	ReleasePool();
	g_pExecutive = NULL;
	if(m_pExecutive)
	{
		m_pExecutive->DeleteAllGXEventTriggers();
		m_pExecutive->DeleteAllGXLights();
		m_pExecutive->DeleteAllGXObjects();
		m_pExecutive->UnloadAllPreLoadedGXObject(0);
		m_pExecutive->DeleteGXMap(NULL);
		m_pExecutive->Release();
		m_pExecutive = NULL;
	}
}

BOOL CGraphicEngine::CreateExecutive(HWND hWnd,DISPLAY_INFO* pDispInfo,DWORD MaxShadowNum,DWORD ShadowMapDetail,float SightDistance,DWORD FPS)
{
	return TRUE;
}


BOOL CGraphicEngine::BeginProcess(GX_FUNC pBeforeRenderFunc,GX_FUNC pAfterRenderFunc)
{
#ifdef _DEBUG
	if(g_bDisplayFPS)
		m_pExecutive->GetRenderer()->BeginPerformanceAnalyze();
#endif
	
	if(bRenderSky)
		return m_pExecutive->Run(m_BackGroungColor,pBeforeRenderFunc,pAfterRenderFunc,BEGIN_RENDER_FLAG_DONOT_CLEAR_FRAMEBUFFER);
	else
		return m_pExecutive->Run(m_BackGroungColor,pBeforeRenderFunc,pAfterRenderFunc,0);		
}

void CGraphicEngine::EndProcess()
{	
#ifdef _DEBUG
	if(g_bDisplayFPS)
		m_pExecutive->GetRenderer()->EndPerformanceAnalyze();
#endif

	m_pExecutive->GetGeometry()->Present(NULL);

	ProcessGarbageObject();
}

void CGraphicEngine::SetBackGroundColor(DWORD color)
{
	m_BackGroungColor = color;
	g_bColor = color;
}

void CGraphicEngine::SetFixHeight(BOOL bFix,float height)
{
	g_bFixHeight = bFix;
	g_fFixHeight = height;
}

CObjectBase* GetSelectedObject(int MouseX,int MouseY)
{
	static CObjectBase* pSelectedObject;
	CEngineObject* pEngineObject;
	static VECTOR3 pos;
	static float dist;
	static POINT pt;
	pt.x = MouseX;
	pt.y = MouseY;

	DWORD modelindex,objectindex;
	GXOBJECT_HANDLE handle = g_pExecutive->GXOGetObjectWithScreenCoord(&pos,&modelindex,&objectindex,&dist,&pt,0,0);
	if(handle == NULL)
		return NULL;

	pEngineObject = (CEngineObject*)g_pExecutive->GetData(handle);
	if(pEngineObject == NULL)
		return NULL;
	
	return pEngineObject->m_pObject;
}

VECTOR3* GetPickedPosition(int MouseX,int MouseY)
{
	static VECTOR3 TargetPos;
	float fDist;
	POINT pt;
	pt.x = MouseX;
	pt.y = MouseY;
	BOOL bFound = TRUE;
	if(g_bFixHeight)
		TargetPos = GetXYZFromScreenXY2(g_pExecutive->GetGeometry(),MouseX,MouseY,g_ScreenInfo.dwWidth,g_ScreenInfo.dwHeight,g_fFixHeight);
	else
		bFound = g_pExecutive->GXMGetHFieldCollisionPointWithScreenCoord(
			&TargetPos, &fDist, &pt);
		
	if(bFound == FALSE)
		return NULL;

	return &TargetPos;
}

void AddGarbageObject(GXOBJECT_HANDLE handle)
{
	if(g_pExecutive)
	{
		g_pExecutive->SetData(handle,0);
		GarbageObjectList.AddHead(handle);
	}
}

void ProcessGarbageObject()
{
	GXOBJECT_HANDLE handle;
	while(handle = (GXOBJECT_HANDLE)GarbageObjectList.RemoveTail())
	{
		if(g_pExecutive)
			g_pExecutive->DeleteGXObject(handle);
	}
}

void AddPool(GXOBJECT_HANDLE handle,char* filename)
{
	EffectGarbage* pGarbage = new EffectGarbage;
	strcpy(pGarbage->filename,filename);
	pGarbage->handle = handle;
	EffectPool.AddHead(pGarbage);
}
GXOBJECT_HANDLE GetObjectHandle(char* szFileName,GXSchedulePROC pProc,void* pData,DWORD dwFlag)
{
	EffectGarbage* pGarbage;
	PTRLISTPOS pos = EffectPool.GetHeadPosition();
	PTRLISTPOS beforepos;
	while(pos)
	{
		beforepos = pos;
		pGarbage = (EffectGarbage *)EffectPool.GetNext(pos);
		if(strcmp(pGarbage->filename,szFileName)==0)
		{
			EffectPool.RemoveAt(beforepos);
			GXOBJECT_HANDLE h = pGarbage->handle;
			g_pExecutive->SetData(h,pData);
			delete pGarbage;
			return h;
		}
	}

	return g_pExecutive->CreateGXObject(szFileName,pProc,pData,dwFlag);
}

void ReleasePool()
{
	EffectGarbage* pGarbage;
	while(pGarbage = (EffectGarbage*)EffectPool.RemoveTail())
	{
		delete pGarbage;
	}
}