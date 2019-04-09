#ifndef _BATTLEFACTORY_VIMUSTREET_H
#define _BATTLEFACTORY_VIMUSTREET_H

#include "BattleFactory.h"
#include "Battle_VimuStreet.h"

/*
struct BATTLE_INFO_BASE
{
	DWORD BattleID;		-->indexgenerate?
	BYTE BattleKind;	-->enum?
	BYTE BattleState;	-->enum?
	DWORD BattleTime;	-->Á¦ÇÑ½Ã°£?
};
*/

class CBattleFactory_VimuStreet : public CBattleFactory
{
public:
	CBattleFactory_VimuStreet();
	virtual ~CBattleFactory_VimuStreet();

	virtual CBattle* CreateBattle(BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum);
	virtual void DeleteBattle(CBattle* pBattle);
	

};




#endif