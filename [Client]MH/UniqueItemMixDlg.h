/////////////////////////////////////////////////////////////////////////////////////////////
// UniqueItemMixDlg.h: interface for the UniqueItemMixDlg class.
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef	_UNIQUEITEMMIXDLG_H
#define _UNIQUEITEMMIXDLG_H

#if	_MSC_VER > 1000
#pragma once
#endif

#include "./interface/cDialog.h"
#include "VirtualItem.h"
#include "UniqueItemMixProgressBarDlg.h"

class CItem;

class CUniqueItemMixDlg : public cDialog
{
	cIconDialog*	m_pUniqueItemSlotDlg;
	CVirtualItem	m_VirtualItem;
	cStatic*		m_pMaterialsStatic;
	CUniqueItemMixProgressBarDlg* m_pUniqueItemMixProgressBarDlg;	//2007. 10. 22. CBH - 유니크 아이탬 조합 처리바 게이지

public:

	CUniqueItemMixDlg();
	virtual ~CUniqueItemMixDlg();
	void Release( BOOL bFlag = TRUE );
	void Linking();
	virtual void Render();
	virtual void SetActive( BOOL bFlag );
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon* icon);
	void AddUniqueItem(CItem* pItem);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void OnOkBtn();
	void UniqueItemMix();
	BOOL EnoughMaterial( DWORD dwNeedItemIdx, WORD wNeedItemDur, MATERIAL_ARRAY* sMaterial, WORD &wItemPos );
	void OnCancelBtn();
	void ViewUniqueItemMixMaterial(CItem* pItem);
};


#endif // _UNIQUEITEMMIXDLG_H