#include "stdafx.h"
#include ".\petwearslot.h"
#include "Player.h"


CPetWearSlot::CPetWearSlot(void)
{
}

CPetWearSlot::~CPetWearSlot(void)
{
}

ERROR_ITEM CPetWearSlot::UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag/*=UB_ALL*/, WORD state/*=SS_NONE*/, DWORD RareDBIdx/* = 0*/)
{
	ERROR_ITEM rt = CItemSlot::UpdateItemAbs(pPlayer, whatAbsPos, dwDBIdx, wItemIdx, position, quickPosition, Dur, flag, state);

	if(rt == EI_TRUE)	
	{
		//ApplyPetEquipItem(pPlayer);
	}

	return rt;
}

ERROR_ITEM CPetWearSlot::InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state/*=SS_NONE*/)
{
	ERROR_ITEM rt = CItemSlot::InsertItemAbs(pPlayer, absPos, pItem);

	if(rt == EI_TRUE)	
	{
		ApplyPetEquipItem(pPlayer, pItem->wIconIdx, TRUE);
	}

	return rt;
}

ERROR_ITEM CPetWearSlot::DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state/*=SS_NONE*/)
{
	ERROR_ITEM rt = CItemSlot::DeleteItemAbs(pPlayer, absPos, pItemOut);

	if(rt == EI_TRUE)	
	{
		ApplyPetEquipItem(pPlayer, pItemOut->wIconIdx, FALSE);
	}

	return rt;
}

BOOL CPetWearSlot::IsAddable(POSTYPE absPos, ITEMBASE * pItemBase)
{
	return TRUE;
}

void CPetWearSlot::ApplyPetEquipItem(CPlayer* pPlayer, DWORD ItemIdx, BOOL bAdd)
{
	pPlayer->GetPetManager()->SetPetEquipOption(ItemIdx, bAdd);
}