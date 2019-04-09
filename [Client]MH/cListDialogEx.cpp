#include "stdafx.h"
#include "cListDialogEx.h"
#include "./Interface/cButton.h"
#include "./Interface/cGuageBar.h"
#include "./Interface/cFont.h"


cListDialogEx::cListDialogEx()
{
}

cListDialogEx::~cListDialogEx()
{
}


void cListDialogEx::ListMouseCheck( LONG x, LONG y, DWORD we )
{
	int selIdx = PtIdxInRow(x, y);

	if( selIdx < m_lLineNum ) // && selIdx != -1)
	{
		m_SelectedRowIdx = selIdx;
		if( we & WE_LBTNCLICK )
		{
			PTRLISTPOS pos = m_ListItem.FindIndex(m_lTopListItemIdx+m_SelectedRowIdx);
			if( pos == NULL ) return;
			LINKITEM* item = NULL;
			LONG i = 0;

			item = (LINKITEM*)m_ListItem.GetAt(pos);
			if( item && item->dwType  > emLink_Null )//&& item->dwType < emLink_Image )				
			{
				cbWindowFunc(m_ID, m_pParent, WE_ROWCLICK);	//PPP
				we |= WE_ROWCLICK;
			}
		}
	}
	else
	{
		m_SelectedRowIdx = -1;
	}
}

void cListDialogEx::Render()
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


	// List Text Display //////////////////////////////////////////////////////////////

	PTRLISTPOS pos = m_ListItem.FindIndex( m_lTopListItemIdx );
	LINKITEM* item = NULL;
	LONG i = 0;
	//Test
	char ReadBuff[256] = {0,};
	char WriteBuff[256] = {0,};
	char temp[256] = {0,};

	int ReadPos = 0;
	int WritePos = 0;

	while(pos)
	{
		item = (LINKITEM*)m_ListItem.GetNext(pos);

		if(i < m_lLineNum)
		{
			if(CFONT_OBJ->IsCreateFont(m_wFontIdx))
			{
				RECT rect={(LONG)m_absPos.x+m_textRelRect.left, (LONG)m_absPos.y+m_textRelRect.top + (m_lLineHeight*i), 1,1};

				if( item->dwType  > emLink_Null )//&& item->dwType < emLink_Image ) // 마우스에 대한 처리를 하는경우
				{
					if( m_SelectedRowIdx == i )
					{
						m_tmpPos.x = m_absPos.x + 2;
						m_tmpPos.y = m_absPos.y + m_textRelRect.top + (((float)m_SelectedRowIdx)*m_lLineHeight) - m_fGap;
						m_OverImage.RenderSprite(&m_OverImageScale,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
					}

					if(CFONT_OBJ->IsCreateFont(4))
					{
						if( m_SelectedRowIdx == i ) // Over의 경우
							CFONT_OBJ->RenderFont(4,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->dwOverRGB, m_alpha * m_dwOptionAlpha / 100));
						else // 일반적인 경우
							CFONT_OBJ->RenderFont(4,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
					}
					else
					{
						if( m_SelectedRowIdx == i )
							CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->dwOverRGB, m_alpha * m_dwOptionAlpha / 100));
						else // 일반적인 경우
							CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
					}
				}
				else
				{
					CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));

					LINKITEM* pNext = (LINKITEM*)item->NextItem;
					LONG lWid = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, item->string, lstrlen(item->string) );

					// 동일 라인에 다음 아이템이 존재하면..
					while(pNext)
					{
						rect.left += lWid;
						CFONT_OBJ->RenderFont(m_wFontIdx,pNext->string,lstrlen(pNext->string),&rect,RGBA_MERGE(pNext->rgb, m_alpha * m_dwOptionAlpha / 100));

						lWid = CFONT_OBJ->GetTextExtent( m_wFontIdx, pNext->string, lstrlen(pNext->string) );
						pNext = (LINKITEM*)pNext->NextItem;
					}
				}
			}
			i++;
		}
		else
			break;
	}

	////////////////////////////////////////////////////////////////////////////////////

	if(IsMaxLineOver())
	{	
		if(m_pUpwardBtn)
			m_pUpwardBtn->Render();
		if(m_pDownwardBtn)
			m_pDownwardBtn->Render();
		if(m_pGuageBar)
			m_pGuageBar->Render();
	}	

	cDialog::RenderComponent();			//component render
}

