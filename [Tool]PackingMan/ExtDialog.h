#if !defined(AFX_EXTDIALOG_H__87DC570E_505C_4164_8E81_EBF164EF748A__INCLUDED_)
#define AFX_EXTDIALOG_H__87DC570E_505C_4164_8E81_EBF164EF748A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExtDialog dialog

class CExtDialog : public CDialog
{
// Construction
public:
	CExtDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExtDialog)
	enum { IDD = IDD_DIALOG_EXT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExtDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTDIALOG_H__87DC570E_505C_4164_8E81_EBF164EF748A__INCLUDED_)
