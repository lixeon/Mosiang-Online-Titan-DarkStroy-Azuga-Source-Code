// ShopInvenSlot.h: interface for the CShopInvenSlot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOPINVENSLOT_H__9EE2A709_6E2A_413E_BC42_77B5B15AC78B__INCLUDED_)
#define AFX_SHOPINVENSLOT_H__9EE2A709_6E2A_413E_BC42_77B5B15AC78B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemSlot.h"



class CShopInvenSlot : public CItemSlot
{
public:
	CShopInvenSlot();
	virtual ~CShopInvenSlot();

	WORD GetEmptyCell(WORD * EmptyCellPos, WORD NeedCount = 1);
	BOOL CheckQPosForItemIdx( WORD ItemIdx );
	BOOL CheckItemLockForItemIdx( WORD ItemIdx );
};

#endif // !defined(AFX_SHOPINVENSLOT_H__9EE2A709_6E2A_413E_BC42_77B5B15AC78B__INCLUDED_)
