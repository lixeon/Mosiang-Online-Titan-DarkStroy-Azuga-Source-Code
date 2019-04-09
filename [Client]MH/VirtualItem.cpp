// VirtualItem.cpp: implementation of the CVirtualItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VirtualItem.h"
#include "ItemManager.h"
#include "./Interface/cFont.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVirtualItem::CVirtualItem()
{
	m_pItemLink = NULL;
	m_type = WT_VIRTUALITEM;
	cIcon::Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, NULL, -1 );
}

CVirtualItem::~CVirtualItem()
{
	m_pItemLink = NULL;
}
void CVirtualItem::SetLinkItem(CBaseItem * pItem)
{
	m_pItemLink = pItem;
	if(pItem)
	{
		SetBasicImage(m_pItemLink->GetBasicImage());
	}
	else
	{
		SetCurImage(NULL);
	}
}
eITEM_KINDBIT CVirtualItem::GetSrcItemKind()
{
	if(m_pItemLink)
	{
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_pItemLink->GetItemIdx());
		return (eITEM_KINDBIT)pInfo->ItemKind;
	}
	return eKIND_ITEM_MAX;
}
void CVirtualItem::Render()
{
	cIcon::Render();

	if( m_pItemLink && ITEMMGR->IsDupItem(m_pItemLink->GetItemIdx()) )//m_pItemLink->GetDurability() >= 1 )
	{
		static char nums[3];
		wsprintf(nums,"%2d", m_pItemLink->GetDurability());
		RECT rect={(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));		// color hard coding : taiyo 
	}
}