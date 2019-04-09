#if !defined(AFX_SPECULARDLG_H__99477B02_231A_46BA_B18D_CE897ABBE1DB__INCLUDED_)
#define AFX_SPECULARDLG_H__99477B02_231A_46BA_B18D_CE897ABBE1DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpecularDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpecularDlg dialog

class CSpecularDlg : public CDialog
{
// Construction
public:
	CSpecularDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpecularDlg)
	enum { IDD = IDD_SPECULAR_DIALOG };
	BOOL	m_bSpecularEnable;	
	float	m_fSpecularIntensity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecularDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpecularDlg)
	afx_msg void OnSpecSetOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECULARDLG_H__99477B02_231A_46BA_B18D_CE897ABBE1DB__INCLUDED_)
