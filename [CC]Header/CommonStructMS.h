#ifndef __COMMONSTRUCTMS_H__
#define __COMMONSTRUCTMS_H__

#pragma pack(push,1)
/**********************************************************************************/
/*			¾Æ·¡ÀÇ Å¬·¡½º¿¡¸¸ »ç¿ëµÇ´Â DEFINE			 						  */
/**********************************************************************************/

// allready defined in servergamestruct.h
/*
struct MSGMC :public MSGROOT
{
	DWORD dwConnectionIdxMC;
};
struct MSGUSERCOUNT :public MSGMC
{
	WORD ServerPort;
	DWORD dwUserCount;
};
struct MSGEXECUTESERVER : public MSGMC
{
	WORD ServerType;
	WORD ServerNum;
	WORD ServerPort;
	char ServerIP[MAX_IPADDRESS_SIZE];
};
struct MSGSHUTDOWNSERVER : public MSGEXECUTESERVER
{
};

struct MSGNOTICE : public MSGMC
{
	char Msg[256];
};
struct SERVERINFO
{
	WORD	wServerKind;						// ¼­¹ö Á¾·ù : agent, map, distribute
	char	szIPForServer[MAX_IPADDRESS_SIZE];
	char	szIPForUser[MAX_IPADDRESS_SIZE];	
	WORD	wPortForServer;						// ¼­¹ö Æ÷Æ®
	WORD	wPortForUser;						// À¯Àú Æ÷Æ®
	DWORD	dwConnectionIndex;					// Ä¿³Ø¼Ç ÀÎµ¦½º : °íÀ¯ÀÎµ¦½º
	WORD	wServerNum;							// 0 ~ (¸ã¹øÈ£ È¤Àº ¼­¹ö¹øÈ£)

	//--------agent field----------------
	WORD	wAgentUserCnt;
};

struct MSG_PWRUP_BOOTUP : public MSGROOT
{
	DWORD dwProcessID;
	SERVERINFO BootInfo;
};

struct MSG_PWRUP_BOOTLIST : public MSGROOT
{
	void	AddList(SERVERINFO * pInfo)
	{
		BootList[Num++] = *pInfo;
	}
	int		GetSize() { return sizeof(MSG_PWRUP_BOOTLIST) - (MAX_SERVERINFO_LIST - Num)*sizeof(SERVERINFO);	}
	WORD		MapServerPort;
	WORD		Num;
	SERVERINFO BootList[MAX_SERVERINFO_LIST];
};

struct MSERVERINFO
{
	MSERVERINFO()
	{
		serverInfo.szIPForServer[0] = 0;
		serverInfo.wPortForServer = 0;
		dwConnectionIndex = 0;
	}
	SERVERINFO serverInfo;
	DWORD	dwConnectionIndex;					// Ä¿³Ø¼Ç ÀÎµ¦½º : °íÀ¯ÀÎµ¦½º
	DWORD	ProcessID;							//mapserver
	DWORD	dwExpiredTime;							//mapserver
	WORD	wServerState;							//mapserver
};

struct MSGMSINFO : public MSGROOT
{
	MSERVERINFO MSInfo;
};

enum
{
	SERVERSTATE_NONE =0,
	SERVERSTATE_OFF=1,
	SERVERSTATE_ON=2,
	SERVERSTATE_ABNORMAL=3,
	SERVERSTATE_RECOVER=4,
};

struct SERVERSTATE
{
	WORD ServerType;
	WORD ServerNum;
	WORD ServerPort;
	char ServerIP[MAX_IPADDRESS_SIZE];
	WORD ServerState;
	int	 UserLevel;
	DWORD UserCount;
};

#define MAX_SERVERSTATE_NUM	128
struct MSGSERVERSTATE : public MSGMC
{
	SERVERSTATE serverState;
};

struct MSGMONITORTOOLALLSTATE : public MSGROOT
{

	void AddServerState(SERVERSTATE * pState)
	{
		serverState[num++] = *pState;
	}
	int GetSize()
	{
		return sizeof(MSGMONITORTOOLALLSTATE) - (MAX_SERVERSTATE_NUM - num)*sizeof(SERVERSTATE);
	}
	int num;
	SERVERSTATE serverState[MAX_SERVERSTATE_NUM];
};

*/

enum eITEMMONEYSEARCHTYPE
{
	eSearchType_ChrName = 0,
	eSearchType_ChrNameItemDBIdx,
	eSearchType_ChrNameItemIdx,
	eSearchType_ItemDBIdx,
	eSearchType_ItemIdx,
};

enum eGMTOOLSEARCHTYPE
{
	eSearchType_GMToolNone = 0,
	eSearchType_GMToolChrName,
	eSearchType_GMToolLogType,
	eSearchType_GMToolChrNameLogType,
};

enum eSKILLSERACHTYPE
{
	eSearchType_Character = 0,
	eSearchType_CharacterExp,
};

enum eCHARACTERSERACHTYPE
{
	eSearchType_Skill = 0,
	eSearchType_SkillExp,
};

enum eLOGKIND
{
	eLogKind_ItemMoneyLogFromChrName = 0,
	eLogKind_ItemMoneyLogFromItemDBIdx,
	eLogKind_BasicLog,
	eLogKind_ExpLog,
	eLogKind_SkillLog,
	eLogKind_SkillExpLog,
	eLogKind_MoneyLog,
	eLogKind_MoneyWrongLog,
};

/// 060831 PKH 문파토너먼트 로그타입
enum eGUILDTN_LOGKIND
{
	eNONE = 0,
	eNONEGUILD,
	eGUILD,
};

/// 060831 PKH 공성전 로그 타입
enum eSIEGEWAR_LOGKIND
{
	eNONESIEGEWAR = 1,
	eSIEGEWAR,
};

enum eTOTALLOGKIND
{
	eTotalLogKind_Character = 0,
	eTotalLogKind_ExpPoint,
	eTotalLogKind_Mugong,
	eTotalLogKind_MugongExp,
	eTotalLogKind_Money,
	eTotalLogKind_MoneyWrong,	
	eTotalLogKind_Ability,
	eTotalLogKind_ItemOption,
	eTotalLogKind_Guild,
};

enum eETCLOGKIND
{
	eEtcLogKind_Cheat = 0,
	eEtcLogKind_Hacking,
};

enum eOPERPOWER
{
	eOperPower_Master = 0,
	eOperPower_Monitor,
	eOperPower_Patroller,
	eOperPower_Auditor,
	eOperPower_Eventer,
	eOperPower_QA,	
	eOperPower_Max,
};

struct MSG_MCDWORD : public MSGMC
{
	DWORD dwData;
};

//////////////////////////////////////////////////////////////////////////
//  º¹±¸ Tool¿¡¼­ »ç¿ëÇÏ´Â Àü¼Û ±¸Á¶Ã¼ [2/2/2004]
struct USERINFO
{
	DWORD	dwMemberNo;					// À¯ÀÏÇÑ DB ÀÎµ¦½º
	WORD	wCharacterCount;			// ¼ÒÀ¯ Ä³¸¯¼ö(¼­¹ö±º)
	BYTE	bLoginFlag;					// Á¢¼Ó¿©ºÎ ÆÇ´Ü

	char	strID[20];					// °èÁ¤ ID
	char	strPass[20];				// °èÁ¤ ºñ¹Ð¹øÈ£
	char	strName[20];				// »ç¿ëÀÚ ÀÌ¸§
	char	strIdCard[15];				// ÁÖ¹Î¹øÈ£
	char	strPhon[20];				// ¿¬¶ôÃ³
	char	strEmail[50];				// E-Mail

	char	strRegDateTime[30];			// µî·Ï ³¯Â¥
	char	strLastLoginTime[30];		// ÃÖ±Ù Á¢¼Ó ½Ã°£
	char	strLastLogoutTime[30];		// ÃÖ±Ù Á¾·á ½Ã°£
	char	strTotalTime[50];			// ÃÑ Á¢¼Ó ½Ã°£
	int		nTotalTime;					// ÃÊ´ÜÀ§ °è»ê 
	
	WORD	wUserLevel;					// À¯Àú ·¹º§
};

struct SIMPlECHARACTERINFO
{
	DWORD	dwUserIdx;					// user
	DWORD	dwCharNo;					// Ä³¸¯ÅÍ °íÀ¯¹øÈ£
	char	strName[20];				// Ä³¸¯¸í
	char	strCreateDate[30];			// »ý¼º ½Ã°£
};

struct PHYOKUK_TOTALINFO
{
	WORD		PyokukCount;
	DWORD		Money;
	ITEMBASE	Pyokuk[SLOT_PYOGUK_NUM];
};

struct MUNPAWAREHOUSEINFO
{
	ITEMBASE	WareHouse[SLOT_MUNPAWAREHOUSE_NUM];
};

struct MUNPABASEINFO
{
	DWORD		dwMunpaIdx;
	char		strMunpaName[20];
	char		strMunjuName[20];
	char		strRegDate[20];
	WORD		wMemberNum;
	WORD		wLocation;
	BYTE		bMunpaLevel;
	DWORD		dwFamous;
	BYTE		bKind;
	MONEYTYPE	dwMoney;
};

struct MUNPAMEMBERINFO
{
	DWORD		dwCharno;
	char		strName[20];
	LEVELTYPE	level;
	BYTE		bJikChek;
	char		strRegDate[30];
};

struct GAMELOGITEM1
{
	DWORD		dwLogidx; 
	BYTE		bLogtype;
	DWORD		dwCharIdx; 
	DWORD		dwTargetIdx; 
	char		strTargetName[20];
	DWORD		dwItemIdx; 
	DWORD		dwItemDBIdx; 
	char		strLogdate[30]; 
	MONEYTYPE	dwMoney;
};

struct GAMELOGITEM
{
	DWORD		dwLogIdx; 
	DWORD		dwLogType;
	DWORD		dwItemIdx;
	DWORD		dwItemDBIdx;
	DWORD		dwCurChrIdx;
	DWORD		dwBeforeChrIdx;
	DWORD		dwMoney;
	char		strLogDate[30];
	char		strCurChrName[20];
	char		strBeforeChrName[20];
};

struct LOGITEMMONEY
{
	DWORD		dwLogIdx; 
	DWORD		dwLogType;
	DWORD		dwFromChrIdx;
	char		sFromChrName[20];
	DWORD		dwToChrIdx;
	char		sToChrName[20];
	DWORD		dwFromTotalMoney;
	DWORD		dwToTotalMoney;
	DWORD		dwChangeMoney;
	DWORD		dwItemIdx;
	DWORD		dwItemDBIdx;
	DWORD		dwItemDur;
	char		sRegDate[30];
	DWORD		dwItemFromPosition;
	DWORD		dwItemToPosition;
};

struct CHARACTERLOG
{
	DWORD		dwLogIdx;
	DWORD		dwChrIdx;
	DWORD		dwGrade;
	DWORD		dwGengoal;
	DWORD		dwDex;
	DWORD		dwStrength;
	DWORD		dwSimmak;
	DWORD		dwPoint;
	char		sLogDate[30];
};

struct EXPPOINTLOG
{
	DWORD		dwLogIdx;
	DWORD		dwLogType;
	DWORD		dwChrIdx;
	DWORD		dwGrade;
	DWORD		dwChangeValue;
	DWORD		dwKillerKind;
	DWORD		dwKillerIdx;
	DWORD		dwExpPoint;
	DWORD		dwAbilPoint;
	char		sLogDate[30];
};

struct MUGONGLOG
{
	DWORD		dwLogIdx;
	DWORD		dwLogType;
	DWORD		dwChrIdx;
	DWORD		dwSkillIdx;
	DWORD		dwSkillDBIdx;
	DWORD		dwSkillLevel;
	DWORD		dwSkillExp;
	char		sLogDate[30];
};

struct MUGONGEXPLOG
{
	DWORD		dwLogIdx;
	DWORD		dwChrIdx;
	DWORD		dwChangeValue;
	DWORD		dwSkillIdx;
	DWORD		dwSkillDBIdx;
	DWORD		dwSkillLevel;
	DWORD		dwSkillExp;
	DWORD		dwSkillPos;
	char		sLogDate[30];
};

struct GAMELOGMONEY 
{
	DWORD		dwLogIdx;
	BYTE		LogType;
	DWORD		dwCharIdx;
	DWORD		dwInvenMoney;
	DWORD		dwPyogukMoney;
	char		strLogdate[30];
};

struct OPERATOR
{
	DWORD	dwOperIdx;						// Idx
	char	strOperID[17];					// ID
	char	strOperPWD[17];					// PWD
	char	strOperName[17];				// Name
	WORD	wOperPower;						// POWER
	char	strRegDate[20];
};

struct IPINFO
{
	DWORD dwIPIdx;
	char strIP[17];
	char strRegDate[20];
};

struct OPERATOR_TOTALINFO
{
	WORD		wOperCount;
	OPERATOR	pOper[100];

	WORD		wIPCount;
	IPINFO		pIp[100];
};

struct CHARACTERCREATEINFO
{
	DWORD	dwUserIdx;
	DWORD	dwCharIdx;					// Ä³¸¯ÅÍ °íÀ¯¹øÈ£
	char	sName[20];					// Ä³¸¯¸í
	DWORD	dwFlag;
	char	sCreateDate[30];
	char	sCreateIP[16];
	char	sDeleteDate[30];
	char	sDeleteIP[16];
	char	sRecoverDate[30];
	char	sRecoverIP[16];
};

struct TMSGBASE : public MSGROOT
{
	DWORD dwTemplateIdx;
};

struct TMSG_OPERATOR_LOGIN : public MSGROOT
{
	OPERATOR		sOper;
	IPINFO			sIP;
};

struct TMSG_QUERYMEMBERNUM : public TMSGBASE
{
	WORD	wServer;
	WORD	wType;
	char	strSearch[17];
};

struct TMSG_USERSEARCH : public TMSGBASE
{
	WORD	wServer;
	BYTE	bType;
	char	strSearch[17];
};

struct TMSG_SIMPLECHARACTERINFO : public TMSGBASE
{
	WORD	wServer;
	DWORD	dwMemberNo;
};

struct TMSG_MUNPASEARCH : public TMSGBASE
{
	WORD	wServer;
	char	strSearch[17];
};

struct TMSG_PETSEARCH : public TMSGBASE
{
	WORD	wServer;
	DWORD	dwPetIndex;
	DWORD	dwChrIdx;
};

struct TMSG_PETLISTSEARCH : public TMSGBASE
{
	WORD	wServer;
	DWORD	loginidx;
};

struct TMSG_GAMELOGSEARCH : public TMSGBASE
{
	WORD	wServer;
	WORD	wType;
	DWORD	dwChrIdx;
	char	sName[17];
	DWORD	dwItem;	
	BOOL	bDayOnly;
	char	sSDate[30];
	char	sEDate[30];
};

struct TMSG_GAMELOGITEMSEARCH : public TMSGBASE
{
	WORD	wServer;
	WORD	wLogKind;		// name or dbindex
	char	strId[17];
	DWORD	dwItemDBIndex;
	DWORD	dwLogType;
	BOOL	bDayOnly;
	char	strStart[30];
	char	strEnd[30];
};

struct TMSG_MUNPABASEINFO : public TMSGBASE
{
	MUNPABASEINFO		sBaseInfo;
};

struct TMSG_USERSEARCHRESULT : public TMSGBASE
{
	USERINFO sUser;
	WORD	wServer;
};

struct TMSG_SIMPLERESULT : public TMSGBASE
{
	BYTE Count;
	SIMPlECHARACTERINFO sSimple[100];

	int GetMsgSize()	{ return sizeof(TMSG_SIMPLERESULT) - sizeof(SIMPlECHARACTERINFO)*(100-Count); }
};

struct TMSG_CHARACTERCREATEINFO : public TMSGBASE
{
	BYTE Count;
	CHARACTERCREATEINFO sInfo[100];

	int GetMsgSize()	{ return sizeof(TMSG_CHARACTERCREATEINFO) - sizeof(CHARACTERCREATEINFO)*(100-Count); }
};

struct TMSG_CHECKNAME : public TMSGBASE
{
	char	sName[32];
};

struct TMSG_CHARACTERRECOVER : public TMSGBASE
{
	DWORD	dwChrIdx;
	char	sName[32];
};

struct MAINQUESTINFO
{
	DWORD	dwCharIdx;
	DWORD	dwQuestIdx;
	DWORD	dwSubQuestDo;
	DWORD	dwEndParam;
	DWORD	dwRegDate;
};

struct TMSG_MAINQUESTINFO : public TMSGBASE
{
	DWORD			dwCount;
	MAINQUESTINFO	MainQuestInfo[100];

	DWORD	GetMsgLength()	{ return sizeof(TMSG_MAINQUESTINFO) - sizeof(MAINQUESTINFO)*(100-dwCount); }
};

struct SUBQUESTINFO
{
	DWORD	dwCharIdx;
	DWORD	dwQuestIdx;
	DWORD	dwSubQuestIdx;
	DWORD	dwData;
	DWORD	dwRegDate;
};

struct TMSG_SUBQUESTINFO : public TMSGBASE
{
	DWORD			dwCount;
	SUBQUESTINFO	SubQuestInfo[32];

	DWORD	GetMsgLength()	{ return sizeof(TMSG_SUBQUESTINFO) - sizeof(SUBQUESTINFO)*(32-dwCount); }
};

struct sUsingShopItemInfo
{
	DWORD	dwCharIdx;
	DWORD	dwItemIdx;
	DWORD	dwItemDBIdx;
	DWORD	dwItemParam;
	DWORD	dwBeginTime;
	DWORD	dwRemainTime;
};

struct TMSG_USINGSHOPITEMINFO : public TMSGBASE
{
	DWORD				dwCount;
	sUsingShopItemInfo	Info[100];

	DWORD	GetMsgLength()	{ return sizeof(TMSG_USINGSHOPITEMINFO) - sizeof(sUsingShopItemInfo)*(100-dwCount); }
};


struct TMSG_WORD : public TMSGBASE
{
	WORD		wData;
};

struct TMSG_DWORD : public TMSGBASE
{
	DWORD		dwData;
};

struct TMSG_DWORD2 : public TMSGBASE
{
	DWORD		dwData1;
	DWORD		dwData2;
};

struct TMSG_DWORD3 : public TMSGBASE
{
	DWORD		dwData1;
	DWORD		dwData2;
	DWORD		dwData3;
};

struct TMSG_NAME_DWORD : public TMSGBASE
{
	DWORD		dwChrIdx;
	char		sName[17];
	DWORD		dwData;
};

struct TMSG_CHARACTER_BASEINFO : public TMSGBASE
{
	BASEOBJECT_INFO			m_sObjinfo;
	CHARACTER_TOTALINFO		m_sTotalinfo;
	HERO_TOTALINFO			m_sHeroinfo;
};

struct TMSG_CHARACTER_INVENINFO : public TMSGBASE
{
	ITEM_TOTALINFO	m_sIteminfo;
};

// InvenÀÇ ¸ðµç Á¤º¸ °¡Á®¿À±âÀ§ÇØ
struct TMSG_CHARACTER_ITEMBASE : public TMSGBASE
{
	DWORD		dwCount;
	ITEMBASE	ItemInfo[100];
	
	TMSG_CHARACTER_ITEMBASE()
	{
		dwCount = 0;
		memset( ItemInfo, 0, sizeof(ITEMBASE)*100 );
	}
	void	Add( ITEMBASE* pItemBase )
	{
		ItemInfo[dwCount] = *pItemBase;
		++dwCount;
	}
	int		GetSize()	{ return sizeof( TMSG_CHARACTER_ITEMBASE ) - sizeof(ITEMBASE)*(100-dwCount); }
};

struct TMSG_CHARACTER_PYOGUKINFO : public TMSGBASE
{
	char		m_sUserName[17];
	DWORD		m_dwUserIdx;
	WORD		m_wPyogukNum;
	MONEYTYPE	m_dwMoney;
};

struct TMSG_ITEMINFO_LARGECOUNT : public TMSGBASE
{
	BOOL		m_bEnd;
	WORD		m_wCount;
	ITEMBASE	m_Item[100];
};

struct TMSG_ITEMOPTIONINFO : public TMSGBASE
{
	BOOL				bEnd;
	WORD				wCount;
	ITEM_OPTION_INFO	OptionInfo[100];
};

struct TMSG_RAREITEMOPTIONINFO : public TMSGBASE
{
	BOOL					bEnd;
	WORD					wCount;
	ITEM_RARE_OPTION_INFO	RareOptionInfo[100];
};

struct TMSG_IPTOTALINFO : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	IPINFO		sIP[100];
};

struct TMSG_OPERTOTALINFO : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	OPERATOR	sOper[100];
};

struct TMSG_MUNPA_MEMBERINFO : public TMSGBASE
{
	BOOL			bEnd;
	WORD			wCount;
	MUNPAMEMBERINFO	sMember[100];
};

struct TMSG_GAMELOGITEM1 : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	char		strCharName[17];
	BYTE		bDayOnly;
	char		strStart[30];
	char		strEnd[30];
	GAMELOGITEM1	sLog[100];
};

struct TMSG_GAMELOGITEM : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	DWORD		dwCharIdx;
	DWORD		dwItemDBIdx;
	BOOL		bDayOnly;
	char		strStart[30];
	char		strEnd[30];
	GAMELOGITEM	sLog[100];

	TMSG_GAMELOGITEM()
	{
		dwCharIdx = 0;
		memset( sLog, 0, sizeof(GAMELOGITEM)*100 );
	}
};

struct TMSG_LOGITEMMONEY : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	WORD		wType;
	DWORD		dwCharIdx;
	DWORD		dwItem;
	BOOL		bDayOnly;
	char		sSDate[30];
	char		sEDate[30];
	LOGITEMMONEY	sLog[100];
};

struct TMSG_CHARACTERLOG : public TMSGBASE	// characterlog
{
	BOOL		bEnd;
	WORD		wCount;
	DWORD		dwCharIdx;
	BOOL		bDayOnly;
	char		sSDate[30];
	char		sEDate[30];
	CHARACTERLOG	sLog[100];
};

struct TMSG_EXPPOINTLOG : public TMSGBASE	// exppointlog
{
	BOOL		bEnd;
	WORD		wCount;
	DWORD		dwCharIdx;
	BOOL		bDayOnly;
	char		sSDate[30];
	char		sEDate[30];
	EXPPOINTLOG	sLog[100];
};

struct TMSG_MUGONGLOG : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	DWORD		dwCharIdx;
	BOOL		bDayOnly;
	char		sSDate[30];
	char		sEDate[30];
	MUGONGLOG	sLog[100];
};

struct TMSG_MUGONGEXPLOG : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	DWORD		dwCharIdx;
	BOOL		bDayOnly;
	char		sSDate[30];
	char		sEDate[30];
	MUGONGEXPLOG	sLog[100];
};

struct MONEYLOG
{
	DWORD		dwLogIdx;
	DWORD		dwChrIdx;
	DWORD		dwLogType;
	DWORD		dwChangeValue;
	DWORD		dwInvenMoney;
	DWORD		dwTargetIdx;
	DWORD		dwTargetMoney;
	char		sLogDate[30];
};

struct TMSG_MONEYLOG : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	DWORD		dwCharIdx;
	BOOL		bDayOnly;
	char		sSDate[30];
	char		sEDate[30];
	MONEYLOG	sLog[100];
};

struct MONEYWRONGLOG
{
	DWORD		dwLogIdx;
	DWORD		dwLogType;
	DWORD		dwChrIdx;
	DWORD		dwChangeValue;
	DWORD		dwTotalMoney;
	DWORD		dwPyogukMoney;
	DWORD		dwTargetIdx;
	char		sLogDate[30];
};

struct TMSG_MONEYWRONGLOG : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	DWORD		dwCharIdx;
	BOOL		bDayOnly;
	char		sSDate[30];
	char		sEDate[30];
	MONEYWRONGLOG	sLog[100];
};

struct ABILITYLOG
{
	DWORD		dwLogIdx;
	DWORD		dwChrIdx;
	DWORD		dwAbilityIdx;
	DWORD		dwAbilityLevelTo;
	DWORD		dwAbilityExpFrom;
	DWORD		dwAbilityExpTo;
	char		sLogDate[30];	
};

struct TMSG_ABILITYLOG : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	DWORD		dwCharIdx;
	BOOL		bDayOnly;
	char		sSDate[30];
	char		sEDate[30];
	ABILITYLOG	sLog[100];
};

struct ITEMOPTIONLOG
{
	DWORD		dwLogIdx;
	DWORD		dwOptionDBIdx;
	DWORD		dwChrIdx;
	DWORD		dwItemDBIdx;
	WORD		wGengol;
	WORD		wMinchub;
	WORD		wCheryuk;
	WORD		wSimmek;
	WORD		wLife;
	WORD		wNeryuk;
	WORD		wShield;
	WORD		wFireRegis;
	WORD		wWaterRegist;
	WORD		wTreeRegist;
	WORD		wGoldRegist;
	WORD		wEarthRegist;
	WORD		wPhyAttack;
	WORD		wCritical;
	WORD		wFireAttack;
	WORD		wWaterAttack;
	WORD		wTreeAttack;
	WORD		wGoldAttack;
	WORD		wEarthAttack;
	WORD		wPhyDefense;
	char		sLogDate[30];
};

struct TMSG_ITEMOPTIONLOG : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	DWORD		dwCharIdx;
	BOOL		bDayOnly;
	char		sSDate[30];
	char		sEDate[30];
	ITEMOPTIONLOG	sLog[100];
};

struct CHEATLOG
{
	DWORD		dwLogIdx;
	DWORD		dwChrIdx;
	DWORD		dwLogType;
	DWORD		dwParam;
	DWORD		dwItemDBIdx;
	DWORD		dwItemIdx;
	char		sLogDate[30];
};

struct TMSG_CHEATLOG : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	DWORD		dwCharIdx;
	BOOL		bDayOnly;
	char		sSDate[30];
	char		sEDate[30];
	CHEATLOG	sLog[100];
};

struct HACKINGLOG
{
	DWORD		dwLogIdx;
	DWORD		dwChrIdx;
	DWORD		dwCheatKind;
	char		sLogDate[30];
};

struct TMSG_HACKINGLOG : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	DWORD		dwCharIdx;
	BOOL		bDayOnly;
	char		sSDate[30];
	char		sEDate[30];
	HACKINGLOG	sLog[100];
};

struct TOOLLOG
{
	DWORD		dwLogIdx;
	DWORD		dwLogType;
	DWORD		dwLogKind;
	DWORD		dwOperIdx;
	char		sOperName[17];
	DWORD		dwTargetIdx;
	char		sTargetName[17];
	DWORD		dwParam1;
	DWORD		dwParam2;
	DWORD		dwParam3;
	DWORD		dwParam4;
	char		sLogDate[30];
};

struct TMSG_TOOLLOG : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	DWORD		dwLogType;
	char		sName[17];
	BOOL		bDayOnly;
	char		sSDate[30];
	char		sEDate[30];
	TOOLLOG		sLog[100];
};

struct TMSG_GAMELOGMONEY : public TMSGBASE
{
	BOOL			bEnd;
	WORD			wCount;
	char			strCharName[17];
	BYTE			bDayOnly;
	char			strStart[30];
	char			strEnd[30];
	GAMELOGMONEY	sLog[100];
};

struct TMSG_CHARACTER_MUGONGINFO : public TMSGBASE
{
	MUGONG_TOTALINFO	m_sMugongInfo;
};

struct TMSG_CHARACTER_MUGONGINFO2 : public TMSGBASE
{
	DWORD			m_dwCount;
	MUGONGBASE		m_MugongBase[5];
};

struct TMSG_CHARACTER_ABILITYINFO : public TMSGBASE
{
	ABILITY_TOTALINFO	sAbility;
};

struct TMSG_ADMIN_INSERTOPERATOR : public TMSGBASE
{
	char	strID[17];
	char	strName[17];
	char	strPass[17];
	WORD	wPower;
};

struct TMSG_CHARACTER_UPDATEBASEINFO : public TMSGBASE
{
	DWORD		CharIdx;
	char		ObjectName[MAX_NAME_LENGTH+1];
	EXPTYPE		ExpPoint;
	LEVELTYPE	Level;
	FAMETYPE	BadFame;
	WORD		wGenGol;
	WORD		wMinChub;
	WORD		wCheRyuk;
	WORD		wSimMek;
	LEVELTYPE	LevelUpPoint;
	MONEYTYPE	Money;
	DWORD		AbilityExp;

//	char		NickName[MAX_NAME_LENGTH+1];
//	char		MunpaName[MAX_MUNPA_NAME+1];
//	BYTE		PositionInMunpa;
//	BYTE		Stage;
//	DWORD		PartyID;
};

struct TMSG_PET_UPDATEBASEINFO : public TMSGBASE
{
	DWORD		dwPetDBIdx;
	WORD		wRest;
	WORD		wAlive;
	WORD		wGrade;
	DWORD		dwFriendShip;
	DWORD		dwStamina;
};


struct TMSG_ADMIN_INSERTIP : public TMSGBASE
{
	char	strIP[17];
};

struct TMSG_ITEM_INSERT : public TMSGBASE
{
	BYTE				bTable;
	DWORD				dwTagetIdx;
	ITEMBASE			Item;
	ITEM_OPTION_INFO	Option;
	
	char				sCharName[17];		// log¸¦ À§ÇØ
};

struct TMSG_ITEM_UPDATE : public TMSGBASE
{
	DWORD				dwCharIdx;
	DWORD				dwDBIdx;
	DWORD				dwOptionIdx;
	ITEMBASE			Item;
	ITEM_OPTION_INFO	Option;
	BYTE				bDeleteOption;

	char				sCharName[17];		// log¸¦ À§ÇØ
};

struct TMSG_ITEM_DELETE : public TMSGBASE
{
	DWORD				dwCharIdx;
	DWORD				dwItemIdx;
	WORD				wItemPosition;
	DWORD				dwDBIdx;
	DWORD				dwOptionIdx;

	char				sCharName[17];		// log¸¦ À§ÇØ
};

struct TMSG_MUGONG_EDIT : public TMSGBASE
{
	DWORD				dwData;
	char				sName[17];
	MUGONGBASE			Mugong;
};

struct TMSG_CHARACTER_UPDATE_ABILITY : public TMSGBASE
{
	DWORD				dwCharIdx;
	char				sName[17];
	ABILITY_TOTALINFO	sAbility;
};

struct GUILDBASEINFO
{
	DWORD	dwGuildIdx;
	char	sGuildName[17];
	DWORD	dwMasterIdx;
	char	sMasterName[17];
	DWORD	dwGuildLevel;
	DWORD	dwGuildMap;
	DWORD	dwUnionIdx;
	DWORD	dwGuildMoney;	
	char	sCreateDate[30];
};

struct TMSG_GUILD : public TMSGBASE
{
	DWORD			dwCount;
	GUILDBASEINFO	GuildInfo[100];

	TMSG_GUILD()
	{
		dwCount = 0;
		memset( GuildInfo, 0, sizeof(GUILDBASEINFO)*100 );
	}
	void	Add( GUILDBASEINFO* pInfo )
	{
		GuildInfo[dwCount] = *pInfo;
		++dwCount;
	}
	int		GetSize()	{ return sizeof(TMSG_GUILD) - sizeof(GUILDBASEINFO)*(100-dwCount); }
};

struct TGUILDMEMBERINFO 
{
	DWORD	dwChrIdx;
	char	sName[17];
	DWORD	dwLevel;
	DWORD	dwPosition;
};

struct TMSG_GUILDMEMBERINFO : public TMSGBASE
{
	DWORD				dwCount;
	TGUILDMEMBERINFO	MemberInfo[100];

	TMSG_GUILDMEMBERINFO()
	{
		dwCount = 0;
		memset( MemberInfo, 0, sizeof(TGUILDMEMBERINFO)*100 );
	}
	void	Add( TGUILDMEMBERINFO* pInfo )
	{
		MemberInfo[dwCount] = *pInfo;
		++dwCount;
	}
	int		GetSize()	{ return sizeof(TMSG_GUILDMEMBERINFO) - sizeof(TGUILDMEMBERINFO)*(100-dwCount); }
};

struct TMSG_ITEMOPTIONINFO2 : public TMSGBASE
{
	DWORD				dwCount;
	ITEM_OPTION_INFO	ItemOptionInfo[100];

	TMSG_ITEMOPTIONINFO2()
	{
		dwCount = 0;
		memset( ItemOptionInfo, 0, sizeof(ITEM_OPTION_INFO)*100 );
	}
	void	Add( ITEM_OPTION_INFO* pInfo )
	{
		ItemOptionInfo[dwCount] = *pInfo;
		++dwCount;
	}
	int		GetSize()	{ return sizeof(TMSG_ITEMOPTIONINFO2) - sizeof(ITEM_OPTION_INFO)*(100-dwCount); }
};

////////////////////////////////////pet
struct PETBASEINFO
{
	DWORD	dwPetDBIdx;
	DWORD	dwMasterIdx;
	DWORD	dwSummonItemDBIdx;
	DWORD	dwKind;
	DWORD	dwGrade;
	DWORD	dwStamina;
	DWORD	dwFriendShip;
	WORD	wAlive;
	WORD	wSummon;
	WORD	wRest;
	char	Loginid[17];
	DWORD	dwCharIdx;
};

struct PETLIST
{
	DWORD	dwPetDBIdx;
	DWORD	dwKind;
	DWORD	dwGrade;
	DWORD	dwStamina;
	DWORD	dwFriendShip;
	DWORD	dwChrIdx;
	char	dwChr_Name[17];
	DWORD	dwMasterIdx;
};

struct USERLOGINCHK
{
	int	UserLoginChk;
};


struct TMSG_USERLOGINCHK : public TMSGBASE
{
	DWORD				dwCount;
	USERLOGINCHK			UserLoginChk[100];

	TMSG_USERLOGINCHK()
	{
		dwCount = 0;
		memset( UserLoginChk, 0, sizeof(USERLOGINCHK)*100 );
	}
	void	Add( USERLOGINCHK* pInfo )
	{
		UserLoginChk[dwCount] = *pInfo;
		++dwCount;
	}
	int		GetSize()	{ return sizeof(TMSG_USERLOGINCHK) - sizeof(TMSG_USERLOGINCHK)*(100-dwCount); }
};

struct TMSG_PETINFO : public TMSGBASE
{
	DWORD				dwCount;
	PETBASEINFO			PetInfo[100];

	TMSG_PETINFO()
	{
		dwCount = 0;
		memset( PetInfo, 0, sizeof(PETBASEINFO)*100 );
	}
	void	Add( PETBASEINFO* pInfo )
	{
		PetInfo[dwCount] = *pInfo;
		++dwCount;
	}
	int		GetSize()	{ return sizeof(TMSG_PETINFO) - sizeof(PETBASEINFO)*(100-dwCount); }
};

struct TMSG_PETBASEINFO : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	PETBASEINFO	sOper[100];
};

struct TMSG_PETLISTRESULT : public TMSGBASE
{
	DWORD Count;
	PETLIST sPetList[100];
	
/*	TMSG_PETLISTRESULT()
	{
		Count = 0;
		memset( sPetList, 0, sizeof(PETLIST)*100 );
	}
	void	Add( PETLIST* pInfo )
	{
		sPetList[Count] = *pInfo;
		++Count;
	}

	int GetMsgSize()	{ return sizeof(TMSG_PETLISTRESULT) - sizeof(TMSG_PETLISTRESULT)*(100-Count); }
	*/
};

struct MAINPETLIST
{
	DWORD	dwPetDBIdx;
	DWORD	dwKind;
	DWORD	dwGrade;
	DWORD	dwStamina;
	DWORD	dwFriendShip;
};

struct TMSG_MAINPETLIST : public TMSGBASE
{
	DWORD			dwCount;
	MAINPETLIST	MainPetList[100];

	DWORD	GetMsgLength()	{ return sizeof(TMSG_MAINPETLIST) - sizeof(MAINPETLIST)*(100-dwCount); }
};


//////////////////////////////////////////////////////////////////////////

//delete character list
struct DELCHARLIST	
{

	DWORD	dwcharacter_idx;			//캐릭터 index
	DWORD	dwuser_idx;					//유저 index
	char	character_name[17];			//캐릭터 명
	WORD	wflag;						//구분(1-생성, 2-삭제, 3-복구)
	char	create_date[11];			//캐릭터생성일자
	char	create_datetime[10];		//캐릭터생성시간
	char	create_ip[16];				//캐릭터생성시아이피
	char	delete_date[11];			//캐릭터삭제일자
	char	delete_datetime[10];		//캐릭터삭제시간
	char	delete_ip[16];				//캐릭터삭제시아이피
	char	recover_date[11];			//캐릭터복구일자
	char	recover_datetime[10];		//캐릭터복구시간
	char	recover_ip[16];				//캐릭터복구시아이피

};

//검색시 날라오는 패킷
struct TMSG_DELCHARLISTSEARCH : public TMSGBASE
{
	WORD	wServer;
	char	delcharname[17];
};

//검색 결과 날라가는 패킷
struct TMSG_DELCHARLISRESULT : public TMSGBASE
{
	DWORD Count;
	DELCHARLIST sDelcharList[100];
};

//////////////////////////////////////////////////////////////////////
//pet log page
struct LOGPET
{
	DWORD		dwIdx; 
	DWORD		dwLogType;
	DWORD		dwPetidx;
	DWORD		dwPetSummonItemDBIdx;
	DWORD		dwUserIdx;
	DWORD		dwCharacterIdx;
	WORD		wGrade;
	WORD		wFriendShip;
	WORD		wStamina;
	WORD		wAlive;
	char		sLogDate[30];
};

struct TMSG_LOGPET : public TMSGBASE
{
	BOOL		bEnd;
	WORD		wCount;
	WORD		wType;
	DWORD		dwCharIdx;
	DWORD		dwItem;
	BOOL		bDayOnly;
	char		sSDate[30];
	char		sEDate[30];
	LOGPET		sLog[100];
};

/// 060831 PKH 문파토너먼트 로그정보
struct sGuildtn
{
	unsigned long	idx; 
	unsigned long	tournamentidx;
	unsigned long	guildidx;
	unsigned short	logkind;
	unsigned short	ranking;
	char			registtime[30];
};

/// 060831 PKH 문파토너먼트 로그정보
struct MSG_LOGGUILDTN : public TMSGBASE
{
	int				end;
	unsigned short	count;
	unsigned short	type;
	unsigned long	guildidx;
	int				dayonly;
	char			startdate[30];
	char			enddate[30];
	sGuildtn		logdata[100];
};

/// 060831 PKH 공성전로그
struct sSiegeWar
{
	unsigned long	idx; 
	unsigned long	siegewaridx;
	unsigned short	mapnum;
	unsigned long	guildidx;
	unsigned short	logkind;
	unsigned long	engraveidx;
	char			registtime[30];
};

/// 060831 PKH 공성전 로그
struct MSG_LOGSIEGEWAR : public TMSGBASE
{
	int				end;
	unsigned short	count;
	unsigned short	type;
	unsigned long	guildidx;
	int				dayonly;
	char			startdate[30];
	char			enddate[30];
	sSiegeWar		logdata[100];
};

/// 060915 PKH GMTOOLLOG
struct sGMLogType
{
	unsigned long	dbidx; 
	unsigned long	character_idx;
	unsigned short	gmlogtype;
	unsigned long	logkind;
	unsigned long	param1;
	unsigned long	param2;
	char			registtime[30];
};

/// 060915 PKH GMTOOLLOG
struct MSG_LOGGMTOOL : public TMSGBASE
{
	int				end;
	unsigned short	count;
	unsigned short	type;
	unsigned long	character_idx;
	unsigned short	gmlogtype;
	int				dayonly;
	char			startdate[30];
	char			enddate[30];
	sGMLogType		logdata[100];
};


// 061108 RaMa -
struct MSG_GUILDMONEYUPDATE : public TMSGBASE
{
	DWORD		GuildIdx;
	DWORD		Money;
};

#pragma pack(pop)

#endif