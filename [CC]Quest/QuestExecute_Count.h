// QuestExecute_Count.h: interface for the CQuestExecute_Count class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTEXECUTE_COUNT_H__DE6A351F_3B48_4B9F_9724_EBAABAAD99B2__INCLUDED_)
#define AFX_QUESTEXECUTE_COUNT_H__DE6A351F_3B48_4B9F_9724_EBAABAAD99B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestExecute.h"

class CQuestExecute_Count : public CQuestExecute 
{
protected:
	DWORD		m_dwRealSubQuestIdx;
	DWORD		m_dwMaxCount;	
	DWORD		m_dwWeaponKind;
	DWORD		m_dwParam1;
	
public:
	CQuestExecute_Count( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestExecute_Count();

	virtual BOOL	Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );
};

#endif // !defined(AFX_QUESTEXECUTE_COUNT_H__DE6A351F_3B48_4B9F_9724_EBAABAAD99B2__INCLUDED_)
