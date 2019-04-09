// RMListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "RMListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRMListCtrl

CRMListCtrl::CRMListCtrl()
{
}

CRMListCtrl::~CRMListCtrl()
{
}


BEGIN_MESSAGE_MAP(CRMListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CRMListCtrl)
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CRMListCtrl message handlers

/*
LRESULT CRMListCtrl::OnSetFont(WPARAM wParam, LPARAM)
{
	LRESULT res =  Default();

	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );
	
	return res;
}
*/

void CRMListCtrl::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
	LOGFONT lf;
	GetFont()->GetLogFont( &lf );

	if( lf.lfHeight < 0 )
		lpMeasureItemStruct->itemHeight = (-lf.lfHeight) + 10; 
	else
		lpMeasureItemStruct->itemHeight = lf.lfHeight + 10; 
}
