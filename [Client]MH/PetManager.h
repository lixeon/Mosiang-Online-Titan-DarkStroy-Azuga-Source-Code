#pragma once

#include "./Interface/cImage.h"

#define PETMGR	USINGTON(CPetManager)

#define PET_DIST_CHECKTIME 500
#define PET_MAX_FRIENDSHIP		10000000
#define PET_SKILLCHARGE_CHECKTIME	1000
#define PET_MAX_SKILL_CHARGE	10000
#define PET_RESUMMON_VALID_TIME	30000

#define MAX_PET_BUFF_NUM	3

#define CRISTMAS_EVENTPET 8

enum ePetFeedResult{ePFR_Sucess=0, ePFR_Unsummoned, ePFR_StaminaFull};

enum ePetUpgradeDlgGrade{ePUDG_Default, ePUDG_Grade2=1, ePUDG_Grade3=2};

enum ePetRevivalDlgGrade{ePRDG_Default, ePRDG_Grade1, ePRDG_Grade2, ePRDG_Grade3, ePRDG_Grade_All};	//ePRDG_Grade_All => ShopItem 용

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

enum {ePetRestBtn,ePetUseBtn};

//펫종류
enum ePetKind{ePK_None, ePK_CommonPet=1, ePK_ShopItemPet=2, ePK_EventPet=4,};

class CPet;

class cDialog;
class CPetStateDlg;
class CPetStateMiniDlg;
class CPetInventoryDlg;

class CPetManager
{
	CYHHashTable<PET_TOTALINFO>		m_PetInfoList;
	CYHHashTable<cImage>			m_PetImageList;

	DWORD				m_dwStateCheckTime;				//펫 스탯 체크 타임( 10초 )
	DWORD				m_dwStaminaDecrease;			//스테미나 감소량
	DWORD				m_dwOldFriendShipForToolTipChange;	//툴팁 친밀도 갱신을 위한 저장

	DWORD				m_dwDistCheckTime;					//프로세스 타임(주인-펫거리체크)
	BOOL				m_bValidDistance;				//움직여야 될 거리인가
	BOOL				m_bReadytoMove;					//움직여도 될 조건인가(movemgr에서 세팅)

	DWORD				m_dwSkillRechargeCheckTime;		//스킬게이지 체크 타임(1초)
	DWORD				m_dwSkillRechargeAmount;
	BOOL				m_bSkillGuageFull;
	BOOL				m_bReadyToSendSkillMsg;

	DWORD				m_dwResummonDelayTime;			//펫 재소환 딜레이//봉인후 30초 카운트

	CPet*				m_pCurSummonPet;
	BOOL				m_bIsPetStateDlgToggle;			//토글 상태 저장
	cDialog*			m_pCurPetStateDlg;				//현재 펫 상태창
	CPetStateDlg*		m_pStateDlg;					//펫 상태큰창
	CPetStateMiniDlg*	m_pStateMiniDlg;				//펫 상태작은창
	CPetInventoryDlg*	m_pInvenDlg;
	BOOL				m_bReadyToSendRestMsg;			//펫 휴식 상태 설정정보 서버 리턴 뒤 TRUE

	WORD				m_wPetKind;						//펫종류. 일반/아이템몰/이벤트 펫

public:
	CPetManager(void);
	~CPetManager(void);

	void	PetMgrInit();
	void	PetMgrRelease();
	void	PetMgrProcess();
	void	AddHeroPet(CPet* pPet);
	void	InitPetInfo(PET_TOTALINFO* pPetInfo, int num);
	void	AddPetInfo(PET_TOTALINFO* pPetInfo);
	void	RemovePetInfo(DWORD dwSummonItemDBIdx);
	void	ReleasePetInfoList();
	PET_TOTALINFO*	GetPetInfo(DWORD dwItemDBIdx);
	void	UpdateCurPetInfo(CPet* pPet);
	void	OnPetRemove(CPet* pPet);

	void	SetResummonCheckTime();
	BOOL	CheckResummonAvailable();
	DWORD	GetPetResummonRestTime();

	BOOL	CheckDefaultFriendship(DWORD dwItemDBIdx);
	DWORD	GetPetFriendship(DWORD dwItemDBIdx);
	BOOL	IsCurPetStaminaFull();
	BOOL	IsCurPetSummonItem(DWORD dwItemDBIdx);
	void	RemovePetFromTable(DWORD dwPetID);
	void	SetCurSummonPet(CPet* pPet)	{	m_pCurSummonPet=pPet;	}
	CPet*	GetCurSummonPet() {	return m_pCurSummonPet;	}
	CPet*	GetHeroPet(DWORD dwPetID);
	DWORD	GetHeroPetID();
	BOOL	CheckPetSummoned();
	BOOL	CheckPetAlive(DWORD dwSummonItemDBIdx);
	void	ReleasePetTable();

	void	NetworkMsgParse(BYTE Protocol, void* pMsg);

//펫 스탯
	//스킬 게이지 충전
	void	CalcSkillRecharge();
	void	SetSkillRechargeAmount(DWORD amount);
	BOOL	IsSkillGuageFull()	{	return m_bSkillGuageFull;	}
	void	SetSkillGuageFull(BOOL bVal)	{m_bSkillGuageFull = bVal;}

	//따라가기
	void	CheckDistWithMaster();
	BOOL	IsValidDist() {	return m_bValidDistance;	}
	void	SetMoveReady(BOOL bVal)	{	m_bReadytoMove = bVal;	}

	//UI
	void	OpenPetStateDlg();		//현재 펫 상태창 열기
	void	OpenPetInvenDlg();
	void	SetCurPetStateDlg(cDialog* pDlg) {m_pCurPetStateDlg=pDlg;}	//현재 펫 상태창 세팅
	void	SetPetStateDlg(CPetStateDlg* pDlg) {m_pStateDlg=pDlg;}
	void	SetPetStateMiniDlg(CPetStateMiniDlg* pDlg) {m_pStateMiniDlg=pDlg;}
	void	SetPetInventoryDlg(CPetInventoryDlg* pDlg) {m_pInvenDlg=pDlg;}
	//060310 UI 저장
	void	SetPetDlgToggle(BOOL bVal)	{	m_bIsPetStateDlgToggle = bVal;	}	//FALSE 가 큰창(기본)
	BOOL	GetPetDlgToggle()	{	return m_bIsPetStateDlgToggle;	}
	cDialog* GetCurPetStateDlg()	{	return m_pCurPetStateDlg;	}
	void	TogglePetStateDlg();
	void	SetPetStateDlgInfo(CPet* pPet);
	void	SetPetStateMiniDlgInfo(CPet* pPet);
	void	SetPetStateDlgUseRestInfo(CPet* pPet);
	void	ClosePetAllDlg();

	void	SetPetGuageText(DWORD dwStamina, DWORD dwFriendShip);

	DWORD	GetPetValidInvenMaxTabNum();
	void	SetPetValidInvenTab();

//	BOOL	GetPetDlgToggle() {return m_bIsPetStateDlgToggle;}			//토글상태가져오기. 큰창/작은창
	
	//rest
	void	SetCurPetRest(BOOL bRest);
	void	SendPetRestMsg(BOOL bRest);//펫 사용/휴식 정보 보내기
	void	SetReadyToSendRestMsg(BOOL bReady) {	m_bReadyToSendRestMsg = bReady;	}
	BOOL	IsReadyToSendRestMsg()	{	return m_bReadyToSendRestMsg;	}

	//seal
	void	SendPetSealMsg();

	//ability icon. pet skill
	void	CheckRestNSkillUse();	//무공창 스킬버튼 기능. 휴식상태면 해제시키고 게이지풀이면 스킬 사용. 기획자에게 문의후 알게됨.

	//skill
	void	InitPetSkillGuage();
	void	UseCurPetSkill();
	void	SetReadyToSendSkillMsg(BOOL bReady) {	m_bReadyToSendSkillMsg = bReady;	}
	BOOL	IsReadyToSendSkillMsg()	{	return m_bReadyToSendSkillMsg;	}
	//SW060324 펫 버프 추가
	void	AddMasterStatFromPetBuff(player_calc_stats* pMasterAdditionalStat);
	void	RefleshPetMaintainBuff();

	//pet equip
	WORD	GetPetEquipItemMax(DWORD dwItemDBIdx);

	void	LoadPetImage();
	cImage* GetPetImage(DWORD dwPetIdx);
	void	RemovePetImage();

	void	Release();

	void	SetCurSummonPetKind(CPet* pPet);
	BOOL	CheckCurSummonPetKindIs(int kind);
};

EXTERNGLOBALTON(CPetManager)

//UI 정보관리
//HERO's Pet 관리, PetObject는 하나만 있다. 모든 소유 펫들의 기본정보만 관리.
//HERO의 펫 소환 아이템에 가지고 있는 펫 아이템의 펫 정보를 가지고 있다. 아이템 툴팁 등에 보여준다.
