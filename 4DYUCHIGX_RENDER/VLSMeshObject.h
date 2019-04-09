#pragma once

#include "VLMeshObject.h"


class CVLFaceGroup;

class CVLSMeshObject : public CVLMeshObject
{
	VECTOR3*				m_pv3Local;
	CVertexBuffer			m_VB;
	CVertexBuffer			m_VBProjected;
	VECTOR3					m_v3CameraPointPrv;
	DWORD*					m_pVertexColor;
	
//	MATRIX4					m_matViewPrv;


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

	BOOL	__stdcall		Update(DWORD dwFlag);
	

	CVLSMeshObject();
};

