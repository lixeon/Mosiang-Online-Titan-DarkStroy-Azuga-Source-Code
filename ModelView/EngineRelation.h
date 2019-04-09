//
//		Engine relation Module.
//
//				2002.12.17
//				Trust Pak


#include "StdAfx.h"
#include "../4DyuchiGRX_common/typedef.h"
#include "../4DyuchiGRX_common/IExecutive.h"
#include "../4DyuchiGRX_common/IRenderer.h"
#include "../4DyuchiGRX_common/IAudio.h"
#include "../4DyuchiGRX_common/math.inl"
#include "../4DyuchiGXGFunc/global.h"


#pragma comment(lib, "../4DyuchiGXGFunc/SS3DGFuncN.lib")



#ifndef __TP_ENGINE_RELATION__
#define __TP_ENGINE_RELATION__




/// Constants  ///////////////////////////////////////////////////////////////

const int	MAX_SOUND_OBJECT = 256;



/// Function prototypes //////////////////////////////////////////////////////


VOID ToolMain(VOID);

BOOL InitTool(HWND hWnd);
BOOL TerminateTool(VOID);
VOID DrawWork(VOID);
VOID ClearGXObjects(VOID);

DWORD __stdcall AfterRender(void);


DWORD __stdcall ModelProcCh(I4DyuchiGXExecutive* pExecutive, GXOBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData);
DWORD __stdcall ModelProcMod(I4DyuchiGXExecutive* pExecutive, GXOBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData);



BOOL SetCurrentRenderedGxObject(GXOBJECT_HANDLE gxh);
BOOL SetCurrentRenderedAnimation(DWORD dwAnimationIndex);
BOOL SetCurrentRenderedMaterial(DWORD dwMaterialIndex);

BOOL SetCurrentAnimationFrame(DWORD dwFrame);


BOOL MakeCameraMatrix(VECTOR3* pv3Out,
					  VECTOR3* pv3Axis,
					  MATRIX4* pmatPitch,
					  MATRIX4* pmatYaw);


BOOL SetFramePerSecond(DWORD dwFps);


BOOL CreateLight(VOID);
BOOL UpdateLightInformation(DWORD dwViewPortNo);
BOOL EnableLight(DIRECTIONAL_LIGHT_DESC* pLightDesc, DWORD dwViewPortNo);
BOOL DisableLight(DWORD dwViewPortNo);


// Specular eable relation.

BOOL UpdateSpecularEnable(BOOL bSpecular, FLOAT fIntensity);


// Font for render camera information.

BOOL FontCreate(VOID);
BOOL FontDestroy(VOID);


/// Global Objects ///////////////////////////////////////////////////////////

extern I4DyuchiGXExecutive* 	g_pExecutive;
extern I4DyuchiAudio*			g_pAudio;

#endif
