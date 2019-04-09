// ItemSlot.h: interface for the CItemSlot class.
// created by taiyo
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSLOT_H__8056E436_4AA0_474D_AFFB_BA0ED84B6697__INCLUDED_)
#define AFX_ITEMSLOT_H__8056E436_4AA0_474D_AFFB_BA0ED84B6697__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//enum UPDATE_BIT{ UB_DBIDX = 1, UB_ICONIDX=2, UB_ABSPOS=4, UB_QABSPOS=8, UB_DURA=16, UB_ALL=31, };
//SW050920 Rare
enum UPDATE_BIT{ UB_DBIDX = 1, UB_ICONIDX=2, UB_ABSPOS=4, UB_QABSPOS=8, UB_DURA=16, UB_RARE=32, UB_ALL=63, };
enum ERROR_ITEM { EI_TRUE = 0, EI_OUTOFPOS = 1, EI_NOTEQUALDATA, EI_EXISTED, EI_NOTEXIST, EI_LOCKED, EI_PASSWD, EI_NOTENOUGHMONEY, EI_NOSPACE, EI_MAXMONEY };
// SS_LOCKOMIT 을 수행한 함수 후에 락이 풀린다. 
// 주의 : Updateitemabs도 락이 풀린다!
enum SLOT_STATE { SS_NONE = 0, SS_PREINSERT=1, SS_LOCKOMIT = 2, SS_CHKDBIDX = 4, };

class CPlayer;
class CPurse;

class CItemSlot  
{
public:
	CItemSlot();
	virtual ~CItemSlot();

	virtual void Init(POSTYPE startAbsPos, POSTYPE slotNum, ITEMBASE * pInfo, SLOTINFO * pSlotInfo);
	void Release();
	
	virtual ITEMBASE const * GetItemInfoAbs(POSTYPE absPos);
	void GetItemInfoAll(ITEMBASE * pItemBaseAll);
	void SetItemInfoAll(ITEMBASE * pItemBaseAll);
	char * _ToStringSlot();

	//virtual ERROR_ITEM InsertItemAbs(POSTYPE absPos, ITEMBASE * pItem, WORD state=SS_NONE);
	//virtual ERROR_ITEM UpdateItemAbs(POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=UB_ALL);
	//virtual ERROR_ITEM DeleteItemAbs(POSTYPE absPos, ITEMBASE * pItemOut);

	//virtual ERROR_ITEM UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=UB_ALL, WORD state=SS_NONE);
	//SW050920 Rare
	virtual ERROR_ITEM UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=UB_ALL, WORD state=SS_NONE, DWORD RareDBIdx = 0);
	virtual ERROR_ITEM InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state=SS_NONE);
	virtual ERROR_ITEM DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state=SS_NONE);
		

	POSTYPE GetStartPos() { return m_StartAbsPos; }
	POSTYPE GetSlotNum() { return m_SlotNum; }
	inline virtual BOOL IsEmpty(POSTYPE absPos);

	CPurse*	GetPurse() { return m_pPurse; }
	MONEYTYPE GetMoney();

//	virtual MONEYTYPE Addition(MONEYTYPE money);
//	virtual MONEYTYPE Subtraction(MONEYTYPE money);

	/* purse func */
	BOOL CreatePurse( CPurse* pNewPurse, void* pOwner, MONEYTYPE money, MONEYTYPE max = MAXMONEY );

	virtual BOOL IsAddable(POSTYPE absPos, ITEMBASE * pItemBase){	return TRUE;	}
	virtual WORD GetEmptyCell(WORD * EmptyCellPos, WORD NeedCount = 1) { return 0; }
	virtual void SetLock(POSTYPE absPos, BOOL val);
	virtual BOOL IsLock(POSTYPE absPos);

	virtual WORD GetItemCount();

	// RaMa	
	virtual BOOL CheckQPosForItemIdx( WORD ItemIdx )		{	return TRUE;	}
	virtual BOOL CheckItemLockForItemIdx( WORD ItemIdx )	{	return TRUE;	}

	
protected:
	virtual inline BOOL IsEmptyInner(POSTYPE absPos);
	virtual inline BOOL IsPosIn(POSTYPE absPos);
	virtual inline BOOL IsPassword(POSTYPE absPos);

	ITEMBASE *		m_ItemBaseArray;
	SLOTINFO *		m_SlotInfoArray;
	POSTYPE			m_StartAbsPos;
	POSTYPE			m_SlotNum;						// Slot의 갯수

	/* purse value */
	CPurse*			m_pPurse;
};

#endif // !defined(AFX_ITEMSLOT_H__8056E436_4AA0_474D_AFFB_BA0ED84B6697__INCLUDED_)
