#if !defined(AFX_LOGOPERVIEW_H__417EE672_1BD5_4C1E_B94D_27AA5743213F__INCLUDED_)
#define AFX_LOGOPERVIEW_H__417EE672_1BD5_4C1E_B94D_27AA5743213F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogOperView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogOperView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CLDPropPage;
class CLDPropSheet;
class CLogOperDoc;

class CLogOperView : public CFormView
{
protected:
	CLogOperView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLogOperView)

// Form Data
public:
	//{{AFX_DATA(CLogOperView)
	enum { IDD = IDD_LOG_SHEET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CLDPropSheet*	m_pSheet;
	CLDPropPage*	m_pPage;

// Operations
public:
	void	SetOperLog( TMSG_TOOLLOG* pMsg );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogOperView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLogOperView();
	CLogOperDoc*	GetDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CLogOperView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGOPERVIEW_H__417EE672_1BD5_4C1E_B94D_27AA5743213F__INCLUDED_)
