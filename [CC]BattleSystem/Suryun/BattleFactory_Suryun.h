// BattleFactory_Suryun.h: interface for the CBattleFactory_Suryun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLEFACTORY_SURYUN_H__48CF0168_A395_4B60_B8CA_CD56E43C8A36__INCLUDED_)
#define AFX_BATTLEFACTORY_SURYUN_H__48CF0168_A395_4B60_B8CA_CD56E43C8A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\BattleFactory.h"
#include "Battle_Suryun.h"

class CBattleFactory_Suryun : public CBattleFactory  
{
#ifdef _MAPSERVER_
	CMemoryPoolTempl<CBattle_Suryun> m_BattlePool;
#endif
	
public:
	CBattleFactory_Suryun();
	virtual ~CBattleFactory_Suryun();

	virtual CBattle*	CreateBattle( BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum );
	virtual void		DeleteBattle( CBattle* pBattle );
};

#endif // !defined(AFX_BATTLEFACTORY_SURYUN_H__48CF0168_A395_4B60_B8CA_CD56E43C8A36__INCLUDED_)
