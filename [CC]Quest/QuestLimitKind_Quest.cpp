// QuestLimitKind_Quest.cpp: implementation of the CQuestLimitKind_Quest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestLimitKind_Quest.h"

#include "QuestScriptLoader.h"
#include "Quest.h"
#ifdef _MAPSERVER_
#include "QuestGroup.h"
#else
#include "QuestManager.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestLimitKind_Quest::CQuestLimitKind_Quest( DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestLimitKind( dwLimitKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	GetScriptParam( m_dwIdx, pTokens );
}

CQuestLimitKind_Quest::~CQuestLimitKind_Quest()
{
}

BOOL CQuestLimitKind_Quest::CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_
	switch( m_dwLimitKind )
	{
	case eQuestLimitKind_Quest:
		{
			if( pQuestGroup->GetQuest( m_dwIdx )->IsQuestComplete() )
				return TRUE;
		}
		break;
	case eQuestLimitKind_SubQuest:
		{
			if( pQuest->IsSubQuestComplete( m_dwIdx ) )
				return TRUE;
		}
		break;
	}
#endif
	return FALSE;
}

BOOL CQuestLimitKind_Quest::CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
#ifndef _MAPSERVER_
	switch( m_dwLimitKind )
	{
	case eQuestLimitKind_Quest:
		{
			if( QUESTMGR->GetQuest( m_dwIdx )->IsQuestComplete() )
				return TRUE;
		}
		break;
	case eQuestLimitKind_SubQuest:
		{
			if( QUESTMGR->GetQuest( dwQuestIdx )->IsSubQuestComplete( m_dwIdx ) )
				return TRUE;
		}
		break;
	}
#endif
	return FALSE;
}