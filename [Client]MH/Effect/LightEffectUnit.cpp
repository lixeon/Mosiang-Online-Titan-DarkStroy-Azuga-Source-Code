// LightEffectUnit.cpp: implementation of the CLightEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LightEffectUnit.h"
#include "LightEffectUnitDesc.h"
#include "..\Object.h"
#include "EffectTriggerMoveUnitDesc.h"

#include "GameResourceManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

float LIGHTING_CLOSING_TIME = 100;

CLightEffectUnit::CLightEffectUnit()
{
}

CLightEffectUnit::~CLightEffectUnit()
{
}

void CLightEffectUnit::Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam)
{
	m_pLight = NULL;
	m_bIsMoving = FALSE;
	m_bIsClosing = FALSE;

	CEffectUnit::Initialize(pEffectUnitDesc,pEffectParam);
}
void CLightEffectUnit::Release()
{
	if(m_pLight)
	{
		delete m_pLight;
		m_pLight = NULL;
	}
	
	CLightEffectUnitDesc::ReleaseUnit(this);
}

void CLightEffectUnit::DoOn(EFFECTPARAM* pParam,TARGETSET* pObject)
{
	CLightEffectUnitDesc* pDesc = (CLightEffectUnitDesc*)m_pEffectUnitDesc;

#ifdef _TESTCLIENT_
	if(GAMERESRCMNGR->m_TestClientInfo.LightEffect == 0)
		return;
#endif

	if(m_pLight == NULL)
		m_pLight = new CEngineLight;

	VECTOR3 pos;
	GetEffectStartPosition(&pos,pObject->pTarget,pParam);

	m_pLight->InitNormalLight(pDesc->m_ColorIndex,pDesc->m_fRadius,&pos);
	
	m_bIsClosing = FALSE;
}
void CLightEffectUnit::DoOff(EFFECTPARAM* pParam,TARGETSET* pObject)
{
	if(m_pLight == NULL)
	{

	}
	else
	{
		m_dwCloseTime = gCurTime;
		m_bIsClosing = TRUE;
	}
}
void CLightEffectUnit::DoMove(EFFECTPARAM* pParam,TARGETSET* pObject,CEffectTriggerMoveUnitDesc* pMoveDesc)
{
	CLightEffectUnitDesc* pDesc = (CLightEffectUnitDesc*)m_pEffectUnitDesc;

	VECTOR3 pos,cpos;
	VRand(&pos,&pMoveDesc->m_ToPosition,&pMoveDesc->m_ToRandom);	// 랜덤 값에 따라 조정해줌
	
	if(pDesc->m_bRelatedCoordinate)		// 상대 좌표로 변환
	{
		float angle = pObject->pTarget->GetAngle();
		TransToRelatedCoordinate(&pos,&pos,angle);
	}

	pObject->pTarget->GetPosition(&cpos);
	m_TargetPos = cpos + pos;
	

	VECTOR3 dir = m_TargetPos - m_CurPosition;
	m_MoveDirection = dir / float(pMoveDesc->m_MoveTime);
	m_EstimatedTime = pMoveDesc->m_MoveTime;

	m_bIsMoving = TRUE;
}

void CLightEffectUnit::DoAttach(EFFECTPARAM* pParam,TARGETSET* pObject,char* AttachPartName)
{
	CLightEffectUnitDesc* pDesc = (CLightEffectUnitDesc*)m_pEffectUnitDesc;

	m_pLight->AttachToObject(pObject->pTarget->GetEngineObject(),AttachPartName);
}

void CLightEffectUnit::DoDetach(EFFECTPARAM* pParam,TARGETSET* pObject)
{
	CLightEffectUnitDesc* pDesc = (CLightEffectUnitDesc*)m_pEffectUnitDesc;
	
	m_pLight->DetachFromObject(pObject->pTarget->GetEngineObject());
}

void CLightEffectUnit::DoProcess(DWORD tickTime,EFFECTPARAM* pParam)
{
	CLightEffectUnitDesc* pDesc = (CLightEffectUnitDesc*)m_pEffectUnitDesc;

	if(m_bIsMoving)
	{
		if(m_EstimatedTime < tickTime)
		{
			m_CurPosition = m_TargetPos;
			m_bIsMoving = FALSE;
		}
		else
		{
			m_CurPosition = m_CurPosition + (m_MoveDirection * float(tickTime));
			m_EstimatedTime -= tickTime;
		}
		

		if(m_pLight)
			m_pLight->SetPosition(&m_CurPosition);
	}

	if(m_bIsClosing)
	{
		DWORD ElapTime = gCurTime - m_dwCloseTime;
		if(ElapTime > LIGHTING_CLOSING_TIME)
		{
			m_bIsClosing = FALSE;
			if(m_pLight)
			{
				m_pLight->Release();
				m_pLight = NULL;
			}
		}
		else
		{
			float rate = 1- ElapTime / float(LIGHTING_CLOSING_TIME);
			float radius = rate * pDesc->m_fRadius;
			if(m_pLight)
				m_pLight->SetRadius(radius);
		}

	}
}