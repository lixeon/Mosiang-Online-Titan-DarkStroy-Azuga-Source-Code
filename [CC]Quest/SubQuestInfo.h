// SubQuestInfo.h: interface for the CSubQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBQUESTINFO_H__AC05E747_C207_4E04_8D35_09F423BA4A7E__INCLUDED_)
#define AFX_SUBQUESTINFO_H__AC05E747_C207_4E04_8D35_09F423BA4A7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtrList.h"
#include "QuestDefines.h"

class CQuestLimit;
class CQuestTrigger;
class CQuestNpcScript;
class CQuestNpc;

class CQuestGroup;
class CQuest;
class CQuestEvent;

class CSubQuestInfo  
{
protected:
	cPtrList	m_QuestLimitList;
	cPtrList	m_QuestTriggerList;	
	cPtrList	m_QuestNpcScriptList;
	cPtrList	m_QuestNpcList;
	DWORD		m_dwMaxCount;

public:
	CSubQuestInfo();
	virtual ~CSubQuestInfo();
	
	void	Release();
	// script
	void	AddQuestLimit( CQuestLimit* pQuestLimit );
	void	AddQuestTrigger( CQuestTrigger* pQuestTrigger );
	void	AddQuestNpcScipt( CQuestNpcScript* pQuestNpcScript );
	void	AddQuestNpc( CQuestNpc* pQuestNpc );

	// event
	void	OnQuestEvent( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest, CQuestEvent* pQuestEvent );
	// npc
	BOOL	IsNpcRelationQuest( DWORD dwNpcIdx );
	DWORD	GetNpcScriptPage( DWORD dwNpcIdx );
	DWORD	GetNpcMarkType( DWORD dwNpcIdx );	
	int		IsQuestState( DWORD dwNpcIdx );
	
	// getfn
	cPtrList*	GetQuestNpcList()		{ return &m_QuestNpcList; }
	
	// item use
	BOOL	CheckLimitCondition( DWORD dwQuestIdx );

	DWORD	GetMaxCount()						{ return m_dwMaxCount; }
	void	SetMaxCount( DWORD dwMaxCount )		{ m_dwMaxCount = dwMaxCount; }
};

#endif // !defined(AFX_SUBQUESTINFO_H__AC05E747_C207_4E04_8D35_09F423BA4A7E__INCLUDED_)
