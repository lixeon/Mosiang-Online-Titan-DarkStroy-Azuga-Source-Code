#if !defined(AFX_SECTIONINFODLG_H__94FD1C6A_0D76_4A70_8CBE_E258CC3EF61B__INCLUDED_)
#define AFX_SECTIONINFODLG_H__94FD1C6A_0D76_4A70_8CBE_E258CC3EF61B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SectionInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSectionInfoDlg dialog

class CSectionInfoDlg : public CDialog
{
// Construction
public:
	CSectionInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSectionInfoDlg)
	enum { IDD = IDD_SECTION_INFO };
	CString	m_strTop;
	CString	m_strRight;
	CString	m_strSectionNum;
	CString	m_strLeft;
	CString	m_strBottom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectionInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSectionInfoDlg)
	afx_msg void OnSectionDlgOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECTIONINFODLG_H__94FD1C6A_0D76_4A70_8CBE_E258CC3EF61B__INCLUDED_)
