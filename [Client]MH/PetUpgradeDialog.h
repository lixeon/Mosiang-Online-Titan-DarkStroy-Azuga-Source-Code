#pragma once
//#include "d:\darkstory\main_dev\[client]mh\interface\cdialog.h"
#include "./interface/cDialog.h"
#include "VirtualItem.h"

class cIconDialog;
class CItem;
class cStatic;

class CPetUpgradeDialog : public cDialog
{
	cIconDialog*	m_pIconDlg;
	CVirtualItem	m_VirtualItem;
	cStatic*		m_pGradeText[2];

	DWORD		m_UpgradeItemPos;
	DWORD		m_UpgradeItemIdx;

	int			m_iGrade;

public:
	CPetUpgradeDialog(void);
	virtual ~CPetUpgradeDialog(void);

	void Linking();
	void SetActive( BOOL val );

	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void ReleaseItem();
	void AddVirtualItem( CItem* pItem );

	void SetUpgradeItemInfo(DWORD dwItemPos, DWORD dwItemIdx)
	{
		m_UpgradeItemPos = dwItemPos;
		m_UpgradeItemIdx = dwItemIdx;
	}
	void InitUpgradeItemInfo()	{m_UpgradeItemPos = m_UpgradeItemIdx = 0;}

	void PetUpgradeSyn();
	void PetUpgradeAck();
	void PetUpgradeNack();
};
