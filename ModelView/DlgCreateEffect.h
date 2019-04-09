#if !defined(AFX_DLGCREATEEFFECT_H__B7094077_D394_4450_A9DB_272D14165CAD__INCLUDED_)
#define AFX_DLGCREATEEFFECT_H__B7094077_D394_4450_A9DB_272D14165CAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCreateEffect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateEffect dialog

class CDlgCreateEffect : public CDialog
{
	char*		m_pszEffectName;
// Construction
public:
	CDlgCreateEffect(CWnd* pParent = NULL);   // standard constructor
	void		SetEffectNameResulPtr(char* pPtr) {m_pszEffectName = pPtr;}
// Dialog Data
	//{{AFX_DATA(CDlgCreateEffect)
	enum { IDD = IDD_DIALOG_CREATE_EFFECT };
	CString	m_strEffectName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCreateEffect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCreateEffect)
	afx_msg void OnButtonEffectNameOk();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCREATEEFFECT_H__B7094077_D394_4450_A9DB_272D14165CAD__INCLUDED_)
