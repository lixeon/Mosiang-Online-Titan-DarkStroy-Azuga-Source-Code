// cFont.h: interface for the cFont class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _cFONT_H_
#define _cFONT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "cWindowDef.h"
#include "../Engine/GraphicEngine.h"
#include "../4DyuchiGRX_common/IGeometry.h"


#define CFONT_OBJ	USINGTON(cFont)

typedef struct _FONT_DESC
{
	WORD wWidth;
	WORD wHeight;
	//WORD wHalfWidth;
	//WORD wHalfHeight;
}FONT_DESC;

class cFont  
{
public:
	enum FONT_KIND{FONT0=0, FONT1, FONT2, FONT3, FONT4, FONT5, FONTMAX};
	cFont();
	virtual ~cFont();
	void Release();
	// GetInstance()
//	MAKESINGLETON(cFont);
	BOOL CreateFontObject(LOGFONT *pFont, WORD idx);
	void RenderFont(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color);

//юс╫ц
	void RenderFontAlpha(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color);

	IDIFontObject * GetFont(WORD idx)
	{
		#ifdef _DEBUG
		if(idx>FONTMAX||idx<0) return NULL;
		#endif //_DEBUG
		return m_pFont[idx];
	}
	BOOL IsCreateFont(WORD idx)
	{
		if(idx>FONTMAX||idx<0) return FALSE;
		if(m_pFont[idx]) return TRUE;
		else return FALSE;
	}
	LONG GetTextExtent(WORD hFIdx, char *str, LONG strSize);
	LONG GetTextHeight(WORD hFIdx) { return m_FontDesc[hFIdx].wHeight; }
	LONG GetTextWidth(WORD hFIdx) { return m_FontDesc[hFIdx].wWidth; }
	IDIFontObject * m_pFont[FONTMAX];
	FONT_DESC m_FontDesc[FONTMAX];
	
//KES 030915
	HFONT m_hFont[FONTMAX];
	LOGFONT m_LogFont[FONTMAX];
	LONG GetTextExtentEx( WORD hFIdx, char* str, int strLen );
};
EXTERNGLOBALTON(cFont);
#endif // _cFONT_H_
