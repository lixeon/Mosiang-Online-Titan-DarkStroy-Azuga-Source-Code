// HelpRequestManager.h: interface for the CHelpRequestManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELPREQUESTMANAGER_H__BFA03744_2DF3_425A_A957_6FC03BCACD5E__INCLUDED_)
#define AFX_HELPREQUESTMANAGER_H__BFA03744_2DF3_425A_A957_6FC03BCACD5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMonster;
class CHelpRequestManager  
{
	enum
	{
		HELPREQUEST_NONE			= 0,
		HELPREQUEST_ONETIMEIFHP50	= 1,
		HELPREQUEST_ALWAYSIFHP30	= 2,
		HELPREQUEST_DIE				= 3,
		HELPREQUEST_ALWAYS			= 4,
	};
public:
	CHelpRequestManager();
	virtual ~CHelpRequestManager();

	static void MonsterRequestProcess(CMonster * pMonster);
	static void SetHelperMonster(CMonster* pAsker, CMonster * pHelper, CPlayer * pTargeter);
};

#endif // !defined(AFX_HELPREQUESTMANAGER_H__BFA03744_2DF3_425A_A957_6FC03BCACD5E__INCLUDED_)
