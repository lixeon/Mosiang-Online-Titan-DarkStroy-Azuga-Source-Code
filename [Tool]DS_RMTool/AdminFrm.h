#if !defined(AFX_ADMINFRM_H__362BDDBA_9E7A_48FB_A001_8BBC1F34AD38__INCLUDED_)
#define AFX_ADMINFRM_H__362BDDBA_9E7A_48FB_A001_8BBC1F34AD38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdminFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdminFrm frame

class CAdminFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CAdminFrm)
protected:
	CAdminFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminFrm)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdminFrm();

	// Generated message map functions
	//{{AFX_MSG(CAdminFrm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINFRM_H__362BDDBA_9E7A_48FB_A001_8BBC1F34AD38__INCLUDED_)
