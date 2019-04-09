// QuestEvent.cpp: implementation of the CQuestEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestEvent.h"

#include "StrTokenizer.h"
#include "QuestScriptLoader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CQuestEvent::CQuestEvent( DWORD dwQuestEventKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: m_dwQuestEventKind(dwQuestEventKind)
{
	GetScriptParam( m_dwParam1, pTokens );
	GetScriptParam( m_lParam2, pTokens );
}
CQuestEvent::CQuestEvent( DWORD dwQuestEventKind, DWORD dwParam1, LONG lParam2 )
{
	SetValue(dwQuestEventKind,dwParam1,lParam2);
}

CQuestEvent::~CQuestEvent()
{
}

void CQuestEvent::SetValue( CQuestEvent* pQuestEvent )
{
	m_dwQuestEventKind = pQuestEvent->m_dwQuestEventKind;
	m_dwParam1 = pQuestEvent->m_dwParam1;
	m_lParam2 = pQuestEvent->m_lParam2;
}

void CQuestEvent::SetValue( DWORD dwQuestEventKind, DWORD dwParam1, LONG lParam2 )
{
	m_dwQuestEventKind = dwQuestEventKind;
	m_dwParam1 = dwParam1;
	m_lParam2 = lParam2;
}

BOOL CQuestEvent::IsSame( CQuestEvent* pQuestEvent )
{
	if( m_dwQuestEventKind != pQuestEvent->m_dwQuestEventKind )
		return FALSE;
	if( m_dwParam1 != pQuestEvent->m_dwParam1 )
		return FALSE;
	if( m_lParam2 != pQuestEvent->m_lParam2 )
		return FALSE;
	return TRUE;
}
