// SkillObjectSingleUnit_Recharge.h: interface for the CSkillObjectSingleUnit_Recharge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTSINGLEUNIT_RECHARGE_H__E0E0C19A_24A7_4EDF_857A_1430E7FB1B72__INCLUDED_)
#define AFX_SKILLOBJECTSINGLEUNIT_RECHARGE_H__E0E0C19A_24A7_4EDF_857A_1430E7FB1B72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectSingleUnit.h"
#include "ValueInSung.h"

class CSkillObjectSingleUnit_Recharge : public CSkillObjectSingleUnit  
{
	WORD m_Recharge;
	WORD m_Attrib;

	virtual void DoInit(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Operate(SKILLOBJECT_INFO* pSkillObjectInfo,CSkillObjectTargetList* pTargetList,float SkillTreeAmp);

public:
	CSkillObjectSingleUnit_Recharge(DWORD SingleUnitNum);
	virtual ~CSkillObjectSingleUnit_Recharge();

#ifdef _MHCLIENT_
	virtual void StartSingleUnitEffect(CObject* pOperator,CTargetList* pTList);
#endif
};

#endif // !defined(AFX_SKILLOBJECTSINGLEUNIT_RECHARGE_H__E0E0C19A_24A7_4EDF_857A_1430E7FB1B72__INCLUDED_)
