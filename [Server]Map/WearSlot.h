// WearSlot.h: interface for the CWearSlot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEARSLOT_H__AB13B8D4_8C19_43C5_A507_62C3BC88896C__INCLUDED_)
#define AFX_WEARSLOT_H__AB13B8D4_8C19_43C5_A507_62C3BC88896C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemSlot.h"

class CWearSlot : public CItemSlot  
{
public:
	CWearSlot();
	virtual ~CWearSlot();

	virtual ERROR_ITEM UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=UB_ALL, WORD state=SS_NONE, DWORD RareDBIdx = 0);
	virtual ERROR_ITEM InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state=SS_NONE);
	virtual ERROR_ITEM DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state=SS_NONE);

	virtual BOOL IsAddable(POSTYPE absPos, ITEMBASE * pItemBase);
	// ¿ÜºÎ¿¡¼­ È£ÃâÇØ¾ß Å¬¶óÀÌ¾ðÆ®, DB, ¸ðµç usrÀû¿ë
	void ApplyItem(CPlayer * pPlayer);
};

#endif // !defined(AFX_WEARSLOT_H__AB13B8D4_8C19_43C5_A507_62C3BC88896C__INCLUDED_)
