// BattleFactory_Suryun.cpp: implementation of the CBattleFactory_Suryun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleFactory_Suryun.h"
#ifdef _MAPSERVER_
#include "GridSystem.h"
#include "TileManager.h"
#endif

#include "Battle_Suryun.h"
#include "BattleTeam_Suryun.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleFactory_Suryun::CBattleFactory_Suryun()
{
#ifdef _MAPSERVER_
	m_BattlePool.Init(100,10,"CBattleFactory_Suryun");
#endif
}

CBattleFactory_Suryun::~CBattleFactory_Suryun()
{
#ifdef _MAPSERVER_
	m_BattlePool.Release();
#endif
}

CBattle* CBattleFactory_Suryun::CreateBattle( BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum )
{
#ifdef _MAPSERVER_	
	//弊府靛 积己
	g_pServerSystem->GetGridSystem()->CreateGridTable(pCreateInfo->BattleID, eGridType_Murim, 512,512,MapNum);
	
	//鸥老 积己
	if(g_pServerSystem->GetMap()->GetTileManager()->CreateTileGroup(pCreateInfo->BattleID, MapNum, 0, 0, 51200, 51200) == FALSE)
	{
		return NULL;
	}
	
	CBattle_Suryun* pBattle = m_BattlePool.Alloc();
#else
	CBattle_Suryun* pBattle = new CBattle_Suryun;
#endif
	

	
	pBattle->Initialize( pCreateInfo, NULL, NULL );
	return pBattle;
}

void CBattleFactory_Suryun::DeleteBattle( CBattle* pBattle )
{
#ifdef _MAPSERVER_
	g_pServerSystem->GetGridSystem()->DeleteGridTable(pBattle->GetBattleID());
	g_pServerSystem->GetMap()->GetTileManager()->DeleteTileGroup(pBattle->GetBattleID());
	
	m_BattlePool.Free((CBattle_Suryun*)pBattle);
#else
	delete pBattle;
#endif

}
