// SkillObjectAttachUnit_Immune.h: interface for the CSkillObjectAttachUnit_Immune class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTATTACHUNIT_IMMUNE_H__7D5B9531_D064_4222_9B35_7DB300824A42__INCLUDED_)
#define AFX_SKILLOBJECTATTACHUNIT_IMMUNE_H__7D5B9531_D064_4222_9B35_7DB300824A42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectAttachUnit.h"

class CSkillObjectAttachUnit_Immune : public CSkillObjectAttachUnit  
{
public:
	CSkillObjectAttachUnit_Immune();
	virtual ~CSkillObjectAttachUnit_Immune();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetDamage(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
};

#endif // !defined(AFX_SKILLOBJECTATTACHUNIT_IMMUNE_H__7D5B9531_D064_4222_9B35_7DB300824A42__INCLUDED_)
