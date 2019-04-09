#include "stdafx.h"
#include ".\bobusangmanager.h"
#include "GameIn.h"
#include "DealDialog.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "ObjectStateManager.h"
//#include "DealDialog.h"

GLOBALTON(BobusangManager)

BobusangManager::BobusangManager(void)
{
	m_bOnDealWithBobusang = FALSE;
	InitDealItemList();
}

BobusangManager::~BobusangManager(void)
{
}

void BobusangManager::InitDealItemList()
{
	POS pos = m_Dealer.m_DealItemList.GetFirstPos();
	DealerItem* pDealItem = NULL;
	while( pDealItem = m_Dealer.m_DealItemList.GetNextPos(pos) )
	{
		if( pDealItem )
			SAFE_DELETE(pDealItem);
	}
	m_Dealer.m_DealItemList.DeleteAll();
}

void BobusangManager::InitDealItemList( DealerItem* pItem, DWORD Num )
{
	InitDealItemList();

	DealerItem* pDealItem = NULL;
	for( DWORD i = 0; i < Num; ++i )
	{
		pDealItem = new DealerItem;
		*pDealItem = pItem[i];
		pDealItem->Tab = 0;		// 항상 1번 탭만 활성화 (기획상)
	
		m_Dealer.m_DealItemList.AddTail(pDealItem);
	}
}

void BobusangManager::ResetDealItemInfo( DWORD itemIdx, DWORD itemPos, DWORD itemCount )
{
	POS pos = m_Dealer.m_DealItemList.GetFirstPos();
	DealerItem* pDealItem = NULL;
	while( pDealItem = m_Dealer.m_DealItemList.GetNextPos(pos) )
	{
		if( itemIdx == pDealItem->ItemIdx && itemPos == pDealItem->Pos )
		{
			pDealItem->ItemCount = itemCount;
			break;
		}
	}

	//Dealdialog 의 아이템 갱신.
	CDealDialog * pDlg = GAMEIN->GetDealDialog();

	if(!pDlg)
	{
		ASSERT(0);
		return;
	}

	if( pDlg->IsActive() )
	{
		WORD dealerIdx = (WORD)pDlg->GetDealerIdx();
		//pDlg->ShowDealer(dealerIdx);
		pDlg->ResetItemInfo(dealerIdx, itemIdx, itemPos, itemCount);
	}
}

void BobusangManager::SendEndDealMsg()
{
	MSGBASE msg;
	msg.Category = MP_BOBUSANG;
	msg.Protocol = MP_BOBUSANG_LEAVE_GUEST_SYN;
	msg.dwObjectID = HEROID;

	NETWORK->Send(&msg, sizeof(msg));
}

void BobusangManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_BOBUSANG_ALL_DEALITEMINFO_TO_GUEST:
		{
			MSG_ADDABLE_ONEKIND* pmsg = (MSG_ADDABLE_ONEKIND*)pMsg;
			CAddableInfoIterator iter(&pmsg->AddableInfo);

			switch(iter.GetInfoKind())
			{
			case(CAddableInfoList::BobusangItemInfo):
				{
					DealerItem	SellingItem[SLOT_NPCINVEN_NUM];
					iter.GetInfoData(&SellingItem);
					InitDealItemList(SellingItem, pmsg->DataNum);
				}
				break;
			default:
				break;
			}

			if( HERO->GetState() != eObjectState_Die)
				GAMEIN->GetDealDialog()->ShowDealer(BOBUSANG_wNpcUniqueIdx);

		}
		break;
	case MP_BOBUSANG_ADD_GUEST_NACK:
		{
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		}
		break;
	case MP_BOBUSANG_DEALITEMINFO_TO_GUEST:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			ResetDealItemInfo(pmsg->dwData1, pmsg->dwData2, pmsg->dwData3);
		}
		break;
	case MP_BOBUSANG_LEAVE_GUEST_ACK:
		{
			if( HERO->GetState() != eObjectState_Die)
				GAMEIN->GetDealDialog()->SetActive(FALSE);

			SetOnDealing(FALSE);

			InitDealItemList();
		}
		break;
	case MP_BOBUSANG_NOTIFY_FOR_DISAPPEARANCE:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1640));
			return;
		}
		break;
	default:
		break;
	}

}

#ifdef _GMTOOL_ 

void BobusangManager::InitCheatData()
{
	m_bCheatReady	 = FALSE;
	m_flgValidity	 = FALSE;
	memset(&m_BobusangInfo, 0, sizeof(m_BobusangInfo));
}

BOOL BobusangManager::CheckCheatReady()
{
	if( FALSE == m_bCheatReady )
	{
		CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : A new Bobusang is not ready TYPE BOBUREADY..");
		return FALSE;
	}
	return TRUE;
}

void BobusangManager::PrintInfo( BOBUSANGINFO* pInfo, char* pStr /*= "NEW"*/, int state /*= eBBSAS_NONE*/ )
{
	static char str[256] = {0,};
	static char msg[128]	= "[%4s] %2d Channel BOBUSANG %s MAP:%2d POS:%2d SELL:%2d";
	static char msgApr[128] = "APPEARANCE   : %2dY/%2dM/%2dD/%2dH/%2dN";
	static char msgDsa[128] = "DISAPPEARANCE: %2dY/%2dM/%2dD/%2dH/%2dN";
	static char* msgState[BBS_STATE_KINDMAX] = {"□", "○", "●", "▷", "◁", "◈",};

	stTIME app, disapp;
	app.SetTime(pInfo->AppearanceTime);
	disapp.SetTime(pInfo->DisappearanceTime);

	int n,m;
	n = 0;
	m = state +1;
	while(m)
	{
		m /= 2;
		++n;
	}

	CHATMGR->AddMsg( CTC_JACKPOT, msg,
		pStr,
		pInfo->AppearanceChannel + 1,
		msgState[n],
		pInfo->AppearanceMapNum,
		pInfo->AppearancePosIdx + 1,
		pInfo->SellingListIndex + 1);
	CHATMGR->AddMsg( CTC_JACKPOT, msgApr,
		app.GetYear(),
		app.GetMonth(),
		app.GetDay(),
		app.GetHour(),
		app.GetMinute() );
	CHATMGR->AddMsg( CTC_JACKPOT, msgDsa,
		disapp.GetYear(),
		disapp.GetMonth(),
		disapp.GetDay(),
		disapp.GetHour(),
		disapp.GetMinute()
		);
}

#endif