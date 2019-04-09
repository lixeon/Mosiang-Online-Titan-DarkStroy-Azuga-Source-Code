// Distribute_Random.h: interface for the CDistribute_Random class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTRIBUTE_RANDOM_H__1C01C3E8_DCEC_4A12_8A43_6FE06D9A48DB__INCLUDED_)
#define AFX_DISTRIBUTE_RANDOM_H__1C01C3E8_DCEC_4A12_8A43_6FE06D9A48DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DistributeWay.h"

#define DISTRIBUTERAND CDistribute_Random::GetInstance()

class CDistribute_Random : public CDistributeWay  
{
protected:
	CDistribute_Random();
public:
	GETINSTANCE(CDistribute_Random);
	virtual ~CDistribute_Random();

//	virtual void SendAbil(LEVELTYPE MonsterLevel, PARTY_RECEIVE_MEMBER* pRealMember);
	virtual void SendItem(PARTY_RECEIVE_MEMBER* pRealMember, WORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel);
};

#endif // !defined(AFX_DISTRIBUTE_RANDOM_H__1C01C3E8_DCEC_4A12_8A43_6FE06D9A48DB__INCLUDED_)
