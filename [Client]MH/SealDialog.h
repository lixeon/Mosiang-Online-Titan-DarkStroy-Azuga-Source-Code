// SealDialog.h: interface for the CSealDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEALDIALOG_H__7AF799F4_2546_48F3_887A_DC86EDB7BEF0__INCLUDED_)
#define AFX_SEALDIALOG_H__7AF799F4_2546_48F3_887A_DC86EDB7BEF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "VirtualItem.h"

class cIconDialog;
class CVirtualItem;
class CItem;



class CSealDialog : public cDialog
{
	cIconDialog*	m_pIconDlg;
	CVirtualItem	m_VirtualItem;

	DWORD	m_ItemIdx;
	DWORD	m_ItemPos;

public:
	CSealDialog();
	virtual ~CSealDialog();

	void Linking();
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	void ReleaseItem();
	void AddVirtualItem( CItem* pItem );

	void SetItemInfo(DWORD ItemIdx, DWORD ItemPos)		{	m_ItemIdx=ItemIdx;	m_ItemPos=ItemPos;		}
	void ItemSealSyn();
	void ItemSealAck();
	void ItemSealNAck();
};

#endif // !defined(AFX_SEALDIALOG_H__7AF799F4_2546_48F3_887A_DC86EDB7BEF0__INCLUDED_)
