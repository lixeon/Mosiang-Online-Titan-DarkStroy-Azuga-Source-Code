// QuestLimit.h: interface for the CQuestLimit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTLIMIT_H__B72ED18D_6647_469A_B38E_3BD1B295B469__INCLUDED_)
#define AFX_QUESTLIMIT_H__B72ED18D_6647_469A_B38E_3BD1B295B469__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtrList.h"
#include "QuestDefines.h"

class CStrTokenizer;
class CQuestGroup;
class CQuest;

class CQuestLimit  
{
protected:
	cPtrList	m_QuestLimitKindList;

public:
	CQuestLimit( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestLimit();

	void	Release();
	// script
	BOOL	ReadLimitKind( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	// check
	BOOL	CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );	// server
	BOOL	CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx );							// client
};

#endif // !defined(AFX_QUESTLIMIT_H__B72ED18D_6647_469A_B38E_3BD1B295B469__INCLUDED_)
