// SkillObjectFirstUnit_Attack.cpp: implementation of the CSkillObjectFirstUnit_Attack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectFirstUnit_Attack.h"
#include "SkillInfo.h"
#include "Battle.h"
#include "..\[CC]Header\CommonCalcFunc.h"
#include "Object.h"

#ifdef _MAPSERVER_
#include "TacticManager.h"
#include "AttackManager.h"
#include "StateMachinen.h"
#include "SkillObject_Server.h"
#include "SkillManager_server.h"
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectFirstUnit_Attack::CSkillObjectFirstUnit_Attack(SKILLINFO* pSkillInfo)
: CSkillObjectFirstUnit(pSkillInfo)
{
#ifdef _MAPSERVER_
	m_pSkillInfo = NULL;
#endif
}

CSkillObjectFirstUnit_Attack::~CSkillObjectFirstUnit_Attack()
{

}

void CSkillObjectFirstUnit_Attack::Init( CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pPlayer /* = NULL  */)
{
	m_bMugong = pSkillInfo->GetSkillInfo()->ComboNum == SKILL_COMBO_NUM;
	m_PhysicalAttack = pSkillInfo->GetSkillInfo()->FirstPhyAttack[SkillLevel-1];	

#ifdef _MAPSERVER_
	m_pSkillInfo = pSkillInfo;
#endif

	WORD mi,ma;

	mi = pSkillInfo->GetSkillInfo()->FirstAttAttackMin[SkillLevel-1];
	ma = pSkillInfo->GetSkillInfo()->FirstAttAttackMax[SkillLevel-1];

	m_AttribAttack.SetValue(mi,ma);
	m_CriticalRate = pSkillInfo->GetCriticalRate(SkillLevel);
	m_AttAttackRate = pSkillInfo->GetFirstAttAttack(SkillLevel);

	CSkillObjectFirstUnit::Init(pSkillInfo,SkillLevel);
}


#ifdef _MAPSERVER_

int CSkillObjectFirstUnit_Attack::ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,
												   MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp)
{
	CTargetListIterator iter(&pAddMsg->TargetList);
	STLIST* pList;
	RESULTINFO DamageInfo;
	
	DWORD AmplifiedPower = 0;
	DWORD AmplifiedPowerAtt = 0;
	float AmplifiedPowerAttRate = 0;
	WORD AmplifiedPowerAttrib = 0;

	if(m_PhysicalAttack)		// 물리 공격일때만
		AmplifiedPower = pOperator->GetSpecialStateParam1(eSpecialState_AmplifiedPowerPhy);

	WORD Attrib = m_Attrib;
#ifdef _JAPAN_LOCAL_
	AmplifiedPowerAttrib = pOperator->GetSpecialStateParam2(eSpecialState_AmplifiedPowerAtt);
	if( Attrib == 0 || Attrib == 6 || Attrib == AmplifiedPowerAttrib )
	{
		AmplifiedPowerAtt = pOperator->GetSpecialStateParam1(eSpecialState_AmplifiedPowerAtt);
		AmplifiedPowerAttRate = pOperator->GetSpecialStateParam3(eSpecialState_AmplifiedPowerAtt);
	}
#else
	if( m_AttribAttack.GetMax() == 0 && m_AttAttackRate == 0)
		Attrib = pOperator->GetSpecialStateParam2(eSpecialState_AmplifiedPowerAtt);

	if( Attrib == pOperator->GetSpecialStateParam2(eSpecialState_AmplifiedPowerAtt) )
	{
		AmplifiedPowerAtt = pOperator->GetSpecialStateParam1(eSpecialState_AmplifiedPowerAtt);
		AmplifiedPowerAttRate = pOperator->GetSpecialStateParam3(eSpecialState_AmplifiedPowerAtt);
	}
#endif

	
	LEVELTYPE OperLevel = pOperator->GetLevel();

//041213 KES decreaserate
	float fRange				= (float)m_pSkillInfo->GetSkillInfo()->SkillRange;
	float fDecreaseRate			= (float)m_pSkillInfo->GetSkillInfo()->DamageDecreaseForDist / 100.0f;

	if( fDecreaseRate > 1.0f ) fDecreaseRate = 1.0f;
			
	BOOL bDecrease = fDecreaseRate && m_pSkillInfo && fRange;

	VECTOR3 OperatorPos;
	pTargetList->GetSkillObjectPosition( &OperatorPos );
//---
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	// 스킬공격력
	if(pAddMsg->SkillObjectInfo.Option)
	{
		SKILLOPTION* pSkillOption = SKILLMGR->GetSkillOption(pAddMsg->SkillObjectInfo.Option);

		if(pSkillOption)
		{
			if(pSkillOption->PhyAtk)
				m_PhysicalAttack = m_PhysicalAttack * (1.0f + pSkillOption->PhyAtk);
			if(pSkillOption->AttAtk)
				m_AttAttackRate = m_AttAttackRate * (1.0f + pSkillOption->AttAtk);
		}
	}
	//////////////////////////////////////////////////////////////////////////

	int nCount = 0;

	pTargetList->SetPositionHead();
	while(pList = pTargetList->GetNextTargetList())
	{
		DamageInfo.Clear();

		if(pOperator->GetBattle()->IsEnemy(pOperator,pList->pObject))
		{

			float fDecreaseDamageRate = 1.0f;	//공격력율
//*041213 KES decreaserate---------------------------------------------
			if( bDecrease )
			{
				VECTOR3 TargetPos;
				pList->pObject->GetPosition( &TargetPos );

				float dist = CalcDistanceXZ( &OperatorPos, &TargetPos );
//
				if( dist > fRange )			dist = fRange;
				else if( dist < 0.0f )		dist = 0.0f;

				fDecreaseDamageRate = ( 1 - fDecreaseRate ) * ( fRange - dist ) / fRange + fDecreaseRate;
			}
///-------------------------------------------------------------------

			DWORD AttackMin;
			DWORD AttackMax;

			if(m_pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB && pOperator->GetObjectKind() == eObjectKind_Player)
			{
				DWORD AttackPower = TACTICMGR->GetAttackValue( (CPlayer*)pOperator, (DWORD)m_PhysicalAttack );
				ATTACKMGR->AttackJinbub( pOperator, pList->pObject, AttackPower, 
										 m_AttribAttack.GetMin(), m_AttribAttack.GetMax(),
										 &DamageInfo, fDecreaseDamageRate );
			}
			else
			{
				AttackMin = (DWORD)(m_AttribAttack.GetMin()*SkillTreeAmp+AmplifiedPowerAtt);
				AttackMax = (DWORD)(m_AttribAttack.GetMax()*SkillTreeAmp+AmplifiedPowerAtt);

				ATTACKMGR->Attack(m_bMugong,
					pOperator,pList->pObject, AmplifiedPower,
					m_PhysicalAttack*SkillTreeAmp,
					Attrib,
					AttackMin,
					AttackMax,
					m_AttAttackRate*SkillTreeAmp+AmplifiedPowerAttRate,
					m_CriticalRate,
					&DamageInfo,
					TRUE, fDecreaseDamageRate, AmplifiedPowerAttrib );	// TRUE 는 First Unit은 카운터 데미지 있음
			}
		
#ifdef _HK_LOCAL_
			if( pList->pObject->GetObjectKind() & eObjectKind_Monster )
#endif
				++nCount;


		}

		DefaultFirstEffect(pOperator,pList,&DamageInfo);
		
		iter.AddTargetWithResultInfo(pList->pObject->GetID(),pList->bTargetKind,&DamageInfo);
	}
	iter.Release();

	if(pOperator->GetObjectKind() == eObjectKind_Player)
	{
		if(m_pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB)
			TACTICMGR->AddGarbageTactic((CPlayer*)pOperator);
	}

	m_pSkillInfo = NULL;

	return nCount;
}

#endif

#ifdef _MHCLIENT_
void CSkillObjectFirstUnit_Attack::StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget)
{
	CSkillObjectFirstUnit::StartFirstEffect(pOperator,pTList,pMainTarget);

	// for delete Effect
	pOperator->GetSpecialStateParam1(eSpecialState_AmplifiedPowerPhy);
}
#endif





//===============================
//CSkillObjectFirstUnit_AttackLife
//===============================

CSkillObjectFirstUnit_AttackLife::CSkillObjectFirstUnit_AttackLife(SKILLINFO* pSkillInfo)
: CSkillObjectFirstUnit(pSkillInfo)
{
#ifdef _MAPSERVER_
	m_pSkillInfo = NULL;
#endif

	m_AttackRate = 0.0f;
	m_SuccessRate = 0.0f;

}

CSkillObjectFirstUnit_AttackLife::~CSkillObjectFirstUnit_AttackLife()
{

}

void CSkillObjectFirstUnit_AttackLife::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper)
{
	m_AttackRate = pSkillInfo->GetAttackLifeRate(SkillLevel-1);	
	m_SuccessRate = pSkillInfo->GetAttackSuccessRate(SkillLevel-1);

#ifdef _MAPSERVER_
	m_pSkillInfo = pSkillInfo;
#endif

	CSkillObjectFirstUnit::Init(pSkillInfo,SkillLevel);
}


#ifdef _MAPSERVER_

int CSkillObjectFirstUnit_AttackLife::ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,
												   MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp)
{
	CTargetListIterator iter(&pAddMsg->TargetList);
	STLIST* pList;
	RESULTINFO DamageInfo;
	

	LEVELTYPE OperLevel = pOperator->GetLevel();

	int nCount = 0;

	pTargetList->SetPositionHead();
	while(pList = pTargetList->GetNextTargetList())
	{
		DamageInfo.Clear();

		if(pOperator->GetBattle()->IsEnemy(pOperator,pList->pObject))
		{

			//attack을 새로 만들어야 한다.

			int nSeed = rand()%100;

			if( pList->pObject->GetObjectKind() == eObjectKind_BossMonster )
				nSeed = 9999999;

			if( nSeed < (int)(m_SuccessRate*100.0f) + (int)(OperLevel-pList->pObject->GetLevel()) )
			{
				ATTACKMGR->AttackAbs( pOperator, pList->pObject, eAAK_LIFE, m_AttackRate, &DamageInfo );
			}

#ifdef _HK_LOCAL_
			if( pList->pObject->GetObjectKind() & eObjectKind_Monster )
#endif
				++nCount;

		}

		DefaultFirstEffect(pOperator,pList,&DamageInfo);
		
		iter.AddTargetWithResultInfo(pList->pObject->GetID(),pList->bTargetKind,&DamageInfo);
	}
	iter.Release();

	m_pSkillInfo = NULL;

	return nCount;
}

#endif

#ifdef _MHCLIENT_
void CSkillObjectFirstUnit_AttackLife::StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget)
{
	CSkillObjectFirstUnit::StartFirstEffect(pOperator,pTList,pMainTarget);

	// for delete Effect
//	pOperator->GetSpecialStateParam1(eSpecialState_AmplifiedPowerPhy);
}
#endif


//===============================
//CSkillObjectFirstUnit_AttackShield
//===============================

CSkillObjectFirstUnit_AttackShield::CSkillObjectFirstUnit_AttackShield(SKILLINFO* pSkillInfo)
: CSkillObjectFirstUnit(pSkillInfo)
{
#ifdef _MAPSERVER_
	m_pSkillInfo = NULL;
#endif

	m_AttackRate = 0.0f;
	m_SuccessRate = 0.0f;

}

CSkillObjectFirstUnit_AttackShield::~CSkillObjectFirstUnit_AttackShield()
{

}

void CSkillObjectFirstUnit_AttackShield::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper)
{
	m_AttackRate = pSkillInfo->GetAttackShieldRate(SkillLevel-1);	
	m_SuccessRate = pSkillInfo->GetAttackSuccessRate(SkillLevel-1);

#ifdef _MAPSERVER_
	m_pSkillInfo = pSkillInfo;
#endif

	CSkillObjectFirstUnit::Init(pSkillInfo,SkillLevel);
}


#ifdef _MAPSERVER_

int CSkillObjectFirstUnit_AttackShield::ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,
												   MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp)
{
	CTargetListIterator iter(&pAddMsg->TargetList);
	STLIST* pList;
	RESULTINFO DamageInfo;
	

	LEVELTYPE OperLevel = pOperator->GetLevel();

	int nCount = 0;

	pTargetList->SetPositionHead();
	while(pList = pTargetList->GetNextTargetList())
	{
		DamageInfo.Clear();

		if(pOperator->GetBattle()->IsEnemy(pOperator,pList->pObject))
		{

			//attack을 새로 만들어야 한다.

			int nSeed = rand()%100;

			if( pList->pObject->GetObjectKind() == eObjectKind_BossMonster )
				nSeed = 9999999;

			if( nSeed < (int)(m_SuccessRate*100.0f) + (int)(OperLevel-pList->pObject->GetLevel()) )
			{
				ATTACKMGR->AttackAbs( pOperator, pList->pObject, eAAK_SHIELD, m_AttackRate, &DamageInfo );
			}

#ifdef _HK_LOCAL_
			if( pList->pObject->GetObjectKind() & eObjectKind_Monster )
#endif
				++nCount;

		}
//TEMP!
//	char buf[128];
//	wsprintf( buf, "none %d", nCount );
//	g_Console.LOG( 4, buf );

		DefaultFirstEffect(pOperator,pList,&DamageInfo);
		
		iter.AddTargetWithResultInfo(pList->pObject->GetID(),pList->bTargetKind,&DamageInfo);
	}
	iter.Release();

	m_pSkillInfo = NULL;

	return nCount;
}

#endif

#ifdef _MHCLIENT_
void CSkillObjectFirstUnit_AttackShield::StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget)
{
	CSkillObjectFirstUnit::StartFirstEffect(pOperator,pTList,pMainTarget);

	// for delete Effect
//	pOperator->GetSpecialStateParam1(eSpecialState_AmplifiedPowerPhy);
}
#endif
