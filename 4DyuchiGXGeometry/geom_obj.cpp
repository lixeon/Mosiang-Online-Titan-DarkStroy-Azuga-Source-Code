#include "MaterialList.h"
#include "geom_obj.h"
#include "../4DyuchiGXGFunc/global.h"
#include "../4DyuchiGRX_common/IGeometry.h"
#include "model.h"
#include "geometry_global.h"
#include "CoGeometry.h"
#include "global_variable.h"


CMeshObject::CMeshObject()
{
//	memset((char*)this+sizeof(CBaseObject),0,sizeof(CMeshObject)-sizeof(CBaseObject));
	m_objType = OBJECT_TYPE_MESH;
	m_dwMaxVertexNum = 0;
	m_dwVertexNum = 0;
	m_dwOriginalVertexNum = 0;
	m_dwExtVertexNum = 0;
	m_dwTexVertexNum = 0;
	m_dwMtlIndex = 0;
	m_dwFaceGroupNum = 0;
	m_dwReserved = 0;

	m_sortLink.pPrv = NULL;
	m_sortLink.pItem = NULL;
	m_sortLink.pNext = NULL;
	
	m_pv3World = NULL;
	m_pv3Local = NULL;
	m_ptUV = NULL;
	m_pv3NormalWorld = NULL;
	m_pv3NormalLocal = NULL;
	m_pVertexColor = NULL;
	m_pv3TangentULocal = NULL;
	m_pLightTexture = NULL;
	m_pFaceGroup = NULL;
	m_dwTotalFacesNum = NULL;
	m_pdwExtVertexIndex = NULL;
	m_pDeviceMeshObj = NULL;
	memset(&m_mtlDefault,0,sizeof(m_mtlDefault));
	memset(m_v3Rect,0,sizeof(m_v3Rect));
	m_fLengthX = 0.0f;
	m_fLengthY = 0.0f;

	m_pColMeshTemp = NULL;
	m_bShaded = FALSE;
	m_dwColMeshObjectDescOffset = 0;	
	m_pColMeshModelDescWorld = NULL;
	
	
}

BOOL __stdcall CMeshObject::IsIncludeViewVolume()
{
	BOOL	bResult = FALSE;
	if (m_pModel)
	{
		// 모델에 속한 오브젝트라면 일단 뷰에 들어가는 것으로 간주하자.
		// 함부로 메모리에서 내려버리면 곤란하다.
		bResult = TRUE;
		goto lb_return;
	}


	COLLISION_MESH_OBJECT_DESC*	pColMeshDesc;
	pColMeshDesc = GetColMeshObject(m_pColMeshModelDescWorld);
		
	VIEW_VOLUME*	pVV;
	pVV = m_pGeometry->INL_GetViewvolume();
	
	// 오브젝트가 뷰볼륨안에 들어가지 않는 경우 클립
	if (ClipRenderObjectWithViewVolume(pVV,pColMeshDesc,0))
		bResult = TRUE;
	
lb_return:
	return bResult;
}

BOOL __stdcall CMeshObject::IsIncludeViewSphere()
{
	BOOL	bResult = TRUE;
	if (m_pModel)
	{
		// 모델에 속한 오브젝트라면 일단 뷰에 들어가는 것으로 간주하자.
		// 함부로 메모리에서 내려버리면 곤란하다.
		goto lb_return;
	}

	VECTOR3	v3CameraPos;
	float	fFar;
	float	fRs;
		

	
	v3CameraPos = *m_pGeometry->INL_GetCameraEye();
	fRs = m_pGeometry->INL_GetRadiusForLoadResource();
	fFar = m_pGeometry->INL_GetCameraFar();
	if (fRs < fFar)
	{
		fRs = fFar;
	}	
	
	COLLISION_MESH_OBJECT_DESC*	pColMeshDesc;
	pColMeshDesc = GetColMeshObject(m_pColMeshModelDescWorld);

	float	fDist;
	fDist = CalcDistance(&pColMeshDesc->boundingSphere.v3Point,&v3CameraPos);
		

	if (fDist > fRs + pColMeshDesc->boundingSphere.fRs)
		bResult = FALSE;
	
lb_return:
	return bResult;
}
BOOL __stdcall CMeshObject::OnFault()
{
	return FALSE;
}

BOOL __stdcall CMeshObject::OnOut()
{
	return TRUE;
}
BOOL __stdcall CMeshObject::ApplyView(MATRIX4* pView,DWORD dwRefIndex,DWORD dwFlag)
{
	BOOL		bResult = FALSE;

	if (!m_pModel)
		goto lb_return;

	if (!m_pModel->GetCameraMatrix(pView,dwRefIndex))
		goto lb_return;


	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CMeshObject::ApplyResultMatrix(MATRIX4** ppResult,DWORD dwRefIndex,DWORD dwFlag)
{
	MATRIX4*	pResult;


	if (m_pDefaultResultMatrix )	// 디폴트 매트릭스가 세팅되어있으면 모델이 아니다.
		pResult = *ppResult = m_pDefaultResultMatrix;
	else
	{
		CBaseObject::ApplyResultMatrix(ppResult,dwRefIndex,dwFlag);
		pResult = *ppResult;
	
//	pResult = *ppResult = m_pModel->GetObjectResultMatrix(dwRefIndex,GetIndex());
	}

	return TRUE;
}
BOOL __stdcall CMeshObject::ApplyMorph(VECTOR3** ppv3Local,TVERTEX** pptv,DWORD dwRefIndex,DWORD dwFlag)
{
	BOOL			bResult = FALSE;

	MODEL_STATUS*	pStatus;
	*pptv = NULL;
	*ppv3Local = NULL;
	CMotionObject*	pMot = NULL;
	


	if (!m_pModel)
		goto lb_return;


	// 스태틱 모델은 m_pModel이 NULL이다.모델에 대해서만 적용된다.
	if (!m_pModel->GetModelStatus(&pStatus,dwRefIndex))
		goto lb_return;


	pMot = m_pModel->GetMotionObject(m_szObjName,m_dwNameLen,dwRefIndex,pStatus->dwMotionIndex);
	
	if (!pMot)
		goto lb_return;
	
	if (!pMot->SetMorphFrame(ppv3Local,m_dwVertexNum,pptv,m_dwTexVertexNum,pStatus->dwFrame))
		goto lb_return;
		 

	bResult = TRUE;

lb_return:
	return bResult;

}
/*
BOOL __stdcall CMeshObject::ApplyVertexNormal(BYTE* pVertex,DWORD dwVertexStartOffset,DWORD dwVertexSize,DWORD dwRefIndex,DWORD dwFlag)
{
	VECTOR3	v3Normal;
	v3Normal.x = 0.0f;
	v3Normal.y = 0.0f;
	v3Normal.z = 0.0f;

	pVertex += dwVertexStartOffset;
	for (DWORD i=0; i<m_dwVertexNum; i++)
	{
		*(VECTOR3*)pVertex = m_pv3Normal[i];
		pVertex += dwVertexSize;
	}
	return TRUE;
}
*/
struct VLVERTEX
{
	float	x;
	float	y;
	float	z;

	float	nx;
	float	ny;
	float	nz;

	float	u;
	float	v;
};


BOOL __stdcall CMeshObject::ApplyWorldMatrixForPhysique(MATRIX4* pResultWorldMatrix,DWORD dwRefIndex,DWORD dwFlag)
{
	BOOL	bResult;

	SET_FRAME_ARGS*	pArgsStatus;
	
	bResult = m_pModel->GetModelSetFrameArgs(&pArgsStatus,dwRefIndex);
	if (!bResult)
		goto lb_return;

//	SetIdentityMatrix(pResultWorldMatrix);
	*pResultWorldMatrix = pArgsStatus->matWorldForPhysique;
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CMeshObject::ApplyPhysique(BYTE* pVertex,DWORD dwVertexSize,DWORD dwRefIndex,DWORD dwFlag)
{
	BOOL		bResult = TRUE;
	
	VLVERTEX*		pv = (VLVERTEX*)pVertex;

	MATRIX4*	pMatrixEntry;
	MATRIX4*	pMatrixEntryForPhysiqueLocal;

	if (!m_pModel->GetMatrixEntry(&pMatrixEntry,&pMatrixEntryForPhysiqueLocal,dwRefIndex))
		goto lb_return;
/*
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	DWORD		dwObjNum;
	dwObjNum = m_pModel->GetObjectNum();
	
	MATRIX4			matWorld;
	MATRIX4			matWorldInverse;
	ApplyWorldMatrixForPhysique(&matWorld,dwRefIndex,dwFlag);

	SetInverseMatrix(&matWorldInverse,&matWorld);
	
	DWORD	i;
	for (i=0; i<dwObjNum; i++)
	{
		MatrixMultiply2(pMatrixEntryForPhysiqueLocal+i,pMatrixEntry+i,&matWorldInverse);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
	if (dwFlag == APPLY_PHYSIQUE_RECALC_NORMAL)
	{
		m_Physique.PhysiqueTransformPosAndNormal(pVertex,dwVertexSize,pMatrixEntryForPhysiqueLocal);
	}
	else if (dwFlag == (APPLY_PHYSIQUE_WRITE_UV | APPLY_PHYSIQUE_RECALC_NORMAL))
	{
		m_Physique.PhysiqueTransformPosAndNormalAndUV(pVertex,m_ptUV,dwVertexSize,pMatrixEntryForPhysiqueLocal);
	}
	else if (dwFlag == APPLY_PHYSIQUE_WRITE_UV)
	{
		m_Physique.PhysiqueTransformPosAndWriteUV(pVertex,m_ptUV,dwVertexSize,pMatrixEntryForPhysiqueLocal);
	}
	else
		m_Physique.PhysiqueTransform(pVertex,dwVertexSize,pMatrixEntryForPhysiqueLocal);

	
	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL __stdcall CMeshObject::ApplyMotionStatus(MOTION_STATUS* pMotionStatus,DWORD dwRefIndex,DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	if (!m_pModel)
		goto lb_return;
	
	MODEL_STATUS*	pStatus;
	if (!m_pModel->GetModelStatus(&pStatus,dwRefIndex))
		goto lb_return;

	pMotionStatus->dwFrame = pStatus->dwFrame;
	pMotionStatus->dwMotionIndex = pStatus->dwMotionIndex;
	pMotionStatus->pMotionUID = pStatus->pMotionObject;
	bResult = TRUE;
	
lb_return:
	return bResult;
}
void CMeshObject::SetAlignedViewMatrix(MATRIX4* pMatrixEntry)
{
	VECTOR3			v3Center;
	VECTOR3			v3TransformedCenter;
	MATRIX4			matPos,matScale;
	
		
	VECTOR3			v3Scale;

	float		fLengthX;
	float		fLengthY;
	VECTOR3		v3Rect[4];
	MATRIX4*	pMatResult;

	if (m_pDefaultResultMatrix)
		pMatResult = m_pDefaultResultMatrix;
	else 
		pMatResult = pMatrixEntry + m_dwIndex;
	

	TransformVector3_VPTR2(v3Rect,m_v3Rect,pMatResult,4);
	

	fLengthX = CalcDistance(v3Rect+3,v3Rect+0);
	fLengthY = CalcDistance(v3Rect+1,v3Rect+0);

	v3Scale.x = fLengthX / m_fLengthX;
	v3Scale.y = fLengthY / m_fLengthY;
	v3Scale.z = 1.0f;

	SetScaleMatrix(&matScale,&v3Scale);

	v3Center.x = 0;
	v3Center.y = 0;
	v3Center.z = 0;
	TransformVector3_VPTR2(&v3TransformedCenter,&v3Center,pMatResult,1);
	TranslateMatrix(&matPos,(VECTOR3*)&v3TransformedCenter);

	SetIdentityMatrix(pMatResult);
	MatrixMultiply2(pMatResult,&matScale,m_pGeometry->GetBillBoardMatrix());
	MatrixMultiply2(pMatResult,pMatResult,&matPos);
}
void CMeshObject::ResetTransform(MATRIX4* pResult)
{

	if (m_meshFlag.GetTransformType() == TRANSFORM_TYPE_ALIGN_VIEW)
	{

		VECTOR3			v3Center;
		VECTOR3			v3TransformedCenter;
		MATRIX4			matPos,matScale;
		
			
		VECTOR3			v3Scale;

		float		fLengthX;
		float		fLengthY;
		VECTOR3		v3Rect[4];

		MATRIX4		matResult = *pResult;

		TransformVector3_VPTR2(v3Rect,m_v3Rect,&matResult,4);
		

		fLengthX = CalcDistance(v3Rect+3,v3Rect+0);
		fLengthY = CalcDistance(v3Rect+1,v3Rect+0);

		v3Scale.x = fLengthX / m_fLengthX;
		v3Scale.y = fLengthY / m_fLengthY;
		v3Scale.z = 1.0f;

		SetScaleMatrix(&matScale,&v3Scale);

		v3Center.x = 0;
		v3Center.y = 0;
		v3Center.z = 0;
		TransformVector3_VPTR2(&v3TransformedCenter,&v3Center,&matResult,1);
		TranslateMatrix(&matPos,(VECTOR3*)&v3TransformedCenter);

		MatrixMultiply2(&matResult,&matScale,&matPos);

		TransformVector3_VPTR1(m_pv3World,&m_TM.mat4Inverse,m_dwVertexNum);
		TransformVector3_VPTR1(m_pv3World,&matResult,m_dwVertexNum);
		
		ResetTM(&m_TM);
		TranslateMatrix(&m_TM.mat4,&v3TransformedCenter);
		SetInverseMatrix(&m_TM.mat4Inverse,&m_TM.mat4);
		
	}
	else
	{
		MATRIX4	matResult;
		MatrixMultiply2(&matResult,&m_TM.mat4Inverse,pResult);
		TransformVector3_VPTR1(m_pv3World,&matResult,m_dwVertexNum);
//		TransformVector3_VPTR1(m_pv3World,&m_TM.mat4Inverse,m_dwVertexNum);
//		TransformVector3_VPTR1(m_pv3World,pResult,m_dwVertexNum);
		if (m_pLightTexture)
		{
			m_pLightTexture->ResetTransform(&m_TM.mat4Inverse,pResult);
		}
		ResetTM(&m_TM);

	}

}
VECTOR3* CMeshObject::CreateVertexList(DWORD dwNum)
{
	m_pv3World = new VECTOR3[dwNum];
	m_dwMaxVertexNum = dwNum;

	return m_pv3World;
}
TVERTEX* CMeshObject::CreateTexVertexList(DWORD dwNum)
{
	m_ptUV = new TVERTEX[dwNum];
	return m_ptUV;
}


BOOL CMeshObject::CommitDevice(IDIMeshObject* pDevObj,CMaterialList* pMtlList)
{
	m_pDeviceMeshObj = pDevObj;
	
	MESH_DESC	desc;
	memset(&desc,0,sizeof(desc));

	DWORD i;
	
	m_pv3Local = new VECTOR3[m_dwVertexNum];

	TransformVector3_VPTR2(m_pv3Local,m_pv3World,&m_TM.mat4Inverse,m_dwVertexNum);


	if (m_pv3NormalWorld)
	{
		SetVertexNormal(m_pv3NormalWorld,m_pv3World,m_dwVertexNum);

		
		if (!m_Physique.GetVertexNum())
		{
			m_pv3NormalLocal = new VECTOR3[m_dwVertexNum];
			SetVertexNormal(m_pv3NormalLocal,m_pv3Local,m_dwVertexNum);

			if (m_ptUV)
			{
				m_pv3TangentULocal = new VECTOR3[m_dwVertexNum];
				SetVertexTangentU(m_pv3TangentULocal,m_pv3Local,m_ptUV,m_dwVertexNum);
			}
		}
	}
	if (pDevObj)
	{


		desc.dwFaceGroupNum  = m_dwFaceGroupNum;
		desc.dwVertexNum = m_dwVertexNum;
		desc.meshFlag = m_meshFlag;

		desc.pMatrixWorldInverse = &m_TM.mat4Inverse;
		desc.ptvTexCoordList = m_ptUV;
		desc.pv3WorldList = m_pv3World;
		desc.pv3LocalList = m_pv3Local;
		desc.pVertexColor = m_pVertexColor;
		desc.pv3NormalLocal = m_pv3NormalLocal;
		desc.pv3TangentULocal = m_pv3TangentULocal;
		

		if (m_pVertexColor)
		{
			desc.pVertexColor = m_pVertexColor;
		}
		if (m_pLightTexture)
		{
			m_pLightTexture->GetLightTextureDesc(&desc.LightTexture);
		}


		DWORD* pdwVertexColor = m_pVertexColor;
		pDevObj->StartInitialize(&desc,this,NULL);

		FACE_DESC	fdesc;
		memset(&fdesc,0,sizeof(fdesc));

		for (i=0; i<m_dwFaceGroupNum; i++)
		{
			fdesc.dwFacesNum = m_pFaceGroup[i].GetFacesNum();
			fdesc.dwMtlIndex = m_pFaceGroup[i].GetMtlIndex();

			
			fdesc.pIndex = m_pFaceGroup[i].GetFacesPtr();
			if (m_pFaceGroup[i].GetMaterial())
				fdesc.pMaterial = m_pFaceGroup[i].GetMaterial()->GetMaterialInfo();
			else 
				fdesc.pMaterial = NULL;

			fdesc.ptUVLight1 = m_pFaceGroup[i].GetLightUV1Ptr();
		//	fdesc.ptUVLight2 = pFaceGroup->GetLightUV2Ptr();

			pDevObj->InsertFaceGroup(&fdesc);
		}
		pDevObj->EndInitialize();
	}

	if (m_meshFlag.GetTransformType() != TRANSFORM_TYPE_ILLUSION)
	{
		delete [] m_pv3Local;
		m_pv3Local = NULL;
	}


	delete [] m_pv3NormalLocal;
	m_pv3NormalLocal = NULL;


	return TRUE;
}
BOOL CMeshObject::Initialize(CMaterialList* pMtlList,CoModel* pModel,CoGeometry* pGeometry)
{
	CMaterial*		pCurMtl;

	m_pGeometry = pGeometry;
	m_pModel = pModel;
		
	

	if (m_meshFlag.GetTransformType() == TRANSFORM_TYPE_ALIGN_VIEW)
	{
#ifdef _DEBUG
		if (m_dwFaceGroupNum != 1)
		{
			char	txt[512];
			memset(txt,0,512);
			wsprintf(txt,"CMeshObject::Initialize(), if (m_dwFaceGroupNum != 1), File:%s , Line:%d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}

		if (m_pFaceGroup[0].GetFacesNum() > 2)
		{
			char	txt[512];
			memset(txt,0,512);
			wsprintf(txt,"CMeshObject::Initialize(), if (m_pFaceGroup[0].GetFacesNum() > 2), File:%s , Line:%d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
#endif
	}

	DWORD i;

	//피지크 초기화.
	if (m_Physique.GetVertexNum())
		m_Physique.Initialize(pModel);

	for (i=0; i<m_dwFaceGroupNum; i++)
	{
		pCurMtl = pMtlList->GetMaterial(m_pFaceGroup[i].GetMtlIndex());
		m_pFaceGroup[i].SetMaterial(NULL,pCurMtl);//m_pVertexColor,pCurMtl);
	}

	if (m_meshFlag.GetTransformType() == TRANSFORM_TYPE_ILLUSION)
	{
		if (m_dwMtlIndex != 0xffffffff)
		{
			CMaterial*	pMtl;
			pMtl = pMtlList->GetMaterial(m_dwMtlIndex);
			m_mtlDefault = *pMtl->GetMaterialInfo();	
		}
#ifdef _DEBUG
		else
		{
			char	txt[256];
			wsprintf(txt,"잔상처리용매쉬%s에 매터리얼이 설정되어있지않습니다",m_szObjName);
			MessageBox(NULL,txt,"Error",MB_OK);
		}
#endif
	}
	return TRUE;
}
DWORD CMeshObject::SetVertexNormal(VECTOR3* pv3NormalResult,VECTOR3* pv3VertexList,DWORD dwVertexNum)
{
	DWORD	i;
	memset(pv3NormalResult,0,sizeof(VECTOR3)*dwVertexNum);
	for (i=0; i<m_dwFaceGroupNum; i++)
	{
		SetFaceNormalToVertex(pv3NormalResult,pv3VertexList,dwVertexNum,m_pFaceGroup[i].GetFacesPtr(),m_pFaceGroup[i].GetFacesNum());
	}
	DWORD	dwCount = 0;

	for (i=0; i<dwVertexNum; i++)
	{
		if (!(pv3NormalResult[i].x == 0.0f && 
			pv3NormalResult[i].y == 0.0f && 
			pv3NormalResult[i].z == 0.0f)) 
		{

			Normalize(&pv3NormalResult[i],&pv3NormalResult[i]);
			dwCount++;
		}
	}
	return dwCount;
}
DWORD CMeshObject::SetVertexTangentU(VECTOR3* pv3TangentUResult,VECTOR3* pv3VertexList,TVERTEX* ptvList,DWORD dwVertexNum)
{
	DWORD	i;
	DWORD	dwCount = 0;
	memset(pv3TangentUResult,0,sizeof(VECTOR3)*dwVertexNum);
	
	for (i=0; i<m_dwFaceGroupNum; i++)
	{
		SetFaceTangentToVertex(pv3TangentUResult,pv3VertexList,ptvList,dwVertexNum,m_pFaceGroup[i].GetFacesPtr(),m_pFaceGroup[i].GetFacesNum());
	}
	

	for (i=0; i<dwVertexNum; i++)
	{
		if (!(pv3TangentUResult[i].x == 0.0f && 
			pv3TangentUResult[i].y == 0.0f && 
			pv3TangentUResult[i].z == 0.0f)) 
		{

			Normalize(&pv3TangentUResult[i],&pv3TangentUResult[i]);
			dwCount++;
		}
	}
	return dwCount;
}
DWORD CMeshObject::CreateIVertexList(IVERTEX** ppVertex,MATRIX4* pTM)
{
	if (!m_dwFaceGroupNum || !m_dwTotalFacesNum)
		return 0;
	
	IVERTEX*		pv = new IVERTEX[m_dwTotalFacesNum*3];
	IVERTEX*		pvSrc;

	DWORD			dwResult;
	DWORD			dwTotalVertexCount = 0;
	IVERTEX*		pEntry = pv;

	for (DWORD i=0; i<m_dwFaceGroupNum; i++)
	{
		dwResult = m_pFaceGroup[i].CreateIVertexList(m_pv3World,m_ptUV,&pvSrc);
		dwTotalVertexCount += dwResult;
		
		memcpy(pEntry,pvSrc,sizeof(IVERTEX)*dwResult);
		pEntry += dwResult;
		m_pFaceGroup[i].ReleaseIVertexList(pvSrc);
	}
	
	*ppVertex = pv;

	if (pTM)
	{
		TransformNSizeVector3_VPTR1((char*)pv,sizeof(IVERTEX),&m_TM.mat4Inverse,dwTotalVertexCount);
		TransformNSizeVector3_VPTR1((char*)pv,sizeof(IVERTEX),pTM,dwTotalVertexCount);
	}

	return dwTotalVertexCount;
}


BOOL CMeshObject::CreateBoundingMesh(COLLISION_MODEL_DESC* pEntryDesc,COLLISION_MESH_OBJECT_DESC* pDesc,COLLISION_MESH_OBJECT_DESC* pDescWorld)
{
	m_dwColMeshObjectDescOffset = (DWORD)pDesc - (DWORD)pEntryDesc;
	if (pDescWorld)
		CreateBoundingMesh(pDesc,pDescWorld);

	return TRUE;
}

BOOL CMeshObject::CreateBoundingMesh(COLLISION_MESH_OBJECT_DESC* pDescLocal,COLLISION_MESH_OBJECT_DESC* pDescWorld)
{
	VECTOR3*	pv3New;
	VECTOR3*	pv3Pos;

	BOOL		bResult = FALSE;
	DWORD		i,k,dwVertexNum;
	WORD*		pIndex;

	memset(pDescLocal,0,sizeof(COLLISION_MESH_OBJECT_DESC));
	memset(pDescWorld,0,sizeof(COLLISION_MESH_OBJECT_DESC));
	COLLISION_MESH_OBJECT_DESC	descWorld;

	if (!m_dwTotalFacesNum)
		goto lb_return;

	if (!m_dwVertexNum)
		goto lb_return;

	
	dwVertexNum = m_dwTotalFacesNum*3;
	


	pv3New = pv3Pos = new VECTOR3[dwVertexNum];

	
	for (k=0; k<m_dwFaceGroupNum; k++)
	{
		pIndex = m_pFaceGroup[k].GetFacesPtr();
		for (i=0; i<m_pFaceGroup[k].GetFacesNum()*3; i++)
		{
			pv3Pos[i] = m_pv3World[pIndex[i]];
			
		}
		pv3Pos += m_pFaceGroup[k].GetFacesNum()*3;
	}

	descWorld = *pDescWorld;
	bResult = ::CalcBoundingMesh(pv3New,dwVertexNum,&descWorld);
	


	TransformVector3_VPTR1(pv3New,&m_TM.mat4Inverse,dwVertexNum);

	bResult = ::CalcBoundingMesh(pv3New,dwVertexNum,pDescLocal);
	pDescLocal->boundingSphere.fRs = descWorld.boundingSphere.fRs;

	if (m_meshFlag.GetTransformType() == TRANSFORM_TYPE_ALIGN_VIEW)
	{
		
		memcpy(m_v3Rect,pDescLocal->boundingBox.v3Oct,sizeof(VECTOR3)*4);
		m_fLengthX = CalcDistance(m_v3Rect+3,m_v3Rect+0);
		m_fLengthY = CalcDistance(m_v3Rect+1,m_v3Rect+0);
	}
	*pDescWorld = descWorld;
		
	
	delete [] pv3New;
lb_return:
	return bResult;

}
BOOL CMeshObject::IsCollisionMeshAndRay(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,VECTOR3* pv3Pos,VECTOR3* pv3Dir,MATRIX4* pTM)
{
	BOOL	bResult = FALSE;
	

	VECTOR3*	pv3Temp = m_pv3World;
	BOOL		bNeedDel = FALSE;
	MATRIX4		matWorld;

	float	fMinDist = 900000.0f;
	VECTOR3	v3IntersectPoint;
	WORD	wIndex;

	float	fDist;

	if (pTM)
	{
		
		pv3Temp = new VECTOR3[m_dwVertexNum];
	
	//	TransformVector3_VPTR2(pv3Temp,m_pv3World,&m_TM.mat4Inverse,m_dwVertexNum);
	//	TransformVector3_VPTR1(pv3Temp,pTM,m_dwVertexNum);
	
		MatrixMultiply2(&matWorld,&m_TM.mat4Inverse,pTM);
		TransformVector3_VPTR2(pv3Temp,m_pv3World,&matWorld,m_dwVertexNum);
	
		bNeedDel = TRUE;
	}

	for (DWORD i=0; i<m_dwFaceGroupNum; i++)
	{
		if (m_pFaceGroup[i].IsCollision(&v3IntersectPoint,&fDist,&wIndex,pv3Pos,pv3Dir,pv3Temp))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;

				*pfDist = fDist;
				*pv3IntersectPoint = v3IntersectPoint;
				*pdwFaceGroupIndex = i;
				bResult = TRUE;
			}
		}
	}
	
	if (bNeedDel)
		delete [] pv3Temp;

	return bResult;
}

void CMeshObject::ReleaseIVertexList(IVERTEX* pv)
{
	delete [] pv;
}

void CMeshObject::ReleaseVertexList()
{
	if (m_pv3World)
	{
		delete [] m_pv3World;
		m_pv3World = NULL;
	}
	if (m_pv3Local)
	{
		delete [] m_pv3Local;
		m_pv3Local = NULL;
	}
	m_dwVertexNum = 0;
}
void CMeshObject::ReleaseTexVertexList()
{
	if (m_ptUV)
	{
		delete [] m_ptUV;
		m_ptUV = NULL;
		m_dwTexVertexNum = 0;
	}
}
void CMeshObject::ReleaseTangentUVector()
{
	if (m_pv3TangentULocal)
	{
		delete [] m_pv3TangentULocal;
		m_pv3TangentULocal = NULL;
	}
}
void CMeshObject::ReleaseFaceGroups()
{
	if (m_pFaceGroup)
	{
		delete [] m_pFaceGroup;
		m_pFaceGroup = NULL;
		m_dwFaceGroupNum = 0;
	}
}
BOOL CMeshObject::Duplicate(CMeshObject* pMeshObject,MATRIX4* pObjResult,COLLISION_MESH_OBJECT_DESC* pColMeshObjDesc)
{
	// 인덱스가 바뀌지 않도록 인덱스를 백업해두고 카피가 끝나면 다시 세팅한다.
	DWORD	dwObjIndex = pMeshObject->GetIndex();
	CBaseObject::Duplicate(pMeshObject);

	memcpy((char*)pMeshObject+sizeof(CBaseObject),(char*)this+sizeof(CBaseObject),sizeof(FILE_MESH_HEADER));
	
	pMeshObject->SetIndex(dwObjIndex);
	pMeshObject->m_dwTotalFacesNum = m_dwTotalFacesNum;
	memcpy(pMeshObject->m_v3Rect,m_v3Rect,sizeof(VECTOR3)*4);
	pMeshObject->m_fLengthX = m_fLengthX;
	pMeshObject->m_fLengthY = m_fLengthY;
	if (m_pv3World)
	{
		pMeshObject->m_pv3World = new VECTOR3[m_dwVertexNum];
		memcpy(pMeshObject->m_pv3World,m_pv3World,sizeof(VECTOR3)*m_dwVertexNum);
	}
	if (m_pv3NormalWorld)
	{
		pMeshObject->m_pv3NormalWorld = new VECTOR3[m_dwVertexNum];
		memcpy(pMeshObject->m_pv3NormalWorld,m_pv3NormalWorld,sizeof(VECTOR3)*m_dwVertexNum);
	}

	if (m_ptUV)
	{
		pMeshObject->m_ptUV = new TVERTEX[m_dwTexVertexNum];
		memcpy(pMeshObject->m_ptUV,m_ptUV,sizeof(TVERTEX)*m_dwTexVertexNum);
	}
	if (m_pLightTexture)
	{
		pMeshObject->m_pLightTexture = new CLightTexture;
		m_pLightTexture->Duplicate(pMeshObject->m_pLightTexture);
	}

	if (m_pFaceGroup)
	{
		DuplicateFaceGroup(pMeshObject);
	}
	if (m_pdwExtVertexIndex)
	{
		pMeshObject->m_pdwExtVertexIndex = new DWORD[m_dwExtVertexNum];
		memcpy(pMeshObject->m_pdwExtVertexIndex,m_pdwExtVertexIndex,sizeof(DWORD)*m_dwExtVertexNum);
	}

	if (pObjResult)
	{
		pMeshObject->ResetTransform(pObjResult);
	
	}

	if (pMeshObject->m_meshFlag.GetTransformType() != TRANSFORM_TYPE_ALIGN_VIEW)
	{
		pMeshObject->m_meshFlag.SetTransformType(TRANSFORM_TYPE_SOLID);
		ResetTM(&pMeshObject->m_TM);

		if (pMeshObject->m_meshFlag.GetShadeType() != SHADE_TYPE_LIGHT_MAP)
		{
			pMeshObject->m_pVertexColor = new DWORD[m_dwVertexNum];
			memset(pMeshObject->m_pVertexColor,0xffffffff,sizeof(DWORD)*m_dwVertexNum);
		}

	}


	pMeshObject->m_pColMeshTemp = new COLLISION_MESH_OBJECT_DESC;
	memcpy(pMeshObject->m_pColMeshTemp,pColMeshObjDesc,sizeof(COLLISION_MESH_OBJECT_DESC));



	return TRUE;
}
BOOL CMeshObject::CreateFaceGroup(DWORD dwNum)
{
	BOOL	bResult = FALSE;
	if (!dwNum)
		goto lb_return;

	m_pFaceGroup = new CFaceGroup[dwNum];
	m_dwFaceGroupNum = dwNum;
	DWORD i;
	for (i=0; i<dwNum; i++)
	{
		m_pFaceGroup[i].SetIndex(i);
	}
	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL CMeshObject::DuplicateFaceGroup(CMeshObject* pMeshObject)
{
	DWORD	i;
	pMeshObject->CreateFaceGroup(m_dwFaceGroupNum);
	
	for (i=0; i<m_dwFaceGroupNum; i++)
	{
		m_pFaceGroup[i].Duplicate(&pMeshObject->m_pFaceGroup[i]);
	}
	return TRUE;
}
BOOL CMeshObject::BeginShadeVertexColor(DWORD dwColor)
{

	DWORD	i;

	for (i=0; i<m_dwVertexNum; i++)
	{
		m_pVertexColor[i] = dwColor; 
	}
	return TRUE;
}
void CMeshObject::ShadeVertexColor(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc)
{
	::ShadeVertexColor(m_pVertexColor,m_pv3World,m_pv3NormalWorld,m_dwVertexNum,pv3,dwFacesNum,pLight,pFunc);
	m_bShaded = TRUE;
}
void CMeshObject::EndShadeVertexColor(FILE* fp)
{
	if (fp)
		fwrite(m_pVertexColor,sizeof(DWORD),m_dwVertexNum,fp);
}

DWORD CMeshObject::ReadVertexColor(void* pFP)
{

	if (!m_pVertexColor)
		m_pVertexColor = new DWORD[m_dwVertexNum];

	if (!pFP)
	{
		memset(m_pVertexColor,0xffffffff,sizeof(DWORD)*m_dwVertexNum);
		return 0;
	}

	DWORD	dwLen = 0;
	
	dwLen += g_pFileStorage->FSRead(pFP,m_pVertexColor,sizeof(DWORD)*m_dwVertexNum);
	m_bShaded = TRUE;

	return dwLen;
}
DWORD CMeshObject::ReadFile(void* pFP,DWORD dwVersion)
{
	DWORD	dwLen = 0;
	
	FILE_MESH_HEADER *	pHeader = (FILE_MESH_HEADER*)&(this->m_dwMaxVertexNum);
	
	dwLen += CBaseObject::ReadFile(pFP,dwVersion);
	dwLen += g_pFileStorage->FSRead(pFP,pHeader,sizeof(FILE_MESH_HEADER));

	
	
	if (m_dwVertexNum)
	{
		CreateVertexList(m_dwVertexNum);
		dwLen += g_pFileStorage->FSRead(pFP,m_pv3World,sizeof(VECTOR3)*m_dwVertexNum);

	}
	if (m_dwTexVertexNum)
	{
		CreateTexVertexList(m_dwTexVertexNum);
		dwLen += g_pFileStorage->FSRead(pFP,m_ptUV,sizeof(TVERTEX)*m_dwTexVertexNum);
	}
	if (m_dwExtVertexNum)
	{
		m_pdwExtVertexIndex = new DWORD[m_dwExtVertexNum];
		dwLen += g_pFileStorage->FSRead(pFP,m_pdwExtVertexIndex,sizeof(DWORD)*m_dwExtVertexNum);
	}

	DWORD num = m_dwFaceGroupNum;
	m_dwFaceGroupNum = 0;

	CreateFaceGroup(num);
	
	for (DWORD i=0; i<m_dwFaceGroupNum; i++)
	{
		dwLen += m_pFaceGroup[i].ReadFile(pFP);
		m_dwTotalFacesNum += m_pFaceGroup[i].GetFacesNum();
		
	}

	if (dwVersion >= 0x00000002)
		dwLen += m_Physique.ReadFile(pFP);
	else
		dwLen += m_Physique.ReadFileOld(pFP);

	if (m_meshFlag.GetShadeType() == SHADE_TYPE_LIGHT_MAP)
	{
		m_pLightTexture = new CLightTexture;
		dwLen += m_pLightTexture->ReadFile(pFP);
	}
	else// if (m_meshFlag.GetShadeType() == SHADE_TYPE_VERTEX_LIGHT_IM)
	{
		if (m_meshFlag.GetTransformType() != TRANSFORM_TYPE_ALIGN_VIEW)
		{
			m_pv3NormalWorld = new VECTOR3[m_dwVertexNum];
			dwLen += g_pFileStorage->FSRead(pFP,m_pv3NormalWorld,sizeof(VECTOR3)*m_dwVertexNum);

			
		}
	}

	return dwLen;
}
DWORD CMeshObject::WriteFile(FILE* fp)
{
	
	DWORD	oldPos = ftell(fp);
	
	FILE_MESH_HEADER*	pHeader = (FILE_MESH_HEADER*)&(this->m_dwMaxVertexNum);
	CBaseObject::WriteFile(fp);
	fwrite(pHeader,sizeof(FILE_MESH_HEADER),1,fp);		// 헤더부터 쓰고...
	
	if (m_dwVertexNum)
		fwrite(m_pv3World,sizeof(VECTOR3),m_dwVertexNum,fp);
	
	if (m_dwTexVertexNum)
		fwrite(m_ptUV,sizeof(TVERTEX),m_dwTexVertexNum,fp);

	if (m_dwExtVertexNum)
		fwrite(m_pdwExtVertexIndex,sizeof(DWORD),m_dwExtVertexNum,fp);




	for (DWORD i=0; i<m_dwFaceGroupNum; i++)
	{
		m_pFaceGroup[i].WriteFile(fp);
		
	}
	m_Physique.WriteFile(fp,m_pModel);

	if (m_meshFlag.GetShadeType() == SHADE_TYPE_LIGHT_MAP)
	{
		m_pLightTexture->WriteFile(fp);

	}
	else
	{
		if (m_meshFlag.GetTransformType() != TRANSFORM_TYPE_ALIGN_VIEW)
		{
			fwrite(m_pv3NormalWorld,sizeof(VECTOR3),m_dwVertexNum,fp);
		}
	}

	return (ftell(fp)-oldPos);
	
}
void CMeshObject::ReleaseLightTexture()
{
	if (m_pLightTexture)
	{
		delete m_pLightTexture;
		m_pLightTexture = NULL;
	}
}
void CMeshObject::ReleaseVertexColor()
{
	if (m_pVertexColor)
	{
		delete [] m_pVertexColor;
		m_pVertexColor = NULL;
	}
}
void CMeshObject::ReleaseVertexNormal()
{
	if (m_pv3NormalWorld)
	{
		delete [] m_pv3NormalWorld;
		m_pv3NormalWorld = NULL;
	}
	if (m_pv3NormalLocal)
	{
		delete [] m_pv3NormalLocal;
		m_pv3NormalLocal = NULL;
	}
}
void CMeshObject::Optimize()
{


	if (!m_Physique.GetVertexNum())
		ReleaseTexVertexList();

	ReleaseLightTexture();
	ReleaseVertexColor();
	ReleaseVertexNormal();
	ReleaseTangentUVector();
	
	if (m_pDeviceMeshObj)
		m_pDeviceMeshObj->DisableUpdate();

}

void CMeshObject::ReleaseLightTexturePatches()
{
	if (m_pLightTexture)
	{
		m_pLightTexture->ReleasePatches();
	}
}

void CMeshObject::ReleaseIDIMeshObject()
	{
		if (m_pDeviceMeshObj)
		{
			m_pDeviceMeshObj->Release();
			m_pDeviceMeshObj = NULL;
		}
	}
CMeshObject::~CMeshObject()
{
	CBaseObject::~CBaseObject();

	ReleaseIDIMeshObject();
	ReleaseFaceGroups();
	ReleaseVertexList();

	ReleaseTexVertexList();
	ReleaseLightTexture();
	ReleaseVertexColor();
	ReleaseVertexNormal();
	ReleaseTangentUVector();

	
	if (m_pdwExtVertexIndex)
	{
		delete [] m_pdwExtVertexIndex;
		m_pdwExtVertexIndex = NULL;
	}

	if (m_pColMeshTemp)
	{
		delete m_pColMeshTemp;
		m_pColMeshTemp = NULL;
	}

}



void __stdcall CMeshObject::GetTileBuffer(TILE_BUFFER_DESC** ppTileBufferDesc,DWORD* pdwTileBufferDescNum)
{
}
DWORD __stdcall CMeshObject::GetTileBufferDescNum()
{
	return 0;
}
TILE_BUFFER_DESC* __stdcall	CMeshObject::GetTileBufferDesc(DWORD dwIndex)
{
	return NULL;
}

/*
CFaceGroup* CMeshObject::AllocFaceGroup()
{
	CFaceGroup*		pFaceGroup = new CFaceGroup;
	
	pFaceGroup->m_pNextFaceGroup = m_pBaseFaceGroup;
	m_pBaseFaceGroup = pFaceGroup;

	m_dwFaceGroupNum++;
	pFaceGroup->SetIndex(m_dwFaceGroupNum);

	return pFaceGroup;
}*/

/*
VECTOR3* CMeshObject::CreateVertexWorldList()
{
	m_pv3World = new VECTOR3[m_dwVertexNum];
	memcpy(m_pv3World,m_pv3World,sizeof(VECTOR3)*m_dwVertexNum);
	return m_pv3World;
}*/

/*
void CMeshObject::ClearVertexList()
{
	memset(m_pv3World,0,sizeof(VECTOR3)*m_dwVertexNum);
}*/





/*
void CMeshObject::ReleaseWorldVertexList()
{
	if (m_pv3World)
	{
		delete [] m_pv3World;
		m_pv3World = NULL;
	}
}*/



/*
void CMeshObject::ClearVBCache(I4DyuchiGXRenderer* pRenderer)
{

	if (m_Physique.GetVertexNum())
	{
		if (m_pDeviceMeshObj)
			pRenderer->ClearVBCacheWithIDIMeshObject(m_pDeviceMeshObj);
	}
}*/

