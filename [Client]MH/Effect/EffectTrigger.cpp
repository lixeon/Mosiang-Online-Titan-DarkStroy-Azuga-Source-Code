// EffectTrigger.cpp: implementation of the CEffectTrigger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTrigger.h"
#include "EffectUnit.h"
#include "Effect.h"
#include "EffectTriggerUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTrigger::CEffectTrigger()
{
}

CEffectTrigger::~CEffectTrigger()
{
}

void CEffectTrigger::Init(int TriggerNum,CEffectTriggerUnitDesc** ppEffectTriggerUnitArray)
{
	m_TriggerUnitNum = TriggerNum;
	if(m_TriggerUnitNum > MAX_TRIGGER_NUM)
	{
		MessageBox(NULL,"MaxTriggerNum must be less than 32",0,0);
	}
	m_ppEffectTriggerUnitArray = ppEffectTriggerUnitArray;
	memset(m_pExecuted,0,sizeof(bool)*MAX_TRIGGER_NUM);
}

void CEffectTrigger::Release()
{

}

void CEffectTrigger::Process(DWORD ElapseTime,CEffect* pEffect)
{
	for(int n=0;n<m_TriggerUnitNum;++n)
	{
		if(m_pExecuted[n] == TRUE)
			continue;

		if(m_ppEffectTriggerUnitArray[n] == NULL)
		{
			m_pExecuted[n] = TRUE;
			continue;
		}

		if(m_ppEffectTriggerUnitArray[n]->Check(ElapseTime))
		{
			m_pExecuted[n] = m_ppEffectTriggerUnitArray[n]->Operate(pEffect) ? true : false;
		}
	}
}

void CEffectTrigger::ExecuteAll(CEffect* pEffect)
{
	for(int n=0;n<m_TriggerUnitNum;++n)
	{
		if(m_pExecuted[n] == TRUE)
			continue;

		if(m_ppEffectTriggerUnitArray[n] == NULL)
		{
			m_pExecuted[n] = TRUE;
			continue;
		}
		m_pExecuted[n] = m_ppEffectTriggerUnitArray[n]->Operate(pEffect) ? true : false;
	}
//kes****	
//	for(n=0;n<m_TriggerUnitNum;++n)
//	{
//		if(m_pExecuted[n] == FALSE)
//			ASSERT(0);
//	}
//****
}

void CEffectTrigger::Reset()
{
	for(int n=0;n<m_TriggerUnitNum;++n)
	{
		m_pExecuted[n] = FALSE;
	}
}