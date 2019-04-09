#include "stdafx.h"
#include "BattleFactory_MurimField.h"
#include "BattleTeam_MurimField.h"
#include "Battle_MurimField.h"

#ifdef _MAPSERVER_
#include "../BattleSystem_server.h"
#endif


CBattleFactory_MurimField::CBattleFactory_MurimField()
{
}

CBattleFactory_MurimField::~CBattleFactory_MurimField()
{
}

CBattle* CBattleFactory_MurimField::CreateBattle( BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum )
{
	MSG_BATTLESETTING_SYN_MURIMFIELD* pInfo = (MSG_BATTLESETTING_SYN_MURIMFIELD*)pCreateInfo;

#ifdef _MAPSERVER_
	ASSERT( pCreateInfo->BattleKind == eBATTLE_KIND_MURIMFIELD )
	CBattleFactory::CreateBattle( pCreateInfo, MapNum );				//±×¸®µå¸¦ ¸¸µç´Ù.
#endif

	// team1 setting
	CBattleTeam_MurimField* pBattleTeam1 = new CBattleTeam_MurimField;
	pBattleTeam1->Initialize( pInfo->BattleID, &pInfo->TeamInfo[eBattleTeam1] );

	// team2 setting
	CBattleTeam_MurimField* pBattleTeam2 = new CBattleTeam_MurimField;
	pBattleTeam2->Initialize( pInfo->BattleID, &pInfo->TeamInfo[eBattleTeam2] );

	// battle setting
	CBattle_MurimField* pBattle = new CBattle_MurimField;
	pBattle->Initialize( pInfo, pBattleTeam1, pBattleTeam2 );
	
	return pBattle;	
}

void CBattleFactory_MurimField::DeleteBattle( CBattle* pBattle )
{
#ifdef _MAPSERVER_
	ASSERT( pBattle->GetBattleKind() == eBATTLE_KIND_MURIMFIELD )
	CBattleFactory::DeleteBattle( pBattle );					//±×¸®µå¸¦ Áö¿î´Ù.
#endif

	delete pBattle;
}
