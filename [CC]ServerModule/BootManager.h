// BootManager.h: interface for the CBootManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOOTMANAGER_H__682A3B42_83C6_4A98_9324_5618C58BA011__INCLUDED_)
#define AFX_BOOTMANAGER_H__682A3B42_83C6_4A98_9324_5618C58BA011__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BOOTMNGR	USINGTON(CBootManager)

class CNetwork;
class CServerTable;
class CBootManager  
{
	SERVERINFO* GetServerInfoByIp(WORD ServerType,char* publicIp,char* privateIp);
	SERVERINFO* GetServerInfoByNum(WORD ServerType, WORD serverNum);
	BOOL CheckLinkServer(WORD SelfServerType,WORD ToServerType);

public:
	CBootManager();
	virtual ~CBootManager();

	void NotifyBootUpToMS(CNetwork * pNet);
	void BactchConnectToMap(CNetwork * pNet, CServerTable * pServerTable);
	BOOL ConnectToMS(CNetwork * pNet, CServerTable * pServerTable);


	BOOL StartServer(CNetwork * pNet, 	SERVERINFO * pSelfServerInfo);
	void AddBootList(CServerTable * pServerTable, SERVERINFO * pServerInfo);
	void AddSelfBootList(WORD MyServerKind, WORD MyServerNum,  CServerTable * pServerTable);
	BOOL RemoveBootList(CServerTable * pServerTable, WORD key);
	void UpdateBootList(CServerTable * pServerTable, SERVERINFO * pServerInfo, DWORD dwConnectionIdx);
	void SendConnectAckMsg(CNetwork * pNet, DWORD dwConnectionIndex, SERVERINFO * pServerInfo);
	void SendConnectSynMsg(CNetwork * pNet, DWORD dwConnectionIdxTo, CServerTable * pServerTable);
	void AddBootListINI(WORD MyServerKind, WORD MyServerNum,  CServerTable * pServerTable);
	void NetworkMsgParse(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
	
	BOOL	ReadServerSetNum();
};
EXTERNGLOBALTON(CBootManager);
#endif // !defined(AFX_BOOTMANAGER_H__682A3B42_83C6_4A98_9324_5618C58BA011__INCLUDED_)
