#include "Base_facegroup.h"
#include "../4DyuchiGXGFunc/global.h"
#include "IBCache.h"


void CBaseFaceGroup::CullFaceListWithCameraEye(VECTOR3* pv3Eye)
{
	m_dwCulledWithCameraFacesNum = CullBackFace(m_pwFaceIndexList,m_pFaceList,pv3Eye,m_dwFacesNum);
}
DWORD CBaseFaceGroup::CullFaceListWithLight(CIndexBuffer* pIB,VECTOR3* pLightPoint)
{
	WORD*	pDest;
	DWORD dwCulledWithLightFacesNum = 0;

	if (!m_dwCulledWithCameraFacesNum)
		goto lb_return;


	pIB->LockPtr(&pDest);
	dwCulledWithLightFacesNum = ::BuildIndexBufferWithFaceIndexList	(pDest,m_pFaceList,m_pwFaceIndexList,pLightPoint,m_dwCulledWithCameraFacesNum);
	pIB->UnlockPtr();

lb_return:
	return dwCulledWithLightFacesNum;
}
DWORD CBaseFaceGroup::CullFaceListWithShadowLight(CIndexBuffer* pIB,VECTOR3* pLightPoint,BOUNDING_SPHERE* pSphere)
{
	WORD*	pDest;

	DWORD	dwCulledWithShadowLightFacesNum = 0;
	if (!m_dwCulledWithCameraFacesNum)
		goto lb_return;

	pIB->LockPtr(&pDest);
	dwCulledWithShadowLightFacesNum = ::BuildIndexBufferWithFaceIndexListForShadow(pDest,m_pFaceList,m_pwFaceIndexList,pLightPoint,pSphere,m_dwCulledWithCameraFacesNum);
	pIB->UnlockPtr();

lb_return:
	return dwCulledWithShadowLightFacesNum;

}
DWORD CBaseFaceGroup::BuildPrjLightMesh(CIBCache* pIBCache,DWORD dwLightIndex,VECTOR3* pv3LightPos,BOOL bCameraUpdate)
{
	
	
	pIBCache->Hit();
	if (!m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].pIBPrjLight)
		goto lb_update;

	if (bCameraUpdate)
	{
		// 카메라 위치가 변경되면 무조건 업데이트.
		goto lb_update_and_free;
	}

	float	fDist;
	fDist = CalcDistance(pv3LightPos,&m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].v3LightPosPrv);
	if (fDist > MIN_UNIT*2.0f)
		goto lb_update_and_free;

	/*

	// 라이트 위치가 변경되면 업데이트 
	if (pv3LightPos->x != m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].v3LightPosPrv.x)
		goto lb_update_and_free;

	if (pv3LightPos->y != m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].v3LightPosPrv.y)
		goto lb_update_and_free;

	if (pv3LightPos->z != m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].v3LightPosPrv.z)
		goto lb_update_and_free;
*/
	goto lb_return;

lb_update_and_free:
	pIBCache->FreeIB(m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].pIBPrjLight);

lb_update:
	m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].v3LightPosPrv = *pv3LightPos;
	
	if (!pIBCache->GetIB(&m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].piUseCount,m_dwCulledWithCameraFacesNum*3,&m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].pIBPrjLight,(void*)this))
	{
		m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].dwCulledWithLightFacesNum = 0;
		goto lb_return;
	}

	m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].dwCulledWithLightFacesNum = 
		CullFaceListWithLight(m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].pIBPrjLight,pv3LightPos);

	(*m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].piUseCount)++;
	
lb_return:
	return m_pPrjMeshDesc->prjLightMeshList[dwLightIndex].dwCulledWithLightFacesNum;
	
}

DWORD CBaseFaceGroup::BuildPrjShadowMesh(CIBCache* pIBCache,DWORD dwShadowIndex,VECTOR3* pv3ShadowLightPos,BOUNDING_SPHERE* pShadowSenderPos,BOOL bCameraUpdate)
{

	pIBCache->Hit();
	if (!m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].pIBPrjShadow)
		goto lb_update;

	
	IBCACHE_ITEM*	pDelItem;
	pDelItem = (IBCACHE_ITEM*)m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].pIBPrjShadow;
//	if (pDelItem->dwIndexInAllocatedTable == 0xcccccccc)
//		__asm int 3


	if (bCameraUpdate)
		goto lb_update_and_free;

	// 라이트 위치가 변경되면 업데이트 
	float	fDist;
	fDist = CalcDistance(pv3ShadowLightPos,&m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].v3ShadowLightPos);
	if (fDist > MIN_UNIT*2.0f)
		goto lb_update_and_free;

	fDist = CalcDistance(&pShadowSenderPos->v3Point,&m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].bpShadowSenderPrv.v3Point);
	if (fDist > MIN_UNIT*2.0f)
		goto lb_update_and_free;
		
	/*
	if (pv3ShadowLightPos->x != m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].v3ShadowLightPos.x)
		goto lb_update_and_free;

	if (pv3ShadowLightPos->y != m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].v3ShadowLightPos.y)
		goto lb_update_and_free;

	if (pv3ShadowLightPos->z != m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].v3ShadowLightPos.z)
		goto lb_update_and_free;

	if (pShadowSenderPos->v3Point.x != m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].bpShadowSenderPrv.v3Point.x)
		goto lb_update_and_free;

	if (pShadowSenderPos->v3Point.y != m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].bpShadowSenderPrv.v3Point.y)
		goto lb_update_and_free;

	if (pShadowSenderPos->v3Point.z != m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].bpShadowSenderPrv.v3Point.z)
		goto lb_update_and_free;

	if (pShadowSenderPos->fRs != m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].bpShadowSenderPrv.fRs)
		goto lb_update_and_free;
*/
	goto lb_return;

lb_update_and_free:

	pIBCache->FreeIB(m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].pIBPrjShadow);
	

lb_update:
	m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].v3ShadowLightPos = *pv3ShadowLightPos;
	m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].bpShadowSenderPrv = *pShadowSenderPos;
	
	if (!pIBCache->GetIB(&m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].piUseCount,m_dwCulledWithCameraFacesNum*3,&m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].pIBPrjShadow,(void*)this))
	{
		m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].dwCulledWithShadowLightFacesNum = 0;
		goto lb_return;
	}

	m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].dwCulledWithShadowLightFacesNum = 
		CullFaceListWithShadowLight(m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].pIBPrjShadow,pv3ShadowLightPos,pShadowSenderPos);

	(*m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].piUseCount)++;
	
lb_return:
	return m_pPrjMeshDesc->prjShadowMeshList[dwShadowIndex].dwCulledWithShadowLightFacesNum;

}

void CBaseFaceGroup::ReleasePrjMesh(CIBCache* pIBCache)
{
	if (m_pPrjMeshDesc)
	{
		DWORD i;
		for (i=0; i<MAX_PRJIMAGE_LIGHT_NUM_PER_SCENE; i++)
		{
			if (m_pPrjMeshDesc->prjLightMeshList[i].pIBPrjLight)
			{
				pIBCache->FreeIB(m_pPrjMeshDesc->prjLightMeshList[i].pIBPrjLight);
			}
		}

		for (i=0; i<MAX_SHADOW_LIGHT_NUM_PER_SCENE; i++)
		{
			if (m_pPrjMeshDesc->prjShadowMeshList[i].pIBPrjShadow)
			{
				pIBCache->FreeIB(m_pPrjMeshDesc->prjShadowMeshList[i].pIBPrjShadow);
			}
		}
		delete m_pPrjMeshDesc;
		m_pPrjMeshDesc = NULL;
	}
}
CBaseFaceGroup::~CBaseFaceGroup()
{

	if (m_pPrjMeshDesc)
	{
		delete m_pPrjMeshDesc;
		m_pPrjMeshDesc = NULL;
	}

	if (m_pFaceList)
	{
		delete [] m_pFaceList;
		m_pFaceList = NULL;
	}
	if (m_pwFaceIndexList)
	{
		delete [] m_pwFaceIndexList;
		m_pwFaceIndexList = NULL;
	}
}