// AbilityGroup.cpp: implementation of the CAbilityGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbilityGroup.h"
#include "AbilityManager.h"
#include "Player.h"
#include "QuickManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbilityGroup::CAbilityGroup()
{
	m_AbilityTable.Initialize(8);
}

CAbilityGroup::~CAbilityGroup()
{

}

void CAbilityGroup::Init(ABILITY_TOTALINFO* pTotalInfo)
{
	Release();

	CAbilityInfo* pInfo;
	BYTE Level;
	POSTYPE QuickPos;
	WORD AbilityIndex;
	char* pLevels;
	char* pQuickPosition;
	for(int kind = 0;kind<eAGK_Max;++kind)
	{
		pLevels = pTotalInfo->AbilityDataArray[kind];
		pQuickPosition = pTotalInfo->AbilityQuickPositionArray[kind];
		for(int n=0;n<MAX_ABILITY_NUM_PER_GROUP;++n)
		{
			Level = GetAbilityLevel_WithDBValue(pLevels[n]);
			if(Level == 0)
				continue;

			AbilityIndex = GetAbilityIndex_WithKindPosition(kind,n);

			// 06. 01 이영준 - 단축창 변경
			//QuickPos = GetAbilityQuickPosition(pQuickPosition[n]);
			QuickPos = CQuickManager::GetAbilityQuickPosition(kind, n, pTotalInfo);

			pInfo = ABILITYMGR->GetAbilityInfo(AbilityIndex);
			if(pInfo == NULL)
				continue;
			
			if(m_AbilityTable.GetData(AbilityIndex) != NULL)
				continue;
			
			AddAbility(pInfo,Level,QuickPos);
		}
	}
}

void CAbilityGroup::Release()
{
	CAbility* pAbility;
	m_AbilityTable.SetPositionHead();
	while(pAbility = m_AbilityTable.GetData())
	{
		ABILITYMGR->ReleaseAbility(pAbility);
	}
	m_AbilityTable.RemoveAll();
}

void CAbilityGroup::AddAbility(CAbilityInfo* pAbilityInfo,BYTE Level,POSTYPE QuickPos)
{
	ASSERT(pAbilityInfo);
	CAbility* pAbility = ABILITYMGR->MakeNewAbility();
	pAbility->Init(pAbilityInfo,Level,QuickPos);
	m_AbilityTable.Add(pAbility,pAbility->GetIndex());
}

void CAbilityGroup::DeleteAbility(CAbility* pAbility,POSTYPE QuickPos)
{
	ASSERT(pAbility);
	m_AbilityTable.Remove( pAbility->GetIndex() );
	ABILITYMGR->ReleaseAbility( pAbility );	
}

void CAbilityGroup::SetPositionHead()
{
	m_AbilityTable.SetPositionHead();
}
CAbility* CAbilityGroup::GetNextAbility(int GroupNum)
{
	CAbility* pAbility = m_AbilityTable.GetData();
	if(pAbility == NULL)
		return NULL;
	if(GroupNum == eAGK_Max || pAbility->GetKind() == GroupNum)
		return pAbility;
	else
		return GetNextAbility(GroupNum);
}

CAbility* CAbilityGroup::GetAbility(WORD Idx)
{
	CAbility* pAbility = m_AbilityTable.GetData(Idx);
	return pAbility;
}

void CAbilityGroup::SetAbilityExp(DWORD Exp)
{
#ifdef _MAPSERVER_
	GetOwenerObject()->m_HeroInfo.AbilityExp = Exp;
#endif
	m_AbilityExp = Exp;
}
