// QuestEvent.h: interface for the CQuestEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTEVENT_H__0FAE1D70_917B_4C7D_A672_60DE84A4413B__INCLUDED_)
#define AFX_QUESTEVENT_H__0FAE1D70_917B_4C7D_A672_60DE84A4413B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStrTokenizer;

class CQuestEvent
{
public:
	DWORD	m_dwQuestEventKind;
	DWORD	m_dwParam1;
	LONG	m_lParam2;
	
public:
	CQuestEvent() : m_dwQuestEventKind(0), m_dwParam1(0), m_lParam2(0)	{}
	CQuestEvent( DWORD dwQuestEventKind, DWORD dwParam1, LONG lParam2 );	// ÀÌº¥Æ® ¹ß»ý¿ë
	CQuestEvent( DWORD dwQuestEventKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	// script·ÎºÎÅÍ 
	virtual ~CQuestEvent();
	
	void	SetValue( CQuestEvent* pQuestEvent );
	void	SetValue( DWORD dwQuestEventKind, DWORD dwParam1, LONG lParam2 );
	BOOL	IsSame( CQuestEvent* pQuestEvent );
};

#endif // !defined(AFX_QUESTEVENT_H__0FAE1D70_917B_4C7D_A672_60DE84A4413B__INCLUDED_)
