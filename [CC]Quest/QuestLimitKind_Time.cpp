// QuestLimitKind_Time.cpp: implementation of the QuestLimitKind_Time class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestLimitKind_Time.h"

#include "QuestScriptLoader.h"
#ifdef _MAPSERVER_
#include "Player.h"
#else
#include "ObjectManager.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

QuestLimitKind_Time::QuestLimitKind_Time( DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestLimitKind( dwLimitKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{

}

QuestLimitKind_Time::~QuestLimitKind_Time()
{

}
