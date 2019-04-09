#ifndef _HEIGHT_FILED_H
#define _HEIGHT_FILED_H


#include "../4DyuchiGRX_common/IGeometry.h"
#include "../4DyuchiGRX_common/IRenderer.h"
#include "../4DyuchiGXGFunc/global.h"

class CHFieldObject;
class CoGeometry;
class CHFieldTools;
class CHFieldEditHelper;

#define BUILD_BOUNDING_MESH 0x00000001

// 현재 버젼 정보
#define	HEIGHT_FIELD_VERSION		 0x00000001

struct TILE_AREA
{
//	HFPOS		hfPosStart;			// 좌측 상단 꼭지점 포인트
//	HFPOS		hfSize;				// 좌측 상단 기준으로 크기
};

class CoHeightField : public IHeightField
{
	DWORD					m_dwRefCount;
	I4DyuchiGXRenderer*		m_pRenderer;
	CoGeometry*				m_pGeometry;
	DWORD					m_dwIndexInGeometry;
	IDIHeightField*			m_pIDIHeightField;

	HFIELD_DESC				m_hfDesc;
	
	DWORD					m_dwObjNum;
	DWORD					m_dwYFNum;

	BOOL					m_bEnableDrawAlphaMap;
	DWORD					m_dwTotalAlphaMapNum;
	DWORD					m_dwAlphaMapWidthHeight;
	
	float					m_fHFieldDetailRSPerObj;
	

		
	COLLISION_MESH_OBJECT_DESC*		m_pColMeshObjDescList;
	COLLISION_MESH_OBJECT_DESC		m_colWolrdBoundingMesh;
	CHFieldObject*			m_pHFieldObjectList;

	// for tool...
	BOOL					m_bEnableAlphaMap;
	CHFieldTools*			m_pHFieldTools;

	float					m_fDetailDistance[MAX_HFIELD_DETAIL_NUM];
	float					m_fDetailRS[MAX_HFIELD_DETAIL_NUM];
	char					m_szTempVCLFileName[MAX_NAME_LEN];
	char					m_szFileName[MAX_NAME_LEN];
	

	void					CreateYFList();
	void					CreateHFObjectList();
	
	DWORD					ReadHFieldDesc(void* pFP);
	
	BOOL					BuildHField(LOAD_CALLBACK_DESC* pCallbackDesc,DWORD dwFlag);
	BOOL					CreateIndexBuffer(DWORD dwFacesNumPerObj,DWORD dwDetailLevelNum,DWORD dwSkipStep);
	
	void					BuildIndexBuffer(WORD* pIndex,DWORD dwFacesNumPerObj,DWORD dwPitch,DWORD dwStep,DWORD dwIndicesNum);
	
	void					SetBoundaryRight(WORD* pIndex,DWORD dwFacesNumPerObj,DWORD dwPitch,DWORD dwStep);
	void					SetBoundaryLeft(WORD* pIndex,DWORD dwFacesNumPerObj,DWORD dwPitch,DWORD dwStep);
	void					SetBoundaryUp(WORD* pIndex,DWORD dwFacesNumPerObj,DWORD dwPitch,DWORD dwStep);
	void					SetBoundaryDown(WORD* pIndex,DWORD dwFacesNumPerObj,DWORD dwPitch,DWORD dwStep);
	void					DivIndexBuffer(WORD* pIndex,DWORD dwFacesNumPerObj,DWORD dwSkipFacesNum,DWORD dwStep,DWORD dwIndicesNum);
	DWORD					WriteTextureTable(FILE* fp);
	DWORD					WriteTileTable(FILE* fp);
	DWORD					WriteAlphaMapHeader(FILE* fp);
	DWORD					WriteAlphaMapBody(char* szFileName);

	DWORD					ReadTileTable(void* pFP);
	DWORD					ReadTextureTable(void* pFP);
	DWORD					ReadAlphaMapHeader(void* pFP);
	DWORD					ReadAlphaMapBody(char* szFileName);
	

	void					GetVertexPosRect(DWORD_RECT* pRect,DWORD dwCenterVertexX,DWORD dwCenterVertexZ,float fRadius);
	BOOL					IsIntersectPoint(VECTOR3* pv3IntersectPoint,float* pfDist,VECTOR3* pv3Orig,VECTOR3* pv3Dir,DWORD dwPosX,DWORD dwPosZ);

	DWORD					GetIncludeVertexObjectList(CHFieldObject** ppHFieldObj,DWORD dwMaxObjNum,HFIELD_POS* pVertexList,DWORD dwVertexNum);
	DWORD					GetIncludeVertexTileList(DWORD* pdwTileList,DWORD dwMaxTile,HFIELD_POS* pVertexList,DWORD dwVertexNum);

	DWORD					GetVertexListWithScreenCoord(HFIELD_POS* pVertexList,DWORD dwMaxVertexNum,RECT* pRect,DWORD dwVertexOffset,DWORD dwViewportIndex);
	BOOL					IsValideObjIndex(DWORD dwObjX,DWORD dwObjZ);
//	BOOL					SetTileOutLine(HFIELD_POS* phfStart,WORD wTileIndex);
public:
	void					SetIndexInGeometry(DWORD dwIndex) {m_dwIndexInGeometry = dwIndex;}
	char*					GetHFieldFileName()	{return m_szFileName;}
	CHFieldTools*			GetHFieldTools() {return m_pHFieldTools;}
	IDIHeightField*			GetIDIHFieldManager() {return m_pIDIHeightField;}
	void					CalcDetail(DWORD dwObjIndex,VECTOR3* pv3ViewPoint);
	void					SetDetailLevel(DWORD dwObjIndex,DWORD dwDetailLevel);
	DWORD					GetDetailLevelNum() {return (DWORD)m_hfDesc.bDetailLevelNum;}
	float*					GetYFactorPtr() {return m_hfDesc.pyfList;}
	DWORD					GetFacesNumX() {return m_hfDesc.dwFacesNumX;}
	DWORD					GetFacesNumZ() {return m_hfDesc.dwFacesNumZ;}
	DWORD					GetFacesNumPerObjAxis() {return m_hfDesc.dwFacesNumPerObjAxis;}
	

	void					GetYFactors(DWORD* pdwPosX,DWORD* pdwPosY,DWORD* pdwWidthNum,DWORD* pdwHeightNum,FLOAT_RECT* pfRect);
	void					SetGeometry(CoGeometry*	pGeometry) {m_pGeometry = pGeometry;}
	CoGeometry*				GetGeometry() {return m_pGeometry;}

	BOOL					Initialize(CoGeometry* pGeometry,I4DyuchiGXRenderer* pRenderer);
	
	

	
    STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);

	COLLISION_MESH_OBJECT_DESC*		__stdcall		GetCollisionMeshObjectDesc(DWORD dwObjIndex);
	void							__stdcall		GetWorldBoundingMesh(COLLISION_MESH_OBJECT_DESC* pColDesc);
	
	BOOL		__stdcall	Create(HFIELD_CREATE_DESC* pDesc,DWORD dwFlag);
	BOOL		__stdcall	LoadTilePalette(TEXTURE_TABLE* pTexTable,DWORD dwTileTextureNum);
	BOOL		__stdcall	ReplaceTile(char* szFileName,DWORD dwTexIndex);
	BOOL		__stdcall	RenderObject(DWORD dwObjIndex,DWORD dwAlpha,DWORD dwFlag);

	BOOL		__stdcall	GetHFieldDesc(HFIELD_DESC* pDesc);
	BOOL		__stdcall	SetYFacorPosList(float* pyfList,DWORD dwYFNumX,DWORD dwYFNumZ);
	BOOL		__stdcall	SetYFactorFromModel(I3DModel* pModel,DWORD dwRefIndex,float fGridLength,float fMinY,float fMaxY);

	DWORD		__stdcall	GetObjectNum();
	DWORD		__stdcall	GetObjectIndex(DWORD dwSeqIndex);
	BOOL		__stdcall	ReadFile(char* szFileName,DWORD dwIndexBufferNumLV0,LOAD_CALLBACK_DESC* pCallbackDesc,DWORD dwFlag);
	BOOL		__stdcall	WriteFile(char* szFileName,DWORD dwFlag);
	
	
	BOOL		__stdcall	BeignShadeLightMapObject(DWORD dwColor);
	BOOL		__stdcall	ShadeLightMapObject(DWORD dwObjIndex,VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pDesc,SHADE_FUNC pFunc);
	void		__stdcall	EndShadeLightMapObject();
	
	DWORD		__stdcall	GetTileListWithScreenCoord(HFIELD_POS* pTileList,DWORD dwMaxTileNum,RECT* pRect,DWORD dwViewportIndex);
	DWORD		__stdcall	GetVertexListWithScreenCoord(HFIELD_POS* pVertexList,DWORD dwMaxVertexNum,RECT* pRect,DWORD dwViewportIndex);
	BOOL		__stdcall	GetVertexPosWithScreenCoord(DWORD* pdwPosX, DWORD* pdwPosZ, VECTOR3 OUT *pWhere, POINT* ptCursor, float fVertexRadius,DWORD dwViewportIndex);
	DWORD		__stdcall	GetObjectPosWithScreenCoord(VECTOR3* pv3IntersectPoint,POINT* ptCursor,DWORD dwViewportIndex);
	BOOL		__stdcall	GetTilePosWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwPosX,DWORD* pdwPosZ,POINT* ptCursor,DWORD dwViewportIndex);
	BOOL		__stdcall	GetTileRectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,RECT* pRect,POINT* ptCursor,DWORD dwBrushSize,DWORD dwViewportIndex);
	BOOL		__stdcall	GetIntersectPointWithScreenCoord(VECTOR3* pv3IntersectPoint,float*pfDist,POINT* ptCursor,DWORD dwViewportIndex);
	BOOL		__stdcall	GetIntersectPointWithRay(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwPosX,DWORD* pdwPosZ,VECTOR3* pv3Orig,VECTOR3* pv3Dir);

	BOOL		__stdcall	GetY(float* pfHeight,float x,float z);
		
	void		__stdcall	CalcXZ(float* px,float* pz,DWORD dwPosX,DWORD dwPosZ);
	void		__stdcall	CalcY(float* py,DWORD dwPosX,DWORD dwPosZ);


	
	
	
	BOOL		__stdcall	ResetCollisionMesh();
	void		__stdcall	ResetHeight(float h);
	void		__stdcall	MakeHeightFieldHigh(DWORD dwX, DWORD dwZ, float fHeight, float fRadius,DWORD dwActionType);
	BOOL		__stdcall	AdjustHeight(HFIELD_POS* pVertexList,DWORD dwVertexNum,float fValue,DWORD dwActionType);
	BOOL		__stdcall	InterpolateField(DWORD dwX, DWORD dwZ, float fBias, float fRadius);
	BOOL		__stdcall	SetTile(DWORD dwPosX,DWORD dwPosZ,WORD wTileIndex);
	BOOL		__stdcall	GetTile(WORD* pwTileIndex,DWORD dwPosX,DWORD dwPosZ);
	BOOL		__stdcall	SetTileRect(RECT* pRect,WORD wTileIndex);
	BOOL		__stdcall	SetTileList(HFIELD_POS* phfStart,HFIELD_POS* pTileList,WORD* pTexTileIndexList,DWORD dwTileNum,WORD wTileIndex);
	void		__stdcall	SetVertexColor(DWORD dwColor);

	BOOL		__stdcall	RenderVertex(HFIELD_POS* pVertexList,DWORD dwVertexNum,DWORD dwColor);
	BOOL		__stdcall	RenderTile(HFIELD_POS* phfStart,HFIELD_POS* pTileList,DWORD dwTileNum,WORD* pwTileIndexList,DWORD dwColor);
	void		__stdcall	SetHeightFieldDetail(DWORD dwDetailLevel);
	void		__stdcall	EnableRender(DWORD dwObjIndex);
	void		__stdcall	DisableRender(DWORD dwObjIndex);
	void		__stdcall	FreezeDetailLevel(DWORD dwObjIndex,DWORD dwDetailLevel);
	void		__stdcall	UnFreezeDetailLevel(DWORD dwObjIndex);

	BOOL		__stdcall	SetLightVertexel(POINT* ptCursor,VECTOR3* pv3IntersectPoint,float fRs,DWORD dwColor,DWORD dwViewportIndex);
	BOOL		__stdcall	GetLightVertexel(DWORD* pdwColor,POINT* ptCursor,VECTOR3* pv3IntersectPoint,DWORD dwViewportIndex);
	void		__stdcall	Optimize();
	DWORD		__stdcall	GetXZFrom3DCoord(HFIELD_POS* phfPos,DWORD dwMaxHFPosNum,VECTOR3* pv3IntersectPoint,float fRs);
	BOOL		__stdcall	SetAlphaTexel(POINT* ptCursor,VECTOR3* pv3IntersectPoint,float fRs,DWORD dwTileIndex,DWORD dwActionFlag,DWORD dwViewportIndex);
	DWORD		__stdcall	GetExtTileIndexTop(POINT* ptCursor,VECTOR3* pv3IntersectPoint,DWORD dwViewportIndex);

	void		__stdcall	EnableBlend(BOOL bEnable);
	void		__stdcall	UpdateAllTiles();
	void		__stdcall	UpdateAllIDIMeshObjects();

	void		__stdcall	ClearAlphaMapAll();
	void		__stdcall	ClearAlphaMapWithObjectIndex(DWORD dwObjIndex);
	void		__stdcall	ClearAlphaMapWithTileIndex(DWORD dwObjIndex,DWORD dwTileIndex);
	void		__stdcall	GetAlphaMapResourceStatus(DWORD* pdwMapNum,DWORD* pdwMemSize);
	DWORD		__stdcall	QueryNeedTriNumWithAABB(DWORD* pdwStartX,DWORD* pdwStartZ,DWORD* pdwWidthNum,DWORD* pdwHeightNum,MAABB* pAABB);
	DWORD		__stdcall	GetTriListWithAABB(VECTOR3* pv3TriListOut, DWORD dwMaxTriCount, MAABB* pAABB);
		

	CoHeightField();
	~CoHeightField();



};

#endif

