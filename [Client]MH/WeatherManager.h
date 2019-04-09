// WeatherManager.h: interface for the CWeatherManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEATHERMANAGER_H__5EB34DB4_AE8E_4654_B1FB_2A4A701DAFB1__INCLUDED_)
#define AFX_WEATHERMANAGER_H__5EB34DB4_AE8E_4654_B1FB_2A4A701DAFB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\[cc]header\commonstruct.h"
#include "interface\cImage.h"
#include "ENGINE\EngineEffect.h"

#define WEATHERMGR	USINGTON(CWeatherManager)

class CWeatherManager  
{
	WORD	m_wState;


	VECTOR3 m_EffectPos;
	CEngineEffect m_WeatherEffect[eWS_Max];

public:
	BOOL    m_bSnow;    // 눈 On/Off 2005.12.28일 맴버변수 추가 
	BOOL    m_bSnowFt;  // 눈 On/Off 2005.12.28일 체크 할때마다 호출 
	BOOL    m_bSnowNet; // 눈 On/Off 2005.12.28일 체크 할때마다 호출

public:

	CWeatherManager();
	virtual ~CWeatherManager();

	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	void SetWeather(BOOL bSnow); // 2005.12.30일 추가 눈 On/Off
	
	WORD GetWeatherState() { return m_wState; };

	void Init();
	void Release();
	void SetWeather(WORD MapNum, stWeatherTime &stWeather);
	void SetWeather(WORD MapNum, WORD wState);
	void ReturnSchedule(WORD MapNum);
		
	void Render();
	void Process();
};

EXTERNGLOBALTON(CWeatherManager);

#endif // !defined(AFX_WEATHERMANAGER_H__5EB34DB4_AE8E_4654_B1FB_2A4A701DAFB1__INCLUDED_)
