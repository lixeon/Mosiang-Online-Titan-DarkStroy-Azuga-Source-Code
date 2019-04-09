// SkillObjectAttachUnit_Dodge1.h: interface for the CSkillObjectAttachUnit_Dodge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTATTACHUNIT_DODGE1_H__4C5FC0B4_547A_4B22_BE34_92EE62ACA38B__INCLUDED_)
#define AFX_SKILLOBJECTATTACHUNIT_DODGE1_H__4C5FC0B4_547A_4B22_BE34_92EE62ACA38B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectAttachUnit.h"

class CSkillObjectAttachUnit_Dodge : public CSkillObjectAttachUnit  
{
	float m_fDodgeRate;
public:
	CSkillObjectAttachUnit_Dodge();
	virtual ~CSkillObjectAttachUnit_Dodge();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetDodgeRate(float Original,float& CalcedUp,float& CalcedDown);

};

#endif // !defined(AFX_SKILLOBJECTATTACHUNIT_DODGE1_H__4C5FC0B4_547A_4B22_BE34_92EE62ACA38B__INCLUDED_)
