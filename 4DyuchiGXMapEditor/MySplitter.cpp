// MySplitter.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "MySplitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySplitter

IMPLEMENT_DYNCREATE(CMySplitter, CSplitterWnd)

CMySplitter::CMySplitter()
{
}

CMySplitter::~CMySplitter()
{
}
/*
BOOL CMySplitter::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	return m_wndSplitter.Create(this,
		2, 2,       // TODO: adjust the number of rows, columns
		CSize(10, 10),  // TODO: adjust the minimum pane size
		pContext);
}
*/

BEGIN_MESSAGE_MAP(CMySplitter, CSplitterWnd)
	//{{AFX_MSG_MAP(CMySplitter)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySplitter message handlers

void CMySplitter::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CSplitterWnd::OnLButtonDown(nFlags, point);
}

void CMySplitter::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CSplitterWnd::OnLButtonUp(nFlags, point);
}

void CMySplitter::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CSplitterWnd::OnMouseMove(nFlags, point);
}
