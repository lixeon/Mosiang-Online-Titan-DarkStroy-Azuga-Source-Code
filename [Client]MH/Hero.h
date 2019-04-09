// Hero.h: interface for the CHero class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HERO_H__736E5181_650C_4E06_A8F5_1EEAF4C74F89__INCLUDED_)
#define AFX_HERO_H__736E5181_650C_4E06_A8F5_1EEAF4C74F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Player.h"
#include "CAction.h"
#include "AbilityGroup.h"
#include "../[CC]Skill/DelayGroup.h"
#include "..\[CC]Header\GameResourceStruct.h"

class CHero : public CPlayer  
{
	CDelayGroup		m_DelayGroup;

	//{
	HERO_TOTALINFO		m_HeroTotalInfo;
	EXPTYPE				m_maxExpPoint;
	//}

	player_calc_stats m_charStats;
	player_calc_stats m_itemStats;
	SET_ITEM_OPTION m_setItemStats;	// 2007. 6. 18. CBH - 세트아이탬 능력치 구조체 추가
	UNIQUE_ITEM_OPTION_INFO m_UniqueItemStats;	// magi82 - UniqueItem(070626)

	CAction		m_MovingAction;
	CAction		m_NextAction;

	CAction		m_SkillStartAction;		// 무공발동 상태가 지나면 시작할 엑션

	//////////////////////////////////////////////////////////////////////////
	DWORD		m_CurComboNum;

	BOOL		m_bIsAutoAttackMode;
	BOOL		m_bIsAutoAttacking;
	CActionTarget m_AutoAttackTarget;
	
	BOOL		m_bSkillCooling;
	DWORD		m_SkillCoolTimeDuration;
	DWORD		m_SkillCoolTimeStart;
	//////////////////////////////////////////////////////////////////////////

	BOOL		m_bIsKyungGongMode;
	BYTE		m_PyogukNum;

	CAbilityGroup m_AbilityGroup;

	BOOL		m_bUngiFlag;
	
	// Play Time
	stPlayTime	m_stPlayTime;

	DWORD	m_dwLastSocietyAction;

	BOOL		m_bActionPenalty;

	//
	
public:
	CHero();
	virtual ~CHero();
	void InitHero(HERO_TOTALINFO * pHeroTotalInfo);
	void GetHeroTotalInfo(HERO_TOTALINFO * info);
	HERO_TOTALINFO* GetHeroTotalInfo()		{	return &m_HeroTotalInfo;		}
	void Process();

	CDelayGroup*	GetDelayGroup()		{	return &m_DelayGroup;	}
	CAbilityGroup*	GetAbilityGroup()	{	return &m_AbilityGroup;	}

	player_calc_stats* GetCharStats()			{	return &m_charStats;		}
	player_calc_stats* GetItemStats()			{	return &m_itemStats;		}
	// 2007. 6. 18. CBH - 세트아이탬 능력치 구조체 반환 함수 추가
	SET_ITEM_OPTION* GetSetItemStats()		{	return &m_setItemStats;		}	
	UNIQUE_ITEM_OPTION_INFO* GetUniqueItemStats(){	return &m_UniqueItemStats;	}	// magi82 - UniqueItem(070626)
	
	// InitXXX계열의 함수 호출 후  인터페이스에 표시할 필요가있는 것들 위해
	void ChangeLife(int changeval);
	void ChangeShield(int changeval);
	void ApplyInterface();
	virtual void SetMaxLife(DWORD maxlife);
	virtual void SetMaxShield(DWORD maxlife);
	virtual void SetLife(DWORD life, BYTE type = 1);
	virtual void SetShield(DWORD life, BYTE type = 1);
	virtual DWORD GetNaeRyuk(){ return m_HeroTotalInfo.wNaeRyuk; }
	virtual DWORD DoGetMaxNaeRyuk();
	virtual void SetMaxNaeRyuk(DWORD val);
	virtual void SetNaeRyuk(DWORD val, BYTE type = 1);
	virtual DWORD DoGetPhyDefense()
	{ return (int)(GetCharStats()->PhysicalDefense+GetItemStats()->PhysicalDefense+GetAbilityStats()->DefenceUp+GetSetItemStats()->wPhyDef
	+GetUniqueItemStats()->nDefen) >= 0 ? GetCharStats()->PhysicalDefense+GetItemStats()->PhysicalDefense+GetAbilityStats()->DefenceUp+
	GetSetItemStats()->wPhyDef+GetUniqueItemStats()->nDefen : 0; }
	virtual float DoGetAttDefense(WORD Attrib)
	{ return GetItemStats()->AttributeResist.GetElement_Val(Attrib) + GetAbilityStats()->AttRegistUp.GetElement_Val(Attrib) + GetSetItemStats()->AttrRegist.GetElement_Val(Attrib); }	
	virtual DWORD DoGetPhyAttackPowerMin();
	virtual DWORD DoGetPhyAttackPowerMax();
#ifdef _JAPAN_LOCAL_
	virtual float DoGetAddAttackRange(){	return (float)GetMinChub() / 2.f;	}
#else
	virtual float DoGetAddAttackRange(){	return GetMinChub() / 3.f;	}
#endif

	virtual DWORD DoGetCritical(){	return GetCharStats()->Critical + GetItemStats()->Critical + GetSetItemStats()->wCriticalPercent;	}
	virtual DWORD DoGetDecisive(){	return GetCharStats()->Decisive + GetItemStats()->Critical + GetSetItemStats()->wCriticalPercent;	}

	LEVELTYPE GetMaxLevel()		{	return m_HeroTotalInfo.MaxLevel;	}
	void SetMaxLevel( LEVELTYPE Level )			{	m_HeroTotalInfo.MaxLevel = Level;	}

	void SetGuageName(char * szName);
	
	MONEYTYPE GetMoney() { return m_HeroTotalInfo.Money; }
	void SetMoney(MONEYTYPE Money);
	EXPTYPE GetExpPoint() { return m_HeroTotalInfo.ExpPoint; }
	EXPTYPE GetMaxExpPoint() { return m_maxExpPoint; }
	void SetExpPoint(EXPTYPE dwPoint, BYTE type=1);
	void SetMaxExpPoint(EXPTYPE dwPoint);
	virtual void SetLevel(LEVELTYPE level);
	
	void SetMunpa();

	void SetGenGol(WORD val);
	void SetMinChub(WORD val);
	void SetCheRyuk(WORD val);
	void SetSimMek(WORD val);

	// magi82 - UniqueItem(070629) - 스텟이 마이너스가 될 경우 예외처리 추가
	WORD GetGenGol(){ return (int)(m_HeroTotalInfo.wGenGol + GetAbilityStats()->StatGen + GetItemStats()->GenGol + GetShopItemStats()->Gengol
		+ GetAvatarOption()->Gengol + GetSetItemStats()->wGenGol + GetUniqueItemStats()->nGengol) >= 0 ? (WORD)(m_HeroTotalInfo.wGenGol
		+ GetAbilityStats()->StatGen + GetItemStats()->GenGol + GetShopItemStats()->Gengol + GetAvatarOption()->Gengol + GetSetItemStats()->wGenGol
		+ GetUniqueItemStats()->nGengol) : 0; }

	WORD GetMinChub(){ return (int)(m_HeroTotalInfo.wMinChub + GetAbilityStats()->StatMin + GetItemStats()->MinChub + GetShopItemStats()->Minchub
		+ GetAvatarOption()->Minchub + GetSetItemStats()->wMinChub + GetUniqueItemStats()->nMinChub) >= 0 ? (WORD)(m_HeroTotalInfo.wMinChub
		+ GetAbilityStats()->StatMin + GetItemStats()->MinChub + GetShopItemStats()->Minchub + GetAvatarOption()->Minchub + GetSetItemStats()->wMinChub
		+ GetUniqueItemStats()->nMinChub) : 0; }

	WORD GetCheRyuk(){ return (int)(m_HeroTotalInfo.wCheRyuk + GetAbilityStats()->StatChe + GetItemStats()->CheRyuk + GetShopItemStats()->Cheryuk
		+ GetAvatarOption()->Cheryuk + GetSetItemStats()->wCheRyuk + GetUniqueItemStats()->nCheRyuk) >= 0 ? (WORD)(m_HeroTotalInfo.wCheRyuk
		+ GetAbilityStats()->StatChe + GetItemStats()->CheRyuk + GetShopItemStats()->Cheryuk + GetAvatarOption()->Cheryuk + GetSetItemStats()->wCheRyuk
		+ GetUniqueItemStats()->nCheRyuk) : 0; }

	WORD GetSimMek(){ return (int)(m_HeroTotalInfo.wSimMek + GetAbilityStats()->StatSim + GetItemStats()->SimMaek + GetShopItemStats()->Simmek
		+ GetAvatarOption()->Simmek + GetSetItemStats()->wSimMek + GetUniqueItemStats()->nSimMek) >= 0 ? (WORD)(m_HeroTotalInfo.wSimMek
		+ GetAbilityStats()->StatSim + GetItemStats()->SimMaek + GetShopItemStats()->Simmek + GetAvatarOption()->Simmek + GetSetItemStats()->wSimMek
		+ GetUniqueItemStats()->nSimMek) : 0; }
	
	void SetAbilityExp(DWORD val)	{	m_AbilityGroup.SetAbilityExp(val);	}
	DWORD GetAbilityExp()			{ return m_AbilityGroup.GetAbilityExp(); 	}

	int GetMugongLevel(WORD MugongIdx);
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	WORD GetSkillOptionIndex(WORD MugongIdx);
	//////////////////////////////////////////////////////////////////////////
	
	void SetFame(FAMETYPE val);
	FAMETYPE GetFame()			{ return m_HeroTotalInfo.Fame;	}
	virtual void SetBadFame( FAMETYPE val );

	DWORD GetPartyIdx() { return m_HeroTotalInfo.PartyID; }
	void SetPartyIdx(DWORD PartyId) { m_HeroTotalInfo.PartyID = PartyId;	}
/*문파*/		
	void SetMunpaName(char* Name);
	
	void SetGuildName(char* Name);
	char* GetGuildName();
	virtual void ClearGuildInfo();
	char* GetGuildEntryDate();
	void SetGuildEntryDate(char* day);
	
	BYTE GetPyogukNum()				{ return m_PyogukNum;	}
	void SetPyogukNum(BYTE num)		{ m_PyogukNum = num;	}
	
	// stage
	virtual void	SetStage( BYTE Stage );

public:

	//////////////////////////////////////////////////////////////////////////
	// SkillManager에서 쓰는 함수들
	BOOL IsAutoAttacking()	{	return m_bIsAutoAttacking;	}
	CActionTarget* GetAutoAttackTarget()	{	return &m_AutoAttackTarget;	}

	void SetMovingAction(CAction* pAction)	{	m_MovingAction.CopyFrom(pAction);	}
	void SetNextAction(CAction* pAction)	{	m_NextAction.CopyFrom(pAction);	}
	void SetSkillStartAction(CAction* pAction)	{	m_SkillStartAction.CopyFrom(pAction);	}

	CAction* GetNextAction()	{	return &m_NextAction;	}

	DWORD GetCurComboNum()			{ return m_CurComboNum < MAX_COMBO_NUM ? m_CurComboNum : 0;	}
	void SetCurComboNum(DWORD num)	{ m_CurComboNum = num;	}

	void EnableAutoAttack(CActionTarget* pTarget)
	{	if(m_bIsAutoAttackMode && pTarget->GetTargetKind() == eActionTargetKind_Object){
		m_bIsAutoAttacking = TRUE;	m_AutoAttackTarget.CopyFrom(pTarget); }
	}
	void DisableAutoAttack()
	{	m_bIsAutoAttacking = FALSE; m_AutoAttackTarget.CopyFrom(NULL); }
	void ToggleAutoAttackMode()
	{	m_bIsAutoAttackMode = !m_bIsAutoAttackMode;	}

	void ClearTarget(CObject* pObject);

	BOOL IsSkillCooling()	{	return m_bSkillCooling;	}
	void SetSkillCoolTime(DWORD CoolTime)	{
		m_SkillCoolTimeDuration = CoolTime;
		m_SkillCoolTimeStart = gCurTime;
		m_bSkillCooling = TRUE;
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 경공
	BOOL GetKyungGongMode();
	void SetKyungGongMode(BOOL bMode);
	//////////////////////////////////////////////////////////////////////////	
	
	//////////////////////////////////////////////////////////////////////////
	// ObjectStateManager에서 State의 시작과 끝에서 호출해주는 함수들
	void OnStartObjectState(BYTE State);
	void OnEndObjectState(BYTE State);
	
//KES
	virtual BOOL StartSocietyAct( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE );
	virtual BOOL EndSocietyAct();
	void	StartSocietyActForQuest( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE );
	BOOL CanSocietyAction( DWORD curTime );
	//////////////////////////////////////////////////////////////////////////

	void OnHitTarget(CObject* pMainTarget);
	
	// 행동 함수들...
	void Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive);
	void Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage);
	void Revive(VECTOR3* pRevivePos);
	void Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal);

	friend class CAttackManager;

	WORD GetKyungGongGrade()	{	return m_HeroTotalInfo.KyungGongGrade;	}
	void SetKyungGongGrade(WORD grade);
	
	virtual void AddStatus(CStatus* pStatus);
	virtual void RemoveStatus(CStatus* pStatus);
	

	// 서버를 변경하고 아무런 처리가 없을겨우 사용할것..
	void HeroStateNotify(BYTE State);
	
	BYTE GetNaeRyukPercent();

	// PlayTime
	void SetPlayTime(int time)	{	m_stPlayTime.value = time;	}
	int GetPlayTime()			{	m_stPlayTime.value;	}
/*	void GetPlayTime(int& Year, int& Day, int& Hour, int& Minute, int& Second)
	{
		m_stPlayTime.GetTime(Year, Day, Hour, Minute, Second);
	}*/

	//
	void CalcShopItemOption( WORD wIdx, BOOL bAdd, DWORD Param=0 );
	void ActiveOptionInfoToInterface();
	void CheckShopItemUseInfoToInventory( WORD ItemIdx, WORD ItemPos );

	void RefreshLevelupPoint();
	void SetHeroLevelupPoint( LEVELTYPE dwPoint )	{	m_HeroTotalInfo.LevelUpPoint = dwPoint;	}
	DWORD GetHeroLevelUpPoint()					{	return m_HeroTotalInfo.LevelUpPoint;	}
	
#ifdef _JAPAN_LOCAL_
	DWORD GetExtraInvenSlot()					{	return m_CharacterInfo.ExtraInvenSlot;		}
	DWORD GetExtraPyogukSlot()					{	return m_CharacterInfo.ExtraPyogukSlot;		}
	DWORD GetExtraMugongSlot()					{	return m_CharacterInfo.ExtraMugongSlot;		}

//KES: Show AttrAttack Damage
	DWORD DoGetAttAttackPowerMax( WORD Attrib );
	DWORD DoGetAttAttackPowerMin( WORD Attrib );
	DWORD GetAttribPlusPercent( WORD Attrib );

#endif
#ifdef _HK_LOCAL_
	DWORD GetExtraInvenSlot()					{	return m_CharacterInfo.ExtraInvenSlot;		}
	DWORD GetExtraPyogukSlot()					{	return m_CharacterInfo.ExtraPyogukSlot;		}
	DWORD GetExtraMugongSlot()					{	return m_CharacterInfo.ExtraMugongSlot;		}
#endif
#ifdef _TL_LOCAL_
	DWORD GetExtraInvenSlot()					{	return m_CharacterInfo.ExtraInvenSlot;		}
	DWORD GetExtraPyogukSlot()					{	return m_CharacterInfo.ExtraPyogukSlot;		}
	DWORD GetExtraMugongSlot()					{	return m_CharacterInfo.ExtraMugongSlot;		}
#endif

	////////////////////////////////////////////////////////
	//06. 06 2차 전직 - 이영준
	//이펙트 생략(무초)
	BOOL IsSkipSkill();
	////////////////////////////////////////////////////////

	//SW070127 타이탄
	BOOL CanUseTitanSkill();

	//////2007. 6. 14. CBH - 세트아이탬 능력치 계산 함수 추가
	void AddSetitemOption(SET_ITEM_OPTION* pSetItemOption);
	void ClearSetitemOption();
	//////////////////////////////////////////////////////////


	void SetNoActionPenalty()					{	m_bActionPenalty = FALSE;		}

	// magi82 - UniqueItem(070626)
	void AddUniqueItemOption(UNIQUE_ITEM_OPTION_INFO* pUniqueOption);
	void ClearUniqueItemOption()	{	ZeroMemory(&m_UniqueItemStats, sizeof(m_UniqueItemStats));	}
	//////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_HERO_H__736E5181_650C_4E06_A8F5_1EEAF4C74F89__INCLUDED_)
