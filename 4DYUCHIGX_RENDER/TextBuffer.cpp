#include "TextBuffer.h"

BOOL __stdcall RenderFontItem(CTextBuffer* pBuffer,FONT_ITEM* pFontItem)
{
	return pBuffer->Render(pFontItem);
}

CTextBuffer::CTextBuffer()
{
	memset(this,0,sizeof(CTextBuffer));
}

BOOL CTextBuffer::Initialize(DWORD dwMaxBufferSize)
{
	return m_Buffer.Initialize(dwMaxBufferSize);
}

BOOL CTextBuffer::PushTextItem(DPCQ_HANDLE pDPCQ,IDIFontObject* pFont,TCHAR* str,DWORD dwLen,RECT* pRect,DWORD dwColor,CHAR_CODE_TYPE type,int iZOrder,DWORD dwFlag)
{
	BOOL	bResult = FALSE;

	DWORD	dwCharSize;
	DWORD	dwBufferSize;
	DWORD	dwStrSize;
	

	if (type == CHAR_CODE_TYPE_ASCII)
		dwCharSize = 1;
	else 
		dwCharSize = 2;

	dwStrSize = dwCharSize*dwLen;
	dwBufferSize = dwStrSize + FONT_ITEM_HEADER_SIZE;

	FONT_ITEM* pFontItem = (FONT_ITEM*)m_Buffer.AllocBuffer(dwBufferSize);

	if (!pFontItem)
		goto lb_return;
	
	pFontItem->dwItemSize = dwBufferSize;
	pFontItem->pFont = pFont;
	pFontItem->dwLen = dwLen;
	pFontItem->rect = *pRect;
	pFontItem->dwColor = dwColor;
	pFontItem->type = type;
	pFontItem->dwFlag = dwFlag;
	memcpy(pFontItem->pStr,str,dwStrSize);

	DWORD	dwArgList[2];
	dwArgList[0] = (DWORD)this;
	dwArgList[1] = (DWORD)pFontItem;

	DPCQPushDPC(pDPCQ,RenderFontItem,2,dwArgList,NULL,iZOrder);

	m_dwCurrentItemNum++;

	bResult = TRUE;
lb_return:

	return bResult;
}
BOOL CTextBuffer::Render(FONT_ITEM* pFontItem)
{

	return pFontItem->pFont->DrawText(
			pFontItem->pStr,
			pFontItem->dwLen,
			&pFontItem->rect,
			pFontItem->dwColor,
			pFontItem->type,
			pFontItem->dwFlag);
}/*
void CTextBuffer::Process()
{
	char* pBuffer = m_Buffer.GetBufferPtr();
	for (DWORD i=0; i<m_dwCurrentItemNum; i++)
	{
		((FONT_ITEM*)pBuffer)->pFont->DrawText(
			((FONT_ITEM*)pBuffer)->pStr,
			((FONT_ITEM*)pBuffer)->dwLen,
			&((FONT_ITEM*)pBuffer)->rect,
			((FONT_ITEM*)pBuffer)->dwColor,
			((FONT_ITEM*)pBuffer)->type);

		pBuffer += ((FONT_ITEM*)pBuffer)->dwItemSize;
	}
	m_dwCurrentItemNum = 0;
	m_Buffer.Clear();
}
*/

CTextBuffer::~CTextBuffer()
{
}