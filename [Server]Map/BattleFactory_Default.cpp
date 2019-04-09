// BattleFactory_Default.cpp: implementation of the CBattleFactory_Default class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleFactory_Default.h"
#include "GridSystem.h"
#include "TileManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleFactory_Default::CBattleFactory_Default()
{

}

CBattleFactory_Default::~CBattleFactory_Default()
{

}

CBattle* CBattleFactory_Default::CreateBattle( BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum )
{
#ifdef _MAPSERVER_
	//±×¸®µå »ý¼º
	g_pServerSystem->GetGridSystem()->CreateGridTable(pCreateInfo->BattleID, eGridType_General, 512,512, MapNum);
	
	//Å¸ÀÏ »ý¼º
	if(g_pServerSystem->GetMap()->GetTileManager()->CreateTileGroup(pCreateInfo->BattleID, MapNum, 0, 0, 51200, 51200) == FALSE)
	{
		return NULL;
	}
	CBattle* pBattle = new CBattle;
	pBattle->Initialize( pCreateInfo, NULL, NULL );
	return pBattle;
#endif
	return NULL;
}

void CBattleFactory_Default::DeleteBattle( CBattle* pBattle )
{
#ifdef _MAPSERVER_
	g_pServerSystem->GetGridSystem()->DeleteGridTable(pBattle->GetBattleID());
	g_pServerSystem->GetMap()->GetTileManager()->DeleteTileGroup(pBattle->GetBattleID());
#endif
}
