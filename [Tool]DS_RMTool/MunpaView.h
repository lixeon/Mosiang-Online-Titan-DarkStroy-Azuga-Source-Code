#if !defined(AFX_MUNPAVIEW_H__E6C7C107_F059_4B94_945B_4B70A8A9349B__INCLUDED_)
#define AFX_MUNPAVIEW_H__E6C7C107_F059_4B94_945B_4B70A8A9349B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MunpaView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMunpaView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//#include "CommonRMControl.h"

class CLDPropPage;
class CLDPropSheet;
class CMunpaInfo;
class CMunpaDoc;

enum eMunPage
{
	eMunPage_MunpaInfo = 0, eMunPage_Max, 
};

class CMunpaView : public CFormView
{
protected:
	CMunpaView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMunpaView)

// Form Data
public:
	//{{AFX_DATA(CMunpaView)
	enum { IDD = IDD_GUILD_SHEET };
	//}}AFX_DATA

// Attributes
public:
	// Sheet and Page
	CLDPropSheet*		m_pSheet;
	CLDPropPage*		m_pPageList[eMunPage_Max];
	//

// Operations
public:
	CMunpaDoc* GetDocument();

	CMunpaInfo*		GetMunpaInfoPage() { return (CMunpaInfo*)m_pPageList[eMunPage_MunpaInfo]; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMunpaView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMunpaView();

	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMunpaView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DS_RMToolView.cpp
inline CMunpaDoc* CMunpaView::GetDocument()
   { return (CMunpaDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUNPAVIEW_H__E6C7C107_F059_4B94_945B_4B70A8A9349B__INCLUDED_)
