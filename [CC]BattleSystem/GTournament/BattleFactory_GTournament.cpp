// BattleFactory_GTournament.cpp: implementation of the CBattleFactory_GTournament class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleFactory_GTournament.h"
#include "Battle_GTournament.h"

#ifdef _MAPSERVER_
#include "Battle_GTournament.h"
#include "BattleSystem_server.h"
#include "GridSystem.h"
#include "TileManager.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleFactory_GTournament::CBattleFactory_GTournament()
{
#ifdef _MAPSERVER_
	m_BattlePool.Init(16,8,"CBattle_GTournament");
#endif
}

CBattleFactory_GTournament::~CBattleFactory_GTournament()
{
#ifdef _MAPSERVER_
	m_BattlePool.Release();
#endif
}

CBattle* CBattleFactory_GTournament::CreateBattle(BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum)
{	
	BATTLE_INFO_GTOURNAMENT* pInfo = (BATTLE_INFO_GTOURNAMENT*)pCreateInfo;

#ifdef _MAPSERVER_
	
	pInfo->BattleID = BATTLESYSTEM->MakeNewBattleID();

	//弊府靛 积己
	g_pServerSystem->GetGridSystem()->CreateGridTable(pInfo->BattleID, eGridType_Murim, 512,512,MapNum);
	
	//鸥老 积己
	if(g_pServerSystem->GetMap()->GetTileManager()->CreateTileGroup(pInfo->BattleID, MapNum, 0, 0, 51200, 51200) == FALSE)
	{
		return NULL;
	}

	CBattle_GTournament* pBattle = m_BattlePool.Alloc();
#else
	CBattle_GTournament* pBattle = new CBattle_GTournament;
#endif
	
	CBattleTeam_GTournament* pBTeam1 = new CBattleTeam_GTournament;
	pBTeam1->Init( &pInfo->MemberInfo, eBattleTeam1 );
	CBattleTeam_GTournament* pBTeam2 = new CBattleTeam_GTournament;
	pBTeam2->Init( &pInfo->MemberInfo, eBattleTeam2 );

	pBattle->Initialize( pCreateInfo, pBTeam1, pBTeam2 );
	
	return pBattle;
}

void CBattleFactory_GTournament::DeleteBattle(CBattle* pBattle)
{
#ifdef _MAPSERVER_
	g_pServerSystem->GetGridSystem()->DeleteGridTable(pBattle->GetBattleID());
	g_pServerSystem->GetMap()->GetTileManager()->DeleteTileGroup(pBattle->GetBattleID());

	BATTLESYSTEM->ReleaseBattleID(pBattle->GetBattleID());
	m_BattlePool.Free((CBattle_GTournament*)pBattle);
#else
	delete pBattle;	
#endif
}
