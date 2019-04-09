// MSManager.h: interface for the CMSManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSMANAGER_H__DDCC1387_94BD_4E85_88E0_F3F3BB80A447__INCLUDED_)
#define AFX_MSMANAGER_H__DDCC1387_94BD_4E85_88E0_F3F3BB80A447__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MSMGR USINGTON(CMSManager)

class CMSManager  
{
	char	m_AutoPatchVersion[13];		
public:
	CMSManager();
	virtual ~CMSManager();
	BOOL ExecuteServer(WORD ServerType, WORD ServerNum);
	BOOL ServerOff(WORD Port);
	BOOL KillServer(WORD Port);
	BOOL AutoPatch();

	void ServerUnRegiest(DWORD dwConnectionIdx);
	void AddServerRegiest(DWORD dwConnectionIdx, DWORD ProcessID, SERVERINFO * pInfo);
	void SendServerState(WORD wState, MSERVERINFO* pInfo);
	void SendServerState(WORD wState, SERVERINFO* pInfo);
	void SendServerLevel( DWORD level, MSERVERINFO* pInfo);
	void SendToNotice(MSGNOTICE * pMsg, DWORD dwLen);
	void SendToChangeUserLevel( MSGROOT* pMsg, DWORD dwLen );
	void SendMSInfo();
	
	void	SendToDistribute( char* pMsg, DWORD dwLen );
	void	SendToAgent( char* pMsg, DWORD dwLen );

	
	void	ReadAutoPatchVersion();
	char*	GetAutoPatchVersion()	{ return m_AutoPatchVersion; }
};
EXTERNGLOBALTON(CMSManager)
#endif // !defined(AFX_MSMANAGER_H__DDCC1387_94BD_4E85_88E0_F3F3BB80A447__INCLUDED_)
