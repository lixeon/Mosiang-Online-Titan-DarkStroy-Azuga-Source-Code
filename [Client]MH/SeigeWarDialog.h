// SeigeWarDialog.h: interface for the CSeigeWarDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEIGEWARDIALOG_H__13DCA43D_EDE6_4C08_89E0_D6C1944111C8__INCLUDED_)
#define AFX_SEIGEWARDIALOG_H__13DCA43D_EDE6_4C08_89E0_D6C1944111C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

#define ENGRAVETIME		60


class cTextArea;
class cStatic;
class cButton;
class cListDialog;
class cEditBox;
class CObjectGuagen;


class CSWTimeRegDlg : public cDialog
{
protected:
	cTextArea*	m_pText;
	cEditBox*	m_pYearEdit;
	cEditBox*	m_pMonEdit;
	cEditBox*	m_pDayEdit;
	cEditBox*	m_pHourEdit;
	cButton*	m_pRegistBtn;

public:
	CSWTimeRegDlg();
	virtual ~CSWTimeRegDlg();

	virtual void	SetActive( BOOL val );
	void			Linking();
};


class CSWProtectRegDlg : public cDialog
{
protected:
	cListDialog*	m_pListDlg;
	cButton*		m_pRegBtn;
	int				m_nSelectedIdx;

	SIEGEWAR_GUILDSENDINFO	m_GuildList[SIEGEWAR_MAXGUILDCOUNT_PERTEAM];

public:
	CSWProtectRegDlg();
	virtual ~CSWProtectRegDlg();

	virtual DWORD	ActionEvent( CMouse* mouseInfo );
	virtual void	SetActive( BOOL val );	

	void			Linking();
	void			ShowList( SEND_SW_PROPOSALGUILDLIST* pMsg );
	int				GetSelectedListIdx()	{ return m_nSelectedIdx; }
	char*			GetSelectedGuildName( int nIdx );
	DWORD			GetSelectedGuildIdx();
	
	void			Test();
};


class CSWInfoDlg : public cDialog
{
protected:
	cListDialog*	m_pDefList;
	cListDialog*	m_pOffList;
	cButton*		m_pCloseBtn;

public:
	CSWInfoDlg();
	virtual ~CSWInfoDlg();

	virtual void	SetActive( BOOL val );
	void			Linking();
	void			ShowSWInfo( SEND_SW_GUILDLIST* pMsg );
};


class CSWProfitDlg : public cDialog
{
protected:
	cStatic*	m_pstBasicTax;
	cEditBox*	m_pChangeTaxEdit;
	cStatic*	m_pstBasicMoney;
	cEditBox*	m_pOutMoneyEdit;
	cButton*	m_pTaxChangeOkBtn;
	cButton*	m_pProfitOkBtn;
	cButton*	m_pCancelBtn;
	
public:
	CSWProfitDlg();
	virtual ~CSWProfitDlg();

	virtual void	SetActive( BOOL val );
	void			Linking();
	void			ShowProfitDlg( int nTaxRate, DWORD dwMoney );
};


class CSWEngraveDialog : public cDialog
{
protected:
	CObjectGuagen*	m_pEngraveGuagen;
	cStatic*		m_pRemaintimeStatic;
	
	BOOL		m_bEngrave;
	float		m_ProcessTime;
	DWORD		m_BackupTime;

public:
	CSWEngraveDialog();
	virtual ~CSWEngraveDialog();

	virtual void SetActive( BOOL val );
	void Linking();
	void Process();

	void StartEngrave();
	void CancelEngraveSyn();
	void CancelEngrave();
};


class CSWTimeDialog : public cDialog
{
	cStatic*		m_pTimeStatic;
	cStatic*		m_pMunpaName;

	DWORD			m_FightTime;
	BOOL			m_bStartBattle;

public:
	CSWTimeDialog();
	virtual ~CSWTimeDialog();

	void Linking();
	void Process();

	void SetMunpaName( char* strName );
	void StartTime( DWORD Time );
	void EndTime();
};

class CSWStartTimeDialog : public cDialog
{
	cStatic*	m_pTime0;		// 공성시간
	cStatic*	m_pTime1;		// 신청시간시작
	cStatic*	m_pTime2;		// 신청시간끝
	cStatic*	m_pTime3;		// 수락시간시작
	cStatic*	m_pTime4;		// 수락시간끝
	cButton*	m_pOkBtn;

public:
	CSWStartTimeDialog();
	virtual ~CSWStartTimeDialog();

	void	Linking();
	void	ShowStartTimeDlg( DWORD dwTime0, DWORD dwTime1 );

	virtual void	SetActive( BOOL val );

	void	Test();
};

#endif // !defined(AFX_SEIGEWARDIALOG_H__13DCA43D_EDE6_4C08_89E0_D6C1944111C8__INCLUDED_)
