// AbilityUpdater_Interface.cpp: implementation of the CAbilityUpdater_Interface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbilityUpdater_Interface.h"
#include "SuryunDialog.h"
#include "GameIn.h"
#include "AbilityGroup.h"
#include "AbilityManager.h"
#include "QuickManager.h"
#include "SkillPointRedist.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbilityUpdater_Interface::CAbilityUpdater_Interface()
{

}

CAbilityUpdater_Interface::~CAbilityUpdater_Interface()
{

}

void CAbilityUpdater_Interface::Update(DWORD& Flag,DWORD Param,CAbilityGroup* pGroup,CYHHashTable<CAbilityInfo>* pInfoTable)
{
	if(Flag == ABILITYUPDATE_ABILITYQUICK_CHANGED)
		return;

	BYTE Level;
	int State;
	CAbility* pAbility;
	CAbilityInfo* pInfo;
	CSuryunDialog* pDlg = GAMEIN->GetSuryunDialog();
	pDlg->SetAbilityExp(pGroup->GetAbilityExp());
	CSkillPointRedist* pSkDlg = GAMEIN->GetSkPointDlg();
	pSkDlg->SetAbilityExp(pGroup->GetAbilityExp());

	if(Flag != ABILITYUPDATE_ABILITYEXP_CHANGED)
	{
		// to each Ability
		pGroup->SetPositionHead();
		while(pAbility = pGroup->GetNextAbility(eAGK_Max))
		{
			State = eAIS_Usable;
			Level = pAbility->GetLevel();
			pDlg->UpdateAbility(State,Level,pAbility->GetAbilityInfo());
		}
		
		// each AbilityInfo
		pInfoTable->SetPositionHead();
		while(pInfo = pInfoTable->GetData())
		{
			State = ABILITYMGR->GetState(pGroup,pInfo);
			Level = 0;
			// 06. 01. 어빌리티창 인터페이스 수정 - 이영준
			// 기존 안보이는 조건까지 보이는 조건에 들어갔다
			if(State == eAIS_OnlyVisible || State == eAIS_NotAvailable)
				pDlg->UpdateAbility(State,Level,pInfo);
		}
	}
	
	if(Flag == ABILITYUPDATE_ABILITYINITED)
	{
		pGroup->SetPositionHead();
		while(pAbility = pGroup->GetNextAbility(eAGK_Max))
		{
			if(pAbility->GetQuickPosition() == 0)
				continue;

			// 06. 01 이영준 - 단축창 변경
			// 초기 설정 함수 AddQuickItemReal -> SetQuickItemReal
			// add quick item
			QUICKMGR->SetQuickItemReal(pAbility->GetQuickPosition(),
				pAbility->GetAbilityInfo()->GetAbsPosition(), pAbility->GetIndex() + MIN_ABILITY_INDEX );
/*
			CAbilityIcon* pAbilityIcon = GAMEIN->GetSuryunDialog()->GetAbilityIconAbs(pAbility->GetAbilityInfo()->GetAbsPosition());
			ASSERT(pAbilityIcon);
			if(!pAbilityIcon) continue;

			CQuickItem * pNewQItem = QUICKMGR->NewQuickItem((CBaseItem*)pAbilityIcon);

			pNewQItem->SetPosition(QuickPos);
			GAMEIN->GetQuickDialog()->AddQuickItem(QuickPos, pNewQItem);	*/
		}		
	}

	// 06. 01. 어빌리티창 인터페이스 수정 - 이영준
	// 레벨업시 툴팁을 리셋한다
	if(Flag == ABILITYUPDATE_CHARACTERLEVEL_CHANGED)
	{
		pGroup->SetPositionHead();
		while(pAbility = pGroup->GetNextAbility(eAGK_Max))
		{
			pDlg->UpdateAbility(pAbility->GetAbilityInfo());
		}
				
		pInfoTable->SetPositionHead();
		while(pInfo = pInfoTable->GetData())
		{
			pDlg->UpdateAbility(pInfo);
		}

	}
}
