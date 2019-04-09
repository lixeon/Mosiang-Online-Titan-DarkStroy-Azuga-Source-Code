// CObjectActionManager.cpp: implementation of the CObjectActionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectActionManager.h"
#include "ObjectStateManager.h"
#include "Object.h"
#include "MoveManager.h"
#include "ObjectManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CObjectActionManager);
CObjectActionManager::CObjectActionManager()
{

}

CObjectActionManager::~CObjectActionManager()
{

}

void CObjectActionManager::Die(CObject* pObject,CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive)
{
	if(MOVEMGR->IsMoving(pObject)==TRUE)
		MOVEMGR->MoveStop(pObject);
	if(pObject->GetState() != eObjectState_Die)
	{
		if(pObject->GetState() != eObjectState_None)
			OBJECTSTATEMGR->EndObjectState(pObject,pObject->GetState());

		OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_Die);
		pObject->Die(pAttacker,bFatalDamage,bCritical, bDecisive);
	}
//	else
//	{
//		int a=0;
//	}
}

void CObjectActionManager::Damage(CObject* pObject,CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage)
{
	pObject->Damage(pAttacker,DamageKind,Damage,ShieldDamage,bCritical, bDecisive, titanObserbDamage);
}

void CObjectActionManager::Dodge(CObject* pObject,CObject* pAttacker)
{
	if( pObject == HERO || pAttacker == HERO )
		EFFECTMGR->AddDamageNumberDodge(pObject);
}

void CObjectActionManager::Revive(CObject* pObject,VECTOR3* pRevivePos)
{
	OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_Die);
	pObject->Revive(pRevivePos);
}
	
void CObjectActionManager::Heal(CObject* pObject,CObject* pHealer,BYTE DamageKind,DWORD HealVal)
{
	pObject->Heal(pHealer,DamageKind,HealVal);
}
void CObjectActionManager::Recharge(CObject* pObject,CObject* pRecharger,BYTE RechargeKind,DWORD RechargeVal)
{
	pObject->Recharge(pRecharger,RechargeKind,RechargeVal);
}
void CObjectActionManager::ApplyTargetList(CObject* pOperator,CTargetList* pTList,BYTE DamageKind)
{
	CObject* pTarget;
	RESULTINFO rinfo;
	CTargetListIterator iter(pTList);
	while(iter.GetNextTarget())
	{
		pTarget = OBJECTMGR->GetObject(iter.GetTargetID());
		iter.GetTargetData(&rinfo);
		if(pTarget == NULL)
			continue;
		OBJECTACTIONMGR->ApplyResult(pTarget,pOperator,&rinfo,DamageKind);
	}
	iter.Release();
}
void CObjectActionManager::ApplyResult(CObject* pObject,CObject* pOperator,RESULTINFO* pRInfo,BYTE DamageKind)
{
	if(pRInfo->RealDamage || pRInfo->ShieldDamage)
	{
		Damage(pObject,pOperator,DamageKind,pRInfo->RealDamage,pRInfo->ShieldDamage,pRInfo->bCritical,pRInfo->bDecisive,pRInfo->TitanObsorbDamage);
		if(pObject->IsDied())
			Die(pObject,pOperator,FALSE,pRInfo->bCritical,pRInfo->bDecisive);
	}
	if(pRInfo->CounterDamage)
	{
		Damage(pOperator,pObject,eDamageKind_Counter,pRInfo->CounterDamage,0,0,0,pRInfo->TitanObsorbDamage);
		if(pOperator->IsDied())
			Die(pOperator,pObject,FALSE,FALSE,FALSE);
	}
	if(pRInfo->HealLife)
		Heal(pObject,pOperator,0,pRInfo->HealLife);
	if(pRInfo->RechargeNaeryuk)
		Recharge(pObject,pOperator,0,pRInfo->RechargeNaeryuk);	
	
	if(pRInfo->StunTime)
		pObject->StartSpecialState(eSpecialState_Stun,pRInfo->StunTime,0,0,0,FindEffectNum("monster_stun_s.beff"),0);
	
	
	if(pRInfo->Vampiric_Life)
	{
		if(pOperator->GetID() == HERO->GetID())
			HERO->ChangeLife(pRInfo->Vampiric_Life);
		else
			pOperator->SetLife(pOperator->GetLife()+pRInfo->Vampiric_Life);
	}
	
	if(pRInfo->Vampiric_Naeryuk)
	{
		pOperator->SetNaeRyuk(pOperator->GetNaeRyuk()+pRInfo->Vampiric_Naeryuk);
	}
}