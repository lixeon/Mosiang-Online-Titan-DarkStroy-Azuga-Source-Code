// Player.h: interface for the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_)
#define AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"

#include "ExchangeManager.h"
#include "OptionManager.h"
#include "ItemContainer.h"
#include "InventorySlot.h"
#include "PyogukSlot.h"
#include "WearSlot.h"
#include "ShopItemSlot.h"
#include "ShopInvenSlot.h"
#include "PetInvenSlot.h"
#include "PetWearSlot.h"
#include "TitanWearSlot.h"
#include "TitanShopItemSlot.h"
#include "Purse.h"
#include "AbilityGroup.h"
#include "../[CC]Skill/DelayGroup.h"
#include "ShopItemManager.h"
#include "PetManager.h"
#include "TitanManager.h"
#include "..\[CC]Header\GameResourceStruct.h"

// jsd quest
#include "..\[CC]Quest\QuestDefines.h"
#include "QuestGroup.h"


//
class cStreetStall;
class CMonster;
class CItemContainer;
class CItemSlot;
class CQuestBase;
class CQuestScript;
class CQuestGroup;
class CPet;
class CTitan;

enum
{
	PLAYERINITSTATE_ONLY_ADDED = 0,
	PLAYERINITSTATE_HERO_INFO = 1,
	PLAYERINITSTATE_MUGONG_INFO = 2,
	PLAYERINITSTATE_ITEM_INFO = 4,
	PLAYERINITSTATE_ITEM_OPTION_INFO = 8,
	PLAYERINITSTATE_ABILITY_INFO = 16,
	PLAYERINITSTATE_PYOGUK_INFO = 32,
	PLAYERINITSTATE_SHOPITEM_USEDINFO = 64,	
	PLAYERINITSTATE_ACKMSG_SEND = 128,
	PLAYERINITSTATE_INITED = 256,
	PLAYERINITSTATE_GMCONTROL_INFO = 512,
	PLAYERINITSTATE_ITEM_RARE_OPTION_INFO = 1024,
	PLAYERINITSTATE_PET_INFO = 2048,
	PLAYERINITSTATE_TITAN_INFO = 4096,
};

enum
{
	MONEY_ADDITION = 0,
	MONEY_SUBTRACTION,
};

//SW051112 무쌍모드
#define MUSSANG_VALID_TIME		1000*60*10
enum eMussangStates{ eMSS_None=1, eMSS_Ready=2, eMSS_InMode=4, eMSS_LevelUp=8};

#define GETITEM_FLAG_INVENTORY	0x00000001
#define GETITEM_FLAG_WEAR		0x00000002
#define GETITEM_FLAG_PETWEAR	0x00000004
#define GETITEM_FLAG_TITANWEAR	0x00000008		// magi82 - Titan(070209)
#define GETITEM_FLAG_TITANSHOP	0x00000010		// magi82 - Titan(070227)
#define WANTED_DESTROYED_TIME 48*3600*1000
class CPlayer : public CObject  
{
private:
	//2007. 12. 10. CBH - 스킨 딜레이 시간 10초 CPlayer에서만 쓰기떄문에 클래스 안에 enum으로 잡았다.
    enum SKIN_DELAYTIME
	{
		eSkinDelayTime = 10000,
	};
/*
#ifdef _JAPAN_LOCAL_
	BOOL m_bPartyHelp;
#endif
*/	
	CDelayGroup m_DelayGroup;
	DWORD m_ChannelID;

	BOOL m_bPetInvenInit;

//창고를 아이템을 한번만 받구 싶다.
	BOOL m_bGotWarehouseItems;
//

	BOOL m_bPyogukInit;
	BOOL m_bShopInit;
	DWORD m_UniqueIDinAgent;
	int m_initState;
	DWORD m_dwHpMpBitFlag;	// magi82 경험치마다 상태 회복하는 플래그 20% 40% 60% 80%
	CHARACTER_TOTALINFO m_HeroCharacterInfo;
	HERO_TOTALINFO		m_HeroInfo;
	MUGONG_TOTALINFO	m_HeroMugongInfo;
	
	CAbilityGroup		m_AbilityGroup;

	CItemContainer		m_ItemContainer;
	CInventorySlot		m_InventorySlot;
	CWearSlot			m_WearSlot;
	CPyogukSlot			m_PyogukSlot;
	CShopItemSlot		m_ShopItemSlot;			// ItemMall용	
	CShopInvenSlot		m_ShopInvenSlot;
	CPetInvenSlot		m_PetInvenSlot;			// Pet Inventory
	CPetWearSlot		m_PetWearSlot;			// Pet Wear
	CTitanWearSlot		m_TitanWearSlot;		// magi82 - Titan(070207)
	CTitanShopItemSlot	m_TitanShopItemSlot;	// magi82 - Titan(070227)
	CInventoryPurse		m_InventoryPurse;
	CPyogukPurse		m_PyogukPurse;

	// RaMa - QuestItem관련
//	sQUESTITEM			m_sQuestItem[MAX_QUEST_ITEM];
//	DWORD				m_dwQuestItemCount;

//	QUICKITEM m_QuickItem[MAX_QUICKSHEET_NUM][MAX_QUICKITEMPERSHEET_NUM];
//	quick_cur_info m_quick_info;

	char m_StreetStallTitle[MAX_STREETSTALL_TITLELEN+1];

	player_calc_stats m_charStats;
	player_calc_stats m_itemStats;
	SET_ITEM_OPTION m_setItemStats;		// 2007. 6. 12. CBH - 세트아이탬 능력치 구조체 추가
	UNIQUE_ITEM_OPTION_INFO m_UniqueItemStats;	// magi82 - UniqueItem(070626)
	ABILITY_STATS m_AbilityStats;
		 
	// LIFE RECOVER는 COBJECT로
	RECOVER_TIME m_NaeRyukRecoverTime;
	YYRECOVER_TIME m_YYNaeRyukRecoverTime;

	WORD m_ChangeBeforeMapNum;
	WORD m_ChangeWantMapNum;

	DWORD m_CurComboNum;

	WORD m_ChangeNpcIndex;

	BYTE m_curGravity;

// KES HACK CHECK
	int	m_nHackCount;			//skill + move
	DWORD m_dwHackStartTime;
	
	DWORD	m_dwSkillDelayStartTime;		// speedhack check;
	DWORD	m_dwLastSkillAnimationTime;

// KES 050221
	DWORD m_dwLastSocietyAction;

// KES EXCHNAGE 040112
//	CExchangeRoom*		m_pExchangeRoom;
	sEXCHANGECONTAINER	m_ExchangeContainer;
// KES VIMU 031101
	//베틀중인가?
	BOOL				m_bVimuing;
	DWORD				m_dwOpPlayerID;

// KES OPTION 031213
	sGAMEOPTION			m_GameOption;
// KES PK
	DWORD				m_dwPKModeStartTime;	//with gCurTime
	DWORD				m_dwPKContinueTime;
// KES BADFAME
	DWORD				m_dwLastReduceHour;		//with PlayTime
	void				AddBadFameReduceTime();
// KES EXIT
	BOOL				m_bExit;	//종료플래그
	DWORD				m_dwExitStartTime;
	BOOL				m_bNormalExit;
	BOOL				m_bWaitExitPlayer;

//
	DWORD				m_dwUserLevel;
///////////	

// LBS 노점상 관련 03.09.29/////
	cStreetStall* m_pGuetStall;
////////////////////////////////

///////////////////////////////////////////
// LBS 시간별 Log 관련 04.02.06
//	DWORD				m_dwStartTime;
	DWORD				m_dwProgressTime;
//	MHTIMEMGR_OBJ->GetMHTime()
///////////////////////////////////////////
	
	WANTEDTYPE			m_WantedIDX;
	cPtrList			m_WantedList;

	WORD				m_MurdererKind;
	DWORD				m_MurdererIDX;
	BOOL				m_bReadyToRevive; //죽은 상태에서만 씀

	//SW060831 추가. 캐릭터를 죽인 대상의 ObjectKind
	WORD				m_MurderObjKind;
	BOOL				m_bPenaltyByDie;
	//
	DWORD				m_LoginTime;
	DWORD				m_ContinuePlayTime;	
	
	// RaMa - 04.11.08 ( ShopItem으로 추가되는 옵션 )
	SHOPITEMOPTION		m_ShopItemOption;
	AVATARITEMOPTION	m_AvatarOption;
	CShopItemManager	m_ShopItemManager;

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	SKILLSTATSOPTION	m_SkillStatsOption;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 06. 07. 스킬개선 - 이영준
	// 플레이어가 자기에게 영향을 미치는 스킬의 리스트를 갖게한다.
	CYHHashTable<CSkillObject> m_SkillList;
	//////////////////////////////////////////////////////////////////////////

	//SW051129 Pet
	CPetManager			m_PetManager;
	//CPet*				m_pCurPet;		//소환펫

	//SW070127 타이탄
	CTitanManager		m_TitanManager;

	DWORD				m_InitPosition;
	REVIVEDATA			m_ReviveData;
	int					m_ReviveTime;
	BOOL				m_bSavePointInited;
	
	//SW051112 무쌍모드
	DWORD				m_dwMussangPoint;
	DWORD				m_dwMussangStartTime;
	BOOL				m_bIsMussangReady;
	BYTE				m_bMussangFlag;

	////////////////////////////////////////////////////////////////////////////
	// 06. 07. 상태강제변경 - 이영준
	// 일정이상 스킬 사용실패시 특별한 상태를 제외한 나머지 상태는 모두 초기화
	WORD	m_SkillFailCount;
	////////////////////////////////////////////////////////////////////////////

	// 06.09.25 RaMa 추석이벤트
	int					m_FullMoonEventHCt;
	float				m_fFullMoonEventTime;

	DWORD				m_LastActionTime;
	BOOL				m_bActionPanelty;


	//SW061211 크리스마스이벤트
	DWORD				m_CristmasEventSnowIdx;

	///////////////////////////////////////////////////////////////////////////////////
	DWORD m_dwSkinDelayTime;	//2007. 12. 10. CBH - 스킨교체 딜레이 시간
	BOOL m_bSkinDelayResult;	//2007. 12. 10. CBH - 스킨교체 딜레이 구분	
	
	void SkinDelayTimeProcess();	//2007. 12. 10. CBH - 스킨 딜레이 시간 처리
	///////////////////////////////////////////////////////////////////////////////////
    
	void DoSetInited();
	virtual void SetInitedGrid();

	friend class CCharacterCalcManager;
	friend class CInventoryPurse;
	friend class CAbilityGroup;
	
	void SetMurderIDX(DWORD CharacterIDX)			{ m_MurdererIDX = CharacterIDX; }
	void SetMurderKind(WORD Kind)					{ m_MurdererKind = Kind; }

	// magi82 - SOS(070720)
	int					m_SOSState;		// 0이면 SOS 상태 아님, 0이 아니면 SOS 상태


	// 06. 03 국내무쌍 - 이영준
#ifndef _JAPAN_LOCAL_
	DWORD	m_dwMussangTime;
	WORD	m_wMussangStat;
#endif

#ifdef _JAPAN_LOCAL_
	DWORD	m_dwCharacterSlotNum;
#endif
#ifdef _HK_LOCAL_
	DWORD	m_dwCharacterSlotNum;
#endif
#ifdef _TL_LOCAL_
	DWORD	m_dwCharacterSlotNum;
#endif

public:
/*
#ifdef _JAPAN_LOCAL_
	void SetPartyHelp( BOOL bPartyHelp ) { m_bPartyHelp = bPartyHelp; }
	BOOL IsPartyHelp() { return m_bPartyHelp; }
#endif
*/
	void	SetMurderObjKind(WORD ObjKind)		{	m_MurderObjKind = ObjKind;	}
	WORD	GetMurderObjKind()					{	return m_MurderObjKind;	}

	void	SetPenaltyByDie(BOOL bVal)		{	m_bPenaltyByDie = bVal;	}	//방파 비무 시 상대에 의해 죽었을 시 TRUE //강제종료 시 패널티 체크를 위한 변수
	BOOL	IsPenaltyByDie()				{	return m_bPenaltyByDie;	}

	BOOL IsGotWarehouseItems() { return m_bGotWarehouseItems; }
	void SetGotWarehouseItems( BOOL bGot ) { m_bGotWarehouseItems = bGot; }

	void SetWaitExitPlayer( BOOL bWait ) { m_bWaitExitPlayer = bWait; }
	BOOL IsWaitExitPlayer() { return m_bWaitExitPlayer; }

	void SetChannelID(DWORD id)		{	m_ChannelID = id;	}
	DWORD GetChannelID()			{	return m_ChannelID;	}
// KES
	BOOL				m_bNeedRevive;
//
	DWORD GetSkillDelayStartTime()				{ return m_dwSkillDelayStartTime; }
	void SetSkillDelayStartTime( DWORD dwTime )	{m_dwSkillDelayStartTime = dwTime; }

	DWORD GetLastSkillAnimationTime()			{ return m_dwLastSkillAnimationTime; }
	void SetLastSkillAnimationTime( DWORD dwTime ) { m_dwLastSkillAnimationTime = dwTime; }
	
	void SpeedHackCheck();
	BOOL CanSocietyAction( DWORD curTime );
	
	int GetGender() { return m_HeroCharacterInfo.Gender; }
//
	HERO_TOTALINFO* GetHeroTotalInfo();

	ABILITY_STATS*	GetAbilityStats()	{	return &m_AbilityStats;	}
	CAbilityGroup*	GetAbilityGroup()	{	return &m_AbilityGroup;	}
	CDelayGroup*	GetDelayGroup()		{	return &m_DelayGroup;	}

	CItemSlot * GetSlot(POSTYPE absPos);
	CItemSlot * GetSlot(eITEMTABLE tableIdx);

	//CItemContainer * GetContainer() { return m_pItemContainer; } 
	void SetUniqueIDinAgent(DWORD uniqueid)	{	m_UniqueIDinAgent = uniqueid;	}
	DWORD GetUniqueIDinAgent()	{	return m_UniqueIDinAgent;	}
	
	void InitBaseObjectInfo(BASEOBJECT_INFO* pBaseInfo);

	BOOL IsVisible()
	{
		return (m_HeroCharacterInfo.bVisible ? TRUE : FALSE);
	}
	void SetVisible(BOOL val)
	{
		m_HeroCharacterInfo.bVisible = val ? true : false;
	}
	
	// Gravity --------------------------------------------------------------
	virtual DWORD GetGravity();
	virtual void SetGravity(DWORD gravity)
	{
	}
	// Gravity --------------------------------------------------------------

	CPlayer();
	virtual ~CPlayer();
	void InitClearData();

	virtual BOOL Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo);
	virtual void Release();

	// 초기화 
	void InitCharacterTotalInfo(CHARACTER_TOTALINFO* pTotalInfo);
	void InitHeroTotalInfo(HERO_TOTALINFO* pHeroInfo);
	void InitMugongTotalInfo(MUGONG_TOTALINFO* pMugongInfo);
	void InitItemTotalInfo(ITEM_TOTALINFO* pItemInfo);
	void InitPyogukItemInfo(PYOGUK_ITEM* pItemInfo);

	// 펫아이템 관련
	void InitPetInvenItemInfo(PETINVEN_ITEM* pItemInfo);
	void SendPetInvenItemInfo();
	void InitPetWearItemInfo(ITEMBASE* pWearItemInfo);

	int GetInitState(){ return m_initState; }
	void SetInitState(int initstate,DWORD protocol);

	// 파티 관련 함수	
	void SendPlayerLifeToParty(DWORD life, DWORD PartyIdx);
	void SendPlayerShieldToParty(DWORD Shield, DWORD PartyIdx);
	void SendPlayerNaeRyukToParty(DWORD naeryuk, DWORD PartyIdx);
	void SendMoveInfotoParty();

	// 캐릭터 수치 설정 //////////////////////////////////////////////////////////
	LEVELTYPE GetPlayerLevelUpPoint() { return m_HeroInfo.LevelUpPoint;	}
	void SetPlayerLevelUpPoint(LEVELTYPE val);
	EXPTYPE GetPlayerExpPoint() { return m_HeroInfo.ExpPoint; }
	DOUBLE GetPlayerTotalExpPoint();
	void SetPlayerExpPoint(EXPTYPE point,BYTE bDie=0);
	void SetLevelUpStatPoint();			// magi82
	void SetNormalStatPoint();			// magi82
	BYTE GetExpFlag()	{	return m_HeroInfo.ExpFlag;	}	// magi82(41)
	void AddPlayerExpPoint(EXPTYPE Exp);
	void ReduceExpPoint(EXPTYPE minusExp, BYTE bLogType);
	DWORD GetPlayerAbilityExpPoint() { return m_AbilityGroup.GetAbilityExp(); }
	void SetPlayerAbilityExpPoint(DWORD point,BOOL bUse=0);
	void AddAbilityExp(DWORD AbilityExp);

	DWORD AddExpPointToMugong(WORD MugongIdx,DWORD exp,BOOL bSuryunComplete = FALSE);
	void SetPlayerMugongSung(POSTYPE abspos, BYTE sung, WORD Reason);
	void MugongLevelDown(WORD MugongIdx, WORD Reason);
	
	FAMETYPE GetFame()			{ return m_HeroInfo.Fame;	}
	void SetFame(FAMETYPE val)	{ m_HeroInfo.Fame = val; 	}

	FAMETYPE GetBadFame()			{ return m_HeroCharacterInfo.BadFame;	}
	void SetBadFame(FAMETYPE val)	{ m_HeroCharacterInfo.BadFame = val; 	}

	// RaMa - 04.11.10    -> ShopItemOption 추가  AvatarOption추가(05.02.16)
	// magi82 - UniqueItem(070629) - 스텟이 마이너스가 될 경우 예외처리 추가
	WORD GetCheRyuk(){ return (int)(m_HeroInfo.wCheRyuk + GetAbilityStats()->StatChe + GetItemStats()->CheRyuk + GetShopItemStats()->Cheryuk
		+ GetAvatarOption()->Cheryuk + GetSetItemStats()->wCheRyuk + GetUniqueItemStats()->nCheRyuk) >= 0 ? (WORD)(m_HeroInfo.wCheRyuk
		+ GetAbilityStats()->StatChe + GetItemStats()->CheRyuk + GetShopItemStats()->Cheryuk + GetAvatarOption()->Cheryuk + GetSetItemStats()->wCheRyuk
		+ GetUniqueItemStats()->nCheRyuk) : 0; }

	WORD GetSimMek(){ return (int)(m_HeroInfo.wSimMek + GetAbilityStats()->StatSim + GetItemStats()->SimMaek + GetShopItemStats()->Simmek
		+ GetAvatarOption()->Simmek + GetSetItemStats()->wSimMek + GetUniqueItemStats()->nSimMek) >= 0 ? (WORD)(m_HeroInfo.wSimMek
		+ GetAbilityStats()->StatSim + GetItemStats()->SimMaek + GetShopItemStats()->Simmek + GetAvatarOption()->Simmek + GetSetItemStats()->wSimMek
		+ GetUniqueItemStats()->nSimMek) : 0; }

	WORD GetGenGol(){ return (int)(m_HeroInfo.wGenGol + GetAbilityStats()->StatGen + GetItemStats()->GenGol + GetShopItemStats()->Gengol
		+ GetAvatarOption()->Gengol + GetSetItemStats()->wGenGol + GetUniqueItemStats()->nGengol) >= 0 ? (WORD)(m_HeroInfo.wGenGol
		+ GetAbilityStats()->StatGen + GetItemStats()->GenGol + GetShopItemStats()->Gengol + GetAvatarOption()->Gengol + GetSetItemStats()->wGenGol
		+ GetUniqueItemStats()->nGengol) : 0; }

	WORD GetMinChub(){ return (int)(m_HeroInfo.wMinChub + GetAbilityStats()->StatMin + GetItemStats()->MinChub + GetShopItemStats()->Minchub
		+ GetAvatarOption()->Minchub + GetSetItemStats()->wMinChub + GetUniqueItemStats()->nMinChub) >= 0 ? (WORD)(m_HeroInfo.wMinChub
		+ GetAbilityStats()->StatMin + GetItemStats()->MinChub + GetShopItemStats()->Minchub + GetAvatarOption()->Minchub + GetSetItemStats()->wMinChub
		+ GetUniqueItemStats()->nMinChub) : 0; }

	void SetCheRyuk(WORD val);
	void SetSimMek(WORD val);
	void SetGenGol(WORD val);
	void SetMinChub(WORD val);

	void AddGenGol(int val);
	void AddSimMek(int val);
	void AddMinChub(int val);
	void AddCheRyuk(int val);
	
	void SetStage( BYTE stage );
	BYTE GetStage()		{ return m_HeroCharacterInfo.Stage; }
	
	WORD GetPlayerLoginPoint(){return m_HeroCharacterInfo.LoginPoint_Index;}
	void SetLoginPoint(WORD LoginPoint){m_HeroCharacterInfo.LoginPoint_Index = LoginPoint;}
	void InitMapChangePoint(){m_HeroCharacterInfo.MapChangePoint_Index = 0;}
	void SetMapChangePoint(WORD point){m_HeroCharacterInfo.MapChangePoint_Index = point;}	// YH
	WORD GetMapChangePoint() { return m_HeroCharacterInfo.MapChangePoint_Index;	} //hs for party
	
	void CalcState();
	
	///////////////////////////////////////////////////////////////////////
	// 무공	
	void SetMugongBase(POSTYPE abspos, MUGONGBASE * mugong)
	{
#ifdef _JAPAN_LOCAL_
		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
 			ASSERT(0);
		POSTYPE pos = abspos - TP_MUGONG_START;
#elif defined _HK_LOCAL_
		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
 			ASSERT(0);

		POSTYPE pos = abspos - TP_MUGONG_START;
#elif defined _TL_LOCAL_
		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
 			ASSERT(0);

		POSTYPE pos = abspos - TP_MUGONG_START;
#else
		if( abspos < TP_MUGONG1_START && abspos >= TP_MUGONG2_END )
		{
			if( abspos < TP_JINBUB_START && abspos >= TP_JINBUB_END )
				ASSERT(0);
		}

		DWORD EndPos = TP_MUGONG1_END;
		if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MugongExtend ) )
			EndPos = TP_MUGONG2_END;

		POSTYPE pos;
		if( abspos >= TP_MUGONG1_START && abspos < TP_TITANMUGONG_END )
			pos = abspos - TP_MUGONG1_START;
		/*
		else if( abspos >= TP_JINBUB_START && abspos < TP_JINBUB_END )
			pos = abspos - TP_JINBUB_START + (SLOT_MUGONG_NUM*2);
			*/
		else
		{
			ASSERT(0);
			return;
		}
#endif
		if( mugong->dwDBIdx != 0 && mugong->Position == 0)
			CRITICALASSERT(0);

		// magi82 - Titan(070611) 타이탄 무공변환 주석처리
		// 스킬인덱스가 10000보다 크면 타이탄 스킬
		//if(mugong->wIconIdx < 10000)
            memcpy(&m_HeroMugongInfo.mugong[pos], mugong, sizeof(MUGONGBASE));
		//else
		//	memcpy(&m_HeroMugongInfo.Titanmugong[pos], mugong, sizeof(MUGONGBASE));
	}
	MUGONGBASE * GetMugongBase(POSTYPE abspos)
	{
#ifdef _JAPAN_LOCAL_
		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
   		{	
			ASSERT(0);
 			return NULL;
   		}

		POSTYPE pos = abspos - TP_MUGONG_START;
#elif defined _HK_LOCAL_
		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
   		{	
			ASSERT(0);
 			return NULL;
   		}
		POSTYPE pos = abspos - TP_MUGONG_START;
#elif defined _TL_LOCAL_
		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
   		{	
			ASSERT(0);
 			return NULL;
   		}
		POSTYPE pos = abspos - TP_MUGONG_START;
#else
		if( abspos < TP_MUGONG1_START && abspos >= TP_MUGONG2_END )
		{	
			if( abspos < TP_JINBUB_START && abspos >= TP_JINBUB_END )
			{
				ASSERT(0);
				return NULL;
			}
		}

		DWORD EndPos = TP_MUGONG1_END;
		if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MugongExtend ) )
			EndPos = TP_MUGONG2_END;


		POSTYPE pos;
		if( abspos >= TP_MUGONG1_START && abspos < TP_TITANMUGONG_END )
			pos = abspos - TP_MUGONG1_START;
		/*
		else if( abspos >= TP_JINBUB_START && abspos < TP_JINBUB_END )
			pos = abspos - TP_MUGONG1_START;	// TP_JINBUB_START + (SLOT_MUGONG_NUM*2);
		else if( abspos >= TP_TITANMUGONG_START && abspos < TP_TITANMUGONG_END )	
			pos = abspos - TP_TITANMUGONG_START;
		*/
		else
			return NULL;
#endif
		return &m_HeroMugongInfo.mugong[pos];
	}	
	MUGONGBASE * GetMugongBaseByMugongIdx(WORD MugongIdx);
	int GetMugongLevel(WORD MugongIdx);
	int GetMugongNum();
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	WORD GetSkillOptionIndex(WORD MugongIdx);
	//////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////

	
	void GetCharacterTotalInfo(CHARACTER_TOTALINFO* pRtInfo);
	void GetHeroTotalInfo(HERO_TOTALINFO* pRtInfo);
	void GetMugongTotalInfo(MUGONG_TOTALINFO* pRtInfo);

	// 아이템 //////////////////////////////////////////////////////////////////////
	void GetItemtotalInfo(ITEM_TOTALINFO* pRtInfo,DWORD dwFlag);
	void GetPyogukItemInfo(PYOGUK_ITEM * pRtInfo);

	WORD GetWearedItemIdx(DWORD WearedPosition)	{	return m_HeroCharacterInfo.WearedItemIdx[WearedPosition];	}
	void SetWearedItemIdx(DWORD WearedPosition,WORD ItemIdx);
	BOOL UseItem(WORD abs_pos);
	BOOL UseConsumptionItem(WORD abs_pos);
	BOOL UseEquipItem(WORD abs_pos);
	BOOL UseMugongItem(WORD abs_pos);

	void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);
	
	virtual void SendMsg(MSGBASE* pMsg,int MsgLen);
	
private:
	void MoneyUpdate( MONEYTYPE money ); //(Money Class에서 호출 ), DB에 Set
	CYHHashTable<ITEM_RARE_OPTION_INFO> m_ItemRareOptionList;
	CYHHashTable<ITEM_OPTION_INFO> m_ItemOptionList;
public:
	//SW050920 Rare
	void AddItemRareOption(ITEM_RARE_OPTION_INFO* pInfo);
	BOOL RemoveItemRareOption(DWORD dwRareIdx);
	ITEM_RARE_OPTION_INFO* GetItemRareOption(DWORD dwRareIdx);

	void AddItemOption(ITEM_OPTION_INFO * pInfo);
	BOOL RemoveItemOption(DWORD dwOptionIdx);
	ITEM_OPTION_INFO * GetItemOption(DWORD dwOptionIdx);

	MONEYTYPE SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag = 0, eITEMTABLE tableIdx = eItemTable_Inventory, BYTE LogType = 0, DWORD TargetIdx = 0 );
	MONEYTYPE GetMoney(eITEMTABLE tableIdx = eItemTable_Inventory);
	MONEYTYPE GetMaxPurseMoney(eITEMTABLE TableIdx);
	void SetMaxPurseMoney(eITEMTABLE TableIdx, DWORD MaxMoney);
	BOOL IsEnoughAdditionMoney(MONEYTYPE money, eITEMTABLE tableIdx = eItemTable_Inventory );

//	void ObtainMoney(DWORD addmoney);		//몹죽여서 얻었을때만 호출
//	void SetMoneyNoMsgNoUpdate(DWORD money);
	
	player_calc_stats*	GetCharStats()			{	return &m_charStats;		}
	player_calc_stats*	GetItemStats()			{	return &m_itemStats;		}
	SET_ITEM_OPTION* GetSetItemStats()		{	return &m_setItemStats;		}	// 2007. 6. 12. CBH - 세트아이탬 능력치 반환 함수 추가
	UNIQUE_ITEM_OPTION_INFO* GetUniqueItemStats(){	return &m_UniqueItemStats;	}	// magi82 - UniqueItem(070626)
	SHOPITEMOPTION*		GetShopItemStats()		{	return &m_ShopItemOption;	}
	AVATARITEMOPTION*	GetAvatarOption()		{	return &m_AvatarOption;		}

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	SKILLSTATSOPTION*	GetSkillStatsOption()		{	return &m_SkillStatsOption;	}
	//////////////////////////////////////////////////////////////////////////
	
//	void AddHuntItemAndSendToPlayer(WORD DropItemNum);
//	BOOL HuntItemDBResult(DWORD dwPlayerID, ITEMBASE * itemInfo);
	DWORD GetPartyIdx()	{	return m_HeroInfo.PartyID;	}
	void SetPartyIdx(DWORD PartyIDx) { m_HeroInfo.PartyID = PartyIDx; }

	/*문파가 바뀌네...*/
	DWORD GetMunpaIdx()	{	return m_HeroCharacterInfo.MunpaID;	}
	void SetMunpaIdx(DWORD MunpaIDx) { m_HeroCharacterInfo.MunpaID = MunpaIDx; }
	
	BYTE GetMunpaMemberRank() { return m_HeroCharacterInfo.PositionInMunpa; 	}
	void SetMunpaMemberRank(BYTE rank){ m_HeroCharacterInfo.PositionInMunpa = rank; 	} //문파에서의 자신의 지위 설정
	void SetMunpaName(char* MunpaName){ SafeStrCpy(m_HeroCharacterInfo.GuildName, MunpaName, MAX_MUNPA_NAME+1); }
	/**/

	DWORD GetGuildIdx()	{	return m_HeroCharacterInfo.MunpaID;	}
	void SetGuildIdx(DWORD GuildIdx) { m_HeroCharacterInfo.MunpaID = GuildIdx; }

	void SetGuildInfo(DWORD GuildIdx, BYTE Grade, char* GuildName, MARKNAMETYPE MarkName);
	BYTE GetGuildMemberRank() { return m_HeroCharacterInfo.PositionInMunpa; 	}
	void SetGuildMemberRank(BYTE rank){ m_HeroCharacterInfo.PositionInMunpa = rank; 	} //문파에서의 자신의 지위 설정
	void SetGuildName(char* GuildName){ SafeStrCpy(m_HeroCharacterInfo.GuildName, GuildName, MAX_MUNPA_NAME+1); }
	void SetGuildMarkName(MARKNAMETYPE MarkName);
	char* GetGuildCanEntryDate();
	void SetGuildCanEntryDate(char* date);
	void ClrGuildCanEntryDate();
	void SetNickName(char* NickName);

	BYTE GetLifePercent();
	BYTE GetShieldPercent();
	BYTE GetNaeRyukPercent();
	
	// 상태 변환
	void OnStartObjectState(BYTE State,DWORD dwParam);
	void OnEndObjectState(BYTE State);

	WORD GetWearedWeapon() { return m_HeroCharacterInfo.WearedItemIdx[eWearedItem_Weapon]; }
	WORD GetWeaponEquipType();
	WORD GetTitanWeaponEquipType();

	void SetCurComboNum(DWORD combonum)	{	m_CurComboNum = combonum;	}
	DWORD GetCurComboNum()				{	return m_CurComboNum;	}
//	DWORD GetCurMugongNum()				{ return GetCurQuickItem()->dwIconIdx; }

	void SetPeaceMode(BOOL bPeace)	{	m_HeroCharacterInfo.bPeace = bPeace?true:false;	}
	BOOL GetPeaceMode()				{	return m_HeroCharacterInfo.bPeace;		}
	
	//표국
	void AddPyogukItem(ITEMBASE * pPyogukItem);
	void InitPyogukInfo(BYTE pyoguknum,MONEYTYPE money);
	BOOL IsPyogukInit() {return m_bPyogukInit; }
	void SetPyogukNum(BYTE n);
	BYTE GetPyogukNum();

	void RevivePresentSpot();
	void ReviveLogIn();
	void ReviveAfterVimu( BOOL bSendMsg = TRUE );

	//현상금
	void ClearWantedList();
	WANTEDTYPE GetWantedIdx()	{ return m_WantedIDX; }
	void SetWantedIdx(WANTEDTYPE val)	{ m_WantedIDX = val; }
		
	DWORD GetWantedNum()			{ return m_WantedList.GetCount(); }
	
	void ClearMurderIdx();
	DWORD GetMurderIDX()						{ return m_MurdererIDX;	}
	WORD GetMurderKind()						{ return m_MurdererKind;}
	BOOL IsReadyToRevive()						{ return m_bReadyToRevive;	}
	void SetReadyToRevive(BOOL val)				{ m_bReadyToRevive = val;	}
	void AddWanted(WANTEDTYPE WantedIDX);
	void DeleteWanted(WANTEDTYPE WantedIDX);
	BOOL IsWantedOwner(WANTEDTYPE WantedIDX);
	

// KES EXCHNAGE 030922 
	void SetExchangeContainer( sEXCHANGECONTAINER* ExchangeContainer )
											{ m_ExchangeContainer = *ExchangeContainer; }
	sEXCHANGECONTAINER* GetExchangeContainer() { return &m_ExchangeContainer; }
// KES VIMU 031101
	void SetVimuing( BOOL bVimuing ) { m_bVimuing = bVimuing; }
	BOOL IsVimuing() { return m_bVimuing; }
	void SetVimuOpPlayerID( DWORD dwOpPlayerID ) { m_dwOpPlayerID = dwOpPlayerID; }
	DWORD GetVimuOpPlayerID()	{ return m_dwOpPlayerID; }
//	BOOL IsKilledWithPanelty()			{ return m_bNeedRevive; }
// KES OOPTION 031213
	sGAMEOPTION* GetGameOption() { return &m_GameOption; }
	void SetGameOption( sGAMEOPTION* pGameOption )	{ m_GameOption = *pGameOption; }
	void SetAvatarView( bool bNoAvatarView )		{ m_HeroCharacterInfo.bNoAvatarView = bNoAvatarView;	}
	bool GetAvatarView()		{ return m_HeroCharacterInfo.bNoAvatarView;	}
// KES PK 040205
	int PKModeOn();
	BOOL PKModeOff();
	void PKModeOffForce();
	BOOL IsPKMode()		{ return m_HeroCharacterInfo.bPKMode; }
	void SetPKStartTimeReset() { m_dwPKModeStartTime = gCurTime; }
	void AddPKContinueTime( DWORD dwTime ) { m_dwPKContinueTime += dwTime; }

	void SetPKModeEndtime();
// KES EXIT
	BOOL IsExitStart() { return m_bExit; }
	int CanExitStart();
	void SetExitStart( BOOL bExit );
	int CanExit();
	void ExitCancel();
	void SetNormalExit() { m_bNormalExit = TRUE; }
	BOOL IsNormalExit() { return m_bNormalExit; }
//
	void SetUserLevel( DWORD dwUserLevel ) { m_dwUserLevel = dwUserLevel; }
	DWORD GetUserLevel() { return m_dwUserLevel; }
////////


// LBS 노점상 관련 03.09.29 /////////////////////////////////////////////
	void SetGuestStall( cStreetStall* pStall ) { m_pGuetStall = pStall; }
	cStreetStall* GetGuestStall() { return m_pGuetStall; }
	void SetStreetStallTitle(char* title);
	void GetStreetStallTitle(char* title);
/////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// 오버라이딩 함수들
	virtual void DoDie(CObject* pAttacker);
	virtual void DoDamage(CObject* pAttacter,RESULTINFO* pDamageInfo,DWORD beforeLife);
	virtual DWORD Damage(CObject* pAttacker,RESULTINFO* pDamageInfo);
	
	void SetMaxLife(DWORD val);
	void SetMaxShield(DWORD val);
	void SetMaxNaeRyuk(DWORD val);
	
	LEVELTYPE GetMaxLevel()		{	return m_HeroInfo.MaxLevel;	}
	//////////////////////////////////////////////////////////////////////////
	// 정보 얻어오는 함수들
	virtual LEVELTYPE GetLevel();
	virtual void SetLevel(LEVELTYPE level);
	virtual DWORD GetLife();
	virtual void SetLife(DWORD Life,BOOL bSendMsg = TRUE);
	virtual DWORD GetShield();
	virtual void SetShield(DWORD Shield,BOOL bSendMsg = TRUE);
	virtual DWORD GetNaeRyuk();
	virtual void SetNaeRyuk(DWORD valb,BOOL SendMsg = TRUE);	
	virtual DWORD DoGetMaxLife();
	virtual DWORD DoGetMaxShield();
	virtual DWORD DoGetMaxNaeRyuk();
	virtual DWORD DoGetPhyDefense();
	virtual float DoGetAttDefense(WORD Attrib);
	virtual float GetAttribPlusPercent(WORD Attrib);
	virtual DWORD DoGetPhyAttackPowerMin();
	virtual DWORD DoGetPhyAttackPowerMax();

	virtual float DoGetMoveSpeed();

#ifdef _JAPAN_LOCAL_
	virtual float DoGetAddAttackRange(){	return (float)GetMinChub() / 2.f;	}
#else
	virtual float DoGetAddAttackRange(){	return GetMinChub() / 3.f;	}
#endif

	virtual DWORD DoGetCritical();
	virtual DWORD DoGetDecisive();



	WORD GetKyungGongGrade()	{	return m_HeroInfo.KyungGongGrade;	}
	void SetKyungGongGrade(WORD grade)	{	m_HeroInfo.KyungGongGrade = grade;	}

	ITEMOBTAINARRAYINFO * GetArray(WORD id);
	void AddArray(ITEMOBTAINARRAYINFO * pInfo);
	void RemoveArray(DWORD id);
	CYHHashTable<ITEMOBTAINARRAYINFO> m_ItemArrayList;

	BOOL AddFollowList(CMonster * pMob);
	void RemoveFollowList(DWORD ID);
//	void RemoveFollowAsFarAs(DWORD GAmount);
	BOOL RemoveFollowAsFarAs(DWORD GAmount, CObject* pObject );

	CYHHashTable<CMonster> m_FollowMonsterList;

// 초보자 가이드
	void AddQuest(CQuestBase* pQuest);
	void RemoveQuest(DWORD QuestIdx);
	BOOL SetQuestState(DWORD QuestIdx, QSTATETYPE value);
	CYHHashTable<CQuestBase>	m_QuestList;

	virtual void StateProcess();

	// PlayTime
	DWORD GetPlayTime()				{	return m_HeroInfo.Playtime;	}
	void SetLoginTime(int time)		{	m_LoginTime = time;	}

	void ReviveLogInPenelty();
// jsd quest
protected:
	CQuestGroup		m_QuestGroup;
public:
	CQuestGroup*	GetQuestGroup()		{ return &m_QuestGroup; }
	void			QuestProcess();

	void			RSetMoney( MONEYTYPE money, BYTE flag );	

	DWORD			m_dwOldMapNum;
	WORD			m_wKillMonsterLevel;

	void	SetKillMonsterLevel( WORD wMonLevel )	{ m_wKillMonsterLevel = wMonLevel; }
	WORD	GetKillMonsterLevel()					{ return m_wKillMonsterLevel; }

	// ShopItem
	CShopItemManager* GetShopItemManager()			{	return &m_ShopItemManager;	}
	void SetShopItemInit( BOOL bInit )				{	m_bShopInit = bInit;	}
	void InitShopItemInfo(SHOP_ITEM* pItemInfo);
	void SendShopItemInfo();

	void UpdateLogoutToDB();
	
	void SetInitPoint( DWORD Pos )		{	m_InitPosition = Pos;	}
	DWORD GetInitPoint()				{	return m_InitPosition;	}
	
	BOOL IsAbleReviveOther();
	BOOL ReviveShopItem( WORD ItemIdx );
	void SetReviveData( DWORD Id, WORD ItemIdx, POSTYPE ItemPos )
	{	m_ReviveData.TargetID=Id;	m_ReviveData.ItemIdx=ItemIdx;	m_ReviveData.ItemPos=ItemPos;	}
	REVIVEDATA* GetReviveData()			{	return &m_ReviveData;	}
	void SetReviveTime( DWORD time )	{	 m_ReviveTime = time;	}

	BOOL GetSavePointInit()					{	return m_bSavePointInited;	}
	void SetSavePointInit( BOOL val )		{	m_bSavePointInited = val;	}
	
	// ItemLoad 수정
	void InitShopInvenInfo(ITEMBASE* pShopWareHouse);

	//SW051129 Pet
	CPetManager* GetPetManager()			{	return &m_PetManager;	}
	//void SetCurPet(CPet* pPet)			{	m_pCurPet = pPet;	}
	//	CPet*	GetCurPet()					{	return m_pCurPet;	}
	CPet*	GetCurPet()					{	return m_PetManager.GetCurSummonPet();	}
	
	BOOL GetKyungGongIdx()				{	return m_MoveInfo.KyungGongIdx;	}

protected:
	//SW080515 채널별 세팅
	BOOL		m_bDieForBattleChannel;

	BOOL		m_bDieForGFW;		// 문파전상대한테 죽었을때...
	BOOL		m_bDieInSpecialMap;
	WORD		m_wReturnMapNum;
	WORD		m_wGTReturnMapNum;
	int			m_ObserverBattleIdx;

public:
	void		SetDieForBattleChannel( BOOL bVal )	{	m_bDieForBattleChannel = bVal;	}	//배틀채널에서 평상공격으로 죽었을 때 해당. 살기나 기타 battle상태 제외.
	BOOL		IsDieForBattleChannel()		{	return m_bDieForBattleChannel;	}
	void		SetDieForGFW( BOOL bDie )	{ m_bDieForGFW = bDie; }
	BOOL		IsDieFromGFW()				{ return m_bDieForGFW; }
	void		SetDieInSpecialMap( BOOL bDie, WORD wReturnMap )	{ m_bDieInSpecialMap = bDie; m_wReturnMapNum = wReturnMap; }	
	
	void		SendPlayerToMap();	
	
	// Guild Tournament
	void SetReturnMapNum( WORD num )	{	m_wGTReturnMapNum = num;	}
	WORD GetReturnMapNum()			{	return m_wGTReturnMapNum;	}
	void SetObserverBattleIdx( int Idx )		{	m_ObserverBattleIdx = Idx;	}
	int	GetObserverBattleIdx()				{	return m_ObserverBattleIdx;	}

	void		SetChangeAbilityKyungGongLevel( WORD wLevel );

	// guildunion
	DWORD GetGuildUnionIdx()		{ return m_HeroCharacterInfo.dwGuildUnionIdx; }
	void InitGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx );
	void SetGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx );
	void SetGuildUnionMarkIdx( DWORD dwMarkIdx )	{ m_HeroCharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx; }

	// SiegeWar
	void SetRestraintMode( bool val )			{	m_HeroCharacterInfo.bRestraint = val;	}
	BOOL IsRestraintMode()						{	return m_HeroCharacterInfo.bRestraint;	}
	void ReviveVillage();

	//
	void SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo );	

	// character attribute (속성)
#ifdef _JAPAN_LOCAL_
//	DWORD	GetCharAttr()			{ return m_HeroCharacterInfo.dwCharacterAttr; }
	int		GetMainCharAttr()		{ return m_HeroCharacterInfo.nMainAttr; }		// 주속성	
	int		GetSubCharAttr()		{ return m_HeroCharacterInfo.nSubAttr; }		// 부속성
	BOOL	ChangeMainCharAttr( int nMainAttr );
	BOOL	ChangeSubCharAttr( int nSubAttr );	
	int		WhatIsAttrRelation( int nOtherMainAttr );		// 속성관계는?			

	// ItemSlot
	void	SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot );
	DWORD	GetExtraInvenSlot()			{	return m_HeroCharacterInfo.ExtraInvenSlot;		}
	DWORD	GetExtraPyogukSlot()		{	return m_HeroCharacterInfo.ExtraPyogukSlot;		}
	DWORD	GetExtraMugongSlot()		{	return m_HeroCharacterInfo.ExtraMugongSlot;		}
	void	SetExtraCharacterSlot( DWORD SlotNum )		{	m_dwCharacterSlotNum = SlotNum;	}
	DWORD	GetExtraCharacterSlot()		{	return m_dwCharacterSlotNum;	}
#endif

#ifdef _HK_LOCAL_
	// ItemSlot
	void	SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot );
	DWORD	GetExtraInvenSlot()			{	return m_HeroCharacterInfo.ExtraInvenSlot;		}
	DWORD	GetExtraPyogukSlot()		{	return m_HeroCharacterInfo.ExtraPyogukSlot;		}
	DWORD	GetExtraMugongSlot()		{	return m_HeroCharacterInfo.ExtraMugongSlot;		}
	void	SetExtraCharacterSlot( DWORD SlotNum )		{	m_dwCharacterSlotNum = SlotNum;	}
	DWORD	GetExtraCharacterSlot()		{	return m_dwCharacterSlotNum;	}
#endif

#ifdef _TL_LOCAL_
	// ItemSlot
	void	SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot );
	DWORD	GetExtraInvenSlot()			{	return m_HeroCharacterInfo.ExtraInvenSlot;		}
	DWORD	GetExtraPyogukSlot()		{	return m_HeroCharacterInfo.ExtraPyogukSlot;		}
	DWORD	GetExtraMugongSlot()		{	return m_HeroCharacterInfo.ExtraMugongSlot;		}
	void	SetExtraCharacterSlot( DWORD SlotNum )		{	m_dwCharacterSlotNum = SlotNum;	}
	DWORD	GetExtraCharacterSlot()		{	return m_dwCharacterSlotNum;	}
#endif



	//SW051112 무쌍모드
	void	InitMussangPoint()			{	m_dwMussangPoint = 0;		}
	void	SetMussangMode(BOOL bMode)	{	m_HeroCharacterInfo.bMussangMode = bMode;	}
	BOOL	IsMussangMode()				{	return m_HeroCharacterInfo.bMussangMode;	}
	BOOL	IsMussangReady()			{	return m_bIsMussangReady;	}
	DWORD	StartMussangMode();
	void	CheckMussangTime();
	void	SetMussangMaxPoint();
//	DWORD	GetMussangMaxPoint()		{	return m_HeroInfo.MaxMussangPoint;	}
	DWORD	GetMussangMaxPoint();
	void	AddMussangPointFromLevel(DWORD dwMonsterLevel);
	void	SendMussangInfo();
	void	SendMussangEnd();	//캐릭 죽었을 시 또는 지속 시간 종료시
	void	CheatMussangReady();

#ifdef _JAPAN_LOCAL_
	virtual DWORD CalcShieldDamage(DWORD ShieldDamage);
#else
	// 06. 03 국내무쌍 - 이영준
	virtual DWORD CalcShieldDamage(DWORD ShieldDamage, DWORD &RealShieldDamage);
	WORD	GetMussangStat() { return m_wMussangStat; }
#endif

	////////////////////////////////////////////////////////
	//06. 06 2차 전직 - 이영준
	//이펙트 생략(무초)
	BOOL IsSkipSkill();
	////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	void AddSkillStatsOption(SKILLOPTION *pSkillOption);
	void RemoveSkillStatsOption(SKILLOPTION *pSkillOption);
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신
	BOOL IsHide() { return m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_Hide]; } 
	BOOL IsDetect()
	{ 
		return (m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_Detect] ||
			m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_DetectItem]); 
	} 
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 06. 07. 스킬개선 - 이영준
	CYHHashTable<CSkillObject>* GetSkillList() { return &m_SkillList; }
	//////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// 06. 07. 상태강제변경 - 이영준
	// 일정이상 스킬 사용실패시 특별한 상태를 제외한 나머지 상태는 모두 초기화
	BOOL CanSkillState();
	////////////////////////////////////////////////////////////////////////////

	// 06.09.25 RaMa 추석이벤트
	void IncreaseEventHammerCount();

	//SW061211 크리스마스이벤트
	void SummonEventPetRndm();
	void SetEventSnowIdx(int snowIdx)	{ m_CristmasEventSnowIdx = snowIdx;	}
	DWORD GetEventSnowIdx()	{	return m_CristmasEventSnowIdx;	}

	//SW061205 깃발 NPC
	void SendFlagNPCState();

	void InitTitanWearItemInfo( ITEMBASE* pTitanItemInfo );	// magi82 - Titan(070209)
	void InitTitanShopItemInfo( ITEMBASE* pTitanItemInfo );	// magi82 - Titan(070228)

	//SW070127 타이탄
	BOOL InTitan();
	BOOL CanUseTitanSkill();
	CTitanManager*	GetTitanManager()	{	return &m_TitanManager;	}
	CTitan* GetCurTitan()	{	return m_TitanManager.GetCurRidingTitan();	}
	void SetTitanWearInfo(POSTYPE slotPos, ITEMBASE* pItemBase, BOOL bIn);
	void ApplyTitanWearItem();

	///// 2007. 6. 14. CBH - 세트아이탬 능력치 처리 함수
	void AddSetitemOption(SET_ITEM_OPTION* pSetItemOption);
	void ClearSetitemOption();
	///////////////////////////////////////////////////////

	// 
	void SetActionTime()				{	m_LastActionTime = gCurTime;	}
	BOOL IsActionPanelty()				{	return m_bActionPanelty;		}
	void SetActionPanelty()				{	m_bActionPanelty = TRUE;		}
	void SendNoActionPaneltyMsg();
	void InsetNoActionPaneltyLog( BYTE eLogKind );

	// magi82 - UniqueItem(070626)
	void AddUniqueItemOption(UNIQUE_ITEM_OPTION_INFO* pUniqueOption);
	void ClearUniqueItemOption()	{	ZeroMemory(&m_UniqueItemStats, sizeof(m_UniqueItemStats));	}
	//////////////////////////////////////////////////////////////////////////

	// magi82 - SOS(070720)
	int GetSOSState()	{	return m_SOSState;	}
	void SetSOSState(int num)	{	m_SOSState = num;	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	void InitSkinDelay();			//2007. 12. 10. CBH - 스킨교체 딜레이 시간 관련 초기화
	void StartSkinDelay();			//2007. 12. 10. CBH - 스킨교체 딜레이 시작	
	BOOL GetSkinDelayResult();		//2007. 12. 10. CBH - 스킨 딜레이중인지 반환
	DWORD GetSkinDelayTime();		//2007. 12. 10. CBH - 스킨 딜레이 남은시간 반환
	/////////////////////////////////////////////////////////////////////////////////////////////////

	// AutoNote
protected:
	DWORD	m_dwAutoNoteAskPlayerIdx;		// 오토질문상대인덱스
	DWORD	m_dwLastAttackTimeForAutoNote;

public:
	void SetAutoAskPlayerIdx( DWORD dwPlayerIdx )	{ m_dwAutoNoteAskPlayerIdx = dwPlayerIdx; }
	DWORD GetAutoAskPlayerIdx()						{ return m_dwAutoNoteAskPlayerIdx; }

	void SetLastAttackTimeForAutoNote( DWORD dwTime )	{ m_dwLastAttackTimeForAutoNote = dwTime; }
	DWORD GetLastAttackTimeForAutoNote()				{ return m_dwLastAttackTimeForAutoNote; }
};

#endif // !defined(AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_)
