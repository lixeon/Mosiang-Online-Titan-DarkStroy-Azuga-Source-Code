// SkillObjectAttachUnit_CounterAttack.h: interface for the CSkillObjectAttachUnit_CounterAttack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTATTACHUNIT_COUNTERATTACK_H__5EE32B49_A360_489C_BDB9_9F6E74965698__INCLUDED_)
#define AFX_SKILLOBJECTATTACHUNIT_COUNTERATTACK_H__5EE32B49_A360_489C_BDB9_9F6E74965698__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectAttachUnit.h"

class CSkillObjectAttachUnit_CounterAttack : public CSkillObjectAttachUnit  
{
	WORD m_Attrib;
	float m_AttrCounterRate;
	float m_PhyCounterRate;
public:
	CSkillObjectAttachUnit_CounterAttack();
	virtual ~CSkillObjectAttachUnit_CounterAttack();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetCounterAttackDamage(WORD Attrib,DWORD AttrDamage,DWORD PhyDamage,RESULTINFO* pDamageInfo,DWORD& CounterDamage);
};

#endif // !defined(AFX_SKILLOBJECTATTACHUNIT_COUNTERATTACK_H__5EE32B49_A360_489C_BDB9_9F6E74965698__INCLUDED_)
