#pragma once

#include "BaseMeshObject.h"
#include "d3d_helper.h"
#include "VertexBuffer.h"
#include "CTexture.h"

class CLMFaceGroup;

class CLMMeshObject : public CBaseMeshObject
{	
	VECTOR3*				m_pv3Local;
	TVERTEX*				m_ptvTex;
		
	CLMFaceGroup*			m_pFaceGroup;
	CVertexBuffer			m_VBProjected;
	CTexture				m_TexStaticLight;
	VECTOR3					m_v3CameraPointPrv;
//	MATRIX4					m_matViewPrv;
//	LPDIRECT3DTEXTURE8		m_pStaticLightTexture;

	DWORD					m_dwSurfaceWidth;
	DWORD					m_dwSurfaceHeight;
	char*					m_pImageBuffer;

	TEXTURE_PLANE*			m_pTexturePlane;
	
	DWORD					m_dwTexturePlaneNum;
	CMeshFlag				m_meshFlag;

//	BOOL					m_bMustRestoreLightTexture;
//	LIGHT_DESC				m_lightDescPrv;
//	DWORD					m_dwLightNumPrv;
	
	BOOL					CreateStaticLightTexture(char* pBits,DWORD dwWidth,DWORD dwHeight,DWORD dwBPS);
	
	BOOL					CreatePatch(char* pBits,DWORD dwXSize,DWORD dwYSize,DWORD dwBPS);
	void					SortWithAlpha();
	BOOL					Render(DWORD dwColor);
//	void					RestoreLightTexture();
	
public:
	// IDIMeshObject의 인터페이스 
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

	BOOL	__stdcall		Update(DWORD dwFlag);
	

	
	
	CLMMeshObject();
	virtual ~CLMMeshObject();

};