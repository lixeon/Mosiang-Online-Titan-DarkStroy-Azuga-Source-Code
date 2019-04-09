#if !defined(AFX_VIEWEROPTIONDLG_H__BE929574_C3D9_4A0F_A28B_4B00D7B1AF37__INCLUDED_)
#define AFX_VIEWEROPTIONDLG_H__BE929574_C3D9_4A0F_A28B_4B00D7B1AF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewerOptionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewerOptionDlg dialog

class CLogItemMoneyPage;

class CViewerOptionDlg : public CDialog
{
// Construction
public:
	CViewerOptionDlg(CWnd* pParent = NULL);   // standard constructor

	CLogItemMoneyPage*	m_pPage;
	void	SetPage( CLogItemMoneyPage* pPage )		{ m_pPage = pPage; }
	BOOL	m_bVOption[14];

// Dialog Data
	//{{AFX_DATA(CViewerOptionDlg)
	enum { IDD = IDD_DLG_ITEMMONEY_VIEWER_OPTION };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewerOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewerOptionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	afx_msg void OnCheck5();
	afx_msg void OnCheck6();
	afx_msg void OnCheck7();
	afx_msg void OnCheck8();
	afx_msg void OnCheck9();
	afx_msg void OnCheck10();
	afx_msg void OnCheck11();
	afx_msg void OnCheck12();
	afx_msg void OnCheck13();
	afx_msg void OnCheck14();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWEROPTIONDLG_H__BE929574_C3D9_4A0F_A28B_4B00D7B1AF37__INCLUDED_)
