#if !defined(AFX_MAPSELECTDLG_H__7805D9E8_EB07_4E41_8FDE_8BF21F7CFAA3__INCLUDED_)
#define AFX_MAPSELECTDLG_H__7805D9E8_EB07_4E41_8FDE_8BF21F7CFAA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapSelectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapSelectDlg dialog

class CMapSelectDlg : public CDialog
{
// Construction
public:
	CMapSelectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMapSelectDlg)
	enum { IDD = IDD_MAPSELECTDLG };
	CEdit	m_SelectMapEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMapSelectDlg)
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPSELECTDLG_H__7805D9E8_EB07_4E41_8FDE_8BF21F7CFAA3__INCLUDED_)
