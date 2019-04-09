// EventMapMgr.h: interface for the CEventMapMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTMAPMGR_H__D6936E6B_3242_4790_846D_BE1E0D310350__INCLUDED_)
#define AFX_EVENTMAPMGR_H__D6936E6B_3242_4790_846D_BE1E0D310350__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	EVENTMAPNUM	58

enum eEVENTMAPSTATE
{
	eEventState_None = 0,
	eEventState_Ready,
	eEventState_Start,
	eEventState_End,
	eEventState_UserOut,
};

class CEventMap  
{
	CYHHashTable<CObject>	m_PlayerTable;
	int						m_nChannelNum;
	int						m_nState;
	DWORD					m_dwTime;

	DWORD					m_dwWinnerIdx;
	DWORD					m_dwWinnerPartyIdx;
	
public:
	CEventMap();
	virtual ~CEventMap();

	void	Init( int nChannelNum )		{ m_nChannelNum = nChannelNum; }
	void	Release();
	void	AddMember( CObject* pObject );
	void	RemoveMember( CObject* pObject );
	BOOL	IsEnemy( CObject* pOper, CObject* pTarget );

	void	Process();
	void	Ready();
	void	Start();	
	void	End();
	void	UserOut();

	void	Judge();
	void	JudgeFromOverTime();
};


#define EVENTMAPMGR CEventMapMgr::GetInstance()

class CEventMapMgr  
{
	CYHHashTable<CEventMap>		m_EventMapTable;
	DWORD						m_dwMapNum;

public:
	MAKESINGLETON( CEventMapMgr );

	CEventMapMgr();
	virtual ~CEventMapMgr();

	void	Init( DWORD dwMapNum, int nChannelNum );
	void	Remove();
	BOOL	IsEventMap();
	BOOL	IsEnemy( CObject* pOper, CObject* pTarget, DWORD dwChannel );
	void	AddMember( CObject* pObject, int nTeam, int nChannel );
	void	RemoveMember( CObject* pObject, int nTeam, int nChannel );
	
	void	Process();	
	void	ReadyEvent( DWORD dwChannel );
	void	StartEvent( DWORD dwChannel );
	
	virtual	void	NetworkMsgParse( BYTE protocol, MSGBASE* pmsg );
};

#endif // !defined(AFX_EVENTMAPMGR_H__D6936E6B_3242_4790_846D_BE1E0D310350__INCLUDED_)
