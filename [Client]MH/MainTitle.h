// MainTitle.h: interface for the CMainTitle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINTITLE_H__82C6A42B_0EEA_4F49_BA0C_33F88B3FFEF3__INCLUDED_)
#define AFX_MAINTITLE_H__82C6A42B_0EEA_4F49_BA0C_33F88B3FFEF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"
#include "cImageSelf.h"
#include "Monster.h"

#define TITLE USINGTON(CMainTitle)

class CEngineMap;
class CEngineCamera;
class cDialog;
class CServerListDialog;
class CIntroReplayDlg;

#define _MAX_MAINTITLE_MONSTER_NUM 3


class CMainTitle : public CGameState  
{
	CEngineCamera*	m_pCamera;

	BOOL	m_bDisconntinToDist;
	DWORD	m_DistAuthKey;
	DWORD	m_UserIdx;
		
//KES 031107
	cDialog*	m_pLogoWindow;
	DWORD		m_dwStartTime;
	BOOL		m_bInit;

#ifdef TAIWAN_LOCAL
	cDialog*	m_pAdvice;
#endif
// JSD
	CServerListDialog*	m_pServerListDlg;
	char				m_DistributeAddr[16];
	WORD				m_DistributePort;
	BOOL				m_bServerList;

//KES
	DWORD		m_dwDiconWaitTime; // for disconnect from Distribute;
	DWORD		m_dwWaitTime;	//for agent connection
	BOOL		m_bWaitConnectToAgent;
	BOOL		m_bNoDiconMsg;

	DWORD		m_ConnectionServerNo;

	// magi82 - Intro(070802)
	CIntroReplayDlg* m_pIntroReplayDlg;

public:
	CMainTitle();
	virtual ~CMainTitle();
	BOOL	Init(void* pInitParam);
	void	Release(CGameState* pNextGameState);

	void	Process();
	void	BeforeRender();
	void	AfterRender();	
	void	NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);

	DWORD	GetDistAuthKey()		{ return m_DistAuthKey;	}
	DWORD	GetUserIdx()			{ return m_UserIdx; }
	void	OnLoginError(DWORD errorcode,DWORD dwParam);
	void	OnDisconnect();
	
	// JSD
	void	ConnectToServer( int index );
	void	ShowServerList();
	void	SetServerList()		{ m_bServerList = TRUE; }

	CServerListDialog*	GetServerListDialog()	{ return m_pServerListDlg; }

//KES
	void	StartWaitConnectToAgent( BOOL bStart );
	void	NoDisconMsg()			{ m_bNoDiconMsg = TRUE; }
	
	BOOL	m_bDynamicDlg;

	cImageSelf m_imageLoginBarTop;
	cImageSelf m_imageLoginBarBottom;

	// magi82 - ServerListBackImage(070802)
	cImageSelf m_imageLoginBackGround;

#ifdef _HK_LOCAL_
	DWORD		m_dwEnc;
	DWORD		m_dwOld;
	BYTE		m_enkey[8];
#endif

	//trustpak 2005/04/01
private:	
	
#ifndef TAIWAN_LOCAL
	void	InitMainTitleMonsters(void);	
	void	RemoveMainTitleMonsters(void);
	void	ProcessMainTitleMonsters(void);

	CMonster*	m_apMonster[_MAX_MAINTITLE_MONSTER_NUM];
	VECTOR3		m_av3Target[_MAX_MAINTITLE_MONSTER_NUM];	
	VECTOR3		m_av3LastPosition[_MAX_MAINTITLE_MONSTER_NUM];
	DWORD		m_adwLastActionTime[_MAX_MAINTITLE_MONSTER_NUM];
	INT			m_aiLastAction[_MAX_MAINTITLE_MONSTER_NUM];

	RECT	m_rcMainTitleMonsterArea;

#endif


///


	

public:


};

EXTERNGLOBALTON(CMainTitle)

#endif // !defined(AFX_MAINTITLE_H__82C6A42B_0EEA_4F49_BA0C_33F88B3FFEF3__INCLUDED_)
