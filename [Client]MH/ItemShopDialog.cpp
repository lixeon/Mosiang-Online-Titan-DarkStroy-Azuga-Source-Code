// ItemShopDialog.cpp: implementation of the CItemShopDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemShopDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cIcon.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cWindowManager.h"
#include "ItemShopGridDialog.h"
#include "ItemManager.h"
#include "./Input/UserInput.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "ChatManager.h"
#include "cMsgBox.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemShopDialog::CItemShopDialog()
{
	m_type = WT_ITEMSHOPDIALOG;
	m_curTabNum	= 0;
	m_bChangeTabWithMouseOver	= FALSE;
	m_MouseOverBeginTime	= 0;
	m_MouseOverDelayTime	= 500;
}

CItemShopDialog::~CItemShopDialog()
{

}


void CItemShopDialog::SetActive( BOOL val )
{
	if( val == FALSE)
	{
		if( HERO->GetState() == eObjectState_Deal)
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
	
	cDialog::SetActive( val );
}



void CItemShopDialog::Linking()
{
	for(int i=0; i<MAX_ITEMSHOP_PAGE; i++)
	{
		m_pPageBtn[i] = (cPushupButton*)GetWindowForID( ITMALL_BTN1+i );
		m_pItemGridDialog[i] = (CItemShopGridDialog*)GetWindowForID( ITMALL_GRID1 + i );
		m_pItemGridDialog[i]->SetTabNumber(i);
	}
	m_IndexGenerator.Init( 999, IG_SHOPITEM_STARTINDEX );
}


DWORD CItemShopDialog::ActionEvent(CMouse * mouseInfo)
{
	//아이콘 드래그중일때.. 다른 탭으로 이동해야한다.
	if( WINDOWMGR->IsDragWindow() && WINDOWMGR->GetDragDlg() )
	{
		if( WINDOWMGR->GetDragDlg()->GetType() == WT_ITEM )
		{
			DWORD we = WE_NULL;
			if(!m_bActive) return we;
			we = cDialog::ActionEvent(mouseInfo);

			BOOL bMouseOver = FALSE;
			for( DWORD i = 0 ; i < MAX_ITEMSHOP_PAGE ; i++ )
			{
				m_pPageBtn[i]->ActionEvent( mouseInfo );
				
				if( we & WE_MOUSEOVER )
				if( m_pPageBtn[i]->PtInWindow( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )
				{
					bMouseOver = TRUE;
					
					if( m_bChangeTabWithMouseOver )
					{
						if( i != m_curTabNum )
						{
							TabChange(i);		//순서지키기(수련창때문..)
							m_curTabNum = i;
						}
					}
					else
					{
						if( m_MouseOverBeginTime == 0 )
							m_MouseOverBeginTime = gCurTime;
						else if( gCurTime - m_MouseOverBeginTime > m_MouseOverDelayTime )
							m_bChangeTabWithMouseOver = TRUE;
					}
				}				
			}
			
			if( !bMouseOver )
				m_MouseOverBeginTime = 0;
			
//			we |= m_ppWindowTabSheet[m_bSelTabNum]->ActionEvent( mouseInfo );
			return we;
		}
	}

	m_MouseOverBeginTime		= 0;
	m_bChangeTabWithMouseOver = FALSE;
	return cDialog::ActionEvent( mouseInfo );	
}


void CItemShopDialog::SetItemInfo(SEND_SHOPITEM_INFO* pItemInfo)
{
	if(!pItemInfo)		return;

	DWORD dwtab = 0;
	
	if( pItemInfo->ItemCount > SLOT_SHOPITEM_IMSI )
		pItemInfo->ItemCount = SLOT_SHOPITEM_IMSI;

	for(int i=0; i<SLOT_SHOPITEM_IMSI; i++)
	{
		cIcon* pIcon;
		dwtab = GetTabNumber(TP_SHOPITEM_START+i);
		CItem* pItem = m_pItemGridDialog[dwtab]->GetItemForPos(TP_SHOPITEM_START+i);
		if( pItem )
		{
			int pos = i%TABCELL_SHOPITEM_NUM;
			m_pItemGridDialog[dwtab]->DeleteIcon( pos, &pIcon );
		}
	}
	
	BOOL bPresent = FALSE;
	for(i=0; i<pItemInfo->ItemCount; i++)
	{
		if( pItemInfo->Item[i].dwDBIdx != 0 )
		{
			if( pItemInfo->Item[i].ItemParam & ITEM_PARAM_PRESENT )
			{
				bPresent = TRUE;
				pItemInfo->Item[i].ItemParam ^= ITEM_PARAM_PRESENT;
			}

			dwtab = GetTabNumber(pItemInfo->Item[i].Position);
			m_pItemGridDialog[dwtab]->AddItem( &pItemInfo->Item[i] );			
		}
	}

	if( bPresent )
		WINDOWMGR->MsgBox( MBI_PRESENT_NOTIFY, MBT_OK, CHATMGR->GetChatMsg(733) );
}

DWORD CItemShopDialog::GetTabNumber(POSTYPE absPos)
{
	return (absPos - TP_SHOPITEM_START) / TABCELL_SHOPITEM_NUM;
}

BOOL CItemShopDialog::AddItem(CItem* pItem)
{
	DWORD dwtab = GetTabNumber(pItem->GetItemBaseInfo()->Position);

	return m_pItemGridDialog[dwtab]->AddItem( (ITEMBASE*)pItem->GetItemBaseInfo() );
}

BOOL CItemShopDialog::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	DWORD dwtab = GetTabNumber(Pos);	

	return m_pItemGridDialog[dwtab]->DeleteItem(Pos, ppItem);
}


void CItemShopDialog::ShopItemDelete(DWORD dwDBIdx, POSTYPE absPos, DWORD dwCount)
{
}


BOOL CItemShopDialog::FakeMoveIcon(LONG x, LONG y, cIcon* icon)
{
	ASSERT(icon);

	if( icon->GetType() == WT_ITEM )	
		return m_pItemGridDialog[m_curTabNum]->FakeMoveItem(x, y, (CItem*)icon);	
	else
		return FALSE;
}


void CItemShopDialog::TabChange(LONG ID)
{
	for(int i=0; i<MAX_ITEMSHOP_PAGE; i++)
	{
		m_pPageBtn[i]->SetPush(FALSE);
		m_pItemGridDialog[i]->SetActive(FALSE);
	}
	m_pPageBtn[ID]->SetPush(TRUE);
	m_pItemGridDialog[ID]->SetActive(TRUE);
	m_curTabNum = ID;
}


CItem * CItemShopDialog::GetItemForPos(POSTYPE absPos)
{
	if(TP_SHOPITEM_START <= absPos && absPos < TP_SHOPITEM_END)
	{
		int num = GetTabNumber(absPos);
		return (CItem *)m_pItemGridDialog[num]->GetIconForIdx(absPos-TP_SHOPITEM_START-TABCELL_SHOPITEM_NUM*num);
	}
	else
		return NULL;
}