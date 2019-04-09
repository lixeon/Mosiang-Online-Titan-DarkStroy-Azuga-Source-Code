// MixDialog.h: interface for the CMixDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIXDIALOG_H__C0B6025C_18E9_41C8_B5FB_84EE4E7D97F5__INCLUDED_)
#define AFX_MIXDIALOG_H__C0B6025C_18E9_41C8_B5FB_84EE4E7D97F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
#include "VirtualItem.h"
#include "ItemShow.h"

class ITEM_MIX_INFO;
class ITEM_MIX_RES;

enum eMixPos
{
	eMixPos_Basic,
	eMixPos_Result,
	eMixPos_Max,
};
enum eMixReleaseOpt
{
	eMixBasicRelease	= 0x01,
	eMixResultRelease	= 0x02,
	eMixAllRelease		= 0x03,
};
#define MAX_MIXGRID_NUM		20
class cStatic;
class CItem;
class cIconDialog;
class cIconGridDialog;
class CMixDialog    : public cDialog
{	
public:
	CMixDialog();
	virtual ~CMixDialog();
	void Release(eMixReleaseOpt op=eMixAllRelease, BOOL bMsg = TRUE);
	void Linking();
	BOOL IsEmpty(eMixPos pos);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	void ViewResultItemList(ITEM_MIX_INFO * pMixItemInfo);
	static void MixOkBtn(CMixDialog * pThis);
	static void MixCancelBtn(CMixDialog * pThis);
	void DeleteViewResultItemAll();
	CVirtualItem * GetVirtualItem(POSTYPE relPos);
	BOOL AddVirtualItem(POSTYPE relPos, CVirtualItem * pItem);
	BOOL AddVirtualItemWrap(POSTYPE relPos, CItem * pItem);
	BOOL EnoughMaterial(WORD wItemIdx, WORD ItemNum, MATERIAL_ARRAY * ItemOut, WORD & ItemPosNum);
	static BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	void ViewMaterialsDesc(ITEM_MIX_RES * pResultItem);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void ResetCurSelCellPos();
	BOOL AddRealItem( CItem *pOrigItem );
	void DieCheck();		//강화, 업그레이드에도..

	void SetNowMixing( BOOL bMixing ) { m_bMixing = bMixing; }
	BOOL IsNowMixing() { return m_bMixing; }

	//
	void SetShopItemInfo( DWORD ShopItemIdx, DWORD ShopItemPos );
	virtual void SetActiveRecursive(BOOL val);
	BOOL IsShopItemUse()			{	return m_bShopItemUse;	}


protected:
	cIconDialog * m_pMixDlg;
	cIconGridDialog * m_pMixGridDlg;
	cStatic * m_pMaterialsStatic;
	CVirtualItem m_VirtualItem[eMixPos_Max];
	CItemShow m_ViewShowItem[MAX_MIXGRID_NUM];

	BOOL		m_bMixing;
	BOOL		m_bShopItemUse;
	DWORD		m_ShopItemIdx;
	DWORD		m_ShopItemPos;
	cStatic*	m_pTitle;
	char		m_Title[128];
};

#endif // !defined(AFX_MIXDIALOG_H__C0B6025C_18E9_41C8_B5FB_84EE4E7D97F5__INCLUDED_)
