// InventoryExDialog.h: interface for the CInventoryExDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INVENTORYEXDIALOG_H__6F0EE0F4_67B8_4D9B_9988_6E5ABD8BA7FB__INCLUDED_)
#define AFX_INVENTORYEXDIALOG_H__6F0EE0F4_67B8_4D9B_9988_6E5ABD8BA7FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cTabDialog.h"
//#include "WindowIDEnum.h"
//#include ".\interface\cWindowHeader.h"

#include "WearedExDialog.h"
#include "ItemShopInven.h"

//#define CELLNUMPERPAGE				20

class CWearedExDialog;
class CItem;
class CMugongBase;
class CDealItem;
class cStatic;
class cMsgBox;
class cPushupButton;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//	인벤토리 : 탭의 개수에 따라 절대위치와 상대위치를 사용해 조작함
//	장착창	 : WEARED_STARTPOSITION ~ WEARED_ENDPOSITION의 위치를 가짐
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
enum { INV=0, WEAR=1, SHOPIVN=2, };
enum eINVENTORY_STATE { eITEMDEFAULT_STATE, eITEMUPGRADE_STATE, eITEMDEAL_STATE, };
class CInventoryExDialog : public cTabDialog  
{
	// 부활주문서 관련
	CItem*		m_pSelectedShopItem;
	REVIVEDATA	m_ReviveData;

public:
	CInventoryExDialog();
	virtual ~CInventoryExDialog();
	virtual void Render();
	virtual void Add(cWindow * window);
	virtual DWORD ActionEvent(CMouse * mouseInfo);

	BOOL	AddItem(ITEMBASE * itemBase);
	BOOL	AddItem(CItem* pItem);
	BOOL	DeleteItem(POSTYPE Pos,CItem** ppItem);
	void	MoveItem(POSTYPE FromPos, POSTYPE ToPos);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	BOOL	GetBlankPositionRestrictRef(WORD & absPos);
	BOOL	CheckItemLockForItemIdx(WORD wItemIdx);

	void	Linking();

	BOOL	GetPositionForXYRef(BYTE param, LONG x, LONG y, WORD& pos);
	
	void	SetMoney(DWORD Money);
	BOOL	IsExist(POSTYPE abs_pos);
	BOOL	CanBeMoved(CItem* pItem,POSTYPE pos);
	
	void	OnActionEvnet(LONG lId, void * p, DWORD we);
	

	void	UseItem(CItem* pItem);
	// data ope
	CItem * GetItemForPos(POSTYPE absPos);
	CItem * GetCurSelectedItem(BYTE type);								// inv(0), wear(1), SHOPIVN(2)
	BOOL	UpdateItemDurabilityAdd(POSTYPE absPos, int dur);				// 영약 아이템 중복처리를 위해(-)가 올수 도 있음: 아이템 사용했을 때
	void	SetState(eINVENTORY_STATE state){ m_iInventoryState = state; }
	WORD	GetTabIndex(POSTYPE absPos);
	
//KES EXCHNAGE 030929
	int		GetBlankNum();
//
//KES 040316 맵전환 로딩관련 수정	
	void	ReleaseInventory();

	virtual void SetActive(BOOL val);


	DURTYPE	GetTotalItemDurability(WORD wItemIdx);	
	CItem * GetItemLike(WORD wItemIdx);

	// RaMa - 04.12.09
	void OnShopItemBtn();
	void OffShopItemBtn(LONG lId);
	void ItemUnSealingSync();
	void ItemUnSealing(DWORD absPos);
	void SetReviveData( DWORD ID, DWORD ItemIdx, DWORD ItemPos )
	{	m_ReviveData.TargetID = ID;		m_ReviveData.ItemIdx = (WORD)ItemIdx;		m_ReviveData.ItemPos = (POSTYPE)ItemPos;		}
	REVIVEDATA GetReviveData()			{	return m_ReviveData;	}
	void SetShopItemInvenBtn( BOOL val );
	CItem*	GetSelectedShopItem()		{	return m_pSelectedShopItem;	}
	void ResetSelectedShopItem()		{	m_pSelectedShopItem = NULL;	}
	CItemShopInven* GetShopInven()		{	return m_pItemShopInven;	}


private:
	void	FakeBuyItem(LONG mouseX, LONG mouseY, CDealItem * icon);
	BOOL	IsDivideFunctionKey();
	BOOL	FakeMoveItem(LONG mouseX, LONG mouseY, CItem * icon);				// network syn msg send
	BOOL	FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	BOOL	FakeItemCombine( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	BOOL	FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	static void OnFakeItemDivideOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnFakeItemDivideCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );

	CWearedExDialog		* m_pWearedDlg;
	cStatic				* m_pMoneyEdit;
	// RaMa -04.12.09
	CItemShopInven*		m_pItemShopInven;
	cPushupButton*		m_pItemShopBtn;
	BOOL				m_bShopInven;

	BOOL				m_fSpread;
	eINVENTORY_STATE	m_iInventoryState;

	MSG_ITEM_DIVIDE_SYN	m_divideMsg;

//버튼푸쉬 처리
	DWORD	m_BtnPushstartTime;	
	DWORD	m_BtnPushDelayTime;
	BOOL	m_bPushTabWithMouseOver;

// quest
public:
	cMsgBox*			m_pQuestMsgBox;
	CItem*				m_pTempQuestItem;
	void	ItemUseForQuestStart();

	//SW061211 크리스마스이벤트
	CItem*	GetPriorityItemForCristmasEvent();	// 시간되면 확장성 고려해서 다시! //GetPriorityItemFor(flag cristmasEvent)
	DWORD	GetPriorityItemIdx();
};

#endif // !defined(AFX_INVENTORYEXDIALOG_H__6F0EE0F4_67B8_4D9B_9988_6E5ABD8BA7FB__INCLUDED_)
														
