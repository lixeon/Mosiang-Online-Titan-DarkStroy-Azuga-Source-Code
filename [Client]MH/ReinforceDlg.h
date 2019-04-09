// ReinforceDlg.h: interface for the CReinforceDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REINFORCEDLG_H__42C1D27D_B785_4E11_9F69_0C9D848052A4__INCLUDED_)
#define AFX_REINFORCEDLG_H__42C1D27D_B785_4E11_9F69_0C9D848052A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
#include "VirtualItem.h"
#include "ItemShow.h"

#define MAX_REINFORCEGRID_NUM		20

enum eReinforceReleaseOpt
{
	eReinforceBasicRelease		= 0x01,
	eReinforceViewRelease		= 0x02,
	eReinforceAllRelease		= 0x03,
};
class CItem;
class cIconDialog;
class cIconGridDialog;
class cStatic;
class CReinforceDlg : public cDialog  
{
public:
	CReinforceDlg();
	virtual ~CReinforceDlg();

	void Release(eReinforceReleaseOpt op=eReinforceAllRelease);
	void Linking();

	BOOL IsEmpty(POSTYPE pos);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	
	BOOL AddVirtualItemWrap(POSTYPE relPos, CItem * pItem);
	BOOL AddVirtualItem(POSTYPE relPos, CVirtualItem * pItem);
	CVirtualItem * GetVirtualItem(POSTYPE relPos);

	static void ReinforceOkBtn(CReinforceDlg * pThis);
	static void ReinforceCancelBtn(CReinforceDlg * pThis);
	
	static BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void SetActiveRecursive(BOOL val);
	
	void DieCheck();

	//	
	void SetShopItemInfo( DWORD ShopItemIdx, DWORD ShopItemPos );
	void ReinforceOkBtnWithShopItem(CReinforceDlg * pThis);
	BOOL IsShopItemUse()		{	return m_bShopItemUse;	}

	void SetGravityText();

protected:
	cIconDialog * m_pReinforceDlg;
	cIconGridDialog * m_pReinforceGridDlg;
	cStatic*	m_pReinforceGravityDlg;

	CVirtualItem	m_VirtualItem[MAX_REINFORCEGRID_NUM+1];
	BOOL			m_bShopItemUse;
	DWORD			m_ShopItemIdx;
	DWORD			m_ShopItemPos;
	cStatic*		m_pTitle;
	char			m_Title[128];
};

#endif // !defined(AFX_REINFORCEDLG_H__42C1D27D_B785_4E11_9F69_0C9D848052A4__INCLUDED_)
