// TitanPartsChangeDlg.h: interface for the CTitanPartsChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TITANPARTSCHANGEDLG_H
#define _TITANPARTSCHANGEDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "GameResourceStruct.h"

class CWearedExDialog;
class CItem;

class CTitanPartsChangeDlg : public cDialog
{
	CWearedExDialog* m_pTitanPartsWearedDlg;
	TITAN_STATS_CTRL m_TitanPartsChangeCtrl;
	DWORD m_dwWearInfoArray[eTitanWearedItem_Max];

public:

	CTitanPartsChangeDlg();
	virtual ~CTitanPartsChangeDlg();
	void Linking();
	virtual void SetActive( BOOL val );
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void UpdateData( BOOL bFlag );
	BOOL GetPositionForXYRef(LONG x, LONG y, WORD& pos);
	BOOL DeleteItem(POSTYPE Pos,CItem** ppItem);
	BOOL AddItem(ITEMBASE* pItemBase);
	BOOL AddItem(CItem* pItem);
	BOOL CanBeMoved(CItem* pItem,POSTYPE pos);
	void SetTitanStatsInfo();
	//CWearedExDialog* GetTitanPartsWearedDlg()	{	return m_pTitanPartsWearedDlg;	}
	CItem* GetItemForPos(POSTYPE absPos);
	CItem* GetCurSelectItem();
};

#endif // _TITANPARTSCHANGEDLG_H