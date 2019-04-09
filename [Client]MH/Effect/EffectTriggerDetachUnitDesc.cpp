// EffectTriggerDetachUnitDesc.cpp: implementation of the CEffectTriggerDetachUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerDetachUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerDetachUnitDesc::CEffectTriggerDetachUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{
}

CEffectTriggerDetachUnitDesc::~CEffectTriggerDetachUnitDesc()
{

}


void CEffectTriggerDetachUnitDesc::ParseScript(CMHFile* pFile)
{
}
BOOL CEffectTriggerDetachUnitDesc::Operate(CEffect* pEffect)
{
	return pEffect->GetEffectUnit(m_dwUnitNum)->Detach(pEffect->GetEffectParam());
}

