// SealDialog.cpp: implementation of the CSealDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SealDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cIcon.h"
#include "./Interface/cIconDialog.h"
#include "Item.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "./Interface/cScriptManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSealDialog::CSealDialog()
{
	m_type = WT_ITEM_SEAL_DLG;
	
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	m_VirtualItem.SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
	m_VirtualItem.SetMovable(FALSE);

	m_ItemIdx = 0;
	m_ItemPos = 0;
}

CSealDialog::~CSealDialog()
{

}


void CSealDialog::Linking()
{
	m_pIconDlg = (cIconDialog*)GetWindowForID( IT_LOCKICON );
}


void CSealDialog::ReleaseItem()
{
	if( m_pIconDlg )
	{
		CVirtualItem* pVItem = NULL;

		m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );
		if( pVItem )			
			pVItem->GetLinkItem()->SetLock( FALSE );
		
		m_VirtualItem.SetLinkItem( NULL );
	}

	m_ItemIdx = 0;
	m_ItemPos = 0;

	// 서버로 취소했다고 보낸다.

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

	SetActive( FALSE );
}


BOOL CSealDialog::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if(pOrigIcon->GetType() != WT_ITEM) return FALSE;
	if( m_bDisable ) return FALSE;

	CItem * pOrigItem = (CItem *)pOrigIcon;
	if( pOrigItem->IsLocked() ) return FALSE;
	
	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if( pOrigItem->GetItemKind() != eSHOP_ITEM_MAKEUP && pOrigItem->GetItemKind() !=eSHOP_ITEM_DECORATION )	
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1078) );
		return FALSE;
	}
	if( pOrigItem->GetItemInfo()->SellPrice != eShopItemUseParam_Forever )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1078) );
		return FALSE;
	}
	if( (pOrigItem->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1078) );
		return FALSE;
	}
	if( pOrigItem->GetUseParam() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1077) );
		return FALSE;
	}

	pOrigItem->SetLock(TRUE);
	
	AddVirtualItem( pOrigItem );

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

	return FALSE;
}


void CSealDialog::AddVirtualItem( CItem* pItem )
{
	m_VirtualItem.SetData( pItem->GetItemIdx() );
	m_VirtualItem.SetLinkItem( pItem );
	
	SHOPITEMBASE* pShopItemInfo = NULL;
	pShopItemInfo = ITEMMGR->GetUsedItemInfo( pItem->GetItemBaseInfo()->wIconIdx );
	if( pShopItemInfo )
		ITEMMGR->AddUsedAvatarItemToolTip( pShopItemInfo );
	m_pIconDlg->AddIcon( 0, (cIcon*)&m_VirtualItem );
}


void CSealDialog::ItemSealSyn()
{
	if( !m_VirtualItem.GetLinkItem() )			return;
	
	CItem* pItem = ITEMMGR->GetItem( m_VirtualItem.GetLinkItem()->GetDBIdx() );
	if( !pItem )
	{
		if( m_pIconDlg )
		{
			CVirtualItem* pVItem = NULL;
			m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )			
				pVItem->GetLinkItem()->SetLock( FALSE );
		}

		m_ItemIdx = 0;
		m_ItemPos = 0;

		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		return;
	}
	
	MSG_DWORD4	msg;
	SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_SEAL_SYN );
	msg.dwObjectID	= HEROID;
	msg.dwData1 = m_ItemIdx;
	msg.dwData2 = m_ItemPos;
	msg.dwData3 = pItem->GetItemBaseInfo()->wIconIdx;
	msg.dwData4 = pItem->GetItemBaseInfo()->Position;
	NETWORK->Send( &msg, sizeof(msg) );

	SetActive( FALSE );
}


void CSealDialog::ItemSealAck()
{
	CVirtualItem* pVItem = NULL;

	m_ItemIdx = 0;
	m_ItemPos = 0;

	CItem* pItem = ITEMMGR->GetItem( m_VirtualItem.GetLinkItem()->GetDBIdx() );
	if( pItem )
	{
		DWORD Param = pItem->GetItemBaseInfo()->ItemParam;
		Param ^= ITEM_PARAM_SEAL;
		pItem->SetItemParam( Param );
		pItem->SetLock( FALSE );

		if( m_pIconDlg )
			m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );
			
		SHOPITEMBASE* pShopItemInfo = NULL;
		pShopItemInfo = ITEMMGR->GetUsedItemInfo( pItem->GetItemBaseInfo()->wIconIdx );
		if( pShopItemInfo )
		{
			ITEMMGR->SetToolTipIcon( (cIcon*)pItem );
			ITEMMGR->RemoveUsedItemInfo( pShopItemInfo->ItemBase.wIconIdx );
		}

		m_VirtualItem.SetLinkItem( NULL );
	}

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
}


void CSealDialog::ItemSealNAck()
{
	CVirtualItem* pVItem = NULL;

	m_ItemIdx = 0;
	m_ItemPos = 0;

	CItem* pItem = ITEMMGR->GetItem( m_VirtualItem.GetLinkItem()->GetDBIdx() );
	if( pItem )
	{
		pItem->SetLock( FALSE );

		if( m_pIconDlg )
			m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );

		m_VirtualItem.SetLinkItem( NULL );
	}

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
}
