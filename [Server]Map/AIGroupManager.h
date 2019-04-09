// AIGroupManager.h: interface for the CAIGroupManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIGROUPMANAGER_H__19E2050E_AF77_4BA9_ADDB_7513F927618C__INCLUDED_)
#define AFX_AIGROUPMANAGER_H__19E2050E_AF77_4BA9_ADDB_7513F927618C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AIGroupPrototype.h"

#define GROUPMGR	CAIGroupManager::GetInstance()
class CAIGroup;

class CAIGroupManager  
{
	CYHHashTable<CAIGroup> m_groupList;
	CAIGroupManager();
public:
	virtual ~CAIGroupManager();
	GETINSTANCE(CAIGroupManager)

	void Release();
	void AddGroup(CAIGroup * aiGroup);
	void RemoveGroup(DWORD dwGroupID, DWORD dwGridID);

	CAIGroup * GetGroup(DWORD dwGroupID, DWORD dwGridID);

	CRegenObject * GetRegenObject(DWORD dwGroupID, DWORD dwGridID, DWORD dwID);

	void RegenProcess();
};

#endif // !defined(AFX_AIGROUPMANAGER_H__19E2050E_AF77_4BA9_ADDB_7513F927618C__INCLUDED_)
