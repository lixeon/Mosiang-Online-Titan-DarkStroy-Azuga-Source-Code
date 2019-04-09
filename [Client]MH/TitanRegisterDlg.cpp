// TitanRegisterDlg.cpp: implementation of the CTitanRegisterDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TitanRegisterDlg.h"
#include "WindowIDEnum.h"
#include "item.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "cIconDialog.h"
#include "cScriptManager.h"
#include "cWindowManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "TitanManager.h"

CTitanRegisterDlg::CTitanRegisterDlg()
{
	m_pTitanRegisterDlg = NULL;

	cImage imgTooltip;
	SCRIPTMGR->GetImage( 63, &imgTooltip, PFT_HARDPATH );

	m_VirtualItem.SetToolTip( "", RGB_HALF(255,255,255), &imgTooltip );
	m_VirtualItem.SetMovable(FALSE);
}

CTitanRegisterDlg::~CTitanRegisterDlg()
{

}

void CTitanRegisterDlg::Release(BOOL bFlag)
{
	// 해당 Icon정보를 삭제
	CVirtualItem * pVItem;
	if(!m_pTitanRegisterDlg->IsAddable(0))
	{
		m_pTitanRegisterDlg->DeleteIcon( 0, (cIcon**)&pVItem );
		if( bFlag )	// 서버에 락을 해체해야함
		{
			MSG_WORD msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_TITAN_REGISTER_RELEASEITEM;
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		pVItem->GetLinkItem()->SetLock(FALSE);
		pVItem->SetLinkItem(NULL);
	}
}

void CTitanRegisterDlg::Linking()
{
	m_pTitanRegisterDlg = (cIconDialog *)GetWindowForID(TITAN_USE_SLOT);
}

void CTitanRegisterDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( !val )
	{
		Release();

		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}

BOOL CTitanRegisterDlg::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if(pOrigIcon->GetType() != WT_ITEM)	return FALSE;
	if( m_bDisable ) return FALSE;

	CItem* pOrigItem = (CItem*)pOrigIcon;
	if( pOrigItem->IsLocked() )	return FALSE;

	// 옵션 아이템이면 FALSE
	if( ITEMMGR->IsOptionItem(pOrigItem->GetItemIdx(), pOrigItem->GetDurability()) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1509) );
		return FALSE;
	}

	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if(bits != eTITAN_ITEM_PAPER)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1509) );
		return FALSE;
	}

	if( TITANMGR->GetRegistedTitanItemDBIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1508) );
		return FALSE;
	}

	if( FALSE != TITANMGR->IsRegistedTitan(pOrigItem->GetDBIdx()) ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1544) );
		return FALSE;
	}

	Release();

	// 타이탄 증서 넣음
	AddTitanPaper(pOrigItem);

	// Lock Item
	pOrigItem->SetLock(TRUE);

	// Send Message
	MSG_ITEM	msgItem;
	msgItem.Category	= MP_ITEM;
	msgItem.Protocol	= MP_ITEM_TITAN_REGISTER_ADDITEM_SYN;
	msgItem.dwObjectID	= HEROID;
	msgItem.ItemInfo	= *pOrigItem->GetItemBaseInfo();
	NETWORK->Send( &msgItem, sizeof(msgItem) );

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_MunpaWarehouse);

	return FALSE;
}

void CTitanRegisterDlg::AddTitanPaper(CItem* pItem)
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

	m_pTitanRegisterDlg->AddIcon(0, (cIcon*)&m_VirtualItem);
}

DWORD CTitanRegisterDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	m_VirtualItem.ActionEvent(mouseInfo);

	return we;
}

BOOL CTitanRegisterDlg::OnActionEvent( LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case TITAN_USE_OKBTN:	// 확인하기
		{
			CTitanRegisterDlg* pTitanRegisterDlg = (CTitanRegisterDlg*)p;
			CTitanRegisterDlg::RegisterOkBtn(pTitanRegisterDlg);
		}
		break;
	case TITAN_USE_CANCELBTN:
		{
			CTitanRegisterDlg* pTitanRegisterDlg = (CTitanRegisterDlg*)p;
			
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

			pTitanRegisterDlg->SetActive( FALSE );
		}
	}

	return TRUE;
}

void CTitanRegisterDlg::RegisterOkBtn(CTitanRegisterDlg * pThis)
{
	// 증서가 들어가있는지 체크
	CVirtualItem* pItem = (CVirtualItem*)pThis->m_pTitanRegisterDlg->GetIconForIdx(0);
	if(!pItem)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1507) );
		return;
	}

	if( TITANMGR->GetRegistedTitanItemDBIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1508) );
		return;
	}

	if( FALSE == TITANMGR->IsRegistedTitan(pItem->GetLinkItem()->GetDBIdx()) ) 
	{
		MSG_DWORD2 msg;
		ZeroMemory(&msg, sizeof(MSG_DWORD2));
		SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_REGISTER_SYN );
		msg.dwObjectID = HEROID;
		msg.dwData1 = pItem->GetLinkItem()->GetItemIdx();
		msg.dwData2 = pItem->GetLinkItem()->GetPosition();

		NETWORK->Send(&msg, sizeof(MSG_DWORD2));
	}
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1544) );
	}
}

void CTitanRegisterDlg::DieCheck()
{
	if( HERO->GetState() == eObjectState_Die )
	{
		Release();
		SetActive( FALSE );
	}
}