// ServerSystem.h: interface for the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)
#define AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CServerSystem  
{
public:
	BOOL	m_bEnableStart;
	BOOL	m_bEnableMapChange;
	BOOL	m_bEnableAttack;
	BOOL	m_bEnableChat;

	BOOL	m_bEnterAgent;
	
public:
	CServerSystem();
	virtual ~CServerSystem();

	BOOL	Start( int sId, int eId, int channel );
	void	Process();	
	void	End();		

};

extern CServerSystem * g_pServerSystem;
void OnConnectToServerSuccess(DWORD dwIndex, void* pVoid);
void OnConnectToServerFail(void* pVoid);

void ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void OnAcceptServer(DWORD dwConnectionIndex);
void OnDisconnectServer(DWORD dwConnectionIndex);
void OnAcceptUser(DWORD dwConnectionIndex);
void OnDisconnectUser(DWORD dwConnectionIndex);

void GameProcess();

void ButtonProc0();
void ButtonProc1();
void ButtonProc2();
void ButtonProc3();
void ButtonProc4();
void ButtonProc5();
void ButtonProc6();
void ButtonProc7();
void ButtonProc8();
void ButtonProc9();
void OnCommand(char* szCommand);


#endif // !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)
