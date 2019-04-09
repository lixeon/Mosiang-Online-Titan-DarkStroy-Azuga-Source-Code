// cListDialog.cpp: implementation of the cListDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cListDialog.h"
#include "cButton.h"
#include "cGuageBar.h"
#include "cFont.h"
#include "../Input/Mouse.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cListDialog::cListDialog()
{
	m_lLineHeight		= LINE_HEIGHT;		//default
	m_lLineNum			= 0;
	m_lTopListItemIdx	= 0;
	memset(&m_textRelRect, 0, sizeof(RECT));
	m_minMiddleNum		= 0;
	m_maxMiddleNum		= 0;
	m_wMaxLine			= 50;
	m_middleNum			= 0;
	m_topHeight			= m_downHeight = m_middleHeight = 0;

	m_type				= WT_LISTDIALOG;
	
	m_pLengthLongBtn	= m_pLengthShortBtn = NULL;
	m_pUpwardBtn		= m_pDownwardBtn = NULL;
	m_pGuageBar			= NULL;

	m_SelectedRowIdx	= -1;
	m_ClickSelected		= -1;
	m_fGap				= 0.0f;

	m_bAutoScroll		= TRUE;
	m_bShowSelect		= FALSE;
}

cListDialog::~cListDialog()
{
}

void cListDialog::Add(cWindow * btn)
{
	if(btn->GetType() == WT_BUTTON)
	{
		if(!m_pUpwardBtn)
		{
			m_pUpwardBtn = (cButton *)btn;
		}
		else if(!m_pDownwardBtn)
		{
			m_pDownwardBtn = (cButton *)btn;
		}
		else if(!m_pLengthLongBtn)
		{
			m_pLengthLongBtn = (cButton *)btn;
		}
		else if(!m_pLengthShortBtn)
		{
			m_pLengthShortBtn = (cButton *)btn;
		}
	}
	else if(btn->GetType() == WT_GUAGEBAR)
	{
		m_pGuageBar = (cGuageBar *)btn;
	}

	cDialog::Add(btn);
}

void cListDialog::Init(LONG x, LONG y, WORD wid, cImage * topImage, WORD topHei, cImage * middleImage, WORD middleHei, cImage * downImage, WORD downHei, cImage * overImage, BYTE num, LONG ID)
{
	// #POINT에서 입력한 height는 사용하지 않는다
	m_TopImage		= *topImage; 
	m_MiddleImage	= *middleImage; 
	m_DownImage		= *downImage;
	m_OverImage		= *overImage;

	m_topHeight		= topHei; m_middleHeight = middleHei; m_downHeight = downHei;
	m_middleNum		= num;
	int hei			= topHei + downHei + ( num * middleHei );

	cDialog::Init(x,y,wid,hei,NULL,ID);
}

void cListDialog::InitList(WORD wMaxLine, RECT * textClippingRect)
{
	m_wMaxLine = wMaxLine;
	CopyRect(&m_textRelRect, textClippingRect);

	// m_middleNum는 Init()에서 셋팅된 값
	// InitList앞에 m_minMiddleNum, m_maxMiddleNum가 설정되어 있어야 한다
	Resize( m_middleNum );
}


BOOL cListDialog::Resize(BYTE middleNum)
{
	if(middleNum > m_maxMiddleNum) return FALSE;
	if(middleNum < m_minMiddleNum) return FALSE;

	int var_len = (middleNum - m_middleNum)*m_middleHeight;
	m_middleNum = middleNum;
	m_textRelRect.bottom += var_len;
	m_height = m_topHeight+m_downHeight+(m_middleNum*m_middleHeight);
//	m_lLineHeight = LINE_HEIGHT;		// 임시 텍스트 높이
	m_lLineNum = (m_textRelRect.bottom-m_textRelRect.top)/m_lLineHeight;
//	m_lLineNum = m_middleNum;
	return TRUE;
}

BOOL cListDialog::IsMaxLineOver()
{
	if(m_lLineNum > GetItemCount())
		return FALSE;
	else
		return TRUE;
}

DWORD cListDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;

	if( IsMaxLineOver() )
	{	
		if(m_pUpwardBtn)
		{
			m_pUpwardBtn->SetActive(TRUE);
		}
		if(m_pDownwardBtn)
		{
			m_pDownwardBtn->SetActive(TRUE);
		}

		if(m_pGuageBar)
		{
			m_pGuageBar->SetActive(TRUE);
			m_pGuageBar->ActionEvent(mouseInfo);
		}
			
	}
	else if( m_bAutoScroll )
	{
		if(m_pUpwardBtn)
		{
			m_pUpwardBtn->SetActive(FALSE);
		}
		if(m_pDownwardBtn)
		{
			m_pDownwardBtn->SetActive(FALSE);
		}
		if(m_pGuageBar)
		{
			m_pGuageBar->SetActive(FALSE);
		}
	}

	DWORD we2 = WE_NULL;

	if(m_pLengthLongBtn)
	{
		we2 = m_pLengthLongBtn->ActionEvent(mouseInfo);
		if(we2 & WE_BTNCLICK)
			onSize(TRUE);
	}
	if(m_pLengthShortBtn)
	{
		we2 = m_pLengthShortBtn->ActionEvent(mouseInfo);
		if(we2 & WE_BTNCLICK)
			onSize(FALSE);
	}

	if(m_pUpwardBtn)
	{
		we2 = m_pUpwardBtn->ActionEvent(mouseInfo);
		if(we2 & WE_BTNCLICK)
			OnUpwardItem();
	}

	if(m_pDownwardBtn)
	{
		we2 = m_pDownwardBtn->ActionEvent(mouseInfo);
		if(we2 & WE_BTNCLICK)
			OnDownwardItem();
	}


	LONG cnt = GetItemCount();
	int itemCnt = cnt - m_lLineNum;
	if(itemCnt > 0)
	{
		if(m_pGuageBar)
		if(m_pGuageBar->IsDrag() )
		{
			m_lTopListItemIdx = (LONG)(((float)itemCnt)*m_pGuageBar->GetCurRate());
		}
	}
	else
		m_lTopListItemIdx = 0;

	// LBS 03.10.20  Mouse에 대한 체크를 한다. (각 Line에 대한 check)
	we |= cDialog::ActionEvent(mouseInfo);

//	LONG x = mouseInfo->GetMouseX();
//	LONG y = mouseInfo->GetMouseY();
	
//	if( mouseInfo->LButtonDown() )
//	if( PtInWindow( x, y ) /*&& !WINDOWMGR->IsMouseDownUsed()*/ )
//		we |= WE_LBTNCLICK;
	ListMouseCheck( mouseInfo );

	return we;
}

BOOL cListDialog::PtInWindow(LONG x, LONG y)
{
	if( !m_bActive ) return FALSE;

	WORD height = m_topHeight + m_middleNum * m_middleHeight; 
	if(m_absPos.x > x || m_absPos.y > y || m_absPos.x+m_width < x || m_absPos.y+height < y)
		return FALSE;
	else
		return TRUE;
}

void cListDialog::ListMouseCheck( CMouse * mouseInfo )
{
	LONG x = mouseInfo->GetMouseX();
	LONG y = mouseInfo->GetMouseY();
	

	int selIdx = PtIdxInRow(x, y);

	if( selIdx < m_lLineNum && selIdx != -1)
	{

		m_SelectedRowIdx = selIdx;
		if( mouseInfo->LButtonDown() )
		{
			ITEM* item = NULL;
			LONG i = 0;

			m_ClickSelected  = selIdx + m_lTopListItemIdx;

			PTRLISTPOS pos = m_ListItem.FindIndex(m_lTopListItemIdx+m_SelectedRowIdx);
			if( pos == NULL ) return;

			item = (ITEM*)m_ListItem.GetAt(pos);
			if( item )
			{
				cbWindowFunc(m_ID, m_pParent, WE_ROWCLICK);	//PPP
//				we |= WE_ROWCLICK;
			}
		}
	}
	else
	{
		m_SelectedRowIdx = -1;
	}
}

void cListDialog::Render()
{
	if(!m_bActive) return;
	cDialog::RenderWindow();			//component render

	static VECTOR2 m_tmpPos;
	m_TopImage.RenderSprite(NULL,NULL,0,&m_absPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));

	m_tmpPos.x = m_absPos.x;
	m_tmpPos.y = m_absPos.y+m_topHeight;
	for(int j = 0 ; j < m_middleNum ; j++)
	{
		m_MiddleImage.RenderSprite(NULL,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		m_tmpPos.y += m_middleHeight;
	}

		m_DownImage.RenderSprite(NULL,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));

	// Over Image Display 
	if( m_SelectedRowIdx != -1 )
	{
		m_tmpPos.x = m_absPos.x + 2;
		m_tmpPos.y = m_absPos.y + m_textRelRect.top + (((float)m_SelectedRowIdx)*m_lLineHeight) - m_fGap;
		m_OverImage.RenderSprite(&m_OverImageScale,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	}
	//

	// List Text Display //////////////////////////////////////////////////////////////

	PTRLISTPOS pos = m_ListItem.FindIndex( m_lTopListItemIdx );
	ITEM* item = NULL;
	LONG i = 0;

	// 문자 중간에 하이라이트 색을 넣었을 경우(Item의 Line을 체크)
	// RaMa 04.8.3
	int bline = -1;
	int blinelen = 0;
	int line = -1;
	int linelen = 0;
	DWORD dwFontColor = 0xffffffff;

	while(pos)
	{
		item = (ITEM*)m_ListItem.GetNext(pos);
		if(line < m_lLineNum-1)
//		if(i < m_lLineNum)
		{
			if(CFONT_OBJ->IsCreateFont(m_wFontIdx))
			{
				// 이전과 같은 라인이면 이전의 길이를 구한다.
				if(item->line != -1 && item->line == bline)
				{					
					linelen += blinelen;
					blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
				}
				// 새로운 라인이면
				else if(item->line != -1 && item->line != bline)
				{
					linelen = 0;
					++line;
					bline = item->line;
					blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
				}
				// 일반적인 경우
				else
				{					
					linelen = 0;
					++line;
				}

				RECT rect={(LONG)m_absPos.x+m_textRelRect.left+linelen, (LONG)m_absPos.y+m_textRelRect.top + (m_lLineHeight*line), 1,1};

				if( (m_ClickSelected-m_lTopListItemIdx) == line && (m_bShowSelect == TRUE))
					CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(0xffffff00, m_alpha * m_dwOptionAlpha / 100));
				else
					CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));


/*				// 다른 라인이면 줄을 바꿔준다.				
				RECT rect={(LONG)m_absPos.x+m_textRelRect.left, (LONG)m_absPos.y+m_textRelRect.top + (m_lLineHeight*i), 1,1};
				//rect.right = rect.left + 1;
				//rect.bottom = rect.top + 1;
				
				CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));				
*/
			}
//			i++;
		}
		else
			break;
	}
	////////////////////////////////////////////////////////////////////////////////////

	cDialog::RenderComponent();			//component render
}


void cListDialog::onSize(BOOL fSize)
{
	if(fSize)
	{
		// long
		if(!Resize(m_middleNum+1)) return;
		//1. 윈도우 이동
		SetAbsY((LONG)(GetAbsY()-m_middleHeight));
		SetHeight(cWindow::GetHeight()+m_middleHeight);
		//2. guagebar변화
		m_pGuageBar->SetAbsY((LONG)(m_pGuageBar->GetAbsY()-m_middleHeight));
		m_pGuageBar->SetHeight(m_pGuageBar->GetHeight()+m_middleHeight);
		m_pGuageBar->SetInterval(m_pGuageBar->GetInterval()+m_middleHeight);
		//3. upward 위치 변화
		m_pUpwardBtn->SetAbsY((LONG)(m_pUpwardBtn->GetAbsY()-m_middleHeight));
		
		OnUpwardItem();	//KES 040309
	}
	else
	{
		// short
		if(!Resize(m_middleNum-1)) return;
		//1. 윈도우 이동
		SetAbsY((LONG)((GetAbsY()+m_middleHeight)));
		SetHeight(cWindow::GetHeight()-m_middleHeight);
		//2. guagebar변화
		m_pGuageBar->SetAbsY((LONG)(m_pGuageBar->GetAbsY()+m_middleHeight));
		m_pGuageBar->SetHeight(m_pGuageBar->GetHeight()-m_middleHeight);
		m_pGuageBar->SetInterval(m_pGuageBar->GetInterval()-m_middleHeight);
		//3. upward 위치 변화
		m_pUpwardBtn->SetAbsY((LONG)(m_pUpwardBtn->GetAbsY()+m_middleHeight));
		
		OnDownwardItem();	//KES 040309
	}
}

void cListDialog::OnUpwardItem() 
{ 
	LONG cnt = GetItemCount();
	if(cnt < m_lLineNum) return;

	if(m_lTopListItemIdx < 1) return;
	m_lTopListItemIdx--;

	if( m_pGuageBar )
	m_pGuageBar->SetCurRate( (float)m_lTopListItemIdx/(float)(cnt-m_lLineNum) );
	
}

void cListDialog::OnDownwardItem() 
{ 
	LONG cnt = GetItemCount();
	if(cnt < m_lLineNum) return;

	if((cnt-m_lTopListItemIdx-1) < m_lLineNum) return;
	m_lTopListItemIdx++;

	if( m_pGuageBar )
	m_pGuageBar->SetCurRate( (float)m_lTopListItemIdx/(float)(cnt-m_lLineNum) );
}

void cListDialog::AddItem(char * str, DWORD color, int line)
{
	ITEM* pItem = new ITEM;
	strcpy( pItem->string, str);
	pItem->rgb = color;
	pItem->line = line;
//	pItem->line = -1;
	cListItem::AddItem(pItem);	

//맨아래일때만
/*	if( m_pGuageBar )
	if( m_lTopListItemIdx == GetItemCount() - m_lLineNum -1 )
	{
		m_pGuageBar->SetCurValue(m_pGuageBar->GetMaxValue());
		m_lTopListItemIdx = GetItemCount() - m_lLineNum;
	}
*/
	if( m_pGuageBar )
	if( m_lTopListItemIdx == GetItemCount() - m_lLineNum -1 )
	{
		m_pGuageBar->SetCurValue(m_pGuageBar->GetMaxValue());
		m_lTopListItemIdx = GetItemCount() - m_lLineNum;
	}

	
}


void cListDialog::SetTopListItemIdx(int Idx)
{
	m_lTopListItemIdx=Idx;
	LONG cnt = GetItemCount();

	if( m_pGuageBar )
	{
		float rate = (float)m_lTopListItemIdx/(float)(cnt-m_lLineNum);
		if(rate > 1.f)		rate = 1.f;
		else if(rate< 0.f)	rate = 0.f;
		m_pGuageBar->SetCurRate( rate );		
	}	
}

//void cListDialog::RemoveItem( char * str )
//{
//	
//}

int cListDialog::PtIdxInRow(LONG x, LONG y)
{
	LONG cnt = GetItemCount();

	for(int i = 0 ; i < cnt ; i++)
	{
		if( m_absPos.x < x 
			&& m_absPos.y < y
			&& x < m_absPos.x+m_width 
			&& y < m_absPos.y+m_textRelRect.top+(i+1)*m_lLineHeight
			&& y < m_absPos.y+m_textRelRect.bottom )	//2007. 12. 17. CBH - 리스트에서 범위를 넘어가면 클릭되는 현상 수정

			return i;
	}

	return -1;
}

int cListDialog::GetCurSelectedRowIdx()
{
	if( m_SelectedRowIdx == -1 ) 
		return m_SelectedRowIdx;

	return m_SelectedRowIdx+m_lTopListItemIdx;
}

void cListDialog::SetCurSelectedRowIdx(int Idx)
{
	if(Idx < 0)
	{
		SetTopListItemIdx(0);

		m_SelectedRowIdx = -1;
		m_ClickSelected = -1;
	}
	else
	{
		if(Idx <= GetItemCount() - m_lLineNum)
		{
			SetTopListItemIdx(Idx);
		
			m_SelectedRowIdx = 0;
			m_ClickSelected = Idx;
		}
		else
		{
			SetTopListItemIdx(GetItemCount() - m_lLineNum);
			
			m_SelectedRowIdx = Idx - GetItemCount();
			m_ClickSelected = Idx;
		}
	}
}

void cListDialog::ResetGuageBarPos()
{ 
	if( m_pGuageBar )
	{
		m_pGuageBar->SetCurValue(0); 

		int itemCnt = GetItemCount() - m_lLineNum;
		
		if(itemCnt > 0)
			m_lTopListItemIdx = (LONG)(((float)itemCnt)*m_pGuageBar->GetCurRate());
		else
			m_lTopListItemIdx = 0;
	}

	m_SelectedRowIdx = -1;
	m_ClickSelected = -1;
}

void cListDialog::SetLineHeight( LONG height )
{ 
	m_lLineHeight = height; 

	if( height > 10 )
		m_fGap = ((float)height - 10.0f)/2.0f; // 임시로 (나중에 글자의 높이를 알아와서 계산한다.)
	else
		m_fGap = 0.0f;
}

void cListDialog::SetShowSelect(BOOL val)
{
	m_bShowSelect = val;
}

ITEM* cListDialog::GetSelectedITEM( CMouse * mouseInfo )
{
	LONG x = mouseInfo->GetMouseX();
	LONG y = mouseInfo->GetMouseY();

	int selIdx = PtIdxInRow(x, y);

	if( selIdx < m_lLineHeight && selIdx != -1 )
	{
		ITEM* pItem = NULL;
		PTRLISTPOS pos = m_ListItem.FindIndex( m_lTopListItemIdx + selIdx );
		if( NULL == pos )
		{
			return NULL;
		}

		pItem = (ITEM*)m_ListItem.GetAt(pos);
		if( pItem )
		{
			return pItem;
		}
	}

	return NULL;
}
