#include "confirm.h"
#include "memory_object.h"
#include "confirm_type.h"
#include "collect_confirm_packet.h"

#ifdef _NEED_CONFIRM

#define RECV_THREAD_SUCCESS 0
#define	RECV_THREAD_FAIL	1



char*	g_pCodeMemory = NULL;
char*	g_pRecvBuffer = NULL;
DWORD	g_dwRecvedSize = 0;
DWORD	g_dwMaxRecvSize = 65535;
SOCKET	g_socketConfirm = INVALID_SOCKET;
WORD	g_wPort = 0;
HANDLE	g_hRecvThreadStart;
HANDLE	g_hRecvThreadEvent[2];
DWORD	g_dwAddr;

DWORD WINAPI RecvThread(LPVOID lpVoid);
BOOL RecvBytes(SOCKET s,char* pBuffer,int len);
BOOL SendBytes(SOCKET s,char* pBuffer,int len);
BOOL CollectCodeBytes(char* pDest,char* pSrc,DWORD dwSize);
DWORD GenerateConfirmPacket(char* pDest);

BOOL GetConfirmData(CONFIRM_PACKET* pPacket)
{
	memset(pPacket,0,pPacket->GetPacketSize());
	pPacket->wID = PTCL_CONFIRM_DLL;

	return TRUE;
}
SOCKET Connect(DWORD dwAddr,WORD wPort)
{
	sockaddr_in addr;
	
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(wPort);
	addr.sin_addr.s_addr = dwAddr;
	int socket;
	
	
	socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,NULL, 0,NULL);

	
	if(INVALID_SOCKET == socket)
		return socket;
	
	if(SOCKET_ERROR == connect(socket, (LPSOCKADDR)&addr, sizeof(addr)))
		return INVALID_SOCKET;
	
	return socket;
}


BOOL ConfirmFromESofnetServer(char* name,WORD port)
{
	if (!g_pCodeMemory)
	{
		g_pCodeMemory = new char[256];
		memset(g_pCodeMemory,0xcc,256);
	}

	if (!g_pRecvBuffer)
	{
		g_pRecvBuffer = new char[65535];
		memset(g_pRecvBuffer,0,65535);
	}
	HOSTENT*	hp = gethostbyname(name);
	g_dwAddr = *(DWORD*)hp->h_addr;
	g_wPort = port;

	for (DWORD i=0; i<2; i++)
		g_hRecvThreadEvent[i] = CreateEvent(NULL,FALSE,FALSE,NULL);

	g_hRecvThreadStart = CreateEvent(NULL,FALSE,FALSE,NULL);

	HANDLE hThread = CreateThread(NULL,NULL,RecvThread,NULL,NULL,NULL);
	CloseHandle(hThread);
	SetEvent(g_hRecvThreadStart);

	DWORD index = WaitForMultipleObjects(2,g_hRecvThreadEvent,FALSE,INFINITE);

	if (RECV_THREAD_FAIL == index )
	{
		MessageBox(NULL,"Fail to confirm from eSofnet Server, server down???","Error",MB_OK);
		return FALSE;
	}
	else if (RECV_THREAD_SUCCESS == index)
	{
		
		__asm
		{
			mov			esi,dword ptr[g_pCodeMemory]
			mov			dword ptr[InitializeMemoryObjectChain],esi

		}
		return TRUE;
	}
	return FALSE;
}

void CleanupConfirm()
{
	for (DWORD i=0; i<2; i++)
		CloseHandle(g_hRecvThreadEvent[i]);

	CloseHandle(g_hRecvThreadStart);

	
	if (g_pCodeMemory)
	{
		delete g_pCodeMemory;
		g_pCodeMemory = NULL;
	}
	if (g_pRecvBuffer)
	{
		delete g_pRecvBuffer;
		g_pRecvBuffer = NULL;
	}
}

BOOL SendBytes(SOCKET s,char* pBuffer,int len)
{
	int result;
	while (len)
	{

		result = send(s,pBuffer,len,NULL);
		if (SOCKET_ERROR == result)
		{
			closesocket(s);
			return FALSE;
		}
		len -= result;
		pBuffer += result;
	}
	return TRUE;
}
BOOL RecvBytes(SOCKET s,char* pBuffer,int len)
{
	int result;
	while (len)
	{

		result = recv(s,pBuffer,len,NULL);
		if (SOCKET_ERROR == result)
		{
			closesocket(s);
			return FALSE;
		}
		len -= result;
		pBuffer += result;
	}
	return TRUE;
}

DWORD WINAPI RecvThread(LPVOID lpVoid)
{
//	DWORD		dwTransferredBytes;							// Commented by chan78 at 2001/10/17
	DWORD		dwFlag = NULL;
	WORD		wLen = 0;
	SOCKET		s = INVALID_SOCKET;

	CONFIRM_PACKET	cfPacket;
	
	if (!GetConfirmData(&cfPacket))
		goto lb_fail;

	wLen = (WORD)cfPacket.GetPacketSize();					// Modified by chan78 at 2001/10/17

	WaitForSingleObject(g_hRecvThreadStart,INFINITE);

	s = Connect(g_dwAddr,g_wPort);

	if (INVALID_SOCKET == s)
		goto lb_fail;

	if (FALSE == SendBytes(s,(char*)&wLen,2))
		goto lb_fail;

	if (FALSE == SendBytes(s,(char*)&cfPacket,cfPacket.GetPacketSize()))
		goto lb_fail;

	if (FALSE == RecvBytes(s,(char*)&wLen,2))
		goto lb_fail;

	if (FALSE == RecvBytes(s,g_pRecvBuffer,(int)wLen))
		goto lb_fail;

lb_success:
	CollectCodeBytes(g_pCodeMemory,g_pRecvBuffer,(DWORD)wLen);
	closesocket(s);
	SetEvent(g_hRecvThreadEvent[RECV_THREAD_SUCCESS]);
	return 0;

lb_fail:
	closesocket(s);
	SetEvent(g_hRecvThreadEvent[RECV_THREAD_FAIL]);
	return 0;

}
#endif

/*
  55						 
  8b ec	 			 
  53						 
  56						 
  57						 
  33 f6						 
  8b 5d 08					 
  8b 4d 0c					 
  b8 40 00 00 00			
  8b d3						
  89 73 04					
  83 c2 4c					
  89 13		 
  8b f3		 
  89 43 08	 
  83 c3 4c	 
  e2 ee		 
  83 eb 4c	 
  c7 03 00 00 00 00		
  5f	
  5e	
  5b	
  5d	
  c3	
*/