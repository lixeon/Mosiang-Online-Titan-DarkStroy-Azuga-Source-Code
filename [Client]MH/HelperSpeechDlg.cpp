#include "stdafx.h"
#include "HelperSpeechDlg.h"
#include "WindowIDEnum.h"
#include "HelperManager.h"
#include "cPage.h"
#include "cDialogueList.h"
#include "mhtimemanager.h"
#include "Helper.h"
#include "./Input/Mouse.h"

#define ALPHA_DELAY 400
#define DEFAULT_ALPHA	180

cHelperSpeechDlg::cHelperSpeechDlg()
{
	m_type = WT_HELPERSPEECHDIALOG;
	m_bUseComponent = TRUE;

//	m_pNextBtn = NULL;
//	m_pPrevBtn = NULL;
//	m_pExitBtn = NULL;

	m_nLineNum = 0;
	m_pCurPage = NULL;

	m_vHelperPos.x = m_vHelperPos.y = 0;
	m_bFadeIn = FALSE;
	m_bFadeOut = FALSE;
	m_bClose = FALSE;

	m_dwStartTime = 0;
}

cHelperSpeechDlg::~cHelperSpeechDlg()
{

}

void cHelperSpeechDlg::Init( int nx, int ny, int nwid, int nhei, int nlinehei, LONG ID )
{
	// ¶óÀÎÀÇ ³ôÀÌ°ª ¹Þ¾Æ¿Â´Ù.
	m_nLineHeight = nlinehei;
	m_wMaxLine = 50;

	m_nCurAlpha = 0;

	cBalloonOutline::SetBalloonSize( nwid, nhei, 0 );

	cDialog::Init( nx, ny, nwid, nhei, NULL, ID );
}

DWORD cHelperSpeechDlg::ActionEvent(CMouse * mouseInfo)
{
	m_dwCurTime = gCurTime;

	DWORD we = WE_NULL;

	if( m_bFadeIn )
	{
		if( m_nCurAlpha <= 0 )
		{
			m_nCurAlpha = 0;
			// ½ÇÇà..
			if( m_bClose )
			{
				// Á¾·á Ã³¸® ½ÇÇà..
				m_bClose = FALSE;
				Exit();
				return 0;
			}

			m_bFadeIn = FALSE;
		}
		if( ALPHA_DELAY-100 < m_dwCurTime - m_dwStartTime )
		{
			//m_nCurAlpha = 255 - ( m_dwCurTime - m_dwStartTime ) * 255 / ALPHA_DELAY;
			m_nCurAlpha -= 30;
			if( m_nCurAlpha < 0 ) m_nCurAlpha = 0;
			cBalloonOutline::SetCurAlhpa( m_nCurAlpha );
		}
	} 
	else if( m_bFadeOut )
	{
		if( m_nCurAlpha >= DEFAULT_ALPHA )
		{
			m_nCurAlpha = DEFAULT_ALPHA;

			if( m_pCurPage->GetPageId() == 1 )
			{
				HELPERMGR->GetHelper()->SetStartTime( m_dwCurTime );
			}
			m_bFadeOut = FALSE;
		}

		if( ALPHA_DELAY < m_dwCurTime - m_dwStartTime )
		{
			m_nCurAlpha += 30;
			if( m_nCurAlpha > DEFAULT_ALPHA ) m_nCurAlpha = DEFAULT_ALPHA;

			cBalloonOutline::SetCurAlhpa( m_nCurAlpha );
		}
	}

	we |= cDialog::ActionEvent(mouseInfo);
	if(	we & WE_LBTNCLICK )
	{
		CloseDialog();
	}

	return we;
}

void cHelperSpeechDlg::Linking()
{
	// ÁÂ¿ì ÀÌµ¿¹öÆ°°ú ´Ý±â ¹öÆ°À» ¸µÅ©ÇØ ³õ´Â´Ù.
//	m_pNextBtn = (cButton*)GetWindowForID( HSI_NEXTBTN );
//	m_pPrevBtn = (cButton*)GetWindowForID( HSI_PREVBTN );
//	m_pExitBtn = (cButton*)GetWindowForID( HSI_EXITBTN );
}

BOOL cHelperSpeechDlg::StartFadeOut( DWORD dwNextIdx ) 
{ 
	if( !OpenDialog( dwNextIdx ) )
	{
		Exit();
		return FALSE;
	}
	else
	{
		m_dwStartTime = m_dwCurTime = gCurTime;
		m_bFadeOut = TRUE; 
	}

	return TRUE;
}

void cHelperSpeechDlg::StartFadeIn() 
{ 
	if( !IsActive() ) return;

	m_dwStartTime = m_dwCurTime = gCurTime;
	m_bFadeIn = TRUE; 
}

void cHelperSpeechDlg::CloseDialog()
{
	m_bFadeIn = TRUE;
	m_bClose = TRUE;
}

void cHelperSpeechDlg::Render()
{
	if( !IsActive() ) return;

	// ¸»Ç³¼± ÀÌ¹ÌÁö¸¦ Âï´Â´Ù.
	cBalloonOutline::RenderOutline( m_absPos.x, m_absPos.y, m_vHelperPos.x+60, m_nLineHeight*m_nLineNum );

	// ´ë»ç¸¦ Âï´Â´Ù. (ListItemÀ» ÀÌ¿ëÇØ¼­ Âï°í ±ÛÀÚÀÇ »ö±òÀ» Ç¥ÇöÇÒ ¼ö ÀÖµµ·Ï ÇÑ´Ù.)
	if( !m_bFadeOut && !m_bFadeIn )
	{
		PTRLISTPOS pos = m_ListItem.GetHeadPosition();
		LINKITEM* item = NULL;
		LONG i = 0;
		while(pos)
		{
			item = (LINKITEM*)m_ListItem.GetNext(pos);
			
			if(CFONT_OBJ->IsCreateFont(m_wFontIdx))
			{
				RECT rect={(LONG)m_absPos.x+m_textRelRect.left, (LONG)m_absPos.y+m_textRelRect.top + (m_nLineHeight*i), 1,1};
				
				CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
				
				LINKITEM* pNext = (LINKITEM*)item->NextItem;
				LONG lWid = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, item->string, lstrlen(item->string) );
				
				// µ¿ÀÏ ¶óÀÎ¿¡ ´ÙÀ½ ¾ÆÀÌÅÛÀÌ Á¸ÀçÇÏ¸é..
				while(pNext)
				{
					rect.left += lWid;
					CFONT_OBJ->RenderFont(m_wFontIdx,pNext->string,lstrlen(pNext->string),&rect,RGBA_MERGE(pNext->rgb, m_alpha * m_dwOptionAlpha / 100));
					
					lWid = CFONT_OBJ->GetTextExtent( m_wFontIdx, pNext->string, lstrlen(pNext->string) );
					pNext = (LINKITEM*)pNext->NextItem;
				}
			}
			i++;
		}

		// ComponentµéÀ» Âï´Â´Ù.
//		if( m_bUseComponent )
//			RenderComponent();
	}
	////////////////////////////////////////////////////////////////////////////////////

}

void cHelperSpeechDlg::RenderComponent()
{
	// ¸µÅ©°¡ ÀÖ´Â °æ¿ì¸¦ »ý°¢ÇÑ´Ù.
	// x¹öÆ°Àº Ç×»ó ³Ö´Â´Ù.

	if(!m_bActive) return;

	//window of componetList
	PTRLISTPOS pos = m_pComponentList->GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pComponentList->GetNext(pos);
		if(window->IsActive())
			window->Render();
	}
}

BOOL cHelperSpeechDlg::OpenDialog( DWORD dwPageId )
{
	// ÃÊ±âÈ­
	cListItem::RemoveAll();
	m_nLineNum = 0;
	m_pCurPage = NULL;

	// ÆäÀÌÁö¿¡ ´ëÇÑ Á¤º¸¸¦ ¿äÃ»ÇÑ´Ù.
	cPageBase* pPage = HELPERMGR->GetPage( dwPageId );
	if( pPage == NULL ) return FALSE;

	m_pCurPage = pPage; // ÀÓ½Ã·Î ÇöÀç Ç¥½Ã ÆäÀÌÁö¸¦ °¡Áø´Ù.

	// ÆäÀÌÁö Á¤º¸¸¦ ÀÌ¿ëÇÏ¿© ÆäÀÌÁö¸¦ ±¸¼ºÇÑ´Ù.
	DWORD dwMsg = pPage->GetRandomDialogue(); // ¸Þ¼¼ÁöÀÇ ¹øÈ£

	// ¸Þ¼¼Áö ¹øÈ£¸¦ ÀÌ¿ëÇÏ¿© »ç¿ëÇÒ ¸Þ¼¼Áö¸¦ °¡Á®¿Â´Ù.
	cDialogueList* pList = HELPERMGR->GetDialogueList();
	if( pList == NULL ) return FALSE;

// ´ë»ç Á¤º¸ ¼ÂÆÃ ½ÃÀÛ
	WORD wIdx = 0;
	WORD LineInfo = 0;
	DIALOGUE* temp = NULL;

	LINKITEM* pItem = NULL;
	LINKITEM* Prev = NULL;

	while(1)
	{
		temp = pList->GetDialogue( dwMsg, wIdx );
		if( temp == NULL ) break;

		// ¶óÀÎÁ¤º¸°¡ ÀÖÀ¸¸é ¾ÆÀÌÅÛÀ» Ã·°¡ÇÑ´Ù.(ÇÊ¼ö!!)
		pItem = new LINKITEM;
		strcpy(pItem->string, temp->str);
		pItem->rgb = temp->dwColor;

		if( Prev == NULL )
		{
			cListItem::AddItem(pItem);
			LineInfo = temp->wLine;
			++m_nLineNum;
		}
		else
		{
			if( LineInfo == temp->wLine )
			{
				Prev->NextItem = pItem;
			}
			else
			{
				cListItem::AddItem(pItem);
				LineInfo = temp->wLine;
				++m_nLineNum;
			}
		}
		
		Prev = pItem;

		++wIdx;
	}
/*
	if( m_bUseComponent ) // ¹öÆ°À» ³Ö¾úÀ» °æ¿ì¿¡ ¾Æ·¡ 3Ä­À» ¶ç¿î´Ù.
	{
		for( int i=0; i< 3;++i)
		{
			LINKITEM* pItem = new LINKITEM;
			char* temp = " ";
			strcpy(pItem->string, temp);
			
			cListItem::AddItem( pItem );
			++m_nLineNum;
		}
	}
*/
	SetActive( TRUE );
	ResetDialog();		// DialogÁ¤º¸¸¦ »õ·Î °è»êÇÑ´Ù.

	return TRUE;
}

void cHelperSpeechDlg::ResetDialog()
{
	// Component µéÀÇ »ó´ëÀ§Ä¡°ªÀ» º¯È­ ½ÃÅ²´Ù.
	float fHei = m_nLineHeight*m_nLineNum;

	// Ã¢ÀÇ À§Ä¡¿Í Å©±â¸¦ º¯È­ ½ÃÅ²´Ù.

	SetAbsXY( m_vHelperPos.x,  m_vHelperPos.y - fHei - 20 );
	SetHeight( fHei + m_fBorder );

//	if( m_bUseComponent )
//	{
		// Component¿¡ ´ëÇÑ Ã³¸® 
//		m_pExitBtn->SetAbsXY( m_absPos.x + m_width - 5, m_absPos.y + 5 );
//		m_pNextBtn->SetAbsXY();
//		m_pPrevBtn->SetAbsXY();
//	}

	m_textRelRect.bottom = fHei;
}

void cHelperSpeechDlg::AddPage( DWORD dwPageId )
{ 
	PTRLISTSEARCHSTART(m_NextPagelist, DWORD*, pId)
		if( *pId == dwPageId )
			return;
	PTRLISTSEARCHEND

	DWORD* page = new DWORD;
	*page = dwPageId;
	m_NextPagelist.AddTail(page);
}


void cHelperSpeechDlg::Exit() // Close ButtonÀ» ´­·¶À»¶§¸¸ ¹ß»ýµÈ´Ù.
{
	// µî·ÏµÈ µµ¿ò¸»ÀÌ ÀÖ´Â°æ¿ì¿¡ ´ëÇÑ Ã³¸®¸¦ ³Ö´Â´Ù.
	if( !m_NextPagelist.IsEmpty() )
	{
		DWORD* data = (DWORD*)m_NextPagelist.RemoveHead();
		StartFadeOut(*data);
		SAFE_DELETE(data);

		return;
	}

	m_bUseComponent = TRUE;
	
	m_nLineNum = 0;
	m_pCurPage = NULL;
	
	m_bFadeIn = FALSE;
	m_bFadeOut = FALSE;

	m_dwStartTime = 0;
	
	HELPERMGR->SetActive(FALSE);
}

DWORD cHelperSpeechDlg::GetCurPageIdx()
{ 
	return m_pCurPage->GetPageId(); 
}
