// cJackpotManager.h: interface for the cJackpotManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CJACKPOTMANAGER_H__45416D80_86D8_4ED1_A99C_7939A3648E03__INCLUDED_)
#define AFX_CJACKPOTMANAGER_H__45416D80_86D8_4ED1_A99C_7939A3648E03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MONEY_PER_MON	10
#define UPDATE_TICKTIME_LENGTH	60000

#define JACKPOTMGR	USINGTON(cJackpotManager)

enum ePrizeKind{ NO_PRIZE = -1, FST_PRIZE, SND_PRIZE, THR_PRIZE, NUM_PRIZE };	//당첨 종류 //common 으로..

class cJackpotManager  
{
	DWORD	m_dwJackpotTotalMoney;	// Not Real TotalMoney //!Update when Every MapChanges
	DWORD	m_dwIncreaseMoney;		// For AddRndMoney

	//Snake_legs
	DWORD	m_dwFakeOldTickTime;
	int		m_iRndArg;
		
	bool	m_bIconActive;			// keep the states itself
	bool	m_bActive;

public:
	cJackpotManager();
	virtual ~cJackpotManager();

	void SetActive(bool A) {m_bActive=A;}
	bool IsActive() {return m_bActive;}
	void SetIconActive(bool A) {m_bIconActive=A;}
	bool IsIconActive() {return m_bIconActive;}

	void SetJPTotalMoneyTest(DWORD TempMoney) {m_dwJackpotTotalMoney = TempMoney;}

	void Process();
	void NetworkMsgParse(BYTE Protocol, void* Msg);
	DWORD GetJPTotalMoney(){return m_dwJackpotTotalMoney;}
	void AddFakeMoneyForDraw();	// Plus Money Client User Itself	//!Link to where MonsterDieMsg
	void AddRndMoneyForDraw();	// Fake as Realtime AddTotalMoney	//Process

	void Init();
	void Release();

};

EXTERNGLOBALTON(cJackpotManager)
#endif // !defined(AFX_CJACKPOTMANAGER_H__45416D80_86D8_4ED1_A99C_7939A3648E03__INCLUDED_)

/*
 *	Agent로 부터 일정시간마다 JackpotTotalMoney 갱신.
 *	당첨자 발생시 갱신.
*/
