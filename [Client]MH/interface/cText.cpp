// cText.cpp: implementation of the cText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cText.h"
#include "cFont.h"
#include "line_node.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cText::cText()
{
	m_textXY.x = 3;
	m_textXY.y = 3;

	//RECT	m_m_leftTopPos;
/*	m_line_num = 0;
	m_box_width = 0;
	m_box_height = 0;*/
	m_dwStartTime = 0;
	m_dwDisappearTime = 0;

	topLine = NULL;
}

cText::~cText()
{
	Release();
}

void cText::Release()
{
	line_node * curLineNode = topLine;
	line_node * tmpNode = NULL;
	while(curLineNode)
	{
		tmpNode = curLineNode;
		curLineNode = curLineNode->nextLine;
		SAFE_DELETE(tmpNode);
	}
	topLine = NULL;
}

DWORD cText::ActionEvent(CMouse * mouseInfo)
{
	if( !m_bActive ) return WE_NULL;

	if(GetTickCount() - m_dwStartTime <	m_dwDisappearTime)
	{
		m_bActive = TRUE;
	}
	else
	{
		m_dwStartTime = m_dwDisappearTime = 0;
		m_bActive = FALSE;
	}
	return cWindow::ActionEvent(mouseInfo);
}


void cText::Render()
{
	//cWindow::Render();			// 직접 구현

	if(!m_dwDisappearTime) return;
	if(!m_bActive) return;

	line_node * curLineNode = topLine;
	RECT rect = {(long)(m_absPos.x + m_textXY.x), (long)(m_absPos.y + m_textXY.y), 1, 1};
	while(curLineNode)
	{
		if(strcmp(curLineNode->line,"")!=0)
			CFONT_OBJ->RenderFont(m_wFontIdx,curLineNode->line,curLineNode->len,&rect,RGBA_MERGE(m_fgColor, m_alpha * m_dwOptionAlpha / 100));
		rect.top +=LINE_HEIGHT;
		curLineNode = curLineNode->nextLine;
	}
}

/*
void cText::SetText(char * text)
{
	
	if(*text == 0) return;
	
	char * sp = text;
	m_line_num = 0;
	line_node * curLineNode = topLine = new line_node;
	curLineNode->nextLine = NULL;
	char * cur_line = topLine->line;
	while(*sp != 0)
	{
		if(*sp & 0x80)
		{
			*cur_line++ = *sp++;
			*cur_line++ = *sp++;
		}
		else
		{
			switch(*sp)
			{
			case '^':
				{
					++sp;
					char tmp = *sp;
					switch(tmp)
					{
					case TEXT_NEWLINECHAR:	// new line
						{
							*cur_line = 0;
							curLineNode->len = strlen(curLineNode->line);
							int real_len = CFONT_OBJ->GetTextExtent(m_wFontIdx, curLineNode->line, curLineNode->len);
							if(m_line_width_max < real_len)
							{
								m_line_width_max = real_len;
							}
							m_line_num++;

							curLineNode = curLineNode->nextLine = new line_node;
							curLineNode->nextLine = NULL;
							cur_line = curLineNode->line;
						}
						break;
					case 't':
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
			}//- switch()
			++sp;
		}
	}
	curLineNode->len = strlen(curLineNode->line);
	int real_len = CFONT_OBJ->GetTextExtent(m_wFontIdx, curLineNode->line, curLineNode->len);
	if(m_line_width_max < real_len)
	{
		m_line_width_max = real_len;
	}

}
	*/
