#ifndef _BATTLEFACTORY_MURIMFILED_H
#define _BATTLEFACTORY_MURIMFILED_H

#include "BattleFactory.h"


class CBattleFactory_MurimField : public CBattleFactory
{
public:
	CBattleFactory_MurimField();
	virtual ~CBattleFactory_MurimField();

public:	// virtual func
	virtual CBattle*	CreateBattle( BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum );
	virtual void		DeleteBattle( CBattle* pBattle );

};


#endif