// GameResourceManager.h: interface for the CGameResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMERESOURCEMANAGER_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_)
#define AFX_GAMERESOURCEMANAGER_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameResourceStruct.h"
//#include "MapChange.h"
class CMHFile;
#include "ptrlist.h"
#include <vector>

#define MIDRSLTN_W	1024
#define MIDRSLTN_H	768

#define LOWRSLTN_W	800
#define LOWRSLTN_H	600

#define GAMERESRCMNGR	USINGTON(CGameResourceManager)

//SW061111 저해상도옵션

#define GET_MAINWIN_W	GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth
#define GET_MAINWIN_H	GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight

///////////////////////////////////////////////////////////////////////////////
// 06. 05. PreLoadData 추가기능 - 이영준
// 이펙트 PreLoad
struct sPRELOAD_EFFECT_INFO{
	int MapNum;
	int Count;

	cPtrList Effect;
};
///////////////////////////////////////////////////////////////////////////////

struct DROPINFO_TIMAP
{
	DWORD	wExpPoint;
	DWORD	wMoneyMin;
	DWORD	wMoneyMax;
};
struct MONSTERDROP_TIMAP
{
	DROPINFO_TIMAP	stDropInfo[MAX_PARTY_LISTNUM];
};

class CGameResourceManager  
{
	BOOL m_bMonsterRegenLoad;
	BOOL m_bNpcRegenLoad;
	BOOL m_bMonsterListLoad;
	BOOL m_bNpcListLoad;
	BOOL m_bMonsterDropItem;
	BOOL m_bLowResolution;
    DWORD m_PaneltyTime;

	// magi82 //////////////////////////////////////////////////////////////////////////
	CMemoryPoolTempl<LEV_PENALTY>*		m_LevPenaltyPool;
	CYHHashTable<LEV_PENALTY>			m_LevPenaltyTable;
	////////////////////////////////////////////////////////////////////////////////////

	// magi82 - UniqueItem(070625)
	CYHHashTable<UNIQUE_ITEM_OPTION_INFO>	m_UniqueItemListTable;
	//////////////////////////////////////////////////////////////////////////

	// magi82 - UniqueItem(070709)
	CYHHashTable<UNIQUE_ITEM_MIX_INFO>		m_UniqueItemMixTable;
	
	DWORD m_MaxNpcChxList;
	CHXLIST* m_NpcChxList;

	CYHHashTable<MONSTERDROP_TIMAP>		m_TitanMapMosterDropTable;

	CYHHashTable<SKIN_SELECT_ITEM_INFO>	m_NomalClothesSkinTable;	//2007. 12. 4. CBH - 일반 의복 스킨 리스트 추가
	CYHHashTable<SKIN_SELECT_ITEM_INFO>	m_CotumeSkinTable;		//2007. 12. 4. CBH - 코스튬 스킨 리스트 추가	

	// magi82(37) 맵 속성 데이터


	CYHHashTable<stMAPKINDINFO>	m_MapKindStateInfoTable;	// 맵 상태 정보 테이블


	DWORD m_MaxItemChxCount;
	CHXLIST* m_ItemChxList;

	//060608 - fixed by wonju-------------------------------------------------------------------------
private:
	//맵에 분포하는 몬스터의 레벨
	struct MonsterInfoInMap
	{
		unsigned int MapNumber;
		unsigned int MinLevel;
		unsigned int MaxLevel;
	};

	struct ItemChangeRateofLv
	{
		unsigned int Level;    //레벨
		unsigned int Rate[10]; //변화율 -> 속성수치 +0 ~ +9 : 각각의 변화율
	};

	struct ItemChangeRateofAttrib
	{
		
		struct _attribInfo
		{
			WORD changeIndex;
			unsigned int changeRate;
		};
		
		_attribInfo attribinfo[5]; //0 base 1 life 2 mana 3 power 4 defend
	};

	std::vector<MonsterInfoInMap> m_MonsterInfoInMapList; //by wonju
	std::vector<ItemChangeRateofLv> m_ItemChangeRateofLvList; //Add 060705 by wonju
	std::vector<ItemChangeRateofAttrib> m_ItemChangeRateAttribList; //Add 060707 by wonju
	std::vector<std::string> m_UserCommendList; //Add 060727 by wonju

public:

	void LoadUserCommendList(); //Add 060727 by wonju

	void LoadItemChangeRateofLv();    //Add 060705 by wonju
	void LoadItemChangeRateofAttrib();//Add 060707 by wonju
	
	void LoadMonsterInfoInMapList(); //by wonju

	std::vector<std::string>& GetUserCommendList() { return m_UserCommendList; }
	std::vector<MonsterInfoInMap>& GetMonsterInfoInMapList() { return m_MonsterInfoInMapList; }
	int CheckAttribItemIndex(WORD idx);
	std::vector<ItemChangeRateofAttrib>& GetItemChangeRateAttribList() { return m_ItemChangeRateAttribList; }
	std::vector<ItemChangeRateofLv>& GetItemChangeRateOfLvList() { return m_ItemChangeRateofLvList; }
	unsigned int GetMinMonsterLvInMap(unsigned int MapNum);
	
	
	//------------------------------------------------------------------------------------------------
public:
	
	WORD m_TotalNpcGroupCount;
	WORD m_MaxNpcRegenCount;
	CGameResourceManager();
	virtual ~CGameResourceManager();

	//GETINSTANCE(CGameResourceManager);


	/* Á÷¾÷º° ±âº» ½ºÅÝÆ÷ÀÎÆ® */
	CharBasicStats				m_BasicStatsForJob[MAX_JOB];

	/* ¸ó½ºÅÍ Æ÷ÀÎÅÍ */	
	// 0 index»ç¿ë¾ÈÇÔ
	// second index 
	// 0 => -5, 1 => -4, 2 => -3, 3 => -2, ....
	DWORD m_PLAYERxMONSTER_POINT[MAX_PLAYERLEVEL_NUM][MAX_MONSTERLEVELPOINTRESTRICT_NUM + MONSTERLEVELRESTRICT_LOWSTARTNUM + 1];
	

	/* Ä³¸¯ÅÍ »ý¼º ±âº» ¼öÄ¡ */
//	CharMakeCombo* InitCharMakeCombo();

	/* Ä³¸¯ÅÍ °æÇèÄ¡ */
	EXPTYPE m_CharacterExpPoint[MAX_CHARACTER_LEVEL_NUM];
	BOOL LoadExpPoint();
	EXPTYPE GetMaxExpPoint(LEVELTYPE level)
	{ 
		// 06.09.14 RaMa  수정
		if( level > 0 )			return m_CharacterExpPoint[level-1];
		else					return m_CharacterExpPoint[0];
	}
	/* ¸ó½ºÅÍ Æ÷ÀÎÅÍ */
	BOOL LoadPlayerxMonsterPoint();
	DWORD GetPLAYERxMONSTER_POINT(LEVELTYPE level,int levelgap);

	CYHHashTable<BASE_PET_LIST> m_PetListTable;
	BOOL LoadPetList();
	BASE_PET_LIST*  GetPetListInfo( int idx );
	void ResetPetList();

	PET_RULE m_PetRule;
	BOOL LoadPetRule();
	PET_RULE* GetPetRule()	{	return &m_PetRule;	}

	//CYHHashTable<PET_BUFF_LIST>	m_PetBuffTable;
	PET_BUFF_LIST* m_pBuffList;
	BOOL LoadPetBuffList();
	PET_BUFF_LIST* GetPetBuffInfo(int idx);
	void ResetPetBuffList();

//========================================================	
	//SW070127 타이탄
	DWORD m_TitanSpellCostInfo[MAX_MAP_ENUMCOUNT+1];	// 맵별 마나소모 차등
	BOOL LoadTitanSpellCostInfo();
	DWORD GetTitanSpellCost(WORD MapNum);

	CYHHashTable<BASE_TITAN_LIST> m_TitanListTable;
	BOOL LoadTitanList();
	BASE_TITAN_LIST* GetTitanListInfo(DWORD idx);
	TITANINFO_GRADE* GetTitanGradeInfo(DWORD idx, DWORD grade);
	void RemoveTitanList();

	/*
	CYHHashTable<TITAN_SKILLLINK> m_TitanSkillLink;
	BOOL LoadTitanSkillLink();
	//TITAN_SKILLLINK* GetTitanSkillLink(DWORD idx);
	TITAN_SKILLLINK* GetTitanSkillIdx(DWORD charSkillIdx);		// 캐릭터 스킬인덱스로 부터..
	void RemoveTitanSkillLink();*/

	/*
	CYHHashTable<TITAN_EQUIPITEM_ENDURANCE> m_TitanEquipEnduranceInfo;
	BOOL LoadTitanEquipEnduranceInfo();							// 부품별 내구도 정보.
	TITAN_EQUIPITEM_ENDURANCE* GetTitanEquipEndurance(DWORD titanEquipItemIdx);
	DWORD GetDecreaseEnduranceFromItemIdx(DWORD titanEquipItemIdx);
	void RemoveTitanEquipEnduranceInfo();*/

	TITAN_RULE	m_TitanRule;
	BOOL	LoadTitanRule();
	TITAN_RULE*	GetTitanRule()	{	return &m_TitanRule;	}
	
//=======================================================

	//SW061205 깃발 NPC
	//enum{eGTFlg, eSGFlg, eFlgKind};
	enum DayOfWeek{Sun,Mon,Tue,Wed,Thu,Fri,Sat,Week,};
	BYTE FlagNpcSet[eFlgKind][Week];
	BOOL LoadFlagNpcInfo();
	BYTE GetFlagFromDate(int flgKind, int day);

	/* ¸ó½ºÅÍ ¸®½ºÆ® */
	BOOL LoadMonsterList();

	/* Æ¯¼ö ¸ó½ºÅÍ µ¥ÀÌÅÍ ¸®½ºÆ®  */
	CYHHashTable<BASE_MONSTER_LIST> m_SMonsterListTable;
	void LoadSMonsterList();
	void LoadSMonsterInfo( int idx, char * fileName );
	BASE_MONSTER_LIST * GetMonsterListInfo(int idx);
	int GetMonsterIndexForName( char* pStrName );
	void ResetMonsterList();

	void GetMonsterStats(WORD wMonsterKind, monster_stats * stats);				// ¸ó½ºÅÍ °´Ã¼ ÃÊ±âÈ­µÉ¶§
	BOOL LoadNpcChxList();
	char* GetMonsterChxName(DWORD index);
	char* GetNpcChxName(DWORD index);
 
	BOOL LoadTacticStartInfoList();
	BOOL LoadTaticAbilityInfo();	
	
	NPC_LIST m_NpcList[MAX_NPC_NUM];
	BOOL LoadNpcList(NPC_LIST* pNpcList);

	NPC_LIST* GetNpcInfo(WORD kind){return &m_NpcList[kind-1];}

	STATIC_NPCINFO*	GetStaticNpcInfo( WORD wNpcUniqueIdx );

	//SW070626 보부상NPC
#define BOBUSANG_POSNUM_MAX	4	// 맵별 출현 최대 위치갯수
	BobusangPosPerMap	m_PosPerMap[MAX_MAP_ENUMCOUNT][BOBUSANG_POSNUM_MAX];	// 맵별 생성 정보
	BOOL LoadBobusangNpcInfo();
	BobusangPosPerMap* GetBobusangPos(DWORD mapNum, DWORD posNum);

	// ¸ÊÀüÈ¯/·Î±×ÀÎÆ÷ÀÎÆ®
	BOOL LoadMapChangeArea(); //060608 - Add by wonju
	BOOL LoadMapChangeArea(MAPCHANGE_INFO* pMapChangeArea);
	BOOL LoadLoginPoint(LOGINPOINT_INFO* pLogin);
	MAPCHANGE_INFO m_MapChangeArea[MAX_POINT_NUM];
	LOGINPOINT_INFO m_LoginPoint[MAX_POINT_NUM];
	void GetRegenPoint(VECTOR3& pos,VECTOR3& RandPos,WORD LoginIndex,WORD MapchangeIndex);
	void GetRegenPointFromEventMap(VECTOR3& pos,VECTOR3& RandPos,WORD LoginIndex,WORD MapchangeIndex,DWORD dwTeam);
	MAPCHANGE_INFO* GetMapChangeInfo(DWORD Kind)
	{
		for(int i = 0; i < MAX_POINT_NUM; i++)
		{
			if(m_MapChangeArea[i].Kind == Kind)
				return &m_MapChangeArea[i];
		}
		return NULL;
	}
//KES for Cheat
	MAPCHANGE_INFO* GetMapChangeInfoForDestMapNum( WORD wDestMapNum )
	{
		for(int i = 0; i < MAX_POINT_NUM; i++)
		{
			if(m_MapChangeArea[i].MoveMapNum == wDestMapNum)
			{
				return &m_MapChangeArea[i];
			}
		}
		return NULL;
	}

	MAPCHANGE_INFO* GetMapChangeInfoFromToMapNum( WORD wFromMapNum, WORD wToMapNum )
	{
		for(int i = 0; i < MAX_POINT_NUM; ++i)
		{
			if(m_MapChangeArea[i].CurMapNum == wFromMapNum && m_MapChangeArea[i].MoveMapNum == wToMapNum)
				return &m_MapChangeArea[i];
		}
		return NULL;
	}
	
	LOGINPOINT_INFO* GetLoginPointInfo(DWORD Kind)
	{
		for( int i = 0; i < MAX_POINT_NUM; i++)
		{
			if(m_LoginPoint[i].Kind == Kind)
				return &m_LoginPoint[i];
		}
		return NULL;
	}
	LOGINPOINT_INFO* GetLoginPointInfoFromMapNum(WORD MapNum)
	{
		for( int i = 0; i < MAX_POINT_NUM; i++)
		{
			if(m_LoginPoint[i].MapNum == MapNum)
				return &m_LoginPoint[i];
		}
		ASSERT(0);
		return &m_LoginPoint[0];
	}
	BOOL CheckCorrectLoginPoint(VECTOR3* pos);

	BOOL LoadDeveloperInfo();

	// Avatar Item
	CYHHashTable<AVATARITEM>		m_AvatarEquipTable;
	BOOL LoadAvatarEquipList();
	// ShopItem Dup Option
	CYHHashTable<SHOPITEMDUP>		m_ShopItemDupOptionTable;
	BOOL LoadShopItemDupList();

	// magi82 - UniqueItem(070625)
	BOOL	LoadUniqueItemOptionList();
	UNIQUE_ITEM_OPTION_INFO*	GetUniqueItemOptionList( DWORD dwItemIdx );

	// magi82 - UniqueItem(070709)
	BOOL	LoadUniqueItemMixList();
	UNIQUE_ITEM_MIX_INFO*	GetUniqueItemMixList( DWORD dwItemIdx );

	// magi82(37) 맵 속성 데이터


	CYHHashTable<stMAPKINDINFO>* GetMapKindInfoTable() { return &m_MapKindStateInfoTable; }	
	void	LoadMapKindInfo();
	stMAPKINDINFO* GetMapKindInfo(DWORD dwMapNum);

	BOOL LoadItemChxList();
	char* GetItemChxName(DWORD index);

	//2007. 12. 4. CBH - 스킨 아이탬 관련 함수	//////////////////////////////////
	BOOL LoadNomalClothesSkinList();		//의복 세트 스킨 리스트	
	SKIN_SELECT_ITEM_INFO* GetNomalClothesSkinList( DWORD dwKeyIndex );
	DWORD GetNomalClothesSkinListCountNum();
	void RemoveNomalClothesSkinList();
	CYHHashTable<SKIN_SELECT_ITEM_INFO>* GetNomalClothesSkinTable();

	BOOL LoadCostumeSkinList();		//코스튬 스킨 리스트	
	SKIN_SELECT_ITEM_INFO* GetCostumeSkinList( DWORD dwKeyIndex );
	DWORD GetCostumeSkinListCountNum();
	void RemoveCostumeSkinList();
	CYHHashTable<SKIN_SELECT_ITEM_INFO>* GetCostumeSkinTable();
	//////////////////////////////////////////////////////////////////
	
//-----------------------------------------------------------------------------------------------------------//
//		Å¬¶óÀÌ¾ðÆ®¸¸ »ç¿ëÇÏ´Â ºÎºÐ
#ifdef _CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//

	MOD_LIST			m_ModFileList[GENDER_MAX];
	MOD_LIST			m_HairModList[GENDER_MAX];
	MOD_LIST			m_FaceModList[GENDER_MAX];
	BOOL LoadModFileList(MOD_LIST pModList[GENDER_MAX]);
	BOOL LoadHairModFileList(MOD_LIST pHairModList[GENDER_MAX]);
	BOOL LoadFaceModFileList(MOD_LIST pFaceModList[GENDER_MAX]); //pjscode
	BOOL LoadBodyModFileList(MOD_LIST pBodyModList[GENDER_MAX]);

	char*				m_TitanChxList[TITANKIND_MAX][MAX_TITAN_GRADE];
	MOD_LIST			m_TitanModFileList;
	//MOD_LIST			m_TitanHairModeList[TITANKIND_MAX];
	//MOD_LIST			m_TitanFaceModList[TITANKIND_MAX];
    MOD_LIST			m_TitanAttachModList[TITANKIND_MAX];	
	BOOL LoadTitanModFileList(MOD_LIST* pModList);
	//BOOL LoadTitanHairModList(MOD_LIST pHairModeList[TITANKIND_MAX]);
	//BOOL LoadTitanFaceModList(MOD_LIST pFaceModList[TITANKIND_MAX]);
	BOOL LoadTitanAttachList(MOD_LIST pAttachModList[TITANKIND_MAX]);
	//BOOL LoadTitanBodyModList(MOD_LIST pTitanBodyModList[TITANKIND_MAX]);

	GAMEDESC_INI		m_GameDesc;
	BOOL LoadGameDesc();
	
	void LoadPreDataTable();
	void DeletePreLoadData();
	void LoadPreMonsterData(sPRELOAD_INFO* pPreLoadInfo, MAPTYPE MapNum);	
	CYHHashTable<sPRELOAD_INFO>		m_PreLoadDataTable;
	sPRELOAD_INFO*	GetPreDataInfo(int MapNum)		{	return m_PreLoadDataTable.GetData(MapNum);	}
	cPtrList m_PreLoadEfflist;
	cPtrList* GetPreLoadEfflist()	{	return &m_PreLoadEfflist;	}
	cPtrList m_PreLoadModlist;
	cPtrList* GetPreLoadModlist()	{	return &m_PreLoadModlist;	}
	///////////////////////////////////////////////////////////////////////////////
	// 06. 04. PreLoadData 추가기능 - 이영준
	WORD* m_PreLoadItemlist;
	WORD* GetPreLoadItemlist()	{	return m_PreLoadItemlist;	}
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	// 06. 05. PreLoadData 추가기능 - 이영준
	CYHHashTable<sPRELOAD_EFFECT_INFO>		m_PreLoadEffectTable;
	sPRELOAD_EFFECT_INFO*	GetPreEffectInfo(int MapNum)		{	return m_PreLoadEffectTable.GetData(MapNum);	}
	///////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////
	//+10 이상 아이탬의 이펙트 관련 추가
	CYHHashTable<PLUSITEM_EFFECT_INFO> m_PlusItemEffectInfoTable;	//2008. 6. 18. CBH - +10 이상 아이탬의 이펙트 정보 테이블

	BOOL LoadPlusItemEffect();
	void RemovePlusItemEffectTable();
	PLUSITEM_EFFECT_INFO* GetPlusItemEffectInfo(WORD wItemType);
	////////////////////////////////////////////////////////////////////////////////

	


	void PreLoadData();	
	
	// serverlist
	SEVERLIST	m_ServerList[30];
	int			m_nMaxServerList;
	BOOL		LoadServerList();
	char*		GetServerSetName();			// 현재서버셋의 이름을 리턴

	//SW061111 저해상도옵션
	BOOL	IsLowResolution()	{	return m_bLowResolution;	};

#ifdef _TESTCLIENT_
	TESTCLIENTINFO m_TestClientInfo;
	BOOL LoadTestClientInfo();
#endif

//-----------------------------------------------------------------------------------------------------------//
#endif //_CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------//
//		¼­¹ö¸¸ »ç¿ëÇÏ´Â ºÎºÐ
#ifdef _SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
	
//	void GetMonsterStats(WORD wMonsterKind, MONSTER_STATS * stats);



	MAPTYPE m_LoadingMapNum;
	MAPTYPE GetLoadMapNum()
	{
		return m_LoadingMapNum;
	}
	void SetLoadMapNum(MAPTYPE Mapnum);
	

	BOOL LoadNpcRegen(NPC_REGEN pNpcRegen[MAX_NPC_GROUPNUM][MAX_NPC_REGEN_NUM]);

	MAPCHANGE_INFO* GetMapChangeAreaValue(){return m_MapChangeArea;}
	LOGINPOINT_INFO* GetLoginPointValue(){return m_LoginPoint;}
	
	void SetNpcRegenLoadFlag(BOOL flag){m_bNpcRegenLoad = flag;}
	void SetNpcListLoadFlag(BOOL flag){m_bNpcListLoad = flag;}
	BOOL GetNpcRegenLoadFlag(){return m_bNpcRegenLoad;}
	BOOL GetNpcListLoadFlag(){return m_bNpcListLoad;}
	

	CYHHashTable<STATIC_NPCINFO>	m_StaticNpcTable;
	BOOL	LoadStaticNpc();
	void	ReleaseStaticNpc();
	NPC_LIST* GetNpcList(WORD JobKind);

	
	CYHHashTable<STATIC_NPCINFO>	m_WareNpcTable;
	BOOL	IsInWareNpcPosition( VECTOR3* pvPos );

	// 06. 05 HIDE NPC - 이영준
	CYHHashTable<HIDE_NPC_INFO>		m_HideNpcTable;
	BOOL	LoadHideNpcList();
	HIDE_NPC_INFO* GetHideNpcInfo(WORD UniqueIdx) { return m_HideNpcTable.GetData(UniqueIdx); }
	CYHHashTable<HIDE_NPC_INFO>* GetHideNpcTable() { return &m_HideNpcTable; }

	// 06.09.25 RaMa - 추석이벤트
	int		m_FullMoonEventRate[eFULLMOONEVENT_MAX];
	BOOL	LoadFullMoonEventRate();
	int		GetFullMoonEventIdx( int Rate );

	// magi82 //////////////////////////////////////////////////////////////////////////
	BOOL	LoadLevPenalty();
	LEV_PENALTY*	GetLevPenalty( LEVELTYPE Lev );
	////////////////////////////////////////////////////////////////////////////////////

	BOOL	LoadPaneltyTime();
	DWORD	GetPaneltyTime()			{	return m_PaneltyTime;	}

	void LoadTitanMapMonsterDropInfo();
	
	MONSTERDROP_TIMAP*	GetTitanMapMonsterDropInfo( WORD wLevel )		{	return m_TitanMapMosterDropTable.GetData( wLevel );	}

//-----------------------------------------------------------------------------------------------------------//
#endif //_SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
};
EXTERNGLOBALTON(CGameResourceManager);
#endif // !defined(AFX_GAMERESOURCEMANAGER_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_)
