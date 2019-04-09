#pragma once
#include "../ImageNumber.h"

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
	eSVVMD_TIME_STARTRENDER = 2000,
};

class CSurvivalModeManager
{
	CImageNumber	m_ImageNumber;
	cImage			m_ImageStart;

	DWORD			m_dwSVVModeStartTime;
	DWORD			m_dwElapsedTime;

	WORD	m_wModeState;
	DWORD	m_dwStateRemainTime;

	int		m_nUserTotalCount;
	int		m_nUserAlive;

	DWORD			m_dwUsingCountLimit;
	DWORD			m_dwItemUsingCounter;

public:
	CSurvivalModeManager(void);
	~CSurvivalModeManager(void);

	void	Init();
	void	Release();
	void	NetworkMsgParse(BYTE Protocol, void* pMsg);

	void	Process();
	void	Render();

	BOOL	CheckRemainTime();
	void	ChangeStateTo(WORD nextState);

	//SW061129 홍콩추가요청작업 - 사용갯수제한
	void	SetUsingCountLimit(DWORD limit) {	m_dwUsingCountLimit = limit;	}
	BOOL	AddItemUsingCount();	//제한 카운트 안넘었으면 TRUE 반환

	int		AddAliveUserCount(BOOL bPlus);

	void	SetCurModeState(WORD state);
	WORD	GetCurModeState()	{	return m_wModeState;	}
};

EXTERNGLOBALTON(CSurvivalModeManager);
