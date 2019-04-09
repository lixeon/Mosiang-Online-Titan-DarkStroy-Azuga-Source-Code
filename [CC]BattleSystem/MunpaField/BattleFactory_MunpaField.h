// BattleFactory_MunpaField.h: interface for the CBattleFactory_MunpaField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLEFACTORY_MUNPAFIELD_H__7E4A90BC_2F34_4556_834F_D328FA5A3107__INCLUDED_)
#define AFX_BATTLEFACTORY_MUNPAFIELD_H__7E4A90BC_2F34_4556_834F_D328FA5A3107__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BattleFactory.h"
#include "Battle_MunpaField.h"

class CBattleFactory_MunpaField : public CBattleFactory  
{
public:
	CBattleFactory_MunpaField();
	virtual ~CBattleFactory_MunpaField();

	virtual CBattle* CreateBattle(BATTLE_INFO_BASE* pCreateInfo, MAPTYPE MapNum);	// ³»ºÎ¼ÂÆÃ¿¡ ÀÇÇÑ Create
	virtual void DeleteBattle(CBattle* pBattle);
	
};

#endif // !defined(AFX_BATTLEFACTORY_MUNPAFIELD_H__7E4A90BC_2F34_4556_834F_D328FA5A3107__INCLUDED_)
