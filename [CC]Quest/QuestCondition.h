// QuestCondition.h: interface for the CQuestCondition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTCONDITION_H__ED1643D9_D166_4EB4_B20B_C085F28ED8E6__INCLUDED_)
#define AFX_QUESTCONDITION_H__ED1643D9_D166_4EB4_B20B_C085F28ED8E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CQuestEvent;

class CQuestCondition  
{
protected:
	DWORD			m_dwQuestIdx;
	DWORD			m_dwSubQuestIdx;
	CQuestEvent*	m_pQuestEvent;

public:
	CQuestCondition( CQuestEvent* pQuestEvent, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestCondition();

	CQuestEvent*	GetQuestEvent()		{ return m_pQuestEvent; }

	BOOL	CheckCondition( CQuestEvent* pQuestEvent );	
	BOOL	IsSameQuest( DWORD dwQuestIdx )	{ return (m_dwQuestIdx == dwQuestIdx) ? TRUE : FALSE; }	
};

#endif // !defined(AFX_QUESTCONDITION_H__ED1643D9_D166_4EB4_B20B_C085F28ED8E6__INCLUDED_)
