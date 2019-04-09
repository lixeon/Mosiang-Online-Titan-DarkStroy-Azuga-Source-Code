// QuestNpcData.cpp: implementation of the CQuestNpcData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestNpcData.h"

#include "QuestScriptLoader.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestNpcData::CQuestNpcData( DWORD dwKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	GetScriptParam( m_dwNpcIdx, pTokens );
}

CQuestNpcData::~CQuestNpcData()
{
}
