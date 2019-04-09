// ItemShopInven.h: interface for the CShopItemInven class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSHOPINVEN_H__075D6B7C_6C13_4913_8580_0420C12357E9__INCLUDED_)
#define AFX_ITEMSHOPINVEN_H__075D6B7C_6C13_4913_8580_0420C12357E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cIconGridDialog.h"


class CItem;



class CItemShopInven : public cIconGridDialog
{
public:
	CItemShopInven();
	virtual ~CItemShopInven();

	// Add
	BOOL AddItem(CItem* pItem);
	POSTYPE GetRelativePosition( POSTYPE absPos);

	// Delete
	BOOL DeleteItem(POSTYPE Pos,CItem** ppItem);
	
	// Move
	BOOL FakeMoveItem(LONG MouseX, LONG MouseY, CItem* pSrcItem);
	CItem* GetItemForPos(POSTYPE absPos);
	BOOL FakeGeneralItemMove(POSTYPE ToPos, CItem * pFromItem, CItem * pToItem);
	BOOL CanBeMoved(CItem* pItem,POSTYPE pos);
	
	// Use	
	BOOL GetBlankPositionRestrictRef(WORD& absPos);

	//SW061207 풀인벤 경고
	BOOL GetBlankNum();
};

#endif // !defined(AFX_ITEMSHOPINVEN_H__075D6B7C_6C13_4913_8580_0420C12357E9__INCLUDED_)
