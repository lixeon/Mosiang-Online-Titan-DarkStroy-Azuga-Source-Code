#if !defined(AFX_FRONTOFCAMERADLG_H__324E1364_BC86_4450_AF29_F9E232B3831D__INCLUDED_)
#define AFX_FRONTOFCAMERADLG_H__324E1364_BC86_4450_AF29_F9E232B3831D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrontOfCameraDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrontOfCameraDlg dialog

class CFrontOfCameraDlg : public CDialog
{
// Construction
public:
	CFrontOfCameraDlg(CWnd* pParent = NULL);   // standard constructor

	void	SetGXObjectFileName(char* pszFileName);
	void	UpdateDialog(void);

// Dialog Data
	//{{AFX_DATA(CFrontOfCameraDlg)
	enum { IDD = IDD_DIALOG_FRONT_OF_CAMERA };
	BYTE	m_byAlpha;
	float	m_fDistance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrontOfCameraDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFrontOfCameraDlg)
	afx_msg void OnFocAlphaButton();
	afx_msg void OnFocDistanceButton();
	afx_msg void OnFocRemoveButton();
	afx_msg void OnFocBrowseButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRONTOFCAMERADLG_H__324E1364_BC86_4450_AF29_F9E232B3831D__INCLUDED_)
