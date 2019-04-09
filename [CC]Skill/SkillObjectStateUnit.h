// SkillObjectStateUnit.h: interface for the CSkillObjectStateUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTSTATEUNIT_H__34F8AAF7_94B4_479A_9AB4_2AA623C8E491__INCLUDED_)
#define AFX_SKILLOBJECTSTATEUNIT_H__34F8AAF7_94B4_479A_9AB4_2AA623C8E491__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillInfo.h"

class CSkillObject;
class CSkillObjectTargetList;

class CSkillObjectStateUnit  
{
protected:
#ifdef _MHCLIENT_
	WORD m_StateEffectNum[MAX_SKILLMAINTAIN_EFFECT];
	WORD m_StateIconNum[MAX_SKILLMAINTAIN_EFFECT];
#endif

	CSkillObject* m_pSkillObject;
	WORD m_PNTarget;		// 0:Positive 1:Negative
public:
	CSkillObjectStateUnit();
	virtual ~CSkillObjectStateUnit();

	WORD GetPNTarget()	{	return m_PNTarget;	}

	void SetSkillObject(CSkillObject* pSkillObject)	{ m_pSkillObject = pSkillObject;	}

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel) = 0;
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL) = 0;

	void StartState(CSkillObjectTargetList* pTargetList);
	void EndState(CSkillObjectTargetList* pTargetList);
	void StartState(CObject* pObject,BYTE bTargetKind);
	void EndState(CObject* pObject,BYTE bTargetKind);

	//SW050913 ¼öÁ¤
	virtual void DoStartState(CObject* pObject, CObject* pSkillOperator=NULL) = 0;
	virtual void DoEndState(CObject* pObject) = 0;
};

#endif // !defined(AFX_SKILLOBJECTSTATEUNIT_H__34F8AAF7_94B4_479A_9AB4_2AA623C8E491__INCLUDED_)
