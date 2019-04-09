// DS_RMToolView.h : interface of the CDS_RMToolView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DS_RMTOOLVIEW_H__C2538C18_3B78_4485_9F87_FE5B38E32D13__INCLUDED_)
#define AFX_DS_RMTOOLVIEW_H__C2538C18_3B78_4485_9F87_FE5B38E32D13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonRMControl.h"
//#include "RMListCtrl.h"

class CDS_RMToolView : public CFormView
{
protected: // create from serialization only
	CDS_RMToolView();
	DECLARE_DYNCREATE(CDS_RMToolView)

public:
	//{{AFX_DATA(CDS_RMToolView)
	enum { IDD = IDD_ACCOUNTINFO };
	CButton	m_btnChangeUserlevel;
	CListCtrl	m_lcCharInfo;
	CListCtrl	m_listCharacter;
	CComboBox	m_cbUserLevel;	
	CEdit	m_etRecover;
	CEdit	m_etDelete;
	CEdit	m_etSearch;
	CStatic	m_stStatus;
	int		m_nSearchKind;
	CButton	m_btnCheckName;
	CButton	m_btnRecover;
	//}}AFX_DATA

	CMREdit m_StatusEdit[eUserTemp_MAX]; //À¯Àú Á¤º¸Ã¢

// Attributes
public:
	CDS_RMToolDoc* GetDocument();

// Operations
public:
	WORD		m_wPower;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDS_RMToolView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDS_RMToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void InitListControl();

// Generated message map functions
protected:
	//{{AFX_MSG(CDS_RMToolView)
	afx_msg void OnDblclkListCharInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListChar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUserbtnSearch();
	afx_msg void OnRadioUserID();
	afx_msg void OnRadioCharName();
	afx_msg void OnRadioJumin();
	afx_msg void OnClickListChar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnCheckname();
	afx_msg void OnBtnRecover();
	afx_msg void OnButtonChangeUserlevel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	InitUserInfo();
	void	SetUserInfo( USERINFO* user );
	void	SetSimpleCharInfo( BYTE Count, SIMPlECHARACTERINFO* simple );
	void	SetCharacterCreateInfo( BYTE Count, CHARACTERCREATEINFO* pInfo );
	char*	GetUserLevel( DWORD dwUserlevel, char* temp );
	void	EnableRecoverButton();
	void	RecoverError( DWORD dwKind );
	void	ChangeUserLevel( DWORD dwUserLevel );

	CFont	m_Font;
};

#ifndef _DEBUG  // debug version in DS_RMToolView.cpp
inline CDS_RMToolDoc* CDS_RMToolView::GetDocument()
   { return (CDS_RMToolDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DS_RMTOOLVIEW_H__C2538C18_3B78_4485_9F87_FE5B38E32D13__INCLUDED_)
