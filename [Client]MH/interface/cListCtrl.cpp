// cListCtrl.cpp: implementation of the cListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cListCtrl.h"
#include "cFont.h"
#include "../Input/Mouse.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
cListCtrl::cListCtrl()
{
	m_bHeadLineHeight=0;
	m_bBodyLineHeight=0;
	m_wLinePerPage = 0;
	SetRect(&m_textRelRect, 3,4,0,0);
	m_type = WT_LISTCTRL;
	m_SelectedRowIdx = -1;

	m_lTopListItemIdx = 0;

	m_OverImageScale.x = 1;
	m_OverImageScale.y = 1;
	
	m_nOverIdx = -1;
	m_fGap = 0.0f;
	m_wSelectOption = 0;
}

cListCtrl::~cListCtrl()
{

}

void cListCtrl::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cWindow::Init(x,y,wid,hei,basicImage,ID);
}

void cListCtrl::InitListCtrl(WORD wMaxColumns, WORD wLinePerPage)
{
	m_wLinePerPage = wLinePerPage;
}

void cListCtrl::InitListCtrlImage(cImage * HeadImage, BYTE HHeight, cImage * BodyImage, BYTE BHeight, cImage * OverImage )
{
	m_HeadImage=*HeadImage;
	m_BodyImage=*BodyImage;
	m_bHeadLineHeight=HHeight;
	m_bBodyLineHeight=BHeight;
	m_OverImage = *OverImage;

	if( m_bBodyLineHeight > 10)
		m_fGap = ((float)m_bBodyLineHeight-10.0f)/2.0f;
}
void cListCtrl::Render()
{
	VECTOR2 m_tmpPos = m_absPos;
	m_HeadImage.RenderSprite(NULL,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	m_tmpPos.y += m_bHeadLineHeight;

	for(int k = 0 ; k < m_wLinePerPage ; k++)
	{
		m_BodyImage.RenderSprite(NULL,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		m_tmpPos.y += m_bBodyLineHeight;
	}

	if( m_wSelectOption == 0 )
	{
		// Over Image Display 
		if( m_nOverIdx != -1 )
		{
			m_tmpPos.x = m_absPos.x;
			m_tmpPos.y = m_absPos.y + m_textRelRect.top + m_bHeadLineHeight + (((float)m_nOverIdx)*m_bBodyLineHeight ) - m_fGap;
			m_OverImage.RenderSprite(&m_OverImageScale,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		}
	}
	else if( m_wSelectOption == 1 )
	{
		if( m_nOverIdx != 0 )
		{
			m_tmpPos.x = m_absPos.x;
			m_tmpPos.y = m_absPos.y + m_textRelRect.top + m_bHeadLineHeight + (((float)m_SelectedRowIdx)*m_bBodyLineHeight ) - m_fGap;
			m_OverImage.RenderSprite(&m_OverImageScale,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		}
	}

	// HEAD TEXT DISPLAY //////////////////////////////////////////////////////////////
	RECT headRect = {(LONG)m_absPos.x+m_textRelRect.left, (LONG)m_absPos.y+m_textRelRect.top,0,0};
	for(int l = 0 ; l < m_Cols ; l++)
	{
		headRect.right = headRect.left + 1;
		headRect.bottom = headRect.top + 1;
		CFONT_OBJ->RenderFont(m_wFontIdx,pszColumnHeader[l],lstrlen(pszColumnHeader[l]),&headRect,RGBA_MERGE(pColumnTextColor[l], m_alpha * m_dwOptionAlpha / 100));		
		headRect.left += pColumnSize[l];
	}
	
	// BODY TEXT DISPLAY //////////////////////////////////////////////////////////////
	POS pos = m_pItemList->GetIndexPos(m_lTopListItemIdx);
	RITEM * ritem = NULL;
	LONG i = 0;
	while (pos)
    {
        ritem = m_pItemList->GetNextPos(pos);
		if(i < m_wLinePerPage)
		{
			RECT rect={(LONG)m_absPos.x+m_textRelRect.left, (LONG)m_absPos.y+m_bHeadLineHeight+m_textRelRect.top+(m_bBodyLineHeight*i), 0,0};
			for(int j = 0 ; j < m_Cols ; j++)
			{
				CFONT_OBJ->RenderFont(m_wFontIdx,ritem->pString[j],lstrlen(ritem->pString[j]),&rect,RGBA_MERGE(ritem->rgb[j], m_alpha * m_dwOptionAlpha / 100));
				//pGeometry->RenderFont(CFONT_OBJ->GetFont(m_wFontIdx),ritem->pString[j],lstrlen(ritem->pString[j]),&rect,RGBA_MERGE(ritem->rgb[j], m_alpha),CHAR_CODE_TYPE_ASCII,0,0);
				rect.left += pColumnSize[j];
			}
			i++;
		}
		else
			break;
	}
	////////////////////////////////////////////////////////////////////////////////////

	cWindow::Render();
}

DWORD cListCtrl::ActionEvent(CMouse * mouseInfo)
{
//KES INTERFACE 031009
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;

	LONG x = mouseInfo->GetMouseX();
	LONG y = mouseInfo->GetMouseY();

//	BOOL LeftUp = mouseInfo->LButtonUp();
	BOOL LeftDown = mouseInfo->LButtonDown();

	we = cWindow::ActionEvent(mouseInfo);

//	if( !WINDOWMGR->IsMouseOverUsed() )
	if( we & WE_MOUSEOVER )
	{
		m_nOverIdx = PtIdxInRow(x, y);
		if( m_nOverIdx > m_wLinePerPage || m_pItemList->GetCount() <= m_nOverIdx )
			m_nOverIdx = -1;
	}
	else
	{
		m_nOverIdx = -1;
	}

//	if( !WINDOWMGR->IsMouseDownUsed() )
	if( we & WE_LBTNCLICK )
	{
		LONG x = mouseInfo->GetMouseEventX();
		LONG y = mouseInfo->GetMouseEventY();

		int selIdx = PtIdxInRow(x, y);
		if(selIdx < m_wLinePerPage)
		{
			//RITEM * ritem = m_pItemList->GetAt(selIdx);
			//m_SelectedRowIdx = ritem->dwData[0];
			m_SelectedRowIdx = selIdx;
			cbWindowFunc(m_ID, m_pParent, WE_ROWCLICK);	//PPP
			we |= WE_ROWCLICK;
		}
		else
		{
			m_SelectedRowIdx = 0;
		}
	}
	else if( we & WE_LBTNDBLCLICK )
	{
		LONG x = mouseInfo->GetMouseEventX();
		LONG y = mouseInfo->GetMouseEventY();

		int selIdx = PtIdxInRow(x, y);
		if(selIdx < m_wLinePerPage)
		{
			//RITEM * ritem = m_pItemList->GetAt(selIdx);
			//m_SelectedRowIdx = ritem->dwData[0];
			m_SelectedRowIdx = selIdx;
			cbWindowFunc(m_ID, m_pParent, WE_ROWDBLCLICK);	//PPP
			we |= WE_ROWDBLCLICK;
		}
		else
		{
			m_SelectedRowIdx = 0;
		}
	}

	return we;
}

void cListCtrl::SetMargin(const RECT * textRelRect)
{
	CopyRect(&m_textRelRect, textRelRect);
}

WORD cListCtrl::PtIdxInRow(LONG x, LONG y)
{
	for(int i = 0 ; i < m_wLinePerPage ; i++)
	{
		if( m_absPos.x < x 
			&& m_absPos.y+m_bHeadLineHeight < y 
			&& x < m_absPos.x+m_width 
			&& y < m_absPos.y+m_bHeadLineHeight+(i+1)*m_bBodyLineHeight)
			return i;
	}

	return m_wLinePerPage+1;
}
/*
void cListCtrl::ListMouseCheck( LONG x, LONG y, DWORD we )
{
	int selIdx = PtIdxInRow(x, y);

	if(selIdx < m_wLinePerPage)
	{
		if(LeftUp)
		{
			//RITEM * ritem = m_pItemList->GetAt(selIdx);
			//m_SelectedRowIdx = ritem->dwData[0];
			m_SelectedRowIdx = selIdx;
			cbWindowFunc(m_ID, m_pParent, WE_ROWCLICK);	//PPP
			we |= WE_ROWCLICK;
		}
	}
}
*/
