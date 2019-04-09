// GMToolManager.h: interface for the CGMToolManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMTOOLMANAGER_H__0388E03B_98FF_4024_98A4_5E8C67D7DBFD__INCLUDED_)
#define AFX_GMTOOLMANAGER_H__0388E03B_98FF_4024_98A4_5E8C67D7DBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef _GMTOOL_

#define GMTOOLMGR USINGTON(CGMToolManager)


enum eGM_MENU
{
	eMenu_Move,
	eMenu_Where,
	eMenu_Item,
	eMenu_Hide,
	eMenu_Chat,
	eMenu_PK,
	eMenu_Discon,
	eMenu_Block,
	
	eMenu_Mob,
	eMenu_Counsel,
	eMenu_Notice,
	
	eMenu_Event,
	eMenu_EventMap,
	eMenu_EventNotify,
	eMenu_Weather,
	

	eMenu_Count,
};


enum eGM_PROHIBIT_KIND
{
	eGM_Forbid_Connect,
	eGM_Forbid_Chat,
	eGM_Forbid_PK,
};


class CGMToolManager  
{
protected:
	HWND	m_hWndDlg;

	HWND	m_hWndSub[eMenu_Count];

	int		m_nSelectMenu;
	BOOL	m_bShow;
	BOOL	m_bCanUse;
	
	BOOL	m_bSubShow;
	BYTE	m_cbChannelCount;

//for BossMob
	LONG	m_lBossStartIndex;

//chat counsel
	HWND	m_hWndChatList;
	int		m_nNumNow;
	int		m_nNumWait;
	char	m_strChatList[1024*16];//16384
	int		m_nBufLen;
//gm power
	HWND	m_hWndLoginDlg;
	BOOL	m_bLogin;
	int		m_nPower;
	
//임시
public:
	int		m_nNullMove;
public:

	CGMToolManager();
	virtual ~CGMToolManager();
//
	void Login( BOOL bLogin, int nGMPower );
//
	void SetChannelCount( BYTE cbChannelCount ) { m_cbChannelCount = cbChannelCount; }
	void SetChatListHwnd( HWND hWnd ) { m_hWndChatList = hWnd; }

	BOOL CreateGMDialog();
	BOOL DestroyGMDialog();
	BOOL CreateGMSubDialog();
	BOOL DestroyGMSubDialog();

	BOOL IsGMDialogMessage( LPMSG pMessage );

	void ShowGMDialog( BOOL bShow, BOOL bUpdate = FALSE );
	BOOL IsShowing()	{ return m_bShow; }

	BOOL IsSubShow()	{ return m_bSubShow; }

	void CanUse( BOOL bUse ) { m_bCanUse = bUse; }
	
	void SetPositionByMainWindow();
	void SetPositionSubDlg();

	void OnClickMenu( int nMenu );

//---
	BOOL OnGMMoveCommand( int nMethod );	
	BOOL OnUserMoveCommand( int nMethod );
	BOOL OnWhereCommand();
	BOOL OnItemCommand();
	BOOL OnBlockCommand();
	BOOL OnHideCommand();
	BOOL OnChatCommand();
	BOOL OnDisconCommand();
	BOOL OnPKCommand();

	BOOL OnRegenCommand();
	BOOL OnNoticeCommand();

	BOOL OnEventCommand();
	BOOL OnEventMapCommand( int nKind );
	BOOL OnEventNotifyCommand( BOOL bOn );
	
	BOOL OnWeatherApplyCommand();
	BOOL OnWeatherExecutionCommand();
	BOOL OnWeatherReturnCommand();

//
	void RemoveIdFromList();
	void CaptureChat( char* FileName );
	void SaveChatList();
	void AddChatMsg( char* pName, char* pMsg, int nKind );
//
	void DisplayWhereInfo( char* MapName, int nChannel, int nX, int nY );

};

INT_PTR CALLBACK GMLoginDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK GMDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK GMSubMoveDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK GMSubWhereDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK GMSubItemDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK GMSubHideDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK GMSubChatDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK GMSubBlockDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK GMSubDisconDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK GMSubPKDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

INT_PTR CALLBACK GMSubRegenDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK GMSubNoticeDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK GMSubCounselDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK GMCounselEditProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK GMCounselChatProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

INT_PTR CALLBACK GMSubEventDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK GMSubEventMapDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK GMSubEventNotifyDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

// 2005 크리스마스 이벤트
INT_PTR CALLBACK GMSubWeatherDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

EXTERNGLOBALTON(CGMToolManager)


#endif //_GMTOOL_

#endif // !defined(AFX_GMTOOLMANAGER_H__0388E03B_98FF_4024_98A4_5E8C67D7DBFD__INCLUDED_)
