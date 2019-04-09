// ShopItemSlot.cpp: implementation of the CShopItemSlot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShopItemSlot.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShopItemSlot::CShopItemSlot()
{

}

CShopItemSlot::~CShopItemSlot()
{

}

void CShopItemSlot::SetItemInfo(DWORD dwIndex, ITEMBASE * pItemBase)
{
	//memcpy(&m_ItemBaseArray[m_StartAbsPos], pItemBase, sizeof(ITEMBASE)*m_SlotNum);
	memcpy(&m_ItemBaseArray[m_StartAbsPos+dwIndex], pItemBase, sizeof(ITEMBASE));	
}
