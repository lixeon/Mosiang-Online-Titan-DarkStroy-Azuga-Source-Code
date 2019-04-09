// QuestNpcData.h: interface for the CQuestNpcData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTNPCDATA_H__BFE07D68_8F07_4142_9C5B_1CBFD5DC1D69__INCLUDED_)
#define AFX_QUESTNPCDATA_H__BFE07D68_8F07_4142_9C5B_1CBFD5DC1D69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStrTokenizer;

class CQuestNpcData  
{
protected:
	DWORD		m_dwNpcIdx;

public:
	CQuestNpcData( DWORD dwKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestNpcData();

	// getfn
	DWORD	GetNpcIdx()			{ return m_dwNpcIdx; }
};

#endif // !defined(AFX_QUESTNPCDATA_H__BFE07D68_8F07_4142_9C5B_1CBFD5DC1D69__INCLUDED_)
