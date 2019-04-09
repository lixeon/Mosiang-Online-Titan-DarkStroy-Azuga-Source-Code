// PartyWarMgr.h: interface for the CPartyWarMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYWARMGR_H__6C8B06D8_B89F_45E1_A1F0_AC9C09455ABA__INCLUDED_)
#define AFX_PARTYWARMGR_H__6C8B06D8_B89F_45E1_A1F0_AC9C09455ABA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlayer;

struct sPWMember
{
	DWORD	dwMemberIdx;
	BOOL	bEnableWar;
};

class CPartyWarTeam
{
protected:
	DWORD			m_dwPartyIdx;
	sPWMember		m_Member[MAX_PARTY_LISTNUM];
	char			m_sMasterName[MAX_NAME_LENGTH+1];
	int				m_nAliveNum;

	BOOL			m_bLock;
	BOOL			m_bReady;
	VECTOR3			m_vWarPos;

public:
	CPartyWarTeam();
	virtual ~CPartyWarTeam();

	void	SetPartyIdx( DWORD dwPartyIdx )		{ m_dwPartyIdx = dwPartyIdx; }
	DWORD	GetPartyIdx()						{ return m_dwPartyIdx; }
	BOOL	IsAlive()							{ return m_nAliveNum == 0 ? FALSE: TRUE; }
	void	SetLock( BOOL bLock )				{ m_bLock = bLock; }
	BOOL	IsLock()							{ return m_bLock; }
	void	SetReady( BOOL bReady )				{ m_bReady = bReady; }
	BOOL	IsReady()							{ return m_bReady; }
	char*	GetMasterName()						{ return m_sMasterName; }
	void	SetMasterName( char* pName )		{ SafeStrCpy( m_sMasterName, pName, MAX_NAME_LENGTH+1 ); }
	VECTOR3*	GetWarPosition()				{ return &m_vWarPos; }
	
	BOOL	IsAddableMember( DWORD dwMemberIdx, int nIndex );
	BOOL	IsWarMember( DWORD dwMemberIdx );
	void	InitMember( DWORD dwMemberIdx, int nIndex );	
	void	AddMember( DWORD dwMemberIdx, int nIndex );
	void	RemoveMember( DWORD dwMemberIdx, int nIndex );
	BOOL	MemberDie( DWORD dwMemberIdx );	
	
	void	SendMsgToAllMember( MSGBASE* pMsg, int nLength );
	void	SendMsgToAllMemberInWar( MSGBASE* pMsg, int nLength );
	void	SendMsgToMaster( MSGBASE* pMsg, int nLength );
	void	SendMsgRevive();

	void	SendMsgToGridInWar( MSGBASE* pMsg, int nLength );
	void	SendMsgToGridInOne( MSGBASE* pMsg, int nLength );
};

enum ePartyWarState
{
	ePartyWar_Null,
	ePartyWar_PreWait,
	ePartyWar_Wait,
	ePartyWar_Ready,
	ePartyWar_Fight,
	ePartyWar_Result,
	ePartyWar_End,
};

class CPartyWar 
{
protected:
	DWORD			m_dwIdx;
	CPartyWarTeam	m_Team1;
	CPartyWarTeam	m_Team2;		
	int				m_nState;
	DWORD			m_dwWarTime;
	DWORD			m_dwWinner;

public:
	CPartyWar();
	virtual ~CPartyWar();

	DWORD	GetIndex()		{ return m_dwIdx; }
	int		GetState()		{ return m_nState; }
	void	GetPartyIdx( DWORD* pTeam1, DWORD* pTeam2 )
	{
		*pTeam1 = m_Team1.GetPartyIdx();
		*pTeam2 = m_Team2.GetPartyIdx();
	}

	void	Init( DWORD dwPartyIdx1, DWORD dwPartyIdx2, DWORD dwIdx );
	int		IsMemberInPartyWar( DWORD dwPlayerIdx, DWORD dwPartyIdx );
	BOOL	IsEnemy( CPlayer* pTarget, DWORD dwTargetPartyIdx );
	BOOL	PlayerDie( DWORD dwPlayerIdx, DWORD dwPartyIdx );
	void	RemovePlayer( DWORD dwPlayerIdx, DWORD dwPartyIdx );

	void	Process();
	void	ProcessPreWait();
	void	ProcessWait();
	void	ProcessReady();
	void	ProcessFight();
	void	ProcessResult();
	void	ProcessEnd();

	void	PartyWarSuggestAccept( DWORD dwChannel );
	void	PartyWarSuggestDeny( DWORD dwPartyIdx );
	void	PartyWarAddMember( DWORD dwPartyIdx, DWORD dwMemberIdx, int nIndex );
	void	PartyWarRemoveMember( DWORD dwPartyIdx, DWORD dwMemberIdx, int nIndex );
	void	PartyWarLock( DWORD dwPartyIdx );
	void	PartyWarUnLock( DWORD dwPartyIdx );
	void	PartyWarStart( DWORD dwPartyIdx );
	void	PartyWarCancel( DWORD dwPartyIdx );
};


#define PARTYWARMGR CPartyWarMgr::GetInstance()

class CPartyWarMgr  
{
protected:
	CYHHashTable<DWORD>		m_PartyWarIdxTable;
	CYHHashTable<CPartyWar>	m_PartyWarTable;
	DWORD					m_dwPartyWarTableIdx;
	
public:
	MAKESINGLETON( CPartyWarMgr );

	CPartyWarMgr();
	virtual ~CPartyWarMgr();

	void	Init();
	void	Release();

	int		IsMemberInPartyWar( CPlayer* pPlayer );
	BOOL	IsEnemy( CPlayer* pOper, CPlayer* pTarget );
	BOOL	PlayerDie( CPlayer* pPlayer, CPlayer* pAttacker);
	void	RemovePlayer( CPlayer* pPlayer );
	void	RegistPartyWar( DWORD dwPartyIdx1, DWORD dwPartyIdx2 );
	void	UnRegistPartyWar( CPartyWar* pPartyWar );
	void	Process();

	void	NetworkMsgParse( BYTE Protocol, void* pMsg );

	void	Msg_PartyWarSuggest( void* pMsg );	
	void	Msg_PartyWarSuggestAccept( void* pMsg );
	void	Msg_PartyWarSuggestDeny( void* pMsg );
	void	Msg_PartyWarAddMemberSyn( void* pMsg );
	void	Msg_PartyWarRemoveMemberSyn( void* pMsg );
	void	Msg_PartyWarLock( void* pMsg );
	void	Msg_PartyWarUnLock( void* pMsg );
	void	Msg_PartyWarStart( void* pMsg );
	void	Msg_PartyWarCancel( void* pMsg );

	void	SendNackMsg( CPlayer* pPlayer, DWORD dwState );

	BOOL	IsinSamePartyWar( CPlayer* pPlayer1, CPlayer* pPlayer2 );
};

#endif // !defined(AFX_PARTYWARMGR_H__6C8B06D8_B89F_45E1_A1F0_AC9C09455ABA__INCLUDED_)
