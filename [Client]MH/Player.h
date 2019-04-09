// Player.h: interface for the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_)
#define AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"

class cStreetStallTitleTip; // LBS 03.09.23

class CPet;
//class CPetBase;
class CTitan;
class CMapObject;

class CPlayer : public CObject  
{
	void InitPlayer(CHARACTER_TOTALINFO* pTotalInfo);

	
	int m_StandardMotion;
	int m_Move_Start_Motion;
	int m_Move_Ing_Motion;
	int m_Move_End_Motion;

	ABILITY_STATS m_Ability;
	
	WANTEDTYPE			m_WantedIDX;

	//cTextStaticWid m_chatToolTip;										// 채팅풍선	
	
	int	m_ItemUseMotion;
	int m_ItemUseEffect;

	//SW070127 타이탄
	BOOL m_bInTitan;
	float m_TitanMoveSpeed;
	BOOL m_bTitanPreView;

	//2007. 8. 8. CBH - 세트아이탬 이펙트
	DWORD m_dwSetItemEffectID;

protected:
	// CHERO에서 접근하기 위해 
	CHARACTER_TOTALINFO m_CharacterInfo;	
	SHOPITEMOPTION		m_ShopItemOption;	
	AVATARITEMOPTION	m_AvatarOption;

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	SKILLSTATSOPTION	m_SkillStatsOption;
	//////////////////////////////////////////////////////////////////////////
	//SW070127 타이탄
	TITAN_APPEARANCEINFO	m_TitanAppearanceInfo;
	TITAN_APPEARANCEINFO	m_TitanPreViewAppInfo;

	// 노점상 타이틀 LBS 03.10.02
	cStreetStallTitleTip* m_pSSTitle;
	//SW051129 Pet
	CPet*				m_pPet;

	CTitan*				m_pTitan;

	// magi82(44) - 노점상 꾸미기 업데이트
	CMapObject*			m_DecorationAddress;
	DWORD				m_dwDecorationStartTime;
	BOOL				m_bDecorationFlag;	// TRUE면 생성되므로 알파값 증기, FALSE면 해제 되므로 알파값 감소

	//2008. 1. 21. CBH - 스킨 딜레이 관련 변수
	DWORD m_dwSkinDelayTime;	//딜레이 총 시간 변수
	BOOL m_bSkinDelayResult;	//딜레이중인지 체크하는 변수
	///////////////////////////////////////////////////

public:
	CPlayer();
	virtual ~CPlayer();

	inline ABILITY_STATS* GetAbilityStats()		{	return &m_Ability;	}
	
	void GetCharacterTotalInfo(CHARACTER_TOTALINFO* pRtInfo);
	CHARACTER_TOTALINFO* GetCharacterTotalInfo()	{	return &m_CharacterInfo;	}

	void Process();

	friend class CObjectManager;
	friend class CAppearanceManager;
	friend void CS_BtnFuncEnter(LONG lId, void* p, DWORD we);

	void SetWearedItemIdx(DWORD WearedPosition,WORD ItemIdx);
	WORD GetWearedItemIdx(DWORD WearedPosition)					{	return m_CharacterInfo.WearedItemIdx[WearedPosition];	}
	WORD GetWeaponEquipType();
	WORD GetTitanWeaponEquipType();
	WORD GetTitanWearedItemIdx(DWORD WearedPosition);


	/*문파*/
	void SetMunpaID(DWORD MunpaID)	{ m_CharacterInfo.MunpaID = MunpaID; 	}
	DWORD GetMunpaIdx()		{ return m_CharacterInfo.MunpaID; 	}
	
	void SetMunpaMemberRank(BYTE rank){ m_CharacterInfo.PositionInMunpa = rank; 	} //문파에서의 자신의 지위 설정
	BYTE GetMunpaMemberRank()		{ return m_CharacterInfo.PositionInMunpa; 	} //문파에서의 자신의 지위
	/*길드*/
	void SetGuildIdxRank(DWORD GuildIdx, BYTE Rank); 
	DWORD GetGuildIdx()		{ return m_CharacterInfo.MunpaID; 	}
	void SetGuildIdx(DWORD GuildIdx);
	void SetGuildMemberRank(BYTE rank){ m_CharacterInfo.PositionInMunpa = rank; 	} //문파에서의 자신의 지위 설정
	BYTE GetGuildMemberRank()		{ return m_CharacterInfo.PositionInMunpa; 	} //문파에서의 자신의 지위
	void SetGuildMarkName(MARKNAMETYPE MarkName);
	void SetNickName(char* pNickName);
	void SetGuildName(char* GuildName);
	char* GetGuildName();
	
	MARKNAMETYPE GetGuildMarkName();
	virtual void ClearGuildInfo();
	
	char* GetNickName();


	// magi82(47)
	void SetStreetStallBalloonImage( DWORD eKind, BOOL bFlag );	// ekind : 구매노점상인지 판매노점상인지  bFlag : TRUE면 세팅 FLASE면 초기화

	
	void SetPeaceMode(bool bPeace)		{	m_CharacterInfo.bPeace = bPeace;	}
	
	void SetBaseMotion();
	int GetStandardMotion()		{return m_StandardMotion;	}
	
	virtual void SetMotionInState(BYTE State);

//KES
	virtual BOOL StartSocietyAct( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE );

	//////////////////////////////////////////////////////////////////////////
	// ObjectStateManager에서 State의 시작과 끝에서 호출해주는 함수들
	virtual void OnStartObjectState(BYTE State);
	virtual void OnEndObjectState(BYTE State);
	//////////////////////////////////////////////////////////////////////////
	
	// LBS 노점상 관련 03.09.30//////////////////////
	void ShowStreetStallTitle( BOOL bShow, char* strTitle, DWORD dwColor = RGB_HALF( 0, 0, 200 ) );
	void SetStreetStallTitle( char* strTitle );
	void ShowStreetBuyStallTitle( BOOL bShow, char* strTitle, DWORD dwColor = RGB_HALF( 0, 0, 200 ) );
	void SetStreetBuyStallTitle( char* strTitle );
	
	/////////////////////////////////////////////////

	// 행동 함수들...
	virtual void Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive);
	virtual void Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage);
	virtual void Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal);
	virtual void Recharge(CObject* pHealer,BYTE RechargeKind,DWORD RechargeVal);
	
	virtual DWORD GetLife(){	return m_CharacterInfo.Life; }
	virtual void SetMaxLife(DWORD maxlife){ m_CharacterInfo.MaxLife = maxlife; }
	virtual void SetLife(DWORD life, BYTE type = 1);
	virtual DWORD DoGetMaxLife();

	virtual DWORD GetShield(){	return m_CharacterInfo.Shield; }
	virtual void SetMaxShield(DWORD maxShield){ m_CharacterInfo.MaxShield = maxShield; }
	virtual void SetShield(DWORD Shield, BYTE type = 1);
	virtual DWORD DoGetMaxShield();

	virtual float DoGetMoveSpeed();

	BYTE GetLifePercent();
	BYTE GetShieldPercent();
	
	virtual void SetLevel(LEVELTYPE level) { m_CharacterInfo.Level = level; }
	LEVELTYPE GetLevel() { return m_CharacterInfo.Level; }
	
	BYTE GetGender() { return m_CharacterInfo.Gender; }
	//////////////////////////////////////////////////////////////////////////
	// EffectManager에서 호출해주는 함수들
	
	// 이펙트가 시간이 다되서 끝났을때 호출
	BOOL OnEndEffectProc(HEFFPROC hEff);

	//////////////////////////////////////////////////////////////////////////

	//PKMODE
	void SetPKMode( BOOL bPKMode );
	BOOL IsPKMode()		{ return m_CharacterInfo.bPKMode; }

	//악명
	virtual void SetBadFame(FAMETYPE val);
	FAMETYPE GetBadFame()			{ return m_CharacterInfo.BadFame;	}	
	
	//현상금
	WANTEDTYPE GetWantedIdx()	{ return m_WantedIDX; }
	void SetWantedIdx(WANTEDTYPE val)	{ m_WantedIDX = val; }
	
	// quest
//	CQuestGroup*	m_pQuestGroup;
//	CQuestGroup*	GetQuestGroup()		{ return m_pQuestGroup; }

	// ShopItemOption
	void SetAvatarInfo(WORD* Avatar)			{	memcpy( m_ShopItemOption.Avatar, Avatar, sizeof(WORD)*eAvatar_Max);	}
	void SetShopItemOptionInfo(SHOPITEMOPTION* pInfo);
	SHOPITEMOPTION*		GetShopItemStats()		{	return &m_ShopItemOption;	}
	void SetItemUseMotionIdx( int Idx )			{	m_ItemUseMotion = Idx;	}
	void SetItemUseEffectIdx( int Idx )			{	m_ItemUseEffect = Idx;	}

	//
	AVATARITEMOPTION*	GetAvatarOption()		{	return &m_AvatarOption;		}

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	SKILLSTATSOPTION*	GetSkillStatsOption()		{	return &m_SkillStatsOption;	}
	//////////////////////////////////////////////////////////////////////////
	
	// Pet
	void SetPet(CPet* pet);
	CPet* GetPet()					{	return m_pPet;	}

	// stage
	virtual void	SetStage( BYTE Stage );
	BYTE			GetStage()		{ return m_CharacterInfo.Stage; }

	// guildunion
	void	SetGuildUnionIdx( DWORD dwUnionIdx )	{ m_CharacterInfo.dwGuildUnionIdx = dwUnionIdx; }
	void	SetGuildUnionName( char* pName )		{ strncpy( m_CharacterInfo.sGuildUnionName, pName, MAX_GUILD_NAME+1 ); }
	void	SetGuildUnionMarkIdx( DWORD dwMarkIdx )	{ m_CharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx; }
	DWORD	GetGuildUnionIdx()		{ return m_CharacterInfo.dwGuildUnionIdx; }
	char*	GetGuildUnionName()		{ return m_CharacterInfo.sGuildUnionName; }
	DWORD	GetGuildUnionMarkIdx()	{ return m_CharacterInfo.dwGuildUnionMarkIdx; }
	
	//
	void SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo );

	//
	void SetSiegeName( DWORD NameType );
	void SetRestraintMode( bool val )			{	m_CharacterInfo.bRestraint = val;	}
	BOOL IsRestraintMode()						{	return m_CharacterInfo.bRestraint;	}

	//
	void CalcAvatarOption();

	// for attribute - japan
#ifdef _JAPAN_LOCAL_
	void	SetMainCharAttr( int nMainAttr )	{ m_CharacterInfo.nMainAttr = nMainAttr; }
	void	SetSubCharAttr( int nSubAttr )		{ m_CharacterInfo.nSubAttr = nSubAttr; }
	int 	GetMainCharAttr()					{ return m_CharacterInfo.nMainAttr; }		// 주속성	
	int		GetSubCharAttr()					{ return m_CharacterInfo.nSubAttr; }		// 부속성
	int		WhatIsAttrRelation( int nOtherMainAttr );		// 속성관계는?
#endif

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	void AddSkillStatsOption(SKILLOPTION *pSkillOption);
	void RemoveSkillStatsOption(SKILLOPTION *pSkillOption);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	BOOL IsHide() { return m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_Hide]; } 
	BOOL IsDetect() 
	{ 
		return (m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_Detect] ||
			m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_DetectItem]); 
	} 
	//////////////////////////////////////////////////////////////////////////

	// 06.09.25 RaMa
	void SetFullMoonEventIndex( BYTE Index )		{	m_CharacterInfo.EventIndex = Index; }
	BYTE GetFullMoonEventIndex()					{	return m_CharacterInfo.EventIndex;	}

	//SW070127 타이탄
	void SetTitanAppearanceInfo(TITAN_APPEARANCEINFO* pInfo);
	void SetTitanMoveSpeed();
	void RidingTitan(BOOL bVal)		{	m_bInTitan = bVal;	}
	BOOL InTitan()					{	return m_bInTitan;	}
	
	void SetTitanPreView( TITAN_APPEARANCEINFO* pInfo );
	void ReleaseTitanPreView();
	BOOL IsTitanPreView()			{	return m_bTitanPreView;	}
	TITAN_APPEARANCEINFO*	GetTitanPreViewInfo()		{	return &m_TitanPreViewAppInfo;	}
	

	void	SetCurTitan(CTitan* pTitan);
	CTitan* GetCurTitan()			{	return m_pTitan;	}

	TITAN_APPEARANCEINFO* GetTitanAppearInfo()		{	return	&m_TitanAppearanceInfo;	}
	float GetTitanMoveSpeed()		{	return m_TitanMoveSpeed;	}

	bool GetAvatarView()							{	return m_CharacterInfo.bNoAvatarView;	}
	void SetAvatarView( bool bNoAvatarView )		{	m_CharacterInfo.bNoAvatarView = bNoAvatarView;	}

	//2007. 8. 8. CBH - 세트아이탬 이펙트 관련 추가
	DWORD GetSetItemEffectID();
	void SetSetItemEffectID(DWORD dwEffectID);

	// magi82(44) - 노점상 꾸미기 업데이트
	CMapObject* GetDecorationAddress()	{	return m_DecorationAddress;	}
	void SetDecorationAddress(CMapObject* pMapObject)	{	m_DecorationAddress = pMapObject;	}
	DWORD GetDecorationStartTime()	{	return m_dwDecorationStartTime;	}
	BOOL GetDecorationFlag()	{	return m_bDecorationFlag;	}
	void SetDecorationInfo(DWORD dwTime, BOOL bFlag)	{	m_dwDecorationStartTime = dwTime;	m_bDecorationFlag = bFlag;	}

	//2008. 1. 21. CBH - 스킨 딜레이 관련 함수
	void InitSkinDelayTime();
	void StartSkinDelayTime();
	BOOL CheckSkinDelay();
	/////////////////////////////////////////////
};

#endif // !defined(AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_)
