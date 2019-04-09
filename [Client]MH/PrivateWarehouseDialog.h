// PrivateWarehouseDialog.h: interface for the CPrivateWarehouseDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRIVATEWAREHOUSEDIALOG_H__D1BBC08D_9A8C_4E16_9321_00C0EE24AA17__INCLUDED_)
#define AFX_PRIVATEWAREHOUSEDIALOG_H__D1BBC08D_9A8C_4E16_9321_00C0EE24AA17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cIconGridDialog.h"
#include "Item.h"

class CPrivateWarehouseDialog  : public cIconGridDialog
{
public:
	CPrivateWarehouseDialog();
	virtual ~CPrivateWarehouseDialog();

	
	BOOL AddItem(CItem* pItem);
	BOOL AddItem(ITEMBASE* pItemInfo);
	BOOL DeleteItem(POSTYPE Pos,CItem** ppItem);

	
	void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void FakeMoveItem( LONG mouseX, LONG mouseY, CItem * pFromItem );
	void FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem);

	
	CItem * GetItemForPos(POSTYPE absPos);
	POSTYPE GetRelativePosition( POSTYPE absPos);
	int GetTabIndex( POSTYPE absPos );


private:
	void FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	void FakeItemCombine( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	BOOL CanBeMoved(CItem* pItem,POSTYPE pos);
	static void OnFakePyogukItemDivideCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnFakePyogukItemDivideOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
};



#endif // !defined(AFX_PRIVATEWAREHOUSEDIALOG_H__D1BBC08D_9A8C_4E16_9321_00C0EE24AA17__INCLUDED_)
