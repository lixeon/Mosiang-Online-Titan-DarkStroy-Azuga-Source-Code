#include "gxobject.h"
#include "../4DyuchiGXGFunc/global.h"
#include "GXDefault.h"
#include "GXLight.h"
#include "../4DyuchiGRX_myself97/myself97common.h"
#include "../4DyuchiGRX_myself97/CheckClock.h"
//#include "../4DyuchiGRX_myself97/MBSPTree.h"
#include "../4DyuchiGRX_myself97/CollisionTest/DummyCollisionTest.h"
#include "../4DyuchiGRX_myself97/CollisionTest/SphereCollisionTest.h"
#include "../4DyuchiGRX_myself97/CollisionTest/AAEllipsoidCollisionTest.h"

#include "global_variable.h"


// warning number 4012 is 'unreferenced label' warning.
#pragma warning( disable : 4102 )



BOOL __stdcall CoGXObject::IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
	DWORD	dwIndex = 0xffffffff;
	DWORD	dwFaceIndex;
	BOOL	bResult = FALSE;

	if (PICK_TYPE_DEFAULT != m_dwPickTypeFlag)
	{
		dwFlag = m_dwPickTypeFlag;
	}

	*pdwModelIndex = 0xffffffff;
	for (DWORD i=0; i<m_dwModelNum; i++)
	{

		dwIndex = m_ModelList[i].pModel->GetMeshObjectWithScreenCoord(
			pv3IntersectPoint,
			pfDist,
			&dwFaceIndex,
			m_ModelList[i].dwRefIndex,
			m_pParentMatrix,
			&m_matTransform,
			m_dwCurrentFrame,
			m_dwCurrentMotionIndex,
			ptCursor,
			dwViewportIndex,
			dwFlag);

		if (dwIndex != 0xffffffff)
		{
			*pdwObjIndex = dwIndex;
			*pdwModelIndex = i;
			bResult = TRUE;
			goto lb_return;
		}
	}

lb_return:
	return bResult;
}


BOOL CoGXObject::Initialize(CoExecutive* pExecutive,MODEL_HANDLE* pModelHandle,DWORD dwModelNum,GXSchedulePROC pProc,DWORD dwFlag)
{
	
	BOOL	bResult = FALSE;
	DWORD	i;
	
	memset(this,0,sizeof(CoGXObject));
	CGXMapObject::Cleanup();
	SetIdentityMatrix(&m_matTransform);
	m_pExecutive	=	pExecutive;
	m_dwEffectIndex = 0xffffffff;


	m_dwModelNum = dwModelNum;
	for (i=0; i<dwModelNum; i++)
	{
		m_ModelList[i] =  pModelHandle[i];
	
	}
	
	m_pProc = pProc;
	if (dwFlag & GXOBJECT_CREATE_TYPE_DEFAULT_PROC)
	{
		m_pProc = GXDefaultSchedulePROC;
	}

	SetIdentityMatrix(&m_matTransform);
	SetIdentityMatrix(&m_matScale);
	SetIdentityMatrix(&m_matRot);

	m_v3Axis.x = 0.0f;
	m_v3Axis.y = 1.0f;
	m_v3Axis.z = 0.0f;
	
	m_v3Scale.x = 1.0f;
	m_v3Scale.y = 1.0f;
	m_v3Scale.z = 1.0f;


	m_dwCurrentMotionIndex = 1;
	m_dwObjectType = GX_MAP_OBJECT_TYPE_OBJECT;

	//m_dwRenderFlag = RENDER_TYPE_SEND_SHADOW | RENDER_TYPE_UPDATE_SHADING;
	m_dwPropertyFlag = dwFlag;
	m_dwScheduleFlag = 0;
	m_dwRenderFlag = RENDER_TYPE_SEND_SHADOW | RENDER_TYPE_RECV_SHADOW | RENDER_TYPE_UPDATE_SHADING;// | RENDER_TYPE_SELF_ILLUNUM;
	m_dwAlphaFlag = 0x000000ff;
	m_dwID = 0xfffffff0;
	m_pHashHandle = NULL;

	if (dwFlag & GXOBJECT_CREATE_TYPE_EFFECT)
	{
		EnableAsEffect();

	}
	InitializeCollisionMesh();


	m_pCollisionTestFn	=	DummyCollisionTest;

	// 속도 벡터는 0
	m_v3Velocity.x		=	0.0f;
	m_v3Velocity.y		=	0.0f;
	m_v3Velocity.z		=	0.0f;

//	m_bApplyVelocity	=	FALSE;

	m_bLanding			=	FALSE;

	bResult = TRUE;


lb_return:
	return bResult;

}

void CoGXObject::SetEffectIndex(DWORD dwEffectIndex)
{
	m_dwEffectIndex = dwEffectIndex;
	if (dwEffectIndex == 0xffffffff)
		m_dwRenderFlag &= (~RENDER_TYPE_USE_EFFECT);
	else
		m_dwRenderFlag |= RENDER_TYPE_USE_EFFECT;
}
DWORD CoGXObject::GetEffectIndex()
{
	return m_dwEffectIndex;
}


BOOL CoGXObject::InitializeIllusionEffect(DWORD dwMaxIllusionFrameNum,char* szObjName,void* pMtlHandle,DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	
	DWORD	i,dwRefIndex,dwObjIndex;
	I3DModel*	pModel;
	for (i=0; i<m_dwModelNum; i++)
	{
		dwObjIndex = m_ModelList[i].pModel->GetObjectIndex(szObjName);
		if (dwObjIndex != 0xffffffff)
		{
			pModel = m_ModelList[i].pModel;
			dwRefIndex = m_ModelList[i].dwRefIndex;
			goto lb_init;

		}
	}
	goto lb_return;

lb_init:
	m_pModelForIllusion = pModel;
	m_dwObjIndexForIllusion = dwObjIndex;
	m_dwRefIndexForIllusion = dwRefIndex;	
	
	bResult = pModel->InitializeIllusionEffect(dwRefIndex,dwMaxIllusionFrameNum,dwObjIndex,pMtlHandle,dwFlag);

lb_return:
	return bResult;
}

void CoGXObject::BeginIllusionEffect()
{
	if (m_pModelForIllusion)
	{
		m_pModelForIllusion->SetBeginIllusionEffect(m_dwRefIndexForIllusion,m_dwObjIndexForIllusion);
	}
}

void CoGXObject::EndIllusionEffect()
{
	if (m_pModelForIllusion)
	{
		m_pModelForIllusion->SetEndIllusionEffect(m_dwRefIndexForIllusion,m_dwObjIndexForIllusion);
	}
}

DWORD CoGXObject::GetAttachedGXObjects(GXOBJECT_HANDLE* pGXOList,DWORD dwMaxNum)
{
	DWORD	dwNum = m_dwChildGXObjectNum;

	if (dwNum > dwMaxNum)
		dwNum = dwMaxNum;

	for (DWORD i=0; i<dwNum; i++)
	{
		pGXOList[i] = (GXOBJECT_HANDLE)m_pChildGXObject[i];
	}
	return dwNum;

}

void CoGXObject::EnableAsEffect()
{
	EnableSelfIllumin();
	DisablePick();
	DisableSendShadow();

	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		m_ModelList[i].pModel->SetZOrder(9,m_ModelList[i].dwRefIndex);
	}
	m_dwPropertyFlag |= GXOBJECT_CREATE_TYPE_EFFECT;
}

void CoGXObject::SetZOrder(int iZOrder)
{
	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		m_ModelList[i].pModel->SetZOrder(iZOrder,m_ModelList[i].dwRefIndex);
	}
}

void CoGXObject::DisableAsEffect()
{
	DisableSelfIllumin();
	EnablePick();
	EnableSendShadow();

	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		m_ModelList[i].pModel->SetZOrder(0,m_ModelList[i].dwRefIndex);
	}
	m_dwPropertyFlag &= GXOBJECT_CREATE_TYPE_EFFECT_INVERSE;
}

void CoGXObject::SetPosition(VECTOR3* pv3Pos, BOOL bDoInterpolation)
{

	if (m_dwPropertyFlag & GXMAP_OBJECT_COMMON_TYPE_LOCK_TRANSFORM)
		return;

	m_v3Pos = *pv3Pos;

	if( bDoInterpolation == FALSE)	
	{
		SetOldPosition( &m_v3Pos);
	}

	CGXMap*	pMap = m_pExecutive->GetGXMap();
	if (pMap)
	{
		if (m_dwPropertyFlag & GXOBJECT_CREATE_TYPE_APPLY_HFIELD)
		{
			ResetPosition(pMap);
			SetOldPosition( &m_v3Pos);
		}
	}
		

	UpdateCollisionMesh();
	
	DWORD	i;	
	for (i=0; i<m_dwChildGXObjectNum; i++)
	{
		m_pChildGXObject[i]->UpdateCollisionMesh();
	
	}
	for (i=0; i<m_dwChildGXLightNum; i++)
	{
		m_pChildLight[i]->UpdateCollisionMesh();
	}
}

void CoGXObject::MovePosition(VECTOR3* pv3Pos) 
{


	VECTOR3		v3Pos;
	VECTOR3_ADD_VECTOR3(&v3Pos,&m_v3Pos,pv3Pos);
	SetPosition(&v3Pos, TRUE);

}

void CoGXObject::ResetPosition(CGXMap* pMap)
{
	if (pMap->GetHField())
	{
		pMap->GetHField()->GetY(
			&m_v3Pos.y,// out y
			m_v3Pos.x,			// in x
			m_v3Pos.z			// in z
			);
	}
}
DWORD CoGXObject::GetObjectIndex(char* szObjName,DWORD dwModelIndex)
{
	DWORD		dwObjIndex = 0xffffffff;
	if (dwModelIndex >= MAX_MODEL_NUM_PER_GXOBJECT)
		goto lb_return;

	if (!m_ModelList[dwModelIndex].pModel)
		goto lb_return;

	dwObjIndex = m_ModelList[dwModelIndex].pModel->GetObjectIndex(szObjName);
lb_return:
	
	return dwObjIndex;
}
BOOL CoGXObject::GetWorldMatrixPerObject(MATRIX4* pMatWorld,DWORD dwModelIndex,DWORD dwObjIndex)
{
	BOOL	bResult = FALSE;
	if (dwModelIndex >= MAX_MODEL_NUM_PER_GXOBJECT)
		goto lb_return;

	if (!m_ModelList[dwModelIndex].pModel)
		goto lb_return;

	bResult = m_ModelList[dwModelIndex].pModel->GetWorldMatrix(pMatWorld,m_ModelList[dwModelIndex].dwRefIndex,dwObjIndex);

lb_return:
	return bResult;
}
BOOL CoGXObject::RequestAttach(CoGXObject* pFromObj,DWORD dwModelIndex,char* szObjName)
{
	BOOL		bResult = FALSE;
	DWORD		dwIndex = 0xffffffff;	
	I3DModel*	pModel = NULL;
	DWORD		dwRefIndex;
	DWORD		i;
	
	if (m_dwChildGXObjectNum >= MAX_ATTATCH_OBJECTS_NUM)
		goto lb_return;

	if (dwModelIndex >= m_dwModelNum)
		goto lb_all_search;

	
	dwIndex = m_ModelList[dwModelIndex].pModel->GetObjectIndex(szObjName);
	if (dwIndex != 0xffffffff)
	{
        pModel = m_ModelList[dwModelIndex].pModel;
		dwRefIndex = m_ModelList[dwModelIndex].dwRefIndex;
		goto lb_attach;
	}

lb_all_search:
	for (i=0; i<m_dwModelNum; i++)
	{
		dwIndex = m_ModelList[i].pModel->GetObjectIndex(szObjName);
		if (dwIndex != 0xffffffff)
		{
			pModel = m_ModelList[i].pModel;
			dwRefIndex = m_ModelList[i].dwRefIndex;
			goto lb_attach;
		}
	}
	
	goto lb_return;

lb_attach:

#ifdef	_DEBUG
	for (i=0; i<m_dwChildGXObjectNum; i++)
	{
		if (m_pChildGXObject[i] == pFromObj)
		{
			char	txt[512];
			memset(txt,0,512);
			wsprintf(txt,"Already Attached GXObjectHandle  : %x\n",pFromObj);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
		}
	}
#endif

	m_pChildGXObject[m_dwChildGXObjectNum] = pFromObj;
	m_dwChildGXObjectNum++;

	pFromObj->PostAttach(this,pModel->GetObjectResultMatrix(dwRefIndex,dwIndex));
	pFromObj->UpdateCollisionMesh();
	pFromObj->DisableSchedule();
	
	
	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CoGXObject::RequestDetach(CoGXObject* pFromObj)
{
	BOOL	bResult = FALSE;

	for (DWORD i=0; i<m_dwChildGXObjectNum; i++)
	{
		if (m_pChildGXObject[i] == pFromObj)
		{
			m_pChildGXObject[i] = m_pChildGXObject[m_dwChildGXObjectNum-1];
			m_pChildGXObject[m_dwChildGXObjectNum-1] = NULL;

			pFromObj->PostDetach(this);
			pFromObj->UpdateCollisionMesh();
			pFromObj->EnableSchedule();
			m_dwChildGXObjectNum--;
			bResult = TRUE;
			goto lb_return;
		}
	}
		
lb_return:
	return bResult;

}
BOOL CoGXObject::DetachAllGXObjects()
{	
	for (DWORD i=0; i<m_dwChildGXObjectNum; i++)
	{
		if (m_pChildGXObject[i])
		{
			m_pChildGXObject[i]->PostDetach(this);
			m_pChildGXObject[i]->EnableSchedule();
			m_pChildGXObject[i] = NULL;
		}
	}
	m_dwChildGXObjectNum = 0;
	return TRUE;
}
BOOL CoGXObject::PostAttach(CoGXObject* pFromObj,MATRIX4* pMat)
{
	m_pParentGXObject = pFromObj;
	m_pParentMatrix = pMat;

	return TRUE;
}
BOOL CoGXObject::PostDetach(CoGXObject* pFromObj)
{
	m_pParentGXObject = NULL;
	m_pParentMatrix = NULL;

	return TRUE;
}

void CoGXObject::ChangeDirection(float fRad)
{
	SetDirection(&m_v3Axis,m_fRad+fRad);
}
void CoGXObject::SetDirection(VECTOR3* pv3Axis,float fRad)
{
	if (m_dwPropertyFlag & GXMAP_OBJECT_COMMON_TYPE_LOCK_TRANSFORM)
		return;

	// xy평면 기준이다.
	   
	FLOAT     fCos = (FLOAT)cosf( fRad );
    FLOAT     fSin = (FLOAT)sinf( fRad );

    m_matRot._11 = ( pv3Axis->x * pv3Axis->x ) * ( 1.0f - fCos ) + fCos;
    m_matRot._12 = ( pv3Axis->x * pv3Axis->y ) * ( 1.0f - fCos ) - (pv3Axis->z * fSin);
    m_matRot._13 = ( pv3Axis->x * pv3Axis->z ) * ( 1.0f - fCos ) + (pv3Axis->y * fSin);

    m_matRot._21 = ( pv3Axis->y * pv3Axis->x ) * ( 1.0f - fCos ) + (pv3Axis->z * fSin);
    m_matRot._22 = ( pv3Axis->y * pv3Axis->y ) * ( 1.0f - fCos ) + fCos ;
    m_matRot._23 = ( pv3Axis->y * pv3Axis->z ) * ( 1.0f - fCos ) - (pv3Axis->x * fSin);

    m_matRot._31 = ( pv3Axis->z * pv3Axis->x ) * ( 1.0f - fCos ) - (pv3Axis->y * fSin);
    m_matRot._32 = ( pv3Axis->z * pv3Axis->y ) * ( 1.0f - fCos ) + (pv3Axis->x * fSin);
    m_matRot._33 = ( pv3Axis->z * pv3Axis->z ) * ( 1.0f - fCos ) + fCos;
       
	m_v3Axis = *pv3Axis;
	m_fRad = fRad;


	UpdateCollisionMesh();
}

void CoGXObject::SetDirectionFPSStyle(VECTOR3* pv3Angles)
{
	MATRIX4 matYaw;
	MATRIX4 matPitch;
	MATRIX4 matRoll;

	SetRotationXMatrix(&matPitch, pv3Angles->x);
	SetRotationYMatrix(&matYaw, pv3Angles->y);
	SetRotationZMatrix(&matRoll, pv3Angles->z);	

	MatrixMultiply3(&m_matRot, &matRoll, &matPitch, &matYaw);

	// 회전 각도값을 저장해 놓는다. 

	m_v3Angle = *pv3Angles;

	UpdateCollisionMesh();
}

void CoGXObject::MoveForward(float fDistance)
{
	VECTOR3		v3NewPos;
	EstimatedMoveForward((VECTOR3*)&v3NewPos,fDistance);
	SetPosition(&v3NewPos, TRUE);
}
void CoGXObject::SetScale(VECTOR3* pv3Scale)
{
	if (m_dwPropertyFlag & GXMAP_OBJECT_COMMON_TYPE_LOCK_TRANSFORM)
		return;

	m_v3Scale = *pv3Scale;
	SetScaleMatrix(&m_matScale,pv3Scale);
	UpdateCollisionMesh();
}
void CoGXObject::GetScale(VECTOR3* pv3Scale)
{
	*pv3Scale = m_v3Scale;
}

void CoGXObject::EstimatedMoveForward(VECTOR3* pv3Pos,float fDistance)
{
	MATRIX4		matRot;
	VECTOR3		dir_src;
	VECTOR3		dir_dest;

	dir_src.x = 0.0f;
	dir_src.y = 0.0f;
	dir_src.z = -1.0f;

	SetRotationMatrix(&matRot,&m_v3Axis,m_fRad);
	TransformVector3_VPTR2(&dir_dest,&dir_src,&matRot,1);

	dir_dest.x *= fDistance;
	dir_dest.y *= fDistance;
	dir_dest.z *= fDistance;


	VECTOR3	To;
	To.x	=	m_v3Pos.x	+	dir_dest.x;
	To.y	=	m_v3Pos.y	+	dir_dest.y;
	To.z	=	m_v3Pos.z	+	dir_dest.z;
	COLLISION_TEST_RESULT	CollTestResult;
	this->TryMoveTo( &CollTestResult, &To, 0);
	
	*pv3Pos = CollTestResult.Candidate;
}

BOOL CoGXObject::Replace(DWORD dwModelIndex,char* szFileName)
{
	BOOL		bResult = FALSE;
	//I3DModel*	pModel;
	//DWORD		dwRefIndex;
	MODEL_HANDLE model_handle;
	char		ext[32];

	memset(&model_handle, 0, sizeof(MODEL_HANDLE));

	if (dwModelIndex >= m_dwModelNum)
		goto lb_return;
	

	if (!GetExt(ext,szFileName))
	{
		goto lb_return;
	}

	if (!memcmp(ext,".chx",4) || !memcmp(ext,".CHX",4))
	{
		goto lb_return;
	}

	
	//if (0xffffffff == m_pExecutive->GetGeometry()->LoadModel(&pModel,szFileName,0))
	//	goto lb_return;		

	if (0xffffffff == m_pExecutive->LoadGXObject(&model_handle, 1, szFileName))
	{
		goto lb_return;
	}	

	if (NULL == model_handle.pModel)
	{
		goto lb_return;

	}

	//pModel->Initialize(m_pExecutive->GetRenderer(),0);

	//dwRefIndex = pModel->Reference();
	//if (dwRefIndex == 0xffffffff)
	//{
	//	pModel->Release();
	//	goto lb_return;
	//}
	

	if (0 == model_handle.pModel->GetMotionNum(model_handle.dwRefIndex))
	{
		m_ModelList[dwModelIndex].pModel->DuplicateAnimation(model_handle.dwRefIndex,
															 model_handle.pModel,										
															 m_ModelList[dwModelIndex].dwRefIndex);			

	}	

	if (0 == model_handle.pModel->GetMaterialNum(model_handle.dwRefIndex))
	{
		m_ModelList[dwModelIndex].pModel->DuplicateMaterial(model_handle.dwRefIndex,
															model_handle.pModel,
															m_ModelList[dwModelIndex].dwRefIndex);
	}	
	
	//m_ModelList[dwModelIndex].pModel->DuplicateAnimation(dwRefIndex,pModel,m_ModelList[dwModelIndex].dwRefIndex);
	//m_ModelList[dwModelIndex].pModel->DuplicateMaterial(dwRefIndex,pModel,m_ModelList[dwModelIndex].dwRefIndex);
	

	m_ModelList[dwModelIndex].pModel->UnReference(m_ModelList[dwModelIndex].dwRefIndex);
	m_ModelList[dwModelIndex].pModel->Release();

	
	//m_ModelList[dwModelIndex].pModel = pModel;
	//m_ModelList[dwModelIndex].dwRefIndex = dwRefIndex;	
	
	m_ModelList[dwModelIndex].pModel		= model_handle.pModel;
	m_ModelList[dwModelIndex].dwRefIndex	= model_handle.dwRefIndex;


	m_ModelList[dwModelIndex].pModel->UpdateColMeshDesc(
		m_ModelList[dwModelIndex].dwRefIndex,m_pParentMatrix,&m_matTransform,m_dwCurrentFrame,m_dwCurrentMotionIndex,
		m_dwRenderFlag | RENDER_TYPE_UPDATE_COLLISION_BONEMESH_DESC);


	InitializeCollisionMesh();
	UnRegistFileNameHash();
	bResult = TRUE;

lb_return:
	return bResult;
}


//
// CoGXObject::Replace 원본..
// 

/*
BOOL CoGXObject::Replace(DWORD dwModelIndex,char* szFileName)
{
	BOOL		bResult = FALSE;
	I3DModel*	pModel;
	DWORD		dwRefIndex;

	if (dwModelIndex >= m_dwModelNum)
		goto lb_return;

	if (0xffffffff == m_pExecutive->GetGeometry()->LoadModel(&pModel,szFileName,0))
		goto lb_return;

	pModel->Initialize(m_pExecutive->GetRenderer(),0);

	dwRefIndex = pModel->Reference();
	if (dwRefIndex == 0xffffffff)
	{
		pModel->Release();
		goto lb_return;
	}
	


	m_ModelList[dwModelIndex].pModel->DuplicateAnimation(dwRefIndex,pModel,m_ModelList[dwModelIndex].dwRefIndex);
	m_ModelList[dwModelIndex].pModel->DuplicateMaterial(dwRefIndex,pModel,m_ModelList[dwModelIndex].dwRefIndex);

	m_ModelList[dwModelIndex].pModel->UnReference(m_ModelList[dwModelIndex].dwRefIndex);
	m_ModelList[dwModelIndex].pModel->Release();

	m_ModelList[dwModelIndex].pModel = pModel;
	m_ModelList[dwModelIndex].dwRefIndex = dwRefIndex;

	m_ModelList[dwModelIndex].pModel->UpdateColMeshDesc(
		m_ModelList[dwModelIndex].dwRefIndex,m_pParentMatrix,&m_matTransform,m_dwCurrentFrame,m_dwCurrentMotionIndex,
		m_dwRenderFlag | RENDER_TYPE_UPDATE_COLLISION_BONEMESH_DESC);


	InitializeCollisionMesh();
	UnRegistFileNameHash();
	bResult = TRUE;

lb_return:
	return bResult;
}
*/


DWORD g_dwPrvTick = 0;

DWORD CoGXObject::OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2)
{
	SetOldPosition( &m_v3Pos);
/*	if (m_pChildGXObject[0])
	{
		DWORD	dwCurTick = GetTickCount();
		DWORD	dwElapsedTick = dwCurTick - g_dwPrvTick;

		g_dwPrvTick = dwCurTick;
	
		char szBuff[0xff];
		wsprintf(szBuff, "%d\n", dwElapsedTick);
		OutputDebugString(szBuff);
		__asm nop
	}
*/
	if (m_pProc)
		m_pProc(pExecutive,this,msg,arg1,arg2,m_pData);

//	if( m_bApplyVelocity)
//		OnFrameApplyVelocity();


//	DefaultProc();
	for (DWORD i=0; i<m_dwChildGXObjectNum; i++)
	{
		m_pChildGXObject[i]->OnFrame(pExecutive,msg,arg1,arg2);
	}

	return 0;
}
void CoGXObject::DefaultProc()
{
/*	MATRIX4		mat;

	VECTOR3		eye,at;
	//eye = m_cameraControl.pParentMatrix
	// 카메라 기준 방향은 (0,0,0) (0,0,시야거리)
	if (m_cameraControl.bHasCamera)
	{
		eye = *(VECTOR3*)&m_cameraControl.pCameraMatrix->_41;
		at = *(VECTOR3*)&m_cameraControl.pCameraTargetMatrix->_41;

		m_pCameraObject->SetCamera(&eye,&at);
	}*/
//	pExecutive->GXORender(gxh,0);
}
DWORD CoGXObject::DuplicateModelHandle(MODEL_HANDLE* pDest)
{
	DWORD	dwResult = 0;
	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		DWORD	dwDestRefIndex;
		dwDestRefIndex = m_ModelList[i].pModel->Reference();

		if (dwDestRefIndex == 0xffffffff)
			goto lb_return;


		DWORD	dwMotionNum,dwMaterialNum;
		dwMotionNum = m_ModelList[i].pModel->GetMotionNum(m_ModelList[i].dwRefIndex);

		if (dwMotionNum)
		{
			m_ModelList[i].pModel->DuplicateAnimation(dwDestRefIndex,m_ModelList[i].pModel,m_ModelList[i].dwRefIndex);
		}
		dwMaterialNum = m_ModelList[i].pModel->GetMaterialNum(m_ModelList[i].dwRefIndex);
		if (dwMaterialNum)
		{
			m_ModelList[i].pModel->DuplicateMaterial(dwDestRefIndex,m_ModelList[i].pModel,m_ModelList[i].dwRefIndex);
		}

		
		pDest[i].pModel = m_ModelList[i].pModel;
		pDest[i].pModel->AddRef();
		pDest[i].dwRefIndex = dwDestRefIndex;
		dwResult++;
	}
lb_return:
	return dwResult;
}
BOOL CoGXObject::Render()
{
	BOOL	bResult = FALSE;

	DWORD	i,j;
	
	

	if (m_pParentMatrix)
		goto lb_return;

	*(VECTOR3*)&m_matTransform._41 = m_v3InterpolatedRenderPos;
//	*(VECTOR3*)&m_matTransform._41 = m_v3Pos;

	for (i=0; i<m_dwModelNum; i++)
	{
		bResult |= m_pExecutive->INL_GetGeometry()->RenderModel(
			m_ModelList[i].pModel,
			m_ModelList[i].dwRefIndex,
			NULL,
			&m_matTransform,
//			&matTransform,
			m_dwCurrentFrame,
			m_dwCurrentMotionIndex,
			m_dwAlphaFlag,
			m_dwCurrentMtlSetIndex,
			m_dwEffectIndex,
			m_dwRenderFlag);
		//| RENDER_TYPE_SELF_ILLUMIN);
		 
		// | RENDER_TYPE_IGNORE_VIEWVOLUME_CLIP);
	}
	if (bResult)
	{
		for (j=0; j<m_dwChildGXObjectNum; j++)
		{
			m_pChildGXObject[j]->RenderByParent(m_pChildGXObject[j]->GetParentMatrix());
		}
	}
lb_return:

	return bResult;
}
void CoGXObject::SetAlphaFlag(DWORD dwFlag)
{
	m_dwAlphaFlag = dwFlag;
	for (DWORD i=0; i<m_dwChildGXObjectNum; i++)
	{
		m_pChildGXObject[i]->SetAlphaFlag(dwFlag);
	}
}
BOOL CoGXObject::RenderByParent(MATRIX4* pParentMatrix)
{
	BOOL		bResult = FALSE;

//	m_pParentGXObject->UpdateCollisionMesh();
	/*
	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		bResult |= m_pExecutive->INL_GetGeometry()->RenderModel(
			m_ModelList[i].pModel,
			m_ModelList[i].dwRefIndex,
			m_pParentMatrix,
			NULL,
			m_dwCurrentFrame,
			m_dwCurrentMotionIndex,
			m_dwAlphaFlag,
			0,
			m_dwEffectIndex,
			m_dwRenderFlag | RENDER_TYPE_IGNORE_VIEWVOLUME_CLIP);
	}*/


	// 어태치되었을 경우는 scale만 인정한다.
	
	/*
	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		bResult |= m_pExecutive->INL_GetGeometry()->RenderModel(
			m_ModelList[i].pModel,
			m_ModelList[i].dwRefIndex,
			pParentMatrix,
			&m_matScale,
			m_dwCurrentFrame,
			m_dwCurrentMotionIndex,
			m_dwAlphaFlag,
			0,
			m_dwEffectIndex,
			m_dwRenderFlag | RENDER_TYPE_IGNORE_VIEWVOLUME_CLIP);
	}
	*/

	//trustpak 2005/07/05
	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		bResult |= m_pExecutive->INL_GetGeometry()->RenderModel(
			m_ModelList[i].pModel,
			m_ModelList[i].dwRefIndex,
			pParentMatrix,
			&m_matScale,
			m_dwCurrentFrame,
			m_dwCurrentMotionIndex,
			m_dwAlphaFlag,
			m_dwCurrentMtlSetIndex,
			m_dwEffectIndex,
			m_dwRenderFlag | RENDER_TYPE_IGNORE_VIEWVOLUME_CLIP);
	}
	///

	return bResult;
}

BOOL CoGXObject::RenderByParentDirectModel(MATRIX4* pParentMatrix)
{
	BOOL		bResult = FALSE;

	/*
	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		m_ModelList[i].pModel->Render(
			m_ModelList[i].dwRefIndex,
			pParentMatrix,
			&m_matScale,
			m_dwCurrentFrame,
			m_dwCurrentMotionIndex,
			m_dwAlphaFlag,
			0,
			m_dwEffectIndex,
			m_dwRenderFlag | RENDER_TYPE_DISABLE_ZCLIP | RENDER_TYPE_SELF_ILLUMIN | RENDER_TYPE_IGNORE_VIEWVOLUME_CLIP);
		//
	}
	*/
	//trustpak 2005/07/05
	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		m_ModelList[i].pModel->Render(
			m_ModelList[i].dwRefIndex,
			pParentMatrix,
			&m_matScale,
			m_dwCurrentFrame,
			m_dwCurrentMotionIndex,
			m_dwAlphaFlag,
			m_dwCurrentMtlSetIndex,
			m_dwEffectIndex,
			m_dwRenderFlag | RENDER_TYPE_DISABLE_ZCLIP | RENDER_TYPE_SELF_ILLUMIN | RENDER_TYPE_IGNORE_VIEWVOLUME_CLIP);
		//
	}

	///
	return bResult;
}


/*
BOOL CoGXObject::AttachCamera(IDICameraObject* pCamera,char* szCameraName,char*szCameraTargetName)
{
	BOOL	bResult = FALSE;

	DWORD	dwCameraIndex;
	DWORD	dwCameraTargetIndex;

	if (!m_pModel)
		goto lb_return;

	dwCameraIndex = m_pModel->GetObjectIndex(szCameraName);
	dwCameraTargetIndex = m_pModel->GetObjectIndex(szCameraTargetName);

	if (dwCameraIndex == 0xffffffff || dwCameraTargetIndex == 0xffffffff) 
		goto lb_return;

	if (FALSE == m_cameraControl.bHasCamera)
	{
		pCamera->GetCamera(&m_cameraControl.v3EyeOld,&m_cameraControl.v3AtOld);
		m_cameraControl.pCameraMatrix = m_pModel->GetObjectResultMatrix(m_dwModelRefIndex,dwCameraIndex);
		m_cameraControl.pCameraTargetMatrix = m_pModel->GetObjectResultMatrix(m_dwModelRefIndex,dwCameraTargetIndex);;
		m_cameraControl.bHasCamera = TRUE;
		m_pCameraObject = pCamera;
		bResult = TRUE;
	}


lb_return:
	return bResult;
}*/
/*
void CoGXObject::DetachCamera()
{
	if (m_cameraControl.bHasCamera)
	{
		m_pCameraObject->SetCamera(&m_cameraControl.v3EyeOld,&m_cameraControl.v3AtOld);
		m_pCameraObject = NULL;
		m_cameraControl.bHasCamera = FALSE;
	}
}*/
BOOL CoGXObject::AttachLight(CoGXLight* pLight,char* szObjName,VECTOR3* pv3Offset,DWORD dwAttachType)
{
	BOOL		bResult = FALSE;
	DWORD		dwIndex = 0xffffffff;
	I3DModel*	pModel = NULL;
	DWORD		dwRefIndex;
	DWORD		i;
	

	if (m_dwChildGXLightNum >= MAX_ATTATCH_OBJECTS_NUM)
		goto lb_return;

	for (i=0; i<m_dwModelNum; i++)
	{
		dwIndex = m_ModelList[i].pModel->GetObjectIndex(szObjName);
		if (dwIndex != 0xffffffff)
		{
			pModel = m_ModelList[i].pModel;
			dwRefIndex = m_ModelList[i].dwRefIndex;
			goto lb_attach;
		}
	}
	goto lb_return;

lb_attach:
#ifdef	_DEBUG
	for (i=0; i<m_dwChildGXLightNum; i++)
	{
		if (m_pChildLight[i] == pLight)
		{
			char	txt[512];
			wsprintf(txt,"Already Attached GXLightHandle  : %x\n",pLight);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
		}
	}
#endif
	m_pChildLight[m_dwChildGXLightNum] = pLight;
	m_dwChildGXLightNum++;
	pLight->PostAttach(this,pModel->GetObjectResultMatrix(dwRefIndex,dwIndex),pv3Offset,dwAttachType);
	
	bResult = TRUE;

	
lb_return:


	return bResult;
}
BOOL CoGXObject::DetachLight(CoGXLight* pLight)
{
	BOOL		bResult = FALSE;
	for (DWORD i=0; i<m_dwChildGXLightNum; i++)
	{
		if (m_pChildLight[i] == pLight)
		{
			m_pChildLight[i] = m_pChildLight[m_dwChildGXLightNum-1];
			m_pChildLight[m_dwChildGXLightNum-1] = NULL;
			pLight->PostDetach();
			m_dwChildGXLightNum--;
			bResult = TRUE;
			goto lb_return;
		}
	}
lb_return:
	return bResult;
}

BOOL CoGXObject::DetachAllLights()
{
	for (DWORD i=0; i<m_dwChildGXLightNum; i++)
	{
		if (m_pChildLight[i])
		{
			m_pChildLight[i]->PostDetach();
			m_pChildLight[i] = NULL;
		}
	}
	m_dwChildGXLightNum = 0;
lb_return:
	return TRUE;
}
DWORD CoGXObject::Release()
{
	CGXMap*	pMap = m_pExecutive->GetGXMap();
	if (pMap)
	{
		if (m_dwIndexInInitialTable != 0xffffffff)
			pMap->DeleteInitialGXObject(m_dwIndexInInitialTable);

		if (m_dwClipperIndex != 0xffffffff)
			pMap->DeleteDynamicObject(m_dwClipperIndex);
	}

	Cleanup();
	CGXMapObject::Cleanup();
	return 0;
	
}
void CoGXObject::InitializeCollisionMesh()
{
	VECTOR3		v3[8*MAX_MODEL_NUM_PER_GXOBJECT];
	DWORD		dwVerticesNum = m_dwModelNum*8;
	
	VECTOR3*	pv3Entry = v3;
	
	
	if (!m_dwModelNum)
		return;

	COLLISION_MODEL_DESC			colModelDesc;
	COLLISION_MESH_OBJECT_DESC*		pColMeshObjDesc;
	BOOL bColModel;
	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		bColModel = m_ModelList[i].pModel->GetCollisionModelDesc(&colModelDesc,m_ModelList[i].dwRefIndex);
#ifdef _DEBUG
		if (!bColModel)
		{
			char	txt[512];
			memset(txt,0,512);
			wsprintf(txt,"CoGXObject::InitializeCollisionMesh(), if (!bColModel), File:%s , Line:%d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
#endif 
		pColMeshObjDesc = &colModelDesc.colMeshModel;
		memcpy(pv3Entry,pColMeshObjDesc->boundingBox.v3Oct,sizeof(VECTOR3)*8);
		pv3Entry += 8;
	}
	CalcBoundingMesh(v3,dwVerticesNum,&m_colMeshDescLocal);
	m_colMeshDescWorld.boundingSphere.fRs = m_colMeshDescLocal.boundingSphere.fRs;

	MatrixMultiply2(&m_matTransform,&m_matScale,&m_matRot);
	*(VECTOR3*)&m_matTransform._41 = m_v3Pos;

	
	MATRIX4		matTransform = m_matTransform;

	for (i=0; i<m_dwModelNum; i++)
	{
		m_ModelList[i].pModel->UpdateColMeshDesc(m_ModelList[i].dwRefIndex,m_pParentMatrix,&m_matTransform,m_dwCurrentFrame,m_dwCurrentMotionIndex,m_dwRenderFlag | RENDER_TYPE_UPDATE_COLLISION_BONEMESH_DESC);
	}
	
	if (m_pParentMatrix)
		MatrixMultiply2(&matTransform,&matTransform,m_pParentMatrix);
	
	TransformVector3_VPTR2(
		&m_colMeshDescWorld.boundingSphere.v3Point,
		&m_colMeshDescLocal.boundingSphere.v3Point,
		&matTransform,1);

	TransformVector3_VPTR2(
		m_colMeshDescWorld.boundingBox.v3Oct,
		m_colMeshDescLocal.boundingBox.v3Oct,
		&matTransform,8);

}

void CoGXObject::UpdateCollisionMesh()
{

	MatrixMultiply2(&m_matTransform,&m_matScale,&m_matRot);
	*(VECTOR3*)&m_matTransform._41 = m_v3Pos;

	
	MATRIX4		matTransform = m_matTransform;

	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		m_ModelList[i].pModel->UpdateColMeshDesc(m_ModelList[i].dwRefIndex,m_pParentMatrix,&m_matTransform,m_dwCurrentFrame,m_dwCurrentMotionIndex,m_dwRenderFlag | RENDER_TYPE_UPDATE_COLLISION_BONEMESH_DESC);
	}
	
//	if (m_pParentMatrix)
//	{
//		MatrixMultiply2(&matTransform,&matTransform,m_pParentMatrix);
//	}

	
	if (m_pParentMatrix)
	{
		// 어태치 되었을 경우 scale만 인정한다.
		MatrixMultiply2(&matTransform,&m_matScale,m_pParentMatrix);
	}

	
	TransformVector3_VPTR2(
		&m_colMeshDescWorld.boundingSphere.v3Point,
		&m_colMeshDescLocal.boundingSphere.v3Point,
		&matTransform,1);

	TransformVector3_VPTR2(
		m_colMeshDescWorld.boundingBox.v3Oct,
		m_colMeshDescLocal.boundingBox.v3Oct,
		&matTransform,8);

	CGXMap*	pMap = m_pExecutive->GetGXMap();
	if (pMap)
	{
		if (m_dwClipperIndex != 0xffffffff)
			pMap->UpdateDynamicObject(m_dwClipperIndex);
	}
}
void CoGXObject::DisableUnloadPreLoaded()
{
	CGXMapObject::DisableUnloadPreLoaded();
	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		if (m_ModelList[i].pModel)
		{
			m_ModelList[i].pModel->DisableUnloadPreLoadedResource(m_ModelList[i].dwRefIndex);
		}
	}
}
void CoGXObject::EnableUnloadPreLoaded()
{
	CGXMapObject::EnableUnloadPreLoaded();
	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		if (m_ModelList[i].pModel)
		{
			m_ModelList[i].pModel->EnableUnloadPreLoadedResource(m_ModelList[i].dwRefIndex);
		}
	}
}
void CoGXObject::SetAxisAlignOK(BOOL bResult)
{
	
	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		if (m_ModelList[i].pModel)
		{
			m_ModelList[i].pModel->SetAxisAlignOK(bResult,m_ModelList[i].dwRefIndex);
		}
	}
}
COLLISION_MESH_OBJECT_DESC*	__stdcall CoGXObject::GetCollisionMeshObjectDesc()
{
	
	return &m_colMeshDescWorld;
	
}
void CoGXObject::Unload()
{
	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		if (m_ModelList[i].pModel)
		{

			m_ModelList[i].pModel->UnloadMotionPerReference(m_ModelList[i].dwRefIndex);
			m_ModelList[i].pModel->UnloadMaterialPerReference(m_ModelList[i].dwRefIndex);
			m_ModelList[i].pModel->Unload();
		}
	}
}
void CoGXObject::Cleanup()
{
	if (m_pParentGXObject)
	{
		RequestDetach(m_pParentGXObject);
	}

	DetachAllLights();
	DetachAllGXObjects();

	UnRegistFileNameHash();

	for (DWORD i=0; i<m_dwModelNum; i++)
	{
		if (m_ModelList[i].pModel)
		{

			m_ModelList[i].pModel->UnReference(m_ModelList[i].dwRefIndex);
			m_ModelList[i].pModel->Release();
			m_ModelList[i].pModel = NULL;
			m_ModelList[i].dwRefIndex = 0xffffffff;
		}
	}
	ReleaseID();
	
}
void CoGXObject::UnRegistFileNameHash()
{
	if (m_pModelFileItem)
	{
		if (m_pPrvObject)
			m_pPrvObject->m_pNextObject = m_pNextObject;
		else
			m_pModelFileItem->pGXObject = m_pNextObject;


		if (m_pNextObject)
			m_pNextObject->m_pPrvObject = m_pPrvObject;


		m_pExecutive->DeleteModelFileDesc(m_pModelFileItem);
		m_pModelFileItem = NULL;
	}
}
DWORD __stdcall CoGXObject::GetObjectIndexInModel()
{
	return 0xffffffff;
}

DWORD __stdcall CoGXObject::CreateIVertexList(IVERTEX** ppVertex)
{
	DWORD	dwTotaFacesNum = 0;
	IVERTEX*	pVertices[MAX_MODEL_NUM_PER_GXOBJECT];
	DWORD		dwFacesNum[MAX_MODEL_NUM_PER_GXOBJECT];
	DWORD		i;

	memset(pVertices,0,sizeof(IVERTEX*)*MAX_MODEL_NUM_PER_GXOBJECT);

	for (i=0; i<m_dwModelNum; i++)
	{
		dwFacesNum[i] = m_ModelList[i].pModel->CreateIVertexList(&pVertices[i],m_ModelList[i].dwRefIndex,m_pParentMatrix,&m_matTransform,m_dwCurrentFrame,m_dwCurrentMotionIndex);
		dwTotaFacesNum += dwFacesNum[i]; 
	}
	IVERTEX*	pv3Entry;
	pv3Entry = *ppVertex = new IVERTEX[dwTotaFacesNum*3];

	for (i=0; i<m_dwModelNum; i++)
	{
		if (dwFacesNum[i])
		{
			memcpy(pv3Entry,pVertices[i],sizeof(IVERTEX)*dwFacesNum[i]);
			pv3Entry += dwFacesNum[i];

			m_ModelList[i].pModel->ReleaseIVertexList(pVertices[i]);
		}
	}
	return dwTotaFacesNum;
}



void __stdcall CoGXObject::ReleaseIVertexList(IVERTEX*	pVertex)
{
	if (pVertex)
		delete [] pVertex;
}
BOOL __stdcall CoGXObject::ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc)
{
	return FALSE;
}

void CoGXObject::SetBoundingVolume( BOUNDING_VOLUME* pBV)
{
	m_BV	=	*pBV;
	switch( m_BV.dwType)
	{
		case	BOUNDING_VOLUME_TYPE_NONE:
			m_pCollisionTestFn	=	DummyCollisionTest;
			break;
		case	BOUNDING_VOLUME_TYPE_SPHERE:
			m_pCollisionTestFn	=	SphereCollisionTest;
			break;
		case	BOUNDING_VOLUME_TYPE_AAELLIPSOID:
			m_pCollisionTestFn	=	AAEllipsoidCollisionTest;
			break;
		default:
#ifdef _DEBUG
		char	txt[128];
		memset(txt,0,128);
		wsprintf(txt,"Fail to CoGXObject::SetBoundingVolume()\n");
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
#endif
		break;
	}
}

// 출력결과로 후보결과 리턴 외에 콜백 함수 호출.
void CoGXObject::TryMoveTo( COLLISION_TEST_RESULT* pResult, VECTOR3* pTo, DWORD dwBufferIndex)
{
	// 중력 가속도 적용.?
	// v = v0 + a * t;

	// 타입 체크해서 미리 뺄건 빼고.
	if( m_BV.dwCollisionTargetFlag == COLLISION_TARGET_FLAG_TEST_NONE || m_BV.dwType == BOUNDING_VOLUME_TYPE_NONE)
	{
		pResult->bLand	=	FALSE;

		pResult->LastVelocity.x	=	0.0f;
		pResult->LastVelocity.y	=	0.0f;
		pResult->LastVelocity.z	=	0.0f;

		pResult->Candidate		=	*pTo;

		return	;
	}

	COLLISION_TEST_RESULT		CollTestResult;
	m_pCollisionTestFn( m_pExecutive, this, &CollTestResult, pTo, dwBufferIndex);
	*pResult	=	CollTestResult;

	return	;

}

void	CoGXObject::MovePositionWithCollide(VECTOR3* pv3Pos)
{
	VECTOR3		To, Candidate;
	To.x	=	m_v3Pos.x	+	pv3Pos->x;
	To.y	=	m_v3Pos.y	+	pv3Pos->y;
	To.z	=	m_v3Pos.z	+	pv3Pos->z;

	COLLISION_TEST_RESULT		CollResult;
	TryMoveTo( &CollResult, &To, 0);
	m_bLanding		=	CollResult.bLand;
	m_v3Velocity	=	CollResult.LastVelocity;
	Candidate		=	CollResult.Candidate;

	SetPosition(&Candidate, TRUE);

}
/*
BOOL __stdcall CoGXObject::IsPreCollisionWithScreenCoord(VECTOR3* pv3RayPos,VECTOR3* pv3RayDir)
{
	BOOL	bResult = FALSE;

	float	t = 
		( DotProduct(pv3RayDir,&m_colMeshDescWorld.boundingSphere.v3Point) - DotProduct(pv3RayPos,pv3RayDir) ) /
		DotProduct(pv3RayDir,pv3RayDir);

	
	VECTOR3		v3PosX;
	VECTOR3_MUL_FLOAT(&v3PosX,pv3RayDir,t);
	VECTOR3_ADDEQU_VECTOR3(&v3PosX,pv3RayPos);

	if ( CalcDistance(&v3PosX,&m_colMeshDescWorld.boundingSphere.v3Point) > m_colMeshDescWorld.boundingSphere.fRs)
		goto lb_return;


	bResult = TRUE;
lb_return:
	return bResult;
}

*/

void CoGXObject::SetOldPosition(VECTOR3 *pPos)
{
	//	2004/01/08	보간을 위해 추가.
	m_v3OldPos	=	*pPos;
	m_fCurrentRenderTime	=	0.0f;
	
}

void CoGXObject::InterpolatePosition( float fTime)
{
	m_v3InterpolatedRenderPos	=	m_v3OldPos;

	VECTOR3			Velocity;
	VECTOR3_SUB_VECTOR3( &Velocity, &m_v3Pos, &m_v3OldPos);

	float	fVelocity	=	VECTOR3Length( &Velocity);
//	if( fVelocity == 0.0f || fTime == 0.0f)
//		return	;

	m_fCurrentRenderTime	=	fTime;

	VECTOR3_MULEQU_FLOAT( &Velocity, m_fCurrentRenderTime);
	VECTOR3_ADDEQU_VECTOR3( &m_v3InterpolatedRenderPos, &Velocity);

}

void CoGXObject::GetInterpolatedPosition( VECTOR3* pv3Pos)
{
	*pv3Pos		=	m_v3InterpolatedRenderPos;
}

BOOL CoGXObject::IsLanding( void)
{
	return	m_bLanding;
}

void CoGXObject::GetLastVelocityAfterCollisionTest(VECTOR3 *pLastVelocity)
{
	*pLastVelocity	=	m_v3Velocity;
}
