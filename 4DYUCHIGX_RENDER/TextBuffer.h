#ifndef TEXT_BUFFER_H
#define TEXT_BUFFER_H

#include "../4DyuchiGRX_Common/IRenderer.h"
#include "VariableBuffer.h"
#include "../4DyuchiGXGFunc/global.h"


struct FONT_ITEM
{
	DWORD				dwItemSize;
	IDIFontObject*		pFont;
	DWORD				dwLen;
	RECT				rect;
	DWORD				dwColor;
	CHAR_CODE_TYPE		type;
	DWORD				dwFlag;
	TCHAR				pStr[1];
};

#define FONT_ITEM_HEADER_SIZE	40
class CTextBuffer
{
	CVariableBuffer		m_Buffer;
	DWORD				m_dwCurrentItemNum;
public:
	BOOL				Initialize(DWORD dwMaxBufferSize);
	BOOL				PushTextItem(DPCQ_HANDLE pDPCQ,IDIFontObject* pFont,TCHAR* str,DWORD dwLen,RECT* pRect,DWORD dwColor,CHAR_CODE_TYPE type,int iZOrder,DWORD dwFlag);
	void				Process();
	BOOL				Render(FONT_ITEM* pFontItem);
	void				Clear() {m_Buffer.Clear();}
	CTextBuffer();
	~CTextBuffer();
};

#endif
