// ColorPalette.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "ColorPalette.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorPalette

IMPLEMENT_DYNAMIC(CColorPalette, CColorDialog)

CColorPalette::CColorPalette(COLORREF clrInit, DWORD dwFlags, CWnd* pParentWnd) :
	CColorDialog(clrInit, dwFlags, pParentWnd)
{
//	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);



}


BEGIN_MESSAGE_MAP(CColorPalette, CColorDialog)
	//{{AFX_MSG_MAP(CColorPalette)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

