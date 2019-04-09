// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Item.h"
#include "ItemManager.h"
#include "ObjectManager.h"
#include "QuickItem.h"
#include "GameIn.h"
#include "./Interface/cFont.h"

#include "QuickDialog.h"
#include "ChatManager.h"
#include "PetManager.h"
#include "TitanManager.h"
#include "interface/cScriptManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItem::CItem(ITEMBASE* pInfo)
{
	m_type = WT_ITEM;
	m_bAdded = FALSE;
	memcpy(&m_ItemBaseInfo,pInfo,sizeof(ITEMBASE));

	m_pLinkItem	= NULL;
	m_LinkPos	= 0;
	m_Param = 0;	

	SCRIPTMGR->GetImage( 127, &m_DelayImage  );
}

CItem::~CItem()
{
}
eITEM_KINDBIT CItem::GetItemKind()
{
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_ItemBaseInfo.wIconIdx);
	ASSERT(pInfo);	//KES 040512
	if(pInfo)
		return (eITEM_KINDBIT)pInfo->ItemKind;

	return (eITEM_KINDBIT)0;
}


const ITEM_INFO* CItem::GetItemInfo()
{
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_ItemBaseInfo.wIconIdx);
	ASSERT(pInfo);
	if(pInfo)
	ASSERT(pInfo->ItemIdx == m_ItemBaseInfo.wIconIdx);
	return pInfo;
}

void CItem::SetItemBaseInfo( ITEMBASE * itemInfo )
{
	memcpy( &m_ItemBaseInfo, itemInfo, sizeof( ITEMBASE ) );

}

void CItem::Render()
{
	if( !m_bActive )		return;	

	if( m_ItemBaseInfo.ItemParam & ITEM_PARAM_SEAL )
	{
		cDialog::Render();
		
		RECT rect={(LONG)m_absPos.x+9, (LONG)m_absPos.y+15, 1,1};
		CFONT_OBJ->RenderFont(0,CHATMGR->GetChatMsg(773),strlen(CHATMGR->GetChatMsg(773)),&rect,RGBA_MAKE(250,250,128,255));
	}
	// magi82 - Titan(070423) 등록된 타이탄 증서
	else if( m_ItemBaseInfo.ItemParam & ITEM_PARAM_TITAN_REGIST )
	{
		cDialog::Render();

		DWORD dwRemainTime = 0;
		float delayRate = HERO->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Item,m_ItemBaseInfo.wIconIdx, &dwRemainTime );
		if( delayRate > 0.0f )
		{
			VECTOR2 vScale = { 1.0f, delayRate };
			VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
			m_DelayImage.RenderSprite( &vScale, NULL, 0.0f, &pos, 0xff000000  );
		}

		RECT rect={(LONG)m_absPos.x+9, (LONG)m_absPos.y+15, 1,1};
		CFONT_OBJ->RenderFont(0,CHATMGR->GetChatMsg(1515),4,&rect,RGBA_MAKE(250,250,128,255));	// magi82 - Titan(070427)
	}
	else if( PETMGR->IsCurPetSummonItem(m_ItemBaseInfo.dwDBIdx) )
	{
		cDialog::Render();

		RECT rect={(LONG)m_absPos.x+9, (LONG)m_absPos.y+15, 1,1};
		CFONT_OBJ->RenderFont(0,CHATMGR->GetChatMsg(1246),strlen(CHATMGR->GetChatMsg(773)),&rect,RGBA_MAKE(250,250,128,255));
	}
	else
		cIcon::Render();

	if( m_Param == 1 )
	{
		m_LockImage.RenderSprite( NULL, NULL, 0, &m_absPos, 
			RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100));
	}

	if( ITEMMGR->IsDupItem(m_ItemBaseInfo.wIconIdx) )// && m_ItemBaseInfo.Durability >= 1 )
	{
		static char nums[3];
		wsprintf(nums,"%2d", m_ItemBaseInfo.Durability);
		RECT rect={(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));		// color hard coding : taiyo 
	}

	if( GetItemKind() == eYOUNGYAK_ITEM_TITAN )
	{
		DWORD dwRemainTime = 0;
		float delayRate = HERO->GetDelayGroup()->CheckTitanPotionDelay(&dwRemainTime );
		if( delayRate > 0.0f )
		{
			VECTOR2 vScale = { 1.0f, delayRate };
			VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
			m_DelayImage.RenderSprite( &vScale, NULL, 0.0f, &pos, 0xff000000  );
		}
	}
}

void CItem::SetLock( BOOL bLock )
{
	cIcon::SetLock( bLock );
	
	SetMovable( !bLock );
	//아이템 사용도 금지
}

BOOL CItem::CanMoveToDialog( cDialog* ptargetdlg )
{
	if( GetItemKind() == eQUEST_ITEM_EQUIP )
	{
		int type = ptargetdlg->GetType();
		switch( type )
		{
		case WT_STREETSTALLDIALOG:
		case WT_STREETBUYSTALLDIALOG:
		case WT_DEALDIALOG:
		case WT_MUGONGSURYUNDIALOG:
		case WT_GUILDWAREHOUSEDLG:
		case WT_QUESTTOTALDIALOG:
		case WT_ITEMSHOPDIALOG:
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CItem::CanDelete()
{
	if( GetItemKind() == eQUEST_ITEM_EQUIP )
		return FALSE;
	return TRUE;
}