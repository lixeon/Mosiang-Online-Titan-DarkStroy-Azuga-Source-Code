// SkillInfo.h: interface for the CSkillInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLINFO_H__DE3D09D4_1B17_47B4_A0AD_3EBF402EF280__INCLUDED_)
#define AFX_SKILLINFO_H__DE3D09D4_1B17_47B4_A0AD_3EBF402EF280__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtrList.h"
#include "MHFile.h"

class CHero;
class CSkillObject;
class CActionTarget;
class CSkillArea;

class CSkillInfo  
{
	cPtrList m_SkillObjectPool;
	SKILLINFO m_SkillInfo;
	
	DWORD m_SkillOperateAnimationTime[GENDER_MAX];		//시작동작시간
	
	WORD m_BeforeSkill;
	WORD m_AfterSkill;
	WORD m_SkillTree[5];

public:
	CSkillInfo();
	~CSkillInfo();

	BOOL InitSkillInfo(CMHFile* pFile);
	BOOL InitDummySkillInfo(CSkillInfo* pOriginalSkillInfo);
	BOOL CheckOriginal(CSkillInfo* pSkillInfo);

	CSkillObject* GetSkillObject();
	void ReleaseSkillObject(CSkillObject* pSObj,int kind);

	inline void SetSkillOperateAnimationTime(DWORD aniTimeMale,DWORD aniTimeFemale)
	{	m_SkillOperateAnimationTime[GENDER_MALE] = aniTimeMale;
	m_SkillOperateAnimationTime[GENDER_FEMALE] = aniTimeFemale;	}
	DWORD GetSkillOperateAnimationTime(int gender);

#ifdef _MAPSERVER_
	BOOL IsExcutableSkillState(CObject* pObject,int SkillLevel, SKILLOPTION* pOption);
	BOOL IsInSkillRange(CObject* pObject,VECTOR3* pTargetPos,float TargetRadius, SKILLOPTION* pOption);
	BOOL ComboTest(CObject* pObject);

	BOOL CheckEvade(MAINTARGET* pMainTarget);
#else
	BOOL IsExcutableSkillState(CHero* pHero,int SkillLevel, SKILLOPTION* pOption);
	BOOL IsInSkillRange(CHero* pHero,CActionTarget* pTarget, SKILLOPTION* pOption);
	BOOL ComboTest(CHero* pHero);
	BOOL IsValidTarget(CHero* pHero,CActionTarget* pTarget);
	BOOL ConvertTargetToSkill(CHero* pHero,CActionTarget* pTarget);
#endif


	// Get Skill Info
	inline SKILLINFO* GetSkillInfo()	{	return &m_SkillInfo;		}
	inline float GetSkillRange()		{	return m_SkillInfo.SkillRange;		}
	inline BOOL IsMugong()				{	return (m_SkillInfo.ComboNum == SKILL_COMBO_NUM);		}
	inline BOOL IsTargetAreaFitToHero(){	return (m_SkillInfo.TargetAreaPivot == 1);	}
	inline short GetSkillAreaIndex()	{	return m_SkillInfo.TargetAreaIdx;		}
	inline WORD GetSkillIndex()		{	return m_SkillInfo.SkillIdx;		}
	inline DWORD GetSkillContinueTime(){	return m_SkillInfo.Duration;	}
	inline char* GetSkillName()		{	return m_SkillInfo.SkillName;	}
	inline DWORD GetNeedExp(int sung)	{	return sung < 12 ? m_SkillInfo.NeedExp[sung] : -1;	}
	inline int GetLowImage()			{	return m_SkillInfo.LowImage;	}
	inline int GetHighImage()			{	return m_SkillInfo.HighImage;	}
	inline WORD GetWeaponType()		{	return m_SkillInfo.WeaponKind;	}
	inline WORD GetSkillKind()		{	return m_SkillInfo.SkillKind;	}
	inline WORD GetTieUpType()	{ return m_SkillInfo.TieUpType;	}
	inline WORD GetInterval() { return m_SkillInfo.Interval; }
	inline WORD GetAttrib() { return m_SkillInfo.Attrib;	}
	inline WORD GetPositiveResultTargetType() { return m_SkillInfo.PositiveResultTargetType;	}
	inline WORD GetNegativeResultTargetType() { return m_SkillInfo.NegativeResultTargetType;	}
	inline WORD GetDelayTime()	{	return (WORD)(m_SkillInfo.DelayTime);	}
	inline WORD GetCounterDodgeKind()	{	return m_SkillInfo.CounterDodgeKind;	}
	inline WORD GetCounterEffect()	{	return m_SkillInfo.CounterEffect;	}

//041213 KES decrease skill power
	inline int	GetDamageDecreaseForDist() { return m_SkillInfo.DamageDecreaseForDist; }
	
	inline float GetUpPhyAttack(int SkillLevel) { return m_SkillInfo.UpPhyAttack[SkillLevel-1];	}
	inline WORD GetContinueAttAttack(int SkillLevel) { return m_SkillInfo.ContinueAttAttack[SkillLevel-1];	}
	inline float GetContinueAttAttackRate(int SkillLevel) { return m_SkillInfo.ContinueAttAttackRate[SkillLevel-1];	}
	
	inline WORD GetContinueRecoverNaeRyuk(int SkillLevel)  { return m_SkillInfo.ContinueRecoverNaeRyuk[SkillLevel-1];	}
	inline WORD GetContinueRecoverLife(int SkillLevel)  { return m_SkillInfo.ContinueRecoverLife[SkillLevel-1];	}
	inline WORD GetContinueRecoverShield(int SkillLevel)  { return m_SkillInfo.ContinueRecoverShield[SkillLevel-1];	}

	inline WORD GetNeedNaeRyuk(int SkillLevel)	{	return m_SkillInfo.NeedNaeRyuk[SkillLevel-1];	}
	inline float GetFirstPhyAttack(int SkillLevel) { return m_SkillInfo.FirstPhyAttack[SkillLevel-1];	}
	inline float GetFirstAttAttack(int SkillLevel) { return m_SkillInfo.FirstAttAttack[SkillLevel-1];	}
	inline WORD GetFirstAttAttackMin(int SkillLevel) { return m_SkillInfo.FirstAttAttackMin[SkillLevel-1];	}
	inline WORD GetFirstAttAttackMax(int SkillLevel) { return m_SkillInfo.FirstAttAttackMax[SkillLevel-1];	}		
	inline float GetCounterDodgeRate(int SkillLevel)	{	return m_SkillInfo.CounterDodgeRate[SkillLevel-1];	}
	inline float GetStunRate(int SkillLevel)	{	return m_SkillInfo.StunRate[SkillLevel-1];	}	
	inline float GetCriticalRate(int SkillLevel)	{	return m_SkillInfo.CriticalRate[SkillLevel-1];	}	
	inline WORD GetStunTime(int SkillLevel)	{	return m_SkillInfo.StunTime[SkillLevel-1];	}	
	inline float GetDispelAttackFeelRate(int SkillLevel)	{	return m_SkillInfo.DispelAttackFeelRate[SkillLevel-1];	}
	
	inline float GetChangeSpeedProbability(int SkillLevel)	{	return m_SkillInfo.ChangeSpeedProbability[SkillLevel-1];	}
	
	inline WORD GetUpMaxNaeRyuk(int SkillLevel) { return m_SkillInfo.UpMaxNaeRyuk[SkillLevel-1];	}
	inline WORD GetUpMaxLife(int SkillLevel) { return m_SkillInfo.UpMaxLife[SkillLevel-1];	}
	inline WORD GetUpMaxShield(int SkillLevel) { return m_SkillInfo.UpMaxShield[SkillLevel-1];	}

	inline float GetUpPhyDefense(int SkillLevel) { return m_SkillInfo.UpPhyDefence[SkillLevel-1];	}
	inline float GetUpAttDefense(int SkillLevel) { return m_SkillInfo.UpAttDefence[SkillLevel-1];	}
	
	inline float GetVampiricLife(int SkillLevel) { return m_SkillInfo.VampiricLife[SkillLevel-1];	}
	inline float GetVampiricNaeryuk(int SkillLevel) { return m_SkillInfo.VampiricNaeryuk[SkillLevel-1];	}

	inline WORD GetAmplifiedPowerPhy(int SkillLevel) { return m_SkillInfo.AmplifiedPowerPhy[SkillLevel-1];	}
	inline WORD GetAmplifiedPowerAtt(int SkillLevel) { return m_SkillInfo.AmplifiedPowerAtt[SkillLevel-1];	}
	inline float GetAmplifiedPowerAttRate(int SkillLevel) { return m_SkillInfo.AmplifiedPowerAttRate[SkillLevel-1];	}
	
	inline BOOL IsLifeSkill()	{	return m_SkillInfo.Life ? TRUE : FALSE;	}
	
	inline WORD GetDownMaxNaeRyuk(int SkillLevel) { return m_SkillInfo.DownMaxNaeRyuk[SkillLevel-1];	}
	inline WORD GetDownMaxLife(int SkillLevel) { return m_SkillInfo.DownMaxLife[SkillLevel-1];	}
	inline WORD GetDownMaxShield(int SkillLevel) { return m_SkillInfo.DownMaxShield[SkillLevel-1];	}
	inline float GetDownPhyDefense(int SkillLevel) { return m_SkillInfo.DownPhyDefence[SkillLevel-1];	}
	inline float GetDownAttDefense(int SkillLevel) { return m_SkillInfo.DownAttDefence[SkillLevel-1];	}
	inline float GetDownPhyAttack(int SkillLevel) { return m_SkillInfo.DownPhyAttack[SkillLevel-1];	}

	inline float GetDamageRate(int SkillLevel) { return m_SkillInfo.DamageRate[SkillLevel-1]; }	
	inline float GetAttackRate(int SkillLevel) { return m_SkillInfo.AttackRate[SkillLevel-1]; }	

	inline float GetAttackLifeRate(int SkillLevel) { return m_SkillInfo.AttackLifeRate[SkillLevel-1]; }	
	inline float GetAttackShieldRate(int SkillLevel) { return m_SkillInfo.AttackShieldRate[SkillLevel-1]; }	
	inline float GetAttackSuccessRate(int SkillLevel) { return m_SkillInfo.AttackSuccessRate[SkillLevel-1]; }	

	inline float GetReverseVampiricLife(int SkillLevel)	{ return m_SkillInfo.VampiricReverseLife[SkillLevel-1]; }
	inline float GetReverseVampiricNaeryuk(int SkillLevel)	{ return m_SkillInfo.VampiricReverseNaeryuk[SkillLevel-1]; }

	inline float GetAttackPhyLastUp(int SkillLevel)	{ return (float)(m_SkillInfo.AttackPhyLastUp[SkillLevel-1]); }
	inline float GetAttackAttLastUp(int SkillLevel)	{ return (float)(m_SkillInfo.AttackAttLastUp[SkillLevel-1]); }
	
	BOOL IsMultipleTargetSkill();
	VECTOR3* GetTargetAreaPivotPos(VECTOR3* pOperatorPos,VECTOR3* pTargetPos);
	
#ifdef _MHCLIENT_
	void SendMsgToServer(CHero* pOperator,CActionTarget* pTarget);
#endif

	void SetSkillTree(WORD Before,WORD After,WORD* pArray);
	inline WORD GetBeforeSkill()	{	return m_BeforeSkill;	}
	inline WORD GetAfterSkill()	{	return m_AfterSkill;	}

	////////////////////////////////////////////////////////
	//06. 06 2차 전직 - 이영준
	//이펙트 생략(무초)
	WORD GetSkipEffectKind()	{ return m_SkillInfo.SkipEffect; }
	BOOL CanSkipEffect()	{ return m_SkillInfo.CanSkipEffect; }
	////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	// 경공과 같이 On/Off 형식의 자신의 상태 변화용
	WORD GetSpecialState()	{ return m_SkillInfo.SpecialState; }
	////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환
	// 변환 유형 - 0: 변환불가, 1: 공격형, 2:보조형, 3:모두가능
	WORD GetChangeKind() { return m_SkillInfo.ChangeKind; }
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	// 06. 08. 2차 보스 - 이영준
	// 추가 회전 각
	// 2차 보스 스킬중 공격하는 대상보다 조금 틀어진 방향을 향해야 하는 스킬을 위해 추가
	// 일반 몬스터도 사용가능
	int GetAddDegree() { return m_SkillInfo.AddDegree; }
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	// 06. 08. 2차 보스 - 이영준
	// 안전거리
	// 안전거리 안쪽은 타겟이 아니다
	WORD GetSafeRange() { return m_SkillInfo.SafeRange; }
	////////////////////////////////////////////////////////

	//WORD GetTitanLinkIdx()	{	return m_SkillInfo.LinkSkillIdx;	}
};

#endif // !defined(AFX_SKILLINFO_H__DE3D09D4_1B17_47B4_A0AD_3EBF402EF280__INCLUDED_)
