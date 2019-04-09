#if !defined(AFX_MONSTERLISTVIEWDLG_H__25FADEB7_8590_4EE6_97F8_EC729726034E__INCLUDED_)
#define AFX_MONSTERLISTVIEWDLG_H__25FADEB7_8590_4EE6_97F8_EC729726034E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonsterListViewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonsterListViewDlg dialog

class CMonsterListViewDlg : public CDialog
{
// Construction
public:
	int m_SaveCount;
	CMonsterListViewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMonsterListViewDlg)
	enum { IDD = IDD_MONSTERLISTVIEW };
	CStatic	m_TotMonCount;
	CButton	m_CloseViewBtn;
	CButton	m_SaveBtn;
	CListBox	m_MonsterListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterListViewDlg)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonsterListViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCloseview();
	afx_msg void OnSaveok();
	afx_msg void OnDblclkMonsterlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONSTERLISTVIEWDLG_H__25FADEB7_8590_4EE6_97F8_EC729726034E__INCLUDED_)
