// DealDialog.cpp: implementation of the CDealDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DealDialog.h"
#include "DealItem.h"
#include "ItemManager.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cStatic.h"
#include "./interface/cTabDialog.h"
#include "./interface/cPushupButton.h"
#include "GameIn.h"
#include "ObjectStateManager.h"
#include "cDivideBox.h"
#include "InventoryExDialog.h"
#include "mhFile.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "MHMap.h"
#include "SiegeWarMgr.h"
#include "TitanManager.h"
#include "BobusangManager.h"
#include "NpcScriptDialog.h"
#include "FortWarManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void BuyItem(LONG lId, void * p, DWORD we);

CDealDialog::CDealDialog()
{
	m_type = WT_DEALDIALOG;
	m_pMoneyEdit = NULL;
	m_CurSelectedItemIdx = 0;
	m_lCurSelItemPos = -1;
	m_DealerTable.Initialize(32);			// #define MAX_DEALER_NUM 100
	m_fShow = FALSE;

	m_DealerIdx = 0;
}

CDealDialog::~CDealDialog()
{
	Release();
}


void CDealDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_ICONGRIDDIALOG)
		AddTabSheet(curIdx2++, window);
	else 
		cDialog::Add(window);
}

void CDealDialog::ShowDealer(WORD DealerKey)
{
	if(m_fShow) HideDealer();
	SelectTab(0);
	SetMoney(0, 2);

	//DealerData * pDealer = m_DealerTable.GetData(DealerKey);
	//SW070626 보부상NPC
	DealerData* pDealer = NULL;
	if( DealerKey == BOBUSANG_wNpcUniqueIdx )	//DealerKey는 uniqueId.
	{
		BOBUSANGMGR->SetOnDealing(TRUE);
		pDealer = BOBUSANGMGR->GetBobusangDealer();
	}
	else
	{
		pDealer = m_DealerTable.GetData(DealerKey);
	}

	if(pDealer == NULL)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(197) );
		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		return;
	}

	SetActive(TRUE);
	
	for( int i = 0; i < m_bTabNum; ++i )
	{
		cPushupButton* Btn = GetTabBtn(i);
		Btn->SetActive(FALSE);
	}

	CDealItem* pItem = NULL;
	cImage tmpImage;
	m_DealerIdx = DealerKey;
	DWORD DealIdx = 0;
	DealerItem* pDealItem = NULL;

	POS pos = pDealer->m_DealItemList.GetFirstPos();
	while(pDealItem = pDealer->m_DealItemList.GetNextPos(pos))
	{
		pItem = new CDealItem;
		pItem->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(pDealItem->ItemIdx, &tmpImage),IG_DEALITEM_START+DealIdx++);
		pItem->SetData(pDealItem->ItemIdx);
		pItem->SetItemIdx(pDealItem->ItemIdx);

		//SW070626 보부상NPC
		//갯수표시
		pItem->SetCount(pDealItem->ItemCount);

		//pItem->SetMovable(FALSE);
		ITEMMGR->SetToolTipIcon(pItem);			// toolTip
		//가격툴팁
		char buf[32];
		DWORD ItemPrice = pItem->GetBuyPrice();
		if(MAP->IsVillage() == FALSE)
			ItemPrice = (DWORD)(ItemPrice * 1.2);

		ItemPrice = SWPROFIT->CalTexRateForBuy( ItemPrice );
		// 요새전
		ItemPrice = FORTWARMGR->CalTexRateForBuy( ItemPrice );

		wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma(ItemPrice) );
		//wsprintf( buf, CHATMGR->GetChatMsg(214), pItem->GetBuyPrice() );
		pItem->AddToolTipLine( buf, TTTC_BUYPRICE );

		if( !ITEMMGR->CanEquip(pDealItem->ItemIdx) )
			pItem->SetToolTipImageRGB( TTCLR_ITEM_CANNOTEQUIP );
		WINDOWMGR->AddWindow(pItem);

		cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(pDealItem->Tab);
		dlg->AddIcon(pDealItem->Pos, pItem);

		//SW050819 
		cPushupButton* Btn = GetTabBtn(pDealItem->Tab);
		if( !Btn->IsActive() )
		{
			Btn->SetActive(TRUE);
		}
	}
	GAMEIN->GetInventoryDialog()->SetActive(TRUE);	// taiyo : 유저가 불편할 수 있지 않은가?
	
	if( HERO->GetState() != eObjectState_Deal )
		OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);

//	SetActive(TRUE);

	m_CurSelectedItemIdx	= 0;
	m_lCurSelItemPos		= -1;
	m_fShow = TRUE;

	ITEMMGR->SetPriceToItem( TRUE );
}

void CDealDialog::HideDealer()
{
	if(!m_fShow) return;

	
	cIcon* pIcon;
	for(int tab=0;tab<m_bTabNum;++tab)
	{
		cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tab);
		for(int n=0;n<dlg->m_nCol*dlg->m_nRow;++n)
		{
			pIcon = dlg->GetIconForIdx(n);
			if(pIcon != NULL)
			{
				dlg->DeleteIcon(pIcon);
				//delete pIcon;
				pIcon->SetActive( FALSE );
				WINDOWMGR->AddListDestroyWindow( pIcon );
				pIcon = NULL;
			}
			
		}
	}
	m_DealerIdx = 0;

//KES 위에 WINDOMGR->AddListDestroyWindow( pIcon ) 로 대체.하였음
//process 중에 바로 delete했을시에 문제가 생김.
//	for(DWORD n=0;n<m_DealIdx;++n)
//	{
//		WINDOWMGR->DeleteWindowForID(IG_DEALITEM_START+n);
//	}

//죽은 상태에서 들어올 수도 있다. 체크하는것 추가 필요
	if( HERO->GetState() != eObjectState_Die )
	{
		if( HERO->GetState() == eObjectState_Deal  )
		OBJECTSTATEMGR->EndObjectState(HERO,eObjectState_Deal);
	}

	m_fShow = FALSE;

	ITEMMGR->SetPriceToItem( FALSE );
}


void CDealDialog::LoadDealerItem(CMHFile* fp)
{
	char buff[1024];
	char mapname[256];
	char npcname[256];
//	DWORD DealerKey;
	BOOL IsItem = FALSE;
	WORD ItemIdx;

	//SW070626 보부상NPC
	int	ItemCount = 0;
	
	DealerItem* pItem;
	DealerData* pCurDealer = NULL;
	char seps[]   = "\t\n";
	char* token;
	int count = 0;
	while(1)
	{
		if(fp->IsEOF())
		{
			break;
		}
		count++;
		WORD map_num = fp->GetWord();
		fp->GetString(mapname);
		WORD npc_kind = fp->GetWord();
		fp->GetString(npcname);
		WORD npc_index = fp->GetWord();
		WORD point_x = fp->GetWord();
		WORD point_z = fp->GetWord();
		WORD angle = fp->GetWord();
		BYTE tabnum = fp->GetByte();
		BYTE Pos = 0;
		
		fp->GetLine(buff,1024);

		token = strtok( buff, seps );
		if(token == NULL)
			continue;
		token = strtok( NULL, seps );
		ItemIdx = atoi(token);

		DWORD testIdx = ItemIdx;
		//SW070626 보부상NPC
		// 아이템 판매량 추가	-1: 무제한 0: 판매불가 1~: 갯수(5까지..겹치는 아이템 표시부분에 제한이 걸려있다)
		token = strtok( NULL, seps );
		ItemCount = atoi(token);

		pCurDealer = m_DealerTable.GetData(npc_index);
		if(pCurDealer == NULL)
		{
			pCurDealer = new DealerData;
			m_DealerTable.Add(pCurDealer,npc_index);
		}
		

		if( ItemIdx != 0 )
		{
			pItem = new DealerItem;
			ASSERT(tabnum)
			pItem->Tab = tabnum-1;
			pItem->Pos = Pos++;
			pItem->ItemIdx = ItemIdx;
			pItem->ItemCount = ItemCount;
			pCurDealer->m_DealItemList.AddTail(pItem);
		}
		else
		{
			Pos++;
		}

		while( 1 )
		{
			token = strtok( NULL, seps );
			if(token == NULL)
				break;
			token = strtok( NULL, seps );
			ItemIdx = atoi(token);
			
			//SW070626 보부상NPC
			// 아이템 판매량 추가	-1: 무제한 0: 판매불가 1~: 갯수(5까지..겹치는 아이템 표시부분에 제한이 걸려있다)
			token = strtok( NULL, seps );
			ItemCount = atoi(token);

			if( ItemIdx != 0 )
			{
				pItem = new DealerItem;
				pItem->Tab = tabnum-1;
				pItem->Pos = Pos++;
				pItem->ItemIdx = ItemIdx;
				pItem->ItemCount = ItemCount;
				pCurDealer->m_DealItemList.AddTail(pItem);
			}
			else
			{
				Pos++;
			}
		}		
	}
}
void CDealDialog::Linking()
{
	m_pMoneyEdit = (cStatic *)GetWindowForID(DE_MONEYEDIT);
	m_pMoneyEdit->SetTextXY( 4, 4 );
	m_pMoneyEdit->SetAlign( TXT_RIGHT );
}
void CDealDialog::Release()
{
	DealerData* pDealer = NULL;
	m_DealerTable.SetPositionHead();
	while(pDealer = m_DealerTable.GetData())
	{
		POS pos = pDealer->m_DealItemList.GetFirstPos();
		while( DealerItem* pDealerItem = pDealer->m_DealItemList.GetNextPos( pos ) )
		{
			delete pDealerItem;
		}

		pDealer->m_DealItemList.DeleteAll();
		delete pDealer;
	}
	m_DealerTable.RemoveAll();
}

void CDealDialog::OnSelectedItem()
{	
	cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
	CDealItem* pItem = (CDealItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
	if(!pItem)
	{
		SetMoney(0, 2);
		m_CurSelectedItemIdx	= 0;
		m_lCurSelItemPos		= -1;
		return;
	}

	m_CurSelectedItemIdx	= pItem->GetItemIdx();
	m_lCurSelItemPos		= gridDlg->GetCurSelCellPos();
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(m_CurSelectedItemIdx);
	ASSERT(pItemInfo);

	
	DWORD ItemPrice = pItemInfo->BuyPrice;
	if(MAP->IsVillage() == FALSE)
		ItemPrice = (DWORD)(ItemPrice * 1.2);
	ItemPrice = SWPROFIT->CalTexRateForBuy( ItemPrice );
	// 요새전
	ItemPrice = FORTWARMGR->CalTexRateForBuy( ItemPrice );
	
	SetMoney(ItemPrice, 1);
}
void CDealDialog::Render()
{
	cDialog::RenderWindow();
	cDialog::RenderComponent();
	cTabDialog::RenderTabComponent();
}

void CDealDialog::SetMoney(DWORD value, BYTE colorType)
{
	if(colorType == 0)		//파는가격표시
		m_pMoneyEdit->SetFGColor(RGB_HALF(255,0,0));
	else if(colorType == 1)	//사는가격표시
		m_pMoneyEdit->SetFGColor(TTTC_BUYPRICE);
	else
		m_pMoneyEdit->SetFGColor(RGB_HALF(255,255,255));

	m_pMoneyEdit->SetStaticText( AddComma( value ) );
}

BOOL CDealDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
//	if( icon->GetType() == WT_STALLITEM || icon->GetType() == WT_EXCHANGEITEM || icon->GetType() == WT_DEALITEM ) return FALSE;
	if( icon->GetType() != WT_ITEM ) return FALSE;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ((CItem*)icon)->GetItemIdx() );
	if( !pItemInfo || pItemInfo->ItemKind & eSHOP_ITEM )		return FALSE;
	
	if( icon->GetType() == WT_ITEM )
	{
		CItem * pItem = ( CItem * )icon;

		BYTE tableIdx = ITEMMGR->GetTableIdxForAbsPos(pItem->GetPosition());
		if( tableIdx == eItemTable_PetInven || tableIdx == eItemTable_PetWeared )
			return FALSE;

		if( ITEMMGR->IsDupItem( pItem->GetItemIdx() ) && pItem->GetDurability() > 1 )
		{
			// 나누기 창 띄우기
			//BYTE tableIdx = ITEMMGR->GetTableIdxForAbsPos(pItem->GetPosition());
			m_sellMsg.Category			= MP_ITEM;
			m_sellMsg.Protocol			= MP_ITEM_SELL_SYN;
			m_sellMsg.dwObjectID		= HEROID;

			m_sellMsg.TargetPos			= pItem->GetPosition();
			m_sellMsg.SellItemNum		= (WORD)pItem->GetDurability();
			m_sellMsg.wSellItemIdx		= pItem->GetItemIdx();
			m_sellMsg.wDealerIdx		= m_DealerIdx;

			cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_SELL, (LONG)pItem->GetAbsX(), (LONG)pItem->GetAbsY(),
				OnFakeSellItem, OnCancelSellItem, this, pItem, CHATMGR->GetChatMsg(186) );
			pDivideBox->SetValue( m_sellMsg.SellItemNum );
			pDivideBox->SetMaxValue( MAX_YOUNGYAKITEM_DUPNUM );	//define?	//confirm
			//일단 디바이드 박스 에서만 막음. 차후에 메세지 보냈을때 막는 방법 고려해서 추가.
			SetDisable( TRUE );
			GAMEIN->GetInventoryDialog()->SetDisable( TRUE );
			return FALSE;
		}
		else
		{
			SetDisable( TRUE );
			GAMEIN->GetInventoryDialog()->SetDisable( TRUE );

			m_sellMsg.Category			= MP_ITEM;
			m_sellMsg.Protocol			= MP_ITEM_SELL_SYN;
			m_sellMsg.dwObjectID		= HEROID;

			m_sellMsg.TargetPos			= pItem->GetPosition();
			m_sellMsg.SellItemNum		= (WORD)pItem->GetDurability();
			m_sellMsg.wSellItemIdx		= pItem->GetItemIdx();
			m_sellMsg.wDealerIdx		= m_DealerIdx;
			
			DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
			// 요새전
			SellPrice = FORTWARMGR->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );

//			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_SELLITEM, MBT_YESNO, CHATMGR->GetChatMsg( 164 ), 
//				pItem->GetItemInfo()->ItemName, AddComma( pItem->GetItemInfo()->SellPrice ) );
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_SELLITEM, MBT_YESNO, CHATMGR->GetChatMsg( 164 ),
				pItem->GetItemInfo()->ItemName, AddComma( SellPrice ) );
							
			return FALSE;
		}
	}
	return FALSE;
}

void CDealDialog::OnFakeSellItem( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CDealDialog * tDlg = ( CDealDialog * )vData1;
	CItem* pItem = (CItem*)vData2;
	ASSERT( pItem );

	if( param1 == 0 ) 
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Deal);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		return;
	}

	tDlg->m_sellMsg.SellItemNum = (WORD)((tDlg->m_sellMsg.SellItemNum > param1 ? param1 : tDlg->m_sellMsg.SellItemNum));

	WORD SellNum = tDlg->m_sellMsg.SellItemNum;
	if(ITEMMGR->IsOptionItem( tDlg->m_sellMsg.wSellItemIdx, tDlg->m_sellMsg.SellItemNum ))
		SellNum = 1;

	char temp[32];
//	SafeStrCpy( temp, AddComma( pItem->GetItemInfo()->SellPrice ), 32 );
//	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_SELLITEM, MBT_YESNO, CHATMGR->GetChatMsg( 165 ), 
//		pItem->GetItemInfo()->ItemName, SellNum, temp, AddComma( pItem->GetItemInfo()->SellPrice * SellNum ) );

	DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
	// 요새전
	SellPrice = FORTWARMGR->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
	SafeStrCpy( temp, AddComma( SellPrice ), 32 );
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_SELLITEM, MBT_YESNO, CHATMGR->GetChatMsg( 165 ), 
		pItem->GetItemInfo()->ItemName, SellNum, temp, AddComma( SellPrice * SellNum ) );
}

void CDealDialog::OnCancelSellItem( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CDealDialog * tDlg = ( CDealDialog * )vData1;

	tDlg->SetDisable( FALSE );
	GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
}


void CDealDialog::SendSellItemMsg()
{
	// m_sellMsg다시 한번 점검
	//m_sellMsg.TargetPos			= pItem->GetPosition();
	//m_sellMsg.SellItemNum			= pItem->GetDurability();
	//m_sellMsg.wSellItemIdx		= pItem->GetItemIdx();
	WORD TargetTableIdx = ITEMMGR->GetTableIdxForAbsPos(m_sellMsg.TargetPos);
	CItem * pTargetItem = ITEMMGR->GetItemofTable(TargetTableIdx, m_sellMsg.TargetPos);
	if(!pTargetItem)
	{
		OnCancelSellItem(0, NULL, 0, this, NULL);
		return ;
	}

	if(m_sellMsg.wSellItemIdx != pTargetItem->GetItemIdx())
	{
		OnCancelSellItem(0, NULL, 0, this, NULL);
		return ;
	}
	if(m_sellMsg.SellItemNum > pTargetItem->GetDurability())
	{
		OnCancelSellItem(0, NULL, 0, this, NULL);
		return ;
	}
	// magi82 - Titan(070604)
	// 등록된 증서는 팔수없다.
	if(pTargetItem->GetDBIdx() == TITANMGR->GetRegistedTitanItemDBIdx())
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1552) );
		OnCancelSellItem(0, NULL, 0, this, NULL);
		return;
	}

	NETWORK->Send( &m_sellMsg, sizeof(m_sellMsg) );
}
/*

void CDealDialog::CancelSellItem()
{
	SetDisable( FALSE );
	GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
}

void CDealDialog::FakeSellItem( LONG mouseX, LONG mouseY, CItem * pItem, DURTYPE Dur )
{
	POSTYPE Pos = pItem->GetPosition();

	MSG_ITEM_SELL_SYN msg;
	msg.Category		= MP_ITEM;
	msg.Protocol		= MP_ITEM_SELL_SYN;
	msg.dwObjectID		= HEROID;
	msg.TargetPos		= Pos;
	msg.wSellItemIdx	= pItem->GetItemIdx();
	msg.SellItemNum		= Dur;

	NETWORK->Send(&msg,sizeof(msg));

}*/


void CDealDialog::OnSellPushed()
{
	CItem * pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(INV);
	if(pItem)
		FakeMoveIcon( (LONG)(pItem->GetAbsX() + 20), (LONG)(pItem->GetAbsY() + 20), (cIcon *)pItem);
}

void CDealDialog::OnFakeBuyItem( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
/*	// 살수 있는지 체크 필
	tDlg->m_buyMsg.BuyItemNum	= param1;
	NETWORK->Send( &tDlg->m_buyMsg, sizeof(tDlg->m_buyMsg) );
*/
	if( param1 == 0 )
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Deal);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		return;
	}

	CDealDialog * tDlg = ( CDealDialog * )vData1;
	CDealItem* pItem = (CDealItem*)vData2;
	ASSERT( pItem );

	tDlg->m_buyMsg.BuyItemNum	= (WORD)param1;
	
	
	MONEYTYPE BuyPrice = pItem->GetBuyPrice();
	if(MAP->IsVillage() == FALSE)
		BuyPrice = (MONEYTYPE)(BuyPrice * 1.2);
	BuyPrice = SWPROFIT->CalTexRateForBuy( BuyPrice );
	// 요새전
	BuyPrice = FORTWARMGR->CalTexRateForBuy( BuyPrice );
	char temp[32];	//AddComma를 2번쓰지 마시오.
	SafeStrCpy( temp, AddComma( BuyPrice ), 32 );

	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_BUYITEM, MBT_YESNO, CHATMGR->GetChatMsg( 166 ), 
		pItem->GetItemName(), tDlg->m_buyMsg.BuyItemNum, 
		temp, AddComma( BuyPrice * tDlg->m_buyMsg.BuyItemNum ) );
}

void CDealDialog::OnCancelBuyItem( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CDealDialog * tDlg = ( CDealDialog * )vData1;
	tDlg->SetDisable( FALSE );
	GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
}

void CDealDialog::OnBuyPushed()
{
	if(m_lCurSelItemPos == -1)
		return;
	
	cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
	CDealItem* pItem = (CDealItem*)gridDlg->GetIconForIdx((WORD)m_lCurSelItemPos);

	if( pItem )
	{
		FakeBuyItem( (LONG)(pItem->GetAbsX()+20), (LONG)(pItem->GetAbsY()+20), pItem);
	}
}


void CDealDialog::SendBuyItemMsg()
{
	NETWORK->Send( &m_buyMsg, m_buyMsg.GetSize() );
}

void CDealDialog::CancelBuyItem()
{
	SetDisable( FALSE );
	GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
}

void CDealDialog::FakeBuyItem(LONG x, LONG y, CDealItem* pItem)
{
	WORD itemIdx = pItem->GetItemIdx();
	if( itemIdx == 0 ) return;

	if( 0 == pItem->GetCount() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1641) );
		return;
	}

	memset(&m_buyMsg, 0, sizeof(m_buyMsg));

	if( ITEMMGR->IsDupItem( itemIdx ) )
	{
		// 나누기 창 띄우기
		m_buyMsg.Category		= MP_ITEM;
		m_buyMsg.Protocol		= MP_ITEM_BUY_SYN;
		m_buyMsg.dwObjectID		= HEROID;
		m_buyMsg.wBuyItemIdx	= itemIdx;
		m_buyMsg.wDealerIdx		= m_DealerIdx;
		//m_buyMsg.BuyItemNum		= 0;

		cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_BUY, x, y, OnFakeBuyItem, OnCancelBuyItem,
														this, pItem, CHATMGR->GetChatMsg(187) );
		if( !pDivideBox )
			return;

		pDivideBox->SetMaxValue( MAX_ITEMBUY_NUM );
		pDivideBox->SetValue(1);

		SetDisable( TRUE );
		GAMEIN->GetInventoryDialog()->SetDisable( TRUE );

	}
	else
	{
		if( HERO->GetMoney() < pItem->GetBuyPrice() )
		{
			//돈이 부족합니다.
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
			return;			
		}

		//보부상과 거래일 경우 교환권 체크 & 정보 세팅
		if( FALSE == SetDemandItem() )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1639) );
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
			GAMEIN->GetInventoryDialog()->SetActive(FALSE);
			return;
		}

		SetDisable( TRUE );
		GAMEIN->GetInventoryDialog()->SetDisable( TRUE );

		m_buyMsg.Category		= MP_ITEM;
		m_buyMsg.Protocol		= MP_ITEM_BUY_SYN;
		m_buyMsg.dwObjectID		= HEROID;
		m_buyMsg.wBuyItemIdx	= itemIdx;
		m_buyMsg.wDealerIdx		= m_DealerIdx;
		m_buyMsg.BuyItemNum		= 1;

		//msgbox띠우기
		MONEYTYPE BuyPrice = pItem->GetBuyPrice();
		if(MAP->IsVillage() == FALSE)
			BuyPrice = (MONEYTYPE)(BuyPrice*1.2);
		BuyPrice = SWPROFIT->CalTexRateForBuy( BuyPrice );
		// 요새전
		BuyPrice = FORTWARMGR->CalTexRateForBuy( BuyPrice );
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_BUYITEM, MBT_YESNO, CHATMGR->GetChatMsg( 167 ), 
			pItem->GetItemName(), AddComma( BuyPrice ) );

	}
}

void CDealDialog::SetActive(BOOL val)
{
	if( m_bDisable ) return;	

	if(val == FALSE)
	{
		if( m_DealerIdx == BOBUSANG_wNpcUniqueIdx )
		{
			if( TRUE == BOBUSANGMGR->GetOnDealing() )
				BOBUSANGMGR->SetOnDealing(FALSE);
			BOBUSANGMGR->SendEndDealMsg();
		}

		HideDealer();
	}
	cTabDialog::SetActive(val);
}
void CDealDialog::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if(/*we == WE_RBTNCLICK ||*/ we == WE_LBTNDBLCLICK)
	{
		OnBuyPushed();
	}
}

void CDealDialog::ResetItemInfo( DWORD npcIdx, DWORD itemIdx,  DWORD itemPos, DWORD count )
{
	DealerData* pDealer = NULL;

	if( npcIdx == BOBUSANG_wNpcUniqueIdx )	//DealerKey는 uniqueId.
	{
		pDealer = BOBUSANGMGR->GetBobusangDealer();
	}
	else
	{
		pDealer = m_DealerTable.GetData(npcIdx);
	}
	if( !pDealer )
	{
		ASSERT(0);
		return;
	}
	DealerItem* pItem = NULL;
	POS pos = pDealer->m_DealItemList.GetFirstPos();

	while( pItem = pDealer->m_DealItemList.GetNextPos(pos) )
	{
		if( itemPos == pItem->Pos && itemIdx == pItem->ItemIdx )
		{
			pItem->ItemCount = count;

			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			CDealItem* pItem = (CDealItem*)gridDlg->GetIconForIdx((WORD)itemPos);

			pItem->SetCount((int)count);

			break;
		}
	}
}

BOOL CDealDialog::SetDemandItem()
{// !!!추후 함수 확장. 필요아이템 갯수 충족시 까지 인벤 검색하여 세팅.
	CItem* pItem = NULL;

	WORD demandItemIdx = BOBUSANG_ITEM_TICKET;
#define EX_BOBUSANG_DEMAND_TICKETNUM	1
	DWORD demandNum = EX_BOBUSANG_DEMAND_TICKETNUM;

	if( m_DealerIdx == BOBUSANG_wNpcUniqueIdx )
	{
		CInventoryExDialog* pInven = GAMEIN->GetInventoryDialog();
		BYTE TabNum = pInven->GetTabNum();
		for( BYTE i = 0 ; i < TabNum ; ++i )
		{
			cIconGridDialog* gridDlg = (cIconGridDialog*)(pInven->GetTabSheet(i));

			for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
			{
				if(!gridDlg->IsAddable(j) )
				{
					CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START+j/*+TABCELL_INVENTORY_NUM*i*/);
					if(pItem->GetItemIdx() == demandItemIdx)
					{
						DWORD dur = 1;
						if( pItem->GetItemKind() & eYOUNGYAK_ITEM )
						{
							dur = pItem->GetDurability();
						}

						if( demandNum > dur )
						{
							m_buyMsg.AddDemandSlot(pItem->GetItemIdx(), pItem->GetPosition(), dur);
							demandNum -= dur;

							if( 0 == demandNum )
								return TRUE;
						}
						else
						{
							m_buyMsg.AddDemandSlot(pItem->GetItemIdx(), pItem->GetPosition(), demandNum);
							return TRUE;
						}
					}
				}
			}
		}
		return FALSE;
	}
	return TRUE;
}
