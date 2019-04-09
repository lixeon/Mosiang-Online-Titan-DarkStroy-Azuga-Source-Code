///////////////////////////////////////////////////////////////////////////////////////////////
// UniqueItemCurseCancellationDlg.cpp: interface for the CUniqueItemCurseCancellationDlg class.
///////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "UniqueItemCurseCancellationDlg.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "cScriptManager.h"
#include "Item.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "cIconDialog.h"

CUniqueItemCurseCancellationDlg::CUniqueItemCurseCancellationDlg()
{
	m_pUniqueItemSlotDlg = NULL;
	m_wShopItemPos = 0;

	cImage imgTooltip;
	SCRIPTMGR->GetImage(63, &imgTooltip, PFT_HARDPATH);

	m_VirtualItem.SetToolTip("", RGB_HALF(255,255,255), &imgTooltip);
	m_VirtualItem.SetMovable(FALSE);
}

CUniqueItemCurseCancellationDlg::~CUniqueItemCurseCancellationDlg()
{
	
}

void CUniqueItemCurseCancellationDlg::Release( BOOL bFlag )
{
	CVirtualItem* pVItem;
	if(!m_pUniqueItemSlotDlg->IsAddable(0))
	{
		m_pUniqueItemSlotDlg->DeleteIcon(0, (cIcon**)&pVItem);

		if(bFlag)
		{
			MSG_WORD msg;
			SetProtocol(&msg, MP_ITEMEXT, MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_RELEASE);
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		
		pVItem->GetLinkItem()->SetLock(FALSE);
		pVItem->SetLinkItem(NULL);
	}
}

void CUniqueItemCurseCancellationDlg::Linking()
{
	m_pUniqueItemSlotDlg = (cIconDialog*)GetWindowForID(UNIQUEITEMCURSECANCELLATION_ICON);
}

void CUniqueItemCurseCancellationDlg::SetActive( BOOL bFlag )
{
	cDialog::SetActive( bFlag );

	if( !bFlag )
	{
		Release();

		if(HERO->GetState() == eObjectState_Deal)
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}

BOOL CUniqueItemCurseCancellationDlg::FakeMoveIcon(LONG x, LONG y, cIcon* icon)
{
	if(icon->GetType() != WT_ITEM)	return FALSE;
	if(m_bDisable)	return FALSE;

	CItem* pItem = (CItem*)icon;
	if(pItem->IsLocked())
	{
		return FALSE;
	}
	
	// 유니크 아이템인지 체크
    eITEM_KINDBIT bits = pItem->GetItemKind();
	if(bits != eEQUIP_ITEM_UNIQUE)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1625));
		return FALSE;
	}
	
	// 해당 유니크 아이템의 정보가 있는지 체크
	UNIQUE_ITEM_OPTION_INFO* pInfo = GAMERESRCMNGR->GetUniqueItemOptionList(pItem->GetItemIdx());
	if( !pInfo )
	{
		return FALSE;
	}

	// 해당 유니크아이템이 저주를 받았는지 체크
	if(pInfo->dwCurseCancellation == 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1626));
		return FALSE;
	}

	// 이미 들어간 유니크아이템 초기화
	Release();

	// 락걸기
	pItem->SetLock(TRUE);

	// 유니크 아이템 추가
	AddCarseIncantation(pItem);

	// Send Message
	MSG_ITEM	msgItem;
	msgItem.Category	= MP_ITEMEXT;
	msgItem.Protocol	= MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ADDITEM_SYN;
	msgItem.dwObjectID	= HEROID;
	msgItem.ItemInfo	= *pItem->GetItemBaseInfo();
	NETWORK->Send( &msgItem, sizeof(msgItem) );

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

	return FALSE;
}

void CUniqueItemCurseCancellationDlg::AddCarseIncantation(CItem* pItem)
{
	m_VirtualItem.SetData(pItem->GetItemIdx());
	m_VirtualItem.SetLinkItem(pItem);

	if(pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()))
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, ITEMMGR->GetItemOption(pItem->GetDurability()), ITEMMGR->GetItemRareOption(pItem->GetRareness()));
	}
	else
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()));
	}

	m_pUniqueItemSlotDlg->AddIcon(0, (cIcon*)&m_VirtualItem);
}

DWORD CUniqueItemCurseCancellationDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	m_VirtualItem.ActionEvent(mouseInfo);

	return we;
}

void CUniqueItemCurseCancellationDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
    switch(lId)
	{
	case UNIQUEITEMCURSECANCELLATION_OKBTN:
		{
			OnOkBtn();
		}
		break;
	case UNIQUEITEMCURSECANCELLATION_CANCELBTN:
		{
			OnCancelBtn();
		}
		break;
	}
}

void CUniqueItemCurseCancellationDlg::OnOkBtn()
{
	// 슬롯에 아이템이 있는지 체크
	CVirtualItem* pItem = (CVirtualItem*)m_pUniqueItemSlotDlg->GetIconForIdx(0);
	if( !pItem )
		return;

	UNIQUE_ITEM_OPTION_INFO* pInfo = GAMERESRCMNGR->GetUniqueItemOptionList(pItem->GetLinkItem()->GetItemIdx());
	if( !pInfo )
		return;

	if(pInfo->dwCurseCancellation == 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1626));
	}
	
	MSG_DWORD_WORD2 msg;
	SetProtocol(&msg, MP_ITEMEXT, MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_SYN);
	msg.dwObjectID = HEROID;
	msg.dwData = pItem->GetSrcItemIdx();
	msg.wData1 = (WORD)pItem->GetSrcPosition();
	msg.wData2 = (WORD)m_wShopItemPos;
	NETWORK->Send(&msg, sizeof(MSG_DWORD_WORD2));
}

void CUniqueItemCurseCancellationDlg::OnCancelBtn()
{
	SetActive(FALSE);
}