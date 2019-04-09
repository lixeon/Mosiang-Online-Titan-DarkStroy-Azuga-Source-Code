#include "stdafx.h"
#include ".\TitanWearSlot.h"
#include "Player.h"
#include "StatsCalcManager.h"



CTitanWearSlot::CTitanWearSlot(void)
{
}

CTitanWearSlot::~CTitanWearSlot(void)
{
}

ERROR_ITEM CTitanWearSlot::UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag/*=UB_ALL*/, WORD state/*=SS_NONE*/, DWORD RareDBIdx/* = 0*/)
{
	ERROR_ITEM rt = CItemSlot::UpdateItemAbs(pPlayer, whatAbsPos, dwDBIdx, wItemIdx, position, quickPosition, Dur, flag, state);

	if(rt == EI_TRUE)	
	{
		ITEMBASE temp;
		temp.wIconIdx = wItemIdx;
		temp.dwDBIdx = dwDBIdx;
		// magi82(28)
		temp.Durability = 0;
		temp.ItemParam = 0;
		temp.Position = 0;
		temp.QuickPosition = 0;
		temp.RareIdx = 0;

		pPlayer->SetTitanWearInfo(whatAbsPos,&temp,TRUE);
		ApplyItem(pPlayer);
	}

	return rt;
}

ERROR_ITEM CTitanWearSlot::InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state/*=SS_NONE*/)
{
	ERROR_ITEM rt = CItemSlot::InsertItemAbs(pPlayer, absPos, pItem);

	if(rt == EI_TRUE)	
	{
		pPlayer->SetTitanWearInfo(absPos,pItem,TRUE);
		ApplyItem(pPlayer);
	}

	return rt;
}

ERROR_ITEM CTitanWearSlot::DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state/*=SS_NONE*/)
{
	ERROR_ITEM rt = CItemSlot::DeleteItemAbs(pPlayer, absPos, pItemOut);

	if(rt == EI_TRUE)	
	{
		pPlayer->SetTitanWearInfo(absPos,pItemOut,FALSE);
		ApplyItem(pPlayer);
	}

	return rt;
}

BOOL CTitanWearSlot::IsAddable(POSTYPE absPos, ITEMBASE * pItemBase)
{
	return TRUE;
}

void CTitanWearSlot::ApplyItem( CPlayer* pPlayer )
{
	pPlayer->ApplyTitanWearItem();
	//STATSMGR->CalcTitanItemStats(pPlayer);

	// 탑승 상태로 장착하는 것이 아니므로 주변에 정보를 보내줄 필요가 없다.?
	// 있다. 클라이언트에 정보 세팅해놓고 소환시 그 정보대로 보여준다.
}
