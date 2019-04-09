#include "stdafx.h"
#include "skillobjectattachunit_skipeffect.h"

CSkillObjectAttachUnit_SkipEffect::CSkillObjectAttachUnit_SkipEffect(void)
{
}

CSkillObjectAttachUnit_SkipEffect::~CSkillObjectAttachUnit_SkipEffect(void)
{
}

void CSkillObjectAttachUnit_SkipEffect::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper)
{
	m_SkipKind = pSkillInfo->GetSkipEffectKind();

#ifdef _MHCLIENT_
	m_StateIconNum[1] = eStatusIcon_SkipEffect;
#endif
}