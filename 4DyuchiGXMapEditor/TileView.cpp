// TileView2.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "TileView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DialogTilePalette.h"

/////////////////////////////////////////////////////////////////////////////
// CTileView

CTileView::CTileView()
{
	m_pPalette = NULL;
}

CTileView::~CTileView()
{
}


BEGIN_MESSAGE_MAP(CTileView, CWnd)
	//{{AFX_MSG_MAP(CTileView)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTileView message handlers


void CTileView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if (m_pPalette)
	{
//		m_pPalette->RenderPalette();
	}
	// Do not call CWnd::OnPaint() for painting messages
}

void CTileView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pPalette)
	{
		m_pPalette->SelectTile((POINT*)&point);
		CWnd::OnLButtonDown(nFlags, point);
	}
}
