// PartyWarDialog.h: interface for the CPartyWarDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYWARDIALOG_H__EA488474_EB4B_4B81_B78F_1C187FEBC386__INCLUDED_)
#define AFX_PARTYWARDIALOG_H__EA488474_EB4B_4B81_B78F_1C187FEBC386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"

class cButton;
class cStatic;
class cCheckBox;
class cTextArea;

class CPartyWarDialog : public cDialog    
{
protected:
	cCheckBox*		m_pcbName[14];
	cStatic*		m_psName[14];
	cButton*		m_pbtnLock;
	cButton*		m_pbtnUnLock;
	cButton*		m_pbtnStart;
	cButton*		m_pbtnCancel;
	cTextArea*		m_pTitle;
	cStatic*		m_pTeam1;
	cStatic*		m_pTeam2;
	cStatic*		m_pTime;

	cImage			m_LockImage;

	BOOL			m_bPartyLock;
	BOOL			m_bEnemyLock;

public:
	CPartyWarDialog();
	virtual ~CPartyWarDialog();

	virtual void	Render();

	void	Linking();

	void	ShowPartyWarDlg( BOOL bMaster );
	void	HidePartyWarDlg();
	BOOL	NoChangeCheckBoxState( int nIndex );
	void	SetPartyMemberName( char* pName, int nIndex, BOOL bLogged );
	void	AddPartyWarMember( int nIndex );
	void	RemovePartyWarMember( int nIndex );

	BOOL	IsLock()				{ return m_bPartyLock; }
	void	SetLock( BOOL bParty, BOOL bMaster );
	void	SetUnLock( BOOL bMaster );	

	void	SetTime( DWORD dwTime );

	// test
	void	TestPartyWarDialog();
};

#endif // !defined(AFX_PARTYWARDIALOG_H__EA488474_EB4B_4B81_B78F_1C187FEBC386__INCLUDED_)

