#if !defined(AFX_LOGFRM_H__66591512_02B5_47B9_8589_6D6719B75CA6__INCLUDED_)
#define AFX_LOGFRM_H__66591512_02B5_47B9_8589_6D6719B75CA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogFrm frame

class CLogFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CLogFrm)
protected:
	CLogFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogFrm)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLogFrm();

	// Generated message map functions
	//{{AFX_MSG(CLogFrm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGFRM_H__66591512_02B5_47B9_8589_6D6719B75CA6__INCLUDED_)
