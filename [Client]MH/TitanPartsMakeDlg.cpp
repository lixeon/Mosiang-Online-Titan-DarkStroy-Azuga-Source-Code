// TitanPartsMakeDlg.cpp: implementation of the CTitanPartsMakeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TitanPartsMakeDlg.h"
#include "INTERFACE\cIconDialog.h"
#include "INTERFACE\cIconGridDialog.h"
#include "interface/cStatic.h"
#include "WindowIDEnum.h"
#include "interface/cScriptManager.h"
#include "item.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "MixManager.h"
#include "cWindowManager.h"
#include "StreetStallManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
#include "TitanManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CTitanPartsMakeDlg::CTitanPartsMakeDlg()
{
	m_pTitanPartsMakeDlg = NULL;
	m_pTitanPartsMakeGridDlg = NULL;
	m_pTitanPartsProgressBarDlg = NULL;

	cImage imgTooltip;
	SCRIPTMGR->GetImage( 63, &imgTooltip, PFT_HARDPATH );

	for( int i = 0; i < eTPMPos_Max; i++ )
	{
		m_VirtualItem[i].SetToolTip( "", RGB_HALF(255,255,255), &imgTooltip );
		m_VirtualItem[i].SetMovable(FALSE);
	}

	for( int i = 0; i < MAX_TPMGRID_NUM; i++ )
	{
		m_ViewShowItem[i].SetToolTip( "", RGB_HALF(255,255,255), &imgTooltip );
		m_ViewShowItem[i].SetMovable(FALSE);
	}

	m_bShopItemUse	= FALSE;
	m_ShopItemIdx	= 0;
	m_ShopItemPos	= 0;
	m_pTitle = NULL;
	ZeroMemory( m_Title, sizeof(m_Title) );
}

CTitanPartsMakeDlg::~CTitanPartsMakeDlg()
{
	m_pTitanPartsProgressBarDlg = NULL;
}

void CTitanPartsMakeDlg::Release(eTitanPartsMakeReleaseOpt op, BOOL bMsg)
{
	ResetCurSelCellPos();	// 선택된 그리드다이얼로그 취소

	CVirtualItem * pVItem;

	if(!m_pTitanPartsMakeDlg->IsAddable(eTPMPos_Basic) && (op & eTPMBasicRelease))
	{
		m_pTitanPartsMakeDlg->DeleteIcon( eTPMPos_Basic, (cIcon**)&pVItem );
		pVItem->GetLinkItem()->SetLock(FALSE);
		if( bMsg )
		{
            MSG_WORD msg;
			SetProtocol( &msg, MP_ITEM, MP_ITEM_TPM_RELEASEITEM );
			msg.dwObjectID = HEROID;
			msg.wData = pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		pVItem->SetLinkItem(NULL);
		DeleteViewResultItemAll();
		m_pMaterialsStatic->SetStaticText(" ");
	}

	if(!m_pTitanPartsMakeDlg->IsAddable(eTPMPos_Result) && (op & eTPMResultRelease))
	{
		m_pTitanPartsMakeDlg->DeleteIcon( eTPMPos_Result, (cIcon**)&pVItem );
		pVItem->GetLinkItem()->SetLock(FALSE);
		if( bMsg )
		{
			MSG_WORD msg;
			SetProtocol( &msg, MP_ITEM, MP_ITEM_TPM_RELEASEITEM );
			msg.dwObjectID = HEROID;
			msg.wData = pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		pVItem->SetLinkItem(NULL);
	}
}

void CTitanPartsMakeDlg::Linking()
{
	m_pMaterialsStatic = (cStatic *)GetWindowForID(TITAN_TITANPARTSMAKEMATERIALS);
	m_pMaterialsStatic->SetMultiLine(TRUE);
	m_pMaterialsStatic->InitMultiLine();
	m_pTitanPartsMakeDlg = (cIconDialog *)GetWindowForID(TITAN_TITANPARTSMAKEICONDLG);
	m_pTitanPartsMakeGridDlg = (cIconGridDialog *)GetWindowForID(TITAN_TITANPARTSMAKEICONGRIDDLG);
	m_pTitle = (cStatic*)GetWindowForID(TITAN_TITANPARTSMAKEICONTITLE);

	if( m_pTitle )
		strcpy( m_Title, m_pTitle->GetStaticText() );
	m_bTPMaking = FALSE;

	m_pTitanPartsProgressBarDlg = GAMEIN->GetTitanPartsProgressBarDlg();
	m_pTitanPartsProgressBarDlg->Linking();
	m_pTitanPartsProgressBarDlg->SetSuccessTime(5000); //5초
}

void CTitanPartsMakeDlg::Render()
{
	if(m_pTitanPartsProgressBarDlg->GetSuccessProgress() == TRUE)
	{
		m_pTitanPartsProgressBarDlg->InitProgress();
		SetDisable(FALSE);
		TitanPartsMake();		
	}

	cDialog::Render();
}

void CTitanPartsMakeDlg::SetActive(BOOL val)
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
	else
	{
		Release();

		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(206));
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

BOOL CTitanPartsMakeDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case TITAN_TITANPARTSMAKEICONGRIDDLG:
		{
			cIconGridDialog* pTitanPartsMakeGridDlg = (cIconGridDialog *)GetWindowForID(lId);	//PPP

			if(we == WE_LBTNCLICK)
			{
				//CTitanPartsMakeDlg * pPar = (CTitanPartsMakeDlg *)pTitanPartsMakeGridDlg->GetParent();
				LONG SelectPos = pTitanPartsMakeGridDlg->GetCurSelCellPos();
				CItemShow * pSelItemShow = (CItemShow *)pTitanPartsMakeGridDlg->GetIconForIdx((WORD)SelectPos);
				if(NULL == pSelItemShow)
				{
					m_pMaterialsStatic->SetStaticText(" ");
					return FALSE;
				}

				// 재료를 표시해준다
				CVirtualItem * pBasicItem = (CVirtualItem *)m_pTitanPartsMakeDlg->GetIconForIdx(eTPMPos_Basic);
				ITEM_MIX_INFO * pTitanPartsMakeInfo = TITANMGR->GetTPMItemInfo(pBasicItem->GetSrcItemIdx());
				if(!pTitanPartsMakeInfo) 
					return FALSE;
				ViewMaterialsDesc(&pTitanPartsMakeInfo->psResultItemInfo[SelectPos]);
			}
		}
		break;
	case TITAN_TITANPARTSMAKEOKBTN:
		{
			MixOkBtn();
		}
		break;
	case TITAN_TITANPARTSMAKECANCELBTN:
		{
			MixCancelBtn();
		}
		break;
	}

	return TRUE;
}

void CTitanPartsMakeDlg::MixOkBtn()
{
	//조합을 누르고 결과를 기다리는 중
	if( IsNowMixing() == TRUE )
		return;

	// 기본 아이템 존재?
	CVirtualItem * pBasicItem = (CVirtualItem *)m_pTitanPartsMakeDlg->GetIconForIdx(eTPMPos_Basic);
	if(NULL == pBasicItem)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(618) );
		return ;
	}

	// 결과선택 ok?
	if(-1 == m_pTitanPartsMakeGridDlg->GetCurSelCellPos())
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(203) );
		return;
	}
	CItemShow * pSelItemShow = (CItemShow *)m_pTitanPartsMakeGridDlg->GetIconForIdx((WORD)m_pTitanPartsMakeGridDlg->GetCurSelCellPos());
	if(NULL == pSelItemShow)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(203) );
		return ;
	}

	ITEM_MIX_INFO * pTitanPartsMakeInfo = TITANMGR->GetTPMItemInfo(pBasicItem->GetSrcItemIdx());
	for(int i = 0 ; i < pTitanPartsMakeInfo->wResultItemNum ; ++i)
	{
		if(pTitanPartsMakeInfo->psResultItemInfo[i].wResItemIdx == pSelItemShow->GetItemIdx())
		{
			DWORD dwMoney;
			if(ITEMMGR->IsDupItem(pBasicItem->GetSrcItemIdx()))
			{
				dwMoney = pTitanPartsMakeInfo->psResultItemInfo[i].Money * pBasicItem->GetSrcDurability();
			}
			else
			{
				dwMoney = pTitanPartsMakeInfo->psResultItemInfo[i].Money;
			}

			if( dwMoney > HERO->GetMoney() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
				return ;
			}

			MATERIAL_ARRAY ItemOut[TP_INVENTORY_END-TP_INVENTORY_START];
			WORD ItemPosNum;
			WORD needItemIdx;
			DURTYPE needItemNum;

			for(int j = 0 ; j < pTitanPartsMakeInfo->psResultItemInfo[i].wMaterialItemNum ; ++j)
			{
				needItemIdx = pTitanPartsMakeInfo->psResultItemInfo[i].psMaterialItemInfo[j].wMatItemIdx;
				needItemNum = pTitanPartsMakeInfo->psResultItemInfo[i].psMaterialItemInfo[j].wMatItemNum;
				if(ITEMMGR->IsDupItem(pBasicItem->GetSrcItemIdx()))
					needItemNum *= pBasicItem->GetSrcDurability();
				if(!EnoughMaterial(needItemIdx, (WORD)needItemNum, ItemOut, ItemPosNum))
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(205) );
					return;
				}				
			}
		}
	}

	m_pTitanPartsProgressBarDlg->SetActive(TRUE);
	m_pTitanPartsProgressBarDlg->StartProgress();
	SetDisable(TRUE);
}

void CTitanPartsMakeDlg::TitanPartsMake()
{
	CVirtualItem * pBasicItem = (CVirtualItem *)m_pTitanPartsMakeDlg->GetIconForIdx(eTPMPos_Basic);
	CItemShow * pSelItemShow = (CItemShow *)m_pTitanPartsMakeGridDlg->GetIconForIdx((WORD)m_pTitanPartsMakeGridDlg->GetCurSelCellPos());
	ITEM_MIX_INFO * pTitanPartsMakeInfo = TITANMGR->GetTPMItemInfo(pBasicItem->GetSrcItemIdx());
	for(int i = 0 ; i < pTitanPartsMakeInfo->wResultItemNum ; ++i)
	{
		if(pTitanPartsMakeInfo->psResultItemInfo[i].wResItemIdx == pSelItemShow->GetItemIdx())
		{
			MSG_ITEM_MIX_SYN msg;
			msg.Init();
			msg.Category			= MP_ITEM;
			msg.Protocol			= MP_ITEM_TPM_SYN;
			msg.dwObjectID			= HEROID;
			msg.wBasicItemIdx		= pBasicItem->GetSrcItemIdx();
			msg.BasicItemPos		= pBasicItem->GetSrcPosition();

			msg.dwRareIdx			= pBasicItem->GetRareness();
			msg.wResultItemIdx		= pSelItemShow->GetItemIdx();	
			msg.ResultIndex			= i;
			if( m_bShopItemUse )
			{
				msg.ShopItemIdx = (WORD)m_ShopItemIdx;
				msg.ShopItemPos = (WORD)m_ShopItemPos;
			}

			// 필요한 돈은 있는가?
			DWORD dwMoney;
			if(ITEMMGR->IsDupItem(pBasicItem->GetSrcItemIdx()))
			{
				dwMoney = pTitanPartsMakeInfo->psResultItemInfo[i].Money * pBasicItem->GetSrcDurability();
			}
			else
			{
				dwMoney = pTitanPartsMakeInfo->psResultItemInfo[i].Money;
			}
			/*
			if( dwMoney > HERO->GetMoney() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
				return ;
			}
			*/


			// 재료 enough?
			//			MATERIAL_ARRAY ItemOut[MAX_MIXGRID_NUM]; //버그!!!
			MATERIAL_ARRAY ItemOut[TP_INVENTORY_END-TP_INVENTORY_START];
			WORD ItemPosNum;
			WORD needItemIdx;
			DURTYPE needItemNum;


			for(int j = 0 ; j < pTitanPartsMakeInfo->psResultItemInfo[i].wMaterialItemNum ; ++j)
			{
				needItemIdx = pTitanPartsMakeInfo->psResultItemInfo[i].psMaterialItemInfo[j].wMatItemIdx;
				needItemNum = pTitanPartsMakeInfo->psResultItemInfo[i].psMaterialItemInfo[j].wMatItemNum;
				if(ITEMMGR->IsDupItem(pBasicItem->GetSrcItemIdx()))
					needItemNum *= pBasicItem->GetSrcDurability();
				
				if(!EnoughMaterial(needItemIdx, (WORD)needItemNum, ItemOut, ItemPosNum))
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
			SetNowMixing( TRUE );

			return;
		}
	}
}

BOOL CTitanPartsMakeDlg::EnoughMaterial(WORD wItemIdx, WORD ItemNum, MATERIAL_ARRAY * ItemOut, WORD & ItemPosNum)
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

void CTitanPartsMakeDlg::MixCancelBtn()
{
	SetActive(FALSE);
}

DWORD CTitanPartsMakeDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	for(int i = 0 ; i < eTPMPos_Max ; ++i)
	{
		m_VirtualItem[i].ActionEvent(mouseInfo);
	}
	return we;
}

BOOL CTitanPartsMakeDlg::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if(pOrigIcon->GetType() != WT_ITEM)	return FALSE;
	if( m_bDisable ) return FALSE;

	CItem* pOrigItem = (CItem*)pOrigIcon;
	if( pOrigItem->IsLocked() )	return FALSE;

	// 인벤토리 아이템이 아니면 FALSE
	if( !ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(612) );
		return FALSE;
	}

	// 옵션 아이템이면 FALSE
	if( ITEMMGR->IsOptionItem(pOrigItem->GetItemIdx(), pOrigItem->GetDurability()) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
		return FALSE;
	}


	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	/*if( bits != eTITAN_ITEM_RECIPE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
		return FALSE;
	}
*/

	// 해당 아이템이 만들수 있는 결과 아이템들의 리스트
	ITEM_MIX_INFO* pTitanPartsMakeInfo = TITANMGR->GetTPMItemInfo(pOrigItem->GetItemIdx());
	if(!pTitanPartsMakeInfo)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );	
		return FALSE;
	}

	// 영약, 보석을 제외한 모든 템은 1개만 가능
	if( GetItemKind(pOrigItem->GetItemIdx()) != eYOUNGYAK_ITEM &&
		GetItemKind(pOrigItem->GetItemIdx()) != eEXTRA_ITEM_JEWEL &&
		pOrigItem->GetDurability() > 1)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );	
		return FALSE;
	}

	if((bits & eEQUIP_ITEM) && m_bShopItemUse)
	{
		ITEM_INFO* pShopItem = ITEMMGR->GetItemInfo((WORD)m_ShopItemIdx);
		if(!pShopItem)	return FALSE;
		if(!(pShopItem->ItemKind & eSHOP_ITEM))	return FALSE;

		ITEM_INFO* pBaseItemInfo = ITEMMGR->GetItemInfo(pOrigItem->GetItemIdx());
		if(!pBaseItemInfo)	return FALSE;

		if( pBaseItemInfo->LimitLevel < pShopItem->GenGol || pBaseItemInfo->LimitLevel > pShopItem->MinChub )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
			return FALSE;
		}
	}
	// Lock Item
	pOrigItem->SetLock(TRUE);

	// Send Message
	MSG_ITEM	msgItem;
	msgItem.Category	= MP_ITEM;
	msgItem.Protocol	= MP_ITEM_TPM_ADDITEM_SYN;
	msgItem.dwObjectID	= HEROID;
	msgItem.ItemInfo	= *pOrigItem->GetItemBaseInfo();
	NETWORK->Send( &msgItem, sizeof(msgItem) );

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_MunpaWarehouse);

	if(!IsEmpty(eTPMPos_Basic))
	{
		Release(eTPMBasicRelease);
	}
	Release(eTPMResultRelease);

	//add Item
	AddVirtualItemWrap(eTPMPos_Basic, pOrigItem);

	ViewResultItemList(pTitanPartsMakeInfo);

	m_pMaterialsStatic->SetStaticText( CHATMGR->GetChatMsg(613) );

	return FALSE;
}

BOOL CTitanPartsMakeDlg::IsEmpty(eTitanPartsMakePos pos)
{
	if( pos >= eTPMPos_Max )
		return FALSE;

	return m_pTitanPartsMakeDlg->IsAddable(pos);
}

void CTitanPartsMakeDlg::ResetCurSelCellPos()
{
	m_pTitanPartsMakeGridDlg->SetCurSelCellPos(-1);
	m_bTPMaking = FALSE;
}

void CTitanPartsMakeDlg::DeleteViewResultItemAll()
{
	for(int i = 0 ; i < m_pTitanPartsMakeGridDlg->GetCellNum() ; ++i )
	{
		CItemShow * pSItemOut = NULL;
		m_pTitanPartsMakeGridDlg->DeleteIcon(i, (cIcon**)&pSItemOut);
	}
}

BOOL CTitanPartsMakeDlg::AddVirtualItemWrap(POSTYPE relPos, CItem * pItem)
{
	m_VirtualItem[relPos].SetData(pItem->GetItemIdx());
	m_VirtualItem[relPos].SetLinkItem(pItem);

	if( pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()) )
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[relPos], ITEMMGR->GetItemOption(pItem->GetDurability())
			, ITEMMGR->GetItemRareOption(pItem->GetRareness()) );
	}	
	else
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[relPos], NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()));

	return AddVirtualItem(relPos, &m_VirtualItem[relPos]);
}

BOOL CTitanPartsMakeDlg::AddVirtualItem(POSTYPE relPos, CVirtualItem * pItem)
{
	//empty 체크후 
	if(m_pTitanPartsMakeDlg->IsAddable(relPos))
	{
		m_pTitanPartsMakeDlg->AddIcon(relPos, (cIcon *)pItem);
		return TRUE;
	}
	return FALSE;
}

CVirtualItem * CTitanPartsMakeDlg::GetVirtualItem(POSTYPE relPos)
{
	return (CVirtualItem *)m_pTitanPartsMakeDlg->GetIconForIdx(relPos);
}

void CTitanPartsMakeDlg::ViewMaterialsDesc(ITEM_MIX_RES * pResultItem)// 보류 //
{
	char line[64] = {0,};
	char szDescText[384] = {0,};
	CVirtualItem * pBasicItem = (CVirtualItem *)m_pTitanPartsMakeDlg->GetIconForIdx(eTPMPos_Basic);

	if(pBasicItem->GetSrcDurability() >1 )
	{		
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

	for(int i = 0 ; i < pResultItem->wMaterialItemNum ; ++i)
	{
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

void CTitanPartsMakeDlg::ViewResultItemList(ITEM_MIX_INFO * pTPMItemInfo)
{
	if(!pTPMItemInfo) return;
	DeleteViewResultItemAll();

	DURTYPE Dur = m_VirtualItem[eTPMPos_Basic].GetLinkItem()->GetDurability();

	for(int i = 0 ; i < pTPMItemInfo->wResultItemNum ; ++i)
	{
		m_ViewShowItem[i].SetData(pTPMItemInfo->psResultItemInfo[i].wResItemIdx);
		m_ViewShowItem[i].Init(pTPMItemInfo->psResultItemInfo[i].wResItemIdx, Dur);

		//레어 조합
		ITEMMGR->SetToolTipIcon( (cIcon*)&m_ViewShowItem[i], NULL, ITEMMGR->GetItemRareOption(m_VirtualItem[eTPMPos_Basic].GetRareness()) );

		m_pTitanPartsMakeGridDlg->AddIcon(i, (cIcon*)&m_ViewShowItem[i]);
	}
}

BOOL CTitanPartsMakeDlg::AddRealItem( CItem *pOrigItem )
{
	return TRUE;
}

void CTitanPartsMakeDlg::DieCheck()
{
	if( HERO->GetState() == eObjectState_Die )
	{
		Release();
		SetActiveRecursive( FALSE );
	}
}

void CTitanPartsMakeDlg::SetShopItemInfo( DWORD ShopItemIdx, DWORD ShopItemPos )
{
	m_ShopItemIdx = ShopItemIdx;
	m_ShopItemPos = ShopItemPos;
	m_bShopItemUse = TRUE;
}