#pragma once

#include "../4DyuchiGRX_Common/IRenderer.h"
#include "../4DyuchiGRX_Common/IGeometry.h"
#include "GXMapObject.h"

class CoGXObject;



class CoGXLight : public CGXMapObject
{
	I3DModel*				m_pSymbolModel;
	DWORD					m_dwModelRefIndex;

	LIGHT_DESC				m_LightDesc;
	
	// 프로젝티브 이미지 라이트일때 텍스쳐 크기 
	DWORD					m_dwWidth;
	DWORD					m_dwHeight;

	VECTOR3					m_v3FromDistancePivot;	// 링크될 당시의 부모오브젝트의 피봇과의 거리 
	VECTOR3					m_v3Dir;				// from에서 to까지 방향,거리 벡터 
	

	MATRIX4*				m_pParentMatrix;
	CoGXObject*				m_pParentGXObject;

	GXLightSchedulePROC		m_pProc;
	COLLISION_MESH_OBJECT_DESC	m_colMeshDesc;
	void*					m_pExternalItem;
	void					Cleanup();
	
public:
	COLLISION_MESH_OBJECT_DESC*	__stdcall	GetCollisionMeshObjectDesc();
	DWORD						__stdcall	OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2);
	DWORD						__stdcall	GetObjectIndexInModel();
	BOOL						__stdcall	Render();		
	DWORD						__stdcall	CreateIVertexList(IVERTEX** ppVertex);
	void						__stdcall	ReleaseIVertexList(IVERTEX*	pVertex);
	BOOL						__stdcall	ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc);
	

	BOOL						__stdcall	IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
	
	DWORD					GetImageWidth() {return m_dwWidth;}
	DWORD					GetImageHeight() {return m_dwHeight;}
	BOOL					Initialize(CoExecutive* pExecutive,LIGHT_DESC* pDesc,MATERIAL* pMtl,GXSchedulePROC pProc,DWORD dwFlag);
	void					UpdateCollisionMesh();
	void					GetLightDesc(LIGHT_DESC* pLightDesc) {*pLightDesc = m_LightDesc;}
	void					SetLightDesc(LIGHT_DESC* pLightDesc);

	void					DisableStaticShadow()	{ m_dwPropertyFlag |=GXLIGHT_TYPE_STATIC_SHADOW_DISABLE;}
	void					EnableStaticShadow()	{ m_dwPropertyFlag &= GXLIGHT_TYPE_STATIC_SHADOW_DISABLE_INVERSE;}
	BOOL					IsDiableStaticShadow()	{ if (m_dwPropertyFlag & GXLIGHT_TYPE_STATIC_SHADOW_DISABLE) return TRUE; else return FALSE;}

	void					EnableDynamicLight()	{ m_dwPropertyFlag |=GXLIGHT_TYPE_ENABLE_DYNAMIC_LIGHT;}
	void					DisableDynamicLight()	{ m_dwPropertyFlag &= GXLIGHT_TYPE_ENABLE_DYNAMIC_LIGHT_INVERSE;}
	BOOL					IsEnableDynamicLight()	{ if (m_dwPropertyFlag & GXLIGHT_TYPE_ENABLE_DYNAMIC_LIGHT) return TRUE; else return FALSE;}
	
	

	void					SetPosition(VECTOR3* pv3Pos);
	void					MovePosition(VECTOR3* pv3Pos);
	void					GetPosition(VECTOR3* pv3Pos) {*pv3Pos = m_LightDesc.v3Point;}
	void					PostAttach(CoGXObject* pObj,MATRIX4* pMat,VECTOR3* pv3Offset,DWORD dwAttachType);
	void					PostDetach();
	
	void					DefaultProc();

	
	DWORD					Release();

	/////////////////////////*/
};
