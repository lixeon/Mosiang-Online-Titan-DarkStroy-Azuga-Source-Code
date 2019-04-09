#pragma once
#include "SkillObjectFirstUnit.h"
#include "ValueInSung.h"

////////////////////////////////////////////////////////////////////////////////////
// 06. 06. 2차 전직 - 이영준
// 상태변화

class CSkillObjectFirstUnit_SingleSpecialState :
	public CSkillObjectFirstUnit
{
	WORD m_State;

#ifdef _MAPSERVER_
	int SendAck(CObject* pOperator, WORD State, bool bVal);
	int SendNack(CObject* pOperator, WORD State, WORD Err);
#endif

public:
	CSkillObjectFirstUnit_SingleSpecialState(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectFirstUnit_SingleSpecialState(void);

	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
#ifdef _MAPSERVER_
	virtual int ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,
							MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp);
#endif
	
#ifdef _MHCLIENT_
	virtual void StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget);
#endif
};
