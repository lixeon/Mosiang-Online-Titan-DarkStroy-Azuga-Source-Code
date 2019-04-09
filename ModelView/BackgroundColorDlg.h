#if !defined(AFX_BACKGROUNDCOLORDLG_H__69B7664D_00E5_4582_9922_D6E2A95DED31__INCLUDED_)
#define AFX_BACKGROUNDCOLORDLG_H__69B7664D_00E5_4582_9922_D6E2A95DED31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackgroundColorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBackgroundColorDlg dialog

class CBackgroundColorDlg : public CDialog
{
// Construction
public:
	CBackgroundColorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBackgroundColorDlg)
	enum { IDD = IDD_SET_BACKGROUND_COLOR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackgroundColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBackgroundColorDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSetBackgroundcolorDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKGROUNDCOLORDLG_H__69B7664D_00E5_4582_9922_D6E2A95DED31__INCLUDED_)
