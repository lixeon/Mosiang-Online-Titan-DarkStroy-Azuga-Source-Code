// QuestNpcScript.h: interface for the CQuestNpcScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTNPCSCRIPT_H__D5E07727_465F_40AF_93D5_889FF980954F__INCLUDED_)
#define AFX_QUESTNPCSCRIPT_H__D5E07727_465F_40AF_93D5_889FF980954F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtrList.h"

class CStrTokenizer;

class CQuestNpcScript  
{
protected:
	DWORD		m_dwQuestIdx;
	DWORD		m_dwSubQuestIdx;
	cPtrList	m_NpcScriptList;

public:
	CQuestNpcScript( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestNpcScript();

	void	Release();
	// script
	BOOL	ReadNpcScript( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	// getfn
	DWORD	IsSameNpc( DWORD dwNpcIdx, DWORD* pQuestIdx, DWORD* pSubQuestIdx );
	DWORD	IsSameNpcMark( DWORD dwNpcIdx, DWORD* pQuestIdx, DWORD* pSubQuestIdx );
};

#endif // !defined(AFX_QUESTNPCSCRIPT_H__D5E07727_465F_40AF_93D5_889FF980954F__INCLUDED_)
