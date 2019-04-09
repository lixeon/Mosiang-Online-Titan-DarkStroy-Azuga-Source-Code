#if !defined(AFX_LOGOPERPAGE_H__655BB61D_E115_4705_A9CE_28B2C007652C__INCLUDED_)
#define AFX_LOGOPERPAGE_H__655BB61D_E115_4705_A9CE_28B2C007652C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogOperPage.h : header file
//

#include "LDPropSheet.h"
/////////////////////////////////////////////////////////////////////////////
// CLogOperPage dialog
class CLogOperDoc;

class CLogOperPage : public CLDPropPage
{
	DECLARE_DYNCREATE(CLogOperPage)

// Construction
public:
	CLogOperPage();
	~CLogOperPage();

	CLogOperDoc*	m_pDocument;
	CFont			m_Font;

	void	SetDocument( CLogOperDoc* pDoc )	{ m_pDocument = pDoc; }

	CYHHashTable<TOOLLOG>	m_ToolLogTable;
	void	ReleaseToolLogTable();
	void	SetOperLog( TMSG_TOOLLOG* pMsg );
	void	InitListControl();
	void	UpdateListControl();
	void	InitComboBoxData();
	void	UpdateListControlFromLogKind( int nLogType, int nLogKind );

	char*	GetLogTypeName( int nLogType, char* temp );
	char*	GetLogKindName( int nLogType, int nLogKind, char* temp );
	char*	GetParam1Name( int nLogType, int nLogKind, DWORD dwParam1, char* temp );
	char*	GetParam2Name( int nLogType, int nLogKind, DWORD dwParam2, char* temp );
	char*	GetParam3Name( int nLogType, int nLogKind, DWORD dwParam3, char* temp );
	char*	GetParam4Name( int nLogType, int nLogKind, DWORD dwParam4, char* temp );
	
	void	ChangeFont();

// Dialog Data
	//{{AFX_DATA(CLogOperPage)
	enum { IDD = IDD_LOG_PAGE_OPER };
	CComboBox	m_cbRMLog;
	CComboBox	m_cbGMLog;
	CEdit	m_etOperName;
	CListCtrl	m_lcOper;
	CDateTimeCtrl	m_StartTime;
	CDateTimeCtrl	m_EndTime;
	BOOL	m_bOneDay;
	int		m_nLogKind;
	int		m_nSelectLog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLogOperPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLogOperPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckLogOneday();
	afx_msg void OnButtonLogSearch();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnSelchangeComboRmlog();
	afx_msg void OnSelchangeComboGmlog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGOPERPAGE_H__655BB61D_E115_4705_A9CE_28B2C007652C__INCLUDED_)
