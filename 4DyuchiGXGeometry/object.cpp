#include "object.h"
#include "../4DyuchiGXGFunc/global.h"
#include "../4DyuchiGRX_common/IRenderer.h"
#include "geometry_global.h"
#include "model.h"
#include "global_variable.h"


CBaseObject::CBaseObject()
{
	memset(this,0,sizeof(CBaseObject));
	/*
	m_dwMaxMotionObjectNum = 0;
	memset(&m_TM,0,sizeof(NODE_TM));
	memset(m_szObjName,0,sizeof(m_szObjName));
	m_ppMotionObjectList = NULL;

	m_dwIndex = 0;
	m_ppChildObjectList = 0;
	m_pChildObjectIndexList = NULL;
	m_dwChildObjectNum = 0;
	m_pParentObject = NULL;
*/
	m_objType = OBJECT_TYPE_UNKNOWN;
}
/*
void __stdcall CalcInverseRotate(MATRIX4* pResult,MATRIX4* pTransformMat)
{
	VECTOR3		v3Front;
	
	v3Front.x = 0.0f;
	v3Front.y = 0.0f;
	v3Front.z = -1.0f;

	MATRIX4		matRot = *pTransformMat;
	matRot._41 = 0.0f;
	matRot._42 = 0.0f;
	matRot._43 = 0.0f;


	VECTOR3		v3Dir;
	TransformVector3_VPTR2(&v3Dir,&v3Front,&matRot,1);
	float ang = CalcAngle(&v3Front,&v3Dir);
	
	VECTOR3		v3Axis;
	CrossProduct(&v3Axis,&v3Dir,&v3Front);
	Normalize(&v3Axis,&v3Axis);
	if (ang == 0.0f )
		SetIdentityMatrix(pResult);
	else
		SetRotationMatrix(pResult,&v3Axis,-1.0f*ang);
	
	TransformVector3_VPTR1(&v3Dir,pResult,1);


}*/
BOOL CBaseObject::SetFrame(MATRIX4* pResult,MATRIX4* pMatrixEntry,DWORD dwRefIndex,DWORD dwFrame,DWORD dwMotionIndex)
{
	// 모션 인덱스와 프레임 카운트를 참조하여 모션 오브젝트로부터 애니메이션 키를 읽어서 매트릭스를 만든다.
	MATRIX4		matRot,matPos,matScale;


	VECTOR3*	axis = (VECTOR3*)&m_TM.fRotAxisX;
	VECTOR3*	pos = (VECTOR3*)&m_TM.fPosX;
	VECTOR3*	scale = (VECTOR3*)&m_TM.fScaleX;
	
	
	BOOL		bHasRot = FALSE;
	BOOL		bHasPos = FALSE;
	BOOL		bHasScale = FALSE;
	

	CMotionObject* pMot = m_pModel->GetMotionObject(m_szObjName,m_dwNameLen,dwRefIndex,dwMotionIndex);
	

	if (pMot)
	{
		bHasRot = pMot->SetRotMatrix(&matRot,&m_TM,dwFrame);
		bHasPos = pMot->SetPosMatrix(&matPos,&m_TM,dwFrame);
		bHasScale = pMot->SetScaleMatrix(&matScale,&m_TM,dwFrame);
	} 
	if (!bHasRot)
	{

		if (m_TM.fRotAng)
			SetRotationMatrix(&matRot,axis,(-1)*m_TM.fRotAng);
		else
			SetIdentityMatrix(&matRot);	
	}
	if (!bHasPos)
	{
		TranslateMatrix(&matPos,pos);
	}


	if (!bHasScale)
	{
		SetScaleMatrix(&matScale,scale);
	}

//	MatrixMultiply3(pResult,&matScale,&matRot,&matPos);
	
	MatrixMultiply2(pResult,&matScale,&matRot);
	MatrixMultiply2(pResult,pResult,&matPos);

	if (m_pParentObject)
	{
		MatrixMultiply2(pResult,pResult,m_pParentObject->GetIndex() + pMatrixEntry);


	}	
/*		if (!IsEnableRotate())
		{
//			MatrixMultiply2(pResult,pResult,m_pParentObject->GetRotMatrix());	

//			pResult->_11 = 1.0f;
//			pResult->_12 = 0.0f;
//			pResult->_13 = 0.0f;
//			pResult->_14 = 0.0f;
			
//			pResult->_21 = 0.0f;
//			pResult->_22 = 1.0f;
//			pResult->_23 = 0.0f;
//			pResult->_24 = 0.0f;
			
//			pResult->_31 = 0.0f;
//			pResult->_32 = 0.0f;
//			pResult->_33 = 1.0f;
//			pResult->_34 = 0.0f;

//			MatrixMultiply2(pResult,pResult,m_pParentObject->GetScaleMatrix());	
//			MatrixMultiply2(pResult,pResult,m_pParentObject->GetPosMatrix());	

			MATRIX4	matInverseRot;
			
			CalcInverseRotate(&matInverseRot,pParentMat);
			CalcInverseRotate(&matInverseRot,pResult);
			MatrixMultiply2(pResult,&matInverseRot,pResult);	
//			MatrixMultiply2(pResult,pResult,&matInverseRot);	
		}*/

	return TRUE;
}



DWORD CBaseObject::ReadFile(void* pFP,DWORD dwVersion)
{	
	DWORD	dwLen = 0;
	FILE_BASE_OBJECT_HEADER* pHeader = (FILE_BASE_OBJECT_HEADER*)&(this->m_dwIndex);

	dwLen += g_pFileStorage->FSRead(pFP,pHeader,sizeof(FILE_BASE_OBJECT_HEADER));

	if (m_dwChildObjectNum)
	{
		m_pChildObjectIndexList = new DWORD[m_dwChildObjectNum];
		dwLen += g_pFileStorage->FSRead(pFP,m_pChildObjectIndexList,sizeof(DWORD)*m_dwChildObjectNum);
	}
	m_dwNameLen = lstrlen(m_szObjName);
	return dwLen;

}


BOOL CBaseObject::Duplicate(CBaseObject* pBaseObject)
{
	memcpy(pBaseObject,this,sizeof(FILE_BASE_OBJECT_HEADER));
	pBaseObject->m_dwNameLen = m_dwNameLen;

	return TRUE;
}
BOOL __stdcall CBaseObject::ApplyResultMatrix(MATRIX4** ppResult,DWORD dwRefIndex,DWORD dwFlag)
{
	if (m_pDefaultResultMatrix )
		*ppResult = m_pDefaultResultMatrix;
	else
		*ppResult = m_pModel->GetObjectResultMatrix(dwRefIndex,GetIndex());

		

	return TRUE;
}

BOOL __stdcall CBaseObject::ApplyWorldMatrixForPhysique(MATRIX4* pResultWorldMatrix,DWORD dwRefIndex,DWORD dwFlag)
{
	return FALSE;
}

BOOL __stdcall CBaseObject::ApplyPhysique(BYTE* pVertex,DWORD dwVertexSize,DWORD dwRefIndex,DWORD dwFlag)
{
	return FALSE;
}
BOOL __stdcall CBaseObject::ApplyMorph(VECTOR3** ppv3Local,TVERTEX** pptv,DWORD dwRefIndex,DWORD dwFlag)
{
	return FALSE;
}
BOOL __stdcall CBaseObject::ApplyView(MATRIX4* pView,DWORD dwRefIndex,DWORD dwFlag)
{
	return FALSE;
}
BOOL __stdcall CBaseObject::ApplyVertexNormal(BYTE* pVertex,DWORD dwVertexStartOffset,DWORD dwVertexSize,DWORD dwRefIndex,DWORD dwFlag)
{
	return FALSE;
}
BOOL __stdcall CBaseObject::ApplyMotionStatus(MOTION_STATUS* pMotionStatus,DWORD dwRefIndex,DWORD dwFlag)
{
	return FALSE;
}

void CBaseObject::ResetDefaultResultMatrix()
{
	MATRIX4*	pResult = m_pModel->GetObjectResultMatrix(0,GetIndex());
	*pResult = m_TM.mat4;
}
DWORD CBaseObject::WriteFile(FILE* fp)
{
	DWORD	oldPos = ftell(fp);
	
	FILE_BASE_OBJECT_HEADER* pHeader = (FILE_BASE_OBJECT_HEADER*)&(this->m_dwIndex);
	fwrite(pHeader,sizeof(FILE_BASE_OBJECT_HEADER),1,fp);
	if (m_dwChildObjectNum)
		fwrite(m_pChildObjectIndexList,sizeof(DWORD),m_dwChildObjectNum,fp);
	
	return (ftell(fp)-oldPos);
	
}
CBaseObject::~CBaseObject()
{
	if (m_pChildObjectIndexList)
	{
		delete [] m_pChildObjectIndexList;
		m_pChildObjectIndexList = NULL;
	}
}

