// SkillObjectSingleUnit_Heal.h: interface for the CSkillObjectSingleUnit_Heal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTSINGLEUNIT_HEAL_H__24A259B4_A3D8_4756_BB56_A14FA3B7FD60__INCLUDED_)
#define AFX_SKILLOBJECTSINGLEUNIT_HEAL_H__24A259B4_A3D8_4756_BB56_A14FA3B7FD60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectSingleUnit.h"
#include "ValueInSung.h"

class CSkillObjectSingleUnit_Heal : public CSkillObjectSingleUnit  
{
	WORD m_Heal;
	WORD m_Attrib;

	virtual void DoInit(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Operate(SKILLOBJECT_INFO* pSkillObjectInfo,CSkillObjectTargetList* pTargetList,float SkillTreeAmp);

public:
	CSkillObjectSingleUnit_Heal(DWORD SingleUnitNum);
	virtual ~CSkillObjectSingleUnit_Heal();

#ifdef _MHCLIENT_
	virtual void StartSingleUnitEffect(CObject* pOperator,CTargetList* pTList);
#endif
};

#endif // !defined(AFX_SKILLOBJECTSINGLEUNIT_HEAL_H__24A259B4_A3D8_4756_BB56_A14FA3B7FD60__INCLUDED_)
