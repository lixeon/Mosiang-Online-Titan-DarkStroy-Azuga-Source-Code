// BossMonster.cpp: implementation of the CBossMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BossMonster.h"
#include "GameResourceManager.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "PartyManager.h"
#include "OptionManager.h"

#ifdef _TESTCLIENT_
#include "MHFile.h"
//#include "BossMonsterInfo.h"
#include "MoveManager.h"
#include "SkillManager_client.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBossMonster::CBossMonster()
{

}

CBossMonster::~CBossMonster()
{

}

void CBossMonster::Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage)
{	
//	CheckTmpFunc();
	
	if(Damage != 0)
	{
	/*
		if(GetState() == eObjectState_None)
			{
				//if(pAttacker)
				//	MOVEMGR->SetLookatPos(this,&pAttacker->GetCurPosition(),0,gCurTime);
			}
			*/
/*	
		if(GetState() == eObjectState_None)
		{
			ChangeMotion(1,FALSE);
			switch(DamageKind) {
			case eDamageKind_Front:
				ChangeMotion(eMonsterMotion_DamageFront,FALSE);
				break;
			case eDamageKind_Left:
				ChangeMotion(eMonsterMotion_DamageLeft,FALSE);
				break;
			case eDamageKind_Right:
				ChangeMotion(eMonsterMotion_DamageRight,FALSE);
				break;
			}	
		}
*/		
		int DamageEffect = -1;
		//no damage ani..
/*
		switch(DamageKind) {
		case eDamageKind_Front:
		case eDamageKind_Counter:
		case eDamageKind_ContinueDamage:
			DamageEffect = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind())->Damage1DramaNum;
			break;
		case eDamageKind_Left:
			DamageEffect = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind())->Damage1DramaNum;
			break;
		case eDamageKind_Right:
			DamageEffect = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind())->Damage1DramaNum;
			break;
		}*/

		if(DamageEffect != -1)
		{
			TARGETSET set;
			set.pTarget = this;
			set.ResultInfo.bCritical = bCritical;
			set.ResultInfo.bDecisive = bDecisive;
			
			EFFECTMGR->StartEffectProcess(DamageEffect,pAttacker,&set,1,GetID());
		}
	}

//	if( pAttacker )
//	if( pAttacker == HERO || PARTYMGR->IsPartyMember( pAttacker->GetID() ) )	//자신의 데미지 번호만 표시 //KES 040801
//		EFFECTMGR->AddDamageNumber(Damage,pAttacker,this,DamageKind,bCritical);

	//SW050804 옵션창 파티데미지 보기 추가
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
	
	// Life 셋팅
	DWORD life = GetLife();
	if(life < Damage)
		life = 0;
	else
		life = life - Damage;
	SetLife(life);
	
	// Shield 셋팅
	DWORD Shield = GetShield();
	if(Shield < ShieldDamage)
		Shield = 0;
	else
		Shield = Shield - ShieldDamage;
	SetShield(Shield,FALSE);	
}

void CBossMonster::SetMotionInState(BYTE State)
{
	switch(State)
	{
	case eObjectState_Move:
		{
			m_ObjectState.State_Start_Motion = eBossMonsterMotion_Walk;
			m_ObjectState.State_Ing_Motion = eBossMonsterMotion_Walk;
			m_ObjectState.State_End_Motion = eBossMonsterMotion_Walk;
			m_ObjectState.State_End_MotionTime = 0;
		}
		break;
	case eObjectState_None:
		{
			m_ObjectState.State_Start_Motion = eBossMonsterMotion_Stand;
			m_ObjectState.State_Ing_Motion = eBossMonsterMotion_Stand;
			m_ObjectState.State_End_Motion = eBossMonsterMotion_Stand;
			m_ObjectState.State_End_MotionTime = 0;
		}
		break;
	case eObjectState_Rest:
		{
			m_ObjectState.State_Start_Motion = -1;
			m_ObjectState.State_Ing_Motion = -1;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = 0;
		}
		break;
	default:
		{
			m_ObjectState.State_Start_Motion = -1;
			m_ObjectState.State_Ing_Motion = -1;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = 0;
		}
		break;
	}
}

void CBossMonster::AddStatus(CStatus* pStatus)
{
	//boss block status
}
