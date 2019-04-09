// QuestLimitKind.cpp: implementation of the CQuestLimitKind class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestLimitKind.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestLimitKind::CQuestLimitKind( DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	m_dwQuestIdx = dwQuestIdx;
	m_dwSubQuestIdx = dwSubQuestIdx;
	m_dwLimitKind = dwLimitKind;
}

CQuestLimitKind::~CQuestLimitKind()
{
}