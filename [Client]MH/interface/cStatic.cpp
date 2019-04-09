// cStatic.cpp: implementation of the cStatic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cStatic.h"
#include "cFont.h"
#include "cWindowFunc.h"
#include "MHFile.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


cStatic::cStatic()
{
	m_type = WT_STATIC;
	m_fMultiLine = FALSE;
	*m_szStaticText = 0;
	m_textXY.x = 0;
	m_textXY.y = 0;
	m_fgColor = RGB_HALF(255,255,255);
	m_bShadow = FALSE;
	m_shadowTextXY.x = 1;
	m_shadowTextXY.y = 1;
	m_shadowColor = RGB_HALF(10,10,10);
}

cStatic::~cStatic()
{
	m_StaticText.Release();
}
void cStatic::InitMultiLine()
{
	if(m_fMultiLine)
	{
		m_StaticText.Init(m_wFontIdx, m_fgColor, NULL);
	}
}

void cStatic::SetFontIdx(WORD fontIdx)
{ 
	m_wFontIdx = fontIdx;
	m_StaticText.SetFontIdx(fontIdx);
}

void cStatic::Render()
{
	cWindow::Render();

	if(!m_fMultiLine)
	{	
		if(*m_szStaticText != 0)
		{
			LONG TxtPosX;

			if( m_nAlign & TXT_LEFT )
			{
				TxtPosX = (LONG)m_absPos.x + m_textXY.x;
			}
			else if( m_nAlign & TXT_RIGHT )
			{
				TxtPosX = (LONG)m_absPos.x + m_width - m_textXY.x 
					      - CFONT_OBJ->GetTextExtentEx( m_wFontIdx, m_szStaticText, strlen(m_szStaticText) );
			}
			else //TXT_CENTER
			{
				TxtPosX = (LONG)m_absPos.x + ( m_width
					      - CFONT_OBJ->GetTextExtentEx( m_wFontIdx, m_szStaticText, strlen(m_szStaticText) ) ) / 2;
			}

//			RECT rect={(LONG)m_absPos.x+m_textXY.x+m_shadowTextXY.x, (LONG)m_absPos.y+m_textXY.y+m_shadowTextXY.y, 1,1};
			RECT rect={ TxtPosX, (LONG)m_absPos.y + m_textXY.y, 1, 1 };

			if( m_bShadow )
			{
				RECT rcShadow = rect;
				OffsetRect( &rcShadow, m_shadowTextXY.x, m_shadowTextXY.y );
				CFONT_OBJ->RenderFont(m_wFontIdx,m_szStaticText,lstrlen(m_szStaticText),
							&rcShadow,RGBA_MERGE(m_shadowColor, m_alpha * m_dwOptionAlpha / 100));
			}

			//foreground color
			CFONT_OBJ->RenderFont(m_wFontIdx,m_szStaticText,lstrlen(m_szStaticText),
							&rect,RGBA_MERGE(m_fgColor, m_alpha * m_dwOptionAlpha / 100));
		}
	}
	else
	{
		if(m_StaticText.IsValid())
		{
			if(CFONT_OBJ->IsCreateFont(m_wFontIdx))
			{
				m_StaticText.SetXY((LONG)m_absPos.x+m_textXY.x, (LONG)m_absPos.y+m_textXY.y);
				m_StaticText.Render();
			}
		}
	}
}

void cStatic::SetStaticText(char * text)
{
	if(!m_fMultiLine)
	{
		char buf[256]={0,};
		char * aa = buf;
		_parsingKeywordString(text, &aa);
		strcpy(m_szStaticText, buf); 
	}
	else
	{
		m_StaticText = text;
	}

}
