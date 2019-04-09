#pragma once
#include "object.h"
#include "..\[CC]Header\GameResourceStruct.h"

class CPlayer;
struct BASE_PET_LIST;

//enum{ePET_FROM_DB, ePET_FROM_ITEM};		//DB로부터 펫생성, 소환아이템 첫사용으로 팻생성
#define PET_DEFAULT_FRIENDLY 3000000		//펫 기본 친밀도, 3000/10000
#define PET_REVIVAL_FRIENDLY 2000000		//펫 부활후 친밀도
#define PET_MAX_FRIENDLY	10000000		//펫 최대 친밀도 100%, 정수로 계산한다. 클라이언트 표시만 소수로..
//#define PET_FRIENDSHIP_PER_SKILL_GRADE1		10	//펫 스킬 시전시 친밀도 증가량
//#define PET_FRIENDSHIP_PER_SKILL_GRADE2		5	//펫 스킬 시전시 친밀도 증가량

#define PET_STATE_CHECK_TIME 10000		//펫 상태 체크 시간, 스테미너 관련

#define PET_MAX_GRADE		3


//enum eSTAMINA_DECREASE_AMOUNT{eSDA_REST = 5, eSDA_STAND = 10, eSDA_MOVE = 20,};	//eSDA

enum eFRIENDSHIP_INCREASE_AMOUNT	//eFIA
{
	//eFIA_GRADE1_SKILLOPER = 10,
	//eFIA_GRADE1_PLAY1HOUR = 20,
	//eFIA_GRADE2_SKILLOPER = 5,
	//eFIA_GRADE2_PLAY1HOUR = 10,
	eFIA_MASTER_DIE		= -1000000,
//	eFIA_STAMINA_ZERO	= -100,		//10분마다
	eFIA_STAMINA_ZERO	= -2000,		//10초마다
	eFIA_TRADE			= -1000000,
	eFIA_UPGRADE_FAIL	= -1000000,
};

enum ePET_MOTION		//chx 파일내 ani 순서.. ANIMATION INDEX
{
	ePM_STAND = 1,		//기본
	ePM_MOVE,			//이동
	ePM_SUBSTAND,		//보조
	ePM_KGONG,			//경공이동
	ePM_UNGI,			//캐릭 운기시
	ePM_MASTER_SKILL,	//캐릭 공격시 응원
	ePM_MASTER_DIE,		//캐릭 죽음시
	ePM_SKILL,			//펫스킬 사용시
	ePM_DIE,			//펫죽음
	ePM_STAMINA_ZERO,	//스태미나 0일 때 (대사만)
	ePM_DIED = 10,		//펫죽음상태 홀딩(Client에서만 사용)
	//기획에서 펫 나중에 추가작업으로 스태미나 0일때 대사추가. 실제 애니데이터는 '죽은상태'가 들어있음.

};

enum ePET_BASIC_STATE
{
	ePBS_NONE,
	ePBS_MOVING,
	ePBS_UNGI,
};

enum ePET_MOTION_FREQUENCY
{
	ePMF_RANDOM,
	ePMF_ALWAYS,
};

class CPet : public CObject
{
	PET_TOTALINFO	m_PetTotalInfo;
	DWORD			m_dwMaxStamina;
//	sPetState		m_PetState;

	DWORD			m_dwStateCheckTime;		//펫 상태 설정 체크	> 확률에 의해 모션과 대사 정보를 보낸다.
	DWORD			m_dwRndGapTime;			//펫 상태 랜덤 텀.	10초에서 60초까지.. > 설정 체크의 차이를 둔다.
	BYTE			m_CurBasicState;		//펫 3가지 기본 상태

	DWORD			m_dwInfoCheckTime;		//펫 정보 체크
	DWORD			m_dwStaminaDecrease;	//10초간 펫 스태미나 소모량
	WORD			m_wFrameCounter;		//10초간 프레임 카운터

	CPlayer*		m_pMaster;
	BASE_PET_LIST*	m_pBaseInfo;
//	BOOL			m_bSeal;
//	BOOL			m_bRest;

public:
	CPet(void);
	virtual ~CPet(void);

	virtual void DoDie(CObject* pAttacker);
	//virtual void OnStartObjectState(BYTE State,DWORD dwParam);
	//virtual void OnEndObjectState(BYTE State);
	virtual void Release();

	void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);

	virtual float DoGetMoveSpeed();

	//초기화
	//정보 세팅
	void	InitPet(PET_TOTALINFO* pTotalinfo);
	void	SetMaster(CPlayer* pPlayer) {m_pMaster=pPlayer;}
	CPlayer*	GetMaster() {	return m_pMaster;	}

	void	GetPetTotalInfoRt(PET_TOTALINFO* pRtInfo);
	void	GetPetMasterNameRt(char* pRtMasterName);
	const PET_TOTALINFO* GetPetTotalInfo()	{	return &m_PetTotalInfo;	}
	DWORD	GetPetSummonItemDBIdx()	{	return m_PetTotalInfo.PetSummonItemDBIdx;	}
	WORD	GetPetKind()	{	return m_PetTotalInfo.PetKind;	}
	void	SetPetStamina(DWORD dwStamina);
	void	SetPetMaxStamina(DWORD val, BOOL bSendMsg = FALSE);
	void	SetPetFriendShip(DWORD dwFriendShip);
	void	SetPetAlive(BOOL bVal)	{	m_PetTotalInfo.bAlive = bVal;	}

	//SW060509 GM 선택펫치트
	DWORD	GetPetFriendShip()	{ return m_PetTotalInfo.PetFriendly; }
	//봉인상태
//	BOOL	IsPetSealed()	{	return m_bSeal;	}
//	void	SetPetSeal(BOOL bSeal)	{m_bSeal=bSeal;}

	//펫상태
	void	PetGradeUp(WORD wPetLevel);
	DWORD	GetPetCurGrade()	{	return m_PetTotalInfo.PetGrade;	}
	void	CheckPetMoving();
	BOOL	IsPetMoving()	{	return m_MoveInfo.bMoving;	}
	void	SetPetRest(BOOL bRest) {	m_PetTotalInfo.bRest = bRest;	}
	BOOL	IsPetRest()		{	return m_PetTotalInfo.bRest;	}
	void	SetPetSummonning(BOOL bVal)	{	m_PetTotalInfo.bSummonning = bVal;	}
	void	CheckCurBasicState();
	void	SetPetBasicState(BYTE basicstate) {	m_CurBasicState = basicstate;	}
	BYTE	GetPetBasicState()	{	return m_CurBasicState;	}
	BYTE	GetMotionNumFromBasicState();
	void	GetPetMotionFromBasicState();		//몇초 마다 세가지 상태에서의 모션 구함.
	void	GetRandMotionNSpeech(DWORD state=0, BYTE frequency=ePMF_RANDOM);	//모션과 대사는 항상 같이.. 모션만 나올 수 있음.

//	void	SetPetActionState(DWORD state)	{	m_PetState.stateOld=m_PetState.stateCur;m_PetState.stateCur=state;	}
//	DWORD	GetPetCurActionState()	{	return m_PetState.stateCur;	}

	void	Process();
	const BASE_PET_LIST* GetPetBaseInfo() {	return m_pBaseInfo;	}

	//펫스탯
	void	CalcPetMaxStamina();
	void	CalcStamina();					//스테미나 0이면 10분 마다 친밀도 1% 감소
	void	AddStamina(int stamina, BOOL bSendMsg = FALSE);
	//void	PlusStamina(DWORD amount);
	void	AddFriendship(int friendship, BOOL bSendMsg = FALSE);
	void	CalcFriendship();
	void	CalcSkillRecharge();
	BOOL	IsPetMaxFriendship();
	BOOL	IsPetStaminaZero()		{	return (0 == m_PetTotalInfo.PetStamina);	}
	BOOL	IsPetStaminaFull();

	//펫 교환	//메니져로 옮기자...
	//BOOL	IsCanExchange()	{	return (m_PetTotalInfo.PetFriendly>PET_DEFAULT_FRIENDLY);	}

	//펫 스킬
	BOOL	UsePetSkill();

	void	SendPetInfoMsg();
	void	SendPetDieMsg();

};
