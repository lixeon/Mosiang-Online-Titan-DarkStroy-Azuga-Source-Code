// QuestManager.h: interface for the CQuestManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTMANAGER_H__AC4F30D2_DA87_4A14_8DF7_C2AFCBF24758__INCLUDED_)
#define AFX_QUESTMANAGER_H__AC4F30D2_DA87_4A14_8DF7_C2AFCBF24758__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtrList.h"
#include "..\[CC]Quest\QuestDefines.h"
#include "..\[CC]Quest\QuestEvent.h"
#include "QuestItemManager.h"
#include "NewbieGuide.h"


class CQuestInfo;
class CSubQuestInfo;
class CQuestString;
class CQuest;
class CQuestUpdater;
class CNewbieGuide;
class CNpc;

#define QUESTMGR	USINGTON(CQuestManager)

class CQuestManager  
{
protected:
	CYHHashTable<CQuestInfo>	m_QuestInfoTable;	// script information
	CYHHashTable<CQuestString>	m_QuestStringTable;
	CYHHashTable<CQuest>		m_QuestTable;		// quest information
	CYHHashTable<QUESTNPCINFO>	m_QuestNpcTable;	// quest npc
	
	CQuestUpdater*				m_QuestUpdater;
	CQuestItemManager			m_QuestItemMGR;

	CNpc*						m_pNpcArray[32];	// npc information
	DWORD						m_dwMaxNpc;

	CNewbieGuide				m_NewbieGuide;		// newbieguide
	
	DWORD						m_dwLastMsg;
	
public:
	CQuestManager();
	virtual ~CQuestManager();

	void	Release();
	// script
	CQuestInfo*		GetQuestInfo( DWORD dwQuestIdx )	{ return m_QuestInfoTable.GetData( dwQuestIdx ); }
	CQuestString*	GetQuestString( DWORD dwQuestIdx )	{ return m_QuestStringTable.GetData( dwQuestIdx );	}
	CQuest*			GetQuest( DWORD dwQuestIdx )		{ return m_QuestTable.GetData( dwQuestIdx ); }
	CYHHashTable<CQuest>*	GetQuestTable()				{ return &m_QuestTable; }
	char*			GetQuestTitle( DWORD dwQuestIdx );
	BOOL	LoadQuestScript( char* strFileName );
	BOOL	LoadQuestString( char* strFileName );
	void	LoadNewbieGuide( char* strFileName );
	void	LoadQuestNpc( char* strFileName );
	void	CreateAllQuestFromScript();				// script 

	// quest information clear
	void	InitializeQuest();

	// quest set
	void	SetMainQuestData( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime );
	void	SetSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwData, DWORD dwTime );	

	void	DeleteQuest( DWORD dwQuestIdx, BOOL bCheckTime = FALSE );
	void	StartQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime );
	void	EndQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime );
	void	StartSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime );
	void	EndSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime );
	void	UpdateSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwSubData, DWORD dwSubTime );

	void	QuestItemAdd( DWORD dwItemIdx, DWORD dwCount );
	void	QuestItemDelete( DWORD dwItemIdx );
	void	QuestItemUpdate( DWORD dwType, DWORD dwItemIdx, DWORD dwCount );
	
	// NpcScript
	BOOL	IsNpcRelationQuest( DWORD dwNpcIdx );
	DWORD	GetNpcScriptPage( DWORD dwNpcIdx );
	void	SendNpcTalkEvent( DWORD dwNpcIdx, DWORD dwNpcScriptIdx );
	void	SendNpcTalkEventForEvent( DWORD dwNpcIdx, DWORD dwNpcScriptIdx );
	// NpcMark
	void	SetNpcData( CNpc* pNpc );
	void	ProcessNpcMark();
	// newbieguide
	void	CheckGameEvent( GAMEEVENT* pGE )	{ m_NewbieGuide.CheckEvent( pGE ); }
	// quest npc
	void	AddQuestNpc( CSubQuestInfo* pSubQuestInfo );
	
	BOOL	IsQuestStarted( DWORD dwQuestIdx );	

	BOOL IsQuestComplete(DWORD dwQuestIdx);

	// Network
	void	NetworkMsgParse( BYTE Protocol, void* pMsg );

	//test
	void	AddStringTest();
	void	ViewToggle();
	void	AddJournalTest();	
	//
	void	QuestAbandon();
	void	QuestAbandon( DWORD dwQuestIdx );
	void	QuestAbandonClose();		

	void	QuestTimeLimit( DWORD dwQuestIdx, DWORD dwTime );
	void	RegistCheckTime( DWORD dwQuestIdx, BYTE bCheckType, DWORD dwCheckTime );
	void	UnRegistCheckTime( DWORD dwQuestIdx );

	void	QuestEventAck( DWORD dwQuestIdx );
	void	QuestEventNack( DWORD dwQuestIdx, DWORD dwError );
};

EXTERNGLOBALTON(CQuestManager);

#endif // !defined(AFX_QUESTMANAGER_H__AC4F30D2_DA87_4A14_8DF7_C2AFCBF24758__INCLUDED_)
