// JackpotManager_Agent.h: interface for the JackpotManager_Agent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JACKPOTMANAGER_AGENT_H__C8E3A334_5375_4B29_BE0E_A60870DE09BD__INCLUDED_)
#define AFX_JACKPOTMANAGER_AGENT_H__C8E3A334_5375_4B29_BE0E_A60870DE09BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	DB_UPDATE_TIMELENGTH	60000

#define JACKPOTMGR	USINGTON(JackpotManager_Agent)

class JackpotManager_Agent  
{
	MONEYTYPE	m_dwTotalMoney;
	DWORD		m_dwUpdateTimeLength;
	DWORD		m_dwLastDBUpdateTime;
	bool		m_bManager;
	

public:

	void	CheckManager();	//잭팟관리하는 Agent인지
	void	SetTotalMoney(MONEYTYPE TotalMoney) {m_dwTotalMoney = TotalMoney;}
	void	SetUpdateTimeLength(DWORD TimeLength) {m_dwUpdateTimeLength = TimeLength;}
	void	GetTotalMoneyFrDB();	//DB에 쿼리...생성될때..그리고 정기적으로
	void	SendMsgAgentTotalMoney();
	void	ReceiveMsgAgentTotalMoney();
	void	SendMsgUserTotalMoney();	//총금액 정보를 유저에게 보낸다. //!맵 접속 유저인지 확인
//	void	SendMsgUserPrizeNotify();	//당첨 정보를 유저에게 보낸다.	//MAP 에서 처리
	void	SendMsgAddUserTotalMoney(DWORD CharacterID);	//총금액 정보를 Distribute_Server 나온 유저에게 보낸다.

	void	Start();
	void	Process();
	void	Init();
	void	Release();

	JackpotManager_Agent();
	virtual ~JackpotManager_Agent();

};

EXTERNGLOBALTON(JackpotManager_Agent)

#endif // !defined(AFX_JACKPOTMANAGER_AGENT_H__C8E3A334_5375_4B29_BE0E_A60870DE09BD__INCLUDED_)


/*
 *	일정 시간마다 DB에서 TotalMoney를 받아와 Client 에게 보낸다.
 *	잿팟 정보를 받으면 TotalMoney를 갱신하고 Client 에게 보낸다.
 */