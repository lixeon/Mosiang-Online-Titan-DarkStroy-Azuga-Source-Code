#include "stdafx.h"
#include "CQuestManager.h"
#include "CQuestBase.h"
#include "NewbieGuid.h"
#include "ChatManager.h"

GLOBALTON(CQuestManager1);
CQuestManager1::CQuestManager1()
{
	m_QuestList = new CYHHashTable<CQuestBase>;
	m_QuestList->Initialize(100);
}

CQuestManager1::~CQuestManager1()
{
	Release();
	SAFE_DELETE(m_QuestList);
}

void CQuestManager1::Init()
{
}

void CQuestManager1::Release()
{
	m_QuestList->SetPositionHead();
	CQuestBase* pBase = NULL;

	while( pBase = m_QuestList->GetData() )
	{
		SAFE_DELETE(pBase);
	}
	m_QuestList->RemoveAll();
}

void CQuestManager1::AddQuest(CQuestBase* pQuest)
{
	m_QuestList->Add(pQuest, pQuest->GetQuestIdx());
}

void CQuestManager1::CheckState(GAMEEVENT* event)
{
	m_QuestList->SetPositionHead();
	CQuestBase* pBase = NULL;

	while( pBase = m_QuestList->GetData() )
	{
		// check
		pBase->CheckCondition(event);
	}
}

void CQuestManager1::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_QUEST_TOTALINFO:
		{
			// QuestList를 구성한다.
			SEND_QUEST_TOTALINFO* pmsg = (SEND_QUEST_TOTALINFO*)pMsg;
			if( pmsg->wCount != 0 )
			{
				CQuestBase* pQuest = NULL;
				for(int i=0;i<pmsg->wCount;++i)
				{
					if( pmsg->QuestList[i].QuestIdx == 1 ) // 초보자 가이드 땜..
					{
						CNewbieGuid* pGuid = new CNewbieGuid;
						pGuid->Init(pmsg->QuestList[i].QuestIdx, pmsg->QuestList[i].state);
						AddQuest(pGuid);
					}
					else
					{
						pQuest = new CQuestBase;
						pQuest->Init(pmsg->QuestList[i].QuestIdx, pmsg->QuestList[i].state);
						AddQuest(pQuest);
					}
				}
			}
		}
		break;
		/*
	case MP_QUEST_REMOVE_NOTIFY:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			m_QuestList->Remove(pmsg->dwData);
		}
		break;
	case MP_QUEST_TEST_START_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, "%d번 퀘스트를 시작합니다.", pmsg->dwData );
		}
		break;
		*/
	default:
		break;
	}
}