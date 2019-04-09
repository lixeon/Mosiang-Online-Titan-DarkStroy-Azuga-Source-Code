// EffectTriggerLinkUnitDesc.cpp: implementation of the CEffectTriggerLinkUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerLinkUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerLinkUnitDesc::CEffectTriggerLinkUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{

}

CEffectTriggerLinkUnitDesc::~CEffectTriggerLinkUnitDesc()
{

}


void CEffectTriggerLinkUnitDesc::ParseScript(CMHFile* pFile)
{
	
}
BOOL CEffectTriggerLinkUnitDesc::Operate(CEffect* pEffect)
{	
	return pEffect->GetEffectUnit(m_dwUnitNum)->Link(pEffect->GetEffectParam());
	
	return TRUE;
}
