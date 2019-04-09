#if !defined(AFX_LIGHTDLG_H__EC3526AB_8855_4CCE_9807_D18D444A820B__INCLUDED_)
#define AFX_LIGHTDLG_H__EC3526AB_8855_4CCE_9807_D18D444A820B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LightDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLightDlg dialog

class CLightDlg : public CDialog
{
// Construction
public:
	CLightDlg(CWnd* pParent = NULL);   // standard constructor

	BYTE m_byAmbientR;
	BYTE m_byAmbientG;
	BYTE m_byAmbientB;

	BYTE m_byDiffuseR;
	BYTE m_byDiffuseG;
	BYTE m_byDiffuseB;

	FLOAT m_fDirectionX;
	FLOAT m_fDirectionY;
	FLOAT m_fDirectionZ;	


// Dialog Data
	//{{AFX_DATA(CLightDlg)
	enum { IDD = IDD_LIGHT_DIALOG };
	BOOL	m_bLightOn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLightDlg)
	afx_msg void OnLightOkButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTDLG_H__EC3526AB_8855_4CCE_9807_D18D444A820B__INCLUDED_)
