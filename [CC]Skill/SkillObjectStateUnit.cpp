// SkillObjectStateUnit.cpp: implementation of the CSkillObjectStateUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectStateUnit.h"
#include "SkillObjectTargetList.h"
#include "SkillObject_server.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectStateUnit::CSkillObjectStateUnit()
{
	m_pSkillObject = NULL;
	
#ifdef _MHCLIENT_
	for(int n=0;n<MAX_SKILLMAINTAIN_EFFECT;++n)
	{
		m_StateEffectNum[n] = 0;
		m_StateIconNum[n] = 0;
	}
#endif
}

CSkillObjectStateUnit::~CSkillObjectStateUnit()
{

}

void CSkillObjectStateUnit::StartState(CSkillObjectTargetList* pTargetList)
{
	CObject* pTarget;
	BYTE bTargetKind;
	pTargetList->SetPositionHead();
	if(m_PNTarget & SKILLRESULTKIND_POSITIVE)
	{
		while(pTargetList->GetNextPositiveTarget(&pTarget,&bTargetKind))
		{
			DoStartState(pTarget,(CObject*)m_pSkillObject->GetOperator());
		}
	}
	else if(m_PNTarget & SKILLRESULTKIND_NEGATIVE)
	{
		while(pTargetList->GetNextNegativeTarget(&pTarget,&bTargetKind))
		{
			DoStartState(pTarget,(CObject*)m_pSkillObject->GetOperator());
		}
	}
	else
	{
		ASSERT(0);
	}
}
void CSkillObjectStateUnit::EndState(CSkillObjectTargetList* pTargetList)
{
	CObject* pTarget;
	BYTE bTargetKind;
	pTargetList->SetPositionHead();
	if(m_PNTarget & SKILLRESULTKIND_POSITIVE)
	{
		while(pTargetList->GetNextPositiveTarget(&pTarget,&bTargetKind))
		{
			DoEndState(pTarget);
		}
	}
	else if(m_PNTarget & SKILLRESULTKIND_NEGATIVE)
	{
		while(pTargetList->GetNextNegativeTarget(&pTarget,&bTargetKind))
		{
			DoEndState(pTarget);
		}
	}
	else
	{
		ASSERT(0);
	}
}

void CSkillObjectStateUnit::StartState(CObject* pObject,BYTE bTargetKind)
{
	if(m_PNTarget & bTargetKind)
	{
		DoStartState(pObject);
	}
}
void CSkillObjectStateUnit::EndState(CObject* pObject,BYTE bTargetKind)
{
	if(m_PNTarget & bTargetKind)
	{
		DoEndState(pObject);
	}
}
