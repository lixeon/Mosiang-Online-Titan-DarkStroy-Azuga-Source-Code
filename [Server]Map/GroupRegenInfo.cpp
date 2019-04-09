// GroupRegenInfo.cpp: implementation of the CGroupRegenInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GroupRegenInfo.h"
#include "RegenConditionInfo.h"
#include "RegenManager.h"
#include "AIGroupPrototype.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGroupRegenInfo::CGroupRegenInfo()
{

}

CGroupRegenInfo::~CGroupRegenInfo()
{
	POS pos = m_RegenConditionList.GetFirstPos();

	while( pos )
	{
		CRegenConditionInfo* pInfo = m_RegenConditionList.GetNextPos( pos );
		if( pInfo )
			delete pInfo;
	}


	m_RegenConditionList.DeleteAll();

}

DWORD CGroupRegenInfo::GetWaitRegenObjectNum()
{
	return m_RegenIDList.GetCount();
}

void CGroupRegenInfo::AddID(DWORD id)
{
	m_RegenIDList.AddTail(id);
}

void CGroupRegenInfo::RemoveID(DWORD id)
{
	DWORD dwID = 0;
	POS pos = m_RegenIDList.GetFirstPos();
	while(pos)
	{
		dwID = m_RegenIDList.GetAtPos(pos);
		if(dwID == id)
		{
			m_RegenIDList.DeletePos(pos);
			break;
		}
		m_RegenIDList.GetNextPos(pos);
	}
}

void CGroupRegenInfo::AddCondition(CRegenConditionInfo * pInfo)
{
	m_RegenConditionList.AddTail(pInfo);
}
void CGroupRegenInfo::RegenCheck(DWORD CurObjectNum, DWORD MaxObjectNum)
{
	float ratio = (float)CurObjectNum/(float)MaxObjectNum;
	
	CRegenConditionInfo * pCInfo = NULL;
	int nCount = m_RegenConditionList.GetCount();
	for(int i = 0 ; i < nCount ; ++i)
	{
		pCInfo = m_RegenConditionList.GetAt(i);
		//taiyo
		// if(ratio <= pBObj->fRemainderRatio)이면 안됨
		// fRemainderRatio = 0인것은 리젠하지 않는 것
		if( pCInfo->fRemainderRatio != 0 )
		if( ratio >= ( 1 - pCInfo->fRemainderRatio ) )
		{
			pCInfo->dwStartRegenTick = gCurTime;
			pCInfo->bRegen = TRUE;
			break;			
		}
	}
}

void CGroupRegenInfo::RegenProcess()
{
	POS pos = m_RegenConditionList.GetFirstPos();
	while(pos)
	{
		CRegenConditionInfo * pConditionInfo = m_RegenConditionList.GetNextPos(pos);
		if(pConditionInfo)
		if(pConditionInfo->bRegen)
		{
			if(pConditionInfo->dwStartRegenTick + pConditionInfo->dwRegenDelay < gCurTime)
			{
				pConditionInfo->dwStartRegenTick = gCurTime;
//				pConditionInfo->dwStartRegenTick = 0;
				pConditionInfo->bRegen = FALSE;

				REGENMGR->RegenGroup(pConditionInfo->dwTargetGroupID, m_pGroup->GetGridID());
			}
			break;
		}
	}
	
}

void CGroupRegenInfo::ForceRegen()
{
	// respawn start
	BOOL bRegenFailed = FALSE;
	DWORD dwID = 0;
	while(dwID = m_RegenIDList.DeleteHead())
	{	
		if(!RegenObject(dwID))
		{
			m_RegenIDTmpList.AddTail(dwID);
			bRegenFailed = TRUE;
		}
	}
	while(dwID = m_RegenIDTmpList.DeleteHead())
	{
		m_RegenIDList.AddTail(dwID);
	}

	if( bRegenFailed )
		m_pGroup->RegenCheck();



	/*
	
	POS pos = m_RegenIDList.GetFirstPos();
	while(pos)
	{
		dwID = m_RegenIDList.GetAtPos(pos);
		if(RegenObject(dwID))
		{
			m_RegenIDList.DeletePos(pos);
		}
		else
			m_RegenIDList.GetNextPos(pos);
	}
	*/
}

BOOL CGroupRegenInfo::RegenObject(DWORD dwID)
{
	return REGENMGR->RegenObject(m_pGroup->GetGroupID(), m_pGroup->GetGridID(), dwID);
}

void CGroupRegenInfo::SetGroup(CAIGroup * pGroup)
{
	m_pGroup = pGroup;
}

void CGroupRegenInfo::SetRegenDelayTime(DWORD delaytime)
{
	POS pos = m_RegenConditionList.GetFirstPos();
	while(pos)
	{
		CRegenConditionInfo * pConditionInfo = m_RegenConditionList.GetNextPos(pos);
		if( pConditionInfo )
			pConditionInfo->dwRegenDelay = delaytime;
	}
}

void CGroupRegenInfo::SetRegenRandGridID(DWORD GridID)
{
	POS pos = m_RegenConditionList.GetFirstPos();
	while(pos)
	{
		//confirm
		CRegenConditionInfo * pConditionInfo = m_RegenConditionList.GetNextPos(pos);

		if( m_pGroup )
			m_pGroup->SetRandomGridID(GridID);
	}
}
