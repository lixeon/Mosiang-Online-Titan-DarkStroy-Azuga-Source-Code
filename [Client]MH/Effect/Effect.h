// Effect.h: interface for the CEffect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECT_H__389F4965_B0EA_4A59_BEF7_2A3FF488DF0F__INCLUDED_)
#define AFX_EFFECT_H__389F4965_B0EA_4A59_BEF7_2A3FF488DF0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTrigger.h"

class CEffectUnit;
class CEffectDesc;
class CObject;

struct TARGETSET
{
	TARGETSET()
	{
		pTarget = NULL;
	}
	CObject* pTarget;
	RESULTINFO ResultInfo;
};

struct EFFECTPARAM
{
	DWORD m_StartTime;
	CObject* m_pOperator;
	//SW050913 추가 이펙트 렌더 옵션 관련
	CObject* m_pSkillOperator;
	//TARGETSET m_Operator;
	

	int m_nTargetNum;
	TARGETSET m_pTargetSet[MAX_TARGET_NUM];
	MAINTARGET m_MainTarget;
	VECTOR3 m_AddPos;

	DWORD m_dwFlag;
	float m_DamageRate;

	EFFECTPARAM()
	{
		m_DamageRate = 0;
		m_AddPos.x = m_AddPos.y = m_AddPos.z = 0;
		m_nTargetNum = 0;
		m_pSkillOperator = NULL;
	}
	MAINTARGET* GetMainTarget()
	{
		return &m_MainTarget;
	}

	void Copy(EFFECTPARAM* pParam)
	{
		this->m_DamageRate = pParam->m_DamageRate;
		this->m_StartTime = pParam->m_StartTime;
		this->m_pOperator = pParam->m_pOperator;
		this->m_pSkillOperator = pParam->m_pSkillOperator;
		this->m_nTargetNum = pParam->m_nTargetNum;
		this->m_dwFlag = pParam->m_dwFlag;
		this->m_MainTarget = pParam->m_MainTarget;
		this->m_AddPos = pParam->m_AddPos;
		if(m_nTargetNum)
		{
			memcpy(m_pTargetSet,pParam->m_pTargetSet,sizeof(TARGETSET)*m_nTargetNum);		
		}
	}
	void SetTargetSetArray(TARGETSET* pTargetSetArray,int MaxTargetSet,MAINTARGET* pMainTarget);
};

#define MAX_EFFECT_UNITNUM	32

class CEffect  
{
	BOOL m_EndFlag;
	EFFECTPARAM m_Param;

	DWORD m_EffectID;
	DWORD m_EffectRefCount;

	CEffectDesc* m_pEffectDesc;

	CEffectUnit* m_ppEffectUnitArray[MAX_EFFECT_UNITNUM];
	CEffectTrigger m_EffectTrigger;

	int m_MaxEffectUnitNum;
	void SetEffectUnit(int num,CEffectUnit* pEffectUnit);

public:
	CEffect();
	virtual ~CEffect();

	void Init(int maxEffectUnitNum,EFFECTPARAM* pParam,CEffectDesc* pEffectDesc,
				 int TriggerNum,CEffectTriggerUnitDesc** ppEffectTriggerUnitArray);
	void Release();

	BOOL Process(DWORD CurTime);

	CEffectUnit* GetEffectUnit(int n)	{	return m_ppEffectUnitArray[n];	}
	EFFECTPARAM* GetEffectParam()		{	return &m_Param;				}

	void SetDamage(TARGETSET* TargetSetArray,int MaxTargetSet,MAINTARGET* pMainTarget);

	DWORD GetEffectFlag()	{	return m_Param.m_dwFlag;	}
	DWORD GetEffectEndTime();

	DWORD GetOperatorAnimatioEndTime();
	
	void Reset();

	BOOL IsEndFlag()	{	return m_EndFlag;	}
	void SetEndFlag()	{	m_EndFlag = TRUE;	}
	BOOL IsRepeatEffect();
	DWORD GetNextEffect();

	void SetEffectID(DWORD id)	{	m_EffectID = id;	}
	DWORD GetEffectID()	{	return m_EffectID;	}

	void IncRefCount()	{	m_EffectRefCount++;	}
	void DecRefCount()	{	m_EffectRefCount--;	}
	DWORD GetRefCount()	{	return m_EffectRefCount;	}
	void SetRefCount(DWORD refcount)	{	m_EffectRefCount = refcount;	}
	
	int GetEffectKind();

	friend class CEffectDesc;
};

#endif // !defined(AFX_EFFECT_H__389F4965_B0EA_4A59_BEF7_2A3FF488DF0F__INCLUDED_)
