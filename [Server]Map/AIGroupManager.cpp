// AIGroupManager.cpp: implementation of the CAIGroupManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AIGroupManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAIGroupManager::CAIGroupManager()
{
	m_groupList.Initialize(300);
}

CAIGroupManager::~CAIGroupManager()
{
	Release();
}
void CAIGroupManager::Release()
{
	CAIGroup * pGroup= NULL;
	m_groupList.SetPositionHead();
	while((pGroup = m_groupList.GetData()))
	{
		delete pGroup;
		pGroup = NULL;
	}
	m_groupList.RemoveAll();
}

void CAIGroupManager::AddGroup(CAIGroup * aiGroup)
{
	m_groupList.Add(aiGroup, aiGroup->GetGroupID());
}

void CAIGroupManager::RemoveGroup(DWORD dwGroupID, DWORD dwGridID)
{
	CAIGroup * pGroup = NULL;
	m_groupList.StartGetMultiData(dwGroupID);
	while(pGroup = m_groupList.GetMultiData())
	{
		if(pGroup->GetGridID() == dwGridID)
		{
			m_groupList.RemoveCurMultiData();
			delete pGroup;
			pGroup = NULL;
			break;
		}
	}
}

CAIGroup * CAIGroupManager::GetGroup(DWORD dwGroupID, DWORD dwGridID)
{
	CAIGroup * pGroup = NULL;
	m_groupList.StartGetMultiData(dwGroupID);
	while(pGroup = m_groupList.GetMultiData())
	{
		if(pGroup->GetGridID() == dwGridID)
		{
			return pGroup;
		}
	}
	return NULL;
}


CRegenObject * CAIGroupManager::GetRegenObject(DWORD dwGroupID, DWORD dwGridID, DWORD dwID)
{
	CAIGroup * pGroup = NULL;
	m_groupList.StartGetMultiData(dwGroupID);
	while(pGroup = m_groupList.GetMultiData())
	{
		if(pGroup->GetGridID() == dwGridID)
		{
			return pGroup->GetRegenObject(dwID);
		}
	}
	return NULL;
}

void CAIGroupManager::RegenProcess()
{
	CAIGroup * pGroup= NULL;
	m_groupList.SetPositionHead();
	while((pGroup = m_groupList.GetData()))
	{
		pGroup->m_RegenInfo.RegenProcess();
	}
}