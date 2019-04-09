// GuildFieldWarDialog.h: interface for the CGuildFieldWarDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDFIELDWARDIALOG_H__AC719F6A_AD6A_4CE4_B9BF_F134B119BABF__INCLUDED_)
#define AFX_GUILDFIELDWARDIALOG_H__AC719F6A_AD6A_4CE4_B9BF_F134B119BABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"

class cTextArea;
class cEditBox;
class cButton;
class cStatic;
class cListDialog;
class cCheckBox;

class CGFWarDeclareDlg : public cDialog  
{
protected:	
	cTextArea*	m_pMsg;
	cEditBox*	m_pEditBox;
	cTextArea*	m_pMsg1;
	cTextArea*	m_pMsg2;
	cTextArea*	m_pMsg3;
	cCheckBox*	m_pMoneyCheck;
	cEditBox*	m_pMoneyEdit;
	cStatic*	m_pMoneyStatic;
	cButton*	m_pOkBtn;
	cButton*	m_pCancelBtn;
	
	BOOL		m_bRender;
	cImage		m_LockImage;

public:
	CGFWarDeclareDlg();
	virtual ~CGFWarDeclareDlg();

	virtual void	SetActive( BOOL val );
	virtual void	Render();
	void			Linking();
	void			ShowMoneyEdit();
	BOOL			IsChecked();
};

class CGFWarResultDlg : public cDialog  
{
protected:	
	cStatic*	m_pTitle;
	cTextArea*	m_pMsg;
	cStatic*	m_pStatic0;
	cStatic*	m_pStatic1;
	cStatic*	m_pStatic2;
	cStatic*	m_pStatic3;
	cStatic*	m_pStatic4;
	cStatic*	m_pStatic5;
	cButton*	m_pOkBtn;
	cButton*	m_pCancelBtn;
	cButton*	m_pConfirmBtn;

public:
	CGFWarResultDlg();
	virtual ~CGFWarResultDlg();

	void	Linking();
	void	ShowDeclare( GUILDINFO* pInfo );
	void	ShowResult( DWORD dwKind );
};

class CGFWarInfoDlg : public cDialog  
{
protected:	
	cStatic*	m_pTitle;
	cStatic*	m_pMsg;
	cListDialog*	m_pGuildListDlg;
	cTextArea*	m_pText;
	cStatic*	m_pStatic0;
	cStatic*	m_pStatic1;
	cStatic*	m_pStatic2;
	cButton*	m_pSuggestOkBtn;
	cButton*	m_pSurrendOkBtn;
	cButton*	m_pGuildUnionRemoveOkBtn;
	cButton*	m_pCancelBtn;

	int			m_nSelectedIdx;

public:
	CGFWarInfoDlg();
	virtual ~CGFWarInfoDlg();

	virtual DWORD	ActionEvent( CMouse* mouseInfo );

	void	Linking();
	void	ShowSuggest();
	void	ShowSurrend();
	void	ShowGuildUnion();
	void	AddGuildInfoToList();
	void	AddGuildUnionInfoToList();
	int		GetSelectedListIdx()		{ return m_nSelectedIdx; }
};

class CGuildWarInfoDlg : public cDialog
{
protected:
	cListDialog*	m_pBlackGuildListDlg;
	cListDialog*	m_pWhiteGuildListDlg;
	cStatic*		m_pStatic;
	cButton*		m_pCloseBtn;
	
public:
	CGuildWarInfoDlg();
	virtual ~CGuildWarInfoDlg();

	virtual void	SetActive( BOOL val );
	
	void	Linking();
	void	AddListData();
};


#endif // !defined(AFX_GUILDFIELDWARDIALOG_H__AC719F6A_AD6A_4CE4_B9BF_F134B119BABF__INCLUDED_)
