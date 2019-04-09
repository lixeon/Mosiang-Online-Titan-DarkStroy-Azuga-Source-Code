// BattleFactory_MunpaField.cpp: implementation of the CBattleFactory_MunpaField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleFactory_MunpaField.h"

#include "BattleTeam_MunpaField.h"

#ifdef _MAPSERVER_
#include "BattleSystem_server.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleFactory_MunpaField::CBattleFactory_MunpaField()
{

}

CBattleFactory_MunpaField::~CBattleFactory_MunpaField()
{

}

CBattle* CBattleFactory_MunpaField::CreateBattle(BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum)
{
	MSG_BATTLESETTING_SYN_MUNPAFIELD* Info = (MSG_BATTLESETTING_SYN_MUNPAFIELD*)pCreateInfo;
	
#ifdef _MAPSERVER_
	ASSERT(pCreateInfo->BattleKind == eBATTLE_KIND_MURIMFIELD)
	CBattleFactory::CreateBattle(pCreateInfo, MapNum); //±×¸®µå¸¦ ¸¸µç´Ù.
#endif
	// team1 setting
	CBattleTeam_MunpaField* pBattleTeam1 = new CBattleTeam_MunpaField;
	pBattleTeam1->Initialize(Info->BattleID,&Info->TeamInfo[eBattleTeam1]);

	// team2 setting
	CBattleTeam_MunpaField* pBattleTeam2 = new CBattleTeam_MunpaField;
	pBattleTeam2->Initialize(Info->BattleID,&Info->TeamInfo[eBattleTeam2]);

	// battle setting
	CBattle_MunpaField* pBattle = new CBattle_MunpaField;
	pBattle->Initialize(Info,pBattleTeam1,pBattleTeam2);
	
	return pBattle;	
}

void CBattleFactory_MunpaField::DeleteBattle(CBattle* pBattle)
{
#ifdef _MAPSERVER_
	ASSERT(pBattle->GetBattleKind() == eBATTLE_KIND_MURIMFIELD)
	CBattleFactory::DeleteBattle(pBattle); //±×¸®µå¸¦ Áö¿î´Ù.
#endif

	delete pBattle;
}
