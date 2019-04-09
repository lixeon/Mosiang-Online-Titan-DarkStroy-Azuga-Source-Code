// ServerSystem.h: interface for the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSYSTEM_H__6D60EE32_C8B5_44A7_883C_8EF113DD06B4__INCLUDED_)
#define AFX_SERVERSYSTEM_H__6D60EE32_C8B5_44A7_883C_8EF113DD06B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CServerSystem  
{
protected:

	void SetNetworkParser();//---네트워크 파서 설정

public:
	CServerSystem();
	virtual ~CServerSystem();

	void Start(WORD ServerNum);
	void End();
	void Process();

//...?
	void ReleaseAuthKey(DWORD key);
};

void GameProcess();

void ProcessDBMessage();
void ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);

void ButtonProc1();
void ButtonProc2();
void ButtonProc3();
void OnCommand( char* szCommand );

void OnAcceptServer(DWORD dwConnectionIndex);
void OnDisconnectServer(DWORD dwConnectionIndex);
void OnAcceptUser(DWORD dwConnectionIndex);
void OnDisconnectUser(DWORD dwConnectionIndex);

void OnConnectServerSuccess(DWORD dwConnectionIndex, void* pVoid);
void OnConnectServerFail(void* pVoid);

extern CServerSystem * g_pServerSystem;

#endif // !defined(AFX_SERVERSYSTEM_H__6D60EE32_C8B5_44A7_883C_8EF113DD06B4__INCLUDED_)
