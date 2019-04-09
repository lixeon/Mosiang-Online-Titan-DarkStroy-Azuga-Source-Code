#include "stdafx.h"
#include ".\rarecreatedialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cIcon.h"
#include "./Interface/cIconDialog.h"
#include "Item.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "./Interface/cScriptManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"



CRareCreateDialog::CRareCreateDialog(void)
{
	m_type = WT_ITEM_RARECREATE_DLG;

	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	m_VirtualItem.SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
	m_VirtualItem.SetMovable(FALSE);

	m_ItemIdx = 0;
	m_ItemPos = 0;
}

CRareCreateDialog::~CRareCreateDialog(void)
{
}

void CRareCreateDialog::Linking()
{
	m_pIconDlg = (cIconDialog*)GetWindowForID( RareCreate_ICON );
}

BOOL CRareCreateDialog::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if(pOrigIcon->GetType() != WT_ITEM) return FALSE;
	if( m_bDisable ) return FALSE;

	CItem * pOrigItem = (CItem *)pOrigIcon;
	if( pOrigItem->IsLocked() ) return FALSE;

	ITEM_INFO* pBaseItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx() );
	if( !pBaseItemInfo )
		return FALSE;

	// 장비아이템 체크
	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if( !(bits & eEQUIP_ITEM) )
		return FALSE;

	// 06.09.25 RaMa - 이벤트아이템을 강화, 레어를 못 만들도록
	if( pBaseItemInfo->WeaponType > WP_KEY )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1455) );
		return FALSE;
	}

	// 조합된건지 체크
	if( bits <= eEQUIP_ITEM_SHOES && pOrigItem->GetItemIdx() % 10 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1234) );
		return FALSE;
	}

	// 강화 아이템은 못 올라간다.
	ITEM_OPTION_INFO* pOptionInfo = ITEMMGR->GetItemOption( pOrigItem->GetDurability() );	
	if( pOrigItem->GetDurability() != 0 || pOptionInfo->dwOptionIdx != 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1234) );
		return FALSE;
	}

	// 레어가 가능한지 판별
	if( !ITEMMGR->IsRareItemAble( pOrigItem->GetItemIdx() ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1234) );
		return FALSE;
	}

	// 인벤토리에서 오는지 체크
	if(!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()))
		return FALSE;

	ITEM_INFO* pShopItem = ITEMMGR->GetItemInfo( (WORD)m_ItemIdx );
	if( !pShopItem )
		return FALSE;

	if( pBaseItemInfo->LimitLevel < pShopItem->GenGol || pBaseItemInfo->LimitLevel > pShopItem->MinChub )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
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

void CRareCreateDialog::ReleaseItem()
{
	if( m_pIconDlg )
	{
		CVirtualItem* pVItem = NULL;

		m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );
		if( pVItem )			
			pVItem->GetLinkItem()->SetLock( FALSE );

		m_VirtualItem.SetLinkItem( NULL );
	}

	CItem* pOriItem = ITEMMGR->GetItemofTable( eItemTable_ShopInven, (POSTYPE)m_ItemPos );
	if( pOriItem )
		pOriItem->SetLock( FALSE );

	m_ItemIdx = 0;
	m_ItemPos = 0;


	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

	SetActive( FALSE );
}

void CRareCreateDialog::AddVirtualItem( CItem* pItem )
{
	m_VirtualItem.SetData( pItem->GetItemIdx() );
	m_VirtualItem.SetLinkItem( pItem );

	SHOPITEMBASE* pShopItemInfo = NULL;
	pShopItemInfo = ITEMMGR->GetUsedItemInfo( pItem->GetItemBaseInfo()->wIconIdx );
	if( pShopItemInfo )
		ITEMMGR->AddUsedAvatarItemToolTip( pShopItemInfo );
	m_pIconDlg->AddIcon( 0, (cIcon*)&m_VirtualItem );
}

void CRareCreateDialog::ItemRareCreateSyn()
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
	SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_RARECREATE_SYN );
	msg.dwObjectID	= HEROID;
	msg.dwData1 = m_ItemIdx;
	msg.dwData2 = m_ItemPos;
	msg.dwData3 = pItem->GetItemBaseInfo()->wIconIdx;
	msg.dwData4 = pItem->GetItemBaseInfo()->Position;
	NETWORK->Send( &msg, sizeof(msg) );

	SetActive( FALSE );
}

void CRareCreateDialog::ItemRareCreateAck()
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

		ITEMMGR->SetToolTipIcon( (cIcon*)pItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()) );

		m_VirtualItem.SetLinkItem( NULL );
	}

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
}

void CRareCreateDialog::ItemRareCreateNAck()
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

