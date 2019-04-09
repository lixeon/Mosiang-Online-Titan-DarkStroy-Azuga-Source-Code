// TileSetView.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "TileSetView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTileSetView

CTileSetView::CTileSetView()
{
}

CTileSetView::~CTileSetView()
{
}


BEGIN_MESSAGE_MAP(CTileSetView, CWnd)
	//{{AFX_MSG_MAP(CTileSetView)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTileSetView message handlers

void CTileSetView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (NULL != m_pDlgTileSet)
	{
		m_pDlgTileSet->RenderTileSet();
	}
	
	// Do not call CWnd::OnPaint() for painting messages
}


void CTileSetView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (NULL != m_pDlgTileSet)
	{
		m_pDlgTileSet->DisplaySelectedTileTextureFileName(point.x, point.y);
		CWnd::OnLButtonDown(nFlags, point);
	}
}
