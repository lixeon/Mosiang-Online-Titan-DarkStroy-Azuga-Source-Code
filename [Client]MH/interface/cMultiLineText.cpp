// cMultiLineText.cpp: implementation of the cMultiLineText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cMultiLineText.h"
#include "GameResourceManager.h"
#include "cFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cMultiLineText::cMultiLineText()
{
	m_fgColor  =0x00ffffff;
	m_imgColor = 0x00ffffff;
	m_line_idx = 0;
	topLine = NULL;
	m_wFontIdx = 0;
	m_max_line_width = 0;
	m_fValid = FALSE;
	SetRect(&m_m_leftTopPos,0,0,0,0);
//	m_fSurvive = FALSE;
	m_alpha = 255;
	m_dwOptionAlpha = 100;
	m_bNamePannel = FALSE;
}

cMultiLineText::~cMultiLineText()
{
	Release();
}

void cMultiLineText::Init(WORD fontIdx, DWORD fgColor, cImage * bgImage, DWORD imgColor )
{
	Release();
	m_wFontIdx = fontIdx;
	m_fgColor = fgColor;
	if( bgImage )
	m_bgImage = *bgImage;
	m_imgColor = imgColor;
}

void cMultiLineText::Release()
{
	LINE_NODE * curLineNode = topLine;
	LINE_NODE * tmpNode = NULL;
	
	while(curLineNode)
	{
		tmpNode = curLineNode;
		curLineNode = curLineNode->nextLine;
		SAFE_DELETE(tmpNode);		
	}

	topLine = NULL;
	m_fValid = FALSE;
	m_line_idx = 0;
	m_max_line_width = 0;
	m_bNamePannel = FALSE;
}

void cMultiLineText::Render()
{
	LINE_NODE * curLineNode = topLine;
	RECT rect = m_m_leftTopPos;

	//화면밖으로 툴팁이 나가는것 방지
	//KES 030901 해상도 옵션에 따라 1024, 768이 달라져야 한다.
	if( (DWORD)(rect.left + m_max_line_width) > GET_MAINWIN_W ||
		(DWORD)(rect.top + (m_line_idx+1)*LINE_HEIGHT) > GET_MAINWIN_H )
	{
		if( rect.left - ( TOOLTIP_MOUSESIZE_X+4 + m_max_line_width ) >= 0 )
			rect.left -= TOOLTIP_MOUSESIZE_X+4 + m_max_line_width;
		else
			rect.left -= TOOLTIP_MOUSESIZE_X;

		if( rect.top - ( TOOLTIP_MOUSESIZE_Y + (m_line_idx+1)*LINE_HEIGHT ) >= 0 )
			rect.top -= TOOLTIP_MOUSESIZE_Y+4 + (m_line_idx+1)*LINE_HEIGHT;
		else 
			rect.top -= TOOLTIP_MOUSESIZE_Y-4;
	}

	if(!m_bgImage.IsNull())
	{
		VECTOR2 scaleRate, Pos;

		if( m_bNamePannel )
		{
			scaleRate.x = (float)((m_max_line_width+5)>>2);
			scaleRate.y = (float)(((m_line_idx+1)*LINE_HEIGHT+1)>>2);
			Pos.x = (float)rect.left-2;
			Pos.y = (float)rect.top-3;
		}
		else
		{
			scaleRate.x = (float)((m_max_line_width+10)>>2);
			scaleRate.y = (float)(((m_line_idx+1)*LINE_HEIGHT+10)>>2);
			Pos.x = (float)rect.left-5;
			Pos.y = (float)rect.top-5;
		}
		
		m_bgImage.RenderSprite(&scaleRate,NULL,0,&Pos,RGBA_MERGE(m_imgColor, m_alpha * m_dwOptionAlpha / 100 ));
	}

	while(curLineNode)
	{
		rect.right = rect.left + 1;
		rect.bottom = rect.top + 1;

		CFONT_OBJ->RenderFont(m_wFontIdx,curLineNode->line,curLineNode->len,&rect,RGBA_MERGE(curLineNode->color, m_alpha * m_dwOptionAlpha / 100 ) );
		rect.top += LINE_HEIGHT;

		curLineNode = curLineNode->nextLine;
	}	
}

void cMultiLineText::SetText(char* text )
{
	if( text == NULL ) return;
	if( *text == 0 ) return;

	if( topLine != NULL ) Release();
	
	char * sp	= text;
	m_line_idx	= 0;
	m_max_line_width = 0; //KES 초기화

	LINE_NODE * curLineNode = topLine = new LINE_NODE;
	curLineNode->nextLine = NULL;
	curLineNode->color = m_fgColor;

	char * cur_line = topLine->line;

	while(*sp != 0)
	{
//		if(*sp & 0x80)
		if( IsDBCSLeadByte(*sp) )
		{
			*cur_line++ = *sp++;
			*cur_line++ = *sp++;
		}
		else
		{
			switch(*sp)
			{
			case TEXT_DELIMITER:
				{
					++sp;
					char tmp = *sp; //
					switch(tmp)
					{
					case TEXT_NEWLINECHAR:	// new line
						{
							*cur_line = 0;
							curLineNode->len = strlen(curLineNode->line);

							int real_len = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, curLineNode->line, curLineNode->len);
							if(m_max_line_width < real_len)
							{
								m_max_line_width = real_len;
							}
							m_line_idx++;
							//while(curLineNode && curLineNode->nextLine)
							//{
							//	curLineNode = curLineNode->nextLine;
							//}
							curLineNode = curLineNode->nextLine = new LINE_NODE;
							curLineNode->nextLine = NULL;
							cur_line = curLineNode->line;
							curLineNode->color = m_fgColor;
						}
						break;
					case TEXT_TABCHAR:
						{
						}
						break;
					case TEXT_SPACECHAR:
						{	
							*cur_line = ' ';
							++cur_line;
						}
						break;
					}// - switch()
				}
				break;

			default:
				{
					*cur_line = *sp;
					++cur_line;
				}
				break;
			}//- switch()
			++sp;
		}
	}
	
//	*cur_line = 0;	//KES 추가
	curLineNode->len = strlen(curLineNode->line);
	curLineNode->nextLine = NULL;
//	int real_len = CFONT_OBJ->GetTextExtent(m_wFontIdx, curLineNode->line, curLineNode->len);
	int real_len = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, curLineNode->line, curLineNode->len);
	if(m_max_line_width < real_len)
	{
		m_max_line_width = real_len;
	}

	m_fValid = TRUE;
}

void cMultiLineText::AddLine( char* text, DWORD dwColor )
{
	if( text == NULL ) return;

	char* sp	= text;

	LINE_NODE* pLineNode	= new LINE_NODE;
	pLineNode->nextLine		= NULL;
	pLineNode->color		= dwColor;
	strcpy( pLineNode->line, text );
	pLineNode->len			= strlen(pLineNode->line);
	m_fValid = TRUE;

	LINE_NODE* pTail		= topLine;
	if( pTail )
	{
		while( pTail->nextLine )
			pTail = pTail->nextLine;

		pTail->nextLine = pLineNode;
		++m_line_idx;
	}
	else
	{
		topLine = pLineNode;
	}

	int	real_len = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, pLineNode->line, pLineNode->len );

	if(m_max_line_width < real_len)
	{
		m_max_line_width = real_len;
	}
}

void cMultiLineText::AddNamePannel( DWORD dwLength )
{
	LINE_NODE* pLineNode	= new LINE_NODE;
	pLineNode->nextLine		= NULL;
	pLineNode->color		= 0xffffffff;
	pLineNode->len			= strlen(pLineNode->line);
	m_fValid = TRUE;

	LINE_NODE* pTail		= topLine;
	if( pTail )
	{
		while( pTail->nextLine )
			pTail = pTail->nextLine;

		pTail->nextLine = pLineNode;
		++m_line_idx;
	}
	else
	{
		topLine = pLineNode;
	}
	
	char buf[MAX_NAME_LENGTH+1];
	strncpy( buf, "AAAAAAAAAAAAAAAA", MAX_NAME_LENGTH );
	int real_len = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, buf, dwLength );

	if(m_max_line_width < real_len)
	{
		m_max_line_width = real_len;
	}

	m_bNamePannel = TRUE;
}

// 임시로 제작된 함수 //어디에 쓰는 물건인고...--;
BOOL cMultiLineText::SetText(VECTOR3 * pPos3, char * szText)
{	
	VECTOR3 OutPos3;
	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, pPos3, &OutPos3);
	if(OutPos3.x < 0 || OutPos3.x > 1 || OutPos3.y < 0 || OutPos3.y > 1)
		return FALSE;

	SetXY((long)(GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth*OutPos3.x-30), (long)(GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight*OutPos3.y));

	SetText(szText);

	return TRUE;
}
