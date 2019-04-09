// QuestNpc.h: interface for the CQuestNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTNPC_H__98C86EC2_02F3_4878_8C8E_8D9AD352BCB9__INCLUDED_)
#define AFX_QUESTNPC_H__98C86EC2_02F3_4878_8C8E_8D9AD352BCB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtrList.h"

class CStrTokenizer;

class CQuestNpc  
{
protected:
	DWORD		m_dwQuestIdx;
	DWORD		m_dwSubQuestIdx;
	cPtrList	m_NpcDataList;

public:
	CQuestNpc( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestNpc();

	void	Release();
	// script
	BOOL	ReadNpcData( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	
	// getfn
	cPtrList*	GetNpcDataList()		{ return &m_NpcDataList; }
};

#endif // !defined(AFX_QUESTNPC_H__98C86EC2_02F3_4878_8C8E_8D9AD352BCB9__INCLUDED_)
