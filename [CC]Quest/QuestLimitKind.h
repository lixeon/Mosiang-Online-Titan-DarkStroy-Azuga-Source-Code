// QuestLimitKind.h: interface for the CQuestLimitKind class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTLIMITKIND_H__4048A25A_091D_4BEB_986E_3C324C0D32EF__INCLUDED_)
#define AFX_QUESTLIMITKIND_H__4048A25A_091D_4BEB_986E_3C324C0D32EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestDefines.h"

class CStrTokenizer;
class CQuestGroup;
class CQuest;

class CQuestLimitKind  
{
protected:
	DWORD	m_dwQuestIdx;
	DWORD	m_dwSubQuestIdx;
	DWORD	m_dwLimitKind;
	
public:
	CQuestLimitKind( DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestLimitKind();

	virtual BOOL	CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest ) = 0;
	virtual BOOL	CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx ) = 0;	
};

#endif // !defined(AFX_QUESTLIMITKIND_H__4048A25A_091D_4BEB_986E_3C324C0D32EF__INCLUDED_)
