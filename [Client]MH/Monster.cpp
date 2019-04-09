// Monster.cpp: implementation of the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "MHCamera.h"
#include "MoveManager.h"
#include ".\interface\cWindowManager.h"
#include "ObjectGuagen.h"
#include "ObjectManager.h"
#include "GameResourceManager.h"
#include "MOTIONDESC.h"
#include "GameIn.h"
#include "ObjectStateManager.h"

#include "MonsterGuageDlg.h"
#include "PartyManager.h"

#include "OptionManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonster::CMonster()
{
	m_pSInfo = NULL;

	memset(&m_DummyMonsterInfo,0xaa,sizeof(MONSTER_TOTALINFO));
}

CMonster::~CMonster()
{

}


void CMonster::InitMonster(MONSTER_TOTALINFO* pTotalInfo)
{
	memcpy(&m_DummyMonsterInfo,pTotalInfo,sizeof(MONSTER_TOTALINFO));
	memcpy(&m_MonsterInfo,pTotalInfo,sizeof(MONSTER_TOTALINFO));
	m_pSInfo = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind());
}

void CMonster::CheckTmpFunc()
{
	if(
		m_DummyMonsterInfo.MonsterKind != m_MonsterInfo.MonsterKind ||
		m_DummyMonsterInfo.Group != m_MonsterInfo.Group ||
		m_DummyMonsterInfo.MapNum != m_MonsterInfo.MapNum )
	{
		ASSERT(0);
	}
}
void CMonster::Process()
{
//	CheckTmpFunc();

	if(IsDied())
	{
		DWORD time;
		time = REMOVEFADE_START_TIME;
		if(GetObjectKind() == eObjectKind_BossMonster)
		{
			switch(GetMonsterKind())
			{
			case eBOSSKIND_EMPERIORTOMB:
			case eBOSSKIND_EMPERIORTOMB_EVENT:
				{
					time += 5000;
				}
				break;
			case eBOSSKIND_MOOUN:
				{
					time += 1000;
				}
				break;
			case eBOSSKIND_COWF:
				{
					time += 1000;
				}
				break;
			case eBOSSKIND_COWL:
				{
					time += 1000;
				}
				break;
			case eBOSSKIND_COWKING:
				{
					time += 3000;
				}
				break;
			}
		}

		if( gCurTime - m_DiedTime > time )
		{
			ShowObjectName( FALSE );
			ShowChatBalloon( FALSE, NULL );
			//그림자도 없애주자.
			if(m_ShadowObj.IsInited())
			{
				m_ShadowObj.Release();
			}

			//DWORD ToTime = time;
			//DWORD ElTime = gCurTime - m_DiedTime - time;
			//float alp = 1 - (ElTime / (float)ToTime);
			float alp = GetAlphaValue(time, m_DiedTime+time, FALSE);

			if(alp <= 0)
				alp = 0;
			GetEngineObject()->SetAlpha(alp);
			
			if( GAMEIN->GetMonsterGuageDlg() )
			if( GAMEIN->GetMonsterGuageDlg()->IsActive() == TRUE )
			{
				if( GAMEIN->GetMonsterGuageDlg()->GetCurMonster() == this )
				{
					GAMEIN->GetMonsterGuageDlg()->SetActive(FALSE);
				}
			}

		}
#ifdef _TESTCLIENT_
		if(gCurTime - m_DiedTime > MONSTERREMOVE_TIME)
		{
			OBJECTMGR->AddGarbageObject(this);
		}
#endif
	}
/*	
	// 죽었을때 일정한 시간이 지나면 창 띄운다.
	if(GetState() == eObjectState_Die)
	{		
		if(gCurTime - GetStateStartTime() > REMOVEFADE_START_TIME)
		{
			// 게이지 삭제
			if( GAMEIN->GetMonsterGuageDlg() )
			{
				CMonster * pSelectedObj = GAMEIN->GetMonsterGuageDlg()->GetCurMonster();//OBJECTMGR->GetSelectedObject();
				if( pSelectedObj == this )
				{
//					OBJECTMGR->SetSelectedObject(NULL);
					GAMEIN->GetMonsterGuageDlg()->SetActive(FALSE);
				}
			}
		}
	}
*/	
	//////////////////////////////////////////////////////////////////////////
	
	CObject::Process();
}

void CMonster::SetMotionInState(BYTE State)
{
//	CheckTmpFunc();
	switch(State)
	{
	case eObjectState_Move:
		{			
			switch(m_MoveInfo.KyungGongIdx)
			{
			case eMA_WALKAROUND:
				{
					m_ObjectState.State_Start_Motion = GetMotionIDX(eMonsterMotionN_Walk);
					m_ObjectState.State_Ing_Motion = GetMotionIDX(eMonsterMotionN_Walk);
					m_ObjectState.State_End_Motion = GetMotionIDX(eMonsterMotionN_Walk);
				}
				break;
			case eMA_RUNAWAY:
				{
					m_ObjectState.State_Start_Motion = GetMotionIDX(eMonsterMotionN_Run);
					m_ObjectState.State_Ing_Motion = GetMotionIDX(eMonsterMotionN_Run);
					m_ObjectState.State_End_Motion = GetMotionIDX(eMonsterMotionN_Run);
				}
				break;
			case eMA_PERSUIT:
				{
					m_ObjectState.State_Start_Motion = GetMotionIDX(eMonsterMotionN_Run);
					m_ObjectState.State_Ing_Motion = GetMotionIDX(eMonsterMotionN_Run);
					m_ObjectState.State_End_Motion = GetMotionIDX(eMonsterMotionN_Run);
				}
				break;
			}
			m_ObjectState.State_End_MotionTime = 0;
			
			ChangeMotion(m_ObjectState.State_Start_Motion,FALSE);
			if(m_ObjectState.State_Ing_Motion != -1)
				ChangeBaseMotion(m_ObjectState.State_Ing_Motion);
		}
		break;
	case eObjectState_None:
		{
			m_ObjectState.State_Start_Motion = GetMotionIDX(eMonsterMotionN_Stand);
			m_ObjectState.State_Ing_Motion = GetMotionIDX(eMonsterMotionN_Stand);
			m_ObjectState.State_End_Motion = GetMotionIDX(eMonsterMotionN_Stand);
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
/*
		switch(State)
		{
		case eObjectState_Move:
			{
				m_ObjectState.State_Start_Motion = eMonsterMotion_Walk;
				m_ObjectState.State_Ing_Motion = eMonsterMotion_Walk;
				m_ObjectState.State_End_Motion = eMonsterMotion_Walk;
				m_ObjectState.State_End_MotionTime = 0;
			}
			break;
		case eObjectState_None:
			{
				m_ObjectState.State_Start_Motion = eMonsterMotion_Standard;
				m_ObjectState.State_Ing_Motion = eMonsterMotion_Standard;
				m_ObjectState.State_End_Motion = eMonsterMotion_Standard;
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
		}*/

}

//////////////////////////////////////////////////////////////////////////
// 행동 함수들..
void CMonster::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive)
{
//	CheckTmpFunc();
//	OBJECTSTATEMGR->EndObjectState(this,eObjectState_Die,REMOVEFADE_START_TIME);
	if(pAttacker)
		MOVEMGR->SetLookatPos(this,&pAttacker->GetCurPosition(),0,gCurTime);

	int DieEffect;
	if(bFatalDamage)
		DieEffect = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind())->Die2DramaNum;	// 날라가서 죽기
	else
		DieEffect = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind())->Die1DramaNum;	// 그냥 죽기
		
	if(DieEffect != -1)
	{
		TARGETSET set;
		set.pTarget = this;
		set.ResultInfo.bCritical = bCritical;
		set.ResultInfo.bDecisive = bDecisive;
		
		EFFECTMGR->StartEffectProcess(DieEffect,pAttacker,&set,1,GetID(),
							EFFECT_FLAG_DIE|EFFECT_FLAG_GENDERBASISTARGET);
	}

	// Life 셋팅
	SetLife(0);
	
	CObject::Die(pAttacker,bFatalDamage,bCritical,bDecisive);
}
void CMonster::Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage)
{	
//	CheckTmpFunc();
	// 2005 크리스마스 이벤트
	BOOL bEvent = FALSE;
	
	if( pAttacker )
	{
        if( pAttacker->GetObjectKind() == eObjectKind_Player )
		if( ((CPlayer*)pAttacker)->GetWeaponEquipType() == WP_EVENT ||
			((CPlayer*)pAttacker)->GetWeaponEquipType() == WP_EVENT_HAMMER )
            bEvent = TRUE;
	}
	
	if(Damage != 0 || bEvent)
	{
		if(GetState() == eObjectState_None)
		{
			if(pAttacker)
				MOVEMGR->SetLookatPos(this,&pAttacker->GetCurPosition(),0,gCurTime);
		}
		
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
		
		int DamageEffect = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind())->Damage1DramaNum;

		if(DamageEffect != -1)
		{
			TARGETSET set;
			set.pTarget = this;
			set.ResultInfo.bCritical = bCritical;
			set.ResultInfo.bDecisive = bDecisive;
			
			EFFECTMGR->StartEffectProcess(DamageEffect,pAttacker,&set,1,GetID(),
											EFFECT_FLAG_GENDERBASISTARGET);
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
void CMonster::Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal)
{
//	CheckTmpFunc();
	DWORD life = GetLife() + HealVal;
	SetLife(life);
}

void CMonster::Attack(int AttackNum,CObject* pTarget)
{
	/*int AttackEffect;
	if(AttackNum)
		AttackEffect = GAMERESRCMNGR->GetMonsterInfo(GetMonsterKind())->Attack1DramaNum;
	else
		AttackEffect = GAMERESRCMNGR->GetMonsterInfo(GetMonsterKind())->Attack2DramaNum;

	if(AttackEffect != -1)
	{
		TARGETSET set;
		set.pTarget = pTarget;
		set.TargetData = 0;
		EFFECTMGR->StartMonEffectProcess(AttackEffect,this,&set,1,pTarget->GetID());
	}*/
}

float CMonster::GetWeight()
{
//	CheckTmpFunc();
	BASE_MONSTER_LIST* pList = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind());
	ASSERT(pList->MonsterKind == GetMonsterKind());
	return pList->Weight;
}

float CMonster::GetRadius()
{
//	CheckTmpFunc();
	BASE_MONSTER_LIST* pList = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind());
	ASSERT(pList->MonsterKind == GetMonsterKind());
	return (float)pList->MonsterRadius;
}

void CMonster::SetLife(DWORD val, BYTE type)
{
//	CheckTmpFunc();
	m_MonsterInfo.Life = val;

	if( GAMEIN->GetMonsterGuageDlg() && m_pSInfo )
	{
		if( GAMEIN->GetMonsterGuageDlg()->GetCurMonster() == this )
		{
			GAMEIN->GetMonsterGuageDlg()->SetMonsterLife( val, m_pSInfo->Life );
		}
	}
}

void CMonster::SetShield(DWORD val, BYTE type)
{
//	CheckTmpFunc();
	m_MonsterInfo.Shield = val;

	if( GAMEIN->GetMonsterGuageDlg() && m_pSInfo )
	{
		if( GAMEIN->GetMonsterGuageDlg()->GetCurMonster() == this )
		{
			GAMEIN->GetMonsterGuageDlg()->SetMonsterShield( val, m_pSInfo->Shield );
		}
	}
}

float CMonster::DoGetMoveSpeed()
{
//	CheckTmpFunc();
	float Speed;
	WORD kind = GetMonsterKind();
//	if(kind == 0)
//		int a =1;
	switch(m_MoveInfo.KyungGongIdx)
	{
	case eMA_WALKAROUND:
		{
			Speed = (float)GAMERESRCMNGR->GetMonsterListInfo(kind)->WalkMove;
		}
		break;
	case eMA_RUNAWAY:
		{
			Speed = (float)GAMERESRCMNGR->GetMonsterListInfo(kind)->RunawayMove;
		}
		break;
	case eMA_PERSUIT:
		{
			Speed = (float)GAMERESRCMNGR->GetMonsterListInfo(kind)->RunMove;
		}
		break;
	default:
		{
			Speed = 0;
//			ASSERT(0);
		}
		break;
	}
	return Speed;
}

void CMonster::AddStatus(CStatus* pStatus)
{
	if(GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind())->ObjectKind != eObjectKind_FieldBossMonster)
		CObject::AddStatus(pStatus);
}

/// 2007. 7. 3. CBH - 몬스터 종류 반환 함수 추가
WORD CMonster::GetObjectKind()
{
	return m_pSInfo->ObjectKind;
}

WORD CMonster::GetMonsterLevel()
{
	return m_pSInfo->Level;
}
/////////////////////////////////////////////////////////
