#if !defined(AFX_SKINPROGRESSBAR_H__7B539047_6422_4DE4_B8EE_E9CAB9B82AED__INCLUDED_)
#define AFX_SKINPROGRESSBAR_H__7B539047_6422_4DE4_B8EE_E9CAB9B82AED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinProgressBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkinProgressBar window

class CSkinProgressBar : public CProgressCtrl
{
// Construction
public:
	CSkinProgressBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinProgressBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSkinProgressBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinProgressBar)
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINPROGRESSBAR_H__7B539047_6422_4DE4_B8EE_E9CAB9B82AED__INCLUDED_)
