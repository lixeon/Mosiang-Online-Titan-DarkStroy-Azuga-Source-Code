#pragma once

#include "../4DyuchiGRX_common/IRenderer.h"


class CoD3DDevice;


class CBaseFontObject : public IDIFontObject
{
protected:
	DWORD				m_dwRefCount;
	CoD3DDevice*		m_pRenderer;

public:
	STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);

	

	void	__stdcall	BeginRender() = 0;
	void	__stdcall	EndRender() = 0;
	BOOL	__stdcall	DrawText(TCHAR* str,DWORD dwLen,RECT* pRect,DWORD dwColor,CHAR_CODE_TYPE type,DWORD dwFlag) = 0;

	virtual BOOL	__stdcall	CreateFont(CoD3DDevice* pRenderer,LOGFONT* pLogFont) = 0;
	

	
	virtual ~CBaseFontObject();

	


};

