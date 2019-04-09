#if !defined(AFX_QUESTINFODLG_H__6F10E07A_FAD9_4C13_A6FD_6701B270DFA3__INCLUDED_)
#define AFX_QUESTINFODLG_H__6F10E07A_FAD9_4C13_A6FD_6701B270DFA3__INCLUDED_

#include "SubQuestDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuestInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQuestInfoDlg dialog

class CQuestInfoDlg : public CDialog
{
// Construction
public:
	CSubQuestDlg m_SubQuestInfoDlg;
	CQuestInfoDlg(CWnd* pParent = NULL);   // standard constructor

	CFont		m_Font;
	DWORD		m_dwDocumentIdx;
	DWORD		m_dwCharIdx;
	char		m_sCharName[32];
	BOOL		m_bInit;
	POSITION	m_Pos;

	void		SetInitData( DWORD dwDocumentIdx, DWORD dwCharIdx, char* sCharName );
	void		InitQuestListCtrl();
	void		UpdateQuestListCtrl( TMSG_MAINQUESTINFO* pMsg );
	void		UpdateSubQuestData( TMSG_SUBQUESTINFO* pMsg );
	void		SendMainQuestInfo();

// Dialog Data
	//{{AFX_DATA(CQuestInfoDlg)
	enum { IDD = IDD_DLG_QUEST };
	CButton	m_btnQuestDelete;
	CEdit	m_etCharName;
	CListCtrl	m_lcQuest;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuestInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQuestInfoDlg)
	afx_msg void OnQuestDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClickQuestListctrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkQuestListctrl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUESTINFODLG_H__6F10E07A_FAD9_4C13_A6FD_6701B270DFA3__INCLUDED_)
