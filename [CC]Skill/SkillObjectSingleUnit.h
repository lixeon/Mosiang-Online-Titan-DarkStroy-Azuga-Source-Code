// SkillObjectSingleUnit.h: interface for the CSkillObjectSingleUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTSINGLEUNIT_H__22FC11C7_CB20_4083_9BA1_21856DCCB4E7__INCLUDED_)
#define AFX_SKILLOBJECTSINGLEUNIT_H__22FC11C7_CB20_4083_9BA1_21856DCCB4E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectTargetList.h"
#include "SkillInfo.h"

class CSkillObject;

class CSkillObjectSingleUnit  
{
	DWORD m_SingleUnitNum;
	DWORD m_Interval;
	DWORD m_OperateCount;
	
protected:
	WORD m_PNTarget;		// 0:Positive 1:Negative
	CSkillObject* m_pSkillObject;	

	virtual void DoInit(CSkillInfo* pSkillInfo,WORD SkillLevel)		{}

public:
	CSkillObjectSingleUnit(DWORD SingleUnitNum);
	virtual ~CSkillObjectSingleUnit();

	WORD GetPNTarget()	{	return m_PNTarget;	}
	void SetSkillObject(CSkillObject* pSkillObject)	{ m_pSkillObject = pSkillObject;	}
	
	DWORD GetSingleUnitNum()		{	return m_SingleUnitNum;	}
	
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Release()	{};

	void Update(SKILLOBJECT_INFO* pSkillObjectInfo,CSkillObjectTargetList* pTargetList,float SkillTreeAmp);
	virtual void Operate(SKILLOBJECT_INFO* pSkillObjectInfo,CSkillObjectTargetList* pTargetList,float SkillTreeAmp) = 0;

#ifdef _MHCLIENT_
	virtual void StartSingleUnitEffect(CObject* pOperator,CTargetList* pTList) = 0;
#endif
};

#endif // !defined(AFX_SKILLOBJECTSINGLEUNIT_H__22FC11C7_CB20_4083_9BA1_21856DCCB4E7__INCLUDED_)
