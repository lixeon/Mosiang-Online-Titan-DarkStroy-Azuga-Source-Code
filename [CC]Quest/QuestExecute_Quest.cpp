// QuestExecute_Quest.cpp: implementation of the CQuestExecute_Quest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestExecute_Quest.h"

#include "QuestScriptLoader.h"
#ifdef _MAPSERVER_
#include "QuestGroup.h"
#include "QuestManager.h"
#include "QuestInfo.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestExecute_Quest::CQuestExecute_Quest( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestExecute( dwExecuteKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	m_dwExeQuestIdx = m_dwExeSubQuestIdx = 0;
	if( m_dwExecuteKind == eQuestExecute_StartSub )
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );
		GetScriptParam( m_dwExeSubQuestIdx, pTokens );
	}
	else if( m_dwExecuteKind == eQuestExecute_EndQuest )
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );
	}
	else if( m_dwExecuteKind == eQuestExecute_EndOtherSub )
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );
		GetScriptParam( m_dwExeSubQuestIdx, pTokens );
	}
	else if( m_dwExecuteKind == eQuestExecute_RegenMonster )
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );
	}
	else if( m_dwExecuteKind == eQuestExecute_MapChange )
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );
		GetScriptParam( m_dwExeSubQuestIdx, pTokens );
	}
	else if( m_dwExecuteKind == eQuestExecute_ChangeStage )
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );
	}
	else if( m_dwExecuteKind == eQuestExecute_ChangeSubAttr )
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );
	}
	else if( m_dwExecuteKind == eQuestExecute_EndOtherQuest )
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );
		GetScriptParam( m_dwExeSubQuestIdx, pTokens );
	}
	else if( m_dwExecuteKind == eQuestExecute_SaveLoginPoint )
	{
		GetScriptParam( m_dwExeQuestIdx, pTokens );
	}
}

CQuestExecute_Quest::~CQuestExecute_Quest()
{
}

BOOL CQuestExecute_Quest::Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_
	switch( m_dwExecuteKind )
	{
	case eQuestExecute_EndQuest:
		{			
			pQuestGroup->EndQuest( m_dwQuestIdx, m_dwExeQuestIdx );
		}
		break;		
	case eQuestExecute_StartSub:
		{
			pQuestGroup->StartSubQuest( m_dwExeQuestIdx, m_dwExeSubQuestIdx );
		}
		break;
	case eQuestExecute_EndSub:
		{
			pQuestGroup->EndSubQuest( m_dwQuestIdx, m_dwSubQuestIdx );
		}
		break;
	case eQuestExecute_EndOtherSub:
		{
			pQuestGroup->EndSubQuest( m_dwExeQuestIdx, m_dwExeSubQuestIdx );
		}
		break;
	case eQuestExecute_RegenMonster:
		{
			pQuestGroup->RegenMonster( m_dwExeQuestIdx );
		}
		break;
	case eQuestExecute_MapChange:
		{
			pQuestGroup->MapChange( (WORD)m_dwExeQuestIdx, (WORD)m_dwExeSubQuestIdx );
		}
		break;
	case eQuestExecute_ChangeStage:
		{
			pQuestGroup->ChangeStage( (BYTE)m_dwExeQuestIdx );
		}
		break;
	case eQuestExecute_EndOtherQuest:
		{
			pQuestGroup->EndOtherQuest( m_dwExeQuestIdx, m_dwExeSubQuestIdx );
		}
		break;
	case eQuestExecute_SaveLoginPoint:
		{
			pQuestGroup->SaveLoginPoint( m_dwExeQuestIdx );
		}
		break;
#ifdef _JAPAN_LOCAL_
	case eQuestExecute_ChangeSubAttr:
		{
			pQuestGroup->ChangeSubAttr( m_dwExeQuestIdx );
		}
		break;
#endif	// _JAPAN_LOCAL_
	}
#endif	// _MAPSERVER_
	return TRUE;
}
