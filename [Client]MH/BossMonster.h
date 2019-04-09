// BossMonster.h: interface for the CBossMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOSSMONSTER_H__F4B18B29_BDC0_4858_B5AD_BEC837D5583B__INCLUDED_)
#define AFX_BOSSMONSTER_H__F4B18B29_BDC0_4858_B5AD_BEC837D5583B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Monster.h"

class CBossMonster : public CMonster  
{
	
public:
	CBossMonster();
	virtual ~CBossMonster();

	virtual void Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage);
	virtual void SetMotionInState(BYTE State);
	virtual void AddStatus(CStatus* pStatus);
};

#endif // !defined(AFX_BOSSMONSTER_H__F4B18B29_BDC0_4858_B5AD_BEC837D5583B__INCLUDED_)
