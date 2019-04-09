#pragma once

#define SVVMODEMGR USINGTON(CSurvivalModeManager)

enum eSurvivalModeState
{
	eSVVMode_None,
	eSVVMode_Ready,
	eSVVMode_Fight,
	eSVVMode_End,
};

enum eSurvivalModeStateTime
{
	eSVVMD_TIME_READY = 15000,
	eSVVMD_TIME_END = 10000,
};

class CSurvivalModeManager
{
	WORD	m_wModeState;
	DWORD	m_dwStateRemainTime;

	DWORD	m_dwUsingCountLimit;

	int		m_nUserAlive;

	CYHHashTable<CObject>	m_SVModeUserTable;	//GM 제외
	CYHHashTable<DWORD>		m_SVItemUsingCounter;	//SVModeUser의 아이템 사용횟수 제한.
	cPtrList				m_SVModeAliveUserList;	//GM 제외

public:
	CSurvivalModeManager(void);
	~CSurvivalModeManager(void);

	void	Init();
	void	Release();
	void	Process();
	void	NetworkMsgParse(DWORD dwConnectionIndex, BYTE Protocol, void* pMsg);

	void	SendMsgToAllSVModeUser(MSGBASE* pMsg, int msgsize);
	void	SendAliveUserCount();
	void	SendNackMsg(CPlayer* pGM, BYTE Protocol, BYTE errstate);

	BOOL	CheckRemainTime();
	//SW061129 홍콩추가요청작업 - 사용갯수제한
	void	SetUsingCountLimit(DWORD limit);
	BOOL	AddItemUsingCount(CPlayer* pPlayer);	//제한 카운트 안넘었으면 TRUE 반환

	void	ChangeStateTo(WORD nextState);
	//서바이벌 맵의 모든 유저를 살리고 게이지 풀.
	void	ReadyToSurvivalMode();
	void	ReturnToMap();

	void	AddSVModeUser(CObject* pObject);
	void	RemoveSVModeUser(CObject* pObject);
	//int		AddAliveUserCount(BOOL bPlus);
	void	AddAliveUser(CObject* pObject);
	void	RemoveAliveUser(CObject* pObject);

	void	SetCurModeState(WORD state);
	WORD	GetCurModeState()	{	return m_wModeState;	}
};

EXTERNGLOBALTON(CSurvivalModeManager)
