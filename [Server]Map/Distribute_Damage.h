// Distribute_Damage.h: interface for the CDistribute_Damage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTRIBUTE_DAMAGE_H__7AD90AB5_DC90_4B01_8880_E2AB6180AE08__INCLUDED_)
#define AFX_DISTRIBUTE_DAMAGE_H__7AD90AB5_DC90_4B01_8880_E2AB6180AE08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DistributeWay.h"

#define DISTRIBUTEDAM CDistribute_Damage::GetInstance()

class CDistribute_Damage : public CDistributeWay  
{
protected:	
	CDistribute_Damage();
public:
	GETINSTANCE(CDistribute_Damage);
	virtual ~CDistribute_Damage();

//	virtual void SendAbil(LEVELTYPE MonsterLevel, PARTY_RECEIVE_MEMBER* pRealMember);
	virtual void SendItem(PARTY_RECEIVE_MEMBER* pRealMember, WORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel);

};

#endif // !defined(AFX_DISTRIBUTE_DAMAGE_H__7AD90AB5_DC90_4B01_8880_E2AB6180AE08__INCLUDED_)
