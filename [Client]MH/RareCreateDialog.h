#pragma once


#include "./interface/cDialog.h"
#include "VirtualItem.h"

class cIconDialog;
class CItem;


class CRareCreateDialog : public cDialog
{
	cIconDialog*	m_pIconDlg;
	CVirtualItem	m_VirtualItem;

	DWORD		m_ItemIdx;
	DWORD		m_ItemPos;

public:
	CRareCreateDialog(void);
	~CRareCreateDialog(void);

	void Linking();
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	void ReleaseItem();
	void AddVirtualItem( CItem* pItem );

	void SetShopItemInfo(DWORD ItemIdx, DWORD ItemPos)		{	m_ItemIdx=ItemIdx;	m_ItemPos=ItemPos;		}
	void ItemRareCreateSyn();
	void ItemRareCreateAck();
	void ItemRareCreateNAck();
};
