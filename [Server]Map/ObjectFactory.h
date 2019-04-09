// ObjectFactory.h: interface for the CObjectFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTFACTORY_H__324EFDDF_059A_428C_94F2_76BC8E75E765__INCLUDED_)
#define AFX_OBJECTFACTORY_H__324EFDDF_059A_428C_94F2_76BC8E75E765__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"
//class CObject;
class CPlayer;
class CTitan;
class CPet;
class CMonster;
class CNpc;
class CBossMonster;
class CTacticObject;
class CMapObject;

// 필드보스 - 05.12 이영준
class CFieldBossMonster;
class CFieldSubMonster;

class CObjectFactory
{
private:
	CMemoryPoolTempl<CPlayer>* PlayerPool;
	CMemoryPoolTempl<CPet>* PetPool;
	CMemoryPoolTempl<CMonster>* MonsterPool;	
	CMemoryPoolTempl<CNpc>* NpcPool;	
	CMemoryPoolTempl<CTacticObject>* TacticPool;	
	CMemoryPoolTempl<CBossMonster>* BossMonsterPool;
	
	// 필드보스 - 05.12 이영준
	CMemoryPoolTempl<CFieldBossMonster>* FieldBossMonsterPool;
	CMemoryPoolTempl<CFieldSubMonster>* FieldSubMonsterPool;
	
	CMemoryPoolTempl<CMapObject>*	MapObjectPool;

	CMemoryPoolTempl<CTitan>*	TitanPool;
	CMemoryPoolTempl<TITAN_TOTALINFO>* TitanInfoPool;

public:
	CObjectFactory();
	virtual ~CObjectFactory();
	
	void Init();
	void Release();

	CObject* MakeNewObject(EObjectKind Kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo);
	void ReleaseObject(CObject* pObject);

	TITAN_TOTALINFO* MakeTitanInfo(TITAN_TOTALINFO* titanInfo);
	void ReleaseTitanInfo(TITAN_TOTALINFO* titanInfo);
};
extern CObjectFactory * g_pObjectFactory; 
#endif // !defined(AFX_OBJECTFACTORY_H__324EFDDF_059A_428C_94F2_76BC8E75E765__INCLUDED_)
