#include "CoStaticModel.h"
#include "model.h"
#include "geom_obj.h"
#include "geometry_global.h"
#include "CoGeometry.h"
//#include <io.h>
#include "global_variable.h"
#include "CollisionStaticModel.h"
#include	"../4DyuchiGRX_myself97/myself97common.h"
#include <crtdbg.h>

CoStaticModel::CoStaticModel()
{
	memset((char*)this+4,0,sizeof(CoStaticModel)-4);
}

STDMETHODIMP CoStaticModel::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (IID_IUnknown == refiid)
		*ppv = this;
	else if (IID_HeightField == refiid)
		*ppv = (IHeightField*)this;
	else 
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();
	return NOERROR;
	
}
STDMETHODIMP_(ULONG) CoStaticModel::AddRef(void)
{
	m_dwRefCount++;
	return 0;
}
STDMETHODIMP_(ULONG) CoStaticModel::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}
BOOL CoStaticModel::Initialize(I4DyuchiGXRenderer* pDev,DWORD dwMaxStaticObjectNum,DWORD dwMaxMaterialNum)
{
	m_pRenderer = pDev;
	m_pIndexItemTableMeshObject = ITCreate();
	ITInitialize(m_pIndexItemTableMeshObject,dwMaxStaticObjectNum);
	m_dwMaxMaterialNum = dwMaxMaterialNum;
	m_pMaterialList = new CMaterialList;
	m_bRecvShadowEnable = TRUE;
	return TRUE;
}


BOOL __stdcall CoStaticModel::BeginCreateMesh(DWORD dwFlag)
{
	// 현존하는 매터리얼을 몽땅 해쉬에 넣기.
	if (!m_pMaterialList->GetMaterialNum())
	{
		m_pMaterialList->Initialize(m_dwMaxMaterialNum);
		m_pMaterialList->BeginBulidMaterialList(m_dwMaxMaterialNum);
	}
	else
	{
		ResetMaterialAll();
	}


	return TRUE;
}

void CoStaticModel::ResetMaterialAll()
{
	CMaterialList*	pMtlListTemp = new CMaterialList;
	pMtlListTemp->Initialize(4096);
	pMtlListTemp->BeginBulidMaterialList(4096);

	for (DWORD i=0; i<ITGetItemNum(m_pIndexItemTableMeshObject); i++)
	{
		CMeshObject* pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,i);
		for (DWORD j=0; j<pObj->GetFaceGroupNum(); j++)
		{
			ResetMaterial(pObj->GetFaceGroup()+j,pMtlListTemp);
		}
	}
	delete m_pMaterialList;
	m_pMaterialList = pMtlListTemp;
}
/*
BOOL __stdcall CoStaticModel::CopyMesh_I3DModel_To_I3DStaticModel(I3DModel* pModel,DWORD dwRefIndex,MATRIX4* pTransformMatrix)
{
	CoModel*	pSrcModel = (CoModel*)pModel;
	
	pSrcModel->UpdateColMeshDesc(dwRefIndex,NULL,pTransformMatrix,0,0,0);
	for (DWORD i=0; i<pModel->GetMeshObjectNum(); i++)
	{
		AddObjectFromModel(pModel,pModel->GetMeshObjectIndex(i),pTransformMatrix,0);
	}
	InitializeObjects();
	return TRUE;
}
*/
BOOL __stdcall CoStaticModel::EndCreateMesh()
{
//	m_pMaterialList->EndBulidMaterialList();
	return TRUE;
}
void CoStaticModel::InitializeObjects()
{
//	CMeshFlag	flag;
//	CreateCollisionDesc(flag);


	DWORD			i;
	CMeshObject*	pObj;
	

	DWORD	dwNum = ITGetItemNum(m_pIndexItemTableMeshObject);
	
	m_pDefaultMatrixList = new MATRIX4[dwNum];
	memset(m_pDefaultMatrixList,0,sizeof(MATRIX4)*dwNum);

	for (i=0; i<dwNum; i++)
	{	

		pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,i);
		pObj->SetResultMatrixPtr(m_pDefaultMatrixList + i);
		pObj->ResetResultMatrix();
		pObj->SetGeometry(m_pGeometry);
		pObj->SetCollsionModelDescEntry(m_pColMeshModelDescWorld);
		pObj->Initialize(m_pMaterialList,NULL,m_pGeometry);

/*		
		CMeshFlag	flag;
		flag = ((CMeshObject*)pObj)->GetMeshFlag();
		flag.SetRigidType(RIGID_TYPE_RIGID);
		
		
		if (!m_pRenderer)
			continue;


		if (!pObj->GetDeviceObject())
		{
			IDIMeshObject*	pMeshObj;
			pMeshObj = m_pRenderer->CreateMeshObject(flag);

			if (pMeshObj)
			{
			//	_ASSERT(_CrtCheckMemory());

				pObj->Initialize(pMeshObj,m_pMaterialList,NULL,m_pGeometry);
				pMeshObj->Update(0);
				
			}
		}*/
	}
}

CMeshObject* CoStaticModel::AllocObject()
{
	CMeshObject*	pObj = new CMeshObject;
	pObj->SetObjectType(OBJECT_TYPE_MESH);

	
	DWORD dwIndex = ITAddItem(m_pIndexItemTableMeshObject,(void*)pObj);
	if (dwIndex == 0xffffffff)
	{
		delete pObj;
		pObj = NULL;
	}
	pObj->SetIndex(dwIndex);

	return pObj;
}
BOOL __stdcall CoStaticModel::AddModel(I3DModel* pModel,DWORD dwRefIndex,MATRIX4* pTransformMatrix,DWORD dwFlag)
{
	
	BOOL		bResult = FALSE;
	CoModel*	pSrcModel = (CoModel*)pModel;
	DWORD		i,j;
	
	CMeshFlag	flag;

	CMeshObject* pMeshObjectDest;

	pSrcModel->UpdateColMeshDesc(dwRefIndex,NULL,pTransformMatrix,0,0,0);
	
	DWORD	dwMeshObjNum = pSrcModel->GetMeshObjectNum();
	for (i=0; i<dwMeshObjNum; i++)
	{
		pMeshObjectDest = AllocObject();
		if (!pMeshObjectDest)
			goto lb_return;

		//dwIndex = pMeshObject->GetIndex();

		pSrcModel->Duplicate(pMeshObjectDest,i,dwRefIndex);
	
		for (j=0; j<pMeshObjectDest->GetFaceGroupNum(); j++)
		{
			
			ResetMaterial(pMeshObjectDest->GetFaceGroup()+j,m_pMaterialList);
			
		}
		
	}

	CreateCollisionDesc(flag);
	InitializeObjects();
	bResult = TRUE;

lb_return:
	return bResult;
}
/*
DWORD __stdcall	CoStaticModel::AddObjectFromBrush(I3DBrush* pBrush,DWORD dwFlag)
{
	DWORD		dwIndex = 0xffffffff;
	CoBrush*	pSrcBrush = (CoBrush*)pBrush;
	DWORD		i,j;

	CMeshObject* pMeshObjectSrc;
	CMeshObject* pMeshObjectDest = AllocObject();

	if (!pMeshObjectDest)
		goto lb_return;

	dwIndex = pMeshObjectDest->GetIndex();
	
	pMeshObjectSrc = pSrcBrush->GetMeshObject();
	pMeshObjectSrc->Duplicate(pMeshObjectDest,NULL);
	pMeshObjectDest->ResetTransform(pMeshObjectSrc->GetResultMatrix());
	
	for (i=0; i<pMeshObjectDest->GetFaceGroupNum(); i++)
	{
		for (j=0; j<pMeshObjectDest->GetFaceGroupNum(); j++)
		{
			ResetMaterial(pMeshObjectDest->GetFaceGroup()+j,m_pMaterialList);
		}
	}

	InitializeObjects();

lb_return:
	return dwIndex;

}*/
BOOL __stdcall CoStaticModel::DeleteObject(DWORD dwIndex)
{
	// 링크드 리스트에서 제거하는 루틴을 추가해야한다.
	__asm int 3
	BOOL	bResult = FALSE;
	CMeshObject*	pObj = (CMeshObject*)ITGetItem(m_pIndexItemTableMeshObject,dwIndex);
	
	ITDeleteItem(m_pIndexItemTableMeshObject,dwIndex);
	delete pObj;
	bResult = TRUE;

	ResetMaterialAll();

lb_return:
	return bResult;
}

void CoStaticModel::ResetMaterial(CFaceGroup* pFaceGroup,CMaterialList* pMtlList)
{
	MATERIAL*		pMtl;
	
	if (pFaceGroup->GetMaterial())
	{

		pMtl  = pFaceGroup->GetMaterial()->GetMaterialInfo();
		
		DWORD dwMtlIndex = pMtlList->AddMaterial(pMtl);
		pFaceGroup->SetMtlIndex(dwMtlIndex);
		pFaceGroup->SetMaterial(NULL,pMtlList->GetMaterial(pFaceGroup->GetMtlIndex()));
	}
}
COLLISION_MESH_OBJECT_DESC*	__stdcall CoStaticModel::GetCollisionMeshObjectDesc(DWORD dwObjIndex)
{
	COLLISION_MESH_OBJECT_DESC* pColDesc = NULL;

	CMeshObject* pMeshObj = (CMeshObject*)ITGetItem(m_pIndexItemTableMeshObject,dwObjIndex);
	if (!pMeshObj)
		goto lb_return;

	pColDesc = pMeshObj->GetColMeshObject(m_pColMeshModelDescWorld);

lb_return:
	return pColDesc;
}
void __stdcall CoStaticModel::GetWorldBoundingMesh(COLLISION_MESH_OBJECT_DESC* pColDesc)
{
	if (m_pColMeshModelDescWorld)
		*pColDesc = m_pColMeshModelDescWorld->colMeshModel;
	else
		memset(pColDesc,0,sizeof(COLLISION_MESH_OBJECT_DESC));



	
}
COLLISION_MODEL_DESC* CoStaticModel::CreateCollisionDesc(CMeshFlag flag)
{
	CleanColMesh();

	DWORD				dwBoundingVertexNum;
	DWORD				size;
	VECTOR3				*pv3,*pv3Offset;	
	DWORD				i;

	if (!ITGetItemNum(m_pIndexItemTableMeshObject))
		goto lb_return;

	m_dwColMeshSize = size = sizeof(COLLISION_MESH_OBJECT_DESC) +
		sizeof(COLLISION_MESH_OBJECT_DESC)*ITGetItemNum(m_pIndexItemTableMeshObject) + 
		sizeof(DWORD);

	
	
	m_pColMeshModelDescWorld = (COLLISION_MODEL_DESC*)new char[size];
	


	m_pColMeshModelDescWorld->dwColMeshObjectDescNum = ITGetItemNum(m_pIndexItemTableMeshObject);


	dwBoundingVertexNum = ITGetItemNum(m_pIndexItemTableMeshObject)*8;
	pv3 = pv3Offset = new VECTOR3[dwBoundingVertexNum];

	CMeshObject* pObj;

	for (i=0; i<ITGetItemNum(m_pIndexItemTableMeshObject); i++)
	{
		pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,i);

		
		if ((pObj->GetMeshFlag().GetTransformType() == TRANSFORM_TYPE_ALIGN_VIEW) || (!pObj->GetColMeshTemp()))
		{
			pObj->CreateBoundingMesh(
				m_pColMeshModelDescWorld,
				m_pColMeshModelDescWorld->colMeshObjectDesc+i,
				m_pColMeshModelDescWorld->colMeshObjectDesc+i);
		}
		else
		{

			pObj->CreateBoundingMesh(
				m_pColMeshModelDescWorld,
				m_pColMeshModelDescWorld->colMeshObjectDesc+i,
				NULL);

			memcpy(pObj->GetColMeshObject(m_pColMeshModelDescWorld),pObj->GetColMeshTemp(),sizeof(COLLISION_MESH_OBJECT_DESC));
			
		}

//		lstrcpy(m_pColMeshModelDescWorld->colMeshObjectDesc[i].szObjName,pObj->GetObjectName());
		m_pColMeshModelDescWorld->colMeshObjectDesc[i].dwObjIndex = pObj->GetIndex();


		memcpy(pv3Offset,(m_pColMeshModelDescWorld->colMeshObjectDesc+i)->boundingBox.v3Oct,sizeof(VECTOR3)*8);
		pv3Offset += 8;
	}


	// 모델 전체의 충돌매쉬 계산
	// 로컬이든 월드든 상관없이 모델의 충돌메쉬는 월드좌표게를 따른다.
	::CalcBoundingMesh(pv3,dwBoundingVertexNum,&m_pColMeshModelDescWorld->colMeshModel);
	
	if (pv3)
		delete [] pv3;

	
lb_return:
	return m_pColMeshModelDescWorld;

}


BOOL __stdcall CoStaticModel::RenderObject(DWORD dwObjIndex,DWORD dwAlpha,DWORD dwMtlSetIndex,DWORD dwFlag)
{
	// static 오브젝트만을 위한...
	// 인덱스가 매쉬오브젝트가 아닐수도 있다...주의필요...


	BOOL							bResult = FALSE;

	COLLISION_MESH_OBJECT_DESC*		pColMeshDesc = NULL;
	LIGHT_INDEX_DESC*				pAppliedLightIndex = NULL;
	DWORD							dwAppliedLightNum = 0;

	LIGHT_INDEX_DESC*				pAppliedSpotLightIndex = NULL;
	DWORD							dwAppliedSpotLightNum = 0;


	float							fDistance = 0.0f;

	
	VIEW_VOLUME*		pVV = m_pGeometry->INL_GetViewvolume();
	
	CMeshObject* pMeshObj = (CMeshObject*)ITGetItem(m_pIndexItemTableMeshObject,dwObjIndex);

	if (pMeshObj->GetMeshFlag().GetTransformType() == TRANSFORM_TYPE_ALIGN_VIEW)
	{
		pMeshObj->ResetResultMatrix();		// 빌보드땜시 꼭 필요..
		pMeshObj->SetAlignedViewMatrix(NULL);
	}


	

	dwAppliedLightNum = m_pGeometry->GetDynamicRTLightNum();

	if (dwAppliedLightNum)
	{
		pAppliedLightIndex = m_pGeometry->AllocLightIndex(dwAppliedLightNum);
		if (!pAppliedLightIndex)
			dwAppliedLightNum = 0;
		
	}

	
	pColMeshDesc = pMeshObj->GetColMeshObject(m_pColMeshModelDescWorld);

	// 라이트 범위가 오브젝트에 닿지 않는 경우는 클립
	dwAppliedLightNum = ClipLightWithRS(pAppliedLightIndex,pColMeshDesc,m_pGeometry->GetDynamicRTLightDescBuffer(),dwAppliedLightNum,0);
	
	// 오브젝트가 뷰볼륨안에 들어가지 않는 경우 클립
	if (ClipRenderObjectWithViewVolume(pVV,pColMeshDesc,0))
	{	
		// 스폿 라이트가 있을 경우.현재의 라이트 갯수는 최대 스폿라이트 갯수를 초과할수 없다.
		if (m_pGeometry->GetSpotLightNum() & ~(m_bRecvShadowEnable-1))
		{
			pAppliedSpotLightIndex = m_pGeometry->AllocLightIndex(m_pGeometry->GetSpotLightNum());
			if (!pAppliedSpotLightIndex)
			{
				dwAppliedSpotLightNum = 0;
				goto lb_render_mesh;
			}

			for (DWORD i=0; i<m_pGeometry->GetSpotLightNum(); i++)
			{
			//	if (IsCollisionSpherAndSphere
			//		(&m_pGeometry->GetSpotLightDesc(i)->v3Point,m_pGeometry->GetSpotLightDesc(i)->fRs,
			//		&pColMeshDesc->boundingSphere.v3Point,pColMeshDesc->boundingSphere.fRs))
			//	{
					if (ClipRenderObjectWithViewVolume(m_pGeometry->GetSpotLightViewVolume(i),pColMeshDesc,0))
					{
						pAppliedSpotLightIndex[dwAppliedSpotLightNum].bLightIndex = i;
						pAppliedSpotLightIndex[dwAppliedSpotLightNum].pMtlHandle = m_pGeometry->GetSpotLightDesc(i)->pMtlHandle;
						pAppliedSpotLightIndex[dwAppliedSpotLightNum].bTexOP = m_pGeometry->GetSpotLightDesc(i)->dwProjTexIndex & 0x000000ff;

						dwAppliedSpotLightNum++;
					}
			//	}
			}
		}
lb_render_mesh:
		///////////////////////////
		fDistance = DotProduct(&m_pGeometry->INL_GetCameraEyePlane()->v3Up,&pColMeshDesc->boundingSphere.v3Point) 
					+ m_pGeometry->INL_GetCameraEyePlane()->D;

		fDistance += ( (float)(pMeshObj->GetMeshFlag().GetRenderZPriorityValue() * RENDER_ZPRIORITY_UNIT ) );

		if (!pMeshObj->GetDeviceObject())
		{
			OnFaultObject(pMeshObj);
		}

		m_pRenderer->RenderMeshObject(pMeshObj->GetDeviceObject(),
			0,
			fDistance,				// 카메라 eye로부터의 거리 
			dwAlpha,
			pAppliedLightIndex,dwAppliedLightNum,	// 포인트 라이트 포인터와 갯수 	
			pAppliedSpotLightIndex,dwAppliedSpotLightNum,	// 스폿라이트 인덱스 포인터와 갯수.
			dwMtlSetIndex,
			0,
			dwFlag);
			
		if (m_pGeometry->INL_GetDrawDebugFlag() & DEBUG_DRAW_STATIC_MODEL_COL_MESH)
		{
			m_pRenderer->RenderBox(pMeshObj->GetColMeshObject(m_pColMeshModelDescWorld)->boundingBox.v3Oct,0xff00ff00);
		
		}
	}	
	bResult = TRUE;

lb_return:
	return bResult;
}
void __stdcall CoStaticModel::EnableRecvShadow(BOOL bEnable)
{
	m_bRecvShadowEnable = bEnable;
}
BOOL CoStaticModel::CommitIDIMeshObject(CMeshObject* pObj)
{
	

	BOOL		bResult = FALSE;
	
	CMeshFlag		flag;

	if (pObj->GetDeviceObject())
		goto lb_return;

	
	flag = ((CMeshObject*)pObj)->GetMeshFlag();
	flag.SetRigidType(RIGID_TYPE_RIGID);
	
	IDIMeshObject*	pDevObj;
	pDevObj = m_pRenderer->CreateMeshObject(flag);

	pObj->CommitDevice(pDevObj,m_pMaterialList);
	pObj->m_sortLink.pItem = (void*)pObj;
	pDevObj->Update(0);

	LinkToSortLink(&m_pDevAllocObjHead,&m_pDevAllocObjTail,&pObj->m_sortLink);
	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL CoStaticModel::DecommitIDIMeshObject(CMeshObject* pObj)
{
	pObj->ReleaseIDIMeshObject();
	UnLinkFromSortLink(&m_pDevAllocObjHead,&m_pDevAllocObjTail,&pObj->m_sortLink);
	return TRUE;
}
BOOL CoStaticModel::CommitAllIDIMeshObjectsInViewVolume()
{
	CMeshObject*	pObj;
	DWORD	dwNum = ITGetItemNum(m_pIndexItemTableMeshObject);
	for (DWORD i=0; i<dwNum; i++)
	{	
		pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,i);
		if (pObj->GetDeviceObject())
			continue;

		if (!pObj->IsIncludeViewVolume())
			continue;

		CommitIDIMeshObject(pObj);
	}

	return TRUE;
}
BOOL CoStaticModel::CommitAllIDIMeshObjectsInViewSphere()
{
	CMeshObject*	pObj;
	DWORD	dwNum = ITGetItemNum(m_pIndexItemTableMeshObject);
	for (DWORD i=0; i<dwNum; i++)
	{	
		pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,i);
		if (pObj->GetDeviceObject())
			continue;

		if (!pObj->IsIncludeViewSphere())
			continue;
		
lb_commit:
		CommitIDIMeshObject(pObj);
	}

	return TRUE;
}
void CoStaticModel::OnFaultObject(CMeshObject* pObj)
{
	char	curPath[_MAX_PATH];
	GetCurrentDirectory(sizeof(curPath),curPath);

	SetCurrentDirectory(m_szSTMPath);
	if (!m_pDevAllocObjHead)
	{
	//	CommitAllIDIMeshObjectsInViewVolume();
		CommitAllIDIMeshObjectsInViewSphere();
	}
	else
	{
		CommitIDIMeshObject(pObj);
	}
	SetCurrentDirectory(curPath);
}
BOOL __stdcall CoStaticModel::RenderCollisionStaticModel(DWORD dwFlag)
{
	BOOL	bResult = FALSE;

	if (!m_pCollisionStaticModel)
		goto lb_return;

	bResult = m_pCollisionStaticModel->Render(m_pRenderer,dwFlag);

lb_return:
	return bResult;
}
DWORD __stdcall CoStaticModel::CreateIVertexList(IVERTEX** ppVertex)
{
	// 리턴값은 페이스 갯수

	if (!ITGetItemNum(m_pIndexItemTableMeshObject))
		return 0;

	DWORD			dwResult;
	DWORD			dwTotalVertexCount = 0;
	DWORD			dwTotalFacesNum = 0;
	DWORD			dwViewAlignedFacesNum = 0;
	DWORD			i;
	CMeshObject*	pObj;

	for (i=0; i<ITGetItemNum(m_pIndexItemTableMeshObject); i++)
	{
		pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,i);
		dwTotalFacesNum += pObj->GetTotalFacesNum();
	}
	IVERTEX*		pvSrc;
	IVERTEX*		pv = new IVERTEX[dwTotalFacesNum*3];
	IVERTEX*		pEntry = pv;

	for (i=0; i<ITGetItemNum(m_pIndexItemTableMeshObject); i++)
	{
		pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,i);
		if (pObj->GetMeshFlag().GetTransformType() == TRANSFORM_TYPE_ALIGN_VIEW)
		{
			dwViewAlignedFacesNum += pObj->GetTotalFacesNum();
		}
		else 
		{
			dwResult = pObj->CreateIVertexList(&pvSrc,NULL);
			if (dwResult)
			{
				dwTotalVertexCount += dwResult;
				memcpy(pEntry,pvSrc,sizeof(IVERTEX)*dwResult);
				pObj->ReleaseIVertexList(pvSrc);
				pEntry += dwResult;
			}
		}
	}
	*ppVertex = pv;
	
	return dwTotalVertexCount/3;
	
}
DWORD __stdcall	CoStaticModel::CreateIVertexListWithIndex(IVERTEX** ppVertex,DWORD dwObjIndex)
{	
	// 리턴값은 페이스 갯수
	DWORD		dwResult = 0;
	CMeshObject*	pMeshObject;

	pMeshObject = (CMeshObject*)ITGetItem(m_pIndexItemTableMeshObject,dwObjIndex);
	
	if (!pMeshObject)
		goto lb_return;

	if (pMeshObject->GetMeshFlag().GetTransformType() == TRANSFORM_TYPE_ALIGN_VIEW)
		goto lb_return;
	
	dwResult = pMeshObject->CreateIVertexList(ppVertex,NULL);

lb_return:
	return dwResult/3;
}
void __stdcall CoStaticModel::ReleaseIVertexList(IVERTEX*	pVertex)
{
	delete [] pVertex;

}
DWORD __stdcall CoStaticModel::GetObjectIndex(char* szObjName)
{
	DWORD		dwIndex = 0xffffffff;

	CBaseObject* pObj = GetObject(szObjName);
	if (!pObj)
		goto lb_return;

	dwIndex = pObj->GetIndex();

lb_return:
	return dwIndex;
}
void __stdcall CoStaticModel::Optimize()
{
	CMeshObject*	pObj;
	DWORD			i;

	if (m_bOptimizedFlag)
		goto lb_return;

	for (i=0; i<ITGetItemNum(m_pIndexItemTableMeshObject); i++)
	{
		pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,i);

		pObj->ReleaseLightTexturePatches();
	//	pObj->Optimize();
	}
	
	m_bOptimizedFlag = TRUE;
	
lb_return:
	return;

}
DWORD __stdcall CoStaticModel::GetObjectNum()
{
	return ITGetItemNum(m_pIndexItemTableMeshObject);
}
DWORD __stdcall CoStaticModel::GetObjectIndex(DWORD dwSeqIndex)
{
	CMeshObject*	pObj;
	DWORD	dwIndex = 0xffffffff;
	
	DWORD	dwNum = ITGetItemNum(m_pIndexItemTableMeshObject);

	if (dwSeqIndex >= dwNum)
		goto lb_return;

	pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,dwSeqIndex);

	dwIndex = pObj->GetIndex();

lb_return:
	return dwIndex;
}

BOOL __stdcall CoStaticModel::BeignShadeLightMapObject(DWORD dwColor)
{
	CMeshObject*	pObj;
	DWORD			i;

	m_dwAmbientColor = dwColor;

	for (i=0; i<ITGetItemNum(m_pIndexItemTableMeshObject); i++)
	{
		pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,i);
		
		if (pObj->GetLightTexture())
		{
			pObj->GetLightTexture()->BeginCreateLightTexture(dwColor,0);
		}
		if (pObj->GetVertexColor())
		{
			pObj->BeginShadeVertexColor(dwColor);
		}

	}
	return TRUE;
}
BOOL __stdcall CoStaticModel::ShadeLightMapObject(DWORD dwObjIndex,VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pDesc,SHADE_FUNC pFunc)
{
	BOOL		bResult = FALSE;

	CMeshObject*	pMeshObject;

	if (dwObjIndex >= ITGetItemNum(m_pIndexItemTableMeshObject))
		goto lb_return;

	pMeshObject = (CMeshObject*)ITGetItem(m_pIndexItemTableMeshObject,dwObjIndex);

	if (pMeshObject->GetLightTexture())
	{
		pMeshObject->GetLightTexture()->ShadeLightTextureAdd(pv3,dwFacesNum,pDesc,pFunc);
	}
	if (pMeshObject->GetVertexColor())
	{
		pMeshObject->ShadeVertexColor(pv3,dwFacesNum,pDesc,pFunc);
	}

	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CoStaticModel::GetLightTexturePatch(CMeshObject** ppMeshObj,CLightTexture** ppLightTexture,TEXTURE_PLANE** ppTexturePlane,PATCH** ppPatch,POINT* ptCursor,DWORD dwObjIndex,VECTOR3* pv3IntersectPoint,DWORD dwViewportIndex)
{
	VECTOR3		v3Pos;
	VECTOR3		v3Dir;

	BOOL			bResult = FALSE;
	MATRIX4			matProj,matView;
	CMeshObject*	pObj;
	CLightTexture*	pLightTexture;
	TEXTURE_PLANE*	pTexturePlane;


	TEXTURE_PLANE*	pResultTexturePlane;

	VECTOR3			v3Tri[6];
	float			fMinDist = 900000.0f;
	VECTOR3			v3NearIntersectPoint;

	pObj = (CMeshObject*)ITGetItem(m_pIndexItemTableMeshObject,dwObjIndex);
	pLightTexture = pObj->GetLightTexture();
	if (!pLightTexture)
		goto lb_return;

	VIEWPORT*	pVP;
	pVP = m_pGeometry->INL_GetViewport(dwViewportIndex);
	
	m_pGeometry->GetProjectionMatrix(&matProj,dwViewportIndex);
	m_pGeometry->GetViewMatrix(&matView,dwViewportIndex);
	
	POINT	p;
	p.x = ptCursor->x - pVP->rcClip.left;
	p.y = ptCursor->y - pVP->rcClip.top;

	if (p.x < 0 || p.y < 0)
		goto lb_return;

	
	if (ptCursor->x > pVP->rcClip.right || ptCursor->y > pVP->rcClip.bottom)
		goto lb_return;


	CalcRay(&v3Pos,&v3Dir,p.x,p.y,(DWORD)pVP->wClipWidth,(DWORD)pVP->wClipHeight,&matProj,&matView);


	pTexturePlane = pLightTexture->GetTexturePlanePtr();



	DWORD i,j;

	for (i=0; i<pLightTexture->GetTexturePlaneNum(); i++)
	{
		v3Tri[0] = pTexturePlane[i].v3Rect[0];
		v3Tri[1] = pTexturePlane[i].v3Rect[1];
		v3Tri[2] = pTexturePlane[i].v3Rect[2];
		
		v3Tri[3] = pTexturePlane[i].v3Rect[0];
		v3Tri[4] = pTexturePlane[i].v3Rect[2];
		v3Tri[5] = pTexturePlane[i].v3Rect[3];	
		
		float	dp;
		dp = DotProduct(&pTexturePlane[i].plane.v3Up,pv3IntersectPoint) + pTexturePlane[i].plane.D;
		if (dp > ONE_CM || dp < -1.0f * ONE_CM)
			continue;

		for (j=0; j<2; j++)
		{

			VECTOR3		v3IntersectPoint;
			float		fDist,u,v;
			if (IntersectTriangle(&v3IntersectPoint,&v3Pos,&v3Dir,&v3Tri[j*3+0],&v3Tri[j*3+1],&v3Tri[j*3+2],&fDist,&u,&v,FALSE))
			{
				if (fDist < fMinDist)
				{
					fMinDist = fDist;
					v3NearIntersectPoint = v3IntersectPoint;
					bResult = TRUE;
					pResultTexturePlane = pTexturePlane+i;
				}
			}
		}
	}
	if (!bResult)
		goto lb_return;

	VECTOR3		duN,dvN,vp;
	float		fDistU,fDistV,fLenU,fUnitU,fLenV,fUnitV;
	VECTOR3_SUB_VECTOR3(&vp,&v3NearIntersectPoint,&pResultTexturePlane->v3Rect[0]);

	VECTOR3_SUB_VECTOR3(&duN,&pResultTexturePlane->v3Rect[3],&pResultTexturePlane->v3Rect[0]);
	fLenU = VECTOR3Length(&duN);
	fUnitU = fLenU / (float)(pResultTexturePlane->dwTexelSize-2);

	Normalize(&duN,&duN);
	fDistU = DotProduct(&duN,&vp);
	
	VECTOR3_SUB_VECTOR3(&dvN,&pResultTexturePlane->v3Rect[1],&pResultTexturePlane->v3Rect[0]);
	fLenV = VECTOR3Length(&dvN);
	fUnitV = fLenV / (float)(pResultTexturePlane->dwTexelSize-2);

	Normalize(&dvN,&dvN);
	fDistV = DotProduct(&dvN,&vp);

	WORD	wPosX,wPosY;
	wPosX = (WORD)( fDistU / fUnitU ) + 1;
	wPosY = (WORD)( fDistV / fUnitV ) + 1;

	DWORD	dwIndex;
	dwIndex = wPosX + pResultTexturePlane->dwTexelSize*wPosY;
	if (dwIndex >= pResultTexturePlane->dwTexelSize*pResultTexturePlane->dwTexelSize)
		goto lb_return;

	*ppTexturePlane = pResultTexturePlane;
	*ppPatch = pResultTexturePlane->pPatch+dwIndex;
	*ppLightTexture = pLightTexture;
	*ppMeshObj = pObj;

	bResult = TRUE;

lb_return:
	return bResult;

}

BOOL __stdcall CoStaticModel::SetLightTexel(POINT* ptCursor,DWORD dwObjIndex,VECTOR3* pv3IntersectPoint,DWORD dwColor,DWORD dwViewportIndex)
{
	TEXTURE_PLANE*	pTexturePlane;
	PATCH*			pPatch;
	CLightTexture*	pLightTexture;
	CMeshObject*	pMeshObj;
	BOOL			bResult = FALSE;
	

	if (!GetLightTexturePatch(&pMeshObj,&pLightTexture,&pTexturePlane,&pPatch,ptCursor,dwObjIndex,pv3IntersectPoint,dwViewportIndex))
		goto lb_return;

	pPatch->dwColor = dwColor;
	pLightTexture->UpdatePatch(pPatch);
	pMeshObj->GetDeviceObject()->Update(0);

	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CoStaticModel::GetLightTexel(DWORD* pdwColor,POINT* ptCursor,DWORD dwObjIndex,VECTOR3* pv3IntersectPoint,DWORD dwViewportIndex)
{
	TEXTURE_PLANE*	pTexturePlane;
	PATCH*			pPatch;
	CLightTexture*	pLightTexture;
	CMeshObject*	pMeshObj;
	BOOL			bResult = FALSE;
	

	if (!GetLightTexturePatch(&pMeshObj,&pLightTexture,&pTexturePlane,&pPatch,ptCursor,dwObjIndex,pv3IntersectPoint,dwViewportIndex))
		goto lb_return;

	*pdwColor = pPatch->dwColor;
	bResult = TRUE;

lb_return:
	return bResult;
}
void __stdcall CoStaticModel::EndShadeLightMapObject()
{
	CMeshObject*	pObj;
	FILE*			fpLM;
	FILE*			fpVCL;

	DWORD			i;

	memset(m_szTempLMFileName,0,MAX_NAME_LEN);
	lstrcpy(m_szTempLMFileName,"stlightmap");
	lstrcat(m_szTempLMFileName,".tmp");
	
	fpLM = fopen(m_szTempLMFileName,"wb");

	memset(m_szTempVCLFileName,0,MAX_NAME_LEN);
	lstrcpy(m_szTempVCLFileName,"stvertexcolor");
	lstrcat(m_szTempVCLFileName,".vcl");

	fpVCL = fopen(m_szTempVCLFileName,"wb");
	
	for (i=0; i<ITGetItemNum(m_pIndexItemTableMeshObject); i++)
	{
		pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,i);
	
		if (pObj->GetLightTexture())
		{
			pObj->GetLightTexture()->EndCreateLightTexture(NULL,fpLM);
		}
		if (pObj->GetVertexColor())
		{
			pObj->EndShadeVertexColor(fpVCL);
		}
		if (pObj->GetDeviceObject())
			pObj->GetDeviceObject()->Update(0);
	}
	fclose(fpLM);
	fclose(fpVCL);
}

BOOL __stdcall CoStaticModel::WriteFile(char* szFileName)
{
	BOOL		bResult = FALSE;

	CMeshObject*	pObj;
	DWORD			i,dwObjNum;
	char			szNewLMFileName[MAX_NAME_LEN];


	


	FILE* fp = fopen(szFileName,"wb");
	
	if (!fp)
		goto lb_return;

	DWORD	dwVersion;
	dwVersion = STATIC_MODEL_VERSION;
	fwrite(&dwVersion,sizeof(DWORD),1,fp);
		
	m_pMaterialList->WriteFile(fp);
	dwObjNum = ITGetItemNum(m_pIndexItemTableMeshObject);

	fwrite(&dwObjNum,sizeof(DWORD),1,fp);

	for (i=0; i<ITGetItemNum(m_pIndexItemTableMeshObject); i++)
	{
		pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,i);
		pObj->WriteFile(fp);
	}

	WriteCollisionMesh(fp);
	
	DWORD	dwPropertyNum;
	dwPropertyNum = 0;
	fwrite(&dwPropertyNum,sizeof(DWORD),1,fp);

	fclose(fp);

	EndShadeLightMapObject();

	
	
	memset(szNewLMFileName,0,MAX_NAME_LEN);
	RemoveExt(szNewLMFileName,szFileName);
	lstrcat(szNewLMFileName,".lm");
	remove(szNewLMFileName);
	rename(m_szTempLMFileName,szNewLMFileName);

	memset(szNewLMFileName,0,MAX_NAME_LEN);
	RemoveExt(szNewLMFileName,szFileName);
	lstrcat(szNewLMFileName,".vcl");
	remove(szNewLMFileName);
	rename(m_szTempVCLFileName,szNewLMFileName);

	

	bResult = TRUE;

lb_return:
	return bResult;
}
DWORD CoStaticModel::WriteCollisionMesh(FILE* fp)
{
	DWORD	oldPos = ftell(fp);

	fwrite(&m_dwColMeshSize,sizeof(DWORD),1,fp);
	fwrite(m_pColMeshModelDescWorld,sizeof(char),m_dwColMeshSize,fp);

	return (ftell(fp)-oldPos);

}
DWORD CoStaticModel::ReadCollisionMesh(void* pFP)
{
	DWORD	dwLen = 0;

	dwLen += g_pFileStorage->FSRead(pFP,&m_dwColMeshSize,sizeof(DWORD));

	m_pColMeshModelDescWorld = (COLLISION_MODEL_DESC*)new char[m_dwColMeshSize];

	dwLen += g_pFileStorage->FSRead(pFP,m_pColMeshModelDescWorld,sizeof(char)*m_dwColMeshSize);

	
	COLLISION_MESH_OBJECT_DESC*	pColMesh;

	for (DWORD i=0; i<ITGetItemNum(m_pIndexItemTableMeshObject); i++)
	{
		// 단지 오프셋 세팅..
		CMeshObject* pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,i);


		if (pObj->GetMeshFlag().GetTransformType() == TRANSFORM_TYPE_ALIGN_VIEW)
			pColMesh = m_pColMeshModelDescWorld->colMeshObjectDesc+i;
		else
			pColMesh = NULL;

		pObj->CreateBoundingMesh(
			m_pColMeshModelDescWorld,
			m_pColMeshModelDescWorld->colMeshObjectDesc+i,
			pColMesh);
				
	}
	return dwLen;
}


BOOL __stdcall CoStaticModel::ReadFile(char* szFileName,LOAD_CALLBACK_DESC* pCallbackDesc,DWORD dwFlag)
{
	BOOL		bResult = FALSE;

	CMeshObject*	pMeshObj;
	char			szRCFileName[MAX_NAME_LEN];
	
	DWORD		i;
	DWORD		dwIndex;
	DWORD	dwObjectNum;

	void*	pFPLM;
	void*	pFPVCL;

	LOAD_CALLBACK_FUNC	pFunc = NULL;

	void* pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_BINARY);
	if (!pFP)
	{
		// FILE_NOT_FOUND /////////////////////////////////////////////////////////
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_FILE_NOT_FOUND,1,(void*)dwAddr,szFileName);
		///////////////////////////////////////////////////////////////////////////
		goto lb_return;
	}

	DWORD	dwVersion;

	g_pFileStorage->FSRead(pFP,&dwVersion,sizeof(DWORD));

#ifdef _DEBUG
	if (!dwVersion || dwVersion > 0x00000010)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"CoModel::ReadFile(), if (!dwVersion || dwVersion > 0x00000010), File:%s , Line:%d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif
	if (pCallbackDesc)
	{
		pFunc = pCallbackDesc->pFunc;

	}


	m_pMaterialList->ReadFile(pFP);
	if (pFunc)
	{
		pFunc(*pCallbackDesc->pdwCurCount,pCallbackDesc->dwTotalCount,NULL);
		(*pCallbackDesc->pdwCurCount)++;
	}

	g_pFileStorage->FSRead(pFP,&dwObjectNum,sizeof(DWORD));
	
	
	memset(szRCFileName,0,MAX_NAME_LEN);
	RemoveExt(szRCFileName,szFileName);
	lstrcat(szRCFileName,".lm");


	pFPLM = g_pFileStorage->FSOpenFile(szRCFileName,FSFILE_ACCESSMODE_BINARY);



	memset(szRCFileName,0,MAX_NAME_LEN);
	RemoveExt(szRCFileName,szFileName);
	lstrcat(szRCFileName,".vcl");
	
	pFPVCL = g_pFileStorage->FSOpenFile(szRCFileName,FSFILE_ACCESSMODE_BINARY);

	DWORD	dwLightTextureCreateFlag;
	if (dwFlag & STATIC_MODEL_LOAD_ENABLE_SHADE)
		dwLightTextureCreateFlag = 0;
	else 
		dwLightTextureCreateFlag = LIGHTTEXTURE_CREATE_TYPE_NOT_CREATE_PATCH;


	for (i=0; i<dwObjectNum; i++)
	{
		pMeshObj = AllocObject();
		dwIndex = pMeshObj->GetIndex();


		if (pFunc)
		{
			pFunc(*pCallbackDesc->pdwCurCount,pCallbackDesc->dwTotalCount,NULL);
			(*pCallbackDesc->pdwCurCount)++;
		}
		pMeshObj->ReadFile(pFP,dwVersion);

		if (pMeshObj->GetLightTexture())
		{

			pMeshObj->GetLightTexture()->CreateLightMapImage(pFPLM,dwLightTextureCreateFlag);

		}

		if (pMeshObj->GetMeshFlag().GetShadeType() != SHADE_TYPE_LIGHT_MAP
			&& pMeshObj->GetMeshFlag().GetTransformType() != TRANSFORM_TYPE_ALIGN_VIEW)
			pMeshObj->ReadVertexColor(pFPVCL);
		
		pMeshObj->SetIndex(dwIndex);
	}

	ReadCollisionMesh(pFP);

	if (pFPVCL)
		g_pFileStorage->FSCloseFile(pFPVCL);

	
	if (pFPLM)
		g_pFileStorage->FSCloseFile(pFPLM);



	InitializeObjects();

	if (!(dwFlag & STATIC_MODEL_LOAD_ENABLE_SHADE))
	{
		Optimize();
	}

	char		szColSTMFileName[_MAX_PATH];
	RemoveExt(szColSTMFileName,szFileName);

	m_pCollisionStaticModel = new CCollisionStaticModel;
	lstrcat(szColSTMFileName,".col");
	if (!m_pCollisionStaticModel->ReadFile(szColSTMFileName))
	{
		delete m_pCollisionStaticModel;
		m_pCollisionStaticModel = NULL;

	}

	bResult = TRUE;


	DWORD	dwPathLen;
	memset(m_szSTMPath,0,sizeof(m_szSTMPath));
	dwPathLen = GetNamePath(m_szSTMPath,szFileName);
	if (!dwPathLen)
		GetCurrentDirectory(sizeof(m_szSTMPath),m_szSTMPath);

	
	lstrcpy(m_szFileName,szFileName);

lb_closefile:
	g_pFileStorage->FSCloseFile(pFP);


lb_return:
	return bResult;
}
CMeshObject* CoStaticModel::GetObject(char* szObjName)
{
	CMeshObject* pResult = NULL;
	CMeshObject*	pObj;
	DWORD			i;

	for (i=0; i<ITGetItemNum(m_pIndexItemTableMeshObject); i++)
	{
		pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,i);
		if (!lstrcmp(szObjName,pObj->GetObjectName()))
		{
			pResult = pObj;
			goto lb_return;
		}
	}
lb_return:
	return pResult;

}

DWORD __stdcall	CoStaticModel::GetMeshObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
/*	DWORD		i;
	float		fDist;
	VECTOR3		v3IntersectPoint;
	DWORD		dwFaceGroupIndex;

	INDEX_COL_MESH_STATIC	result;
	result.dwIndex	=	0xffffffff;
	result.fDist	=	900000.0f;
	result.dwFaceGroupIndex = 0xffffffff;

	*pdwFaceGroupIndex = 0xffffffff;
	
	DWORD dwNum = ITGetItemNum(m_pIndexItemTableMeshObject);

	for (i=0; i<dwNum; i++)
	{
		if (IsCollisionMeshObject(&v3IntersectPoint,&fDist,&dwFaceGroupIndex,i,dwPosX,dwPosY,dwFlag))
		{
			if (fDist <= result.fDist)
			{
				result.fDist = fDist;
				result.dwIndex = i;
				result.dwFaceGroupIndex = dwFaceGroupIndex;
				result.v3IntersectPoint = v3IntersectPoint;
			}
		}
	}
lb_return:
	if (result.dwIndex != 0xffffffff)
	{
		*pfDist = result.fDist;
		*pdwFaceGroupIndex = result.dwFaceGroupIndex;
		*pv3IntersectPoint = result.v3IntersectPoint;
	}	
	return result.dwIndex;*/
	return 0xffffffff;
}
BOOL __stdcall	CoStaticModel::IsCollisionMeshObject(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,DWORD dwObjIndex,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
	VECTOR3		v3Pos;
	VECTOR3		v3Dir;

	MATRIX4			matProj,matView;
	CMeshObject*	pObj;
	COLLISION_MESH_OBJECT_DESC* pColMeshDesc;
	

	
	VECTOR3		v3IntersectPoint;
	BOOL		bResult = FALSE;

	VIEWPORT*	pVP = m_pGeometry->INL_GetViewport(dwViewportIndex);

	m_pGeometry->GetProjectionMatrix(&matProj,dwViewportIndex);
	m_pGeometry->GetViewMatrix(&matView,dwViewportIndex);
	
	POINT	p;
	p.x = ptCursor->x - pVP->rcClip.left;
	p.y = ptCursor->y - pVP->rcClip.top;

	if (p.x < 0 || p.y < 0)
		goto lb_return;

	
	if (ptCursor->x > pVP->rcClip.right || ptCursor->y > pVP->rcClip.bottom)
		goto lb_return;


	CalcRay(&v3Pos,&v3Dir,p.x,p.y,(DWORD)pVP->wClipWidth,(DWORD)pVP->wClipHeight,&matProj,&matView);
	
	pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,dwObjIndex);
	if (pObj->GetMeshFlag().GetTransformType() == TRANSFORM_TYPE_ALIGN_VIEW)
		goto lb_return;

	pColMeshDesc = GetCollisionMeshObjectDesc(dwObjIndex);
	
	if (IsCollisionMeshAndRay(&v3IntersectPoint,pfDist,pColMeshDesc,&v3Pos,&v3Dir))
	{
		if (PICK_TYPE_PER_FACE == dwFlag )
		{
			if (!pObj->IsCollisionMeshAndRay(&v3IntersectPoint,pfDist,pdwFaceGroupIndex,&v3Pos,&v3Dir,NULL))
				goto lb_return;

		}
	}
	else 
		goto lb_return;

	bResult = TRUE;
	*pv3IntersectPoint = v3IntersectPoint;
	

lb_return:
	return bResult;
}
void CoStaticModel::ReleaseObjects()
{
	CMeshObject*	pObj;
	DWORD			i;

	//주의할 필요가 있다.인덱스테이블에서 아이템을 하나씩 삭제할때마다 아이템의 순서가 바뀌고 갯수도 줄어든다.
	DWORD dwNum = ITGetItemNum(m_pIndexItemTableMeshObject);
	for (i=0; i<dwNum; i++)
	{
		pObj = (CMeshObject*)ITGetItemSequential(m_pIndexItemTableMeshObject,0);
		if (pObj)
		{
			ITDeleteItem(m_pIndexItemTableMeshObject,pObj->GetIndex());
			delete pObj;
		}
	}
}
void CoStaticModel::Clean()
{
	if (m_pCollisionStaticModel)
	{
		delete m_pCollisionStaticModel;
		m_pCollisionStaticModel = NULL;
	}
	m_dwColMeshSize = 0;
	if (m_pMaterialList)
	{
		delete m_pMaterialList;
		m_pMaterialList = NULL;
	}
}
void CoStaticModel::CleanColMesh()
{
	if (m_pDefaultMatrixList)
	{
		delete [] m_pDefaultMatrixList;
		m_pDefaultMatrixList = NULL;
	}
	if (m_pColMeshModelDescWorld)
	{
		delete [] m_pColMeshModelDescWorld;
		m_pColMeshModelDescWorld = NULL;
	}
}



BOOL __stdcall CoStaticModel::RunManageResource(DWORD dwViewportIndex)
{
	BOOL	bResult = FALSE;

	VECTOR3	v3CameraPos;
	float	fFar;
	
	v3CameraPos = *m_pGeometry->GetCameraEye(dwViewportIndex);
	fFar = m_pGeometry->GetCameraFar(dwViewportIndex);
	

	float	fDist;
	fDist = CalcDistance(&v3CameraPos,&m_v3PrvCameraPos);

	if (fDist < DEFAULT_RESOURCE_SCHDULE_DISTANCE)
		goto lb_return;

	m_v3PrvCameraPos = v3CameraPos;

	SORT_LINK*	pCur;
	SORT_LINK*	pNext;

	pCur = m_pDevAllocObjHead;

	CMeshObject* pObj;
	
	while (pCur)
	{
#ifdef	_DEBUG
						// 이진영 추가 2004/03/39.
		if( pCur->pPrv)
		{
			if( pCur->pPrv->pNext != pCur)
				__asm int 3;
		}
#endif
		pNext = pCur->pNext;
		pObj = (CMeshObject*)pCur->pItem;
		if (!pObj->IsIncludeViewSphere())
			DecommitIDIMeshObject(pObj);
			/*


		pSphere = &pObj->GetColMeshObject(m_pColMeshModelDescWorld)->boundingSphere;
		fDist = CalcDistance(&pSphere->v3Point,&v3CameraPos);
		

		if (fDist > fFar + pSphere->fRs)
			DecommitIDIMeshObject(pObj);
*/
		pCur = pNext;
	}

	bResult = TRUE;
lb_return:
	return bResult;

}
CoStaticModel::~CoStaticModel()
{


	CleanColMesh();
	Clean();
	ReleaseObjects();
	ITRelease(m_pIndexItemTableMeshObject);

	if (m_pGeometry)
	{
		if (m_dwIndexInGeometry)
			m_pGeometry->DeleteFileItem(m_dwIndexInGeometry);
	}
	

}

// 주어진 AA타원체를 To로 밀어서 결과를 리턴한다.
BOOL CoStaticModel::CollisionTestAAELLIPSOID( VECTOR3* pOut, VECTOR3* pOutLastVelocity, BOOL* pbOutLanding, CollisionTestCallBackProcedure CallBack, AAELLIPSOID* pEllipsoid, VECTOR3* pTo, DWORD dwOutputBufferIndex)
{
#ifdef	_DEBUG
	if( m_pCollisionStaticModel == 0)
	{	
		PrintfDebugString("CoStaticModel::CollisionTestAAELLIPSOID() CCollisionStaticModel이 없다. 이걸 쓸려면 .col이 제대로 로드되었는지 확인요망. (There are no CCollisionStaticModel. did you make .col?)\n");
		__asm int 3;
		return	FALSE;
	}
#endif

	return	m_pCollisionStaticModel->CollisionTestAAELLIPSOID( pOut, pOutLastVelocity, pbOutLanding, CallBack, pEllipsoid, pTo, dwOutputBufferIndex);
}

BOOL CoStaticModel::CollisionTestLine( float* pOutT, VECTOR3* pFrom, VECTOR3* pTo, DWORD dwOutputBufferIndex)
{
#ifdef	_DEBUG
	if( m_pCollisionStaticModel == 0)
	{	
		PrintfDebugString("CoStaticModel::CollisionTestAAELLIPSOID() CCollisionStaticModel이 없다. 이걸 쓸려면 .col이 제대로 로드되었는지 확인요망. (There are no CCollisionStaticModel. did you make .col?)\n");
		__asm int 3;
		return	FALSE;
	}
#endif

	return	m_pCollisionStaticModel->CollisionTestLine( pOutT, pFrom, pTo, dwOutputBufferIndex);
}

// 동시에 호출할 갯수. 아마 쓰레드만큼.?
void CoStaticModel::SetSimultaneousAccessCount( DWORD dwCount)
{
	m_pCollisionStaticModel->SetSimultaneousAccessCount( dwCount);
}
