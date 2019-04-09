#if !defined(AFX_INMAPMONSTERDLG_H__F1BD0434_4D00_4513_87EF_EA8248AA567C__INCLUDED_)
#define AFX_INMAPMONSTERDLG_H__F1BD0434_4D00_4513_87EF_EA8248AA567C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InMapMonsterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInMapMonsterDlg dialog

class CInMapMonsterDlg : public CDialog
{
// Construction
public:
	CInMapMonsterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInMapMonsterDlg)
	enum { IDD = IDD_INMAP_MONSTER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInMapMonsterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInMapMonsterDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INMAPMONSTERDLG_H__F1BD0434_4D00_4513_87EF_EA8248AA567C__INCLUDED_)
