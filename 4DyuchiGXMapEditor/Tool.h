// Tool.h: interface for the CTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOL_H__08FA5210_1863_4F15_BDF2_EF1E9A5DBDA0__INCLUDED_)
#define AFX_TOOL_H__08FA5210_1863_4F15_BDF2_EF1E9A5DBDA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _MFC


#include "../4DyuchiGRX_common/stdafx.h"
#include "../4DyuchiGRX_common/IGeometry.h"
#include "../4DyuchiGRX_common/IRenderer.h"
#include "../4DyuchiGRX_common/IExecutive.h"
#include "../4DyuchiGX_UTIL/MToolMouseStatus.h"

#include "ColorPalette.h"
#include "DialogTilePalette.h"
#include "DialogConsole.h"
#include "DialogHFieldProperty.h"
#include "RightView.h"


//yh
#include "TileSetView.h"
#include "DialogTileSet.h"
#include "DialogEditTileSet.h"
#include "DialogEditEntry.h"
//

//trustpak
#include "LoadFailedTextureDialog.h"
//

//In
#include "VoidList.h"
//

#define MAX_LIGHT_NUM		64
#define MAX_CAMERA_NUM		16
#define MAX_OBJECTS_NUM		4096


//Added by KBS
#define MOVE_SPOT_START_TILE_NO		9	
#define MAX_MOVE_SPOT_NUM			10
#define START_SPOT_START_TILE_NO	19	
#define MAX_START_SPOT_NUM			5	
//--


enum VIEW_TYPE
{
	VIEW_TYPE_TOP		=	0x00000000,
	VIEW_TYPE_LEFT		=	0x00000001,
	VIEW_TYPE_RIGHT		=	0x00000002,
	VIEW_TYPE_BOTTOM	=	0x00000003,
	VIEW_TYPE_FRONT		=	0x00000004,
	VIEW_TYPE_BACK		=	0x00000005,
	VIEW_TYPE_FREE		=	0x00000006
};

enum EDIT_TYPE
{
	EDIT_TYPE_MOVE		= 0x00000001,
	EDIT_TYPE_ROT		= 0x00000002,
	EDIT_TYPE_SCALE		= 0x00000004,
	EDIT_TYPE_CREATE	= 0x00000008
};

enum EDIT_MODE
{
	EDIT_MODE_TILE				= 0x10000000,
	EDIT_MODE_VERTEX			= 0x20000000,
	EDIT_MODE_GXOBJECT			= 0x30000000,
	EDIT_MODE_BRUSH				= 0x40000000,
	EDIT_MODE_GET_TILE_INFO		= 0x50000000,
	EDIT_MODE_GET_HFIELDOBJECT	= 0x60000000,
	EDIT_MODE_LIGHT				= 0x70000000,
	EDIT_MODE_TRIGGER			= 0x80000000,
	EDIT_MODE_DRAW_LIGHTMAP		= 0x90000000,
	EDIT_MODE_DRAW_VERTEXCOLOR	= 0xA0000000,
	EDIT_MODE_DRAW_HFIELD_ALPHAMAP = 0xB0000000,
	//yh
	EDIT_MODE_TILESET			= 0xC0000000
	//
	
};
#define EDIT_MODE_MASK			0xf0000000
#define EDIT_MODE_MASK_INVERSE	0x0fffffff
enum MOUSE_STATUS
{
	LBUTTON_DOWN		=		0x00000001,
	LBUTTON_UP			=		0x00000002,
	MOVE				=		0x00000004

};
#define DEFAULT_PICK_DISTANCE 1000.0f

enum AXIS_TYPE
{
	NOT_SET			= 0x00000000,
	X_AXIS_LOCK		= 0x00000001,
	Y_AXIS_LOCK		= 0x00000002,
	Z_AXIS_LOCK		= 0x00000004
};
struct TILE_INFO
{
	DWORD		dwPosX;
	DWORD		dwPosY;

};



//trustpak

#define DEFAULT_SS3D_MAP_FILE_NAME		"제목없음"
#define LOAD_FAILED_TEXTURE_TABLE_SIZE	512

class CDlgBarTool;
///

//------------------------------------------------------------------------------//
// CorumZero MapTool 추가작업.
#pragma pack( push, 1 )

struct SectionSplitter
{
	bool		bSectionInputMode;
	
	WORD		wSectionMany;	
	
	POINT		mousePos;
	POINT		clickPos;
	RECT		rcCurSection;			// 현재 설정할려고 하는 세션의 범위.
	RECT		rcOldSection;
	WORD		wCurSectionTileMany;	// 현재 설정한 세션의 타일 개수.
	LPWORD		pwCurTNum;
	
	CSectionList*	pSectionList;

	SectionSplitter()
	{
		bSectionInputMode	= false;

		wSectionMany		= 0;
		rcCurSection.top	= 0;
		rcCurSection.bottom	= 0;
		rcCurSection.left	= 0;
		rcCurSection.bottom	= 0;

		rcOldSection.top	= 0;
		rcOldSection.bottom	= 0;
		rcOldSection.left	= 0;
		rcOldSection.bottom	= 0;
		
		wCurSectionTileMany	= 0;
		pwCurTNum			= NULL;

		pSectionList		= new CSectionList;		// 걍 생성자에서 생성.
	}

	~SectionSplitter()
	{
		if( pwCurTNum ) delete pwCurTNum;

		if( pSectionList )
		{
			pSectionList->RemoveAll();
			delete pSectionList;
			pSectionList = NULL;
		}
	}
};
#pragma pack( pop )

//------------------------------------------------------------------------------//

class CTool  
{
	float						m_fHFieldVertexColorBrushSize;
	POINT						m_ptMousePos;
	DWORD						m_dwMouseStatus;
	HMODULE						m_hModGFunc;

	BOOL						m_bRenderPerMouseMove;
	BOOL						m_TileBrushMode;
	BOOL						m_bEnableViewColSTM;

	BOOL						m_bSelectMode;
	BOOL						m_bPickColorMode;
	VECTOR2						m_v2PointSelectRect[4];
	BOOL						m_bDrawSelectedRect;
	DWORD						m_dwSelectedVertexNum;
	HFIELD_POS*					m_pSelectedVertexIndex;
	DWORD						m_dwMaxSelectedVertexNum;

	DWORD						m_dwSelectedTileNum;
	HFIELD_POS*					m_pSelectedTileIndex;
	DWORD						m_dwMaxSelectedTileNum;

	DWORD						m_dwCurrentViewportIndex;
	CPoint						m_ptMousePrvDown;
	CPoint						m_ptMousePrvDrg;
	I3DModel*					m_pArrow;
	DWORD						m_dwArrowRefIndex;
	VECTOR3						m_v3ArrowPos;
	DWORD						m_dwMoveAxis;

	C3DBrush*					m_pBrush;
	VBHASH_HANDLE				m_pHash;
	

	GXOBJECT_HANDLE				m_gxoSelected;
	GXLIGHT_HANDLE				m_gxlSelected;
	VECTOR3						m_v3PickedPos;
	PLANE						m_planePicked;
	GXTRIGGER_HANDLE			m_gxtSelected;
//	I3DModel*					m_pSelectedModel;

	MToolMouseStatus			m_Mouse;
	CWnd*						m_pEditWindow;
	I4DyuchiGXExecutive*		m_pExecutive;
	I4DyuchiGXRenderer*			m_pRendererForPalette;
	I4DyuchiFileStorage*		m_pFileStorage;
	IDIFontObject*				m_pFont;
	HFIELD_DESC					m_hfDesc;
	HFIELD_CREATE_DESC			m_hfcreate;
	DWORD						m_dwHFieldDetail;
	

	TILE_INFO					m_tileInfo;
	DWORD						m_dwEditMode;
	DWORD						m_dwRenderMode;
	IHeightField*				m_pHFieldViewOnly;
	IHeightField*				m_pHField;
	DWORD						m_dwSelectedHFieldObjectIndex;
	I3DStaticModel*				m_pStaticModel;
	GXMAP_HANDLE				m_ghMap;
	CDialogTilePalette*			m_pPalette;

	
	//yh
	CDialogTileSet*				m_pTileSetDialog;
	CDialogEditTileSet*			m_pEditTileSetDialog;
	CDialogEditEntry*			m_pEditEntryDialog;
	//

	//trustpak

	CLoadFailedTextureDialog*	m_pLoadFailedTextureDialog;	

	char						m_szCurrentMapFileName[_MAX_PATH];
	char						m_szCurrentTilePaletteFileName[_MAX_PATH];
	char						m_szCurrentTileSetFileName[_MAX_PATH];

	BOOL						m_bDisplayCameraInfo;	
	TEXTURE_TABLE				m_aLoadFailedTextureTable[LOAD_FAILED_TEXTURE_TABLE_SIZE];	

	DWORD						m_dwRenderToolMode;

	BOOL						m_bRenderStaticModel;


	///
	
	MAABB						m_WorldBox;
		
	DWORD						m_dwStaticModelAlpha;
	DWORD						m_dwHFieldAlpha;
	DWORD						m_dwViewHFieldAlpha;

//	CDialogConsole*				m_pConsole;

	CPoint						m_pointMousePos;
	BOOL						m_bIsHFieldCreateMode;
	BOOL						m_bEnableLightMap;
	BOOL						m_bEnableViewOnlyHField;
	BOOL						m_bEnableBoundingBox;
	BOOL						m_bEnableViewOnlyLightTexture;
	BOOL						m_bViewTilePalette;

	
	//yh
	BOOL						m_bViewTileSet;
	//

	VECTOR3						m_v3Clicked;


	BOOL						m_bMapBuild;
	LIGHT_DESC					m_lightDesc[MAX_LIGHT_NUM];
	DWORD						m_dwLightNum;
	CAMERA_DESC					m_cameraDesc[MAX_CAMERA_NUM];
	DWORD						m_dwCameraNum;
	GXOBJECT_HANDLE				m_gxhList[MAX_OBJECTS_NUM];
	DWORD						m_dwGXObjectNum;
	GXLIGHT_HANDLE				m_gxhLightPrj;
	float						m_fLightPrjHeight;


	DWORD						m_dwBackColor;

	HWND						m_hWndStatus;
	//BOOL						m_bCopy;
	BOOL						m_bLightCopy;

	//trustpak 2005/06/09
	BOOL						m_bObjectCopy;

	///
	char						m_szConsoleText[512];

	VIEW_TYPE					m_CurrentViewMode;		// 2002/03/13 추가.
	char						m_szDefaultPath[_MAX_PATH];
	char						m_szHFieldName[_MAX_PATH];
	
		
	void						RenderConsoleText();
	BOOL						AttachDevice();
	BOOL						CreateHeightField(HFIELD_DESC* pDesc);
	void						OnClickLight(POINT* ptCursor,UINT nFlags);
	void						OnClickTile(POINT* pt,UINT nFlags);
	
	//yh
	void						OnClickTileSet(POINT* pt,UINT nFlags);
	//

	void						OnClickVertex(POINT* pt,UINT nFlags);
	void						OnClickGXObject(POINT* pt,UINT nFlags);
	void						OnClickHFieldObject(POINT* ptCursor,UINT nFlags);
	void						OnClickBrush(POINT *ptCursor,UINT nFlags);
	void						OnClickTrigger(POINT* ptCursor,UINT nFlags);
	void						OnClickLightMap(POINT* pt,UINT nFlags);
	BOOL						OnClickHFieldVertexColor(POINT* pt,VECTOR3* pv3IntersectPoint,UINT nFlags);
	BOOL						OnClickHFieldAlphaTexel(POINT* pt,VECTOR3* pv3IntersectPoint,UINT nFlags);




	void						OnRClickLight(POINT* ptCursor,UINT nFlags);
	void						OnRClickGXObject(POINT* ptCursor,UINT nFlags);
	void						OnRClickGXTrigger(POINT* ptCursor,UINT nFlags);
	void						OnRClickLightMap(POINT* pt,UINT nFlags);
	BOOL						OnRClickHFieldVertexColor(POINT* pt,VECTOR3* pv3IntersectPoint,UINT nFlags);

	void						DrawBrushArea(VECTOR3* pv3IntersectPoint);
	BOOL						GetPosIntersect(VECTOR3* pv3Pos,POINT* pPoint);

	
	BOOL						GetMovePos(VECTOR3* pv3Pos,VECTOR3* pv3BasePoint,POINT* pPoint);
	BOOL						GetRot(float* pfRad,VECTOR3* pv3Rot,POINT* pPoint);
	BOOL						GetScale(VECTOR3* pv3Scale,VECTOR3* pv3BasePoint,POINT* pPoint);
	
	void						GetTileInfo(TILE_INFO* pTileInfo,POINT* pt);
	void						UpdateStatus();
	void						ClearSelectedItem();
	BOOL						PutBrush();
	void						CreateBrushArea();

	
	
public:
	void						OnClearAlphaMapWithTileIndex(POINT* ptCursor,UINT nFlags);
	void						OnClearAlphaMapWithObjectIndex(POINT* ptCursor,UINT nFlags);
	void						OnClearAlphaMapAll(POINT* ptCursor,UINT nFlags);

	void						EnableHFieldBlend(BOOL bEnable);
	void						SetHFieldVertexColorBrushSize(float fRadius);
	void						ReplaceTile(char* szFileName,DWORD dwTileIndex);
	void						GetDirectionalLightDesc(DIRECTIONAL_LIGHT_DESC* pLightDesc);
	void						EnableDirectionalLight(DIRECTIONAL_LIGHT_DESC* pLightDesc);
	void						DisableDirectionalLight();
	void						ChangeHFieldIndexBufferNum(DWORD dwNum);
	void						UpdateTilePalette();
	void						SetHFieldDetail(DWORD dwDetail) {m_dwHFieldDetail = dwDetail;}
	DWORD						GetSelectedTileList(HFIELD_POS** ppTileList);
	void						SetSelectMode(BOOL bSwitch) {m_bSelectMode = bSwitch;}
	void						SetPickColorMode(BOOL bSwitch) {m_bPickColorMode = bSwitch;}
	
	
	void						SetAlphaStaticModel(DWORD dwAlpha);
	void						SetAlphaHField(DWORD dwAlpha);
	void						SetAlphaViewOnlyHField(DWORD dwAlpha);

	DWORD						GetAlphaStaticModel() {return m_dwStaticModelAlpha;}
	DWORD						GetAlphaHField() {return m_dwHFieldAlpha;}
	DWORD						GetAlphaViewOnlyHField() {return m_dwViewHFieldAlpha;}

	void						OnDeleteAllGXLights();
	void						OnDeleteAllGXObjects();
	void						OnNewMap();
	void						ResetHeight(float h);
	
	void						SetHFieldCreateDesc(HFIELD_CREATE_DESC*	phfCreate);
	void						BeginCreateStaticModel();
	void						EndCreateStaticModel();

	void						ResetDefaultCamera(DWORD dwViewportIndex);
	void						ResetCamera(DWORD dwViewportIndex);
	void						CleanHeightField();
	I4DyuchiGXExecutive*		GetExecutive() {return m_pExecutive;}
	I4DyuchiGXRenderer*			GetRenderer() {return m_pExecutive->GetRenderer();}
	I4DyuchiGXGeometry*			GetGeometry() {return m_pExecutive->GetGeometry();}
	I4DyuchiFileStorage*		GetFileStorage() {return m_pFileStorage;}

	void						SetMousePos(CPoint* pPoint) {m_pointMousePos = *pPoint;}
	CPoint*						GetMousePos() {return &m_pointMousePos;}
	BOOL						Initialize(CWnd* pWnd);
	BOOL						CreateNewHeightField();
	BOOL						CreateNewStaticModel();
	
	BOOL						ExportTileTable(char* szFileName);
	BOOL						ExportTileTableForCorum(char* szFileName);
	BOOL						ExportHeightField(char* szFileName);
	BOOL						ImportHeightField(char* szFileName,DWORD dwIndexBufferNum);
	BOOL						ImportViewOnlyHeightField(char* szFileName);
	BOOL						ExportStaticModel(char* szFileName);
	BOOL						ImportStaticModel(char* szFileName);
	
	BOOL						CreateStaticModelFromMod(char* szFileName);
	BOOL						CreateLightFromMod(char* szFileName);
	BOOL						SampleFromMod(char* szFileName);


	void						OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void						SetConsoleText(char* szStr,DWORD len);
		
	void						Render();	

	BOOL						BuildMap( MAABB* pWorldBox);
	BOOL						ShadeLightMap(DWORD dwAmbientColor,DWORD dwFlag);
	BOOL						ToggleLightmap();
	BOOL						ToggleViewOnlyLightTexture();
	BOOL						ToggleViewOnlyHField();
	BOOL						ToggleViewColSTM();
	BOOL						ToggleBoundingBox();

	BOOL						ViewTilePalette();
	
	//yh
	BOOL						ViewTileSet();
	void						SetPickTileSetMode();
	//
		
	void						SetLightMode();
	void						SetPickTileMode();
	void						SetPickVertexMode();
	void						SetBrushMode();
	void						SetGXObjectMode();
	void						SetTileInfoMode();
	void						SetHFieldObjectMode();
	void						SetTriggerMode();
	void						SetEditType(DWORD dwFlag);
	void						SetTileBrushMode(BOOL bSwitch);
	void						SetDrawLightMapMode();
	void						SetDrawHFieldVertexColor();
	void						SetDrawHFieldaAlphaMap();

	void						OnLButtonDown(UINT nFlags, CPoint* point);
	void						OnLButtonUp(UINT nFlags, CPoint* point);
	void						OnRButtonDown(UINT nFlags, CPoint* point);
	void						OnMButtonDown(UINT nFlags, CPoint* point);


	void						SetRenderMode(DWORD dwFlag);

	//trustpak

	void						EnableRenderBothMode(void);
	void						DisableRenderBothMode(void);

	///
	
	void						LockXAxis();
	void						LockYAxis();
	void						LockZAxis();
	void						SetFreeAxis() {m_dwMoveAxis = 0;}
	void						CleanStaticModel();
	
	void						SetCamera(DWORD dwFlag,DWORD dwViewportIndex);
	void						OnMouseMove(UINT nFlags, CPoint point);

	void						ResetTransformBrush();
	void						OnSize(UINT nType, int cx, int cy);
	void						SaveMap(char* szFileName);
	void						LoadMap(char* szFileName);
	
	void						LoadGXObjects(char* szFileName);
	void						LoadGXObjects(FILE* fp);

	void						LoadLights(char* szFileName);
	void						LoadLights(FILE* fp);



	void						SaveGXObjects(char* szFileName);
	void						SaveGXObjects(FILE* fp);
	void						SaveLights(char* szFileName);
	void						SaveLights(FILE* fp);
	void						DisableRenderHFObj();
	void						EnableRenderHFObj();
	void						FreezeHFObjDetail(DWORD dwDetailLevel);
	void						UnfreezeHFObjDetail();

	void						RenderTileText();			//Added by KBS 030224
	
	void						SetLightPositionAll( VECTOR3	v3LightAddPos );	// 모든 라이트의 포지션에서 인자만큼 더해준다.
	void						FillSelecteTile();
	void						RotateSelecteTile();
	CTool();
	~CTool();
	
	// CorumZero MapTool 추가작업.
	SectionSplitter*			m_pSectionSplitter;
	CNpcInfoList*				m_pNpcInfoList;
	SectionSplitter*			m_pSpotSplitter;		//Added by KBS
	SectionSplitter*			m_pStartSplitter;		//0219
	
	IHeightField*				GetHField() { return m_pHField; }
	CNpcInfoList*				GetNpcInfoList() { return m_pNpcInfoList; }

	void						OnSectionSplitterDown(UINT nFlags, POINT *pt);
	void						OnSectionSplitterMove(UINT nFlags, POINT *pt);
	void						OnSectionSplitterUp(UINT nFlags, POINT *pt);

	//Added by KBS
	void						OnSpotSplitterDown( UINT nFlags, POINT *pt );
	void						OnSpotSplitterMove(UINT nFlags, POINT *pt);
	void						OnSpotSplitterUp(UINT nFlags, POINT *pt);
	void						OnStartSpotSplitterDown( UINT nFlags, POINT *pt );	//0219
	void						OnStartSpotSplitterMove(UINT nFlags, POINT *pt);	//0219	
	void						OnStartSpotSplitterUp(UINT nFlags, POINT *pt);		//0219
	//--


	void						EnableSectionSplitter();
	void						DisableSectionSplitter();

	//Added by KBS
	void						EnableSpotSplitter();
	void						DisableSpotSplitter();
	void						EnableStartSpotSplitter();	//0219
	void						DisableStartSpotSplitter();	//0219
	//--

	Section_Info*				GetSectionInfo();
	void						DeleteSection( Section_Info* pSection_Info );
	
	//Added by KBS
	Section_Info*				GetSpotInfo();			
	void						DeleteSpot( Section_Info* pSection_Info );
	Section_Info*				GetStartSpotInfo();									//0219
	void						DeleteStartSpot( Section_Info* pSection_Info );		//0219
	//--

	BOOL						CreateNPC( Npc_Info* pNpcInfo );
	BOOL						Loadttb( const char* szFileName );
	//
	
	// 2002/11/04
	void						GetWorldBox( MAABB* pWorldBox){ *pWorldBox = m_WorldBox;}
	void						SetWorldBox( MAABB* pWorldBox){ m_WorldBox = *pWorldBox;}



	// Back Ground color modify.

	void						SetBackGroundColor(BYTE byRed, BYTE byGreen, BYTE byBlue);


	//trustpak
	void						SetCurrentMapFileName(char* pszMapFileName /* in */);
	void						GetCurrentMapFileName(char* pszMapFileName /* out */, int iBufSize);

	void						SetCurrentTilePalleteFileName(char* pszTilePalletFileName /* in */);
	void						GetCurrentTilePalleteFileName(char* pszTilePalletFileName /* out */, int iBufSize);
	
	void						SetCurrentTileSetFileName(char* pszTileSetFileName /* in */);
	void						GetCurrentTileSetFileName(char* pszTileSetFileName /* out */, int iBufSize);

	void						UpdateLoadFailedTextureDialog(void);
	void						ShowFailedTextureDialog(void);

	BOOL						ToggleDisplayCameraInfo(void);	

		
	VOID						SetDlgBarTool(CDlgBarTool* pDlgBarTool);
	BOOL						ToggleTileEditMode(VOID);
	BOOL						ToggleTileSetEditMode(VOID);

	VOID						SetRenderToolMode(DWORD dwRenderToolMode);
	DWORD						GetRenderToolMode(VOID)	{ return m_dwRenderToolMode; }


	BOOL						ToggleRenderStaticModel(VOID);

	VOID						DisableRenderStaticModel(VOID);
	VOID						EnableRenderStaticMode(VOID);

	//



private:
//	void MakeHeightFieldHigh( DWORD dwX, DWORD dwZ, float fHeight, float fRadius);
};

extern CTool* g_pTool;
extern CDlgBarTool* g_pDlgBarTool;

DWORD AddIndex(HFIELD_POS* pPosListDest,DWORD dwPosSrcNum,HFIELD_POS* pPosListSrc,DWORD dwPosDestNum);

#endif // !defined(AFX_TOOL_H__08FA5210_1863_4F15_BDF2_EF1E9A5DBDA0__INCLUDED_)
