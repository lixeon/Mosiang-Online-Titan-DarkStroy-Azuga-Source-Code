#pragma once

#include "FortWarWareSlot.h"


#pragma pack(push,1)

enum eFortWarState
{
	eFortWarState_None = 0,
	eFortWarState_Before10Min,
	eFortWarState_Ing,
};

enum eFortWarDay
{
	eFortWarDay_Sun = 0,
	eFortWarDay_Mon,
	eFortWarDay_Tue,
	eFortWarDay_Wed,
	eFortWarDay_Thur,
	eFortWarDay_Fri,
	eFortWarDay_Sat,
	eFortWarDay_Max,
};

struct sFortWarTimeInfo
{
	BOOL		bOn;
	SYSTEMTIME	FortWarStartTime;
	DWORD		dwWarTime;
};

struct sFortWarInfo
{
	int		nRelationFortWarIDX;
	DWORD	dwCharacterIdx;
	DWORD	dwGuildIdx;
	char	CharacterName[MAX_NAME_LENGTH+1];
	DWORD	dwProfitMoney;
};

#pragma pack(pop)

#define FORTWARMGR	USINGTON(CFortWarManager)

class CPlayer;
class CMonster;

class CFortWarManager
{
protected:
	BOOL			m_bLoadDataFromDB;					// DB에서 정보 모두 받았는지?
	BOOL			m_bFortWarMap;						// 요새전 맵인지
	DWORD			m_dwChannelNum;						// 요새전 진행 채널 번호
	DWORD			m_dwWarState;						// 요새전 상태
	DWORD			m_dwWarTime;						
    sFortWarTimeInfo	m_FortWarTimeInfo[eFortWarDay_Max];
	STATIC_NPCINFO*	m_pNpcInfo;							// 각인npc

	CPlayer*		m_pEngravePlayer;					// 각인중인 캐릭터
	DWORD			m_dwEngraveTime;					// 각인 시간(60초)

//	char			m_EngraveSuccessPlayerName[MAX_NAME_LENGTH+1];	// 각인에 성공한 플레이어 이름

	CYHHashTable<sFortWarInfo>	m_FortWarInfoTable;			// 모든 요새전 정보
	WORD						m_wRelationFortWarMapNum;	// 관계된 요새맵번호
	int							m_nRelationFortWarIDX;		// 관계된 요새 인덱스(2100000001,2100000002,2100000003)

	// 공성창고는 모든맵이 가지고 있으나 요새창고는 관련된 맵만 가진다...
	CFortWarWareSlot	m_FortWarWareSlot01;				// 관계된 요새맵의 창고01
	CFortWarWareSlot	m_FortWarWareSlot02;				// 관계된 요새맵의 창고02
	CFortWarWareSlot	m_FortWarWareSlot03;				// 관계된 요새맵의 창고03
	CFortWarWareSlot	m_SiegeWarWareSlot;					// 공성창고
	int					m_nRelationSiegeWarIDX;				// 공성 인덱스(2100000000)
	BOOL				m_bSiegeWarWareUsableMap;			// 공성창고 이용할 수 있는 맵인지?

	CYHHashTable<CPlayer>	m_FortWarWarePlayerTable01;		// 요새창고보고있는 캐릭터정보
	CYHHashTable<CPlayer>	m_FortWarWarePlayerTable02;		// 요새창고보고있는 캐릭터정보
	CYHHashTable<CPlayer>	m_FortWarWarePlayerTable03;		// 요새창고보고있는 캐릭터정보
	CYHHashTable<CPlayer>	m_SiegeWarWarePlayerTable;		// 공성창고보고있는 캐릭터정보

	DWORD		m_dwFortWarProfitMoney;				// 요새창고 돈
	int			m_nFortWarTexRate;					// 사냥세(1/10000)
	int			m_nFortWarWareRate;					// 창고이용시(1/10000)
	DWORD		m_dwUpdateTime;

protected:
    int GetSecondInterval( SYSTEMTIME curTime, SYSTEMTIME basicTime );			// 리턴값이 양수이면 이전, 음수이면 이후...

public:
	CFortWarManager(void);
	~CFortWarManager(void);

	void Init();
	void Release();
	
	// 요새전 관련
	void ProcessFortWar();
	BOOL LoadFortWarInfo();
	void AddPlayer( CPlayer* pPlayer );
	void RemovePlayer( CPlayer* pPlayer );	
	void DiePlayer( CPlayer* pPlayer );
	void SetTotalFortWarInfoFromDB( sFortWarInfo info );
	void UpdateFortWarInfo( int nWarIDX, DWORD dwCharacterIdx, char* pCharacterName, DWORD dwGuildIdx );
	void CompleteLoadDataFromDB()	{ m_bLoadDataFromDB = TRUE; }
	DWORD GetMasterID();
	char* GetMasterName();
	DWORD GetMasterGuildID();

	// 이득관련
	void ProcessDBUpdateProfitMoney();
	DWORD AddProfitMoneyFromItemSell( DWORD dwMoney );
	DWORD AddProfitMoneyFromItemBuy( DWORD dwMoney );
	DWORD AddProfitMoneyFromMonster( DWORD dwMoney );
	void SetFortWarProfitMoneyFromDB( int nWarIDX, DWORD dwMoney );

	// 창고관련
	void RemoveWarePlayerTable( CPlayer* pPlayer );
	void TotalFortWarItemLoad();
	void SetTotalFortWarItemInfoFromDB( int nWarIDX, ITEMBASE* pItem );
	BOOL AddProfitItemFromMonster( CPlayer* pPlayer, DWORD dwItemIdx, WORD wItemNum );
	void AddProfitItemFromDBResult( int nWarIDX, ITEMBASE* pItem );
	CFortWarWareSlot* GetFortWarWareSlot( int nWarIDX );
	void SendItemInfoToFortWarePlayer( int nWarIDX, MSGBASE* pMsg, int nSize );
	void SendItemInfoToFortWarePlayerExceptOne( int nWarIDX, DWORD dwPlayerID, MSGBASE* pMsg, int nSize );
	void SendItemInfoToSeigeWarePlayer( MSGBASE* pMsg, int nSize );
	void SendItemInfoToSeigeWarePlayerExceptOne( DWORD dwPlayerID, MSGBASE* pMsg, int nSize );
	void SetSiegeWarWareUsable( BOOL bUse )		{ m_bSiegeWarWareUsableMap = bUse; }

	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );

protected:
	// msg 처리함수들...
	void Msg_MP_FORTWAR_ENGRAVE_START_SYN( void* pMsg );
	void Msg_MP_FORTWAR_ENGRAVE_CANCEL_SYN( void* pMsg );

    void Msg_MP_FORTWAR_WAREHOUSE_INFO_SYN( void* pMsg );
	void Msg_MP_FORTWAR_SIEGEWAREHOUSE_INFO_SYN( void* pMsg );
	void Msg_MP_FORTWAR_WAREHOUSE_LEAVE( void* pMsg );
	void Msg_MP_FORTWAR_WAREHOUSE_ITEM_MOVE_SYN( void* pMsg );
	void Msg_MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_SYN( void* pMsg );

	void Msg_MP_FORTWAR_WAREHOUSE_ITEM_INSERT_TO_MAP( void* pMsg );
	void Msg_MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_INVEN_TO_MAP( void* pMsg );
	void Msg_MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE_TO_MAP( void* pMsg );

	void Msg_MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_SYN( void* pMsg );
	void Msg_MP_FORTWAR_WAREHOUSE_MONEY_TO_MAP( void* pMsg );
	void Msg_MP_FORTWAR_SIEGEWAREHOUSE_PUTOUT_MONEY_SYN( void* pMsg );
};

EXTERNGLOBALTON(CFortWarManager)
