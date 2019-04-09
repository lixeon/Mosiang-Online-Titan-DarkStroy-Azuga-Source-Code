// CObjectActionManager.h: interface for the CObjectActionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTACTIONMANAGER_H__C167471E_D98E_42E2_88BB_0C7037BD0430__INCLUDED_)
#define AFX_OBJECTACTIONMANAGER_H__C167471E_D98E_42E2_88BB_0C7037BD0430__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define OBJECTACTIONMGR USINGTON(CObjectActionManager)
class CObject;

class CObjectActionManager  
{
public:
	//MAKESINGLETON(CObjectActionManager);
	CObjectActionManager();
	virtual ~CObjectActionManager();

	// Çàµ¿ ÇÔ¼öµé...
	void Die(CObject* pObject,CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive);
	void Damage(CObject* pObject,CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage);
	void Dodge(CObject* pObject,CObject* pAttacker);
	void Revive(CObject* pObject,VECTOR3* pRevivePos);
	void Heal(CObject* pObject,CObject* pHealer,BYTE DamageKind,DWORD HealVal);
	void Recharge(CObject* pObject,CObject* pRecharger,BYTE RechargeKind,DWORD RechargeVal);

	void ApplyTargetList(CObject* pOperator,CTargetList* pTList,BYTE DamageKind);
	void ApplyResult(CObject* pObject,CObject* pOperator,RESULTINFO* pRInfo,BYTE DamageKind);
	
};
EXTERNGLOBALTON(CObjectActionManager);
#endif // !defined(AFX_OBJECTACTIONMANAGER_H__C167471E_D98E_42E2_88BB_0C7037BD0430__INCLUDED_)
