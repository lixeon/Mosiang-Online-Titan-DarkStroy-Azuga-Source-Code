// CReinforceResetDlg.cpp: implementation of the CReinforceResetDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReinforceResetDlg.h"
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

CReinforceResetDlg::CReinforceResetDlg()
{
	m_type = WT_ITEM_REINFORCERESET_DLG;

	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	m_VirtualItem.SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
	m_VirtualItem.SetMovable(FALSE);

	m_ItemIdx = 0;
	m_ItemPos = 0;
}

CReinforceResetDlg::~CReinforceResetDlg()
{

}


void CReinforceResetDlg::Linking()
{
	m_pIconDlg = (cIconDialog*)GetWindowForID( RF_DefaultICON );
}


BOOL CReinforceResetDlg::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if(pOrigIcon->GetType() != WT_ITEM) return FALSE;
	if( m_bDisable ) return FALSE;

	CItem * pOrigItem = (CItem *)pOrigIcon;
	if( pOrigItem->IsLocked() ) return FALSE;
	
	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if( !(bits & eEQUIP_ITEM) || bits > eEQUIP_ITEM_SHOES )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1231) );
		return FALSE;
	}
	ITEM_OPTION_INFO* pOptionInfo = ITEMMGR->GetItemOption( pOrigItem->GetDurability() );	
	if( pOrigItem->GetDurability() == 0 || pOptionInfo->dwOptionIdx == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1232) );
		return FALSE;
	}
	if(!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()))
		return FALSE;

	pOrigItem->SetLock(TRUE);	
	AddVirtualItem( pOrigItem );

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

	return FALSE;
}



void CReinforceResetDlg::ReleaseItem()
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


	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

	SetActive( FALSE );
}



void CReinforceResetDlg::AddVirtualItem( CItem* pItem )
{
	m_VirtualItem.SetData( pItem->GetItemIdx() );
	m_VirtualItem.SetLinkItem( pItem );
	
	SHOPITEMBASE* pShopItemInfo = NULL;
	pShopItemInfo = ITEMMGR->GetUsedItemInfo( pItem->GetItemBaseInfo()->wIconIdx );
	if( pShopItemInfo )
		ITEMMGR->AddUsedAvatarItemToolTip( pShopItemInfo );
	m_pIconDlg->AddIcon( 0, (cIcon*)&m_VirtualItem );
}



void CReinforceResetDlg::ItemResetSyn()
{
	if( !m_VirtualItem.GetLinkItem() )		return;

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
	SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_REINFORCERESET_SYN );
	msg.dwObjectID	= HEROID;
	msg.dwData1 = m_ItemIdx;
	msg.dwData2 = m_ItemPos;
	msg.dwData3 = pItem->GetItemBaseInfo()->wIconIdx;
	msg.dwData4 = pItem->GetItemBaseInfo()->Position;
	NETWORK->Send( &msg, sizeof(msg) );

	SetActive( FALSE );
}


void CReinforceResetDlg::ItemResetAck()
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

		ITEMMGR->RemoveItemOption( pItem->GetDurability() );
		pItem->SetDurability( 0 );		
		ITEMMGR->SetToolTipIcon( (cIcon*)pItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()) );

		m_VirtualItem.SetLinkItem( NULL );
	}

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
}


void CReinforceResetDlg::ItemResetNAck()
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
