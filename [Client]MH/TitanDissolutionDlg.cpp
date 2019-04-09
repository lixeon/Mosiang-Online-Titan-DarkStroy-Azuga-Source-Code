// TitanDissolutionDlg.cpp: implementation of the CTitanDissolutionDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TitanDissolutionDlg.h"
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

CTitanDissolutionDlg::CTitanDissolutionDlg()
{
	m_pTitanDissolutionDlg = NULL;

	cImage imgTooltip;
	SCRIPTMGR->GetImage( 63, &imgTooltip, PFT_HARDPATH );

	m_VirtualItem.SetToolTip( "", RGB_HALF(255,255,255), &imgTooltip );
	m_VirtualItem.SetMovable(FALSE);
}

CTitanDissolutionDlg::~CTitanDissolutionDlg()
{

}

void CTitanDissolutionDlg::Release(BOOL bFlag)
{
	// 해당 Icon정보를 삭제
	CVirtualItem * pVItem;
	if(!m_pTitanDissolutionDlg->IsAddable(0))
	{
		m_pTitanDissolutionDlg->DeleteIcon( 0, (cIcon**)&pVItem );
		if( bFlag )	// 서버에 락을 해체해야함
		{
			MSG_WORD msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_TITAN_DISSOLUTION_RELEASEITEM;
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		pVItem->GetLinkItem()->SetLock(FALSE);
		pVItem->SetLinkItem(NULL);
	}
}

void CTitanDissolutionDlg::Linking()
{
	m_pTitanDissolutionDlg = (cIconDialog *)GetWindowForID(TITAN_BONGIN_SLOT);
}

void CTitanDissolutionDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( !val )
	{
		Release(TRUE);

		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}

BOOL CTitanDissolutionDlg::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
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

	if( FALSE == TITANMGR->CheckRecallAvailable() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1553));
		return FALSE;
	}

    DWORD dwTemp[eTitanWearedItem_Max] = { 0, };
	if(0 < TITANMGR->GetTitanEquipTotalInfo(dwTemp))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1514) );
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
	msgItem.Protocol	= MP_ITEM_TITAN_DISSOLUTION_ADDITEM_SYN;
	msgItem.dwObjectID	= HEROID;
	msgItem.ItemInfo	= *pOrigItem->GetItemBaseInfo();
	NETWORK->Send( &msgItem, sizeof(msgItem) );

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_MunpaWarehouse);

	return FALSE;
}

void CTitanDissolutionDlg::AddTitanPaper(CItem* pItem)
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

	m_pTitanDissolutionDlg->AddIcon(0, (cIcon*)&m_VirtualItem);
}

DWORD CTitanDissolutionDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	m_VirtualItem.ActionEvent(mouseInfo);

	return we;
}

BOOL CTitanDissolutionDlg::OnActionEvent( LONG lId, void * p, DWORD we)
{	
	switch( lId )
	{
	case TITAN_BONGIN_OKBTN:	// 확인하기
		{
			CTitanDissolutionDlg* pTitanDissolutionDlg = (CTitanDissolutionDlg*)p;
			CTitanDissolutionDlg::DissolutionOkBtn(pTitanDissolutionDlg);
		}
		break;
	case TITAN_BONGIN_CANCELBTN:
		{
			CTitanDissolutionDlg* pTitanDissolutionDlg = (CTitanDissolutionDlg*)p;
			pTitanDissolutionDlg->SetActive( FALSE );
		}	
	}

	return TRUE;
}

void CTitanDissolutionDlg::DieCheck()
{
	if( HERO->GetState() == eObjectState_Die )
	{
		Release();
		SetActive( FALSE );
	}
}

void CTitanDissolutionDlg::DissolutionOkBtn(CTitanDissolutionDlg * pThis)
{
	// 증서가 들어가있는지 체크
	CVirtualItem* pItem = (CVirtualItem*)pThis->m_pTitanDissolutionDlg->GetIconForIdx(0);
	if(!pItem)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1507) );
		return;
	}

	if( !TITANMGR->GetRegistedTitanItemDBIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1513) );
		return;
	}

	if( TRUE == TITANMGR->IsRegistedTitan(pItem->GetLinkItem()->GetDBIdx()) )
	{
		MSG_DWORD2 msg;
		ZeroMemory(&msg, sizeof(MSG_DWORD2));
		SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_DISSOLUTION_SYN );
		msg.dwObjectID = HEROID;
		msg.dwData1 = pItem->GetLinkItem()->GetItemIdx();
		msg.dwData2 = pItem->GetLinkItem()->GetPosition();

		NETWORK->Send(&msg, sizeof(MSG_DWORD2));
	}
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1536) );
	}
}