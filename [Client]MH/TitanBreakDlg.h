// TitanBreakDlg.h: interface for the CTitanBreakDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TITANBREAKDLG_H
#define _TITANBREAKDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "VirtualItem.h"
#include "ItemShow.h"
#include "GameResourceStruct.h"

#define MAX_TITAN_BREAK_GRID_NUM		25

class CItem;
class cStatic;
class cIconDialog;
class cIconGridDialog;

class CTitanBreakDlg : public cDialog
{
	cIconDialog * m_pTitanBreakDlg;
	cIconGridDialog * m_pTitanBreakGridDlg;
	CVirtualItem m_VirtualItem;
	CItemShow m_ViewShowItem[MAX_TITAN_BREAK_GRID_NUM];
	cStatic* m_pTitanBreakMoney;

public:

	CTitanBreakDlg();
	virtual ~CTitanBreakDlg();
	void Release(BOOL bFlag = TRUE);
	void Linking();
	virtual void SetActive( BOOL val );
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	static BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	static void BreakOkBtn(CTitanBreakDlg * pThis);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void AddTitanItem(CItem* pItem);
	void ViewMaterialList(TITAN_BREAKINFO* pTitanBreakInfo);
	void DeleteViewMaterialListAll();
	void DieCheck();
};

#endif // _TITANBREAKDLG_H