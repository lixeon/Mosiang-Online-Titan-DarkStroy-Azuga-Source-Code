// ShopItemSlot.h: interface for the CShopItemSlot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOPITEMSLOT_H__54D91C0C_E16D_42B9_855A_6ADA75D6EE91__INCLUDED_)
#define AFX_SHOPITEMSLOT_H__54D91C0C_E16D_42B9_855A_6ADA75D6EE91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemSlot.h"
 

class CShopItemSlot : public CItemSlot
{
public:
	CShopItemSlot();
	virtual ~CShopItemSlot();

	void SetItemInfo(DWORD dwIndex, ITEMBASE * pItemBase);

};

#endif // !defined(AFX_SHOPITEMSLOT_H__54D91C0C_E16D_42B9_855A_6ADA75D6EE91__INCLUDED_)
