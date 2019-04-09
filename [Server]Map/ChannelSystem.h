// ChannelSystem.h: interface for the CChannelSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNELSYSTEM_H__31F42C7E_BEB4_4E59_8965_CD2E6D08B770__INCLUDED_)
#define AFX_CHANNELSYSTEM_H__31F42C7E_BEB4_4E59_8965_CD2E6D08B770__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Channel.h"

#define CHANNELSYSTEM CChannelSystem::GetInstance()

class CChannelSystem  
{
	CChannel*	m_Channel[MAX_CHANNEL_NUM];
	char		m_ChannelName[MAX_CHANNEL_NAME+1];
	DWORD		m_dwCount;
	
public:
	CChannelSystem();
	virtual ~CChannelSystem();
	GETINSTANCE(CChannelSystem);

	void	LoadChannelInfo();
	void	CreateChannel(BYTE Kind, BYTE num);
	void	IncreasePlayerNum(DWORD ChannelID);
	void	DecreasePlayerNum(DWORD ChannelID);
	void	SendChannelInfo(MSG_DWORD* pInfo, DWORD dwConnectionIndex);
	DWORD	GetChannelID(int num);
	DWORD	GetChannelCount()			{ return m_dwCount;	}
	
	// jsd
	void	SendChannelInfoToMS( DWORD dwConnectionIndex );

	//2008. 5. 8. CBH - 맵 이동 채널정보 관련 처리 함수
	void    SendMapChangeChannelInfo(MSG_DWORD3* pInfo, DWORD dwConnectionIndex);

	//SW080515 채널별 세팅
	float	GetRegenRate(DWORD ChannelID);
	float	GetDropRate(DWORD ChannelID);
	BOOL	IsBattleChannel(DWORD ChannelID);

	void	GetChannelInfo( MSG_CHANNEL_INFO* pInfo );

protected:
	CChannel	m_QMChannel[1000];
	BOOL		m_bQuestMap;

public:
	void	InitQuestMapChannel();
	DWORD	CreateQuestMapChannel();
	void	DestroyQuestMapChannel( DWORD dwChannel );
	BOOL	IsQuestMap()		{ return m_bQuestMap; }
	
	void	Init( WORD wMapNum );
	DWORD	GetRandomChannelID();
	DWORD	GetLastChannelID();
};

#endif // !defined(AFX_CHANNELSYSTEM_H__31F42C7E_BEB4_4E59_8965_CD2E6D08B770__INCLUDED_)
