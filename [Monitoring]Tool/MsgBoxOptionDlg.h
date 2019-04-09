#if !defined(AFX_MSGBOXOPTIONDLG_H__C5C2BE93_8FE8_4E86_8170_75F1F56D576C__INCLUDED_)
#define AFX_MSGBOXOPTIONDLG_H__C5C2BE93_8FE8_4E86_8170_75F1F56D576C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgBoxOptionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgBoxOptionDlg dialog

class CMsgBoxOptionDlg : public CDialog
{
// Construction
public:
	CMsgBoxOptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMsgBoxOptionDlg)
	enum { IDD = IDD_DIALOG_MSGBOX_OPTION };
	BOOL	m_bServer;
	BOOL	m_bExeVer;
	BOOL	m_bMaxUser;
	BOOL	m_bUserLevel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgBoxOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMsgBoxOptionDlg)
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGBOXOPTIONDLG_H__C5C2BE93_8FE8_4E86_8170_75F1F56D576C__INCLUDED_)
