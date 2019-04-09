// ChatDialog.h: interface for the CChatDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATDIALOG_H__0EB53C8B_A556_4FC0_9D50_99DA6BCEC48B__INCLUDED_)
#define AFX_CHATDIALOG_H__0EB53C8B_A556_4FC0_9D50_99DA6BCEC48B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class cEditBox;
class cPushupButton;
class cListDialog;

enum 
{
	WHOLE,
	PARTY,
	GUILD,
	ALLIANCE,
	SHOUT,
	MAX_CHAT_COUNT,
};

#define CHATLIST_TEXTLEN	65
#ifdef _TL_LOCAL_
#define CHATLIST_TEXTEXTENT	390
#endif

class CChatDialog  : public cDialog
{

protected:

	cEditBox*		m_pChatEditBox;

	cListDialog*	m_pSheet[MAX_CHAT_COUNT];
	cPushupButton*	m_pPBMenu[MAX_CHAT_COUNT];

	int				m_nCurSheetNum;

//	LONG			m_lMenuX;
//	LONG			m_lMenuY;
//	LONG			m_lMenuHeight;
	
//	BOOL			m_bShowMenu;
	BOOL			m_bHideChatDialog;

	char			m_cPreWord[MAX_CHAT_COUNT];
	
	BOOL			m_bShowGuildTab;
	cPushupButton*	m_pAllShout;
	
	char			m_SelectedName[MAX_NAME_LENGTH+1];
public:

	CChatDialog();
	virtual ~CChatDialog();

	void Linking();
	virtual DWORD ActionEvent( CMouse * mouseInfo );
	void AddMsg( BYTE ChatLimit, DWORD MsgColor, char* str );
	void AddMsgAll( DWORD MsgColor, char* str );
	void OnActionEvent(LONG lId, void * p, DWORD we);

	cEditBox* GetChatEditBox() { return m_pChatEditBox;	}

	cListDialog* GetSheet( int nSheet ) { return m_pSheet[nSheet]; }
	int	GetCurSheetNum()				{ return m_nCurSheetNum; }
	BYTE GetLineNum();

//	void ShowMenu( BOOL bShow );
	void SelectMenu( int nSheet );
	void SetEditBoxPreWord();

	BOOL IsPreWord( char c );

	void HideChatDialog( BOOL bHide );

	void ShowGuildTab( BOOL bShow );
	
	//
	WORD GetSheetPosY();
	WORD GetSheetHeight();
	
	//
	void SetAllShoutBtnPushed( BOOL val );

	//SW070720 대화창 이름 선택
	void GetSelectedName(CMouse* mouseInfo);
};



/*
class CChatDialog  : public cTabDialog
{
public:
	CChatDialog();
	virtual ~CChatDialog();
	virtual void Add(cWindow * window);
	void Linking();
	
	void AddMsg( BYTE ChatLimit, DWORD MsgColor, char* str );
	void OnActionEvent(LONG lId, void * p, DWORD we);

	cEditBox* GetChatEditBox() { return m_pChatEditBox;	}

protected:

	cEditBox*		m_pChatEditBox;

};
*/


#endif // !defined(AFX_CHATDIALOG_H__0EB53C8B_A556_4FC0_9D50_99DA6BCEC48B__INCLUDED_)
