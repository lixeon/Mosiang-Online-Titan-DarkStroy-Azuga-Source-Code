// QuestLimitKind_Level.h: interface for the CQuestLimitKind_Level class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTLIMITKIND_LEVEL_H__8B4328F3_3190_4E4D_82D4_184C42A6B07C__INCLUDED_)
#define AFX_QUESTLIMITKIND_LEVEL_H__8B4328F3_3190_4E4D_82D4_184C42A6B07C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestLimitKind.h"

class CQuestLimitKind_Level : public CQuestLimitKind
{
protected:
	DWORD		m_dwMin;
	DWORD		m_dwMax;

public:
	CQuestLimitKind_Level( DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestLimitKind_Level();

	virtual BOOL	CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );
	virtual BOOL	CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx );
};

#endif // !defined(AFX_QUESTLIMITKIND_LEVEL_H__8B4328F3_3190_4E4D_82D4_184C42A6B07C__INCLUDED_)
