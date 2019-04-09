// WearSlot.cpp: implementation of the CWearSlot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WearSlot.h"
#include "StatsCalcManager.h"
#include "PackedData.h"
#include "UserTable.h"

#include "MapDBMsgParser.h"
#include "Player.h"

#include "ObjectStateManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWearSlot::CWearSlot()
{

}

CWearSlot::~CWearSlot()
{

}

BOOL CWearSlot::IsAddable(POSTYPE absPos, ITEMBASE * pItemBase)
{
	return TRUE;
}

void CWearSlot::ApplyItem(CPlayer* pPlayer)
{
	// ÃÖÀûÈ­ ÇÊ¿ä!
	// Charactertable¿¡ ÀÖ´Â Á¤º¸ Áö¿ò
	CharacterTotalInfoUpdate(pPlayer);

	// ¾ÆÀÌÅÛ ½ºÅÝ º¯°æ
	STATSMGR->CalcItemStats(pPlayer);

	// ÃÖÀûÈ­ ÇÊ¿ä!
	APPEARANCE_INFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_APPEARANCE_CHANGE;
	msg.PlayerID = pPlayer->GetID();
	for(int n=0;n<eWearedItem_Max;++n)
	{
		msg.WearedItem[n] = pPlayer->GetWearedItemIdx(n);
	}
	
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &msg, sizeof(msg));
}
ERROR_ITEM CWearSlot::InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state)
{
	ERROR_ITEM rt = CItemSlot::InsertItemAbs(pPlayer, absPos, pItem);
	if(rt == EI_TRUE)	
	{
		if( pPlayer->GetState() == eObjectState_Immortal )
			OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );
		pPlayer->SetWearedItemIdx(absPos - m_StartAbsPos, pItem->wIconIdx);
		ApplyItem(pPlayer);
	}
	return rt;
}
ERROR_ITEM CWearSlot::UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag, WORD state, DWORD RareDBIdx)
{
	ERROR_ITEM rt = CItemSlot::UpdateItemAbs(pPlayer, whatAbsPos, dwDBIdx, wItemIdx, position, quickPosition, Dur, flag, state);
	if(rt == EI_TRUE)	
	{
		if( pPlayer->GetState() == eObjectState_Immortal )
			OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );
		if(UB_ICONIDX & flag)
		{
			pPlayer->SetWearedItemIdx(whatAbsPos - m_StartAbsPos, wItemIdx);
			ApplyItem(pPlayer);
		}
	}
	return rt;
}
ERROR_ITEM CWearSlot::DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state)
{
	ERROR_ITEM rt = CItemSlot::DeleteItemAbs(pPlayer, absPos, pItemOut);
	if(rt == EI_TRUE)	
	{
		if( pPlayer->GetState() == eObjectState_Immortal )
			OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );
		pPlayer->SetWearedItemIdx(absPos - m_StartAbsPos, 0);
		ApplyItem(pPlayer);
	}
	return rt;
}
