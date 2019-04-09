#include "stdafx.h"
#include "HelpDialog.h"
#include "cDialogueList.h"
#include "cHyperTextList.h"
#include "WindowIDEnum.h"
#include "HelpDicManager.h"

cHelpDialog::cHelpDialog()
{
	m_type = WT_HELPDIALOG;	
	m_pListDlg = NULL;
}

cHelpDialog::~cHelpDialog()
{
}


void cHelpDialog::Linking()
{
	m_pListDlg = (cListDialogEx*)this->GetWindowForID( HI_LISTDLG );
}


BOOL cHelpDialog::OpenDialog()
{
	// ÃÊ±âÈ­
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)
		m_sHyper[i].Init();

	m_pListDlg->RemoveAll();
	m_nHyperCount = 0;

	// ÆäÀÌÁö¿¡ ´ëÇÑ Á¤º¸¸¦ ¿äÃ»ÇÑ´Ù.

	cPage* pMainPage = HELPDICMGR->GetMainPage();
	if( pMainPage == NULL ) return FALSE;

	m_dwCurPageId = pMainPage->GetPageId(); // ÀÓ½Ã·Î ÇöÀç Ç¥½Ã ÆäÀÌÁö¸¦ °¡Áø´Ù.

	// ÆäÀÌÁö Á¤º¸¸¦ ÀÌ¿ëÇÏ¿© ÆäÀÌÁö¸¦ ±¸¼ºÇÑ´Ù.
	DWORD dwMsg = pMainPage->GetRandomDialogue(); // ¸Þ¼¼ÁöÀÇ ¹øÈ£

	// ¸Þ¼¼Áö ¹øÈ£¸¦ ÀÌ¿ëÇÏ¿© »ç¿ëÇÒ ¸Þ¼¼Áö¸¦ °¡Á®¿Â´Ù.
	cDialogueList* pList = HELPDICMGR->GetDialogueList();
	if( pList == NULL ) return FALSE;

	cHyperTextList* pHyper = HELPDICMGR->GetHyperTextList();
	if( pHyper == NULL ) return FALSE;

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
			m_pListDlg->cListItem::AddItem(pItem);
			LineInfo = temp->wLine;
		}
		else
		{
			if( LineInfo == temp->wLine )
			{
				Prev->NextItem = pItem;
			}
			else
			{
				m_pListDlg->cListItem::AddItem(pItem);
				LineInfo = temp->wLine;
			}
		}
		
		Prev = pItem;

		++wIdx;
	}


// ¸µÅ© Á¤º¸¸¦ °¡Á®¿Í¼­ ¼³Á¤ÇÑ´Ù. (ÃßÈÄ ÀÛ¾÷ : Á¶°ÇÀ» °Ë»öÇÏ¿© º¸¿©ÁÙ ¸µÅ©¿¡ ´ëÇÑ Ç¥½Ã)
	HYPERLINK* pLink;
	int nLinkCount = pMainPage->GetHyperLinkCount(); 


// ¸µÅ©¸¦ È­¸é¿¡ Ãâ·ÂÇÏ±âÀü¿¡ ¶óÀÎÀ» ¶ç¿î´Ù.
	if( nLinkCount > 0 )
	{
		for( int i=0; i< 3;++i)
		{
			LINKITEM* pItem = new LINKITEM;
			char* temp = " ";
			strcpy(pItem->string, temp);
			
			m_pListDlg->cListItem::AddItem( pItem );
		}
	}

	for( int j = 0 ; j < nLinkCount ; ++j )
	{
		pLink = pMainPage->GetHyperText(j);
		if( pLink == NULL ) break;

		temp = pHyper->GetHyperText( pLink->wLinkId );
		if( temp )
		{
			// Ãß°¡
			LINKITEM* pItem = new LINKITEM;
			strcpy(pItem->string, temp->str);
			pItem->dwType = pLink->wLinkType;
			
			m_pListDlg->cListItem::AddItem( pItem );

			// Data
			m_sHyper[m_nHyperCount].bUse = TRUE;
			m_sHyper[m_nHyperCount].dwListItemIdx = m_pListDlg->GetItemCount()-1;
			m_sHyper[m_nHyperCount].sHyper = *pLink;
			++m_nHyperCount;
		}
	}

	SetActive( TRUE );

	return TRUE;
}

BOOL cHelpDialog::OpenLinkPage( DWORD dwPageId )
{
	// ÃÊ±âÈ­
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)
		m_sHyper[i].Init();

	m_pListDlg->RemoveAll();
	m_nHyperCount = 0;

	// ÆäÀÌÁö¿¡ ´ëÇÑ Á¤º¸¸¦ ¿äÃ»ÇÑ´Ù.
	cPage* pPage = HELPDICMGR->GetPage( dwPageId );
	if( pPage == NULL ) return FALSE;

	m_dwCurPageId = dwPageId; // ÀÓ½Ã·Î ÇöÀç Ç¥½Ã ÆäÀÌÁö¸¦ °¡Áø´Ù.

	// ÆäÀÌÁö Á¤º¸¸¦ ÀÌ¿ëÇÏ¿© ÆäÀÌÁö¸¦ ±¸¼ºÇÑ´Ù.
	DWORD dwMsg = pPage->GetRandomDialogue(); // ¸Þ¼¼ÁöÀÇ ¹øÈ£

	// ¸Þ¼¼Áö ¹øÈ£¸¦ ÀÌ¿ëÇÏ¿© »ç¿ëÇÒ ¸Þ¼¼Áö¸¦ °¡Á®¿Â´Ù.
	cDialogueList* pList = HELPDICMGR->GetDialogueList();
	if( pList == NULL ) return FALSE;

	cHyperTextList* pHyper = HELPDICMGR->GetHyperTextList();
	if( pHyper == NULL ) return FALSE;

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
			m_pListDlg->cListItem::AddItem(pItem);
			LineInfo = temp->wLine;
		}
		else
		{
			if( LineInfo == temp->wLine )
			{
				Prev->NextItem = pItem;
			}
			else
			{
				m_pListDlg->cListItem::AddItem(pItem);
				LineInfo = temp->wLine;
			}
		}
		
		Prev = pItem;

		++wIdx;
	}


// ¸µÅ© Á¤º¸¸¦ °¡Á®¿Í¼­ ¼³Á¤ÇÑ´Ù. (ÃßÈÄ ÀÛ¾÷ : Á¶°ÇÀ» °Ë»öÇÏ¿© º¸¿©ÁÙ ¸µÅ©¿¡ ´ëÇÑ Ç¥½Ã)
	HYPERLINK* pLink;
	int nLinkCount = pPage->GetHyperLinkCount(); 


// ¸µÅ©¸¦ È­¸é¿¡ Ãâ·ÂÇÏ±âÀü¿¡ ¶óÀÎÀ» ¶ç¿î´Ù.
	if( nLinkCount > 0 )
	{
		for( int i=0; i< 3;++i)
		{
			LINKITEM* pItem = new LINKITEM;
			char* temp = " ";
			strcpy(pItem->string, temp);
			
			m_pListDlg->cListItem::AddItem( pItem );
		}
	}

	for( int j = 0 ; j < nLinkCount ; ++j )
	{
		pLink = pPage->GetHyperText(j);
		if( pLink == NULL ) break;

		temp = pHyper->GetHyperText( pLink->wLinkId );
		if( temp )
		{
			// Ãß°¡
			LINKITEM* pItem = new LINKITEM;
			strcpy(pItem->string, temp->str);
			pItem->dwType = pLink->wLinkType;
			
			m_pListDlg->cListItem::AddItem( pItem );

			// Data
			m_sHyper[m_nHyperCount].bUse = TRUE;
			m_sHyper[m_nHyperCount].dwListItemIdx = m_pListDlg->GetItemCount()-1;
			m_sHyper[m_nHyperCount].sHyper = *pLink;
			++m_nHyperCount;
		}
	}

	return TRUE;
}


void cHelpDialog::EndDialog()
{
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)
		m_sHyper[i].Init();

	m_pListDlg->RemoveAll();

	m_nHyperCount = 0;

//	SetActive( FALSE );
}


HYPER* cHelpDialog::GetHyperInfo( DWORD dwIdx )
{
	for(int i=0;i<m_nHyperCount;++i)
	{
		if( m_sHyper[i].bUse && m_sHyper[i].dwListItemIdx == dwIdx)
			return &m_sHyper[i];
	}

	return NULL;
}

void cHelpDialog::HyperLinkParser( DWORD dwIdx )
{
	if( m_nHyperCount == 0 ) return;

	int nType = -1;
	DWORD dwPageIdx;

	for( int i=0 ; i<m_nHyperCount ; ++i)
	{
		if( m_sHyper[i].dwListItemIdx == dwIdx )
		{
			nType = m_sHyper[i].sHyper.wLinkType;
			dwPageIdx = m_sHyper[i].sHyper.dwData;
			break;
		}
	}

	if( nType == emLink_Page )
	{
		OpenLinkPage( dwPageIdx );
		m_pListDlg->ResetGuageBarPos();
	}

	if( nType == emLink_Open )
	{
		// 
	}

	if( nType == emLink_End )
	{
		EndDialog();
	}

	return;
}

void cHelpDialog::SetActive( BOOL val )
{
	if( m_bActive == val ) return;

	if(!val) EndDialog();	

	cDialog::SetActiveRecursive( val );
}

