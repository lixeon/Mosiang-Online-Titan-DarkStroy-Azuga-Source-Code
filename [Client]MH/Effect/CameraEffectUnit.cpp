// CameraEffectUnit.cpp: implementation of the CCameraEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CameraEffectUnit.h"
#include "CameraEffectUnitDesc.h"
#include "..\ObjectManager.h"
#include "..\MHCamera.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCameraEffectUnit::CCameraEffectUnit()
{
	
}

CCameraEffectUnit::~CCameraEffectUnit()
{
}

void CCameraEffectUnit::Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam)
{
	CCameraEffectUnitDesc* pDesc = (CCameraEffectUnitDesc*)m_pEffectUnitDesc;

	CAMERA->InitCamera(pDesc->m_CameraNum,pDesc->m_InitAngleX,pDesc->m_InitAngleY,pDesc->m_InitDistance,(CObject*)NULL);

	CEffectUnit::Initialize(pEffectUnitDesc,pEffectParam);
}
void CCameraEffectUnit::Release()
{
	CCameraEffectUnitDesc::ReleaseUnit(this);
}

BOOL CCameraEffectUnit::On(EFFECTPARAM* pParam)
{
	CCameraEffectUnitDesc* pDesc = (CCameraEffectUnitDesc*)m_pEffectUnitDesc;

	CObject* pObject;
	if(pDesc->IsDangledToOperator())	// 오퍼레이터 or 타켓
		pObject = pParam->m_pOperator;
	else
		pObject = pParam->m_pTargetSet[0].pTarget;

	if(pObject->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
		return TRUE;
	
	if((pParam->m_dwFlag & EFFECT_FLAG_HEROATTACK) == FALSE)
		return TRUE;

	CAMERA->SetCameraPivotObject(pDesc->m_CameraNum,pObject);

	if(pDesc->m_bRelatedCoordinate)		// 상대각도로 변환
	{
		float angleDeg = pDesc->m_InitAngleY+RADTODEG(pObject->GetAngle());
		CAMERA->SetTargetAngleY(pDesc->m_CameraNum,angleDeg,0);
	}

	CAMERA->SetCurCamera(pDesc->m_CameraNum);

	return TRUE;
}
BOOL CCameraEffectUnit::Off(EFFECTPARAM* pParam)
{
	CCameraEffectUnitDesc* pDesc = (CCameraEffectUnitDesc*)m_pEffectUnitDesc;

	CObject* pObject;
	if(pDesc->IsDangledToOperator())	// 오퍼레이터 or 타켓
		pObject = pParam->m_pOperator;
	else
		pObject = pParam->m_pTargetSet[0].pTarget;

	if(pObject->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
		return TRUE;	
	
	if(pParam->m_pOperator->GetID() != HERO->GetID() &&
		pParam->m_pTargetSet[0].pTarget->GetID() != HERO->GetID() )		// if He is not Hero, just return.
		return TRUE;


	CAMERA->ChangeToDefaultCamera();

	return TRUE;
}

BOOL CCameraEffectUnit::CameraRotate(EFFECTPARAM* pParam,float fRotateAngle,DWORD Duration)
{
	CCameraEffectUnitDesc* pDesc = (CCameraEffectUnitDesc*)m_pEffectUnitDesc;

	CAMERA->Rotate(pDesc->m_CameraNum,fRotateAngle,Duration);

	return TRUE;
}

BOOL CCameraEffectUnit::CameraAngle(EFFECTPARAM* pParam,float fTargetAngle,DWORD Duration)
{
	CCameraEffectUnitDesc* pDesc = (CCameraEffectUnitDesc*)m_pEffectUnitDesc;

	CAMERA->SetTargetAngleX(pDesc->m_CameraNum,fTargetAngle,Duration);

	return TRUE;
}

BOOL CCameraEffectUnit::CameraZoom(EFFECTPARAM* pParam,float fZoomDistance,DWORD Duration)
{
	CCameraEffectUnitDesc* pDesc = (CCameraEffectUnitDesc*)m_pEffectUnitDesc;

	CAMERA->ZoomDistance(pDesc->m_CameraNum,fZoomDistance,Duration);

	return TRUE;
}

BOOL CCameraEffectUnit::ChangeCamera(EFFECTPARAM* pParam,int Camera,DWORD Duration)
{
	CCameraEffectUnitDesc* pDesc = (CCameraEffectUnitDesc*)m_pEffectUnitDesc;

	CObject* pObject;
	if(pDesc->IsDangledToOperator())	// 오퍼레이터 or 타켓
		pObject = pParam->m_pOperator;
	else
		pObject = pParam->m_pTargetSet[0].pTarget;

	if(pObject->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
		return TRUE;	
	
	if(pParam->m_pOperator->GetID() != HERO->GetID() &&
		pParam->m_pTargetSet[0].pTarget->GetID() != HERO->GetID() )		// if He is not Hero, just return.
		return TRUE;


	CAMERA->ChangeCameraSmooth(Camera,Duration);

	return TRUE;
}

void CCameraEffectUnit::DoProcess(DWORD tickTime,EFFECTPARAM* pParam)
{

}