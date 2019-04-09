// TitanPartsMakeDlg.h: interface for the CTitanPartsMakeDlg class.
//
//////////////////////////////////////////////////////////////////////


#ifndef _TITANPARTSMAKEDLG_H
#define _TITANPARTSMAKEDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "VirtualItem.h"
#include "ItemShow.h"
#include "TitanPartsProgressBarDlg.h"

enum eTitanPartsMakePos
{
	eTPMPos_Basic,
	eTPMPos_Result,
	eTPMPos_Max,
};
enum eTitanPartsMakeReleaseOpt
{
	eTPMBasicRelease	= 0x01,
	eTPMResultRelease	= 0x02,
	eTPMAllRelease		= 0x03,
};
#define MAX_TPMGRID_NUM		20

class cStatic;
class CItem;
class cIconDialog;
class cIconGridDialog;

class ITEM_MIX_INFO;
class ITEM_MIX_RES;

class CTitanPartsMakeDlg : public cDialog
{
	cIconDialog * m_pTitanPartsMakeDlg;
	cIconGridDialog * m_pTitanPartsMakeGridDlg;
	CTitanPartsProgressBarDlg* m_pTitanPartsProgressBarDlg;	//2007. 10. 23. CBH - ≈∏¿Ã≈∫ ∆ƒ√˜ ¡∂«’ ∞‘¿Ã¡ˆ
	cStatic * m_pMaterialsStatic;
	CVirtualItem m_VirtualItem[eTPMPos_Max];
	CItemShow m_ViewShowItem[MAX_TPMGRID_NUM];
	cStatic*	m_pTitle;
	char m_Title[128];
	BOOL m_bTPMaking;

	BOOL		m_bShopItemUse;
	DWORD		m_ShopItemIdx;
	DWORD		m_ShopItemPos;


public:
	CTitanPartsMakeDlg();
	virtual ~CTitanPartsMakeDlg();
	void Release(eTitanPartsMakeReleaseOpt op=eTPMAllRelease, BOOL bMsg = TRUE);
	void Linking();
	virtual void Render();
	virtual void SetActive( BOOL val );
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	void MixOkBtn();
	void TitanPartsMake();
	BOOL EnoughMaterial(WORD wItemIdx, WORD ItemNum, MATERIAL_ARRAY * ItemOut, WORD & ItemPosNum);
	void MixCancelBtn();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	BOOL IsEmpty(eTitanPartsMakePos pos);
	void ResetCurSelCellPos();
	void DeleteViewResultItemAll();
	BOOL AddVirtualItemWrap(POSTYPE relPos, CItem * pItem);
	BOOL AddVirtualItem(POSTYPE relPos, CVirtualItem * pItem);
	CVirtualItem * GetVirtualItem(POSTYPE relPos);
	void ViewMaterialsDesc(ITEM_MIX_RES * pResultItem);
	void ViewResultItemList(ITEM_MIX_INFO * pTPMItemInfo);
	BOOL AddRealItem( CItem *pOrigItem );
	void DieCheck();
	void SetShopItemInfo( DWORD ShopItemIdx, DWORD ShopItemPos );

	void SetNowMixing( BOOL bMixing ) { m_bTPMaking = bMixing; }
	BOOL IsNowMixing() { return m_bTPMaking; }
	BOOL IsShopItemUse()			{	return m_bShopItemUse;	}

};

#endif // _TITANPARTSMAKEDLG_H
