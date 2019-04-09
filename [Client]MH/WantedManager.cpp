// WantedManager.cpp: implementation of the CWantedManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WantedManager.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "WantNpcDialog.h"
#include "WantRegistDialog.h"
#include "ReviveDialog.h"
#include "WantedDialog.h"
#include "JournalDialog.h"
#include "PKManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CWantedManager)

CWantedManager::CWantedManager()
{
}

CWantedManager::~CWantedManager()
{
}

void CWantedManager::Init()
{
	m_MurdererID = 0;
	m_bIsWaiting = eNone;
	m_bCanRevive = FALSE;
}

void CWantedManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_WANTED_INFO_BY_USERLOGIN:
		{
			SEND_WANTEDLIST * pmsg = (SEND_WANTEDLIST*)pMsg;
			m_Wanted.SetInfo(pmsg);
			SetWaiting(eNone);
			
			HERO->SetWantedIdx(pmsg->WantedIdx);
			GAMEIN->GetWantedDialog()->SetInfo(pmsg->List);
			//quest interface update
		}
		break;
	case MP_WANTED_ORDERLIST_ACK:
		{
			SEND_WANTNPCLIST* pmsg = (SEND_WANTNPCLIST*)pMsg;
			GAMEIN->GetWantNpcDialog()->SetActive(TRUE);
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_Npc);
			GAMEIN->GetWantNpcDialog()->SetWantedList(pmsg);
		}
		break;
	case MP_WANTED_MYLIST_ACK:
		{
			SEND_WANTNPCLIST* pmsg = (SEND_WANTNPCLIST*)pMsg;
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_My);
			GAMEIN->GetWantNpcDialog()->SetWantedList(pmsg);
		}
		break;
	case MP_WANTED_KILLEDBYPLAYER:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			SetMurdererID(pmsg->dwData);
			GAMEIN->GetWantRegistDialog()->SetWantedName(pmsg->Name);
			GAMEIN->GetWantRegistDialog()->SetActive( TRUE );	//딜레이 후에 active된다.
			SetWaiting(eStart);
		}
		break;
	case MP_WANTED_REGIST_ACK:
		{
			SetWaiting(eNone);
			SetMurdererID(0);
			//메시지 처리
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(426));
		}
		break;
	case MP_WANTED_BUYRIGHT_ACK:
		{
			SEND_WANTEDLIST_TO_CLIENT * pmsg = (SEND_WANTEDLIST_TO_CLIENT*)pMsg;
			BuyWantedRightResult(&pmsg->List);	
			CHATMGR->AddMsg(CTC_GETMONEY, CHATMGR->GetChatMsg(118), "500");
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(429), pmsg->List.WantedName);
			
			OBJECTMGR->ApplyOverInfoOptionToAll();
		}
		break;
	case MP_WANTED_GIVEUPRIGHT_ACK:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(428), m_Wanted.GetWantedName(pmsg->dwData));
			GiveUpWantedRightResult(pmsg->dwData);
			OBJECTMGR->ApplyOverInfoOptionToAll();
		}
		break;
	case MP_WANTED_NOTCOMPLETE:
		{
			SEND_WANTED_NOTCOMPLETE* pmsg = (SEND_WANTED_NOTCOMPLETE*)pMsg;
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(437), pmsg->CompleterName, m_Wanted.GetWantedName(pmsg->WantedIDX));
			DeleteFromWantedList(pmsg->WantedIDX, eJournal_Wanted_FailbyOther);
			//interface update			
			OBJECTMGR->ApplyOverInfoOptionToAll();
		}
		break;
	case MP_WANTED_COMPLETE:
		{
			MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(427), m_Wanted.GetWantedName(pmsg->dwData1) , pmsg->dwData2);
			DeleteFromWantedList(pmsg->dwData1, eJournal_Wanted_Succeed);
			OBJECTMGR->ApplyOverInfoOptionToAll();
		}
		break;
	case MP_WANTED_NOTCOMPLETE_BY_DELCHR_TO_CL:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			//interface update
			DeleteFromWantedList(pmsg->dwData, eJournal_Wanted_FailbyDelChr);
		}
		break;
	case MP_WANTED_LET_KNOW_REGIST:
		{
			m_Wanted.InitInfo();
		}
		break;
	case MP_WANTED_DESTROYED_TO_CLIENT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if(pmsg->dwData == HERO->GetWantedIdx()) //현상범이면
			{
				HERO->SetWantedIdx(0);
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(682));
			}
			else //척살권 가진 사람이면
			{
				if(m_Wanted.IsInList(pmsg->dwData) == TRUE)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(683), m_Wanted.GetWantedName(pmsg->dwData));
					DWORD WantedIdx = 0;
					WantedIdx = m_Wanted.GetWantedChrID(pmsg->dwData);
					DeleteFromWantedList(pmsg->dwData, eJournal_Wanted_FailbyTime);
					CObject* pPlayer = OBJECTMGR->GetObject(WantedIdx);
					if(pPlayer != 0)
					{
						OBJECTMGR->ApplyOverInfoOption(pPlayer);
					}					
				}
			}
		}
		break;
	case MP_WANTED_SEARCH_ACK:
		{
			SEND_WANTNPCLIST * pmsg = (SEND_WANTNPCLIST *)pMsg;
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_Search);
			GAMEIN->GetWantNpcDialog()->SetWantedList(pmsg);
		}
		break;
		//////////////////////////////////////////////////////////////////////////
		//NACK
	
	case MP_WANTED_REGIST_NACK:
		{
			SetMurdererID(0);			
			SetWaiting(eDone);
			//실패 매세지..
			ASSERT(0);
		}
		break;
	case MP_WANTED_ORDERLIST_NACK: 
		{
			SEND_WANTNPCLIST info;
			memset(&info, 0, sizeof(info));
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_Npc);
			GAMEIN->GetWantNpcDialog()->SetWantedList(&info);
			GAMEIN->GetWantNpcDialog()->SetActive(TRUE);
		}
		break;
	case MP_WANTED_MYLIST_NACK: // 가진게 없을 때도 nack가 온다.
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			if(pmsg->bData == 0) // 가진게 없다.
			{
				SEND_WANTNPCLIST info;
				memset(&info, 0, sizeof(info));
				GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_My);
				GAMEIN->GetWantNpcDialog()->SetWantedList(&info);
			}
			else //  1: 가려는 페이지가 총 페이지보다 크다. 
				ASSERT(0);
		}
		break;
	case MP_WANTED_BUYRIGHT_NACK:
		{
			MSG_INT * pmsg = (MSG_INT*)pMsg;
			if(pmsg->nData == eBWR_Err_FullVolunteer)
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(693));
			else
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(570));
		}	
		break;
	case MP_WANTED_SEARCH_NACK:
		{
		}
		break;
	default:
		ASSERT(0);
	}
}




BOOL CWantedManager::IsInList(WANTEDTYPE WantedIDX)
{
	return m_Wanted.IsInList(WantedIDX);
}

BOOL CWantedManager::IsInList( char* strName )
{
	return m_Wanted.IsInList( strName );
}

void CWantedManager::RegistKillerSyn(MONEYTYPE Prize)
{
	if(CanRegist(Prize) == FALSE)
	{
		return;
	}

	MSG_DWORD2 msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_REGIST_SYN;
	msg.dwData1 = m_MurdererID;
	msg.dwData2 = Prize;
	msg.dwObjectID = HEROID;

	NETWORK->Send(&msg, sizeof(msg));
	
	SetWaiting(eWaiting);	
	GAMEIN->GetWantRegistDialog()->SetActive(FALSE);
	SetMurdererID(0);
}

BOOL CWantedManager::CanRegist(MONEYTYPE Prize)
{
	ASSERT(m_MurdererID);

	if(IsWaiting() != eStart)
		return FALSE;	
	if(HERO->GetMoney() < Prize)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
		return FALSE;
	}

	return TRUE;
}

void CWantedManager::BuyWantedRightSyn(WANTEDTYPE WantedIDX)
{
	if(CanBuyWantedRight(WantedIDX) == FALSE)
	{
		//nack
		return;
	}

	MSG_DWORD msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_BUYRIGHT_SYN;
	msg.dwData = WantedIDX;
	msg.dwObjectID = HEROID;
	NETWORK->Send(&msg, sizeof(msg));
}

void CWantedManager::BuyWantedRightResult(WANTEDLIST * pInfo)
{
	m_Wanted.Add(pInfo);
	
	GAMEIN->GetWantedDialog()->AddInfo(pInfo);
}

BOOL CWantedManager::CanBuyWantedRight(WANTEDTYPE WantedIDX)
{
	if(HERO->GetMoney() < WANTED_CHARGE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
		return FALSE;
	}		
	if(m_Wanted.IsFull() == TRUE )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(542) );
		return FALSE;
	}	
	if(m_Wanted.IsInList(WantedIDX) == TRUE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(543) );
		return FALSE;
	}
	if(HERO->GetWantedIdx() != 0)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(675) );
		return FALSE;
	}
	return TRUE;
}

void CWantedManager::DeleteFromWantedList(WANTEDTYPE WantedIDX, BYTE DelKind)
{
	JOURNALINFO info;
	char temp[64];
	SYSTEMTIME ti;
	WANTEDLIST Info[MAX_WANTED_NUM];

	WANTEDLIST* pWInfo = m_Wanted.GetWantedInfo(WantedIDX);
	info.Kind = eJournal_Wanted;
	info.Param = DelKind;
	SafeStrCpy(info.ParamName, pWInfo->WantedName, MAX_NAME_LENGTH+1);
	
	GetSystemTime(&ti);
	sprintf(temp,"%d.%02d.%02d",ti.wYear,ti.wMonth ,ti.wDay);
	SafeStrCpy(info.RegDate, temp, 11);
	GAMEIN->GetJournalDialog()->JournalItemAdd(&info);

	m_Wanted.Delete(WantedIDX);
	m_Wanted.GetTotalInfo(Info);
	GAMEIN->GetWantedDialog()->SetInfo(Info);
}

void CWantedManager::GiveUpWantedRightSyn(WANTEDTYPE WantedIDX)
{
	if(m_Wanted.IsInList(WantedIDX) == FALSE)
	{
		ASSERT(0);
		return;
	}

	MSG_DWORD msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_GIVEUPRIGHT_SYN;
	msg.dwData = WantedIDX;
	msg.dwObjectID = HEROID;
	NETWORK->Send(&msg, sizeof(msg));
}

void CWantedManager::GiveUpWantedRightResult(WANTEDTYPE WantedIDX)
{
	//delete
	m_Wanted.Delete(WantedIDX);
	
	//interface update
	WANTEDLIST Info[MAX_WANTED_NUM];
	m_Wanted.GetTotalInfo(Info);
	GAMEIN->GetWantedDialog()->SetInfo(Info);
	MyWantedListSyn(1);
}

void CWantedManager::SortWantedListSyn(WORD Page)
{
	if(Page == 0)
		return;
	BYTE type = GAMEIN->GetWantNpcDialog()->GetOrderType();

	SEND_WANTED_ORDER_SYN msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_ORDERLIST_SYN;
	msg.dwObjectID = HEROID;
	msg.page = Page;
	msg.type =  type;

	NETWORK->Send(&msg, sizeof(msg));
	GAMEIN->GetWantNpcDialog()->SetCurPage(Page);
}

void CWantedManager::MyWantedListSyn(WORD Page)
{
	ASSERT((Page>0) && (Page<6));	
	
	MSG_WORD msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_MYLIST_SYN;
	msg.dwObjectID = HEROID;
	msg.wData = Page;

	NETWORK->Send(&msg, sizeof(msg));	
	GAMEIN->GetWantNpcDialog()->SetCurPage(Page);
}

void CWantedManager::SetActiveDialog()
{
	if(GetMurdererID() == 0)
	{
		//임시로 PK루팅을 당하는 중에는 부활창이 안뜨도록
		if( !PKMGR->IsPKLooted() )
		{
			if(!GAMEIN->GetReviveDialog()->IsActive())
				GAMEIN->GetReviveDialog()->SetActive(TRUE);
		}
	}
	else if((IsWaiting() == eNone) && (!GAMEIN->GetReviveDialog()->IsActive()))
	{
		if( !GAMEIN->GetWantRegistDialog()->IsActive())
		{
			GAMEIN->GetWantRegistDialog()->SetActive(TRUE);
			SetWaiting(eStart);
		}
	}
}

BOOL CWantedManager::IsChrIDInList(DWORD CharacterID)
{
	return m_Wanted.IsChrIDInList(CharacterID);
}

void CWantedManager::SearchSyn(char* WantedName, WORD gotoPage)
{
	if(strcmp(WantedName, "") == 0)
		return;
			
	MSG_NAME_WORD msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_SEARCH_SYN;
	msg.dwObjectID = HEROID;
	SafeStrCpy(msg.Name, WantedName, MAX_NAME_LENGTH+1);
	msg.wData = gotoPage;
	NETWORK->Send(&msg, sizeof(msg));	
	GAMEIN->GetWantNpcDialog()->SetCurPage(gotoPage);
}