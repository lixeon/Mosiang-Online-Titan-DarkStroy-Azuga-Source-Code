// GraphicEngine.cpp: implementation of the CGraphicEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphicEngine.h"

#include "EngineObject.h"

//#define LIGHT_CONFIG_FILE "./Resource/light.cfg"
#define LIGHT_CONFIG_FILE "./Resource/Client/light.cfg"

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

int g_SlowCount;
int g_FastCount;


// ÀÌ ÇÚµéÀº °¡Áö°í ÀÖ´Ù°¡ Á¾·áÇÒ¶§ FreeLibrary()·Î ÇØÁ¦ÇÑ´Ù.
HMODULE        g_hExecutiveHandle;

DWORD __stdcall MHErrorHandleProc(ERROR_TYPE type,DWORD dwErrorPriority,void* pCodeAddress,char* szStr)
{
	if(dwErrorPriority == 0)
	{
		ASSERTMSG(0,szStr);
	}
	return 0;
}

CGraphicEngine::CGraphicEngine(HWND hWnd,DISPLAY_INFO* pDispInfo,DWORD MaxShadowNum,DWORD ShadowMapDetail,float SightDistance,DWORD FPS)
{
	m_BackGroungColor = 0;
	ASSERT(!g_pExecutive);
	CreateExecutive(hWnd,pDispInfo,MaxShadowNum,ShadowMapDetail,SightDistance,FPS);
	g_pExecutive = m_pExecutive;
	g_ScreenInfo = *pDispInfo;
	
	m_bRender = TRUE;
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
		m_pExecutive->UnloadAllPreLoadedGXObject(0);	//NULLÃß°¡
		m_pExecutive->DeleteGXMap(NULL);
		m_pExecutive->Release();
		m_pExecutive = NULL;
	}
	
	FreeLibrary(g_hExecutiveHandle);
}

BOOL CGraphicEngine::CreateExecutive(HWND hWnd,DISPLAY_INFO* pDispInfo,DWORD MaxShadowNum,DWORD ShadowMapDetail,float SightDistance,DWORD FPS)
{
	g_SlowCount = 0;
	g_FastCount = 0;
	HRESULT hr;
	
	///* Load DLL »ç¿ë
	// ¸ÕÀú executiveÀÇ DLLÀ» ·ÎµåÇÑ´Ù.
	g_hExecutiveHandle = LoadLibrary("SS3DExecutiveForMuk.dll");
	
	// DllCreateInstance ´Â 3°³ÀÇ ¿£Áø DLLÀÌ ¸ðµÎ ÀÚ½ÅÀÇ °ÍÀ» °¡Áö°í ÀÖ´Ù.°¢ ¿£Áø COMÀÇ ÀÎÅÍÆäÀÌ½º¸¦ 
	// ¾ò°Ô ÇØÁÖ´Â ÇÔ¼öÀÌ´Ù. ÀÏ´Ü executiveÀÇ °Í¸¸ ¾ò¾î³»¸é ´Ù¸¥°Ç executiveÃÊ±âÈ­ ½Ã¿¡ ÀÚµ¿À¸·Î ¾òÀ»¼ö ÀÖ´Ù.
	CREATE_INSTANCE_FUNC        pFunc;
	pFunc = (CREATE_INSTANCE_FUNC)GetProcAddress(g_hExecutiveHandle,"DllCreateInstance");
	hr = pFunc((void**)&m_pExecutive);
	if (hr != S_OK)
	{		
		MessageBox(NULL,"Executive»ý¼º ½ÇÆÐ","Error",MB_OK);
		return FALSE;
	}
	
	if(g_bUsingEnginePack)
	{
		PACKFILE_NAME_TABLE table[7];
		memset(table,0,sizeof(PACKFILE_NAME_TABLE)*7);
		strcpy(table[0].szFileName,"Effect.pak");
		strcpy(table[1].szFileName,"Character.pak");
		strcpy(table[2].szFileName,"Map.pak");
		strcpy(table[3].szFileName,"Monster.pak");
		strcpy(table[4].szFileName,"Npc.pak");
//		strcpy(table[5].szFileName,"cloud.pak");
//		strcpy(table[5].szFileName,"Mapobject.pak");
		strcpy(table[5].szFileName,"Pet.pak");
		strcpy(table[6].szFileName,"Titan.pak");
//		strcpy(table[7].szFileName,"Map2.pak");
		// geometry DLLÀÇ ÀÌ¸§, renderer DLLÀÇ ÀÌ¸§À» ³Ö¾îÁÖ¸é executive°¡ ¾Ë¾Æ¼­ »ý¼º,ÃÊ±âÈ­ÇØÁØ´Ù.
		m_pExecutive->InitializeFileStorageWithoutRegistry("SS3DFileStorage.dll",
			25000,4096,128,FILE_ACCESS_METHOD_FILE_OR_PACK,table,7);
	}
	else
	{
		// geometry DLLÀÇ ÀÌ¸§, renderer DLLÀÇ ÀÌ¸§À» ³Ö¾îÁÖ¸é executive°¡ ¾Ë¾Æ¼­ »ý¼º,ÃÊ±âÈ­ÇØÁØ´Ù.
		m_pExecutive->InitializeFileStorageWithoutRegistry("SS3DFileStorage.dll",
			25000,4096,128,FILE_ACCESS_METHOD_ONLY_FILE,NULL,0);
	}

	pDispInfo->dwRefreshRate = 0;
	m_pExecutive->InitializeWithoutRegistry("SS3DGeometryForMuk.dll","SS3DRendererForMuk.dll",
		hWnd, pDispInfo, 8000, 300,0,1, 0,MHErrorHandleProc);
	//*/
	
	/*  COM »ç¿ë
	// Excutive»ý¼º
	hr = CoCreateInstance(
		CLSID_4DyuchiGXExecutive,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_4DyuchiGXExecutive,
		(void**)&m_pExecutive);
	
	if (hr != S_OK)
	{
		MessageBox(NULL,"Can not find CLSID_4DyuchiGXExecutive","Error",MB_OK);
		return FALSE;
	}

	m_pExecutive->Initialize(hWnd, pDispInfo, 20000, 32,10000,32, 32,MHErrorHandleProc);
	*/
	
	
	m_pExecutive->PreCreateLight(LIGHT_CONFIG_FILE,0);
	
	
	m_pExecutive->GetGeometry()->SetDrawDebugFlag(0);
	
	m_pExecutive->GetGeometry()->SetViewport(NULL,0);
	
	m_pExecutive->GetGeometry()->SetAmbientColor(0,0xaaaaaaaa);
	
//	
//	m_pExecutive->GetGeometry()->CreateShadowMap("psteam_1.tga",MaxShadowNum,ShadowMapDetail,0);
	m_pExecutive->GetGeometry()->SetShadowFlag(ENABLE_PROJECTION_SHADOW | ENABLE_PROJECTION_TEXMAP);
	
	VECTOR3 pos;
	pos.x = pos.y = pos.z = 0;
	m_pExecutive->GetGeometry()->ResetCamera(&pos,100,SightDistance,gPHI/4,0);


	m_pExecutive->GetRenderer()->BeginRender(0,0,0);
	m_pExecutive->GetRenderer()->EndRender();

	m_pExecutive->SetFramePerSec(FPS);
	
	m_pExecutive->GetRenderer()->SetVerticalSync(FALSE);
	
	m_pExecutive->GetRenderer()->EnableSpecular(0);

	m_pExecutive->GetRenderer()->SetAlphaRefValue(5);
	
	//m_pExecutive->GetRenderer()->SetRenderMode(2);

	return TRUE;
}


BOOL CGraphicEngine::BeginProcess(GX_FUNC pBeforeRenderFunc,GX_FUNC pAfterRenderFunc)
{
#ifdef _DEBUG
	if(g_bDisplayFPS)
		m_pExecutive->GetRenderer()->BeginPerformanceAnalyze();
#endif
	if(gTickTime > 100)
		++g_SlowCount;
	else if(g_SlowCount>0)
		--g_SlowCount;
	if(gTickTime < 50)
		++g_FastCount;
	else if(g_FastCount>0)
		--g_FastCount;

	
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

	if( m_bRender )	//KES
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

CObjectBase* GetSelectedObjectBoneCheck(int MouseX, int MouseY)
{
	static CObjectBase* pSelectedObject;
	CEngineObject* pEngineObject;
	static VECTOR3 pos;
	static float dist;
	static POINT pt;
	pt.x = MouseX;
	pt.y = MouseY;
	

	DWORD modelindex,objectindex;
	GXOBJECT_HANDLE handle = g_pExecutive->GXOGetObjectWithScreenCoord(&pos,&modelindex,&objectindex,&dist,&pt,0,PICK_TYPE_PER_BONE_OBJECT);	
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

BOOL IsGameSlow()
{
	return g_SlowCount > 5;
}

BOOL IsGameFast()
{
	return g_FastCount > 5;
}

void GetFieldHeight(VECTOR3* pPos)
{
	g_pExecutive->GXMGetHFieldHeight(&pPos->y,pPos->x,pPos->z);
}


BOOL GetCollisonPointWithRay(VECTOR3& From, VECTOR3& To,float height,VECTOR3& Result)
{
	if(From.y <= To.y)
		return FALSE;
	
	float	t = (-1*(height+From.y)) / (To.y - From.y);		// Á÷¼± ¹æÁ¤½Ä¿¡¼­ y°¡ 0ÀÏ¶§ t°ª.
	float	x = From.x+t * (To.x-From.x);
	float	z = From.z+t * (To.z-From.z);
	Result.x	=	x;
	Result.y	=	height;
	Result.z	=	z;
	
	return	TRUE;
}
