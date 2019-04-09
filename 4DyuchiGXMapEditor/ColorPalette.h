#if !defined(AFX_COLORPALETTE_H__263407D6_A8C9_4F2A_9806_BCFA550B620F__INCLUDED_)
#define AFX_COLORPALETTE_H__263407D6_A8C9_4F2A_9806_BCFA550B620F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorPalette.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorPalette dialog

class CColorPalette : public CColorDialog
{
	DECLARE_DYNAMIC(CColorPalette)

public:
	CColorPalette(COLORREF clrInit = 0, DWORD dwFlags = 0,
			CWnd* pParentWnd = NULL);

protected:
	//{{AFX_MSG(CColorPalette)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORPALETTE_H__263407D6_A8C9_4F2A_9806_BCFA550B620F__INCLUDED_)
