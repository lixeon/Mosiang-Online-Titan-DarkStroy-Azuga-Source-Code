// TList.cpp: implementation of the CTList class.
//
//////////////////////////////////////////////////////////////////////

#include "TList.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTList::CTList():
m_CurPos(NULL),
m_hMainWnd(NULL),
m_hScrollWnd(NULL),
m_hMainDC(NULL),
m_dwListStyle(0),
m_BGColor(RGB(10,10,32)),
m_FGColor(RGB(128,128,255)),
m_hFont(NULL),
m_hBGPenInlt(NULL),
m_hBGPenInrb(NULL),
m_hBGPenOutlt(NULL),
m_hBGPenOutrb(NULL),
m_hBGBrush(NULL),
m_wLineHeight(20),
m_wLineMargin(0),
m_nlinePerScreen(0),
m_nScrollMaxPos(0),
m_nScrollPos(0),
m_x(0),
m_y(0),
m_width(0),
m_height(0),
m_Indention(5)
{
	
}

CTList::~CTList()
{
	DestoryList();
}

void CTList::DestoryList()
{
	ResetContent();
	m_MemoryPool.Release();

	if(m_hBGPenInlt)
		DeleteObject(m_hBGPenInlt);
	if(m_hBGPenOutlt)
		DeleteObject(m_hBGPenOutlt);
	if(m_hBGPenInrb)
		DeleteObject(m_hBGPenInrb);
	if(m_hBGPenOutrb)
		DeleteObject(m_hBGPenOutrb);

	if(m_hBGBrush)
		DeleteObject(m_hBGBrush);

	if(m_hMainDC)
		ReleaseDC(m_hMainWnd, m_hMainDC);
}
void CTList::ResetContent()
{
	LINENODE * pDelNode = NULL;
	while(pDelNode = DeleteHead())
	{
		m_MemoryPool.Free(pDelNode);
		pDelNode = NULL;
	}
}

BOOL CTList::DeleteString(char * pszString)
{
	LINENODE * pDelNode = NULL;
	if(pDelNode = DeleteHead())
	{
		if(pszString != NULL)
			strncpy(pszString, pDelNode->szLine, MAX_LINE_SIZE);

		m_MemoryPool.Free(pDelNode);
		pDelNode = NULL;
		return TRUE;
	}
	return FALSE;
}
char * CTList::GetString(LISTIDX index)
{
	if(index < (DWORD)GetCount())
		return GetAt(index)->szLine;
	return 0;
}
LISTIDX CTList::AddString(char * szString)
{
	return AddString(szString, m_FGColor);
}

LISTIDX CTList::AddString(char * szString, DWORD rgb)
{
	LINENODE * pNode = m_MemoryPool.Alloc();
	strncpy(pNode->szLine, szString, MAX_LINE_SIZE);
	pNode->rgb = rgb;

	AddTail(pNode);
	if(GetCount() > m_nlinePerScreen)
		m_nScrollPos = GetCount() - m_nlinePerScreen;
	UpdateScrollBar();
	UpdateScrollBarPos();
	Draw();
	return GetCount();
}

LISTIDX CTList::GetLineCount()
{
	return GetCount();
}
void CTList::SetListFirstPos(LISTIDX index)
{
	if(index == 0)
		m_CurPos = GetFirstPos();
	else
	{
		m_CurPos = GetIndexPos(index);
	}
}
LINENODE * CTList::GetListNextPos()
{
	return GetNextPos(m_CurPos);
}
void CTList::UpdateScrollBarPos()
{
	SetScrollPos(m_hScrollWnd, SB_CTL, m_nScrollPos, TRUE);
}
void CTList::UpdateScrollBar()
{
	if(m_dwListStyle & TL_DISABLENOSCROLL)
	{	
		if( GetCount() > m_nlinePerScreen )
		{	
			if(!IsWindowVisible(m_hScrollWnd))
				ShowWindow(m_hScrollWnd, TRUE);
			m_nScrollMaxPos = GetCount() - m_nlinePerScreen;
		}
		else
		{
			if(IsWindowVisible(m_hScrollWnd))
				ShowWindow(m_hScrollWnd, FALSE);

			m_nScrollMaxPos = 0;
			m_nScrollPos = 0;
			SetScrollPos(m_hScrollWnd, SB_CTL, m_nScrollPos, TRUE);
		}
		SetScrollRange(m_hScrollWnd, SB_CTL, 0, m_nScrollMaxPos, TRUE);
	}
	else
	{
		if( GetCount() > m_nlinePerScreen )
		{
			m_nScrollMaxPos = GetCount() - m_nlinePerScreen;
			SetScrollRange(m_hScrollWnd, SB_CTL, 0, m_nScrollMaxPos, TRUE);
		}
		else
		{
			m_nScrollMaxPos = 1;
			SetScrollRange(m_hScrollWnd, SB_CTL, 0, m_nScrollMaxPos, TRUE);
		}
		
	}
}

void CTList::SetBGColor(BGCOLOR color)
{
	m_BGColor = color;
}
void CTList::SetLineHeightMargin(WORD mg)
{
	m_wLineMargin = mg;
	UpdateSize();
}
void CTList::SetFGColor(FGCOLOR color)
{
	m_FGColor = color;
}
void CTList::CreateFont()
{
	m_wLineHeight = NAIVEFONT_LINEHEIGHT;
	UpdateSize();
}

void CTList::UpdateSize()
{
	m_nlinePerScreen = ( m_height - FRAMEBORDER * 2 ) / (m_wLineHeight+m_wLineMargin);
	UpdateScrollBar();
}

void CTList::SetFrameEdge()
{
	if(m_hBGPenInlt)
		DeleteObject(m_hBGPenInlt);
	if(m_hBGPenOutlt)
		DeleteObject(m_hBGPenOutlt);
	if(m_hBGPenInrb)
		DeleteObject(m_hBGPenInrb);
	if(m_hBGPenOutrb)
		DeleteObject(m_hBGPenOutrb);

	if(m_hBGBrush)
		DeleteObject(m_hBGBrush);

	m_hBGPenInlt = CreatePen(PS_SOLID, 1, FRAMELEFTTOP_INCOLOR);
	m_hBGPenOutlt = CreatePen(PS_SOLID, 1, FRAMELEFTTOP_OUTCOLOR);
	m_hBGPenInrb = CreatePen(PS_SOLID, 1, FRAMERIGHTBOTTOM_INCOLOR);
	m_hBGPenOutrb = CreatePen(PS_SOLID, 1, FRAMERIGHTBOTTOM_OUTCOLOR);

	m_hBGBrush = CreateSolidBrush(m_BGColor);
}
void CTList::CreateFont(LPLOGFONT pLogFont)
{
	if(m_dwListStyle & TL_NAIVEFONT) return ;
	
	if(m_hFont) DeleteObject(m_hFont);
	m_hFont = CreateFontIndirect(pLogFont);
	m_wLineHeight = (WORD)pLogFont->lfHeight;
	
	UpdateSize();
}

void CTList::SetPos(int x, int y)
{
	m_x					= x;
	m_y					= y;

	
	if(m_dwListStyle & TL_SIMPLEFRAME)
	{
		// valid list rect
		m_WindowRect.left	= x+FRAMEBORDER_HALF;
		m_WindowRect.top	= y+FRAMEBORDER_HALF;
		m_WindowRect.right	= m_WindowRect.left + m_width - 2*FRAMEBORDER_HALF - SCROLLWIDTH;
		m_WindowRect.bottom = m_WindowRect.top + m_height - 2*FRAMEBORDER_HALF;
	}
	else
	{
		// valid list rect
		m_WindowRect.left	= x+FRAMEBORDER;
		m_WindowRect.top	= y+FRAMEBORDER;
		m_WindowRect.right	= m_WindowRect.left + m_width - 2*FRAMEBORDER - SCROLLWIDTH;
		m_WindowRect.bottom = m_WindowRect.top + m_height - 2*FRAMEBORDER;
	}

	int scrollX			= 0;
	int scrollY			= 0;
	int scrollWidth		= 0;
	int scrollHeight	= 0;
	GetScrollWindowPos(scrollX, scrollY, scrollWidth, scrollHeight);
	MoveWindow(m_hScrollWnd, scrollX, scrollY, scrollWidth, scrollHeight, TRUE);

	UpdateSize();
	Draw();
}

void CTList::SetSize(int width, int height)
{
	m_width				= width;
	m_height			= height;

	if(m_dwListStyle & TL_SIMPLEFRAME)
	{
		m_WindowRect.right	= m_WindowRect.left + m_width - 2*FRAMEBORDER_HALF - SCROLLWIDTH;
		m_WindowRect.bottom = m_WindowRect.top + m_height - 2*FRAMEBORDER_HALF;
	}
	else
	{
		m_WindowRect.right	= m_WindowRect.left + m_width - 2*FRAMEBORDER - SCROLLWIDTH;
		m_WindowRect.bottom = m_WindowRect.top + m_height - 2*FRAMEBORDER;
	}


	int scrollX			= 0;
	int scrollY			= 0;
	int scrollWidth		= 0;
	int scrollHeight	= 0;
	GetScrollWindowPos(scrollX, scrollY, scrollWidth, scrollHeight);
	MoveWindow(m_hScrollWnd, scrollX, scrollY, scrollWidth, scrollHeight, TRUE);
	
	UpdateSize();
	Draw();
}

void CTList::GetScrollWindowPos(int& x, int& y, int& width, int& height)
{
	if(m_dwListStyle & TL_SIMPLEFRAME)
	{
		x			= m_x + m_width - SCROLLWIDTH;
		y			= m_y + FRAMEBORDER_HALF;
		width		= SCROLLWIDTH;
		height		= m_height - 2*FRAMEBORDER_HALF;
	}
	else
	{
		x			= m_x + m_width - SCROLLWIDTH;
		y			= m_y + FRAMEBORDER;
		width		= SCROLLWIDTH;
		height		= m_height - 2*FRAMEBORDER;
	}
}

void CTList::CreateList(DWORD dwStyle, int x, int y, int width, int height, DWORD MaxLine, DWORD ExLine, HWND hWnd, HINSTANCE hInst)
{
	m_MemoryPool.Init(MaxLine, ExLine);
	m_hMainWnd				= hWnd;
	m_hMainDC				= GetDC(m_hMainWnd);
	m_dwListStyle			= dwStyle;

	SetPos(x, y);
	SetSize(width, height);

	if(m_dwListStyle & TL_NAIVEFONT)
	{		
		CreateFont();
	}

	SetBGColor(m_BGColor);
	SetFGColor(m_FGColor);
	SetFrameEdge();

	int scrollX			= 0;
	int scrollY			= 0;
	int scrollWidth		= 0;
	int scrollHeight	= 0;
	GetScrollWindowPos(scrollX, scrollY, scrollWidth, scrollHeight);
	m_hScrollWnd = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, 
								scrollX, scrollY, scrollWidth, scrollHeight, hWnd, NULL, hInst, NULL);


	UpdateSize();

	SetScrollPos(m_hScrollWnd, SB_CTL, m_nScrollPos, TRUE);

	Draw();
	OnDraw();
}

void CTList::Draw()
{
	m_bRedraw = TRUE;
}

void CTList::DrawFrame()
{
	if(m_dwListStyle & TL_SIMPLEFRAME)
	{
		// inner bg
		SelectObject(m_hMainDC, m_hBGBrush);
		Rectangle(m_hMainDC, m_WindowRect.left-FRAMEBORDER_HALF, m_WindowRect.top-FRAMEBORDER_HALF, m_WindowRect.right+FRAMEBORDER_HALF, m_WindowRect.bottom+FRAMEBORDER_HALF);
	}
	else
	{
		// outline frame
		//SelectObject(m_hMainDC, m_hBGPenOut);
		RectangleEx(m_WindowRect.left-FRAMEBORDER, m_WindowRect.top-FRAMEBORDER, m_WindowRect.right+FRAMEBORDER, m_WindowRect.bottom+FRAMEBORDER, m_hBGPenOutlt, m_hBGPenOutrb);

		// inner bg
		SelectObject(m_hMainDC, m_hBGBrush);
		Rectangle(m_hMainDC, m_WindowRect.left-FRAMEBORDER_HALF, m_WindowRect.top-FRAMEBORDER_HALF, m_WindowRect.right+FRAMEBORDER, m_WindowRect.bottom+FRAMEBORDER);

		// inline frame
		//SelectObject(m_hMainDC, m_hBGPenIn);
		RectangleEx(m_WindowRect.left-FRAMEBORDER_HALF, m_WindowRect.top-FRAMEBORDER_HALF, m_WindowRect.right+FRAMEBORDER_HALF, m_WindowRect.bottom+FRAMEBORDER_HALF, m_hBGPenInlt, m_hBGPenInrb);

	}
}
void CTList::RectangleEx(int left, int top, int right, int bottom, HPEN Penlt, HPEN Penrb)
{
	SelectObject(m_hMainDC, Penlt);
	// left
	MoveToEx(m_hMainDC, left, top, NULL); 
	LineTo(m_hMainDC, left, bottom); 
	// top
	MoveToEx(m_hMainDC, left, top, NULL); 
	LineTo(m_hMainDC, right, top); 

	SelectObject(m_hMainDC, Penrb);
	// right
	MoveToEx(m_hMainDC, right, top, NULL); 
	LineTo(m_hMainDC, right, bottom); 
	// bottom
	MoveToEx(m_hMainDC, left, bottom, NULL); 
	LineTo(m_hMainDC, right+1, bottom); 
}

BOOL CTList::OnDraw(BOOL bInstance)
{
	if(!bInstance)
	{
		if(!m_bRedraw)
			return FALSE;
	}

	m_bRedraw = FALSE;
	
	HFONT oldFont;

	DrawFrame();

	SetTextColor(m_hMainDC, m_FGColor);
	SetBkColor(m_hMainDC, m_BGColor);

	if(!(m_dwListStyle & TL_NAIVEFONT))
	{
		oldFont = (HFONT)SelectObject(m_hMainDC, m_hFont);
	}
	
	POS pos;
	LINENODE * pNode;
	int xPos = 0;
	int nTLine = GetCount();
	pos = GetIndexPos(m_nScrollPos);
	for(int i = m_nScrollPos, j = 0 ; i < m_nScrollPos+m_nlinePerScreen && i < nTLine ; ++i, ++j)
	{
		pNode = GetNextPos(pos);
		
		int lineH = (m_wLineHeight+m_wLineMargin);
		int YPOS = m_WindowRect.top + lineH * j;
		
		if(m_dwListStyle & TL_LINENUMBER)
		{
			xPos = m_WindowRect.left + m_Indention;
			sprintf(m_tmpEndLine, "%d", i);
			TextOut(m_hMainDC, xPos, YPOS, m_tmpEndLine, strlen(m_tmpEndLine));
			xPos += 43;
		}
		else
		{
			xPos = m_WindowRect.left + m_Indention;	
		}

		if(!(m_dwListStyle & TL_NAIVEFONT))
		{
			SetTextColor(m_hMainDC, pNode->rgb);
		}

		sprintf(m_tmpEndLine, "%s", pNode->szLine);
		ExtTextOut(m_hMainDC, xPos, YPOS, ETO_CLIPPED, &m_WindowRect, m_tmpEndLine, strlen(m_tmpEndLine), 0);

		if(!(m_dwListStyle & TL_NAIVEFONT))
		{
			SetTextColor(m_hMainDC, m_FGColor);
		}
		
	}


	if(!(m_dwListStyle & TL_NAIVEFONT))
	{
		SelectObject(m_hMainDC, oldFont);
	}
	return TRUE;
}

void CTList::MessageProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_HSCROLL:
	case WM_VSCROLL:
		switch(LOWORD(wParam))
		{
		case SB_LINEUP:
			{
				--m_nScrollPos;
			}
			break;
		case SB_LINEDOWN:
			{
				++m_nScrollPos;
			}
			break;
		case SB_PAGEUP:
			{
				--m_nScrollPos;
			}
			break;
		case SB_PAGEDOWN:
				++m_nScrollPos;
			break;
		case SB_THUMBPOSITION:
			{
				m_nScrollPos = HIWORD(wParam);
			}
			break;
		case SB_THUMBTRACK:
			{
				if(!(TL_DISABLESCROLLTUMBTRACK & m_dwListStyle))
				{
					m_nScrollPos = HIWORD(wParam);
				}
			}
			break;
		}

		m_nScrollPos = max(0, min(m_nScrollPos, m_nScrollMaxPos));
		UpdateScrollBarPos();

		if(m_dwListStyle & TL_SCROLLTUMBTRACKUPDATE)
			OnDraw(TRUE);
			//InvalidateRect(m_hMainWnd, &m_WindowRect, FALSE);
		else
			Draw();
		break;
		
	}
	
}
