// QuestTrigger.h: interface for the CQuestTrigger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTTRIGGER_H__E9012D36_44B8_469F_A5D6_4E42CDE29383__INCLUDED_)
#define AFX_QUESTTRIGGER_H__E9012D36_44B8_469F_A5D6_4E42CDE29383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtrList.h"
#include "QuestDefines.h"

class CQuestCondition;
class CStrTokenizer;

class CQuestGroup;
class CQuest;
class CQuestEvent;

class CQuestTrigger  
{
protected:
	CQuestCondition*	m_pQuestCondition;
	cPtrList			m_QuestExeList;	
	DWORD				m_dwEndParam;

public:
	CQuestTrigger( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestTrigger();

	void	Release();
	
	// script
	BOOL	ReadTrigger( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );

	// event
	BOOL	OnQuestEvent( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest, CQuestEvent* pQuestEvent );

	//
	DWORD	GetEndParam()		{ return m_dwEndParam;	}
};

#endif // !defined(AFX_QUESTTRIGGER_H__E9012D36_44B8_469F_A5D6_4E42CDE29383__INCLUDED_)
