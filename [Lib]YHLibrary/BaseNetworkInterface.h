#pragma once

#include <initguid.h>
#include "stdafx.h"
#include "Desc_BaseNetwork.h"

/*************************************************************************************/
// ÀÌ È­ÀÏÀº Àý´ë ¼öÁ¤µÇ¾î¼­ ¾È µË´Ï´Ù.
/*************************************************************************************/

// {78771B7B-6E5E-4659-87E4-ABE9F0793AA1}
DEFINE_GUID(CLSID_SC_BASENETWORK_DLL, 
0x78771b7b, 0x6e5e, 0x4659, 0x87, 0xe4, 0xab, 0xe9, 0xf0, 0x79, 0x3a, 0xa1);

// {DCED45F1-980B-4578-9F1D-C0586A5E3964}
DEFINE_GUID(IID_SC_BASENETWORK_DLL, 
0xdced45f1, 0x980b, 0x4578, 0x9f, 0x1d, 0xc0, 0x58, 0x6a, 0x5e, 0x39, 0x64);

interface ISC_BaseNetwork : public IUnknown
{
	virtual void	__stdcall					CloseAllConnection() = 0;
	virtual void	__stdcall					CloseConnection(DWORD dwIndex) = 0;
	virtual BOOL	__stdcall					Send(DWORD dwConnectionIndex,char* msg,DWORD length) = 0;
	virtual DWORD	__stdcall					ConnectToServer(char* szIP,WORD port) = 0;
	virtual BOOL	__stdcall					StartServer(char* ip,WORD port) =0;
	virtual void	__stdcall					CompulsiveDisconnect(DWORD dwIndex) =0;
	virtual DWORD	__stdcall					InitNetwork(HWND hwnd, DWORD dwMaxConnect,DESC_BASENETWORK* pDesc )=0;
	virtual	void	__stdcall					SocketEventProc() = 0;
};

typedef HRESULT (__stdcall *DllGetClassObject_BaseNetwork)( REFCLSID , REFIID , LPVOID*);