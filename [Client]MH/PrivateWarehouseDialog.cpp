// PrivateWarehouseDialog.cpp: implementation of the CPrivateWarehouseDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrivateWarehouseDialog.h"
#include "ItemManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "GameIn.h"
#include "cDivideBox.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "./Input/UserInput.h"
#include "PyogukDialog.h"
#include "InventoryExDialog.h"
#include "ChatManager.h"
#include "TitanManager.h"

#include "PetManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrivateWarehouseDialog::CPrivateWarehouseDialog()
{
	m_type		= WT_ICONGRIDDIALOG;
	m_nIconType = WT_ITEM;	//이 그리드다이얼로그에서 다룰 아이템 종류
}

CPrivateWarehouseDialog::~CPrivateWarehouseDialog()
{
	
}


void CPrivateWarehouseDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_ICONGRIDDIALOG;

}

BOOL CPrivateWarehouseDialog::AddItem(ITEMBASE* pItemInfo)
{
	if( pItemInfo->dwDBIdx == 0 )
	{
		DEBUGMSG( 0, "Item DB idx == 0" );
		return FALSE;
	}

	CItem * newItem = ITEMMGR->MakeNewItem( pItemInfo,"CPrivateWarehouseDialog::AddItem" );
	if(newItem == NULL)
		return FALSE;
	
	return AddItem( newItem );
}


BOOL CPrivateWarehouseDialog::AddItem(CItem* pItem)
{
	ASSERT(pItem);
	//색 변환
	ITEMMGR->RefreshItem( pItem );

	POSTYPE relPos = GetRelativePosition(pItem->GetPosition());
	return AddIcon(relPos, pItem);
}

BOOL CPrivateWarehouseDialog::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	BYTE num = GAMEIN->GetPyogukDialog()->GetSelectedPyoguk();
	WORD A = TP_PYOGUK_START;
	WORD B = TABCELL_PYOGUK_NUM*(num);
	if(!IsAddable(Pos - A-B))
		return DeleteIcon(Pos-TP_PYOGUK_START-TABCELL_PYOGUK_NUM*(num), (cIcon **)ppItem);
	else
		return FALSE;
}

BOOL CPrivateWarehouseDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{	
	ASSERT(icon);
	
	if(icon->GetType() == WT_ITEM)
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ((CItem*)icon)->GetItemIdx() );
		if( !pItemInfo || pItemInfo->ItemKind & eSHOP_ITEM )
			return FALSE;

		FakeMoveItem(x, y, (CItem *)icon);
	}
	
	return FALSE;
}

void CPrivateWarehouseDialog::FakeMoveItem( LONG mouseX, LONG mouseY, CItem * pFromItem )
{
	WORD ToPos=0;

	const ITEMBASE* pInfo = pFromItem->GetItemBaseInfo();

	//!!! 임시 타이탄 장비 이동 막기	-> magi82 - Titan(071015) 타이탄 장착템이 창고이동 되도록 처리가 안되었기 때문에 임시로 넣었지만.. 처리완료해서 주석처리함
	//if( ITEMMGR->IsTitanEquipItem(pInfo->wIconIdx) )
	//{
	//	CHATMGR->AddMsg(CTC_SYSMSG, "이동 할 수 없는 아이템 입니다.");
	//	return;
	//}

	// magi82(20) - Titan(071105) 계약된 타이탄 증서 아이템은 창고에 들어가지 않는다.
	if(TITANMGR->IsRegistedTitan(pFromItem->GetDBIdx()))
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1578));
		return;
	}

	if( PETMGR->IsCurPetSummonItem(pInfo->dwDBIdx) )
		return;

	if( !GetPositionForXYRef(mouseX, mouseY, ToPos ) )	
		return;

	BYTE num = GAMEIN->GetPyogukDialog()->GetSelectedPyoguk();
	ToPos = ToPos+TP_PYOGUK_START + TABCELL_PYOGUK_NUM*(num); // 절대위치 넘어 옴

	CItem * pToItem = GetItemForPos( ToPos );
	
/*
	if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
	{
		if( KEYBOARD->GetKeyPressed(KEY_MENU) && !( pToItem ) )
		{
			FakeItemDivide( ToPos, pFromItem, pToItem );
			return;
		}
		else if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )
		{
			FakeItemCombine( ToPos, pFromItem, pToItem );
			return;
		}
	}
*/
	if(pToItem)
	if(pToItem->IsLocked() == TRUE)
		return;
	FakeGeneralItemMove( ToPos, pFromItem, pToItem );
}

POSTYPE CPrivateWarehouseDialog::GetRelativePosition( POSTYPE absPos)
{
	return ( absPos - TP_PYOGUK_START ) %	TABCELL_PYOGUK_NUM;
}
 
CItem * CPrivateWarehouseDialog::GetItemForPos(POSTYPE absPos)
{
	if(TP_PYOGUK_START <= absPos && absPos < TP_PYOGUK_END)
	{
		int num  = GAMEIN->GetPyogukDialog()->GetPyogukNum(absPos);
		return (CItem *)GetIconForIdx(absPos-TP_PYOGUK_START-TABCELL_PYOGUK_NUM*num);
	}

	return NULL;
}

void CPrivateWarehouseDialog::FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem)
{
	if(CanBeMoved(pFromItem, ToPos) == FALSE)
		return;
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if (ToIdx >= eItemTable_Max) return;

	if(pFromItem->GetPosition() == ToPos)
		return;
	if(pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
	{
		return;
	}
	

	MSG_ITEM_MOVE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_MOVE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.wFromItemIdx		= pFromItem->GetItemIdx();
	msg.ToPos				= ToPos;
	msg.wToItemIdx			= (pToItem?pToItem->GetItemIdx():0);

	NETWORK->Send( &msg, sizeof(msg) );
}

void CPrivateWarehouseDialog::FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( !CanBeMoved( pFromItem, ToPos ) )
		return;
	if( pFromItem->GetPosition() == ToPos )
		return;

	GAMEIN->GetPyogukDialog()->SetDividMsg( pFromItem, ToPos );
	// Locking
//	pFromItem->SetMovable( FALSE );
	
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return;

	GAMEIN->GetInventoryDialog()->SetDisable(TRUE);

	CPyogukDialog* pPyogukDlg = GAMEIN->GetPyogukDialog();
	pPyogukDlg->SetDisable(TRUE);

	cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_DIVIDE_INV, (LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), OnFakePyogukItemDivideOk, OnFakePyogukItemDivideCancel, pPyogukDlg, pFromItem, CHATMGR->GetChatMsg(185) );
	pDivideBox->SetValue( 1 );
	pDivideBox->SetMaxValue( MAX_YOUNGYAKITEM_DUPNUM );
}

void CPrivateWarehouseDialog::OnFakePyogukItemDivideCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	cDialog * ToDlg = ( cDialog * )vData1;
	ToDlg->SetDisable(FALSE);
	GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
}

void CPrivateWarehouseDialog::OnFakePyogukItemDivideOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CPyogukDialog * ptDlg = ( CPyogukDialog * )vData1;
	cDialog * pfDlg = (cDialog *)vData2;
	if( param1 == 0 ) 
	{
		ptDlg->SetDisable(FALSE);
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		return;
	}

	if( ptDlg->m_divideMsg.FromDur > param1 )
	{
		ptDlg->m_divideMsg.ToDur			= param1;
		ptDlg->m_divideMsg.FromDur		= ptDlg->m_divideMsg.FromDur - param1;
		NETWORK->Send( &ptDlg->m_divideMsg, sizeof(ptDlg->m_divideMsg) );
	}
	else
	{
		//아이템 이동
		ptDlg->FakeGeneralItemMove(ptDlg->m_divideMsg.ToPos, (CItem *)vData2, NULL);
		ptDlg->SetDisable(FALSE);
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
	}
}

void CPrivateWarehouseDialog::FakeItemCombine( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( pFromItem->GetPosition() == pToItem->GetPosition() )
		return;
	if( pFromItem->GetItemIdx() != pToItem->GetItemIdx() )
		return;

	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return;

	if( pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
		return;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return;

	MSG_ITEM_COMBINE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_COMBINE_SYN;
	msg.dwObjectID			= HEROID;
	msg.FromPos				= pFromItem->GetPosition();
	msg.ToPos				= pToItem->GetPosition();
	msg.wItemIdx			= pFromItem->GetItemIdx();
	msg.FromDur				= pFromItem->GetDurability();
	msg.ToDur				= pToItem->GetDurability();

	NETWORK->Send( &msg, sizeof(msg) );
}

BOOL CPrivateWarehouseDialog::CanBeMoved(CItem* pItem,POSTYPE pos)
{
	ASSERT(pItem);
	if(pItem)
	{
		POSTYPE fromPos = pItem->GetPosition();
		POSTYPE toPos = pos;
		if( (TP_WEAR_START <= fromPos && fromPos < TP_WEAR_END) ||
			(TP_WEAR_START <= toPos && toPos < TP_WEAR_END) )
		{
			if( OBJECTSTATEMGR->CheckAvailableState(HERO,eCAS_EQUIPITEM) == FALSE )
			{
				CAction act;
				act.InitAction(eActionKind_EquipItem,(void*)pItem->GetDBIdx(),0,NULL);
				HERO->SetNextAction(&act);
				return FALSE;
			}
		}

		if( (TP_PETWEAR_START <= fromPos && fromPos < TP_PETWEAR_END ) ||
			(TP_PETINVEN_START <= fromPos && fromPos < TP_PETINVEN_END ) )
			return FALSE;

	}
	if(TP_PYOGUK_START <= pos && pos < TP_PYOGUK_END)
	{
		// 무조건 TRUE
		return TRUE;
	}
/*	else if(TP_MUNPAWAREHOUSE_START <= pos && pos < TP_MUNPAWAREHOUSE_END)
	{
		// 무조건 TRUE
		return TRUE;
	}*/
	else if(TP_INVENTORY_START <= pos && pos < TP_INVENTORY_END)
	{
		// 무조건 TRUE
		return TRUE;
	}
	else if(TP_WEAR_START <= pos && pos < TP_WEAR_END)
	{
		// 장착하는 아이탬이 아니면 FALSE
		if( !( pItem->GetItemKind() & eEQUIP_ITEM ) )
			return FALSE;

		// 들어가야 할자리가 아니면 FALSE
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		if(!pInfo) return FALSE;
		if(pInfo->EquipKind == eWearedItem_Ring1)
		{	
			if(pos-TP_WEAR_START != eWearedItem_Ring1 && pos-TP_WEAR_START != eWearedItem_Ring2)
				return FALSE;
		}
		else
		{
			if(pos-TP_WEAR_START != pInfo->EquipKind)
				return FALSE;
		}
		
		return ITEMMGR->CanEquip(pItem->GetItemIdx());
	}
	else
		return FALSE;
}
	
