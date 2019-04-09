#pragma once

//#include "stdafx.h"
#include <initguid.h>
#include "network_guid.h"
#include "typedef.h"

interface I4DyuchiNET : public IUnknown
{
	virtual  BOOL			__stdcall			CreateNetwork(DESC_NETWORK* desc) = 0;
	virtual  void 			__stdcall			BreakMainThread() = 0;
	virtual  void			__stdcall			ResumeMainThread() = 0;

	virtual  void			__stdcall			SetUserInfo(DWORD dwConnectionIndex,void* user) = 0;
	virtual  void*			__stdcall			GetUserInfo(DWORD dwConnectionIndex) = 0;

	virtual  void			__stdcall			SetServerInfo(DWORD dwConnectionIndex,void* server) = 0;
	virtual  void*			__stdcall			GetServerInfo(DWORD dwConnectionIndex) = 0;

	virtual  sockaddr_in*	__stdcall			GetServerAddress(DWORD dwConnectionIndex) = 0;
	virtual  sockaddr_in*	__stdcall			GetUserAddress(DWORD dwConnectionIndex) = 0;
	virtual  BOOL			__stdcall			GetServerAddress(DWORD dwConnectionIndex,char* pIP,WORD* pwPort) = 0;
	virtual  BOOL			__stdcall			GetUserAddress(DWORD dwConnectionIndex,char* pIP,WORD* pwPort) = 0;
	virtual  BOOL			__stdcall			SendToServer(DWORD dwConnectionIndex,char* msg,DWORD length,FLAG_SEND flag) = 0;
	virtual  BOOL			__stdcall			SendToUser(DWORD dwConnectionIndex,char* msg,DWORD length,FLAG_SEND flag) = 0;
	virtual  void			__stdcall			CompulsiveDisconnectServer(DWORD dwConnectionIndex) = 0;
	virtual  void			__stdcall			CompulsiveDisconnectUser(DWORD dwConnectionIndex) = 0;
	virtual  int			__stdcall			GetServerMaxTransferRecvSize() = 0;
	virtual  int			__stdcall			GetServerMaxTransferSendSize() = 0;
	virtual  int			__stdcall			GetUserMaxTransferRecvSize() = 0;
	virtual  int			__stdcall			GetUserMaxTransferSendSize() = 0;
	virtual  void			__stdcall			BroadcastServer(char* pMsg,DWORD len,FLAG_SEND flag) = 0;
	virtual  void			__stdcall			BroadcastUser(char* pMsg,DWORD len,FLAG_SEND flag) = 0;
	virtual  DWORD			__stdcall			GetConnectedServerNum() = 0;
	virtual  DWORD			__stdcall			GetConnectedUserNum() = 0;
	virtual  WORD			__stdcall			GetBindedPortServerSide() = 0;
	virtual  WORD			__stdcall			GetBindedPortUserSide() = 0;
	virtual  BOOL			__stdcall			ConnectToServerWithUserSide(char* szIP,WORD port,CONNECTSUCCESSFUNC,CONNECTFAILFUNC,void* pExt) = 0;
	virtual  BOOL			__stdcall			ConnectToServerWithServerSide(char* szIP,WORD port,CONNECTSUCCESSFUNC,CONNECTFAILFUNC,void* pExt) = 0;	
	virtual  BOOL			__stdcall			StartServerWithUserSide(char* ip,WORD port) = 0;
	virtual  BOOL			__stdcall			StartServerWithServerSide(char* ip,WORD port) = 0;
	virtual  HANDLE			__stdcall			GetCustomEventHandle(DWORD index) = 0;
	virtual	 BOOL			__stdcall			PauseTimer(DWORD dwCustomEventIndex) = 0;
	virtual	 BOOL			__stdcall			ResumeTimer(DWORD dwCustomEventIndex) = 0;
};
