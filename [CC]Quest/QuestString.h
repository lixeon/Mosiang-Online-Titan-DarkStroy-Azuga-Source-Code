// QuestString.h: interface for the CQuestString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTSTRING_H__B9E2D767_CA20_4E93_BC6F_C83DA1368719__INCLUDED_)
#define AFX_QUESTSTRING_H__B9E2D767_CA20_4E93_BC6F_C83DA1368719__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ptrlist.h"
#include "..\[CC]Quest\QuestDefines.h"


class CQuestString
{	
	cPtrList		m_TitleList;
	cPtrList		m_DescList;

	DWORD			m_mIndex;
	DWORD			m_sIndex;
	DWORD			m_Key;
	
public:
	
	CQuestString();
	virtual ~CQuestString();

	void	StringLoader(char* str);
	void	SetIndex(DWORD mIdx, DWORD sIdx)		{	m_mIndex = mIdx;	m_sIndex=sIdx;	COMBINEKEY(m_mIndex, m_sIndex, m_Key);	}
	DWORD	GetQuestID()	{	return m_mIndex;	}
	DWORD	GetSubID()		{	return m_sIndex;	}
	DWORD	GetKey()		{	return m_Key;		}
	void	AddLine(char* pStr, int& line, BOOL bTitle=FALSE);

	cPtrList*	GetTitle()	{	return &m_TitleList;	}
	cPtrList*	GetDesc()	{	return &m_DescList;	}
};

#endif // !defined(AFX_QUESTSTRING_H__B9E2D767_CA20_4E93_BC6F_C83DA1368719__INCLUDED_)
