// QuestLimitKind_Quest.h: interface for the CQuestLimitKind_Quest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTLIMITKIND_QUEST_H__F6BD9EEC_1613_4D16_B5B9_999DEF6F0864__INCLUDED_)
#define AFX_QUESTLIMITKIND_QUEST_H__F6BD9EEC_1613_4D16_B5B9_999DEF6F0864__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestLimitKind.h"

class CQuestLimitKind_Quest : public CQuestLimitKind  
{
protected:
	DWORD		m_dwIdx;

public:
	CQuestLimitKind_Quest( DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestLimitKind_Quest();

	virtual BOOL	CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );
	virtual BOOL	CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx );
};

#endif // !defined(AFX_QUESTLIMITKIND_QUEST_H__F6BD9EEC_1613_4D16_B5B9_999DEF6F0864__INCLUDED_)
