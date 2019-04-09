///////////////////////////////////////////////////////////////////////////////////////////////
// UniqueItemMixDlg.cpp: interface for the UniqueItemMixDlg class.
///////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "UniqueItemMixDlg.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "cScriptManager.h"
#include "Item.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "cIconDialog.h"
#include "GameIn.h"
#include "InventoryExDialog.h"

CUniqueItemMixDlg::CUniqueItemMixDlg()
{
	m_pUniqueItemSlotDlg = NULL;
	m_pUniqueItemMixProgressBarDlg = NULL;

	cImage imgTooltip;
	SCRIPTMGR->GetImage(63, &imgTooltip, PFT_HARDPATH);

	m_VirtualItem.SetToolTip("", RGB_HALF(255,255,255), &imgTooltip);
	m_VirtualItem.SetMovable(FALSE);
}

CUniqueItemMixDlg::~CUniqueItemMixDlg()
{
	m_pUniqueItemMixProgressBarDlg = NULL;
}

void CUniqueItemMixDlg::Release( BOOL bFlag )
{
	CVirtualItem* pVItem;
	if(!m_pUniqueItemSlotDlg->IsAddable(0))
	{
		m_pUniqueItemSlotDlg->DeleteIcon(0, (cIcon**)&pVItem);

		if(bFlag)
		{
			MSG_WORD msg;
			SetProtocol(&msg, MP_ITEMEXT, MP_ITEMEXT_UNIQUEITEM_MIX_RELEASE);
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}

		m_pMaterialsStatic->SetStaticText(" ");
		pVItem->GetLinkItem()->SetLock(FALSE);
		pVItem->SetLinkItem(NULL);
	}
}

void CUniqueItemMixDlg::Linking()
{
	m_pUniqueItemSlotDlg = (cIconDialog*)GetWindowForID(UNIQUEITEMMIX_ICON);
	m_pMaterialsStatic = (cStatic*)GetWindowForID(UNIQUEITEMMIX_MATERIALS);
	m_pMaterialsStatic->SetMultiLine(TRUE);
	m_pMaterialsStatic->InitMultiLine();	

	m_pUniqueItemMixProgressBarDlg = GAMEIN->GetUniqueItemMixProgressBarDlg();
	m_pUniqueItemMixProgressBarDlg->SetSuccessTime(5000);	//5초 셋팅
}

//2007. 10. 22. CBH - 게이지 만땅일때 처리를 위해 재정의 추가
void CUniqueItemMixDlg::Render()
{	
	if(m_pUniqueItemMixProgressBarDlg->GetSuccessProgress() == TRUE)
	{
		m_pUniqueItemMixProgressBarDlg->InitProgress();
		SetDisable(FALSE);
		UniqueItemMix();		
	}
	
	cDialog::Render();
}

void CUniqueItemMixDlg::SetActive( BOOL bFlag )
{
	cDialog::SetActive( bFlag );

	if( !bFlag )
	{
		Release();

		if(HERO->GetState() == eObjectState_Deal)
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}

BOOL CUniqueItemMixDlg::FakeMoveIcon(LONG x, LONG y, cIcon* icon)
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

	// 해당 유니크 아이템이 조합 가능한지를 체크
	if(pInfo->MixFlag == 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1628));
		return FALSE;
	}

	// 해당 유니크 아이템 조합 리스트
	UNIQUE_ITEM_MIX_INFO* pMixInfo = GAMERESRCMNGR->GetUniqueItemMixList(pItem->GetItemIdx());
	if( !pMixInfo )
	{
		return FALSE;
	}

	// 이미 들어간 유니크아이템 초기화
	Release();

	// 락걸기
	pItem->SetLock(TRUE);

	// 유니크 아이템 추가
	AddUniqueItem(pItem);

	ViewUniqueItemMixMaterial(pItem);

	// Send Message
	MSG_ITEM	msgItem;
	msgItem.Category	= MP_ITEMEXT;
	msgItem.Protocol	= MP_ITEMEXT_UNIQUEITEM_MIX_ADDITEM_SYN;
	msgItem.dwObjectID	= HEROID;
	msgItem.ItemInfo	= *pItem->GetItemBaseInfo();
	NETWORK->Send( &msgItem, sizeof(msgItem) );

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

	return FALSE;
}

void CUniqueItemMixDlg::AddUniqueItem(CItem* pItem)
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

DWORD CUniqueItemMixDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	m_VirtualItem.ActionEvent(mouseInfo);

	return we;
}

void CUniqueItemMixDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case UNIQUEITEMMIX_OKBTN:
		{
			OnOkBtn();		
		}
		break;
	case UNIQUEITEMMIX_CANCELBTN:
		{
			OnCancelBtn();
		}
		break;
	}
}

void CUniqueItemMixDlg::OnOkBtn()
{
	// 슬롯에 아이템이 있는지 체크
	CVirtualItem* pItem = (CVirtualItem*)m_pUniqueItemSlotDlg->GetIconForIdx(0);
	if( !pItem )
		return;

	UNIQUE_ITEM_OPTION_INFO* pInfo = GAMERESRCMNGR->GetUniqueItemOptionList(pItem->GetLinkItem()->GetItemIdx());
	if( !pInfo )
		return;

	// 해당 유니크 아이템이 조합 가능한지를 체크
	if(pInfo->MixFlag == 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1628));
		return;
	}

	// 해당 유니크 아이템 조합 리스트
	UNIQUE_ITEM_MIX_INFO* pMixInfo = GAMERESRCMNGR->GetUniqueItemMixList(pItem->GetLinkItem()->GetItemIdx());
	if( !pMixInfo )
	{
		return;
	}

	//재료 체크
	MATERIAL_ARRAY sMaterial[TP_INVENTORY_END - TP_INVENTORY_START];
	WORD	wMaterialTotalCnt = 0;
	DWORD	dwNeedItemIdx = 0;
	WORD	wNeedItemDur = 0;
	WORD	wItemPos = 0;
	
	for( int i = 0; i < pMixInfo->wMaterialKindNum; i++ )
	{
		dwNeedItemIdx = pMixInfo->sUniqueItemMixMaterial[i].dwMaterialIdx;
		wNeedItemDur = pMixInfo->sUniqueItemMixMaterial[i].wMaterialNum;
		if( EnoughMaterial(dwNeedItemIdx, wNeedItemDur, sMaterial, wItemPos) == FALSE )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(205) );
			return;
		}		
	}

	m_pUniqueItemMixProgressBarDlg->SetActive(TRUE);
	m_pUniqueItemMixProgressBarDlg->StartProgress();
	SetDisable(TRUE);
}

void CUniqueItemMixDlg::UniqueItemMix()
{
	CVirtualItem* pItem = (CVirtualItem*)m_pUniqueItemSlotDlg->GetIconForIdx(0);
	if( !pItem )
		return;

	// 해당 유니크 아이템 조합 리스트
	UNIQUE_ITEM_MIX_INFO* pMixInfo = GAMERESRCMNGR->GetUniqueItemMixList(pItem->GetLinkItem()->GetItemIdx());
	if( !pMixInfo )
	{
		return;
	}

	MSG_UNIQUEITEM_MIX_SYN msg;
	ZeroMemory(&msg, sizeof(MSG_UNIQUEITEM_MIX_SYN));
	SetProtocol(&msg, MP_ITEMEXT, MP_ITEMEXT_UNIQUEITEM_MIX_SYN);
	msg.dwObjectID = HEROID;
	msg.wBasicItemIdx = pItem->GetLinkItem()->GetItemIdx();
	msg.wBasicItemPos = pItem->GetLinkItem()->GetPosition();

	// 재료가 충분한가?
	MATERIAL_ARRAY sMaterial[TP_INVENTORY_END - TP_INVENTORY_START];
	WORD	wMaterialTotalCnt = 0;
	DWORD	dwNeedItemIdx = 0;
	WORD	wNeedItemDur = 0;
	WORD	wItemPos = 0;

	for( int i = 0; i < pMixInfo->wMaterialKindNum; i++ )
	{
		dwNeedItemIdx = pMixInfo->sUniqueItemMixMaterial[i].dwMaterialIdx;
		wNeedItemDur = pMixInfo->sUniqueItemMixMaterial[i].wMaterialNum;
		if( EnoughMaterial(dwNeedItemIdx, wNeedItemDur, sMaterial, wItemPos) == FALSE )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(205) );
			return;
		}

		for( int j = 0; j < wItemPos; j++ )
		{
			msg.AddMaterial(sMaterial[j].wItemIdx, sMaterial[j].ItemPos, sMaterial[j].Dur);
		}
	}

	NETWORK->Send(&msg, msg.GetSize());
}

BOOL CUniqueItemMixDlg::EnoughMaterial( DWORD dwNeedItemIdx, WORD wNeedItemDur, MATERIAL_ARRAY* sMaterial, WORD &wItemPos )
{
	WORD wItemDur = wNeedItemDur;	// 필요한 재료 아이템의 갯수(인벤에서 재료아이템을 찾을때마다 그 갯수만큼 줄여준다.)
	ZeroMemory(sMaterial, sizeof(MATERIAL_ARRAY) * (TP_INVENTORY_END - TP_INVENTORY_START));
	wItemPos = 0;

	for( int i = TP_INVENTORY_START; i < TP_INVENTORY_END && wItemDur != 0; i++ )
	{
		// 해당 슬롯의 아이템이 존재하고 그 아이템이 필요한 재료아이템과 인덱스가 같아야한다.
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
		if(pItem && pItem->GetItemIdx() == dwNeedItemIdx)
		{
			// 해당 재료아이템에 락이 걸려있으면 재료아이템을 사용할수 없다.
			if(pItem->IsLocked())
				continue;

			sMaterial[wItemPos].wItemIdx = (WORD)dwNeedItemIdx;
			sMaterial[wItemPos].ItemPos = i;

			// 해당 재료아이템이 묶음 아이템일때
			if( ITEMMGR->IsDupItem((WORD)dwNeedItemIdx) )
			{
				// 해당 재료아이템의 갯수가 필요한 갯수보다 부족할때..
				if(wItemDur > pItem->GetDurability())
				{
					sMaterial[wItemPos++].Dur = pItem->GetDurability();
                    wItemDur -= (WORD)pItem->GetDurability();
				}
				else	// 해당 재료아이템의 갯수가 필요한 갯수와 같거나 더 많을때..
				{
					sMaterial[wItemPos++].Dur = wItemDur;
					wItemDur = 0;
				}
			}
			else	// 해당 재료아이템이 묶음 아이템이 아닐때
			{
				// 해당 재료아이템의 인덱스와 포지션을 지정해주고 묶음 아이템이 아니므로 갯수는 1이라고 한다.
				sMaterial[wItemPos++].Dur = 1;
				wItemDur--;
			}
		}
	}

	if(wItemDur == 0)
		return TRUE;
	else
		return FALSE;
}

void CUniqueItemMixDlg::OnCancelBtn()
{
	SetActive(FALSE);
}

void CUniqueItemMixDlg::ViewUniqueItemMixMaterial(CItem* pItem)
{
	// 해당 유니크 아이템 조합 리스트
	UNIQUE_ITEM_MIX_INFO* pMixInfo = GAMERESRCMNGR->GetUniqueItemMixList(pItem->GetItemIdx());
	if( !pMixInfo )
	{
		return;
	}

	char line[64] = {0,};
	char szDescText[384] = {0,};

	wsprintf(szDescText, "%s^n^n", ITEMMGR->GetItemInfo((WORD)pMixInfo->dwItemIdx)->ItemName);

	for( int i = 0; i < pMixInfo->wMaterialKindNum; i++ )
	{
		wsprintf(line, "%s X %d^n", ITEMMGR->GetItemInfo((WORD)pMixInfo->sUniqueItemMixMaterial[i].dwMaterialIdx)->ItemName, pMixInfo->sUniqueItemMixMaterial[i].wMaterialNum);
		strcat(szDescText, line);
	}

	m_pMaterialsStatic->SetStaticText(szDescText);
}
