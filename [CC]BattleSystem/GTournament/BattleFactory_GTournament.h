// BattleFactory_GTournament.h: interface for the CBattleFactory_GTournament class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLEFACTORY_GTOURNAMENT_H__D0E836D2_638F_43C7_B45B_1914B32ED3E5__INCLUDED_)
#define AFX_BATTLEFACTORY_GTOURNAMENT_H__D0E836D2_638F_43C7_B45B_1914B32ED3E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BattleFactory.h"

#ifdef _MAPSERVER_
#include "Battle_GTournament.h"
#endif


class CBattleFactory_GTournament : public CBattleFactory
{
#ifdef _MAPSERVER_
	CMemoryPoolTempl<CBattle_GTournament> m_BattlePool;
#endif


public:
	CBattleFactory_GTournament();
	virtual ~CBattleFactory_GTournament();

	virtual CBattle* CreateBattle(BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum);
	virtual void DeleteBattle(CBattle* pBattle);

};

#endif // !defined(AFX_BATTLEFACTORY_GTOURNAMENT_H__D0E836D2_638F_43C7_B45B_1914B32ED3E5__INCLUDED_)
