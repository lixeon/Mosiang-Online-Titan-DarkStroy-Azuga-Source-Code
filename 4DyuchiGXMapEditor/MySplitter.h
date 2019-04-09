#if !defined(AFX_MYSPLITTER_H__93F14DDC_A685_4B71_858E_8C8C8FEAD014__INCLUDED_)
#define AFX_MYSPLITTER_H__93F14DDC_A685_4B71_858E_8C8C8FEAD014__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySplitter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySplitter frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMySplitter : public CSplitterWnd
{
	DECLARE_DYNCREATE(CMySplitter)
protected:

// Attributes
protected:
//	CSplitterWnd    m_wndSplitter;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySplitter)
	protected:
//	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	CMySplitter();           // protected constructor used by dynamic creation
	virtual ~CMySplitter();

	// Generated message map functions
	//{{AFX_MSG(CMySplitter)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSPLITTER_H__93F14DDC_A685_4B71_858E_8C8C8FEAD014__INCLUDED_)
