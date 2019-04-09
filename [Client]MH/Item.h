// Item.h: interface for the CItem class.aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__68113CBB_0A76_42D7_90B6_C64FBAF3E85C__INCLUDED_)
#define AFX_ITEM_H__68113CBB_0A76_42D7_90B6_C64FBAF3E85C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./BaseItem.h"
#include "GameResourceManager.h"

class CExchangeItem;

class CItem : public CBaseItem
{
protected:
	ITEMBASE m_ItemBaseInfo;
	
	POSTYPE			m_LinkPos;	//KES 030919 교환창 및 노점상 연결위치
	CExchangeItem*	m_pLinkItem;

	DWORD			m_Param;	

	cImage			m_DelayImage;

public:
	BOOL m_bAdded;

	CItem(ITEMBASE* pInfo);
	virtual ~CItem();

	virtual inline DWORD	GetDBIdx()					{	return m_ItemBaseInfo.dwDBIdx;	}
	virtual inline WORD		GetItemIdx()				{	return m_ItemBaseInfo.wIconIdx;	}
	virtual inline POSTYPE	GetPosition()				{	return m_ItemBaseInfo.Position;	}
	virtual inline void		SetPosition(POSTYPE pos)	{	m_ItemBaseInfo.Position = pos;	}

	virtual inline POSTYPE	GetQuickPosition()				{	return m_ItemBaseInfo.QuickPosition;	}
	virtual inline void		SetQuickPosition(POSTYPE pos)	{	m_ItemBaseInfo.QuickPosition = pos;		}

	virtual inline DWORD	GetRareness()					{   return m_ItemBaseInfo.RareIdx; }
	virtual inline DURTYPE	GetDurability()					{	return m_ItemBaseInfo.Durability;	}
	virtual inline void		SetDurability(DURTYPE dur)		{	m_ItemBaseInfo.Durability = dur;		}

//KES 030919 교환창 및 노점상 연결위치	
	inline POSTYPE GetLinkPosition() { return m_LinkPos; }
	inline void SetLinkPosition(POSTYPE pos)	{ m_LinkPos = pos; }
	void SetLinkItem( CExchangeItem* pItem )	{ m_pLinkItem = pItem; }
	CExchangeItem* GetLinkItem()				{ return m_pLinkItem; }

	const ITEMBASE*	GetItemBaseInfo() { return &m_ItemBaseInfo; }
	void SetItemBaseInfo( ITEMBASE * itemInfo );
	
	virtual void SetLock( BOOL bLock );
	

	eITEM_KINDBIT GetItemKind();
	
	const ITEM_INFO* GetItemInfo();
	
	virtual void Render();
	
	virtual BOOL CanMoveToDialog( cDialog* ptargetdlg );
	virtual BOOL CanDelete();
	
	void SetItemParam(ITEMPARAM Param)		{ m_ItemBaseInfo.ItemParam = Param;	}
	DWORD GetItemParam()					{ return m_ItemBaseInfo.ItemParam;	}
	void SetUseParam( DWORD Param )			{ m_Param = Param;	}
	DWORD GetUseParam()						{ return m_Param;	}
};

#endif // !defined(AFX_ITEM_H__68113CBB_0A76_42D7_90B6_C64FBAF3E85C__INCLUDED_)
