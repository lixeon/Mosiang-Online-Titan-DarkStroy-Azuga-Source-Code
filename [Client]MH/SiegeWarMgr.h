// SiegeWarMgr.h: interface for the CSiegeWarMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIEGEWARMGR_H__C540DE3F_BB61_4E0E_A87F_279AE7747B9D__INCLUDED_)
#define AFX_SIEGEWARMGR_H__C540DE3F_BB61_4E0E_A87F_279AE7747B9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define SIEGEMGR	CSiegeWarMgr::GetInstance()

class CPlayer;
class CGuild;
class CMapObject;


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


class CSiegeWarMgr  
{	
	// ¸¶À»¸Ê°ú °ø¼º¸ÊÀÇ ÀÎµ¦½º		0-¸¶À», 1-°ø¼º¸Ê, 2...->¿µÇâ¹Þ´Â ¸Ê¹øÈ£
	DWORD			m_SiegeWarMapNum[SIEGEWAR_MAX_SIEGEMAP][SIEGEWAR_MAX_AFFECTED_MAP];	
	DWORD			m_SiegeMapCount;			// ÀüÃ¼ °ø¼º¸Ê °¹¼ö

	CASTLEGATE_BASEINFO	m_CastleGateInfoList[MAX_CASTLEGATE_NUM];
	int					m_CastleGateTotalCount;
	
public:
	MAKESINGLETON(CSiegeWarMgr);
	CSiegeWarMgr();
	virtual ~CSiegeWarMgr();

	void Init();
	DWORD GetSiegeWarMapNum();
	void AddPlayer( CPlayer* pPlayer );
	void NetworkMsgParse( BYTE Protocol,void* pMsg );

	void LoadCastleGateInfo();
//	void CreateCastleGate( SEND_GATE_TOTALINFO* pGateInfo );
	void CreateCastleGate( CASTLEGATE_SENDINFO* pGateInfo );
	CMapObject* AddCastleGate( BASEOBJECT_INFO* pObjectInfo, BASEMOVE_INFO* pMoveInfo, CASTLEGATE_BASEINFO* pMapobjInfo );
	void DeleteCastleGate( DWORD GateID );
};







#define SWPROFIT USINGTON(CSiegeWarProfit)

class CSiegeWarProfit
{
protected:
	DWORD		m_dwGuildIdx;
	char		m_sGuildName[MAX_GUILD_NAME+1];
	int			m_nTaxRate;
	DWORD		m_dwMoney;

public:
	CSiegeWarProfit();
	virtual ~CSiegeWarProfit();

	void	Init();
	DWORD	CalTexRateForSell( DWORD dwMoney );
	DWORD	CalTexRateForBuy( DWORD dwMoney );
	void	NetworkMsgParse( BYTE Protocol,void* pMsg );

	void	SendProfitInfoSyn();
	void	SendChangTexrateSyn( int nTaxRate );
	void	SendGetMoneySyn( DWORD dwMoney );
	BOOL	CheckSWProfit( int nType, int nTaxRate, DWORD dwMoney );
};

EXTERNGLOBALTON(CSiegeWarProfit)

#endif // !defined(AFX_SIEGEWARMGR_H__C540DE3F_BB61_4E0E_A87F_279AE7747B9D__INCLUDED_)
