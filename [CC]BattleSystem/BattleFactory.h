// BattleFactory.h: interface for the CBattleFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLEFACTORY_H__82B2B60A_9A86_47AF_BEEC_7420B7C57B13__INCLUDED_)
#define AFX_BATTLEFACTORY_H__82B2B60A_9A86_47AF_BEEC_7420B7C57B13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"

class CBattleFactory  
{
public:
	CBattleFactory();
	virtual ~CBattleFactory();

	// ´ÙÀ½µÑÁßÀÇ ÇÏ³ª´Â ¹Ýµå½Ã overriding ÇØ¾ßÇÔ
	virtual CBattle* CreateBattle(BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum);	// ³»ºÎ¼ÂÆÃ¿¡ ÀÇÇÑ Create
	virtual void DeleteBattle(CBattle* pBattle) {}
};

#endif // !defined(AFX_BATTLEFACTORY_H__82B2B60A_9A86_47AF_BEEC_7420B7C57B13__INCLUDED_)
