// UpgradeDlg.cpp: implementation of the CUpgradeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UpgradeDlg.h"
#include "Item.h"
#include "INTERFACE\cIconDialog.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "./Interface/cScriptManager.h"
#include "ItemManager.h"
#include "./Interface/cWindowManager.h"
#include "ObjectManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUpgradeDlg::CUpgradeDlg()
{
	m_pUpgradeDlg = NULL;

	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	for(int i = 0 ; i < eMAXITEM_POS ; ++i)
	{
		m_VirtualItem[i].SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
		m_VirtualItem[i].SetMovable(FALSE);
	}

}

CUpgradeDlg::~CUpgradeDlg()
{
	// don't call Release() 
	// pre delete item 
	m_pUpgradeDlg = NULL;
}
void CUpgradeDlg::Release(eReleaseOpt op)
{
	// 기본 아이템 링크
	// 비급서 아이템 링크
	// 결과 아이템 링크
	CVirtualItem * pVItem;
	if(m_pUpgradeDlg)
	{
		if(!m_pUpgradeDlg->IsAddable(eBASICITEM_POS) && (op & eBasicRelease) )
		{
			m_pUpgradeDlg->DeleteIcon(eBASICITEM_POS, (cIcon **)&pVItem);
			m_VirtualItem[eBASICITEM_POS].GetLinkItem()->SetLock(FALSE);
			m_VirtualItem[eBASICITEM_POS].SetLinkItem(NULL);

			//pVItem->SetLinkItem(NULL);
			//pVItem->GetLinkItem()->SetLock(FALSE);
			//pVItem->SetLinkItem(NULL);
		}

		if(!m_pUpgradeDlg->IsAddable(eUPGRADEITEM_POS) && (op & eUpgradeRelease) )
		{
			m_pUpgradeDlg->DeleteIcon(eUPGRADEITEM_POS, (cIcon **)&pVItem);
			m_VirtualItem[eUPGRADEITEM_POS].GetLinkItem()->SetLock(FALSE);
			m_VirtualItem[eUPGRADEITEM_POS].SetLinkItem(NULL);
			//pVItem->GetLinkItem()->SetLock(FALSE);
			//pVItem->SetLinkItem(NULL);
		}

		if(!m_pUpgradeDlg->IsAddable(eRESULTITEM_POS) && (op & eResultRelease) )
		{
			m_pUpgradeDlg->DeleteIcon(eRESULTITEM_POS, (cIcon **)&pVItem);
			m_VirtualItem[eRESULTITEM_POS].GetLinkItem()->SetLock(FALSE);
			m_VirtualItem[eRESULTITEM_POS].SetLinkItem(NULL);
			//pVItem->GetLinkItem()->SetLock(FALSE);
			//pVItem->SetLinkItem(NULL);
		}
	}

}
void CUpgradeDlg::Linking()
{
	m_pUpgradeDlg = (cIconDialog *)GetWindowForID(ITD_UPGRADEGRID);
}

BOOL CUpgradeDlg::CanBasicEquipItem(CItem * pBasicItem)
{
	CVirtualItem * pUpgradeItem = (CVirtualItem *)m_pUpgradeDlg->GetIconForIdx(eUPGRADEITEM_POS);
	if(pUpgradeItem)
	{
		eITEM_KINDBIT upbits = pUpgradeItem->GetSrcItemKind();
		eITEM_KINDBIT bits = pBasicItem->GetItemKind();
		if( upbits == eEXTRA_ITEM_UPGRADE_ATTACK)
		{
			// 무기
			if((bits >= eEQUIP_ITEM_WEAPON && bits < eEQUIP_ITEM_WEAPON+6))
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else if( upbits == eEXTRA_ITEM_UPGRADE_DEFENSE)
		{
			// 방어구 
			if(bits == eEQUIP_ITEM_DRESS ||
				bits == eEQUIP_ITEM_HAT ||
				bits == eEQUIP_ITEM_SHOES)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	else
	{
		if(!m_pUpgradeDlg->IsAddable(eBASICITEM_POS)) 
			return FALSE;	
	}
	return TRUE;
}

BOOL CUpgradeDlg::CanUpgradeEquipItem(CItem * pUpgradeItem)
{
	CVirtualItem * pBasicItem = (CVirtualItem *)m_pUpgradeDlg->GetIconForIdx(eBASICITEM_POS);
	if(pBasicItem)
	{
		eITEM_KINDBIT upbits = pUpgradeItem->GetItemKind();
		eITEM_KINDBIT bits = pBasicItem->GetSrcItemKind();
		if((bits >= eEQUIP_ITEM_WEAPON && bits < eEQUIP_ITEM_WEAPON+6))
		{
			if(upbits == eEXTRA_ITEM_UPGRADE_ATTACK)
				return TRUE;
			else 
				return FALSE;
		}
		else if(bits == eEQUIP_ITEM_DRESS ||
				bits == eEQUIP_ITEM_HAT ||
				bits == eEQUIP_ITEM_SHOES)
		{
			if(upbits == eEXTRA_ITEM_UPGRADE_DEFENSE)
				return TRUE;
			else 
				return FALSE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{

	}
	return TRUE;
}

eErrorUpgrade CUpgradeDlg::CanUpgradeItem(CItem * pBasicItem)
{
	eITEM_KINDBIT bits = pBasicItem->GetItemKind();
	if(bits & eEQUIP_ITEM)
	{
		if(CanBasicEquipItem(pBasicItem))
			return eErrorUpgrade_NoError;
		else
			return eErrorUpgrade_OnlyBasicItem;
	}
	else if(bits & eEXTRA_ITEM_UPGRADE)
	{
		if(CanUpgradeEquipItem(pBasicItem))
			return eErrorUpgrade_NoError;
		else
			return eErrorUpgrade_OnlyExtraItem;
	}
	return eErrorUpgrade_UnKnownError;
}


BOOL CUpgradeDlg::AddVirtualItem(POSTYPE relPos, CVirtualItem * pItem)
{
	//empty 체크후 
	if(m_pUpgradeDlg->IsAddable(relPos))
	{
		m_pUpgradeDlg->AddIcon(relPos, (cIcon *)pItem);
		return TRUE;
	}
	return FALSE;
}
DWORD CUpgradeDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	for(int i = 0 ; i < eMAXITEM_POS ; ++i)
	{
		m_VirtualItem[i].ActionEvent(mouseInfo);
	}

	return we;
}

BOOL CUpgradeDlg::AddVirtualItemWrap(POSTYPE relPos, CItem * pItem)
{
	m_VirtualItem[relPos].SetData(pItem->GetItemIdx());
	m_VirtualItem[relPos].SetLinkItem(pItem);

//	if(pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()))
//		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[relPos], ITEMMGR->GetItemOption(pItem->GetDurability()));
	//SW050920 Rare
	if( (pItem->GetDurability() != 0 || pItem->GetRareness() != 0) 
		&& !ITEMMGR->IsDupItem(pItem->GetItemIdx()))
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[relPos], ITEMMGR->GetItemOption(pItem->GetDurability())
			, ITEMMGR->GetItemRareOption(pItem->GetRareness()) );
	}
	else
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[relPos]);
	return AddVirtualItem(relPos, &m_VirtualItem[relPos]);
}

BOOL CUpgradeDlg::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if(pOrigIcon->GetType() != WT_ITEM) return FALSE;
	if( m_bDisable ) return FALSE;

	CItem * pOrigItem = (CItem *)pOrigIcon;
	if( pOrigItem->IsLocked() ) return FALSE;

	// 인벤토리이외에 아이템 FALSE
	if(!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 620 ) );
		return FALSE;
	}

	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	
	if(bits & eEQUIP_ITEM)
	{
		if(!CanBasicEquipItem(pOrigItem))
		{
			//비급서에 맞지 않는 무기/방어구는 등록 불가능
			//CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(170) );
			//return FALSE;	
			Release();

		}
		else
		{
			if(!m_pUpgradeDlg->IsAddable(eRESULTITEM_POS) )
			{
				Release();
			}
			else if(!m_pUpgradeDlg->IsAddable(eBASICITEM_POS) )
			{
				Release(eBasicRelease);
			}
		}
		
		AddVirtualItemWrap(eBASICITEM_POS, pOrigItem);
	}
	else if(bits & eEXTRA_ITEM_UPGRADE)
	{
		if(!CanUpgradeEquipItem(pOrigItem))
		{
			//무기/방어구에 맞지 않는 비급서
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(171) );
			return FALSE;
		}

		if(!m_pUpgradeDlg->IsAddable(eRESULTITEM_POS) )
		{
			Release();
		}
		else if(!m_pUpgradeDlg->IsAddable(eUPGRADEITEM_POS) )
		{
			Release(eUpgradeRelease);
		}

		AddVirtualItemWrap(eUPGRADEITEM_POS, pOrigItem);
	}
	else
	{
		//등급업에 사용할 수 없는 아이템
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(172) );
		return FALSE;
	}
	

/*
	switch(CanUpgradeItem(pOrigItem))
	{
	case eErrorUpgrade_OnlyBasicItem:
		{
			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(170) );
			return FALSE;
		}
		break;
	case eErrorUpgrade_OnlyExtraItem:
		{
			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(173) );
			return FALSE;
		}
		break;
	case eErrorUpgrade_UnKnownError:
		{
			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(170) );
			return FALSE;
		}
		break;
	}

	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if(bits & eEQUIP_ITEM)
	{
		AddVirtualItemWrap(eBASICITEM_POS, pOrigItem);
		//m_VirtualItem[eBASICITEM_POS].SetLinkItem(pOrigItem);
		//AddVirtualItem(eBASICITEM_POS, &m_VirtualItem[eBASICITEM_POS]);
	}
	else if(bits & eEXTRA_ITEM_UPGRADE)
	{
		AddVirtualItemWrap(eUPGRADEITEM_POS, pOrigItem);
		//m_VirtualItem[eUPGRADEITEM_POS].SetLinkItem(pOrigItem);
		//AddVirtualItem(eUPGRADEITEM_POS, &m_VirtualItem[eUPGRADEITEM_POS]);
	}
	*/
	
	//LOCK ITEM
	pOrigItem->SetLock(TRUE);
	
	return FALSE;
}
BOOL CUpgradeDlg::IsUpgrade()
{
	if(m_pUpgradeDlg->IsAddable(eBASICITEM_POS))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 619 ) );
		return FALSE;
	}
	if(m_pUpgradeDlg->IsAddable(eUPGRADEITEM_POS))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 620 ) );
		return FALSE;
	}
	return TRUE;
}

CVirtualItem * CUpgradeDlg::GetVirtualItem(POSTYPE relPos)
{
	return (CVirtualItem *)m_pUpgradeDlg->GetIconForIdx(relPos);
}
BOOL CUpgradeDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(we)
	{
	case WE_CLOSEWINDOW:
		{
//			CUpgradeDlg * pUpdradeDlg = (CUpgradeDlg*)p;	//PPP
			CUpgradeDlg * pUpdradeDlg = (CUpgradeDlg*)WINDOWMGR->GetWindowForID(lId);
			pUpdradeDlg->Release();
			return TRUE;
		}
		break;
	}
	
	switch( lId )
	{
	case ITD_UPGRADE_OKBTN:
		{
			CUpgradeDlg * pUpdradeDlg = (CUpgradeDlg*)p;
			CUpgradeDlg::UpgradeOkBtn(pUpdradeDlg);
		}
		break;
	case ITD_UPGRADE_CANCELBTN:
		{
			CUpgradeDlg * pUpdradeDlg = (CUpgradeDlg*)p;
			CUpgradeDlg::UpgradeCancelBtn(pUpdradeDlg);
		}
		break;
	}
	
	return TRUE;
}
void CUpgradeDlg::UpgradeOkBtn(CUpgradeDlg * pThis)
{
	if(!pThis->IsUpgrade()) return;

	CVirtualItem * pTargetItem = pThis->GetVirtualItem(eBASICITEM_POS);
	CVirtualItem * pMatItem = pThis->GetVirtualItem(eUPGRADEITEM_POS);

	MSG_ITEM_UPGRADE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_UPGRADE_SYN;
	msg.dwObjectID			= HEROID;
	msg.ItemPos				= pTargetItem->GetSrcPosition();
	msg.wItemIdx			= pTargetItem->GetSrcItemIdx();
	msg.MaterialItemPos		= pMatItem->GetSrcPosition();
	msg.wMaterialItemIdx	= pMatItem->GetSrcItemIdx();


	NETWORK->Send(&msg, sizeof(msg));
}

void CUpgradeDlg::UpgradeCancelBtn(CUpgradeDlg * pThis)
{
	// basic item delete
	// upgrade item delete
	
	pThis->Release();
	pThis->SetActiveRecursive(FALSE);
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(175) );
}

void CUpgradeDlg::DieCheck()
{
	if( HERO->GetState() == eObjectState_Die )
	{
		Release();
		SetActiveRecursive( FALSE );
	}
}