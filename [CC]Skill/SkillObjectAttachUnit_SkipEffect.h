#pragma once
#include "SkillObjectAttachUnit.h"

class CSkillObjectAttachUnit_SkipEffect :
	public CSkillObjectAttachUnit
{
	WORD	m_SkipKind;

public:
	CSkillObjectAttachUnit_SkipEffect(void);
	virtual ~CSkillObjectAttachUnit_SkipEffect(void);

	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	virtual void GetSkipEffectKind(WORD& SkipEffect) { SkipEffect = m_SkipKind; }
};
