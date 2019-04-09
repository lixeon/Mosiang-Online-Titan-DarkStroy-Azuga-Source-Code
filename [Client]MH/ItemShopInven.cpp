// ItemShopInven.cpp: implementation of the CShopItemInven class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemShopInven.h"

#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "ItemManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cWindowManager.h"
#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemShopInven::CItemShopInven()
{
	m_type		= WT_ICONGRIDDIALOG;
	m_nIconType = WT_ITEM;	//이 그리드다이얼로그에서 다룰 아이템 종류
}

CItemShopInven::~CItemShopInven()
{

}



BOOL CItemShopInven::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	return DeleteIcon(Pos-TP_SHOPINVEN_START, (cIcon **)ppItem);
}


BOOL CItemShopInven::AddItem(CItem* pItem)
{
	if(!pItem)		return FALSE;	

	// ItemManager에 아이템 정보추가
	if( pItem->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL )
	{
		pItem->SetMovable(TRUE);
	}

	//색 변환
	ITEMMGR->RefreshItem( pItem );

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItem->GetItemBaseInfo()->wIconIdx );
	if( !pItemInfo )		return FALSE;

	if( pItemInfo->ItemKind==eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind==eSHOP_ITEM_DECORATION )
	{
		SHOPITEMBASE* pUsedInfo = ITEMMGR->GetUsedItemInfo( pItem->GetItemBaseInfo()->wIconIdx );
		if( pUsedInfo )
			ITEMMGR->AddUsedAvatarItemToolTip( pUsedInfo );
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP )
	{
		SHOPITEMBASE* pUsedInfo = ITEMMGR->GetUsedItemInfo( pItem->GetItemBaseInfo()->wIconIdx );
		if( pUsedInfo )
			ITEMMGR->AddUsedShopEquipItemToolTip( pUsedInfo );		
	}


	POSTYPE relPos = GetRelativePosition(pItem->GetPosition());
	
	return AddIcon(relPos, pItem);
}



POSTYPE CItemShopInven::GetRelativePosition( POSTYPE absPos)
{
	return ( absPos - TP_SHOPINVEN_START ) % TABCELL_SHOPINVEN_NUM;
}



BOOL CItemShopInven::FakeMoveItem(LONG MouseX, LONG MouseY, CItem* pSrcItem)
{
	WORD ToPos=0;
	
	if( !GetPositionForXYRef(MouseX, MouseY, ToPos) )
		return FALSE;	
	
	ToPos = ToPos+TP_SHOPINVEN_START;

	CItem * pToItem = GetItemForPos( ToPos );
	
	if(pToItem && pToItem->IsLocked() == TRUE)
		return FALSE;
	
	return FakeGeneralItemMove( ToPos, pSrcItem, pToItem );
}


CItem* CItemShopInven::GetItemForPos(POSTYPE absPos)
{
	if(TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END)
	{
		return (CItem*)GetIconForIdx(absPos-(TP_SHOPINVEN_START));
	}

	return NULL;
}


BOOL CItemShopInven::FakeGeneralItemMove(POSTYPE ToPos, CItem * pFromItem, CItem * pToItem)
{
	if(CanBeMoved(pFromItem, ToPos) == FALSE)
		return FALSE;
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if (ToIdx >= eItemTable_Max) return FALSE;

	if(pFromItem->GetPosition() == ToPos)
		return FALSE;

	if( pToItem && pToItem->GetUseParam() == 1 )
	{
		if( FromIdx != ToIdx )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(748) );
			return FALSE;
		}
	}
	
	if(pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
	{
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

	NETWORK->Send( &msg, sizeof(msg) );
	return TRUE;
}


BOOL CItemShopInven::CanBeMoved(CItem* pItem,POSTYPE pos)
{
	ASSERT(pItem);

	if( TP_SHOPITEM_START <= pos && pos < TP_SHOPINVEN_END )
	{
		POSTYPE fromPos = pItem->GetPosition();
		if( TP_SHOPITEM_START <= fromPos && fromPos < TP_SHOPINVEN_END )
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}


BOOL CItemShopInven::GetBlankPositionRestrictRef(WORD& absPos)
{	
	for(int j = 0 ; j < GetCellNum() ; ++j)
	{
		if(IsAddable(j))
		{
			absPos = TP_SHOPINVEN_START+j;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CItemShopInven::GetBlankNum()
{
	int nBlankNum = 0;

	for( int i = 0; i < GetCellNum(); ++i )
	{
		if( IsAddable(i) )
		{
			++nBlankNum;
		}
	}

	return nBlankNum;
}
