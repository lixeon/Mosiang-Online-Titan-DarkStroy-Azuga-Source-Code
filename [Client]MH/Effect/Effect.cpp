// Effect.cpp: implementation of the CEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Effect.h"

#include "EffectUnit.h"
#include "EffectTrigger.h"
#include "EffectDesc.h"
#include "..\Object.h"
#include "EffectUnitDesc.h"

#include "AnimationEffectUnitDesc.h"
#include "EffectTriggerUnitDesc.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void EFFECTPARAM::SetTargetSetArray(TARGETSET* pTargetSetArray,int MaxTargetSet,MAINTARGET* pMainTarget)
{
	m_nTargetNum = MaxTargetSet;
	memcpy(m_pTargetSet,pTargetSetArray,sizeof(TARGETSET)*m_nTargetNum);
	
	m_MainTarget = *pMainTarget;
}


CEffect::CEffect()
{

}

CEffect::~CEffect()
{
}

void CEffect::Init(int maxEffectUnitNum,EFFECTPARAM* pParam,CEffectDesc* pEffectDesc,
				 int TriggerNum,CEffectTriggerUnitDesc** ppEffectTriggerUnitArray)
{
	m_EffectTrigger.Init(TriggerNum,ppEffectTriggerUnitArray);
	
	m_Param.Copy(pParam);

	m_pEffectDesc = pEffectDesc;

	m_MaxEffectUnitNum = maxEffectUnitNum;
	if(m_MaxEffectUnitNum > MAX_EFFECT_UNITNUM)
	{
		MessageBox(NULL,"MaxEffectUnit must be less than 32",0,0);
	}
	memset(m_ppEffectUnitArray,0,sizeof(CEffectUnit*)*m_MaxEffectUnitNum);
	m_EndFlag = FALSE;
	
	m_EffectRefCount = 1;
}

void CEffect::Release()
{
	m_EffectTrigger.Release();
	for(int n=0;n<m_MaxEffectUnitNum;++n)
	{
		if(m_ppEffectUnitArray[n])
			m_ppEffectUnitArray[n]->Release();
	}
}

void CEffect::SetEffectUnit(int num,CEffectUnit* pEffectUnit)
{
	m_ppEffectUnitArray[num] = pEffectUnit;
}

BOOL CEffect::Process(DWORD CurTime)
{
	DWORD ElapsedTime = CurTime - m_Param.m_StartTime;
	m_EffectTrigger.Process(ElapsedTime,this);
	
	for(int n=0;n<m_MaxEffectUnitNum;++n)
	{
		m_ppEffectUnitArray[n]->Process(gTickTime,&m_Param);
	}

	if(ElapsedTime >= m_pEffectDesc->GetEffectEndTime())
	{
		m_EffectTrigger.ExecuteAll(this);			// 아직 실행하지 않은 이펙트가 있다면 실행시킨다.
		return TRUE;		// TRUE 반환하면 EffectManager가 이펙트를 삭제한다.
	}
	else
		return FALSE;
}

void CEffect::SetDamage(TARGETSET* TargetSetArray,int MaxTargetSet,MAINTARGET* pMainTarget)
{
	m_Param.SetTargetSetArray(TargetSetArray,MaxTargetSet,pMainTarget);
		
	for(int n=0;n<m_MaxEffectUnitNum;++n)
	{
		m_ppEffectUnitArray[n]->InitEffect(MaxTargetSet,TargetSetArray);
	}
}

DWORD CEffect::GetEffectEndTime()
{
	return m_pEffectDesc->GetEffectEndTime();
}

void CEffect::Reset()
{
	m_Param.m_StartTime = gCurTime;
	m_EffectTrigger.Reset();
}

BOOL CEffect::IsRepeatEffect()
{
	return m_pEffectDesc->m_bRepeat;
}

DWORD CEffect::GetNextEffect()
{
	return m_pEffectDesc->m_NextEffect;
}

int CEffect::GetEffectKind()
{
	return m_pEffectDesc->GetEffectKind();
}
