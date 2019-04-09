// EffectTriggerOnUnitDesc.cpp: implementation of the CEffectTriggerOnUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerOnUnitDesc.h"
#include "EffectUnit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerOnUnitDesc::CEffectTriggerOnUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{

}

CEffectTriggerOnUnitDesc::~CEffectTriggerOnUnitDesc()
{

}

void CEffectTriggerOnUnitDesc::ParseScript(CMHFile* pFile)
{
}

BOOL CEffectTriggerOnUnitDesc::Operate(CEffect* pEffect)
{
	return pEffect->GetEffectUnit(m_dwUnitNum)->On(pEffect->GetEffectParam());
}
