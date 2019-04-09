// ObjectFactory.cpp: implementation of the CObjectFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "ObjectFactory.h"
#include "Player.h"
#include "Titan.h"
#include "Pet.h"
#include "BossMonster.h"
#include "Npc.h"
#include "TacticObject.h"
#include "MapObject.h"
#include "MemoryChecker.h"
// 필드보스 - 05.12 이영준
#include "FieldBossMonster.h"
#include "FieldSubMonster.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CObjectFactory * g_pObjectFactory=NULL;

CObjectFactory::CObjectFactory()
{
	PlayerPool = NULL;
	TitanPool	= NULL;
	TitanInfoPool = NULL;
	PetPool		= NULL;
	MonsterPool = NULL;
	NpcPool = NULL;
	TacticPool = NULL;
	BossMonsterPool = NULL;
	MapObjectPool = NULL;

	// 필드보스 - 05.12 이영준
	FieldBossMonsterPool = NULL;
	FieldSubMonsterPool = NULL;
}

CObjectFactory::~CObjectFactory()
{
	Release();
}

void CObjectFactory::Init()
{	
	PlayerPool = new	CMemoryPoolTempl<CPlayer>;
	PetPool		= new	CMemoryPoolTempl<CPet>;
	MonsterPool = new	CMemoryPoolTempl<CMonster>;
	NpcPool = new		CMemoryPoolTempl<CNpc>;
	TacticPool = new	CMemoryPoolTempl<CTacticObject>;
	BossMonsterPool = new 	CMemoryPoolTempl<CBossMonster>;
	MapObjectPool = new CMemoryPoolTempl<CMapObject>;

	TitanPool	= new	CMemoryPoolTempl<CTitan>;
	TitanInfoPool	= new CMemoryPoolTempl<TITAN_TOTALINFO>;

	// 필드보스 - 05.12 이영준
	FieldBossMonsterPool = new CMemoryPoolTempl<CFieldBossMonster>;
	FieldSubMonsterPool = new CMemoryPoolTempl<CFieldSubMonster>;

	PlayerPool->Init(MAX_TOTAL_PLAYER_NUM,100,"PlayerPool");
	PetPool->Init(MAX_TOTAL_PET_NUM,100,"PetPool");
	MonsterPool->Init(MAX_TOTAL_MONSTER_NUM,100,"MonsterPool");
	BossMonsterPool->Init(MAX_TOTAL_BOSSMONSTER_NUM, 5, "BossMonsterPool");

	// 필드보스 - 05.12 이영준
	FieldBossMonsterPool->Init(MAX_TOTAL_BOSSMONSTER_NUM, 5, "FieldBossMonsterPool");
	FieldSubMonsterPool->Init(MAX_TOTAL_BOSSMONSTER_NUM * 10, 5, "FieldSubMonsterPool");
	
	NpcPool->Init(MAX_TOTAL_NPC_NUM,10,"NpcPool");
	TacticPool->Init(MAX_TOTAL_TACTIC_NUM,10,"TacticPool");
	MapObjectPool->Init( MAX_MAPOBJECT_NUM, 10, "MapObjectPool" );

	TitanPool->Init(MAX_TOTAL_TITAN_NUM,100,"TitanPool");
	TitanInfoPool->Init(MAX_TITANINFO_NUM,100,"TitanInfoPool");
}

void CObjectFactory::Release()
{
	SAFE_DELETE(PlayerPool);
	SAFE_DELETE(TitanPool);
	SAFE_DELETE(TitanInfoPool);
	SAFE_DELETE(PetPool);
	SAFE_DELETE(MonsterPool);
	SAFE_DELETE(NpcPool);
	SAFE_DELETE(TacticPool);
	SAFE_DELETE(BossMonsterPool);
	
	// 필드보스 - 05.12 이영준
	SAFE_DELETE(FieldBossMonsterPool);
	SAFE_DELETE(FieldSubMonsterPool);

	SAFE_DELETE(MapObjectPool);
}

CObject* CObjectFactory::MakeNewObject(EObjectKind Kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	CObject* pObject;
	switch(Kind)
	{
	case eObjectKind_Player:
		// 객체를 생성해서 리턴시켜준다
		pObject = (CObject*)PlayerPool->Alloc();
		break;

	case eObjectKind_Titan:
		pObject = (CObject*)TitanPool->Alloc();
		break;

	case eObjectKind_Pet:
		pObject = (CObject*)PetPool->Alloc();
		break;


    case eObjectKind_ToghterPlayMonster:
	case eObjectKind_SpecialMonster:
	case eObjectKind_Monster:
	case eObjectKind_Mining:
	case eObjectKind_Collection:		
	case eObjectKind_Hunt:
	case eObjectKind_TitanMonster:
		{
			// 객체를 생성해서 리턴시켜준다
			pObject = (CObject*)MonsterPool->Alloc();
/*			
			// Memory Check
			stMemoryInfo* pMemoryInfo = MEMORYCHK->GetMemoryInfo( eObjectFactory_Monster );
			if( pMemoryInfo )
				pMemoryInfo->Increase( sizeof(CMonster) );
			//
*/
		}
		break;

	case eObjectKind_Npc:
		// 객체를 생성해서 리턴시켜준다
		pObject = (CObject*)NpcPool->Alloc();
		break;

	case eObjectKind_Tactic:
		// 객체를 생성해서 리턴시켜준다
		pObject = (CObject*)TacticPool->Alloc();
		break;

	case eObjectKind_BossMonster:
		// 객체를 생성해서 리턴시켜준다
		pObject = (CObject*)BossMonsterPool->Alloc();
		break;	
	
	// 필드보스 - 05.12 이영준
	case eObjectKind_FieldBossMonster:
		pObject = (CObject*)FieldBossMonsterPool->Alloc();
		break;	
	case eObjectKind_FieldSubMonster:
		pObject = (CObject*)FieldSubMonsterPool->Alloc();
		break;

	case eObjectKind_MapObject:
	case eObjectKind_CastleGate:
		pObject = (CObject*)MapObjectPool->Alloc();
		break;	
	}
	
	ASSERT(pObject);
	
	if(pObject->Init(Kind,AgentNum,pBaseObjectInfo) == FALSE)
	{
		ReleaseObject(pObject);
		return FALSE;
	}
	
	return pObject;
}

void CObjectFactory::ReleaseObject(CObject* pObject)
{
	BYTE Kind = pObject->GetObjectKind();
	pObject->Release();
	switch(Kind)
	{
	case eObjectKind_Player:
		PlayerPool->Free((CPlayer*)pObject);
		break;

	case eObjectKind_Titan:
		TitanPool->Free((CTitan*)pObject);
		break;

	case eObjectKind_Pet:
		PetPool->Free((CPet*)pObject);
		break;

	case eObjectKind_ToghterPlayMonster:
	case eObjectKind_SpecialMonster:
	case eObjectKind_Monster:
		{
			MonsterPool->Free((CMonster*)pObject);
/*
			// Memory Check
			stMemoryInfo* pMemoryInfo = MEMORYCHK->GetMemoryInfo( eObjectFactory_Monster );
			if( pMemoryInfo )
				pMemoryInfo->Decrease( sizeof(CMonster) );
			//
*/
		}
		
		break;

	case eObjectKind_Npc:
		NpcPool->Free((CNpc*)pObject);
		break;

	case eObjectKind_Tactic:
		TacticPool->Free((CTacticObject*)pObject);
		break;

	case eObjectKind_BossMonster:
		BossMonsterPool->Free((CBossMonster*)pObject);
		break;
	
	// 필드보스 - 05.12 이영준
	case eObjectKind_FieldBossMonster:
		FieldBossMonsterPool->Free((CFieldBossMonster*)pObject);
		break;
	case eObjectKind_FieldSubMonster:
		FieldSubMonsterPool->Free((CFieldSubMonster*)pObject);
		break;
		
	case eObjectKind_MapObject:
	case eObjectKind_CastleGate:
		MapObjectPool->Free((CMapObject*)pObject);
		break;
	case eObjectKind_Mining:		
		MonsterPool->Free((CMonster*)pObject);
		break;
	case eObjectKind_Collection:		
		MonsterPool->Free((CMonster*)pObject);
		break;
	case eObjectKind_Hunt:		
		MonsterPool->Free((CMonster*)pObject);
		break;
	case eObjectKind_TitanMonster:
		MonsterPool->Free((CMonster*)pObject);
		break;
	}
}

TITAN_TOTALINFO* CObjectFactory::MakeTitanInfo( TITAN_TOTALINFO* titanInfo )
{
	TITAN_TOTALINFO* pInfo = (TITAN_TOTALINFO*)TitanInfoPool->Alloc();

	memcpy(pInfo, titanInfo, sizeof(TITAN_TOTALINFO));

	return pInfo;
}

void CObjectFactory::ReleaseTitanInfo( TITAN_TOTALINFO* titanInfo )
{
	TitanInfoPool->Free((TITAN_TOTALINFO*)titanInfo);
}

