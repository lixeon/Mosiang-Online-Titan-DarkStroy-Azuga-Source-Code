// UpgradeDlg.h: interface for the CUpgradeDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPGRADEDLG_H__61D234DC_033D_4234_BB42_2D8C44A2E078__INCLUDED_)
#define AFX_UPGRADEDLG_H__61D234DC_033D_4234_BB42_2D8C44A2E078__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
#include "VirtualItem.h"

enum
{
	eBASICITEM_POS,
	eUPGRADEITEM_POS,
	eRESULTITEM_POS,
	eMAXITEM_POS,
};

enum eErrorUpgrade
{
	eErrorUpgrade_NoError,
	eErrorUpgrade_AleadyExist,	
	eErrorUpgrade_OnlyBasicItem,
	eErrorUpgrade_OnlyExtraItem,
	eErrorUpgrade_UnKnownError,
};
class cIcon;
class CItem;
class cIconDialog;
class CUpgradeDlg : public  cDialog
{
	enum eReleaseOpt
	{
		eBasicRelease	= 0x01,
		eUpgradeRelease = 0x02,
		eResultRelease	= 0x04,
		eAllRelease		= 0x07,
	};
public:
	CUpgradeDlg();
	virtual ~CUpgradeDlg();

	void Release(eReleaseOpt op = eAllRelease);
	void Linking();
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	
	BOOL IsUpgrade();
	static void UpgradeOkBtn(CUpgradeDlg * pThis);
	static void UpgradeCancelBtn(CUpgradeDlg * pThis);
	BOOL AddVirtualItem(POSTYPE relPos, CVirtualItem * pItem);
	BOOL CanUpgradeEquipItem(CItem * pBasicItem);
	eErrorUpgrade CanUpgradeItem(CItem * pBasicItem);
	BOOL CanBasicEquipItem(CItem * pBasicItem);
	CVirtualItem * GetVirtualItem(POSTYPE relPos);
	BOOL AddVirtualItemWrap(POSTYPE relPos, CItem * pItem);
	
	static BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	virtual DWORD ActionEvent(CMouse * mouseInfo);

	void DieCheck();

protected:
	cIconDialog * m_pUpgradeDlg;
	CVirtualItem m_VirtualItem[eMAXITEM_POS];
};

#endif // !defined(AFX_UPGRADEDLG_H__61D234DC_033D_4234_BB42_2D8C44A2E078__INCLUDED_)
