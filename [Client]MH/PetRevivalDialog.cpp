#include "stdafx.h"
#include ".\petrevivaldialog.h"
#include "./Interface/cIconDialog.h"
#include "./interface/cStatic.h"
#include "./Interface/cScriptManager.h"
#include "WindowIDEnum.h"
#include "Item.h"
#include "ItemManager.h"
#include "Pet.h"
#include "PetManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "ChatManager.h"

CPetRevivalDialog::CPetRevivalDialog(void)
{
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	m_VirtualItem.SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
	m_VirtualItem.SetMovable(FALSE);

	m_pIconDlg = NULL;
	m_pGradeText[0]	= NULL;
	m_pGradeText[1]	= NULL;
	m_pGradeText[2]	= NULL;

	m_ItemIdx = 0;
	m_ItemPos = 0;

	m_iGrade = ePRDG_Default;
}

CPetRevivalDialog::~CPetRevivalDialog(void)
{
}

void CPetRevivalDialog::Linking()
{
	m_pIconDlg = (cIconDialog*)GetWindowForID( Pet_Revival_ICON );
	m_pGradeText[0] = (cStatic*)GetWindowForID( PR_GRADE1 );
	m_pGradeText[1] = (cStatic*)GetWindowForID( PR_GRADE2 );
	m_pGradeText[2] = (cStatic*)GetWindowForID( PR_GRADE3 );
}

void CPetRevivalDialog::SetActiveWithGradeTxt( BOOL val )
{
	if(val)
	for(int i = 0; i < 3; ++i )
	{
		if(i+1 == val)
		{
			m_pGradeText[i]->SetActive(TRUE);
		}
		else
		{
			m_pGradeText[i]->SetActive(FALSE);
		}
	}

	m_iGrade = val;

	cDialog::SetActive(val);
}

void CPetRevivalDialog::SetActive( BOOL val )
{
	if(FALSE == val && m_iGrade)
		m_pGradeText[m_iGrade-1]->SetActive(FALSE);

	m_iGrade = ePRDG_Default;

	cDialog::SetActive(val);
}

BOOL CPetRevivalDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
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
	{//친밀도 검사	//펫 소환 아이템이면 죽은 펫이 맞는가
		PET_TOTALINFO* pInfo = PETMGR->GetPetInfo(pOrigItem->GetDBIdx());
		if(!pInfo)	return FALSE;

		if( pInfo->bAlive || pInfo->PetFriendly )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1250) );
			return FALSE;
		}
	}

	if( m_iGrade )	//ShopItem 에서는 0
	if( m_iGrade != ((PET_TOTALINFO*)PETMGR->GetPetInfo(pOrigItem->GetDBIdx()))->PetGrade )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1283) );
		return FALSE;
	}

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

void CPetRevivalDialog::ReleaseItem()
{
	if( m_pIconDlg )
	{
		CVirtualItem* pVItem = NULL;

		m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );
		if( pVItem )
			pVItem->GetLinkItem()->SetLock( FALSE );

		m_VirtualItem.SetLinkItem( NULL );
	}

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

	SetActive( FALSE );
}

void CPetRevivalDialog::AddVirtualItem( CItem* pItem )
{
	m_VirtualItem.SetData( pItem->GetItemIdx() );
	m_VirtualItem.SetLinkItem( pItem );

	m_pIconDlg->AddIcon( 0, (cIcon*)&m_VirtualItem );

	//펫 툴팁 추가
	ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, NULL, NULL, pItem->GetDBIdx());
}

void CPetRevivalDialog::PetRevivalSyn()
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

		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);

		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		return;
	}

	MSG_DWORD4	msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_REVIVAL_SYN;
	msg.dwObjectID = HEROID;

	msg.dwData1 = pItem->GetDBIdx();
	msg.dwData2 = m_ItemPos;
	msg.dwData3 = m_ItemIdx;
	msg.dwData4 = m_iGrade;

	NETWORK->Send( &msg, sizeof(msg) );

	SetActive( FALSE );
}

void CPetRevivalDialog::PetRevivalAck( int flgFromShopItem )
{
	CVirtualItem* pVItem = NULL;

	CItem* pItem = ITEMMGR->GetItem( m_VirtualItem.GetLinkItem()->GetDBIdx() );

	if( pItem )
	{
		pItem->SetLock( FALSE );
		DWORD FullColor = 0xffffffff;
		ITEMMGR->SetIconColorFilter((cIcon*)pItem, FullColor);
		if( m_pIconDlg )
			m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );

		//정보 갱신.
		PET_TOTALINFO* pInfo = PETMGR->GetPetInfo(pItem->GetDBIdx());
		if( !pInfo )
		{
			ASSERT(0);
			return;
		}

		BASE_PET_LIST* pList = GAMERESRCMNGR->GetPetListInfo(pInfo->PetKind);
		if( !pList )	return;

		if(pInfo)
		{
			ASSERT(FALSE==pInfo->bAlive);

			pInfo->bAlive = TRUE;
			DWORD friendly = 0;
			if(0==flgFromShopItem)
				friendly = GAMERESRCMNGR->GetPetRule()->RevivalFriendship_Shop;
			else
				friendly = GAMERESRCMNGR->GetPetRule()->RevivalFriendship;
			pInfo->PetFriendly = friendly;
			pInfo->PetStamina = pList->StaminaMax[pInfo->PetGrade-1];
		}

		ITEMMGR->SetToolTipIcon( (cIcon*)pItem );

		m_VirtualItem.SetLinkItem( NULL );
	}


	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
}

void CPetRevivalDialog::PetRevivalNack()
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

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

}