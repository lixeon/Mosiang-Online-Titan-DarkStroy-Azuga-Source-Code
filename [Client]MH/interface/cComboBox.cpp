// cComboBox.cpp: implementation of the cComboBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cComboBox.h"
#include "cWindowManager.h"
#include "cPushupButton.h"
#include "cFont.h"
#include "../Input/Mouse.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cComboBox::cComboBox()
{
	m_pComboBtn = NULL;
	m_wFontIdx = 0;
	m_ComboTextColor = RGB_HALF(255,255,255);
	memset(m_szComboText, 0, MAX_COMBOTEXT_SIZE);
	m_type = WT_COMBOBOX;
	m_topHeight=0;
	m_middleHeight=0;
	m_downHeight=0;
	m_listWidth = 0;
	SetRect(&m_textClippingRect, 3,4,0,0);
	
	m_nCurSelectedIdx = -1;
	m_OverImageScale.x = m_OverImageScale.y = 1;
	m_nOverIdx = -1;
}

cComboBox::~cComboBox()
{
	SAFE_DELETE(m_pComboBtn);
}

void cComboBox::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cbFUNC Func, LONG ID)
{
	cWindow::Init(x,y,wid,hei,basicImage,ID);
	if(Func)
		cbWindowFunc = Func;

}
void cComboBox::InitComboList(WORD wid, cImage * topImage, WORD topHei, cImage * middleImage, WORD middleHei, cImage * downImage, WORD downHei, cImage * overImage )
{
	m_listWidth=wid;
	m_TopImage=*topImage;				m_topHeight=topHei;
	m_MiddleImage=*middleImage;			m_middleHeight=middleHei;
	m_DownImage=*downImage;				m_downHeight=downHei;
	m_OverImage = *overImage;
}
DWORD cComboBox::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;

	we = cWindow::ActionEvent( mouseInfo );
	if( m_bDisable ) return we;

	if( we & WE_LBTNCLICK )	//
	{
		DWORD we2 = m_pComboBtn->ActionEvent(mouseInfo);

		if(we2 & WE_PUSHDOWN)
		{
			cbWindowFunc(m_ID, m_pParent, WE_PUSHDOWN);	//PPP
			return we;
		}
		else if(we2 & WE_PUSHUP)
		{
			cbWindowFunc(m_ID, m_pParent, WE_PUSHUP);	//PPP
			return we;
		}
	}
	
	if(m_pComboBtn->IsPushed())
	{
		ListMouseCheck( mouseInfo );
	}
	
	return we;
}


void cComboBox::ListMouseCheck( CMouse * mouseInfo )
{
	LONG x = mouseInfo->GetMouseX();
	LONG y = mouseInfo->GetMouseY();

	BOOL LeftDown = mouseInfo->LButtonDown();
	
	if( !WINDOWMGR->IsMouseOverUsed() )
	{
		m_nOverIdx = PtIdxInComboList(x, y);
		
		if( m_nOverIdx > GetItemCount() )
			m_nOverIdx = -1;
		else
			WINDOWMGR->SetMouseOverUsed();
	}
	else
	{
		m_nOverIdx = -1;
	}
	
	if( LeftDown )
	{
		LONG x2 = mouseInfo->GetMouseEventX();
		LONG y2 = mouseInfo->GetMouseEventY();

		m_nCurSelectedIdx = PtIdxInComboList(x2, y2);
		
		if(m_nCurSelectedIdx > GetItemCount() || m_nCurSelectedIdx == -1 )
		{
			m_pComboBtn->SetPush(FALSE);
			m_nCurSelectedIdx = -1;
		}
		else if( !WINDOWMGR->IsMouseDownUsed() )
		{
			m_pComboBtn->SetPush(FALSE);
			m_nOverIdx = -1;
			
			PTRLISTPOS p = m_ListItem.FindIndex(m_nCurSelectedIdx);
			ITEM * item = (ITEM*)m_ListItem.GetAt(p);
			if( item )
				strcpy(m_szComboText, item->string);
			// 리스트 선택 콜백
			cbWindowFunc(m_ID, m_pParent, WE_COMBOBOXSELECT);	//PPP

			WINDOWMGR->SetMouseDownUsed();
		}
	}
}

WORD cComboBox::PtIdxInComboList(LONG x, LONG y)
{
	int listnum = GetItemCount();
	for(int i = 0 ; i < listnum ; i++)
	{
		if(m_absPos.x < x && m_absPos.y+m_height < y && x < m_absPos.x+m_listWidth && y < m_absPos.y+m_height+(i+1)*m_middleHeight)
			return i;
	}

	return listnum+1;
}

void cComboBox::Render()
{
	cWindow::Render();
	m_pComboBtn->Render();

	if(m_pComboBtn->IsPushed())
	{
		// 리스트 보임
		int listnum = GetItemCount();
		switch(listnum)
		{
		case 0:
			break;
		case 1:
			{
				VECTOR2 start_pos;
				start_pos.x = m_absPos.x;
				start_pos.y = m_absPos.y+m_height;
				m_DownImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
			}
			break;
		default:
			{
				VECTOR2 start_pos;
				start_pos.x = m_absPos.x;
				start_pos.y = m_absPos.y+m_height;
				m_TopImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
				for(int i = 0 ; i < listnum - 2 ; i++)
				{
					start_pos.y += m_middleHeight;
					m_MiddleImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
				}
				start_pos.y += m_middleHeight;
				m_DownImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));

			}
			break;

		}
		
		VECTOR2 m_tmpPos;
		// Over Image Display 
		if( m_nOverIdx != -1 )
		{
			m_tmpPos.x = m_absPos.x + 2;
			m_tmpPos.y = m_absPos.y + (((float)m_nOverIdx+1)*m_middleHeight) + 1;
			m_OverImage.RenderSprite(&m_OverImageScale,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		}
		//


		PTRLISTPOS pos = m_ListItem.GetHeadPosition();
		ITEM* item = NULL;
		LONG i = 0;

		while(pos)
		{
			item = (ITEM*)m_ListItem.GetNext(pos);
			RECT rect={(LONG)m_absPos.x+m_textClippingRect.left, (LONG)m_absPos.y+m_height+m_textClippingRect.top+(m_middleHeight*i), 1,1};
			//rect.right = rect.left + 1;
			//rect.bottom = rect.top + 1;
			CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
			i++;
		}
		// 항목 선택 bg

/* 이전 사항
		POS pos = m_pItemList->GetFirstPos();
		ITEM * item = NULL;
		LONG i = 0;
		while (pos)
		{
			item = m_pItemList->GetNextPos(pos);
			RECT rect={(LONG)m_absPos.x+m_textClippingRect.left, (LONG)m_absPos.y+m_height+m_textClippingRect.top+(m_middleHeight*i), 1,1};
			//rect.right = rect.left + 1;
			//rect.bottom = rect.top + 1;
			CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
			i++;
		}
		// 항목 선택 bg
*/
	}



	RECT rect={(LONG)m_absPos.x+5, (LONG)m_absPos.y+6, 1,1};
	//rect.right = rect.left + 1;
	//rect.bottom = rect.top + 1;
	CFONT_OBJ->RenderFont(m_wFontIdx,m_szComboText,lstrlen(m_szComboText),&rect,RGBA_MERGE(m_ComboTextColor, m_alpha * m_dwOptionAlpha / 100));
}

void cComboBox::Add(cWindow * pushupBtn)
{
	if(pushupBtn->GetType() == WT_PUSHUPBUTTON)
	{
		if(pushupBtn == (cWindow *)0x00000001)
		{ASSERT(0);}
		m_pComboBtn = (cPushupButton *)pushupBtn;
		m_pComboBtn->SetAbsXY((LONG)(m_absPos.x+m_pComboBtn->m_relPos.x), (LONG)(m_absPos.y+m_pComboBtn->m_relPos.y));
		m_pComboBtn->SetParent(this);
	}
}

void cComboBox::SetAbsXY(LONG x, LONG y)
{
	cWindow::SetAbsXY(x,y);
	if(m_pComboBtn)
	m_pComboBtn->SetAbsXY((LONG)m_absPos.x+(LONG)m_pComboBtn->GetRelX(), (LONG)m_absPos.y+(LONG)m_pComboBtn->GetRelY());
}
void cComboBox::SetMargin(RECT textClippingRect)
{
	m_textClippingRect = textClippingRect;
}
