// TitanManager.h: interface for the CTitanManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(TitanManager_H)
#define TitanManager_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "HashTable.h"
#include "GameResourceStruct.h"

enum eTitanNotifyMsg {
	TNM_MASTER_LIFE,
	TNM_EQUIP_ENDURANCE,
	TNM_EQUIP_NUM,
	TNM_CANNOT_DO,
};
enum eTitanRepairTableType { INVEN, TITAN };

#define TITANMGR USINGTON(CTitanManager)

#define MAX_TITANGRADE	3
#define TITAN_EQUIPITEM_ENDURANCE_MAX	10000000	// 천만

//#define TITAN_EFFECT_MAINTAIN	3620
#define TITAN_EFFECT_RIDEIN	3624
#define TITAN_EFFECT_GETOFF	3625
#define TITAN_EFFECT_FORCED_GETOFF 3626

enum	GetOffKindofReason{eNormal, eFromUser, eMasterLifeRate, eExhaustFuel, eExhaustSpell,};

class CMHFile;
class ITEM_MIX_INFO;
class ITEM_MIX_RES;
class ITEM_MIX_MATERIAL;
class CVirtualItem;
class CTitan;
class CItem;

class CTitanManager  
{
	CTitan*									m_pCurRidingTitan;
	DWORD									m_dwRecallCheckTime;

	BOOL									m_bAvaliableEndurance;	// 내구도 유효성 검사를 위한 상태변수.

	DWORD									m_TitanBreakRate;	// 타이탄 분해 확률
	DWORD									m_RegistedTitanItemDBIdx;	// 등록된 타이탄아이템의 DB인덱스
	CItem*									m_TitanRefairItem;			// 수리할 타이탄 아이템

	titan_calc_stats						m_TitanStats;

	CYHHashTable<ITEM_MIX_INFO>				m_TPMItemTable;

	CMemoryPoolTempl<TITAN_PARTS_KIND>*		m_TitanPartsKindPool;
	CYHHashTable<TITAN_PARTS_KIND>			m_TitanPartsKindTable;

	CYHHashTable<TITAN_UPGRADEINFO>			m_TitanUpgradeInfoTable;

	CYHHashTable<TITAN_BREAKINFO>			m_TitanBreakInfoTable;

//// 타이탄 정보
	CYHHashTable<TITAN_TOTALINFO>				m_TitanInfoList;
	CYHHashTable<TITAN_ENDURANCE_ITEMINFO>		m_ItemEnduranceList;
	CYHHashTable<TITAN_ENDURANCE_ITEMINFO>		m_ItemUsingEnduranceList;

	stTIME									m_serverT;	// 서버 로그인 시각. GAMEIN 시 초기화.

	TITAN_SHOPITEM_OPTION m_TitanShopitemOption;	// magi82(26)

public:
	CTitanManager();
	virtual ~CTitanManager();
	void	TitanMgrInit();
	void	TitanMgrRelease();
	void	Release();

	// 파츠 제작 테이블 관련 함수
	void	LoadTPMList();
	void	LoadTPMResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo);
	ITEM_MIX_INFO * GetTPMItemInfo(WORD wItemIdx);

	// 파츠 속성 테이블 관련 함수
	BOOL LoadTitanPartsKindList();
	TITAN_PARTS_KIND* GetTitanPartsKind(DWORD partsIdx);

	// 타이탄 업그레이드 테이블 관련 함수
	void	LoadTitanUpgradeInfoList();
	void	LoadTitanUpgradeInfoResultItemInfo(CMHFile * fp, TITAN_UPGRADE_MATERIAL * pMaterialInfo);
	TITAN_UPGRADEINFO * GetTitanUpgradeInfoItemInfo(DWORD titanIdx);

	// 타이탄 분해 테이블 관련 함수
	void	LoadTitanBreakInfoMaterialInfo(CMHFile * fp, TITAN_BREAK_MATERIAL * pMaterialInfo);
	void	LoadTitanBreakInfoList();
	TITAN_BREAKINFO * GetTitanBreakInfoItemInfo(DWORD dwIdx);

	void	SetCurRidingTitan(CTitan* pTitan)	{	m_pCurRidingTitan = pTitan;	}
	CTitan*	GetCurRidingTitan()		{	return m_pCurRidingTitan;	}
	DWORD	GetCallItemDBIdx();

	void	ChangeTitanGuageFuel(int changeAmount);
	void	ChangeTitanGuageSpell(int changeAmount);
	// 타이탄 소환/해제 관련
	BOOL	CheckRecallAvailable();
	void	SetRecallRemainTime(DWORD RecallState, DWORD stTimeValue = 0);	// magi82(18)
	BOOL	CheckTimeRecallAvailable();

	// magi82 - Titan(070604)
	stTIME	GetTitanResummonTime();

	// 타이탄 등록 관련
	BOOL	IsRegistedTitan(DWORD dwCallItemDBIdx);

	// 타이탄 업그레이드 관련
	DWORD	GetTitanGrade(DWORD dwCallItemDBIdx);

	//// 타이탄 정보
	void	InitTitanInfo(TITAN_TOTALINFO* pTitanInfo, int num);
	void	AddTitanInfo(TITAN_TOTALINFO* pTitanInfo);
	void	RemoveTitanInfo(DWORD dwCallItemDBIdx);
	void	ReleaseTitanInfoList();
	TITAN_TOTALINFO* GetTitanInfo(DWORD dwCallItemDBIdx);
	void	UpdateCurTitanInfo(CTitan* pTitan);
	void	OnTitanRemove(CTitan* pTitan);

	// 타이탄 장비 내구도 정보
	void	InitTitanEquipItemEnduranceInfo(TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo, int num);
	void	AddTitanEquipItemEnduranceInfo(TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo);
	void	RemoveTitanEquipInfo(DWORD dwEquipItemDBIdx);
	void	ReleaseTitanEquipInfo();
	TITAN_ENDURANCE_ITEMINFO* GetTitanEnduranceInfo(DWORD dwEquipItemDBIdx);

	// 타이탄 사용중 장착 아이템 관련 (for DBUpdate)
	void	AddTitanUsingEquipItemList(TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo);
	void	RemoveTitanUsingEquipItemList(DWORD equipItemDBIdx);

	BOOL	CheckUsingEquipItemNum();
	BOOL	CheckUsingEquipItemEndurance();
	BOOL	IsAvaliableEndurance()	{	return m_bAvaliableEndurance;	}

	int		GetTitanEquipTotalInfo(DWORD* temp);	// magi82 - Titan(070423)
	void	SetTitanEnduranceView( DWORD dwEquipItemDBIdx );// magi82 - Titan(070424)
	void	SetTitanTotalEnduranceView();			// magi82 - Titan(070424)

	BOOL	CheckMasterLifeAmountForRiding();

	// magi82 - Titan(070418)
	DWORD	GetRegistedTitanItemDBIdx()	{	return m_RegistedTitanItemDBIdx;	}
	void	SetRegistedTitanItemDBIdx(DWORD dwIdx)	{	m_RegistedTitanItemDBIdx = dwIdx;	}

	void	SetTitanStats(titan_calc_stats* pStats)	{	m_TitanStats = *pStats;	}
	titan_calc_stats*	GetTitanStats()	{	return &m_TitanStats;	}

	void	CheckNotifyMsg(int checkKind);	//eTitanNotifyMsg
	void	NetworkMsgParse(BYTE Protocol, void* pMsg);

	// magi82 - Titan(070418)
	CItem*	GetTitanRepairItem()	{	return m_TitanRefairItem;	}
	void	SetTitanRepairItem( CItem* pItem )	{	m_TitanRefairItem = pItem;	}

	// magi82 - Titan(070515)
	void SetTitanRepairTotalEquipItemLock( BOOL bFlag );

	// magi82 - Titan(070619) - 수리를 위한 함수(TRUE:수리비, FALSE:수리할 아이템 골라내기)
	DWORD GetTitanEnduranceTotalInfo(MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN* msg, BOOL bFlag = FALSE);

	void SetTitanPaperDelayTime();

	void SetServerLogInTime();

	// magi82(26)
	void	SetTitanShopitemOption(DWORD dwItemIdx, BOOL bAdd);
	TITAN_SHOPITEM_OPTION* GetTitanShopitemOption()	{	return &m_TitanShopitemOption;	};
};
EXTERNGLOBALTON(CTitanManager)
#endif // !defined(TitanManager_H)

