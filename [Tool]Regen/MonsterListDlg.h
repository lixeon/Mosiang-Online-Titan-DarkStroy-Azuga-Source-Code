#if !defined(AFX_MONSTERLISTDLG_H__353D3E6F_F064_4D02_87DE_B946D95F1EBD__INCLUDED_)
#define AFX_MONSTERLISTDLG_H__353D3E6F_F064_4D02_87DE_B946D95F1EBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonsterListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonsterListDlg dialog

class CMonsterListDlg : public CDialog
{
// Construction
public:
	CMonsterListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMonsterListDlg)
	enum { IDD = IDD_MONSTERLIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonsterListDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONSTERLISTDLG_H__353D3E6F_F064_4D02_87DE_B946D95F1EBD__INCLUDED_)
