#if !defined(AFX_MUNPAFRM_H__81D785F0_D078_40E8_821D_FD05B5550079__INCLUDED_)
#define AFX_MUNPAFRM_H__81D785F0_D078_40E8_821D_FD05B5550079__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MunpaFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMunpaFrm frame

class CMunpaFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMunpaFrm)
protected:
	CMunpaFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMunpaFrm)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMunpaFrm();

	// Generated message map functions
	//{{AFX_MSG(CMunpaFrm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUNPAFRM_H__81D785F0_D078_40E8_821D_FD05B5550079__INCLUDED_)
