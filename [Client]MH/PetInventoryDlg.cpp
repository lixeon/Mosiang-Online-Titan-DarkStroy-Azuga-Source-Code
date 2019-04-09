#include "stdafx.h"
#include ".\petinventorydlg.h"
#include ".\petwearedexdialog.h"

#include "ItemManager.h"
#include "ChatManager.h"

#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cStatic.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cScriptManager.h"

#include "cDivideBox.h"

#include "./Input/UserInput.h"
#include "./Audio/MHAudioManager.h"

#include "ObjectManager.h"
#include "PKManager.h"

#include "PetManager.h"

#include "InventoryExDialog.h"
#include "ItemShopInven.h"
#include "GameIn.h"

CPetInventoryDlg::CPetInventoryDlg(void)
{
	m_pPetWearedDlg = NULL;

	m_dwValidTabNum = 0;

	m_bItemInfoInit	= FALSE;

	m_BtnPushstartTime = 0;
	m_BtnPushDelayTime = 0;
	m_bPushTabWithMouseOver	= FALSE;

	for( int i = 0; i < TAB_PET_INVENTORY_NUM; ++i )
	{
		m_pIconGridDlg[i] = NULL;
	}
}

CPetInventoryDlg::~CPetInventoryDlg(void)
{
	m_pPetWearedDlg = NULL;

	m_dwValidTabNum = 0;

	m_bItemInfoInit	= FALSE;

	m_BtnPushstartTime = 0;
	m_BtnPushDelayTime = 500;
	m_bPushTabWithMouseOver	= FALSE;
}

void CPetInventoryDlg::Linking()
{
	m_pPetWearedDlg = (CPetWearedExDialog*)GetWindowForID(PET_WEAREDDLG);
	m_pIconGridDlg[0] = (cIconGridDialog*)GetWindowForID(PET_TABDLG1);
	m_pIconGridDlg[1] = (cIconGridDialog*)GetWindowForID(PET_TABDLG2);
	m_pIconGridDlg[2] = (cIconGridDialog*)GetWindowForID(PET_TABDLG3);

	SCRIPTMGR->GetImage( 65, &m_LockImage, PFT_HARDPATH );
}

void CPetInventoryDlg::Render()
{
	cDialog::Render();
	cTabDialog::RenderTabComponent();

	VECTOR2 vPos;

//	for( int i = (TAB_PET_INVENTORY_NUM - m_dwValidTabNum); i < TAB_PET_INVENTORY_NUM; ++i )
	if(!IsActive())	return;
		
//	if(m_pIconGridDlg[0]->IsActive() && m_pIconGridDlg[1]->IsActive())	return;

	for( int i = m_dwValidTabNum; i < TAB_PET_INVENTORY_NUM; ++i )
	{
		if(!m_pIconGridDlg[i]->IsActive()) continue;	//y?초기값이 모두 TRUE 이다.

		for( int j = 0; j < TABCELL_PETINVEN_NUM; ++j )
		{
			if( !m_pIconGridDlg[i]->IsAddable( j ) ) continue;
			vPos.x = ( m_pIconGridDlg[i]->m_absPos.x + 5 ) + ( j % 5 ) * 45;
			vPos.y = ( m_pIconGridDlg[i]->m_absPos.y + 5 ) + ( j / 5 ) * 45;

			m_LockImage.RenderSprite( NULL, NULL, 0.0f, &vPos,
				RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );
		}
	}
}

void CPetInventoryDlg::SetActive(BOOL bVal)
{
	cTabDialog::SetActive(bVal);
}

void CPetInventoryDlg::Add( cWindow * window )
{
	if(window->GetType() == WT_PUSHUPBUTTON)
	{
		AddTabBtn(curIdx1++, (cPushupButton*)window);
	}
	else if(window->GetType() == WT_ICONGRIDDIALOG)
	{
		AddTabSheet(curIdx2++, window);
		((cIconGridDialog*)window)->SetDragOverIconType( WT_ITEM );
	}
	else
		cTabDialog::Add(window);
}

DWORD CPetInventoryDlg::ActionEvent(CMouse * mouseInfo)
{
	//아이콘 드래그중 탭이동 처리
	if( WINDOWMGR->IsDragWindow() && WINDOWMGR->GetDragDlg() )
	{
		if( WINDOWMGR->GetDragDlg()->GetType() == WT_ITEM )
		{
			DWORD we = WE_NULL;
			if(!m_bActive) return we;
			we = cDialog::ActionEvent(mouseInfo);

			BOOL bMouseOver = FALSE;
			for( int i = 0; i < m_bTabNum; ++i )
			{
				m_ppPushupTabBtn[i]->ActionEvent( mouseInfo );

				if( we & WE_MOUSEOVER )
					if( m_ppPushupTabBtn[i]->IsActive() && m_ppPushupTabBtn[i]->PtInWindow( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )
					{
						bMouseOver = TRUE;

						if( m_bPushTabWithMouseOver )
						{
							if( i != m_bSelTabNum )
							{
								SelectTab(i);
								m_bSelTabNum = i;
							}
						}
						else
						{
							if( 0 == m_BtnPushstartTime )
							{
								m_BtnPushstartTime = gCurTime;
							}
							else if( gCurTime - m_BtnPushstartTime > m_BtnPushDelayTime )
							{
								m_bPushTabWithMouseOver = TRUE;
							}
						}
					}
			}

			if( !bMouseOver )
				m_BtnPushstartTime = 0;

			we |= m_ppWindowTabSheet[m_bSelTabNum]->ActionEvent( mouseInfo );
			return we;
		}
	}

	m_BtnPushstartTime = 0;
	m_bPushTabWithMouseOver = FALSE;
	return cTabDialog::ActionEvent( mouseInfo );
}

void CPetInventoryDlg::OnActionEvent( LONG lId, void *p, DWORD we )
{
	if( we == WE_LBTNDBLCLICK )
	{
		CItem* pItem = NULL;
		if( lId == PET_WEAREDDLG )
		{
			pItem = (CItem*)m_pPetWearedDlg->GetIconForIdx((WORD)m_pPetWearedDlg->GetCurSelCellPos());
			if(!pItem)	return;
		}
		else
			return;

		POSTYPE emptyPos = 0;
		CInventoryExDialog* pInven = GAMEIN->GetInventoryDialog();
		if(!pInven)	return;
		CItemShopInven* pShopInven = pInven->GetShopInven();
		if(!pShopInven)	return;
		if( pShopInven->GetBlankPositionRestrictRef(emptyPos) )
		{
			CItem* pToItem = pInven->GetItemForPos(emptyPos);
			FakeGeneralItemMove(emptyPos, pItem, pToItem);
			return;
		}
		else
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1292) );
			return;
		}
	}
}

void CPetInventoryDlg::ApplyValidInvenTab( DWORD dwValidTabNum )
{//아이템과 그리드 유/무효 처리
	m_dwValidTabNum = dwValidTabNum;

	POSTYPE InvalidStartPos = 0;
	InvalidStartPos = POSTYPE(TP_PETINVEN_START + TABCELL_PETINVEN_NUM * dwValidTabNum);

	POSTYPE absPos = 0;
	for( absPos = TP_PETINVEN_START; absPos < InvalidStartPos; ++absPos )
	{
		CItem* pItem = GetItemForPos(absPos);
		if( pItem )
		{
			pItem->SetLock(FALSE);
		}
	}

	for( absPos = InvalidStartPos; absPos < TP_PETINVEN_END; ++absPos )
	{
		CItem* pItem = GetItemForPos(absPos);
		if( pItem )
		{
			pItem->SetLock(TRUE);
		}
	}
}

WORD CPetInventoryDlg::GetPetIvenTabIndex( POSTYPE absPos )
{
	return ( absPos - TP_PETINVEN_START ) / TABCELL_PETINVEN_NUM;
}

BOOL CPetInventoryDlg::AddItem( ITEMBASE* pItemBase )
{
	if( pItemBase->dwDBIdx == 0 )
	{
//		ASSERTMSG( 0, "PetInven NewItem DBIdx is Null");
		return FALSE;
	}
	CItem* pNewItem = ITEMMGR->MakeNewItem( pItemBase, "CPetInventoryDlg::AddItem" );
	if(pNewItem == NULL)
		return FALSE;

	return AddItem(pNewItem);
}

BOOL CPetInventoryDlg::AddItem(CItem* pItem)
{
	if( NULL == pItem )
	{
		ASSERT(pItem);
		return FALSE;
	}

	ITEMMGR->RefreshItem(pItem);

	int Pos = pItem->GetPosition();
	if( TP_PETINVEN_START <= Pos && Pos < TP_PETINVEN_END )
	{
		int TabIdx = GetPetIvenTabIndex(Pos);
		cIconGridDialog* dlg = (cIconGridDialog*)GetTabSheet(TabIdx);

		WORD RelPos = Pos - TP_PETINVEN_START - (TABCELL_PETINVEN_NUM * TabIdx);
		CItem* pExItem = (CItem*)dlg->GetIconForIdx(RelPos);	//동일위치에 기존아이템이 있으면
		if(pExItem)
		{
			ASSERT(0);
			return FALSE;
		}
		else
		{
			return dlg->AddIcon(RelPos, pItem);
		}
	}
	else if( TP_PETWEAR_START <= Pos && Pos < TP_PETWEAR_END )
	{
		return m_pPetWearedDlg->AddItem(Pos - TP_PETWEAR_START, pItem);
	}
	else
	{
		return FALSE;
	}
}

BOOL CPetInventoryDlg::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	if( TP_PETINVEN_START <= Pos && Pos < TP_PETINVEN_END )
	{
		int TabIdx = GetPetIvenTabIndex(Pos);
		cIconGridDialog* dlg = (cIconGridDialog*)GetTabSheet(TabIdx);

		WORD RelPos = Pos - TP_PETINVEN_START - (TABCELL_PETINVEN_NUM * TabIdx);
		if(!dlg->IsAddable(RelPos))
		{
			return dlg->DeleteIcon(RelPos, (cIcon**)ppItem);
		}
		else
			return FALSE;
	}
	else if( TP_PETWEAR_START <= Pos && Pos < TP_PETWEAR_END )
	{
		if(!m_pPetWearedDlg->IsAddable(Pos - TP_PETWEAR_START))
		{
			return m_pPetWearedDlg->DeleteItem(Pos - TP_PETWEAR_START, (cIcon**)ppItem);
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}

CItem* CPetInventoryDlg::GetItemForPos(POSTYPE absPos)
{
	if( TP_PETINVEN_START <= absPos && absPos < TP_PETINVEN_END )
	{
		int tabNum = GetPetIvenTabIndex(absPos);

		return (CItem*)m_pIconGridDlg[tabNum]->GetIconForIdx(absPos - TP_PETINVEN_START - TABCELL_PETINVEN_NUM*tabNum);
//		int TabIdx = GetPetIvenTabIndex(absPos);
//		cIconGridDialog* dlg = (cIconGridDialog*)GetTabSheet(TabIdx);
//
//		WORD RelPos = absPos - TP_PETINVEN_START - (TABCELL_PETINVEN_NUM * TabIdx);
//
//		return (CItem*)dlg->GetIconForIdx(RelPos);
	}
	else if( TP_PETWEAR_START <= absPos && absPos < TP_PETWEAR_END )
	{
		return (CItem*)m_pPetWearedDlg->GetIconForIdx(absPos - TP_PETWEAR_START);
	}
	else
		return NULL;
}

BOOL CPetInventoryDlg::FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( !CanBeMoved( pFromItem, ToPos) )
		return FALSE;
	if( pFromItem->GetPosition() == ToPos )
		return FALSE;
	if( pFromItem->IsLocked() )
		return FALSE;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if( eItemTable_Max <= FromIdx )	return FALSE;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if( eItemTable_Max <= ToIdx )	return FALSE;

	if( 1 == pFromItem->GetDurability() )
	{
		return FakeGeneralItemMove(ToPos, pFromItem, pToItem);
	}

	m_MsgDivide.Category			= MP_ITEM;
	m_MsgDivide.Protocol			= MP_ITEM_DIVIDE_SYN;
	m_MsgDivide.dwObjectID			= HEROID;

	m_MsgDivide.wItemIdx = pFromItem->GetItemIdx();
	m_MsgDivide.FromPos = pFromItem->GetPosition();
	m_MsgDivide.FromDur = pFromItem->GetDurability();
	m_MsgDivide.ToPos = ToPos;
	m_MsgDivide.ToDur = 0;

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_DIVIDE_INV, (LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), OnFakeItemDivideOk, OnFakeItemDivideCancel, this, pFromItem, CHATMGR->GetChatMsg(185) );
	pDivideBox->SetMaxValue( 5 );
	pDivideBox->SetValue( 1 );	//나누는데 맥스치를 넣기도 0을넣기도 이상... 1이 젤 무난하다.

	return FALSE;
}

BOOL CPetInventoryDlg::FakeItemCombine( POSTYPE ToPos, CItem* pFromItem, CItem* pToItem )
{
	//제자리면
	if( pFromItem->GetPosition() == pToItem->GetPosition() )
		return FALSE;
	//다른 아이템이면
	if( pFromItem->GetItemIdx() != pToItem->GetItemIdx() )
		return FALSE;

	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return FALSE;

	//기존 아이템이 위치 교환이 안되면
	if( pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
		return FALSE;

	if( pFromItem->IsLocked() || pToItem->IsLocked() )
		return FALSE;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max)	return FALSE;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max)		return FALSE;

	MSG_ITEM_COMBINE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_COMBINE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.ToPos				= pToItem->GetPosition();
	msg.wItemIdx			= pFromItem->GetItemIdx();
	msg.FromDur				= pFromItem->GetDurability();
	msg.ToDur				= pToItem->GetDurability();

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	NETWORK->Send( &msg, sizeof(msg) );

	return TRUE;
}

void CPetInventoryDlg::OnFakeItemDivideCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
}

void CPetInventoryDlg::OnFakeItemDivideOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CPetInventoryDlg* tDlg = (CPetInventoryDlg*)vData1;
	if( 0 == param1 )
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return;
	}

	WORD TargetTableIdx = ITEMMGR->GetTableIdxForAbsPos(tDlg->m_MsgDivide.FromPos);
	CItem* pTargetItem = ITEMMGR->GetItemofTable(TargetTableIdx, tDlg->m_MsgDivide.FromPos);
	if(!pTargetItem)
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	if(pTargetItem->IsLocked())
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	if(tDlg->m_MsgDivide.wItemIdx != pTargetItem->GetItemIdx())
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	if(tDlg->m_MsgDivide.FromDur != pTargetItem->GetDurability())
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	CItem* pToItem = ITEMMGR->GetItemofTable(ITEMMGR->GetTableIdxForAbsPos(tDlg->m_MsgDivide.ToPos), tDlg->m_MsgDivide.ToPos);
	if(pToItem)
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	
	if( tDlg->m_MsgDivide.FromDur > param1 )
	{
		tDlg->m_MsgDivide.ToDur			= param1;
		tDlg->m_MsgDivide.FromDur		= tDlg->m_MsgDivide.FromDur - param1;
		NETWORK->Send( &tDlg->m_MsgDivide, sizeof(tDlg->m_MsgDivide) );
	}
	else
	{
		tDlg->FakeGeneralItemMove(tDlg->m_MsgDivide.ToPos, (CItem*)vData2, NULL);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
	}
}

BOOL CPetInventoryDlg::FakeMoveIcon( LONG x, LONG y, cIcon * icon )
{
	ASSERT(icon);
	if( m_bDisable )	return FALSE;

	if( PKMGR->IsPKLooted() )	return FALSE;

	if( icon->GetType() == WT_STALLITEM || icon->GetType() == WT_EXCHANGEITEM || icon->GetType() == WT_DEALITEM ) return FALSE;

	if( icon->GetType() == WT_ITEM )
	{
		return FakeMoveItem(x, y, (CItem*)icon);
	}
	
	return FALSE;
}

BOOL CPetInventoryDlg::FakeMoveItem( LONG mouseX, LONG mouseY, CItem * pFromItem )
{
	WORD ToPos = 0;

	if( !GetPositionForXYRef( mouseX, mouseY, ToPos ))
		return FALSE;

	// 유효탭이 아니면
	if(ITEMMGR->GetTableIdxForAbsPos(ToPos) == eItemTable_PetInven)
	if( (DWORD)GetPetIvenTabIndex(ToPos) + 1 > m_dwValidTabNum )
		return FALSE;
	
	// 소환중인 펫의 아이템은 이동 불가
	const ITEMBASE* pInfo = pFromItem->GetItemBaseInfo();
	if( PETMGR->IsCurPetSummonItem(pInfo->dwDBIdx) )
		return FALSE;

	CItem* pToItem = GetItemForPos( ToPos );

	if(ITEMMGR->GetTableIdxForAbsPos(ToPos) == eItemTable_PetWeared)
	{//펫 장착창 소리
		AUDIOMGR->Play(59, HERO);
	}
	else
	{
		AUDIOMGR->Play(58, HERO);
	}

	//교환
	if( pToItem )
		if( pToItem->IsLocked() ) return FALSE;

	if( pFromItem->IsLocked() )	return FALSE;

	POSTYPE FromItemPos = pFromItem->GetPosition();

	//이부분 매우 이상하다. 결국 다 FakeGeneralItemMove(.. 로 가는데?
	if( TP_PYOGUK_START <= FromItemPos && FromItemPos < TP_PYOGUK_END )
	{
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	}
	else if( TP_INVENTORY_START <= FromItemPos && FromItemPos < TP_INVENTORY_END )
	{
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	}

	if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
	{
		if( IsDivideFunctionKey() && !( pToItem ) )
		{
			return FakeItemDivide( ToPos, pFromItem, pToItem );
		}
		else if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )
		{
			return FakeItemCombine( ToPos, pFromItem, pToItem );
		}
	}

	return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
}

BOOL CPetInventoryDlg::IsDivideFunctionKey()
{
	return KEYBOARD->GetKeyPressed(KEY_MENU);
}
/*
BOOL CPetInventoryDlg::FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( !CanBeMoved( pFromItem, ToPos ) )
		return FALSE;
	if( pFromItem->GetPosition() == ToPos )
		return FALSE;
	if( pFromItem->IsLocked() )
		return FALSE;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max)	return FALSE;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max)		return FALSE;

	if( 1 == pFromItem->GetDurability() )
	{
		return FakeGeneralItemMove(ToPos, pFromItem, pToItem);
	}

	m_MsgDivide.Category			= MP_ITEM;
	m_MsgDivide.Protocol			= MP_ITEM_DIVIDE_SYN;
	m_MsgDivide.dwObjectID			= HEROID;

	m_MsgDivide.wItemIdx	= pFromItem->GetItemIdx();
	m_MsgDivide.FromPos		= pFromItem->GetPosition();
	m_MsgDivide.FromDur		= pFromItem->GetDurability();
	m_MsgDivide.ToPos		= ToPos;
	m_MsgDivide.ToDur		= 0;

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_ShopInven);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_DIVIDE_INV, (LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), OnFakeItemDivideOk, OnFakeItemDivideCancel, this, pFromItem, CHATMGR->GetChatMsg(185) );
	pDivideBox->SetMaxValue( 5 );
	pDivideBox->SetValue( 1 );

	return FALSE;
}*/

BOOL CPetInventoryDlg::GetPositionForXYRef( LONG x, LONG y, WORD& pos )
{
	WORD position = 0;

	cIconGridDialog* pGridDlg = (cIconGridDialog*)GetTabSheet(GetCurTabNum());
	if(pGridDlg->GetPositionForXYRef(x, y, position))
	{
		pos = position + TP_PETINVEN_START + TABCELL_PETINVEN_NUM*GetCurTabNum();
		return TRUE;
	}

	if(m_pPetWearedDlg->GetPositionForXYRef(x, y, position))
	{
		pos = position + TP_PETWEAR_START;
		return TRUE;
	}

	return FALSE;
}

BOOL CPetInventoryDlg::CanBeMoved(CItem* pItem,POSTYPE pos)
{
	ASSERT(pItem);

	POSTYPE fromPos = 0;
	ITEM_INFO* pInfo = NULL;
	if(pItem)
	{
		fromPos = pItem->GetPosition();
		pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
	}
	else
		return FALSE;

	if( !(TP_INVENTORY_START <= fromPos && fromPos < TP_INVENTORY_END) &&
		!(TP_SHOPINVEN_START <= fromPos && fromPos < TP_SHOPINVEN_END) &&
		!(TP_PETINVEN_START <= fromPos && fromPos < TP_PETINVEN_END) &&
		!(TP_PETWEAR_START <= fromPos && fromPos < TP_PETWEAR_END) )
		return FALSE;

	if(TP_PETINVEN_START <= pos && pos < TP_PETINVEN_END)
	{//인벤외 불가
		if( TP_INVENTORY_START <= fromPos && fromPos < TP_INVENTORY_END )
			return TRUE;
		else if( TP_PETINVEN_START <= fromPos && fromPos < TP_PETINVEN_END )
			return TRUE;
		else
			return FALSE;
	}
	else if(TP_PETWEAR_START <= pos && pos < TP_PETWEAR_END)
	{//펫 장착창에는 펫 장착 아이템만
		if(TP_PETWEAR_START <= fromPos && fromPos < TP_PETWEAR_END)
			return FALSE;

		if( TRUE == m_pPetWearedDlg->CheckDuplication( pItem->GetItemIdx() ) )
			return FALSE;
		
		if(pItem->GetItemKind() == eSHOP_ITEM_PET_EQUIP || 
			pItem->GetItemKind() == eQUEST_ITEM_PET_EQUIP)
			return TRUE;
		else
			return FALSE;
	}
	else if(TP_SHOPINVEN_START <= pos && pos < TP_SHOPINVEN_END)
	{
		if( TP_PETWEAR_START <= fromPos && fromPos < TP_PETWEAR_END )
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}

BOOL CPetInventoryDlg::FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem)
{
	//제자리면
	//if( pFromItem->GetPosition() == pToItem->GetPosition() )
	//	return FALSE;
	if( pFromItem->GetItemParam() & ITEM_PARAM_SEAL )
		return FALSE;

	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return FALSE;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max)	return FALSE;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max)		return FALSE;

	if( pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
	{
		return FALSE;
	}

	if(pToItem && (FromIdx == eItemTable_MunpaWarehouse))
	{
		if(FromIdx == eItemTable_GuildWarehouse)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(53) );
		}
		return FALSE;
	}

	MSG_ITEM_MOVE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_MOVE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.wFromItemIdx		= pFromItem->GetItemIdx();
	msg.ToPos				= ToPos;
	msg.wToItemIdx			= (pToItem?pToItem->GetItemIdx():0);

	if( msg.FromPos == msg.ToPos )
		return FALSE;

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	NETWORK->Send( &msg, sizeof(msg) );
	return TRUE;
}
