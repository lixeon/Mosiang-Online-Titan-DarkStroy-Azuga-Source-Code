// QuestExecute.h: interface for the CQuestExecute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTEXECUTE_H__4FDEA86A_E9B2_40CC_A83D_F7864A04BDBA__INCLUDED_)
#define AFX_QUESTEXECUTE_H__4FDEA86A_E9B2_40CC_A83D_F7864A04BDBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestDefines.h"

class CStrTokenizer;
class CQuestGroup;
class CQuest;

class CQuestExecute  
{
protected:
	DWORD	m_dwQuestIdx;
	DWORD	m_dwSubQuestIdx;
	DWORD	m_dwExecuteKind;

public:
	CQuestExecute( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestExecute();

	virtual BOOL	Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest ) = 0;
	
	DWORD			GetQuestExecuteKind()		{ return m_dwExecuteKind;	}
};

#endif // !defined(AFX_QUESTEXECUTE_H__4FDEA86A_E9B2_40CC_A83D_F7864A04BDBA__INCLUDED_)
