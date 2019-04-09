// MonitorServerTable.h: interface for the CMonitorServerTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONITORSERVERTABLE_H__C5DF5642_7225_48BC_861B_F8B9F6973A9F__INCLUDED_)
#define AFX_MONITORSERVERTABLE_H__C5DF5642_7225_48BC_861B_F8B9F6973A9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MServerTable USINGTON(CMonitorServerTable)
enum 
{
	TYPE_NONE = 0,
	TYPE_MAS = 1,
	TYPE_MS = 2,
	TYPE_RMS = 3, 
};

#include "stdio.h"
#include "MemoryPoolTempl.h"
#include "Hashtable.h"

class CMonitorServerTable  
{
	CMemoryPoolTempl<MSERVERINFO> m_InfoPool;
	CYHHashTable<MSERVERINFO> m_InfoTable;
	friend class CMonitorNetwork;
public:
	CMonitorServerTable();
	virtual ~CMonitorServerTable();
	void LoadMonitorInfo(WORD typeServer,char* pMyPublicIp,char* pMyPrivateIp);

	void AddServer(MSERVERINFO * info, DWORD key);
	void RemoveServer(DWORD key);
	void RemoveServerForConn(DWORD dwConnectionIdx);
	void RemoveAllServer();
	MSERVERINFO * GetMASInfo() { return m_pMASInfo; }
	MSERVERINFO * GetMSInfo() { return m_pMSInfo; }
	MSERVERINFO * GetServerInfoForIP(char * szIP);
	MSERVERINFO * GetServerInfoForPort(WORD port);
	MSERVERINFO * GetServerInfoForConn(DWORD dwConnectionIdx);
	MSERVERINFO * GetMSServerInfoForIP(char * szIP);
	
	MSERVERINFO * GetDataInfo();
	void SetPositionHeadInfo();
	
	MSERVERINFO * m_pMASInfo;
	MSERVERINFO * m_pMSInfo;

	char	m_sPath[256];
};

EXTERNGLOBALTON(CMonitorServerTable)
#endif // !defined(AFX_MONITORSERVERTABLE_H__C5DF5642_7225_48BC_861B_F8B9F6973A9F__INCLUDED_)
