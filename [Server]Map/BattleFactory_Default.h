// BattleFactory_Default.h: interface for the CBattleFactory_Default class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLEFACTORY_DEFAULT_H__3BD8D860_0299_4B1D_865A_DA1CB202A83E__INCLUDED_)
#define AFX_BATTLEFACTORY_DEFAULT_H__3BD8D860_0299_4B1D_865A_DA1CB202A83E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BattleFactory.h"

class CBattleFactory_Default : public CBattleFactory  
{
public:
	CBattleFactory_Default();
	virtual ~CBattleFactory_Default();

	virtual CBattle*	CreateBattle( BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum );
	virtual void		DeleteBattle( CBattle* pBattle );
};

#endif // !defined(AFX_BATTLEFACTORY_DEFAULT_H__3BD8D860_0299_4B1D_865A_DA1CB202A83E__INCLUDED_)
