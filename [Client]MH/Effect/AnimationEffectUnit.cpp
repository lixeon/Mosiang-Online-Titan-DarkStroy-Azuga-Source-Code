// AnimationEffectUnit.cpp: implementation of the CAnimationEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AnimationEffectUnit.h"
#include "AnimationEffectUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnimationEffectUnit::CAnimationEffectUnit()
{

}

CAnimationEffectUnit::~CAnimationEffectUnit()
{

}

void CAnimationEffectUnit::DoOn(EFFECTPARAM* pParam,TARGETSET* pObject)
{
	if(pObject->pTarget->GetState() == eObjectState_Die)
	{
		if((pParam->m_dwFlag & EFFECT_FLAG_DIE) == FALSE)
			return;
	}

	CAnimationEffectUnitDesc* pDesc = (CAnimationEffectUnitDesc*)m_pEffectUnitDesc;

	pObject->pTarget->ChangeMotion(1,FALSE);
	if(pDesc->m_CustumMotion)
		pObject->pTarget->GetEngineObject()->ChangeCustumMotion(pDesc->m_MotionNum,
								pDesc->m_StartFrame,pDesc->m_EndFrame,pDesc->m_bRepeat);
	else
		pObject->pTarget->ChangeMotion(pDesc->m_MotionNum,pDesc->m_bRepeat);
}

void CAnimationEffectUnit::DoSetBaseMotion(EFFECTPARAM* pParam,TARGETSET* pObject,int BaseAnimationNum)
{
	if(pObject->pTarget->GetState() == eObjectState_Die)
	{
		if((pParam->m_dwFlag & EFFECT_FLAG_DIE) == FALSE)
			return;
	}
	
	pObject->pTarget->ChangeBaseMotion(BaseAnimationNum);
}

void CAnimationEffectUnit::Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam)
{

	CEffectUnit::Initialize(pEffectUnitDesc,pEffectParam);
}
void CAnimationEffectUnit::Release()
{
	CAnimationEffectUnitDesc::ReleaseUnit(this);
}