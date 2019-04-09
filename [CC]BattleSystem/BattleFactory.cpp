// BattleFactory.cpp: implementation of the CBattleFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleFactory.h"

#ifdef _MAPSERVER_
//#include "GridSystem.h"
#include "..\[CC]Header\GameResourceManager.h"
//#include "TileManager.h"
#include ".\MurimField\BattleTeam_MurimField.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleFactory::CBattleFactory()
{
	
}

CBattleFactory::~CBattleFactory()
{

}

CBattle* CBattleFactory::CreateBattle(BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum)
{
	return NULL;
}
