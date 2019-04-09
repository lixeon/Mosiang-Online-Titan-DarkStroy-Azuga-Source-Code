// StreetBuyStall.h: interface for the CStreetBuyStall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STREETBUYSTALL_H__D249A0E1_09CF_4695_BF50_CE8068674BC8__INCLUDED_)
#define AFX_STREETBUYSTALL_H__D249A0E1_09CF_4695_BF50_CE8068674BC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "./Interface/cIconGridDialog.h"
#include "./Input/Mouse.h"
#include "BuyRegDialog.h"

class cIcon;
class cIconGridDialog;
class cEditBox;
class cTextArea;
class cButton;

enum SELL_ERROR_CODE
{
	// Chat_Msg와 연동
	eSE_BUYER_NOT_SPACE = 1216,	//1216
	eSE_BUYER_NOT_MONEY,		//1217
	eSE_ERROR,					//1218
	eSE_RARE,					//1219
	eSE_OPTION,					//1220

	eSE_NOT_ALLOW = 1223,		//1223
	eSE_NOT_SELECT = 1225,		//1225
	eSE_NOT_EXIST,				//1226
	eSE_NOT_HAVE,				//1227
	
	eSE_QUICK = 348,			//348
	eSE_WEARED = 359,			//359
};

enum BUY_STALL_DLG_STATE
{
	eBSDS_NOT_OPENED,
	eBSDS_OPENED,
	eBSDS_SELL,
};

enum BUY_STALL_OPTION
{
	eBSO_DIVE,
	eBSO_INPUTMONEY,
	eBSO_INPUTMONEY_DUP,
};

#define DEFAULT_TITLE_TEXT	CHATMGR->GetChatMsg(366)
#define DEFAULT_MONEY_TEXT	"0"
//#define SLOT_STREETBUYSTALL_NUM		5

class CBuyItem;
class CBuyRegDialog;
class CItem;

class CStreetBuyStall : public cDialog  
{
	DWORD m_DlgState;
	
	cEditBox* m_pTitleEdit;
	cEditBox* m_pMoneyEdit;
	cButton* m_pEnterBtn;
	cIconGridDialog * m_pStallGrid;
	cButton* m_pRegistBtn;
	cButton* m_pSellBtn;
	
	DWORD m_MoneyArray[SLOT_STREETBUYSTALL_NUM];
	BUY_REG_INFO m_RegInfoArray[SLOT_STREETBUYSTALL_NUM];
	BUY_REG_INFO m_FakeRegInfo;

//	WORD m_nUsable;		// 등록 가능한 아이템 갯수
	
	DWORD m_TotalMoney;
	char m_OldTitle[MAX_STREETSTALL_TITLELEN+1];			
	int m_nCurSelectedItem;
	
	DWORD m_dwOwnnerId;									
	
	void* m_pData;

	CItem* m_pSellItem;
	POSTYPE m_SellItemPos;	// 판매하려는 아이템의 실제 위치;

public:
	CStreetBuyStall();
	virtual ~CStreetBuyStall();

	void Linking();
	
	void ShowSellStall();
	void ShowBuyStall();
	
	void OnCloseStall(BOOL bDelOption = FALSE);
	
	void OnMoneyEditClick();
	void OnTitleEditClick();

//	void SetUsableNum(WORD num) { m_nUsable = num; };
	
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	virtual void FakeMoveItem(LONG mouseX, LONG mouseY, cIcon * icon) {}
	virtual void SetDisable(BOOL val);
	void FakeDeleteItem( POSTYPE pos );
	void FakeRegistItem( BUY_REG_INFO RegInfo, ITEM_INFO* pItemInfo);

	void OnActionEvnet(LONG lId, void * p, DWORD we);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual DWORD ActionEventWindow(CMouse * mouseInfo);
	virtual void SetActive( BOOL val );
	
	void OnRegBtn();
	BOOL OnSellBtn();
	
	void RegistMoney();
	void RegistMoney( POSTYPE pos, DWORD dwMoney );
	void RegistTitle( char* title, BOOL bSucess );
	void EditTitle( char* title );
	
public:
	void GetTitle( char* pStrTitle );
	BUY_REG_INFO GetBuyRegInfo(POSTYPE pos) { return m_RegInfoArray[pos]; };
	
	int GetCurSelectedItemNum() { return m_nCurSelectedItem; }
	void SetCurSelectedItemNum(int num) { m_nCurSelectedItem = num; }
	
	cIcon* GetCurSelectedItem();
	DWORD GetCurSelectedItemIdx();
	
	cIcon* GetItem( POSTYPE pos );
	DWORD GetItemMoney( POSTYPE pos ) { return m_MoneyArray[pos]; }
	
	BOOL AddItem( POSTYPE pos, cIcon* pItem );
	void DeleteItem( POSTYPE pos );
	void DeleteItemAll(BOOL bDelOption = FALSE);
	void ResetItemInfo( POSTYPE pos, WORD volume );
	
	void ChangeItemStatus( POSTYPE pos, WORD volume, DWORD money);
	
	BOOL SelectedItemCheck();
	BOOL MoneyEditCheck();
	
	DWORD GetDlgState() { return m_DlgState; }
	
	DWORD GetStallOwnerId() { return m_dwOwnnerId; }
	void SetStallOwnerId( DWORD dwId ) { m_dwOwnnerId = dwId; }
	
	void ResetDlgData();

	// 나누기창 생성
	BOOL ShowDivideBox(WORD Volume);
	static void OnDivideItem( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnDivideItemCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnFakeRegistItem( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnRegistItemCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	
	void* GetData() { return m_pData; }
	void SetData( void* pData ) { m_pData = pData; }
	
	cIconGridDialog* GetGridDialog() { return m_pStallGrid; }

	POSTYPE	GetSellItemPos() { return m_SellItemPos; }
	CItem* GetSellItem() { return m_pSellItem; }
	void DelSellItem() { m_pSellItem = NULL; }

	BOOL SellError(WORD errCode = eSE_ERROR);
};


#endif // !defined(AFX_STREETBUYSTALL_H__D249A0E1_09CF_4695_BF50_CE8068674BC8__INCLUDED_)
