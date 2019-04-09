//
//		Engine relation Module.
//
//				2002.12.17
//				Trust Pak



#include "StdAfx.h"
#include "EngineRelation.h"
#include "ToolInfo.h"
#include "DlgEffectPalette.h"


/// Global Object ////////////////////////////////////////////////////////////

I4DyuchiGXExecutive* 	g_pExecutive	= NULL;
I4DyuchiAudio*			g_pAudio		= NULL;

GXOBJECT_HANDLE			g_hGxObject;

//const char* const		g_szAudioInitError = "Audio장치 실패로 Wav재생 기능을 사용할 수 없습니다";
const char* const		g_szAudioInitError = "Failed to initialize Audio component object.\nModelview can't play wav file.";



//----------------------------------------------------------------------------
// Name : InitTool
// Description :
//----------------------------------------------------------------------------
BOOL InitTool(HWND hWnd)
{
	CoInitialize(NULL);

	if (S_OK != CoCreateInstance(CLSID_4DyuchiGXExecutive,
								 NULL,
								 CLSCTX_INPROC_SERVER,
								 IID_4DyuchiGXExecutive,
								 (void**)&g_pExecutive))
	{
		return FALSE;
	}

	DISPLAY_INFO	dispInfo;
	memset(&dispInfo, 0, sizeof(dispInfo));
	
	dispInfo.dwWidth	= 800;
	dispInfo.dwHeight	= 600;
	dispInfo.dwBPS		= 2;
	dispInfo.dispType = WINDOW_WITH_BLT;
	dispInfo.dwRefreshRate = 85;

	g_pExecutive->InitializeFileStorage(0,4096,0,FILE_ACCESS_METHOD_ONLY_FILE,NULL,0);
	g_pExecutive->Initialize(hWnd,
		                   &dispInfo,
						   4096,
						   512,
						   1024,
						   32,
						   32,
						   NULL);

	g_pExecutive->GetGeometry()->SetViewport(NULL,0);
	SetFramePerSecond(g_ToolInfo.m_dwFps);
	
	// Initialize Camera
	
	g_ToolInfo.m_Camera.InitCamera(g_pExecutive, 0);
	g_ToolInfo.m_Camera.ResetCamera();


	// Load Grid Information.

	g_ToolInfo.CreateDefaultMap();
	
	g_ToolInfo.LoadGridInformation();
	g_ToolInfo.CreateGridInformation();	
	g_ToolInfo.DisplayBoundingBox(g_ToolInfo.m_bDisplayBoundingBox);
	

	// Initialize Light.

	CreateLight();	

	// Create Font.

	FontCreate();


	// Initialize Audio module.

	if (S_OK != CoCreateInstance(CLSID_4DyuchiAudio,
								 NULL,
								 CLSCTX_INPROC_SERVER,
								 IID_4DyuchiAudio,
								 (void**)&g_pAudio))
	{
		MessageBox(hWnd, g_szAudioInitError, "Error!", MB_OK);
		g_pAudio = NULL;
	}

	if (NULL != g_pAudio)
	{
		if (FALSE == g_pAudio->Initialize(hWnd, MAX_SOUND_OBJECT))
		{
			MessageBox(hWnd, g_szAudioInitError, "Error!", MB_OK);	
			
			g_pAudio->Release();
			g_pAudio = NULL;
		}		
	}

	g_pDlgEffectPalette = new CDlgEffectPalette;
	g_pDlgEffectPalette->DisableView();

//	g_pExecutive->GetRenderer()->SetAlphaRefValue(0);

	g_pExecutive->GetRenderer()->SetAlphaRefValue(5);

	return TRUE;
}


//----------------------------------------------------------------------------
// Name : TerminateTool
// Description :
//----------------------------------------------------------------------------
BOOL TerminateTool(VOID)
{
	if (g_pDlgEffectPalette)
	{
		delete g_pDlgEffectPalette;
		g_pDlgEffectPalette = NULL;
	}

	g_ToolInfo.ClearGridInformation();

	// Destroy Font

	FontDestroy();

	if (NULL != g_pExecutive)
	{
		g_pExecutive->DeleteAllGXObjects();
		g_pExecutive->Release();
	}

	if (NULL != g_pAudio)
	{
		g_ToolInfo.DeleteAllAudio();
		g_pAudio->Release();
	}

	CoFreeUnusedLibraries();
	CoUninitialize();

	return TRUE;	
}


//----------------------------------------------------------------------------
// Name : DrawWork
// Description :
//----------------------------------------------------------------------------
VOID DrawWork(VOID)
{
	g_pExecutive->GetRenderer()->BeginPerformanceAnalyze();	

	DWORD	dwClearColor	=	0xff000000;

	dwClearColor	+=	(g_ToolInfo.m_dwBackGroundColorR << 16);
	dwClearColor	+=	(g_ToolInfo.m_dwBackGroundColorG << 8);
	dwClearColor	+=	(g_ToolInfo.m_dwBackGroundColorB);

//	g_pExecutive->Run(0xFF3A6EA5,
	g_pExecutive->Run(dwClearColor,
					  NULL,
					  AfterRender,
					  BEGIN_RENDER_FLAG_CLEAR_ZBUFFER);	

	g_pExecutive->GetRenderer()->EndPerformanceAnalyze();

	g_pExecutive->GetGeometry()->Present(NULL);

	
}


//----------------------------------------------------------------------------
// Name : AfterRender
// Description :
//----------------------------------------------------------------------------
DWORD __stdcall AfterRender(void)
{
	if (TRUE == g_ToolInfo.m_bRenderCameraInfo)
	{
		g_ToolInfo.RenderCameraInfo();
	}

	return 0;
}


//----------------------------------------------------------------------------
// Name : ClearGXObject
// Description :
//----------------------------------------------------------------------------
VOID ClearGXObjects(VOID)
{
	if (NULL != g_pExecutive)
	{
		//g_pExecutive->DeleteAllGXObjects();

		int i = 0;

		for (i = 0; i < g_ToolInfo.m_lstGXObject.GetCount(); i++)
		{
			POSITION pos = g_ToolInfo.m_lstGXObject.FindIndex(i);
			GXOBJECT_HANDLE gxh = g_ToolInfo.m_lstGXObject.GetAt(pos);
			g_pExecutive->DeleteGXObject(gxh);
		}
	}
}

//----------------------------------------------------------------------------
// Name : ModelProc
// Description :
//----------------------------------------------------------------------------
DWORD __stdcall ModelProcCh(I4DyuchiGXExecutive* pExecutive, GXOBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData)
{
	// Check animation information.

	if (FALSE == g_ToolInfo.m_bIsChFile)
	{
		return 0xFFFFFFFF;
	}

	if (0 == pExecutive->GXOGetMotionNum(gxh, g_ToolInfo.m_iCurrentModNo))
	{
		// It has no animation information.

		return 0xFFFFFFFF;
	}


	// Is it able to play animation?

	if (TRUE == g_ToolInfo.m_bNowPlayed)
	{
		// Get Current Frame.

		DWORD dwFrame = pExecutive->GXOGetCurrentFrame(gxh);
		
		// And Play wav file process.

		if (0 == dwFrame && TRUE == g_ToolInfo.m_bPlaySound)
		{
			g_ToolInfo.PlayWav(g_ToolInfo.m_iCurrentWavNo);			
		}
		
		// Get Current Animation index.

		DWORD dwMotionIndex = pExecutive->GXOGetCurrentMotionIndex(gxh);		

		// Get Motion description.

		MOTION_DESC motionDesc;
		BOOL bHasMotion = pExecutive->GXOGetMotionDesc(gxh, &motionDesc, dwMotionIndex, 0);

		// Control Animation information.

		if (TRUE == bHasMotion)
		{
			// Checking Frame scope.

			if (dwFrame >= motionDesc.dwLastFrame)
			{
				pExecutive->GXOSetCurrentFrame(gxh, 0);
			}
			else
			{
				/////////////////////////////////////////////
				// 이 부분에다 추가..yuchi
//				VECTOR3	v3Axis;
//				float	fRad;
//				pExecutive->GXOGetDirection(gxh,&v3Axis,&fRad);
//				fRad += 0.03f;
//				pExecutive->GXOSetDirection(gxh,&v3Axis,fRad);
				//////////////////////////////////////////////
				pExecutive->GXOSetCurrentFrame(gxh, dwFrame + arg1);    // Increase frame count to reference argument.
			}

			// Update user interface

			g_ToolInfo.m_iScrollPos = pExecutive->GXOGetCurrentFrame(gxh);
		}
	}

	return 0;
}


//----------------------------------------------------------------------------
// Name : ModelProcMod
// Description :
//----------------------------------------------------------------------------
DWORD __stdcall ModelProcMod(I4DyuchiGXExecutive* pExecutive, GXOBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData)
{
	if (TRUE == g_ToolInfo.m_bIsChFile)
	{
		return 0xFFFFFFFF;
	}

	if (FALSE == g_ToolInfo.m_bNowPlayed)
	{
		return 0xFFFFFFFF;
	}

	// Get Animation count to be binded handle of GX-object.

	DWORD dwMotionNo = pExecutive->GXOGetMotionNum(gxh, 0);

	// Get Current Animation index.

	DWORD dwMotionIndex = pExecutive->GXOGetCurrentMotionIndex(gxh);

	// Get Current Frame.

	DWORD dwFrame = pExecutive->GXOGetCurrentFrame(gxh);

	// And Play wav file process.

	if (0 == dwFrame && TRUE == g_ToolInfo.m_bPlaySound)
	{
		g_ToolInfo.PlayWav(g_ToolInfo.m_iCurrentWavNo);			
	}

	// Get Motion description.

	MOTION_DESC motionDesc;
	BOOL bHasMotion = pExecutive->GXOGetMotionDesc(gxh, &motionDesc, dwMotionIndex, 0);


	// Control Animation information.

	if (TRUE == bHasMotion)
	{
		// Checking Frame scope.

		if (dwFrame >= motionDesc.dwLastFrame)
		{
			pExecutive->GXOSetCurrentFrame(gxh, 0);
		}
		else
		{
			/////////////////////////////////////////////
			// 이 부분에다 추가..yuchi
//			VECTOR3	v3Axis;
//			float	fRad;
//			pExecutive->GXOGetDirection(gxh,&v3Axis,&fRad);
//			fRad += 0.03f;
//			pExecutive->GXOSetDirection(gxh,&v3Axis,fRad);
			//////////////////////////////////////////////
		


			pExecutive->GXOSetCurrentFrame(gxh, dwFrame + arg1);    // Increase frame count to reference argument.			
		}

		// Update user interface

		g_ToolInfo.m_iScrollPos = pExecutive->GXOGetCurrentFrame(gxh);
	}

	return 0;
}

//----------------------------------------------------------------------------
// Name : CreateLight
// Description :
//----------------------------------------------------------------------------
BOOL CreateLight(VOID)
{
	g_ToolInfo.m_Camera.SetViewPortNo(0);
	UpdateLightInformation(g_ToolInfo.m_Camera.GetViewPortNo());
	
	return TRUE;
}

//----------------------------------------------------------------------------
// Name : SetCurrentRenderedGxObject
// Description :
//----------------------------------------------------------------------------
BOOL SetCurrentRenderedGxObject(GXOBJECT_HANDLE gxh)
{	
	if (FALSE == g_ToolInfo.m_bIsChFile && FALSE == g_ToolInfo.m_bIsAllSelect)
	{		
		int i = 0;

		for (i = 0; i < g_ToolInfo.m_lstGXObject.GetCount(); i++)
		{
			POSITION pos = g_ToolInfo.m_lstGXObject.FindIndex(i);
			GXOBJECT_HANDLE gxhObject = g_ToolInfo.m_lstGXObject.GetAt(pos);

			g_pExecutive->DisableRender(gxhObject);
		}

		g_pExecutive->EnableRender(gxh);
	}
	else // It is Chr or Chx file
	{
		int i = 0;

		for (i = 0; i < g_ToolInfo.m_lstGXObject.GetCount(); i++)
		{
			POSITION pos = g_ToolInfo.m_lstGXObject.FindIndex(i);
			GXOBJECT_HANDLE gxhObject = g_ToolInfo.m_lstGXObject.GetAt(pos);
			
			g_pExecutive->EnableRender(gxhObject);
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : SetCurrentRenderedAnimation
// Description :
//----------------------------------------------------------------------------
BOOL SetCurrentRenderedAnimation(DWORD dwAnimationIndex)
{
	int i = 0;

	for (i = 0; i < g_ToolInfo.m_lstGXObject.GetCount(); i++)
	{		
		// Above code is increasing dwAnimationIndex.
		// Cuase argument value starts from zero base value.
		// But GXOSetCurrentMotionIndex Function recieve argument from One base value.

		POSITION pos = g_ToolInfo.m_lstGXObject.FindIndex(i);
		GXOBJECT_HANDLE gxh = g_ToolInfo.m_lstGXObject.GetAt(pos);

		g_pExecutive->GXOSetCurrentMotionIndex(gxh,
											   dwAnimationIndex +1);

		g_pExecutive->GXOSetCurrentFrame(gxh,
										 0);
	}

	return TRUE;	
}

//----------------------------------------------------------------------------
// Name : ToolMain
// Description :
//----------------------------------------------------------------------------
BOOL SetCurrentRenderedMaterial(DWORD dwMaterialIndex)
{
	int i = 0;

	for (i = 0; i < g_ToolInfo.m_lstGXObject.GetCount(); i++)
	{
		// Above code is incresing dwMaterialIndex.
		// Becuase argument value starts from zero value.
		// But GXOSetCurrentMaterialIndex function recieve argument starts from one.	
		
		POSITION pos = g_ToolInfo.m_lstGXObject.FindIndex(i);
		GXOBJECT_HANDLE gxh = g_ToolInfo.m_lstGXObject.GetAt(pos);

		g_pExecutive->GXOSetCurrentMaterialIndex(gxh,
												 dwMaterialIndex +1);
		
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : ToolMain
// Description :
//----------------------------------------------------------------------------
VOID ToolMain(VOID)
{
	DrawWork();
}

//----------------------------------------------------------------------------
// Name : MakeCameraMatrix
// Description :
//----------------------------------------------------------------------------
BOOL MakeCameraMatrix(VECTOR3* pv3Out,
					  VECTOR3* pv3Axis,
					  MATRIX4* pmatPitch,
					  MATRIX4* pmatYaw)
{
	VECTOR3 v3Result;
	VECTOR3 v3Temp;

	TransformVector3_VPTR2(&v3Result,
					       pv3Axis,
						   pmatPitch,
						   1);

	v3Temp.x = v3Result.x;
	v3Temp.y = v3Result.y;
	v3Temp.z = v3Result.z;

	TransformVector3_VPTR2(&v3Result,
						   &v3Temp,
						   pmatYaw,
						   1);	
	

	memcpy(pv3Out, &v3Result, sizeof(VECTOR3));	

	
	return TRUE;
}

//---------------------------------------------------------------------------
// Name : SetFramePerSecond
// Description :
//---------------------------------------------------------------------------
BOOL SetFramePerSecond(DWORD dwFps)
{
	g_pExecutive->SetFramePerSec(dwFps);

	return TRUE;
}


//---------------------------------------------------------------------------
// Name : UpdateLightInformation.
// Description :
//---------------------------------------------------------------------------
BOOL UpdateLightInformation(DWORD dwViewPortNo)
{
	if (FALSE == g_ToolInfo.m_bOnLight)
	{
		DisableLight(dwViewPortNo);
	}
	else
	{

		DIRECTIONAL_LIGHT_DESC LightDesc;

		LightDesc.dwAmbient = 0xFF000000 | g_ToolInfo.m_abyAmbient[0] << 16 |
										   g_ToolInfo.m_abyAmbient[1] << 8  |
										   g_ToolInfo.m_abyAmbient[2];

		LightDesc.dwDiffuse = 0xFF000000 | g_ToolInfo.m_abyDiffuse[0] << 16 |
										   g_ToolInfo.m_abyDiffuse[1] << 8  |
										   g_ToolInfo.m_abyDiffuse[2];

		LightDesc.dwSpecular = 0xFF000000 | g_ToolInfo.m_abySpecular[0] << 16 |
											g_ToolInfo.m_abySpecular[1] << 8  |
											g_ToolInfo.m_abySpecular[2];

		LightDesc.v3Dir.x  = g_ToolInfo.m_afDirection[0];
		LightDesc.v3Dir.y  = g_ToolInfo.m_afDirection[1];
		LightDesc.v3Dir.z  = g_ToolInfo.m_afDirection[2];

		EnableLight(&LightDesc, dwViewPortNo);
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// Name : EnableLight
// Description :
//---------------------------------------------------------------------------
BOOL EnableLight(DIRECTIONAL_LIGHT_DESC* pLightDesc, DWORD dwViewPortNo)
{
	g_pExecutive->GetGeometry()->SetDirectionalLight(pLightDesc, dwViewPortNo);
	g_pExecutive->GetGeometry()->EnableDirectionalLight(dwViewPortNo);

	return TRUE;
}

//---------------------------------------------------------------------------
// Name : DisableLight
// Description :
//---------------------------------------------------------------------------
BOOL DisableLight(DWORD dwViewPortNo)
{	
	g_pExecutive->GetGeometry()->DisableDirectionalLight(dwViewPortNo);

	return TRUE;
}

//---------------------------------------------------------------------------
// Name : SetCurrentAnimationFrame
// Description :
//---------------------------------------------------------------------------
BOOL SetCurrentAnimationFrame(DWORD dwFrame)
{
	int i = 0;

	for (i = 0; i < g_ToolInfo.m_lstGXObject.GetCount(); i++)
	{
		// Above code is incresing dwMaterialIndex.
		// Becuase argument value starts from zero value.
		// But GXOSetCurrentMaterialIndex function recieve argument starts from one.	
		
		POSITION pos = g_ToolInfo.m_lstGXObject.FindIndex(i);
		GXOBJECT_HANDLE gxh = g_ToolInfo.m_lstGXObject.GetAt(pos);

		g_pExecutive->GXOSetCurrentFrame(gxh,
										 dwFrame);		
		
	}

	return TRUE;
}


//---------------------------------------------------------------------------
// Name : FontCreate
// Description : 
//    Make Font object of Super shovel 3D Engine.
//---------------------------------------------------------------------------
BOOL FontCreate(VOID)
{
	LOGFONT	font;

	font.lfHeight		= -12;
	font.lfWidth		= 0;
	font.lfEscapement	= 0;
	font.lfOrientation	= 0;
	font.lfWeight		= FW_BOLD;
	font.lfItalic		= 0;
	font.lfUnderline	= 0;
	font.lfStrikeOut	= 0;
	font.lfCharSet		= DEFAULT_CHARSET;
	font.lfOutPrecision = 0;
	font.lfClipPrecision = 0;
	font.lfQuality =  PROOF_QUALITY;
	font.lfPitchAndFamily = 0;
	lstrcpy(font.lfFaceName,"芥竟");

	g_ToolInfo.m_pFont = g_pExecutive->GetRenderer()->CreateFontObject(&font,D3DX_FONT);	
	
	if (!g_ToolInfo.m_pFont)
	{
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// Name : FontDestroy
// Description :
//---------------------------------------------------------------------------
BOOL FontDestroy(VOID)
{
	if (NULL != g_ToolInfo.m_pFont)
	{
		g_ToolInfo.m_pFont->Release();		
		g_ToolInfo.m_pFont = NULL;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// Name : UpdateSpecularEnable
// Description :
//---------------------------------------------------------------------------
BOOL UpdateSpecularEnable(BOOL bSpecular, FLOAT fIntensity)
{
	if (TRUE == bSpecular)
	{
		g_pExecutive->GetRenderer()->EnableSpecular(fIntensity);
	}
	else
	{
		g_pExecutive->GetRenderer()->DisableSpecular();
	}

	return TRUE;
}

//---------------------------------------------------------------------------