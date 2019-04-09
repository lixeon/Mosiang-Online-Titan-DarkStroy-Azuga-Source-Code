
// Tool.cpp: implementation of the CTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "4DyuchiGXMapEditor.h"
#include "Tool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "../4DyuchiGRX_myself97/My2DLib.h"
#include "../4DyuchiGXGFunc/global.h"
#include "mainfrm.h"
#include "LightProperty.h"
#include "ObjectProperty.h"
#include "TriggerProperty.h"
#include "ItemCount.h"
#include <crtdbg.h>


//yh
#include "TileSetApplyer.h"
//


//In
#include "TileTableDesc.h"
//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTool* g_pTool = NULL;
CDlgBarTool* g_pDlgBarTool = NULL;

VECTOR3	g_v3Point;
//VECTOR3	g_v3TriList[4096*3];
//DWORD	g_dwMaxTriCount = 4096;
//DWORD	g_dwTriCount = 0;

float __stdcall ShadeFactor(float fLightRs,float fLightRsRs,float fDistance,float fDistDist,float cosang)
{
	float fFallOff,rsfactor;

	rsfactor = (fLightRs - fDistance)*(fLightRs - fDistance) / fLightRsRs;
	fFallOff	= rsfactor + cosang*rsfactor;
//	fFallOff	= cosang*rsfactor;


//	float fFallOff = (fLightRs - fDistance)*(fLightRs - fDistance)*1.7f / fLightRsRs + cosang*0.3f;
	return fFallOff;

}

DWORD AddIndex(HFIELD_POS* pPosListDest,DWORD dwPosDestNum,HFIELD_POS* pPosListSrc,DWORD dwPosSrcNum)
{
	for (DWORD i=0; i<dwPosSrcNum; i++)
	{
		for (DWORD j=0; j<dwPosDestNum; j++)
		{
			if (pPosListSrc[i].dwX == pPosListDest[j].dwX && pPosListSrc[i].dwZ == pPosListDest[j].dwZ)
				goto lb_skip;
		}
		pPosListDest[dwPosDestNum].dwX = pPosListSrc[i].dwX;
		pPosListDest[dwPosDestNum].dwZ = pPosListSrc[i].dwZ;
		dwPosDestNum++;

lb_skip:
		__asm nop
	}
	return dwPosDestNum;
}
CTool::CTool()
{
	memset(this,0,sizeof(CTool));

	m_dwSelectedHFieldObjectIndex = 0xffffffff;
	m_dwStaticModelAlpha = 255;
	m_dwHFieldAlpha = 255;
	m_dwViewHFieldAlpha = 255;
	m_bEnableViewOnlyHField = TRUE;
	m_fHFieldVertexColorBrushSize = 200.0f;
#ifdef _DEBUG
	int	flag = _CRTDBG_ALLOC_MEM_DF |_CRTDBG_LEAK_CHECK_DF;

	_CrtSetDbgFlag(flag);
#endif

	GetModuleFileName(NULL,m_szDefaultPath,sizeof(m_szDefaultPath));
	GetNamePath(m_szDefaultPath,m_szDefaultPath);
//	GetCurrentDirectory(sizeof(m_szDefaultPath),m_szDefaultPath);	


	//trustpak 2005/06/09

	m_bRenderStaticModel = TRUE;

	///


}

void CTool::SetConsoleText(char* szStr,DWORD len)
{
	if (len >= 512)
		return;
	
	strcpy(m_szConsoleText,szStr);
	m_szConsoleText[len] = 0;
	Render();


}
void CTool::SetTileBrushMode(BOOL bSwitch)
{
	if (bSwitch)
	{
		m_bRenderPerMouseMove = TRUE;
		m_TileBrushMode = TRUE;
	}
	else
	{
		m_bRenderPerMouseMove = FALSE;
		m_TileBrushMode = FALSE;
	}

}


void CTool::Render()

{
	if (!m_pExecutive)
		return;

	if (m_pHField)
	{
		m_pHField->SetHeightFieldDetail(m_dwHFieldDetail );
	}
	
	MATRIX4	mat4;

	m_pExecutive->GetRenderer()->BeginPerformanceAnalyze();

	//m_pExecutive->GetGeometry()->BeginRender(0,0x000000ff,0);
	m_pExecutive->GetGeometry()->BeginRender(0, m_dwBackColor, 0);
	
	m_pExecutive->GetGeometry()->RenderSphere(&g_v3Point,10.0f,0xffffffff);
	m_pExecutive->Process();


	if (m_pArrow)
	{
		SetIdentityMatrix(&mat4);
		m_pExecutive->GetGeometry()->RenderModel(m_pArrow,m_dwArrowRefIndex,NULL,&mat4,0,0,255,0,0,RENDER_TYPE_SELF_ILLUMIN);
	}

	if (!m_bMapBuild)
	{


		if (m_pHField)
		{
			for (DWORD i=0; i<m_pHField->GetObjectNum(); i++)
			{
				DWORD	dwAlpha = 255;
				if (i == m_dwSelectedHFieldObjectIndex)
					m_pExecutive->GetGeometry()->RenderHField(m_pHField,i,128,0);
				else
					m_pExecutive->GetGeometry()->RenderHField(m_pHField,i,m_dwHFieldAlpha,0);
			}

		}
		
		if (m_pStaticModel)
		{
			for (DWORD i=0; i<m_pStaticModel->GetObjectNum(); i++)
			{
				m_pExecutive->GetGeometry()->RenderStaticModel(m_pStaticModel,m_pStaticModel->GetObjectIndex(i),m_dwStaticModelAlpha,0,0);
			}
		
		}		
		
		if ((m_dwEditMode & EDIT_MODE_MASK) == EDIT_MODE_BRUSH)
		{
			C3DBrush*		pBrush = g_pRightView->GetSelectedBrush();
			if (pBrush)
			{
				pBrush->Render(m_pExecutive->GetGeometry());
			}
		}
		if (m_bEnableViewOnlyHField)
			{
			if (m_pHFieldViewOnly)
			{
				for (DWORD i=0; i<m_pHFieldViewOnly->GetObjectNum(); i++)
				{
					m_pExecutive->GetGeometry()->RenderHField(m_pHFieldViewOnly,i,m_dwViewHFieldAlpha,0);
				}
			}
		}

	}
	else
	{
	/*	for (DWORD i=0; i<m_pHField->GetObjectNum(); i++)
		{
			DWORD	dwAlpha = 255;
			if (i == m_dwSelectedHFieldObjectIndex)
				m_pExecutive->GetGeometry()->RenderHField(m_pHField,i,128,0);
			else
				m_pExecutive->GetGeometry()->RenderHField(m_pHField,i,m_dwHFieldAlpha,0);
		}*/

	}
lb_skip:
	m_pExecutive->Render();
	if (m_bDrawSelectedRect)
	{
		m_pExecutive->GetRenderer()->RenderLine(m_v2PointSelectRect+0,m_v2PointSelectRect+1,0xff00ff00);
		m_pExecutive->GetRenderer()->RenderLine(m_v2PointSelectRect+1,m_v2PointSelectRect+2,0xff00ff00);
		m_pExecutive->GetRenderer()->RenderLine(m_v2PointSelectRect+2,m_v2PointSelectRect+3,0xff00ff00);
		m_pExecutive->GetRenderer()->RenderLine(m_v2PointSelectRect+3,m_v2PointSelectRect+0,0xff00ff00);
	}
	if (m_bEnableViewColSTM)
	{

		if (m_pStaticModel)
		{
			m_pStaticModel->RenderCollisionStaticModel(0);
		}
	}
	m_pExecutive->GetGeometry()->EndRender();

	m_pExecutive->GetGeometry()->BeginRender(0,0,BEGIN_RENDER_FLAG_DONOT_CLEAR_FRAMEBUFFER);
	
	if (m_dwSelectedVertexNum)
		m_pHField->RenderVertex(m_pSelectedVertexIndex,m_dwSelectedVertexNum,0xffff0000);


	if (m_dwSelectedTileNum)
	{
		HFIELD_POS		hfPos;
		hfPos.dwX = 0;
		hfPos.dwZ = 0;
		m_pHField->RenderTile(&hfPos,m_pSelectedTileIndex,m_dwSelectedTileNum,NULL,0xffff0000);
	}
	if (g_pRightView->GetSelectedTileBrush() && m_TileBrushMode)
	{
		HFIELD_POS	hfPos;
		WORD*		pwTexTileIndexList;
		HFIELD_POS*	pTileList;
		VECTOR3		v3Pos;
		float		fDist;
		if (m_pHField->GetTilePosWithScreenCoord(&v3Pos,&fDist,&hfPos.dwX,&hfPos.dwZ,&m_ptMousePos,m_dwCurrentViewportIndex))
		{
			DWORD dwTileNum = g_pRightView->GetSelectedTileBrush()->GetTileList(&pTileList,&pwTexTileIndexList);
			m_pHField->RenderTile(&hfPos,pTileList,dwTileNum,pwTexTileIndexList,0x7fffffff);
		}


		
	}

	//Added by KBS 030224
#ifdef CORUM_ONLINE
	RenderTileText();
#endif
	//--
/////////////////////////////
//	if (g_dwTriCount)
//	{
//		m_pExecutive->GetRenderer()->RenderTriVector3(g_v3TriList,g_dwTriCount,0);
//	}
/////////////////////////////


	//trustpak

	if (TRUE == m_bDisplayCameraInfo)
	{
		CAMERA_DESC camera_desc;
		m_pExecutive->GetGeometry()->GetCameraDesc(&camera_desc, 0);
		
		char szCameraInfo[0x200] = { 0, };
		
		sprintf(szCameraInfo, "Camera Pos > x : %f, y : %f, z : %f \nCamera Angle > x : %f, y : %f, z : %f \n", camera_desc.v3From.x,
																												 camera_desc.v3From.y,
																												 camera_desc.v3From.z,
																												 camera_desc.fXRot,
																												 camera_desc.fYRot,
																												 camera_desc.fZRot);

		RECT rc = { 10, 60, 400, 50 };	

		m_pFont->DrawText(szCameraInfo, 
						  strlen(szCameraInfo), 
						  &rc, 
						  0xff00ff00, 
						  CHAR_CODE_TYPE_ASCII, 
						  0);		
		
	}	


	////
	
	m_pExecutive->GetGeometry()->EndRender();

	m_pExecutive->GetRenderer()->EndPerformanceAnalyze();
	m_pExecutive->GetGeometry()->Present(NULL);
	
//	RenderConsoleText();

//	m_pExecutive->GetRenderer()->RenderPoint(&m_v3Clicked,0xffffffff);
	
	VECTOR2	v2Point;
	v2Point.x = 320;
	v2Point.y = 200;
//	m_pExecutive->GetRenderer()->RenderCircle(&v2Point,200,0xffffffff);
	
		
	m_pPalette->RenderPalette();
	UpdateStatus();
}
DWORD CTool::GetSelectedTileList(HFIELD_POS** ppTileList)
{
	*ppTileList = m_pSelectedTileIndex;
	return m_dwSelectedTileNum;
}
void CTool::RenderConsoleText()
{
/*	DWORD	len = lstrlen(m_szConsoleText);
	long width = len*40;

	RECT		rect;
	rect.left = 10;
	rect.top = 350;
	rect.right = rect.left + width;
	rect.bottom = rect.top + 40;

	m_pFont->DrawText(m_szConsoleText,len,&rect,0xffffffff,CHAR_CODE_TYPE_ASCII);
*/
}

void CTool::SetAlphaStaticModel(DWORD dwAlpha)
{
	m_dwStaticModelAlpha = dwAlpha;
	m_pExecutive->GXMSetSTMAlphaFlag(dwAlpha);
}

void CTool::SetAlphaHField(DWORD dwAlpha)
{
	m_dwHFieldAlpha = dwAlpha;
	m_pExecutive->GXMSetHFieldAlphaFlag(dwAlpha);
}

void CTool::SetAlphaViewOnlyHField(DWORD dwAlpha)
{
	m_dwViewHFieldAlpha = dwAlpha;
}

void CTool::DisableRenderHFObj()
{
	if (m_pHField)
	{
		m_pHField->DisableRender(m_dwSelectedHFieldObjectIndex);
		Render();
	}
}
void CTool::EnableRenderHFObj()
{
	if (m_pHField)
	{
		m_pHField->EnableRender(m_dwSelectedHFieldObjectIndex);
		Render();
	}
}

void CTool::FreezeHFObjDetail(DWORD dwDetailLevel)
{
	if (m_pHField)
	{

		m_pHField->FreezeDetailLevel(m_dwSelectedHFieldObjectIndex,dwDetailLevel);
		Render();
	}
}
void CTool::UnfreezeHFObjDetail()
{
	if (m_pHField)
	{
		m_pHField->UnFreezeDetailLevel(m_dwSelectedHFieldObjectIndex);
		Render();
	}
}

void CTool::SetHFieldCreateDesc(HFIELD_CREATE_DESC*	phfCreate)
{

	if (m_pHField)
		return;
	
	m_hfcreate = *phfCreate;

}
BOOL CTool::CreateNewHeightField()
{	
	if (m_ghMap)
	{
		m_pExecutive->DeleteGXMap(m_ghMap);
		m_ghMap = NULL;
	}
	CleanHeightField();

	CDialogHFieldProperty*		pHFieldProperty = new CDialogHFieldProperty;
	pHFieldProperty->DoModal();

	delete pHFieldProperty;
	pHFieldProperty = NULL;


	m_pPalette->GetTextureTable(&m_hfcreate.pTexTable,&m_hfcreate.dwTileTextureNum);
	
	if (!m_hfcreate.dwTileTextureNum)
	{
		::MessageBox(NULL,"타일 팔레트를 먼저 생성해주세요","Error",MB_OK);
		return FALSE;
	}
	m_pExecutive->GetGeometry()->CreateHeightField(&m_pHField,0);
	m_pHField->Create(&m_hfcreate,HFIELD_MODEL_LOAD_ENABLE_SHADE);
	
//	for (DWORD i=0; i<4; i++)
//		ResetDefaultCamera(i);

	HFIELD_DESC	hfdesc;

//	m_pExecutive->GetRenderer()->SetLightMapFlag(0);
	m_pHField->GetHFieldDesc(&hfdesc);

	CreateHeightField(&hfdesc);
	m_bIsHFieldCreateMode = TRUE;

	Render();
	
	return TRUE;

}
void CTool::UpdateTilePalette()
{
	if (m_pHField)
	{
		m_pPalette->GetTextureTable(&m_hfcreate.pTexTable,&m_hfcreate.dwTileTextureNum);
		m_pHField->LoadTilePalette(m_hfcreate.pTexTable,m_hfcreate.dwTileTextureNum);
	}
}
void CTool::CleanStaticModel()
{
	if (m_pStaticModel)
	{
		m_pStaticModel->Release();
		m_pStaticModel = NULL;
	}
	m_bMapBuild = FALSE;
}
BOOL CTool::CreateNewStaticModel()
{
	
	if (m_ghMap)
	{
		m_pExecutive->DeleteGXMap(m_ghMap);
		m_ghMap = NULL;
	}
	CleanStaticModel();

	m_pExecutive->GetGeometry()->CreateStaticModel(&m_pStaticModel,4000,500,0);
	if (!m_pStaticModel)
		return FALSE;

	m_pStaticModel->BeginCreateMesh(0);

	return TRUE;
}
BOOL CTool::Initialize(CWnd* pWnd)
{
	CoInitialize(NULL);

	m_pEditWindow = pWnd;
	
	HRESULT hr;

	// Excutive생성
	hr = CoCreateInstance(
           CLSID_4DyuchiGXExecutive,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_4DyuchiGXExecutive,
           (void**)&m_pExecutive);

	if (hr != S_OK)
		__asm int 3

	m_pExecutive->InitializeFileStorage(0,4096,_MAX_PATH,FILE_ACCESS_METHOD_ONLY_FILE,NULL,0);
	m_pExecutive->GetFileStorage(&m_pFileStorage);
	m_pExecutive->Initialize(pWnd->m_hWnd,NULL,8192,512,512,32,32,NULL);
	
//	m_pExecutive->Initialize(g_pMainFrame->m_hWnd,NULL,4096,512,512);
	g_pMainFrame->DockControl();
	
	m_pPalette = new CDialogTilePalette(pWnd);

	
	//yh
	m_pTileSetDialog = new CDialogTileSet;
	m_pEditTileSetDialog = new CDialogEditTileSet;
	m_pEditEntryDialog = new CDialogEditEntry;
	m_pTileSetDialog->Initialize(m_pPalette,m_pEditTileSetDialog);
	m_pTileSetDialog->DisableView();
	m_pEditTileSetDialog->Initialize(m_pTileSetDialog,m_pEditEntryDialog);
	m_pEditTileSetDialog->DisableView();
	m_pEditEntryDialog->Initialize(m_pEditTileSetDialog,m_pPalette);
	m_pEditEntryDialog->DisableView();
	//

	//trustpak

	m_pLoadFailedTextureDialog = new CLoadFailedTextureDialog;

	///

	
	//m_pPalette->Initialize(64,m_pEditEntryDialog);

	//trustpak
	m_pPalette->Initialize(64,m_pEditEntryDialog, m_pTileSetDialog);

	//
	m_pPalette->DisableView();


	SHORT_RECT	rect;
	rect.top = 100;
	rect.left = 100;
	rect.right = 700;
	rect.bottom = 600;
	
	

	m_pExecutive->GetGeometry()->SetViewport(NULL,0);
	
	m_pFont = m_pExecutive->GetRenderer()->CreateFontObject(NULL,D3DX_FONT);
	m_Mouse.Initialize(m_pExecutive->GetGeometry());

	if (0xffffffff == m_pExecutive->GetGeometry()->LoadModel(&m_pArrow,"arrow.mod",0))
		::MessageBox(NULL,"Arrow.mod Not found","Error",NULL);

	if (m_pArrow)
	{
		m_pArrow->Initialize(m_pExecutive->GetRenderer(),0);
		m_dwArrowRefIndex = m_pArrow->Reference();
		m_pArrow->SetZOrder(9,m_dwArrowRefIndex);
	}

	m_pExecutive->SetSymbol(SYMBOL_TYPE_LIGHT,"light_icon.mod");
	ResetDefaultCamera(0);

	CreateNewStaticModel();


	//trustpak 2005/06/09
	m_pExecutive->SetPickingMode(PICKING_MODE_TOOL);
	//m_pExecutive->SetRenderMode(RENDER_MODE_TOOL);

	SetRenderToolMode(RENDER_MODE_TOOL);

	///
	

	m_pHash = VBHCreate();
	VBHInitialize(m_pHash,32,sizeof(DWORD),8192);
	
	m_dwMaxSelectedVertexNum = 8192;
	m_pSelectedVertexIndex = new HFIELD_POS[m_dwMaxSelectedVertexNum];


	m_dwMaxSelectedTileNum = 4096;
	m_pSelectedTileIndex = new HFIELD_POS[m_dwMaxSelectedTileNum];


	m_pExecutive->GetRenderer()->SetLightMapFlag(LIGHTMAP_FLAG_ENABLE);

	//In
	m_pSectionSplitter = new SectionSplitter;
	m_pNpcInfoList = new CNpcInfoList;
	m_pSpotSplitter = new SectionSplitter;
	m_pStartSplitter = new SectionSplitter;	
	//


	//trustpak
	memset(m_szCurrentMapFileName, 0, sizeof(m_szCurrentMapFileName));
	strncpy(m_szCurrentMapFileName, DEFAULT_SS3D_MAP_FILE_NAME, sizeof(m_szCurrentMapFileName));

	memset(m_szCurrentTilePaletteFileName, 0, sizeof(m_szCurrentTilePaletteFileName));
	memset(m_szCurrentTileSetFileName, 0, sizeof(m_szCurrentTileSetFileName));

	memset(m_aLoadFailedTextureTable, 0, sizeof(m_aLoadFailedTextureTable));
	m_pExecutive->GetRenderer()->SetLoadFailedTextureTable(m_aLoadFailedTextureTable, LOAD_FAILED_TEXTURE_TABLE_SIZE);
	//


	m_pExecutive->GetRenderer()->SetAlphaRefValue(5);

	return TRUE;
}
void CTool::CreateBrushArea()
{

	if (m_gxhLightPrj)
	{
		m_pExecutive->DeleteGXLight(m_gxhLightPrj);
		m_gxhLightPrj = NULL;
	}
	// 하이트필드에 브러쉬 영역을 표시할 이미지 라이트 생성

	LIGHT_DESC	imagelight;
	memset(&imagelight,0,sizeof(imagelight));

	imagelight.dwDiffuse = 0xffffffff;

	// 일반적인 마법진처럼 표현하려면 수직으로 투영해야한다.
	// 따라서 from, to좌표는 높이만 다르다.
	m_fLightPrjHeight = 400.0f;

	imagelight.v3Point.x = 0.0f;		
	imagelight.v3Point.y = m_fLightPrjHeight;
	imagelight.v3Point.z = 0.0f;
	imagelight.v3To.x = imagelight.v3Point.x;
	imagelight.v3To.y = 0.0f;
	imagelight.v3To.z = imagelight.v3Point.z;

	// 라이트 뷰볼륨의 fov.좁히면 텍스쳐 이미지가 작게, 넓히면 크게 투영되는 효과를 얻는다.
	// 투영될 텍스쳐 이미지의 크기는 fov외에도 라이트의 높이로 조절할수 있다.
	imagelight.fFov = PI/2.0f;

	// 이와같이 수직으로 투영되는 라이트인 경우는 up벡터는 다음과 같이 설정한다.
	imagelight.v3Up.x = 1.0f;
	imagelight.v3Up.y = 0.0f;
	imagelight.v3Up.z = 0.0f;

	// 라이트의 범위.
	imagelight.fRs = m_fLightPrjHeight;
	imagelight.fWidth = 200.0f;
	imagelight.fHeight = 200.0f;
	

	// 외부에 텍스쳐를 직접적으로 다룰순 없다.이와같이 매터리얼을 설정해서 사용한다.


	char dirOld[_MAX_PATH];
	GetCurrentDirectory(sizeof(dirOld),dirOld);

	SetCurrentDirectory(m_szDefaultPath);


	MATERIAL	mtl;
	memset( &mtl, 0, sizeof(mtl));
	mtl.dwDiffuse	=	0xffffffff;
	mtl.dwAmbient	=	0xffffffff;

	
	
	lstrcpy( mtl.szDiffuseTexmapFileName,"brush.tga");
	
	// 라이트를 생성한다.
	m_gxhLightPrj = m_pExecutive->CreateGXLight(&imagelight,NULL,NULL,
		0x00,		// ADD로 라이트를 뿌린다.텍스쳐는 투영될 위치에 더해진 색으로 렌더링된다.0x00인 경우 곱하기가 된다.즉 더 어두워진다.
		&mtl,		// 투영될 텍스쳐를 포함하는 매터리얼 디스크립터 
	//	GXLIGHT_TYPE_ENABLE_SHADOW |
		GXLIGHT_TYPE_ENABLE_IMAGE_PROJECTION | // 텍스쳐 이미지를 프로젝션하는 라이트임을 명시.
		GXLIGHT_TYPE_ENABLE_DYNAMIC_LIGHT |
		GXLIGHT_TYPE_DISABLE_LIGHT_COLOR |
		GXLIGHT_TYPE_ONLY_USE_TOOL	|
		GXLIGHT_TYPE_DISABLE_NOT_RENDER_MODEL_IN_TOOL);


	VECTOR3		v3Pos;
	v3Pos.x		=	0.0f;
	v3Pos.y		=	0.0f;
	v3Pos.z		=	0.0f;
	
 	m_pExecutive->GXLSetPosition( m_gxhLightPrj, &v3Pos);		// 초기 위치를 설정.


	SetCurrentDirectory(dirOld);

}
void CTool::ResetDefaultCamera(DWORD dwViewportIndex)
{

	VECTOR3		v3From;
	v3From.x	=	v3From.y	=	v3From.z	=	0.0f;
	VECTOR3		v3Rotate;
	v3Rotate.x	=	v3Rotate.y	=	v3Rotate.z	=	0.0f;
	this->ResetCamera(dwViewportIndex);
	// 뷰 타입의 기본값들을 미리 저장.
//	VIEW_TYPE_FREE
		m_Mouse.PushCameraAngleAndPos( VIEW_TYPE_FREE);

//	VIEW_TYPE_TOP
		this->ResetCamera(dwViewportIndex);
		v3Rotate.x	=	-90.0f * (PI / 180.0f);		// 돌리는 각도로...
		v3Rotate.y	=	0.0f;
		v3Rotate.z	=	0.0f;
		m_pExecutive->GetGeometry()->SetCameraAngleRad( &v3Rotate,dwViewportIndex);
		v3From.y		=	4000.0f;
		m_pExecutive->GetGeometry()->MoveCamera( &v3From,dwViewportIndex);
		m_Mouse.PushCameraAngleAndPos( VIEW_TYPE_TOP);

//	VIEW_TYPE_BOTTOM
		this->ResetCamera(dwViewportIndex);
		v3Rotate.x	=	90.0f  * (PI / 180.0f);		// 돌리는 각도로...
		v3Rotate.y	=	0.0f;
		v3Rotate.z	=	0.0f;
		m_pExecutive->GetGeometry()->SetCameraAngleRad( &v3Rotate,dwViewportIndex);
		v3From.y		=	-4000.0f;
		m_pExecutive->GetGeometry()->MoveCamera( &v3From,dwViewportIndex);
		m_Mouse.PushCameraAngleAndPos( VIEW_TYPE_BOTTOM);

//	VIEW_TYPE_LEFT
		this->ResetCamera(dwViewportIndex);
		v3Rotate.x	=	0.0f;		// 돌리는 각도로...
		v3Rotate.y	=	-90.0f * (PI / 180.0f);
		v3Rotate.z	=	0.0f;
		m_pExecutive->GetGeometry()->SetCameraAngleRad( &v3Rotate,dwViewportIndex);
		v3From.y		=	0.0f;
		v3From.x		=	4000.0f;
		m_pExecutive->GetGeometry()->MoveCamera( &v3From,dwViewportIndex);
		m_Mouse.PushCameraAngleAndPos( VIEW_TYPE_LEFT);

//	VIEW_TYPE_RIGHT
		this->ResetCamera(dwViewportIndex);
		v3Rotate.x	=	0.0f;		// 돌리는 각도로...
		v3Rotate.y	=	90.0f * (PI / 180.0f);
		v3Rotate.z	=	0.0f;
		m_pExecutive->GetGeometry()->SetCameraAngleRad( &v3Rotate,dwViewportIndex);
		v3From.x		=	-4000.0f;
		m_pExecutive->GetGeometry()->MoveCamera( &v3From,dwViewportIndex);
		m_Mouse.PushCameraAngleAndPos( VIEW_TYPE_RIGHT);

//	VIEW_TYPE_FRONT
		this->ResetCamera(dwViewportIndex);
		v3Rotate.x	=	0.0f;		// 돌리는 각도로...
		v3Rotate.y	=	0.0f;
		v3Rotate.z	=	0.0f;
		m_pExecutive->GetGeometry()->SetCameraAngleRad( &v3Rotate,dwViewportIndex);
		v3From.x		=	0.0f;
		v3From.z		=	-4000.0f;
		m_pExecutive->GetGeometry()->MoveCamera( &v3From,dwViewportIndex);
		m_Mouse.PushCameraAngleAndPos( VIEW_TYPE_FRONT);

//	VIEW_TYPE_BACK
		this->ResetCamera(dwViewportIndex);
		v3Rotate.x	=	0.0f;		// 돌리는 각도로...
		v3Rotate.y	=	180.0f * (PI / 180.0f);
		v3Rotate.z	=	0.0f;
		m_pExecutive->GetGeometry()->SetCameraAngleRad( &v3Rotate,dwViewportIndex);
		v3From.z		=	4000.0f;
		m_pExecutive->GetGeometry()->MoveCamera( &v3From,dwViewportIndex);
		m_Mouse.PushCameraAngleAndPos( VIEW_TYPE_BACK);

// 마무리, VIEW_TYPE_FREE를 기본값으로 하고 카메라 설정.
	m_Mouse.PopCameraAngleAndPos( VIEW_TYPE_FREE);
	m_CurrentViewMode	=	VIEW_TYPE_FREE;

}

BOOL CTool::ExportHeightField(char* szFileName)
{
	if (!m_pHField)
	{
		MessageBox(NULL,"생성된 하이트필드가 없습니다.","Error",MB_OK);
		return FALSE;
	}

	m_pHField->WriteFile(szFileName,0);
		
	return TRUE;
}

// Rect 와 Rect 가 겹쳤는지를 판단하는 함수.. 일단 여기다가 걍 정의.. 
BOOL RectInRect( LPRECT src, LPRECT target )
{
	if((src->left <= target->right) &&
    (src->right >= target->left) &&
    (src->top <= target->bottom) &&
    (src->bottom >= target->top))
      return TRUE;
 else
      return FALSE;
}

BOOL CTool::ExportTileTable(char* szFileName)
{
	if (!m_pHField)
	{
		MessageBox(NULL,"생성된 하이트필드가 없습니다.","Error",MB_OK);
		return FALSE;
	}
	FILE*	fp = fopen(szFileName,"wb");
	if (!fp)
		return FALSE;

	HFIELD_DESC	hfDesc;
	m_pHField->GetHFieldDesc(&hfDesc);
	
	// 저장 시작...
	fwrite(&hfDesc.dwTileNumX ,sizeof(DWORD),1,fp);	// x길이 저장.
	fwrite(&hfDesc.dwTileNumZ ,sizeof(DWORD),1,fp);	// z길이 저장.
	fwrite(hfDesc.pwTileTable,sizeof(WORD),hfDesc.dwTileNumX*hfDesc.dwTileNumZ,fp);	// 타일 정보 저장.
	
	fclose(fp);

	return TRUE;
}
BOOL CTool::ImportHeightField(char* szFileName,DWORD dwIndexBufferNum)
{
	
	TEXTURE_TABLE*	pTexTable = NULL;
	BOOL			bResult = FALSE;
	
	CleanHeightField();

	bResult = m_pExecutive->GetGeometry()->CreateHeightField(&m_pHField,0);

	if (!m_pHField->ReadFile(szFileName,dwIndexBufferNum,NULL,HFIELD_MODEL_LOAD_ENABLE_DRAW_ALPHAMAP | HFIELD_MODEL_LOAD_ENABLE_SHADE))
	{
		m_pHField->Release();
		m_pHField = NULL;
		memset(m_szHFieldName,0,sizeof(m_szHFieldName));
		return FALSE;
	}
	lstrcpy(m_szHFieldName,szFileName);


	m_pHField->GetHFieldDesc(&m_hfDesc);
	
	m_pPalette->CleanTextureTable();
	m_pPalette->CreatePalette(m_hfDesc.dwTileTextureNum);
	m_pPalette->SetPalette(m_hfDesc.pTexTable,m_hfDesc.dwTileTextureNum);

//	for (DWORD i=0; i<4; i++)
//		ResetDefaultCamera(i);

//	m_pExecutive->GetRenderer()->SetLightMapFlag(0);

	int sum;
	sum=strlen(szFileName);
		szFileName[sum-1]='l';
	szFileName[sum-2]='a';
	szFileName[sum-3]='p';
	FILE *fp=fopen(szFileName,"wb");
	if (!fp)
		return 0;

	fprintf(fp,"%s       %d\r\n","TILE_NUM",m_hfDesc.dwTileTextureNum);
	for (int i=0;i<m_hfDesc.dwTileTextureNum;i++)
	{
		fprintf(fp,"%d        %s\r\n",m_hfDesc.pTexTable[i].wIndex,m_hfDesc.pTexTable[i].
			szTextureName);
	}

	fclose(fp);
	Render();

	return bResult;

}
BOOL CTool::ImportViewOnlyHeightField(char* szFileName)
{
	TEXTURE_TABLE*	pTexTable = NULL;
	BOOL			bResult = FALSE;
	
	if (m_pHFieldViewOnly)
	{
		m_pHFieldViewOnly->Release();
		m_pHFieldViewOnly = NULL;
	}

	bResult = m_pExecutive->GetGeometry()->CreateHeightField(&m_pHFieldViewOnly,0);

	if (!m_pHFieldViewOnly->ReadFile(szFileName,0,NULL,0))
	{
		m_pHFieldViewOnly->Release();
		m_pHFieldViewOnly = NULL;
		return FALSE;
	}
	
	m_pHFieldViewOnly->GetHFieldDesc(&m_hfDesc);
	
	Render();

	return bResult;

}
void CTool::ChangeHFieldIndexBufferNum(DWORD dwNum)
{
	ImportHeightField(m_szHFieldName,dwNum);
}
void CTool::UpdateStatus()
{
	char	str[128];
	
	HWND	hDlg = g_pMainFrame->GetStatusBarWnd();
	
	SYSTEM_STATUS	status;
	GetGeometry()->GetSystemStatus(&status);

	itoa(status.dwTotalTexMem,str,10);
	SetDlgItemText(hDlg,IDC_STATIC_TOTALTEXMEM,str);

	itoa(status.dwAvaliableTexMem,str,10);
	SetDlgItemText(hDlg,IDC_STATIC_AVTEXMEM,str);


	DWORD	dwAlphaMapNum,dwAlphaMapMemSize;
	if (m_pHField)
	{
		m_pHField->GetAlphaMapResourceStatus(&dwAlphaMapNum,&dwAlphaMapMemSize);
	}
	else
	{
		dwAlphaMapNum = 0;
		dwAlphaMapMemSize = 0;
	}
	itoa(dwAlphaMapNum,str,10);
	SetDlgItemText(hDlg,IDC_STATIC_ALPHAMAP_NUM,str);

	itoa(dwAlphaMapMemSize,str,10);
	SetDlgItemText(hDlg,IDC_STATIC_ALPHAMAP_MEM_SIZE,str);


}
BOOL CTool::SampleFromMod(char* szFileName)
{
	I3DModel* pModel;
	
	if (!m_pHField)
		return FALSE;

	if (0xffffffff == m_pExecutive->GetGeometry()->LoadModel(&pModel,szFileName,0))
		return FALSE;


	pModel->Initialize(NULL,MODEL_INITIALIZE_FLAG_NOT_OPTIMIZE);
	DWORD dwRefIndex = pModel->Reference();

	m_pHField->SetYFactorFromModel(pModel,dwRefIndex,100.0f,-10000.0f,80000.0f);

	pModel->UnReference(dwRefIndex);
	pModel->Release();
	
	

	Render();

	return TRUE;
}
BOOL CTool::CreateStaticModelFromMod(char* szFileName)
{


	I3DModel* pSrcModel;

	if (0xffffffff == m_pExecutive->GetGeometry()->LoadModel(&pSrcModel,szFileName,0))
		return FALSE;

	pSrcModel->Initialize(m_pExecutive->GetRenderer(),MODEL_INITIALIZE_FLAG_NOT_OPTIMIZE);
	m_dwLightNum = pSrcModel->GetLightList(m_lightDesc,MAX_LIGHT_NUM);
	m_dwCameraNum = pSrcModel->GetCameraList(m_cameraDesc,MAX_CAMERA_NUM);

	CleanStaticModel();
	m_pExecutive->GetGeometry()->CreateStaticModel(&m_pStaticModel,4096,512,0);

	if (!m_pStaticModel)
		return FALSE;
	
	m_pStaticModel->BeginCreateMesh(0);

	DWORD	dwRefIndex = pSrcModel->Reference();
	m_pStaticModel->AddModel(pSrcModel,dwRefIndex,NULL,0);
	
//	m_pStaticModel->CommitDevice(0);
//	m_pStaticModel->EndCreateMesh();

	pSrcModel->UnReference(dwRefIndex);
	pSrcModel->Release();
//	if (m_dwCameraNum)
//		m_pExecutive->GetRenderer()->SetCamera(&m_cameraDesc[0].v3From,&m_cameraDesc[0].v3To,&up);

//	m_ghMap = m_pExecutive->CreateGXMap(m_pStaticModel,NULL,0,NULL,8192);

	Render();

	return TRUE;
}
BOOL CTool::ExportStaticModel(char* szFileName)
{
	if (!m_pStaticModel)
		return FALSE;
	

	m_pStaticModel->WriteFile(szFileName);


/*
	char	_STM[] = ".stm";
	char	stmFileName[MAX_NAME_LEN];

	DWORD	len = strlen(szFileName)-4;
	memcpy(stmFileName,szFileName,len);
	strcpy(stmFileName+len,_STM);

	m_pStaticModel->WriteFile(stmFileName);*/
	return TRUE;
}

BOOL CTool::ImportStaticModel(char* szFileName)
{
	I3DStaticModel*	pStaticModel = NULL;
	
	if (0xffffffff == m_pExecutive->GetGeometry()->CreateStaticModel(&pStaticModel,4000,500,0))
		return FALSE;


	if (!pStaticModel->ReadFile(szFileName,NULL,STATIC_MODEL_LOAD_ENABLE_SHADE))
	{
		pStaticModel->Release();
		return FALSE;
	}

	if (m_ghMap)
	{
		m_pExecutive->DeleteGXMap(m_ghMap);
		m_ghMap = NULL;
	}

	CleanStaticModel();

	m_pStaticModel = pStaticModel;
	m_pStaticModel->BeginCreateMesh(0);

	Render();

	return TRUE;
}

BOOL CTool::BuildMap(MAABB* pWorldBox)
{
	if (m_bMapBuild)
	{
//		m_pExecutive->RebuildMap(m_WorldBox.Max.y, m_WorldBox.Min.y);
		m_pExecutive->RebuildMap(pWorldBox->Max.y, pWorldBox->Min.y);
		m_WorldBox	=	*pWorldBox;
		return TRUE;

	}
	if( (pWorldBox->Max.y - pWorldBox->Min.y) <= 0.0f)
	{
		pWorldBox->Max.y	=	pWorldBox->Min.y	+	100.0f;
	}
	m_WorldBox.Max.y	=	pWorldBox->Max.y;
	m_WorldBox.Min.y	=	pWorldBox->Min.y;

//	if (m_ghMap)
//		m_pExecutive->DeleteGXMap(m_ghMap);

	m_ghMap = m_pExecutive->CreateGXMap(NULL,0,NULL);

	m_pExecutive->BeginBuildMap(0);

	if (m_pStaticModel)
		m_pExecutive->InsertStaticModelTOGXMap(m_pStaticModel);
	
	if (m_pHField)
		m_pExecutive->InsertHFieldToGXMap(m_pHField);

	m_pExecutive->EndBuildMap(pWorldBox->Max.y,pWorldBox->Min.z);

	m_pExecutive->GetWorldBoundingBox( &m_WorldBox);

	m_bMapBuild = TRUE;
	CreateBrushArea();

	return TRUE;
}
BOOL CTool::ShadeLightMap(DWORD dwAmbientColor,DWORD dwFlag)
{

	if (m_dwLightNum)
	{
		m_pExecutive->GXMStaticShadeMap(dwAmbientColor,m_lightDesc,m_dwLightNum,dwFlag,ShadeFactor);
	//	m_pExecutive->GXMStaticShadeMap(0xff0f0f0f,0,NULL,0);
		Render();
	}
	else
	{
		LIGHT_DESC	light;
		light.v3Point.x = 0.0f;
		light.v3Point.y = 1000.0f;
		light.v3Point.z = 0.0f;
		light.fRs = 10000.0f;
		light.dwDiffuse = 0xffff0000;
		
		m_pExecutive->GXMStaticShadeMap(dwAmbientColor,&light,1,dwFlag,ShadeFactor);
//		m_pExecutive->GXMStaticShadeMap(0xff0f0f0f,0,NULL,0);
//		Render();
	}
	
	return TRUE;
}

void CTool::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_ptMousePos.x = point.x;
	m_ptMousePos.y = point.y;

	m_dwMouseStatus = MOVE;

	//In
	m_pSectionSplitter->mousePos = (POINT)point;
	m_pSpotSplitter->mousePos = (POINT)point;
	m_pStartSplitter->mousePos = (POINT)point;	
	//
	
	// 셀렉트 버튼 눌렀을 경우..
	if (m_bSelectMode)
	{
		if( nFlags & MK_LBUTTON)
		{
			
			m_v2PointSelectRect[0].x = m_ptMousePrvDown.x;
			m_v2PointSelectRect[0].y = m_ptMousePrvDown.y;

			m_v2PointSelectRect[1].x = m_ptMousePrvDown.x;
			m_v2PointSelectRect[1].y = point.y;

			m_v2PointSelectRect[2].x = point.x;
			m_v2PointSelectRect[2].y = point.y;

			m_v2PointSelectRect[3].x = point.x;
			m_v2PointSelectRect[3].y = m_ptMousePrvDown.y;

			if ((m_v2PointSelectRect[3].x - m_v2PointSelectRect[0].x) == 0 && 
				(m_v2PointSelectRect[1].y - m_v2PointSelectRect[0].y) == 0)
			{
				m_bDrawSelectedRect = FALSE;
			}
			else 
				m_bDrawSelectedRect = TRUE;

			Render();
			goto lb_return;
		}
		
	}


	// 카메라 이동..
	BOOL		bDirty;
	bDirty = FALSE;
	if(m_Mouse.MoveMouse( nFlags, point.x, point.y))
	{
		Render();
		goto lb_return;
	}

	switch(m_dwEditMode & EDIT_MODE_MASK)
	{
	case EDIT_MODE_VERTEX:
		if( nFlags & MK_LBUTTON)
		{
		}
		break;

		//yh
	case EDIT_MODE_TILESET:
		{
			if( nFlags & MK_LBUTTON)
			{
				OnClickTileSet((POINT*)&point,nFlags);
			}
		}
		break;
		//

	case EDIT_MODE_TILE:
		if( nFlags & MK_LBUTTON)
		{
			//In
			//Modified by KBS
			if(m_pSectionSplitter->bSectionInputMode)
				OnSectionSplitterMove( nFlags, (POINT*)&point );
			else if(m_pSpotSplitter->bSectionInputMode)
				OnSpotSplitterMove( nFlags, (POINT*)&point );
			else if(m_pStartSplitter->bSectionInputMode)		
				OnStartSpotSplitterMove( nFlags, (POINT*)&point );
			else
				OnClickTile((POINT*)&point,nFlags);
			//--
		}
		
		break;

	case EDIT_MODE_DRAW_LIGHTMAP:
		if( nFlags & MK_LBUTTON)
		{
			OnClickLightMap((POINT*)&point,nFlags);
		}
		break;
	case EDIT_MODE_DRAW_VERTEXCOLOR:
		{
			if( nFlags & MK_LBUTTON)
			{
				BOOL	bResult;
				VECTOR3	v3Pos;
				float	fDist;
	
				bResult = m_pExecutive->GXMGetHFieldCollisionPointWithScreenCoord(&v3Pos,&fDist,(POINT*)&point);
				if (bResult)
				{
					DrawBrushArea(&v3Pos);
					OnClickHFieldVertexColor((POINT*)&point,&v3Pos,nFlags);
						
				}
				Render();
			}
		}
		break;
	case EDIT_MODE_DRAW_HFIELD_ALPHAMAP:
		{
			if( nFlags & MK_LBUTTON)
			{
				BOOL	bResult;
				VECTOR3	v3Pos;
				float	fDist;

				bResult = m_pExecutive->GXMGetHFieldCollisionPointWithScreenCoord(&v3Pos,&fDist,&point);
				if (bResult)
				{
					if (OnClickHFieldAlphaTexel(&point,&v3Pos,nFlags))
						Render();
				}
			}
		}
		break;
	case EDIT_MODE_BRUSH:
	//	bDirty = OnMouseMoveBrushMode(nFlags,point);
		if( nFlags & MK_LBUTTON)
		{
			if (m_pBrush)
			{
				if ( m_dwEditMode & EDIT_TYPE_MOVE)
				{
					VECTOR3	v3Pos;
					if (GetMovePos(&v3Pos,m_pBrush->GetPos(),(POINT*)&point))
					{
						*m_pBrush->GetPos() = v3Pos;
						Render();
						goto lb_return;
					}
				}
				else if ( m_dwEditMode & EDIT_TYPE_ROT)
				{
					GetRot(NULL,m_pBrush->GetRot(),&point);
					Render();
					goto lb_return;
				}
				else if ( m_dwEditMode & EDIT_TYPE_SCALE)
				{
					GetScale(m_pBrush->GetScale(),m_pBrush->GetPos(),&point);
					Render();
					goto lb_return;
				}
			}
			
		}
		break;

	case EDIT_MODE_GXOBJECT:
		if( nFlags & MK_LBUTTON)
		{
			if (m_gxoSelected)
			{
				if ( m_dwEditMode & EDIT_TYPE_MOVE)
				{
					//trustpak 2005/06/09

					if (TRUE == m_bObjectCopy)
					{
						char* pFileName = NULL;
						pFileName = (char*)m_pExecutive->GetData(m_gxoSelected);

						if (NULL != pFileName)
						{
							VECTOR3 v3Pos = { 0.0f, 0.0f, 0.0f };							
							VECTOR3 v3Scale = { 0.0f, 0.0f, 0.0f };
							VECTOR3 v3Axis = { 0.0f, 0.0f, 0.0f };
							FLOAT	fRot = 0.0f;

							m_pExecutive->GXOGetPosition(m_gxoSelected, &v3Pos);							
							m_pExecutive->GXOGetScale(m_gxoSelected, &v3Scale);
							m_pExecutive->GXOGetDirection(m_gxoSelected, &v3Axis, &fRot);

							DWORD dwFlag = 0;
							GXOBJECT_HANDLE gxoNew = m_pExecutive->CreateGXObject(pFileName, NULL, NULL, dwFlag);

							if (NULL != gxoNew)
							{
								// 속성을 그대로 copy.
								m_pExecutive->GXOSetPosition(gxoNew, &v3Pos, FALSE);
								m_pExecutive->GXOSetScale(gxoNew, &v3Scale);
								m_pExecutive->GXOSetDirection(gxoNew, &v3Axis, fRot);

								Render();

								char* pszNewFileName = new char[strlen(pFileName) +1];
								memset(pszNewFileName, 0, strlen(pFileName) +1);
								strncpy(pszNewFileName, pFileName, strlen(pFileName));

								m_pExecutive->SetData(gxoNew, (void*)pszNewFileName);

								m_gxoSelected = gxoNew;

							}
						}

						m_bObjectCopy = FALSE;
					}

					////

					VECTOR3	v3ObjPos;
					m_pExecutive->GXOGetPosition(m_gxoSelected,&v3ObjPos);
	
					VECTOR3	v3Pos;
					if (GetMovePos(&v3Pos,&v3ObjPos,(POINT*)&point))
					{
						m_pExecutive->GXOSetPosition(m_gxoSelected,&v3Pos,FALSE);
						Render();
						goto lb_return;
					}
				}
				else if ( m_dwEditMode & EDIT_TYPE_ROT)
				{
					float		fRad;
					VECTOR3		v3Rot;
					GetRot(&fRad,&v3Rot,&point);
					m_pExecutive->GXOChangeDirection(m_gxoSelected,fRad);
//					GetRot(NULL,&v3Rot,&point);
					Render();
					goto lb_return;
				}
				else if ( m_dwEditMode & EDIT_TYPE_SCALE)
				{
					VECTOR3		v3ObjScale;
					m_pExecutive->GXOGetScale(m_gxoSelected,&v3ObjScale);

					VECTOR3		v3ObjPos;
					m_pExecutive->GXOGetPosition(m_gxoSelected,&v3ObjPos);

					GetScale(&v3ObjScale,&v3ObjPos,&point);

					m_pExecutive->GXOSetScale(m_gxoSelected,&v3ObjScale);
					Render();
					goto lb_return;
				}
			}
		}
		break;

	case EDIT_MODE_LIGHT:
		if (nFlags & MK_LBUTTON)
		{
			if (m_gxlSelected)
			{
				if (m_bLightCopy)
				{
					LIGHT_DESC	light;
					m_pExecutive->GXLGetLightDesc(m_gxlSelected,&light);
					DWORD dwFlag = m_pExecutive->GetPropertyFlag(m_gxlSelected);
					GXLIGHT_HANDLE gxl = m_pExecutive->CreateGXLight(&light,NULL,0,0,NULL,GXLIGHT_TYPE_STATIC);
					if (gxl)
					{
						m_gxlSelected = gxl;
						m_pExecutive->SetPropertyFlag(m_gxlSelected,dwFlag);

					}
					m_bLightCopy = FALSE;
				}
				VECTOR3	v3LightPos;
				m_pExecutive->GXLGetPosition(m_gxlSelected,&v3LightPos);
				
				if (GetMovePos(&v3LightPos,&v3LightPos,&point))
				{
					m_pExecutive->GXLSetPosition(m_gxlSelected,&v3LightPos);
					Render();
					goto lb_return;
				}
				
			}
		}
		break;
	case EDIT_MODE_TRIGGER:
		if (m_gxtSelected)
		{
			EVENT_TRIGGER_DESC evDesc;
			m_pExecutive->GXTGetEventTriggerDesc(m_gxtSelected,&evDesc);

			if ( m_dwEditMode & EDIT_TYPE_MOVE)
			{
				if (GetMovePos(&evDesc.v3Pos,&evDesc.v3Pos,(POINT*)&point))
				{
					m_pExecutive->GXTSetPosition(m_gxtSelected,&evDesc.v3Pos);
					Render();
					goto lb_return;
				}
			}
			else if ( m_dwEditMode & EDIT_TYPE_ROT)
			{
				GetRot(NULL,&evDesc.v3Rot,&point);
				m_pExecutive->GXTSetRotation(m_gxtSelected,&evDesc.v3Rot);
				Render();
				goto lb_return;
			}
			else if ( m_dwEditMode & EDIT_TYPE_SCALE)
			{
				GetScale(&evDesc.v3Scale,&evDesc.v3Pos,&point);
				m_pExecutive->GXTSetScale(m_gxtSelected,&evDesc.v3Scale);
				Render();
				goto lb_return;
			}
		}
		break;
	}
	if (m_bRenderPerMouseMove)
		Render();

lb_return:
	m_ptMousePrvDrg = point;
}
	

void CTool::DrawBrushArea(VECTOR3* pv3IntersectPoint)
{
	if (m_gxhLightPrj)
	{

		VECTOR3		v3Pos;
		v3Pos = g_v3Point = *pv3IntersectPoint;
			 
		v3Pos.y += 200.0f;//m_fLightPrjHeight;
		m_pExecutive->GXLSetPosition(m_gxhLightPrj,&v3Pos);
	}

}
void CTool::SetHFieldVertexColorBrushSize(float fRadius)
{
	if (fRadius == m_fHFieldVertexColorBrushSize)
		return;
	
	m_fHFieldVertexColorBrushSize = fRadius;
	LIGHT_DESC	lightDesc;
	m_pExecutive->GXLGetLightDesc(m_gxhLightPrj,&lightDesc);
	lightDesc.fHeight = lightDesc.fWidth = m_fHFieldVertexColorBrushSize;
	m_pExecutive->GXLSetLightDesc(m_gxhLightPrj,&lightDesc);
}
void CTool::ResetTransformBrush()
{

}

void CTool::SetCamera(DWORD dwFlag,DWORD dwViewportIndex)
{
	if( m_CurrentViewMode == dwFlag)	return ;
	this->m_Mouse.PushCameraAngleAndPos(m_CurrentViewMode);

	VIEW_VOLUME		vv;
	
	m_pExecutive->GetGeometry()->GetViewVolume(&vv,dwViewportIndex);
	switch(dwFlag)
	{
	case VIEW_TYPE_TOP:
		this->m_Mouse.DoNotRotate();
		this->m_Mouse.PopCameraAngleAndPos(VIEW_TYPE_TOP);
		break;
	case VIEW_TYPE_BOTTOM:
		this->m_Mouse.DoNotRotate();
		this->m_Mouse.PopCameraAngleAndPos(VIEW_TYPE_BOTTOM);
		break;
	case VIEW_TYPE_LEFT:
		this->m_Mouse.DoNotRotate();
		this->m_Mouse.PopCameraAngleAndPos(VIEW_TYPE_LEFT);
		break;
	case VIEW_TYPE_RIGHT:
		this->m_Mouse.DoNotRotate();
		this->m_Mouse.PopCameraAngleAndPos(VIEW_TYPE_RIGHT);
		break;
	case VIEW_TYPE_FRONT:
		this->m_Mouse.DoNotRotate();
		this->m_Mouse.PopCameraAngleAndPos(VIEW_TYPE_FRONT);
		break;
	case VIEW_TYPE_BACK:
		this->m_Mouse.DoNotRotate();
		this->m_Mouse.PopCameraAngleAndPos(VIEW_TYPE_BACK);
		break;
	case VIEW_TYPE_FREE:
		this->m_Mouse.AllowRotate();
		this->m_Mouse.PopCameraAngleAndPos(VIEW_TYPE_FREE);
		break;
	}

	m_CurrentViewMode	=	(VIEW_TYPE)dwFlag;

	Render();
}

void CTool::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case 'Z':
	case 'z':
		g_pRightView->ToggleTileBrushMode();
		break;

	case 'b':
	case 'B':
	//	m_pBrushModel = NULL;
		break;
	case 'T':
	case 't':
		if( this->m_CurrentViewMode != VIEW_TYPE_TOP)
			SetCamera(VIEW_TYPE_TOP,m_dwCurrentViewportIndex);
		else
			SetCamera(VIEW_TYPE_BOTTOM,m_dwCurrentViewportIndex);
		break;
	case 'R':
	case 'r':
		if( this->m_CurrentViewMode != VIEW_TYPE_RIGHT)
					SetCamera(VIEW_TYPE_RIGHT,m_dwCurrentViewportIndex);
		else		
					SetCamera(VIEW_TYPE_LEFT,m_dwCurrentViewportIndex);
		break;
	case 'E':
	case 'e':
		if( this->m_CurrentViewMode != VIEW_TYPE_FRONT)
					SetCamera(VIEW_TYPE_FRONT,m_dwCurrentViewportIndex);
		else	
					SetCamera(VIEW_TYPE_BACK,m_dwCurrentViewportIndex);
		break;
	case 'F':
	case 'f':
		SetCamera(VIEW_TYPE_FREE,m_dwCurrentViewportIndex);
		break;
	case VK_RETURN:
		PutBrush();
		break;
	case 'p':
	case 'P':
		g_pMainFrame->GetDlgBalHField()->ToggleCheckSelect();
		break;
	case 'i':
	case 'I':
		g_pMainFrame->GetDlgBalHField()->ToggleCheckPickColor();
		break;
	case VK_UP:
		if (m_dwSelectedVertexNum)
		{
			if (m_pHField->AdjustHeight(m_pSelectedVertexIndex,m_dwSelectedVertexNum,g_pDlgBarHField->GetHeightPitch(),HFIELD_ADJUST_TYPE_ADD))
				Render();

		}
		break;
	case VK_DOWN:
		if (m_dwSelectedVertexNum)
		{
			if (m_pHField->AdjustHeight(m_pSelectedVertexIndex,m_dwSelectedVertexNum,g_pDlgBarHField->GetHeightPitch(),HFIELD_ADJUST_TYPE_SUB))
				Render();

		}

		break;
	}
}
void CTool::OnClickTile(POINT* pt,UINT nFlags)
{
	
	float		fDist;
	if (m_pHField)
	{

		if (g_pRightView->GetSelectedTileBrush() && m_TileBrushMode)
		{
			HFIELD_POS	hfPos;
			WORD*		pwTexTileIndexList;
			HFIELD_POS*	pTileList;
			VECTOR3		v3Pos;
			
			if (m_pHField->GetTilePosWithScreenCoord(&v3Pos,&fDist,&hfPos.dwX,&hfPos.dwZ,&m_ptMousePos,m_dwCurrentViewportIndex))
			{
				DWORD dwTileNum = g_pRightView->GetSelectedTileBrush()->GetTileList(&pTileList,&pwTexTileIndexList);
				m_pHField->SetTileList(&hfPos,pTileList,pwTexTileIndexList,dwTileNum,0);
			}
		}
		else if (g_pDlgBarHField->GetBrushSize() > 1)
		{
			RECT		rect;
			
			if (m_pHField->GetTileRectWithScreenCoord(&m_v3Clicked,&fDist,&rect,pt,g_pDlgBarHField->GetBrushSize(),m_dwCurrentViewportIndex))
			{
				if (m_pHField->SetTileRect(&rect,m_pPalette->GetCurrentTileIndex()))
					Render();

			}
		}
		else
		{
			DWORD		dwPosX,dwPosY;
			if (m_pHField->GetTilePosWithScreenCoord(&m_v3Clicked,&fDist,&dwPosX,&dwPosY,pt,m_dwCurrentViewportIndex))
			{
				if (m_pHField->SetTile(dwPosX,dwPosY,m_pPalette->GetCurrentTileIndex()))
					Render();
			}
		}
	}
/*
	*/

}


//yh
void CTool::OnClickTileSet(POINT* pt,UINT nFlags)
{	
	float		fDist;
	if (m_pHField)
	{
		DWORD		dwPosX,dwPosZ;
		if (m_pHField->GetTilePosWithScreenCoord(&m_v3Clicked,&fDist,&dwPosX,&dwPosZ,pt,m_dwCurrentViewportIndex))
		{
			CTileSet* pTileSet = m_pTileSetDialog->GetSelectedTileSet();
			if(pTileSet == NULL)
			{
				// TileSet을 선택해주세요.
				return;
			}
			HFIELD_DESC desc;
			m_pHField->GetHFieldDesc(&desc);
			if( dwPosX < 2 || dwPosZ < 2 ||
				dwPosX >= (desc.dwTileNumX-2) ||
				dwPosZ >= (desc.dwTileNumZ-2) )
			{
				// 가장자리는 설정이 안됩니다.
				return;
			}

			m_pHField->SetTile(dwPosX,dwPosZ,pTileSet->GetCenterPiece(0)->GetTexture().GetTileIndex());

			DWORD ix,iz;
			WORD wTileIndex;
			TILETEXTURE src[25],res[9];
			int x,z;
			for(z=0;z<5;++z)
			{
				iz = dwPosZ + (4-z) - 2;
				for(x=0;x<5;++x)
				{
					ix = dwPosX + x - 2;
					BOOL rt =m_pHField->GetTile(&wTileIndex,ix,iz);
					ASSERT(rt);

					src[z*5+x].SetTileIndex(wTileIndex);
				}
			}

			CTileSetApplyer applyer;
			applyer.Apply(pTileSet,src,res);

			for(z=0;z<3;++z)
			{
				iz = dwPosZ + (2-z) - 1;
				for(x=0;x<3;++x)
				{
					ix = dwPosX + x - 1;
					BOOL rt = m_pHField->SetTile(ix,iz,res[z*3+x].GetTileIndex());
					//ASSERT(rt);
				}
			}			
		}
		Render();
	}	
}
//

void CTool::FillSelecteTile()
{
	if (m_dwSelectedTileNum)
	{
		HFIELD_POS		hfPos;
		hfPos.dwX = 0;
		hfPos.dwZ = 0;
		m_pHField->SetTileList(&hfPos,m_pSelectedTileIndex,NULL,m_dwSelectedTileNum,m_pPalette->GetCurrentTileIndex());
		Render();
	}
}
void CTool::RotateSelecteTile()
{
	//dwSelectePosNum = m_pHField->GetTileListWithScreenCoord(hfpos,4096,&rect,0);
	//	m_dwSelectedTileNum = AddIndex(m_pSelectedTileIndex,m_dwSelectedTileNum,hfpos,dwSelectePosNum);

	
	WORD	wTileIndex;
	WORD	wDirFlag;
	for (DWORD i=0; i<m_dwSelectedTileNum; i++)
	{

		if (m_pHField->GetTile(&wTileIndex,m_pSelectedTileIndex[i].dwX,m_pSelectedTileIndex[i].dwZ))
		{

			wDirFlag = (((wTileIndex & 0xc000)>>14) + 1) % 4;
			wTileIndex = (WORD)( (wTileIndex & 0x3fff) | (wDirFlag<<14) );

			m_pHField->SetTile(m_pSelectedTileIndex[i].dwX,m_pSelectedTileIndex[i].dwZ,wTileIndex);

//			m_pHField->GetTile(&wTileIndex,m_pSelectedTileIndex[i].dwX,m_pSelectedTileIndex[i].dwZ);
		}
	}
	Render();
	
}
void CTool::OnNewMap()

{	if (m_ghMap)
	{
		m_pExecutive->DeleteGXMap(m_ghMap);
		m_ghMap = NULL;
		m_bMapBuild = FALSE;

	}
	if (m_gxhLightPrj)
	{
		m_pExecutive->DeleteGXLight(m_gxhLightPrj);
		m_gxhLightPrj = NULL;
	}
	OnDeleteAllGXObjects();
	m_pExecutive->DeleteAllGXLights();
	m_pExecutive->DeleteAllGXEventTriggers();
//	m_pExecutive->DeleteAllGXDecal();
	

	CleanHeightField();
	CleanStaticModel();
	VBHDeleteAll(m_pHash);

	//trustpak
	memset(m_szCurrentMapFileName, 0, sizeof(m_szCurrentMapFileName));
	strncpy(m_szCurrentMapFileName, DEFAULT_SS3D_MAP_FILE_NAME, sizeof(m_szCurrentMapFileName));

	memset(m_szCurrentTilePaletteFileName, 0, sizeof(m_szCurrentTilePaletteFileName));
	m_pPalette->DisplayCurrentTilePalleteFileName();


	memset(m_aLoadFailedTextureTable, 0, sizeof(m_aLoadFailedTextureTable));	
	UpdateLoadFailedTextureDialog();

	m_pTileSetDialog->UpdateTexturesOfTileSetDisplayers();
	
	////



}
void CTool::OnDeleteAllGXLights()
{
	m_pExecutive->DeleteAllGXLights();
}
void CTool::OnDeleteAllGXObjects()
{

	DWORD dwGXObjectsNum = m_pExecutive->GetGXObjectsNum();

	for (DWORD i=0; i<dwGXObjectsNum; i++)
	{
		GXOBJECT_HANDLE	gxo = m_pExecutive->GetGXObjectWithSeqIndex(i);
		char*		pFileName = (char*)m_pExecutive->GetData(gxo);		

		if (pFileName)
		{
			delete [] pFileName;
			m_pExecutive->SetData(gxo,NULL);		
		}
	}
	m_pExecutive->DeleteAllGXObjects();
}
void CTool::GetTileInfo(TILE_INFO* pTileInfo,POINT* pt)
{
	pTileInfo->dwPosX = 0xffffffff;
	pTileInfo->dwPosY = 0xffffffff;

	VECTOR3		v3Pos;
	float	fDist;
	if (m_pHField)
	{
		
		m_pHField->GetTilePosWithScreenCoord(&v3Pos,&fDist,&pTileInfo->dwPosX,&pTileInfo->dwPosY,pt,m_dwCurrentViewportIndex);
	}
	g_pMainFrame->SetTilePos(pTileInfo->dwPosX,pTileInfo->dwPosY);
}
void CTool::OnClickVertex(POINT* pt,UINT nFlags)
{
	DWORD		dwPosX ,dwPosZ;
	VECTOR3		v3Where;
	if( m_pHField)		// 버텍스 집기 테스트.
	{
		if( m_pHField->GetVertexPosWithScreenCoord( &dwPosX, &dwPosZ,&v3Where,pt, 600.0f,m_dwCurrentViewportIndex))
		{
		//	m_pHField->MakeHeightFieldHigh(dwPosX,dwPosZ,100.0f,5000.0f,0);
			m_pHField->InterpolateField(dwPosX,dwPosZ,g_pDlgBarHField->GetInterpolationBias(),g_pDlgBarHField->GetRadius());
			Render();
		}
		else 
			__asm nop
	}
}
void CTool::OnClickLightMap(POINT* pt,UINT nFlags)
{
	if (!m_pStaticModel)
		return;

	if (m_bPickColorMode)
	{
		OnRClickLightMap(pt,nFlags);
		return;
	}

	// 맵과의 충돌 지점을 찾아서...
	VECTOR3			v3Pos;
	DWORD		dwObjIndex;
	float		fDist;

	DWORD		dwColor;

		

	if (m_pExecutive->GXMGetCollisionPointWithScreenCoord(&dwObjIndex,&v3Pos,&fDist,pt,PICK_TYPE_PER_FACE))
	{
		g_v3Point = v3Pos;
		if (dwObjIndex != 0xffffffff)
		{

			dwColor = g_pDlgBarHField->GetLightTexelColor();
			if (m_pStaticModel->SetLightTexel(pt,dwObjIndex,&v3Pos,dwColor,m_dwCurrentViewportIndex))
				Render();
		}
	}
}

BOOL CTool::OnClickHFieldVertexColor(POINT* pt,VECTOR3* pv3IntersectPoint,UINT nFlags)
{
	// 맵과의 충돌 지점을 찾아서...
	VECTOR3			v3Pos;
	float		fDist;

	DWORD		dwColor = 0;

	if (m_bPickColorMode)
	{
		return OnRClickHFieldVertexColor(pt,pv3IntersectPoint,nFlags);
	}

	g_v3Point = *pv3IntersectPoint;
	dwColor = g_pDlgBarHField->GetLightTexelColor();
	return m_pHField->SetLightVertexel(pt,pv3IntersectPoint,m_fHFieldVertexColorBrushSize/2.0f,dwColor,m_dwCurrentViewportIndex);
}

BOOL CTool::OnClickHFieldAlphaTexel(POINT* pt,VECTOR3* pv3IntersectPoint,UINT nFlags)
{
	// 맵과의 충돌 지점을 찾아서...
	VECTOR3			v3Pos;
	float		fDist;

	DWORD		dwColor = 0;

//	if (m_bPickColorMode)
//	{
//		return OnRClickHFieldVertexColor(pt,pv3IntersectPoint,nFlags);
//	}
	

	g_v3Point = *pv3IntersectPoint;
	dwColor = g_pDlgBarHField->GetLightTexelColor();
	
	DWORD	dwActionFlag = m_pPalette->GetAlphaBrushActionFlag();
	DWORD	dwTileIndex = m_pPalette->GetCurrentTileIndex();

	/*
	EDIT_ALPHA_TEXEL_OP_ADD		=	0x01000000,
	EDIT_ALPHA_TEXEL_OP_SUB		=	0x02000000,
	EDIT_ALPHA_TEXEL_BRIGHT_0	=	0x00000000,
	EDIT_ALPHA_TEXEL_BRIGHT_1	=	0x00000001,
	EDIT_ALPHA_TEXEL_BRIGHT_2	=	0x00000002,
	EDIT_ALPHA_TEXEL_BRIGHT_3	=	0x00000004*/

	if (nFlags & MK_CONTROL)
	{
		dwTileIndex = m_pHField->GetExtTileIndexTop(pt,pv3IntersectPoint,m_dwCurrentViewportIndex);
		if (0xffffffff != dwTileIndex)
		{
			m_pPalette->SetTile(dwTileIndex);
		}

		return TRUE;
	}

	
	return m_pHField->SetAlphaTexel(pt,pv3IntersectPoint,m_fHFieldVertexColorBrushSize/2.0f,dwTileIndex,dwActionFlag,m_dwCurrentViewportIndex);

}
void CTool::OnClearAlphaMapWithTileIndex(POINT* ptCursor,UINT nFlags)
{
	if (m_pHField)
	{
		VECTOR3		v3IntersectPoint;
		DWORD	dwObjIndex = m_pHField->GetObjectPosWithScreenCoord(&v3IntersectPoint,ptCursor,m_dwCurrentViewportIndex);
		if (dwObjIndex != 0xffffffff)
		{
			DWORD	dwTileIndex = m_pPalette->GetCurrentTileIndex();
			m_pHField->ClearAlphaMapWithTileIndex(dwObjIndex,dwTileIndex);
		}
	}
	Render();
}
void CTool::OnClearAlphaMapWithObjectIndex(POINT* ptCursor,UINT nFlags)
{
	if (m_pHField)
	{
		VECTOR3		v3IntersectPoint;
		DWORD	dwObjIndex = m_pHField->GetObjectPosWithScreenCoord(&v3IntersectPoint,ptCursor,m_dwCurrentViewportIndex);
		if (dwObjIndex != 0xffffffff)
		{
			DWORD	dwTileIndex = m_pPalette->GetCurrentTileIndex();
			m_pHField->ClearAlphaMapWithObjectIndex(dwObjIndex);
		}
	}
	Render();
}
void CTool::OnClearAlphaMapAll(POINT* ptCursor,UINT nFlags)
{
	if (m_pHField)
	{
		m_pHField->ClearAlphaMapAll();
	}
	Render();
}
void CTool::OnRClickLightMap(POINT* pt,UINT nFlags)
{
	// 맵과의 충돌 지점을 찾아서...
	VECTOR3			v3Pos;
	DWORD		dwObjIndex;
	float		fDist;

	DWORD		dwColor = 0;
	if (m_pExecutive->GXMGetCollisionPointWithScreenCoord(&dwObjIndex,&v3Pos,&fDist,pt,PICK_TYPE_PER_FACE))
	{
		g_v3Point = v3Pos;
		if (m_pStaticModel->GetLightTexel(&dwColor,pt,dwObjIndex,&v3Pos,m_dwCurrentViewportIndex))
		{
			g_pDlgBarHField->SetLightTexelColor(dwColor);
		}
	}

}
BOOL CTool::OnRClickHFieldVertexColor(POINT* pt,VECTOR3* pv3IntersectPoint,UINT nFlags)
{
	// 맵과의 충돌 지점을 찾아서...

	BOOL		bResult = FALSE;
	DWORD		dwColor = 0xffffffff;
	g_v3Point = *pv3IntersectPoint;
	if (m_pHField->GetLightVertexel(&dwColor,pt,pv3IntersectPoint,m_dwCurrentViewportIndex))
	{
		g_pDlgBarHField->SetLightTexelColor(dwColor);
		bResult = TRUE;
		
	}
	return bResult;
}
void CTool::OnClickGXObject(POINT* pt,UINT nFlags)
{
	float	fDist;

	if (m_dwEditMode & EDIT_TYPE_CREATE)
	{
		char	szFileName[_MAX_PATH];

		if (!g_pRightView->GetGXObjectName(szFileName))
			return;


		// 맵과의 충돌 지점을 찾아서...
		VECTOR3			v3Pos;
		
		if (!m_pExecutive->GXMGetCollisionPointWithScreenCoord(NULL,&v3Pos,&fDist,pt,PICK_TYPE_PER_FACE))
		{
			v3Pos.x = 0.0f;
			v3Pos.y = 0.0f;
			v3Pos.z = 0.0f;
		}

	
		
		DWORD	dwFlag = 0;
		GXOBJECT_HANDLE		gxh = m_pExecutive->CreateGXObject(szFileName,NULL,0,dwFlag);
		if (gxh)
		{
	//		SET_VECTOR3(&v3Pos,0);
			m_pExecutive->GXOSetPosition(gxh,&v3Pos,FALSE);
			Render();
			
			char	filename[MAX_NAME_LEN];
			lstrcpy(filename,szFileName);
			GetNameRemovePath(filename,filename);
			char*	pFileName = new char[lstrlen(filename)+1];
			lstrcpy(pFileName,filename);

			if (!pFileName)
				__asm int 3

			m_pExecutive->SetData(gxh,pFileName);
		}
		
	}
	else
	{
		PICK_GXOBJECT_DESC	pickDesc[32];
		DWORD	dwModelIndex,dwObjIndex;
//		DWORD	dwCount = m_pExecutive->GXOGetMultipleObjectWithScreenCoord(pickDesc,32,pt,m_dwCurrentViewportIndex,PICK_TYPE_PER_BONE_OBJECT);
	//	m_gxoSelected = m_pExecutive->GXOGetObjectWithScreenCoord(&m_v3PickedPos,&dwModelIndex,&dwObjIndex,&fDist,pt,m_dwCurrentViewportIndex,PICK_TYPE_PER_BONE_OBJECT);
		m_gxoSelected = m_pExecutive->GXOGetObjectWithScreenCoord(&m_v3PickedPos,&dwModelIndex,&dwObjIndex,&fDist,pt,m_dwCurrentViewportIndex,PICK_TYPE_PER_FACE);
		if (m_gxoSelected)
		{
			CAMERA_DESC	cameraDesc;
			m_pExecutive->GetGeometry()->GetCameraDesc(&cameraDesc,m_dwCurrentViewportIndex);
			VECTOR3_SUB_VECTOR3(&m_planePicked.v3Up,&cameraDesc.v3To,&cameraDesc.v3From);
			Normalize(&m_planePicked.v3Up,&m_planePicked.v3Up);
			m_planePicked.D =  -1.0f * DotProduct(&m_planePicked.v3Up,&m_v3PickedPos);

			//trustpak 2005/06/09
			if (nFlags & MK_SHIFT)
			{
				m_bObjectCopy = TRUE;
			}

			///
		}
	}
}
void CTool::OnRClickGXObject(POINT* ptCursor,UINT nFlags)
{
	float	fDist;
	VECTOR3	v3IntersectPoint;
	DWORD	dwModelIndex,dwObjIndex;

	GXOBJECT_HANDLE	gxo = m_pExecutive->GXOGetObjectWithScreenCoord(&v3IntersectPoint,&dwModelIndex,&dwObjIndex,&fDist,ptCursor,m_dwCurrentViewportIndex,PICK_TYPE_PER_FACE);

	DWORD		dwResult;
	if (gxo)
	{
		GXOBJECT_PROPERTY	gxoproperty;
		gxoproperty.dwID = m_pExecutive->GetID(gxo);					// 아이디를 초기화..
		m_pExecutive->GXOGetDirection(gxo,&gxoproperty.v3Axis,&gxoproperty.fRad);
		m_pExecutive->GXOGetPosition(gxo,&gxoproperty.v3Pos);
		m_pExecutive->GXOGetScale(gxo,&gxoproperty.v3Scale);

		gxoproperty.bApplyHField = m_pExecutive->GXOIsHFieldApply(gxo);
		gxoproperty.bAsEffect = m_pExecutive->GXOIsAsEffect(gxo);
		gxoproperty.bLock = m_pExecutive->IsLockTransform(gxo);

		
		CObjectProperty pProperty;
		
		pProperty.SetGXObjectProperty(&gxoproperty);
		dwResult = pProperty.DoModal();
		
		if (dwResult == 0xffffffff)
		{
						
			char*	pFileName = (char*)m_pExecutive->GetData(gxo);
			if (pFileName)
			{
				delete [] pFileName;
				m_pExecutive->SetData(gxo,NULL);
			}
			m_pExecutive->DeleteGXObject(gxo);
		}
		else
		{
			if (gxoproperty.bLock)
                m_pExecutive->LockTransform(gxo);
			else
				m_pExecutive->UnLockTransform(gxo);


			if (gxoproperty.bApplyHField)
				m_pExecutive->GXOEnableHFieldApply(gxo);
			else 
				m_pExecutive->GXODisableHFieldApply(gxo);

			if (gxoproperty.bAsEffect)
				m_pExecutive->GXOEnableAsEffect(gxo);
			else
				m_pExecutive->GXODisableAsEffect(gxo);

			m_pExecutive->GXOSetDirection(gxo,&gxoproperty.v3Axis,gxoproperty.fRad);
			m_pExecutive->GXOSetPosition(gxo,&gxoproperty.v3Pos,FALSE);
			m_pExecutive->GXOSetScale(gxo,&gxoproperty.v3Scale);

			m_pExecutive->SetID(gxo,gxoproperty.dwID);
			
		//	GXMAP_OBJECT_HANDLE gxh = m_pExecutive->GetGXMapObjectWithID(gxoproperty.dwID);
		//	DWORD dwID = m_pExecutive->GetID(gxh);

		}
		Render();

	}
}
void CTool::OnRClickGXTrigger(POINT* ptCursor,UINT nFlags)
{
	float	fDist;
	VECTOR3	v3IntersectPoint;
	GXTRIGGER_HANDLE	gxt = m_pExecutive->GXTGetObjectWithScreenCoord(&v3IntersectPoint,&fDist,ptCursor,m_dwCurrentViewportIndex,0);

	DWORD		dwResult;
	if (gxt)
	{
		GXTRIGGER_PROPERTY	gxtproperty;
		gxtproperty.dwID = m_pExecutive->GetID(gxt);					// 아이디를 초기화..
		m_pExecutive->GXTGetEventTriggerDesc(gxt,&gxtproperty.evDesc);
		
		CTriggerProperty Property;
		
		Property.SetTriggerProperty(&gxtproperty);
		dwResult = Property.DoModal();
		
		if (dwResult == 0xffffffff)
		{
			
			m_pExecutive->DeleteGXEventTrigger(gxt);
		}
		else
		{
			m_pExecutive->GXTSetPosition(gxt,&gxtproperty.evDesc.v3Pos);
			m_pExecutive->SetID(gxt,gxtproperty.dwID);
		
		}

	}
}
BOOL CTool::ToggleViewColSTM()
{
	if (m_bEnableViewColSTM)
		m_bEnableViewColSTM = FALSE;
	else 
		m_bEnableViewColSTM = TRUE;

	return m_bEnableViewColSTM;

}
BOOL CTool::ToggleLightmap()
{
	if (m_bEnableLightMap)
	{
		m_bEnableLightMap = FALSE;
		m_pExecutive->GetRenderer()->SetLightMapFlag(0);
	}
	else
	{
		m_bEnableLightMap = TRUE;
		m_pExecutive->GetRenderer()->SetLightMapFlag(1);
	}
	Render();
	return m_bEnableLightMap;
}
BOOL CTool::ToggleViewOnlyHField()
{
	if (m_bEnableViewOnlyHField)
	{
		m_bEnableViewOnlyHField = FALSE;
	
	}
	else
	{
		m_bEnableViewOnlyHField = TRUE;
		
	}
	Render();
	return m_bEnableViewOnlyHField;
}
BOOL CTool::ToggleViewOnlyLightTexture()
{
	if (m_bEnableViewOnlyLightTexture)
	{
		m_bEnableViewOnlyLightTexture = FALSE;
		m_pExecutive->GetRenderer()->SetLightMapFlag(LIGHTMAP_FLAG_ENABLE);
	}
	else
	{
		
		m_bEnableViewOnlyLightTexture = TRUE;
		m_pExecutive->GetRenderer()->SetLightMapFlag(LIGHTMAP_FLAG_ENABLE | LIGHTMAP_FLAG_DISABLE_TEXTURE | LIGHTMAP_FLAG_DISABLE_MAGFILTER);
	}

	Render();
	return m_bEnableViewOnlyLightTexture;
}

BOOL CTool::ToggleBoundingBox()
{
	if (m_bEnableBoundingBox)
	{
		m_bEnableBoundingBox = FALSE;
		m_pExecutive->GetGeometry()->SetDrawDebugFlag(0);

	}
	else
	{
		m_bEnableBoundingBox = TRUE;
		m_pExecutive->GetGeometry()->SetDrawDebugFlag(	DEBUG_DRAW_MODEL_COL_MESH | DEBUG_DRAW_STATIC_MODEL_COL_MESH);

	}
	return m_bEnableBoundingBox;

}
void CTool::EnableDirectionalLight(DIRECTIONAL_LIGHT_DESC* pLightDesc)
{
	m_pExecutive->GetGeometry()->SetDirectionalLight(pLightDesc,m_dwCurrentViewportIndex);
	m_pExecutive->GetGeometry()->EnableDirectionalLight(m_dwCurrentViewportIndex);
	
}
void CTool::DisableDirectionalLight()
{
	m_pExecutive->GetGeometry()->DisableDirectionalLight(m_dwCurrentViewportIndex);
}
void CTool::GetDirectionalLightDesc(DIRECTIONAL_LIGHT_DESC* pLightDesc)
{
	m_pExecutive->GetGeometry()->GetDirectionalLight(pLightDesc,m_dwCurrentViewportIndex);
}
BOOL CTool::ViewTilePalette()
{
	if (!m_bViewTilePalette)
	{
		m_bViewTilePalette = TRUE;
		m_pPalette->EnableView();
	}
	else
	{
		m_bViewTilePalette = FALSE;
		m_pPalette->DisableView();
	}
	return m_bViewTilePalette;
}

//yh
BOOL CTool::ViewTileSet()
{
	if (!m_bViewTileSet)
	{
		m_bViewTileSet = TRUE;
		m_pTileSetDialog->EnableView();
	}
	else
	{
		m_bViewTileSet = FALSE;
		m_pTileSetDialog->DisableView();
	}
	return m_bViewTileSet;
}
//

void CTool::OnLButtonDown(UINT nFlags, CPoint* point) 
{
	m_dwMouseStatus = LBUTTON_DOWN;

	m_ptMousePrvDown = *point;
	POINT ptCursor = *(POINT*)point;

	if (m_bSelectMode)
		return;

	switch((m_dwEditMode & EDIT_MODE_MASK))
	{
	case EDIT_MODE_VERTEX:
		OnClickVertex(&ptCursor,nFlags);
		break;
		
		//yh
	case EDIT_MODE_TILESET:
		{
			OnClickTileSet(&ptCursor,nFlags);
		}
		break;
		//

	case EDIT_MODE_TILE:
		//In
		{
			/*
			if( !m_pSectionSplitter->bSectionInputMode )
				OnClickTile(&ptCursor,nFlags);
			else 
				OnSectionSplitterDown( nFlags, &ptCursor );
			*/

			//Modified by KBS
			if(m_pSectionSplitter->bSectionInputMode)
				OnSectionSplitterDown( nFlags, &ptCursor );
			else if(m_pSpotSplitter->bSectionInputMode)
				OnSpotSplitterDown( nFlags, &ptCursor );
			else if(m_pStartSplitter->bSectionInputMode)			//0219
				OnStartSpotSplitterDown( nFlags, &ptCursor );
			else
				OnClickTile(&ptCursor,nFlags);
			//--


		}
		break;
	case EDIT_MODE_DRAW_LIGHTMAP:
		OnClickLightMap(&ptCursor,nFlags);
		break;
	case EDIT_MODE_DRAW_VERTEXCOLOR:
		{
			BOOL	bResult;
			VECTOR3	v3Pos;
			float	fDist;

			bResult = m_pExecutive->GXMGetHFieldCollisionPointWithScreenCoord(&v3Pos,&fDist,&ptCursor);
			if (bResult)
			{
				if (OnClickHFieldVertexColor(&ptCursor,&v3Pos,nFlags))
					Render();
			}
		}
		break;
	case EDIT_MODE_DRAW_HFIELD_ALPHAMAP:
		{
			BOOL	bResult;
			VECTOR3	v3Pos;
			float	fDist;

			bResult = m_pExecutive->GXMGetHFieldCollisionPointWithScreenCoord(&v3Pos,&fDist,&ptCursor);
			if (bResult)
			{
				if (OnClickHFieldAlphaTexel(&ptCursor,&v3Pos,nFlags))
					Render();
			}
		}
		break;
	case EDIT_MODE_GXOBJECT:
		OnClickGXObject(&ptCursor,nFlags);
		break;
	case EDIT_MODE_LIGHT:
		OnClickLight(&ptCursor,nFlags);
		break;
	case EDIT_MODE_BRUSH:
		OnClickBrush(&ptCursor,nFlags);
		break;
	case EDIT_MODE_TRIGGER:
		OnClickTrigger(&ptCursor,nFlags);
		break;
	case EDIT_MODE_GET_TILE_INFO:
		m_tileInfo.dwPosX = 0xffffffff;
		m_tileInfo.dwPosY = 0xffffffff;
		GetTileInfo(&m_tileInfo,&ptCursor);
		break;

	case EDIT_MODE_GET_HFIELDOBJECT:
		OnClickHFieldObject(&ptCursor,nFlags);
		break;
	}
}

void CTool::OnMButtonDown(UINT nFlags, CPoint* point) 
{
	POINT ptCursor = *(POINT*)point;

	switch((m_dwEditMode & EDIT_MODE_MASK))
	{
	case EDIT_MODE_DRAW_HFIELD_ALPHAMAP:
		g_pMainFrame->OnHFieldAlphaMapMenu(point);
		break;

	}
}
void CTool::OnRButtonDown(UINT nFlags, CPoint* point) 
{
	POINT ptCursor = *(POINT*)point;

	switch((m_dwEditMode & EDIT_MODE_MASK))
	{

	case EDIT_MODE_VERTEX:
	//	OnClickVertex(&ptCursor);
		break;
	case EDIT_MODE_TILE:
	//	OnClickTile(&ptCursor);
		break;
	case EDIT_MODE_GXOBJECT:
		OnRClickGXObject(&ptCursor,nFlags);
		break;
	case EDIT_MODE_LIGHT:
		OnRClickLight(&ptCursor,nFlags);
		break;
	case EDIT_MODE_TRIGGER:
		OnRClickGXTrigger(&ptCursor,nFlags);
		break;
	case EDIT_MODE_BRUSH:
	//	OnClickBrush(&ptCursor);
		break;
	case EDIT_MODE_GET_TILE_INFO:
	//	m_tileInfo.dwPosX = 0xffffffff;
	//	m_tileInfo.dwPosY = 0xffffffff;
	//	GetTileInfo(&m_tileInfo,&ptCursor);
		break;
	
	case EDIT_MODE_GET_HFIELDOBJECT:
	//	OnClickHFieldObject(&ptCursor);
		break;
	}
}
void CTool::OnRClickLight(POINT* ptCursor,UINT nFlags)
{
	float	fDist;
	VECTOR3	v3IntersectPoint;
	GXLIGHT_HANDLE	gxl = m_pExecutive->GXLGetLightWithScreenCoord(&v3IntersectPoint,&fDist,ptCursor,m_dwCurrentViewportIndex,0);

	DWORD		dwResult;
	if (gxl)
	{
		CLightProperty pProperty;
		GXLIGHT_PROPERTY	lightProperty;

		
		m_pExecutive->GXLGetLightDesc(gxl,&lightProperty.lightDesc);
		pProperty.SetGXLightProperty(&lightProperty);
		if (m_pExecutive->IsRenderable(gxl))
			lightProperty.bLightSwitch = TRUE;
		else 
			lightProperty.bLightSwitch = FALSE;

		if (m_pExecutive->GXLIsDisableStaticShadow(gxl))
			lightProperty.bShadowSwitch = FALSE;
		else 
			lightProperty.bShadowSwitch = TRUE;

		if (m_pExecutive->GXLIsEnableDynamicLight(gxl))
			lightProperty.bEnableDynamicLight = TRUE;
		else 
			lightProperty.bEnableDynamicLight = FALSE;

		dwResult = pProperty.DoModal();
		
		if (dwResult == 0xffffffff)
			m_pExecutive->DeleteGXLight(gxl);
		else
		{
			m_pExecutive->GXLSetLightDesc(gxl,&lightProperty.lightDesc);

			if (!lightProperty.bLightSwitch)
				m_pExecutive->DisableRender(gxl);
			else
				m_pExecutive->EnableRender(gxl);

			
			if (!lightProperty.bShadowSwitch)
				m_pExecutive->GXLDisableStaticShadow(gxl);
			else
				m_pExecutive->GXLEnableStaticShadow(gxl);

			if (lightProperty.bEnableDynamicLight)
				m_pExecutive->GXLEnableDynamicLight(gxl);
			else
				m_pExecutive->GXLDisableDynamicLight(gxl);


			
		}
	}
	

	

}
void CTool::OnClickLight(POINT* ptCursor,UINT nFlags)
{
	if (m_dwEditMode & EDIT_TYPE_CREATE)
	{
		if (!m_bMapBuild)
		{
			MessageBox(NULL,"맵이 빌드된 후에 라이트를 찍을 수 있습니다.","Error",MB_OK);
			return;
		}
		LIGHT_DESC	light;
			
	//	GetPosIntersect(&light.v3Point,ptCursor);
		
		PLANE	plane;
		plane.D = 0;
		plane.v3Up.x = 0;
		plane.v3Up.y = 1;
		plane.v3Up.z = 0;

		if (!m_pExecutive->GetGeometry()->GetIntersectPointWithScreenCoord(&light.v3Point,&plane,ptCursor,m_dwCurrentViewportIndex))
			return;


		
		light.fRs = 1000.0f;
		light.dwDiffuse = 0xffff0000;
		GXLIGHT_HANDLE gxl = m_pExecutive->CreateGXLight(&light,NULL,0,0,NULL,GXLIGHT_TYPE_STATIC);
		
		CLightProperty pProperty;
		GXLIGHT_PROPERTY	lightProperty;

		
		m_pExecutive->GXLGetLightDesc(gxl,&lightProperty.lightDesc);
		pProperty.SetGXLightProperty(&lightProperty);
		
		if (m_pExecutive->IsRenderable(gxl))
			lightProperty.bLightSwitch = TRUE;
		else 
			lightProperty.bLightSwitch = FALSE;

		if (m_pExecutive->GXLIsDisableStaticShadow(gxl))
			lightProperty.bShadowSwitch = FALSE;
		else 
			lightProperty.bShadowSwitch = TRUE;

		pProperty.DoModal();
	
		m_pExecutive->GXLSetLightDesc(gxl,&lightProperty.lightDesc);

		if (!lightProperty.bLightSwitch)
			m_pExecutive->DisableRender(gxl);
		else
			m_pExecutive->EnableRender(gxl);

		if (!lightProperty.bShadowSwitch)
			m_pExecutive->GXLDisableStaticShadow(gxl);
		else
			m_pExecutive->GXLEnableStaticShadow(gxl);


		if (lightProperty.bEnableDynamicLight)
			m_pExecutive->GXLEnableDynamicLight(gxl);
		else
			m_pExecutive->GXLDisableDynamicLight(gxl);


		
		Render();
	}
	else
	{
		float	fDist;
		m_gxlSelected = m_pExecutive->GXLGetLightWithScreenCoord(&m_v3PickedPos,&fDist,ptCursor,m_dwCurrentViewportIndex,0);
		if (m_gxlSelected)
		{
			CAMERA_DESC	cameraDesc;
			m_pExecutive->GetGeometry()->GetCameraDesc(&cameraDesc,m_dwCurrentViewportIndex);
			VECTOR3_SUB_VECTOR3(&m_planePicked.v3Up,&cameraDesc.v3To,&cameraDesc.v3From);
			Normalize(&m_planePicked.v3Up,&m_planePicked.v3Up);
			m_planePicked.D =  -1.0f * DotProduct(&m_planePicked.v3Up,&m_v3PickedPos);
			
			if (nFlags & MK_SHIFT)
				m_bLightCopy = TRUE;
		}
	}
}
void CTool::OnClickBrush(POINT *ptCursor,UINT nFlags)
{

	m_pBrush = g_pRightView->GetBrush(ptCursor);
	if (m_pBrush)
	{
		m_v3PickedPos = *m_pBrush->GetPos();

		CAMERA_DESC	cameraDesc;
		m_pExecutive->GetGeometry()->GetCameraDesc(&cameraDesc,m_dwCurrentViewportIndex);
		VECTOR3_SUB_VECTOR3(&m_planePicked.v3Up,&cameraDesc.v3To,&cameraDesc.v3From);
		Normalize(&m_planePicked.v3Up,&m_planePicked.v3Up);
		m_planePicked.D =  -1.0f * DotProduct(&m_planePicked.v3Up,&m_v3PickedPos);	
		
	}
	else
	{
		VECTOR3		v3IntersectPoint;
		float		fDist;
		DWORD		dwFaceGroupIndex;

		DWORD		dwMeshObjIndex = m_pStaticModel->GetMeshObjectWithScreenCoord(&v3IntersectPoint,&fDist,&dwFaceGroupIndex,ptCursor,m_dwCurrentViewportIndex,PICK_TYPE_PER_FACE);
	}
}
void CTool::OnClickTrigger(POINT* ptCursor,UINT nFlags)
{
			// 맵과의 충돌 지점을 찾아서...
	VECTOR3			v3Pos;
	float			fDist;
	
	DWORD			dwObjIndex;

	if (m_dwEditMode & EDIT_TYPE_CREATE)
	{
		if (m_pExecutive->GXMGetCollisionPointWithScreenCoord(&dwObjIndex,&v3Pos,&fDist,ptCursor,PICK_TYPE_PER_FACE))
		{
			DWORD	dwFlag = 0;
			GXTRIGGER_HANDLE gxh = m_pExecutive->CreateGXEventTrigger(NULL,NULL,dwFlag);
			if (gxh)
			{
		//		SET_VECTOR3(&v3Pos,0);
				m_pExecutive->GXTSetPosition(gxh,&v3Pos);
				Render();
			}
		}
	}
	else
	{
		m_gxtSelected = m_pExecutive->GXTGetObjectWithScreenCoord(&m_v3PickedPos,&fDist,ptCursor,m_dwCurrentViewportIndex,0);
		if (m_gxtSelected)
		{
			CAMERA_DESC	cameraDesc;
			m_pExecutive->GetGeometry()->GetCameraDesc(&cameraDesc,m_dwCurrentViewportIndex);
			VECTOR3_SUB_VECTOR3(&m_planePicked.v3Up,&cameraDesc.v3To,&cameraDesc.v3From);
			Normalize(&m_planePicked.v3Up,&m_planePicked.v3Up);
			m_planePicked.D =  -1.0f * DotProduct(&m_planePicked.v3Up,&m_v3PickedPos);
			
		}
	}
	
}

BOOL CTool::GetRot(float* pfRad,VECTOR3* pv3Rot,POINT* pPoint)
{
	if (!m_pExecutive)
		return FALSE;

	float dx = (float)(pPoint->x - m_ptMousePrvDrg.x);
	
	float rad = dx / 100;

	switch(m_dwMoveAxis)
	{
	case X_AXIS_LOCK:
		pv3Rot->x += rad;
		break;
	case Y_AXIS_LOCK:
		pv3Rot->y += rad;
		break;
	case Z_AXIS_LOCK:
		pv3Rot->z += rad;
		break;
	}
	if (pfRad)
		*pfRad = rad;

	return TRUE;
	
}
BOOL CTool::GetScale(VECTOR3* pv3Scale,VECTOR3* pv3BasePoint,POINT* pPoint)
{
	
	if (!m_pExecutive)
		return FALSE;
	
	PLANE	plane;
	CAMERA_DESC	cameraDesc;
	m_pExecutive->GetGeometry()->GetCameraDesc(&cameraDesc,m_dwCurrentViewportIndex);

	VECTOR3_SUB_VECTOR3(&plane.v3Up,&cameraDesc.v3To,&cameraDesc.v3From);
	Normalize(&plane.v3Up,&plane.v3Up);
	
	VECTOR3 v3Center;
	SET_VECTOR3(&v3Center,0.0f);
	VECTOR3_SUB_VECTOR3(&v3Center,pv3BasePoint,&v3Center);
	plane.D =  -1.0f*VECTOR3Length(&v3Center);

	


	VECTOR3	v3PrvPos,v3CurPos,v3PosInc;
	if (!m_pExecutive->GetGeometry()->GetIntersectPointWithScreenCoord(&v3PrvPos,&plane,&m_ptMousePrvDrg,m_dwCurrentViewportIndex))
		return FALSE;
	
	if (!m_pExecutive->GetGeometry()->GetIntersectPointWithScreenCoord(&v3CurPos,&plane,pPoint,m_dwCurrentViewportIndex))
		return FALSE;

	VECTOR3_SUB_VECTOR3(&v3PosInc,&v3CurPos,&v3PrvPos);
	VECTOR3_DIVEQU_FLOAT(&v3PosInc,300.0f);

	switch(m_dwMoveAxis)
	{
	case X_AXIS_LOCK:
		v3PosInc.y = 0.0f;
		v3PosInc.z = 0.0f;
		break;
	case Y_AXIS_LOCK:
		v3PosInc.x = 0.0f;
		v3PosInc.z = 0.0f;
		break;
	case Z_AXIS_LOCK:
		v3PosInc.x = 0.0f;
		v3PosInc.y = 0.0f;
		break;
	}
	VECTOR3_ADDEQU_VECTOR3(pv3Scale,&v3PosInc);
	
	return TRUE;

}



BOOL CTool::GetMovePos(VECTOR3* pv3Pos,VECTOR3* pv3BasePoint,POINT* pPoint)
{
	if (!m_pExecutive)
		return FALSE;

	VECTOR3		v3CurPos;
	if (!m_pExecutive->GetGeometry()->GetIntersectPointWithScreenCoord(&v3CurPos,&m_planePicked,pPoint,m_dwCurrentViewportIndex))
		return FALSE;

	VECTOR3		v3Offset;
	VECTOR3_SUB_VECTOR3(&v3Offset,&v3CurPos,&m_v3PickedPos);
	
	switch(m_dwMoveAxis)
	{
	case X_AXIS_LOCK:
		v3Offset.y = 0.0f;
		v3Offset.z = 0.0f;
		break;
	case Y_AXIS_LOCK:
		v3Offset.x = 0.0f;
		v3Offset.z = 0.0f;
		break;
	case Z_AXIS_LOCK:
		v3Offset.x = 0.0f;
		v3Offset.y = 0.0f;
		break;
	}

	VECTOR3_ADD_VECTOR3(pv3Pos,&m_v3PickedPos,&v3Offset);
	switch(m_CurrentViewMode)
	{
	case VIEW_TYPE_TOP:
	case VIEW_TYPE_BOTTOM:
		pv3Pos->y = pv3BasePoint->y;
		break;
	case VIEW_TYPE_LEFT:
	case VIEW_TYPE_RIGHT:
		pv3Pos->x = pv3BasePoint->x;
		break;
	case VIEW_TYPE_FRONT:
	case VIEW_TYPE_BACK:
		pv3Pos->z = pv3BasePoint->z;
		break;
	}
/*
	PLANE	plane;
	CAMERA_DESC	cameraDesc;
	m_pExecutive->GetGeometry()->GetCameraDesc(&cameraDesc,m_dwCurrentViewportIndex);

	VECTOR3_SUB_VECTOR3(&plane.v3Up,&cameraDesc.v3To,&cameraDesc.v3From);
	Normalize(&plane.v3Up,&plane.v3Up);
	
	VECTOR3 v3Center;
	SET_VECTOR3(&v3Center,0.0f);
	VECTOR3_SUB_VECTOR3(&v3Center,pv3BasePoint,&v3Center);
	plane.D =  -1.0f*VECTOR3Length(&v3Center);

	


	VECTOR3	v3PrvPos,v3CurPos,v3PosInc;
	if (!m_pExecutive->GetGeometry()->GetIntersectPointWithScreenCoord(&v3PrvPos,&plane,&m_ptMousePrvDrg,m_dwCurrentViewportIndex))
		return FALSE;
	
	if (!m_pExecutive->GetGeometry()->GetIntersectPointWithScreenCoord(&v3CurPos,&plane,pPoint,m_dwCurrentViewportIndex))
		return FALSE;

	VECTOR3_SUB_VECTOR3(&v3PosInc,&v3CurPos,&v3PrvPos);

	switch(m_dwMoveAxis)
	{
	case X_AXIS_LOCK:
		v3PosInc.y = 0.0f;
		v3PosInc.z = 0.0f;
		break;
	case Y_AXIS_LOCK:
		v3PosInc.x = 0.0f;
		v3PosInc.z = 0.0f;
		break;
	case Z_AXIS_LOCK:
		v3PosInc.x = 0.0f;
		v3PosInc.y = 0.0f;
		break;
	}
	VECTOR3_ADD_VECTOR3(pv3Pos,pv3BasePoint,&v3PosInc);
*/
	return TRUE;

}

BOOL CTool::GetPosIntersect(VECTOR3* pv3Pos,POINT* pPoint)
{
	if (!m_pExecutive)
		return FALSE;

	PLANE	plane;
	CAMERA_DESC	cameraDesc;
	m_pExecutive->GetGeometry()->GetCameraDesc(&cameraDesc,m_dwCurrentViewportIndex);

	VECTOR3	v3Dir;
	VECTOR3_SUB_VECTOR3(&v3Dir,&cameraDesc.v3To,&cameraDesc.v3From);
	Normalize(&v3Dir,&v3Dir);
	
	VECTOR3_MUL_FLOAT(&v3Dir,&v3Dir,DEFAULT_PICK_DISTANCE);

	VECTOR3	v3NewTo;
	VECTOR3_ADD_VECTOR3(&v3NewTo,&cameraDesc.v3From,&v3Dir);

	plane.D =  -1.0f * DotProduct(&v3Dir,&v3NewTo);
	plane.v3Up = v3Dir;
	
/*
	switch(m_CurrentViewMode)
	{
	case VIEW_TYPE_TOP:
		
		plane.v3Up.x = 0.0f;
		plane.v3Up.y = 1.0f;
		plane.v3Up.z = 0.0f;
		plane.D = -1.0f * pv3BasePoint->y;
		break;
	case VIEW_TYPE_BOTTOM:
		plane.v3Up.x = 0.0f;
		plane.v3Up.y = -1.0f;
		plane.v3Up.z = 0.0f;
		plane.D = 1.0f * pv3BasePoint->y;
		break;
	case VIEW_TYPE_LEFT:
		plane.v3Up.x = -1.0f;
		plane.v3Up.y = 0.0f;
		plane.v3Up.z = 0.0f;
		plane.D = 1.0f * pv3BasePoint->x;
		break;
	case VIEW_TYPE_RIGHT:
		plane.v3Up.x = 1.0f;
		plane.v3Up.y = 0.0f;
		plane.v3Up.z = 0.0f;
		plane.D = -1.0f * pv3BasePoint->x;
		break;
	case VIEW_TYPE_FRONT:
		plane.v3Up.x = 0.0f;
		plane.v3Up.y = 0.0f;
		plane.v3Up.z = -1.0f;
		plane.D = 1.0f * pv3BasePoint->z;
		break;
	case VIEW_TYPE_BACK:
		plane.v3Up.x = 0.0f;
		plane.v3Up.y = 0.0f;
		plane.v3Up.z = 1.0f;
		plane.D = -1.0f * pv3BasePoint->z;
		break;
	case VIEW_TYPE_FREE:
		plane.v3Up.x = 0.0f;
		plane.v3Up.y = 0.0f;
		plane.v3Up.z = -1.0f;
		plane.D = 1.0f * pv3BasePoint->z;
		break;
	}*/
	return m_pExecutive->GetGeometry()->GetIntersectPointWithScreenCoord(pv3Pos,&plane,pPoint,m_dwCurrentViewportIndex);


}



void CTool::ClearSelectedItem()
{
	m_gxoSelected = NULL;
	m_gxlSelected = NULL;
	m_pBrush = NULL;
	m_gxtSelected = NULL;
}
void CTool::OnSize(UINT nType, int cx, int cy) 
{
	
	m_pExecutive->GetGeometry()->UpdateWindowSize();
	
}
void CTool::OnClickHFieldObject(POINT* ptCursor,UINT nFlags)
{
	if (m_pHField)
	{
		VECTOR3	v3IntersectPoint;
		m_dwSelectedHFieldObjectIndex = m_pHField->GetObjectPosWithScreenCoord(&v3IntersectPoint,ptCursor,m_dwCurrentViewportIndex);

	}
	Render();
}

void CTool::OnLButtonUp(UINT nFlags, CPoint* point)
{
	m_bLightCopy = FALSE;
	if (m_dwMouseStatus == LBUTTON_DOWN)
	{
		if (m_bDrawSelectedRect)
		{
			m_dwSelectedVertexNum = 0;
			m_dwSelectedTileNum = 0;
			memset(m_v2PointSelectRect,0,sizeof(m_v2PointSelectRect));
			m_bDrawSelectedRect = FALSE;
			Render();
		}
		
	}
	m_dwMouseStatus = LBUTTON_UP;

	
	if (m_bSelectMode)
	{
		HFIELD_POS		hfpos[4096];
		DWORD			dwSelectePosNum;
		if (!(nFlags & MK_CONTROL))
		{
			m_dwSelectedVertexNum = 0;
			m_dwSelectedTileNum = 0;
		}

		RECT	rect;
		
		if (m_ptMousePrvDown.x < point->x)
		{
			rect.left = m_ptMousePrvDown.x;
			rect.right = point->x;
		}
		else
		{
			rect.left = point->x;
			rect.right = m_ptMousePrvDown.x;
		}

		if (m_ptMousePrvDown.y < point->y)
		{
			rect.top = m_ptMousePrvDown.y;
			rect.bottom = point->y;
		}
		else
		{
			rect.top = point->y;
			rect.bottom = m_ptMousePrvDown.y;
		}

		
		if ((m_dwEditMode & EDIT_MODE_MASK) == EDIT_MODE_VERTEX)
		{
//////////////////////////////////////////////////////////////////////


			MAABB	aabb;
			
			float	fDist;
			POINT	p1,p2;
			p1.x = rect.left;
			p1.y = rect.top;

			p2.x = rect.right;
			p2.y = rect.bottom;


			m_pHField->GetIntersectPointWithScreenCoord(&aabb.Min,&fDist,&p1,m_dwCurrentViewportIndex);
			m_pHField->GetIntersectPointWithScreenCoord(&aabb.Max,&fDist,&p2,m_dwCurrentViewportIndex);

			if (aabb.Max.x < aabb.Min.x)
			{
				float	temp = aabb.Max.x;
				aabb.Max.x = aabb.Min.x;
				aabb.Min.x = temp;
			}
			if (aabb.Max.z < aabb.Min.z)
			{
				float	temp = aabb.Max.z;
				aabb.Max.z = aabb.Min.z;
				aabb.Min.z = temp;
			}

//			memset(g_v3TriList,0,sizeof(g_v3TriList));
//			g_dwTriCount = m_pHField->GetTriListWithAABB(g_v3TriList,g_dwMaxTriCount,&aabb);

			dwSelectePosNum = m_pHField->GetVertexListWithScreenCoord(hfpos,4096,&rect,0);
			m_dwSelectedVertexNum = AddIndex(m_pSelectedVertexIndex,m_dwSelectedVertexNum,hfpos,dwSelectePosNum);
		}
		else if ((m_dwEditMode & EDIT_MODE_MASK) == EDIT_MODE_TILE)
		{
			dwSelectePosNum = m_pHField->GetTileListWithScreenCoord(hfpos,4096,&rect,0);
			m_dwSelectedTileNum = AddIndex(m_pSelectedTileIndex,m_dwSelectedTileNum,hfpos,dwSelectePosNum);

		}

	}

	//In
	if( ( m_dwEditMode & EDIT_MODE_MASK ) )
	{
		if( m_pSectionSplitter->bSectionInputMode )
			OnSectionSplitterUp( nFlags, (POINT*)&point );

		//Added by KBS
		if( m_pSpotSplitter->bSectionInputMode )
			OnSpotSplitterUp( nFlags, (POINT*)&point );
		

		if( m_pStartSplitter->bSectionInputMode )
			OnStartSpotSplitterUp( nFlags, (POINT*)&point );
		//--
	}
	
	ClearSelectedItem();
	if (m_dwSelectedVertexNum || m_dwSelectedTileNum)
		Render();

}
BOOL CTool::PutBrush()
{
	if (!m_pStaticModel)
		return FALSE;



	C3DBrush*	pBrush = g_pRightView->GetSelectedBrush();
	if (!pBrush)
		return FALSE;

//	m_pStaticModel->AddObjectFromBrush(m_pBrush,0);
	m_pStaticModel->AddModel(pBrush->GetModel(),pBrush->GetModelRefIndex(),pBrush->GetTM(),0);

	return TRUE;
}

void CTool::BeginCreateStaticModel()
{
	if (m_pStaticModel)
		m_pStaticModel->BeginCreateMesh(0);
}
void CTool::EndCreateStaticModel()
{
	if (m_pStaticModel)
		m_pStaticModel->EndCreateMesh();
}
void CTool::SetEditType(DWORD dwFlag)
{
	m_dwEditMode = ( m_dwEditMode & EDIT_MODE_MASK ) | dwFlag;
}
void CTool::SetPickTileMode()
{
	m_dwEditMode = (m_dwEditMode & EDIT_MODE_MASK_INVERSE) | EDIT_MODE_TILE;
}
//yh
void CTool::SetPickTileSetMode()
{
	m_dwEditMode = (m_dwEditMode & EDIT_MODE_MASK_INVERSE) | EDIT_MODE_TILESET;
}
//
void CTool::SetBrushMode()
{
	m_dwEditMode = (m_dwEditMode & EDIT_MODE_MASK_INVERSE) | EDIT_MODE_BRUSH;
}
void CTool::SetPickVertexMode()
{
	m_dwEditMode = (m_dwEditMode & EDIT_MODE_MASK_INVERSE) | EDIT_MODE_VERTEX;
}
void CTool::SetHFieldObjectMode()
{
	m_dwEditMode = (m_dwEditMode & EDIT_MODE_MASK_INVERSE) | EDIT_MODE_GET_HFIELDOBJECT;
}
void CTool::SetGXObjectMode()
{
	m_dwEditMode = (m_dwEditMode & EDIT_MODE_MASK_INVERSE) | EDIT_MODE_GXOBJECT;
}
void CTool::SetTileInfoMode()
{
	m_dwEditMode = (m_dwEditMode & EDIT_MODE_MASK_INVERSE) | EDIT_MODE_GET_TILE_INFO;
}
void CTool::SetLightMode()
{
	m_dwEditMode = (m_dwEditMode & EDIT_MODE_MASK_INVERSE) | EDIT_MODE_LIGHT;
	
}
void CTool::SetTriggerMode()
{
	m_dwEditMode = (m_dwEditMode & EDIT_MODE_MASK_INVERSE) | EDIT_MODE_TRIGGER;
}
void CTool::SetDrawLightMapMode()
{
	m_dwEditMode = (m_dwEditMode & EDIT_MODE_MASK_INVERSE) | EDIT_MODE_DRAW_LIGHTMAP;
}
void CTool::SetDrawHFieldVertexColor()
{
	m_dwEditMode = (m_dwEditMode & EDIT_MODE_MASK_INVERSE) | EDIT_MODE_DRAW_VERTEXCOLOR;
}
void CTool::SetDrawHFieldaAlphaMap()
{
	m_dwEditMode = (m_dwEditMode & EDIT_MODE_MASK_INVERSE) | EDIT_MODE_DRAW_HFIELD_ALPHAMAP;
}
BOOL CTool::CreateHeightField(HFIELD_DESC* pDesc)
{	
	m_hfDesc.dwObjNumX = pDesc->dwObjNumX;
	m_hfDesc.dwObjNumZ = pDesc->dwObjNumZ;
	m_hfDesc.fFaceSize = pDesc->fFaceSize;
	m_hfDesc.dwFacesNumPerObjAxis = pDesc->dwFacesNumPerObjAxis;
	
	return TRUE;
}
void CTool::ReplaceTile(char* szFileName,DWORD dwTileIndex)
{
	if (m_pHField)
		m_pHField->ReplaceTile(szFileName,dwTileIndex);
}

void CTool::CleanHeightField()
{
	
	if (m_pHField)
	{
		m_pHField->Release();
		m_pHField = NULL;
	}

	memset(&m_hfDesc,0,sizeof(m_hfDesc));
	m_bMapBuild = FALSE;

}





void CTool::SetRenderMode(DWORD dwFlag)
{

	//trustpak 
	DisableRenderBothMode();
	///

	if (m_pExecutive)
	{
		m_pExecutive->GetRenderer()->SetRenderMode(dwFlag);
	}
	Render();
}


//trustpak
void CTool::EnableRenderBothMode(void)
{
	if (m_pExecutive)
	{
		m_pExecutive->GetRenderer()->SetRenderWireSolidBothMode(TRUE);
	}	
}

void CTool::DisableRenderBothMode(void)
{
	if (m_pExecutive)
	{
		m_pExecutive->GetRenderer()->SetRenderWireSolidBothMode(FALSE);
	}		
}

///


CTool::~CTool()
{
	OnDeleteAllGXObjects();

	if (m_pSelectedTileIndex)
	{
		delete [] m_pSelectedTileIndex;
		m_pSelectedTileIndex = NULL;
	}
	if (m_pSelectedVertexIndex)
	{
		delete [] m_pSelectedVertexIndex;
		m_pSelectedVertexIndex = NULL;
	}
	OnNewMap();
	VBHRelease(m_pHash);
	

	if (m_pHFieldViewOnly)
	{
		m_pHFieldViewOnly->Release();
		m_pHFieldViewOnly = NULL;
	}
	if (m_pArrow)
	{
		m_pArrow->UnReference(m_dwArrowRefIndex);
		m_pArrow->Release();
		m_pArrow = NULL;
	}

	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}

	if (m_pPalette)
	{
		delete m_pPalette;
		m_pPalette = NULL;

	}
	
	//yh
	if (m_pTileSetDialog)
	{
		delete m_pTileSetDialog;
		m_pTileSetDialog = NULL;
	}
	if (m_pEditTileSetDialog)
	{
		delete m_pEditTileSetDialog;
		m_pEditTileSetDialog = NULL;
	}
	if (m_pEditEntryDialog)
	{
		delete m_pEditEntryDialog;
		m_pEditEntryDialog = NULL;
	}
	//

	//trustpak
	if (NULL != m_pLoadFailedTextureDialog)
	{
		delete m_pLoadFailedTextureDialog;
		m_pLoadFailedTextureDialog = NULL;
	}
	//

	
	if (m_pRendererForPalette)
	{
		m_pRendererForPalette->Release();
		m_pRendererForPalette = NULL;
	}
	if (m_pExecutive)
	{
		m_pExecutive->Release();
		m_pExecutive = NULL;
	}
	
	if (m_pFileStorage)
	{
		m_pFileStorage->Release();
		m_pFileStorage = NULL;
	}

	CoFreeUnusedLibraries();
	CoUninitialize();
	
	
	//In
	if( m_pSectionSplitter )
	{
		delete m_pSectionSplitter;
		m_pSectionSplitter = NULL;
	}

	if( m_pNpcInfoList )
	{
		delete m_pNpcInfoList;
		m_pNpcInfoList = NULL;
	}
	//

	//Added by KBS
	if( m_pSpotSplitter )
	{
		delete m_pSpotSplitter;
		m_pSpotSplitter = NULL;
	}

	if( m_pStartSplitter )
	{
		delete m_pStartSplitter;
		m_pStartSplitter = NULL;
	}
	//--

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
}

void CTool::SaveMap(char* szFileName)
{
	DWORD	i;
	char	stm[MAX_NAME_LEN];
	char	hfl[MAX_NAME_LEN];

	wsprintf(stm,"NA");
	wsprintf(hfl,"NA");

	// 먼저 맵..

	if (!m_pStaticModel && !m_pHField)
	{
		MessageBox(NULL,"맵으로 사용할 매쉬가 없습니다.","Error",MB_OK);
		return;
	}



	if (m_pStaticModel)
	{
		if (m_pStaticModel->GetObjectNum())
		{
			GetNameRemovePath(stm,szFileName);
			RemoveExt(stm,stm);
			lstrcat(stm,".stm");
			m_pStaticModel->WriteFile(stm);
		}
	}
	if (m_pHField)
	{
		GetNameRemovePath(hfl,szFileName);
		RemoveExt(hfl,hfl);
		lstrcat(hfl,".hfl");
		m_pHField->WriteFile(hfl,0);
	}

	FILE*	fp;
	fp = fopen(szFileName,"wt");


	// map meta data
	MAABB	Box;

	if( m_bMapBuild	)
	{
		m_pExecutive->GetWorldBoundingBox( &Box);
		if( Box.Max.x > m_WorldBox.Max.x)
			m_WorldBox.Max.x	=	Box.Max.x;
		if( Box.Min.x < m_WorldBox.Min.x)
			m_WorldBox.Min.x	=	Box.Min.x;

		if( Box.Max.y > m_WorldBox.Max.y)
			m_WorldBox.Max.y	=	Box.Max.y;
		if( Box.Min.y < m_WorldBox.Min.y)
			m_WorldBox.Min.y	=	Box.Min.y;

		if( Box.Max.z > m_WorldBox.Max.z)
			m_WorldBox.Max.z	=	Box.Max.z;
		if( Box.Min.z < m_WorldBox.Min.z)
			m_WorldBox.Min.z	=	Box.Min.z;
	}
	fprintf( fp,"%s\n{\n", PID_GX_METADATA);
	fprintf( fp, "\t%s %f %f %f\n", PID_BOUNDINGBOX_MAX, m_WorldBox.Max.x, m_WorldBox.Max.y, m_WorldBox.Max.z);
	fprintf( fp, "\t%s %f %f %f\n", PID_BOUNDINGBOX_MIN, m_WorldBox.Min.x, m_WorldBox.Min.y, m_WorldBox.Min.z);
	fprintf( fp, "}\n");


	// map mesh
	fprintf(fp,"%s",PID_GX_MAP);
	fprintf(fp,"\n");
	fprintf(fp,"%s","{ \n");

	fprintf(fp,"\t %s %s \n",PID_STATIC_MODEL,stm);
	fprintf(fp,"\t %s %s \n",PID_HEIGHT_FIELD,hfl);

	fprintf(fp,"%s","} \n");


	

	// gxobjects
	DWORD dwGXObjectsNum = m_pExecutive->GetGXObjectsNum();
/*
	CItemCount	itemCount;
	itemCount.Initialize(128,4096);
	for (i=0; i<dwGXObjectsNum; i++)
	{
		GXOBJECT_HANDLE	gxo = m_pExecutive->GetGXObjectWithSeqIndex(i);
		char*		pFileName = (char*)m_pExecutive->GetData(gxo);		
		itemCount.Insert((DWORD)gxo,pFileName,lstrlen(pFileName));
	
	}
	GXOBJECT_HANDLE	gxoh[8192];
	fprintf(fp,"%s \t %u",PID_PRELOAD_GXOBJECT,itemCount.GetItemClassNum());
	fprintf(fp,"\n");
	fprintf(fp,"%s","{ \n");

	for (i=0; i<itemCount.GetItemClassNum(); i++)
	{
		DWORD dwNum = itemCount.GetItem((DWORD*)gxoh,i,8192);
		fprintf(fp,"\t %s \t %u \t %f %f %f \t %f %f %f \t %f %f %f %f \t %X \n",

			
		}

	}
*/
	fprintf(fp,"%s \t %u",PID_GX_OBJECT,dwGXObjectsNum);
	fprintf(fp,"\n");
	fprintf(fp,"%s","{ \n");

	

	for (i=0; i<dwGXObjectsNum; i++)
	{
		GXOBJECT_HANDLE	gxo = m_pExecutive->GetGXObjectWithSeqIndex(i);
		if (!gxo)
			__asm int 3

		char*		pFileName = (char*)m_pExecutive->GetData(gxo);		
		if (!pFileName)
			__asm int 3

		VECTOR3		v3Axis,v3Pos,v3Scale;
		float		fRad;

		DWORD		dwID = m_pExecutive->GetID(gxo);
		DWORD		dwFlag = m_pExecutive->GetPropertyFlag(gxo);
		
		
	
		m_pExecutive->GXOGetPosition(gxo,&v3Pos);
		m_pExecutive->GXOGetDirection(gxo,&v3Axis,&fRad);
		m_pExecutive->GXOGetScale(gxo,&v3Scale);


	
		// | 파일명 | 고유ID | 스케일 | 위치 | 축벡터 | 방향각 | 플래그 
		fprintf(fp,"\t %s \t %u \t %f %f %f \t %f %f %f \t %f %f %f %f \t %X \n",
			pFileName,dwID,
			v3Scale.x,v3Scale.y,v3Scale.z,
			v3Pos.x,v3Pos.y,v3Pos.z,
			v3Axis.x,v3Axis.y,v3Axis.z,fRad,dwFlag);

	}	
	fprintf(fp,"%s","} \n");

	// gxlights
	DWORD	dwGXLightsNum = 0;
	for (i=0; i<m_pExecutive->GetGXLightsNum(); i++)
	{
		GXLIGHT_HANDLE	gxl = m_pExecutive->GetGXLightWithSeqIndex(i);
		DWORD		dwFlag = m_pExecutive->GetPropertyFlag(gxl);

		// LOCK속성은 저장하지 않는다.
		dwFlag &= (~GXMAP_OBJECT_COMMON_TYPE_LOCK_TRANSFORM);

		if (dwFlag & GXLIGHT_TYPE_ONLY_USE_TOOL)
			continue;

		dwGXLightsNum++;

	}

	fprintf(fp,"%s \t %u",PID_GX_LIGHT,dwGXLightsNum);

	fprintf(fp,"\n");
	fprintf(fp,"%s","{ \n");
	
	
	for (i=0; i<m_pExecutive->GetGXLightsNum(); i++)
	{
		LIGHT_DESC	lightDesc;

		GXLIGHT_HANDLE	gxl = m_pExecutive->GetGXLightWithSeqIndex(i);
		DWORD		dwFlag = m_pExecutive->GetPropertyFlag(gxl);

		if (dwFlag & GXLIGHT_TYPE_ONLY_USE_TOOL)
			continue;
		

		// | RGBA | 위치 |범위(CM) | 플래그 
		m_pExecutive->GXLGetLightDesc(gxl,&lightDesc);
		fprintf(fp,"\t %X \t %f %f %f \t %f \t %X \n",
			lightDesc.dwDiffuse,lightDesc.v3Point.x,lightDesc.v3Point.y,lightDesc.v3Point.z,lightDesc.fRs,dwFlag);
			
	}
	fprintf(fp,"%s","} \n");

	// gxevents
	DWORD dwGXTriggerNum = m_pExecutive->GetGXEventTriggersNum();
	fprintf(fp,"%s \t %u",PID_GX_TRIGGER,dwGXTriggerNum);
	fprintf(fp,"\n");
	fprintf(fp,"%s","{ \n");

	
	for (i=0; i<dwGXTriggerNum; i++)
	{
		EVENT_TRIGGER_DESC	triggerDesc;
		GXTRIGGER_HANDLE	gxt = m_pExecutive->GetGXEventTriggerWithSeqIndex(i);
		DWORD				dwFlag = m_pExecutive->GetPropertyFlag(gxt);
		DWORD				dwID = m_pExecutive->GetID(gxt);


		m_pExecutive->GXTGetEventTriggerDesc(gxt,&triggerDesc);
		
		
		// 고유아이디 | RGBA | 위치 | 스케일 | 회전 | 플래그 
		fprintf(fp,"\t %u \t %X \t %f %f %f \t %f %f %f \t %f %f %f %x \n",
			dwID,
			triggerDesc.dwColor,
			triggerDesc.v3Pos.x,triggerDesc.v3Pos.y,triggerDesc.v3Pos.z,
			triggerDesc.v3Scale.x,triggerDesc.v3Scale.y,triggerDesc.v3Scale.z,
			triggerDesc.v3Rot.x,triggerDesc.v3Rot.y,triggerDesc.v3Rot.z,
			dwFlag);
	}
	fprintf(fp,"%s","} \n");

	fclose(fp);
	fp = NULL;	

	//trustpak	
	SetCurrentMapFileName(szFileName);
	//
}

void CTool::LoadMap(char* szFileName)
{
	BOOL	bBuildTree	=	FALSE;

	char	buf[_MAX_PATH];

	OnNewMap();

	FILE*	fp;
	fp = fopen(szFileName,"rt");

	while (fscanf(fp,"%s",buf)!=EOF)
	{
		if( !lstrcmp( buf, PID_GX_METADATA))
		{
			while( lstrcmp( buf, "}"))
			{
				fscanf( fp, "%s", buf);
				if( !lstrcmp( buf, PID_BOUNDINGBOX_MAX))
				{
					fscanf( fp, "%f %f %f", &m_WorldBox.Max.x, &m_WorldBox.Max.y, &m_WorldBox.Max.z);
					
				}
				else if( !lstrcmp( buf, PID_BOUNDINGBOX_MIN))
				{
					fscanf( fp, "%f %f %f", &m_WorldBox.Min.x, &m_WorldBox.Min.y, &m_WorldBox.Min.z);
				}

			}
		}
		if (!lstrcmp(buf,PID_GX_MAP))
		{
			while (lstrcmp(buf,"}"))
			{
				fscanf(fp,"%s",buf);
				if (!lstrcmp(buf,PID_STATIC_MODEL))
				{
					fscanf(fp,"%s",buf);
					ImportStaticModel(buf);
					bBuildTree	=	TRUE;
				}
				if (!lstrcmp(buf,PID_HEIGHT_FIELD))
				{
					fscanf(fp,"%s",buf);
					ImportHeightField(buf,0);
					bBuildTree	=	TRUE;
				}


			}
			if (bBuildTree)
				BuildMap(&m_WorldBox);

			
		}

		if (!lstrcmp(buf,PID_GX_OBJECT))
		{
			DWORD	dwGXObjectsNum;
			fscanf(fp,"%u",&dwGXObjectsNum);

			fscanf(fp,"%s",buf);		// {
			for (DWORD i=0; i<dwGXObjectsNum; i++)
			{
				DWORD		dwFlag;
				DWORD		dwID;
				VECTOR3		v3Scale,v3Pos,v3Axis;
				float		fRad;

				// | 파일명 | 고유ID | 스케일 | 위치 | 축벡터 | 방향각 | 플래그 
				fscanf(fp,"%s",buf);		// 파일명
				fscanf(fp,"%u",&dwID);		// 고유ID
				fscanf(fp,"%f %f %f",&v3Scale.x,&v3Scale.y,&v3Scale.z);
				fscanf(fp,"%f %f %f",&v3Pos.x,&v3Pos.y,&v3Pos.z);
				fscanf(fp,"%f %f %f %f",&v3Axis.x,&v3Axis.y,&v3Axis.z,&fRad);
				fscanf(fp,"%x",&dwFlag);
				
				GXOBJECT_HANDLE gxo = m_pExecutive->CreateGXObject(buf,NULL,NULL,dwFlag);
				if (gxo)
				{

					m_pExecutive->SetID(gxo,dwID);
									
					m_pExecutive->GXOSetScale(gxo,&v3Scale);
					m_pExecutive->GXOSetPosition(gxo,&v3Pos,FALSE);
					m_pExecutive->GXOSetDirection(gxo,&v3Axis,fRad);

					char*	pFileName;
					pFileName = (char*)m_pExecutive->GetData(gxo);
					pFileName = new char[lstrlen(buf)+1];
					lstrcpy(pFileName,buf);
					m_pExecutive->SetData(gxo,pFileName);
					g_pRightView->LoadGXObject(pFileName);
				}
			}
			fscanf(fp,"%s",buf);		// }
		}
		if (!lstrcmp(buf,PID_GX_LIGHT))
		{
			DWORD	dwGXLightsNum;
			fscanf(fp,"%u",&dwGXLightsNum);

		
			fscanf(fp,"%s",buf);		// {
			for (DWORD i=0; i<dwGXLightsNum; i++)
			{
				
				LIGHT_DESC	lightDesc;
				memset(&lightDesc,0,sizeof(LIGHT_DESC));
				DWORD		dwFlag;

				// | RGBA | 위치 |범위(CM) | 플래그 
				fscanf(fp,"%x",&lightDesc.dwDiffuse);
				fscanf(fp,"%f %f %f %f",&lightDesc.v3Point.x,&lightDesc.v3Point.y,&lightDesc.v3Point.z,&lightDesc.fRs);
				fscanf(fp,"%x",&dwFlag);
				dwFlag |= GXLIGHT_TYPE_STATIC;

				GXLIGHT_HANDLE	gxl = m_pExecutive->CreateGXLight(&lightDesc,NULL,NULL,0xffffffff,NULL,dwFlag);
			}
			fscanf(fp,"%s",buf);		// }
		}
		if (!lstrcmp(buf,PID_GX_TRIGGER))
		{
			DWORD	dwGXTriggerNum;
			fscanf(fp,"%u",&dwGXTriggerNum);

		
			fscanf(fp,"%s",buf);		// {
			for (DWORD i=0; i<dwGXTriggerNum; i++)
			{
				EVENT_TRIGGER_DESC	triggerDesc;
				memset(&triggerDesc,0,sizeof(triggerDesc));
				DWORD		dwFlag;
				DWORD		dwID;

					
				// 고유아이디 | RGBA | 위치 | 스케일 | 회전 | 플래그 
				fscanf(fp,"%u",&dwID);
				fscanf(fp,"%x",&triggerDesc.dwColor);
				fscanf(fp,"%f %f %f",&triggerDesc.v3Pos.x,&triggerDesc.v3Pos.y,&triggerDesc.v3Pos.z);
				fscanf(fp,"%f %f %f",&triggerDesc.v3Scale.x,&triggerDesc.v3Scale.y,&triggerDesc.v3Scale.z);
				fscanf(fp,"%f %f %f",&triggerDesc.v3Rot.x,&triggerDesc.v3Rot.y,&triggerDesc.v3Rot.z);
				fscanf(fp,"%x",&dwFlag);

				GXTRIGGER_HANDLE	gxt = m_pExecutive->CreateGXEventTrigger(NULL,NULL,dwFlag);
				m_pExecutive->SetID(gxt,dwID);
				m_pExecutive->GXTSetPosition(gxt,&triggerDesc.v3Pos);
				m_pExecutive->GXTSetScale(gxt,&triggerDesc.v3Scale);
				m_pExecutive->GXTSetRotation(gxt,&triggerDesc.v3Rot);
			}
			fscanf(fp,"%s",buf);		// }
				
		}
		
	} 

	fclose(fp);
	fp = NULL;


	//trustpak	
	SetCurrentMapFileName(szFileName);
	m_pTileSetDialog->UpdateTexturesOfTileSetDisplayers();
	//
}

void CTool::SetLightPositionAll( VECTOR3 v3LightAddPos )
{
	DWORD dwGXLightsNum = m_pExecutive->GetGXLightsNum();
	
	LIGHT_DESC	lightDesc;
	VECTOR3		v3NewPos;

	for( DWORD i=0; i<dwGXLightsNum; i++ )
	{
		GXLIGHT_HANDLE gxl = m_pExecutive->GetGXLightWithSeqIndex(i);

		m_pExecutive->GXLGetLightDesc(gxl,&lightDesc);

		v3NewPos = lightDesc.v3Point + v3LightAddPos;
		m_pExecutive->GXLSetPosition( gxl,  &v3NewPos );
	}
}
void CTool::LoadLights(FILE* fp)
{
	DWORD	dwGXLightsNum;
	char	buf[128];

	fscanf(fp,"%s",buf);
	if (lstrcmp(buf,PID_GX_LIGHT))
		return;

	fscanf(fp,"%u",&dwGXLightsNum);
	fscanf(fp,"%s",buf);		// {
	for (DWORD i=0; i<dwGXLightsNum; i++)
	{
		
		LIGHT_DESC	lightDesc;
		memset(&lightDesc,0,sizeof(LIGHT_DESC));
		DWORD		dwFlag;

		// | RGBA | 위치 |범위(CM) | 플래그 
		fscanf(fp,"%x",&lightDesc.dwDiffuse);
		fscanf(fp,"%f %f %f %f",&lightDesc.v3Point.x,&lightDesc.v3Point.y,&lightDesc.v3Point.z,&lightDesc.fRs);
		fscanf(fp,"%x",&dwFlag);

		dwFlag = GXLIGHT_TYPE_STATIC;
		GXLIGHT_HANDLE	gxl = m_pExecutive->CreateGXLight(&lightDesc,NULL,NULL,0xffffffff,NULL,dwFlag);
	}
	fscanf(fp,"%s",buf);		// }
}
void CTool::LoadLights(char* szFileName)
{
	FILE* fp = fopen(szFileName,"rt");
	if (!fp)
		return;

	LoadLights(fp);
	fclose(fp);


}
void CTool::SaveLights(char* szFileName)
{
	FILE* fp = fopen(szFileName,"wt");
	if (!fp)
		return;

	SaveLights(fp);
	fclose(fp);
}



void CTool::LoadGXObjects(FILE* fp)
{

	char	buf[256];

	fscanf(fp,"%s",buf);
	if (lstrcmp(buf,PID_GX_OBJECT))
		return;

	DWORD	dwGXObjectsNum;
	fscanf(fp,"%u",&dwGXObjectsNum);

	fscanf(fp,"%s",buf);		// {
	for (DWORD i=0; i<dwGXObjectsNum; i++)
	{
		DWORD		dwFlag;
		DWORD		dwID;
		VECTOR3		v3Scale,v3Pos,v3Axis;
		float		fRad;

		// | 파일명 | 고유ID | 스케일 | 위치 | 축벡터 | 방향각 | 플래그 
		fscanf(fp,"%s",buf);		// 파일명
		fscanf(fp,"%u",&dwID);		// 고유ID
		fscanf(fp,"%f %f %f",&v3Scale.x,&v3Scale.y,&v3Scale.z);
		fscanf(fp,"%f %f %f",&v3Pos.x,&v3Pos.y,&v3Pos.z);
		fscanf(fp,"%f %f %f %f",&v3Axis.x,&v3Axis.y,&v3Axis.z,&fRad);
		fscanf(fp,"%x",&dwFlag);
		
		GXOBJECT_HANDLE gxo = m_pExecutive->CreateGXObject(buf,NULL,NULL,dwFlag);
		if (gxo)
		{

			m_pExecutive->SetID(gxo,dwID);
							
			m_pExecutive->GXOSetScale(gxo,&v3Scale);
			m_pExecutive->GXOSetPosition(gxo,&v3Pos,FALSE);
			m_pExecutive->GXOSetDirection(gxo,&v3Axis,fRad);

			char*	pFileName;
			pFileName = (char*)m_pExecutive->GetData(gxo);
			pFileName = new char[lstrlen(buf)+1];
			lstrcpy(pFileName,buf);
			m_pExecutive->SetData(gxo,pFileName);
			g_pRightView->LoadGXObject(pFileName);
		}
	}
	fscanf(fp,"%s",buf);		// }
}
void CTool::LoadGXObjects(char* szFileName)
{
	FILE* fp = fopen(szFileName,"rt");
	if (!fp)
		return;

	LoadGXObjects(fp);
	fclose(fp);


}

void CTool::SaveGXObjects(char* szFileName)
{
	FILE* fp = fopen(szFileName,"wt");
	if (!fp)
		return;

	SaveGXObjects(fp);
	fclose(fp);
}


void CTool::SaveGXObjects(FILE* fp)
{

	DWORD dwGXObjectsNum = m_pExecutive->GetGXObjectsNum();

	fprintf(fp,"%s \t %u",PID_GX_OBJECT,dwGXObjectsNum);
	fprintf(fp,"\n");
	fprintf(fp,"%s","{ \n");
	

	for (DWORD i=0; i<dwGXObjectsNum; i++)
	{
		GXOBJECT_HANDLE	gxo = m_pExecutive->GetGXObjectWithSeqIndex(i);
		if (!gxo)
			__asm int 3

		char*		pFileName = (char*)m_pExecutive->GetData(gxo);		
		if (!pFileName)
			__asm int 3

		VECTOR3		v3Axis,v3Pos,v3Scale;
		float		fRad;

		DWORD		dwID = m_pExecutive->GetID(gxo);
		DWORD		dwFlag = m_pExecutive->GetPropertyFlag(gxo);
		
		
	
		m_pExecutive->GXOGetPosition(gxo,&v3Pos);
		m_pExecutive->GXOGetDirection(gxo,&v3Axis,&fRad);
		m_pExecutive->GXOGetScale(gxo,&v3Scale);


	
		// | 파일명 | 고유ID | 스케일 | 위치 | 축벡터 | 방향각 | 플래그 
		fprintf(fp,"\t %s \t %u \t %f %f %f \t %f %f %f \t %f %f %f %f \t %X \n",
			pFileName,dwID,
			v3Scale.x,v3Scale.y,v3Scale.z,
			v3Pos.x,v3Pos.y,v3Pos.z,
			v3Axis.x,v3Axis.y,v3Axis.z,fRad,dwFlag);

	}	
	fprintf(fp,"%s","} \n");
}

void CTool::SaveLights(FILE* fp)
{
	DWORD dwGXLightsNum = m_pExecutive->GetGXLightsNum();

	fprintf(fp,"%s \t %u",PID_GX_LIGHT,dwGXLightsNum);

	fprintf(fp,"\n");
	fprintf(fp,"%s","{ \n");
	
	
	for (DWORD i=0; i<dwGXLightsNum; i++)
	{
		LIGHT_DESC	lightDesc;

		GXLIGHT_HANDLE	gxl = m_pExecutive->GetGXLightWithSeqIndex(i);
		DWORD		dwFlag = m_pExecutive->GetPropertyFlag(gxl);
		
		if (dwFlag & GXLIGHT_TYPE_ONLY_USE_TOOL)
			continue;

		// | RGBA | 위치 |범위(CM) | 플래그 
		m_pExecutive->GXLGetLightDesc(gxl,&lightDesc);
		fprintf(fp,"\t %X \t %f %f %f \t %f \t %x \n",
			lightDesc.dwDiffuse,lightDesc.v3Point.x,lightDesc.v3Point.y,lightDesc.v3Point.z,lightDesc.fRs,dwFlag);
			
	}
	fprintf(fp,"%s","} \n");
}
BOOL CTool::CreateLightFromMod(char* szFileName)
{
	I3DModel*	pModel;
	LIGHT_DESC	lightDesc[1024];
	DWORD		dwLightNum;

	if (0xffffffff == m_pExecutive->GetGeometry()->LoadModel(&pModel,szFileName,0))
		return FALSE;


	dwLightNum = pModel->GetLightList(lightDesc,1024);

	for (DWORD i=0; i<dwLightNum; i++)
	{
		m_pExecutive->CreateGXLight(&lightDesc[i],NULL,0,0,NULL,GXLIGHT_TYPE_STATIC);
	}
	pModel->Release();
	
	Render();

	return TRUE;
}

void CTool::EnableHFieldBlend(BOOL bEnable)
{
	if (m_pHField)
	{
		m_pHField->EnableBlend(bEnable);
		Render();

	}
}

void CTool::LockXAxis()
{
	m_dwMoveAxis = X_AXIS_LOCK;
}
void CTool::LockYAxis()
{
	m_dwMoveAxis = Y_AXIS_LOCK;
}
void CTool::LockZAxis()
{
	m_dwMoveAxis = Z_AXIS_LOCK;
}
void CTool::ResetHeight(float h)
{
	if (m_pHField)
	{
		if (m_dwSelectedVertexNum)
		{
			if (m_pHField->AdjustHeight(m_pSelectedVertexIndex,m_dwSelectedVertexNum,h,HFIELD_ADJUST_TYPE_SET))
				Render();
		}
		else
		{
			m_pHField->ResetHeight(h);
			Render();
		}
	}
}

/*
void CTool::MakeHeightFieldHigh(DWORD dwX, DWORD dwZ, float fHeight, float fRadius)
{
	if (!m_pHField)
		return;

	HFIELD_DESC	hfDesc;
	m_pHField->GetHFieldDesc(&hfDesc);

	int		xnum,ynum;
	RECT	rect;		// 검사할 범위.

	VECTOR3 Fact;
	VECTOR3	PickFact;
	
	PickFact.y = hfDesc.pzfList[dwX+dwZ*hfDesc.dwZFNumX];
	m_pHField->CalcXZ(&PickFact.x,&PickFact.z,dwX,dwZ);

	ynum = xnum = (int)( (fRadius / hfDesc.fFaceSize) + 0.5f );
	rect.left = (int)dwX - xnum;
	rect.right = (int)dwX + xnum;
	rect.top = (int)dwZ - ynum;
	rect.bottom = (int)dwZ + ynum;

	if (rect.left < 0)
		rect.left = 0;
	
	if (rect.top < 0)
		rect.top = 0;
	
	if (rect.right > hfDesc.dwZFNumX)
		rect.right = hfDesc.dwZFNumX;

	if (rect.bottom > hfDesc.dwZFNumZ)
		rect.bottom = hfDesc.dwZFNumZ;

	for(int j = rect.top; j < rect.bottom; j++)
	{
		for(int i = rect.left; i < rect.right; i++)
		{
			Fact.y = hfDesc.pzfList[i + j*hfDesc.dwZFNumX];
			m_pHField->CalcXZ(&Fact.x,&Fact.z,i,j);
			// 거리 체크.
//			float	fTemp	=	(Fact.x-hfDesc.pzfList[dwX+dwZ*hfDesc.dwZFNumX].x)*(Fact.x-hfDesc.pzfList[dwX+dwZ*hfDesc.dwZFNumX].x) + (Fact.y-hfDesc.pzfList[dwX+dwZ*hfDesc.dwZFNumX].y)*(Fact.y-hfDesc.pzfList[dwX+dwZ*hfDesc.dwZFNumX].y) + (Fact.z-hfDesc.pzfList[dwX+dwZ*hfDesc.dwZFNumX].z)*(Fact.z-hfDesc.pzfList[dwX+dwZ*hfDesc.dwZFNumX].z);
			float	fTemp2	=	(Fact.x-PickFact.x)*(Fact.x-PickFact.x) + (Fact.z-PickFact.z)*(Fact.z-PickFact.z);
			
			if( fTemp2 > fRadius*fRadius)
				continue;

			// 실제로 지형을 올린다.
			float	fDist = (float)sqrt(fTemp2);
			Fact.y = Fact.y + (cos( 3.1415926f * fDist / fRadius)+1.0f)*fHeight/2;
			hfDesc.pzfList[i + j*hfDesc.dwZFNumX]	=	Fact.y;
			
		}
	}

	m_pHField->Update(HEIGHT_FIELD_UPDATE_TYPE_VERTEX_POS);
//	m_pHField->ResetCollisionMesh();

	Render();


}*/


void CTool::ResetCamera(DWORD dwViewportIndex)
{
	VECTOR3		v3Up;
	VECTOR3		v3To;
	VECTOR3		v3From;

	v3Up.x = 0.0f;
	v3Up.y = 1.0f;
	v3Up.z = 0.0f;
	v3To.x = 0.0f;
	v3To.y = 0.0f;
	v3To.z = 1000.0f;
	v3From.x = 0.0f;
	v3From.y = 0.0f;
	v3From.z = 0.0f;
	m_pExecutive->GetGeometry()->ResetCamera(&v3From,DEFAULT_NEAR,DEFAULT_FAR,DEFAULT_FOV,dwViewportIndex);
//	m_pExecutive->GetGeometry()->ResetCamera(&v3From,DEFAULT_NEAR,5000.0f,DEFAULT_FOV,dwViewportIndex);

}

//-----------------------------------------------------------------------------------------//
// In
void CTool::OnSectionSplitterDown( UINT nFlags, POINT *pt )
{
	if( !m_pHField ) return;

	HFIELD_DESC hDesc;
	m_pHField->GetHFieldDesc(&hDesc);
	
	DWORD		dwPosX,dwPosY;
	float		fDist;
	
	if (m_pHField->GetTilePosWithScreenCoord(&m_v3Clicked,&fDist,&dwPosX,&dwPosY,pt,m_dwCurrentViewportIndex))
	{
		SetRect( &m_pSectionSplitter->rcCurSection, 
				 dwPosX, 
				 dwPosY,
				 dwPosX,
				 dwPosY);

		m_pSectionSplitter->clickPos.x = dwPosX;
		m_pSectionSplitter->clickPos.y = dwPosY;
	}
}

void CTool::OnSpotSplitterDown( UINT nFlags, POINT *pt )
{
	if( !m_pHField ) return;

	HFIELD_DESC hDesc;
	m_pHField->GetHFieldDesc(&hDesc);
	
	DWORD		dwPosX,dwPosY;
	float		fDist;
	
	if (m_pHField->GetTilePosWithScreenCoord(&m_v3Clicked,&fDist,&dwPosX,&dwPosY,pt,m_dwCurrentViewportIndex))
	{
		SetRect( &m_pSpotSplitter->rcCurSection, 
				 dwPosX, 
				 dwPosY,
				 dwPosX,
				 dwPosY);

		m_pSpotSplitter->clickPos.x = dwPosX;
		m_pSpotSplitter->clickPos.y = dwPosY;
	}
}

void CTool::OnSectionSplitterMove( UINT nFlags, POINT *pt )
{
	if( !m_pHField ) return;

	DWORD		dwPosX,dwPosY; 
	float		fDist;
	if( nFlags & MK_LBUTTON )
	{
		if (m_pHField->GetTilePosWithScreenCoord(&m_v3Clicked,&fDist,&dwPosX,&dwPosY,pt,m_dwCurrentViewportIndex))
		{
			dwPosX = (DWORD)(m_v3Clicked.x / 125.0f );
			dwPosY = (DWORD)(m_v3Clicked.z / 125.0f );

			// 바로 전 세션의 범위를 저장한다.			
			m_pSectionSplitter->rcOldSection = m_pSectionSplitter->rcCurSection;

			// 현재의 세션을 저장한다.	( 수치의 역전이 일어날경우 보정하는것이다. )
			if( m_pSectionSplitter->clickPos.x >= (LONG)dwPosX ) 
			{
				m_pSectionSplitter->rcCurSection.right = m_pSectionSplitter->clickPos.x;
				m_pSectionSplitter->rcCurSection.left = dwPosX;
			}
			else 
			{
				m_pSectionSplitter->rcCurSection.left = m_pSectionSplitter->clickPos.x;
				m_pSectionSplitter->rcCurSection.right = dwPosX;
			}
			
			if( m_pSectionSplitter->clickPos.y >= (LONG)dwPosY ) 
			{
				m_pSectionSplitter->rcCurSection.bottom = m_pSectionSplitter->clickPos.y;
				m_pSectionSplitter->rcCurSection.top = dwPosY;
			}
			else 
			{
				m_pSectionSplitter->rcCurSection.top = m_pSectionSplitter->clickPos.y;
				m_pSectionSplitter->rcCurSection.bottom = dwPosY;
			}

			// 여기서 좌표가 변했는지 감시 체크한다.
			if( m_pSectionSplitter->rcOldSection.top == m_pSectionSplitter->rcCurSection.top &&
				m_pSectionSplitter->rcOldSection.left == m_pSectionSplitter->rcCurSection.left &&
				m_pSectionSplitter->rcOldSection.right == m_pSectionSplitter->rcCurSection.right &&
				m_pSectionSplitter->rcOldSection.bottom == m_pSectionSplitter->rcCurSection.bottom ) return;
			
			int x, z;
			int zLen = m_pSectionSplitter->rcOldSection.bottom - m_pSectionSplitter->rcOldSection.top + 1;
			int xLen = m_pSectionSplitter->rcOldSection.right - m_pSectionSplitter->rcOldSection.left + 1;
			
			HFIELD_DESC hDesc;
			m_pHField->GetHFieldDesc(&hDesc);
			
			// 이전의 속성을 저장할 메모리 공간을 잡는다.
			if(m_pSectionSplitter->pwCurTNum) 
			{
				// 이때 일단 이전 속성으로 돌리기를 한다.
				for( z = 0; z <zLen; z++ )
				{
					for( x = 0; x<xLen; x++ )
					{
						if( z == 0 || z == ( zLen - 1 ) || x == 0 || x == ( xLen - 1 ) )
						{
							m_pHField->SetTile( 
								m_pSectionSplitter->rcOldSection.left + x, m_pSectionSplitter->rcOldSection.top + z, 
								m_pSectionSplitter->pwCurTNum[ xLen * z + x ] );
						}
					}
				}

				delete[] m_pSectionSplitter->pwCurTNum;
				m_pSectionSplitter->pwCurTNum = NULL;	
			}
			
			// 새로운 세션범위를 넣는다. 
			zLen = m_pSectionSplitter->rcCurSection.bottom - m_pSectionSplitter->rcCurSection.top + 1;
			xLen = m_pSectionSplitter->rcCurSection.right - m_pSectionSplitter->rcCurSection.left + 1;
			
			m_pSectionSplitter->wCurSectionTileMany = zLen * xLen;
			m_pSectionSplitter->pwCurTNum = new WORD[  m_pSectionSplitter->wCurSectionTileMany  ];
			
			for( z = 0; z <zLen; z++ )
			{
				for( x = 0; x<xLen; x++ )
				{
					m_pSectionSplitter->pwCurTNum[ xLen * z + x ] 
						= hDesc.pwTileTable[ hDesc.dwTileNumX * (m_pSectionSplitter->rcCurSection.top + z) + m_pSectionSplitter->rcCurSection.left + x ];
					
					if( z == 0 || z == ( zLen - 1 ) || x == 0 || x == ( xLen - 1 ) )
					{
						m_pHField->SetTile( m_pSectionSplitter->rcCurSection.left + x, m_pSectionSplitter->rcCurSection.top + z, 8 );
					}
				}
			}

			Render();
		}
	}
}


void CTool::OnSpotSplitterMove( UINT nFlags, POINT *pt )
{
	if( !m_pHField ) return;

	DWORD		dwPosX,dwPosY; 
	float		fDist;
	if( nFlags & MK_LBUTTON )
	{
		if (m_pHField->GetTilePosWithScreenCoord(&m_v3Clicked,&fDist,&dwPosX,&dwPosY,pt,m_dwCurrentViewportIndex))
		{
			dwPosX = (DWORD)(m_v3Clicked.x / 125.0f );
			dwPosY = (DWORD)(m_v3Clicked.z / 125.0f );

			// 바로 전 세션의 범위를 저장한다.			
			m_pSpotSplitter->rcOldSection = m_pSpotSplitter->rcCurSection;

			// 현재의 세션을 저장한다.	( 수치의 역전이 일어날경우 보정하는것이다. )
			if( m_pSpotSplitter->clickPos.x >= (LONG)dwPosX ) 
			{
				m_pSpotSplitter->rcCurSection.right = m_pSpotSplitter->clickPos.x;
				m_pSpotSplitter->rcCurSection.left = dwPosX;
			}
			else 
			{
				m_pSpotSplitter->rcCurSection.left = m_pSpotSplitter->clickPos.x;
				m_pSpotSplitter->rcCurSection.right = dwPosX;
			}
			
			if( m_pSpotSplitter->clickPos.y >= (LONG)dwPosY ) 
			{
				m_pSpotSplitter->rcCurSection.bottom = m_pSpotSplitter->clickPos.y;
				m_pSpotSplitter->rcCurSection.top = dwPosY;
			}
			else 
			{
				m_pSpotSplitter->rcCurSection.top = m_pSpotSplitter->clickPos.y;
				m_pSpotSplitter->rcCurSection.bottom = dwPosY;
			}

			// 여기서 좌표가 변했는지 감시 체크한다.
			if( m_pSpotSplitter->rcOldSection.top == m_pSpotSplitter->rcCurSection.top &&
				m_pSpotSplitter->rcOldSection.left == m_pSpotSplitter->rcCurSection.left &&
				m_pSpotSplitter->rcOldSection.right == m_pSpotSplitter->rcCurSection.right &&
				m_pSpotSplitter->rcOldSection.bottom == m_pSpotSplitter->rcCurSection.bottom ) return;
			
			int x, z;
			int zLen = m_pSpotSplitter->rcOldSection.bottom - m_pSpotSplitter->rcOldSection.top + 1;
			int xLen = m_pSpotSplitter->rcOldSection.right - m_pSpotSplitter->rcOldSection.left + 1;
			
			HFIELD_DESC hDesc;
			m_pHField->GetHFieldDesc(&hDesc);
			
			// 이전의 속성을 저장할 메모리 공간을 잡는다.
			if(m_pSpotSplitter->pwCurTNum) 
			{
				// 이때 일단 이전 속성으로 돌리기를 한다.
				for( z = 0; z <zLen; z++ )
				{
					for( x = 0; x<xLen; x++ )
					{
						//if( z == 0 || z == ( zLen - 1 ) || x == 0 || x == ( xLen - 1 ) )
						//{
							m_pHField->SetTile( 
								m_pSpotSplitter->rcOldSection.left + x, m_pSpotSplitter->rcOldSection.top + z, 
								m_pSpotSplitter->pwCurTNum[ xLen * z + x ] );
						//}
					}
				}

				delete[] m_pSpotSplitter->pwCurTNum;
				m_pSpotSplitter->pwCurTNum = NULL;	
			}
			
			// 새로운 세션범위를 넣는다. 
			zLen = m_pSpotSplitter->rcCurSection.bottom - m_pSpotSplitter->rcCurSection.top + 1;
			xLen = m_pSpotSplitter->rcCurSection.right - m_pSpotSplitter->rcCurSection.left + 1;
			
			m_pSpotSplitter->wCurSectionTileMany = zLen * xLen;
			m_pSpotSplitter->pwCurTNum = new WORD[  m_pSpotSplitter->wCurSectionTileMany  ];
			

			WORD wSpotID = m_pSpotSplitter->pSectionList->GetCount();	//새로 할당할 SpotID + 1이 되겠지..

			for( z = 0; z <zLen; z++ )
			{
				for( x = 0; x<xLen; x++ )
				{
					m_pSpotSplitter->pwCurTNum[ xLen * z + x ] 
						= hDesc.pwTileTable[ hDesc.dwTileNumX * (m_pSpotSplitter->rcCurSection.top + z) + m_pSpotSplitter->rcCurSection.left + x ];
					
					//if( z == 0 || z == ( zLen - 1 ) || x == 0 || x == ( xLen - 1 ) )
					//{
						m_pHField->SetTile( m_pSpotSplitter->rcCurSection.left + x, m_pSpotSplitter->rcCurSection.top + z, MOVE_SPOT_START_TILE_NO + wSpotID );
					//}
					
				}
			}

			Render();
		}
	}
}


void CTool::OnSectionSplitterUp( UINT nFlags, POINT *pt )
{
	if( !m_pHField ) return;

	Section_Info*	pSec_Info ;			// Section Info 생성.
	pSec_Info	= new Section_Info;		

	pSec_Info->rcSection = m_pSectionSplitter->rcCurSection;
	pSec_Info->pwTNum = new WORD[ m_pSectionSplitter->wCurSectionTileMany ];
	memcpy( pSec_Info->pwTNum, m_pSectionSplitter->pwCurTNum, sizeof( WORD ) * m_pSectionSplitter->wCurSectionTileMany );
	
	//030226
	delete [] m_pSectionSplitter->pwCurTNum;
	m_pSectionSplitter->pwCurTNum = NULL;
	//--
	
	if( pSec_Info->rcSection.bottom - pSec_Info->rcSection.top == 0 ) 
	{
		DeleteSection( pSec_Info );
		delete pSec_Info;
		return;
	}
	if( pSec_Info->rcSection.right - pSec_Info->rcSection.left == 0 ) 
	{
		DeleteSection( pSec_Info );
		delete pSec_Info;
		return;
	}
	
	m_pSectionSplitter->pSectionList->AddTail( pSec_Info );
}

void CTool::OnSpotSplitterUp( UINT nFlags, POINT *pt )
{
	if( !m_pHField ) return;

	Section_Info*	pSec_Info;			// Section Info 생성.
	pSec_Info	= new Section_Info;

	pSec_Info->rcSection = m_pSpotSplitter->rcCurSection;
	pSec_Info->pwTNum = new WORD[ m_pSpotSplitter->wCurSectionTileMany ];
	memcpy( pSec_Info->pwTNum, m_pSpotSplitter->pwCurTNum, sizeof( WORD ) * m_pSpotSplitter->wCurSectionTileMany );

	//030226
	delete [] m_pSpotSplitter->pwCurTNum;
	m_pSpotSplitter->pwCurTNum = NULL;
	//--
	
	if( pSec_Info->rcSection.bottom - pSec_Info->rcSection.top == 0 ) 
	{
		DeleteSection( pSec_Info );
		delete pSec_Info;
		return;
	}
	if( pSec_Info->rcSection.right - pSec_Info->rcSection.left == 0 ) 
	{
		DeleteSection( pSec_Info );
		delete pSec_Info;
		return;
	}
	
	m_pSpotSplitter->pSectionList->AddTail( pSec_Info );
}

void CTool::EnableSectionSplitter()
{
	POSITION_		pos;
	Section_Info*	pSection_Info;
	int				x, z, zLen, xLen;
	
	HFIELD_DESC hDesc;
	m_pHField->GetHFieldDesc(&hDesc);
			
	pos = m_pSectionSplitter->pSectionList->GetHeadPosition();
	
	while( pos )
	{
		pSection_Info = (Section_Info*)m_pSectionSplitter->pSectionList->GetNext( pos );

		zLen = pSection_Info->rcSection.bottom - pSection_Info->rcSection.top + 1;
		xLen = pSection_Info->rcSection.right - pSection_Info->rcSection.left + 1;

		for( z = 0; z <zLen; z++ )
		{
			for( x = 0; x<xLen; x++ )
			{
				pSection_Info->pwTNum[ xLen * z + x ] 
					= hDesc.pwTileTable[ hDesc.dwTileNumX * (pSection_Info->rcSection.top + z) + pSection_Info->rcSection.left + x ];
				
				if( z == 0 || z == ( zLen - 1 ) || x == 0 || x == ( xLen - 1 ) )
				{
					m_pHField->SetTile( pSection_Info->rcSection.left + x, pSection_Info->rcSection.top + z, 8 );
				}
			}
		}
	}

	Render();
}

void CTool::DisableSectionSplitter()
{
	POSITION_		pos;
	Section_Info*	pSection_Info;
	int				x, z, zLen, xLen;

	pos = m_pSectionSplitter->pSectionList->GetHeadPosition();

	while( pos )
	{
		pSection_Info = (Section_Info*)m_pSectionSplitter->pSectionList->GetNext( pos );

		zLen = pSection_Info->rcSection.bottom - pSection_Info->rcSection.top + 1;
		xLen = pSection_Info->rcSection.right - pSection_Info->rcSection.left + 1;
		
		// 사용중에 여기에 들어오면 메모리를 해지하지 않아 메모리 공간이 있겠지만.
		// 로드한경우에는 메모리 공간이 없음으로 여기서 체크해서 공간을 만들어준다.
		// 로드 당시 메모리를 만들어줄수도 있지만.. 걍 이렇게 할련다..
		if( pSection_Info->pwTNum == NULL )
		{
			pSection_Info->pwTNum = new WORD[ zLen * xLen ];
		}

		for( z = 0; z <zLen; z++ )
		{
			for( x = 0; x<xLen; x++ )
			{
				if( z == 0 || z == ( zLen - 1 ) || x == 0 || x == ( xLen - 1 ) )
				{
					m_pHField->SetTile( pSection_Info->rcSection.left + x, pSection_Info->rcSection.top + z, pSection_Info->pwTNum[ xLen * z + x ] );
				}
			}
		}
	}

	Render();
}


void CTool::EnableSpotSplitter()
{
	POSITION_		pos;
	Section_Info*	pSection_Info;
	int				x, z, zLen, xLen;
	
	HFIELD_DESC hDesc;
	m_pHField->GetHFieldDesc(&hDesc);
			
	pos = m_pSpotSplitter->pSectionList->GetHeadPosition();
	
	int cnt = 0;
	while( pos )
	{
		pSection_Info = (Section_Info*)m_pSpotSplitter->pSectionList->GetNext( pos );

		zLen = pSection_Info->rcSection.bottom - pSection_Info->rcSection.top + 1;
		xLen = pSection_Info->rcSection.right - pSection_Info->rcSection.left + 1;

		for( z = 0; z <zLen; z++ )
		{
			for( x = 0; x<xLen; x++ )
			{
				pSection_Info->pwTNum[ xLen * z + x ] 
					= hDesc.pwTileTable[ hDesc.dwTileNumX * (pSection_Info->rcSection.top + z) + pSection_Info->rcSection.left + x ];
				
				m_pHField->SetTile( pSection_Info->rcSection.left + x, pSection_Info->rcSection.top + z, MOVE_SPOT_START_TILE_NO + cnt );
			}
		}

		cnt++;
	}

	Render();
}

void CTool::DisableSpotSplitter()
{
	POSITION_		pos;
	Section_Info*	pSection_Info;
	int				x, z, zLen, xLen;

	pos = m_pSpotSplitter->pSectionList->GetHeadPosition();

	while( pos )
	{
		pSection_Info = (Section_Info*)m_pSpotSplitter->pSectionList->GetNext( pos );

		zLen = pSection_Info->rcSection.bottom - pSection_Info->rcSection.top + 1;
		xLen = pSection_Info->rcSection.right - pSection_Info->rcSection.left + 1;
		
		// 사용중에 여기에 들어오면 메모리를 해지하지 않아 메모리 공간이 있겠지만.
		// 로드한경우에는 메모리 공간이 없음으로 여기서 체크해서 공간을 만들어준다.
		// 로드 당시 메모리를 만들어줄수도 있지만.. 걍 이렇게 할련다..
		if( pSection_Info->pwTNum == NULL )
		{
			pSection_Info->pwTNum = new WORD[ zLen * xLen ];
		}

		for( z = 0; z <zLen; z++ )
		{
			for( x = 0; x<xLen; x++ )
			{
				m_pHField->SetTile( pSection_Info->rcSection.left + x, pSection_Info->rcSection.top + z, pSection_Info->pwTNum[ xLen * z + x ] );
			}
		}

		
	}

	Render();
}


void CTool::DeleteSection( Section_Info* pSection_Info )
{
	if( pSection_Info == NULL ) return;
	
	int				x, z, zLen, xLen;
	
	zLen = pSection_Info->rcSection.bottom - pSection_Info->rcSection.top + 1;
	xLen = pSection_Info->rcSection.right - pSection_Info->rcSection.left + 1;

	for( z = 0; z <zLen; z++ )
	{
		for( x = 0; x<xLen; x++ )
		{
			if( z == 0 || z == ( zLen - 1 ) || x == 0 || x == ( xLen - 1 ) )
			{
				m_pHField->SetTile( pSection_Info->rcSection.left + x, pSection_Info->rcSection.top + z, pSection_Info->pwTNum[ xLen * z + x ] );
			}
		}
	}
	
	Render();
	
	POSITION_		pos;
	
	pos = m_pSectionSplitter->pSectionList->Find( pSection_Info, NULL );
	if( !pos ) return;
	
	m_pSectionSplitter->pSectionList->RemoveAt( pos );
}


//Added by KBS
void CTool::DeleteSpot( Section_Info* pSection_Info )
{
	if( pSection_Info == NULL ) return;
	
	int				x, z, zLen, xLen;
	
	zLen = pSection_Info->rcSection.bottom - pSection_Info->rcSection.top + 1;
	xLen = pSection_Info->rcSection.right - pSection_Info->rcSection.left + 1;

	for( z = 0; z <zLen; z++ )
	{
		for( x = 0; x<xLen; x++ )
		{
			//if( z == 0 || z == ( zLen - 1 ) || x == 0 || x == ( xLen - 1 ) )
			//{
				m_pHField->SetTile( pSection_Info->rcSection.left + x, pSection_Info->rcSection.top + z, pSection_Info->pwTNum[ xLen * z + x ] );
			//}
		}
	}
	

	
	POSITION_		pos;
	
	pos = m_pSpotSplitter->pSectionList->Find( pSection_Info, NULL );
	if( !pos ) return;
	
	m_pSpotSplitter->pSectionList->RemoveAt( pos );


	Section_Info* pSection_Info2;
	pos = m_pSpotSplitter->pSectionList->GetHeadPosition();
	
	int cnt = 0;
	while( pos )
	{
		pSection_Info2 = (Section_Info*)m_pSpotSplitter->pSectionList->GetNext( pos );
		

		zLen = pSection_Info2->rcSection.bottom - pSection_Info2->rcSection.top + 1;
		xLen = pSection_Info2->rcSection.right - pSection_Info2->rcSection.left + 1;

		for( z = 0; z <zLen; z++ )
		{
			for( x = 0; x<xLen; x++ )
			{
				m_pHField->SetTile( pSection_Info2->rcSection.left + x, pSection_Info2->rcSection.top + z, MOVE_SPOT_START_TILE_NO + cnt );
			}
		}

		cnt++;
		pSection_Info2->wSectionNum	= cnt;		//새로운 ID로 갱신 
		
	}

	Render();
}
//--

Section_Info* CTool::GetSectionInfo()
{
	DWORD		dwPosX,dwPosY;

	POSITION_		pos;
	Section_Info*	pSection_Info;
	POINT			ClickPos;
	float		fDist;

	if (!m_pHField->GetTilePosWithScreenCoord(&m_v3Clicked,&fDist,&dwPosX,&dwPosY,&m_pSectionSplitter->mousePos,m_dwCurrentViewportIndex))
		return NULL;
	
	ClickPos.x	=	dwPosX;
	ClickPos.y	=	dwPosY;
	
	pos = m_pSectionSplitter->pSectionList->GetHeadPosition();
	
	int cnt = 0;

	while( pos )
	{
		pSection_Info = (Section_Info*)m_pSectionSplitter->pSectionList->GetNext( pos );
		cnt++;
		
		// PtInRect 함수가 끝선에서는 인정을 하지 않음으로 일단 노가다 코드 작성.
		//if( PtInRect( &pSection_Info->rcSection, ClickPos ) )
		if( pSection_Info->rcSection.left <= ClickPos.x && pSection_Info->rcSection.right >= ClickPos.x &&
			pSection_Info->rcSection.top <= ClickPos.y && pSection_Info->rcSection.bottom >= ClickPos.y )
		{
			pSection_Info->wSectionNum	= cnt;
			return pSection_Info;
		}
	}

	return NULL;
}

//Added by KBS
Section_Info* CTool::GetSpotInfo()
{
	DWORD		dwPosX,dwPosY;

	POSITION_		pos;
	Section_Info*	pSection_Info;
	
	POINT			ClickPos;
	float		fDist;

	if (!m_pHField->GetTilePosWithScreenCoord(&m_v3Clicked,&fDist,&dwPosX,&dwPosY,&m_pSpotSplitter->mousePos,m_dwCurrentViewportIndex))
		return NULL;
	
	ClickPos.x	=	dwPosX;
	ClickPos.y	=	dwPosY;
	
	pos = m_pSpotSplitter->pSectionList->GetHeadPosition();
	
	int cnt = 0;

	while( pos )
	{
		pSection_Info = (Section_Info*)m_pSpotSplitter->pSectionList->GetNext( pos );
		cnt++;
		
		// PtInRect 함수가 끝선에서는 인정을 하지 않음으로 일단 노가다 코드 작성.
		//if( PtInRect( &pSection_Info->rcSection, ClickPos ) )
		if( pSection_Info->rcSection.left <= ClickPos.x && pSection_Info->rcSection.right >= ClickPos.x &&
			pSection_Info->rcSection.top <= ClickPos.y && pSection_Info->rcSection.bottom >= ClickPos.y )
		{
			pSection_Info->wSectionNum	= cnt;
			return pSection_Info;
		}
	}

	return NULL;
}
//--

BOOL CTool::CreateNPC( Npc_Info* pNpcInfo )
{
	m_pNpcInfoList->AddTail( pNpcInfo );

	return TRUE;
}

BOOL CTool::Loadttb( const char* szFileName )
{
	FILE*	fp;

	fp = fopen( szFileName, "rb" );
	if( !fp ) return FALSE;
	
	// 타일 파렛이 있는지 검사한다.
	m_pPalette->GetTextureTable(&m_hfcreate.pTexTable,&m_hfcreate.dwTileTextureNum);	
	if (!m_hfcreate.dwTileTextureNum)
	{
		::MessageBox(NULL,"타일 팔레트를 먼저 생성해주세요","Error",MB_OK);
		return FALSE;
	}

	// 일단 사용된 모든 ttb의 변수를 지운다. 함수로 만들어 놓고 싶었지만.. 걍 일단.. 하자.. 
	CleanHeightField();

	if( m_pSectionSplitter )
	{
		delete m_pSectionSplitter;
		m_pSectionSplitter = NULL;
	}
	m_pSectionSplitter = new SectionSplitter;

	if( m_pNpcInfoList )
	{
		delete m_pNpcInfoList;
		m_pNpcInfoList = NULL;
	}
	m_pNpcInfoList = new CNpcInfoList;

	if( m_pSpotSplitter ) 
	{
		delete m_pSpotSplitter;
		m_pSpotSplitter = NULL;
	}
	m_pSpotSplitter = new SectionSplitter;

	if( m_pStartSplitter ) 
	{
		delete m_pStartSplitter;
		m_pStartSplitter = NULL;
	}
	m_pStartSplitter = new SectionSplitter;

	// 로드를 시작하자..
	DWORD	dwX, dwZ, dwFaceSize, dwFacePerObj;
	WORD*	pwTileAttr;	
	
	fread( &dwX, sizeof( DWORD ), 1, fp );
	fread( &dwZ, sizeof( DWORD ), 1, fp );
	fread( &dwFaceSize, sizeof( DWORD ), 1, fp );
	fread( &dwFacePerObj, sizeof( DWORD ), 1, fp );
	pwTileAttr	= new WORD[ dwX * dwZ ];
	fread( pwTileAttr, sizeof( WORD ) * dwX * dwZ, 1, fp );
	
	// 여기는 일단 지정해놨다. 뭐 타일 사이즈랑 그런것은 게임에서 고장된 것임으로 ... define 은 나중에..
	m_hfcreate.fFaceSize			= (float)dwFaceSize;

	m_hfcreate.left					= 0.0f;
	m_hfcreate.right				= (float)( dwX * m_hfcreate.fFaceSize );
	m_hfcreate.top					= 0.0f;
	m_hfcreate.bottom				= (float)( dwZ * m_hfcreate.fFaceSize );

	m_hfcreate.dwFacesNumPerObjAxis = dwFacePerObj;
	m_hfcreate.dwObjNumX			= (DWORD)( m_hfcreate.right / m_hfcreate.fFaceSize / m_hfcreate.dwFacesNumPerObjAxis );
	m_hfcreate.dwObjNumZ			= (DWORD)( m_hfcreate.bottom / m_hfcreate.fFaceSize / m_hfcreate.dwFacesNumPerObjAxis );
	m_hfcreate.dwDetailLevelNum		= 1;
	m_hfcreate.dwIndexBufferNumLV0	= 12;

	m_pExecutive->GetGeometry()->CreateHeightField(&m_pHField,0);
	m_pHField->Create(&m_hfcreate,HFIELD_MODEL_LOAD_ENABLE_SHADE | HFIELD_MODEL_LOAD_ENABLE_DRAW_ALPHAMAP);
	
	ResetDefaultCamera(m_dwCurrentViewportIndex);

//	m_pExecutive->GetRenderer()->SetLightMapFlag(0);
	m_pHField->GetHFieldDesc(&m_hfDesc);
	m_bIsHFieldCreateMode = TRUE;

	// 파일 안의 속성에 타라 속성을 바꾸어준다..
	int x, z;
	for( z = 0; z<m_hfDesc.dwTileNumZ; z++ )
	{
		for( x = 0; x<m_hfDesc.dwTileNumX; x++ )
		{
			m_pHField->SetTile( x, z, pwTileAttr[m_hfDesc.dwTileNumX*z+x] );
		}
	}
	
	// 이제 링크정보를 불러들인다.
	WORD wMany;
	Section_Link_Info SectionLinkInfo;
	Section_Info*	pSectionInfo;

	fread( &wMany, sizeof( WORD ), 1, fp );
	for( x=0; x<wMany; x++ )
	{
		fread( &SectionLinkInfo, sizeof( Section_Link_Info ), 1, fp );
		
		pSectionInfo = new Section_Info;
		pSectionInfo->rcSection.left = SectionLinkInfo.x1;
		pSectionInfo->rcSection.top = SectionLinkInfo.y1;
		pSectionInfo->rcSection.right = SectionLinkInfo.x2;
		pSectionInfo->rcSection.bottom = SectionLinkInfo.y2;
		// 타일정보 저장을 위한 메모리를 생성한다.
		pSectionInfo->pwTNum = new WORD[ ( abs(pSectionInfo->rcSection.bottom - pSectionInfo->rcSection.top) + 1 ) *
										 ( abs(pSectionInfo->rcSection.right - pSectionInfo->rcSection.left) + 1 ) ];
		m_pSectionSplitter->pSectionList->AddTail( pSectionInfo );
	}
	
	// 이번엔 몬스터 정보이다.
	Npc_Info*	pNpcInfo;
	fread( &wMany, sizeof( WORD ), 1, fp );
	m_pNpcInfoList->RemoveAll();
	for( x=0; x<wMany; x++ )
	{
		pNpcInfo = new Npc_Info;
		fread( pNpcInfo, sizeof( Npc_Info ), 1, fp );
		m_pNpcInfoList->AddTail( pNpcInfo );
	}
	
	//--------------------------------------------------------------------------------------------------------
	//--								여기서부터 MoveSpot 불러오기 	(Added by KBS)						--
	//--------------------------------------------------------------------------------------------------------
	WORD	wSpotNum;
	Section_Info*	pSpotInfo;
	int i=0;

	fread( &wSpotNum, sizeof( WORD ), 1, fp );	//MoveSpot의 저장된 갯수 

	if(wSpotNum > MAX_MOVE_SPOT_NUM)
	{
		MessageBox(NULL, "Fail to load MoveSpot Info!", NULL, NULL);
		goto lb_Exit;
	}

	for(i=0; i<wSpotNum; i++)
	{
		pSpotInfo = new Section_Info;
		fread( &pSpotInfo->wSectionNum, sizeof( WORD ), 1, fp );	//MoveSpot ID
		fread( &pSpotInfo->rcSection, sizeof( RECT ), 1, fp );		//MoveSpot Rect

		pSpotInfo->pwTNum = new WORD[ ( abs(pSpotInfo->rcSection.bottom - pSpotInfo->rcSection.top) + 1 ) *
										 ( abs(pSpotInfo->rcSection.right - pSpotInfo->rcSection.left) + 1 ) ];

		m_pSpotSplitter->pSectionList->AddTail( pSpotInfo );
	}

	//---------------------------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------------------------
	//--								여기서부터 StartSpot 불러오기 	(Added by KBS)						--
	//--------------------------------------------------------------------------------------------------------
	fread( &wSpotNum, sizeof( WORD ), 1, fp );	//StartSpot의 저장된 갯수 

	if(wSpotNum > MAX_START_SPOT_NUM)
	{
		MessageBox(NULL, "Fail to load StartSpot Info!", NULL, NULL);
		goto lb_Exit;
	}

	for(i=0; i<wSpotNum; i++)
	{
		pSpotInfo = new Section_Info;
		fread( &pSpotInfo->wSectionNum, sizeof( WORD ), 1, fp );	//StartSpot ID
		fread( &pSpotInfo->rcSection, sizeof( RECT ), 1, fp );		//StartSpot Rect

		pSpotInfo->pwTNum = new WORD[ ( abs(pSpotInfo->rcSection.bottom - pSpotInfo->rcSection.top) + 1 ) *
										 ( abs(pSpotInfo->rcSection.right - pSpotInfo->rcSection.left) + 1 ) ];

		m_pStartSplitter->pSectionList->AddTail( pSpotInfo );
	}
	//---------------------------------------------------------------------------------------------------------


lb_Exit:
	fclose( fp );

	Render();
	delete[] pwTileAttr;
	pwTileAttr = NULL;
	
	return TRUE;
}
//-----------------------------------------------------------------------------------------//
void CTool::SetBackGroundColor(BYTE byRed, BYTE byGreen, BYTE byBlue)
{
	m_dwBackColor = 0xFF000000 | (byRed << 16) | (byGreen << 8) | byBlue;
	Render();
}


void CTool::OnStartSpotSplitterDown( UINT nFlags, POINT *pt )
{
	if( !m_pHField ) return;

	HFIELD_DESC hDesc;
	m_pHField->GetHFieldDesc(&hDesc);
	
	DWORD		dwPosX,dwPosY;
	float		fDist;	
	if (m_pHField->GetTilePosWithScreenCoord(&m_v3Clicked,&fDist,&dwPosX,&dwPosY,pt,m_dwCurrentViewportIndex))
	{
		SetRect( &m_pStartSplitter->rcCurSection, 
				 dwPosX, 
				 dwPosY,
				 dwPosX,
				 dwPosY);

		m_pStartSplitter->clickPos.x = dwPosX;
		m_pStartSplitter->clickPos.y = dwPosY;
	}
}


void CTool::OnStartSpotSplitterMove( UINT nFlags, POINT *pt )
{
	if( !m_pHField ) return;

	DWORD		dwPosX,dwPosY; 
	float		fDist;

	if( nFlags & MK_LBUTTON )
	{
		if (m_pHField->GetTilePosWithScreenCoord(&m_v3Clicked,&fDist,&dwPosX,&dwPosY,pt,m_dwCurrentViewportIndex))
		{
			dwPosX = (DWORD)(m_v3Clicked.x / 125.0f );
			dwPosY = (DWORD)(m_v3Clicked.z / 125.0f );

			// 바로 전 세션의 범위를 저장한다.			
			m_pStartSplitter->rcOldSection = m_pStartSplitter->rcCurSection;

			// 현재의 세션을 저장한다.	( 수치의 역전이 일어날경우 보정하는것이다. )
			if( m_pStartSplitter->clickPos.x >= (LONG)dwPosX ) 
			{
				m_pStartSplitter->rcCurSection.right = m_pStartSplitter->clickPos.x;
				m_pStartSplitter->rcCurSection.left = dwPosX;
			}
			else 
			{
				m_pStartSplitter->rcCurSection.left = m_pStartSplitter->clickPos.x;
				m_pStartSplitter->rcCurSection.right = dwPosX;
			}
			
			if( m_pStartSplitter->clickPos.y >= (LONG)dwPosY ) 
			{
				m_pStartSplitter->rcCurSection.bottom = m_pStartSplitter->clickPos.y;
				m_pStartSplitter->rcCurSection.top = dwPosY;
			}
			else 
			{
				m_pStartSplitter->rcCurSection.top = m_pStartSplitter->clickPos.y;
				m_pStartSplitter->rcCurSection.bottom = dwPosY;
			}

			// 여기서 좌표가 변했는지 감시 체크한다.
			if( m_pStartSplitter->rcOldSection.top == m_pStartSplitter->rcCurSection.top &&
				m_pStartSplitter->rcOldSection.left == m_pStartSplitter->rcCurSection.left &&
				m_pStartSplitter->rcOldSection.right == m_pStartSplitter->rcCurSection.right &&
				m_pStartSplitter->rcOldSection.bottom == m_pStartSplitter->rcCurSection.bottom ) return;
			
			int x, z;
			int zLen = m_pStartSplitter->rcOldSection.bottom - m_pStartSplitter->rcOldSection.top + 1;
			int xLen = m_pStartSplitter->rcOldSection.right - m_pStartSplitter->rcOldSection.left + 1;
			
			HFIELD_DESC hDesc;
			m_pHField->GetHFieldDesc(&hDesc);
			
			// 이전의 속성을 저장할 메모리 공간을 잡는다.
			if(m_pStartSplitter->pwCurTNum) 
			{
				// 이때 일단 이전 속성으로 돌리기를 한다.
				for( z = 0; z <zLen; z++ )
				{
					for( x = 0; x<xLen; x++ )
					{
						//if( z == 0 || z == ( zLen - 1 ) || x == 0 || x == ( xLen - 1 ) )
						//{
							m_pHField->SetTile( 
								m_pStartSplitter->rcOldSection.left + x, m_pStartSplitter->rcOldSection.top + z, 
								m_pStartSplitter->pwCurTNum[ xLen * z + x ] );
						//}
					}
				}

				delete[] m_pStartSplitter->pwCurTNum;
				m_pStartSplitter->pwCurTNum = NULL;	
			}
			
			// 새로운 세션범위를 넣는다. 
			zLen = m_pStartSplitter->rcCurSection.bottom - m_pStartSplitter->rcCurSection.top + 1;
			xLen = m_pStartSplitter->rcCurSection.right - m_pStartSplitter->rcCurSection.left + 1;
			
			m_pStartSplitter->wCurSectionTileMany = zLen * xLen;
			m_pStartSplitter->pwCurTNum = new WORD[  m_pStartSplitter->wCurSectionTileMany  ];
			

			WORD wSpotID = m_pStartSplitter->pSectionList->GetCount();	//새로 할당할 SpotID + 1이 되겠지..

			for( z = 0; z <zLen; z++ )
			{
				for( x = 0; x<xLen; x++ )
				{
					m_pStartSplitter->pwCurTNum[ xLen * z + x ] 
						= hDesc.pwTileTable[ hDesc.dwTileNumX * (m_pStartSplitter->rcCurSection.top + z) + m_pStartSplitter->rcCurSection.left + x ];
					
					//if( z == 0 || z == ( zLen - 1 ) || x == 0 || x == ( xLen - 1 ) )
					//{
						m_pHField->SetTile( m_pStartSplitter->rcCurSection.left + x, m_pStartSplitter->rcCurSection.top + z, START_SPOT_START_TILE_NO + wSpotID );
					//}
					
				}
			}

			Render();
		}
	}
}


void CTool::OnStartSpotSplitterUp( UINT nFlags, POINT *pt )
{
	if( !m_pHField ) return;

	Section_Info*	pSec_Info ;			// Section Info 생성.
	pSec_Info	= new Section_Info;		

	pSec_Info->rcSection = m_pStartSplitter->rcCurSection;
	pSec_Info->pwTNum = new WORD[ m_pStartSplitter->wCurSectionTileMany ];
	memcpy( pSec_Info->pwTNum, m_pStartSplitter->pwCurTNum, sizeof( WORD ) * m_pStartSplitter->wCurSectionTileMany );

	//030226
	delete [] m_pStartSplitter->pwCurTNum;
	m_pStartSplitter->pwCurTNum = NULL;
	//--

	
	if( pSec_Info->rcSection.bottom - pSec_Info->rcSection.top == 0 ) 
	{
		DeleteSection( pSec_Info );
		delete pSec_Info;
		return;
	}
	if( pSec_Info->rcSection.right - pSec_Info->rcSection.left == 0 ) 
	{
		DeleteSection( pSec_Info );
		delete pSec_Info;
		return;
	}
	
	m_pStartSplitter->pSectionList->AddTail( pSec_Info );
}



void CTool::EnableStartSpotSplitter()
{
	POSITION_		pos;
	Section_Info*	pSection_Info;
	int				x, z, zLen, xLen;
	
	HFIELD_DESC hDesc;
	m_pHField->GetHFieldDesc(&hDesc);
			
	pos = m_pStartSplitter->pSectionList->GetHeadPosition();
	
	int cnt = 0;
	while( pos )
	{
		pSection_Info = (Section_Info*)m_pStartSplitter->pSectionList->GetNext( pos );

		zLen = pSection_Info->rcSection.bottom - pSection_Info->rcSection.top + 1;
		xLen = pSection_Info->rcSection.right - pSection_Info->rcSection.left + 1;

		for( z = 0; z <zLen; z++ )
		{
			for( x = 0; x<xLen; x++ )
			{
				pSection_Info->pwTNum[ xLen * z + x ] 
					= hDesc.pwTileTable[ hDesc.dwTileNumX * (pSection_Info->rcSection.top + z) + pSection_Info->rcSection.left + x ];
				
				m_pHField->SetTile( pSection_Info->rcSection.left + x, pSection_Info->rcSection.top + z, START_SPOT_START_TILE_NO + cnt );
			}
		}

		cnt++;
	}

	Render();
}



void CTool::DisableStartSpotSplitter()
{
	POSITION_		pos;
	Section_Info*	pSection_Info;
	int				x, z, zLen, xLen;

	pos = m_pStartSplitter->pSectionList->GetHeadPosition();

	while( pos )
	{
		pSection_Info = (Section_Info*)m_pStartSplitter->pSectionList->GetNext( pos );

		zLen = pSection_Info->rcSection.bottom - pSection_Info->rcSection.top + 1;
		xLen = pSection_Info->rcSection.right - pSection_Info->rcSection.left + 1;
		
		// 사용중에 여기에 들어오면 메모리를 해지하지 않아 메모리 공간이 있겠지만.
		// 로드한경우에는 메모리 공간이 없음으로 여기서 체크해서 공간을 만들어준다.
		// 로드 당시 메모리를 만들어줄수도 있지만.. 걍 이렇게 할련다..
		if( pSection_Info->pwTNum == NULL )
		{
			pSection_Info->pwTNum = new WORD[ zLen * xLen ];
		}

		for( z = 0; z <zLen; z++ )
		{
			for( x = 0; x<xLen; x++ )
			{
				m_pHField->SetTile( pSection_Info->rcSection.left + x, pSection_Info->rcSection.top + z, pSection_Info->pwTNum[ xLen * z + x ] );
			}
		}

		
	}

	Render();
}


void CTool::DeleteStartSpot( Section_Info* pSection_Info )
{
	if( pSection_Info == NULL ) return;
	
	int				x, z, zLen, xLen;
	
	zLen = pSection_Info->rcSection.bottom - pSection_Info->rcSection.top + 1;
	xLen = pSection_Info->rcSection.right - pSection_Info->rcSection.left + 1;

	for( z = 0; z <zLen; z++ )
	{
		for( x = 0; x<xLen; x++ )
		{
			//if( z == 0 || z == ( zLen - 1 ) || x == 0 || x == ( xLen - 1 ) )
			//{
				m_pHField->SetTile( pSection_Info->rcSection.left + x, pSection_Info->rcSection.top + z, pSection_Info->pwTNum[ xLen * z + x ] );
			//}
		}
	}
	

	
	POSITION_		pos;
	
	pos = m_pStartSplitter->pSectionList->Find( pSection_Info, NULL );
	if( !pos ) return;
	
	m_pStartSplitter->pSectionList->RemoveAt( pos );


	Section_Info* pSection_Info2;
	pos = m_pStartSplitter->pSectionList->GetHeadPosition();
	
	int cnt = 0;
	while( pos )
	{
		pSection_Info2 = (Section_Info*)m_pStartSplitter->pSectionList->GetNext( pos );
		

		zLen = pSection_Info2->rcSection.bottom - pSection_Info2->rcSection.top + 1;
		xLen = pSection_Info2->rcSection.right - pSection_Info2->rcSection.left + 1;

		for( z = 0; z <zLen; z++ )
		{
			for( x = 0; x<xLen; x++ )
			{
				m_pHField->SetTile( pSection_Info2->rcSection.left + x, pSection_Info2->rcSection.top + z, START_SPOT_START_TILE_NO + cnt );
			}
		}

		cnt++;
		pSection_Info2->wSectionNum	= cnt;		//새로운 ID로 갱신 
		
	}

	Render();
}



Section_Info* CTool::GetStartSpotInfo()
{
	DWORD		dwPosX,dwPosY;

	POSITION_		pos;
	Section_Info*	pSection_Info;
	POINT			ClickPos;
	float		fDist;

	if (!m_pHField->GetTilePosWithScreenCoord(&m_v3Clicked,&fDist,&dwPosX,&dwPosY,&m_pStartSplitter->mousePos,m_dwCurrentViewportIndex))
		return NULL;
	
	ClickPos.x	=	dwPosX;
	ClickPos.y	=	dwPosY;
	
	pos = m_pStartSplitter->pSectionList->GetHeadPosition();
	
	int cnt = 0;

	while( pos )
	{
		pSection_Info = (Section_Info*)m_pStartSplitter->pSectionList->GetNext( pos );
		cnt++;
		
		// PtInRect 함수가 끝선에서는 인정을 하지 않음으로 일단 노가다 코드 작성.
		//if( PtInRect( &pSection_Info->rcSection, ClickPos ) )
		if( pSection_Info->rcSection.left <= ClickPos.x && pSection_Info->rcSection.right >= ClickPos.x &&
			pSection_Info->rcSection.top <= ClickPos.y && pSection_Info->rcSection.bottom >= ClickPos.y )
		{
			pSection_Info->wSectionNum	= cnt;
			return pSection_Info;
		}
	}

	return NULL;
}



BOOL CTool::ExportTileTableForCorum(char* szFileName)
{
	if (!m_pHField)
	{
		MessageBox(NULL,"생성된 하이트필드가 없습니다.","Error",MB_OK);
		return FALSE;
	}
	FILE*	fp = fopen(szFileName,"wb");
	if (!fp)
		return FALSE;

	HFIELD_DESC	hfDesc;
	m_pHField->GetHFieldDesc(&hfDesc);
	
	TileTableDesc	ttd;
	
	ttd.dwTileManyX			=	hfDesc.dwTileNumX;								// x 개수 넣고.
	ttd.dwTileManyZ			=	hfDesc.dwTileNumZ;								// y 개수 넣고.
	ttd.dwFaceSize			=	hfDesc.fTileSize;
	ttd.dwFacesNumPerObjAxis=	hfDesc.dwFacesNumPerObjAxis;
	ttd.wSectionMany	=	m_pSectionSplitter->pSectionList->GetCount();	// 총 Section 개수 넣고.
	
	int				i, j, cnt = 0;
	POSITION_		pos;
	if( ttd.wSectionMany != 0 )
	{
		ttd.pSectionLinkInfo= new Section_Link_Info[ ttd.wSectionMany ];
		
		// Make Section info..
		Section_Info*	pSection_Info;

		pos = m_pSectionSplitter->pSectionList->GetHeadPosition();
		for( i=0; pos != NULL; i++ )
		{
			pSection_Info = (Section_Info*)m_pSectionSplitter->pSectionList->GetNext( pos );
			
			ttd.pSectionLinkInfo[i].wSectionNum = i+1;	// Section Num은 1부터시작한다.
			ttd.pSectionLinkInfo[i].x1			= pSection_Info->rcSection.left;
			ttd.pSectionLinkInfo[i].y1			= pSection_Info->rcSection.top;
			ttd.pSectionLinkInfo[i].x2			= pSection_Info->rcSection.right;
			ttd.pSectionLinkInfo[i].y2			= pSection_Info->rcSection.bottom;
		}

		// Make Section Link Info..
		// 위 루프에 합칠수도 있겠지만.. 그렇게 하면 소스보기 어려워 질꺼 같아서 일단 걍 분리했다. 뭐 툴이니깐.. ^^
		RECT src, target;

		for( i=0; i< ttd.wSectionMany; i++ )
		{
			cnt = 0;
			if( ttd.pSectionLinkInfo[i].x1 != 0 ) src.left	= ttd.pSectionLinkInfo[i].x1 - 1;
			if( ttd.pSectionLinkInfo[i].y1 != 0 ) src.top	= ttd.pSectionLinkInfo[i].y1 - 1;
			if( ttd.pSectionLinkInfo[i].x2 != ttd.dwTileManyX ) src.right	= ttd.pSectionLinkInfo[i].x2 + 1;
			if( ttd.pSectionLinkInfo[i].y2 != ttd.dwTileManyZ ) src.bottom	= ttd.pSectionLinkInfo[i].y2 + 1;

			for( j=0; j< ttd.wSectionMany; j++ )
			{
				if( i != j )	// 나 자신과는 비교하지 않기 위해서..
				{
					target.left		= ttd.pSectionLinkInfo[j].x1;
					target.top		= ttd.pSectionLinkInfo[j].y1;
					target.right	= ttd.pSectionLinkInfo[j].x2;
					target.bottom	= ttd.pSectionLinkInfo[j].y2;

					if( RectInRect( &src, &target ) && ( cnt < MAX_SECTION_LINK_MANY ) )
					{
						ttd.pSectionLinkInfo[i].pwSectionLinkNum[cnt] = ttd.pSectionLinkInfo[j].wSectionNum;
						cnt++;
					}
				}
			}
		}
	}
	
	// 저장 시작...
	fwrite(&ttd.dwTileManyX ,sizeof(DWORD),1,fp);	// x길이 저장.
	fwrite(&ttd.dwTileManyZ ,sizeof(DWORD),1,fp);	// z길이 저장.
	fwrite(&ttd.dwFaceSize ,sizeof(DWORD),1,fp);	
	fwrite(&ttd.dwFacesNumPerObjAxis ,sizeof(DWORD),1,fp);	
	fwrite(hfDesc.pwTileTable,sizeof(WORD),hfDesc.dwTileNumX*hfDesc.dwTileNumZ,fp);	// 타일 정보 저장.
	
	fwrite(&ttd.wSectionMany, sizeof(WORD),1,fp);	// 세션수 저장.

	for( i=0; i<ttd.wSectionMany; i++ )				// 세션의 정보저장.
	{
		fwrite(&ttd.pSectionLinkInfo[i], sizeof(Section_Link_Info), 1, fp);	
	}
	
	//NPC정보 입력.
	WORD wNpcMany = m_pNpcInfoList->GetCount();
	fwrite( &wNpcMany, sizeof(WORD), 1, fp );

	Npc_Info*	pNpcInfo;
	pos = m_pNpcInfoList->GetHeadPosition();
	while( pos )
	{
		pNpcInfo = (Npc_Info*)m_pNpcInfoList->GetNext( pos );
		fwrite( pNpcInfo, sizeof( Npc_Info ), 1, fp );
	}

	

	//--------------------------------------------------------------------------------------------------------
	//--								여기서부터 MoveSpot 저장하기 	(Added by KBS)						--
	//--------------------------------------------------------------------------------------------------------
	
	WORD	wSpotNum = m_pSpotSplitter->pSectionList->GetCount();
	fwrite( &wSpotNum, sizeof(WORD), 1, fp );	//MoveSpot 갯수 

	Section_Info* pSection_Info2;
	pos = m_pSpotSplitter->pSectionList->GetHeadPosition();
	
	while( pos )
	{
		pSection_Info2 = (Section_Info*)m_pSpotSplitter->pSectionList->GetNext( pos );
		
		fwrite( &pSection_Info2->wSectionNum, sizeof(WORD), 1, fp );	//Spot ID
		fwrite( &pSection_Info2->rcSection, sizeof(RECT), 1, fp );		//Spot Rect 범위 
	}
	//------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------
	//--								여기서부터 StartSpot 저장하기 	(Added by KBS)						--
	//--------------------------------------------------------------------------------------------------------
	WORD	wStartNum = m_pStartSplitter->pSectionList->GetCount();
	fwrite( &wStartNum, sizeof(WORD), 1, fp );	//MoveSpot 갯수 

	pos = m_pStartSplitter->pSectionList->GetHeadPosition();
	
	while( pos )
	{
		pSection_Info2 = (Section_Info*)m_pStartSplitter->pSectionList->GetNext( pos );
		
		fwrite( &pSection_Info2->wSectionNum, sizeof(WORD), 1, fp );	//Spot ID
		fwrite( &pSection_Info2->rcSection, sizeof(RECT), 1, fp );		//Spot Rect 범위 
	}
	//------------------------------------------------------------------------------------------------------------

	fclose(fp);

	return TRUE;
}



//Added by KBS 030224
void CTool::RenderTileText()
{
	if(m_pHField)
	{
		DWORD dwPosX, dwPosY;
		VECTOR3 vPos;
		float	fDist;
		if (m_pHField->GetTilePosWithScreenCoord(&vPos,&fDist,&dwPosX,&dwPosY,&m_ptMousePos, m_dwCurrentViewportIndex))
		{
			dwPosX = (DWORD)(vPos.x / 125.0f );
			dwPosY = (DWORD)(vPos.z / 125.0f );
			
			char szTileText[ 0xff ];
			sprintf(szTileText, "Tile_X:%d, Tile_Z:%d; x:%8.2f, y:%8.2f, z:%8.2f; Mouse_X:%d, Mouse_Y:%d, ", dwPosX, dwPosY, vPos.x, vPos.y, vPos.z, m_ptMousePos.x, m_ptMousePos.y);
			DWORD	len = lstrlen(szTileText);
			long width = len*40;

			RECT		rect, rect2;
			rect.left = 10;
			rect.top = 50;
			rect.right = rect.left + width;
			rect.bottom = rect.top + 40;

			rect2 = rect;

			rect.left -= 1;
			m_pFont->DrawText(szTileText,len,&rect,0xff000000,CHAR_CODE_TYPE_ASCII, 0);

			rect.left += 2;	
			m_pFont->DrawText(szTileText,len,&rect,0xff000000,CHAR_CODE_TYPE_ASCII, 0);

			rect.top -= 1;
			m_pFont->DrawText(szTileText,len,&rect,0xff000000,CHAR_CODE_TYPE_ASCII, 0);

			rect.top += 2;
			m_pFont->DrawText(szTileText,len,&rect,0xff000000,CHAR_CODE_TYPE_ASCII, 0);

			m_pFont->DrawText(szTileText,len,&rect2,0xffffff00,CHAR_CODE_TYPE_ASCII, 0);
		}
	}
}
//--


//trustpak
void CTool::SetCurrentMapFileName(char* pszMapFileName /* in */)
{
	if (NULL == pszMapFileName)
	{
		return;
	}

	strncpy(m_szCurrentMapFileName, pszMapFileName, sizeof(m_szCurrentMapFileName));
}


void CTool::GetCurrentMapFileName(char* pszMapFileName /* out */, int iBufSize)
{
	if (NULL == pszMapFileName)
	{
		return;
	}

	strncpy(pszMapFileName, m_szCurrentMapFileName, iBufSize);
}

BOOL CTool::ToggleDisplayCameraInfo(void)
{
	m_bDisplayCameraInfo = !m_bDisplayCameraInfo;

	return m_bDisplayCameraInfo;
}

void CTool::SetCurrentTilePalleteFileName(char* pszTilePalletFileName /* in */)
{
	strncpy(m_szCurrentTilePaletteFileName, pszTilePalletFileName, sizeof(m_szCurrentTilePaletteFileName));
}

void CTool::GetCurrentTilePalleteFileName(char* pszTilePalletFileName /* out */, int iBufSize)
{
	strncpy(pszTilePalletFileName, m_szCurrentTilePaletteFileName, iBufSize);
}

void CTool::SetCurrentTileSetFileName(char* pszTileSetFileName /* in */)
{
	strncpy(m_szCurrentTileSetFileName, pszTileSetFileName, sizeof(m_szCurrentTileSetFileName));
}

void CTool::GetCurrentTileSetFileName(char* pszTileSetFileName /* out */, int iBufSize)
{
	strncpy(pszTileSetFileName, m_szCurrentTileSetFileName, iBufSize);
}

void CTool::UpdateLoadFailedTextureDialog(void)
{
	TEXTURE_TABLE*	pTextureTable = NULL;
	DWORD			dwTableSize = 0;
	DWORD			dwCount = 0;

	m_pExecutive->GetRenderer()->GetLoadFailedTextureTable(&pTextureTable, &dwTableSize, &dwCount);
	m_pLoadFailedTextureDialog->UpdateListBoxData(pTextureTable, dwCount);
}

void CTool::ShowFailedTextureDialog(void)
{
	m_pLoadFailedTextureDialog->DoModal();
}


VOID CTool::SetDlgBarTool(CDlgBarTool* pDlgBarTool)
{
	g_pDlgBarTool = pDlgBarTool;
}


BOOL CTool::ToggleTileEditMode(VOID)
{
	if (NULL == g_pDlgBarTool)
	{
		return FALSE;
	}

	g_pDlgBarTool->ToggleTileEditButton();
	return TRUE;
}

BOOL CTool::ToggleTileSetEditMode(VOID)
{
	if (NULL == g_pDlgBarTool)
	{
		return FALSE;
	}

	g_pDlgBarTool->ToggleTileSetEditButton();

	return TRUE;
}


VOID CTool::SetRenderToolMode(DWORD dwRenderToolMode)
{
	m_dwRenderToolMode = dwRenderToolMode;

	if (NULL != m_pExecutive)
	{
		m_pExecutive->SetRenderMode(m_dwRenderToolMode);
	}
}

VOID CTool::DisableRenderStaticModel(VOID)
{
	if (NULL != m_pExecutive)
	{
		m_pExecutive->DisableRenderAllStaticModels();
	}
}

VOID CTool::EnableRenderStaticMode(VOID)
{
	if (NULL != m_pExecutive)
	{
		m_pExecutive->EnableRenderAllStaticModels();
	}
}

BOOL CTool::ToggleRenderStaticModel(VOID)
{
	m_bRenderStaticModel = !m_bRenderStaticModel;

	if (TRUE == m_bRenderStaticModel)
	{
		EnableRenderStaticMode();
	}
	else
	{
		DisableRenderStaticModel();
	}

	return m_bRenderStaticModel;
}

//