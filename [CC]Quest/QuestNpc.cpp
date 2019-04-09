// QuestNpc.cpp: implementation of the CQuestNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestNpc.h"

#include "QuestScriptLoader.h"
#include "QuestNpcData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestNpc::CQuestNpc( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	m_dwQuestIdx = dwQuestIdx;
	m_dwSubQuestIdx = dwSubQuestIdx;
	ReadNpcData( pTokens, dwQuestIdx, dwSubQuestIdx );
}

CQuestNpc::~CQuestNpc()
{
	Release();
}

void CQuestNpc::Release()
{
	CQuestNpcData* pNpcData = NULL;
	PTRLISTPOS pos = m_NpcDataList.GetHeadPosition();	
	while( pos )
	{
		pNpcData = (CQuestNpcData*)m_NpcDataList.GetNext( pos );
		if( pNpcData )
		{
			delete pNpcData;
		}
	}
}

BOOL CQuestNpc::ReadNpcData( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	char* OneToken;

	while( (OneToken = pTokens->GetNextTokenUpper()) != NULL )
	{
		if( OneToken[0] == '@' )
			m_NpcDataList.AddTail( CQuestScriptLoader::LoadQuestNpcData( OneToken, pTokens, dwQuestIdx, dwSubQuestIdx ) );
	}
	return TRUE;
}