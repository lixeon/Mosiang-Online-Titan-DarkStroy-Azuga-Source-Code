#ifndef HFIELD_OBJECT_H
#define HFIELD_OBJECT_H

#include "BaseMeshObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "CTexture.h"
#include "TileObject.h"

class CHFieldManager;

class CHFieldObject : public CBaseMeshObject,public IDIHFieldObject,public IResourceController
{
	CHFieldManager*		m_pHFieldManager;
	CVertexBuffer		m_VB;
	CIndexBuffer		m_IB;
	DWORD				m_dwDetailLevel;
	DWORD				m_dwPositionMask;
	VECTOR3*			m_pv3Rect;
	DWORD				m_dwPosX;
	DWORD				m_dwPosZ;
	DWORD				m_bMustUpdate;
	float				m_fDistanceFromViewPoint;



//	CTileObject			m_TileObject;
	
	DWORD				m_dwTriNum;
	IGeometryControllerStatic* m_pControllerStatic;
	

public:
	friend class			CHFieldManager;
	void					ReleaseIndexBuffer();
	void					ReleaseVertexBuffer();
	

	
	DWORD					GetDetailLevel()	{return m_dwDetailLevel;}
	void					GetTileBuffer(TILE_BUFFER_DESC** ppTileBufferDesc,DWORD* pdwTileBufferDescNum)
	{
		m_pControllerStatic->GetTileBuffer(ppTileBufferDesc,pdwTileBufferDescNum);

	//	m_TileObject.GetTileBuffer(ppTileBufferDesc,pdwTileBufferDescNum);
	}

	// IDIMeshObject의 인터페이스 
	STDMETHODIMP			QueryInterface(REFIID refiid, PPVOID ppv);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);

	
	BOOL	__stdcall		StartInitialize(MESH_DESC* pDesc,IGeometryController* pControl,IGeometryControllerStatic* pControlStatic);
	BOOL	__stdcall		InsertFaceGroup(FACE_DESC* pDesc);
	void	__stdcall		EndInitialize();

	BOOL	__stdcall		Render(
		DWORD dwRefIndex,										// 애니메이션 컨트롤블럭
		DWORD dwAlpha,
		LIGHT_INDEX_DESC* pDynamicLightIndexList,DWORD dwLightNum,
		LIGHT_INDEX_DESC* pSpotLightIndexList,DWORD dwSpotLightNum,
		DWORD dwMtlSetIndex,
		DWORD dwEffectIndex,
		DWORD dwFlag					
		);
	
	DWORD	__stdcall		GetAlphaFlag();

	BOOL	__stdcall		Update(DWORD dwFlag);


	// IDIHFieldObject의 인터페이스 
	BOOL		__stdcall	Create(DWORD dwPosX,DWORD dwPosZ,DWORD dwDetailLevel,DWORD dwFacesNumPerX,DWORD dwFacesNumPerZ,VECTOR3* pv3Rect,HFIELD_DESC* pHFDesc);
	BOOL		__stdcall	SetYFactor(DWORD dwDestPitch,HFIELD_DESC* pHFDesc);
	BOOL		__stdcall	SetVertexColor(DWORD* pdwColor,DWORD dwVerticesNum);
	BOOL		__stdcall	SetVertexColorAll(DWORD dwColor);
	BOOL		__stdcall	ReBuildMesh(DWORD dwDestPitch,HFIELD_DESC* pHFDesc , DWORD* pdwColor);
	BOOL		__stdcall	SetDetailLevel(DWORD dwDetailLevel);
	void		__stdcall	SetDistanceFromViewPoint(float fDistance);
	void		__stdcall	SetPositionMask(DWORD dwPosMask);			// 0-15
	void		__stdcall	SetMustUpdate();
	BOOL		__stdcall	UpdateAlphaMap(TILE_BUFFER_DESC* pTileBufferDesc);
	void		__stdcall	CleanupAlphaMap();
	

	BOOL		__stdcall	IsIncludeViewVolume();
	BOOL		__stdcall	OnFault();
	BOOL		__stdcall	OnOut();



	CHFieldObject(CHFieldManager* pHFieldManager,DWORD dwFlag);
	virtual ~CHFieldObject();

};


#endif
