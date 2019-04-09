#include "model.h"
#include "geom_obj.h"
#include "camera_obj.h"
#include "light_obj.h"

#include "../4DyuchiGXGFunc/global.h"
#include "../4DyuchiGRX_common/typedef.h"
#include "../4DyuchiGRX_common/IRenderer.h"
#include "../4DyuchiGRX_common/IGeometry.h"
#include "bone_object.h"
#include "CoGeometry.h"
#include "geometry_global.h"
#include "MaterialLib.h"
#include "global_variable.h"
#include "../4dyuchigrx_myself97/myself97common.h"


//#pragma optimize( "", off )

#define _AXIS_ALIGN_OK

STDMETHODIMP CoModel::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (IID_IUnknown == refiid)
		*ppv = this;
	else if (IID_3DModel == refiid)
		*ppv = (I3DModel*)this;
	else 
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();
	return NOERROR;
	
}
STDMETHODIMP_(ULONG) CoModel::AddRef(void)
{
	m_dwRefCount++;
	return 0;
}
STDMETHODIMP_(ULONG) CoModel::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}

CoModel::CoModel()
{
	memset((char*)this+4,0,sizeof(CoModel)-4);

//	m_dwDefaultMtlSetIndex = 0xffffffff;
}
void CoModel::SetGeometry(CoGeometry* pGeometry)
{
	m_pGeometry = pGeometry;
	m_pModelRefManager = m_pGeometry->GetModelRefManager();
}

DWORD CoModel::AddMotion(char* szFileName,DWORD dwRefIndex,DWORD dwMotionIndex)
{
	DWORD				dwResult = 0xffffffff;
	CMotion*			pMotion;

	if (!m_pModelRefManager->IsValidRefIndex(dwRefIndex,m_dwColMeshSize))
		goto lb_return;

	pMotion = m_pGeometry->LoadMotion(szFileName);
	
	if (!pMotion)
		goto lb_return;

	dwResult = AddMotion(pMotion,dwRefIndex,dwMotionIndex);
	if (dwResult == 0xffffffff)
		pMotion->Release();

lb_return:

	return dwResult;
}


DWORD CoModel::AddMotion(CMotion* pMotion,DWORD dwRefIndex,DWORD dwMotionIndex)
{
	DWORD				i;
	DWORD				dwResult = 0xffffffff;
	
	MOTION_LIST_DESC*	pMotionListDesc;
	if (!m_pModelRefManager->GetMotionListDesc(&pMotionListDesc,dwRefIndex))
		goto lb_return;


	if (dwMotionIndex > pMotionListDesc->dwMaxMotionNum)
		goto lb_return;

	if (dwMotionIndex > 0)
	{
		// 애니메이션 인덱스를 지정한 경우..바꾼다.
		DeleteMotion(dwRefIndex,dwMotionIndex);
		dwResult = dwMotionIndex;
		goto lb_set;
	}

	if (pMotionListDesc->dwMotionNum > pMotionListDesc->dwMaxMotionNum)
		goto lb_return;

	for (i=1; i<=pMotionListDesc->dwMaxMotionNum; i++)
	{
		if (!pMotionListDesc->ppMotionList[i])
		{
			dwResult = i;
			goto lb_set;
		}
	}
	goto lb_return;

lb_set:
	pMotionListDesc->ppMotionList[dwResult] = pMotion;
	pMotionListDesc->dwMotionNum++;


lb_return:
	return dwResult;
}

CMotion* CoModel::GetMotion(DWORD dwRefIndex,DWORD dwMotionIndex)
{
	CMotion*	pMotion = NULL;
	MOTION_LIST_DESC*	pMotionListDesc;

	if (!m_pModelRefManager->GetMotionListDesc(&pMotionListDesc,dwRefIndex))
		goto lb_return;

	if (!pMotionListDesc->dwMotionNum)
		goto lb_return;

	if (dwMotionIndex > pMotionListDesc->dwMaxMotionNum)
		goto lb_return;
	
	pMotion = pMotionListDesc->ppMotionList[dwMotionIndex];
	
	
lb_return:
	return pMotion;
}

void __stdcall CoModel::DeleteMotion(DWORD dwRefIndex,DWORD dwMotionIndex)
{
	
	MOTION_LIST_DESC*	pMotionListDesc;
	CMotion*			pMotion;

	if (!m_pModelRefManager->GetMotionListDesc(&pMotionListDesc,dwRefIndex))
		goto lb_return;

	if (!pMotionListDesc->dwMotionNum)
		goto lb_return;

	if (dwMotionIndex > pMotionListDesc->dwMaxMotionNum)
		goto lb_return;

	pMotion = pMotionListDesc->ppMotionList[dwMotionIndex];

	if (!pMotion)
		goto lb_return;

	pMotion->Release();
	pMotionListDesc->ppMotionList[dwMotionIndex] = NULL;
	pMotionListDesc->dwMotionNum--;
	
lb_return:
	return;
}

void __stdcall CoModel::DeleteAllMotionPerReference(DWORD dwRefIndex)
{
	DWORD		i;
	MOTION_LIST_DESC*	pMotionListDesc;
	if (!m_pModelRefManager->GetMotionListDesc(&pMotionListDesc,dwRefIndex))
		goto lb_return;

		
	for (i=0; i<=pMotionListDesc->dwMaxMotionNum; i++)
		DeleteMotion(dwRefIndex,i);
	
		
	if (pMotionListDesc->ppMotionList)
	{
		delete [] pMotionListDesc->ppMotionList;
		pMotionListDesc->ppMotionList = NULL;
		pMotionListDesc->dwMotionNum = 0;
		pMotionListDesc->dwMaxMotionNum = 0;
	}
	
lb_return:
	return;

}
void __stdcall CoModel::DeleteAllMotion()
{
	DWORD	dwMaxIndexNum = m_pModelRefManager->GetRefNum();
	if (!dwMaxIndexNum)
		return;

	DWORD*	pdwRefIndexList = new DWORD[dwMaxIndexNum];
	DWORD dwIndexNum = m_pModelRefManager->GetRefIndexWithModelPtr(pdwRefIndexList,this,dwMaxIndexNum);
	for (DWORD i=0; i<dwIndexNum; i++)
	{
		DeleteAllMotionPerReference(pdwRefIndexList[i]);
	}
	delete [] pdwRefIndexList;

}

BOOL __stdcall CoModel::UnloadMotionPerReference(DWORD dwRefIndex)
{
	DWORD		i;
	MOTION_LIST_DESC*	pMotionListDesc;

	BOOL		bResult = FALSE;
		
	if (!m_pModelRefManager->GetMotionListDesc(&pMotionListDesc,dwRefIndex))
		goto lb_return;

	CMotion*	pMotion;
		
	for (i=0; i<=pMotionListDesc->dwMaxMotionNum; i++)
	{
		pMotion = GetMotion(dwRefIndex,i);
		if (pMotion)
		{
			pMotion->Unload();
		}
	}
	bResult = TRUE;
lb_return:
	return bResult;
}

BOOL __stdcall CoModel::UnloadMaterialPerReference(DWORD dwRefIndex)
{
	CMaterialLib*			pMtlLib;
	MATERIAL_LIST_DESC*		pMaterialListDesc;
	BOOL					bResult = FALSE;
	DWORD					i;

	if (!m_pModelRefManager->GetMaterialListDesc(&pMaterialListDesc,dwRefIndex))
		goto lb_return;

	for (i=1; i<=pMaterialListDesc->dwMaxMaterialNum; i++)
	{
		pMtlLib = GetMaterialLib(dwRefIndex,i);
		if (pMtlLib)
		{
			pMtlLib->Unload();
		}
	}
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CoModel::CreateMotionList(DWORD dwNum,DWORD dwRefIndex)
{
	BOOL	bResult = FALSE;

	MOTION_LIST_DESC*	pMotionListDesc;
	if (!m_pModelRefManager->GetMotionListDesc(&pMotionListDesc,dwRefIndex))
		goto lb_return;


	if (pMotionListDesc->ppMotionList)
		goto lb_return;


	pMotionListDesc->ppMotionList = new CMotion*[dwNum+1];
	memset(pMotionListDesc->ppMotionList,0,sizeof(CMotion*)*(dwNum+1));
	pMotionListDesc->dwMaxMotionNum = dwNum;
	
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CoModel::CreateMaterialList(DWORD dwNum,DWORD dwRefIndex)
{
	BOOL	bResult = FALSE;

	MATERIAL_LIST_DESC*	pMaterialListDesc;
	if (!m_pModelRefManager->GetMaterialListDesc(&pMaterialListDesc,dwRefIndex))
		goto lb_return;


	if (pMaterialListDesc->ppMtlLibList)
		goto lb_return;


	pMaterialListDesc->ppMtlLibList = new CMaterialLib*[dwNum+1];
	memset(pMaterialListDesc->ppMtlLibList,0,sizeof(CMaterialLib*)*(dwNum+1));
	
	pMaterialListDesc->dwMaxMaterialNum = dwNum;
	
	bResult = TRUE;

lb_return:
	return bResult;
}
DWORD __stdcall CoModel::AddMaterial(char* szFileName,DWORD dwRefIndex,DWORD dwMtlIndex)
{
	DWORD				dwResult = 0xffffffff;
	CMaterialLib*		pMtlLib;

	if (!m_pModelRefManager->IsValidRefIndex(dwRefIndex,m_dwColMeshSize))
		goto lb_return;

	pMtlLib = m_pGeometry->LoadMaterial(szFileName);
	
	if (!pMtlLib)
		goto lb_return;

	dwResult = AddMaterial(pMtlLib,dwRefIndex,dwMtlIndex);
	if (dwResult == 0xffffffff)
		pMtlLib->Release();

lb_return:

	return dwResult;
}

DWORD CoModel::AddMaterial(CMaterialLib* pMtlLib,DWORD dwRefIndex,DWORD dwMtlIndex)
{
	DWORD				i;
	DWORD				dwResult = 0xffffffff;

	MATERIAL_LIST_DESC*	pMaterialListDesc;
	if (!m_pModelRefManager->GetMaterialListDesc(&pMaterialListDesc,dwRefIndex))
		goto lb_return;

	if (dwMtlIndex > pMaterialListDesc->dwMaxMaterialNum)
		goto lb_return;

	if (dwMtlIndex > 0)
	{
		// 매터리얼 인덱스를 지정한 경우..바꾼다.
		DeleteMaterial(dwRefIndex,dwMtlIndex);
		dwResult = dwMtlIndex;
		goto lb_set;
	}

	if (pMaterialListDesc->dwMaterialNum > pMaterialListDesc->dwMaxMaterialNum)
		goto lb_return;

	for (i=1; i<=pMaterialListDesc->dwMaxMaterialNum; i++)
	{
		if (!pMaterialListDesc->ppMtlLibList[i])
		{
			dwResult = i;
			goto lb_set;
		}
	}
	goto lb_return;

lb_set:
	pMaterialListDesc->ppMtlLibList[dwResult] = pMtlLib;
	pMaterialListDesc->dwMaterialNum++;

lb_return:
	return dwResult;
}
CMaterialLib* CoModel::GetMaterialLib(DWORD dwRefIndex,DWORD dwMtlIndex)
{
	CMaterialLib*	pMtlLib = NULL;
	MATERIAL_LIST_DESC*	pMaterialListDesc;
	if (!m_pModelRefManager->GetMaterialListDesc(&pMaterialListDesc,dwRefIndex))
		goto lb_return;

	if (!pMaterialListDesc->dwMaterialNum)
		goto lb_return;
	
	if (dwMtlIndex > pMaterialListDesc->dwMaxMaterialNum)
		goto lb_return;

	pMtlLib = pMaterialListDesc->ppMtlLibList[dwMtlIndex];

lb_return:
	return pMtlLib;
}

void __stdcall CoModel::DeleteMaterial(DWORD dwRefIndex,DWORD dwMtlIndex)
{
	CMaterialLib*	pMtlLib;
	MATERIAL_LIST_DESC*	pMaterialListDesc;

	if (!m_pModelRefManager->GetMaterialListDesc(&pMaterialListDesc,dwRefIndex))
		goto lb_return;

	if (!pMaterialListDesc->dwMaterialNum)
		goto lb_return;
	
	if (dwMtlIndex > pMaterialListDesc->dwMaxMaterialNum)
		goto lb_return;

	pMtlLib = pMaterialListDesc->ppMtlLibList[dwMtlIndex];

	if (!pMtlLib)
		goto lb_return;
	
	pMtlLib->Release();
	pMaterialListDesc->ppMtlLibList[dwMtlIndex] = 0;
	pMaterialListDesc->dwMaterialNum--;
	
lb_return:
	return;
}

void __stdcall CoModel::DeleteAllMaterialPerReference(DWORD dwRefIndex)
{
	DWORD		i;
	MATERIAL_LIST_DESC*	pMaterialListDesc;
	if (!m_pModelRefManager->GetMaterialListDesc(&pMaterialListDesc,dwRefIndex))
		goto lb_return;

		
	for (i=1; i<=pMaterialListDesc->dwMaxMaterialNum; i++)
		DeleteMaterial(dwRefIndex,i);
	
		
	if (pMaterialListDesc->ppMtlLibList)
	{
		delete [] pMaterialListDesc->ppMtlLibList;
		pMaterialListDesc->ppMtlLibList = NULL;
		pMaterialListDesc->dwMaterialNum = 0;
		pMaterialListDesc->dwMaxMaterialNum = 0;
	}
	
lb_return:
	return;
}
void __stdcall CoModel::DeleteAllMaterial()
{
	DWORD	dwMaxIndexNum = m_pModelRefManager->GetRefNum();
	if (!dwMaxIndexNum)
		return;

	DWORD*	pdwRefIndexList = new DWORD[dwMaxIndexNum];
	DWORD dwIndexNum = m_pModelRefManager->GetRefIndexWithModelPtr(pdwRefIndexList,this,dwMaxIndexNum);
	for (DWORD i=0; i<dwIndexNum; i++)
	{
		DeleteAllMaterialPerReference(pdwRefIndexList[i]);
	}
	delete [] pdwRefIndexList;

}
void CoModel::Optimize()
{
	if (!m_bOptimizedFlag)
	{
		for (DWORD i=0; i<m_dwMeshObjectNum; i++)
		{
			m_pMeshObjectList[i].Optimize();
		}
		m_bOptimizedFlag = TRUE;
	}
	
}
BOOL CoModel::Initialize(I4DyuchiGXRenderer* pDev,DWORD dwFlag)
{
	BOOL			bResult = TRUE;

	m_pRenderer = pDev;
	
	IDIMeshObject*		pMeshObj = NULL;
	COLLISION_MESH_OBJECT_DESC*		pColMeshDesc;

	CMeshObject*		pViewAlignedObjects[MAX_OBJECTS_NUM_PER_MODEL];
	
	if (m_bInitFlag)
		goto lb_return;

	PrepareAnimation();
/*
	MATERIAL_TABLE*		pMtlTable;
	DWORD	dwMtlNum;
	dwMtlNum = m_MaterialList.CreateMaterialTable(&pMtlTable);
	if (dwMtlNum)
	{
		m_dwDefaultMtlSetIndex = m_pRenderer->CreateMaterialSet(pMtlTable,dwMtlNum);
		m_MaterialList.ReleaseMaterialTable(pMtlTable);
	}
*/	


	DWORD	i;

	for (i=0; i<m_dwMeshObjectNum; i++)
	{
		if (m_pMeshObjectList[i].GetObjectType() != OBJECT_TYPE_MESH)
			continue;

		

		if (!m_pRenderer)
			continue;

		if (m_pMeshObjectList[i].GetFaceGroupNum())
		{
			pMeshObj = m_pRenderer->CreateMeshObject(m_pMeshObjectList[i].GetMeshFlag());
			if (m_pMeshObjectList[i].GetLightTexture())
			{
				m_pMeshObjectList[i].GetLightTexture()->CreateLightMapImage(NULL,0);
			}
		}
		m_pMeshObjectList[i].Initialize(&m_MaterialList,this,m_pGeometry);
		m_pMeshObjectList[i].CommitDevice(pMeshObj,&m_MaterialList);

		if (m_pMeshObjectList[i].GetMeshFlag().GetTransformType() == TRANSFORM_TYPE_ALIGN_VIEW)
		{
			pViewAlignedObjects[m_dwViewAlignedObjectsNum] = m_pMeshObjectList + i;
			m_dwViewAlignedObjectsNum++;
		}
		
	}
	for (i=0; i<m_dwCameraObjectNum; i++)
	{
		m_pCameraObjectList[i].Initialize(this);

	}
	for (i=0; i<m_dwBoneObjectNum; i++)
	{
		pColMeshDesc = m_pBoneObjectList[i].GetColMeshObjectDesc();
		pColMeshDesc->dwObjIndex = GetObjectIndex(m_pBoneObjectList[i].GetObjectName());
		m_pBoneObjectList[i].Initialize(this);
		

	}

	m_ppViewAlignedObjectsList = new CMeshObject*[m_dwViewAlignedObjectsNum];
	memcpy(m_ppViewAlignedObjectsList,pViewAlignedObjects,sizeof(CMeshObject*)*m_dwViewAlignedObjectsNum);

	
	bResult = TRUE;
	m_bInitFlag = TRUE;

	CreateCollisionDesc();
	// 레퍼런스 인덱스 0은 디폴트 콜루젼 메쉬 디스크립터
//	Render(STATIC_MAPOBJECT_REF_INDEX,NULL,NULL,0,1,NULL,0,0x000000ff,RENDER_TYPE_NOT_DRAW);

	if (!(dwFlag & MODEL_INITIALIZE_FLAG_NOT_OPTIMIZE))
		Optimize();
	
	// lod 서브 모델들을 초기화.
	for (i=1; i<=m_dwSubModelNum; i++)
	{
		m_pSubModel[i]->Initialize(pDev,0);
	}
	
	
lb_return:
	return bResult;
}
DWORD __stdcall	CoModel::Reference()
{
	DWORD dwRefIndex = m_pModelRefManager->AllocRefMemory(m_dwColMeshSize,m_dwObjectNum,this);

	if (dwRefIndex == 0xffffffff)
		goto lb_return;

	COLLISION_MODEL_DESC*	pColModelDesc;
	m_pModelRefManager->GetColModelDesc(&pColModelDesc,dwRefIndex);
	memcpy(pColModelDesc,m_pColMeshModelDescLocal,m_dwColMeshSize);

lb_return:
	return dwRefIndex;
}

BOOL __stdcall CoModel::InitializeIllusionEffect(DWORD dwRefIndex,DWORD dwMaxIllusionFrameNum,DWORD dwObjIndex,void* pMtlHandle,DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	CMeshObject* pObj = (CMeshObject*)SelectObject(dwObjIndex);
	
	if (!pObj)
		goto lb_return;

	if (pObj->GetObjectType() != OBJECT_TYPE_MESH)
		goto lb_return;

	if (pObj->GetMeshFlag().GetTransformType() != TRANSFORM_TYPE_ILLUSION)
		goto lb_return;

	bResult = m_pModelRefManager->InitializeIllusionEffect(dwRefIndex,dwMaxIllusionFrameNum,pObj,pMtlHandle,dwFlag);

lb_return:
	return bResult;

}
BOOL __stdcall CoModel::SetIllusionEffectMaterial(DWORD dwRefIndex,void* pMtlHandle)
{
	BOOL	bResult = FALSE;
	ILLUSION_DESC*	pIllusionDesc = m_pModelRefManager->GetModelIllusionDesc(dwRefIndex);
	if (!pIllusionDesc)
		goto lb_return;

	if (pIllusionDesc->pMtlHandleAllocated)
	{
		m_pRenderer->DeleteMaterial(pIllusionDesc->pMtlHandleAllocated);
		pIllusionDesc->pMtlHandleAllocated = NULL;
	}
	pIllusionDesc->pMtlHandle = pMtlHandle;
	bResult = TRUE;
lb_return:
	return bResult;


}


void __stdcall CoModel::UnReference(DWORD dwRefIndex)
{
	
	DeleteAllMotionPerReference(dwRefIndex);
	DeleteAllMaterialPerReference(dwRefIndex);
	
	m_pModelRefManager->FreeRefMemory(dwRefIndex);
}
BOOL CoModel::GetModelStatus(MODEL_STATUS** ppModelStatus,DWORD dwRefIndex)
{
	return m_pModelRefManager->GetModelStatus(ppModelStatus,dwRefIndex);
}
BOOL CoModel::GetModelSetFrameArgs(SET_FRAME_ARGS** ppArgs,DWORD dwRefIndex)
{
	return m_pModelRefManager->GetModelSetFrameArgs(ppArgs,dwRefIndex);

}

BOOL CoModel::GetMatrixEntry(MATRIX4** ppMatrixEntry,MATRIX4** ppMatrixEntryForPhysiqueLocal,DWORD dwRefIndex) 
{
	return m_pModelRefManager->GetMatrixEntry(ppMatrixEntry,ppMatrixEntryForPhysiqueLocal,dwRefIndex);
}
BOOL CoModel::GetCameraMatrix(MATRIX4* pMatView,DWORD dwRefIndex)
{
	BOOL	bResult = FALSE;
	
	
	if (!m_dwCameraObjectNum)
		goto lb_return;

	m_pCameraObjectList[0].GetCameraMatrix(pMatView,dwRefIndex);

	bResult = TRUE;

lb_return:
	return bResult;

}
DWORD __stdcall CoModel::GetCameraList(CAMERA_DESC* pDesc,DWORD dwNum)
{
	
	if (dwNum > m_dwCameraObjectNum)
		dwNum = m_dwCameraObjectNum;

	for (DWORD i=0; i<dwNum; i++)
	{
		pDesc[i].v3From.x = m_pCameraObjectList[i].GetWorldMatrix()->_41;
		pDesc[i].v3From.y = m_pCameraObjectList[i].GetWorldMatrix()->_42;
		pDesc[i].v3From.z = m_pCameraObjectList[i].GetWorldMatrix()->_43;

		pDesc[i].v3To.x = m_pCameraObjectList[i].GetTargetObject()->GetWorldMatrix()->_41;
		pDesc[i].v3To.y = m_pCameraObjectList[i].GetTargetObject()->GetWorldMatrix()->_42;
		pDesc[i].v3To.z = m_pCameraObjectList[i].GetTargetObject()->GetWorldMatrix()->_43;
	}
	return dwNum;

}
DWORD __stdcall	CoModel::GetLightList(LIGHT_DESC* pDesc,DWORD dwNum)
{
	if (dwNum > m_dwLightObjectNum)
		dwNum = m_dwLightObjectNum;

	for (DWORD i=0; i<dwNum; i++)
	{
		pDesc[i].v3Point.x = m_pLightObjectList[i].GetWorldMatrix()->_41;
		pDesc[i].v3Point.y = m_pLightObjectList[i].GetWorldMatrix()->_42;
		pDesc[i].v3Point.z = m_pLightObjectList[i].GetWorldMatrix()->_43;
		pDesc[i].fRs = m_pLightObjectList[i].GetAttentEnd();
		pDesc[i].dwDiffuse = m_pLightObjectList[i].GetColor();
		pDesc[i].dwSpecular = 0.0f;
		pDesc[i].dwAmbient = 0.0f;
		
		
	}
	return dwNum;

}
BOOL CoModel::Duplicate(CMeshObject* pMeshObjectDest,DWORD dwSeqIndex,DWORD dwRefIndex)
{
	BOOL	bResult = FALSE;
	MATRIX4*		pResult = NULL;
	CMeshObject* pMeshObjSrc = (CMeshObject*)m_pMeshObjectList+dwSeqIndex;
	COLLISION_MODEL_DESC*		pColModelDesc;
	COLLISION_MESH_OBJECT_DESC*	pColMeshDesc;
	
	if (pMeshObjSrc->GetObjectType() != OBJECT_TYPE_MESH)
		goto lb_return;


	if (dwRefIndex)
	{
		MATRIX4*	pMatrixEntry;
		MATRIX4*	pMatrixEntryForPhysiqueLocal;
		m_pModelRefManager->GetMatrixEntry(&pMatrixEntry,&pMatrixEntryForPhysiqueLocal,dwRefIndex);
		pResult = pMatrixEntry + pMeshObjSrc->GetIndex();
	}

	if (!m_pModelRefManager->GetColModelDesc(&pColModelDesc,dwRefIndex))
		goto lb_return;

	pColMeshDesc = pMeshObjSrc->GetColMeshObject(pColModelDesc);

	bResult = pMeshObjSrc->Duplicate(pMeshObjectDest,pResult,pColMeshDesc);
	
	bResult = TRUE;
	
lb_return:
	return bResult;
}

COLLISION_MODEL_DESC*  CoModel::CreateCollisionDesc()
{

	DWORD				i;
	DWORD				dwBoundingVertexNum;
	DWORD				size;
	VECTOR3				*pv3,*pv3Offset;	

	if (!m_dwMeshObjectNum)
		goto lb_return;

	m_dwColMeshSize = size = sizeof(COLLISION_MESH_OBJECT_DESC) +
		sizeof(COLLISION_MESH_OBJECT_DESC)*m_dwMeshObjectNum + 
		sizeof(DWORD) +
		m_dwBoneObjectNum*sizeof(COLLISION_MESH_OBJECT_DESC) +	// 본오브젝트용 
		sizeof(MATRIX4)*m_dwMeshObjectNum;						// 결과 매트릭스 
		

	m_dwColMeshDescOffsetBoneEntry = size - m_dwBoneObjectNum*sizeof(COLLISION_MESH_OBJECT_DESC);

	m_pColMeshModelDescWorld = (COLLISION_MODEL_DESC*)new char[size];
	m_pColMeshModelDescLocal = (COLLISION_MODEL_DESC*)new char[size];
	
	m_pColMeshModelDescWorld->dwColMeshObjectDescNum = m_dwMeshObjectNum;
	m_pColMeshModelDescLocal->dwColMeshObjectDescNum = m_dwMeshObjectNum;

	dwBoundingVertexNum = m_dwMeshObjectNum*8;
	pv3 = pv3Offset = new VECTOR3[dwBoundingVertexNum];

	for (i=0; i<m_dwMeshObjectNum; i++)
	{
		m_pMeshObjectList[i].CreateBoundingMesh(
			m_pColMeshModelDescLocal,
			m_pColMeshModelDescLocal->colMeshObjectDesc+i,
			m_pColMeshModelDescWorld->colMeshObjectDesc+i);

//		lstrcpy(m_pColMeshModelDescLocal->colMeshObjectDesc[i].szObjName,m_pMeshObjectList[i].GetObjectName());
		m_pColMeshModelDescLocal->colMeshObjectDesc[i].dwObjIndex = m_pMeshObjectList[i].GetIndex();


//		lstrcpy(m_pColMeshModelDescWorld->colMeshObjectDesc[i].szObjName,m_pMeshObjectList[i].GetObjectName());
		m_pColMeshModelDescWorld->colMeshObjectDesc[i].dwObjIndex = m_pMeshObjectList[i].GetIndex();

		memcpy(pv3Offset,(m_pColMeshModelDescWorld->colMeshObjectDesc+i)->boundingBox.v3Oct,sizeof(VECTOR3)*8);
		pv3Offset += 8;
	}
	
	COLLISION_MESH_OBJECT_DESC*	pBoneMeshObjDesc;
	pBoneMeshObjDesc = (COLLISION_MESH_OBJECT_DESC*)( (char*)m_pColMeshModelDescLocal + m_dwColMeshDescOffsetBoneEntry );
	for (i=0; i<m_dwBoneObjectNum; i++)
	{
		memcpy(pBoneMeshObjDesc,m_pBoneObjectList[i].GetColMeshObjectDesc(),sizeof(COLLISION_MESH_OBJECT_DESC));
		pBoneMeshObjDesc++;
		
	}


	// 모델 전체의 충돌매쉬 계산
	// 로컬이든 월드든 상관없이 모델의 충돌메쉬는 월드좌표게를 따른다.
	::CalcBoundingMesh(pv3,dwBoundingVertexNum,&m_pColMeshModelDescWorld->colMeshModel);
	memcpy(&m_pColMeshModelDescLocal->colMeshModel,&m_pColMeshModelDescWorld->colMeshModel,sizeof(COLLISION_MESH_OBJECT_DESC));
	
	if (pv3)
		delete [] pv3;
lb_return:
	return m_pColMeshModelDescLocal;
}

COLLISION_MESH_OBJECT_DESC* CoModel::GetColMeshObjectLocal(DWORD dwOffset)
{
	COLLISION_MESH_OBJECT_DESC* pDesc = 
	(COLLISION_MESH_OBJECT_DESC*)( (DWORD)m_pColMeshModelDescLocal + (DWORD)dwOffset);
	return pDesc;
}	


MATRIX4* __stdcall CoModel::GetObjectResultMatrix(DWORD dwRefIndex,DWORD dwObjIndex)
{
	MATRIX4*	pMatrixResult = NULL;
	MATRIX4*	pMatrixEntry;
	MATRIX4*	pMatrixEntryForPhysiqueLocal;

	if (!m_pModelRefManager->GetMatrixEntry(&pMatrixEntry,&pMatrixEntryForPhysiqueLocal,dwRefIndex))
		goto lb_return;

	pMatrixResult = pMatrixEntry+dwObjIndex;

lb_return:
	return pMatrixResult;
}

BOOL __stdcall CoModel::Render(DWORD dwRefIndex,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwFrame,DWORD dwMotionIndex,DWORD dwAlpha,DWORD dwMtlSetIndex,DWORD dwEffectIndex,DWORD dwFlag)
{
	BOOL			bResult = FALSE;

	DWORD			i;
	
	
	LIGHT_INDEX_DESC*		pAppliedLightIndex = NULL;
	DWORD					dwAppliedLightNum = 0;
	
	LIGHT_INDEX_DESC*		pAppliedSpotLightIndex = NULL;
	DWORD					dwAppliedSpotLightNum = 0;

	COLLISION_MESH_OBJECT_DESC* pColMeshDesc = NULL;	
	COLLISION_MODEL_DESC*	pColModelDesc = NULL;	
	MATRIX4*				pMatrixEntry = NULL;
	MATRIX4*				pMatrixEntryForPhysiqueLocal = NULL;

	float					fDistance;
	VECTOR3*				pEye = m_pGeometry->GetCameraEye();



	MODEL_REF_DESC	modelRefDesc;
	if (!m_pModelRefManager->GetModelRefDesc(&modelRefDesc,dwRefIndex))
		goto lb_return;


	
	
	
	pColModelDesc = modelRefDesc.pColModelDesc;
	pMatrixEntry = modelRefDesc.pMatrixEntry;
	pMatrixEntryForPhysiqueLocal = modelRefDesc.pMatrixEntryForPhysiqueLocal;


//	dwMtlSetIndex = m_dwDefaultMtlSetIndex;

	if (!(dwFlag & RENDER_TYPE_AS_LOD_SUBMODEL))
	{
		// lod용 서브 모델이 아닌 경우만 트랜스폼을 한다.
		
		SetFrame(modelRefDesc.pModelStatus,pMatrixEntry,pMatrixEntryForPhysiqueLocal,pColModelDesc,pParentMat,pTransformMat,dwRefIndex,dwFrame,dwMotionIndex,RENDER_TYPE_UPDATE_COLLISION_BONEMESH_DESC);
		
		if (m_dwSubModelNum)	// LOD를 할 경우
		{
		
			// 카메라와의 거리를 설정..
			MODEL_STATUS*	pModelStatus;
			m_pGeometry->GetModelRefManager()->GetModelStatus(&pModelStatus,dwRefIndex);
			float	fDistanceFromView;
			
			fDistanceFromView =	CalcDistance(&pColModelDesc->colMeshModel.boundingSphere.v3Point,m_pGeometry->GetCameraEye());

			if (fDistanceFromView == pModelStatus->fDistanceFromView)
				goto lb_render_submodel;
			

			DWORD	i;
			DWORD	dwLodLevel;

			//trustpak 2005/06/24

			if (MODEL_LOD_USING_MODE_SET_LEVEL == m_pGeometry->GetModelLODUsingMode())
			{
				DWORD dwModelLODUsingLevel = m_pGeometry->GetModelLodUsingLevel();

				if (dwModelLODUsingLevel >= m_dwSubModelNum)
				{
					dwLodLevel = m_dwSubModelNum;
				}
				else
				{
					dwLodLevel = dwModelLODUsingLevel;				
				}
			}
			///
			else
			{
				dwLodLevel = m_dwSubModelNum;
				
				for (i=0; i<m_dwSubModelNum+1; i++)
				{
					if (fDistanceFromView < DEFAULT_MODEL_LOD_UNIT*(float)(i+1) )
					{
						dwLodLevel = i;
						break;
					}
				}
				//trustpak 2005/06/24
				//pModelStatus->dwLODLevel = dwLodLevel;
				//pModelStatus->fDistanceFromView = fDistanceFromView;
				//
			}

			//trustpak 2005/06/24
			pModelStatus->dwLODLevel = dwLodLevel;
			pModelStatus->fDistanceFromView = fDistanceFromView;
			///
			
lb_render_submodel:	
			CoModel*	pModel;
			pModel = m_pSubModel[pModelStatus->dwLODLevel];

#ifdef _TRUSTPAK_TEMPORARY_WORK_		

			static int s_iCount = 0;
			++s_iCount;

			if (0 == (s_iCount % 30))
			{
				char szDebugString[0x400];
				memset(szDebugString, 0, 0x400);

				sprintf(szDebugString, " --> Model Name : %s, LOD Level : %d\n",  m_szFileName, pModelStatus->dwLODLevel);
				OutputDebugString(szDebugString);
			}			

#endif

			return pModel->Render(dwRefIndex,pParentMat,pTransformMat,dwFrame,dwMotionIndex,dwAlpha,dwMtlSetIndex,dwEffectIndex,dwFlag | RENDER_TYPE_AS_LOD_SUBMODEL);
		}
	}

	DWORD				dwDynamicLightNum = m_pGeometry->GetDynamicRTLightNum();
	DWORD				dwStaticLightNum = m_pGeometry->GetStaticRTLightNum();

	dwAppliedLightNum = dwDynamicLightNum + dwStaticLightNum;
	if (dwAppliedLightNum)
	{
		pAppliedLightIndex = m_pGeometry->AllocLightIndex(dwAppliedLightNum);
		if (!pAppliedLightIndex)
		{
			dwAppliedLightNum = 0;
			dwDynamicLightNum = 0;
			dwStaticLightNum = 0;

		}
		else
		{
            dwDynamicLightNum = ClipLightWithRS(pAppliedLightIndex,&pColModelDesc->colMeshModel,m_pGeometry->GetDynamicRTLightDescBuffer(),dwDynamicLightNum,0);
            dwStaticLightNum = ClipLightWithRS(pAppliedLightIndex+dwDynamicLightNum,&pColModelDesc->colMeshModel,m_pGeometry->GetStaticRTLightDescBuffer(),dwStaticLightNum,STATIC_RTLIGHT_START_INDEX);
		}
	}

	// 라이트 범위가 오브젝트에 닿지 않는 경우는 클립
	
	
	dwAppliedLightNum = dwDynamicLightNum + dwStaticLightNum;
	
	if (m_pGeometry->GetSpotLightNum())
	{
		pAppliedSpotLightIndex = m_pGeometry->AllocLightIndex(m_pGeometry->GetSpotLightNum());
		if (!pAppliedSpotLightIndex)
		{
			dwAppliedSpotLightNum = 0;
			goto lb_render_mesh;
		}

		for (DWORD i=0; i<m_pGeometry->GetSpotLightNum(); i++)
		{
			if (ClipRenderObjectWithViewVolume(m_pGeometry->GetSpotLightViewVolume(i),&pColModelDesc->colMeshModel,0))
			{
				// 렌더러측 쉐도우 라이트의 송신자의 위치를 세팅.
		//		m_pRenderer->SetShadowLightSenderPosition(&pColModelDesc->colMeshModel.boundingSphere.v3Point,i);
				
				pAppliedSpotLightIndex[dwAppliedSpotLightNum].bLightIndex = (BYTE)i;
				pAppliedSpotLightIndex[dwAppliedSpotLightNum].pMtlHandle = m_pGeometry->GetSpotLightDesc(i)->pMtlHandle;
				pAppliedSpotLightIndex[dwAppliedSpotLightNum].bTexOP = m_pGeometry->GetSpotLightDesc(i)->dwProjTexIndex & 0x000000ff;

							
				dwAppliedSpotLightNum++;
			}
		}
	}

lb_render_mesh:
	for (i=0; i<m_dwMeshObjectNum; i++)
	{
		if (!m_pMeshObjectList[i].GetDeviceObject())
			continue;

		pColMeshDesc = m_pMeshObjectList[i].GetColMeshObject(pColModelDesc);
		fDistance = 
			DotProduct(&m_pGeometry->INL_GetCameraEyePlane()->v3Up,&pColMeshDesc->boundingSphere.v3Point) 
			+ m_pGeometry->INL_GetCameraEyePlane()->D;

		fDistance += modelRefDesc.pModelStatus->fDistanceFactor;		//모델간의 zorder를 두기 위한 팩터.

		fDistance += ( (float)( m_pMeshObjectList[i].GetMeshFlag().GetRenderZPriorityValue() * RENDER_ZPRIORITY_UNIT ) );
		if (RENDER_TYPE_ENABLE_CLIP_PER_OBJECT & dwFlag)
		{
			if (!ClipRenderObjectWithViewVolume(m_pGeometry->INL_GetViewvolume(),pColMeshDesc,0))
				continue;
		}


		DWORD	dwMtlHandle;
		dwMtlHandle = 0;
		if (!modelRefDesc.pMaterialListDesc->ppMtlLibList)
			goto lb_idi_render;

		if (dwMtlSetIndex > modelRefDesc.pMaterialListDesc->dwMaxMaterialNum)
			goto lb_idi_render;
		
		if (!modelRefDesc.pMaterialListDesc->ppMtlLibList[dwMtlSetIndex])
			goto lb_idi_render;

		dwMtlHandle = modelRefDesc.pMaterialListDesc->ppMtlLibList[dwMtlSetIndex]->GetMaterialHandle();

lb_idi_render:
		m_pRenderer->RenderMeshObject(m_pMeshObjectList[i].GetDeviceObject(),
			dwRefIndex,
			fDistance,				// 카메라 eye로부터의 거리 
			dwAlpha,
			pAppliedLightIndex,dwAppliedLightNum,			// 포인트 라이트 포인터와 갯수 
			pAppliedSpotLightIndex,dwAppliedSpotLightNum,	// 스폿라이트 인덱스 포인터와 갯수.
			dwMtlHandle,									// 렌더러로부터 받은 매터리얼 핸들 
			dwEffectIndex,
			dwFlag);
		
	}
	if (modelRefDesc.pIllusionDesc)
	{
		if (modelRefDesc.pIllusionDesc->bEnable)
		{
			if (modelRefDesc.pIllusionDesc->dwFacesNum)
			{
				m_pRenderer->EnableRenderTriBuffer(
					modelRefDesc.pIllusionDesc->pTriBufferHandle,
					modelRefDesc.pIllusionDesc->pMtlHandle,
					modelRefDesc.pIllusionDesc->dwFacesNum);
			}

		}
	}

	if (m_pGeometry->INL_GetDrawDebugFlag())
	{
		if (m_pGeometry->INL_GetDrawDebugFlag() & DEBUG_DRAW_MODEL_COL_MESH)
		{
			for (i=0; i<m_dwMeshObjectNum; i++)
			{
				pColMeshDesc = m_pMeshObjectList[i].GetColMeshObject(pColModelDesc);
				m_pRenderer->RenderBox(pColMeshDesc->boundingBox.v3Oct,0xff00ff00);
	//			m_pGeometry->RenderSphere(&pColMeshDesc->boundingSphere.v3Point,pColMeshDesc->boundingSphere.fRs,0xffffffff);
			}
		}
		if (m_pGeometry->INL_GetDrawDebugFlag() & DEBUG_DRAW_BONE_COL_MESH)
		{
			pColMeshDesc = (COLLISION_MESH_OBJECT_DESC*)( (char*)pColModelDesc + m_dwColMeshDescOffsetBoneEntry );
			for (i=0; i<m_dwBoneObjectNum; i++)
			{			
				m_pRenderer->RenderBox(pColMeshDesc->boundingBox.v3Oct,0xff00ff00);
				pColMeshDesc++;
			}
		}
	}

	bResult = TRUE;
lb_return:

	return bResult;
}


BOOL CoModel::RenderShadow(DWORD dwRefIndex,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwFrame,DWORD dwMotionIndex,LIGHT_DESC* pLightDesc,VIEW_VOLUME* pShadowViewVolume,DWORD dwShadowNum,DWORD dwAlpha,DWORD dwFlag)
{
	BOOL			bResult = FALSE;

	DWORD			i;
		
	COLLISION_MODEL_DESC*	pColModelDesc;
	MATRIX4*				pMatrixEntry;
	MATRIX4*				pMatrixEntryForPhysiqueLocal;

	BYTE					bShadowIndex[MAX_SHADOW_LIGHT_NUM_PER_SCENE+MAX_PRJIMAGE_LIGHT_NUM_PER_SCENE];
	DWORD					dwAppliedShadowNum = 0;

	
	MODEL_REF_DESC	modelRefDesc;
	if (!m_pModelRefManager->GetModelRefDesc(&modelRefDesc,dwRefIndex))
		goto lb_return;




	pColModelDesc = modelRefDesc.pColModelDesc;
	pMatrixEntry = modelRefDesc.pMatrixEntry;
	pMatrixEntryForPhysiqueLocal = modelRefDesc.pMatrixEntryForPhysiqueLocal;		

	SetFrame(modelRefDesc.pModelStatus,pMatrixEntry,pMatrixEntryForPhysiqueLocal,pColModelDesc,pParentMat,pTransformMat,dwRefIndex,dwFrame,dwMotionIndex,0);

	
	BOUNDING_SPHERE	sphere;
	sphere.fRs = 10.0f;
			
	for (i=0; i<dwShadowNum; i++)
	{
	//	if (pLightDesc[i].pMtlHandle)
		if (pLightDesc[i].type == SPOT_LIGHT_TYPE_PRJIMAGE)
		{
			sphere.v3Point = pLightDesc[i].v3Point;
			m_pGeometry->SetShadowLightSenderSphere(&sphere,i);
			continue;
		}

		if (ClipRenderObjectWithViewVolume(pShadowViewVolume+i,&pColModelDesc->colMeshModel,0))
		{	
			bShadowIndex[dwAppliedShadowNum] = (BYTE)i;
			m_pGeometry->SetShadowLightSenderSphere(&pColModelDesc->colMeshModel.boundingSphere,i);
			dwAppliedShadowNum++;
		}
	}
	if (!dwAppliedShadowNum)
		goto lb_return;

	for (i=0; i<m_dwMeshObjectNum; i++)
	{
		if (m_pMeshObjectList[i].GetDeviceObject())
		{

			m_pMeshObjectList[i].GetDeviceObject()->RenderProjection(
				dwRefIndex,
				dwAlpha,
				bShadowIndex,dwAppliedShadowNum,
				dwFlag
				);
		}
	}
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CoModel::SetZOrder(int iOrder,DWORD dwRefIndex)
{
	BOOL			bResult = FALSE;
	MODEL_STATUS*	pModelStatus;
	
	if (!m_pModelRefManager->GetModelStatus(&pModelStatus,dwRefIndex))
		goto lb_return;

	pModelStatus->fDistanceFactor = (float)iOrder * DEFAULT_RENDER_ZORDER_UNIT;
	
	bResult = TRUE;
lb_return:
	return bResult;

}

BOOL __stdcall CoModel::SetAxisAlignOK(BOOL bSwitch,DWORD dwRefIndex)
{
	BOOL			bResult = FALSE;


	MODEL_REF_DESC	modelRefDesc;
	if (!m_pModelRefManager->GetModelRefDesc(&modelRefDesc,dwRefIndex))
		goto lb_return;

	if (!modelRefDesc.pModelStatus)
		goto lb_return;

	modelRefDesc.pModelStatus->bAxisAlignOK = bSwitch;
	
	SetFrame(modelRefDesc.pModelStatus,
		modelRefDesc.pMatrixEntry,
		modelRefDesc.pMatrixEntryForPhysiqueLocal,
		modelRefDesc.pColModelDesc,
		&modelRefDesc.pArgsSetFrame->matTransform[0],
		&modelRefDesc.pArgsSetFrame->matTransform[1],
		dwRefIndex,
		modelRefDesc.pArgsSetFrame->dwFrame,
		modelRefDesc.pArgsSetFrame->dwMotionIndex,
		RENDER_TYPE_UPDATE_ALWAYS);

//	SetFrame(MODEL_STATUS* pModelStatus,MATRIX4* pMatrixEntry,MATRIX4* pMatrixEntryForPhysiqueLocal,COLLISION_MODEL_DESC* pColModelDesc,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwRefIndex,DWORD dwFrame,DWORD dwMotionIndex,DWORD dwFlag);

	bResult = TRUE;
lb_return:
	return bResult;
}

BOOL CoModel::PrepareAnimation()
{
	
	DWORD				dwQCount = 0;
	DWORD				dwQEntry = 0;

	DWORD				dwParentObjectNum = 0;
	DWORD				dwParentObjectCount = 0;
	DWORD				dwChildObjectNum = 0;

	CBaseObject*		pParentObject[MAX_OBJECTS_NUM_PER_MODEL];
	CBaseObject*		pObjectQ[MAX_OBJECTS_NUM_PER_MODEL];
	CBaseObject*		pChild;
	CBaseObject*		pParent;

	DWORD				i;

	for (i=0; i<m_dwObjectNum; i++)
	{
		if (m_ppObjectList[i]->GetParentObjectIndex() == 0xffffffff)
		{
			pParentObject[dwParentObjectNum] = m_ppObjectList[i];
			dwParentObjectNum++;
		}
		else 
		{
			m_ppObjectList[i]->SetParentObject(SelectObject(m_ppObjectList[i]->GetParentObjectIndex()));
		}
	}
	m_pTransformSetList = new TRANSFORM_OBJECT_SET[dwParentObjectNum];
	memset(m_pTransformSetList,0,sizeof(TRANSFORM_OBJECT_SET)*dwParentObjectNum);
	m_dwTransformSetNum = dwParentObjectNum;

	for (i=0; i<dwParentObjectNum; i++)
	{	
		memset(pObjectQ,0,sizeof(CBaseObject*)*MAX_OBJECTS_NUM_PER_MODEL);

		dwQEntry = 0;
		dwQCount = 0;

		pObjectQ[dwQEntry] = pParentObject[i];
		dwQCount++;

lb_loop:
		pParent = pObjectQ[dwQEntry];
		
		
		if (!pParent)
		{
			m_pTransformSetList[i].dwObjectNum = dwQCount;
			m_pTransformSetList[i].ppObjectList = new CBaseObject*[dwQCount];
			memcpy(m_pTransformSetList[i].ppObjectList,pObjectQ,sizeof(CBaseObject*)*dwQCount);
			continue;
		}
		for (DWORD j=0; j<pParent->GetChildObjectNum(); j++)
		{
			pObjectQ[dwQCount] = pChild = SelectObject(pParent->GetChildObjectIndex(j));
			dwQCount++;
		}
		dwQEntry++;
		goto lb_loop;
	}

	return TRUE;
}
void __stdcall CoModel::UpdateColMeshDesc(DWORD dwRefIndex,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwFrame,DWORD dwMotionIndex,DWORD dwFlag)
{
	COLLISION_MODEL_DESC*	pColModelDesc;
	MATRIX4*				pMatrixEntry;
	MATRIX4*				pMatrixEntryForPhysiqueLocal;


	MODEL_REF_DESC	modelRefDesc;
	if (!m_pModelRefManager->GetModelRefDesc(&modelRefDesc,dwRefIndex))
		goto lb_return;
	
	pColModelDesc = modelRefDesc.pColModelDesc;
	pMatrixEntry = modelRefDesc.pMatrixEntry;
	pMatrixEntryForPhysiqueLocal = modelRefDesc.pMatrixEntryForPhysiqueLocal;		



	SetFrame(modelRefDesc.pModelStatus,pMatrixEntry,pMatrixEntryForPhysiqueLocal,pColModelDesc,pParentMat,pTransformMat,dwRefIndex,dwFrame,dwMotionIndex,dwFlag);
	

lb_return:
	return;
}



BOOL CoModel::SetFrame(MODEL_STATUS* pModelStatus,MATRIX4* pMatrixEntry,MATRIX4* pMatrixEntryForPhysiqueLocal,COLLISION_MODEL_DESC* pColModelDesc,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwRefIndex,DWORD dwFrame,DWORD dwMotionIndex,DWORD dwFlag)
{
	
	BOOL			bResult = FALSE;
	DWORD			i;
	CBaseObject*	pObj;

	MATRIX4			matTransform;
	COLLISION_MESH_OBJECT_DESC*	pColMeshDesc;
	MATRIX4*		pResult;

	dwFlag &= RENDER_TYPE_UPDATE_MASK;

	MOTION_LIST_DESC*	pMotionListDesc;
	if (!m_pModelRefManager->GetMotionListDesc(&pMotionListDesc,dwRefIndex))
		goto lb_return;


	DWORD	dwLastFrame;
	void*	pMotionObject;

	dwLastFrame = 0;
	pMotionObject = NULL;

	if (dwMotionIndex > pMotionListDesc->dwMotionNum)
		dwMotionIndex = pMotionListDesc->dwMotionNum;

	if (pMotionListDesc->dwMotionNum)
	{
		pMotionObject = pMotionListDesc->ppMotionList[dwMotionIndex];
		if (pMotionObject)
			dwLastFrame = pMotionListDesc->ppMotionList[dwMotionIndex]->GetLastFrame();
	}

	if (dwFrame >= dwLastFrame)
		dwFrame = dwLastFrame;

	pModelStatus->dwFrame = dwFrame;
	pModelStatus->dwMotionIndex = dwMotionIndex;
	pModelStatus->pMotionObject = pMotionObject;

	SET_FRAME_ARGS*	pPrvArgs;
	m_pModelRefManager->GetModelSetFrameArgs(&pPrvArgs,dwRefIndex);

	SET_FRAME_ARGS	args;
	

	args.pMatrixEntry = pMatrixEntry;
	args.pColModelDesc = pColModelDesc;
	args.pParentMat = pParentMat;
	args.dwFrame = dwFrame;
	args.dwMotionIndex = dwMotionIndex;
	args.dwFlag = dwFlag;
	args.matBillboard = *m_pGeometry->GetBillBoardMatrix();


	memset(args.matTransform,0,sizeof(MATRIX4)*2);

	if (pParentMat)
		args.matTransform[0] = *pParentMat;
		
	if (pTransformMat)
		args.matTransform[1] = *pTransformMat;

	SET_FRAME_ARGS*	pArgDummy;
	pArgDummy = NULL;

	if (dwFlag & RENDER_TYPE_UPDATE_ALWAYS)
		goto lb_start;

//	if (memcmp(&args,pPrvArgs,sizeof(SET_FRAME_ARGS)-sizeof(DWORD)))
	if (memcmp(&args,pPrvArgs,(DWORD)&pArgDummy->matWorldForPhysique - (DWORD)&pArgDummy->pMatrixEntry))
		goto lb_start;

	
	goto lb_true;

lb_start:


	if (!pTransformMat)
	{
		SetIdentityMatrix(&matTransform);
		pTransformMat = &matTransform;
	}
	if (pParentMat)
	{
		MatrixMultiply2(&matTransform,pTransformMat,pParentMat);
		pTransformMat = &matTransform;
	}
//	pTransformMat = &matTransform;

	TransformVector3_VPTR2(
		&pColModelDesc->colMeshModel.boundingSphere.v3Point,
		&m_pColMeshModelDescLocal->colMeshModel.boundingSphere.v3Point,
		pTransformMat,1);

	TransformVector3_VPTR2(
		pColModelDesc->colMeshModel.boundingBox.v3Oct,
		m_pColMeshModelDescLocal->colMeshModel.boundingBox.v3Oct,
		pTransformMat,8);
		
	args.matWorldForPhysique = *pTransformMat;
//		PrintfDebugString("%f\n",args.matWorldForPhysique._11);

	*pPrvArgs = args;

    for (i=0; i<m_dwTransformSetNum; i++)
	{

		pResult = pMatrixEntry + m_pTransformSetList[i].ppObjectList[0]->GetIndex();
		m_pTransformSetList[i].ppObjectList[0]->SetFrame(pResult,pMatrixEntry,dwRefIndex,dwFrame,dwMotionIndex);

		if (pModelStatus->bAxisAlignOK)
			goto lb_init_transform;
		//if ((RENDER_TYPE_UPDATE_ENABLE_SCALE_ATTACHED_MODEL & dwFlag))
		

		if (pParentMat)
		{
			*pResult = *pParentMat;
			goto lb_transform;
		}

lb_init_transform:
		MatrixMultiply2(pResult,pResult,pTransformMat);
		/*

#ifdef _AXIS_ALIGN_OK
		MatrixMultiply2(pResult,pResult,pTransformMat);
#else
		if (pParentMat)
		{
			*pResult = *pParentMat;
		//	MatrixMultiply2(pResult,pResult,pTransformMat);
		}
		else 
		{
			MatrixMultiply2(pResult,pResult,pTransformMat);

		}
#endif*/
lb_transform:

		// pResult 는 해당 매트릭스..여기선 일단 최상위 부모의 매트릭스 
		// 모델의 포지션,회전등을 처리.결과 매트릭스에 곱한다.
		for (DWORD j=1; j<m_pTransformSetList[i].dwObjectNum; j++)
		{
			pResult = pMatrixEntry + m_pTransformSetList[i].ppObjectList[j]->GetIndex();
			m_pTransformSetList[i].ppObjectList[j]->SetFrame(pResult,pMatrixEntry,dwRefIndex,dwFrame,dwMotionIndex);
		}
	}



	for (i=0; i<m_dwViewAlignedObjectsNum; i++)
	{
		m_ppViewAlignedObjectsList[i]->SetAlignedViewMatrix(pMatrixEntry);

	}

	for (i=0; i<m_pColMeshModelDescLocal->dwColMeshObjectDescNum; i++)
	{
		pObj = SelectObject(pColModelDesc->colMeshObjectDesc[i].dwObjIndex);
		pResult = pMatrixEntry + pObj->GetIndex();

		TransformVector3_VPTR2(
			&pColModelDesc->colMeshObjectDesc[i].boundingSphere.v3Point,
			&m_pColMeshModelDescLocal->colMeshObjectDesc[i].boundingSphere.v3Point,
			pResult,1);

		TransformVector3_VPTR2(
			pColModelDesc->colMeshObjectDesc[i].boundingBox.v3Oct,
			m_pColMeshModelDescLocal->colMeshObjectDesc[i].boundingBox.v3Oct,
			pResult,8);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	MATRIX4			matWorldInverse;
	SetInverseMatrix(&matWorldInverse,&args.matWorldForPhysique);

	for (i=0; i<m_dwBoneObjectNum; i++)
	{

		MatrixMultiply2(
			pMatrixEntryForPhysiqueLocal+m_pBoneObjectList[i].GetIndex(),
			pMatrixEntry+m_pBoneObjectList[i].GetIndex(),
			&matWorldInverse);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if ( dwFlag & RENDER_TYPE_UPDATE_COLLISION_BONEMESH_DESC)
	{		
		// 이 부분이 수행되지 않으면 본 매쉬가 제대로 나오지 않는다.
		pColMeshDesc = (COLLISION_MESH_OBJECT_DESC*)( (char*)pColModelDesc + m_dwColMeshDescOffsetBoneEntry );

		for (i=0; i<m_dwBoneObjectNum; i++)
		{
			pResult = pMatrixEntry + m_pBoneObjectList[i].GetIndex();
			
			pColMeshDesc->boundingSphere.fRs = m_pBoneObjectList[i].GetColMeshObjectDesc()->boundingSphere.fRs;
			
			TransformVector3_VPTR2(
				&pColMeshDesc->boundingSphere.v3Point,
				&m_pBoneObjectList[i].GetColMeshObjectDesc()->boundingSphere.v3Point,
				pResult,
				1);

			TransformVector3_VPTR2(
				pColMeshDesc->boundingBox.v3Oct,
				m_pBoneObjectList[i].GetColMeshObjectDesc()->boundingBox.v3Oct,
				pResult,
				8);

			pColMeshDesc++;
		}
	}

	ILLUSION_DESC*	pIllusionDesc;
	pIllusionDesc = m_pModelRefManager->GetModelIllusionDesc(dwRefIndex);

	
	if (!pIllusionDesc)
		goto lb_true;

	if (!pIllusionDesc->pMeshObj->GetLocalVertexPtr())
		goto lb_true;

	if (!pIllusionDesc->bEnable)
		goto lb_true;


	// 트랜스폼된 데이타를 세팅
	MATRIX4*	pResultMatrix;

	pResultMatrix = pMatrixEntry + pIllusionDesc->pMeshObj->GetIndex();
	pIllusionDesc->pIllusionKeyTail->tm = *pResultMatrix;

	TransformVector3_VPTR2(pIllusionDesc->pIllusionKeyTail->pv3List,
		pIllusionDesc->pMeshObj->GetLocalVertexPtr(),
		&pIllusionDesc->pIllusionKeyTail->tm,
		pIllusionDesc->dwVerticesNum);
	
	pIllusionDesc->pIllusionKeyTail->dwFrame = dwFrame;

	pIllusionDesc->pIllusionKeyTail = pIllusionDesc->pIllusionKeyTail->pNext;
	

	if (pIllusionDesc->dwCurIllusionKeyNum < pIllusionDesc->dwMaxIllutionKeyNum)
	{
		pIllusionDesc->dwCurIllusionKeyNum++;
		// 현재 저장된 잔상 프레임 개수가 맥스치보다 작아야 개수를 증가시킨다.

	}
	
	if (pIllusionDesc->dwCurIllusionKeyNum >= 2)
	{
		ILLUSION_KEY_DESC*	pCur;
		DWORD dwFacesNum,k,n,n_max,k_max;

		dwFacesNum = 0;

		IVERTEX*	pivResult = pIllusionDesc->pIVertexList;

		pCur = pIllusionDesc->pIllusionKeyHead;
		n_max = pIllusionDesc->dwCurIllusionKeyNum-1;
		k_max = pIllusionDesc->dwVerticesNum-1;

		for (n=0; n<n_max; n++)
		{

			for (k=0; k<k_max; k++)
			{
				*(VECTOR3*)&pivResult[0].x = pCur->pv3List[k];
				pivResult[0].u1 = (float)n / (float)n_max;
				pivResult[0].v1 = (float)k / (float)k_max;

				
				*(VECTOR3*)&pivResult[1].x = pCur->pv3List[k+1];
				pivResult[1].u1 = (float)n / (float)n_max;
				pivResult[1].v1 = (float)(k+1) / (float)k_max;


				*(VECTOR3*)&pivResult[2].x = pCur->pNext->pv3List[k+1];
				pivResult[2].u1 = (float)(n+1) / (float)n_max;
				pivResult[2].v1 = (float)(k+1) / (float)k_max;


				*(VECTOR3*)&pivResult[3].x = pCur->pv3List[k];
				pivResult[3].u1 = (float)n / (float)n_max;
				pivResult[3].v1 = (float)k / (float)k_max;


				*(VECTOR3*)&pivResult[4].x = pCur->pNext->pv3List[k+1];
				pivResult[4].u1 = (float)(n+1) / (float)n_max;
				pivResult[4].v1 = (float)(k+1) / (float)k_max;


				*(VECTOR3*)&pivResult[5].x = pCur->pNext->pv3List[k];
				pivResult[5].u1 = (float)(n+1) / (float)n_max;
				pivResult[5].v1 = (float)k / (float)k_max;

#ifdef _DEBUG
				if ( (DWORD)(pIllusionDesc->pIVertexList + pIllusionDesc->dwResultVerticesNum) < (DWORD)pivResult)
					__asm int 3
#endif
				pivResult += 6;
				dwFacesNum += 2;

				
				
			}
		
			pCur = pCur->pNext;
		}
		pIllusionDesc->dwFacesNum = dwFacesNum;
//		m_pRenderer->RenderTriVector3(v3Result,dwFacesNum,0);
	}
lb_true:

	bResult = TRUE;
lb_return:

	return bResult;
}


BOOL __stdcall CoModel::GetWorldMatrix(MATRIX4* pMatWorld,DWORD dwRefIndex,DWORD dwObjIndex)
{
	BOOL	bResult = FALSE;

	MODEL_REF_DESC	modelRefDesc;
	if (!m_pModelRefManager->GetModelRefDesc(&modelRefDesc,dwRefIndex))
		goto lb_return;

	if (dwObjIndex >= m_dwObjectNum)
		goto lb_return;

	*pMatWorld = modelRefDesc.pMatrixEntry[dwObjIndex];

	bResult = TRUE;
lb_return:
	return bResult;

}
void __stdcall CoModel::SetBeginIllusionEffect(DWORD dwRefIndex,DWORD dwObjIndex)
{
	ILLUSION_DESC*	pIllusionDesc;
	pIllusionDesc = m_pModelRefManager->GetModelIllusionDesc(dwRefIndex);
	
	if (!pIllusionDesc)
		return;

	pIllusionDesc->dwFacesNum = 0;
	pIllusionDesc->bEnable = TRUE;
	pIllusionDesc->dwCurIllusionKeyNum = 0;
	pIllusionDesc->pIllusionKeyHead = pIllusionDesc->pIllusionKeyTail;
	m_pRenderer->DisableRenderTriBuffer(pIllusionDesc->pTriBufferHandle);

}

void __stdcall CoModel::SetEndIllusionEffect(DWORD dwRefIndex,DWORD dwObjIndex)
{
	ILLUSION_DESC*	pIllusionDesc;
	pIllusionDesc = m_pModelRefManager->GetModelIllusionDesc(dwRefIndex);
	
	if (!pIllusionDesc)
		return;


	pIllusionDesc->bEnable = FALSE;
	pIllusionDesc->dwCurIllusionKeyNum = 0;
	pIllusionDesc->pIllusionKeyHead = pIllusionDesc->pIllusionKeyTail;
	m_pRenderer->DisableRenderTriBuffer(pIllusionDesc->pTriBufferHandle);
	
}

BOOL CoModel::DuplicateStatus(CoModel* pDestModel,DWORD dwDestRefIndex,DWORD dwSrcRefIndex)
{

	return TRUE;
}
BOOL __stdcall CoModel::GetCollisionMeshObjectDesc(COLLISION_MESH_OBJECT_DESC* pColMeshObjDesc,DWORD dwModelRefIndex,DWORD dwObjIndex)
{
	BOOL		bResult = FALSE;

	COLLISION_MODEL_DESC*		pColModelDesc;
	COLLISION_MESH_OBJECT_DESC* pDesc;

	if (!m_pModelRefManager->GetColModelDesc(&pColModelDesc,dwModelRefIndex))
		goto lb_return;

	pDesc = ((CMeshObject*)m_ppObjectList[dwObjIndex])->GetColMeshObject(pColModelDesc);
	*pColMeshObjDesc = *pDesc;

	bResult = TRUE;

lb_return:
	return bResult;

}
BOOL __stdcall CoModel::GetCollisionModelDesc(COLLISION_MODEL_DESC* pColMeshModelDesc,DWORD dwModelRefIndex)
{
	BOOL		bResult = FALSE;
	COLLISION_MODEL_DESC*		pColModelDesc;
	
	if (!m_pModelRefManager->GetColModelDesc(&pColModelDesc,dwModelRefIndex))
		goto lb_return;

	*pColMeshModelDesc = *pColModelDesc;
	
	bResult = TRUE;

lb_return:
	return bResult;

}
DWORD __stdcall CoModel::GetMeshObjectIndex(DWORD dwSeqIndex)
{
	DWORD	dwIndex = 0xffffffff;

	if (dwSeqIndex >= m_dwMeshObjectNum)
		goto lb_return;

	dwIndex = m_pMeshObjectList[dwSeqIndex].GetIndex();

lb_return:
	return dwIndex;
}

DWORD __stdcall CoModel::GetMeshObjectNum()
{
	return m_dwMeshObjectNum;
}
DWORD __stdcall	CoModel::GetObjectNum()
{
	return m_dwObjectNum;
	
}


CBaseObject* CoModel::SelectObject(DWORD dwIndex)
{
	CBaseObject*	pObj = NULL;
	if (dwIndex >= m_dwObjectNum)
		goto lb_return;

	pObj = m_ppObjectList[dwIndex];

lb_return:
	return pObj;
	

}
CMotionObject* CoModel::GetMotionObject(char* szObjName,DWORD dwNameLen,DWORD dwRefIndex,DWORD dwMotionIndex)
{
	CMotionObject*	pMotObj = NULL;
	CMotion*		pMotion;

	MOTION_LIST_DESC*	pMotionListDesc;

	if (!m_pModelRefManager->GetMotionListDesc(&pMotionListDesc,dwRefIndex))
		goto lb_return;

	if (!pMotionListDesc->ppMotionList)
		goto lb_return;

	if (dwMotionIndex > pMotionListDesc->dwMaxMotionNum)
		goto lb_return;
	

	pMotion = pMotionListDesc->ppMotionList[dwMotionIndex];
	if (!pMotion)
		goto lb_return;

//	RDTSC_BEGIN
	pMotObj =  pMotion->GetMotionObject(szObjName,dwNameLen);
//	RDTSC_END


lb_return:
	return pMotObj;


}

BOOL __stdcall CoModel::DuplicateAnimation(DWORD dwDestRefIndex,I3DModel* pDestModel,DWORD dwSrcRefIndex)
{
	DWORD		i;
	BOOL		bResult = FALSE;

	CoModel*	pModel = (CoModel*)pDestModel;
	DWORD dwMotionNum = GetMotionNum(dwSrcRefIndex);
	

	if (!dwMotionNum)
		goto lb_return;

	if (!pModel->CreateMotionList(dwMotionNum,dwDestRefIndex))
		goto lb_return;

	MOTION_LIST_DESC*	pMotionListDesc;
	if (!m_pModelRefManager->GetMotionListDesc(&pMotionListDesc,dwSrcRefIndex))
		goto lb_return;

	for (i=1; i<=pMotionListDesc->dwMaxMotionNum; i++)
	{
		if (pMotionListDesc->ppMotionList[i])
		{
			if (pModel->AddMotion(pMotionListDesc->ppMotionList[i],dwDestRefIndex,0))
				pMotionListDesc->ppMotionList[i]->AddRef();
		}
	}
	bResult = TRUE;

lb_return:
	return bResult;

}
BOOL __stdcall CoModel::DuplicateMaterial(DWORD dwDestRefIndex,I3DModel* pDestModel,DWORD dwSrcRefIndex)
{
	DWORD		i;
	BOOL		bResult = FALSE;

	CoModel*	pModel = (CoModel*)pDestModel;
	DWORD dwMaterialNum = GetMaterialNum(dwSrcRefIndex);
	

	if (!dwMaterialNum)
		goto lb_return;

	if (!pModel->CreateMaterialList(dwMaterialNum,dwDestRefIndex))
		goto lb_return;

	MATERIAL_LIST_DESC*	pMaterialListDesc;
	if (!m_pModelRefManager->GetMaterialListDesc(&pMaterialListDesc,dwSrcRefIndex))
		goto lb_return;

	for (i=1; i<=pMaterialListDesc->dwMaxMaterialNum; i++)
	{
		if (pMaterialListDesc->ppMtlLibList[i])
		{
			
			if (pModel->AddMaterial(pMaterialListDesc->ppMtlLibList[i],dwDestRefIndex,0))
				pMaterialListDesc->ppMtlLibList[i]->AddRef();
		}
	}
	bResult = TRUE;

lb_return:
	return bResult;

}
DWORD __stdcall CoModel::GetObjectIndex(char* szObjName)
{
	DWORD		dwIndex = 0xffffffff;

	CBaseObject* pObj = GetObject(szObjName);
	if (!pObj)
		goto lb_return;

	dwIndex = pObj->GetIndex();

lb_return:
	return dwIndex;
}

char* __stdcall	CoModel::GetObjectName(DWORD dwObjIndex)
{
	char*	pObjName = NULL;
	CBaseObject*	pObj = SelectObject(dwObjIndex);
	if (!pObj)
		goto lb_return;

	pObjName = pObj->GetObjectName();

lb_return:
	return pObjName;
}

DWORD __stdcall	CoModel::GetMeshObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,DWORD dwRefIndex,MATRIX4* pParent,MATRIX4* pTransform,DWORD dwFrame,DWORD dwMotionIndex,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
	MATRIX4		matProj,matView;

	m_pGeometry->GetProjectionMatrix(&matProj,dwViewportIndex);
	m_pGeometry->GetViewMatrix(&matView,dwViewportIndex);

	
	DWORD dwResultIndex = GetMeshObjectWithScreenCoordAndMatrix(
		pv3IntersectPoint,
		pfDist,
		pdwFaceGroupIndex,
		dwRefIndex,
		pParent,
		pTransform,
		dwFrame,
		dwMotionIndex,
		ptCursor,
		&matView,
		&matProj,
		&m_pGeometry->INL_GetViewport(dwViewportIndex)->rcClip,
		dwFlag);
	
	return dwResultIndex;
}
DWORD __stdcall	CoModel::GetMeshObjectWithScreenCoordAndMatrix(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,DWORD dwRefIndex,MATRIX4* pParent,MATRIX4* pTransform,DWORD dwFrame,DWORD dwMotionIndex,POINT* ptCursor,MATRIX4* pMatView,MATRIX4* pMatProj,SHORT_RECT* pClipRect,DWORD dwFlag)
{
	DWORD		i,dwWidth,dwHeight;

	VECTOR3		v3Pos;
	VECTOR3		v3Dir;

	float		fDist,fDistPerFace;
	VECTOR3		v3IntersectPoint,v3IntersectPointPerFace;
//	VECTOR3		v3ResultIntersectTri[3];

	DWORD		dwFaceGroupIndex;

	float		fMinDist = 900000.0f;
	
	DWORD		dwResultIndex = 0xffffffff;

	COLLISION_MESH_OBJECT_DESC* pColMeshDesc;	
	COLLISION_MODEL_DESC*		pColModelDesc;
	MATRIX4*	pMatrixEntry;
	MATRIX4*	pMatrixEntryForPhysiqueLocal;
	
	MODEL_REF_DESC	modelRefDesc;
	if (!m_pModelRefManager->GetModelRefDesc(&modelRefDesc,dwRefIndex))
		goto lb_return;

	pMatrixEntry = modelRefDesc.pMatrixEntry;
	pMatrixEntryForPhysiqueLocal = modelRefDesc.pMatrixEntryForPhysiqueLocal;		

	pColModelDesc = modelRefDesc.pColModelDesc;
		
	dwWidth = pClipRect->right - pClipRect->left;
	dwHeight = pClipRect->bottom - pClipRect->top;

	POINT p;
	if (ptCursor->x > pClipRect->right || ptCursor->y > pClipRect->bottom)
		goto lb_return;

	p.x = ptCursor->x - pClipRect->left;
	p.y = ptCursor->y - pClipRect->top;

	if (p.x < 0 || p.y < 0)
		goto lb_return;


	SetFrame(modelRefDesc.pModelStatus,pMatrixEntry,pMatrixEntryForPhysiqueLocal,pColModelDesc,pParent,pTransform,dwRefIndex,dwFrame,dwMotionIndex,RENDER_TYPE_UPDATE_COLLISION_BONEMESH_DESC);

	CalcRay(&v3Pos,&v3Dir,p.x,p.y,dwWidth,dwHeight,pMatProj,pMatView);
		

	if (PICK_TYPE_PER_BONE_OBJECT != dwFlag)
		goto lb_search_mesh;

	if (!m_dwBoneObjectNum)
		goto lb_search_mesh;



	pColMeshDesc = (COLLISION_MESH_OBJECT_DESC*)( (char*)pColModelDesc + m_dwColMeshDescOffsetBoneEntry );

	for (i=0; i<m_dwBoneObjectNum; i++)
	{
		if (IsCollisionMeshAndRay(&v3IntersectPoint,&fDist,pColMeshDesc,&v3Pos,&v3Dir))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;

				*pfDist = fDist;
				*pdwFaceGroupIndex = 0xffffffff;
				dwResultIndex = pColMeshDesc->dwObjIndex;
				*pv3IntersectPoint = v3IntersectPoint;
			}
		}
		pColMeshDesc++;
	}
	goto lb_return;

lb_search_mesh:
	for (i=0; i<m_dwMeshObjectNum; i++)
	{
		pColMeshDesc = m_pMeshObjectList[i].GetColMeshObject(pColModelDesc);

		if (IsCollisionMeshAndRay(&v3IntersectPoint,&fDist,pColMeshDesc,&v3Pos,&v3Dir))
		{
			if (PICK_TYPE_PER_FACE == dwFlag )
			{
				if (!m_pMeshObjectList[i].IsCollisionMeshAndRay(&v3IntersectPointPerFace,&fDistPerFace,&dwFaceGroupIndex,&v3Pos,&v3Dir,pMatrixEntry+m_pMeshObjectList[i].GetIndex()))
					continue;

				v3IntersectPoint = v3IntersectPointPerFace;
				fDist = fDistPerFace;
			}
			if (fDist < fMinDist)
			{
				fMinDist = fDist;

				*pfDist = fDist;
				dwResultIndex = m_pMeshObjectList[i].GetIndex();
				*pv3IntersectPoint = v3IntersectPoint;
			}
		}
	}
lb_return:
	return dwResultIndex;
}
DWORD CoModel::GetMaterialIndex(char* szMtlName)
{
	return m_MaterialList.GetIndex(szMtlName);
	
}

BOOL CoModel::ReadFile(char* szFileName,DWORD dwFlag)
{

	void*	pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_BINARY);
	if (!pFP)
	{
		// FILE_NOT_FOUND /////////////////////////////////////////////////////////
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_FILE_NOT_FOUND,1,(void*)dwAddr,szFileName);
		///////////////////////////////////////////////////////////////////////////

		return FALSE;
	}
	
	CBaseObject*	pObject;
	OBJECT_TYPE type;
	DWORD		size;
	DWORD		i;
	
	lstrcpy(m_szFileName,szFileName);
	
	FILE_SCENE_HEADER header;
	g_pFileStorage->FSRead(pFP,(char*)&header,sizeof(FILE_SCENE_HEADER));

	
	m_dwMaxMeshObjectNum = header.dwMaxMeshObjectNum;
	m_dwMaxLightObjectNum = header.dwMaxLightObjectNum;
	m_dwMaxCameraObjectNum = header.dwMaxCameraObjectNum;
	m_dwMaxBoneObjectNum = header.dwMaxBoneObjectNum;

	m_dwObjectNum = header.dwObjectNum;
	m_ppObjectList = new CBaseObject*[m_dwObjectNum+4];
	memset(m_ppObjectList,0,sizeof(CBaseObject*)*(m_dwObjectNum+4));



	// 매터리얼 리스트를 수동으로 세팅한다.
	// 매터리얼 인덱스는 중간에 이가 빠지는 일이 없다.완전 선형적이다.익스포트할 당시 그렇게 만들어진다.

	m_MaterialList.Initialize(header.dwMaterialNum+1);
	CMaterial	mtl;
	m_MaterialList.SetMaterialNum(header.dwMaterialNum);

	for (i=0; i<header.dwMaterialNum; i++)
	{

		g_pFileStorage->FSRead(pFP,(char*)&type,sizeof(DWORD));
		g_pFileStorage->FSRead(pFP,(char*)&size,sizeof(DWORD));

		mtl.ReadFile(pFP);	

		BOOL bResult = m_MaterialList.SetMaterial(&mtl,mtl.GetIndex());
			
#ifdef _DEBUG	
		if (!bResult)
		{
		
			char	txt[512];
			memset(txt,0,512);
			wsprintf(txt,"CoModel::ReadFile(), if (!m_MaterialList.SetMaterial()), File:%s , Line:%d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
#endif

	}

	if (m_dwMaxMeshObjectNum)
		m_pMeshObjectList = new CMeshObject[m_dwMaxMeshObjectNum];
	
	if (m_dwMaxLightObjectNum)
		m_pLightObjectList = new CLightObject[m_dwMaxLightObjectNum];
	
	if (m_dwMaxCameraObjectNum)
		m_pCameraObjectList = new CCameraObject[m_dwMaxCameraObjectNum*2];
	
	if (m_dwMaxBoneObjectNum)
		m_pBoneObjectList = new CBoneObject[m_dwMaxBoneObjectNum];


	
	

	for (i=0; i<header.dwObjectNum; i++)
	{
		g_pFileStorage->FSRead(pFP,&type,sizeof(DWORD));

		if (OBJECT_TYPE_MESH == type)
		{
			pObject = m_pMeshObjectList + m_dwMeshObjectNum;
			m_dwMeshObjectNum++;
		}
		else if (OBJECT_TYPE_CAMERA == type)
		{
			pObject = &m_pCameraObjectList[m_dwCameraObjectNum*2];
			m_dwCameraObjectNum++;
		}
		else if (OBJECT_TYPE_CAMERA_TARGET == type)
		{
			pObject = &m_pCameraObjectList[m_dwCameraObjectNum*2+1];
		}
		
		else if (OBJECT_TYPE_LIGHT == type)
		{
			pObject = m_pLightObjectList + m_dwLightObjectNum;
			m_dwLightObjectNum++;
		}
		else if (OBJECT_TYPE_BONE == type)
		{
			pObject = m_pBoneObjectList + m_dwBoneObjectNum;
			m_dwBoneObjectNum++;
		}
	
		g_pFileStorage->FSRead(pFP,&size,sizeof(DWORD));
		pObject->ReadFile(pFP,header.dwVersion);

		pObject->SetModelPtr(this);
		m_ppObjectList[pObject->GetIndex()] = pObject;
#ifdef _DEBUG
		if (pObject->GetIndex() >= m_dwObjectNum)
		{
			char	txt[512];
			memset(txt,0,512);
			wsprintf(txt,"CoModel::ReadFile(), if (pObject->GetIndex() >= m_dwObjectNum), File:%s , Line:%d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
#endif
	}
	g_pFileStorage->FSCloseFile(pFP);
//	fclose(fp);

	if (!(dwFlag & MODEL_READ_TYPE_AS_LOD_SUBMODEL))
	{

		//trustpak 2005/06/24

		if (MODEL_LOD_USING_MODE_NOT_USE != m_pGeometry->GetModelLODUsingMode())
		{
			
		///			
			// lod로드..
			char	suffix[PID_LEN+1+1];
			DWORD	dwSuffixLen;
			
			dwSuffixLen = GetSuffixStringFromExt(suffix,szFileName,PID_LEN+1);
			if (dwSuffixLen)
			{
				if (!memcmp(suffix,PID_MODEL_LOD,PID_LEN) || !memcmp(suffix,PID_MODEL_lod,PID_LEN))
				{
					DWORD	dwSubModelNum;
					char	szLODLevelNum[2];
					GetRightString(szLODLevelNum,suffix,1);
					dwSubModelNum = atoi(szLODLevelNum);


					ReadLODModels(szFileName,dwSubModelNum);
				}
			}

		//trustpak 2005/06/24
		}
		///
	}
#ifdef _DEBUG
	char txt[512];
	wsprintf(txt,"Load %s\n",m_szFileName);
	OutputDebugString(txt);
#endif
	return TRUE;
}
void CoModel::ReadLODModels(char* szFileName,DWORD dwSubModelNum)
{
	char	szLodFileName[MAX_NAME_LEN];
	char	szFullPathFileName[_MAX_PATH];
	DWORD	dwLen;
	
	if (dwSubModelNum > MAX_LOD_MODEL_NUM-1)
		dwSubModelNum = MAX_LOD_MODEL_NUM-1;

	m_pSubModel[0] = this;		// 0번 모델은 최상위 디테일 모델 

	for (int i=dwSubModelNum-1; i>=0; i--)
	{
		dwLen = RemoveExt(szLodFileName,szFileName);
		*(szLodFileName+dwLen-1) = 0;
		wsprintf(szFullPathFileName,"%s%d%s",szLodFileName,i,".mod");

		/*
		HANDLE hFile = CreateFile(szFullPathFileName,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			break;
			*/

		//trustpak 2005/02/04
		if (TRUE != g_pFileStorage->IsExistInFileStorage(szFileName))
		{
			HANDLE hFile = CreateFile(szFullPathFileName,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				break;
			}

			CloseHandle(hFile);
		}


		/*
		CloseHandle(hFile);
		*/
		
		m_dwSubModelNum++;

		m_pSubModel[m_dwSubModelNum] = new CoModel;
		m_pSubModel[m_dwSubModelNum]->ReadFile(szFullPathFileName,MODEL_READ_TYPE_AS_LOD_SUBMODEL);
		m_pSubModel[m_dwSubModelNum]->AddRef();
		m_pSubModel[m_dwSubModelNum]->SetGeometry(m_pGeometry);

#ifdef _TRUSTPAK_TEMPORARY_WORK_		

		char szDebugString[0x400];
		memset(szDebugString, 0, 0x400);

		sprintf(szDebugString, " <-- Read Model : %s, LOD Level : %d\n",  m_szFileName, m_dwSubModelNum);
		OutputDebugString(szDebugString);
		
#endif

	}
}

CMaterial* CoModel::GetMaterial(DWORD dwMtlIndex)
{
	return m_MaterialList.GetMaterial(dwMtlIndex);
	
}

void CoModel::ReleaseMaterials()
{
	m_MaterialList.Release();
	
}

void CoModel::ReleaseObjects()
{
	if (m_pMeshObjectList)
	{
		delete [] m_pMeshObjectList;
		m_pMeshObjectList = NULL;
		m_dwMeshObjectNum = 0;
	}
	if (m_pLightObjectList)
	{
		delete [] m_pLightObjectList;
		m_pLightObjectList = NULL;
		m_dwLightObjectNum = 0;
	}
	if (m_pCameraObjectList)
	{
		delete [] m_pCameraObjectList;
		m_pCameraObjectList = NULL;
		m_dwCameraObjectNum = 0;
	}
	if (m_pBoneObjectList)
	{
		delete [] m_pBoneObjectList;
		m_pBoneObjectList = NULL;
		m_dwBoneObjectNum = 0;
	}
	if (m_ppObjectList)
	{
		delete [] m_ppObjectList;
		m_ppObjectList = NULL;
		m_dwObjectNum = 0;
	}
}
void CoModel::ReleaseTransformSet()
{
	if (m_pTransformSetList)
	{
		for (DWORD i=0; i<m_dwTransformSetNum; i++)
		{
			if (m_pTransformSetList[i].ppObjectList)
				delete [] m_pTransformSetList[i].ppObjectList;
		}
		
		delete [] m_pTransformSetList;
		m_pTransformSetList = NULL;
		m_dwTransformSetNum = 0;
	}
}

DWORD CoModel::CreateIVertexList(IVERTEX** ppVertex,DWORD dwRefIndex,MATRIX4* pParent,MATRIX4* pTransform,DWORD dwFrame,DWORD dwMotionIndex)
{	
	// 리턴값은 페이스 갯수

	if (!m_dwMeshObjectNum)
		return 0;

	COLLISION_MODEL_DESC*	pColModelDesc;	
	MATRIX4*				pMatrixEntry;
	MATRIX4*				pMatrixEntryForPhysiqueLocal;

	MODEL_REF_DESC	modelRefDesc;
	if (!m_pModelRefManager->GetModelRefDesc(&modelRefDesc,dwRefIndex))
		return 0;

	pColModelDesc = modelRefDesc.pColModelDesc;
	pMatrixEntry = modelRefDesc.pMatrixEntry;
	pMatrixEntryForPhysiqueLocal = modelRefDesc.pMatrixEntryForPhysiqueLocal;		

	SetFrame(modelRefDesc.pModelStatus,pMatrixEntry,pMatrixEntryForPhysiqueLocal,pColModelDesc,pParent,pTransform,dwRefIndex,dwFrame,dwMotionIndex,RENDER_TYPE_UPDATE_COLLISION_BONEMESH_DESC);
	

	DWORD			dwResult;
	DWORD			dwTotalVertexCount = 0;
	DWORD			dwTotalFacesNum = 0;
	DWORD			dwViewAlignedFacesNum = 0;

	for (DWORD i=0; i<m_dwMeshObjectNum; i++)
	{
		dwTotalFacesNum += m_pMeshObjectList[i].GetTotalFacesNum();
	}
	IVERTEX*		pvSrc;
	IVERTEX*		pv = new IVERTEX[dwTotalFacesNum*3];
	IVERTEX*		pEntry = pv;

	for (i=0; i<m_dwMeshObjectNum; i++)
	{
		if (m_pMeshObjectList[i].GetMeshFlag().GetTransformType() == TRANSFORM_TYPE_ALIGN_VIEW)
		{
			dwViewAlignedFacesNum += m_pMeshObjectList[i].GetTotalFacesNum();
		}
		else 
		{
			dwResult = m_pMeshObjectList[i].CreateIVertexList(&pvSrc,pMatrixEntry+m_pMeshObjectList[i].GetIndex());
			if (dwResult)
			{
				dwTotalVertexCount += dwResult;
				memcpy(pEntry,pvSrc,sizeof(IVERTEX)*dwResult);
				m_pMeshObjectList[i].ReleaseIVertexList(pvSrc);
				pEntry += dwResult;
			}
		}
	}
	*ppVertex = pv;
	
	return dwTotalVertexCount/3;
}
void __stdcall CoModel::ReleaseIVertexList(IVERTEX*	pVertex)
{
	delete [] pVertex;

}

DWORD __stdcall CoModel::GetMotionNum(DWORD dwRefIndex)
{
	DWORD	dwResult = 0;

	MOTION_LIST_DESC*	pMotionListDesc;
	if (!m_pModelRefManager->GetMotionListDesc(&pMotionListDesc,dwRefIndex))
		goto lb_return;
	
	dwResult =  pMotionListDesc->dwMotionNum;

lb_return:
	return dwResult;
}
DWORD __stdcall CoModel::GetMaterialNum(DWORD dwRefIndex)
{
	DWORD	dwResult = 0;

	MATERIAL_LIST_DESC*	pMaterialListDesc;
	if (!m_pModelRefManager->GetMaterialListDesc(&pMaterialListDesc,dwRefIndex))
		goto lb_return;
	
	dwResult =  pMaterialListDesc->dwMaterialNum;
	

lb_return:
	return dwResult;
}
BOOL __stdcall CoModel::GetMotionDesc(MOTION_DESC* pMotionDesc,DWORD dwMotionIndex,DWORD dwRefIndex)
{
	BOOL	bResult = FALSE;

	MOTION_LIST_DESC*	pMotionListDesc;
	if (!m_pModelRefManager->GetMotionListDesc(&pMotionListDesc,dwRefIndex))
		goto lb_return;



	if (dwMotionIndex > pMotionListDesc->dwMotionNum)
		goto lb_return;

	if (!pMotionListDesc->ppMotionList[dwMotionIndex])
		goto lb_return;

	bResult = pMotionListDesc->ppMotionList[dwMotionIndex]->GetMotionDesc(pMotionDesc);

lb_return:
	return bResult;

}


CBaseObject* CoModel::GetObject(char* szObjName)
{
	CBaseObject* pResult = NULL;
	
	for (DWORD i=0; i<m_dwObjectNum; i++)
	{
		if (!lstrcmp(szObjName,m_ppObjectList[i]->GetObjectName()))
		{
			pResult = m_ppObjectList[i];
			goto lb_return;
		}
	}
lb_return:
	return pResult;

}
void __stdcall CoModel::DisableUnloadPreLoadedResource(DWORD dwRefIndex)
{
	DWORD		i;
	BOOL		bResult = FALSE;

	m_pGeometry->DisableUnloadPreLoadedResource(m_dwIndexInGeometry);

	MOTION_LIST_DESC*	pMotionListDesc;
	if (m_pModelRefManager->GetMotionListDesc(&pMotionListDesc,dwRefIndex))
	{
		CMotion*	pMotion;
		for (i=0; i<=pMotionListDesc->dwMaxMotionNum; i++)
		{
			pMotion = GetMotion(dwRefIndex,i);
			if (pMotion)
			{
				m_pGeometry->DisableUnloadPreLoadedResource(pMotion->GetFileIndex());
			}
		}
	}
	MATERIAL_LIST_DESC*		pMaterialListDesc;
	if (m_pModelRefManager->GetMaterialListDesc(&pMaterialListDesc,dwRefIndex))
	{
		CMaterialLib*			pMtlLib;
		for (i=1; i<=pMaterialListDesc->dwMaxMaterialNum; i++)
		{
			pMtlLib = GetMaterialLib(dwRefIndex,i);
			if (pMtlLib)
			{
				m_pGeometry->DisableUnloadPreLoadedResource(pMtlLib->GetFileIndex());
			}
		}
	}
}
void __stdcall CoModel::EnableUnloadPreLoadedResource(DWORD dwRefIndex)
{	
	DWORD		i;
	BOOL		bResult = FALSE;

	m_pGeometry->EnableUnloadPreLoadedResource(m_dwIndexInGeometry);

	MOTION_LIST_DESC*	pMotionListDesc;
	if (m_pModelRefManager->GetMotionListDesc(&pMotionListDesc,dwRefIndex))
	{
		CMotion*	pMotion;
		for (i=0; i<=pMotionListDesc->dwMaxMotionNum; i++)
		{
			pMotion = GetMotion(dwRefIndex,i);
			if (pMotion)
			{
				m_pGeometry->EnableUnloadPreLoadedResource(pMotion->GetFileIndex());
			}
		}
	}
	MATERIAL_LIST_DESC*		pMaterialListDesc;
	if (m_pModelRefManager->GetMaterialListDesc(&pMaterialListDesc,dwRefIndex))
	{
		CMaterialLib*			pMtlLib;
		for (i=1; i<=pMaterialListDesc->dwMaxMaterialNum; i++)
		{
			pMtlLib = GetMaterialLib(dwRefIndex,i);
			if (pMtlLib)
			{
				m_pGeometry->EnableUnloadPreLoadedResource(pMtlLib->GetFileIndex());
			}
		}
	}
}
BOOL __stdcall CoModel::Unload()
{
	BOOL	bResult = FALSE;
	if (!m_pGeometry)
		goto lb_return;
	
	if (!m_dwIndexInGeometry)
		goto lb_return;

	
	bResult = m_pGeometry->UnloadPreLoadedItem(m_dwIndexInGeometry);
lb_return:
	return bResult;
	
}

CoModel::~CoModel()
{
#ifdef _DEBUG
	char txt[512];
	wsprintf(txt,"Release %s\n",m_szFileName);
	OutputDebugString(txt);
#endif

//	DeleteAllMotion();
//	DeleteAllMaterial();
	for (DWORD i=1; i<=m_dwSubModelNum; i++)
	{
		m_pSubModel[i]->Release();
		m_pSubModel[i] = NULL;
	}
//	DeleteAllMotion();
	ReleaseObjects();
	ReleaseMaterials();
	ReleaseTransformSet();


	if (m_ppViewAlignedObjectsList)
	{
		delete [] m_ppViewAlignedObjectsList;
		m_ppViewAlignedObjectsList = NULL;
	}
	if (m_pColMeshModelDescLocal)
	{
		delete [] m_pColMeshModelDescLocal;
		m_pColMeshModelDescLocal = NULL;
	}
	if (m_pColMeshModelDescWorld)
	{
		delete [] m_pColMeshModelDescWorld;
		m_pColMeshModelDescWorld = NULL;
	}

//	if (m_dwDefaultMtlSetIndex != 0xffffffff)
//		m_pRenderer->DeleteMaterialSet(m_dwDefaultMtlSetIndex);

	if (m_pGeometry)
	{
		if (m_dwIndexInGeometry)
			m_pGeometry->DeleteFileItem(m_dwIndexInGeometry);
	}
	
}



BOOL __stdcall CoModel::RenderObject(DWORD dwObjIndex,DWORD dwAlpha,DWORD dwFlag)
{
	// static 오브젝트만을 위한...
	// 인덱스가 매쉬오브젝트가 아닐수도 있다...주의필요...

	BOOL			bResult = FALSE;
	float			fDistance;

	VIEW_VOLUME*	pVV = m_pGeometry->INL_GetViewvolume();
	COLLISION_MESH_OBJECT_DESC* pColMeshDesc;	

	
	COLLISION_MODEL_DESC*			pColModelDesc = m_pColMeshModelDescWorld;

	if (dwObjIndex >= m_dwObjectNum)
		goto lb_return;

	if (m_ppObjectList[dwObjIndex]->GetObjectType() != OBJECT_TYPE_MESH)
		goto lb_return;

	m_ppObjectList[dwObjIndex]->ResetDefaultResultMatrix();


//	SetIdentityMatrix(GetObjectResultMatrix(0,dwObjIndex));
//	m_ppObjectList[dwObjIndex]->InitializeResultMatrix();

	// 오브젝트가 뷰볼륨안에 들어가지 않는 경우 클립
	pColMeshDesc = ((CMeshObject*)m_ppObjectList[dwObjIndex])->GetColMeshObject(pColModelDesc);
	if (ClipRenderObjectWithViewVolume(pVV,pColMeshDesc,0))
	{	
		fDistance = DotProduct(&m_pGeometry->INL_GetCameraEyePlane()->v3Up,&pColMeshDesc->boundingSphere.v3Point) 
					+ m_pGeometry->INL_GetCameraEyePlane()->D;
		
		
		m_pRenderer->RenderMeshObject(((CMeshObject*)m_ppObjectList[dwObjIndex])->GetDeviceObject(),
			0,
			fDistance,				// 카메라 eye로부터의 거리 
			dwAlpha,
			NULL,0,					// 포인트 라이트 포인터와 갯수 
			NULL,0,					// 스폿라이트 인덱스 포인터와 갯수.
			0,
			0,
			dwFlag);

	
		if (m_pGeometry->INL_GetDrawDebugFlag())
		{
			m_pRenderer->RenderBox(pColMeshDesc->boundingBox.v3Oct,0xff00ff00);
//			m_pRenderer->RenderSphere(&pColMeshDesc->boundingSphere.v3Point,pColMeshDesc->boundingSphere.fRs,0xffffffff);
		}
	}
	
	bResult = TRUE;

lb_return:
	return bResult;
}
char* __stdcall	CoModel::GetModelFileName()
{
	return m_szFileName;
}

//#pragma optimize( "", on )

/*
void __stdcall CoModel::ClearCache(DWORD dwRefIndex)
{
	SET_FRAME_ARGS*	pArgs;
	if (!m_pModelRefManager->GetModelSetFrameArgs(&pArgs,dwRefIndex))
		goto lb_return;

	pArgs->dwFrame = 0xffffffff;
	pArgs->dwMotionIndex = 0xffffffff;
	
	DWORD	i;
	for (i=0; i<m_dwMeshObjectNum; i++)
	{
		m_pMeshObjectList[i].ClearVBCache(m_pRenderer);
	}
lb_return:
	return;
}*/
