#ifndef BASE_FACE_GROUP_H
#define BASE_FACE_GROUP_H

#include "../4DyuchiGRX_Common/typedef.h"
#include "IndexBuffer.h"
#include "renderer_typedef.h"
#include "MaterialManager.h"


struct PRJ_LIGHT_MESH_DESC
{
	CIndexBuffer*			pIBPrjLight;
	int*					piUseCount;
	VECTOR3					v3LightPosPrv;
	DWORD					dwCulledWithLightFacesNum;
		

};

struct PRJ_SHADOW_MESH_DESC
{
	CIndexBuffer*			pIBPrjShadow;
	int*					piUseCount;
	VECTOR3					v3ShadowLightPos;
	BOUNDING_SPHERE			bpShadowSenderPrv;
	DWORD					dwCulledWithShadowLightFacesNum;

};

struct PRJ_MESH_DESC
{
	PRJ_LIGHT_MESH_DESC		prjLightMeshList[MAX_PRJIMAGE_LIGHT_NUM_PER_SCENE];
	PRJ_SHADOW_MESH_DESC	prjShadowMeshList[MAX_SHADOW_LIGHT_NUM_PER_SCENE];
};
class CIBCache;

class CBaseFaceGroup
{
	
protected:
	DWORD					m_dwFacesNum;
	DIMATERIAL*				m_pMaterial;

	TRI_FACE*				m_pFaceList;
	DWORD					m_dwCulledWithCameraFacesNum;
	

	WORD*					m_pwFaceIndexList;
	DWORD					m_dwMtlIndex;

	PRJ_MESH_DESC*			m_pPrjMeshDesc;

public:
	void					ReleasePrjMesh(CIBCache* pIBCache);
	

	CIndexBuffer*			GetIBPrjLightPtr(DWORD dwLightIndex) {return m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].pIBPrjLight;}
	CIndexBuffer*			GetIBPrjShadowPtr(DWORD dwShadowIndex) {return m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].pIBPrjShadow;}
		

	void					SetMtlIndex(DWORD dwIndex) {m_dwMtlIndex = dwIndex;}
	DWORD					GetMtlIndex()	{return m_dwMtlIndex;}

	void					SetMaterial(DIMATERIAL* pMtl,DWORD dwMtlIndex) {m_pMaterial = pMtl; m_dwMtlIndex = dwMtlIndex;}
	DIMATERIAL*				GetMaterial() { return m_pMaterial;}
	
	DWORD					GetCulledWithCameraFacesNum() {return m_dwCulledWithCameraFacesNum;}

//	DWORD					GetCulledWithLightFacesNum(DWORD dwLightIndex) {return m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].dwCulledWithLightFacesNum;}
//	DWORD					GetCulledWithShadowLightFacesNum(DWORD dwShadowIndex) {return m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].dwCulledWithShadowLightFacesNum;}



	DWORD					GetFacesNum()			{return m_dwFacesNum;}

	DWORD					BuildPrjLightMesh(CIBCache* pIBCache,DWORD dwLightIndex,VECTOR3* pv3LightPos,BOOL bCameraUpdate);
	DWORD					BuildPrjShadowMesh(CIBCache* pIBCache,DWORD dwShadowIndex,VECTOR3* pv3ShadowLightPos,BOUNDING_SPHERE* pShadowSenderPos,BOOL bCameraUpdate);
	
	void					CullFaceListWithCameraEye(VECTOR3* pv3Eye);

	DWORD					CullFaceListWithLight(CIndexBuffer* pIB,VECTOR3* pLightPoint);
	DWORD					CullFaceListWithShadowLight(CIndexBuffer* pIB,VECTOR3* pLightPoint,BOUNDING_SPHERE* pSphere);

	

	~CBaseFaceGroup();
};
#endif

