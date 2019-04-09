// MapObject.cpp: implementation of the CMapObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapObject.h"
#include "OptionManager.h"
#include "ObjectManager.h"
#include "PartyManager.h"
#include "GameIn.h"
#include "MonsterGuageDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapObject::CMapObject()
{

}

CMapObject::~CMapObject()
{

}

void CMapObject::InitMapObject( MAPOBJECT_INFO* pInitInfo )
{
	memcpy( &m_MapObjectInfo, pInitInfo, sizeof(MAPOBJECT_INFO) );
}


void CMapObject::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive)
{
	SetLife(0);
	
	CObject::Die(pAttacker,bFatalDamage,bCritical,bDecisive);
}

void CMapObject::Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage)
{	
	if( pAttacker )
	{
		if( OPTIONMGR->IsShowMemberDamage() )
		{
			if( pAttacker == HERO || PARTYMGR->IsPartyMember( pAttacker->GetID() ) )
				EFFECTMGR->AddDamageNumber(Damage,pAttacker,this,DamageKind,bCritical,bDecisive);
		}
		else
		{
			if( pAttacker == HERO )
				EFFECTMGR->AddDamageNumber(Damage,pAttacker,this,DamageKind,bCritical,bDecisive);
		}

	}
	
	// Life 実特
	DWORD life = GetLife();
	if(life < Damage)
		life = 0;
	else
		life = life - Damage;
	SetLife(life);
	
	// Shield 実特
	DWORD Shield = GetShield();
	if(Shield < ShieldDamage)
		Shield = 0;
	else
		Shield = Shield - ShieldDamage;
	SetShield(Shield,FALSE);	
}


void CMapObject::SetLife(DWORD val, BYTE type)
{
	m_MapObjectInfo.Life = val;

	if( GAMEIN->GetMonsterGuageDlg() )
	{
		if( GAMEIN->GetMonsterGuageDlg()->GetObjectType() & eObjectKind_MapObject )
			GAMEIN->GetMonsterGuageDlg()->SetMonsterLife( val, m_MapObjectInfo.MaxLife );
	}
}


void CMapObject::SetShield(DWORD val, BYTE type)
{
	m_MapObjectInfo.Shield = val;

	if( GAMEIN->GetMonsterGuageDlg() )
	{
		if( GAMEIN->GetMonsterGuageDlg()->GetObjectType() & eObjectKind_MapObject )
			GAMEIN->GetMonsterGuageDlg()->SetMonsterShield( val, m_MapObjectInfo.MaxShield );
	}
}