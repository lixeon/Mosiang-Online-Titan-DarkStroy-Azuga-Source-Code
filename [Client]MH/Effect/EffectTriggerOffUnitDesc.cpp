// EffectTriggerOffUnitDesc.cpp: implementation of the CEffectTriggerOffUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerOffUnitDesc.h"
#include "EffectUnit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerOffUnitDesc::CEffectTriggerOffUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{

}

CEffectTriggerOffUnitDesc::~CEffectTriggerOffUnitDesc()
{

}


void CEffectTriggerOffUnitDesc::ParseScript(CMHFile* pFile)
{
}
BOOL CEffectTriggerOffUnitDesc::Operate(CEffect* pEffect)
{
	return pEffect->GetEffectUnit(m_dwUnitNum)->Off(pEffect->GetEffectParam());	
}