// TitanInventoryDlg.cpp: implementation of the CTitanInventoryDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TitanInventoryDlg.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "INTERFACE\cIconDialog.h"
#include "cScriptManager.h"
#include "ItemManager.h"
#include "cWindowManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "MHAudioManager.h"
#include "cWindowManager.h"
#include "TitanManager.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
#include "MouseCursor.h"
#include "cMsgBox.h"

CTitanInventoryDlg::CTitanInventoryDlg()
{
	m_pTitanInvenWearedDlg = NULL;	
	m_pTitanShopItemDlg = NULL;

	ZeroMemory( m_dwWearInfoArray, sizeof(m_dwWearInfoArray) );

	cImage imgTooltip;
	SCRIPTMGR->GetImage( 63, &imgTooltip, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 65, &m_Image );
}

CTitanInventoryDlg::~CTitanInventoryDlg()
{

}

void CTitanInventoryDlg::Linking()
{
	m_pTitanInvenWearedDlg = (CWearedExDialog *)GetWindowForID(TITAN_INVENTORY_WEAREDDLG);	
	m_pTitanShopItemDlg = (cIconDialog*)GetWindowForID(TITAN_INVENTORY_INVEN_SET_DLG);

    m_TitanInvenCtrl.titanType = (cStatic*)GetWindowForID(TITAN_INVENTORY_TYPE);
	m_TitanInvenCtrl.titanAtt = (cStatic*)GetWindowForID(TITAN_INVENTORY_CHARATTACK);
	m_TitanInvenCtrl.titanLongAtt = (cStatic*)GetWindowForID(TITAN_INVENTORY_LONGATTACK);
	m_TitanInvenCtrl.titanMagicAtt = (cStatic*)GetWindowForID(TITAN_INVENTORY_MAGICATTACK);
	m_TitanInvenCtrl.titanCritical = (cStatic*)GetWindowForID(TITAN_INVENTORY_CRITICAL);
	m_TitanInvenCtrl.titanDistance = (cStatic*)GetWindowForID(TITAN_INVENTORY_DISTANCE);
	m_TitanInvenCtrl.titanDef = (cStatic*)GetWindowForID(TITAN_INVENTORY_CHARDEFENSE);
	m_TitanInvenCtrl.titanLife = (cStatic*)GetWindowForID(TITAN_INVENTORY_CHARLIFE);
	//m_TitanInvenCtrl.titanMana = (cStatic*)GetWindowForID(TITAN_INVENTORY_CHARNAERYUK);

	for( int i = 0; i < ATTR_MAX; i++ )
	{
		m_TitanInvenCtrl.TitanAttrDef.pStatic.SetElement_Val(ATTR_FIRE+i,(cStatic *)GetWindowForID(TITAN_INVENTORY_CHARHWA+i));
		m_TitanInvenCtrl.TitanAttrDef.pGuage.SetElement_Val(ATTR_FIRE+i,(cGuagen *)GetWindowForID(TITAN_INVENTORY_DEFENCE_HWA+i));
	}
}

void CTitanInventoryDlg::SetActive(BOOL val)
{
	cDialog::SetActive( val );
	if( val == TRUE )
	{
		SetTitanEquipInfo();
	}	
}

void CTitanInventoryDlg::UpdateData()
{
	char buf[64] = { 0, };
	titan_calc_stats* TitanStatsInfo = TITANMGR->GetTitanStats();	// 타이탄 장착아이템의 각종스텟 계산결과 리스트

	// 타이탄 타입
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, CHATMGR->GetChatMsg(1500), TITANMGR->GetTitanGrade(TITANMGR->GetRegistedTitanItemDBIdx()));
	m_TitanInvenCtrl.titanType->SetStaticText(buf);

	// 타이탄 최소/최대 공격력
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%d ~ %d", TitanStatsInfo->MeleeAttackPowerMin, TitanStatsInfo->MeleeAttackPowerMax);
	m_TitanInvenCtrl.titanAtt->SetStaticText(buf);

	// 타이탄 원거리 공격력
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%d ~ %d", TitanStatsInfo->RangeAttackPowerMin, TitanStatsInfo->RangeAttackPowerMax);
	m_TitanInvenCtrl.titanLongAtt->SetStaticText(buf);

	// 타이탄 외/내공 일격
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%d", TitanStatsInfo->Critical);
	m_TitanInvenCtrl.titanCritical->SetStaticText(buf);

	// 타이탄 사정거리
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%d", TitanStatsInfo->AttackRange);
	m_TitanInvenCtrl.titanDistance->SetStaticText(buf);

	// 타이탄 방어력
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%d", TitanStatsInfo->PhysicalDefense);
	m_TitanInvenCtrl.titanDef->SetStaticText(buf);

	// 타이탄 연료
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%d", TitanStatsInfo->MaxFuel);
	m_TitanInvenCtrl.titanLife->SetStaticText(buf);
/*
	// 타이탄 내력
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%d", TitanStatsInfo->MaxSpell);
	m_TitanInvenCtrl.titanMana->SetStaticText(buf);
*/
	// 타이탄 마법공격(화 수 목 금 토 전부 동일한 수치를 넣었으므로 어떤 값을 넣어도상관없음)
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%.2f", TitanStatsInfo->AttributeAttack.GetElement_Val(ATTR_FIRE));
	m_TitanInvenCtrl.titanMagicAtt->SetStaticText(buf);

	// 타이탄 저항력
	m_TitanInvenCtrl.TitanAttrDef.SetValue(ATTR_FIRE, (WORD)(TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_FIRE)), RGB_HALF(255,255,255));
	m_TitanInvenCtrl.TitanAttrDef.SetValue(ATTR_WATER, (WORD)(TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_WATER)), RGB_HALF(255,255,255));
	m_TitanInvenCtrl.TitanAttrDef.SetValue(ATTR_TREE, (WORD)(TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_TREE)), RGB_HALF(255,255,255));
	m_TitanInvenCtrl.TitanAttrDef.SetValue(ATTR_IRON, (WORD)(TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_IRON)), RGB_HALF(255,255,255));
	m_TitanInvenCtrl.TitanAttrDef.SetValue(ATTR_EARTH, (WORD)(TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_EARTH)), RGB_HALF(255,255,255));
}

BOOL CTitanInventoryDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{

	switch(we)
	{	
	case WE_LBTNDBLCLICK:
		{
			// magi82(26)
			CItem * pItem = NULL;
			if( lId == TITAN_INVENTORY_WEAREDDLG )
			{
				pItem = (CItem*)m_pTitanInvenWearedDlg->GetIconForIdx((WORD)m_pTitanInvenWearedDlg->GetCurSelCellPos());			
				if( !pItem ) return FALSE;
			}
			else if( lId == TITAN_INVENTORY_INVEN_SET_DLG )
			{
				pItem = (CItem*)m_pTitanShopItemDlg->GetIconForIdx((WORD)m_pTitanShopItemDlg->GetCurSelCellPos());			
				if( !pItem ) return FALSE;
			}

			GAMEIN->GetInventoryDialog()->UseItem(pItem);			
		}
		break;
	case WE_LBTNCLICK:
		{
			// magi82(7) - Titan(071024) 타이탄 장착아이템 개별수리
			if( CURSOR->GetCursor() == eCURSOR_TITANREPAIR )
			{
				CItem * pItem = NULL;
				pItem = (CItem*)m_pTitanInvenWearedDlg->GetIconForIdx((WORD)m_pTitanInvenWearedDlg->GetCurSelCellPos());			
				if( !pItem ) return FALSE;

				if(pItem->GetItemKind() & eTITAN_EQUIPITEM)
				{
					TITANMGR->SetTitanRepairItem(pItem);

					// magi82(8) - Titan(071024) 타이탄 장착아이템 개별수리할때 가격 나오게 계산
					TITAN_ENDURANCE_ITEMINFO* pInfo = TITANMGR->GetTitanEnduranceInfo(pItem->GetDBIdx());
					if( pInfo )
					{
						float fEnduranceRate = (float)pInfo->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
						if( fEnduranceRate > 0.0f && fEnduranceRate < 1.0f )
						{
							fEnduranceRate = 1.0f;
						}
						DWORD dwRepairPrice = (DWORD)( (100 - (DWORD)fEnduranceRate) * (pItem->GetItemInfo()->BuyPrice / 100) * 0.09f );

						WINDOWMGR->MsgBox( MBI_TITAN_REPAIR, MBT_YESNO, CHATMGR->GetChatMsg(1538), dwRepairPrice );
						ITEMMGR->SetDisableDialog(TRUE, eItemTable_Titan);
						ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
					}
				}
				else
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1539) );
				}
			}
		}
		break;
	case WE_CLOSEWINDOW:
		{			
			return TRUE;
		}
		break;
	}

	return TRUE;
}

BOOL CTitanInventoryDlg::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	// 아이템인지 체크
	if(icon->GetType() != WT_ITEM)	return FALSE;
	if( m_bDisable ) return FALSE;

	CItem* pFromItem = (CItem*)icon;
	if( pFromItem->IsLocked() )	return FALSE;

	eITEM_KINDBIT bits = pFromItem->GetItemKind();

	//// 마우스 이벤트 발생한곳이 어딘지 체크
	WORD ToPos=0;
	if( !GetPositionForXYRef( x, y, ToPos, bits) )	// 절대위치 넘어 옴
		return FALSE;

	// 목적지 포지션으로 아이템이 이동가능한지 체크
	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return FALSE;

    // 마우스 이벤트 발생한 슬롯에 아이템이 이미 있는지 체크
	CItem * pToItem = NULL;
	if(bits == eSHOP_ITEM_TITAN_EQUIP)
	{		
		pToItem = (CItem *)m_pTitanShopItemDlg->GetIconForIdx(ToPos-TP_TITANSHOPITEM_START);
	}
	else
	{		
		pToItem = (CItem *)m_pTitanInvenWearedDlg->GetIconForIdx(ToPos-TP_TITANWEAR_START);
	}	

	// 만약 목적지 포지션에 아이템이있을때 해당아이템이 옮기려는 아이템의 포지션에 이동가능한지 체크
	if( pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
	{
		return FALSE;
	}

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;

	// 창고 -> 타이탄 슬롯 되면 안됨
	if(FromIdx == eItemTable_Pyoguk)	return FALSE;
	// 펫슬롯 -> 타이탄 슬롯 되면 안됨
	if(FromIdx == eItemTable_PetInven)	return FALSE;

	if(pToItem && (FromIdx == eItemTable_MunpaWarehouse))
	{
		if(FromIdx == eItemTable_GuildWarehouse)
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(53) );
		return FALSE;
	}

	//sound
	if(ITEMMGR->GetTableIdxForAbsPos(ToPos)==eItemTable_Weared)
		AUDIOMGR->Play(59, HERO);
	else
		AUDIOMGR->Play(58, HERO);

 	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return FALSE;

	MSG_ITEM_MOVE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_MOVE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.wFromItemIdx		= pFromItem->GetItemIdx();
	msg.ToPos				= ToPos;
	msg.wToItemIdx			= (pToItem?pToItem->GetItemIdx():0);

	if( msg.FromPos == msg.ToPos )
		return FALSE;

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	NETWORK->Send( &msg, sizeof(msg) );

	return TRUE;
}

BOOL CTitanInventoryDlg::GetPositionForXYRef(LONG x, LONG y, WORD& pos, eITEM_KINDBIT bits)
{
	WORD position=0;										// 상대 위치

	//타이탄 샾아이탬 이면..
	if(bits == eSHOP_ITEM_TITAN_EQUIP)
	{
		if(m_pTitanShopItemDlg->GetPositionForXYRef(x, y, position))
		{
			pos = position+TP_TITANSHOPITEM_START;					// -> 절대 위치로 바꿈
			return TRUE;
		}	
	}
	else	//타이탄 장비...
	{
		if(m_pTitanInvenWearedDlg->GetPositionForXYRef(x, y, position))
		{
			pos = position+TP_TITANWEAR_START;					// -> 절대 위치로 바꿈
			return TRUE;
		}	
	}

	return FALSE;
}

BOOL CTitanInventoryDlg::CanBeMoved(CItem* pItem,POSTYPE pos)
{
	ASSERT(pItem);

	//2007. 11. 15. CBH - 타이탄 탑승 상태에서만 장비 할 수 있게 막는다.
	if(HERO->InTitan() == FALSE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1668) );
		return FALSE;
	}

	if(TP_TITANWEAR_START <= pos && pos < TP_TITANWEAR_END)
	{
		// 타이탄 장착아이템이 아니면 FALSE
		if( ( pItem->GetItemKind() & eTITAN_EQUIPITEM ) )
		{
			// 들어가야 할자리가 아니면 FALSE
			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if(!pInfo) return FALSE;

			if(pos-TP_TITANWEAR_START != pInfo->ItemKind-eTITAN_EQUIPITEM_HELMET)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1505) );
				return FALSE;
			}

			//2007. 10. 25. CBH - 타이탄 장비 착용시 레벨제한 체크					
			if(pInfo->LimitLevel > HERO->GetLevel())
			{
				char temp[256] = {0, };
				sprintf( temp, CHATMGR->GetChatMsg(238), (int)pInfo->LimitLevel );
				CHATMGR->AddMsg( CTC_SYSMSG, temp );
				return FALSE;
			}

			return TRUE;
		}
	}
	else if(TP_TITANSHOPITEM_START <= pos && pos < TP_TITANSHOPITEM_END)
	{
		// 타이탄 장착아이템이 아니면 FALSE
		if( ( pItem->GetItemKind() == eSHOP_ITEM_TITAN_EQUIP ) )
		{
			// 들어가야 할자리가 아니면 FALSE
			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if(!pInfo) return FALSE;

			// magi82(26)
			if( pItem->GetItemParam() & ITEM_PARAM_SEAL )
				return FALSE;

			return TRUE;
		}
	}
	else if(TP_SHOPINVEN_START <= pos && pos < TP_SHOPINVEN_END)
	{
		// 타이탄 장착아이템이 아니면 FALSE
		if( ( pItem->GetItemKind() == eSHOP_ITEM_TITAN_EQUIP ) )
		{
			// 들어가야 할자리가 아니면 FALSE
			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if(!pInfo) return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CTitanInventoryDlg::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	if( TP_TITANWEAR_START <= Pos && Pos < TP_TITANWEAR_END )
	{
		if(!m_pTitanInvenWearedDlg->IsAddable(Pos - TP_TITANWEAR_START))
		{
			return m_pTitanInvenWearedDlg->DeleteItem(Pos - TP_TITANWEAR_START, (cIcon**)ppItem);
		}
		else
			return FALSE;
	}	
	else if( TP_TITANSHOPITEM_START <= Pos && Pos < TP_TITANSHOPITEM_END )
	{
		if(!m_pTitanShopItemDlg->IsAddable(Pos - TP_TITANSHOPITEM_START))
		{
			// magi82(26)
			CItem* pItem = GetItemForPos(Pos);
            if( pItem )
			{
				TITANMGR->SetTitanShopitemOption(pItem->GetItemIdx(), FALSE);
			}

			return m_pTitanShopItemDlg->DeleteIcon(Pos - TP_TITANSHOPITEM_START, (cIcon**)ppItem);
		}
		else
			return FALSE;
	}

	return FALSE;
}

BOOL CTitanInventoryDlg::AddItem( ITEMBASE* pItemBase )
{
	if( pItemBase->dwDBIdx == 0 )
	{
		//		ASSERTMSG( 0, "PetInven NewItem DBIdx is Null");
		return FALSE;
	}
	CItem* pNewItem = ITEMMGR->MakeNewItem( pItemBase, "CTitanInventoryDlg::AddItem" );
	if(pNewItem == NULL)
		return FALSE;

	return AddItem(pNewItem);
}

BOOL CTitanInventoryDlg::AddItem(CItem* pItem)
{
	if( NULL == pItem )
	{
		ASSERT(pItem);
		return FALSE;
	}

	ITEMMGR->RefreshItem(pItem);

	int Pos = pItem->GetPosition();

	if( TP_TITANWEAR_START <= Pos && Pos < TP_TITANWEAR_END )
	{
		return m_pTitanInvenWearedDlg->AddItem(Pos - TP_TITANWEAR_START, pItem);
	}
	else if( TP_TITANSHOPITEM_START <= Pos && Pos < TP_TITANSHOPITEM_END )	
	{
		// magi82(26)
		TITANMGR->SetTitanShopitemOption(pItem->GetItemIdx(), TRUE);

		return m_pTitanShopItemDlg->AddIcon(Pos - TP_TITANSHOPITEM_START, pItem);
	}
	
	return FALSE;	
}

void CTitanInventoryDlg::Render()
{
	if( !m_bActive )		return;

	cDialog::Render();
}

void CTitanInventoryDlg::SetTitanInvenInfo()
{
	if(TITANMGR->GetRegistedTitanItemDBIdx())
	{
		UpdateData();
	}
}

void CTitanInventoryDlg::SetTitanEquipInfo()
{	
	ZeroMemory( m_dwWearInfoArray, sizeof(m_dwWearInfoArray) );

	for( int i = 0; i < eTitanWearedItem_Max; i++ )
	{
		CItem * pItem = (CItem *)m_pTitanInvenWearedDlg->GetIconForIdx(i);
		if( pItem != NULL )
		{
			ITEM_INFO* pinfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			m_dwWearInfoArray[i] = pItem->GetItemIdx();
		}		
	}
}

CItem* CTitanInventoryDlg::GetItemForPos(POSTYPE absPos)
{
	if(TP_TITANWEAR_START <= absPos && absPos < TP_TITANWEAR_END)
	{
		return (CItem*)m_pTitanInvenWearedDlg->GetIconForIdx(absPos-TP_TITANWEAR_START);
	}
	// magi82(26)
	else if(TP_TITANSHOPITEM_START <= absPos && absPos < TP_TITANSHOPITEM_END)
	{
		return (CItem*)m_pTitanShopItemDlg->GetIconForIdx(absPos-TP_TITANSHOPITEM_START);
	}
	else
		return NULL;
}

DWORD CTitanInventoryDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	return we;
}

// magi82(26)
BOOL CTitanInventoryDlg::GetBlankSlotPosition( POSTYPE& pos )
{
	for( int i = 0; i < SLOT_TITANSHOPITEM_NUM; i++ )
	{
		if( m_pTitanShopItemDlg->IsAddable(i) == TRUE )
		{
            pos = TP_TITANSHOPITEM_START + i;
			return TRUE;
		}
	}
	return FALSE;
}
