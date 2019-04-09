#pragma once

#include "../4DyuchiNET_Common/INetwork_GUID.h"
#include "../4DyuchiNET_Common/INetwork.h"
//#include "../4DyuchiNET_Common/typedef.h"



class Co4DyuchiNET : public I4DyuchiNET
{
//	OnIntialFunc 					m_pOnInitialFunc;

public:
    STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);
	BOOL			__stdcall			CreateNetwork(DESC_NETWORK* desc,DWORD dwUserAcceptInterval,DWORD dwServerAcceptInterval,OnIntialFunc pFunc);
	void 			__stdcall			BreakMainThread();
	void			__stdcall			ResumeMainThread();
	void			__stdcall			SetUserInfo(DWORD dwConnectionIndex,void* user);
	void*			__stdcall			GetUserInfo(DWORD dwConnectionIndex);

	void			__stdcall			SetServerInfo(DWORD dwConnectionIndex,void* server);
	void*			__stdcall			GetServerInfo(DWORD dwConnectionIndex);

	sockaddr_in*	__stdcall			GetServerAddress(DWORD dwConnectionIndex);
	sockaddr_in*	__stdcall			GetUserAddress(DWORD dwConnectionIndex);
	BOOL			__stdcall			GetServerAddress(DWORD dwConnectionIndex,char* pIP,WORD* pwPort);
	BOOL			__stdcall			GetUserAddress(DWORD dwConnectionIndex,char* pIP,WORD* pwPort);

	BOOL			__stdcall			SendToServer(DWORD dwConnectionIndex,char* msg,DWORD length,DWORD flag);
	BOOL			__stdcall			SendToUser(DWORD dwConnectionIndex,char* msg,DWORD length,DWORD flag);
	void			__stdcall			CompulsiveDisconnectServer(DWORD dwConnectionIndex);
	void			__stdcall			CompulsiveDisconnectUser(DWORD dwConnectionIndex);
	int				__stdcall			GetServerMaxTransferRecvSize();
	int				__stdcall			GetServerMaxTransferSendSize();
	int				__stdcall			GetUserMaxTransferRecvSize();
	int				__stdcall			GetUserMaxTransferSendSize();
	void			__stdcall			BroadcastServer(char* pMsg,DWORD len,DWORD flag);
	void			__stdcall			BroadcastUser(char* pMsg,DWORD len,DWORD flag);
	DWORD			__stdcall			GetConnectedServerNum();
	DWORD			__stdcall			GetConnectedUserNum();
	WORD			__stdcall			GetBindedPortServerSide();
	WORD			__stdcall			GetBindedPortUserSide();
	BOOL			__stdcall			ConnectToServerWithUserSide(char* szIP,WORD port,CONNECTSUCCESSFUNC,CONNECTFAILFUNC,void* pExt);
	BOOL			__stdcall			ConnectToServerWithServerSide(char* szIP,WORD port,CONNECTSUCCESSFUNC,CONNECTFAILFUNC,void* pExt);
	
	BOOL			__stdcall			StartServerWithUserSide(char* ip,WORD port);
	BOOL			__stdcall			StartServerWithServerSide(char* ip,WORD port);
	HANDLE			__stdcall			GetCustomEventHandle(DWORD index);			
//	BOOL			__stdcall			PauseTimer(DWORD dwCustomEventIndex);						//2007 / 12/19 removed by yuchi
//	BOOL			__stdcall			ResumeTimer(DWORD dwCustomEventIndex);						//2007 / 12/19 removed by yuchi
	
	BOOL			__stdcall			SendToServer(DWORD dwConnectionIndex,INET_BUF* pBuf,DWORD dwNum,DWORD flag);	
	BOOL			__stdcall			SendToUser(DWORD dwConnectionIndex,INET_BUF* pBuf,DWORD dwNum,DWORD flag);	
	BOOL			__stdcall			SendToServer(DWORD dwConnectionIndex,PACKET_LIST* pList,DWORD flag);
	BOOL			__stdcall			SendToUser(DWORD dwConnectionIndex,PACKET_LIST* pList,DWORD flag);
	BOOL			__stdcall			GetMyAddress(char* szOutIP,DWORD dwMaxLen);
	Co4DyuchiNET();
	~Co4DyuchiNET();


private:
	void					DestroyNetwork();
	BOOL					CheckParameter(DESC_NETWORK* desc);
	
	DWORD					m_dwRefCount;
};
