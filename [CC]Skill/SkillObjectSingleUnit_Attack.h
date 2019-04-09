// SkillObjectSingleUnit_Attack.h: interface for the CSkillObjectSingleUnit_Attack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTSINGLEUNIT_ATTACK_H__74016744_0DDD_448B_82BD_723497071122__INCLUDED_)
#define AFX_SKILLOBJECTSINGLEUNIT_ATTACK_H__74016744_0DDD_448B_82BD_723497071122__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectSingleUnit.h"
#include "ValueInSung.h"

class CSkillObjectSingleUnit_Attack : public CSkillObjectSingleUnit  
{
	float m_PhysicalAttack;
	WORD m_Attrib;
	WORD m_AttribAttack;
	float m_AttAttackRate;

	virtual void DoInit(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Operate(SKILLOBJECT_INFO* pSkillObjectInfo,CSkillObjectTargetList* pTargetList,float SkillTreeAmp);

public:

	CSkillObjectSingleUnit_Attack(DWORD SingleUnitNum);
	virtual ~CSkillObjectSingleUnit_Attack();

#ifdef _MHCLIENT_
	virtual void StartSingleUnitEffect(CObject* pOperator,CTargetList* pTList);
#endif
};

#endif // !defined(AFX_SKILLOBJECTSINGLEUNIT_ATTACK_H__74016744_0DDD_448B_82BD_723497071122__INCLUDED_)
