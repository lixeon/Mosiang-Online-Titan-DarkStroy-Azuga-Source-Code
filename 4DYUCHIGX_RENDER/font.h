#pragma once


#include <d3dx8.h>
#include "BaseFontObject.h"
#include "../4DyuchiGRX_common/IRenderer.h"


class CFontBuffer : public CBaseFontObject
{
	int					m_iHeight;
	int					m_iWidth;

	BYTE				m_bItalc;
	BYTE				m_bUnderLine;
	BYTE				m_bStrikeOut;
			
	LPD3DXFONT			m_pD3DXFont;
	
public:
	
	BOOL	__stdcall	CreateFont(CoD3DDevice* pRenderer,LOGFONT* pLogFont);
	void	__stdcall	BeginRender();
	void	__stdcall	EndRender();
	BOOL	__stdcall	DrawText(TCHAR* str,DWORD dwLen,RECT* pRect,DWORD dwColor,CHAR_CODE_TYPE type,DWORD dwFlag);

	
	CFontBuffer();
	virtual ~CFontBuffer();

	


};

