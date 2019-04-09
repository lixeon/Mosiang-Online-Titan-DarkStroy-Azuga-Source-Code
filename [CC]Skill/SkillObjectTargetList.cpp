// SkillObjectTargetList.cpp: implementation of the CSkillObjectTargetList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectTargetList.h"
#include "Object.h"


#ifdef _MAPSERVER_
#include "SkillObject_server.h"
#include "SkillManager_server.h"
#include "UserTable.h"
#include "CharMove.h"
#else
#include "SkillObject_Client.h"
#include "ObjectManager.h"
#endif

CMemoryPoolTempl<STLIST> g_PoolSTLIST;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectTargetList::CSkillObjectTargetList(SKILLINFO* pSkillInfo)
{
	m_TargetTable.Initialize(1);
	m_PositiveTargetType = pSkillInfo->PositiveResultTargetType;
	m_NegativeTargetType = pSkillInfo->NegativeResultTargetType;
	m_bPositionSetHead = FALSE;
	m_SkillRange = pSkillInfo->TargetRange;
	m_SkillAreaIdx = pSkillInfo->TargetAreaIdx;
}

CSkillObjectTargetList::~CSkillObjectTargetList()
{
	m_TargetTable.RemoveAll();
}

DWORD CSkillObjectTargetList::UpdateTargetList(CObject* pObject)
{
	BOOL InList = IsInTargetList(pObject);
	BOOL InTarget = IsInTargetArea(pObject);

	if(InList == TRUE && InTarget == FALSE)
		return SOTL_REMOVED;
	else if(InList == FALSE && InTarget == TRUE)
		return SOTL_ADDED;
	else
		return SOTL_NOTCHANGED;
}

void CSkillObjectTargetList::SetPositionHead()
{
	m_TargetTable.SetPositionHead();
	m_bPositionSetHead = TRUE;
}
STLIST* CSkillObjectTargetList::GetNextTargetList()
{
	STLIST* pList = m_TargetTable.GetData();
	if(pList == NULL)
		return NULL;
//	if(pList->pObject->GetState() == eObjectState_Die)
//		return GetNextTargetList();
//	else
		return pList;
}
CObject* CSkillObjectTargetList::GetNextTarget()
{
	STLIST* pList = GetNextTargetList();
	if(pList)
		return pList->pObject;
	else
		return NULL;
}
BOOL CSkillObjectTargetList::GetNextTarget(WORD PNTarget,CObject** ppObjectOut,BYTE* pTargetKindOut)
{
	if(PNTarget == SKILLRESULTKIND_POSITIVE)
		return GetNextPositiveTarget(ppObjectOut,pTargetKindOut);
	else if(PNTarget == SKILLRESULTKIND_NEGATIVE)
		return GetNextNegativeTarget(ppObjectOut,pTargetKindOut);
	else
	{
		ASSERT(0);
		return FALSE;
	}
}
BOOL CSkillObjectTargetList::GetNextPositiveTarget(CObject** ppObjectOut,BYTE* pTargetKindOut)
{
	*ppObjectOut = NULL;
	STLIST* pList;
	while(pList = GetNextTargetList())
	{
		if(pList->bTargetKind & SKILLRESULTKIND_POSITIVE)
		{
			*ppObjectOut = pList->pObject;
			*pTargetKindOut = pList->bTargetKind;
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CSkillObjectTargetList::GetNextNegativeTarget(CObject** ppObjectOut,BYTE* pTargetKindOut)
{
	*ppObjectOut = NULL;
	STLIST* pList;
	while(pList = GetNextTargetList())
	{
		if(pList->bTargetKind & SKILLRESULTKIND_NEGATIVE)
		{
			*ppObjectOut = pList->pObject;
			*pTargetKindOut = pList->bTargetKind;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CSkillObjectTargetList::IsInTargetList(CObject* pObject)
{
	return m_TargetTable.GetData(pObject->GetID()) ? TRUE : FALSE;
}

BOOL CSkillObjectTargetList::IsInTargetList(DWORD dwObjectID)
{
	return m_TargetTable.GetData(dwObjectID) ? TRUE : FALSE;
}

BYTE CSkillObjectTargetList::GetTargetKind(DWORD dwObjectID)
{
	STLIST* pList = m_TargetTable.GetData(dwObjectID);
	if(pList)
		return pList->bTargetKind;
	else
		return SKILLRESULTKIND_NONE;
}

BYTE CSkillObjectTargetList::AddTargetObject(CObject* pObject,BYTE bTargetKind)
{
	if(IsInTargetList(pObject)==TRUE)
	{
		return SKILLRESULTKIND_NONE;
	}

	STLIST* pList = MakeNewSTLIST(pObject,bTargetKind);
	if(pList == NULL)
		return SKILLRESULTKIND_NONE;

	m_TargetTable.Add(pList,pObject->GetID());

	return pList->bTargetKind;
}
BYTE CSkillObjectTargetList::RemoveTargetObject(DWORD ObjectID)
{
	if(IsInTargetList(ObjectID)==FALSE)
	{
		return SKILLRESULTKIND_NONE;
	}
	
	// yh
	BYTE bTargetKind;
	STLIST* pList = m_TargetTable.GetData(ObjectID);
	if(pList)
	{
		bTargetKind = pList->bTargetKind;
		g_PoolSTLIST.Free(pList);
	}
	m_TargetTable.Remove(ObjectID);

	return bTargetKind;
}

void CSkillObjectTargetList::InitTargetList(CSkillObject* pSkillObject,CTargetList* pTList,CObject* pAddtionalTarget,MAINTARGET* pMainTarget)
{
	m_TargetTable.RemoveAll();
	CObject* pObject;
	CTargetListIterator iter(pTList);

#ifdef _MAPSERVER_
	VECTOR3 MainTargetPos;
	BOOL bMainPos = FALSE;

	if( pSkillObject->GetSkillInfo()->IsTargetAreaFitToHero() )
	{
		if( pSkillObject->GetOperator() )
		{
			pSkillObject->GetOperator()->GetPosition( &MainTargetPos );
			bMainPos = TRUE;
		}
	}
	else
	{
		bMainPos = GetMainTargetPos(pMainTarget,&MainTargetPos,NULL);
	}

	/// 06. 09. 변환무공범위 버그 수정 - 이영준
	SKILLOPTION* pSkillOption = NULL;
	int addrange = 0;

	pSkillOption = SKILLMGR->GetSkillOption(pSkillObject->GetSkillObjectInfo()->Option);
	if(pSkillOption)
		addrange = pSkillOption->Range;

#endif

	while(iter.GetNextTarget())
	{
#ifdef _MAPSERVER_
		pObject = g_pUserTable->FindUser(iter.GetTargetID());
#else
		pObject = OBJECTMGR->GetObject(iter.GetTargetID());
#endif
		if(pObject == NULL)
			continue;

#ifdef _MAPSERVER_

		if(bMainPos)
		{
			if(m_SkillRange)
			{
				VECTOR3* pos = CCharMove::GetPosition(pObject);
				if(CalcDistanceXZ(pos,&MainTargetPos) > m_SkillRange+addrange+300)
					continue;
			}
			else
			{
				STLIST* pList = MakeNewSTLIST(pObject,iter.GetTargetKind());
				if(pList == NULL)
					continue;
				
				m_TargetTable.Add(pList,pObject->GetID());
				
				// 그림범위까지 0 이면 혼자쓰는 타겟이다.
				if(m_SkillAreaIdx == 0)
					break;
			}
		}
#endif

		STLIST* pList = MakeNewSTLIST(pObject,iter.GetTargetKind());
		if(pList == NULL)
			continue;

		m_TargetTable.Add(pList,pObject->GetID());

	}
	iter.Release();

	if(pAddtionalTarget)
	{		
		STLIST* pList = MakeNewSTLIST(pAddtionalTarget,iter.GetTargetKind());
		if(pList && m_TargetTable.GetData(pAddtionalTarget->GetID()) == NULL)
			m_TargetTable.Add(pList,pAddtionalTarget->GetID());
	}

	DoInitTargetList(pSkillObject,pTList);
}

void CSkillObjectTargetList::Release(CSkillObject* pSkillObject)
{
	DoRelease(pSkillObject);

	m_TargetTable.SetPositionHead();
	STLIST* pList;
	while(pList = m_TargetTable.GetData())
	{
		g_PoolSTLIST.Free(pList);
	}
	m_TargetTable.RemoveAll();
}


STLIST* CSkillObjectTargetList::MakeNewSTLIST(CObject* pObject,BYTE bTargetKind)
{
#ifdef _MAPSERVER_
	bTargetKind = SKILLRESULTKIND_NONE;

	if(m_pSkillObject->IsNegativeTarget(pObject))
		bTargetKind |= SKILLRESULTKIND_NEGATIVE;
	if(m_pSkillObject->IsPositiveTarget(pObject))
		bTargetKind |= SKILLRESULTKIND_POSITIVE;	
	
	if(bTargetKind == SKILLRESULTKIND_NONE)
		return NULL;
#endif

	// yh
	STLIST* pList = g_PoolSTLIST.Alloc();
	pList->pObject = pObject;
	
	pList->bTargetKind = bTargetKind;

	return pList;
}