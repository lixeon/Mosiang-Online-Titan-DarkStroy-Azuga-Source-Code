// MNPlayer.h: interface for the MNPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNPLAYER_H__899E6F19_B06B_4820_A019_C218DCA45B3D__INCLUDED_)
#define AFX_MNPLAYER_H__899E6F19_B06B_4820_A019_C218DCA45B3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMNPlayer
{
protected:

//Team ( +observer )
//Player info from DB?MAPSERVER?
//Position( front, channel, room, game )
//
	DWORD		m_UnigueIDinAgent;
	DWORD		m_AgentNum;
	DWORD		m_dwBackMapNum;

	BASEOBJECT_INFO			m_BaseObjectInfo;
	CHARACTER_TOTALINFO		m_CharacterInfo;
	HERO_TOTALINFO			m_HeroInfo;

	PLAYER_LOCATION		m_Location;

	BOOL	m_bCaptain;

public:
	CMNPlayer();
	virtual ~CMNPlayer();

	BOOL Init( DWORD dwPlayerID, DWORD dwAgentNum, DWORD UniqueIDinAgent, DWORD dwBackMapNum );
	void InitPlayerInfo(BASEOBJECT_INFO* pObjinfo, CHARACTER_TOTALINFO* pTotalinfo, HERO_TOTALINFO* pHeroinfo);


//---Network Functions
	void SendMsg( MSGBASE* pMsg, int MsgLen );

//---TeamInfo Get&Set in MurimNet
	void SetTeam( BYTE cbTeam ) { m_BaseObjectInfo.BattleTeam = cbTeam; }
	BYTE GetTeam()				{ return m_BaseObjectInfo.BattleTeam; }

//---LocationInfo Get&Set in MurimNet
	int GetLocation()			{ return m_Location.nKind; }
	DWORD GetLocationIndex()	{ return m_Location.dwIndex; }
	
	void SetLocation( int nKind ) { m_Location.nKind = nKind; }
	void SetLocationIndex( DWORD dwIndex ) { m_Location.dwIndex = dwIndex; }	
	
//---Get Player Info Functions
	DWORD GetID()			{ return m_BaseObjectInfo.dwObjectID; }
	char* GetName()			{ return m_BaseObjectInfo.ObjectName; }
	LEVELTYPE GetLevel()	{ return m_CharacterInfo.Level; }
	
//---Captain
	void SetCaptain( BOOL bCaptain )	{ m_bCaptain = bCaptain; }
	BOOL IsCaptain()					{ return m_bCaptain; }
	
	DWORD GetBackMapNum()				{ return m_dwBackMapNum; }


};

#endif // !defined(AFX_MNPLAYER_H__899E6F19_B06B_4820_A019_C218DCA45B3D__INCLUDED_)
