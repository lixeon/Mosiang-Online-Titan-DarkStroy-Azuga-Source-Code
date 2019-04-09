// SkinStatic.cpp : implementation file
//

#include "stdafx.h"
#include "MHAutoPatch.h"
#include "SkinStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinStatic

CSkinStatic::CSkinStatic()
{
	SetLogFont( "±¼¸²", 11, HANGUL_CHARSET );
	SetFontColor( RGB( 220, 220, 220 ) );
}

CSkinStatic::~CSkinStatic()
{
}


BEGIN_MESSAGE_MAP(CSkinStatic, CStatic)
	//{{AFX_MSG_MAP(CSkinStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinStatic message handlers

void CSkinStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	
	CFont cFont, *pOldFont;
	cFont.CreateFontIndirect( &m_lFont );

	CString text;
	GetWindowText( text );
//	dc.DrawText( text, )
	int nBkMode = dc.SetBkMode( TRANSPARENT );
	COLORREF color = dc.SetTextColor( m_FontColor );
	pOldFont = dc.SelectObject( &cFont );

	RECT rc;
	GetClientRect( &rc );

	if( GetStyle() & SS_RIGHT ) 
		dc.TextOut( rc.right - dc.GetTextExtent(text).cx, rc.top, text );
	else if( GetStyle() & SS_CENTER ) 
		dc.TextOut( ( rc.right - dc.GetTextExtent(text).cx ) / 2, rc.top, text );
	else
		dc.TextOut( rc.left, rc.top, text );


	dc.SetBkMode( nBkMode );
	dc.SetTextColor( color );
	dc.SelectObject( pOldFont );

	cFont.DeleteObject();

	// Do not call CStatic::OnPaint() for painting messages
}

void CSkinStatic::SetLogFont( CHAR* FontName, LONG lFontHeight, BYTE CharSet )
{
	m_lFont.lfHeight = -lFontHeight;
	m_lFont.lfWidth = 0;
	m_lFont.lfEscapement = 0;
	m_lFont.lfOrientation = 0;
	m_lFont.lfWeight = FW_NORMAL;
	m_lFont.lfItalic = 0;
	m_lFont.lfUnderline = 0;
	m_lFont.lfStrikeOut = 0;
	m_lFont.lfCharSet = CharSet;
	m_lFont.lfOutPrecision = 0;
	m_lFont.lfQuality = PROOF_QUALITY;
	m_lFont.lfPitchAndFamily = FF_DONTCARE;
	lstrcpy(m_lFont.lfFaceName, FontName );
}

void CSkinStatic::SetFontColor( COLORREF FontColor )
{
	m_FontColor = FontColor;
}