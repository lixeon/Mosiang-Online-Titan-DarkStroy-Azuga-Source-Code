#if !defined(AFX_EXTDLG_H__13579D67_6576_44EA_BE40_674E12370293__INCLUDED_)
#define AFX_EXTDLG_H__13579D67_6576_44EA_BE40_674E12370293__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExtDlg dialog

class CExtDlg : public CDialog
{
// Construction
public:
	CExtDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExtDlg)
	enum { IDD = IDD_DIALOG_EXT };
	CString	m_sExt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExtDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTDLG_H__13579D67_6576_44EA_BE40_674E12370293__INCLUDED_)
