// QuestGroup.h: interface for the CQuestGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTGROUP_H__0A88AD15_F8B2_4FC9_AADB_26F663A4A647__INCLUDED_)
#define AFX_QUESTGROUP_H__0A88AD15_F8B2_4FC9_AADB_26F663A4A647__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\[CC]Quest\QuestDefines.h"
#include "..\[CC]Quest\QuestEvent.h"

class CQuestEvent;
class CQuestInfo;
class CQuest;
class CPlayer;

struct GQUESTEVENT
{
	DWORD			dwQuestIdx;
	CQuestEvent		QuestEvent;
	GQUESTEVENT() : dwQuestIdx(0)	{}
};

class CQuestGroup  
{
protected:
	PLAYERTYPE*				m_pPlayer;
	CYHHashTable<CQuest>	m_QuestTable;
	CYHHashTable<QUESTITEM>	m_QuestItemTable;
	GQUESTEVENT				m_QuestEvent[MAX_QUESTEVENT_PLYER];
	DWORD					m_dwQuestEventNum;

public:
	CQuestGroup();
	CQuestGroup( PLAYERTYPE* pPlayer );
	virtual ~CQuestGroup();

	void	Release();
	void	Initialize( PLAYERTYPE* pPlayer );

	// 맨처음 유저가 접속했을때
	void	CreateQuest( DWORD dwQuestIdx, CQuestInfo* pQuestInfo );
	// 유저 접속 후 DB데이터 세팅 
	BOOL	SetMainQuestData( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime );
	void	SetSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwData, DWORD dwTime );
	void	SetQuestItem( DWORD dwQuestIdx, DWORD dwItemIdx, DWORD dwItemNum );

	// event
	void	AddQuestEvent( CQuestEvent* pQuestEvent, DWORD dwQuestIdx );

	// trigger
	void	DeleteQuest( DWORD dwQuestIdx );	// giveup quest
	void	EndQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	void	StartSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	void	EndSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	void	ChangeSubQuestValue( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwKind );

	void	EndOtherQuest( DWORD dwOtherQuestIdx, DWORD dwRepeat );
	
	void	AddCount( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount );
	void	AddCountFromQWeapon( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwWeaponKind );
	void	AddCountFromWeapon( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwWeaponKind );
	void	AddCountFromLevelGap( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwMinLevel, DWORD dwMaxLevel );
	void	AddCountFromMonLevel( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwMinLevel, DWORD dwMaxLevel );
	
	void	GiveQuestItem( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwItemIdx, DWORD dwItemNum );
	void	TakeQuestItem( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx,
						   DWORD dwItemIdx, DWORD dwItemNum, DWORD dwProbability );
	void	TakeQuestItemFromQWeapon( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwItemIdx,
									  DWORD dwItemNum, DWORD dwProbability, DWORD dwWeaponKind );
	void	TakeQuestItemFromWeapon( PLAYERTYPE* pPlayer, DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwItemIdx,
									 DWORD dwItemNum, DWORD dwProbability, DWORD dwWeaponKind );
	void	GiveItem( PLAYERTYPE* pPlayer, WORD wItemIdx, DWORD dwItemNum );
	void	GiveMoney( PLAYERTYPE* pPlayer, DWORD dwMoney );
	BOOL	TakeItem( PLAYERTYPE* pPlayer, WORD wItemIdx, DWORD dwItemNum, DWORD dwProbability );
	void	TakeMoney( PLAYERTYPE* pPlayer, DWORD dwMoney );
	void	TakeExp( PLAYERTYPE* pPlayer, DWORD dwExp );
	void	TakeSExp( PLAYERTYPE* pPlayer, DWORD dwSExp );	
	void	TakeMoneyPerCount( PLAYERTYPE* pPlayer, DWORD dwItemIdx, DWORD dwMoney );
	
	void	RegenMonster( DWORD dwRegenType );
	void	MapChange( WORD wCurMap, WORD wReturnMap );

	void	ChangeStage( BYTE bStage );
	
	void	RegistCheckTime( DWORD dwQuestIdx, DWORD dwSubQuestIdx, BYTE bType, DWORD dwDay, DWORD dwHour, DWORD dwMin );
	void	SaveLoginPoint( DWORD dwLoginPointMap );

	// process
	void	Process();
	
	// getfn
	CQuest*	GetQuest( DWORD dwQuestIdx )	{ return m_QuestTable.GetData( dwQuestIdx ); }	

	int GetProcessQuestCount();

	BOOL IsQuestComplete( DWORD dwQuestIdx );

	DWORD	GetSubQuestValue( DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	
#ifdef _JAPAN_LOCAL_
	void	ChangeSubAttr( WORD dwSubAttr );
#endif
};

#endif // !defined(AFX_QUESTGROUP_H__0A88AD15_F8B2_4FC9_AADB_26F663A4A647__INCLUDED_)
