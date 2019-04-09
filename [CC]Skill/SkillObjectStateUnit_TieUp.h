// SkillObjectStateUnit_TieUp.h: interface for the CSkillObjectStateUnit_TieUp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTSTATEUNIT_TIEUP_H__5BE19C5F_8382_4902_A7C4_C2EF39779D82__INCLUDED_)
#define AFX_SKILLOBJECTSTATEUNIT_TIEUP_H__5BE19C5F_8382_4902_A7C4_C2EF39779D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectStateUnit.h"

#define SKILLTIEUP_TYPE_NOTHING		0
#define SKILLTIEUP_TYPE_CANSKILL	1
#define SKILLTIEUP_TYPE_CANMOVE		2
#define SKILLTIEUP_TYPE_TIEDALL		3


class CSkillObjectStateUnit_TieUp : public CSkillObjectStateUnit  
{
	WORD m_TieUpType;
	DWORD m_SkillTime;
public:
	CSkillObjectStateUnit_TieUp();
	virtual ~CSkillObjectStateUnit_TieUp();
	
//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);

	//SW050913 ¼öÁ¤
	virtual void DoStartState(CObject* pObject, CObject* pSkillOperator=NULL);
	virtual void DoEndState(CObject* pObject);
};

#endif // !defined(AFX_SKILLOBJECTSTATEUNIT_TIEUP_H__5BE19C5F_8382_4902_A7C4_C2EF39779D82__INCLUDED_)
