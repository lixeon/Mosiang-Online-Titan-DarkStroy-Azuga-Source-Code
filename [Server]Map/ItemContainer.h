// ItemContainer.h: interface for the CItemContainer class.
// created by taiyo
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMCONTAINER_H__76632FDD_2DBD_4175_B1F5_2F88F531EC1E__INCLUDED_)
#define AFX_ITEMCONTAINER_H__76632FDD_2DBD_4175_B1F5_2F88F531EC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPurse;
class CItemSlot;
//class CPlayer;

class CItemContainer
{
	ITEMBASE	m_TotalItemBase[TP_MAX];
	SLOTINFO	m_TotalSlotInfo[TP_MAX];
	CItemSlot	* m_pItemSlot[eItemTable_TableMax];
	CItemSlot * GetSlot(POSTYPE absPos);
	CItemSlot * GetSlot(eITEMTABLE tableIdx);

	friend class CPlayer;
public:
	CItemContainer();
	virtual ~CItemContainer();

	void Init();
	void Release();

	void SetInit(WORD tableIdx, POSTYPE StartPosition, POSTYPE slotNum, CItemSlot * pNewSlot);
	ITEMBASE const *  GetItemInfoAbs(POSTYPE absPos);

	CPurse*		GetPurse(eITEMTABLE tableIdx);
	
//	BOOL InsertItemAbs(POSTYPE absPos, ITEMBASE * pItem);
//	BOOL UpdateItemAbs(POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=IUB_ALL);
//	BOOL DeleteItemAbs(POSTYPE absPos, ITEMBASE * pItemOut, DWORD checkDBIdx=0);
	
	//BOOL MoveItemAbs(POSTYPE absPos, ITEMBASE * pItem);
};

#endif // !defined(AFX_ITEMCONTAINER_H__76632FDD_2DBD_4175_B1F5_2F88F531EC1E__INCLUDED_)
