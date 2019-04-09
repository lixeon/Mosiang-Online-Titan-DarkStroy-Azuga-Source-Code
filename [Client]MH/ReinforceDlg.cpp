// ReinforceDlg.cpp: implementation of the CReinforceDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReinforceDlg.h"
#include "Item.h"
#include "INTERFACE\cIconDialog.h"
#include "INTERFACE\cIconGridDialog.h"
#include "WindowIDEnum.h"
#include "Gamein.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "ReinforceManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cWindowManager.h"
#include "ObjectManager.h"
#include "./Interface/cStatic.h"
#include "ReinforceDataGuideDlg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _GMTOOL_
#include "CheatMsgParser.h"
#endif

CReinforceDlg::CReinforceDlg()
{
	m_pReinforceDlg = NULL;
	m_pReinforceGridDlg = NULL;
	m_pReinforceGravityDlg = NULL;

	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	for(int i = 0 ; i < MAX_REINFORCEGRID_NUM+1 ; ++i)
	{
		m_VirtualItem[i].SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
		m_VirtualItem[i].SetMovable(FALSE);
	}
	m_bShopItemUse	= FALSE;
	m_ShopItemIdx	= 0;
	m_ShopItemPos	= 0;
	m_pTitle		= NULL;
	memset( m_Title, 0, sizeof(char)*128 );
}

CReinforceDlg::~CReinforceDlg()
{
	// don't call Release() 
	// pre delete item 
}
void CReinforceDlg::Release(eReinforceReleaseOpt op)
{
	CVirtualItem * pVItem = NULL;

	if((op & eReinforceBasicRelease) && !IsEmpty(0))
	{
		m_pReinforceDlg->DeleteIcon(0, (cIcon **)&pVItem);
		pVItem->GetLinkItem()->SetLock(FALSE);
		pVItem->SetLinkItem(NULL);
	}


	if( (op & eReinforceViewRelease) )
	{
		for(int i = 1 ; i < MAX_REINFORCEGRID_NUM+1 ; ++i)
		{
			if(!IsEmpty(i))
			{
				m_pReinforceGridDlg->DeleteIcon(i-1, (cIcon **)&pVItem);
				pVItem->GetLinkItem()->SetLock(FALSE);
				pVItem->SetLinkItem(NULL);
			}
		}		
	}
	
	REINFORCEMGR->SetRareMaterialTotalGravity(0);	//재료 총 비중 초기화.
}
void CReinforceDlg::Linking()
{
	m_pReinforceDlg		= (cIconDialog *)GetWindowForID(ITR_REINFORCEICONDLG);
	m_pReinforceGridDlg = (cIconGridDialog *)GetWindowForID(ITR_REINFORCEICONGRIDDLG);
	m_pTitle = (cStatic *)GetWindowForID( ITR_REINFORCETITLE );

	if( m_pTitle )
		strcpy( m_Title, m_pTitle->GetStaticText() );

	m_pReinforceGravityDlg = (cStatic*)GetWindowForID(ITR_REINFORCEGRAVITY);
	cImage ToolTipImage;
	SCRIPTMGR->GetImage( 63, &ToolTipImage, PFT_HARDPATH );
	m_pReinforceGravityDlg->SetToolTip( CHATMGR->GetChatMsg(1204), RGBA_MAKE(255, 255, 255, 255), &ToolTipImage, TTCLR_DEFAULT );
}

void CReinforceDlg::SetActiveRecursive(BOOL val)
{
	cDialog::SetActiveRecursive( val );

	if( val )
	{
		if( m_bShopItemUse )
		{
			char buf [256];
			sprintf( buf, "%s - (%s)", m_Title, CHATMGR->GetChatMsg( 1091) );
			m_pTitle->SetStaticText( buf );
		}
		else
			m_pTitle->SetStaticText( m_Title );

		SetGravityText();
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

BOOL CReinforceDlg::IsEmpty(POSTYPE pos)
{
	if(pos == 0)
	{
		if(m_pReinforceDlg->IsAddable(0))
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if(m_pReinforceGridDlg->IsAddable(pos-1))
			return TRUE;
		else
			return FALSE;
	}
}
BOOL CReinforceDlg::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if(pOrigIcon->GetType() != WT_ITEM) return FALSE;
	if( m_bDisable ) return FALSE;

	CItem * pOrigItem = (CItem *)pOrigIcon;
	if( pOrigItem->IsLocked() ) return FALSE;

	ITEM_INFO* pBaseItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx() );
	if( !pBaseItemInfo )			return FALSE;

	// 06.09.25 RaMa - 이벤트아이템을 강화, 레어를 못 만들도록
	if( pBaseItemInfo->WeaponType > WP_KEY )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1455) );
		return FALSE;
	}

	// 인벤토리이외에 아이템 FALSE
	if(!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()))
		return FALSE;

	WORD OptKind = REINFORCEMGR->GetTargetOptionKind();	//대상 아이템의 옵션 종류를 가져온다.

	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if(bits & eEQUIP_ITEM)
	{
		if( REINFORCEMGR->GetRareMaterialTotalGravity() )	//등록된 재료가 있으면(기준) 대상 아이템 변경이 불가하다.
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1205) );
			return FALSE;
		}
		else
		{
			if( ITEMMGR->IsRareOptionItem( pOrigItem->GetItemIdx(), pOrigItem->GetRareness() ) )
			{
				REINFORCEMGR->SetTatgetOptionKind(eIOK_Rare);
			}
			else
			{
				REINFORCEMGR->SetTatgetOptionKind(eIOK_Normal);	//일반 아이템
			}
		}


		//SW051011 강화 치트
#ifdef _GMTOOL_
		if(CHEATMGR->m_bReadyToOptionCheat)
		{
			CHEATMGR->m_wTargetItemIdx = pOrigItem->GetItemIdx();
			CHEATMGR->m_ItemAbsPos	= pOrigItem->GetPosition();
			CHATMGR->AddMsg( CTC_CHEAT_2, "OK... TargetItem for ReinforceOption is Registered");
		}
#endif

		if( m_bShopItemUse )
		{
			ITEM_OPTION_INFO * pInfo = ITEMMGR->GetItemOption( pOrigItem->GetDurability() );
			if( pInfo->GetOptionType() == 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1081) );
				return FALSE;
			}
			/*else if( pInfo->GetOptionCount() > 1 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1090) );
				return FALSE;
			}*/
			ITEM_INFO* pShopItem = ITEMMGR->GetItemInfo( (WORD)m_ShopItemIdx );
			if( !pShopItem )				return FALSE;
			if( !(pShopItem->ItemKind & eSHOP_ITEM) )		return FALSE;
			if( pBaseItemInfo->LimitLevel < pShopItem->GenGol || pBaseItemInfo->LimitLevel > pShopItem->MinChub )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
				return FALSE;
			}
		}

		if(!IsEmpty(0))
		{
			Release(eReinforceBasicRelease);
		}
		AddVirtualItemWrap(0, pOrigItem);

		// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
		switch(bits)
		{
		case eEQUIP_ITEM_WEAPON :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(0);
			}
			break;
		case eEQUIP_ITEM_DRESS :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(2);
			}
			break;
		case eEQUIP_ITEM_HAT :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(1);
			}
			break;
		case eEQUIP_ITEM_SHOES :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(3);
			}
			break;
		case eEQUIP_ITEM_RING :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(8);
			}
			break;
		case eEQUIP_ITEM_CAPE :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(4);
			}
			break;
		case eEQUIP_ITEM_NECKLACE :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(7);
			}
			break;
		case eEQUIP_ITEM_ARMLET	:
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(5);
			}
			break;
		case eEQUIP_ITEM_BELT :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(6);
			}
			break;
		}
	}
	// ReinforceInfo must exist
	else if( bits == eEXTRA_ITEM_JEWEL )
	{
		CVirtualItem* pVItem = GetVirtualItem(0);
		if( pVItem )
		{
			// RaMa - 아이템몰 아이템을 사용했을경우
			if( m_bShopItemUse )
			{
				// 기존에 올라간 아이템이랑 같은 종류인지 확인
				for(int i=1; i<MAX_REINFORCEGRID_NUM+1; ++i)
				{
					if( !IsEmpty(i) )
					if( m_VirtualItem[i].GetSrcItemIdx() != pOrigItem->GetItemIdx() )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1080) );							
						return FALSE;
					}
				}
				// 강화가 될 수 있는지 확인
				if( !REINFORCEMGR->CheckReinforceItemWithMeterial( pVItem->GetSrcPosition(), pOrigItem->GetItemIdx(), OptKind ) )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(680) );
					return FALSE;
				}
			}

			//대상아이템 - 재료 옵션 체크
			if( !REINFORCEMGR->CheckValidMaterial( pVItem->GetSrcItemIdx(), pOrigItem->GetItemIdx(), OptKind ) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(680) );
				//강화할 아이템과 맞지 않는 재료입니다.
				return FALSE;
			}

			//재료의 비중 체크
			int OverGravity = REINFORCEMGR->CheckMaterialTotalGravity( pOrigItem->GetItemIdx(), (WORD)pOrigItem->GetDurability(), OptKind );
			if( OverGravity > 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1203), OverGravity );
				//재료의 총 중량이 %d 초과 되었습니다.
				return FALSE;
			}

//			char cTemp[64] = {0,};
//			sprintf(cTemp, "%4d/100", REINFORCEMGR->GetRareMaterialTotalGravity());
//			m_pReinforceGravityDlg->SetStaticText(cTemp);
			SetGravityText();

			BOOL bEmpty = FALSE;
			for(int i = 1 ; i < MAX_REINFORCEGRID_NUM +1 ; ++i)
			{
				if(IsEmpty(i))
				{
					bEmpty = TRUE;
					break;
				}
			}
			if(!bEmpty)	return FALSE;

			AddVirtualItemWrap(i, pOrigItem);
		}
		else
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(222) );
			//"먼저 강화할 아이템을 올려주세요"
			return FALSE;
		}
	}
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(220) );
		return FALSE;
	}


	pOrigItem->SetLock(TRUE);

	return FALSE;
}
CVirtualItem * CReinforceDlg::GetVirtualItem(POSTYPE relPos)
{
	if(relPos == 0)
	{
		return (CVirtualItem *)m_pReinforceDlg->GetIconForIdx(0);
	}
	else
	{
		return (CVirtualItem *)m_pReinforceGridDlg->GetIconForIdx(relPos-1);
	}
}
BOOL CReinforceDlg::AddVirtualItemWrap(POSTYPE relPos, CItem * pItem)
{
	m_VirtualItem[relPos].SetData(pItem->GetItemIdx());
	m_VirtualItem[relPos].SetLinkItem(pItem);
//	if(pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()))
//		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[relPos], ITEMMGR->GetItemOption(pItem->GetDurability()));
	//!!! NULL 확인 SW050920 Rare
	if( pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()) )
	{
		ITEMMGR->SetToolTipIcon( (cIcon*)&m_VirtualItem[relPos], ITEMMGR->GetItemOption(pItem->GetDurability()),
			ITEMMGR->GetItemRareOption(pItem->GetRareness()) );
	}
	else
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[relPos], NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()));
	return AddVirtualItem(relPos, &m_VirtualItem[relPos]);
}

BOOL CReinforceDlg::AddVirtualItem(POSTYPE relPos, CVirtualItem * pItem)
{
	//empty 체크후 
	if(relPos == 0)
	{
		if(IsEmpty(0))
		{
			m_pReinforceDlg->AddIcon(0, (cIcon *)pItem);
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		if(IsEmpty(relPos))
		{
			m_pReinforceGridDlg->AddIcon(relPos-1, (cIcon *)pItem);
			return TRUE;
		}
		return FALSE;
	}
}

DWORD CReinforceDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	for(int i = 0 ; i < MAX_REINFORCEGRID_NUM+1 ; ++i)
	{
		m_VirtualItem[i].ActionEvent(mouseInfo);
	}

	return we;
}

BOOL CReinforceDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(we)
	{
	case WE_CLOSEWINDOW:
		{
			CReinforceDlg * pReinforceDlg = (CReinforceDlg*)WINDOWMGR->GetWindowForID(lId);
			if( pReinforceDlg->m_bShopItemUse )
			{
				CItem* pItem = ITEMMGR->GetItemofTable( eItemTable_ShopInven, (POSTYPE)pReinforceDlg->m_ShopItemPos );
				if( pItem )
					pItem->SetLock( FALSE );

				pReinforceDlg->m_bShopItemUse	= FALSE;
				pReinforceDlg->m_ShopItemIdx	= 0;
				pReinforceDlg->m_ShopItemPos	= 0;
			}
			pReinforceDlg->Release();
			GAMEIN->GetReinforceDataGuideDlg()->Close();
		}
		break;
	}

	switch( lId )
	{
	case ITR_REINFORCEOKBTN:
		{
			CReinforceDlg * pReinforceDlg = (CReinforceDlg *)p;
			CReinforceDlg::ReinforceOkBtn(pReinforceDlg);
		}
		break;
	case ITR_REINFORCECANCELBTN:
		{
			CReinforceDlg * pReinforceDlg = (CReinforceDlg *)p;
			CReinforceDlg::ReinforceCancelBtn(pReinforceDlg);
		}
		break;
	// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
	case ITR_RFGUIDEBTN:
		{
			GAMEIN->GetReinforceDataGuideDlg()->Show();
		}
		break;
	}

	return TRUE;
}

void CReinforceDlg::ReinforceOkBtn(CReinforceDlg * pThis)
{
	if(pThis->IsEmpty(0))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(222) );
		return;
	}
	if( pThis->m_bShopItemUse )
	{
		pThis->ReinforceOkBtnWithShopItem( pThis );
		return;
	}

	MSG_ITEM_REINFORCE_SYN msg;
	msg.Init(MP_ITEM, MP_ITEM_REINFORCE_SYN, HEROID);
	CVirtualItem * pTargetItem = (CVirtualItem *)pThis->m_pReinforceDlg->GetIconForIdx(0);
	msg.wTargetItemIdx	= pTargetItem->GetSrcItemIdx();
	msg.TargetPos		= pTargetItem->GetSrcPosition();

	for(int i = 1 ; i < MAX_REINFORCEGRID_NUM +1 ; ++i)
	{
		if(!pThis->IsEmpty(i))
		{
			CVirtualItem * pJewelItem = (CVirtualItem *)pThis->m_pReinforceGridDlg->GetIconForIdx(i-1);
			msg.AddJewelInfo(pJewelItem->GetSrcItemIdx(), pJewelItem->GetSrcPosition(), pJewelItem->GetDurability() );
		}
	}	
	if(msg.wJewelUnit == 0)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(224) );
		return ;
	}
	NETWORK->Send(&msg, msg.GetSize());

	pThis->SetDisable(TRUE);
}

void CReinforceDlg::ReinforceOkBtnWithShopItem(CReinforceDlg * pThis)
{
	MSG_ITEM_REINFORCE_WITHSHOPITEM_SYN msg;
	msg.Init( MP_ITEM, MP_ITEM_REINFORCE_WITHSHOPITEM_SYN, HEROID );
	CVirtualItem * pTargetItem = (CVirtualItem *)pThis->m_pReinforceDlg->GetIconForIdx(0);
	msg.wTargetItemIdx	= pTargetItem->GetSrcItemIdx();
	msg.TargetPos		= pTargetItem->GetSrcPosition();
	msg.wShopItemIdx	= (WORD)m_ShopItemIdx;
	msg.ShopItemPos		= (POSTYPE)m_ShopItemPos;
	for(int i=1; i<MAX_REINFORCEGRID_NUM+1; ++i)
	{
		if(!pThis->IsEmpty(i))
		{
			CVirtualItem * pJewelItem = (CVirtualItem *)pThis->m_pReinforceGridDlg->GetIconForIdx(i-1);
			msg.AddJewelInfo(pJewelItem->GetSrcItemIdx(), pJewelItem->GetSrcPosition(), pJewelItem->GetDurability() );
		}
	}	
	if(msg.wJewelUnit == 0)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(224) );
		return ;
	}
	NETWORK->Send(&msg, msg.GetSize());
	
	pThis->SetDisable(TRUE);
}

void CReinforceDlg::ReinforceCancelBtn(CReinforceDlg * pThis)
{
	pThis->Release();
	pThis->SetActiveRecursive(FALSE);
	// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
    GAMEIN->GetReinforceDataGuideDlg()->Close();

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(223) );
}



void CReinforceDlg::DieCheck()
{
	if( HERO->GetState() == eObjectState_Die )
	{
		Release();
		SetActiveRecursive( FALSE );
	}
}


void CReinforceDlg::SetShopItemInfo( DWORD ShopItemIdx, DWORD ShopItemPos )
{
	m_ShopItemIdx = ShopItemIdx;
	m_ShopItemPos = ShopItemPos;
	m_bShopItemUse = TRUE;
}

void CReinforceDlg::SetGravityText()
{
	char cTemp[64] = {0,};
	sprintf(cTemp, "%4d/100", REINFORCEMGR->GetRareMaterialTotalGravity());
	m_pReinforceGravityDlg->SetStaticText(cTemp);	
}
