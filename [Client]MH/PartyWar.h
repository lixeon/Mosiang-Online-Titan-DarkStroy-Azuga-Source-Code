// PartyWar.h: interface for the CPartyWar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYWAR_H__44ED79D2_BFFF_4A6B_A039_E50BABF10448__INCLUDED_)
#define AFX_PARTYWAR_H__44ED79D2_BFFF_4A6B_A039_E50BABF10448__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../ImageNumber.h"

#define PARTYWAR USINGTON(CPartyWar)

#define WARSTAGE		2000.0f
#define WARSTAGEBORDER	2050.0f

enum ePartyWarState
{
	ePartyWar_Null,
	ePartyWar_PreWait,
	ePartyWar_Wait,
	ePartyWar_Ready,
	ePartyWar_Fight,
	ePartyWar_Result,
	ePartyWar_End,
	ePartyWar_Proc,
};

class CPlayer;

class CPartyWar  
{
protected:
	PARTY_INFO		m_EnemyPartyInfo;
	DWORD			m_MemberIdx[7];
	int				m_nWarState;
	DWORD			m_dwWarTime;

public:
	CPartyWar();
	virtual ~CPartyWar();

	void	Init();
	void	Release();

	BOOL	IsEnemy( CPlayer* pTarget );
	void	SetPartyWarMember( DWORD dwMemberIdx, int nIndex, BOOL bParty, BOOL bAdd );
	BOOL	IsPartyMember( DWORD dwMemberIdx, int nIndex );
	BOOL	IsPartyMember( int nIndex );	
	BOOL	IsEnemyPartyMember( DWORD dwMemberIdx, int nIndex );
	BOOL	IsEnemyPartyMember( int nIndex );
	BOOL	IsMaster();
	BOOL	IsPartyWar();

	void	NetworkMsgParse( BYTE Protocol, void* pMsg );

	CPlayer*	CanPartyWarSuggest();
	void	SendPartyWarSuggest();
	void	SendPartyWarSuggestReturn( BYTE Protocol );
	void	SendPartyWarMember( int nIndex );
	void	SendPartyWarLock( BOOL bLock );
	void	SendPartyWarStart();
	void	SendPartyWarCancel();	

	void	PartyWarWait( PARTY_INFO* pInfo );
	void	PartyWarReady( MSG_BATTLE_VIMU_CREATESTAGE* pMsg );
	void	PartyWarFight();
	void	PartyWarResult( MSG_DWORD* pMsg );
	void	PartyWarEnd();

protected:	// image
	CImageNumber	m_ImageNumber;
	cImage			m_ImageStart;
	cImage			m_ImageWin;
	cImage			m_ImageLose;
	cImage			m_ImageDraw;
	cImage*			m_pCurShowImage;

	VECTOR3			m_vWarPos;
	VECTOR2			m_vTitlePos;
	VECTOR2			m_vTitleScale;

public:
	void	CreateStage();
	void	DestroyStage();
	void	Render();

	// TEST
	void	TestPartyWarDialog();	
};

EXTERNGLOBALTON(CPartyWar)

#endif // !defined(AFX_PARTYWAR_H__44ED79D2_BFFF_4A6B_A039_E50BABF10448__INCLUDED_)
