// MonitorServerTable.cpp: implementation of the CMonitorServerTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonitorServerTable.h"
#include "ServerListManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CMonitorServerTable)

CMonitorServerTable::CMonitorServerTable()
{
	m_InfoPool.Init(50, 2, "MonitorServer Pool");
	m_InfoTable.Initialize(50);
	m_pMASInfo = NULL;
	m_pMSInfo = NULL;

	memset( m_sPath, 0, 256 );
}

CMonitorServerTable::~CMonitorServerTable()
{
	RemoveAllServer();
	m_InfoTable.RemoveAll();
	if(m_pMSInfo)
		m_InfoPool.Free(m_pMSInfo);
	m_InfoPool.Release();
}
void CMonitorServerTable::LoadMonitorInfo(WORD typeServer,char* pMyPublicIp,char* pMyPrivateIp)
{
	m_pMASInfo = m_InfoPool.Alloc();
	memset(m_pMASInfo, 0, sizeof(MSERVERINFO));
	SERVERINFO* pLoadedMASInfo = SERVERLISTMGR->GetServerInfoByNum(MONITOR_AGENT_SERVER,0);
	strcpy(m_pMASInfo->szIPForServer, pLoadedMASInfo->szIPForServer);
	m_pMASInfo->wPortForServer = pLoadedMASInfo->wPortForServer;
	strcpy(m_pMASInfo->szIPForUser, pLoadedMASInfo->szIPForUser);
	m_pMASInfo->wPortForUser = pLoadedMASInfo->wPortForUser;
	m_pMASInfo->wServerType = pLoadedMASInfo->wServerKind;

	AddServer(m_pMASInfo, m_pMASInfo->wPortForUser);

	if(typeServer == TYPE_MS)
	{	
		m_pMSInfo = m_InfoPool.Alloc();
		SERVERINFO* pLoadedMSInfo = SERVERLISTMGR->GetServerInfoByIp(MONITOR_SERVER,pMyPublicIp);
		if(pLoadedMSInfo == NULL)
		{
			pLoadedMSInfo = SERVERLISTMGR->GetServerInfoByIp(MONITOR_SERVER,pMyPrivateIp);
		}

		memset(m_pMSInfo, 0, sizeof(MSERVERINFO));
		strcpy(m_pMSInfo->szIPForServer, pLoadedMSInfo->szIPForServer);
		m_pMSInfo->wPortForServer = pLoadedMSInfo->wPortForServer;
		m_pMSInfo->wServerType = MONITOR_SERVER;

		GetCurrentDirectory(256,m_sPath);
	}
}
void CMonitorServerTable::AddServer(MSERVERINFO * info, DWORD key)
{
	MSERVERINFO * tmp = m_InfoPool.Alloc();
	memcpy(tmp, info, sizeof(MSERVERINFO));
	m_InfoTable.Add(tmp, key);
}

void CMonitorServerTable::RemoveServer(DWORD key)
{
	MSERVERINFO * tmp = m_InfoTable.GetData(key);
	m_InfoTable.Remove(key);
	m_InfoPool.Free(tmp);
}

void CMonitorServerTable::RemoveAllServer()
{
	m_InfoTable.SetPositionHead();
	MSERVERINFO * info = NULL;
	while(info = (MSERVERINFO *)m_InfoTable.GetData())
	{
		m_InfoTable.Remove(info->wPortForServer);
		m_InfoPool.Free(info);
	}
}
MSERVERINFO * CMonitorServerTable::GetServerInfoForPort(WORD port)
{
	return m_InfoTable.GetData(port);
}
//custom
MSERVERINFO * CMonitorServerTable::GetMSServerInfoForIP(char * szIP)
{
	m_InfoTable.SetPositionHead();
	MSERVERINFO * info = NULL;
	while(info = (MSERVERINFO *)m_InfoTable.GetData())
	{
		if(info->wServerType == MONITOR_SERVER && strcmp(info->szIPForServer, szIP) == 0)
		{
			return info;
		}
	}
	return NULL;
}
MSERVERINFO * CMonitorServerTable::GetServerInfoForIP(char * szIP)
{
	m_InfoTable.SetPositionHead();
	MSERVERINFO * info = NULL;
	while(info = (MSERVERINFO *)m_InfoTable.GetData())
	{
		if(strcmp(info->szIPForServer, szIP) == 0)
		{
			return info;
		}
	}
	return NULL;
}
void CMonitorServerTable::SetPositionHeadInfo()
{
	m_InfoTable.SetPositionHead();
}
MSERVERINFO * CMonitorServerTable::GetDataInfo()
{
	return m_InfoTable.GetData();
}

MSERVERINFO * CMonitorServerTable::GetServerInfoForConn(DWORD dwConnectionIdx)
{
	m_InfoTable.SetPositionHead();
	MSERVERINFO * info = NULL;
	while(info = (MSERVERINFO *)m_InfoTable.GetData())
	{
		if(info->dwConnectionIndex == dwConnectionIdx)
		{
			return info;
		}
	}
	return NULL;
}
void CMonitorServerTable::RemoveServerForConn(DWORD dwConnectionIdx)
{
	m_InfoTable.SetPositionHead();
	MSERVERINFO * info = NULL;
	while(info = (MSERVERINFO *)m_InfoTable.GetData())
	{
		if(info->dwConnectionIndex == dwConnectionIdx)
		{
			m_InfoTable.Remove(info->wPortForServer);
			m_InfoPool.Free(info);
			return ;
		}
	}
	return ;
}