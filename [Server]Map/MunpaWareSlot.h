// MunpaWareSlot.h: interface for the CMunpaWareSlot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUNPAWARESLOT_H__44E42ACB_8C7A_4F18_8252_BFCDEDA216C0__INCLUDED_)
#define AFX_MUNPAWARESLOT_H__44E42ACB_8C7A_4F18_8252_BFCDEDA216C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemSlot.h"
class CPlayer;


class CMunpaWareSlot : public CItemSlot  
{
public:
	CMunpaWareSlot();
	virtual ~CMunpaWareSlot();

	void Init(POSTYPE startAbsPos, POSTYPE slotNum, ITEMBASE * pInfo, SLOTINFO * pSlotInfo);
	virtual ERROR_ITEM UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=UB_ALL, WORD state=SS_NONE, DWORD RareDBIdx=0);
	virtual ERROR_ITEM InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state=SS_NONE);
	virtual ERROR_ITEM DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state=SS_NONE);

	BYTE GetTabNum(POSTYPE absPos);
	void GetItemInfoTab(BYTE TabNum, ITEMBASE* pRtItemArray);

	virtual BOOL IsLock(POSTYPE absPos);
	virtual BOOL IsEmpty(POSTYPE absPos);
protected:
	virtual inline BOOL IsEmptyInner(POSTYPE absPos);
	virtual inline BOOL IsPassword(POSTYPE absPos);
	virtual void SetLock(POSTYPE absPos, BOOL val);

};

#endif // !defined(AFX_MUNPAWARESLOT_H__44E42ACB_8C7A_4F18_8252_BFCDEDA216C0__INCLUDED_)
