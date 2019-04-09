#include "stdafx.h"
#include ".\petupgradedialog.h"
#include "./Interface/cIconDialog.h"
#include "./interface/cStatic.h"
#include "./Interface/cScriptManager.h"
#include "WindowIDEnum.h"
#include "Item.h"
#include "ItemManager.h"
#include "Pet.h"	//define
#include "PetManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "ChatManager.h"

CPetUpgradeDialog::CPetUpgradeDialog(void)
{
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	m_VirtualItem.SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
	m_VirtualItem.SetMovable(FALSE);

	m_pIconDlg = NULL;
	m_pGradeText[0]	= NULL;
	m_pGradeText[1]	= NULL;
	m_UpgradeItemPos = 0;
	m_UpgradeItemIdx = 0;

	m_iGrade	= ePUDG_Default;
}

CPetUpgradeDialog::~CPetUpgradeDialog(void)
{
}

void CPetUpgradeDialog::Linking()
{
	m_pIconDlg = (cIconDialog*)GetWindowForID( PetUpgrade_ICON );
	m_pGradeText[0] = (cStatic*)GetWindowForID( PU_UPGRADE2 );
	m_pGradeText[1] = (cStatic*)GetWindowForID( PU_UPGRADE3 );
}

void CPetUpgradeDialog::SetActive( BOOL val )
{
	if(val)
	{
		BOOL Bit = val-1;
		m_pGradeText[Bit]->SetActive(TRUE);
		m_pGradeText[!Bit]->SetActive(FALSE);

// 		if( 1 == val )
// 		{
// 			m_pGradeText[0]->SetActive(TRUE);
// 			m_pGradeText[1]->SetActive(FALSE);
// 		}
// 		else if( 2 == val )
// 		{
// 			m_pGradeText[0]->SetActive(FALSE);
// 			m_pGradeText[1]->SetActive(TRUE);
// 		}
	}

	m_iGrade = val;

	cDialog::SetActive(val);
}

BOOL CPetUpgradeDialog::FakeMoveIcon( LONG x, LONG y, cIcon * icon )
{
	if( icon->GetType() != WT_ITEM )	return FALSE;
	if( m_bDisable )	return FALSE;

	CItem* pOrigItem = (CItem*)icon;

	if( pOrigItem->IsLocked() )	return FALSE;

	ITEM_INFO* pBaseItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx() );
	if(!pBaseItemInfo)	return FALSE;

	//펫 소환 아이템인가
	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if( !(bits & eSHOP_ITEM_PET) && !(bits & eQUEST_ITEM_PET) )
		return FALSE;
	else
	{//친밀도 검사
		DWORD friendly = PETMGR->GetPetFriendship(pOrigItem->GetDBIdx());
		if( GAMERESRCMNGR->GetPetRule()->MaxFriendship != friendly )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1251) );
			return FALSE;
		}
		if( PETMGR->IsCurPetSummonItem(pOrigItem->GetDBIdx()) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1252) );
			return FALSE;
		}
	}
	
	if( m_iGrade != ((PET_TOTALINFO*)PETMGR->GetPetInfo(pOrigItem->GetDBIdx()))->PetGrade )
	{
		return FALSE;
	}

// 	{//펫 소환 아이템이면 죽은 펫이 맞는가
// 		PET_TOTALINFO* pInfo = PETMGR->GetPetInfo(pOrigItem->GetDBIdx());
// 		if(!pInfo)	return FALSE;
// 
// 		if( pInfo->bAlive || pInfo->PetFriendly )
// 			return FALSE;
// 	}

	//인벤토리에서 오는가
	if( !ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition())
		&& !ITEMMGR->IsEqualTableIdxForPos(eItemTable_ShopInven, pOrigItem->GetPosition()) )
		return FALSE;

	pOrigItem->SetLock(TRUE);
	AddVirtualItem( pOrigItem );
	
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	return FALSE;
}

void CPetUpgradeDialog::ReleaseItem()
{
	if( m_pIconDlg )
	{
		CVirtualItem* pVItem = NULL;

		m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );
		if( pVItem )
			pVItem->GetLinkItem()->SetLock( FALSE );

		m_VirtualItem.SetLinkItem( NULL );
	}

	InitUpgradeItemInfo();

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

	SetActive( FALSE );
}

void CPetUpgradeDialog::AddVirtualItem( CItem* pItem )
{
	m_VirtualItem.SetData( pItem->GetItemIdx() );
	m_VirtualItem.SetLinkItem( pItem );

	m_pIconDlg->AddIcon( 0, (cIcon*)&m_VirtualItem );

	//펫 툴팁 추가
	ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, NULL, NULL, pItem->GetDBIdx());
}

void CPetUpgradeDialog::PetUpgradeSyn()
{
	if( !m_VirtualItem.GetLinkItem() )	return;

	CItem* pItem = ITEMMGR->GetItem( m_VirtualItem.GetLinkItem()->GetDBIdx() );
	if(!pItem)
	{
		if( m_pIconDlg )
		{
			CVirtualItem* pVItem = NULL;
			m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
		}

		InitUpgradeItemInfo();

		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);

		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		return;
	}

	MSG_DWORD3	msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_UPGRADE_SYN;
	msg.dwObjectID = HEROID;

	msg.dwData1 = pItem->GetDBIdx();
	msg.dwData2 = m_UpgradeItemPos;
	msg.dwData3 = m_UpgradeItemIdx;

	NETWORK->Send( &msg, sizeof(msg) );

	SetActive( FALSE );
}

void CPetUpgradeDialog::PetUpgradeAck()
{
	CVirtualItem* pVItem = NULL;
	
	CItem* pItem = ITEMMGR->GetItem( m_VirtualItem.GetLinkItem()->GetDBIdx() );

	if( pItem )
	{
		pItem->SetLock( FALSE );

		if( m_pIconDlg )
			m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );

		//정보 갱신.
		PET_TOTALINFO* pInfo = PETMGR->GetPetInfo(pItem->GetDBIdx());
		BASE_PET_LIST* pList = GAMERESRCMNGR->GetPetListInfo(pInfo->PetKind);
		if(pInfo)
		{
			ASSERT(pInfo->PetGrade < PET_MAX_GRADE);
			++pInfo->PetGrade;
			DWORD friendly = GAMERESRCMNGR->GetPetRule()->DefaultFriendship;
			pInfo->PetFriendly = friendly;
			pInfo->PetStamina = pList->StaminaMax[pInfo->PetGrade-1];
		}

		ITEMMGR->SetToolTipIcon( (cIcon*)pItem );

		m_VirtualItem.SetLinkItem( NULL );
	}

	InitUpgradeItemInfo();

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
}

void CPetUpgradeDialog::PetUpgradeNack()
{
	CVirtualItem* pVItem = NULL;

	CItem* pItem = ITEMMGR->GetItem( m_VirtualItem.GetLinkItem()->GetDBIdx() );
	if( pItem )
	{
		pItem->SetLock( FALSE );

		if( m_pIconDlg )
			m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );

		m_VirtualItem.SetLinkItem( NULL );
	}

	InitUpgradeItemInfo();

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
}
