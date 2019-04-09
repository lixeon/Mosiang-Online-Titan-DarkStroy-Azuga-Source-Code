// BuyRegDialog.h: interface for the CBuyRegDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUYREGDIALOG_H__97E77928_909A_49F6_ACD0_F17FB41E033B__INCLUDED_)
#define AFX_BUYREGDIALOG_H__97E77928_909A_49F6_ACD0_F17FB41E033B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "GameResourceStruct.h"

class cListDialog;
class cEditBox;
class cButton;
class CMouse;
class CExchangeItem;
class cStatic;

#define DEFAULT_MONEY_TEXT	"0"
#define DEFAULT_VOLUME_TEXT	"0"
#define MAX_VOLUME_TEXT	"20"

enum ITEM_TYPE
{
	WEAPON,
	CLOTHES,
	ACCESSORY,
	POTION,
	MATERIAL,
	ETC,
	ITEM_MALL,
	ITEM_TYPE_COUNT,
};

typedef struct _BUYREGINFO{
	WORD Type;
	WORD Item;
	WORD Class;
	WORD Volume;
	DWORD Money;
} BUY_REG_INFO;

class CBuyRegDialog : public cDialog  
{
	cListDialog* m_pTypeList;
	cListDialog* m_pItemList[ITEM_TYPE_COUNT];
	cListDialog* m_pClassList;
	cEditBox* m_pVolumeEdit;
	cEditBox* m_pMoneyEdit;
	cButton* m_pRegBtn;
	cButton* m_pDltBtn;

	cStatic* m_pClassText;
	cStatic* m_pClassRect1;
	cStatic* m_pClassRect2;
	cStatic* m_pItemBigRect;
	cStatic* m_pItemSmallRect1;
	cStatic* m_pItemSmallRect2;

	WORD m_nType;
	WORD m_nItem;
	WORD m_nClass;
	
	WORD m_nVolume;
	DWORD m_nMoney;

	void LoadItemList();
	
public:
	CBuyRegDialog();
	virtual ~CBuyRegDialog();

	void Linking();
	void Show();
	void Close();
	void InitRegInfo();
	void SetRegInfo(BUY_REG_INFO& RegInfo);

	void UpdateType();
	void UpdateItem();
	void UpdateClass();
	ITEM_INFO* GetSelectItemInfo();
	BOOL GetBuyRegInfo(BUY_REG_INFO& RegInfo);

	void OnActionEvnet(LONG lId, void * p, DWORD we);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual DWORD ActionEventWindow(CMouse * mouseInfo);
};

#endif // !defined(AFX_BUYREGDIALOG_H__97E77928_909A_49F6_ACD0_F17FB41E033B__INCLUDED_)
