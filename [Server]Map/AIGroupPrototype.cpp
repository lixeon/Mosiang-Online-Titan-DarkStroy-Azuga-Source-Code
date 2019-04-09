// AIGroupPrototype.cpp: implementation of the CAIGroupPrototype class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AIGroupPrototype.h"
#include "RegenPrototype.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAIGroup::CAIGroup()
:m_dwGroupID(0),m_dwGridID(0)
{
	m_RegenInfo.SetGroup(this);
	m_RegenObjectInfoList.Initialize(10);
}

CAIGroup::~CAIGroup()
{
	// magi82(14) - Memory(071027) 메모리 누수 수정
	CRegenObject * pOInfo = NULL;
	m_RegenObjectInfoList.SetPositionHead();
	while(pOInfo = m_RegenObjectInfoList.GetData())
		delete pOInfo;
	m_RegenObjectInfoList.RemoveAll();
}

DWORD CAIGroup::GetMaxObjectNum()
{
	return m_RegenObjectInfoList.GetDataNum();
}
DWORD CAIGroup::GetCurObjectNum()
{
	return m_RegenInfo.GetWaitRegenObjectNum();
}
void CAIGroup::Die(DWORD id)
{
	m_RegenInfo.AddID(id);
//	m_RegenInfo.RegenCheck(GetCurObjectNum(), GetMaxObjectNum());
}
void CAIGroup::Alive(DWORD id)
{
	m_RegenInfo.RemoveID(id);
//	m_RegenInfo.RegenCheck(GetCurObjectNum(), GetMaxObjectNum());
}

void CAIGroup::RegenCheck()
{
	m_RegenInfo.RegenCheck(GetCurObjectNum(), GetMaxObjectNum());
}

void CAIGroup::AddRegenObject(CRegenObject * pObj)
{
	m_RegenObjectInfoList.Add(pObj, pObj->m_dwObjectID);
}
DWORD CAIGroup::GetGridID()
{
	return m_dwGridID;
}
DWORD CAIGroup::GetGroupID()
{
	return m_dwGroupID;
}
void CAIGroup::AddConditionInfo(CRegenConditionInfo * pInfo)
{
	m_RegenInfo.AddCondition(pInfo);
}

void CAIGroup::RegenProcess()
{
	m_RegenInfo.RegenProcess();
}
void CAIGroup::ForceRegen()
{
	m_RegenInfo.ForceRegen();
}

CRegenObject * CAIGroup::GetRegenObject(DWORD dwObjectID)
{
	return m_RegenObjectInfoList.GetData(dwObjectID);
}

CGroupRegenInfo* CAIGroup::GetRegenInfo()
{
	return &m_RegenInfo;
}

void CAIGroup::SetRandomGridID(DWORD GridID)
{
	m_dwGridID = GridID;

	m_RegenObjectInfoList.SetPositionHead();
	while( CRegenObject* pRegenObject = m_RegenObjectInfoList.GetData() )
	{
		pRegenObject->m_dwGridID = GridID;
	}
}