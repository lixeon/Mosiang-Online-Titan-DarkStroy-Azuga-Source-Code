#pragma once

#include "ItemSlot.h"


class CPlayer;

class CFortWarWareSlot : public CItemSlot
{
protected:
	ITEMBASE	m_TotalItemBase[SLOT_SEIGEFORTWARE_NUM];
	SLOTINFO	m_TotalSlotInfo[SLOT_SEIGEFORTWARE_NUM];

public:
	CFortWarWareSlot(void);
	~CFortWarWareSlot(void);

	void Initialize();
	virtual ERROR_ITEM InsertItemAbs(CPlayer* pPlayer, POSTYPE absPos, ITEMBASE* pItem, WORD state=SS_NONE);
	virtual ERROR_ITEM DeleteItemAbs(CPlayer* pPlayer, POSTYPE absPos, ITEMBASE* pItemOut, WORD state=SS_NONE);
	virtual ERROR_ITEM UpdateItemAbs(CPlayer* pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=UB_ALL, WORD state=SS_NONE, DWORD RareDBIdx=0);	

	WORD GetEmptySlotPos();
	void GetItemInfoAll( MSG_FORTWAR_WAREHOUSE_INFO* pRtInfo );
	ITEMBASE* GetSameItemForDupItem( DWORD dwItemIdx, WORD wItemNum );

	// 
	virtual void SetLock(POSTYPE absPos, BOOL val);
	virtual BOOL IsLock(POSTYPE absPos);
	virtual BOOL IsEmpty(POSTYPE absPos);
protected:
	virtual inline BOOL IsEmptyInner(POSTYPE absPos);
	virtual inline BOOL IsPassword(POSTYPE absPos);
};

