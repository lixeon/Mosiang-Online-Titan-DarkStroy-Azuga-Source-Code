// SkillObjectStateUnit_DummyState.h: interface for the CSkillObjectStateUnit_DummyState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTSTATEUNIT_DUMMYSTATE_H__9A9C520E_5AE0_47D1_870D_AE19E669F381__INCLUDED_)
#define AFX_SKILLOBJECTSTATEUNIT_DUMMYSTATE_H__9A9C520E_5AE0_47D1_870D_AE19E669F381__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectStateUnit.h"

class CSkillObjectStateUnit_DummyState : public CSkillObjectStateUnit  
{
	DWORD m_SkillTime;
public:
	CSkillObjectStateUnit_DummyState();
	virtual ~CSkillObjectStateUnit_DummyState();
	
//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);

	//SW050913 ¼öÁ¤
	virtual void DoStartState(CObject* pObject, CObject* pSkillOperator=NULL);
	virtual void DoEndState(CObject* pObject);
};

#endif // !defined(AFX_SKILLOBJECTSTATEUNIT_DUMMYSTATE_H__9A9C520E_5AE0_47D1_870D_AE19E669F381__INCLUDED_)
