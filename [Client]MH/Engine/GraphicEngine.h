// GraphicEngine.h: interface for the CGraphicEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHICENGINE_H__99693726_A354_4866_9CF2_93713541A218__INCLUDED_)
#define AFX_GRAPHICENGINE_H__99693726_A354_4866_9CF2_93713541A218__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4dyuchiGRX_common/IRenderer.h"
#include "../4DyuchiGXGFunc/global.h"
#include "../4dyuchiGRX_common/IExecutive.h"
#include "../4dyuchiGRX_myself97_util/MouseLib.h"
#include "PtrList.h"

class CObjectBase;

class CGraphicEngine  
{ 
	DWORD m_BackGroungColor;
	I4DyuchiGXExecutive* m_pExecutive;
	BOOL CreateExecutive(HWND hWnd,DISPLAY_INFO* pDispInfo,DWORD MaxShadowNum,DWORD ShadowMapDetail,float SightDistance,DWORD FPS);

	BOOL m_bRender;
		
public:	
	BOOL BeginProcess(GX_FUNC pBeforeRenderFunc,GX_FUNC pAfterRenderFunc);
	void EndProcess();

	void SetBackGroundColor(DWORD color);
	void SetFixHeight(BOOL bFix,float height);

	CGraphicEngine(HWND hWnd,DISPLAY_INFO* pDispInfo,DWORD MaxShadowNum,DWORD ShadowMapDetail,float SightDistance,DWORD FPS);
	virtual ~CGraphicEngine();

//KES
	void PauseRender( BOOL bPause ) { m_bRender = !bPause; }

};

extern BOOL g_bFixHeight;
extern float g_fFixHeight;
extern DWORD g_bColor;
extern BOOL bRenderSky;
extern cPtrList GarbageObjectList;
extern I4DyuchiGXExecutive* g_pExecutive;
extern CObjectBase* GetSelectedObject(int MouseX,int MouseY);
extern CObjectBase* GetSelectedObjectBoneCheck(int MouseX, int MouseY);
extern VECTOR3* GetPickedPosition(int MouseX,int MouseY);
extern void GetFieldHeight(VECTOR3* pPos);
extern void AddGarbageObject(GXOBJECT_HANDLE handle);
extern void ProcessGarbageObject();


struct EffectGarbage
{
	char filename[64];
	GXOBJECT_HANDLE handle;
};
extern void AddPool(GXOBJECT_HANDLE handle,char* filename);
extern GXOBJECT_HANDLE GetObjectHandle(char* szFileName,GXSchedulePROC pProc,void* pData,DWORD dwFlag);
void ReleasePool();

extern BOOL IsGameSlow();
extern BOOL IsGameFast();
extern BOOL GetCollisonPointWithRay(VECTOR3& From, VECTOR3& To,float height,VECTOR3& Result);

#endif // !defined(AFX_GRAPHICENGINE_H__99693726_A354_4866_9CF2_93713541A218__INCLUDED_)
