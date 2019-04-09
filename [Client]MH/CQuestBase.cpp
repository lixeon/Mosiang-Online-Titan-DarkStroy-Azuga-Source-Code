#include "stdafx.h"
#include "CQuestBase.h"

#ifndef _MAPSERVER_	
#include "mhnetwork.h"
#include "objectmanager.h"
#endif

CQuestBase::CQuestBase()
{
	m_nValidNum		= 0;
	m_dwQuestIdx	= 0;	
}

CQuestBase::~CQuestBase()
{
	
}

void CQuestBase::Init(DWORD idx, QSTATETYPE state)
{
	m_dwQuestIdx = idx;
	m_State.value = state;
}

void CQuestBase::SetState(BYTE field)
{
	m_State.SetField(field);

#ifndef _MAPSERVER_	

	MSG_DWORD2 msg;
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_CHANGESTATE;
	msg.dwObjectID = HERO->GetID();
	msg.dwData1 = m_dwQuestIdx;
	msg.dwData2 = m_State.value;
	NETWORK->Send(&msg, sizeof(msg));

#endif
}

void CQuestBase::SetValue(QSTATETYPE value)
{
	m_State.value = value;

#ifndef _MAPSERVER_	

	MSG_DWORD2 msg;
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_CHANGESTATE;
	msg.dwObjectID = HERO->GetID();
	msg.dwData1 = m_dwQuestIdx;
	msg.dwData2 = m_State.value;
	NETWORK->Send(&msg, sizeof(msg));

#endif
}