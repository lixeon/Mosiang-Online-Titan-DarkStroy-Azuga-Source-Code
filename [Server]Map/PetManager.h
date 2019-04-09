#pragma once

#define PET_SKILLCHARGE_CHECKTIME	1000
#define PET_MAX_SKILL_CHARGE	10000
#define PET_MAX_LEVEL		3
#define PET_GRADEUP_PROB_1TO2	80
#define PET_GRADEUP_PROB_2TO3	80

#define PET_RESUMMON_VALID_TIME	30000

#define CRISTMAS_EVENTPET 8
#define CRISTMAS_EVENTPET_SUMMONNING_TIME 60000*30

enum eFncOptPetTotalInfo{ eWithNULLOBJ, eWithNULL };
enum ePetUpgradeResult{eUpgradeSucess=0, eUpgradeFailforProb=1, eUpgradeFailforEtc=2, eUpgradeFailfor3rdUp=3, eUpgradeFailforSamePetSummoned=4,};
enum ePetFeedResult{ePFR_Sucess=0, ePFR_Unsummoned, ePFR_StaminaFull};

//버프
enum ePetBuffKind
{ePB_None,
ePB_Demage_Percent=1,		//일정 확률로 데미지 증가
ePB_Dodge=2,				//일정 확률로 회피
ePB_MasterAllStatUp=3,		//주인캐릭터 스탯 향상
ePB_Item_DoubleChance=4,	//일정 확률로 아이템 획득 2배
ePB_NoForeAtkMonster=5,		//선공몹에게 비선공 효과 //일반몹
ePB_ReduceCriticalDmg=6,	//받는일격데미지감소
ePB_MasterAllStatRound=7,	//주인캐릭터 스탯 반올림
ePB_Item_RareProbUp=8,		//레어획득확률 증가
ePB_MussangTimeIncrease=9,	//무쌍발동시간 증가
ePB_Kind_Max};
//펫소환상태
enum PetSummonning{ePSS_ReleaseSummon,ePSS_SaveSummon};

//펫종류
enum ePetKind{ePK_None, ePK_CommonPet=1, ePK_ShopItemPet=2, ePK_EventPet=4,};

struct BuffData
{
	BuffData():Prob(0),BuffValueData(0),BuffAdditionalData(0) {};
	DWORD	Prob;
	DWORD	BuffValueData;
	DWORD	BuffAdditionalData;
};

enum ePetEquipItemIdx
{
	ePEII_FriendshipIncrease50perc = 55800,
	ePEII_StaminaReductionDecrease50perc = 55801,
};

struct PETEQUIP_ITEMOPTION
{
	PETEQUIP_ITEMOPTION():iPetStaminaReductionDecrease(0),
		iPetStaminaMaxIncreaseAmount(0),
		iPetStaminaRecoverateIncrease(0),
		iPetStaminaRecoverateAmount(0),
		iPetFriendshipIncrease(0),
		fPetFriendshipProtectionRate(0){};
	int	iPetStaminaReductionDecrease;	//펫 스태미나 소비 감소량(%)
	int	iPetStaminaMaxIncreaseAmount;	//펫 스태미나 최대치 증가량(val)
	int	iPetStaminaRecoverateIncrease;	//펫 스태미나 회복 증가량(%)
	int	iPetStaminaRecoverateAmount;	//펫 스태미나 회복 추가량(val)
	int	iPetFriendshipIncrease;	//펫 친밀도 추가 획득량(%)
	//SW070531 펫 친밀도 보호기능
	float fPetFriendshipProtectionRate;	//주인 부활 시 펫 친밀도 하락 보호량(%)
};

class CPlayer;

class CPetManager
{
//	static CIndexGenerator			m_PetIDGenerator;
	static DWORD					m_dwPetObjectID;

	CPlayer*						m_pPlayer;
	CPet*							m_pCurSummonPet;

	PETEQUIP_ITEMOPTION				m_PetEquipOption;

	DWORD							m_dwSkillRechargeCheckTime;
	DWORD							m_dwSkillRechargeAmount;
	BOOL							m_bSkillGuageFull;

	BOOL							m_bPetStaminaZero;

	DWORD							m_dwReleaseDelayTime;

	DWORD							m_dwResummonDelayTime;

	//SW070531 펫 친밀도 보호기능
	int								m_iFriendshipReduceAmount;

	//SW061211 크리스마스이벤트
	DWORD							m_dwEventPetSummonRemainTime;	//봉인될 시각 ->소환시각 + 30Min
	DWORD							m_dwEventPetCheckTime;			//프로세스 체킹

	WORD							m_wPetKind;

	DWORD		m_dwPetValidDistPosCheckTime;

	int								m_BuffFlag;	//펫 버프 정보 관련
	BuffData						m_BuffData[ePB_Kind_Max];

	//CYHHashTable<CPet>				m_PetTable;
	CYHHashTable<PET_TOTALINFO>		m_PetInfoList;

public:
	CPetManager(void);
	virtual ~CPetManager(void);

	void		Init(CPlayer* pPlayer);
	void		Release();

	void		AddPet(CPet* pPet,DWORD	ItemDBIdx);
//	void		RemovePet(DWORD ItemDBIdx);
	void		AddPetTotalInfo(PET_TOTALINFO* pPetInfo,int flagSendMsgTo = eServerOnly);
	void		RemovePetTotalInfo(DWORD dwSummonItemDBIdx);	//펫 교환시
	void		DeletePet(DWORD ItemDBIdx);	//펫 삭제시
//	void		DeletePet(DWORD dwPetID);
	void		ReleaseCurPetMove();
	void		UpGradeSummonPet();
	BOOL		UpGradePet(DWORD dwSummonItemDBIdx, BOOL bCheckProb = TRUE);
	BOOL		RevivalPet(DWORD dwSummonItemDBIdx, int iGrade);
	void		RemovePet();

	void		AddPetInfoList(PET_TOTALINFO* pPetInfo);
	void		RemovePetInfoList(DWORD dwSummonItemDBIdx);
	WORD		GetPetInfoList(PET_TOTALINFO* RtInfo);

	CPet*		GetPet(DWORD ItemDBIdx);
	PET_TOTALINFO* GetPetTotalInfo(DWORD dwItemDBIdx, int flg = eWithNULLOBJ);
	BOOL		CheckPetDefaultFriendship(DWORD dwItemDBIdx);
	CPet*		GetPetFromSummonItemDBIdx(DWORD dwItemDBIdx);
	CPet*		GetCurSummonPet() {	return m_pCurSummonPet;	}
	void		SetCurSummonPetNull()	{	m_pCurSummonPet = NULL;	}

//	void		SummonPet(DWORD dwPetID);
	void		SummonPet(DWORD dwItemDBIdx, BOOL bSummonFromUser = TRUE);
	//SW061211 크리스마스이벤트
	BOOL		CheckBeforeSummonPet(DWORD dwItemDBIdx);
	void		SummonEventPet();
	void		SetCurSummonPetKind(WORD kind)	{ m_wPetKind = kind;	}
	BOOL		CheckCurSummonPetKindIs(int kind);
	void		CheckEventPetSummonRemainTime();

	void		SealPet();

	void		ExchangePet(DWORD dwItemDBIdx, CPlayer* pNewMaster, BOOL bReduceFriendship = TRUE );
	BOOL		IsCurPetSummonItem(DWORD dwItemDBIdx);
	void		FeedUpPet(DWORD dwFeedAmount);
	
	void		PetProcess();	//함수,변수이름은 자세히 또는 구별할수 있게!
	void		CheckPosValidDistWithMaster();

	void		CheckStaminaZero();

	void		CalcPetSkillRecharge();
	BOOL		IsSkillGuageFull()	{	return m_bSkillGuageFull;	}

	void		ReleaseMoveWithDelay(DWORD delayTime);
	void		CheckDelayRelease();

	void		NetworkMsgParse(BYTE Protocol, void* pMsg);

	void		UpdateCurPetInfo();
	void		UpdateLogoutToDB();

	void		SetSommonPetStamina(BYTE bFlag);
	int			SetSommonPetFriendship(DWORD dwFriendship);
	void		SetSommonPetSkillReady();

	void		SetPetBuffInfo();
	void		GetPetBuffResultRt(int BuffKind, void* Data);
	void		RefleshPetMaintainBuff();

	void		SetPetEquipOption(DWORD ItemIdx, BOOL bAddOpt);
	PETEQUIP_ITEMOPTION*	GetPetEquipOption()	{	return &m_PetEquipOption;	};

	void		SetPetSummonning(int flag);	//맵이동시 펫 소환상태 DB저장
	void		CheckSummonningPet();

	void		SetResummonCheckTime();
	BOOL		CheckResummonAvailable();

	void		SendPetInfo(PET_TOTALINFO* pPetInfo);

	//SW070531 펫 친밀도 보호기능
	void		SetReduceAmountPetFriendship(CObject* pAttacker);
	void		ReducePetFriendshipWithMastersRevive();

	/*CPet에 넣어주자.
	void		CalcStamina();
	void		CalcFriendship();
	void		CalcSkillRecharge();
	*/

};
