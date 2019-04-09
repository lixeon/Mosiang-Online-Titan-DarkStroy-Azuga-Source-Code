#if !defined(AFX_BOTTOMPANE_H__BEA08F0B_789B_4072_8FB9_1D6987D3E79C__INCLUDED_)
#define AFX_BOTTOMPANE_H__BEA08F0B_789B_4072_8FB9_1D6987D3E79C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BottomPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBottomPane form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CBottomPane : public CFormView
{
protected:
	CBottomPane();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBottomPane)

// Form Data
public:
	//{{AFX_DATA(CBottomPane)
	enum { IDD = IDD_FORMVIEW_BOTTOM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBottomPane)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBottomPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CBottomPane)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOTTOMPANE_H__BEA08F0B_789B_4072_8FB9_1D6987D3E79C__INCLUDED_)
