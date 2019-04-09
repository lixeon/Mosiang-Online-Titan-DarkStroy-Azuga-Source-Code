// TitanBreakDlg.cpp: implementation of the CTitanBreakDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TitanBreakDlg.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "item.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "TitanManager.h"
#include "ObjectStateManager.h"
#include "cScriptManager.h"
#include "./interface/cStatic.h"
#include "./interface/cIconDialog.h"
#include "./interface/cIconGridDialog.h"

CTitanBreakDlg::CTitanBreakDlg()
{
	m_pTitanBreakDlg = NULL;
	m_pTitanBreakGridDlg = NULL;

	cImage imgTooltip;
	SCRIPTMGR->GetImage( 63, &imgTooltip, PFT_HARDPATH );

	m_VirtualItem.SetToolTip( "", RGB_HALF(255,255,255), &imgTooltip );
	m_VirtualItem.SetMovable(FALSE);

	for( int i = 0; i < MAX_TITAN_BREAK_GRID_NUM; i++ )
	{
		m_ViewShowItem[i].SetToolTip( "", RGB_HALF(255,255,255), &imgTooltip );
		m_ViewShowItem[i].SetMovable(FALSE);
	}
}

CTitanBreakDlg::~CTitanBreakDlg()
{

}

void CTitanBreakDlg::Release(BOOL bFlag)
{
	DeleteViewMaterialListAll();
	m_pTitanBreakMoney->SetStaticText("");

	// 해당 Icon정보를 삭제
	CVirtualItem * pVItem;
	if(!m_pTitanBreakDlg->IsAddable(0))
	{
		m_pTitanBreakDlg->DeleteIcon( 0, (cIcon**)&pVItem );
		if( bFlag )	// 서버에 락을 해체해야함
		{
			MSG_WORD msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_TITANBREAK_RELEASEITEM;
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		pVItem->GetLinkItem()->SetLock(FALSE);
		pVItem->SetLinkItem(NULL);
	}
}

void CTitanBreakDlg::Linking()
{
	m_pTitanBreakDlg = (cIconDialog*)GetWindowForID(TITANBREAK_TITANICONDLG);
	m_pTitanBreakGridDlg = (cIconGridDialog*)GetWindowForID(TITANBREAK_BREAKICONGRIDDLG);
	m_pTitanBreakMoney = (cStatic*)GetWindowForID(TITANBREAK_RFEE);
}

void CTitanBreakDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( !val )
	{
		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}

DWORD CTitanBreakDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	m_VirtualItem.ActionEvent(mouseInfo);

	return we;
}

BOOL CTitanBreakDlg::OnActionEvent( LONG lId, void * p, DWORD we)
{
	switch(we)
	{
	case WE_CLOSEWINDOW:
		{
			CTitanBreakDlg* pTitanBreakDlg = (CTitanBreakDlg*)WINDOWMGR->GetWindowForID(lId);
			pTitanBreakDlg->Release();
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

			return TRUE;
		}
		break;
	}

	switch( lId )
	{
	case TITANBREAK_BTN_OK:	// 확인하기
		{
			CTitanBreakDlg* pTitanBreakDlg = (CTitanBreakDlg*)p;
			CTitanBreakDlg::BreakOkBtn(pTitanBreakDlg);
		}
		break;
	case TITANBREAK_BTN_CANCEL:	// 취소하기
		{
			CTitanBreakDlg* pTitanBreakDlg = (CTitanBreakDlg*)p;
			pTitanBreakDlg->Release(TRUE);
			pTitanBreakDlg->SetActive(FALSE);
		}
		break;
	}

	return TRUE;
}

void CTitanBreakDlg::BreakOkBtn(CTitanBreakDlg * pThis)
{
	// 아이템이 들어가있는지 체크
	CVirtualItem* pItem = (CVirtualItem*)pThis->m_pTitanBreakDlg->GetIconForIdx(0);
	if(!pItem)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1226) );
		return;
	}

	// 분해시킬 타이탄의 정보 가져옴
	TITAN_BREAKINFO* pTitanBreakInfo = TITANMGR->GetTitanBreakInfoItemInfo(pItem->GetSrcItemIdx());
	if(!pItem)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(690) );
		return;
	}

	if(OBJECTMGR->GetHero()->GetMoney() < pTitanBreakInfo->dwMoney)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
		return;
	}

	MSG_DWORD2 msg;
	ZeroMemory(&msg, sizeof(MSG_DWORD2));
	SetProtocol( &msg, MP_ITEM, MP_ITEM_TITANBREAK_SYN );
	msg.dwObjectID = HEROID;
	msg.dwData1 = pTitanBreakInfo->dwIdx;
	msg.dwData2 = pItem->GetSrcPosition();

	NETWORK->Send(&msg, sizeof(MSG_DWORD2));
}

BOOL CTitanBreakDlg::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if(pOrigIcon->GetType() != WT_ITEM)	return FALSE;
	if( m_bDisable ) return FALSE;

	CItem* pOrigItem = (CItem*)pOrigIcon;
	if( pOrigItem->IsLocked() )	return FALSE;

	// 옵션 아이템이면 FALSE
	if( ITEMMGR->IsOptionItem(pOrigItem->GetItemIdx(), pOrigItem->GetDurability()) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(690) );
		return FALSE;
	}

	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if( !(bits & eTITAN_EQUIPITEM) && !(bits & eTITAN_ITEM_PARTS) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(690) );
		return FALSE;
	}

	TITAN_BREAKINFO* pTitanBreakInfo = TITANMGR->GetTitanBreakInfoItemInfo(pOrigItem->GetItemIdx());
	if(!pTitanBreakInfo)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(690) );	
		return FALSE;
	}

	Release();

	// 타이탄 아이템 넣음
	AddTitanItem(pOrigItem);

	// 업그레이드에 필요한 재료를 보여줌
	ViewMaterialList(pTitanBreakInfo);

	// 업그레이드에 필요한 돈을 보여줌
	m_pTitanBreakMoney->SetStaticValue(pTitanBreakInfo->dwMoney);

	// Lock Item
	pOrigItem->SetLock(TRUE);

	// Send Message
	MSG_ITEM	msgItem;
	msgItem.Category	= MP_ITEM;
	msgItem.Protocol	= MP_ITEM_TITANBREAK_ADDITEM_SYN;
	msgItem.dwObjectID	= HEROID;
	msgItem.ItemInfo	= *pOrigItem->GetItemBaseInfo();
	NETWORK->Send( &msgItem, sizeof(msgItem) );

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_MunpaWarehouse);

	return FALSE;
}

void CTitanBreakDlg::AddTitanItem(CItem* pItem)
{
	m_VirtualItem.SetData(pItem->GetItemIdx());
	m_VirtualItem.SetLinkItem(pItem);

	if( pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()) )
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, ITEMMGR->GetItemOption(pItem->GetDurability())
			, ITEMMGR->GetItemRareOption(pItem->GetRareness()) );
	}	
	else
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()));

	m_pTitanBreakDlg->AddIcon(0, (cIcon*)&m_VirtualItem);
} 

void CTitanBreakDlg::ViewMaterialList(TITAN_BREAKINFO* pTitanBreakInfo)
{
	if(!pTitanBreakInfo)	return;
	DeleteViewMaterialListAll();

	for( int i = 0; i < pTitanBreakInfo->wTotalCnt; i++ )
	{
		m_ViewShowItem[i].SetData(pTitanBreakInfo->pTitanBreakMaterial[i].dwMaterialIdx);
		m_ViewShowItem[i].Init((WORD)pTitanBreakInfo->pTitanBreakMaterial[i].dwMaterialIdx, pTitanBreakInfo->pTitanBreakMaterial[i].wMaterCnt);

		ITEMMGR->SetToolTipIcon( (cIcon*)&m_ViewShowItem[i], NULL, ITEMMGR->GetItemRareOption(m_VirtualItem.GetRareness()) );

		m_pTitanBreakGridDlg->AddIcon(i, (cIcon*)&m_ViewShowItem[i]);
	}
}

void CTitanBreakDlg::DeleteViewMaterialListAll()
{
	for( int i = 0; i < m_pTitanBreakGridDlg->GetCellNum(); i++ )
	{
		CItemShow* pSItemOut = NULL;
		m_pTitanBreakGridDlg->DeleteIcon(i, (cIcon**)&pSItemOut);
	}

	m_pTitanBreakGridDlg->SetCurSelCellPos(-1);
}

void CTitanBreakDlg::DieCheck()
{
	if( HERO->GetState() == eObjectState_Die )
	{
		Release();
		SetActiveRecursive( FALSE );
	}
}