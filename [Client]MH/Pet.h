#pragma once
#include "object.h"
#include "GameResourceStruct.h"

#define DEFAULT_PET_FOLLOW_DIST	300		//4m 거리 유지
#define MAX_LIMIT_PET_DIST	1000		//10m 이상이면 워프
#define PET_STATE_CHECK_TIME 10000		//펫 상태 체크 시간, 스테미너 관련
#define PET_DEFAULT_FRIENDLY	3000000		//펫 기본 친밀도, 3000/10000
#define PET_REVIVAL_FRIENDLY	2000000	//펫 부활후 친밀도
#define PET_REDUCE_FRIENDLY_ZEROSTAMINA	1000

#define PET_MAX_GRADE		3

enum eSTAMINA_DECREASE_AMOUNT{eSDA_REST = 50, eSDA_STAND = 100, eSDA_MOVE = 200,};

//enum ePET_ACTION
//{
//	ePA_REST		=	1,
//	ePA_STAND		=	2,	ePA_SUBSTAND	=	3,
//	ePA_MOVE		=	4,	ePA_KGONG		=	5,
//	ePA_SKILL		=	8,
//	ePA_APPEAL		=	16,	ePA_DESPAIR		=	17,
//	ePA_DIE			=	32,
//	ePA_UNGI		=	64,
//};

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

struct sPetState
{
	sPetState() : stateOld(0),stateCur(0) {}
	DWORD	stateOld;
	DWORD	stateCur;
};

class CPet :	public CObject
{
	PET_TOTALINFO	m_PetTotalInfo;
	sPetState		m_PetState;
	char			m_MasterName[MAX_NAME_LENGTH+1];
	DWORD			m_dwMaxStamina;

	DWORD			m_dwStateCheckTime;
	DWORD			m_dwStaminaDecrease;
	BOOL			m_bRest;

	BASE_PET_LIST*	m_pSInfo;
	DWORD			m_dwMasterID;
	CPlayer*		m_pMaster;

	void InitPet(PET_TOTALINFO* pTotalInfo);
public:

	friend class CObjectManager;
	friend class CPetManager;

	CPet(void);
	virtual ~CPet(void);

	//펫상태
	void	PetGradeUp();
	DWORD	GetPetIdx()			{	return m_PetTotalInfo.PetKind;	}
	DWORD	GetPetCurGrade()	{	return m_PetTotalInfo.PetGrade;	}
	DWORD	GetPetSommonItemDBIdx()	{	return m_PetTotalInfo.PetSummonItemDBIdx;	}

//	void	SetPetActionState(DWORD state)	{	m_PetState.stateOld=m_PetState.stateCur;m_PetState.stateCur=state;	}
//	DWORD	GetPetCurActionState()	{	return m_PetState.stateCur;	}

	//펫휴식(스킬비사용)
	void	SetPetRest(BOOL bVal) {	m_bRest=bVal;	}
	BOOL	IsPetRest()		{	return m_bRest;	}
	BOOL	IsPetStaminaFull();

	void Process();

	//펫스탯
	void	SetMaxStamina(DWORD dwMaxStamina);
	DWORD	GetMaxStamina()	{	return m_dwMaxStamina;	}
	void	CalcStamina();
	void	PlusStamina(DWORD amount);
	void	CalcFriendship();
	void	CalcSkillRecharge();

	void SetMaster(CPlayer* pPlayer);
	void SetMasterID(DWORD dwMasterID)	{	m_dwMasterID = dwMasterID;	}
	CPlayer* GetMaster()	{	return m_pMaster;	}
	void SetPetMasterName(DWORD dwMasterIdx);
	void SetPetMasterName(char* pMasterName);
	BASE_PET_LIST*	GetSInfo() {	return m_pSInfo;	}
	WORD GetPetKind()	{	return m_PetTotalInfo.PetKind;	}
	WORD GetPetGrade()	{	return m_PetTotalInfo.PetGrade;	}
	virtual void SetMotionInState(BYTE State);
	virtual float DoGetMoveSpeed();

	void Die();

//	virtual BOOL StartSocietyAct( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE )	{ return FALSE; }
//	virtual BOOL EndSocietyAct()	{ return TRUE; }

//	virtual void Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical);
//	virtual void Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical) {}
//	virtual void Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal) {}
//	virtual void Recharge(CObject* pHealer,BYTE RechargeKind,DWORD RechargeVal) {}	
//	virtual void Revive(VECTOR3* pRevivePos);

	//////////////////////////////////////////////////////////////////////////
	// ObjectStateManager에서 State의 시작과 끝에서 호출해주는 함수들
//	virtual void OnStartObjectState(BYTE State)	{}
//	virtual void OnEndObjectState(BYTE State)	{}
	//////////////////////////////////////////////////////////////////////////

	// 수치 
//	virtual void SetLife(DWORD life, BYTE type = 1){}
//	virtual DWORD GetLife(){ return 0; }
//	virtual void SetShield(DWORD Shield, BYTE type = 1){}
//	virtual DWORD GetShield(){ return 0; }
//	virtual float GetWeight()		{	return 1.f;	}
//	virtual float GetRadius()		{	return 0.f;	}
//	virtual DWORD GetNaeRyuk()		{	return 0; }
//	virtual void SetNaeRyuk(DWORD val, BYTE type = 1)		{}

//	virtual void SetPosition(VECTOR3* pPos);
//	virtual void GetPosition(VECTOR3* pPos);
//	virtual void SetAngle(float AngleRad);
//	virtual float GetAngle();
//	virtual DIRINDEX GetDirectionIndex();	

//	virtual void ChangeMotion(int motion,BOOL bRoop);
//	virtual void ChangeBaseMotion(int motion);
};
