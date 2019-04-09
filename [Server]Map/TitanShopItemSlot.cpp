#include "stdafx.h"
#include ".\TitanShopItemSlot.h"
#include "Player.h"


CTitanShopItemSlot::CTitanShopItemSlot(void)
{
}

CTitanShopItemSlot::~CTitanShopItemSlot(void)
{
}

ERROR_ITEM CTitanShopItemSlot::UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag/*=UB_ALL*/, WORD state/*=SS_NONE*/, DWORD RareDBIdx/* = 0*/)
{
	ERROR_ITEM rt = CItemSlot::UpdateItemAbs(pPlayer, whatAbsPos, dwDBIdx, wItemIdx, position, quickPosition, Dur, flag, state);

	if(rt == EI_TRUE)	
	{
	}

	return rt;
}

ERROR_ITEM CTitanShopItemSlot::InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state/*=SS_NONE*/)
{
	ERROR_ITEM rt = CItemSlot::InsertItemAbs(pPlayer, absPos, pItem);

	if(rt == EI_TRUE)	
	{
		pPlayer->GetTitanManager()->SetTitanShopitemOption(pItem->wIconIdx, TRUE);
	}

	return rt;
}

ERROR_ITEM CTitanShopItemSlot::DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state/*=SS_NONE*/)
{
	ERROR_ITEM rt = CItemSlot::DeleteItemAbs(pPlayer, absPos, pItemOut);

	if(rt == EI_TRUE)	
	{
		pPlayer->GetTitanManager()->SetTitanShopitemOption(pItemOut->wIconIdx, FALSE);
	}

	return rt;
}

BOOL CTitanShopItemSlot::IsAddable(POSTYPE absPos, ITEMBASE * pItemBase)
{
	return TRUE;
}