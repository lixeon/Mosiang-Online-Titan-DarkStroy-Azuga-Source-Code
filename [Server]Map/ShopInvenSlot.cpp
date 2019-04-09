// ShopInvenSlot.cpp: implementation of the CShopInvenSlot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShopInvenSlot.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShopInvenSlot::CShopInvenSlot()
{

}

CShopInvenSlot::~CShopInvenSlot()
{

}

WORD CShopInvenSlot::GetEmptyCell(WORD * EmptyCellPos, WORD NeedCount)
{
	if( NeedCount == 0 ) return 0;

	//POSTYPE EndPos = m_StartAbsPos + m_SlotNum;
	// RaNa - 05.02.02    -> 인벤토리가 꽉찬상태에서 거래 방지. 현재는 한칸만을 사용하니까.
	// 인벤만들때 갯수 줄인걸로 수정 
	POSTYPE EndPos = m_StartAbsPos + m_SlotNum;

	WORD EmptyCellNum = 0;

	for( WORD i = m_StartAbsPos ; i < EndPos ; ++i )
	{
		//const ITEMBASE * pItemBase = GetItemInfoAbs(i);

		if( IsEmpty(i) )
		{
			if( EmptyCellPos )
				EmptyCellPos[EmptyCellNum++] = i;

			if( EmptyCellNum == NeedCount ) break;
		}
	}

	return EmptyCellNum;
}


BOOL CShopInvenSlot::CheckQPosForItemIdx( WORD ItemIdx )
{
	POSTYPE EndPos = m_StartAbsPos + m_SlotNum;
	WORD EmptyCellNum = 0;

	for( WORD i = m_StartAbsPos ; i < EndPos ; ++i )
	{
		const ITEMBASE * pItemBase = GetItemInfoAbs(i);

		if( pItemBase->wIconIdx == ItemIdx && pItemBase->QuickPosition != 0 )
			return FALSE;
	}

	return TRUE;
}

BOOL CShopInvenSlot::CheckItemLockForItemIdx( WORD ItemIdx )
{
	POSTYPE EndPos = m_StartAbsPos + m_SlotNum;
	WORD EmptyCellNum = 0;

	for( WORD i = m_StartAbsPos ; i < EndPos ; ++i )
	{
		const ITEMBASE * pItemBase = GetItemInfoAbs(i);

		if( pItemBase->wIconIdx == ItemIdx && IsLock(i) )
			return FALSE;
	}

	return TRUE;
}
