// TitanUpgradeDlg.h: interface for the CTitanUpgradeDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TITANUPGRADEDLG_H
#define _TITANUPGRADEDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "VirtualItem.h"
#include "ItemShow.h"
#include "GameResourceStruct.h"

#define MAX_TITAN_UPGRADE_GRID_NUM		25

class CItem;
class cStatic;
class cIconDialog;
class cIconGridDialog;

class CTitanUpgradeDlg : public cDialog
{
	cIconDialog * m_pTitanUpgradeDlg;
	cIconGridDialog * m_pTitanUpgradeGridDlg;
	CVirtualItem m_VirtualItem;
	CItemShow m_ViewShowItem[MAX_TITAN_UPGRADE_GRID_NUM];

	cStatic * m_pTitanUpgradeStatic;
	cStatic * m_pTitanUpgradeMoney;

public:

	CTitanUpgradeDlg();
	virtual ~CTitanUpgradeDlg();
	void Release(BOOL bFlag = TRUE);
	void Linking();
	virtual void SetActive( BOOL val );
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	static BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	static void UpgradeOkBtn(CTitanUpgradeDlg * pThis);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void AddTitanPaper(CItem* pItem);
	void ViewMaterialList(TITAN_UPGRADEINFO * pTitanUpgradeInfo);
	void DeleteViewMaterialListAll();
	void DieCheck();
};

#endif // _TITANUPGRADEDLG_H