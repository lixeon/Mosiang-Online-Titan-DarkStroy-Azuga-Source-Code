// QuestNpcScriptData.cpp: implementation of the CQuestNpcScriptData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestNpcScriptData.h"

#include "QuestScriptLoader.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestNpcScriptData::CQuestNpcScriptData( DWORD dwKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	GetScriptParam( m_dwNpcIdx, pTokens );
	GetScriptParam( m_dwScriptPage, pTokens );
	GetScriptParam( m_dwNpcMarkType, pTokens );	
}

CQuestNpcScriptData::~CQuestNpcScriptData()
{
}

