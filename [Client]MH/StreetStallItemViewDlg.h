// StreetStallItemViewDlg.h: interface for the CStreetStallItemViewDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_StreetStallItemViewDlg_H)
#define AFX_StreetStallItemViewDlg_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"
#include "ItemShow.h"

class cIcon;
class cIconGridDialog;
class cButton;

class CStreetStallItemViewDlg : public cDialog  
{
	cButton*				m_CloseBtn;
	cIconGridDialog*		m_pStallGrid;
	STREETSTALL_ITEM_INFO	m_sItemInfo[SLOT_STREETSTALL_NUM];
	CItemShow				m_ViewItem[SLOT_STREETSTALL_NUM];

public:
	CStreetStallItemViewDlg();
	virtual ~CStreetStallItemViewDlg();

	void Linking();
	virtual void SetActive( BOOL val );
	void SetItemInfo( STREETSTALL_ITEM_INFO* pInfo, WORD wMaxCount );
	void LinkItem( MSG_STREETSTALL_ITEMVIEW* pStallInfo );
	void DeleteItemInfo();
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);
};

#endif // !defined(AFX_StreetStallItemViewDlg_H)
