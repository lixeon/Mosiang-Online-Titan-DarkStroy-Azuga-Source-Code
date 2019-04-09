// WantedManager.cpp: implementation of the CWantedManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WantedManager.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "Network.h"
#include "Player.h"
#include "WantedInfo.h"
#include "PackedData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWantedManager::CWantedManager()
{
	m_WantedHashTable.Initialize(10);
}

CWantedManager::~CWantedManager()
{
	void * pInfo;
	m_WantedHashTable.SetPositionHead();
	while(pInfo = m_WantedHashTable.GetData())
		delete pInfo;
	m_WantedHashTable.RemoveAll();
}

void CWantedManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_WANTED_ORDERLIST_SYN:
		{
			SEND_WANTED_ORDER_SYN* pmsg = (SEND_WANTED_ORDER_SYN*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;
			if(m_WantedHashTable.GetDataNum() == 0)
			{
				MSG_BYTE msg;
				msg.Category = MP_WANTED;
				msg.Protocol = MP_WANTED_ORDERLIST_NACK;
				msg.bData = 0;
				pPlayer->SendMsg(&msg, sizeof(msg));
				return;
			}
			DWORD Totalpage = ((m_WantedHashTable.GetDataNum()-1)/WANTUNIT_PER_PAGE) +1;
			if(Totalpage < pmsg->page)
			{
				MSG_BYTE msg;
				msg.Category = MP_WANTED;
				msg.Protocol = MP_WANTED_ORDERLIST_NACK;
				msg.bData = 1;
				pPlayer->SendMsg(&msg, sizeof(msg));
				return;
			}
			char type[16];
			GetOrderType(pmsg->type, type);
			WantedOrderedList(pmsg->dwObjectID, pmsg->page, type, WANTUNIT_PER_PAGE);
		}
		break;
	case MP_WANTED_MYLIST_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;
			if(pPlayer->GetWantedNum() == 0)
			{
				MSG_BYTE msg;
				msg.Category = MP_WANTED;
				msg.Protocol = MP_WANTED_MYLIST_NACK;
				msg.bData = 0;// have no
				pPlayer->SendMsg(&msg, sizeof(msg));
				return;
			}
			DWORD Totalpage = ((pPlayer->GetWantedNum()-1) / WANTUNIT_PER_PAGE)+1;
			if(Totalpage < pmsg->wData)
			{
				MSG_BYTE msg;
				msg.Category = MP_WANTED;
				msg.Protocol = MP_WANTED_MYLIST_NACK;
				msg.bData = 2;// Total less than go to page
				pPlayer->SendMsg(&msg, sizeof(msg));
				ASSERT(0);
				return;
			}
			WantedMyList(pmsg->dwObjectID, pmsg->wData, WANTUNIT_PER_PAGE);
		}
		break;
	case MP_WANTED_REGIST_SYN:
		{
			MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;
			RegistKillerSyn(pPlayer, pmsg->dwData1, pmsg->dwData2);
		}
		break;
	case MP_WANTED_REGIST_CANCEL:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;
			pPlayer->SetReadyToRevive(TRUE);

		}
		break;
	case MP_WANTED_BUYRIGHT_SYN:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;
			BuyWantedRightSyn(pPlayer, pmsg->dwData);
		}
		break;
	case MP_WANTED_GIVEUPRIGHT_SYN:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;
			GiveUpWantedRightSyn(pPlayer, pmsg->dwData);			
		}
		break;
	case MP_WANTED_BUY_TO_MAP:
		{
			MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;
			CWantedInfo * pWantedInfo = GetWantedInfo(pmsg->dwData1);
			if(pWantedInfo == 0)
				return;
			pWantedInfo->SetTotalPrize((MONEYTYPE)(pWantedInfo->GetTotalPrize() + WANTED_CHARGE*0.8));
			pWantedInfo->IncreVolunteerNum();
		}
		break;
	case MP_WANTED_GIVEUPRIGHT_TO_MAP:
		{
			MSGBASE * pmsg = (MSGBASE*)pMsg;
			CWantedInfo * pWantedInfo = GetWantedInfo(pmsg->dwObjectID);
			if(pWantedInfo == 0)
				return;
			pWantedInfo->DecreVolunteerNum();
		}
		break;
	case MP_WANTED_DESTROYED_TO_MAP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer)
			{
				if( pPlayer->IsWantedOwner(pmsg->dwData) == TRUE )
				{
					pPlayer->DeleteWanted(pmsg->dwData);
					MSG_DWORD cmsg;
					cmsg.Category = MP_WANTED;
					cmsg.Protocol = MP_WANTED_DESTROYED_TO_CLIENT;
					cmsg.dwData = pmsg->dwData;
					pPlayer->SendMsg(&cmsg, sizeof(cmsg));
				}
			}
		}
		break;
	//broadcasting
	case MP_WANTED_NOTIFY_REGIST_TO_MAP:
		{
			SEND_WANTEDINFO_TO_SERVER * pmsg = (SEND_WANTEDINFO_TO_SERVER*)pMsg;
			DoRegistKiller(&pmsg->Info);
		}
		break;
	case MP_WANTED_NOTIFY_DELETE_TO_MAP:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			DoDeleteFromWantedList((WANTEDTYPE)pmsg->dwObjectID);			
		}
		break;
	case MP_WANTED_NOTIFY_NOTCOMPLETE_TO_MAP:
		{
			SEND_WANTED_NOTCOMPLETE* pmsg = (SEND_WANTED_NOTCOMPLETE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;
			CanNotComplete(pPlayer, pmsg->WantedIDX, pmsg->CompleterName, pmsg->type);
		}
		break;


		//from agent
	case MP_WANTED_DELETE_BY_DELETECHR: 
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			DoDeleteFromWantedList((WANTEDTYPE)pmsg->dwObjectID);
		}
		break;
	case MP_WANTED_NOTCOMPLETE_BY_DELCHR:
		{
			MSG_WORD*pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;
			CanNotCompleteByDelChr(pPlayer, pmsg->wData);
		}
		break;
	case MP_WANTED_SEARCH_SYN:
		{
			MSG_NAME_WORD * pmsg = (MSG_NAME_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;
			WantedSearch(pmsg->dwObjectID, pmsg->Name, pmsg->wData, WANTUNIT_PER_PAGE);
		}
		break;
	default:
		{
			char buf[68];
			sprintf(buf, "Protocol : %d", ((MSGBASE*)pMsg)->Protocol );
			ASSERTMSG(0, buf);
		}
	}
}

void CWantedManager::LoadWantedList(WANTEDINFO_LOAD* pInfo)
{
	for(int i=0; i<pInfo->count; ++i)
	{
		CWantedInfo* pWantedInfo = new CWantedInfo;
		pWantedInfo->InitWantedInfo(&pInfo->m_WantedInfo[i]);
		m_WantedHashTable.Add(pWantedInfo, pWantedInfo->GetWantedIDX());
	}
}

void CWantedManager::RegistKillerSyn(CPlayer* pPlayer, DWORD TargetID, MONEYTYPE Prize)
{
	DWORD dwMinPrize = MIN_PRIZE;
#ifdef _CHINA_LOCAL_
	dwMinPrize = MIN_PRIZE_CHINA;
#endif
//	if( g_pServerSystem->GetNation() == eNATION_CHINA )
//		dwMinPrize = MIN_PRIZE_CHINA;
	
	if((Prize < dwMinPrize) || (Prize > MAX_PRIZE))
	{
		ASSERT(0);
		SendRegistNackMsg(pPlayer, 1);
		return;
	}
	if(pPlayer->GetMoney() < Prize)
	{
		ASSERT(0);
		SendRegistNackMsg(pPlayer, 2);
		return;
	}
	if(pPlayer->GetMurderIDX() != TargetID)
	{
		ASSERT(0);
		SendRegistNackMsg(pPlayer, 3);
		return;
	}

	pPlayer->SetMoney(Prize, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseWantedRegist, TargetID);

	//db update return wanted_idx, target id, total prize,  -> add to wantedlist
	WantedRegist(pPlayer->GetID(), TargetID, Prize);

	//ack
	MSGBASE msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_REGIST_ACK;
	pPlayer->SendMsg(&msg, sizeof(msg));

	pPlayer->SetReadyToRevive(TRUE);
}

void CWantedManager::SendRegistNackMsg(CPlayer* pPlayer, BYTE err)
{
	MSG_BYTE msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_REGIST_NACK;
	msg.bData = err;
	pPlayer->SendMsg(&msg, sizeof(msg));
	pPlayer->SetReadyToRevive(TRUE);
}

void CWantedManager::RegistKillerResult(WANTEDINFO* pInfo)
{
	// add to WantedList
	DoRegistKiller(pInfo);

	//broadcasting to another map servers
	SEND_WANTEDINFO_TO_SERVER msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_NOTIFY_REGIST_TO_MAP;
	msg.Info.WantedIDX = pInfo->WantedIDX;
	msg.Info.WantedChrIDX = pInfo->WantedChrIDX;
	msg.Info.TotalPrize = pInfo->TotalPrize;
	msg.Info.RegistChrIDX = pInfo->RegistChrIDX;
	SafeStrCpy(msg.Info.WantedChrName, pInfo->WantedChrName, MAX_NAME_LENGTH+1);
	g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
	
	CPlayer* pWanted = (CPlayer*)g_pUserTable->FindUser(pInfo->WantedChrIDX);
	if(!pWanted)
		return;
	pWanted->ClearWantedList();
	MSGBASE rmsg;
	rmsg.Category = MP_WANTED;
	rmsg.Protocol = MP_WANTED_LET_KNOW_REGIST;
	pWanted->SendMsg(&rmsg, sizeof(rmsg));
}

void CWantedManager::DoRegistKiller(WANTEDINFO* pInfo)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pInfo->WantedChrIDX);
	if(pPlayer)
		pPlayer->SetWantedIdx(pInfo->WantedIDX);

	CWantedInfo* pWantedInfo = GetWantedInfo(pInfo->WantedIDX);
	if(pWantedInfo == 0)
	{
		CWantedInfo* pWInfo = new CWantedInfo;
		pWInfo->InitWantedInfo(pInfo);
		m_WantedHashTable.Add(pWInfo, pWInfo->GetWantedIDX());
	}
	else
	{
		pWantedInfo->SetTotalPrize(pInfo->TotalPrize);
	}
}

CWantedInfo* CWantedManager::GetWantedInfo(DWORD WantedIDX)
{
	return m_WantedHashTable.GetData(WantedIDX);
}

void CWantedManager::DeleteFromWantedList(WANTEDTYPE WantedIDX)
{
	//delete
	DoDeleteFromWantedList(WantedIDX);

	//broadcasting
	MSGBASE msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_NOTIFY_DELETE_TO_MAP;
	msg.dwObjectID = WantedIDX;
	
	PACKEDDATA_OBJ->SendToBroadCastMapServer(&msg,sizeof(msg));
}

BOOL CWantedManager::DoDeleteFromWantedList(WANTEDTYPE WantedIDX)
{
	CWantedInfo* pWantedInfo = 0;
	pWantedInfo = GetWantedInfo(WantedIDX);
	if(pWantedInfo == 0)
	{
//		ASSERT(0); //what?
		return FALSE; 
	}
	delete pWantedInfo;
	pWantedInfo = NULL;

	m_WantedHashTable.Remove(WantedIDX);
	return TRUE;
}

void CWantedManager::BuyWantedRightSyn(CPlayer* pPlayer, WANTEDTYPE WantedIDX)
{
	CWantedInfo * pWantedInfo = GetWantedInfo(WantedIDX);
	int rt = CanBuyWantedRight(pPlayer, pWantedInfo);
	if(rt != eBWR_Success)
	{
		//nack
		MSG_INT msg;
		msg.Category = MP_WANTED;
		msg.Protocol = MP_WANTED_BUYRIGHT_NACK;
		msg.nData = rt;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}	
	// db update
	WantedBuyRight(pPlayer->GetID(), WantedIDX, (MONEYTYPE)(WANTED_CHARGE*0.8));
	pWantedInfo->IncreVolunteerNum();
	pPlayer->AddWanted(WantedIDX);
	pPlayer->SetMoney(WANTED_CHARGE, MONEY_SUBTRACTION, 0, eItemTable_Inventory, 
					eMoneyLog_LoseWantedBuyRight, WantedIDX);
	
	pWantedInfo->SetTotalPrize((MONEYTYPE)(pWantedInfo->GetTotalPrize() + WANTED_CHARGE*0.8));
	
	//send ack msg
	SEND_WANTEDLIST_TO_CLIENT msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_BUYRIGHT_ACK;
	msg.List.WantedIDX = WantedIDX;
	msg.List.WantedChrID = pWantedInfo->GetWantedChrIDX();
	SafeStrCpy(msg.List.WantedName, pWantedInfo->GetWantedChrName(), MAX_NAME_LENGTH+1);
	char Date[16];
	SYSTEMTIME ti;
	GetLocalTime(&ti);
	sprintf(Date,"%d.%02d.%02d",ti.wYear,ti.wMonth,ti.wDay);
	SafeStrCpy(msg.List.RegistDate, Date, 11);
	pPlayer->SendMsg(&msg, sizeof(msg));

	//broadcasting
	MSG_DWORD2 bmsg;
	bmsg.Category = MP_WANTED;
	bmsg.Protocol = MP_WANTED_BUY_TO_MAP;
	bmsg.dwData1 = WantedIDX;
	bmsg.dwData2 = (DWORD)(pWantedInfo->GetTotalPrize() + WANTED_CHARGE*0.8);
	PACKEDDATA_OBJ->SendToBroadCastMapServer(&bmsg,sizeof(bmsg));
}

void CWantedManager::GiveUpWantedRightSyn(CPlayer* pPlayer, WANTEDTYPE WantedIDX)
{
	if(IsOwner(pPlayer, WantedIDX) == FALSE)
	{
		ASSERT(0);
		return;
	}	
	CWantedInfo* pWantedInfo = GetWantedInfo(WantedIDX);
	pPlayer->DeleteWanted(WantedIDX);
	pWantedInfo->DecreVolunteerNum();
	//db update
	WantedGiveUpRight(pPlayer->GetID(), WantedIDX);

	//send ack msg
	MSG_DWORD msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_GIVEUPRIGHT_ACK;
	msg.dwData = WantedIDX;
	pPlayer->SendMsg(&msg, sizeof(msg));

	//broadcasting
	MSGBASE bmsg;
	bmsg.Category = MP_WANTED;
	bmsg.Protocol = MP_WANTED_GIVEUPRIGHT_TO_MAP;
	bmsg.dwObjectID = WantedIDX;
	PACKEDDATA_OBJ->SendToBroadCastMapServer(&bmsg,sizeof(bmsg));
}

int CWantedManager::CanBuyWantedRight(CPlayer* pPlayer, CWantedInfo* pWantedInfo)
{
	if(!pWantedInfo)
		return eBWR_Err_Err;
	if(pPlayer->GetMoney() < WANTED_CHARGE)
		return eBWR_Err_Err;
	if(IsOwner(pPlayer, pWantedInfo->GetWantedIDX()) == TRUE)
		return eBWR_Err_Err;
	if(pPlayer->GetWantedNum() >= MAX_WANTED_NUM)
		return eBWR_Err_Err;
	if(pPlayer->GetID() == pWantedInfo->GetWantedChrIDX())
		return eBWR_Err_Err;
	if(pPlayer->GetWantedIdx() != 0)
		return eBWR_Err_Err;
	if(pWantedInfo->GetTotalVolunteerNum() > MAX_WANTED_VOLUNTEER)
		return eBWR_Err_FullVolunteer;
	return eBWR_Success;
}

void CWantedManager::Complete(CPlayer* pPlayer, CPlayer* pTarget)
{
	WANTEDTYPE WantedIDX = pTarget->GetWantedIdx();
	if(WantedIDX == 0)
		return;
	CWantedInfo* pWantedInfo = GetWantedInfo(WantedIDX);
	if(pWantedInfo == 0)
	{
		ASSERT(0);
		return ;
	}
	if((pPlayer->IsWantedOwner(WantedIDX)) == FALSE)
	{
		return;
	}
	
	pTarget->SetWantedIdx(0);
	pPlayer->SetMoney(pWantedInfo->GetTotalPrize(), MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetWantedPrize, 0);
	pPlayer->DeleteWanted(WantedIDX);

	MSG_DWORD2 msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_COMPLETE;
	msg.dwData1 = pWantedInfo->GetWantedIDX();
	msg.dwData2 = pWantedInfo->GetTotalPrize();
	pPlayer->SendMsg(&msg, sizeof(msg)); 

	//db update
	WantedComplete(pPlayer->GetID(), pTarget->GetObjectName(), WantedIDX);
	
	DeleteFromWantedList(WantedIDX);
}

void CWantedManager::CanNotComplete(CPlayer* pPlayer, WANTEDTYPE WantedIDX, char* CompleterName, BYTE type)
{
	if(pPlayer->IsWantedOwner(WantedIDX) == FALSE)
	{
		ASSERT(0); //wak!
		return;
	}
	
	pPlayer->DeleteWanted(WantedIDX);

	SEND_WANTED_NOTCOMPLETE msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_NOTCOMPLETE;
	msg.WantedIDX = WantedIDX;
	SafeStrCpy(msg.CompleterName, CompleterName, MAX_NAME_LENGTH+1);
	msg.type = type;
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}

BOOL CWantedManager::IsOwner(CPlayer* pPlayer, WANTEDTYPE WantedIDX)
{
	if(WantedIDX == 0)
		return FALSE;
	CWantedInfo* pWantedInfo = GetWantedInfo(WantedIDX);
	if(!pWantedInfo)
	{
		char buf[32];
		sprintf(buf, "현상금 인덱스: %d", WantedIDX);
		ASSERTMSG(0, buf);
		return FALSE;
	}
	
	return pPlayer->IsWantedOwner(WantedIDX); 
}

void CWantedManager::GetOrderType(BYTE ordertype, char* type)
{
	switch(ordertype)
	{
	case eWan_Date:
		strcpy(type, "Wanted_IDX"); break;
	case eWan_Prize:
		strcpy(type, "Prize"); break;
	case eWan_Volunteer:
		strcpy(type, "VolunteerNum"); break;
	default:
		ASSERTMSG(0, "현상범 정렬 타입이 없습니다. No Sort Type in Wanted List"); break;		
	}
}

void CWantedManager::SendRegistWanted(CPlayer* pPlayer, CPlayer* pTarget)
{
	MSG_NAME_DWORD msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_KILLEDBYPLAYER;
	msg.dwData = pTarget->GetID();
	SafeStrCpy(msg.Name, pTarget->GetObjectName(), MAX_NAME_LENGTH+1);
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CWantedManager::CanNotCompleteByDelChr(CPlayer* pPlayer, WANTEDTYPE WantedIDX)
{
	if(pPlayer->IsWantedOwner(WantedIDX) == FALSE)
	{
		ASSERT(0); return;
	}
	pPlayer->DeleteWanted(WantedIDX);

	MSG_DWORD msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_NOTCOMPLETE_BY_DELCHR_TO_CL;
	msg.dwData = WantedIDX;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CWantedManager::DestroyWanted(CPlayer* pPlayer)
{
	if(DoDeleteFromWantedList(pPlayer->GetWantedIdx()) == FALSE)
		return;
	
	WantedDestroyed(pPlayer->GetWantedIdx());
	//notify
	DeleteFromWantedList(pPlayer->GetWantedIdx());
	pPlayer->SetWantedIdx(0);

	MSG_DWORD msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_DESTROYED_TO_CLIENT;
	msg.dwData = pPlayer->GetWantedIdx();
	pPlayer->SendMsg(&msg, sizeof(msg));
}


BOOL CWantedManager::IsWantedPlayer(CPlayer* pPlayer, DWORD WantedIDX)
{
	CWantedInfo* pInfo = m_WantedHashTable.GetData(WantedIDX);
	if(pInfo)
	{
		if(pInfo->GetWantedChrIDX() == pPlayer->GetID())
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}
