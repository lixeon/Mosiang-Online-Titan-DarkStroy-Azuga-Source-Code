// TitanInventoryDlg.h: interface for the CTitanInventoryDlg class.
//
//////////////////////////////////////////////////////////////////////


#ifndef _TITANINVENTORYDLG_H
#define _TITANINVENTORYDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "WearedExDialog.h"
#include "ItemShow.h"

class CItem;
class cMsgBox;

class CTitanInventoryDlg : public cDialog
{
	CWearedExDialog* m_pTitanInvenWearedDlg;	
	cIconDialog	* m_pTitanShopItemDlg;
	cImage	m_Image;

	TITAN_STATS_CTRL m_TitanInvenCtrl;

	DWORD m_dwWearInfoArray[eTitanWearedItem_Max];

public:
	CTitanInventoryDlg();
	virtual ~CTitanInventoryDlg();
	void Linking();
	virtual void SetActive( BOOL val );
	void UpdateData();
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	BOOL GetPositionForXYRef(LONG x, LONG y, WORD& pos, eITEM_KINDBIT bits);
	BOOL CanBeMoved(CItem* pItem,POSTYPE pos);
	BOOL DeleteItem(POSTYPE Pos,CItem** ppItem);
	BOOL AddItem(ITEMBASE* pItemBase);
	BOOL AddItem(CItem* pItem);
	virtual void Render();
	void SetTitanInvenInfo();
	void SetTitanEquipInfo();	
	CItem* GetItemForPos(POSTYPE absPos);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	BOOL GetBlankSlotPosition( POSTYPE& pos );	// magi82(26)
};

#endif // _TITANINVENTORYDLG_H
