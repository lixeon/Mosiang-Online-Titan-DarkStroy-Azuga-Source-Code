// MonitorUserTable.h: interface for the CMonitorUserTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONITORUSERTABLE_H__9001AEB0_67AA_4421_AAA9_D53D8340964A__INCLUDED_)
#define AFX_MONITORUSERTABLE_H__9001AEB0_67AA_4421_AAA9_D53D8340964A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct MCINFO
{
	MCINFO()
	{
		dwConnectionIndex = 0;
	}
	DWORD	dwConnectionIndex;					// Ä¿³Ø¼Ç ÀÎµ¦½º : °íÀ¯ÀÎµ¦½º
};
#include "stdio.h"
#include "MemoryPoolTempl.h"
//#include "IndexGenerator.h"
#include "Hashtable.h"

#define MUserTable USINGTON(CMonitorUserTable)

class CMonitorUserTable  
{
	CMemoryPoolTempl<MCINFO> m_InfoPool;
	CYHHashTable<MCINFO> m_InfoTable;
	//CIndexGenerator m_MCIDIndex;
	friend class CMASManager;
public:
	CMonitorUserTable();
	virtual ~CMonitorUserTable();

	void ReleaseAll();

	void AddUser(MCINFO * pInfo, DWORD dwConnectionIdx);
	void RemoveUser(DWORD dwConnectionIdx);
};

EXTERNGLOBALTON(CMonitorUserTable)
#endif // !defined(AFX_MONITORUSERTABLE_H__9001AEB0_67AA_4421_AAA9_D53D8340964A__INCLUDED_)
