// BattleSystem.h: interface for the CBattleSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLESYSTEM_H__7473B4EA_913D_4B3D_9399_9328FBBEDDF1__INCLUDED_)
#define AFX_BATTLESYSTEM_H__7473B4EA_913D_4B3D_9399_9328FBBEDDF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"

class CBattleFactory;

class CHero;

#define BATTLESYSTEM	CBattleSystem::GetInstance()

class CBattleSystem  
{
	CBattle m_DefaultBattle;
	CBattle* m_pCurBattle;
	
	CBattleFactory* m_BattleFactory[eBATTLE_KIND_MAX];	
	
	CBattleSystem();
public:
	MAKESINGLETON(CBattleSystem);
	virtual ~CBattleSystem();

	void Release();
	void ReleaseBattle();

	BOOL CreateBattle(BATTLE_INFO_BASE* pInfo);
	void DeleteBattle(CBattle* pBattle);

	CBattle* GetBattle(CHero* pHero);
	CBattle* GetBattle(DWORD BattleID);

	void Render();
	
	//
	void SetSiegeWarBattleInfo( SEND_SW_BTGUILDLIST* pInfo );

	void NetworkMsgParse(BYTE Protocol,void* pMsg);
};

#endif // !defined(AFX_BATTLESYSTEM_H__7473B4EA_913D_4B3D_9399_9328FBBEDDF1__INCLUDED_)
