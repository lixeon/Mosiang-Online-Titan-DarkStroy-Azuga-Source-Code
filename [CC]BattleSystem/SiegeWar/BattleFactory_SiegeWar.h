// BattleFactory_SiegeWar.h: interface for the CBattleFactory_SiegeWar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLEFACTORY_SIEGEWAR_H__07663A79_3B63_4DF4_ACE5_41CEE7243E86__INCLUDED_)
#define AFX_BATTLEFACTORY_SIEGEWAR_H__07663A79_3B63_4DF4_ACE5_41CEE7243E86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BattleFactory.h"

#ifdef _MAPSERVER_
#include "Battle_SiegeWar.h"
#endif


class CBattleFactory_SiegeWar : public CBattleFactory
{
#ifdef _MAPSERVER_
	CMemoryPoolTempl<CBattle_SiegeWar> m_BattlePool;
#endif

public:
	CBattleFactory_SiegeWar();
	virtual ~CBattleFactory_SiegeWar();

	virtual CBattle* CreateBattle(BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum);
	virtual void DeleteBattle(CBattle* pBattle);
};

#endif // !defined(AFX_BATTLEFACTORY_SIEGEWAR_H__07663A79_3B63_4DF4_ACE5_41CEE7243E86__INCLUDED_)
