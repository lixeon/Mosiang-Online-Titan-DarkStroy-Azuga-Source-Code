// BattleSystem.cpp: implementation of the CBattleSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _MAPSERVER_

#include "BattleSystem_Server.h"
#include "VimuStreet/BattleFactory_VimuStreet.h"
#include "MurimField/BattleFactory_MurimField.h"
#include "MunpaField/BattleFactory_MunpaField.h"
#include "BattleFactory_Default.h"
#include "Suryun/BattleFactory_Suryun.h"
#include "..\[CC]BattleSystem\GTournament\BattleFactory_GTournament.h"
#include "..\[CC]BattleSystem\SiegeWar\BattleFactory_SiegeWar.h"

#include "UserTable.h"
#include "Player.h"
#include "PackedData.h"
#include "..\[CC]Header\GameResourceManager.h"

#include "CharMove.h"

#include "VimuManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CBattleSystem);

CBattleSystem::CBattleSystem()
{
	//m_BattleTable.Initialize(32);
	m_BattleTable.Initialize(128); //MAXÄ¡ ¼³Á¤ÇÊ¿ä

	m_BattleFactory[eBATTLE_KIND_NONE] = new CBattleFactory_Default;
	m_BattleFactory[eBATTLE_KIND_VIMUSTREET] = new CBattleFactory_VimuStreet;
	m_BattleFactory[eBATTLE_KIND_MURIMFIELD] = new CBattleFactory_MurimField;
	m_BattleFactory[eBATTLE_KIND_MUNPAFIELD] = new CBattleFactory_MunpaField;
	m_BattleFactory[eBATTLE_KIND_SURYUN] = new CBattleFactory_Suryun;
	m_BattleFactory[eBATTLE_KIND_GTOURNAMENT] = new CBattleFactory_GTournament;
	m_BattleFactory[eBATTLE_KIND_SIEGEWAR] = new CBattleFactory_SiegeWar;
	
	m_ICHandle = ICCreate();
	ICInitialize(m_ICHandle,BATTLE_ID_MAX);
}

CBattleSystem::~CBattleSystem()
{

}

void CBattleSystem::Release()
{
	m_BattleTable.SetPositionHead();
	CBattle* pBattle;
	while(pBattle = m_BattleTable.GetData())
	{
		ASSERT(pBattle->GetBattleKind() < eBATTLE_KIND_MAX);
		m_BattleFactory[pBattle->GetBattleKind()]->DeleteBattle(pBattle);
	}
	m_BattleTable.RemoveAll();

	// magi82(14) - Memory(071027) 메모리 누수 수정
	PTRLISTPOS pos = m_LoginBattleList.GetHeadPosition();
	while( pos )
	{
		pBattle = (CBattle*)m_LoginBattleList.GetNext( pos );
		SAFE_DELETE(pBattle);
	}
	m_LoginBattleList.RemoveAll();

	for(int n=0;n<eBATTLE_KIND_MAX;++n)
	{
		if(m_BattleFactory[n])
			delete m_BattleFactory[n];
	}

	ICRelease(m_ICHandle);
}

void CBattleSystem::CreateDefaultBattle()
{
	BATTLE_INFO_BASE *pCreateInfo = new BATTLE_INFO_BASE;
	memset(pCreateInfo, 0, sizeof(BATTLE_INFO_BASE));
	pCreateInfo->BattleKind = eBATTLE_KIND_NONE;
	CBattle* pBattle = m_BattleFactory[eBATTLE_KIND_NONE]->CreateBattle(pCreateInfo, GAMERESRCMNGR->GetLoadMapNum());
	if(pBattle)
	{
		m_LoginBattleList.AddTail(pBattle);
	}
}


DWORD CBattleSystem::CreateChannel()
{
	// magi82(14) - Memory(071027) 메모리 누수 수정
	BATTLE_INFO_BASE CreateInfo;
	memset(&CreateInfo, 0, sizeof(BATTLE_INFO_BASE));
	CreateInfo.BattleKind = eBATTLE_KIND_NONE;
    CreateInfo.BattleID = MakeNewBattleID();
	CBattle* pBattle = m_BattleFactory[eBATTLE_KIND_NONE]->CreateBattle(&CreateInfo, GAMERESRCMNGR->GetLoadMapNum());

/*	BATTLE_INFO_BASE *pCreateInfo = new BATTLE_INFO_BASE;
	memset(pCreateInfo, 0, sizeof(BATTLE_INFO_BASE));
	pCreateInfo->BattleKind = eBATTLE_KIND_NONE;
	pCreateInfo->BattleID = MakeNewBattleID();
	CBattle* pBattle = m_BattleFactory[eBATTLE_KIND_NONE]->CreateBattle(pCreateInfo, GAMERESRCMNGR->GetLoadMapNum());
*/
	ASSERT(pBattle);
	m_BattleTable.Add(pBattle,pBattle->GetBattleID());
	
	if(pBattle)
	{
		m_LoginBattleList.AddTail(pBattle);
		return CreateInfo.BattleID;
	}
	return 0;
}

void CBattleSystem::DestroyChannel( DWORD dwChannel )
{
	CBattle* pBattle = m_BattleTable.GetData( dwChannel );
	if( !pBattle )	return;

	m_LoginBattleList.Remove( pBattle );
	m_BattleTable.Remove( dwChannel );
	m_BattleFactory[pBattle->GetBattleKind()]->DeleteBattle( pBattle );
}

DWORD CBattleSystem::MakeNewBattleID()
{
	return ICAllocIndex(m_ICHandle);
}
void CBattleSystem::ReleaseBattleID(DWORD BattleID)
{
	ICFreeIndex(m_ICHandle,BattleID);
}

BOOL CBattleSystem::CreateBattle(BATTLE_INFO_BASE* pInfo, MAPTYPE MapNum)
{
	ASSERT(pInfo->BattleKind < eBATTLE_KIND_MAX);
	
	CBattle* pBattle = m_BattleFactory[pInfo->BattleKind]->CreateBattle(pInfo, MapNum);
	ASSERT(pBattle);
	if(pBattle == NULL)
		return FALSE;

	m_BattleTable.Add(pBattle,pBattle->GetBattleID());

	if(pBattle->GetBattleFlag() & BATTLE_FLAG_LOGINBATTLE)
	{
		m_LoginBattleList.AddTail(pBattle);
	}

	return TRUE;
}

void CBattleSystem::DeleteBattle(CBattle* pBattle)
{
	m_LoginBattleList.Remove(pBattle);
	pBattle->OnDestroy();
	m_BattleTable.Remove(pBattle->GetBattleID());
	m_BattleFactory[pBattle->GetBattleKind()]->DeleteBattle(pBattle);
}

CBattle* CBattleSystem::CheckLoginBattle(CObject* pObject)
{
	BOOL rt;
	PTRLISTSEARCHSTART(m_LoginBattleList,CBattle*,pBattle)
		if(pBattle->GetBattleID() == pObject->GetBattleID())
		{
			rt = pBattle->IsAddableBattle(pObject);
			if(rt == TRUE)
				return pBattle;
		}
	PTRLISTSEARCHEND

	return NULL;
}

BOOL CBattleSystem::AddObjectToBattle(CBattle* pBattle, CObject* pObject)
{
	return pBattle->AddObjectToBattle(pObject);
}

BOOL CBattleSystem::DeleteObjectFromBattle(CObject* pObject)
{
	CBattle* pBattle = GetBattle(pObject);
	if(pBattle == NULL)
		return FALSE;

	if(pBattle->GetBattleKind() == eBATTLE_KIND_NONE) 
		return TRUE;
	return pBattle->DeleteObjectFromBattle(pObject);
}

CBattle* CBattleSystem::GetBattle(DWORD BattleID)
{
	if(BattleID == 0)
		return &m_DefaultBattle;

	CBattle* pBattle = m_BattleTable.GetData(BattleID);
	ASSERT(pBattle);
	
	return pBattle;
}

CBattle* CBattleSystem::GetBattle(CObject* pObject)
{
	return GetBattle(pObject->GetBattleID());
}


void CBattleSystem::Process()
{
	CBattle* pBattle;
	m_BattleTable.SetPositionHead();
	
	cPtrList list;

	while(pBattle = m_BattleTable.GetData())
	{
		if(pBattle->IsDestroyed())
		{
//			DWORD battleID = pBattle->GetBattleID();
//			DeleteBattle(pBattle);
//			m_BattleTable.Remove(battleID);
			list.AddTail(pBattle);
		}
		else
			pBattle->OnTick();
	}
	
	PTRLISTPOS pos = list.GetHeadPosition();
	while( pos )
	{
		CBattle* p = (CBattle*)list.GetNext( pos );
		DWORD battleID = p->GetBattleID();
		DeleteBattle(p);
		m_BattleTable.Remove(battleID);
	}
	list.RemoveAll();
}

void CBattleSystem::NetworkMsgParse(BYTE protocol,MSGBASE* pMsg)
{
	VIMUMGR->NetworkMsgParse( protocol, pMsg ); //Ã³¸®µÈ ¸Þ¼¼Áö´Â ¾Æ·¡·Î ³»·Á°¥ ÇÊ¿ä¾øµµ·Ï.

	switch(protocol)
	{
	case MP_BATTLE_CHAT_TEAM_SYN:
		{

		}
		break;
	case MP_BATTLE_CHAT_MASTER_SYN:
		{

		}
		break;
	case MP_BATTLE_VIMU_REQUEST_SYN:
		{
		}
		break;

	}
}

#endif
