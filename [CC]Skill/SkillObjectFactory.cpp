// SkillObjectFactory.cpp: implementation of the CSkillObjectFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectFactory.h"

#include "SkillInfo.h"

#include "SkillObject_Mine.h"

// terminator
#include "SkillObjectTerminator_Time.h"

// targetlist
#include "SkillObjectTargetList_Area.h"
#include "SkillObjectTargetList_Circle.h"
#include "SkillObjectTargetList_One.h"

// singleunit
#include "SkillObjectSingleUnit_Attack.h"
#include "SkillObjectSingleUnit_Heal.h"
#include "SkillObjectSingleUnit_Recharge.h"

// attachunit
#include "SkillObjectAttachUnit_MaxLife.h"
#include "SkillObjectAttachUnit_Defence.h"
#include "SkillObjectAttachUnit_Attack.h"
#include "SkillObjectAttachUnit_CounterAttack.h"
#include "SkillObjectAttachUnit_Immune.h"
#include "SkillObjectAttachUnit_MoveSpeed.h"
#include "SkillObjectAttachUnit_Dodge1.h"
#include "SkillObjectAttachUnit_Vampiric.h"
#include "skillobjectattachunit_skipeffect.h"

// FirstUnit
#include "SkillObjectFirstUnit.h"
#include "SkillObjectFirstUnit_Attack.h"
#include "SkillObjectFirstUnit_Recover.h"
#include "SkillObjectFirstUnit_SingleSpecialState.h"
#include "SkillObjectFirstUnit_job.h"

// StateUnit
#include "SkillObjectStateUnit_TieUp.h"
#include "SkillObjectStateUnit_StatusAttach.h"
#include "SkillObjectStateUnit_DummyState.h"
#include "SkillObjectStateUnit_AmplifiedPower.h"
#ifndef __MAPSERVER__
#include "SkillManager_client.h"
#endif

#ifdef __MAPSERVER__
#include "MemoryChecker.h"
#include "mherror.h"
#include "SkillManager_server.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectFactory::CSkillObjectFactory()
{
	
}

CSkillObjectFactory::~CSkillObjectFactory()
{

}

CSkillObject* CSkillObjectFactory::MakeNewSkillObject(CSkillInfo* pSkillInfo)
{
	// 임시 하드 코딩
	// 나중에 pSkillInfo에 맞게 생성하고 셋팅해주어야 함

//	int SKsize = 0;

	CSkillObjectTerminator* pTer;
	CSkillObjectTargetList* pTar;
	CSkillObjectSingleUnit* pSingleUnit;
	CSkillObjectAttachUnit* pAttachUnit;
	CSkillObjectFirstUnit* pFirstUnit;
	CSkillObjectStateUnit* pStateUnit;

	SKILLINFO* pInfo = pSkillInfo->GetSkillInfo();
	WORD wSkillKind = pInfo->SkillKind;

	// Terminator
	pTer = new CSkillObjectTerminator_Time(pInfo);
//	SKsize += sizeof(CSkillObjectTerminator_Time);

	// TargetList
	if(pSkillInfo->IsMultipleTargetSkill() == TRUE)
	{
		if(pInfo->TargetRange == 0)
		{
			ASSERT(pInfo->TargetAreaIdx);
			pTar = new CSkillObjectTargetList_Area(pInfo);
//			SKsize += sizeof(CSkillObjectTargetList_Area);
#ifdef __MAPSERVER__

			MHERROR->OutputFile("./Log/Debug.txt", MHERROR->GetStringArg("TargetAreaIdx : %d, NextState: %d", pInfo->TargetAreaIdx));
#endif
		}
		else
		{
			pTar = new CSkillObjectTargetList_Circle(pInfo);
//			SKsize += sizeof(CSkillObjectTargetList_Circle);
		}
	}
	else
	{
		pTar = new CSkillObjectTargetList_One(pInfo);
//		SKsize += sizeof(CSkillObjectTargetList_One);
	}

	// firstunit
	if(pInfo->AIObject == 1)
	{
		pFirstUnit = new CSkillObjectFirstUnit(pInfo);
//		SKsize += sizeof(CSkillObjectFirstUnit);
	}
	else if(pInfo->AttackLifeRate[11] > 0.0f && pInfo->AttackSuccessRate[11] > 0.0f )
	{
		pFirstUnit = new CSkillObjectFirstUnit_AttackLife(pInfo);
	}
	else if(pInfo->AttackShieldRate[11] > 0.0f && pInfo->AttackSuccessRate[11] > 0.0f )
	{
		pFirstUnit = new CSkillObjectFirstUnit_AttackShield(pInfo);
	}
	else if(pInfo->FirstAttAttackMax[11] != 0 || 
			pInfo->FirstAttAttack[11] > 0.000001f|| 
			pInfo->FirstPhyAttack[11] > 0.000001f)
	{
		pFirstUnit = new CSkillObjectFirstUnit_Attack(pInfo);
//		SKsize += sizeof(CSkillObjectFirstUnit_Attack);
	}
	else if(pInfo->FirstRecoverLife[11] != 0 || pInfo->FirstRecoverNaeRyuk[11] != 0)
	{
		pFirstUnit = new CSkillObjectFirstUnit_Recover(pInfo);
//		SKsize += sizeof(CSkillObjectFirstUnit_Recover);
	}
	////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	else if(pInfo->SpecialState)
	{
		pFirstUnit = new CSkillObjectFirstUnit_SingleSpecialState(pInfo);
	}
	////////////////////////////////////////////////////////////////////////////	
	///// 2007. 7. 6. CBH - 전문스킬
	else if(SKILLMGR->CheckSkillKind(wSkillKind) == TRUE)	
	{
		pFirstUnit = new CSkillObjectFirstUnit_Job(pInfo);
	}	
	else
	{
		pFirstUnit = new CSkillObjectFirstUnit(pInfo);
//		SKsize += sizeof(CSkillObjectFirstUnit);
	}

	CSkillObject* pSObj;
	if(pInfo->AIObject == 1)	// 지뢰
	{
		pSObj = new CSkillObject_Mine(pSkillInfo,pTer,pTar,pFirstUnit);
//		SKsize += sizeof(CSkillObject_Mine);
	}
	else
	{
		pSObj = new CSkillObject(pSkillInfo,pTer,pTar,pFirstUnit);
//		SKsize += sizeof(CSkillObject);
	}

	// SingleUnit
	DWORD SingleUnitNum = 0;
	if(pInfo->ContinueAttAttack[11] != 0 || pInfo->ContinueAttAttackRate[11] > 0.000001f )
	{
		pSingleUnit = new CSkillObjectSingleUnit_Attack(SingleUnitNum++);
		pSObj->AddSingleUnit(pSingleUnit);
//		SKsize += sizeof(CSkillObjectSingleUnit_Attack);
	}
	
	if(pInfo->ContinueRecoverLife[11] != 0 && pInfo->ContinueRecoverShield[11] != 0)
	{
		pAttachUnit = new CSkillObjectAttachUnit_RecoverLifeShield();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
	}
	else
	{
		if(pInfo->ContinueRecoverLife[11] != 0)
		{
			pSingleUnit = new CSkillObjectSingleUnit_Heal(SingleUnitNum++);
			pSObj->AddSingleUnit(pSingleUnit);
//			SKsize += sizeof(CSkillObjectSingleUnit_Heal);
		}
		if(pInfo->ContinueRecoverNaeRyuk[11] != 0)
		{
			pSingleUnit = new CSkillObjectSingleUnit_Recharge(SingleUnitNum++);
			pSObj->AddSingleUnit(pSingleUnit);
//			SKsize += sizeof(CSkillObjectSingleUnit_Recharge);
		}
	}

	// AttachUnit
	if( pInfo->UpMaxLife[11] != 0 || 
		pInfo->UpMaxNaeRyuk[11] != 0 ||
		pInfo->UpMaxShield[11] != 0 )		// 최대 생명,내력 상승
	{
		pAttachUnit = new CSkillObjectAttachUnit_MaxLifeNaeRyukUp();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
//		SKsize += sizeof(CSkillObjectAttachUnit_MaxLifeNaeRyukUp);
//		SKsize += sizeof(CSkillObjectStateUnit_StatusAttach);
	}
	if(pInfo->UpAttDefence[11] > 0.000001f || pInfo->UpPhyDefence[11] > 0.000001f)		// 방어력 상승
	{
		pAttachUnit = new CSkillObjectAttachUnit_DefenceUp();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
//		SKsize += sizeof(CSkillObjectAttachUnit_DefenceUp);
//		SKsize += sizeof(CSkillObjectStateUnit_StatusAttach);
	}
	if(pInfo->UpPhyAttack[11] > 0.000001f)		// 물리공격력 상승
	{
		pAttachUnit = new CSkillObjectAttachUnit_AttackUp();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
//		SKsize += sizeof(CSkillObjectAttachUnit_AttackUp);
//		SKsize += sizeof(CSkillObjectStateUnit_StatusAttach);
	}
	if(pInfo->CounterAttAttack[11] > 0.000001f || pInfo->CounterPhyAttack[11] > 0.000001f)	// 반격
	{
		pAttachUnit = new CSkillObjectAttachUnit_CounterAttack();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
//		SKsize += sizeof(CSkillObjectAttachUnit_CounterAttack);
//		SKsize += sizeof(CSkillObjectStateUnit_StatusAttach);
	}
	if(pInfo->Immune != 0)		// 무적
	{
		pAttachUnit = new CSkillObjectAttachUnit_Immune();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
//		SKsize += sizeof(CSkillObjectAttachUnit_Immune);
//		SKsize += sizeof(CSkillObjectStateUnit_StatusAttach);
	}
	if(pInfo->ChangeSpeedType == 1)		// 이동속도 변화
	{
		//랜덤체크
		
		pAttachUnit = new CSkillObjectAttachUnit_MoveSpeed();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_NEGATIVE);
		pSObj->AddStateUnit(pStateUnit);
//		SKsize += sizeof(CSkillObjectAttachUnit_MoveSpeed);
//		SKsize += sizeof(CSkillObjectStateUnit_StatusAttach);
	}
	if(pInfo->CounterDodgeKind == 2)		// 회피
	{
		pAttachUnit = new CSkillObjectAttachUnit_Dodge();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
//		SKsize += sizeof(CSkillObjectAttachUnit_Dodge);
//		SKsize += sizeof(CSkillObjectStateUnit_StatusAttach);
	}
	if(pInfo->VampiricLife[11] > 0.000001f || pInfo->VampiricNaeryuk[11] > 0.000001f)		// 흡기
	{
		pAttachUnit = new CSkillObjectAttachUnit_Vampiric();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
//		SKsize += sizeof(CSkillObjectAttachUnit_Vampiric);
//		SKsize += sizeof(CSkillObjectStateUnit_StatusAttach);
	}	
	if( pInfo->DownMaxLife[11] != 0 || 
		pInfo->DownMaxNaeRyuk[11] != 0 ||
		pInfo->DownMaxShield[11] != 0 )		// 최대 생명,내력 감소
	{
		pAttachUnit = new CSkillObjectAttachUnit_MaxLifeNaeRyukDown();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_NEGATIVE);
		pSObj->AddStateUnit(pStateUnit);
//		SKsize += sizeof(CSkillObjectAttachUnit_MaxLifeNaeRyukDown);
//		SKsize += sizeof(CSkillObjectStateUnit_StatusAttach);
	}
	if(pInfo->DownAttDefence[11] > 0.000001f || pInfo->DownPhyDefence[11] > 0.000001f )		// 방어력 감소
	{
		pAttachUnit = new CSkillObjectAttachUnit_DefenceDown();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_NEGATIVE);
		pSObj->AddStateUnit(pStateUnit);
//		SKsize += sizeof(CSkillObjectAttachUnit_DefenceDown);
//		SKsize += sizeof(CSkillObjectStateUnit_StatusAttach);
	}
	if(pInfo->DownPhyAttack[11] > 0.000001f)		// 물리공격력 감소
	{
		pAttachUnit = new CSkillObjectAttachUnit_AttackDown();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_NEGATIVE);
		pSObj->AddStateUnit(pStateUnit);
//		SKsize += sizeof(CSkillObjectAttachUnit_AttackDown);
//		SKsize += sizeof(CSkillObjectStateUnit_StatusAttach);
	}
	if(pInfo->DamageRate[11] > 0.000001f)		// 데미지 감소율
	{
		pAttachUnit = new CSkillObjectAttachUnit_DamageDownRate();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
	}
	if(pInfo->AttackRate[11] > 0.000001f)		// 공격 데미지 증가
	{
		pAttachUnit = new CSkillObjectAttachUnit_AttackLastUpRate();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
	}
	if(pInfo->VampiricReverseLife[11] > 0.000001f || pInfo->VampiricReverseNaeryuk[11] > 0.000001f )
	{
		pAttachUnit = new CSkillObjectAttachUnit_ReverseVampiric();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
	}
	if(pInfo->AttackPhyLastUp[11] > 0 )
	{
		pAttachUnit = new CSkillObjectAttachUnit_AttackLastPhyUp();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
	}
	if( pInfo->AttackAttLastUp[11] > 0 )
	{
		pAttachUnit = new CSkillObjectAttachUnit_AttackLastAttrUp();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
	}
	////////////////////////////////////////////////////////
	//06. 06 2차 전직 - 이영준
	//이펙트 생략(무초)
	if( pInfo->SkipEffect > 0 )
	{
		pAttachUnit = new CSkillObjectAttachUnit_SkipEffect();
		pStateUnit = new CSkillObjectStateUnit_StatusAttach();
		((CSkillObjectStateUnit_StatusAttach*)pStateUnit)->SetAttachUnit(pAttachUnit,SKILLRESULTKIND_POSITIVE);
		pSObj->AddStateUnit(pStateUnit);
	}
	////////////////////////////////////////////////////////

	// StateUnit
	if(pInfo->TieUpType != SKILLTIEUP_TYPE_NOTHING)
	{
		pStateUnit = new CSkillObjectStateUnit_TieUp();
		pSObj->AddStateUnit(pStateUnit);
//		SKsize += sizeof(CSkillObjectStateUnit_TieUp);
	}
	if( pInfo->ContinueAttAttack[11] != 0 ||
		pInfo->ContinueAttAttackRate[11] > 0.000001f/* ||
		pInfo->ContinueRecoverLife[11] != 0*/)
	{
		pStateUnit = new CSkillObjectStateUnit_DummyState();
		pSObj->AddStateUnit(pStateUnit);
//		SKsize += sizeof(CSkillObjectStateUnit_DummyState);
	}
	if( pInfo->AmplifiedPowerPhy[11] != 0 || 
		pInfo->AmplifiedPowerAtt[11] != 0 ||
		pInfo->AmplifiedPowerAttRate[11] > 0.000001f )
	{
		pStateUnit = new CSkillObjectStateUnit_AmplifiedPower();
		pSObj->AddStateUnit(pStateUnit);		
//		SKsize += sizeof(CSkillObjectStateUnit_AmplifiedPower);
	}
/*
#ifdef __MAPSERVER__
	// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// ┃Memory Check                                              ┃
	// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛	
	stMemoryInfo* pMemoryInfo = MEMORYCHK->GetMemoryInfo( eSkillFactory_MakeSkillObject );
	if( pMemoryInfo )
		pMemoryInfo->Increase( SKsize );
#endif
*/
	return pSObj;
}
