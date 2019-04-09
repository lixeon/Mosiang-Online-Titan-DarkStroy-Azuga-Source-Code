// RegenManager.h: interface for the CRegenManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGENMANAGER_H__D3073732_C75A_4B85_B7C5_91C25476DBDC__INCLUDED_)
#define AFX_REGENMANAGER_H__D3073732_C75A_4B85_B7C5_91C25476DBDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MONSTER_REGEN_RANDOM_RANGE	1500
#define REGENMGR	CRegenManager::GetInstance()


class CRegenPrototype;

class CRegenManager  
{
	//cPtrList m_RegenPrototypeList;
	CYHHashTable<CRegenPrototype> m_RegenPrototypeList;
	void RangePosAtOrig(VECTOR3 * OrigPos, int Range, VECTOR3 * OutPos);
public:
	GETINSTANCE(CRegenManager)
	CRegenManager();
	virtual ~CRegenManager();

	void Release();
	void AddPrototype(CRegenPrototype * pType);
	CRegenPrototype * GetPrototype(DWORD dwID);
	void RegenGroup(DWORD groupID, DWORD dwGridID);
	BOOL RegenObject(DWORD groupID, DWORD dwGridID, DWORD dwObjectID,BOOL bRandomPos = TRUE);
	CMonster* RegenObject(DWORD dwID, DWORD dwSubID, DWORD dwGridID, WORD wObjectKind, WORD wMonsterKind, VECTOR3 * vPos, DWORD dwGroupID, WORD DropItemID = 0, DWORD dwDropRatio = 100 ,BOOL bRandomPos = TRUE, BOOL bEventRegen = FALSE);
};

#endif // !defined(AFX_REGENMANAGER_H__D3073732_C75A_4B85_B7C5_91C25476DBDC__INCLUDED_)
