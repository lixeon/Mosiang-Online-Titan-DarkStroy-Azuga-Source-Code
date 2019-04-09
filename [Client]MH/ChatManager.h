#ifndef _CHATMANAGER_H
#define _CHATMANAGER_H


#define CHATMGR USINGTON(CChatManager)

#define MAX_CHATMSGNUM	2000

class CKeyboard;

#include "ChatDialog.h"

/*
enum eChatKind
{
	eCK_ALL,
	eCK_PARTY,
	eCK_MUNPA,
	eCK_WHISPER,
};
*/
enum eTextClass
{
	CTC_OPERATOR,		//운영자메세지		//운영자
	CTC_GENERALCHAT,	//일반채팅			//일반
	CTC_PARTYCHAT,		//파티채팅			//파티
	CTC_GUILDCHAT,		//문파채팅			//문파
	CTC_ALLIANCE,		//동맹
	CTC_SHOUT,		//외치기
//050630SW
	CTC_TIPS,			//w팁
//-
	CTC_WHISPER,		//귓말				//귓말
	
	//SW050902
	CTC_MONSTERSPEECH,
	CTC_MONSTERSHOUT,

	CTC_ATTACK,			//공격				//공격/방어
	CTC_ATTACKED,		//상대가 공격
	CTC_DEFENCE,		//방어
	CTC_DEFENCED,		//상대가 방어
	CTC_KILLED,			//죽음
	CTC_MPWARNING,		//내공경고			//내공/생명력 경고
	CTC_HPWARNING,		//생명력경고

	CTC_GETITEM,		//아이템획득		//아이템/돈 획득
	CTC_GETMONEY,		//돈획득
	CTC_GETEXP,
	CTC_GETABILITYEXP,
	
	CTC_ABILITYUPGRADE_SUCCESS,
	CTC_ABILITYUPGRADE_FAILED,

	CTC_SYSMSG,			//시스템 메시지
	CTC_TOGM,			//GM알림메세지

	CTC_TOWHOLE,
	CTC_TOPARTY,
	CTC_TOGUILD,

	CTC_CHEAT_1,
	CTC_CHEAT_2,
	CTC_CHEAT_3,
	CTC_CHEAT_4,
	CTC_CHEAT_5,

	CTC_QUEST,
	CTC_GMCHAT,
	CTC_BILLING,	
	CTC_TRACKING,
	CTC_GTOURNAMENT,
	CTC_JACKPOT,
	CTC_GUILD_NOTICE,
	CTC_SURVIVAL,
	CTC_ALERT_YELLOW,
	CTC_ALERT_RED,

	CTC_COUNT,	
};


const DWORD dwChatTextColor[CTC_COUNT] = {

	RGBA_MAKE(0, 255, 60, 255),		//CTC_OPERATOR
	RGBA_MAKE(255, 255, 255, 255),	//CTC_GENERAL
	RGBA_MAKE(185, 241, 69, 255),	//CTC_PARTYCHAT
//	RGBA_MAKE(240, 255, 0, 255),		//CTC_GUILDCHAT
	RGBA_MAKE(1, 209, 247, 255),		//CTC_GUILDCHAT
	RGBA_MAKE(240, 255, 0, 255),		//CTC_ALLIANCE
	RGBA_MAKE(247, 182, 24, 255),		//CTC_SHOUT
//050630SW
	RGBA_MAKE(2, 255, 163, 255),		//CTC_TIPS
//-
//	RGBA_MAKE(235, 178, 206, 255),		//CTC_WHISPER
	RGBA_MAKE(235, 195, 206, 255),		//CTC_WHISPER

	RGBA_MAKE(255, 255, 255, 255),		//CTC_MONSTERSPEECH
	RGBA_MAKE(247, 182, 24, 255),		//CTC_MONSTERSHOUT
	
	RGBA_MAKE(255, 97, 19, 255),		//CTC_ATTACK
	RGBA_MAKE(255, 97, 19, 255),		//CTC_ATTACKED
	RGBA_MAKE(255, 97, 19, 255),		//CTC_DEFENCE
	RGBA_MAKE(255, 97, 19, 255),		//CTC_DEFENCED
	RGBA_MAKE(255, 97, 19, 255),		//CTC_KILLED

	RGBA_MAKE(255, 52, 19, 255),		//CTC_MPWARNING
	RGBA_MAKE(255, 52, 19, 255),		//CTC_HPWARNING

	RGBA_MAKE(2, 255, 163, 255),		//CTC_GETITEM
//	RGBA_MAKE(132, 178, 208, 255),		//CTC_GETMONEY
//	RGBA_MAKE(132, 178, 208, 255),		//CTC_GETEXP
//	RGBA_MAKE(132, 178, 208, 255),		//CTC_GETABILITYEXP
	RGBA_MAKE(200, 200, 200, 255),		//CTC_GETMONEY
	RGBA_MAKE(200, 200, 200, 255),		//CTC_GETEXP
	RGBA_MAKE(200, 200, 200, 255),		//CTC_GETABILITYEXP

	RGBA_MAKE(82, 255, 204, 255),		//CTC_ABILITYUPGRADE_SUCCESS,
	RGBA_MAKE(82, 255, 204, 255),		//CTC_ABILITYUPGRADE_FAILED,

	RGBA_MAKE(82, 255, 204, 255),		//CTC_SYSMSG
	RGBA_MAKE(82, 255, 204, 255),		//CTC_TOGM

	RGBA_MAKE(171, 195, 255, 255),		//CTC_TOWHOLE
	RGBA_MAKE(255, 158, 6, 255),		//CTC_TOPARTY
	RGBA_MAKE(240, 255, 0, 255),		//CTC_TOGUILD

	RGBA_MAKE(255, 0, 0, 255),		//CTC_CHEAT_1
	RGBA_MAKE(255, 255, 0, 255),	//CTC_CHEAT_2
	RGBA_MAKE(255, 0, 255, 255),	//CTC_CHEAT_3
	RGBA_MAKE(0, 255, 255, 255),	//CTC_CHEAT_4
	RGBA_MAKE(100, 50, 100, 255),	//CTC_CHEAT_5

	RGBA_MAKE( 0, 255, 255, 255 ),	//CTC_QUEST
#ifdef _HK_LOCAL_
	RGBA_MAKE( 255, 0, 50, 255 ),	//CTC_GMCHAT
#else
	RGBA_MAKE( 28, 233, 151, 255 ),	//CTC_GMCHAT
#endif
	RGBA_MAKE(0, 255, 60, 255),	//CTC_BILLING

	RGBA_MAKE(255, 20, 20, 255),		//CTC_TRACKING
	RGBA_MAKE(20, 255, 60, 255),		//CTC_GTOURNAMENT
	RGBA_MAKE(144, 252, 222, 255),		//CTC_JACKPOT
	// 06. 03. 문파공지 - 이영준
	RGBA_MAKE(255, 214, 0, 255),		//CTC_GUILD_NOTICE
	RGBA_MAKE(144, 252, 222, 255),		//CTC_SURVIVAL
	RGBA_MAKE(255, 252, 0, 255),		//CTC_ALERT_YELLOW
};



enum eChatOption
{
//	CTO_PARTYTOWHOLE,
//	CTO_MUNPATOWHOLE,
//	CTO_NOWHISPER,

//	CTO_ATTACKED,
//	CTO_KILLED,
//	CTO_ATTACK,
//	CTO_DEFENCE,
//	CTO_DEFENCED,
	
//	CTO_MPWARNING,
//	CTO_HPWARNING,
	CTO_NOSYSMSG,
	CTO_NOITEMMSG,
	CTO_NOEXPMSG,
	CTO_NOCHATMSG,
	CTO_AUTOHIDECHAT,
	CTO_NOSHOUTMSG,
	CTO_NOGUILDMSG,
	CTO_NOALLIANCEMSG,	
	CTO_COUNT,
};


//dword bit option으로 바꾸자
struct sChatOption
{
	BOOL bOption[CTO_COUNT];
};


//채팅동작
struct sSOCIETY_CHAT
{
	int nActNum;
	int nLen;
	char buf[MAX_NAME_LENGTH+1];

};

//특기
struct sABILITY_CHAT
{
	int nAbilityIndex;
	char buf[MAX_NAME_LENGTH+1];
};

#define MAX_WHISPERLIST_PER_SIDE 2



enum eCHAT_DELAY
{
	eCHATDELAY_PARTY,
	eCHATDELAY_GUILD,
	eCHATDELAY_UNION,
	eCHATDELAY_SHOUT,
	eCHATDELAY_WHISPER,
	eCHATDELAY_MAX,
};

class CUserCommendParser;

class CChatManager
{

protected:

	BOOL			m_bPause;
	sChatOption		m_ChatOption;
	CChatDialog*	m_pChatDialog;
	CUserCommendParser* m_pUserCommendParser;

	char*			m_pChatMsg[MAX_CHATMSGNUM];
	char			m_NULL;	//채팅 메세지가 없는 번호일경우 리턴하기위한 템프멤버

//---
	char			m_strLastName[MAX_NAME_LENGTH+2];
	
//----
	cPtrList		m_ListChat[MAX_CHAT_COUNT];
//----
	
	DWORD			m_dwLastChatTime;	//모든 채팅(채팅간전환포함)
	DWORD			m_dwLastChatDelayTime[eCHATDELAY_MAX];	//채팅종류마다 고유의 딜레이

//채팅동작
	cPtrList		m_ListAct;
	cPtrList		m_ListAbility;	

//귓말리스트
	char			m_WhisperList[MAX_WHISPERLIST_PER_SIDE*2+1][MAX_NAME_LENGTH+1];
	int				m_nWhisperListNum;
protected:

	//채팅동작
	void LoadSocietyActionFilter();
	int CaptureSocietyAction( char* pStr );

	void LoadAbilityFilter();

	void LoadChatMsg();

//	BOOL LoadOption( LPCTSTR strPath );
//	BOOL SaveOption( LPCTSTR strPath );
//	void SetDefaultOption();
	
	void PrintMsg( int nClass, char* msg );

//------
	void AddWhisperSenderList( char* pName );
	void AddWhisperReceiverList( char* pName );
	void RemoveWhisperSenderList( char* pName );
	void RemoveWhisperReceiverList( char* pName );

public:

	void ChangeWhisperUser( BOOL bUp );

	CChatManager();
	virtual ~CChatManager();

	////MAKESINGLETON(CChatManager);

	void Init();
	void Release();
//	void Process( CKeyboard* keyInfo );
	void MsgProc( UINT msg, WPARAM wParam );
	
//	void LoadUserOption();
//	void SaveUserOption();

	void SetOption( sChatOption* pChatOption );
	sChatOption* GetOption()					{ return &m_ChatOption; }

//	void AddMsg( int nClass, int nMsgNum, ... );

//	CTC_OPERATOR, CTC_GENERALCHAT, CTC_PARTYCHAT, CTC_GUILDCHAT, CTC_WHISPER, CTC_ATTACK, CTC_ATTACKED, CTC_DEFENCE, CTC_DEFENCED, CTC_KILLED, CTC_MPWARNING, CTC_HPWARNING, CTC_GETITEM, CTC_GETMONEY, CTC_SYSMSG, CTC_TOWHOLE, CTC_TOPARTY, CTC_TOGUILD,
//	void AddMsg( int nClass, int PredefinedMsg, ... );
	void AddMsg( int nClass, char* str, ... );

	char* GetChatMsg( int nMsgNum );

	void NetworkMsgParse( BYTE Protocol, void* pMsg );

	void SetPause( BOOL bPause ) { m_bPause = bPause; }
	void SetChatDialog( CChatDialog* pChatDlg );	//윈도메니져가 dlg만들때 세팅해줄것이다.
	
	void OnInputChatMsg( char* str );
//	void ChatMsgParse( char* str );

	void SetAutoWhisper();

	void AddChatMsg( BYTE ChatLimit, DWORD MsgColor, char* str );
	void AddChatMsgAll( DWORD MsgColor, char* str );

	BOOL CanChatTime( char cToken );

//---------
	void SaveChatList();
	void RestoreChatList();
	void ReleaseChatList();

	void AddMsgToList( BYTE ChatLimit, DWORD MsgColor, char* str );
	void AddMsgAllToList( DWORD MsgColor, char* str );
//----------
	
};
EXTERNGLOBALTON(CChatManager)

BOOL TargetFunc(char *str);
#endif
