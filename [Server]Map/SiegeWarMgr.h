// SiegeWarMgr.h: interface for the CSiegeWarMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIEGEWARMGR_H__0620B52A_B018_4B66_9EF9_6A9D6703763D__INCLUDED_)
#define AFX_SIEGEWARMGR_H__0620B52A_B018_4B66_9EF9_6A9D6703763D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\[CC]BattleSystem\SiegeWar\Battle_SiegeWar.h"

#define SIEGEWARMGR CSiegeWarMgr::GetInstance()

// 상징물과의 거리체크.. 꼭해주자.
enum
{
	NAKYANG_SYMBOL,
	MAX_SYMBOL
};


class CGuild;
class CPlayer;



enum eSW_Error
{
	eSWError_NoError=0,
	eSWError_Error,
	eSWError_NoGuildInfo,					// Don't find guildinfo
	eSWError_NoGuildMaster,					// Not a Guild Master
	eSWError_NoCastleGuild,					// Not a Castle Guild
	eSWError_FailedRegistSiegeWarTime,		// Failed Regist siegewar-time
	eSWError_AlreadyDefenceProposal,		// Already Defence-proposal
	eSWError_AlreadyAttackRegist,			// Registed Attack guild
	eSWError_DontDefenceProposal,			// Don't proposal to defence
	eSWError_NoProposalGuild,				// Not a proposal guild
	eSWError_MyUnionIsCastleUnion,			// Proposal Failed
	eSWError_MyUnionIsOtherTeam,			//	
	eSWError_OverGuildCount,				// Over the guild count at siegewar
	eSWError_RegistTime,					// Regist error : siegewar start-time
	eSWError_NoProposalTime,	
	eSWError_CastleGuild,					// Engrave
	eSWError_NoPerfectGuild,				// Not a Perfect Guild Level(5)
	eSWError_OtherPlayerEngrave,			// Already engrave by the other player
	eSWError_NoBaseVillage,					// Not a Guild-basetown
	eSWError_NoSiegeWarTime,				// Not a siegewar-time
	eSWError_Observer,						//
	eSWError_NoAcceptTime,
};

enum eSW_State
{
	eSWState_Before=0,
	eSWState_Proclamation,
	eSWState_Acceptance,
	eSWState_BeforeSiegeWar,
	eSWState_SiegeWar,
	eSWState_EndSiegeWar,
};

enum eSW_GuildType
{
	eSWGuildState_None=0,
	eSWGuildState_CastleGuild,				// 성문파
	eSWGuildState_DefenceGuild,				// 수성문파
	eSWGuildState_DefenceProposalGuild,		// 수성신청 문파
	eSWGuildState_AttackGuild,				// 공성신청 문파
	eSWGuildState_CastleUnionGuild,			// 성동맹길드
};

enum eSW_GateKind
{
	eSWGateKind_OuterGate1 = 1,
	eSWGateKind_OuterGate2 = 2,
	eSWGateKind_InnerGate1 = 3,
	eSWGateKind_InnerGate2 = 4,
};



class CSiegeWarMgr  
{
	CYHHashTable<CGuild>		m_DefenceProposalList;		// 수성신청 대기문파
	CYHHashTable<CGuild>		m_DefenceAcceptList;		// 수성 문파
	CYHHashTable<CGuild>		m_AttackGuildList;			// 공성측 문파
	CYHHashTable<CGuild>		m_AttackUnionList;			// 공성측 동맹 리스트 - 같은동맹이 수성,공성으로 나뉘지 않도록 하기위해
	
	CBattle_SiegeWar*			m_pBattle;	

	DWORD			m_SiegeWarIdx;
	DWORD			m_SiegeWarState;			// Process State
	DWORD			m_CastleUnionIdx;			// 성주 동맹Idx
	DWORD			m_CastleGuildIdx;			// 성주 길드Idx
	stTIME			m_SiegeWarTime[2];			// 0-수성선포일, 1-공성일

	// 마을맵과 공성맵의 인덱스		0-마을, 1-공성맵, 2...->영향받는 맵번호
	DWORD			m_SiegeWarMapNum[SIEGEWAR_MAX_SIEGEMAP][SIEGEWAR_MAX_AFFECTED_MAP];	
	DWORD			m_SiegeMapCount;			// 전체 공성맵 갯수

	DWORD			m_TaxRate;					// 세금비율
	DWORD			m_EngraveIdx;				// 각인중인 캐릭터Idx
	DWORD			m_EngraveTimer;				// 각인중 시간계산
	DWORD			m_EngraveGuildIdx;			// 각인에 성공한 GuildIdx

	DWORD			m_SiegeWarSuccessTimer;		// 공성성공후 정보갱신 타이머
	DWORD			m_SiegeWarEndTimer;			// 공성종료후 나갈때 까지 시간계산 타이머

	DWORD			m_FightTime;				// 공성남은 시간.

	stCASTLEGATELEVEL		m_GateInfo;
	cPtrList				m_CastleGateList;	// 성문
	CASTLEGATE_BASEINFO		m_CastleGateInfoList[MAX_CASTLEGATE_NUM];
	int						m_CastleGateCount;

	DWORD			m_SymbolIndex[MAX_SYMBOL];

	//SW061205 깃발 NPC
	WORD			m_wCurDay;
	DWORD			m_dwCurFlag;

public:
	MAKESINGLETON( CSiegeWarMgr );
	CSiegeWarMgr();
	virtual ~CSiegeWarMgr();

	//
	void Init();
	void LoadCastleGateInfo();
	void CreateSiegeBattle();
	void SetSiegeWarInfo( DWORD SiegeWarIdx, DWORD TaxRate, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level );
	void SetSiegeGuildInfo( SIEGEWAR_GUILDDBINFO* pGuildList, DWORD Count );
	void CreateCastleGate();
	void AddCastleGate();
	void DeleteCastleGate( DWORD GateID );
	void SetFirstSiegeWarTime();

	
	DWORD AddProposalGuildList( DWORD GuildIdx );
	DWORD AddAttackGuildList( DWORD GuildIdx );
	BOOL IsRegistTime( DWORD dwTime );
	void BreakUpGuild( DWORD GuildIdx );
	BOOL IsPossibleUnion( DWORD GuildIdx1, DWORD GuildIdx2 );
	void CreateUnionCheck( DWORD UnionIdx );
	void AddAcceptGuild( DWORD GuildIdx );
 	void DeleteAcceptGuild( DWORD GuildIdx );
 	void DestoryCastleUnion();
 	void SetCastleUnionIdx( DWORD UnionIdx )				{	m_CastleUnionIdx = UnionIdx;	}


	BOOL IsAcceptGuild( DWORD GuildIdx );
	BOOL IsAttackGuild( DWORD GuildIdx );
	BOOL IsAttackUnion( DWORD UnionIdx );
	void AddPlayer( CPlayer* pPlayer );

	void SetGateLevel( DWORD GateID, DWORD Level )			{	m_GateInfo.SetLevel( GateID, Level );	}
	DWORD GetGateLevel( DWORD GateID )						{	m_GateInfo.GetLevel( GateID );	}

	void Process();
	void StartSiegeWar();
	void EndSiegeWar();	
	void SetBattleInfo();
	void SuccessSiegeWar( DWORD GuildIdx );	
	void SendBattleInfoToVillageMap();
	int AddEngraveSyn( CPlayer* pPlayer, DWORD GuildIdx );

	//
	DWORD GetSiegeWarIdx()					{	return m_SiegeWarIdx;	}
	DWORD GetGuildTeamIdx( DWORD GuildIdx );
	DWORD IsAbleOrganizeUnion( DWORD GuildIdx1, DWORD GuildIdx2 );
	void SendMsgToSeigeGuild( MSGBASE* pMsg, DWORD dwLength );
	DWORD GetEngraveIdx()					{	return m_EngraveIdx;	}
	void CancelEngraveIdx();

	DWORD GetVillageMapNum();
	DWORD GetSiegeMapNum();
	BOOL IsNeedLoadSiegeInfo();
	BOOL IsProcessSiegewar()
	{
		if( m_SiegeWarState == eSWState_SiegeWar )	return TRUE;
		else										return FALSE;
	}
	BOOL IsInSiegeWar( DWORD GuildIdx );
	void	GetSiegeWarTime( DWORD* pTime0, DWORD* pTime1 );
	DWORD GetCastleUnionIdx()				{	return m_CastleUnionIdx;	}
 	DWORD GetCastleGuildIdx()				{	return m_CastleGuildIdx;	}

	void UserLogOut( CPlayer* pPlayer );
	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol,void* pMsg );	

	//SW061205 깃발 NPC
	void	CheckDateforFlagNPC();
	DWORD	GetSWState()	{	return m_SiegeWarState;	}
};

#endif // !defined(AFX_SIEGEWARMGR_H__0620B52A_B018_4B66_9EF9_6A9D6703763D__INCLUDED_)
