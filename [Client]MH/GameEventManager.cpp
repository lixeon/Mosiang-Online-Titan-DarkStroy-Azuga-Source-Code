#include "stdafx.h"
#include "GameEventManager.h"
#include "CQuestManager.h"
#include "QuestManager.h"


GLOBALTON(CGameEventManager);
CGameEventManager::CGameEventManager()
{
	memset(m_EventPool, 0, sizeof(GAMEEVENT)*MAX_EVENTCOUNT);
	m_wEventCount = 0;
}

CGameEventManager::~CGameEventManager()
{
}

void CGameEventManager::AddEvent(DWORD event, DWORD dwData)
{
	if( m_wEventCount > MAX_EVENTCOUNT )
	{
		ASSERTMSG(0, "EventPool Memory Error. ^^;;");
		return;
	}

	m_EventPool[m_wEventCount].Init((BYTE)event, dwData);
	++m_wEventCount;
}

void CGameEventManager::Process()
{
	if( m_wEventCount == 0 ) return;

	for(int i=0; i< m_wEventCount; ++i)
	{
		// event¸¦ »ç¿ëÇÒ °÷¿¡ °ªÀ» ³Ö¾î ÁØ´Ù.
//		QUESTMGR1->CheckState(&m_EventPool[i]);
		QUESTMGR->CheckGameEvent( &m_EventPool[i] );
	}

	// Event »èÁ¦
	memset(m_EventPool, 0, sizeof(GAMEEVENT)*MAX_EVENTCOUNT);
	m_wEventCount = 0;
}
