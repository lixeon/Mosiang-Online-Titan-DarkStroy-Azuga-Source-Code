// StatisticsManager.h: interface for the CStatisticsManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATISTICSMANAGER_H__F3BE24EE_8563_43AF_A51F_4A6EDF55FD8A__INCLUDED_)
#define AFX_STATISTICSMANAGER_H__F3BE24EE_8563_43AF_A51F_4A6EDF55FD8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STATISTICSMGR CStatisticsManager::GetInstance()

class CStatisticsManager  
{
	DWORD m_dwLastSaveTime;
	DWORD m_dwMonsterDieCount[MAX_MONSTER_NUM];

public:
	CStatisticsManager();
	virtual ~CStatisticsManager();

	MAKESINGLETON( CStatisticsManager );

	void Init();
	void Process();

	void SaveMonsterStatistics();
	void MonsterDieCount( WORD wMonsterKind );

};

#endif // !defined(AFX_STATISTICSMANAGER_H__F3BE24EE_8563_43AF_A51F_4A6EDF55FD8A__INCLUDED_)
