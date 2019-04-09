#if !defined(AFX_ILLUSIONSETUPDLG_H__DD27086E_BA61_4366_92B2_E52F98C99847__INCLUDED_)
#define AFX_ILLUSIONSETUPDLG_H__DD27086E_BA61_4366_92B2_E52F98C99847__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IllusionSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIllusionSetupDlg dialog

class CIllusionSetupDlg : public CDialog
{
// Construction
public:
	CIllusionSetupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIllusionSetupDlg)
	enum { IDD = IDD_ILLUSION_SETUP };
	CEdit	m_IllusionLengthEdit;
	CButton	m_ShowCheckBox;
	//}}AFX_DATA
	BOOL OnInitDialog() ;

	BOOL	m_bShow;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIllusionSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIllusionSetupDlg)
	afx_msg void OnIllusionSetupShowCheck();
	afx_msg void OnChangeIllusionSetupDlgLengthEdit();
	afx_msg void OnUpdateIllusionSetupDlgLengthEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ILLUSIONSETUPDLG_H__DD27086E_BA61_4366_92B2_E52F98C99847__INCLUDED_)
