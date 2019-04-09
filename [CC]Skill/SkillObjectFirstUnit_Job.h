#ifndef _SKILLOBJECT_FIRSTUNIT_JOB_H
#define _SKILLOBJECT_FIRSTUNIT_JOB_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectFirstUnit.h"
#include "ValueInSung.h"

class CSkillObjectFirstUnit_Job : public CSkillObjectFirstUnit
{
	CSkillInfo* m_pSkillInfo;
#ifdef _MAPSERVER_
	BOOL m_bJobSkillSuccess;
	BOOL m_bJobSkillFail;
	CPlayer* m_pPlayer;
	CMonster* m_pMonster;
#endif

public:
	CSkillObjectFirstUnit_Job(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectFirstUnit_Job();

	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);

#ifdef _MAPSERVER_
	virtual int ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,
		MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp);
	virtual void FirstUnitResult();	
#endif

#ifdef _MHCLIENT_
	virtual void StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget);
#endif
};

#endif _SKILLOBJECT_FIRSTUNIT_JOB_H
