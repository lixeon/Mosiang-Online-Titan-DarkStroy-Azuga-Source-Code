// StateMng.h: interface for the CStateMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATEMNG_H__286116C1_7DCD_42CD_9156_E219160C0B7B__INCLUDED_)
#define AFX_STATEMNG_H__286116C1_7DCD_42CD_9156_E219160C0B7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ServerConnect.h"
#include "GameEnd.h"
#include "MainTitle.h"
#include "CharSelect.h"
#include "CharMake.h"
#include "GameLoading.h"
#include "GameIn.h"
#include "MapChange.h"

//#define		_LOG_FILE_

enum eGAMESTATE
{
	eGAMESTATE_END = 0,
	eGAMESTATE_CONNECT,
	eGAMESTATE_TITLE,
	eGAMESTATE_CHARSELECT,
	eGAMESTATE_CHARMAKE,
	eGAMESTATE_GAMELOADING,
	eGAMESTATE_GAMEIN,
	eGAMESTATE_MAPCHANGE,
	eGAMESTATE_MAX,
};

class CGameState;

class CStateMng  
{
public:
	int				m_nID;
	DWORD			m_dwChannel;
	CDummyNetwork	m_Network;
	CDummyObjMng	m_DummyObjMgr;

	CGameEnd		m_GameEnd;
	CServerConnect	m_ServerConnect;
	CMainTitle		m_MainTitle;
	CCharSelect		m_CharSelect;
	CCharMake		m_CharMake;
	CGameLoading	m_GameLoading;
	CGameIn			m_GameIn;
	CMapChange		m_MapChange;

protected:
	CGameState*		m_GameState[eGAMESTATE_MAX];
	int				m_nCurState;
	
public:
	IPPORT			m_AgentIpPort;
	BOOL			m_bStart;
	BOOL			m_bEnterGame;
	DWORD			m_dwUserIDx;

	BOOL			m_bEnterAgent;
	DWORD			m_dwLoginTime;
	
public:
	CStateMng();
	virtual ~CStateMng();

	BOOL	Init( int id, int channel );
	void	Exit();
	int		Process();
	void	NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg );

public:
	void	ChangeState( int state, void* pParam, int len );
	int		GetCurState()						{ return m_nCurState; }

	// dummy
	void	EnableStart( BOOL bStart )			{ m_bStart = bStart; }
	void	EnableMapChange( BOOL bEnable )		{ m_DummyObjMgr.m_bEnableMapChange = bEnable; }
	void	EnableAttack( BOOL bEnable )		{ m_DummyObjMgr.m_bEnableAttack = bEnable; }
	void	ForcedMapChange( int mapnum );
	void	EnterGame()							{ m_bEnterGame = TRUE; }
	void	SaveLoginPoint();
	void	EnableChatting( BOOL bChat )		{ m_DummyObjMgr.m_bEnableChat = bChat; }

};

void LOGDUMMY( char* temp, ... );

#endif // !defined(AFX_STATEMNG_H__286116C1_7DCD_42CD_9156_E219160C0B7B__INCLUDED_)
