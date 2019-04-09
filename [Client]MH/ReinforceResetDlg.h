// CReinforceResetDlg.h: interface for the ReinforceResetDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREINFORCERESETDLG_H__C771165D_35B7_4B3F_9EDB_56A8C1CDE255__INCLUDED_)
#define AFX_CREINFORCERESETDLG_H__C771165D_35B7_4B3F_9EDB_56A8C1CDE255__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "VirtualItem.h"

class cButton;
class cIconDialog;
class CItem;



class CReinforceResetDlg : public cDialog
{
	cIconDialog*	m_pIconDlg;
	CVirtualItem	m_VirtualItem;

	DWORD		m_ItemIdx;
	DWORD		m_ItemPos;

public:
	CReinforceResetDlg();
	virtual ~CReinforceResetDlg();

	void Linking();
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	void ReleaseItem();
	void AddVirtualItem( CItem* pItem );

	void SetItemInfo(DWORD ItemIdx, DWORD ItemPos)		{	m_ItemIdx=ItemIdx;	m_ItemPos=ItemPos;		}
	void ItemResetSyn();
	void ItemResetAck();
	void ItemResetNAck();
};

#endif // !defined(AFX_REINFORCERESETDLG_H__C771165D_35B7_4B3F_9EDB_56A8C1CDE255__INCLUDED_)
