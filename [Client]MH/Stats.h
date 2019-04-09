// Stats.h: interface for the CStats class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATS_H__D7E414F0_8BB6_47FF_BD04_228AB2BA7848__INCLUDED_)
#define AFX_STATS_H__D7E414F0_8BB6_47FF_BD04_228AB2BA7848__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameResourceStruct.h"

class CStats  
{
	player_calc_stats m_Stats;
	
public:
	CStats();
	virtual ~CStats();

	void AddOtherStats(CStats* pOtherStats);
	void AddItemInfoStats(const ITEM_INFO* pItemInfo);
	void Clear();
	

};

#endif // !defined(AFX_STATS_H__D7E414F0_8BB6_47FF_BD04_228AB2BA7848__INCLUDED_)
