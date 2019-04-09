// SkillObjectFirstUnit_Recover.h: interface for the CSkillObjectFirstUnit_Recover class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTFIRSTUNIT_RECOVER_H__01840E05_9944_4621_AFD4_9223F9E29050__INCLUDED_)
#define AFX_SKILLOBJECTFIRSTUNIT_RECOVER_H__01840E05_9944_4621_AFD4_9223F9E29050__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectFirstUnit.h"

class CSkillObjectFirstUnit_Recover : public CSkillObjectFirstUnit  
{
	WORD m_RecoverLife;
	WORD m_RecoverNaeRyuk;
	
public:
	CSkillObjectFirstUnit_Recover(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectFirstUnit_Recover();

	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
#ifdef _MAPSERVER_
	virtual int ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,
							MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp);
#endif

	
#ifdef _MHCLIENT_
	virtual void StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget);
	virtual void SetTargetArray(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget);
#endif
};

#endif // !defined(AFX_SKILLOBJECTFIRSTUNIT_RECOVER_H__01840E05_9944_4621_AFD4_9223F9E29050__INCLUDED_)
