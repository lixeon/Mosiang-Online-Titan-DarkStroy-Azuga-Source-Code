// DissolutionDialog.h: interface for the CDissolutionDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISSOLUTIONDIALOG_H__7B5846EA_445A_40D1_8C32_21187E1D4E46__INCLUDED_)
#define AFX_DISSOLUTIONDIALOG_H__7B5846EA_445A_40D1_8C32_21187E1D4E46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cTabDialog.h"
#include "VirtualItem.h"
#include "ItemShow.h"

#define MAX_DISSOLUTION_TABTNUM		3
#define MAX_UNIT_PERTAB				20
#define MAX_DISSOLUTION_UNITNUM		60

class cIconDialog;
class cIconGridDialog;
class CItem;

class CDissolutionDialog : public cTabDialog
{
protected:
	cIconDialog*		m_pDissolutionItemDlg;
	cIconGridDialog*	m_pUnitItemDlg[MAX_DISSOLUTION_TABTNUM];
	CVirtualItem		m_VirtualDissolutionItem;
	CItemShow			m_ShowUnitItem[MAX_DISSOLUTION_UNITNUM];

	int					m_nTotalUnitOutNum;
	ITEMBASE			m_CheckItemInfo;

public:
	CDissolutionDialog();
	virtual ~CDissolutionDialog();
	virtual void Add( cWindow * window );
	virtual DWORD ActionEvent( CMouse* mouseInfo );
	virtual void Render();
	virtual void SetActive(BOOL val);
	void Linking();
	void OnActionEvent( LONG lId, void* p, DWORD we );

	virtual BOOL FakeMoveIcon( LONG x, LONG y, cIcon * icon );

	void AddDissolutionItem( CItem* pOrigItem );
	void RemoveDissolutionItem();
	BOOL ShowUnitItems( WORD wDissolutionItemIdx );
	void RemoveAllUnitItems();

	CVirtualItem* GetDissolutionVirtualItem();
	CItem* GetDissolutionInventoryItem();

//---
	BOOL SendMsg_Dissolution();
	void EndDissolution( BOOL bSuccess );

};

#endif // !defined(AFX_DISSOLUTIONDIALOG_H__7B5846EA_445A_40D1_8C32_21187E1D4E46__INCLUDED_)
