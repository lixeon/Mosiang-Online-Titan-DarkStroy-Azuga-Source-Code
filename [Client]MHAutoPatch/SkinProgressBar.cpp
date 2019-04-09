// SkinProgressBar.cpp : implementation file
//

#include "stdafx.h"
#include "MHAutoPatch.h"
#include "SkinProgressBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinProgressBar

CSkinProgressBar::CSkinProgressBar()
{
}

CSkinProgressBar::~CSkinProgressBar()
{
}


BEGIN_MESSAGE_MAP(CSkinProgressBar, CProgressCtrl)
	//{{AFX_MSG_MAP(CSkinProgressBar)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinProgressBar message handlers

void CSkinProgressBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	RECT rc;
	GetClientRect( &rc );
	int nLower, nUpper, nPos;
	GetRange( nLower, nUpper );
	nPos = GetPos();
	CBrush Brush, *pOldBrush;
	CBrush Back;
	CPen Pen, *pOldPen;
	CPen Pen2;

	CBitmap bmpPB;
	bmpPB.LoadBitmap( IDB_PB );

	Brush.CreatePatternBrush( &bmpPB );
	pOldBrush = dc.SelectObject( &Brush );
	Pen.CreatePen( PS_SOLID, 0, RGB( 198, 0, 0 ) );
	pOldPen = dc.SelectObject( &Pen );
	dc.Rectangle( rc.left, rc.top, rc.right * nPos / (nUpper-nLower), rc.bottom );
	//dc.SelectObject( &Pen );

	Pen2.CreatePen( PS_SOLID, 0, RGB( 153, 153, 153 ) );
	dc.SelectObject( &Pen2 );
	Back.CreateSolidBrush( RGB( 153, 153, 153 ) );
	dc.SelectObject( &Back );
	dc.Rectangle( rc.right * nPos / (nUpper-nLower), rc.top, rc.right, rc.bottom );

	dc.SelectObject( pOldBrush );
	dc.SelectObject( pOldPen );

	bmpPB.DeleteObject();
	// Do not call CProgressCtrl::OnPaint() for painting messages
}

void CSkinProgressBar::OnNcPaint() 
{
	// TODO: Add your message handler code here
	
	// Do not call CProgressCtrl::OnNcPaint() for painting messages
}
