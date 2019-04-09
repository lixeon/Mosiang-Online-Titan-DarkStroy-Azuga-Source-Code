// EventMapInfo.h: interface for the CEventMapInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTMAPINFO_H__1806602E_18AD_42C0_BEB2_345204C5826D__INCLUDED_)
#define AFX_EVENTMAPINFO_H__1806602E_18AD_42C0_BEB2_345204C5826D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../ImageNumber.h"

#define	EVENTMAPNUM	58

#define EVENTMAP USINGTON(CEventMap)

class CPlayer;

enum eEVENTMAPSTATE
{
	eEventState_None = 0,
	eEventState_Ready,
	eEventState_Start,
	eEventState_End,
};

class CEventMap  
{
	CYHHashTable<CPlayer>	m_PlayerTable;
	int						m_nState;
	DWORD					m_dwMapNum;
	DWORD					m_dwTime;
	MSG_DWORD3				m_Result;
	
	DWORD	m_dwChannelNum;
	DWORD	m_dwTeam;

public:
	DWORD					m_dwBeforeMapNum;

public:
	CEventMap();
	virtual ~CEventMap();

	void	Init();
	void	Release();
	void	AddHero();
	void	AddPlayer( CPlayer* pPlayer );
	BOOL	IsEnemy( CObject* pTarget );

	void	Ready();
	void	Start();
	void	End( MSG_DWORD3* pMsg );

	void	SetData( DWORD dwMap, DWORD dwChannel, DWORD dwTeam )
	{
		m_dwMapNum = dwMap;
		m_dwChannelNum = dwChannel;
		m_dwTeam = dwTeam;
	}
	DWORD	GetEventMapNum()	{ return m_dwMapNum; }
	DWORD	GetChannelNum()		{ return m_dwChannelNum; }
	DWORD	GetTeamNum()		{ return m_dwTeam; }	
	
	BOOL	IsEventMap();




protected:
	cImage			m_ImageStart;
	cImage			m_ImageWin;
	cImage			m_ImageLose;
	cImage			m_ImageDraw;
	cImage*			m_pCurShowImage;
	VECTOR2			m_vTitlePos;
	VECTOR2			m_vTitleScale;

public:
	void	Render();
};

EXTERNGLOBALTON(CEventMap)

#endif // !defined(AFX_EVENTMAPINFO_H__1806602E_18AD_42C0_BEB2_345204C5826D__INCLUDED_)
