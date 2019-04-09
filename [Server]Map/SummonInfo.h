// SummonInfo.h: interface for the CSummonInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUMMONINFO_H__200D1758_9FE6_48C6_8D90_0753EDCFD7FD__INCLUDED_)
#define AFX_SUMMONINFO_H__200D1758_9FE6_48C6_8D90_0753EDCFD7FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MHFile.h"
#include <map>

using namespace std;

class CSummonGroup;

class CSummonInfo  
{
	map<int, CSummonGroup*> m_mapSummonGroup;
public:
	CSummonInfo();
	virtual ~CSummonInfo();

	void Release();
	
	void GroupInit(CMHFile * fp);
	void AddSummonGroup(int num, CSummonGroup* pInfo);
	CSummonGroup* GetSummonGroup(int num);
	void RegenGroup(CBossMonster* pBoss, int GroupNum);
};

#endif // !defined(AFX_SUMMONINFO_H__200D1758_9FE6_48C6_8D90_0753EDCFD7FD__INCLUDED_)
