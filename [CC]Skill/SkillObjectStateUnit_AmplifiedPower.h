// SkillObjectStateUnit_AmplifiedPower.h: interface for the CSkillObjectStateUnit_AmplifiedPower class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTSTATEUNIT_AMPLIFIEDPOWER_H__E96EA375_0BBF_4418_880B_91F3832EECB9__INCLUDED_)
#define AFX_SKILLOBJECTSTATEUNIT_AMPLIFIEDPOWER_H__E96EA375_0BBF_4418_880B_91F3832EECB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectStateUnit.h"

class CSkillObjectStateUnit_AmplifiedPower : public CSkillObjectStateUnit  
{	
	DWORD m_SkillTime;
	WORD m_AmplifiedPowerPhy;
	WORD m_AmplifiedPowerAtt;
	float m_AmplifiedPowerAttRate;
	WORD m_Attrib;
	WORD m_Effect;
public:
	CSkillObjectStateUnit_AmplifiedPower();
	virtual ~CSkillObjectStateUnit_AmplifiedPower();
	
//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);

	//SW050913 ¼öÁ¤
	virtual void DoStartState(CObject* pObject, CObject* pSkillOperator=NULL);
	virtual void DoEndState(CObject* pObject);

};

#endif // !defined(AFX_SKILLOBJECTSTATEUNIT_AMPLIFIEDPOWER_H__E96EA375_0BBF_4418_880B_91F3832EECB9__INCLUDED_)
