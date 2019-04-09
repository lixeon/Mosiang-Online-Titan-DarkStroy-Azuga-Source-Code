// QuestNpcScript.cpp: implementation of the CQuestNpcScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestNpcScript.h"

#include "QuestScriptLoader.h"
#include "QuestNpcScriptData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestNpcScript::CQuestNpcScript( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	m_dwQuestIdx = dwQuestIdx;
	m_dwSubQuestIdx = dwSubQuestIdx;
	ReadNpcScript( pTokens, dwQuestIdx, dwSubQuestIdx );
}

CQuestNpcScript::~CQuestNpcScript()
{
	Release();
}

void CQuestNpcScript::Release()
{
	CQuestNpcScriptData* pNpcScriptData = NULL;
	PTRLISTPOS pos = m_NpcScriptList.GetHeadPosition();	
	while( pos )
	{
		pNpcScriptData = (CQuestNpcScriptData*)m_NpcScriptList.GetNext( pos );
		if( pNpcScriptData )
		{
			delete pNpcScriptData;
		}
	}
}

BOOL CQuestNpcScript::ReadNpcScript( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	char* OneToken;

	while( (OneToken = pTokens->GetNextTokenUpper()) != NULL )
	{
		if( OneToken[0] == '@' )
			m_NpcScriptList.AddTail( CQuestScriptLoader::LoadQuestNpcScriptData( OneToken, pTokens, dwQuestIdx, dwSubQuestIdx ) );
	}
	return TRUE;
}

DWORD CQuestNpcScript::IsSameNpc( DWORD dwNpcIdx, DWORD* pQuestIdx, DWORD* pSubQuestIdx )
{
	*pQuestIdx = m_dwQuestIdx;
	*pSubQuestIdx = m_dwSubQuestIdx;

	DWORD dwPage = 0;
	CQuestNpcScriptData* pNpcScriptData = NULL;
	PTRLISTPOS pos = m_NpcScriptList.GetHeadPosition();	
	while( pos )
	{
		pNpcScriptData = (CQuestNpcScriptData*)m_NpcScriptList.GetNext( pos );
		if( pNpcScriptData )
		if( dwPage = pNpcScriptData->IsSameNpc( dwNpcIdx ) )
			return dwPage;
	}
	return dwPage;
}

DWORD CQuestNpcScript::IsSameNpcMark( DWORD dwNpcIdx, DWORD* pQuestIdx, DWORD* pSubQuestIdx )
{
	*pQuestIdx = m_dwQuestIdx;
	*pSubQuestIdx = m_dwSubQuestIdx;

	DWORD dwNpcMark = 0;
	CQuestNpcScriptData* pNpcScriptData = NULL;
	PTRLISTPOS pos = m_NpcScriptList.GetHeadPosition();	
	while( pos )
	{
		pNpcScriptData = (CQuestNpcScriptData*)m_NpcScriptList.GetNext( pos );
		if( pNpcScriptData )
		if( dwNpcMark = pNpcScriptData->IsSameNpcMark( dwNpcIdx ) )
			return dwNpcMark;
	}
	return dwNpcMark;
}
