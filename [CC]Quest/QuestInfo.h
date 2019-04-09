// QuestInfo.h: interface for the CQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTINFO_H__50E7B712_25F6_44BA_91DF_710117F02237__INCLUDED_)
#define AFX_QUESTINFO_H__50E7B712_25F6_44BA_91DF_710117F02237__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestDefines.h"

class CSubQuestInfo;

class CQuestInfo  
{
protected:
	DWORD						m_dwQuestIdx;
	DWORD						m_dwSubQuestCount;
	DWORD						m_dwEndParam;
	CYHHashTable<CSubQuestInfo>	m_SubQuestTable;

public:
	CQuestInfo( DWORD dwQuestIdx );
	virtual ~CQuestInfo();

	void	Release();

	// script
	void	AddSubQuestInfo( DWORD dwSubQuestIdx, CSubQuestInfo* pSubQuest );
	void	SetEndParam( DWORD Data )						{ m_dwEndParam = Data;	}

	// getfn
	DWORD			GetQuestIdx()							{ return m_dwQuestIdx; }
	DWORD			GetSubQuestCount()						{ return m_dwSubQuestCount; }
	CSubQuestInfo*	GetSubQuestInfo( DWORD dwSubQuestIdx )	{ return m_SubQuestTable.GetData( dwSubQuestIdx ); }		
	DWORD			GetEndParam()							{ return m_dwEndParam;	}
};

#endif // !defined(AFX_QUESTINFO_H__50E7B712_25F6_44BA_91DF_710117F02237__INCLUDED_)
