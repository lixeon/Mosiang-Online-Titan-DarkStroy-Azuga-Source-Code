// DealItem.cpp: implementation of the CDealItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DealItem.h"
#include "ItemManager.h"
#include "cFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealItem::CDealItem()
{
	m_type = WT_DEALITEM;

	m_nItemCount = -1;
}

CDealItem::~CDealItem()
{

}
DWORD CDealItem::GetSellPrice()
{
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(m_wItemIdx);
	if(pItemInfo)
		return pItemInfo->SellPrice;
	return 0;
}

DWORD CDealItem::GetBuyPrice()
{
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(m_wItemIdx);
	if(pItemInfo)
		return pItemInfo->BuyPrice;
	return 0;
}


char* CDealItem::GetItemName()
{
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(m_wItemIdx);
	if(pItemInfo)
		return pItemInfo->ItemName;
	return NULL;
}

void CDealItem::Render()
{
	cIcon::Render();

	if( 0 < m_nItemCount )
	{
		static char nums[3];
		wsprintf(nums, "%2d", m_nItemCount);
		RECT rect = {(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1, 1};
		CFONT_OBJ->RenderFont(0, nums, strlen(nums), &rect, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));
	}
	else if( 0 == m_nItemCount )
	{
		//»ÊπÈ«•Ω√
		m_LockImage.RenderSprite( NULL, NULL, 0, &m_absPos, 
			RGBA_MERGE(0x4c951d, m_alpha * m_dwOptionAlpha / 100));
	}

}
