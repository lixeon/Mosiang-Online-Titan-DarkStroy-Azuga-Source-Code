#ifndef __QUESTDEFINES_H__
#define __QUESTDEFINES_H__

#pragma pack(push,1)

#ifdef _MAPSERVER_
#define	PLAYERTYPE	CPlayer
class CPlayer;
#else
#define PLAYERTYPE	CHero
class CHero;
#endif


#define MAX_QUEST_PROBABILITY		10000

#define	MAX_QUEST					1000
#define	MAX_QUESTEVENT_MGR			1000
#define MAX_QUESTEVENT_PLYER		100
#define	MAX_SUBQUEST				32
//#define MAX_ITEMNAME_LENGTH			30
#define	MAX_ITEMDESC_LENGTH			255
#define	MAX_ITEMDESC_LINE			20
#define MAX_PROCESSQUEST			100
#define MAX_COMPLETEQUEST			100
#define VIEW_QUESTITEM_PERPAGE		20
#define	QUEST_GIVEUP_ID				99
#define MAX_QUESTITEM				100


#define TITLE_COLOR					RGB(210, 0, 255)
#define	MAINQUEST_TITLE_COLOR		RGB(128, 200, 128)
#define	MAINQUEST_TITLE_COLOR_SEL	RGB(128, 255, 128)
#define SUBQUEST_TITLE_COLOR		RGB(0, 180, 210)
#define SUBQUEST_TITLE_COLOR_SEL	RGB(0, 200, 255)
#define QUEST_GIVEUP_COLOR			RGB(80, 80, 210)
#define QUEST_GIVEUP_COLOR_SEL		RGB(100, 100, 255)
#define QUEST_DESC_COLOR			RGB(255, 255, 255)
#define QUEST_DESC_HIGHLIGHT		RGB(250, 200, 50)
#define	SELECT_COLOR				RGB(225, 200, 200)
#define	SELECT_COLOR_SEL			RGB(250, 230, 230)
#define	TITAN_MAINQUEST_TITLE_COLOR		RGB(255, 200, 0)
#define	TITAN_MAINQUEST_TITLE_COLOR_SEL	RGB(255, 255, 0)



#define COMBINEKEY(a, b, c)				\
	if(b<100)			c=a*100+b;		\
	else if(b<1000)		c=a*1000+b;		\
	else if(b<10000)	c=a*10000+b;	


enum eQuestEvent
{
	eQuestEvent_NpcTalk = 1,		// Npc´eE­
	eQuestEvent_Hunt,				// Monster≫c³E
	eQuestEvent_EndSub,
	eQuestEvent_Count,	
	eQuestEvent_GameEnter,	
	eQuestEvent_Level,	
	eQuestEvent_UseItem,
	eQuestEvent_MapChange,
	eQuestEvent_Die,	
	eQuestEvent_Time,
	eQuestEvent_HuntAll,
};

enum eQuestExecute
{
	eQuestExecute_EndQuest,
	eQuestExecute_StartQuest,

	eQuestExecute_EndSub,
	eQuestExecute_EndOtherSub,
	eQuestExecute_StartSub,
	
	eQuestExecute_AddCount,	
	eQuestExecute_MinusCount,

	eQuestExecute_GiveQuestItem,	// quest item 창에 들어가는 거
	eQuestExecute_TakeQuestItem,	
	
	eQuestExecute_GiveItem,			// inventory
	eQuestExecute_GiveMoney,
	eQuestExecute_TakeItem,		
	eQuestExecute_TakeMoney,
	eQuestExecute_TakeExp,	
	eQuestExecute_TakeSExp,

	eQuestExecute_RandomTakeItem,

	eQuestExecute_TakeQuestItemFQW,	// from 특정 무기로
	eQuestExecute_AddCountFQW,
	eQuestExecute_TakeQuestItemFW,	// from 무기류
	eQuestExecute_AddCountFW,
	
	eQuestExecute_TakeMoneyPerCount,
	
	eQuestExecute_RegenMonster,
	eQuestExecute_MapChange,
	
	eQuestExecute_ChangeStage,
	
	eQuestExecute_ChangeSubAttr,

	eQuestExecute_RegistTime,

	eQuestExecute_LevelGap,
	eQuestExecute_MonLevel,

	eQuestExecute_EndOtherQuest,

	eQuestExecute_SaveLoginPoint,
};

enum eQuestLimitKind
{
	eQuestLimitKind_Level,
	eQuestLimitKind_Money,
	eQuestLimitKind_Quest,
	eQuestLimitKind_SubQuest,	
	eQuestLimitKind_Stage,
	eQuestLimitKind_Attr,
};

enum eQuestValue
{
	eQuestValue_Add,
	eQuestValue_Minus,
};

enum
{
	eQView_QList,
	eQView_QDesc,
};

enum
{
	eQTree_NotUse,
	eQTree_Close,
	eQTree_Open,
};

enum
{
	eQItem_AddCount,
	eQItem_SetCount,
	eQItem_GetCount,
};

struct SUBQUEST
{
	DWORD	dwMaxCount;
	DWORD	dwData;
	DWORD	dwTime;
	SUBQUEST() : dwMaxCount(0), dwData(0), dwTime(0)	{}
};

struct QUESTITEM
{
	DWORD	dwQuestIdx;
	DWORD	dwItemIdx;
	DWORD	dwItemNum;
	QUESTITEM() : dwQuestIdx(0), dwItemIdx(0), dwItemNum(0)	{}
};

struct QUEST_ITEM_INFO
{
	DWORD		ItemIdx;
	DWORD		QuestKey;
	char		ItemName[MAX_ITEMNAME_LENGTH+1];
	WORD		Image2DNum;
	WORD		SellPrice;
	char		ItemDesc[MAX_ITEMDESC_LENGTH+1];
};

#pragma pack(pop)
#endif
