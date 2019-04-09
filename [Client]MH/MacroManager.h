// MacroManager.h: interface for the CMacroManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MACROMANAGER_H__A85EAD4A_B009_45AE_AC28_B00E31856F59__INCLUDED_)
#define AFX_MACROMANAGER_H__A85EAD4A_B009_45AE_AC28_B00E31856F59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MACROMGR USINGTON(CMacroManager)

class CKeyboard;
//---------------------------------------------------------------------------------------
// taiyo 
// 현재는 임시로 단축키를 셋팅했지만,
// 단축키 모듈을 만들어서 추가할 예정!
//---------------------------------------------------------------------------------------
//KES_030806
// 단축키 모듈
//---------------------------------------------------------------------------------------


////////////
//단축키목록
//#define START_CONFLICT_CHAT		ME_USE_QUICKITEM01	//채팅과 충돌되는 단축키.

enum eMacroEvent {	//다이얼로그와 순서를 맞추어야 한다.

//미리 체크해야할것.
	ME_TOGGLE_EXITDLG,
//	ME_TOGGLE_UNGIMODE,		//*****

///////////////////////////
	ME_USE_QUICKITEM01,
	ME_USE_QUICKITEM02,
	ME_USE_QUICKITEM03,
	ME_USE_QUICKITEM04,
	ME_USE_QUICKITEM05,
	ME_USE_QUICKITEM06,
	ME_USE_QUICKITEM07,

	ME_USE_QUICKITEM08,
	ME_USE_QUICKITEM09,
	ME_USE_QUICKITEM10,

	ME_TOGGLE_MUGONGDLG,
	ME_TOGGLE_INVENTORYDLG,
	ME_TOGGLE_CHARACTERDLG,
	ME_TOGGLE_MUNPADLG,
//	ME_TOGGLE_PARTYDLG,

	ME_TOGGLE_MINIMAP,

	ME_TOGGLE_QUESTDLG,
//New
//	ME_APPLY_EXCHANGE,		//*****
//	ME_OPEN_STREETSTALL,	//*****
	ME_TOGGLE_OPTIONDLG,
//	ME_CLOSE_ALLWINDOW,			//*****
	ME_TOGGLE_FRIENDLIST,
	ME_SEND_MEMO,
//	ME_APPLY_VIMU,			//*****
//
//	ME_CLOSE_ACTIVEDLG,	//매크로가 할 필요 없게 되어있다.. 나중에 지우기.
	
	ME_TOGGLE_MOVEMODE,
//	ME_TOGGLE_KYUNGGONG,			//*****

//	ME_PAGEDN_QUICKSLOT,
//	ME_PAGEUP_QUICKSLOT,
	ME_SELECT_QUICKSLOT1,
	ME_SELECT_QUICKSLOT2,
	ME_SELECT_QUICKSLOT3,
	ME_SELECT_QUICKSLOT4,

	ME_TOGGLE_PEACEWARMODE,

	ME_TOGGLE_AUTOATTACK,
	ME_TOGGLE_AUTOATTACK2,

	ME_TOGGLE_HELP,
	
	ME_TOGGLE_CAMERAVIEWMODE,
//	ME_CAMERAMODE_BACKVIEW,
//	ME_CAMERAMODE_QUARTERVIEW,
//	ME_CAMERAMODE_LOADVIEW,
//	ME_CAMERAMODE_SAVEVIEW,

	ME_SCREENCAPTURE,

	ME_SHOWALLNAME_ON,
	ME_SHOWALLNAME_OFF,

	ME_CHANGE_WHISPERLIST_UP,
	ME_CHANGE_WHISPERLIST_DOWN,
	
	ME_TOGGLE_BIGMAP,
	ME_MUSSANG_ON,
	ME_TOGGLE_KEY_SETTING_TIP_SHOW,
	ME_TOGGLE_KEY_SETTING_TIP_HIDE,

	ME_TOGGLE_TITANINVENTORYDLG,	//2007. 10. 8. CBH - 타이탄 인벤 단축키 추가

	ME_COUNT,					//매크로 총 개수
};

enum eMacroPressEvent
{
	MPE_CAMERAMOVE_ZOOMIN,
	MPE_CAMERAMOVE_ZOOMOUT,
	
	MPE_CAMERAMOVE_UP,
	MPE_CAMERAMOVE_DOWN,
	MPE_CAMERAMOVE_RIGHT,
	MPE_CAMERAMOVE_LEFT,
	
	MPE_COUNT,
};

////////
//조합키
enum eSysKey {

	MSK_NONE	= 1,	//
	MSK_CTRL	= 2,
	MSK_ALT		= 4,
	MSK_SHIFT	= 8,
	MSK_ALL		= MSK_NONE | MSK_CTRL | MSK_ALT | MSK_SHIFT,
};

//////////////
//매크로구조체
struct sMACRO
{
	int		nSysKey;	//조합키
	WORD	wKey;		//실행키
	BOOL	bAllMode;	//채팅모드에 관계없이 되어야하는 것체크
	BOOL	bUp;		//up에 이벤트가 발생해야 할것
};


enum eMacroMode {


	MM_CHAT,					//채팅 우선모드
	MM_MACRO,					//단축키 우선모드

//	MM_DEFAULT_CHAT = 0,		//디폴트 채팅우선모드
//	MM_DEFAULT_GENERAL,			//디폴트 일반모드

//	MM_USERSET_CHAT,			//유저셋 채팅우선모드
//	MM_USERSET_GENERAL,			//유저셋 일반모드

	MM_COUNT,
};




class CMacroManager
{

protected:

	sMACRO		m_DefaultKey[MM_COUNT][ME_COUNT];	//디폴트매크로키
	sMACRO		m_MacroKey[MM_COUNT][ME_COUNT];		//유저매크로키

	sMACRO		m_DefaultPressKey[MM_COUNT][MPE_COUNT];		//디폴트매크로프레스키
	sMACRO		m_MacroPressKey[MM_COUNT][MPE_COUNT];		//유저매크로프레스키

	int			m_nMacroMode;					//선택된 매크로 모드
//	int			m_nUserSetMode;					//유저세팅에서 선택된 모드
	BOOL		m_bPause;
	BOOL		m_bChatMode;
	BOOL		m_bInVisibleCursor;

protected:
	BOOL LoadMacro( LPCTSTR strPath );
	BOOL SaveMacro( LPCTSTR strPath );
	void SetDefaultMacro( int nMacroMode );

public:
	CMacroManager();
	virtual ~CMacroManager();

//	//MAKESINGLETON(CMacroManager);
	BOOL IsVisibleCursor() { return !m_bInVisibleCursor; }

	void Init();
	void KeyboardInput( CKeyboard* keyInfo );
	void KeyboardPressInput( CKeyboard* keyInfo );
	void LoadUserMacro();
	void SaveUserMacro();

	void GetMacro( int nMacroMode, sMACRO* pMacro );		//전체 매크로 얻어옴
	void SetMacro( int nMacroMode, sMACRO* pMacro );		//전테 매크로 세팅

	sMACRO* GetCurMacroKey( int nMacroEvent )	//특정 매크로키 하나 얻어옴
	{ 
		if( nMacroEvent >= ME_COUNT )
			return NULL;
		
		return &m_DefaultKey[m_nMacroMode][nMacroEvent];	//차후에 바꾸자.userset잊지마시오.
	}
	
	void GetDefaultMacro( int nMacroMode, sMACRO* pMacro );

	int GetMacroMode() { return m_nMacroMode; }
	void SetMacroMode( int nMacroMode );

//	int GetUserSetMode() { return m_nUserSetMode; }
//	void SetUserSetMode( int nUserSetMode ) { m_nUserSetMode = nUserSetMode; }

	void SetPause( BOOL bPause ) { m_bPause = bPause; }
	int IsUsableKey( CKeyboard* keyInfo, WORD* wKey, BOOL bUserMode );
	BOOL IsChatMode()	{ return m_bChatMode; }

	void PlayMacro( int nMacroEvent );
	void PlayMacroPress( int nMacroEvent );
/////
};

EXTERNGLOBALTON(CMacroManager)

#endif // !defined(AFX_MACROMANAGER_H__A85EAD4A_B009_45AE_AC28_B00E31856F59__INCLUDED_)
