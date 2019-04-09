// SkillObjectSingleUnit.cpp: implementation of the CSkillObjectSingleUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectSingleUnit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectSingleUnit::CSkillObjectSingleUnit(DWORD SingleUnitNum)
{
	m_SingleUnitNum = SingleUnitNum;
}

CSkillObjectSingleUnit::~CSkillObjectSingleUnit()
{

}

void CSkillObjectSingleUnit::Init(CSkillInfo* pSkillInfo,WORD SkillLevel)
{
	m_OperateCount = 0;
	m_Interval = pSkillInfo->GetInterval();
	DoInit(pSkillInfo,SkillLevel);
}

void CSkillObjectSingleUnit::Update(SKILLOBJECT_INFO* pSkillObjectInfo,
									CSkillObjectTargetList* pTargetList,float SkillTreeAmp)
{
	if(m_Interval == 0)
		return;
	
	DWORD ElapsedTime = gCurTime - pSkillObjectInfo->StartTime;
	DWORD count = ElapsedTime / m_Interval;
	if(m_OperateCount != count)
	{
		ASSERT(m_OperateCount < count);
		Operate(pSkillObjectInfo,pTargetList,SkillTreeAmp);
		m_OperateCount = count;
	}
}
