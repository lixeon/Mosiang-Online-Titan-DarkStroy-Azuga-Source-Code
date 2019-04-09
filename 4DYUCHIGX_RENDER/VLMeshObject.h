#pragma once

#include "BaseMeshObject.h"
#include "vertexbuffer.h"


class CVLFaceGroup;

class CVLMeshObject : public CBaseMeshObject
{
protected:

	CVertexBuffer			m_VB;
	CVLFaceGroup*			m_pFaceGroup;
		
	void					SortWithAlpha();
	BOOL					Render(CVertexBuffer* pVB,DWORD dwColor);
public:
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

	BOOL	__stdcall		RenderProjection(
		DWORD		dwRefIndex,										// 애니메이션 컨트롤블럭
		DWORD		dwAlpha,
		BYTE*		pSpotLightIndex,
		DWORD		dwViewNum,
		DWORD		dwFlag
		);
BOOL __stdcall CVLMeshObject::RenderEffect(
		CVertexBuffer*		pRenderVB,
		CVLFaceGroup*		pFaceGroup,DWORD dwFaceGroupNum,
		DWORD				dwAlpha,
		VLMESH_EFFECT_DESC*	pEffect,
		MATRIX4*			pMatWorld,
		DWORD				dwMtlSetIndex,
		DWORD				dwFlag					
		);

	BOOL	__stdcall		Update(DWORD dwFlag);
		

	CVLMeshObject();
	virtual ~CVLMeshObject();
	
};

