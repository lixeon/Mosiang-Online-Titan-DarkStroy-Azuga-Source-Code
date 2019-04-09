// cIconDialog.cpp: implementation of the cIconDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cIconDialog.h"
#include "cWindowManager.h"
#include "cIcon.h"
#include "../Input/Mouse.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cIconDialog::cIconDialog()
{
	m_wCellNum = 0;
	m_pIconCell=NULL;
	m_type = WT_ICONDIALOG;
	m_acceptableIconType=0xffffffff;			//all accept
	idx = 0;
	m_lCurSelCellPos = -1;
	m_lCurDragOverPos = -1;
	m_bItemDraged = FALSE;
	m_nIconType = WT_ICON;
}

cIconDialog::~cIconDialog()
{
	Release();
}
void cIconDialog::Release()
{
	SAFE_DELETE_ARRAY(m_pIconCell);
}
void cIconDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
}
void cIconDialog::SetCellNum(WORD num)
{
	Release();
	idx = 0;
	m_wCellNum = num;
	m_pIconCell = new cIconCell[m_wCellNum];
	for(int i = 0 ; i < m_wCellNum ; i++ )
	{
		m_pIconCell[i].icon = NULL;
		m_pIconCell[i].use = NOTUSE;
	}
	
}
BOOL cIconDialog::PtInCell(LONG x, LONG y)
{
	if(!m_pIconCell) return FALSE;

	for(int i = 0 ; i < m_wCellNum ; i++)
	{
		if(m_absPos.x+m_pIconCell[i].rel_rect.left > x || m_absPos.y+m_pIconCell[i].rel_rect.top > y ||
			m_absPos.x+m_pIconCell[i].rel_rect.right < x || m_absPos.y+m_pIconCell[i].rel_rect.bottom < y)
			continue;
		else
			return TRUE;
	}
	
	return FALSE;
}
void cIconDialog::AddIconCell(RECT * pRect)
{
	m_pIconCell[idx++].rel_rect = *pRect;
}
void cIconDialog::SetAbsXY(LONG x, LONG y)
{
	LONG tmpX = x - (LONG)m_absPos.x;
	LONG tmpY = y - (LONG)m_absPos.y;

	for(int i = 0 ; i < m_wCellNum ; i++)
	{
		if(m_pIconCell[i].use == USE && m_pIconCell[i].icon && m_pIconCell[i].bOnlyLink == FALSE)
		{
			m_pIconCell[i].icon->SetAbsXY((LONG)m_pIconCell[i].icon->GetAbsX()+tmpX,
											(LONG)m_pIconCell[i].icon->GetAbsY()+tmpY);
		}
	}
	cDialog::SetAbsXY(x,y);
}
void cIconDialog::SetActive(BOOL val)
{
	if( m_bDisable ) return;
	
	cDialog::SetActive(val);
	for(int i = 0 ; i < m_wCellNum ; i++)
	{
		if(m_pIconCell[i].use == USE && m_pIconCell[i].icon)
			m_pIconCell[i].icon->SetActive(val);
	}
}

void cIconDialog::Render()
{
	if(!m_bActive) return;
	cDialog::Render();

	VECTOR2 start_pos;
	if(m_lCurSelCellPos != -1 && m_SelectedBGImage.IsNull())
	{
		start_pos.x = m_absPos.x + m_pIconCell[m_lCurSelCellPos].rel_rect.left;
		start_pos.y = m_absPos.y + m_pIconCell[m_lCurSelCellPos].rel_rect.top;
		m_SelectedBGImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	}

	if(m_lCurDragOverPos != -1 && !m_DragOverBGImage.IsNull())
//	if(IsDragOverDraw())
	{
		start_pos.x = m_absPos.x + m_pIconCell[m_lCurDragOverPos].rel_rect.left -2;
		start_pos.y = m_absPos.y + m_pIconCell[m_lCurDragOverPos].rel_rect.top -2;
		m_DragOverBGImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));

	}
	
	
	for(int i = m_wCellNum-1 ; i >= 0 ; --i)
	{
		if( m_pIconCell[i].use == USE && 
			m_pIconCell[i].icon ) 
		{
			if(m_pIconCell[i].bOnlyLink == FALSE)
			{
				if(m_pIconCell[i].icon->IsDepend())
					m_pIconCell[i].icon->Render();
			}
			else
			{
				BOOL bDepend = m_pIconCell[i].icon->IsDepend();
				m_pIconCell[i].icon->SetDepend(FALSE);
				VECTOR2 pos;
				pos.x = m_absPos.x+m_pIconCell[i].rel_rect.left;
				pos.y = m_absPos.y+m_pIconCell[i].rel_rect.top;
				m_pIconCell[i].icon->Render(&pos);
				m_pIconCell[i].icon->SetDepend(bDepend);
			}
		}
	}
}

DWORD cIconDialog::ActionEvent(CMouse * mouseInfo)
{
	if(!m_bActive) return WE_NULL;

	DWORD we = WE_NULL;
	DWORD we2 = WE_NULL;
	BOOL selCell = FALSE;

	we = cDialog::ActionEvent(mouseInfo);

	POSTYPE pos;
	
	if( !m_bDisable )
	{
// need confirm before delete
//		if(mouseInfo->LButtonDown())
//			m_lCurSelCellPos = -1;

		if(we & WE_LBTNCLICK)
		{
			//KES SMI 031008
			if(GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos))
			{
				m_lCurSelCellPos = pos;
				cbWindowFunc(m_ID, m_pParent, WE_LBTNCLICK);	//PPP
			}
		}
/*
		else if(we & WE_RBTNCLICK)
		{
			if(GetPositionForXYRef(mouseInfo->GetClickDownX(),mouseInfo->GetClickDownY(),pos))
			{
				m_lCurSelCellPos = pos;
				cbWindowFunc(m_ID, m_pParent, WE_RBTNCLICK);	//PPP
			}
		}
*/
		else if(we & WE_LBTNDBLCLICK)
		{
			if(GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos))
			{
				m_lCurSelCellPos = pos;
				cbWindowFunc(m_ID, m_pParent, WE_LBTNDBLCLICK);	//PPP
			}
		}
/*
		else if(we & WE_RBTNDBLCLICK)
		{
			if(GetPositionForXYRef(mouseInfo->GetClickDownX(),mouseInfo->GetClickDownY(),pos))
			{
				m_lCurSelCellPos = pos;
				cbWindowFunc(m_ID, m_pParent, WE_RBTNDBLCLICK);	//PPP
			}
		}
		*/
		else
		{
			if( mouseInfo->LButtonDrag() )
			{
				if( ( we & WE_MOUSEOVER ) && IsDragOverDraw()
					&& GetPositionForXYRef(mouseInfo->GetMouseX(),mouseInfo->GetMouseY(),pos) )
				{
					m_lCurDragOverPos = pos;
					m_bItemDraged = TRUE;
				}
				else
					m_lCurDragOverPos = -1;
			}
			else
			{
				if( m_bItemDraged )	//드래그후에 끝냈음.
				{
					if( GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos) )
						m_lCurSelCellPos = pos;
//					else
//						m_lCurSelCellPos = -1;

					m_bItemDraged = FALSE;
				}

				m_lCurDragOverPos = -1;				
			}
		}
	}
	
	for(int i = 0 ; i < m_wCellNum ; i++)
	{
		//if(m_pIconCell[i].use == USE)
		if( m_pIconCell[i].use == USE && 
			m_pIconCell[i].icon->IsActive() && 
			m_pIconCell[i].icon && 
			m_pIconCell[i].icon->IsDepend() &&
			m_pIconCell[i].bOnlyLink == FALSE)
		{
			we2 = m_pIconCell[i].icon->ActionEvent(mouseInfo);
			/*
			if(we2 & WE_TOPWINDOW)
			{
				m_lCurSelCellPos=i;
				//cbWindowFunc(m_ID, m_pParent, WE_CELLSELECT);	//PPP
				cbWindowFunc(m_ID, m_pParent, WE_LBTNCLICK);	//PPP
				selCell = TRUE;
			}
			*/
		}
	}


	return we;
}

void cIconDialog::SetAlpha(BYTE al)
{ 
	cDialog::SetAlpha(al);
	for(int i = 0 ; i < m_wCellNum ; i++)
	{
		if(m_pIconCell[i].use == USE && m_pIconCell[i].icon)
			m_pIconCell[i].icon->SetAlpha(al);
	}
}

/**/
/**/
/**/
/**/
BOOL cIconDialog::IsAddable(WORD idx)
{
	if(m_pIconCell[idx].use == USE)
		return FALSE;
	return TRUE;
}
BOOL cIconDialog::AddIcon(LONG mouseX, LONG mouseY, cIcon * icon)
{
	ASSERT(icon);

	for(int i = 0 ; i < m_wCellNum ; i++)
	{
		if(m_absPos.x+m_pIconCell[i].rel_rect.left > mouseX || m_absPos.y+m_pIconCell[i].rel_rect.top > mouseY ||
			m_absPos.x+m_pIconCell[i].rel_rect.right < mouseX || m_absPos.y+m_pIconCell[i].rel_rect.bottom < mouseY)
			continue;
		else
		{
			return AddIcon(i, icon);
		}
	}

	return FALSE;
}
BOOL cIconDialog::GetPositionForXYRef(LONG mouseX, LONG mouseY, WORD& pos)
{
	for(int i = 0 ; i < m_wCellNum ; i++)
	{
		if(m_absPos.x+m_pIconCell[i].rel_rect.left > mouseX || m_absPos.y+m_pIconCell[i].rel_rect.top > mouseY ||
			m_absPos.x+m_pIconCell[i].rel_rect.right < mouseX || m_absPos.y+m_pIconCell[i].rel_rect.bottom < mouseY)
			continue;
		else
		{
			pos = i;
			return TRUE;
		}
	}
	return FALSE;
}
BOOL cIconDialog::AddIcon(WORD cellIdx, cIcon * icon,BOOL bOnlyLink)
{
	ASSERT(icon);
	if(cellIdx > m_wCellNum) return FALSE;
	if(m_pIconCell[cellIdx].use == NOTUSE)
	{
		m_pIconCell[cellIdx].icon = icon;
		m_pIconCell[cellIdx].bOnlyLink = bOnlyLink;
		m_pIconCell[cellIdx].use = USE;
		if(bOnlyLink == FALSE)
		{
			m_pIconCell[cellIdx].icon->SetAbsXY((LONG)m_absPos.x+m_pIconCell[cellIdx].rel_rect.left, (LONG)m_absPos.y+m_pIconCell[cellIdx].rel_rect.top);
			m_pIconCell[cellIdx].icon->SetParent(this);
			//현재 창이 디스에이블이면 아이콘도 디스에이블
			if( m_bDisable )
				m_pIconCell[cellIdx].icon->SetDisable( TRUE );
			m_pIconCell[cellIdx].icon->SetActive(m_bActive);
			m_pIconCell[cellIdx].icon->SetDepend(TRUE);
		}
		else
		{
			icon->AddLinkIconDialog(this);
		}
		return TRUE;
	}
	else 
	{
		ASSERT(0);
		return FALSE;
	}
		
}

/********************************************/
/*   Delete operation						*/
/*	 윈도우 메니저에서 삭제를 해준다.		*/
/********************************************/

BOOL cIconDialog::DeleteIcon(WORD cellIdx, cIcon ** icon)
{
	if(cellIdx > m_wCellNum) return FALSE;

	if(m_pIconCell[cellIdx].use == USE)
	{
		m_pIconCell[cellIdx].use = NOTUSE;
		if(icon)
			*icon = m_pIconCell[cellIdx].icon;
		
		if(m_pIconCell[cellIdx].bOnlyLink == FALSE)
		{
			m_pIconCell[cellIdx].icon->SetParent(NULL);
			m_pIconCell[cellIdx].icon->SetDepend(FALSE);
		}
		else
		{
			m_pIconCell[cellIdx].icon->RemoveLinkIconDialog(this);
		}
		
		m_pIconCell[cellIdx].icon = NULL;
		return TRUE;
	}
	else
	{
		if(icon)
		{
			*icon = NULL;
		}
		
		return FALSE;
	}
	
}

void cIconDialog::DeleteIconAll()
{
	for(int i = 0 ; i < m_wCellNum ; i++)
	{
		if(m_pIconCell[i].use == USE)
		{
			if(m_pIconCell[i].bOnlyLink == FALSE)
			{
				m_pIconCell[i].icon->SetParent(NULL);
				m_pIconCell[i].icon->SetDepend(FALSE);
			}
			m_pIconCell[i].icon = NULL;
			m_pIconCell[i].use = NOTUSE;
		}
	}
}

void cIconDialog::SetDisable( BOOL val )
{
	cDialog::SetDisable( val );

	if(val == TRUE)
		int  a= 1;
	for(int i = 0 ; i < m_wCellNum ; i++)
	{
		if( m_pIconCell[i].icon )
			m_pIconCell[i].icon->SetDisable( val );
	}
}

BOOL cIconDialog::IsDragOverDraw()
{
	if( !WINDOWMGR->IsDragWindow() ) return FALSE;
	cDialog* pDlg = WINDOWMGR->GetDragDlg();

	if( pDlg )
	if( pDlg->GetType() == m_nIconType )
		return TRUE;

	return FALSE;
}

void cIconDialog::NotifyLinkIconDeleted(cIcon* pIcon)
{
	for(int i = 0 ; i < m_wCellNum ; i++)
	{
		if(m_pIconCell[i].use == USE)
		{
			if(m_pIconCell[i].bOnlyLink == FALSE && m_pIconCell[i].icon == pIcon)
			{
				m_pIconCell[i].icon = NULL;
				m_pIconCell[i].use = NOTUSE;
			}
		}
	}

}