#if !defined(AFX_LOGOPERFRM_H__B170DEFF_1978_4ADF_BCAC_826B17EA4FA9__INCLUDED_)
#define AFX_LOGOPERFRM_H__B170DEFF_1978_4ADF_BCAC_826B17EA4FA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogOperFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogOperFrm frame

class CLogOperFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CLogOperFrm)
protected:
	CLogOperFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogOperFrm)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLogOperFrm();

	// Generated message map functions
	//{{AFX_MSG(CLogOperFrm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGOPERFRM_H__B170DEFF_1978_4ADF_BCAC_826B17EA4FA9__INCLUDED_)
