// AttackManager.h: interface for the CAttackManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATTACKMANAGER_H__90EBFECC_751A_4C43_AF61_AA0C5F5BAD34__INCLUDED_)
#define AFX_ATTACKMANAGER_H__90EBFECC_751A_4C43_AF61_AA0C5F5BAD34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AttackCalc.h"
#include "Object.h"


enum eATTACKABS_KIND
{
	eAAK_LIFE = 1,
	eAAK_SHIELD = 2,
	eAAK_NAERYUK = 4,
};



#define  ATTACKMGR CAttackManager::GetInstance()
class CAttackManager  
{
	CAttackCalc m_ATTACKCALC;
	float m_nDamageRate;	// magi82 cheat damage 데미지 관련 치트키

	DWORD GetComboPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
		RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate );
	DWORD GetMugongPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
		RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate );
	DWORD GetMugongAttrDamage(CObject* pAttacker,CObject* pTargetObject,
		WORD Attrib,DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate,float fCriticalRate,RESULTINFO* pDamageInfo,float fDecreaseDamageRate );

public:
	GETINSTANCE(CAttackManager);
	CAttackManager();
	virtual ~CAttackManager();

	// 최종 데미지를 넘겨준다. 다만 죽었을 경우엔 DIE 메세지를 뿌려준다.
	void Attack(BOOL bMugong, CObject* pAttacker,CObject* pTarget,DWORD AmplifiedPower,
				float PhyAttackRate,
				WORD Attrib,DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate,
				float fCriticalRate,
				RESULTINFO* pDamageInfo,BOOL bCounter, float fDecreaseDamageRate,
				WORD AmplifiedPowerAttrib, BOOL bContinueAttack = FALSE );
	
	void SetDamageRate(float nRate)	{	m_nDamageRate = nRate;	}	// magi82 cheat damage
	
	void RecoverLife(CObject* pOperator,CObject* pObject,WORD m_Attrib,DWORD RecoverLifeVal,RESULTINFO* pResultInfo);
	void RecoverNaeRyuk(CObject* pOperator,CObject* pObject,WORD m_Attrib,DWORD RecoverNaeRyukVal,RESULTINFO* pResultInfo);
	
	void sendDieMsg(CObject * pAttacker, CObject* pTarget);
	
	inline CAttackCalc& GetAttackCalc()		{	return m_ATTACKCALC;	}

	void AttackAbs( CObject* pAttacker, CObject* pTarget, int nAbsKind, float AttackRate, RESULTINFO* pDamageInfo );
	
	void AttackJinbub( CObject* pAttacker, CObject* pTarget, DWORD AttackPower, 
					   DWORD AttackMin,DWORD AttackMax, RESULTINFO* pDamageInfo,
					   float fDecreaseDamageRate );
	
	DWORD GetJinbubDamage(CObject* pAttacker,CObject* pTargetObject,DWORD AttackPower,
			RESULTINFO* pDamageInfo,float fDecreaseDamageRate);

	//2007. 10. 30. CBH - 데미지 패널티 처리 함수
	DWORD GetPenaltyDemege(CObject* pAttacker,CObject* pTargetObject, DWORD dwDemage);
};

#endif // !defined(AFX_ATTACKMANAGER_H__90EBFECC_751A_4C43_AF61_AA0C5F5BAD34__INCLUDED_)
