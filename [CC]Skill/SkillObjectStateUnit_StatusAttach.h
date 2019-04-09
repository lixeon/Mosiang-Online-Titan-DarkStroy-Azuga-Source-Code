// SkillObjectStateUnit_StatusAttach.h: interface for the CSkillObjectStateUnit_StatusAttach class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTSTATEUNIT_STATUSATTACH_H__0B2F8411_8FAA_47C9_9242_B8057F7078EB__INCLUDED_)
#define AFX_SKILLOBJECTSTATEUNIT_STATUSATTACH_H__0B2F8411_8FAA_47C9_9242_B8057F7078EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectStateUnit.h"

class CSkillObjectAttachUnit;

class CSkillObjectStateUnit_StatusAttach : public CSkillObjectStateUnit  
{
	CSkillObjectAttachUnit* m_pAttachUnit;
	DWORD m_SkillTime;

public:
	CSkillObjectStateUnit_StatusAttach();
	virtual ~CSkillObjectStateUnit_StatusAttach();
	
//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);

	void SetAttachUnit(CSkillObjectAttachUnit* pAttachUnit,WORD PNTarget);

	//SW050913 ¼öÁ¤
	virtual void DoStartState(CObject* pObject, CObject* pSkillOperator=NULL);
	virtual void DoEndState(CObject* pObject);
};

#endif // !defined(AFX_SKILLOBJECTSTATEUNIT_STATUSATTACH_H__0B2F8411_8FAA_47C9_9242_B8057F7078EB__INCLUDED_)
