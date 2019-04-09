// SkillObjectAttachUnit_MoveSpeed.h: interface for the CSkillObjectAttachUnit_MoveSpeed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTATTACHUNIT_MOVESPEED_H__26C373DA_3A51_4352_8BF7_23182FB1FD4C__INCLUDED_)
#define AFX_SKILLOBJECTATTACHUNIT_MOVESPEED_H__26C373DA_3A51_4352_8BF7_23182FB1FD4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectAttachUnit.h"

class CSkillObjectAttachUnit_MoveSpeed : public CSkillObjectAttachUnit  
{
	WORD m_MoveSpeedDown;
	WORD m_MoveSpeedUp;
	
//	float m_Probability;

public:
	CSkillObjectAttachUnit_MoveSpeed();
	virtual ~CSkillObjectAttachUnit_MoveSpeed();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetMoveSpeed(float Original,float& CalcedUp,float& CalcedDown);
};

#endif // !defined(AFX_SKILLOBJECTATTACHUNIT_MOVESPEED_H__26C373DA_3A51_4352_8BF7_23182FB1FD4C__INCLUDED_)
