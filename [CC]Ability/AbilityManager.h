// AbilityManager.h: interface for the CAbilityManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABILITYMANAGER_H__563901F1_B2F0_486F_8A6B_495645D878B8__INCLUDED_)
#define AFX_ABILITYMANAGER_H__563901F1_B2F0_486F_8A6B_495645D878B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HashTable.h"
#include "AbilityInfo.h"
#include "AbilityCommonHeader.h"
#include "PtrList.h"
#include "MemoryPoolTempl.h"

class CAbility;
class CAbilityGroup;
class CAbilityUpdater;
class CAbilityIcon;
class CPlayer;

#define ABILITYMGR	USINGTON(CAbilityManager)

class CAbilityManager  
{
	CMemoryPoolTempl<CAbility>* m_pPool;
	CYHHashTable<CAbilityInfo> m_InfoTable;
	cPtrList m_UpdaterList;

public:
	CAbilityManager();
	~CAbilityManager();

	void Init();
	void Release();

	void AddUpdater(CAbilityUpdater* pUpdater);
	
	void InitAbility(CPlayer* pObject,ABILITY_TOTALINFO* pTotalInfo,CAbilityGroup* pAbilityGroup);
	void InitAbilityExp(CAbilityGroup* pAbilityGroup,DWORD AbilityExp);

	void UpdateAbilityState(DWORD Flag,DWORD Param,CAbilityGroup* pAbilityGroup);
		
	CAbilityInfo* GetAbilityInfo(WORD AbilityIndex);

	int GetState(CAbilityGroup* pGroup,CAbilityInfo* pAbilityInfo);
	BYTE GetAbilityLevel(WORD AbilityIndex,CAbilityGroup* pAbilityGroup);
	void GetAbilityTotalInfo(CAbilityGroup* pGroup,ABILITY_TOTALINFO* pOutAbilityTotalInfo);

	BOOL UpgradeAbility(WORD AbilityIndex, CAbilityGroup* pAbilityGroup);
	BOOL CanUpgrade(CAbilityInfo* pAbilInfo, CAbilityGroup* pAbilityGroup);
		
	BOOL SetAbilityQuickPosition(WORD AbilityIndex, POSTYPE QuickPosition,CAbilityGroup* pAbilityGroup);
	
	BOOL CheckAbilityLevel(WORD AbilityIndex,CAbilityGroup* pAbilityGroup,BYTE Level);
	
	CAbility* MakeNewAbility();
	void ReleaseAbility(CAbility* pAbility);
	

	
	void __TranData();

	// RaMa
	BOOL UpgradeAbilitySkPoint(WORD AbilityIndex, CAbilityGroup* pAbilityGroup);
	BOOL DowngradeAbilitySkPoint(WORD AbilityIndex, CAbilityGroup* pAbilityGroup);
	BOOL CanUpgradeSkPoint(CAbilityInfo* pAbilInfo, CAbilityGroup* pAbilityGroup);
	BOOL CanDowngradeSkPoint(CAbilityInfo* pAbilInfo, CAbilityGroup* pAbilityGroup);

	// magi82(42) - 샵아이템 추가(수련치 초기화)
	BOOL ResetAbilitySkPoint(WORD AbilityIndex, CAbilityGroup* pAbilityGroup);
	BOOL CheckCanResetPoint(CPlayer* pPlayer, CAbilityInfo* pAbilInfo);
	
	CYHHashTable<ABILITY_CALC>	m_CalcInfoTable;
	void ReadCalcInfo();
	ABILITY_CALC*	GetAbilityCalcInfo( BYTE bAbilityLevel );
	float	GetAbilityKyungGongSpeed( WORD wLevel );
};

EXTERNGLOBALTON(CAbilityManager);

#endif // !defined(AFX_ABILITYMANAGER_H__563901F1_B2F0_486F_8A6B_495645D878B8__INCLUDED_)
