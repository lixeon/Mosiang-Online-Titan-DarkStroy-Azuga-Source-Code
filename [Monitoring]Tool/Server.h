// Server.h: interface for the CServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVER_H__9F52F3D2_B7F6_4AA1_9575_EE98885A3E57__INCLUDED_)
#define AFX_SERVER_H__9F52F3D2_B7F6_4AA1_9575_EE98885A3E57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CServer  
{
public:
	int		m_nServerSet;	// ÀÚ½ÅÀÌ ¼ÓÇÑ ¼­¹ö¼Â ¹øÈ£
	int		m_nServerKind;
	int		m_nState;		// None = 0, Off = 1, On = 2, AbNormal = 3,
	char	m_sState[16];
	int		m_nServerNum;
	char	m_sIP[16];	
	int		m_nPort;
	char	m_sName[64];
	DWORD	m_dwUserCount;	
	BOOL	m_bDisconnectUser;

	int		m_nAutoPatchState;
	char	m_sAutoPatchState[16];
	char	m_sAutoPatchVersion[13];

	int		m_nChannelUserCount[10];
	int		m_nMaxChannel;
	
public:
	CServer(int ServerSetNum,SERVERINFO* pInfo);
	virtual ~CServer();

	void	Release();
	int		SetState( int state );
	void	SetAutoPatchState( int nState, char* sVersion );
	void	SetAutoPatchVersion( char* sVersion );
	void	SetChannelUserCount( MSG_CHANNEL_INFO* pChannelInfo );

public:	// operate func
	void	DoServerOn();
	void	DoServerOff();
	void	DoKillServer();
};

#endif // !defined(AFX_SERVER_H__9F52F3D2_B7F6_4AA1_9575_EE98885A3E57__INCLUDED_)
