#if !defined(AFX_HTMLVIEWNOTICE_H__0F98B851_4318_4A94_B2D0_737064BF8189__INCLUDED_)
#define AFX_HTMLVIEWNOTICE_H__0F98B851_4318_4A94_B2D0_737064BF8189__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HtmlViewNotice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewNotice html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CHtmlViewNotice : public CHtmlView
{
public:
	CHtmlViewNotice();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHtmlViewNotice)

// html Data
public:
	//{{AFX_DATA(CHtmlViewNotice)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

protected:
	BOOL bCanNotConnect;
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlViewNotice)
	public:
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnDownloadBegin();
	virtual void OnDownloadComplete();
	virtual void OnNavigateComplete2(LPCTSTR strURL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHtmlViewNotice();

protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHtmlViewNotice)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLVIEWNOTICE_H__0F98B851_4318_4A94_B2D0_737064BF8189__INCLUDED_)
