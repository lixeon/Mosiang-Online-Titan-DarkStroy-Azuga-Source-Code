// ModelViewView.h : interface of the CModelViewView class
//
/////////////////////////////////////////////////////////////////////////////


#include "ToolInfo.h"


#if !defined(AFX_MODELVIEWVIEW_H__8EDEB9D8_6ED2_4F99_ACC7_82FB98E27A60__INCLUDED_)
#define AFX_MODELVIEWVIEW_H__8EDEB9D8_6ED2_4F99_ACC7_82FB98E27A60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CModelViewView : public CView
{
protected: // create from serialization only
	CModelViewView();
	DECLARE_DYNCREATE(CModelViewView)

// Attributes
public:
	CModelViewDoc* GetDocument();

// Operations
public:

	BOOL				m_bRightMouseDown;
	BOOL				m_bLeftMouseDown;

	INT					m_iRightMouseDownX;
	INT					m_iRightMouseDownY;
	INT					m_iLeftMouseDownX;
	INT					m_iLeftMouseDownY;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelViewView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CModelViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CModelViewView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);		
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ModelViewView.cpp
inline CModelViewDoc* CModelViewView::GetDocument()
   { return (CModelViewDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELVIEWVIEW_H__8EDEB9D8_6ED2_4F99_ACC7_82FB98E27A60__INCLUDED_)
