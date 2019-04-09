#ifndef HFIELD_OBJECT_H
#define HFIELD_OBJECT_H

#include "../4DyuchiGRX_common/IGeometry.h"
#include "../4DyuchiGRX_common/IRenderer.h"
#include "LightTexture.h"
#include "TileObject.h"
class CoHeightField;


enum HFOBJ_RENDER_TYPE
{
	HFOBJ_RENDER_TYPE_DETAIL_FIX = 0x00000001,
	HFOBJ_RENDER_TYPE_INVISIBLE	= 0x00000002

};

#define HFOBJ_RENDER_TYPE_DETAIL_FIX_MASK_INVERSE	0xfffffffe
#define HFOBJ_RENDER_TYPE_INVISIBLE_MASK_INVERSE	0xfffffffd

class CHFieldObject : public IGeometryControllerStatic
{
	friend class	CTileObject;
	CTileObject					m_TileObject;
	HFIELD_OBJECT_DESC			m_HFObjDesc;
	IDIMeshObject*				m_pDeviceObject;
	IDIHFieldObject*			m_pHFieldControlObject;
	COLLISION_MESH_OBJECT_DESC*	m_pColMeshObjDesc;
	CoHeightField*				m_pHField;
	DWORD						m_dwDetailLevel;
	CHFieldObject*				m_pSideObj[4];
	VECTOR3						m_v3Rect[4];		// 네 귀퉁이의 버텍스 포지션.
	DWORD						m_dwPosX;			// 버텍스인덱스 기준으로 x좌표
	DWORD						m_dwPosZ;			// 버텍스인덱스 기준으로 z좌표
	DWORD*						m_pdwVertexColor;
	VECTOR3*					m_pv3VertexNormal;
	VECTOR3*					m_pv3Point;
	DWORD						m_dwPropertyFlag;
	float						m_fDistanceFromViewPoint;
	BOOL						m_bRenderEnable;
	

public:
	
	BOOL			__stdcall	IsIncludeViewVolume();
	BOOL			__stdcall	OnFault();
	BOOL			__stdcall	OnOut();

	void				__stdcall	GetTileBuffer(TILE_BUFFER_DESC** ppTileBufferDesc,DWORD* pdwTileBufferDescNum);
	DWORD				__stdcall	GetTileBufferDescNum();
	TILE_BUFFER_DESC*	__stdcall	GetTileBufferDesc(DWORD dwIndex);

	void						CleanupAlphaMap() {m_TileObject.CleanupAlphaMap();}
	void						CleanupAlphaMapWithTileIndex(WORD wTileIndex) {m_TileObject.CleanupAlphaMapWithTileIndex(wTileIndex);}

	IDIHFieldObject*			GetHFieldControlObject() {return m_pHFieldControlObject;}
	CoHeightField*				GetHField(CTileObject* pTileObject);
	BOOL						BuildAllToIDIMeshObject();


	DWORD						GetPropertyFlag()	{return m_dwPropertyFlag;}
	void						SetPropertyFlag(DWORD dwFlag) {m_dwPropertyFlag = dwFlag;}

	void						FreezeDetail()		{m_dwPropertyFlag |= HFOBJ_RENDER_TYPE_DETAIL_FIX;}
	void						UnFreezeDetail()	{m_dwPropertyFlag &= HFOBJ_RENDER_TYPE_DETAIL_FIX_MASK_INVERSE;}

	void						EnableRender()		{m_dwPropertyFlag &= HFOBJ_RENDER_TYPE_INVISIBLE_MASK_INVERSE;}	
	void						DisableRender()		{m_dwPropertyFlag |= HFOBJ_RENDER_TYPE_INVISIBLE;}

	int							IsInvisible()	{return (m_dwPropertyFlag & HFOBJ_RENDER_TYPE_INVISIBLE);}
	DWORD						GetVertexPosX()		{return m_dwPosX;}
	DWORD						GetVertexPosZ()		{return m_dwPosZ;}
	

	COLLISION_MESH_OBJECT_DESC*			GetColMeshDesc() {return m_pColMeshObjDesc;}
	VECTOR3*			GetRect() {return m_v3Rect;}
	DWORD				GetDetailLevel()	{return m_dwDetailLevel;}
	void				SetSideObject(CHFieldObject* pObj,DWORD dwIndex) { m_pSideObj[dwIndex] = pObj;}
	void				SetColMeshDesc(COLLISION_MESH_OBJECT_DESC* pDesc) {m_pColMeshObjDesc = pDesc;}
	void				SetDetailLevel(DWORD dwDetailLevel);	
	void				SetDistanceFromViewPoint(float fDistance);
	void				SetPositionMask();
	BOOL				UpdateVertexPos();
	BOOL				UpdateTile();
	BOOL				ResetCollisionMesh();

	void				SetVertexColor(DWORD dwColor);

	BOOL				BeginShadeVertexColor(DWORD dwColor);
	BOOL				ShadeVertexColor(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,DWORD dwFlag,SHADE_FUNC pFunc);
	void				EndShadeVertexColor(FILE* fp);

	DWORD				ReadVertexColor(void* pFP);


	BOOL				SetLightVertexel(HFIELD_POS* pPos,DWORD dwHFPosNum,DWORD dwColor);
	BOOL				GetLightVertexel(DWORD* pdwColor,HFIELD_POS* pPos);
	BOOL				SetAlphaTexel(VECTOR3* pv3IntersectPoint,float fRs,DWORD dwTexWidthHeight,DWORD dwTileIndex,DWORD dwActionFlag);
	DWORD				GetExtTileTop(DWORD dwTilePosX,DWORD dwTilePosZ) {return m_TileObject.GetExtTileTop(dwTilePosX,dwTilePosZ);}

	void				GetAlphaMap(char** pp2BitsImg,DWORD* pdwWidthHeight,WORD wTileIndex) {m_TileObject.GetAlphaMap(pp2BitsImg,pdwWidthHeight,wTileIndex);}
	void				SetAlphaMap(WORD wTileIndex,char* p2BitsImg,DWORD dwWidthHeight) {m_TileObject.SetAlphaMap(wTileIndex,p2BitsImg,dwWidthHeight);}
	DWORD				WriteAlphaMap(char* pMemDest,DWORD dwObjIndex,DWORD dwImgSize) {return m_TileObject.WriteAlphaMap(pMemDest,dwObjIndex,dwImgSize);}
	void				ReleaseVertexColor();
	void				Optimize();
	BOOL				UpdateIDIMeshObject();
	BOOL				UpdateExtTileInfo(HFIELD_DESC* pHFDesc,WORD wTileIndex,BOOL bSpecifiedTileIndex) {return m_TileObject.UpdateExtTileInfo(pHFDesc,wTileIndex,bSpecifiedTileIndex);}





	IDIMeshObject*		GetDeviceObject()	{return m_pDeviceObject;}
	BOOL				Initialize(CoHeightField* pHField,HFIELD_OBJECT_DESC* pHFObjDesc,DWORD dwFlag);
	CHFieldObject();
	~CHFieldObject();
};

#endif

