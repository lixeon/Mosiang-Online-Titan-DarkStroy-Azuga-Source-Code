// ItemShopGridDialog.h: interface for the CItemShopGridDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSHOPGRIDDIALOG_H__44925A58_B5B3_4F97_BC61_D3E19F91C3E9__INCLUDED_)
#define AFX_ITEMSHOPGRIDDIALOG_H__44925A58_B5B3_4F97_BC61_D3E19F91C3E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cIconGridDialog.h"
#include "IndexGenerator.h"


class CItem;




class CItemShopGridDialog : public cIconGridDialog
{
	CIndexGenerator		m_IndexGenerator;
	DWORD				m_TabNumber;
	
public:
	CItemShopGridDialog();
	virtual ~CItemShopGridDialog();

	void Init();

	// Add
	BOOL AddItem(ITEMBASE* pItemBase);
	POSTYPE GetRelativePosition( POSTYPE absPos);
	
	// Delete
	BOOL DeleteItem(POSTYPE Pos,CItem** ppItem);
	void ShopItemDelete(DWORD dwDBIdx, POSTYPE absPos, DWORD dwCount);

	// Move
	BOOL FakeMoveItem(LONG MouseX, LONG MouseY, CItem* pSrcItem);
	CItem* GetItemForPos(POSTYPE absPos);
	BOOL FakeGeneralItemMove(POSTYPE ToPos, CItem * pFromItem, CItem * pToItem);
	BOOL CanBeMoved(CItem* pItem,POSTYPE pos);

	// 
	void SetTabNumber(DWORD Number)		{	m_TabNumber = Number;	}
};

#endif // !defined(AFX_ITEMSHOPGRIDDIALOG_H__44925A58_B5B3_4F97_BC61_D3E19F91C3E9__INCLUDED_)
