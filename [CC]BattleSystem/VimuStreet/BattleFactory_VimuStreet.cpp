#include "stdafx.h"
#include "BattleFactory_VimuStreet.h"
#include "BattleTeam_VimuStreet.h"

#ifdef _MAPSERVER_
#include "BattleSystem_server.h"
#endif


CBattleFactory_VimuStreet::CBattleFactory_VimuStreet()
{

}

CBattleFactory_VimuStreet::~CBattleFactory_VimuStreet()
{

}


CBattle* CBattleFactory_VimuStreet::CreateBattle(BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum)
{	
	BATTLE_INFO_VIMUSTREET* pInfo = (BATTLE_INFO_VIMUSTREET*)pCreateInfo;

#ifdef _MAPSERVER_
	pInfo->BattleID = BATTLESYSTEM->MakeNewBattleID();
#endif

	// battle setting
	CBattle_VimuStreet* pBattle = new CBattle_VimuStreet;
	pBattle->Initialize(pCreateInfo,NULL,NULL);
	
	return pBattle;
}

void CBattleFactory_VimuStreet::DeleteBattle(CBattle* pBattle)
{
#ifdef _MAPSERVER_
	BATTLESYSTEM->ReleaseBattleID(pBattle->GetBattleID());
#endif

	delete pBattle;	
}
