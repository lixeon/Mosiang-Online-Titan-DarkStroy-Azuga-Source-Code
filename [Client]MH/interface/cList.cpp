// cList.cpp: implementation of the cList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cList.h"
#include "cFont.h"
#include "cButton.h"

#include "../Input/Mouse.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


cList::cList()
{
	m_lLineHeight=LINE_HEIGHT;		//default
	m_lLineNum=0;
	m_lTopListItemIdx=0;
	m_wMaxLine = 0;
	m_type = WT_LIST;
	m_fUpwardLine = TRUE;
	memset(&m_textClippingRect, 0, sizeof(RECT));
	m_pUpwardBtn = m_pDownwardBtn = NULL;
	m_fBarDrag = FALSE;
	m_dragGabY = 0;
}

cList::~cList()
{
	SAFE_DELETE(m_pUpwardBtn);
	SAFE_DELETE(m_pDownwardBtn);
}


void cList::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cWindow::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_LIST;
}
void cList::InitList(WORD wMaxLine, RECT * textClippingRect)
{
	SetMargin(textClippingRect);
	m_wMaxLine = wMaxLine;
	Resize();
}

void cList::SetMargin(RECT * textClippingRect)
{
	CopyRect(&m_textClippingRect, textClippingRect);
}
void cList::Resize()
{
	m_lLineHeight = LINE_HEIGHT;
	m_lLineNum = (m_textClippingRect.bottom - m_textClippingRect.top)/m_lLineHeight;
}
void cList::SetListBar(LONG x, LONG y, WORD wid, WORD hei, int interval)
{
	m_barListPos.x = (float)x;
	m_barListPos.y = (float)y;
	m_barListSize.w = wid;
	m_barListSize.h = hei;
	m_interval = interval - m_barListSize.h;
	
}

BOOL cList::PtInListBar(LONG x, LONG y)
{
	if(m_absPos.x+m_barListPos.x > x || m_absPos.y+m_barListPos.y+m_dragGabY > y || m_absPos.x+m_barListPos.x+m_barListSize.w < x || m_absPos.y+m_barListPos.y+m_barListSize.h+m_dragGabY < y)
		return FALSE;
	else
		return TRUE;
}
DWORD cList::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;

//	BOOL LeftPress = mouseInfo->LButtonPressed();
	long x = mouseInfo->GetMouseX();
	long y = mouseInfo->GetMouseY();

//KES SMI 031008	
	long x2 = mouseInfo->GetMouseEventX();
	long y2 = mouseInfo->GetMouseEventY();
//

	if(m_pUpwardBtn)
	we = m_pUpwardBtn->ActionEvent(mouseInfo);
	if(we & WE_BTNCLICK)
		OnUpwardItem();
	if(m_pDownwardBtn)
	we = m_pDownwardBtn->ActionEvent(mouseInfo);
	if(we & WE_BTNCLICK)
		OnDownwardItem();

	we = cWindow::ActionEvent( mouseInfo );
	
	//KES if문을 바꾸어야... 클릭되었을때 드래그시작.. 클릭을 놓았을때 드래그 끝..	
/*
	if(LeftPress && mouseInfo->LButtonDrag())
	{
		if(PtInListBar(x, y))
			if(!m_fBarDrag)
				m_fBarDrag = TRUE;
	}
	else
		m_fBarDrag = FALSE;
*/	
//	if(LeftPress && mouseInfo->LButtonDrag())

//	if( !WINDOWMGR->IsMouseDownUsed() )
	if( we & WE_LBTNCLICK )	
//	if( mouseInfo->LButtonDown() )
	{
		if(PtInListBar(x2, y2) )
			m_fBarDrag = TRUE;
	}
	
	if( mouseInfo->LButtonUp() )
		m_fBarDrag = FALSE;


	if(m_fBarDrag)
	{
		m_dragGabY = (LONG)((y - m_barListPos.y-m_absPos.y - (m_barListSize.h>>1)));
		if(m_dragGabY < 0)
			m_dragGabY = 0;
		else if(m_dragGabY > m_interval)
			m_dragGabY = m_interval;
		
		LONG cnt = GetItemCount();
		int itemCnt = cnt - m_lLineNum;
		if(itemCnt > 0)
		{
			 int itemMoveUnit = m_interval/((itemCnt<<1)+1);
			 m_lTopListItemIdx = m_dragGabY/itemMoveUnit;
			 if(itemCnt < m_lTopListItemIdx)
				 m_lTopListItemIdx = itemCnt;
		}
	}

	return we;
}
void cList::Render()
{
	static VECTOR2 m_tmpPos;
	cWindow::Render();			//component render
	if(m_pUpwardBtn)
	m_pUpwardBtn->Render();
	if(m_pDownwardBtn)
	m_pDownwardBtn->Render();

	
	m_tmpPos.x = m_barListPos.x + m_absPos.x;
	m_tmpPos.y = m_barListPos.y + m_absPos.y + m_dragGabY;
	m_BarImage.RenderSprite(NULL,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	
	
	// List Text Display //////////////////////////////////////////////////////////////

	PTRLISTPOS pos = m_ListItem.FindIndex(m_lTopListItemIdx);
	ITEM* item = NULL;
	LONG i = 0;
	while(pos)
	{
		item = (ITEM*)m_ListItem.GetNext(pos);
		if(i < m_lLineNum)
		{
			RECT rect={(LONG)m_absPos.x+m_textClippingRect.left, (LONG)m_absPos.y+m_textClippingRect.top + (m_lLineHeight*i), 1,1};
			rect.right = rect.left + 1;
			rect.bottom = rect.top + 1;
			CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
		}
		i++;
	}
/*	
	POS pos = m_pItemList->GetIndexPos(m_lTopListItemIdx);
	ITEM * item = NULL;
	LONG i = 0;
	while (pos)
    {
        item = m_pItemList->GetNextPos(pos);
		if(i < m_lLineNum)
		{
			RECT rect={(LONG)m_absPos.x+m_textClippingRect.left, (LONG)m_absPos.y+m_textClippingRect.top + (m_lLineHeight*i), 1,1};
			rect.right = rect.left + 1;
			rect.bottom = rect.top + 1;
			CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
		}
		i++;
	}
*/	
	////////////////////////////////////////////////////////////////////////////////////
}

void cList::Add(cWindow * btn)
{
	if(btn->GetType() == WT_BUTTON)
	{
		if(!m_pUpwardBtn)
		{
			m_pUpwardBtn = (cButton *)btn;
			m_pUpwardBtn->SetAbsXY((LONG)(m_absPos.x+m_pUpwardBtn->m_relPos.x), (LONG)(m_absPos.y+m_pUpwardBtn->m_relPos.y));
			m_pUpwardBtn->SetParent(this);
		}
		else if(!m_pDownwardBtn)
		{
			m_pDownwardBtn = (cButton *)btn;
			m_pDownwardBtn->SetAbsXY((LONG)(m_absPos.x+m_pDownwardBtn->m_relPos.x), (LONG)(m_absPos.y+m_pDownwardBtn->m_relPos.y));
			m_pDownwardBtn->SetParent(this);
		}
	}
}
void cList::SetAbsXY(LONG x, LONG y)
{
	cWindow::SetAbsXY(x,y);
	if(m_pUpwardBtn)
	m_pUpwardBtn->SetAbsXY(x+(LONG)m_pUpwardBtn->GetRelX(), y+(LONG)m_pUpwardBtn->GetRelY());
	if(m_pDownwardBtn)
	m_pDownwardBtn->SetAbsXY(x+(LONG)m_pDownwardBtn->GetRelX(), y+(LONG)m_pDownwardBtn->GetRelY());
}


void cList::OnUpwardItem() 
{ 
	LONG cnt = GetItemCount();
	if(cnt < m_lLineNum) return;
	if(m_lTopListItemIdx < 1) return;
	m_lTopListItemIdx--;
}
void cList::OnDownwardItem() 
{ 
	LONG cnt = GetItemCount();
	if(cnt < m_lLineNum) return;
	if((cnt-m_lTopListItemIdx-1) < m_lLineNum) return;
	m_lTopListItemIdx++;
}

