#include "stdafx.h"
#include ".\bobusangmanager.h"
#include "UserTable.h"
#include "ItemManager.h"
#include "Player.h"
#include "Npc.h"
#include "ChannelSystem.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "RegenNPC.h"
#include "Network.h"
#include "QuestMapMgr.h"
#include "ChannelSystem.h"

GLOBALTON(BobusangManager)

BobusangManager::BobusangManager(void)
{
}

BobusangManager::~BobusangManager(void)
{
	// magi82(14) - Memory(071027) 메모리 누수 수정
//	BobusangMgr_Release();
}

void BobusangManager::BobusangMgr_Init()
{
	m_pBobusang = NULL;
	DWORD channelCnt = CHANNELSYSTEM->GetChannelCount();
	m_pBobusang = new BOBUSANGTOTALINFO[channelCnt];	// rama's tip!
}

void BobusangManager::BobusangMgr_Process()
{

}

void BobusangManager::BobusangMgr_Release()
{
	DWORD channelCnt = CHANNELSYSTEM->GetChannelCount();

	for( DWORD i = 0; i < channelCnt; ++i )
	{
		RemoveBobusangNpc(i);
	}
	// magi82(14) - Memory(071027) 메모리 누수 수정
	SAFE_DELETE_ARRAY(m_pBobusang);
}

BOOL BobusangManager::MakeNewBobusangNpc( BOBUSANGINFO* pBobusangInfo )
{
	if( !g_bReady )
		return FALSE;

	if( !pBobusangInfo )
	{
		ASSERT(0);
		return FALSE;
	}

	//SetBobusanInfo( pBobusangInfo);
	DWORD channelNum = pBobusangInfo->AppearanceChannel;

	//2008. 5. 19. CBH - 맵의 채널 갯수가 틀리므로 MAX 채널보다 크면 보부상이 생성 되면 안된다.
	if(channelNum > CHANNELSYSTEM->GetChannelCount()-1)
		return FALSE;

	BOBUSANGTOTALINFO* pInfo = &m_pBobusang[channelNum];

	if(pInfo->pBobusang)
	{
		RemoveBobusangNpc(channelNum);
	}

	//BOBUSANGINFO		AppearanceInfo;
	pInfo->AppearanceInfo = *pBobusangInfo;
	//CNpc*				pBobusang;
	NPC_LIST* Npclist;
	BASEOBJECT_INFO Baseinfo;
	NPC_TOTALINFO Npcinfo;
	/*STATIC_NPCINFO* pNpcInfo = GAMERESRCMNGR->GetStaticNpcInfo(BOBUSANG_NPCIDX);

	if( !pNpcInfo )
	{
		ASSERT(0);
		return FALSE;
	}*/

	Npclist = GAMERESRCMNGR->GetNpcInfo(BOBUSANG_NPCIDX);
	REGENNPC_OBJ->AddNpcObjectID();
	Baseinfo.dwObjectID = REGENNPC_OBJ->GetNpcObjectID();
	Baseinfo.BattleID = pInfo->AppearanceInfo.AppearanceChannel + 1;

	SafeStrCpy(Baseinfo.ObjectName, Npclist->Name, MAX_NAME_LENGTH+1);
	Npcinfo.Group = 999;		//!!!
	Npcinfo.MapNum = g_pServerSystem->GetMapNum();
	Npcinfo.NpcKind = Npclist->NpcKind;
	Npcinfo.NpcUniqueIdx = BOBUSANG_wNpcUniqueIdx;//pNpcInfo->wNpcUniqueIdx;
	Npcinfo.NpcJob = Npclist->JobKind;

	BobusangPosPerMap* pPos = GAMERESRCMNGR->GetBobusangPos(Npcinfo.MapNum, pBobusangInfo->AppearancePosIdx);

	pInfo->pBobusang = g_pServerSystem->AddNpc( &Baseinfo, &Npcinfo, &(pPos->ApprPos), pPos->ApprDir );

	// 판매품목/갯수 세팅
	//DealerData* pData = ITEMMGR->GetDealer(...)
	pInfo->pDealItemInfo = ITEMMGR->GetDealer(BOBUSANG_wNpcUniqueIdx);

	if( NULL == pInfo->pDealItemInfo )
	{
		ASSERT(0);
		return FALSE;
	}

	pInfo->pDealItemInfo->MakeSellingList(&pInfo->SellingItemList, pInfo->AppearanceInfo.SellingListIndex);

	//DealerData*		pDealItemInfo;
	//cPtrList			pCustomerList;

	return TRUE;

}

BOOL BobusangManager::RemoveBobusangNpc( DWORD channelNum )
{
	BOBUSANGTOTALINFO* pInfo = &m_pBobusang[channelNum];

	if(!pInfo)
		return FALSE;

	if(!pInfo->pBobusang)
		return FALSE;

	//2008. 5. 19. CBH - 맵의 채널 갯수가 틀리므로 MAX 채널보다 크면 처리 안되게 막는다.
	if(channelNum > CHANNELSYSTEM->GetChannelCount()-1)
		return FALSE;

	// NPC 삭제
	g_pServerSystem->RemoveNpc(pInfo->pBobusang->GetID());
	pInfo->pBobusang = NULL;

	// 기타 정보 삭제
	pInfo->pDealItemInfo = NULL;

	PTRLISTPOS pos = pInfo->SellingItemList.GetHeadPosition();
	while(pos)
	{
		DealerItem* pItem = (DealerItem*)pInfo->SellingItemList.GetAt(pos);
		if(pItem)
		{
			SAFE_DELETE(pItem);
		}
		pInfo->SellingItemList.GetNext(pos);
	}
	pInfo->SellingItemList.RemoveAll();

	pos = pInfo->pCustomerList.GetHeadPosition();
	while(pos)
	{
		CPlayer* pCustomer = (CPlayer*)pInfo->pCustomerList.GetAt(pos);
		
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pCustomer->GetID());
		if(!pPlayer)
			pInfo->pCustomerList.RemoveAt(pos);	// 안해줘도 무방
		else
			SendLeaveGuestMsg(pPlayer);

		pInfo->pCustomerList.GetNext(pos);
	}
	pInfo->pCustomerList.RemoveAll();

	return TRUE;
}

void BobusangManager::SetBobusanInfo( BOBUSANGINFO* pBobusangInfo )
{
	if( !pBobusangInfo )
	{
		ASSERT(0);
		return;
	}

	DWORD channelNum = pBobusangInfo->AppearanceChannel;

	BOBUSANGTOTALINFO* pInfo = &m_pBobusang[channelNum];
	//BOBUSANGINFO		AppearanceInfo;
	pInfo->AppearanceInfo = *pBobusangInfo;
}

BOBUSANGTOTALINFO* BobusangManager::GetBobusang( DWORD mapChannelNum )
{
	// 맵 서버에서 0번 채널은 없다 1부터 시작.
	if( 0 == mapChannelNum )
	{
		ASSERT(0);
		return NULL;
	}

	BOBUSANGTOTALINFO* pInfo = &m_pBobusang[mapChannelNum - 1];

	return pInfo;
}

int BobusangManager::GetBobusangSellingRt( DWORD channelNum, DealerItem* pItemList )
{
	BOBUSANGTOTALINFO* pInfo = GetBobusang(channelNum);

	PTRLISTPOS pos = pInfo->SellingItemList.GetHeadPosition();
	int invenNum = 0;

	while(pos)
	{
		DealerItem* pItem = (DealerItem*)pInfo->SellingItemList.GetAt(pos);

		if( pItem )
		{
			pItemList[invenNum] = *pItem;
			++invenNum;
		}

		pInfo->SellingItemList.GetNext(pos);
	}

	return invenNum;
}

void BobusangManager::AddGuest( CPlayer* pPlayer )
{
	// guestlist 추가.
	BOBUSANGTOTALINFO* pInfo = GetBobusang(pPlayer->GetChannelID());

	if( !pInfo->pBobusang )	return;	// 보부상 출현중 아니면

	PTRLISTPOS pos = pInfo->pCustomerList.GetHeadPosition();
	while(pos)
	{
		CPlayer* pGuest = (CPlayer*)pInfo->pCustomerList.GetAt(pos);
		if( pGuest == pPlayer )
		{
			//	return;	//!!! assertion
			break;
		}

		pInfo->pCustomerList.GetNext(pos);
	}

	pInfo->pCustomerList.AddTail((void*)pPlayer);

	DealerItem	SellingItem[SLOT_NPCINVEN_NUM];

	int dataNum = BOBUSANGMGR->GetBobusangSellingRt(pPlayer->GetChannelID(), SellingItem);

	MSG_ADDABLE_ONEKIND msg;
	msg.Category = MP_BOBUSANG;
	msg.Protocol = MP_BOBUSANG_ALL_DEALITEMINFO_TO_GUEST;	//MP_BOBUSANG_ADD_GUEST_ACK
	msg.AddableInfo.AddInfo( CAddableInfoList::BobusangItemInfo, sizeof(DealerItem)*dataNum, SellingItem);
	msg.DataNum = dataNum;

	pPlayer->SendMsg(&msg, msg.GetSize());
}

void BobusangManager::LeaveGuest( CPlayer* pPlayer )
{
	if( !pPlayer )
		return;
	
	//무효한 채널 값 
	//퀘스트맵에 보부상 들어갈 시 제거.
	if( QUESTMAPMGR->IsQuestMap() )
		return;

	//무효한 채널 값 
	if( CHANNELSYSTEM->GetChannelCount() < pPlayer->GetChannelID() )
	{
		ASSERT(0);
		return;
	}

	// guestlist 제외.
	BOBUSANGTOTALINFO* pInfo = GetBobusang(pPlayer->GetChannelID());

	if( !pInfo->pBobusang )	return; // 보부상 출현중 아니면

	PTRLISTPOS pos = pInfo->pCustomerList.GetHeadPosition();
	while(pos)
	{
		CPlayer* pGuest = (CPlayer*)pInfo->pCustomerList.GetAt(pos);
		if( pGuest == pPlayer )
		{
			pInfo->pCustomerList.RemoveAt(pos);
			SendLeaveGuestMsg( pPlayer );
			break;
		}

		pInfo->pCustomerList.GetNext(pos);
	}
}

void BobusangManager::SendLeaveGuestMsg( CPlayer* pPlayer )
{
	// 클라이언트 판매 아이템 정보 해제
	MSG_DWORD	msg;
	msg.Category = MP_BOBUSANG;
	msg.Protocol = MP_BOBUSANG_LEAVE_GUEST_ACK;

	pPlayer->SendMsg(&msg, sizeof(msg));
}

BOOL BobusangManager::BuyItem( CPlayer* pPlayer, WORD buyItemIdx, WORD BuyItemNum )
{
	// 해당 보부상 정보를 가져와
	BOBUSANGTOTALINFO* pInfo = GetBobusang(pPlayer->GetChannelID());
	if(!pInfo)
	{
		return FALSE;
	}

	// 손님이 맞는지 확인하고
	BOOL bGuest = FALSE;
	PTRLISTPOS pos = pInfo->pCustomerList.GetHeadPosition();
	while(pos)
	{
		CPlayer* pGuest = (CPlayer*)pInfo->pCustomerList.GetAt(pos);

		if(pGuest == pPlayer)
		{
			bGuest = TRUE;
			break;
		}
		pInfo->pCustomerList.GetNext(pos);
	}

	if( FALSE == bGuest )
	{
		return FALSE;
	}

	// 갯수 가능한지 확인하고
	DealerItem* pBuyItem = NULL;
	pBuyItem = GetSellingItem(&pInfo->SellingItemList, buyItemIdx);
	if( !pBuyItem )
	{
		return FALSE;
	}
	if( pBuyItem->ItemCount < BuyItemNum )
	{
		return FALSE;
	}
	// 아이템 갯수 빼준뒤
	
	pBuyItem->ItemCount -= BuyItemNum;
	
	// 정보를 손님들에게 보냄
	MSG_DWORD3 msg;
	msg.Category = MP_BOBUSANG;
	msg.Protocol = MP_BOBUSANG_DEALITEMINFO_TO_GUEST;
	msg.dwData1	= pBuyItem->ItemIdx;
	msg.dwData2 = pBuyItem->Pos;
	msg.dwData3 = pBuyItem->ItemCount;

	SendDealerItemMsgToGuestAll(pInfo->AppearanceInfo.AppearanceChannel, &msg, sizeof(msg));

	return TRUE;
}

DealerItem*	BobusangManager::GetSellingItem( cPtrList* pSellingItemList, WORD buyItemIdx )
{
	if(!pSellingItemList)
		return NULL;

	PTRLISTPOS pos = pSellingItemList->GetHeadPosition();

	while(pos)
	{
		DealerItem* pItem = (DealerItem*)pSellingItemList->GetAt(pos);

		if( pItem->ItemIdx == buyItemIdx )
			return pItem;

		pSellingItemList->GetNext(pos);
	}

	return NULL;
}

void BobusangManager::AppearBobusang( MSG_BOBUSANG_INFO* pmsg )
{
	// BobusangInfo.bin
	if( TRUE == MakeNewBobusangNpc(&pmsg->bobusangInfo) )
	{
		// Agent로 성공 메시지 전달
		MSG_DWORD msg;
		msg.Category = MP_BOBUSANG;
		msg.Protocol = MP_BOBUSANG_APPEAR_MAP_TO_AGENT;
		msg.dwData	= pmsg->bobusangInfo.AppearanceChannel;
		g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );

		//g_Network.Send2AgentServer( (char*)&msg, msg.GetSize() );
	}
}

void BobusangManager::DisappearBobusang( MSG_DWORD2* pmsg )
{
	if( TRUE == RemoveBobusangNpc(pmsg->dwData1) )
	{
		// Agent로 성공 메시지 전달
		MSG_DWORD msg;
		msg.Category = MP_BOBUSANG;
		msg.Protocol = MP_BOBUSANG_DISAPPEAR_MAP_TO_AGENT;
		msg.dwData	= pmsg->dwData1;	//AppearanceChannel
		g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );
	}
}

BOOL BobusangManager::CheckHackBobusangDist( CPlayer* pGuest )
{
	DWORD	guestChannel = pGuest->GetChannelID();

	BOBUSANGTOTALINFO* pInfo = GetBobusang(guestChannel);
	if(!pInfo)
		return FALSE;

	if(!pInfo->pBobusang)
		return FALSE;

	VECTOR3 vGPos, vBPos;
	pGuest->GetPosition(&vGPos);
	//((CNpc*)(pInfo->pBobusang))->GetPosition(&vBPos);
	pInfo->pBobusang->GetPosition(&vBPos);
	float dist = CalcDistanceXZ( &vGPos, &vBPos );
	if( dist > 3000.0f )	return FALSE;

	return TRUE;
}

void BobusangManager::SendDealerItemMsgToGuestAll( DWORD channelNum, MSGBASE* pmsg, int size )
{
	//2008. 5. 19. CBH - 맵의 채널 갯수가 틀리므로 MAX 채널보다 크면 처리되면 안된다.
	if(channelNum > CHANNELSYSTEM->GetChannelCount()-1)
		return;

	BOBUSANGTOTALINFO* pInfo = &m_pBobusang[channelNum];
	if(!pInfo)
	{
		return;
	}

	PTRLISTPOS pos = pInfo->pCustomerList.GetHeadPosition();

	while(pos)
	{
		CPlayer* pGuest = (CPlayer*)pInfo->pCustomerList.GetAt(pos);
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pGuest->GetID());
		if(!pPlayer)
			pInfo->pCustomerList.RemoveAt(pos);
		else
			pPlayer->SendMsg(pmsg, size);

		pInfo->pCustomerList.GetNext(pos);
	}
}

void BobusangManager::NetworkMsgParser( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_BOBUSANG_ADD_GUEST_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			CPlayer* pGuest = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pGuest)	return;

			DWORD channel = pGuest->GetChannelID();

			BOBUSANGTOTALINFO* pInfo = GetBobusang(channel);

			if( !pInfo->pBobusang ||
				FALSE == ITEMMGR->CheckHackNpc( pGuest, pmsg->wData ) )
			{
				pmsg->Protocol = MP_BOBUSANG_ADD_GUEST_NACK;
				pGuest->SendMsg(pmsg, sizeof(*pmsg));
				return;
			}

			AddGuest(pGuest);
		}
		break;
	case MP_BOBUSANG_LEAVE_GUEST_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CPlayer* pGuest = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pGuest)	return;

			DWORD channel = pGuest->GetChannelID();

			BOBUSANGTOTALINFO* pInfo = GetBobusang(channel);

			if( !pInfo->pBobusang )	return;	//npc remove 이 후에도 무방.

			LeaveGuest(pGuest);
		}
		break;
	case MP_BOBUSANG_INFO_AGENT_TO_MAP:
		{
			MSG_BOBUSANG_INFO* pmsg = (MSG_BOBUSANG_INFO*)pMsg;
			// 보부상 정보

			// 현재 보부상 해제 중인지

			// 보부상 생성
			AppearBobusang(pmsg);	// add object
		}
		break;
	case MP_BOBUSANG_DISAPPEAR_AGENT_TO_MAP:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			// 현재 보부상 출현 중인지

			// 보부상 해제
			DisappearBobusang(pmsg);	// remove object

			/*
			MSG_DWORD2	msg;
			msg.Category = MP_BOBUSANG;
			msg.Protocol = MP_BOBUSANG_DISAPPEAR_AGENT_TO_MAP;
			msg.dwData1	= pInfo->AppearanceChannel;
			msg.dwData2 = pInfo->DisappearanceTime;
			*/
		}	
		break;
	case MP_BOBUSANG_NOTIFY_FOR_DISAPPEARANCE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			SendDealerItemMsgToGuestAll(pmsg->dwData1, pmsg, sizeof(MSG_DWORD2));
		}
		break;
	default:
		{
			ASSERT(0);
			break;
		}
	}
}
