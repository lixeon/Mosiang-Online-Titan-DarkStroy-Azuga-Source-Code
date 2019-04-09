// DamageEffectUnit.cpp: implementation of the CDamageEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectManager.h"
#include "DamageEffectUnit.h"
#include "DamageEffectUnitDesc.h"
#include "..\MoveManager.h"
#include "..\ObjectActionManager.h"

#include "..\Engine\EngineSound.h"
#include "..\ObjectManager.h"


//
#include "..\ChatDialog.h"
#include "..\interface\cListDialog.h"
#include "..\GameIn.h"
//


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDamageEffectUnit::CDamageEffectUnit()
{
}

CDamageEffectUnit::~CDamageEffectUnit()
{
}

void CDamageEffectUnit::Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam)
{
	m_DamageOrder = NULL;

	CEffectUnit::Initialize(pEffectUnitDesc,pEffectParam);
}
void CDamageEffectUnit::Release()
{
	if(m_DamageOrder)
	{
		delete [] m_DamageOrder;
		m_DamageOrder = NULL;
	}
	
	CDamageEffectUnitDesc::ReleaseUnit(this);
}

void CDamageEffectUnit::SortDamageOrder(DWORD nTargetNum,TARGETSET* pTargetArray)
{

}

void CDamageEffectUnit::GetDamageNumberVelocity(VECTOR3* pRtVelocity,CObject* pObject,DWORD DamageKind)
{	
	pRtVelocity->y = 0;
	if(DamageKind == eDamageKind_Left)
	{
		pRtVelocity->x = -.1f;
		pRtVelocity->y = 0.8f;
		pRtVelocity->z = .3f;
	}
	else if(DamageKind == eDamageKind_Right)
	{
		pRtVelocity->x = 0.1f;
		pRtVelocity->y = 0.8f;
		pRtVelocity->z = 0.3f;
	}
	else if(DamageKind == eDamageKind_Front)
	{
		pRtVelocity->x = 0;
		pRtVelocity->y = 1.f;
		pRtVelocity->z = .3f;
	}
	else if(DamageKind == eDamageKind_Counter)
	{
		pRtVelocity->x = 0;
		pRtVelocity->y = 7.f;
		pRtVelocity->z = .2f;
	}
	else if(DamageKind == eDamageKind_ContinueDamage)
	{
		pRtVelocity->x = 0;
		pRtVelocity->y = .5f;
		pRtVelocity->z = .15f;
	}
	
	float angle = pObject->GetAngle();
	TransToRelatedCoordinate(pRtVelocity,pRtVelocity,angle);
}

void CDamageEffectUnit::InitEffect(DWORD nTargetNum,TARGETSET* pTargetArray)
{
	if(nTargetNum == 0)
		return;
	ASSERT(nTargetNum);
	
	m_nObjectNum = nTargetNum;

	SortDamageOrder(nTargetNum,pTargetArray);
	
	m_nCurTargetNum = 0;

	m_bInited = TRUE;
}

BOOL CDamageEffectUnit::On(EFFECTPARAM* pParam)
{
	if(m_bInited == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);

	CDamageEffectUnitDesc* pDesc = (CDamageEffectUnitDesc*)m_pEffectUnitDesc;
	pParam->m_DamageRate += pDesc->m_DamageRate;

	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;

		/// 2007. 7. 6. CBH - 전문기술 전용 몬스터 데미지 UI 처리 추가		
		if( GetObjectKindGroup(pObject[n].pTarget->GetObjectKind()) == eOBJECTKINDGROUP_JOB )
		{
			continue;
		}

		m_nCurTargetNum = n;
		DoOn(pParam,&pObject[n]);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 주인공과 데미지 입는 메인타겟과 가까우면 조금씩 떨어트림
	if(pParam->m_dwFlag & EFFECT_FLAG_HEROATTACK)
	{
		DWORD MainTargetID = GetMainTargetID(pParam->GetMainTarget());
		if(MainTargetID && MainTargetID != HEROID)
		{
			CObject* pMainTarget = OBJECTMGR->GetObject(MainTargetID);
			if(pMainTarget)
			{
//				HERO->OnHitTarget(pMainTarget);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	

	return TRUE;
}

DWORD CalcTotalDamage(DWORD CurTotal,float CurRate,float SumRate)
{
	float lateSum = SumRate - CurRate;
	float inverse = 1.f / (1.f - lateSum);
	return (DWORD)(CurTotal * inverse);
}

void CDamageEffectUnit::Damage(CObject* pVictim,CObject* pAttacker,RESULTINFO* pResultInfo,
							   float CurRate,float SumRate,BYTE DamageKind,BOOL bFatalDamage)
{
	if( pResultInfo->RealDamage > 1000000 )
	{
		ASSERT(0);
		return;
	}

	DWORD LifeOriginalTotalDamage = CalcTotalDamage(pResultInfo->RealDamage,CurRate,SumRate);
	DWORD ShieldOriginalTotalDamage = CalcTotalDamage(pResultInfo->ShieldDamage,CurRate,SumRate);
	DWORD VampiricLifeOriginalTotal = CalcTotalDamage(pResultInfo->Vampiric_Life,CurRate,SumRate);
	DWORD VampiricNaeryukOriginalTotal = CalcTotalDamage(pResultInfo->Vampiric_Naeryuk,CurRate,SumRate);
	DWORD LifeDamage;
	DWORD ShieldDamage;
	DWORD VampiricLife;
	DWORD VampiricNaeryuk;
	DWORD TitanTotalDamage = 0;
	DWORD TitanDamage = 0;
	if( pResultInfo->TitanObsorbDamage )
	{
		TitanTotalDamage = CalcTotalDamage(pResultInfo->TitanObsorbDamage,CurRate,SumRate);
	}

	if(SumRate >= 0.98f)
	{	// 마지막 공격이면 나머지 전부를 데미지로
#ifdef _DEBUG
		if(SumRate > 1)
		{
			char temp[256];
			sprintf(temp,"%s Damage Error",m_pEffectUnitDesc->m_EffFilename);
			ASSERTMSG(SumRate <= 1,temp);
		}
#endif

		LifeDamage = pResultInfo->RealDamage;
		ShieldDamage = pResultInfo->ShieldDamage;
		VampiricLife = pResultInfo->Vampiric_Life;
		VampiricNaeryuk = pResultInfo->Vampiric_Naeryuk;
		TitanDamage = pResultInfo->TitanObsorbDamage;

		// 마지막 공격이면 Stun 상태로 만든다.
		if(pResultInfo->StunTime)
		{
			pVictim->StartSpecialState(eSpecialState_Stun,pResultInfo->StunTime,0,0,0,FindEffectNum("monster_stun_s.beff"),0,(HERO == pAttacker)?TRUE:FALSE);
		}
	}
	else
	{
		LifeDamage = (DWORD)(LifeOriginalTotalDamage*CurRate);		// 자신의 비율대로..
		ShieldDamage = (DWORD)(ShieldOriginalTotalDamage*CurRate);		// 자신의 비율대로..
		VampiricLife = (DWORD)(VampiricLifeOriginalTotal*CurRate);		// 자신의 비율대로..
		VampiricNaeryuk = (DWORD)(VampiricNaeryukOriginalTotal*CurRate);		// 자신의 비율대로..
		TitanDamage = (DWORD)(pResultInfo->TitanObsorbDamage*CurRate);
	}
	if( LifeDamage <= pResultInfo->RealDamage )		pResultInfo->RealDamage -= (DWORD)LifeDamage;
	else											pResultInfo->RealDamage = 0;
	if( ShieldDamage <= pResultInfo->ShieldDamage )	pResultInfo->ShieldDamage -= (DWORD)ShieldDamage;
	else											pResultInfo->ShieldDamage = 0;
	if( VampiricLifeOriginalTotal <= pResultInfo->Vampiric_Life )		pResultInfo->Vampiric_Life -= (DWORD)VampiricLifeOriginalTotal;
	else																pResultInfo->Vampiric_Life = 0;
	if( VampiricNaeryukOriginalTotal <= pResultInfo->Vampiric_Naeryuk )		pResultInfo->Vampiric_Naeryuk -= (DWORD)VampiricNaeryukOriginalTotal;
	else																	pResultInfo->Vampiric_Naeryuk = 0;

	if( TitanTotalDamage <= pResultInfo->TitanObsorbDamage )	pResultInfo->TitanObsorbDamage -= (DWORD)TitanDamage;
	else														pResultInfo->TitanObsorbDamage = 0;

	if(pResultInfo->bDodge)
		OBJECTACTIONMGR->Dodge(pVictim,pAttacker);
	else
	{
		OBJECTACTIONMGR->Damage(pVictim,pAttacker,DamageKind,LifeDamage,ShieldDamage,pResultInfo->bCritical,pResultInfo->bDecisive,TitanDamage);
	}

	if(VampiricLife)
	{
		if(pAttacker->GetID() == HERO->GetID())
			HERO->ChangeLife(VampiricLife);
		else
			pAttacker->SetLife(pAttacker->GetLife()+VampiricLife);
	}
	if(VampiricNaeryuk)
	{
		pAttacker->SetNaeRyuk(pAttacker->GetNaeRyuk()+VampiricNaeryuk);
	}
	
	if(SumRate >= 0.98f)		// 데미지를 다 때렸다
	{
		if(pVictim->IsDied())		// 죽었다
		{
			OBJECTACTIONMGR->Die(pVictim,pAttacker,bFatalDamage,pResultInfo->bCritical,pResultInfo->bDecisive);	// 날라가서 죽기
		}
	}
}

void CDamageEffectUnit::DoOn(EFFECTPARAM* pParam,TARGETSET* pObject)
{
	CDamageEffectUnitDesc* pDesc = (CDamageEffectUnitDesc*)m_pEffectUnitDesc;
	Damage(pObject->pTarget, pParam->m_pOperator, &pObject->ResultInfo,
		pDesc->m_DamageRate, pParam->m_DamageRate, pDesc->m_DamageKind,
		pParam->m_dwFlag & EFFECT_FLAG_FATALDAMAGE);

	//////////////////////////////////////////////////////////////////////////
	// 반격 처리
	if(pObject->ResultInfo.CounterDamage)
	{
		RESULTINFO tempinfo;
		tempinfo.Clear();
		tempinfo.RealDamage = pObject->ResultInfo.CounterDamage;
		Damage(pParam->m_pOperator, pObject->pTarget, &tempinfo,
		pDesc->m_DamageRate, pParam->m_DamageRate, eDamageKind_Counter,
		FALSE);
	}
	//////////////////////////////////////////////////////////////////////////

}

void CDamageEffectUnit::DoProcess(DWORD tickTime,EFFECTPARAM* pParam)
{	
	if(m_bInited == FALSE)
		return;
	/*
	float FadeRate = 1;

	if(m_bFading)		// Fade시작..
	{
		if(tickTime >= m_FadeTime)
		{
			m_FadeTime = 0;
			FadeRate = 0;
			for(int n=0;n<m_nObjectNum;++n)
				m_DamageNumberArray[n].Clear();
		}
		else
		{
			m_FadeTime -= tickTime;
		}
		FadeRate = m_FadeTime / (float)pDesc->m_FadeDuration;
	}
	for(int n=0;n<m_nObjectNum;++n)
	{
		m_DamageNumberArray[n].SetAlpha(FadeRate);
	}
	*/
}
