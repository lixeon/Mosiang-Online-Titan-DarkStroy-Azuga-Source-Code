#if !defined(AFX_RMLISTCTRL_H__5AA0D7E2_D221_4A34_A31E_639B6CC78624__INCLUDED_)
#define AFX_RMLISTCTRL_H__5AA0D7E2_D221_4A34_A31E_639B6CC78624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RMListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRMListCtrl window

class CRMListCtrl : public CListCtrl
{
// Construction
public:
	CRMListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRMListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRMListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRMListCtrl)
	//}}AFX_MSG
//	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	afx_msg void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RMLISTCTRL_H__5AA0D7E2_D221_4A34_A31E_639B6CC78624__INCLUDED_)
