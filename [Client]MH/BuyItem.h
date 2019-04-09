// BuyItem.h: interface for the CBuyItem class.
// 구매용 가상 아이템
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUYITEM_H__3002E03D_BE93_4267_B015_37BB522D150A__INCLUDED_)
#define AFX_BUYITEM_H__3002E03D_BE93_4267_B015_37BB522D150A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cIcon.h"

class CItem;

class CBuyItem : public cIcon  
{
	WORD		m_ItemIdx;
	WORD		m_nVolume;
	DWORD		m_nMoney;
	ITEMPARAM	m_dwParam;
	BOOL		m_bVolume;
		
public:
	CBuyItem();
	virtual ~CBuyItem();

	void InitItem(ITEMBASE ItemInfo, WORD Volume, DWORD Money);
	
	WORD GetVolume() { return m_nVolume; }
	void SetVolume(WORD Volume) { m_nVolume = Volume; }
	
	DWORD GetMoney() { return m_nMoney; }
	void SetMoney(DWORD Money) { m_nMoney = Money; }

	WORD GetItemIdx() { return m_ItemIdx; }

	virtual void Render();
};

#endif // !defined(AFX_BUYITEM_H__3002E03D_BE93_4267_B015_37BB522D150A__INCLUDED_)
