#if !defined(AFX_LOGINDLG_H__A7D43098_8DE6_4C95_91E3_12A8CD4D651F__INCLUDED_)
#define AFX_LOGINDLG_H__A7D43098_8DE6_4C95_91E3_12A8CD4D651F__INCLUDED_

#include "MyEdit.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

class CLoginDlg : public CDialog
{
// Construction
public:
	int		m_nReceiveCount;
	BOOL m_bConnect;
	int m_nServerSet;
	void EnterIDPass();
	void LoginError( int error );
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_DIALOG_LOGIN };
	CMyEdit	m_editPass;
	CMyEdit	m_editID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginDlg)
	afx_msg void OnButtonConnect();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDLG_H__A7D43098_8DE6_4C95_91E3_12A8CD4D651F__INCLUDED_)
