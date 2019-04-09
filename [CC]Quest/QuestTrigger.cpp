// QuestTrigger.cpp: implementation of the CQuestTrigger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestTrigger.h"

#include "QuestScriptLoader.h"
#include "QuestCondition.h"
//#include "QuestLimit.h"
#include "QuestExecute.h"
#include "QuestEvent.h"
#ifdef _MAPSERVER_
#include "Player.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestTrigger::CQuestTrigger( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	ReadTrigger( pTokens, dwQuestIdx, dwSubQuestIdx );
}

CQuestTrigger::~CQuestTrigger()
{
	Release();
}

void CQuestTrigger::Release()
{
	if( m_pQuestCondition )
	{
		delete m_pQuestCondition;
		m_pQuestCondition = NULL;
	}

	CQuestExecute* pQuestExecute = NULL;
	PTRLISTPOS pos = m_QuestExeList.GetHeadPosition();
	while( pos )
	{
		pQuestExecute = (CQuestExecute*)m_QuestExeList.GetNext( pos );
		if( pQuestExecute )
		{
			delete pQuestExecute;
		}
	}
	m_QuestExeList.RemoveAll();
}

#include "QuestExecute_Quest.h"

BOOL CQuestTrigger::ReadTrigger( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	char* OneToken;
	m_dwEndParam = 0;

	while( (OneToken = pTokens->GetNextTokenUpper()) != NULL )
	{
		if( OneToken[0] == '@' )
			m_pQuestCondition = CQuestScriptLoader::LoadQuestCondition( OneToken, pTokens, dwQuestIdx, dwSubQuestIdx );
		else if( OneToken[0] == '*' )
		{
			// 반복퀘스트의 확인을 위해서 추가 RaMa - 04.10.26
			CQuestExecute* pQuestExecute = CQuestScriptLoader::LoadQuestExecute( OneToken, pTokens, dwQuestIdx, dwSubQuestIdx );
			if(pQuestExecute && pQuestExecute->GetQuestExecuteKind() == eQuestExecute_EndQuest)
			{
				CQuestExecute_Quest* pQExe_Quest = (CQuestExecute_Quest*)pQuestExecute;
				m_dwEndParam = pQExe_Quest->GetQuestExeIdx();
			}
			m_QuestExeList.AddTail( pQuestExecute );		
		}
	}
	return TRUE;
}

BOOL CQuestTrigger::OnQuestEvent( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest, CQuestEvent* pQuestEvent )
{
	// condition
	if( !m_pQuestCondition->CheckCondition( pQuestEvent ) )
		return FALSE;

	// execute
	BOOL bError = TRUE;
	PTRLISTPOS pos = m_QuestExeList.GetHeadPosition();
	CQuestExecute* pQuestExecute = NULL;
	while( pos )
	{
		pQuestExecute = (CQuestExecute*)m_QuestExeList.GetNext( pos );
		if( pQuestExecute )
		{
			bError = pQuestExecute->Execute( pPlayer, pQuestGroup, pQuest );
			if( bError == FALSE )	break;
		}
	}
#ifdef _MAPSERVER_
	if( bError == FALSE )
	{
		// network send
		MSGBASE Msg;
		Msg.Category = MP_QUEST;
		Msg.Protocol = MP_QUEST_EXECUTE_ERROR;
		Msg.dwObjectID = pPlayer->GetID();
		pPlayer->SendMsg( &Msg, sizeof(Msg) );
	}
#endif

	return TRUE;
}
