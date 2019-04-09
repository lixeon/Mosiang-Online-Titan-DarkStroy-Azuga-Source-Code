#if !defined(AFX_LOGETCPAGE_H__5CD42DF7_B752_4B56_A0F1_19C281FB695B__INCLUDED_)
#define AFX_LOGETCPAGE_H__5CD42DF7_B752_4B56_A0F1_19C281FB695B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogEtcPage.h : header file
//
#include "LDPropSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CLogEtcPage dialog
class CLogDoc;

class CLogEtcPage : public CLDPropPage
{
	DECLARE_DYNCREATE(CLogEtcPage)

// Construction
public:
	CLogEtcPage();
	~CLogEtcPage();

	CLogDoc*		m_pDocument;
	DWORD			m_dwCharIdx;
	BOOL			m_bStart;
	CFont			m_Font;

	void	SetDocument( CLogDoc* pDoc )	{ m_pDocument = pDoc; }
	void	SetCharacterIdx( DWORD dwCharIdx );
	void	SetCheatLog( TMSG_CHEATLOG* pMsg );	
	void	SetHackingLog( TMSG_HACKINGLOG* pMsg );	

	CYHHashTable<CHEATLOG>		m_CheatLogTable;
	CYHHashTable<HACKINGLOG>	m_HackingLogTable;
	void	ReleaseCheatLogTable();
	void	ReleaseHackingLogTable();

	void	InitListControl( DWORD dwKind );
	void	UpdateListControl( DWORD dwKind );

	char*	GetItemName( DWORD dwItemIdx, char* temp );


	void	ChangeFont();

// Dialog Data
	//{{AFX_DATA(CLogEtcPage)
	enum { IDD = IDD_LOG_PAGE_ETC };
	CDateTimeCtrl	m_EndTime;
	CDateTimeCtrl	m_StartTime;
	CListCtrl	m_lcEtc;
	CEdit	m_etCharName;
	CEdit	m_etCharIdx;
	BOOL	m_bOneDay;
	int		m_nLogKind;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLogEtcPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLogEtcPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLogSearch();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnBtnSavetofile();
	afx_msg void OnCheckLogOneday();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGETCPAGE_H__5CD42DF7_B752_4B56_A0F1_19C281FB695B__INCLUDED_)
