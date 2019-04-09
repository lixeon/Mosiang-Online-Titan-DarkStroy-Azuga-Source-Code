#if !defined(AFX_TILESETVIEW_H__3BC62EAF_981A_468A_BF0C_1D38986BAE64__INCLUDED_)
#define AFX_TILESETVIEW_H__3BC62EAF_981A_468A_BF0C_1D38986BAE64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TileSetView.h : header file
//

#include "DialogTileSet.h"

/////////////////////////////////////////////////////////////////////////////
// CTileSetView window

class CTileSetView : public CWnd
{
// Construction
public:
	CTileSetView();

	//trustpak

	CDialogTileSet* m_pDlgTileSet;
	///

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTileSetView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTileSetView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTileSetView)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TILESETVIEW_H__3BC62EAF_981A_468A_BF0C_1D38986BAE64__INCLUDED_)
