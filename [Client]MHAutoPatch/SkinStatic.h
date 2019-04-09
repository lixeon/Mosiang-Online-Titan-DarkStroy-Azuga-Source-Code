#if !defined(AFX_SKINSTATIC_H__BACA122C_28C4_4020_9CE6_DAA0519950DF__INCLUDED_)
#define AFX_SKINSTATIC_H__BACA122C_28C4_4020_9CE6_DAA0519950DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkinStatic window

class CSkinStatic : public CStatic
{
// Construction
public:
	CSkinStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSkinStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	LOGFONT		m_lFont;
	COLORREF	m_FontColor;
public:
	void SetLogFont( CHAR* FontName, LONG lFontHeight, BYTE CharSet = DEFAULT_CHARSET );
	void SetFontColor( COLORREF FontColor );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINSTATIC_H__BACA122C_28C4_4020_9CE6_DAA0519950DF__INCLUDED_)
