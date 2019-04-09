// QuestCondition.cpp: implementation of the CQuestCondition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestCondition.h"
#include "QuestDefines.h"
#include "QuestEvent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestCondition::CQuestCondition( CQuestEvent* pQuestEvent, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	m_dwQuestIdx = dwQuestIdx;
	m_dwSubQuestIdx = dwSubQuestIdx;
	m_pQuestEvent = pQuestEvent;	
}

CQuestCondition::~CQuestCondition()
{
	if( m_pQuestEvent )
	{
		delete m_pQuestEvent;
	}
}

BOOL CQuestCondition::CheckCondition( CQuestEvent* pQuestEvent )
{
	if( m_pQuestEvent->m_dwQuestEventKind == eQuestEvent_HuntAll )
	{
		if( pQuestEvent->m_dwQuestEventKind == eQuestEvent_Hunt )
			return TRUE;
		else 
			return FALSE;
	}

	return m_pQuestEvent->IsSame( pQuestEvent );
}