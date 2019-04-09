// ItemShopDialog.h: interface for the CItemShopDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSHOPDIALOG_H__CE96C43E_AB1C_4A12_A246_158A62C1E586__INCLUDED_)
#define AFX_ITEMSHOPDIALOG_H__CE96C43E_AB1C_4A12_A246_158A62C1E586__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "IndexGenerator.h"


class cPushupButton;
class CItemShopGridDialog;
class CItem;


#define MAX_ITEMSHOP_PAGE	3


class CItemShopDialog : public cDialog
{
	cPushupButton*			m_pPageBtn[MAX_ITEMSHOP_PAGE];
	CItemShopGridDialog*	m_pItemGridDialog[MAX_ITEMSHOP_PAGE];

	CIndexGenerator		m_IndexGenerator;

	//
	DWORD		m_curTabNum;
	BOOL		m_bChangeTabWithMouseOver;
	DWORD		m_MouseOverBeginTime;
	DWORD		m_MouseOverDelayTime;

public:
	CItemShopDialog();
	virtual ~CItemShopDialog();	
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void SetActive( BOOL val );

	void Linking();
	void SetItemInfo(SEND_SHOPITEM_INFO* pItemInfo);
	BOOL AddItem(CItem* pItem);
	// Delete
	BOOL DeleteItem(POSTYPE Pos,CItem** ppItem);
	void ShopItemDelete(DWORD dwDBIdx, POSTYPE absPos, DWORD dwCount);

	// Move
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon* icon);

	//
	void TabChange(LONG ID);
	DWORD GetTabNumber(POSTYPE absPos);
	CItem* GetItemForPos(POSTYPE absPos);
};

#endif // !defined(AFX_ITEMSHOPDIALOG_H__CE96C43E_AB1C_4A12_A246_158A62C1E586__INCLUDED_)
