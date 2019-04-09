#if !defined(AFX_NEWDLG_H__BE94D312_FA27_4794_9142_DD84C4A5DA37__INCLUDED_)
#define AFX_NEWDLG_H__BE94D312_FA27_4794_9142_DD84C4A5DA37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewDlg dialog

class CNewDlg : public CDialog
{
// Construction
public:
	CNewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewDlg)
	enum { IDD = IDD_DIALOG_NEW };
	CString	m_sEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWDLG_H__BE94D312_FA27_4794_9142_DD84C4A5DA37__INCLUDED_)
