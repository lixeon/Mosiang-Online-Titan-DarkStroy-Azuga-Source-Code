#if !defined(AFX_LOGITEMMONEYPAGE_H__1A5D9DC6_8564_4132_9B14_8F9DABF28FC8__INCLUDED_)
#define AFX_LOGITEMMONEYPAGE_H__1A5D9DC6_8564_4132_9B14_8F9DABF28FC8__INCLUDED_

#include "ViewerOptionDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogItemMoneyPage.h : header file
//
#include "LDPropSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CLogItemMoneyPage dialog

class CLogDoc;

class CLogItemMoneyPage : public CLDPropPage
{
	DECLARE_DYNCREATE(CLogItemMoneyPage)

// Construction
public:
	CLogItemMoneyPage();
	~CLogItemMoneyPage();

	CYHHashTable<LOGITEMMONEY>	m_LogTable;
	CLogDoc*			m_pDocument;
	BOOL				m_bStart;
	CViewerOptionDlg	m_ViewerOptionDlg;
	BOOL				m_bVOption[14];
	DWORD				m_dwCharIdx;

	void	ReleaseLogTable();
	void	SetDocument( CLogDoc* pDoc )	{ m_pDocument = pDoc; }
	void	SetLogItemMoney( TMSG_LOGITEMMONEY* pMsg );
	void	SetCharacterIdx( DWORD dwCharIdx );
	void	InitComboItemName();
	void	InitComboViewerHow();
	void	InitLCItemMoneyLog();
	void	UpdateLCItemMoneyLog( DWORD dwHow );
	char*	GetItemName( DWORD dwItemIdx, char* temp );
	char*	GetLogType( DWORD dwLogType, char* temp );
	char*	GetName( DWORD dwLogType, DWORD dwIdx, char* temp );
	char*	GetLCData( LOGITEMMONEY* pData, DWORD dwIndex, char* temp );

	virtual void CommandMessage(UINT nID, int nCode);

	CFont	m_Font;


	void	ChangeFont();

// Dialog Data
	//{{AFX_DATA(CLogItemMoneyPage)
	enum { IDD = IDD_LOG_PAGE_ITEMMONEY };
	CDateTimeCtrl	m_StartTime;
	CDateTimeCtrl	m_EndTime;
	CEdit	m_etCharIdx;
	CButton	m_btnViewerOption;
	CComboBox	m_cbViewerHow;
	CComboBox	m_cbItemName;
	CEdit		m_etDBIdx;
	CEdit		m_etCharName;
	CListCtrl	m_lcItemMoneyLog;
	BOOL		m_bCharName;
	BOOL		m_bItemName;
	BOOL		m_bDBIdx;
	BOOL	m_bOneDay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLogItemMoneyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLogItemMoneyPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckLogCharname();
	afx_msg void OnCheckLogItemname();
	afx_msg void OnCheckLogDbidx();
	afx_msg void OnButtonLogSearch();
	afx_msg void OnCheckLogOneday();
	afx_msg void OnSelchangeComboViewerHow();
	afx_msg void OnButtonVieweroption();
	afx_msg void OnBtnSavetofile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGITEMMONEYPAGE_H__1A5D9DC6_8564_4132_9B14_8F9DABF28FC8__INCLUDED_)
