// QuestExecute_Time.cpp: implementation of the QuestExecute_Time class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestExecute_Time.h"

#include "QuestScriptLoader.h"
#ifdef _MAPSERVER_
#include "QuestGroup.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestExecute_Time::CQuestExecute_Time( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestExecute( dwExecuteKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	GetScriptParam( m_bType, pTokens );
	GetScriptParam( m_dwDay, pTokens );
	GetScriptParam( m_dwHour, pTokens );
	GetScriptParam( m_dwMin, pTokens );
}

CQuestExecute_Time::~CQuestExecute_Time()
{
}

BOOL CQuestExecute_Time::Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_
	switch( m_dwExecuteKind )
	{
	case eQuestExecute_RegistTime:
		{
			pQuestGroup->RegistCheckTime( m_dwQuestIdx, m_dwSubQuestIdx, m_bType, m_dwDay, m_dwHour, m_dwMin );
		}
		break;
	}
#endif	
	return TRUE;
}
