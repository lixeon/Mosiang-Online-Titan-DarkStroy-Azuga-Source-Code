#if !defined(AFX_LOGVIEW_H__1053EEE2_E9E8_4996_825A_5DC8D6818FD2__INCLUDED_)
#define AFX_LOGVIEW_H__1053EEE2_E9E8_4996_825A_5DC8D6818FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ProgressDlg.h"

class CLDPropPage;
class CLDPropSheet;
class CLogDoc;

enum eLogPage
{
	 eItemMoney_Page = 0, eTotal_Page, eEtc_Page, ePageMax, eCharacter_Page, eSkill_Page, eAbility_Page,
};

class CLogView : public CFormView
{
protected:
	CLogView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLogView)

// Form Data
public:
	//{{AFX_DATA(CLogView)
	enum { IDD = IDD_LOG_SHEET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CLDPropSheet*	m_pSheet;
	CLDPropPage*	m_pPageList[ePageMax];

	CProgressDlg*	m_pProgressDlg;
	void	StartDlg();
	void	EndDlg();

// Operations
public:
	void	SetItemMoneyLogCharacterIdx( DWORD dwCharIdx );
	void	SetTotalLogChacterIdx( DWORD dwCharIdx );
	void	SetEtcLogChacterIdx( DWORD dwCharIdx );
	void	SetLogItemMoney( TMSG_LOGITEMMONEY* pMsg );
	void	SetCharacterLog( TMSG_CHARACTERLOG* pMsg );
	void	SetExpPointLog( TMSG_EXPPOINTLOG* pMsg );
	void	SetMugongLog( TMSG_MUGONGLOG* pMsg );
	void	SetMugongExpLog( TMSG_MUGONGEXPLOG* pMsg );
	void	SetMoneyLog( TMSG_MONEYLOG* pMsg );
	void	SetMoneyWrongLog( TMSG_MONEYWRONGLOG* pMsg );
	void	SetAbilityLog( TMSG_ABILITYLOG* pMsg );	
	void	SetCheatLog( TMSG_CHEATLOG* pMsg );	
	void	SetHackingLog( TMSG_HACKINGLOG* pMsg );	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLogView();
	CLogDoc*	GetDocument();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CLogView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGVIEW_H__1053EEE2_E9E8_4996_825A_5DC8D6818FD2__INCLUDED_)
