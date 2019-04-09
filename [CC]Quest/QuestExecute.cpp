// QuestExecute.cpp: implementation of the CQuestExecute class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestExecute.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestExecute::CQuestExecute( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	m_dwQuestIdx = dwQuestIdx;
	m_dwSubQuestIdx = dwSubQuestIdx;
	m_dwExecuteKind = dwExecuteKind;
}

CQuestExecute::~CQuestExecute()
{
}

