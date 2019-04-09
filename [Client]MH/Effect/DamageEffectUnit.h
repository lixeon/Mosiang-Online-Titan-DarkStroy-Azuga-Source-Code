// DamageEffectUnit.h: interface for the CDamageEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAMAGEEFFECTUNIT_H__2DB40F66_7AFA_4FBA_BA12_D06C62EAACDD__INCLUDED_)
#define AFX_DAMAGEEFFECTUNIT_H__2DB40F66_7AFA_4FBA_BA12_D06C62EAACDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnit.h"
#include "DamageNumber.h"

class CDamageEffectUnit : public CEffectUnit  
{
	struct DAMAGEORDER
	{
		DWORD Time;
		float DistFromPivot;
		WORD TargetIndex;		
	};

protected:	
	int m_nObjectNum;
	DAMAGEORDER* m_DamageOrder;

	void SortDamageOrder(DWORD nTargetNum,TARGETSET* pTargetArray);

	
	void Damage(CObject* pVictim,CObject* pAttacker,RESULTINFO* pResultInfo,
							   float CurRate,float SumRate,BYTE DamageKind,BOOL bFatalDamage);

public:
	virtual void Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam);
	virtual void Release();
	
	static void GetDamageNumberVelocity(VECTOR3* pRtVelocity,CObject* pObject,DWORD DamageKind);

	CDamageEffectUnit();
	virtual ~CDamageEffectUnit();

	friend class CDamageEffectUnitDesc;

	void InitEffect(DWORD nTargetNum,TARGETSET* pTargetArray);

	virtual BOOL On(EFFECTPARAM* pParam);

	virtual void DoOn(EFFECTPARAM* pParam,TARGETSET* pObject);

	void DoProcess(DWORD tickTime,EFFECTPARAM* pParam);
};

#endif // !defined(AFX_DAMAGEEFFECTUNIT_H__2DB40F66_7AFA_4FBA_BA12_D06C62EAACDD__INCLUDED_)
