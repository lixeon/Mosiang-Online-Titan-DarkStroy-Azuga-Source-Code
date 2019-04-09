// AIManager.h: interface for the CAIManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIMANAGER_H__1E5F6FF9_7910_4C5D_992E_575B3EA35526__INCLUDED_)
#define AFX_AIMANAGER_H__1E5F6FF9_7910_4C5D_992E_575B3EA35526__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BASE_MONSTER_LIST;
class CAIParam;

//#define AIMNGR	CAIManager::GetInstance()
class CAIManager  
{
public:
//	GETINSTANCE(CAIManager)
	CAIManager();
	virtual ~CAIManager();
	static BOOL IsTargetChange(CPlayer * pAttacker, CMonster * pDefender);
	static DWORD RandCurAttackKind(BASE_MONSTER_LIST	* apr, CAIParam * aprm);
	
	static BOOL IsRunawayStateChange(CMonster * pDefender, CPlayer * pAttacker);
};

#endif // !defined(AFX_AIMANAGER_H__1E5F6FF9_7910_4C5D_992E_575B3EA35526__INCLUDED_)
