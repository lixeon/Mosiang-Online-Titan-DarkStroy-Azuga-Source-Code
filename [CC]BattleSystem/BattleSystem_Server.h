// BattleSystem.h: interface for the CBattleSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLESYSTEM_H__0AD4BA6C_48F8_43DE_993B_EB1A1BE59A9D__INCLUDED_)
#define AFX_BATTLESYSTEM_H__0AD4BA6C_48F8_43DE_993B_EB1A1BE59A9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BattleFactory.h"
#include "..\4DyuchiGXGFunc/global.h"

#define BATTLESYSTEM	USINGTON(CBattleSystem)

class CBattleSystem
{
	CYHHashTable<CBattle> m_BattleTable;
	cPtrList	m_LoginBattleList;

	CBattleFactory* m_BattleFactory[eBATTLE_KIND_MAX];	
	CBattle m_DefaultBattle;

	
	
	INDEXCR_HANDLE		m_ICHandle;
	
public:
//	MAKESINGLETON(CBattleSystem);
	CBattleSystem();
	virtual ~CBattleSystem();

	
	void CreateDefaultBattle();
	DWORD CreateChannel();
	void DestroyChannel( DWORD dwChannel );
	BOOL CreateBattle(BATTLE_INFO_BASE* pInfo, MAPTYPE MapNum);
	void DeleteBattle(CBattle* pBattle);
	
	CBattle* CheckLoginBattle(CObject* pObject);
	BOOL AddObjectToBattle(CBattle* pBattle,CObject* pObject);
	BOOL DeleteObjectFromBattle(CObject* pObject);
	
	CBattle* GetBattle(CObject* pObject);
	CBattle* GetBattle(DWORD BattleID);
	
	DWORD MakeNewBattleID();
	void ReleaseBattleID(DWORD BattleID);
	void Release();

	void Process();
	
	virtual void NetworkMsgParse(BYTE protocol,MSGBASE* pmsg);
};
EXTERNGLOBALTON(CBattleSystem);
#endif // !defined(AFX_BATTLESYSTEM_H__0AD4BA6C_48F8_43DE_993B_EB1A1BE59A9D__INCLUDED_)
