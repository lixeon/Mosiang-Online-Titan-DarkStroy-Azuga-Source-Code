// QuestExecute_Quest.h: interface for the CQuestExecute_Quest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTEXECUTE_QUEST_H__57E0B691_1C0E_4CD1_9AFA_7466B934B556__INCLUDED_)
#define AFX_QUESTEXECUTE_QUEST_H__57E0B691_1C0E_4CD1_9AFA_7466B934B556__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestExecute.h"

class CQuestExecute_Quest : public CQuestExecute  
{
protected:
	DWORD		m_dwExeQuestIdx;
	DWORD		m_dwExeSubQuestIdx;

public:
	CQuestExecute_Quest( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestExecute_Quest();

	virtual BOOL	Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );

	DWORD			GetQuestExeIdx()		{ return m_dwExeQuestIdx;	}
};

#endif // !defined(AFX_QUESTEXECUTE_QUEST_H__57E0B691_1C0E_4CD1_9AFA_7466B934B556__INCLUDED_)
