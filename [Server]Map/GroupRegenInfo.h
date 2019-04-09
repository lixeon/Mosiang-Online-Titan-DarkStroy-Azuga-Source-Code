// GroupRegenInfo.h: interface for the CGroupRegenInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GROUPREGENINFO_H__7D27C0AE_746F_4FAF_A8CC_244D6A8645EE__INCLUDED_)
#define AFX_GROUPREGENINFO_H__7D27C0AE_746F_4FAF_A8CC_244D6A8645EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "cLinkedList.h"
#include "cConstLinkedList.h"
#include "RegenConditionInfo.h"

class CAIGroup;

class CGroupRegenInfo  
{
	cLinkedList<CRegenConditionInfo> m_RegenConditionList;
	cConstLinkedList<DWORD> m_RegenIDList;
	cConstLinkedList<DWORD> m_RegenIDTmpList;
	CAIGroup * m_pGroup;
public:
	CGroupRegenInfo();
	virtual ~CGroupRegenInfo();
	void SetGroup(CAIGroup * pGroup);
	void RegenCheck(DWORD CurObjectNum, DWORD MaxObjectNum);
	void AddCondition(CRegenConditionInfo * pInfo);
	DWORD GetWaitRegenObjectNum();
	void RegenProcess();
	void ForceRegen();
	BOOL RegenObject(DWORD dwID);

	void AddID(DWORD id);
	void RemoveID(DWORD id);

	void SetRegenDelayTime(DWORD delaytime);
	void SetRegenRandGridID(DWORD GridID);
};

#endif // !defined(AFX_GROUPREGENINFO_H__7D27C0AE_746F_4FAF_A8CC_244D6A8645EE__INCLUDED_)
