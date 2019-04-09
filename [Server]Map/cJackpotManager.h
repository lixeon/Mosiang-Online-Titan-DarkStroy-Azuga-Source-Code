// cJackpotManager.h: interface for the cJackpotManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CJACKPOTMANAGER_H__8F994787_2618_43B5_B144_32070D18B413__INCLUDED_)
#define AFX_CJACKPOTMANAGER_H__8F994787_2618_43B5_B144_32070D18B413__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_PROBABILITY_LIMIT	100000000 //확률최대범위 1억

#define JACKPOTMGR	USINGTON(cJackpotManager)

enum ePrizeKind{ NO_PRIZE = -1, FST_PRIZE, SND_PRIZE, THR_PRIZE, FTH_PRIZE, NUM_PRIZE };	//당첨 종류
enum {EXCP_MAP_COUNT = 8};

struct stPrize
{
	stPrize():m_dwProbability(0),m_dwPercent(0),AbsMoney(0){};
	DWORD	m_dwProbability;	//정수화된 확률
	DWORD	m_dwPercent;	//상금 퍼센트 ((%) == m_dwPercent /100)
	DWORD	AbsMoney;
};

class cJackpotManager  
{
	DWORD	m_dwTotalJPMoney;	//서버 총적립금 Fr_DB
	DWORD	m_dwMapJPMoney;	//맵 누적금
	DWORD	m_dwPrizeMoney;	//당첨 금액(Fr_DB)
	DWORD	m_dwPercentage;

	CPlayer*	m_pPlayer;
	CMonster*	m_pMonster;

	int		m_nPrizeKind;	//당첨 종류(NO_PRIZE = -1)
	
	DWORD	m_dwChipPerMon;	//몹당 적립금(Fr_Script)
	DWORD	m_dwUpDateTimeLen;	//적립 금액 Update 시간 간격(Fr_Script)
	DWORD	m_dwLastDBUpdateTime;	//이전 DBUpdate 시각
	stPrize	m_stPrize[NUM_PRIZE];	//당첨별 정보(Fr_Script)
	
//	bool	m_bActive;		//JPMgr active
	BOOL	m_bDoPrize;		//JPMgr active
	BOOL	m_bIsAppMon;	//적용 몹인지
	BOOL	m_bIsAppMap;	//적용 맵인지

public:
	cJackpotManager();
	virtual ~cJackpotManager();

//	void	SetActive(bool A)	{m_bActive = A;}	// No useful yet
	BOOL	IsAppMap(WORD MapNum);
	BOOL	IsAppMon(CPlayer* pPlayer, CMonster* pMonster);
	BOOL	IsUserIn();
	BOOL	GetIsAppMap() {return m_bIsAppMap;}
	void	SetTotalMoney(DWORD TotalMoney) {m_dwTotalJPMoney = TotalMoney;}
	void	SetPrizeOnOff(BOOL bOn) {m_bDoPrize = bOn;}
	void	SetChipPerMon(DWORD Chip) {m_dwChipPerMon = Chip;}
	void	SetPrizeProb(DWORD PrizeKind, DWORD Prob, DWORD Percent)
	{
		m_stPrize[PrizeKind].m_dwProbability = Prob;
		m_stPrize[PrizeKind].m_dwPercent = Percent;
	}
	void	SetDBUpdateTimeLen(DWORD TimeLength) {m_dwUpDateTimeLen = TimeLength;}

	BOOL	LoadJackpotInfo();
	void	Process();
	DWORD	SetProbToDword(DWORD* atom, DWORD* denomi);
	DWORD	MakeRndValue();
	int		CheckPrize(int kind);
	int		CheckPrize();
//	void	DoJackpotTest(CPlayer* pPlayer, CMonster* pMonster);
	void	DoJackpot(CPlayer* pPlayer, CMonster* pMonster);
	void	SendMsgDBMapMoney();
	void	SendMsgDBPrizeInfo(CPlayer* pPlayer, int PrizeKind);
//	void	SendMsgAllUserPrizeNotify();	//!임시
	void	SendMsgAllUserPrizeNotify(DWORD TotalMoney, DWORD PrizeMoney, DWORD PlayerID, DWORD PrizeKind );
//	void	SendMsgMapUserTotalMoney();	//!임시
//	void	SendMsgMapUserTotalMoney(DWORD TotalMoney);
//	void	SendMsgAddUserTotalMoney(CObject* pObject);
//	void	SendMsgPrizeEffect(DWORD PlayerID);
	void	SetPrizeInfo(DWORD TotalMoney, DWORD PrizeMoney, DWORD PlayerID, DWORD PrizeKind);
	DWORD	CalcAddProb(int kind);

	void	Init(WORD Mapnum);
	void	Release();
};

EXTERNGLOBALTON(cJackpotManager)

#endif // !defined(AFX_CJACKPOTMANAGER_H__8F994787_2618_43B5_B144_32070D18B413__INCLUDED_)

/*
 *	몹이 죽을 시 잭팟 조건에 맞을 경우 잭팟 머니를 누적하며
 *	시간 단위로 DB로 업데이트를 하며 총금액을 리턴 받는다
 *	리턴 받은 총금액은 AG통해 CL로 전달 된다
 *	잭팟 머니 누적후 플레이어와 몹의 상태를 통해 잭팟여부를 결정하며
 *	당첨시 당첨 종류 MSG를 DB로 보내고 당첨금액수를 리턴 받는다
 *	AG를 통해 CL로 전달된다
 *	평상시 시간단위 업데이트/ 리턴
 *	당첨시 업데이트/ 리턴
 */

/*
 *	초기화		
 *		적용 맵인지 확인	IsAppMap(WORD MapNum)
 *		잭팟 초기 설정 로딩	LoadJackpotInfo()
 *
 *
 *	두가지 루틴이 있다.
 *	1.잭팟
 *	2.DB UPDATE
 *
 *	1.MONSTER:OBJECT :: DoDie() 에서 시작
 *		DoJackpot()
 *		맵과 몹의 조건이 맞는지 확인하고	GetIsAppMap()/	IsAppMon()
 *		맵에 적립금을 쌓고
 *		당첨여부를 확인한다	CheckPrize()
 *			랜덤값과 당첨확률비교			MakeRndValue()/	CalcAddProb(int kind)
 *			당첨일 경우 해당 배당률과 당첨 정보를 DB로 Query	G_ JackpotPrizeInfo( MONEYTYPE MapMoney, DWORD PlayerID, DWORD PrizeKind, DWORD PrizePercentage)
 *				받아온 정보를	G_ RJackpotPrizeInfo( LPQUERY pData, LPDBMESSAGE pMessage )
 *				맵에 세팅하고	SetPrizeInfo(DWORD TotalMoney, DWORD PrizeMoney, DWORD PlayerID, DWORD PrizeKind)
 *				전체 공지와 이펙트 렌더 메시지를 보낸다	SendMsgAllUserPrizeNotify(DWORD TotalMoney, DWORD PrizeMoney, DWORD PlayerID, DWORD PrizeKind )/	SendMsgPrizeEffect(DWORD PlayerID)
 *		
 *	2.Process()
 *		DB 업데이트 시간과 유저 접속 여부 확인후	IsUserIn()
 *		맵 적립금을 보내고	SendMsgDBMapMoney():JackpotAddTotalMoney( MONEYTYPE MapMoney )
 *		합산한 총 금액을 받아온다	RJackpotAddTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage )
 *		맵 접속 유저들에게 보낸다	SendMsgMapUserTotalMoney(DWORD TotalMoney)
 *		
 *		
 *	ETC
 *		맵에 접속한 유저들에게 맵저장 총적립금을 보낸다		void	SendMsgAddUserTotalMoney(CObject* pObject)
 *		
 *		
 */
