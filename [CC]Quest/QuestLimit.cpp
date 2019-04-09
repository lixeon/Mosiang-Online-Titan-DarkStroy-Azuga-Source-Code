// QuestLimit.cpp: implementation of the CQuestLimit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestLimit.h"

#include "QuestScriptLoader.h"
#include "QuestLimitKind.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestLimit::CQuestLimit( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	ReadLimitKind( pTokens,  dwQuestIdx, dwSubQuestIdx );
}

CQuestLimit::~CQuestLimit()
{
	Release();
}

void CQuestLimit::Release()
{
	PTRLISTPOS pos = m_QuestLimitKindList.GetHeadPosition();
	CQuestLimitKind* pQuestLimitKind = NULL;
	while( pos )
	{
		pQuestLimitKind = (CQuestLimitKind*)m_QuestLimitKindList.GetNext( pos );
		if( pQuestLimitKind )
		{
			delete pQuestLimitKind;
		}
	}
	m_QuestLimitKindList.RemoveAll();
}

BOOL CQuestLimit::ReadLimitKind( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	char* OneToken;

	while( (OneToken = pTokens->GetNextTokenUpper()) != NULL )
	{
		if( OneToken[0] == '&' )
			m_QuestLimitKindList.AddTail( CQuestScriptLoader::LoadQuestLimitKind( OneToken, pTokens, dwQuestIdx, dwSubQuestIdx ) );
	}
	return TRUE;
}

BOOL CQuestLimit::CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
	PTRLISTPOS pos = m_QuestLimitKindList.GetHeadPosition();
	CQuestLimitKind* pQuestLimitKind = NULL;
	while( pos )
	{
		pQuestLimitKind = (CQuestLimitKind*)m_QuestLimitKindList.GetNext( pos );
		if( pQuestLimitKind )
		if( !pQuestLimitKind->CheckLimit( pPlayer, pQuestGroup, pQuest ) )
			return FALSE;
	}
	return TRUE;
}

BOOL CQuestLimit::CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	PTRLISTPOS pos = m_QuestLimitKindList.GetHeadPosition();
	CQuestLimitKind* pQuestLimitKind = NULL;
	while( pos )
	{
		pQuestLimitKind = (CQuestLimitKind*)m_QuestLimitKindList.GetNext( pos );
		if( pQuestLimitKind )
		if( !pQuestLimitKind->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )
			return FALSE;
	}
	return TRUE;
}
