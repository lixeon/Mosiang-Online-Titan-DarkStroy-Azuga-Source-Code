// MixDialog.cpp: implementation of the CMixDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "MixDialog.h"
#include "ChatManager.h"
#include "MixManager.h"
#include "Item.h"
#include "INTERFACE\cIconDialog.h"
#include "INTERFACE\cIconGridDialog.h"
#include "WindowIDEnum.h"
#include "ItemShow.h"
#include "Gamein.h"
#include "ItemManager.h"
#include "InventoryExDialog.h"
#include "interface/cStatic.h"
#include "./Interface/cScriptManager.h"
#include "ObjectManager.h"
#include "./Interface/cWindowManager.h"
#include "objectstatemanager.h"
#include "StreetStallManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMixDialog::CMixDialog()
{
	m_pMixDlg = NULL;
	m_pMixGridDlg = NULL;
	
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	for(int i = 0 ; i < eMixPos_Max ; ++i)
	{
		m_VirtualItem[i].SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
		m_VirtualItem[i].SetMovable(FALSE);
	}
	for(i = 0 ; i < MAX_MIXGRID_NUM ; ++i)
	{
		m_ViewShowItem[i].SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
		m_ViewShowItem[i].SetMovable(FALSE);
	}
	m_bShopItemUse	= FALSE;
	m_ShopItemIdx	= 0;
	m_ShopItemPos	= 0;
	m_pTitle		= NULL;
	memset( m_Title, 0, sizeof(char)*128 );
}

CMixDialog::~CMixDialog()
{
	// don't call Release() 
	// pre delete item 
}
void CMixDialog::Release(eMixReleaseOpt op, BOOL bMsg)
{
	ResetCurSelCellPos();

	CVirtualItem * pVItem;

	if(!m_pMixDlg->IsAddable(eMixPos_Basic) && (op & eMixBasicRelease) )
	{
		m_pMixDlg->DeleteIcon(eMixPos_Basic, (cIcon **)&pVItem);
		pVItem->GetLinkItem()->SetLock(FALSE);
		if( bMsg )
		{
			///서버에도 보내줘야지!!!! KES
			MSG_WORD msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_MIX_RELEASEITEM;
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		pVItem->SetLinkItem(NULL);
		DeleteViewResultItemAll();
		m_pMaterialsStatic->SetStaticText(" ");
	}
	if(!m_pMixDlg->IsAddable(eMixPos_Result) && (op & eMixResultRelease) )
	{
		m_pMixDlg->DeleteIcon(eMixPos_Result, (cIcon **)&pVItem);
		pVItem->GetLinkItem()->SetLock(FALSE);
		if( bMsg )
		{
			///서버에도 보내줘야지!!!! KES
			MSG_WORD msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_MIX_RELEASEITEM;
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}

		pVItem->SetLinkItem(NULL);
	}
}

void CMixDialog::SetActiveRecursive(BOOL val)
{
	if( (STREETSTALLMGR->GetStallKind() == eSK_BUY && OBJECTSTATEMGR->GetObjectState(HERO) == eObjectState_StreetStall_Owner) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(798) );
		return;
	}

	cDialog::SetActiveRecursive( val );

	if( val )
	{
		if( m_bShopItemUse )
		{
			char buf [256];
			sprintf( buf, "%s - (%s)", m_Title, CHATMGR->GetChatMsg( 1092) );
			m_pTitle->SetStaticText( buf );
		}
		else
			m_pTitle->SetStaticText( m_Title );
	}

	if( !val && m_bShopItemUse )
	{
		CItem* pItem = ITEMMGR->GetItemofTable( eItemTable_ShopInven, (POSTYPE)m_ShopItemPos );
		if( pItem )
			pItem->SetLock( FALSE );
		m_bShopItemUse	= FALSE;
		m_ShopItemIdx	= 0;
		m_ShopItemPos	= 0;
	}
}

BOOL CMixDialog::IsEmpty(eMixPos pos)
{
	if( pos >= eMixPos_Max )
		return FALSE;
	return m_pMixDlg->IsAddable(pos);
}
void CMixDialog::Linking()
{
	m_pMaterialsStatic = (cStatic *)GetWindowForID(ITMD_MIXMATERIALS);
	m_pMaterialsStatic->SetMultiLine(TRUE);
	m_pMaterialsStatic->InitMultiLine();
	m_pMixDlg = (cIconDialog *)GetWindowForID(ITMD_MIXICONDLG);
	m_pMixGridDlg = (cIconGridDialog *)GetWindowForID(ITMD_MIXICONGRIDDLG);
	m_pTitle = (cStatic*)GetWindowForID(ITMD_MIXICONTITLE);
	
	if( m_pTitle )
		strcpy( m_Title, m_pTitle->GetStaticText() );
	
	m_bMixing = FALSE;
}

BOOL CMixDialog::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if(pOrigIcon->GetType() != WT_ITEM) return FALSE;
	if( m_bDisable ) return FALSE;

	CItem * pOrigItem = (CItem *)pOrigIcon;
	if( pOrigItem->IsLocked() ) return FALSE;
	
	// 인벤토리이외에 아이템 FALSE
	if(!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 612 ) );
		return FALSE;
	}
	
	// 옵션 아이템 안됨
	if(ITEMMGR->IsOptionItem(pOrigItem->GetItemIdx(), pOrigItem->GetDurability()))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
		return FALSE;
	}
	
//	//!!!임시 SW051007 레어 아이템 안됨
//	if(ITEMMGR->IsRareOptionItem(pOrigItem->GetItemIdx(), pOrigItem->GetRareness()))
//	{
//		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
//		return FALSE;
//	}

	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if( !(bits & eYOUNGYAK_ITEM) && !(bits & eMUGONG_ITEM)
		&& !(bits & eEQUIP_ITEM) && !(bits & eEXTRA_ITEM) 
		&& !(bits & eCHANGE_ITEM) && !(bits & eQUEST_ITEM_START) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
		return FALSE;
	}

	// magi82 - UniqueItem(070627)
	// 유니크아이템이고 조합이 불가능한 유니크라면..
	if(bits == eEQUIP_ITEM_UNIQUE && GAMERESRCMNGR->GetUniqueItemOptionList(pOrigItem->GetItemIdx())->MixFlag == 0)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1625) );
		return FALSE;
	}

	// view list
	ITEM_MIX_INFO * pMixItemInfo = MIXMGR->GetMixItemInfo(pOrigItem->GetItemIdx());
	if(!pMixItemInfo)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
		return FALSE;
	}

	//영약아이템이 아닌 재료아이템일경우 하나밖에 안올라간다.
	if( GetItemKind(pOrigItem->GetItemIdx() ) != eYOUNGYAK_ITEM && 
		GetItemKind(pOrigItem->GetItemIdx() ) != eEXTRA_ITEM_JEWEL && 
		pOrigItem->GetDurability() > 1 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(629) );
		return FALSE;
	}

	if( (bits & eEQUIP_ITEM) && m_bShopItemUse)
	{
		ITEM_INFO* pShopItem = ITEMMGR->GetItemInfo( (WORD)m_ShopItemIdx );
		if( !pShopItem )				return FALSE;
		if( !(pShopItem->ItemKind & eSHOP_ITEM) )		return FALSE;
		ITEM_INFO* pBaseItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx() );
		if( !pBaseItemInfo )			return FALSE;
		if( pBaseItemInfo->LimitLevel < pShopItem->GenGol || pBaseItemInfo->LimitLevel > pShopItem->MinChub )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
			return FALSE;
		}
	}

	//LOCK ITEM
	pOrigItem->SetLock(TRUE);///서버에도 락을 걸어야지!!!!!! KES

//---여기서 메세지보내고
	MSG_ITEM	msgItem;
	msgItem.Category	= MP_ITEM;
	msgItem.Protocol	= MP_ITEM_MIX_ADDITEM_SYN;
	msgItem.dwObjectID	= HEROID;
	msgItem.ItemInfo	= *pOrigItem->GetItemBaseInfo();
	NETWORK->Send( &msgItem, sizeof(msgItem) );

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_MunpaWarehouse);

	if(!IsEmpty(eMixPos_Basic))
	{
		Release(eMixBasicRelease);
	}
	Release(eMixResultRelease);

	//add Item
	AddVirtualItemWrap(eMixPos_Basic, pOrigItem);
	//m_VirtualItem[eMixPos_Basic].SetLinkItem(pOrigItem);
	//m_pMixDlg->AddIcon(eMixPos_Basic, (cIcon*)&m_VirtualItem[eMixPos_Basic]);


	ViewResultItemList(pMixItemInfo);

	m_pMaterialsStatic->SetStaticText( CHATMGR->GetChatMsg(613) );

	return FALSE;
}
void CMixDialog::ViewMaterialsDesc(ITEM_MIX_RES * pResultItem)
{
	char line[64] = {0,};
	char szDescText[384] = {0,};
	CVirtualItem * pBasicItem = (CVirtualItem *)m_pMixDlg->GetIconForIdx(eMixPos_Basic);

	if(pBasicItem->GetSrcDurability() >1 )
	{		
		/*
#ifndef TAIWAN_LOCAL
		wsprintf( szDescText, "%s × %d^n^n", ITEMMGR->GetItemInfo(pResultItem->wResItemIdx)->ItemName, pBasicItem->GetSrcDurability() );
#else
//		wsprintf( szDescText, "%s X %d^n^n", pResultItem->szResItemName, pBasicItem->GetSrcDurability() );
		wsprintf( szDescText, "%s X %d^n^n", ITEMMGR->GetItemInfo(pResultItem->wResItemIdx)->ItemName, pBasicItem->GetSrcDurability() );
#endif
*/
#ifdef TAIWAN_LOCAL
		wsprintf( szDescText, "%s X %d^n^n", ITEMMGR->GetItemInfo(pResultItem->wResItemIdx)->ItemName, pBasicItem->GetSrcDurability() );
#elif defined _JAPAN_LOCAL_
		wsprintf( szDescText, "%s X %d^n^n", ITEMMGR->GetItemInfo(pResultItem->wResItemIdx)->ItemName, pBasicItem->GetSrcDurability() );
#elif defined _HK_LOCAL_
		wsprintf( szDescText, "%s X %d^n^n", ITEMMGR->GetItemInfo(pResultItem->wResItemIdx)->ItemName, pBasicItem->GetSrcDurability() );
#elif defined _TL_LOCAL_
		wsprintf( szDescText, "%s X %d^n^n", ITEMMGR->GetItemInfo(pResultItem->wResItemIdx)->ItemName, pBasicItem->GetSrcDurability() );
#else
		wsprintf( szDescText, "%s × %d^n^n", ITEMMGR->GetItemInfo(pResultItem->wResItemIdx)->ItemName, pBasicItem->GetSrcDurability() );
#endif
	}
	else
		wsprintf( szDescText, "%s^n^n", ITEMMGR->GetItemInfo(pResultItem->wResItemIdx)->ItemName );
//	SafeStrCpy(szDescText, pResultItem->szResItemName, MAX_NAME_LENGTH);
//	strcat(szDescText, "^n^n");

	for(int i = 0 ; i < pResultItem->wMaterialItemNum ; ++i)
	{
//		wsprintf(line, CHATMGR->GetChatMsg(228), pResultItem->psMaterialItemInfo[i].szMatItemName, pResultItem->psMaterialItemInfo[i].wMatItemNum);
		wsprintf(line, CHATMGR->GetChatMsg(228), ITEMMGR->GetItemInfo(pResultItem->psMaterialItemInfo[i].wMatItemIdx)->ItemName, pResultItem->psMaterialItemInfo[i].wMatItemNum);
		strcat(szDescText, line);
		if(pBasicItem->GetSrcDurability() >1 )
		{
#ifdef TAIWAN_LOCAL
			wsprintf(line, " X %d", pBasicItem->GetSrcDurability());
#elif defined _JAPAN_LOCAL_
			wsprintf(line, " X %d", pBasicItem->GetSrcDurability());
#elif defined _HK_LOCAL_
			wsprintf(line, " X %d", pBasicItem->GetSrcDurability());
#elif defined _TL_LOCAL_
			wsprintf(line, " X %d", pBasicItem->GetSrcDurability());
#else
			wsprintf(line, " × %d", pBasicItem->GetSrcDurability());
			
#endif
			strcat(szDescText, line);
		}
		strcat(szDescText, "^n");
	}
	DWORD dwMoney;
	if( ITEMMGR->IsDupItem( pBasicItem->GetSrcItemIdx() ) )
	{
		dwMoney = pResultItem->Money * pBasicItem->GetSrcDurability();
	}
	else
	{
		dwMoney = pResultItem->Money;
	}

	wsprintf(line, CHATMGR->GetChatMsg(229), AddComma( dwMoney ) );
	strcat(szDescText, line);

	m_pMaterialsStatic->SetStaticText(szDescText);
}

void CMixDialog::ViewResultItemList(ITEM_MIX_INFO * pMixItemInfo)
{
	if(!pMixItemInfo) return;
	DeleteViewResultItemAll();
	
	DURTYPE Dur = m_VirtualItem[eMixPos_Basic].GetLinkItem()->GetDurability();
	//char ToolTipText[25];
	for(int i = 0 ; i < pMixItemInfo->wResultItemNum ; ++i)
	{
		//CItemShow * pSItem = MIXMGR->NewShowItem();
		m_ViewShowItem[i].SetData(pMixItemInfo->psResultItemInfo[i].wResItemIdx);
		m_ViewShowItem[i].Init(pMixItemInfo->psResultItemInfo[i].wResItemIdx, Dur);
		//wsprintf(ToolTipText, "[%s]", pMixItemInfo->psResultItemInfo[i].szResItemName);
		//m_ViewShowItem[i].SetToolTip( ToolTipText, RGB_HALF( 255, 255, 255) );
		
		//ITEMMGR->SetToolTipIcon((cIcon*)&m_ViewShowItem[i]);
		//SW051012 레어 조합
		ITEMMGR->SetToolTipIcon( (cIcon*)&m_ViewShowItem[i], NULL, ITEMMGR->GetItemRareOption(m_VirtualItem[eMixPos_Basic].GetRareness()) );

		m_pMixGridDlg->AddIcon(i, (cIcon*)&m_ViewShowItem[i]);
	}
}

void CMixDialog::DeleteViewResultItemAll()
{
	for(int i = 0 ; i < m_pMixGridDlg->GetCellNum() ; ++i )
	{
		CItemShow * pSItemOut = NULL;
		if(m_pMixGridDlg->DeleteIcon(i, (cIcon**)&pSItemOut))
		{
		//	MIXMGR->DeleteShowItem(pSItemOut);
		}
	}
}
BOOL CMixDialog::AddVirtualItemWrap(POSTYPE relPos, CItem * pItem)
{
	m_VirtualItem[relPos].SetData(pItem->GetItemIdx());
	m_VirtualItem[relPos].SetLinkItem(pItem);
	//char ToolTipText[25];
	//wsprintf(ToolTipText, "[%s]", pItem->GetItemInfo()->ItemName );
	//m_VirtualItem[relPos].SetToolTip( ToolTipText, RGB_HALF( 255, 255, 255) );
//	if(pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()))
//		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[relPos], ITEMMGR->GetItemOption(pItem->GetDurability()));
	//!!!NULL 확인 SW050920 Rare
	if( pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()) )
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[relPos], ITEMMGR->GetItemOption(pItem->GetDurability())
			, ITEMMGR->GetItemRareOption(pItem->GetRareness()) );
	}	
	else
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[relPos], NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()));
	return AddVirtualItem(relPos, &m_VirtualItem[relPos]);
}
BOOL CMixDialog::AddVirtualItem(POSTYPE relPos, CVirtualItem * pItem)
{
	//empty 체크후 
	if(m_pMixDlg->IsAddable(relPos))
	{
		m_pMixDlg->AddIcon(relPos, (cIcon *)pItem);
		return TRUE;
	}
	return FALSE;
}
CVirtualItem * CMixDialog::GetVirtualItem(POSTYPE relPos)
{
	return (CVirtualItem *)m_pMixDlg->GetIconForIdx(relPos);
}

void CMixDialog::MixOkBtn(CMixDialog * pThis)
{
	//조합을 누르고 결과를 기다리는 중
	if( pThis->IsNowMixing() == TRUE )
		return;

	// 기본 아이템 존재?
	CVirtualItem * pBasicItem = (CVirtualItem *)pThis->m_pMixDlg->GetIconForIdx(eMixPos_Basic);
	if(NULL == pBasicItem)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(618) );
		return ;
	}

	// 결과선택 ok?
	if(-1 == pThis->m_pMixGridDlg->GetCurSelCellPos())
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(203) );
		return;
	}
	CItemShow * pSelItemShow = (CItemShow *)pThis->m_pMixGridDlg->GetIconForIdx((WORD)pThis->m_pMixGridDlg->GetCurSelCellPos());
	if(NULL == pSelItemShow)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(203) );
		return ;
	}

	ITEM_MIX_INFO * pMixInfo = MIXMGR->GetMixItemInfo(pBasicItem->GetSrcItemIdx());
	for(int i = 0 ; i < pMixInfo->wResultItemNum ; ++i)
	{
		if(pMixInfo->psResultItemInfo[i].wResItemIdx == pSelItemShow->GetItemIdx())
		{
			MSG_ITEM_MIX_SYN msg;
			msg.Init();
			msg.Category			= MP_ITEM;
			msg.Protocol			= MP_ITEM_MIX_SYN;
			msg.dwObjectID			= HEROID;
			msg.wBasicItemIdx		= pBasicItem->GetSrcItemIdx();
			msg.BasicItemPos		= pBasicItem->GetSrcPosition();

//			const ITEMBASE* pItembase	= ITEMMGR->GetItemInfoAbsIn(HERO, pBasicItem->GetSrcPosition());
//			msg.dwRareIdx			= pItembase->RareIdx;
			msg.dwRareIdx			= pBasicItem->GetRareness();
			msg.wResultItemIdx		= pSelItemShow->GetItemIdx();	
			msg.ResultIndex			= i;
			if( pThis->m_bShopItemUse )
			{
				msg.ShopItemIdx = (WORD)pThis->m_ShopItemIdx;
				msg.ShopItemPos = (WORD)pThis->m_ShopItemPos;
			}

			// 필요한 돈은 있는가?
			DWORD dwMoney;
			if(ITEMMGR->IsDupItem(pBasicItem->GetSrcItemIdx()))
			{
				dwMoney = pMixInfo->psResultItemInfo[i].Money * pBasicItem->GetSrcDurability();
			}
			else
			{
				dwMoney = pMixInfo->psResultItemInfo[i].Money;
			}

			if( dwMoney > HERO->GetMoney() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
				return ;
			}
				

			// 재료 enough?
//			MATERIAL_ARRAY ItemOut[MAX_MIXGRID_NUM]; //버그!!!
			MATERIAL_ARRAY ItemOut[TP_INVENTORY_END-TP_INVENTORY_START];
			
			WORD ItemPosNum;
			WORD needItemIdx;
			DURTYPE needItemNum;


			for(int j = 0 ; j < pMixInfo->psResultItemInfo[i].wMaterialItemNum ; ++j)
			{
				needItemIdx = pMixInfo->psResultItemInfo[i].psMaterialItemInfo[j].wMatItemIdx;
				needItemNum = pMixInfo->psResultItemInfo[i].psMaterialItemInfo[j].wMatItemNum;
				if(ITEMMGR->IsDupItem(pBasicItem->GetSrcItemIdx()))
					needItemNum *= pBasicItem->GetSrcDurability();
				if(!pThis->EnoughMaterial(needItemIdx, (WORD)needItemNum, ItemOut, ItemPosNum))
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(205) );
					return;
				}

				for(int k = 0 ; k < ItemPosNum ; ++k)
				{
					if(ITEMMGR->IsDupItem(needItemIdx))
						msg.AddMaterial(needItemIdx, ItemOut[k].ItemPos, ItemOut[k].Dur);
					else
						msg.AddMaterial(needItemIdx, ItemOut[k].ItemPos, 0);
				}	
			}
			
			NETWORK->Send(&msg, msg.GetSize());
			pThis->SetNowMixing( TRUE );

			return;
		}
	}
}
BOOL CMixDialog::EnoughMaterial(WORD wItemIdx, WORD ItemNum, MATERIAL_ARRAY * ItemOut, WORD & ItemPosNum)
{
	ItemPosNum = 0;

	DURTYPE CurRemainNum = ItemNum;

	for(int i = TP_INVENTORY_START ; i < TP_INVENTORY_END && CurRemainNum != 0 ; ++i)
	{
		CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
		if(pItem && pItem->GetItemIdx() == wItemIdx)
		{
			if(pItem->IsLocked())
				continue;

			if(ITEMMGR->IsDupItem(wItemIdx))
			{
				if(CurRemainNum > pItem->GetDurability())
				{
					ItemOut[ItemPosNum].Dur	= pItem->GetDurability();
					CurRemainNum -= pItem->GetDurability();
				}
				else
				{
					ItemOut[ItemPosNum].Dur	= CurRemainNum;
					CurRemainNum = 0;
				}
				ItemOut[ItemPosNum++].ItemPos	= i;
				
			}
			else
			{
				ItemOut[ItemPosNum].Dur = 1;
				ItemOut[ItemPosNum++].ItemPos = i;
				--CurRemainNum;
			}
			
		}
	}

	if(CurRemainNum == 0)
		return TRUE;
	else
		return FALSE;
}

void CMixDialog::MixCancelBtn(CMixDialog * pThis)
{
	// 모든 링크 삭제
	pThis->Release();
	pThis->SetActiveRecursive(FALSE);
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(206));
}


BOOL CMixDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(we)
	{
	case WE_CLOSEWINDOW:
		{
//			CMixDialog * pMixDlg = (CMixDialog *)p; //PPP
			CMixDialog * pMixDlg = (CMixDialog *)WINDOWMGR->GetWindowForID(lId);
			if( pMixDlg->m_bShopItemUse )
			{
				CItem* pItem = ITEMMGR->GetItemofTable( eItemTable_ShopInven, (POSTYPE)pMixDlg->m_ShopItemPos );
				if( pItem )
					pItem->SetLock( FALSE );

				pMixDlg->m_bShopItemUse	= FALSE;
				pMixDlg->m_ShopItemIdx	= 0;
				pMixDlg->m_ShopItemPos	= 0;
			}
			pMixDlg->Release();
			return TRUE;
		}
		break;
	}

	switch( lId )
	{
	case ITMD_MIXICONGRIDDLG:
		{
			//cIconGridDialog * pMixGridDlg = (cIconGridDialog *)p;	//PPP
			cIconGridDialog* pMixGridDlg = (cIconGridDialog *)((cDialog*)p)->GetWindowForID(lId);	//PPP

			if(we == WE_LBTNCLICK)
			{
				CMixDialog * pPar = (CMixDialog *)pMixGridDlg->GetParent();
				LONG SelectPos = pMixGridDlg->GetCurSelCellPos();
				CItemShow * pSelItemShow = (CItemShow *)pMixGridDlg->GetIconForIdx((WORD)SelectPos);
				if(NULL == pSelItemShow)
				{
					pPar->m_pMaterialsStatic->SetStaticText(" ");
					return FALSE;
				}
				
				CVirtualItem * pBasicItem = (CVirtualItem *)pPar->m_pMixDlg->GetIconForIdx(eMixPos_Basic);
				ITEM_MIX_INFO * pMixInfo = MIXMGR->GetMixItemInfo(pBasicItem->GetSrcItemIdx());
				if(!pMixInfo) 
					return FALSE;
				pPar->ViewMaterialsDesc(&pMixInfo->psResultItemInfo[SelectPos]);
			}
		}
		break;
	case ITMD_MIXOKBTN:
		{
			CMixDialog * pMixDlg = (CMixDialog *)p;
			CMixDialog::MixOkBtn(pMixDlg);
		}
		break;
	case ITMD_MIXCANCELBTN:
		{
			CMixDialog * pMixDlg = (CMixDialog *)p;
			CMixDialog::MixCancelBtn(pMixDlg);
		}
		break;
		
	}

	return TRUE;
}


DWORD CMixDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	for(int i = 0 ; i < eMixPos_Max ; ++i)
	{
		m_VirtualItem[i].ActionEvent(mouseInfo);
	}
	return we;
}

void CMixDialog::ResetCurSelCellPos()
{
	m_pMixGridDlg->SetCurSelCellPos(-1);
	m_bMixing = FALSE;
}


BOOL CMixDialog::AddRealItem( CItem *pOrigItem )
{
/*
	ITEM_MIX_INFO * pMixItemInfo = MIXMGR->GetMixItemInfo(pOrigItem->GetItemIdx());
	if( !pMixItemInfo ) return FALSE;

	if(!IsEmpty(eMixPos_Basic))
	{
		Release(eMixBasicRelease);
	}
	Release(eMixResultRelease);

	//add Item
	AddVirtualItemWrap(eMixPos_Basic, pOrigItem);
	//m_VirtualItem[eMixPos_Basic].SetLinkItem(pOrigItem);
	//m_pMixDlg->AddIcon(eMixPos_Basic, (cIcon*)&m_VirtualItem[eMixPos_Basic]);

	ViewResultItemList(pMixItemInfo);

	m_pMaterialsStatic->SetStaticText( CHATMGR->GetChatMsg(613) );
*/
	return TRUE;
}

void CMixDialog::DieCheck()
{
	if( HERO->GetState() == eObjectState_Die )
	{
		Release();
		SetActiveRecursive( FALSE );
	}
}


void CMixDialog::SetShopItemInfo( DWORD ShopItemIdx, DWORD ShopItemPos )
{
	m_ShopItemIdx = ShopItemIdx;
	m_ShopItemPos = ShopItemPos;
	m_bShopItemUse = TRUE;
}
