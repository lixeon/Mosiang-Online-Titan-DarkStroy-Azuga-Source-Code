// cFont.cpp: implementation of the cFont class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cFont.h"



extern HWND _g_hWnd;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(cFont);
cFont::cFont()
{
	for(int i=0;i<FONTMAX;i++)
	{
		m_pFont[i]=NULL;
//KES 030915
		m_hFont[i]=NULL;
	}

	ZeroMemory( m_LogFont, sizeof( m_LogFont ) );
}

cFont::~cFont()
{
	Release();
}
void cFont::Release()
{
	for(int i=0;i<FONTMAX;i++)
	{
		SAFE_RELEASE(m_pFont[i]);
//KES 030915
		if( m_hFont[i] )
			DeleteObject( m_hFont[i] );
	}
}
BOOL cFont::CreateFontObject(LOGFONT * pFont, WORD idx)
{
	if(idx > FONTMAX || idx < 0) return FALSE;
	
	m_pFont[idx] = g_pExecutive->GetRenderer()->CreateFontObject(pFont, SS3D_FONT);//SS3D_FONT);//D3DX_FONT);

//KES 030915
	m_LogFont[idx] = *pFont;
	m_hFont[idx] = CreateFontIndirect( pFont );

//	Getfont
#ifdef _TL_LOCAL_
	HDC hdc = GetDC( _g_hWnd );
	HFONT OldFont = (HFONT)SelectObject( hdc, m_hFont[idx] );

	SIZE size;
	GetTextExtentPoint32( hdc, "Gg", 1, &size );

	SelectObject( hdc, OldFont );

	ReleaseDC( _g_hWnd, hdc );

	m_FontDesc[idx].wWidth = (unsigned short)pFont->lfWidth;
	m_FontDesc[idx].wHeight = size.cy;

#else

	//tmp
//	m_FontDesc[idx].wWidth = 6;
	m_FontDesc[idx].wWidth = (unsigned short)pFont->lfWidth;
	
	m_FontDesc[idx].wHeight = (unsigned short)(-pFont->lfHeight);

#endif
	return TRUE;
}

LONG cFont::GetTextExtent(WORD hFIdx, char * str, LONG strSize)
{
#ifdef _TL_LOCAL_
	HDC hdc = GetDC( _g_hWnd );
	HFONT OldFont = (HFONT)SelectObject( hdc, m_hFont[hFIdx] );

	SIZE size;
	GetTextExtentPoint32( hdc, str, strSize, &size );

	SelectObject( hdc, OldFont );

	ReleaseDC( _g_hWnd, hdc );

	return size.cx;
#elif _KOR_LOCAL_
	HDC hdc = GetDC( _g_hWnd );
	HFONT OldFont = (HFONT)SelectObject( hdc, m_hFont[hFIdx] );

	SIZE size;
	GetTextExtentPoint32( hdc, str, strSize, &size );

	SelectObject( hdc, OldFont );

	ReleaseDC( _g_hWnd, hdc );

	return size.cx;
#else

	if(hFIdx > FONTMAX || hFIdx < 0) return 0;
	WORD wid = m_FontDesc[hFIdx].wWidth;
	WORD DBCNT = 0;	
	WORD SBCNT = 0;
	for(int i = 0 ; i < strSize ; i++)
	{
//		if(str[i] & 0x80)
		if( IsDBCSLeadByte(str[i]) )
		{
			DBCNT++;
			i++;
		}
		else
		{		
			SBCNT++;
		}
	}
	
	LONG TextWidth = SBCNT*wid+DBCNT*2*wid;
	return TextWidth;

#endif
}


LONG cFont::GetTextExtentEx( WORD hFIdx, char* str, int strLen )
{

#ifdef _TL_LOCAL_
	HDC hdc = GetDC( _g_hWnd );
	HFONT OldFont = (HFONT)SelectObject( hdc, m_hFont[hFIdx] );

	SIZE size;
	GetTextExtentPoint32( hdc, str, strLen, &size );

	SelectObject( hdc, OldFont );

	ReleaseDC( _g_hWnd, hdc );

	return size.cx;
#elif _KOR_LOCAL_
	HDC hdc = GetDC( _g_hWnd );
	HFONT OldFont = (HFONT)SelectObject( hdc, m_hFont[hFIdx] );

	SIZE size;
	GetTextExtentPoint32( hdc, str, strLen, &size );

	SelectObject( hdc, OldFont );

	ReleaseDC( _g_hWnd, hdc );

	return size.cx;
#else
// 굴림체폰트를 사용함..

	if(hFIdx > FONTMAX || hFIdx < 0) return 0;
	WORD wid = m_FontDesc[hFIdx].wWidth;
	WORD DBCNT = 0;	
	WORD SBCNT = 0;
	for(int i = 0 ; i < strLen ; i++)
	{
//		if(str[i] & 0x80)
		if( IsDBCSLeadByte(str[i]) )
		{
			DBCNT++;
			i++;
		}
		else
		{
			SBCNT++;
		}
	}

	LONG TextWidth = SBCNT*wid+DBCNT*2*wid;
	return TextWidth;
#endif
}


void cFont::RenderFont(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color)
{
	if(wFontIdx>FONTMAX||wFontIdx<0) return ;
	if(!m_pFont[wFontIdx])
		return ;

	//KES가 수정했음 040722
	pRect->right = pRect->left + GetTextExtent( wFontIdx, text, size ) + GetTextWidth( wFontIdx ) / 4;
	pRect->bottom = pRect->top + GetTextHeight( wFontIdx ) + 5;	//+1안하면 밑줄이 안나온다. 문자 크기 조절에 따라 +5로 변경

	//KES 040722 추가
	if( pRect->left > 1024 || pRect->right < 0 || pRect->bottom < 0 || pRect->top > 768 )
		return;

	if(*text)
	{
		DWORD dwColor = 0xff000000 | color;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_ASCII,0,0);
//		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_UNICODE,0,0);

		//m_pFont[wFontIdx]->DrawText(text, size, pRect,color, CHAR_CODE_TYPE_ASCII,0);
	}

}

//임시
void cFont::RenderFontAlpha(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color)
{
	if(wFontIdx>FONTMAX||wFontIdx<0) return ;
	if(!m_pFont[wFontIdx])
		return ;

	//KES가 수정했음 040722
	pRect->right = pRect->left + GetTextExtent( wFontIdx, text, size ) + GetTextWidth( wFontIdx ) / 4;
	pRect->bottom = pRect->top + GetTextHeight( wFontIdx ) + 5;	//+1안하면 밑줄이 안나온다. 문자 크기 조절에 따라 +5로 변경

	//KES 040722 추가
	if( pRect->left > 1024 || pRect->right < 0 || pRect->bottom < 0 || pRect->top > 768 )
		return;

	if(*text)
	{
		DWORD dwColor = color;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_ASCII,0,0);
//		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_UNICODE,0,0);

		//m_pFont[wFontIdx]->DrawText(text, size, pRect,color, CHAR_CODE_TYPE_ASCII,0);
	}

}
