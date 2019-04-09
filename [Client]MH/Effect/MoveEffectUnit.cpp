// MoveEffectUnit.cpp: implementation of the CMoveEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoveEffectUnit.h"
#include "MoveEffectUnitDesc.h"
#include "..\MoveManager.h"
#include "..\ObjectManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMoveEffectUnit::CMoveEffectUnit()
{

}

CMoveEffectUnit::~CMoveEffectUnit()
{

}

void CMoveEffectUnit::Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam)
{

	CEffectUnit::Initialize(pEffectUnitDesc,pEffectParam);
}
void CMoveEffectUnit::Release()
{
	CMoveEffectUnitDesc::ReleaseUnit(this);
}

void CMoveEffectUnit::DoOn(EFFECTPARAM* pParam,TARGETSET* pObject)
{
	if(pObject->pTarget->GetState() == eObjectState_Die)
	{
		if((pParam->m_dwFlag & EFFECT_FLAG_DIE) == FALSE)
			return;
	}
	if(pObject->pTarget->GetState() ==  eObjectState_Engrave )
		return;
	if(pObject->pTarget->GetObjectKind() & eObjectKind_MapObject )
		return;

	CMoveEffectUnitDesc* pDesc = (CMoveEffectUnitDesc*)m_pEffectUnitDesc;

	if(pDesc->m_bYMove)	// yÃà ÀÌµ¿
	{
		MOVEMGR->EffectYMove(pObject->pTarget,
			pDesc->m_Position.x,pDesc->m_Position.y,
			pDesc->m_Duration);
	}
	else				// ±×³É ÀÌµ¿
	{
		VECTOR3 pos,cpos;
		VRand(&pos,&pDesc->m_Position,&pDesc->m_RandomPos);
		
		if(pDesc->m_bApplyWeight)
			pos = pos * (1/pObject->pTarget->GetWeight()) * gAntiGravity;
		
		if(pDesc->m_bRelatedCoordinate)		// »ó´ë ÁÂÇ¥·Î º¯È¯
		{
			float angle = pObject->pTarget->GetAngle();
			if(pDesc->IsDangledToOperator() == FALSE)
			if(pParam->m_pOperator)
			{
				angle = pParam->m_pOperator->GetAngle();
				angle += gPHI;
			}
			TransToRelatedCoordinate(&pos,&pos,angle);
		}
				
		pObject->pTarget->GetPosition(&cpos);
		pos = cpos + pos;
		
		if(pDesc->m_bTurnChar)
		{
			MOVEMGR->SetLookatPos(pObject->pTarget,&pos,0,gCurTime);
		}
		
		MOVEMGR->HeroEffectMove(pObject->pTarget,&pos,pDesc->m_Duration,pParam->m_pOperator);
	}
}

void CMoveEffectUnit::DoOff(EFFECTPARAM* pParam,TARGETSET* pObject)
{

}

void CMoveEffectUnit::DoProcess(DWORD tickTime,EFFECTPARAM* pParam)
{
	
}
