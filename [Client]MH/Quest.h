// Quest.h: interface for the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEST_H__60770757_F904_4A60_A796_6E1092C9447D__INCLUDED_)
#define AFX_QUEST_H__60770757_F904_4A60_A796_6E1092C9447D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\[CC]Quest\QuestDefines.h"

class CQuestInfo;
class CSubQuestInfo;

class CQuest  
{
protected:
	CQuestInfo*			m_pQuestInfo;							// script Á¤º¸
	CSubQuestInfo*		m_CurSubQuestInfoArray[MAX_SUBQUEST];	// ÇöÀç ÁøÇàÇÒ subquest
	
	DWORD				m_dwSubQuestCount;
	SUBQUEST			m_SubQuestTable[MAX_SUBQUEST];			// subquest Á¤º¸
	DWORD				m_dwSubQuestFlag;
	DWORD				m_dwData;								// 0ÀÌ ¾Æ´Ï¸é Äù½ºÆ® ³¡
	DWORD				m_dwTime;	
	
	BOOL				m_bCheckTime;
	BYTE				m_CheckType;
	stTIME				m_CheckTime;

public:
	CQuest( CQuestInfo* pQuestInfo );
	virtual ~CQuest();

	// quest Á¤º¸¸¸ release
	void	Initialize();
	// set
	void	SetMainQuestData( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime );
	void	SetSubQuestData( DWORD dwSubQuestIdx, DWORD dwSubData, DWORD dwSubTime );
	void	StartQuest( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime );
	void	EndQuest( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime );
	void	StartSubQuest( DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime );
	void	EndSubQuest( DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime );
	void	UpdateSubQuestData( DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwSubData, DWORD dwSubTime );
	// getfn
	inline DWORD	IsQuestComplete()		{ return m_dwData; }
	inline DWORD	GetTime()				{ return m_dwTime; }
	inline DWORD	GetEndParam()			{ return m_dwData; }
	inline DWORD	GetSubQuestFlag()		{ return m_dwSubQuestFlag; }
	inline BOOL		IsTimeCheck()			{ return m_bCheckTime; }
	inline BYTE		GetCheckType()			{ return m_CheckType; }
	inline stTIME*	GetCheckTime()			{ return &m_CheckTime; }

	DWORD			GetQuestIdx();
	BOOL			IsSubQuestComplete( DWORD dwSubQuestIdx );
	CSubQuestInfo*	GetSubQuestInfo( DWORD dwSubQuestIdx );
	SUBQUEST* GetSubQuest(int Index)		{ return &m_SubQuestTable[Index]; }

	// npc
	BOOL	IsNpcRelationQuest( DWORD dwNpcIdx );
	DWORD	GetNpcScriptPage( DWORD dwNpcIdx );
	DWORD	GetNpcMarkType( DWORD dwNpcIdx );
	int		IsQuestState( DWORD dwNpcIdx );
	
	// quest start item
	BOOL	CanItemUse( DWORD dwQuestIdx );

	void	RegistCheckTime( BYTE bCheckType, DWORD dwCheckTime );
	void	UnRegistCheckTime();
};

#endif // !defined(AFX_QUEST_H__60770757_F904_4A60_A796_6E1092C9447D__INCLUDED_)
