// TList.h: interface for the CTList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLIST_H__A5865217_330E_4821_8A0B_0EDFB15D5F15__INCLUDED_)
#define AFX_TLIST_H__A5865217_330E_4821_8A0B_0EDFB15D5F15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cLinkedList.h"
#include "MiruquMemoryPool.h"



typedef DWORD BGCOLOR;
typedef DWORD FGCOLOR;
typedef DWORD LISTIDX;

enum eTLSTYLE
{
	TL_NAIVEFONT				= 0x00000001,
	TL_MULTILINE				= 0x00000002,		// not implemented
	TL_DISABLENOSCROLL			= 0x00000004,
	TL_LINENUMBER				= 0x00000008,
	TL_DISABLESCROLLTUMBTRACK	= 0x00000010,
	TL_SCROLLTUMBTRACKUPDATE	= 0x00000020,
	TL_SIMPLEFRAME				= 0x00000040,
};

#define NAIVEFONT_LINEHEIGHT		16
#define MAX_LINE_SIZE				256

#define FRAMEBORDER					2
#define FRAMEBORDER_HALF			1
#define SCROLLWIDTH					18

#define FRAMELEFTTOP_INCOLOR			RGB(64,64,64)
#define FRAMELEFTTOP_OUTCOLOR			RGB(128,128,128)

#define FRAMERIGHTBOTTOM_INCOLOR		RGB(212,208,200)
#define FRAMERIGHTBOTTOM_OUTCOLOR		RGB(255,255,255)


class LINENODE
{
public:
	char	szLine[MAX_LINE_SIZE];
	DWORD	rgb;
};

// 인덱스로 O(1)인 검색이 되고 
// 검색된 노드가 다음 노드를 링크 하고 있는 알고리즘 구현

class CTList : private cLinkedList<LINENODE>
{
	CMiruquMemoryPool<LINENODE> m_MemoryPool;
public:
	CTList();
	virtual ~CTList();

	void CreateList(DWORD dwStyle, int x, int y, int width, int height, DWORD MaxLine, DWORD ExLine, HWND hWnd, HINSTANCE hInst);
	
	// Data Manipulation Function
	LISTIDX AddString(char * szString, DWORD rgb);
	LISTIDX AddString(char * szString);
	BOOL	DeleteString(char * pszString);
	void	ResetContent();
	char *	GetString(LISTIDX index);
	LISTIDX GetLineCount();
	/*
	void	InsertString(LISTIDX index, char * szString);
	LISTIDX DeleteString(LISTIDX index);
	LISTIDX FindString(char * szString);
	LISTIDX RemoveString();
	LISTIDX GetCurSel();
	void SetCurSel(LISTIDX index);
	*/
	
	BOOL OnDraw(BOOL bInstance = FALSE);
	void Draw();

	// Style Function
	void SetPos(int x, int y);
	void SetSize(int width, int height);
	void SetLineHeightMargin(WORD mg);
	void SetBGColor(BGCOLOR color);
	void SetFGColor(FGCOLOR color);
	void DestoryList();
	void CreateFont(LPLOGFONT pLogFont);
	void CreateFont();
	
	void MessageProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	void SetListFirstPos(LISTIDX index=0);
	LINENODE * GetListNextPos();
private:
	void DrawFrame();
	void SetFrameEdge();
	void UpdateSize();
	void UpdateScrollBar();
	void UpdateScrollBarPos();
	void GetScrollWindowPos(int& x, int& y, int& width, int& height);
	void RectangleEx(int left, int top, int right, int bottom, HPEN Penlt, HPEN Penrb);

	POS			m_CurPos;
	char		m_tmpEndLine[MAX_LINE_SIZE];

	HWND		m_hMainWnd;
	HWND		m_hScrollWnd;

	DWORD		m_dwListStyle;
	
	WORD		m_wLineMargin;
	WORD		m_wLineHeight;

	HDC			m_hMainDC;
	HPEN		m_hBGPenInlt;
	HPEN		m_hBGPenInrb;
	HPEN		m_hBGPenOutlt;
	HPEN		m_hBGPenOutrb;
	HBRUSH		m_hBGBrush;
	BGCOLOR		m_BGColor;
	FGCOLOR		m_FGColor;
	HFONT		m_hFont;
	
	BOOL		m_bRedraw;

	WORD		m_nlinePerScreen;
	WORD		m_nScrollMaxPos;
	int			m_nScrollPos;

	int			m_x;
	int			m_y;
	int			m_width;
	int			m_height;
	RECT		m_WindowRect;
	int			m_Indention;
};

#endif // !defined(AFX_TLIST_H__A5865217_330E_4821_8A0B_0EDFB15D5F15__INCLUDED_)

