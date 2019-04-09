// Channel.h: interface for the CChannel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNEL_H__918176B3_280E_483C_B0D1_7D9DE9CBC256__INCLUDED_)
#define AFX_CHANNEL_H__918176B3_280E_483C_B0D1_7D9DE9CBC256__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct CHANNEL_INFO
{
	DWORD ChannelIDX;
	WORD PlayerNum;

	float	RegenRate;
	float	DropRate;
	BOOL	bBattleChannel;
};

struct CHANNEL_PROPERTY
{
	
};

class CChannel  
{
public:
	CHANNEL_INFO m_ChannelInfo;

public:
	CChannel();
	virtual ~CChannel();

	DWORD GetChannelID()			{ return m_ChannelInfo.ChannelIDX; 	}
	void SetChannelID(DWORD id)		{ m_ChannelInfo.ChannelIDX = id;	}

	WORD GetPlayerNum()				{ return m_ChannelInfo.PlayerNum;	}

	void IncreaseNum()				{ m_ChannelInfo.PlayerNum++; }
	void DecreaseNum()				{ m_ChannelInfo.PlayerNum--; }

	//SW080515 채널별 세팅
	float	GetRegenRate()			{ return m_ChannelInfo.RegenRate;	}
	float	GetDropRate()			{ return m_ChannelInfo.DropRate;	}
	BOOL	IsBattleChannel()		{ return m_ChannelInfo.bBattleChannel;	}

};

#endif // !defined(AFX_CHANNEL_H__918176B3_280E_483C_B0D1_7D9DE9CBC256__INCLUDED_)
