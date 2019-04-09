// DealDialog.h: interface for the CDealDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALDIALOG_H__AC3831B9_327B_4DAD_A22F_DCDF6BAC9E14__INCLUDED_)
#define AFX_DEALDIALOG_H__AC3831B9_327B_4DAD_A22F_DCDF6BAC9E14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cLinkedList.h"
#include "./Interface/cTabDialog.h"

class cStatic;
class CItem;
class CDealItem;
class CMHFile;

#define CELLNUMPERPAGE_DEAL				25
#define MAX_DEALER_NUM					100

struct DealerItem
{
	BYTE Tab;
	BYTE Pos;
	WORD ItemIdx;
	int ItemCount;
};
struct DealerData
{
//	BOOL FindItem(WORD ItemIdx)
//	{
//		DealerItem* pItem = NULL;
//		POS pos = m_DealItemList.GetFirstPos();
//		while(pItem = m_DealItemList.GetNextPos(pos) )
//		{
//			if(pItem->ItemIdx == ItemIdx)
//				return TRUE;
//		}
//
//		return FALSE;
//	}

	cLinkedList<DealerItem> m_DealItemList;
};

class CDealDialog : public cTabDialog  
{
	cStatic					* m_pMoneyEdit;
	CYHHashTable<DealerData>	m_DealerTable;
	BOOL						m_fShow;
	WORD						m_DealerIdx;		// Npc 인덱스로 사용

	WORD						m_CurSelectedItemIdx;
	LONG						m_lCurSelItemPos;

	MSG_ITEM_SELL_SYN			m_sellMsg;
	MSG_ITEM_BUY_SYN			m_buyMsg;
public:
	CDealDialog();
	virtual ~CDealDialog();	
	virtual void Render();
	virtual void Add(cWindow * window);
	
	void LoadDealerItem(CMHFile* fp);
	void SetMoney(DWORD value, BYTE colorType);

	void Linking();
	
	BOOL IsShow() { return m_fShow; }
	void ShowDealer(WORD DealerKey);
	void HideDealer();
	DWORD GetDealerIdx() { return m_DealerIdx; }

	void Release();
	
	virtual void SetActive(BOOL val);

/*
	BOOL CDealDialog::IsAddable(WORD cellX,WORD cellY,cIcon* pIcon);
	BOOL CDealDialog::MoveIcon(WORD cellX, WORD cellY, cIcon * icon);
*/
	void	OnActionEvnet(LONG lId, void * p, DWORD we);
	//void FakeSellItem( LONG mouseX, LONG mouseY, CItem * pItem, DURTYPE Dur );
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	static void OnFakeSellItem( LONG iId, void* p, DWORD param1, void * vData1, void* vData2 );
	static void OnCancelSellItem( LONG iId, void* p, DWORD param1, void * vData1, void* vData2 );
	static void OnFakeBuyItem( LONG iId, void* p, DWORD param1, void * vData1, void* vData2 );
	static void OnCancelBuyItem( LONG iId, void* p, DWORD param1, void * vData1, void* vData2 );
//	void FakeBuyItem(LONG x, LONG y, WORD itemIdx);	//x,y dividebox의 위치잡으려고.
	void FakeBuyItem(LONG x, LONG y, CDealItem* pItem);
	
	void OnSellPushed();
	void OnBuyPushed();
	void OnSelectedItem();
	friend void BuyItem(LONG lId, void * p, DWORD we);
	
	void SendSellItemMsg();
	//void CancelSellItem();
	void SendBuyItemMsg();
	void CancelBuyItem();

	//SW070626 보부상NPC
	void ResetItemInfo(DWORD npcIdx, DWORD itemIdx,  DWORD itemPos, DWORD count);
	BOOL SetDemandItem();
};

#endif // !defined(AFX_DEALDIALOG_H__AC3831B9_327B_4DAD_A22F_DCDF6BAC9E14__INCLUDED_)
