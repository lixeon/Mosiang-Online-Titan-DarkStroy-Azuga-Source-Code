#if !defined(AFX_SUBQUESTDLG_H__377F67A9_7C8A_4854_9EC7_A58C95F4BC67__INCLUDED_)
#define AFX_SUBQUESTDLG_H__377F67A9_7C8A_4854_9EC7_A58C95F4BC67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubQuestDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubQuestDlg dialog

class CSubQuestDlg : public CDialog
{
// Construction
public:
	CSubQuestDlg(CWnd* pParent = NULL);   // standard constructor

	DWORD	m_dwDocumentIdx;
	DWORD	m_dwCharIdx;
	DWORD	m_dwQuestIdx;
	BOOL	m_bInit;
	CFont		m_Font;

	void	SetInitData( DWORD dwDocumentIdx, DWORD dwCharIdx, DWORD dwQuestIdx );
	void	InitSubQuestListCtrl();
	void	UpdateSubQuestListCtrl( TMSG_SUBQUESTINFO* pMsg );
	void	SendSubQuestInfo();

// Dialog Data
	//{{AFX_DATA(CSubQuestDlg)
	enum { IDD = IDD_DLG_SUBQUEST };
	CListCtrl	m_lcSubQuest;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubQuestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSubQuestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBQUESTDLG_H__377F67A9_7C8A_4854_9EC7_A58C95F4BC67__INCLUDED_)
