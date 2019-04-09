// ObjectEffectUnit.cpp: implementation of the CObjectEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectEffectUnit.h"
#include "ObjectEffectUnitDesc.h"
#include "EffectTriggerMoveUnitDesc.h"

#include "..\AppearanceManager.h"
//trustpak 2005/06/27
#include "../ObjectManager.h"
///
#include "Player.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectEffectUnit::CObjectEffectUnit()
{
}

CObjectEffectUnit::~CObjectEffectUnit()
{
}

void CObjectEffectUnit::Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam)
{
	m_bFading = FALSE;
	m_nObjectNum = 0;

	// jsd test
	m_fChangeVel = 0.0f;
	m_fAccel = 0.0f;
	SetVector3( &m_vOriginAngle, 0.0f, 0.0f, 0.0f );
	m_vStartAngle = m_vTotalAngle = m_vChangeAngle = m_vAnglePerTick = m_vOriginAngle;
	m_bTurn = FALSE;
	m_fFlag = 1.0f;
	m_dwTime = 0;

	CEffectUnit::Initialize(pEffectUnitDesc,pEffectParam);
}
void CObjectEffectUnit::Release()
{
	EFFECTOBJECT* pEffObject;
	if(m_nObjectNum)
	{
		for(int n=0;n<m_nObjectNum;++n)
		{
			pEffObject = &m_pEffectObjectArray[n];

			if(pEffObject->m_pAttachedObject)
			{
				if(pEffObject->m_pAttachedObject->IsInited() && pEffObject->m_Object.IsInited())
					pEffObject->m_pAttachedObject->GetEngineObject()->Detach(&pEffObject->m_Object);
			}
			if(pEffObject->m_Object.IsInited())
			{
				pEffObject->m_Object.Release();
			}
		}
	}
	m_nObjectNum = 0;	

	CObjectEffectUnitDesc::ReleaseUnit(this);
}

void CObjectEffectUnit::InitEffect(DWORD nTargetNum,TARGETSET* pTargetArray)
{
	if(m_bInited)
		return;
	if(nTargetNum == 0)
	{
		m_bInited = FALSE;
		return;
	}
	ASSERT(nTargetNum);
	
	m_nObjectNum = nTargetNum;
	CObjectEffectUnitDesc* pDesc = (CObjectEffectUnitDesc*)m_pEffectUnitDesc;
	if(pDesc->m_bOneTargetPos)
		m_nObjectNum = 1;
	
	for(int n=0;n<m_nObjectNum;++n)
		m_pEffectObjectArray[n].Clear();

	m_bFading = FALSE;

	m_bInited = TRUE;
}

void CObjectEffectUnit::MakeEffectUnit(EFFECTOBJECT* pEffObject,VECTOR3* pPos,float angle,
							CObjectEffectUnitDesc* pDesc,EFFECTPARAM* pParam,TARGETSET* pObject)
{
	if( ( !(pParam->m_dwFlag & EFFECT_FLAG_RELATEDTOHERO) &&
		g_EffectOption == EFFECT_OPTION_ONLYHERO) ||
		g_EffectOption == EFFECT_OPTION_NONE )
	{
		return;		
	}

	if(pEffObject->m_Object.IsInited() == FALSE)
	{
		//trustpak 2005/06/27
		DWORD dwEffectOption = OBJECTMGR->GetEffectOption();
				
		// 아래와 같은 조건문은 바람직하지 않지만, 코드의 수정을 최소화 하기 위하여 한번에 작성하였다.
//		if ( ( (eOO_EFFECT_ONLYHERO == dwEffectOption) && (pObject->pTarget == (CObject*)OBJECTMGR->GetHero()) ) ||
//			( (eOO_EFFECT_ONLYHERO == dwEffectOption) && (pParam->m_pOperator == (CObject*)OBJECTMGR->GetHero()) ) )	
		//SW051026 수정
		if( ( eOO_EFFECT_DEFAULT == dwEffectOption ) || //기본 설정이거나
			( ( eOO_EFFECT_ONLYHERO == dwEffectOption ) && //자신 설정이면서
			( pObject->pTarget == (CObject*)HERO || pParam->m_pSkillOperator == (CObject*)HERO || pParam->m_pOperator == (CObject*)HERO ) //자신이 타겟이거나 스킬이나 이펙트 시전자이면
			) )
		{
			char* strObjectName = NULL;
			if(pObject->ResultInfo.bCritical == TRUE)
				strObjectName = pDesc->m_ObjectNameCritical;
			if(strObjectName == NULL)
				strObjectName = pDesc->m_ObjectName;

			if( strcmp(strObjectName,OPERATOR) == 0)
			{
				APPEARANCEMGR->InitAppearanceToEffect(pParam->m_pOperator,&pEffObject->m_Object);
			}
			else if(strcmp(strObjectName,TARGET) == 0) 
			{
				ASSERT(0);
			}
			else
			{
				WORD StartFrame = pDesc->m_StartFrame;
				WORD EndFrame = pDesc->m_EndFrame;

				if(pParam->m_dwFlag & EFFECT_FLAG_LINKTOOBJECT)
					pEffObject->m_Object.Init(strObjectName,pObject->pTarget,eEngineObjectType_SkillObject,StartFrame,EndFrame);
				else
					pEffObject->m_Object.Init(strObjectName,NULL,eEngineObjectType_Effect,StartFrame,EndFrame);			
			}
			
			pEffObject->m_Object.ApplyHeightField(pDesc->m_bApplyHeightField);
			pEffObject->m_Object.SetScale(&pDesc->m_Scale);
			pEffObject->m_Object.SetAlpha(pDesc->m_fAlpha);
			pEffObject->m_Object.SetRepeat(pDesc->m_bRepeat);	
		}
		
		///
	}

	CEngineObject* pCurObject = &pEffObject->m_Object;
	
	pCurObject->Show();
	pCurObject->SetEngObjAngle(angle + pDesc->m_AngleRad);
	pCurObject->SetEngObjPosition(pPos);
	pEffObject->m_CurPosition = *pPos;

	if(pDesc->m_Illusion != 0)
	{
		void* pMtl = NULL;
		if(pDesc->m_IllusionMaterial)
			pMtl = EFFECTMGR->GetIllusionMaterial(pDesc->m_IllusionMaterial);
		pCurObject->InitializeIllusionEffect(pDesc->m_Illusion,pMtl);
		pCurObject->BeginIllusionEffect();
	}
}

void CObjectEffectUnit::DoOn(EFFECTPARAM* pParam,TARGETSET* pObject)
{	
#ifndef _GMTOOL_
	if(pObject->pTarget->GetObjectKind() == eObjectKind_Player)
	{
		// 투명인간은 이펙트 보이지 않음
		//GM은 봄
		if(((CPlayer*)pObject->pTarget)->GetCharacterTotalInfo()->bVisible == FALSE)
			return;
	}
#endif

	CObjectEffectUnitDesc* pDesc = (CObjectEffectUnitDesc*)m_pEffectUnitDesc;

	VECTOR3 pos;
	GetEffectStartPosition(&pos,pObject->pTarget,pParam);

	float angle = 0;
	if(pDesc->m_bRelatedCoordinate == 1)		// 상대 좌표로 변환
	{
		angle = pObject->pTarget->GetAngle();
	}
	else if(pDesc->m_bRelatedCoordinate == 2)	// 타겟한태의 상대좌표지만 각도는 Operator 의 각도를 쓴다.
	{
		angle = pParam->m_pOperator->GetAngle();
	}

	MakeEffectUnit(&m_pEffectObjectArray[m_nCurTargetNum],&pos,angle,pDesc,pParam,pObject);
}

void CObjectEffectUnit::DoOff(EFFECTPARAM* pParam,TARGETSET* pObject)
{
	CObjectEffectUnitDesc* pDesc = (CObjectEffectUnitDesc*)m_pEffectUnitDesc;
	EFFECTOBJECT* pEffObject = &m_pEffectObjectArray[m_nCurTargetNum];
	CEngineObject* pCurObject = &pEffObject->m_Object;

	if(pEffObject->m_pAttachedObject)
	{
		if(pEffObject->m_pAttachedObject->IsInited() && pCurObject)
			pEffObject->m_pAttachedObject->GetEngineObject()->Detach(pCurObject);
	}
	if(pCurObject)
	{
		if(pDesc->m_Illusion != 0)
		{
			pCurObject->EndIllusionEffect();
		}
		pCurObject->Hide();
	}
}

void CObjectEffectUnit::DoMove(EFFECTPARAM* pParam,TARGETSET* pObject,CEffectTriggerMoveUnitDesc* pMoveDesc)
{
	CObjectEffectUnitDesc* pDesc = (CObjectEffectUnitDesc*)m_pEffectUnitDesc;

	EFFECTOBJECT* pEffObject = &m_pEffectObjectArray[m_nCurTargetNum];
	CEngineObject* pCurObject = &pEffObject->m_Object;
	
	VECTOR3 pos,cpos;
	VRand(&pos,&pMoveDesc->m_ToPosition,&pMoveDesc->m_ToRandom);	// 랜덤 값에 따라 조정해줌
	
	if(pDesc->m_bRelatedCoordinate)		// 상대 좌표로 변환
	{
		float angle = pObject->pTarget->GetAngle();
		TransToRelatedCoordinate(&pos,&pos,angle);
	}
	
	if(pMoveDesc->m_bMainTarget == 0)
	{	// 보통으로 이동하는 경우
		pObject->pTarget->GetPosition(&cpos);
	}
	else if(pMoveDesc->m_bMainTarget == 1)
	{	// 메인타겟 중심으로 이동하는 경우
		MAINTARGET* pMainTarget = pParam->GetMainTarget();
		GetMainTargetPos(pMainTarget,&cpos,NULL);
	}
	else if(pMoveDesc->m_bMainTarget == 2)
	{	// 주인공쪽으로 이동하는 경우
		if(pParam->m_pOperator->IsInited())
			pParam->m_pOperator->GetPosition(&cpos);
		else
			SetVector3(&cpos,0,0,0);
	}
	else if(pMoveDesc->m_bMainTarget == 3)
	{	// 이펙트 자신의 위치를 기준으로
		cpos = pEffObject->m_CurPosition;
	}
	else
	{
		ASSERT(0);
	}

	pEffObject->m_TargetPos = cpos + pos;	

	VECTOR3 dir = pEffObject->m_TargetPos - pEffObject->m_CurPosition;
	pEffObject->m_MoveDirection = dir / float(pMoveDesc->m_MoveTime);
	pEffObject->m_EstimatedTime = pMoveDesc->m_MoveTime;

	pEffObject->m_MoveMode = OBJEFFECT_NORMALMOVE;

	if(pMoveDesc->m_bTurn == FALSE)
		pEffObject->m_MoveMode |= OBJEFFECT_FIXANGLEANGLE;
}

void CObjectEffectUnit::DoGravityMove(EFFECTPARAM* pParam,TARGETSET* pObject,CEffectTriggerGravityMoveUnitDesc* pGravityMoveDesc)
{
	CObjectEffectUnitDesc* pDesc = (CObjectEffectUnitDesc*)m_pEffectUnitDesc;
	
	EFFECTOBJECT* pEffObject = &m_pEffectObjectArray[m_nCurTargetNum];
	pEffObject->m_MoveMode = OBJEFFECT_GRAVITYMOVE;

	pGravityMoveDesc->CalcGravityMoveInfo(pObject->pTarget, pParam->m_pOperator, pParam->GetMainTarget(),&pEffObject->m_GravityMoveInfo);

	//
	pEffObject->m_Object.SetEngObjPosition(&pEffObject->m_GravityMoveInfo.m_InitPos);
	pEffObject->m_CurPosition = pEffObject->m_GravityMoveInfo.m_InitPos;
	
	if(pGravityMoveDesc->m_bTurn == FALSE)
		pEffObject->m_MoveMode |= OBJEFFECT_FIXANGLEANGLE;
}

void CObjectEffectUnit::DoAttach(EFFECTPARAM* pParam,TARGETSET* pObject,char* AttachPartName)
{
	CObjectEffectUnitDesc* pDesc = (CObjectEffectUnitDesc*)m_pEffectUnitDesc;

	EFFECTOBJECT* pEffObject = &m_pEffectObjectArray[m_nCurTargetNum];
	CEngineObject* pCurObject = &pEffObject->m_Object;

	ASSERT(pCurObject);

	if(pEffObject->m_pAttachedObject)
	{
		pEffObject->m_pAttachedObject->GetEngineObject()->Detach(pCurObject);
	}
		
	pCurObject->Attach(pObject->pTarget->GetEngineObject(),AttachPartName);
	pEffObject->m_pAttachedObject = pObject->pTarget;
}

void CObjectEffectUnit::DoDetach(EFFECTPARAM* pParam,TARGETSET* pObject)
{
	CObjectEffectUnitDesc* pDesc = (CObjectEffectUnitDesc*)m_pEffectUnitDesc;

	EFFECTOBJECT* pEffObject = &m_pEffectObjectArray[m_nCurTargetNum];
	CEngineObject* pCurObject = &pEffObject->m_Object;

	ASSERT(pCurObject);

	pObject->pTarget->GetEngineObject()->Detach(pCurObject);

	pEffObject->m_pAttachedObject = NULL;
}

void CObjectEffectUnit::DoLink(EFFECTPARAM* pParam,TARGETSET* pObject)
{
	CObjectEffectUnitDesc* pDesc = (CObjectEffectUnitDesc*)m_pEffectUnitDesc;
	if(pParam->m_dwFlag & EFFECT_FLAG_ABSOLUTEPOS)
		return;

	EFFECTOBJECT* pEffObject = &m_pEffectObjectArray[m_nCurTargetNum];
	CEngineObject* pCurObject = &pEffObject->m_Object;

	ASSERT(pCurObject);

	pEffObject->m_bLinked = TRUE;

	VECTOR3 Pos;
	pObject->pTarget->GetPosition(&Pos);
	pEffObject->m_Object.SetEngObjPosition(&Pos);

	if((pParam->m_dwFlag & EFFECT_FLAG_ABSOLUTEPOS) == FALSE)
	{
		pEffObject->m_Object.LinkTo(pObject->pTarget->GetEngineObject());
	}
}

void CObjectEffectUnit::DoFade(EFFECTPARAM* pParam,TARGETSET* pObject,DWORD FadeTime,float Start,float End)
{
	m_bFading = TRUE;
	m_FadeStartTime = gCurTime;
	m_FadeDuration = FadeTime;
	m_FadeStart = Start;
	m_FadeEnd = End;
}

void CObjectEffectUnit::DoAnimate(EFFECTPARAM* pParam,TARGETSET* pObject,int AnimationNum,BOOL bRepeat)
{
	EFFECTOBJECT* pEffObject = &m_pEffectObjectArray[m_nCurTargetNum];
	CEngineObject* pCurObject = &pEffObject->m_Object;
	
	ASSERT(pCurObject);

	pCurObject->ChangeMotion(AnimationNum,bRepeat);
}

void CObjectEffectUnit::DoProcess(DWORD tickTime,EFFECTPARAM* pParam)
{
	CObjectEffectUnitDesc* pDesc = (CObjectEffectUnitDesc*)m_pEffectUnitDesc;

	EFFECTOBJECT* pEffObject;
	CEngineObject* pCurObject;

	float Alpha = 1;
	if(m_bFading)
	{
		DWORD elap = gCurTime - m_FadeStartTime;
		if(elap > m_FadeDuration)
			Alpha = m_FadeEnd;
		else
		{
			float gap = m_FadeEnd - m_FadeStart;
			Alpha = m_FadeStart + gap*(elap/((float)m_FadeDuration));
		}
	}

	for(int n=0;n<m_nObjectNum;++n)
	{		
		pEffObject = &m_pEffectObjectArray[n];
		pCurObject = &pEffObject->m_Object;

		if(pCurObject)
		{
			if(m_bFading)
				pCurObject->SetAlpha(Alpha);
		}

		if(pEffObject->m_MoveMode != OBJEFFECT_NOTMOVE)
		{
			EffectMoveProcess(pParam,pEffObject,tickTime);
		}
	}
}


void CObjectEffectUnit::EffectMoveProcess(EFFECTPARAM* pParam,EFFECTOBJECT* pEffObject,DWORD tickTime)
{
	CEngineObject* pCurObject = &pEffObject->m_Object;
	VECTOR3 lastpos = pEffObject->m_CurPosition;
	if(pEffObject->m_MoveMode & OBJEFFECT_GRAVITYMOVE)
	{		
		GRAVITYMOVEINFO* pGravityMoveInfo = &pEffObject->m_GravityMoveInfo;	
		VECTOR3 targetpos;
		BOOL rtGetEffectPos = pGravityMoveInfo->m_Target.GetEffectPos(
			pGravityMoveInfo->m_pTargetObj,
			pParam->m_pOperator,
			pParam->GetMainTarget(),
			&targetpos);
		if(rtGetEffectPos)
		{
			float dt = tickTime*0.001f;
			float remaintime = pGravityMoveInfo->m_RemainTime;
			if(pGravityMoveInfo->m_RemainTime < dt)
			{
				pEffObject->m_MoveMode = OBJEFFECT_NOTMOVE;
				pEffObject->m_CurPosition = targetpos;
			}
			else
			{
				// calc accel
				VECTOR3 accel = targetpos - pEffObject->m_CurPosition;
				float dist = VECTOR3Length(&accel);
				float a = dist * 2 / (remaintime*remaintime);
				accel = accel/dist*a;
				
				// v = v0 + (accel * dt)
				pGravityMoveInfo->m_Velocity = pGravityMoveInfo->m_Velocity + (accel * dt);
				
				// s = s0 + (v * dt)
				pEffObject->m_CurPosition = 
					pEffObject->m_CurPosition + (pGravityMoveInfo->m_Velocity * dt);
				
				pGravityMoveInfo->m_RemainTime -= dt;
				
				static float fColTime = 0.5f;
				if(pGravityMoveInfo->m_RemainTime < fColTime)
				{
					float rate = pGravityMoveInfo->m_RemainTime / fColTime;
					pEffObject->m_CurPosition = 
						pEffObject->m_CurPosition*rate +
						targetpos*(1-rate);
				}
			}	
		}
		else
		{
			pCurObject->Hide();
		}
	}
	else if(pEffObject->m_MoveMode & OBJEFFECT_NORMALMOVE)
	{
		VECTOR3 lastpos;
		lastpos = pEffObject->m_CurPosition;
		if(pEffObject->m_EstimatedTime < tickTime)
		{
			pEffObject->m_CurPosition = pEffObject->m_TargetPos;
			pEffObject->m_MoveMode = OBJEFFECT_NOTMOVE;
		}
		else
		{
			pEffObject->m_CurPosition = pEffObject->m_CurPosition + (pEffObject->m_MoveDirection * float(tickTime));
			pEffObject->m_EstimatedTime -= tickTime;
		}		
	}
	
	if(pCurObject)
	{
		if((pEffObject->m_MoveMode & OBJEFFECT_FIXANGLEANGLE) == FALSE)
		{
			VECTOR3 dir = pEffObject->m_CurPosition - lastpos;
			pCurObject->SetDirection(&dir);
		}
		pCurObject->SetEngObjPosition(&pEffObject->m_CurPosition);
	}
}