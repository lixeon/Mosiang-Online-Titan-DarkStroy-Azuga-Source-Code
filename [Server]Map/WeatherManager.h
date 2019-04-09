// WeatherManager.h: interface for the CWeatherManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEATHERMANAGER_H__D6E55A4F_B4CE_4D4B_856C_AFA1A1947582__INCLUDED_)
#define AFX_WEATHERMANAGER_H__D6E55A4F_B4CE_4D4B_856C_AFA1A1947582__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\[cc]header\commonstruct.h"

#define WEATHERMGR USINGTON(CWeatherManager)

class CPlayer;

class CWeatherManager  
{
	DWORD m_dwLastCheckTime;

	WORD m_wState;
	WORD m_wStateCount;
	WORD m_wEventCount;
	BOOL m_bExecution;

	CYHHashTable<stWeatherTime>		m_WeatherSchedule;
	CMemoryPoolTempl<stWeatherTime>*	m_pWeatherSchedulePool;

	CYHHashTable<stWeatherTime>		m_EventSchedule;
	CMemoryPoolTempl<stWeatherTime>*	m_pEventSchedulePool;
	
		
public:
	CWeatherManager();
	virtual ~CWeatherManager();

	void Init();
	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	void Process();

	WORD GetWeatherState() { return m_wState; };
	void SendWeather(CPlayer* pPlayer);
	void SendAllWeather();
};

EXTERNGLOBALTON(CWeatherManager);

#endif // !defined(AFX_WEATHERMANAGER_H__D6E55A4F_B4CE_4D4B_856C_AFA1A1947582__INCLUDED_)
