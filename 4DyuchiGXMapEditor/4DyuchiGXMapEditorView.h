// 4DyuchiGXMapEditorView.h : interface of the CMy4DyuchiGXMapEditorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_4DYUCHIGXMAPEDITORVIEW_H__4192DD26_78AF_4729_A784_90CB789E849F__INCLUDED_)
#define AFX_4DYUCHIGXMAPEDITORVIEW_H__4192DD26_78AF_4729_A784_90CB789E849F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../4DyuchiGX_UTIL/MToolMouseStatus.h"


class CMy4DyuchiGXMapEditorView : public CView
{

protected: // create from serialization only
	CMy4DyuchiGXMapEditorView();
	DECLARE_DYNCREATE(CMy4DyuchiGXMapEditorView)

// Attributes
public:
	CMy4DyuchiGXMapEditorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy4DyuchiGXMapEditorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMy4DyuchiGXMapEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


// Generated message map functions
protected:
	//{{AFX_MSG(CMy4DyuchiGXMapEditorView)
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSectionInput();
	afx_msg void OnUpdateSectionInput(CCmdUI* pCmdUI);
	afx_msg void OnSectionInfo();
	afx_msg void OnUpdateSectionInfo(CCmdUI* pCmdUI);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCreateNpc();
	afx_msg void OnUpdateCreateNpc(CCmdUI* pCmdUI);
	afx_msg void OnLoadTtb();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMovespotInput();
	afx_msg void OnUpdateMovespotInput(CCmdUI* pCmdUI);
	afx_msg void OnMovespotInfo();
	afx_msg void OnUpdateMovespotInfo(CCmdUI* pCmdUI);
	afx_msg void OnStartspotInfo();
	afx_msg void OnUpdateStartspotInfo(CCmdUI* pCmdUI);
	afx_msg void OnStartspotInput();
	afx_msg void OnUpdateStartspotInput(CCmdUI* pCmdUI);
	afx_msg void OnSaveTtb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in 4DyuchiGXMapEditorView.cpp
inline CMy4DyuchiGXMapEditorDoc* CMy4DyuchiGXMapEditorView::GetDocument()
   { return (CMy4DyuchiGXMapEditorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_4DYUCHIGXMAPEDITORVIEW_H__4192DD26_78AF_4729_A784_90CB789E849F__INCLUDED_)
