// GameResourceManager.h: interface for the CGameResourceManager class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

//#include "..\[CC]Header\GameResourceStruct.h"
//#include ".\common\Commondefine.h"
#include "MHFile.h"
#include "..\[CC]Ability\AbilityInfo.h"
#include "..\[CC]Quest\QuestString.h"

#define GAMERESRCMNGR	USINGTON(CGameResourceManager)

class CGameResourceManager  
{
protected:
//	CharBasicStats m_BasicStatsForJob[MAX_JOB]; // Á÷¾÷º° ½ºÅÈ
	DWORD m_CharacterExpPoint[MAX_CHARACTER_LEVEL_NUM]; // ·¹º§º° ÇÊ¿ä °æÇèÄ¡

	CYHHashTable<ITEM_INFO>			m_ItemInfoList;
	CYHHashTable<SKILLINFO>			m_SkillInfoList;
	CYHHashTable<KYUNGGONG_INFO>	m_KyunggongInfoList;
	
	CYHHashTable<CAbilityInfo>		m_AbilityInfoList;

	CYHHashTable<NPC_LIST>			m_NpcList;
	CYHHashTable<MONSTEREX_LIST>		m_MonsterList;

	CYHHashTable<CQuestString>		m_QuestStringTable;
	
	CYHHashTable<char>				m_MapNameTable;

public:
	CGameResourceManager();
	virtual ~CGameResourceManager();

	virtual BOOL Init();

	//////////////////////////////////////////////////////////
	////// Data Loading
//	BOOL LoadCharCreateBasicStats();
	BOOL LoadExpPoint();

	BOOL LoadItemList();
	BOOL LoadSkillInfoList();
	BOOL LoadKyunggongInfoList();
	BOOL LoadAbilityInfoList();
	BOOL LoadNpcList();
	BOOL LoadMonsterList();
	BOOL LoadMapNameList();
	char*	GetMapName( DWORD dwMapNum );
	DWORD	GetMapNum( char* sMapName );
	void			LoadQuestString();
	CQuestString*	ReadQuestString(CMHFile* pFile);
	char*			GetQuestTitle( DWORD dwQuestIdx );
	char*			GetSubQuestTitle( DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	////// Check Func
	BOOL IsDupItem( WORD wItemIdx );
	BOOL IsOptionItem( WORD wItemIdx, WORD wDurability );
	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	////// Data È¹µæ ÇÔ¼ö
	DWORD				GetMaxExpPoint(WORD level)		{ return m_CharacterExpPoint[level-1]; }
	ITEM_INFO*			GetItemInfo(WORD wItemIdx)		{ return m_ItemInfoList.GetData(wItemIdx); }
	SKILLINFO*			GetSkillInfo(WORD wSkillIdx)	{ return m_SkillInfoList.GetData(wSkillIdx); }
	CAbilityInfo*		GetAbilityInfo(WORD wAbilityIdx) { return m_AbilityInfoList.GetData(wAbilityIdx); }
	KYUNGGONG_INFO*		GetKyunggongInfo(WORD wKyuggongIdx) { return m_KyunggongInfoList.GetData(wKyuggongIdx); }
	NPC_LIST*			GetNpcList(DWORD dwKind) { return m_NpcList.GetData(dwKind); }
	MONSTEREX_LIST*		GetMonsterInfo(WORD kind) { return m_MonsterList.GetData(kind);	}
	
	WORD				GetItemIdx_ByItemName(char* name);
	WORD				GetMugongIdx_ByMugongName(char* name);

	CYHHashTable<ITEM_INFO>*			GetItemInfo()	{ return &m_ItemInfoList; }
	CYHHashTable<SKILLINFO>*			GetSkillInfo()	{ return &m_SkillInfoList; }
	CYHHashTable<CAbilityInfo>*			GetAbilityInfo() { return &m_AbilityInfoList; }
	CYHHashTable<char>*					GetMapNameTable()	{ return &m_MapNameTable; }
	//////////////////////////////////////////////////////////
};

EXTERNGLOBALTON(CGameResourceManager);

#endif //__GAME_RESOURCE_STRUCT__
