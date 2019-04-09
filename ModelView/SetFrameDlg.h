#if !defined(AFX_SETFRAMEDLG_H__4BEB122A_0F62_4976_B2A1_CBB7483FC5B9__INCLUDED_)
#define AFX_SETFRAMEDLG_H__4BEB122A_0F62_4976_B2A1_CBB7483FC5B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetFrameDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CSetFrameDlg dialog

class CSetFrameDlg : public CDialog
{
// Construction
public:
	CSetFrameDlg(CWnd* pParent = NULL);   // standard constructor

	INT m_iFps;

// Dialog Data
	//{{AFX_DATA(CSetFrameDlg)
	enum { IDD = IDD_FRAME_SPEED_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetFrameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetFrameDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFpsOkButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETFRAMEDLG_H__4BEB122A_0F62_4976_B2A1_CBB7483FC5B9__INCLUDED_)
