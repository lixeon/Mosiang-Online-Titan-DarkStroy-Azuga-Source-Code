#pragma once
//#include "d:\darkstory\main_dev\[client]mh\interface\cdialog.h"
#include "./interface/cDialog.h"
#include "VirtualItem.h"

class cIconDialog;
class CItem;
class cStatic;

class CPetRevivalDialog : public cDialog
{
	cIconDialog*	m_pIconDlg;
	CVirtualItem	m_VirtualItem;
	cStatic*		m_pGradeText[3];

	DWORD		m_ItemIdx;
	DWORD		m_ItemPos;

	int			m_iGrade;

public:
	CPetRevivalDialog(void);
	virtual ~CPetRevivalDialog(void);

	void Linking();
	void SetActiveWithGradeTxt(int iGrade);
	void SetActive(BOOL val);

	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void ReleaseItem();
	void AddVirtualItem( CItem* pItem );

	void SetShopItemInfo(DWORD ItemIdx, DWORD ItemPos)		{	m_ItemIdx=ItemIdx;	m_ItemPos=ItemPos;		}

	void PetRevivalSyn();
	void PetRevivalAck(int flgFromShopItem);
	void PetRevivalNack();
};
