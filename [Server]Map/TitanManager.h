// TitanManager.h: interface for the CTitanManager class.
//////////////////////////////////////////////////////////////////////

#if !defined(TITANMANAGER_H)
#define TITANMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum	EnduranceCalcPoint{eWhenTitanAttack, eWhenTitanDefense};

enum	GetOffKindofReason{eNormal, eFromUser, eMasterLifeRate, eExhaustFuel, eExhaustSpell,};

// magi82(33)
enum	EnduranceException	{	eExceptionNone, eExceptionInven, eExceptionPyoguk	};

// 장착아이템 내구도
enum	{AtAtk, AtDef};

#define MAX_TITANGRADE	3
#define TITAN_EQUIPITEM_ENDURANCE_MAX	10000000	// 천만
class CTitan;

//enum	eTitanEquips{TE_HELMET, TE_ARMOR, TE_GLOVES, TE_LEGS, TE_CLOAK, TE_SHIELD, TE_WEAPON, TE_MAX};	//eTitanWearedItem_Max

struct TitanWearedInfo
{
	WORD TitanEquipItemIdx;
	DWORD TitanEquipItemDBIdx;
};

class CTitanManager  
{
	//static	DWORD	m_gTitanSpellDecrease;
	CMemoryPoolTempl<CPlayer>* PlayerPool;

	CTitan*		m_pCurRidingTitan;
	CPlayer*	m_pMaster;
	titan_calc_stats	m_titanStats;
	titan_calc_stats	m_titanItemStats;
	DWORD		m_dwCurRegistTitanCallItemDBIdx;	//현재 등록상태인 타이탄의 인증서 DBIDX
	DWORD		m_dwRecallCheckTime;
	WORD		TitanScaleForNewOne;	// 100 => 클라이언트에서 1.0f.
	TitanWearedInfo		m_TitanWearedInfo[eTitanWearedItem_Max];
	BOOL		m_bAvaliableEndurance;	// 내구도 유효성 검사를 위한 상태변수.
	TITAN_SHOPITEM_OPTION m_TitanShopitemOption;	// magi82(26)

	CYHHashTable<TITAN_TOTALINFO>				m_TitanInfoList;
	CYHHashTable<TITAN_ENDURANCE_ITEMINFO>		m_ItemEnduranceList;
	CYHHashTable<TITAN_ENDURANCE_ITEMINFO>		m_ItemUsingEnduranceList;
	CMemoryPoolTempl<TITAN_ENDURANCE_ITEMINFO>	m_ItemEndurancePool;

	// 2007. 9. 14. CBH - 타이탄 소환 시간 관련 변수 추가
	DWORD m_dwTitanRecallProcessTime;	
	DWORD m_dwCurrentTime;
	BOOL m_bTitanRecall;
	BOOL m_bTitanRecallClient;
	//////////////////////////////////////////////////////

	// 타이탄 물약 딜레이 시간
	DWORD m_dwTitanEPTime;

	// magi82(23)
	DWORD m_dwTitanMaintainTime;

public:
	CTitanManager();
	virtual ~CTitanManager();

	CTitan*		GetCurRidingTitan()		{	return m_pCurRidingTitan;	}

	void	Init(CPlayer* pPlayer);
	void	Release();

	void	SetRegistTitanCallItemDBIdx(DWORD itemDBIdx)	{	m_dwCurRegistTitanCallItemDBIdx = itemDBIdx;	}
	DWORD	GetRegistTitanCallItemDBIdx()	{	return m_dwCurRegistTitanCallItemDBIdx;	}
	void	RemoveTitan();
	// 타이탄 정보 관련
	void	AddTitanTotalInfo(TITAN_TOTALINFO* pTitanInfo, int flagSendMsgTo = eServerOnly);
	BOOL	AddTitanTotalInfoList(TITAN_TOTALINFO* pTitanInfo);
	TITAN_TOTALINFO*	GetTitanTotalInfo(DWORD callItemDBIdx);
	void	RemoveTitanTotalInfo(DWORD callItemDBIdx);	// 교환시 리스트 상에서만 삭제
	void	DeleteTitanTotalInfo(DWORD callItemDBIdx);	// 소환아이템삭제, 로그아웃시.
	WORD	GetTitanInfoList(TITAN_TOTALINFO* RtInfo);

	void	DeleteTitan(DWORD callItemDBIdx);

	void	CreateNewTitan(CPlayer* pMaster, DWORD dwItemIdx, DWORD dwCallItemDBIdx, WORD wTitanKind, WORD wTitanGrade = 1);
	void	CreateUpgradeTitan(CPlayer* pMaster, DWORD dwItemIdx, DWORD dwCallItemDBIdx);

	void	SetTitanScale(WORD wScale)	{	TitanScaleForNewOne = wScale;	}	// 타이탄 인증서 생성시 사용
	DWORD	GetTitanScale()	{	return TitanScaleForNewOne;	}

	// 타이탄 프로세스
	// 마력관련처리
	DWORD	GetTitanSpellDecrease();
	void	ReduceTitanSpell(DWORD spell);

	// 장착아이템 내구도 처리
	void	TitanProcess();

	void	UpDateCurTitanAndEquipItemInfo();

	void	UpdateCurTitanInfo();

	// 타이탄 탑승
	BOOL	RideInTitan(DWORD callItemDBIdx, BOOL bSummonFromUser = TRUE);

	void	CheckRidingTitan();
	void	SetTitanRiding(BOOL bVal);
	// 타이탄 탑승 조건 검사
	BOOL	CheckBeforeRideInTitan(DWORD callItemDBIdx);
	void	SetRecallCheckTime(DWORD checkTime = 0);

	BOOL	CheckTimeRecallAvailable();

	// 타이탄 탑승/스킬 사용가능 여부. 유효한 파츠가 3개이상 등
	BOOL	CheckEquipState();

	// 타이탄 탑승해제
	BOOL	GetOffTitan(int reason = eNormal);

	// 타이탄 교환
	void	ExchangeTitan(DWORD callItemDBIdx, CPlayer* pNewMaster);

	// 타이탄 등급업
	// 아이템 관련처리(등급별 아이템이 다름.기존 소환아이템 지우기.다음 등급 소환아이템 생성하기)
	// 타이탄 등급 가산처리(DB)
	void	UpgradeTitan(DWORD callItemDBIdx, DWORD newCallItemDBIdx);

	// 타이탄 등록
	// 타이탄 등록처리(DB)
	BOOL	RegistTitan(DWORD	callItemDBIdx);
	
	// 타이탄 등록해제
	// 타이탄 등록해제처리(DB)
	BOOL	CancleTitanRegister(DWORD callItemDBIdx);

	// 타이탄 연료/마력 관련
	int		ApplyYoungYakItemToCurTitan(ITEM_INFO* pItemInfo);
	void	AddCurTitanFuel(WORD amount);
	void	AddCurTitanSpell(WORD amount);
	void	AddCurTitanFuelAsRate(float recoverRate);
	void	AddCurTitanSpellAsRate(float recoverRate);

	void	SetTitanStats();
	titan_calc_stats*	GetTitanStats()		{	return &m_titanStats;		}
	titan_calc_stats*	GetTitanItemStats()	{	return &m_titanItemStats;	}
///////////////////////////////////////////////////////////////////////////////
	// 장착아이템 내구도 정보 생성
	void	MakeNewEnduranceInfo(CPlayer* pOwner, ITEMBASE* pItemInfo, EnduranceException eException = eExceptionNone);	// magi82(33)
	void	DeleteTitanEquip(DWORD itemDBIdx);

	// 타이탄 장착 아이템 데이터 관리
	void	AddTitanEquipItemEnduranceInfo(TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo, int flagSendMsgTo = eServerOnly);
	BOOL	AddTitanEquipItemEnduranceInfoList(TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo);
	TITAN_ENDURANCE_ITEMINFO*	GetTitanItemEnduranceInfo(DWORD equipItemDBIdx);
	void	RemoveTitanEquipItemInfo(DWORD equipItemDBIdx);	// 교환시 리스트 상에서만 삭제
	void	DeleteTitanEquipItemInfo(DWORD equipItemDBIdx);	// 장착아이템 파괴, 또는 삭제시.
	WORD	GetTitanEquipItemInfoList(TITAN_ENDURANCE_ITEMINFO* RtInfo);
	void	DiscardTitanEquipItem(POSTYPE whatPos, WORD whatItemIdx, DWORD titanEquipItemDBIdx);

	// 타이탄 사용중 장착 아이템 관련 (for DBUpdate)
	void	AddTitanUsingEquipItemList(TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo);
	void	RemoveTitanUsingEquipItemList(DWORD equipItemDBIdx);
	void	UpdateUsingEquipItemEnduranceInfo();
	void	GetAppearanceInfo(TITAN_APPEARANCEINFO* RtInfo);

	BOOL	CheckUsingEquipItemNum();
	BOOL	CheckUsingEquipItemEndurance();
	BOOL	IsAvaliableEndurance()	{	return m_bAvaliableEndurance;	}

	// 장착아이템 내구도
	void	DoRandomDecrease(int flgPoint);
	DWORD	GetDecreaseEnduranceFromItemIdx(DWORD itemIdx);
	void	PlusItemEndurance(DWORD titanEquipItemDBIdx, DWORD increaseAmount);
	void	MinusItemEndurance(DWORD titanEquipItemDBIdx, DWORD decreaseAmount);
	void	SendItemEnduranceInfo(TITAN_ENDURANCE_ITEMINFO* pInfo);

	void	SetWearedInfo(WORD wEquipItemKind, ITEMBASE* pItemBase, BOOL bIn);
	//void	SetWearedInfo(WORD wEquipItemKind, DWORD itemIdx, DWORD itemDBIdx);
	TitanWearedInfo*	GetWearedInfo(int equipKind);	// 장착 부위를 받아서 정보 리턴.

	void	ExchangeTitanEquipItem(DWORD dwItemDBIdx, CPlayer* pNewOwner);

	void	SendTitanStats();
	void	SendTitanPartsChange();

	// magi82 - Titan(070515)
	void	SetTitanRepairEquipItem( CPlayer* pPlayer, MSG_DWORD2* pmsg );
	void	SetTitanRepairTotalEquipItem( CPlayer* pPlayer, MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN* pmsg );
	//////////////////////////////////////////////////////////////////////////

	///// 2007. 9. 13. CBH
	BOOL	TitanRecallStartSyn( DWORD callItemDBIdx, BOOL bSummonFromUser = TRUE );	//타이탄 소환 시작
	void	TitanRecallProcess();														//타이탄 시간 처리 함수
	void	InitTitanRecall();															//타이탄 소환 관련 초기화 함수
	void	StartTitanRecall();															//타이탄 소환 시작
	BOOL	RideInTitan();																//타이탄 탑승
	BOOL	IsTitanRecall();															//타이탄 소환 상태 반환
	void	SetRecallProcessTime(DWORD dwRecallProcessTime);							//타이탄 소환 캐스팅 시간 셋팅
	///////////////////////////////////////////////////////////////////////////

	void	MoveTitanEquipItemUpdateToDB(ITEMBASE* pFromItem, ITEMBASE* pToItem, POSTYPE FromPos, POSTYPE ToPos);

	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );

	// magi82(26)
	void	SetTitanShopitemOption(DWORD dwItemIdx, BOOL bAdd);
	TITAN_SHOPITEM_OPTION* GetTitanShopitemOption()	{	return &m_TitanShopitemOption;	};
};


#endif // !defined(TITANMANAGER_H)
