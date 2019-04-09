// EffectTriggerUnitDesc.cpp: implementation of the CEffectTriggerUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerUnitDesc::CEffectTriggerUnitDesc(DWORD dwTime,DWORD dwUnitNum)
{
	m_dwTime = dwTime;
	m_dwUnitNum = dwUnitNum;
}

CEffectTriggerUnitDesc::~CEffectTriggerUnitDesc()
{

}

BOOL CEffectTriggerUnitDesc::Check(DWORD ElapsedTime)
{
	return ElapsedTime >= m_dwTime;
}
