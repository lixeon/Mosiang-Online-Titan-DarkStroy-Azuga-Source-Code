/////////////////////////////////////////////////////////////////////////////////////////////
// UniqueItemCurseCancellationDlg.h: interface for the CUniqueItemCurseCancellationDlg class.
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef	_UNIQUEITEMCURSECANCELLATIONDLG_H
#define _UNIQUEITEMCURSECANCELLATIONDLG_H

#if	_MSC_VER > 1000
#pragma once
#endif

#include "./interface/cDialog.h"
#include "VirtualItem.h"

class CItem;

class CUniqueItemCurseCancellationDlg : public cDialog
{
	cIconDialog*	m_pUniqueItemSlotDlg;
	CVirtualItem	m_VirtualItem;
	POSTYPE			m_wShopItemPos;

public:

	CUniqueItemCurseCancellationDlg();
	virtual ~CUniqueItemCurseCancellationDlg();
	void Release( BOOL bFlag = TRUE );
	void Linking();
	virtual void SetActive( BOOL bFlag );
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon* icon);
	void AddCarseIncantation(CItem* pItem);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void OnOkBtn();
	void OnCancelBtn();
	void SetShopItemPosion(POSTYPE wShopItemPos)	{	m_wShopItemPos = wShopItemPos;	};
};


#endif // _UNIQUEITEMCURSECANCELLATIONDLG_H