#if !defined(AFX_TILEVIEW_H__A25BD8CD_1803_4B1E_97F1_805423AE926C__INCLUDED_)
#define AFX_TILEVIEW_H__A25BD8CD_1803_4B1E_97F1_805423AE926C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TileView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTileView window

class CDialogTilePalette;

class CTileView : public CWnd
{
	CDialogTilePalette*		m_pPalette;

// Construction
public:
	void				SetTilePalette(CDialogTilePalette* pPalette) {m_pPalette = pPalette;}

	CTileView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTileView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTileView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTileView)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TILEVIEW_H__A25BD8CD_1803_4B1E_97F1_805423AE926C__INCLUDED_)
