// AIGroupPrototype.h: interface for the CAIGroupPrototype class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIGROUPPROTOTYPE_H__9B71F348_C5EC_4AE3_B0EE_BB0321DCA84D__INCLUDED_)
#define AFX_AIGROUPPROTOTYPE_H__9B71F348_C5EC_4AE3_B0EE_BB0321DCA84D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "GroupRegenInfo.h"

class CRegenConditionInfo;
class CRegenObject;

class CAIGroup
{
	CYHHashTable<CRegenObject> m_RegenObjectInfoList;
public:
	CAIGroup();
	virtual ~CAIGroup();
	
	inline	DWORD GetMaxObjectNum();
	inline	DWORD GetCurObjectNum();
	void Die(DWORD id);
	void Alive(DWORD id);
	virtual void RegenCheck();
	void	AddConditionInfo(CRegenConditionInfo * pInfo);
	void AddRegenObject(CRegenObject * pObj);

	void ForceRegen();
	void RegenProcess();

	DWORD GetGroupID();
	DWORD GetGridID();


	CRegenObject * GetRegenObject(DWORD dwObjectID);

	DWORD	m_dwGroupID;
	DWORD	m_dwGridID;
	
	CGroupRegenInfo		m_RegenInfo;
	
	CGroupRegenInfo* GetRegenInfo();
	void SetRandomGridID(DWORD GridID);
};

#endif // !defined(AFX_AIGROUPPROTOTYPE_H__9B71F348_C5EC_4AE3_B0EE_BB0321DCA84D__INCLUDED_)
