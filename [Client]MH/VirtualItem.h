// VirtualItem.h: interface for the CVirtualItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIRTUALITEM_H__B62DCBD9_833C_4B73_9B2D_6B4B29C1318F__INCLUDED_)
#define AFX_VIRTUALITEM_H__B62DCBD9_833C_4B73_9B2D_6B4B29C1318F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseItem.h"

class CVirtualItem : public CBaseItem  
{
public:
	CVirtualItem();
	virtual ~CVirtualItem();
	void Init();
	WORD GetSrcItemIdx()			{ return m_pItemLink->GetItemIdx();		}
	POSTYPE GetSrcPosition()		{ return m_pItemLink->GetPosition();	}
	eITEM_KINDBIT GetSrcItemKind();
	
	DWORD	GetRareness()			{   return m_pItemLink->GetRareness();	}
	DURTYPE	GetSrcDurability()		{	return m_pItemLink->GetDurability();	}
	void SetLinkItem(CBaseItem * pItem);
	CBaseItem * GetLinkItem()
	{
		return m_pItemLink;
	}
	virtual void Render();
protected:
	CBaseItem * m_pItemLink;
	

};

#endif // !defined(AFX_VIRTUALITEM_H__B62DCBD9_833C_4B73_9B2D_6B4B29C1318F__INCLUDED_)
