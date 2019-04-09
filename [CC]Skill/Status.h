// Status.h: interface for the CStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUS_H__C222CF70_66F9_47E2_9E10_88DCF6E9EFFA__INCLUDED_)
#define AFX_STATUS_H__C222CF70_66F9_47E2_9E10_88DCF6E9EFFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STATUSCALC(Ori,Up,Dn)	((Ori + Up) - Dn)

#define STATUS_FLAG_LIFE		0x00000001
#define STATUS_FLAG_NAERYUK		0x00000010
#define STATUS_FLAG_DEFENCE		0x00000100
#define STATUS_FLAG_ATTACK		0x00001000
#define STATUS_FLAG_MOVE		0x00010000

class CStatus  
{
protected:
	DWORD m_Flag;
	
public:
	CStatus();
	virtual ~CStatus();
	
	inline DWORD GetFlag()	{	return m_Flag;	}

	virtual void GetMaxLife(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)	{}
	virtual void GetMaxShield(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)	{}
	virtual void GetMaxNaeRyuk(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)	{}
	virtual void GetPhyDefense(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)	{}
	virtual void GetAttDefense(WORD Attrib,float Original,float& CalcedUp,float& CalcedDown)	{}
	virtual void GetDamage(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)	{}
	virtual void GetPhyAttackPowerMin(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)	{}
	virtual void GetPhyAttackPowerMax(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)	{}
	virtual void GetUngiPlusRate(float Original,float& CalcedUp,float& CalcedDown)	{}
	virtual void GetAddAttackRange(float Original,float& CalcedUp,float& CalcedDown)	{}
	virtual void GetCritical(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)	{}
	virtual void GetDodgeRate(float Original,float& CalcedUp,float& CalcedDown)	{}
	
	virtual void GetCounterAttackDamage(WORD Attrib,DWORD AttrDamage,DWORD PhyDamage,RESULTINFO* pDamageInfo,DWORD& CounterDamage)	{}
	
	virtual void GetMoveSpeed(float Original,float& CalcedUp,float& CalcedDown)	{}

	virtual void GetVampiricLife(float& fRate)	{}
	virtual void GetVampiricNaeryuk(float& fRate)	{}

	virtual void GetReverseVampiricLife(float& fRate)	{}
	virtual void GetReverseVampiricNaeryuk(float& fRate)	{}
	
	virtual void GetAttackDamage(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)	{}

	virtual void GetAttackPhyDamagePlus(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)	{}
	virtual void GetAttackAttDamagePlus(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)	{}


	virtual void GetRecoverLife(DWORD& CalcedLife)	{}
	virtual void GetRecoverNaeryuk(DWORD& CalcedNaeryuk)	{}
	virtual void GetRecoverShield(DWORD& CalcedShield)	{}

	virtual void GetSkipEffectKind(WORD& SkipEffect)	{}
	virtual void GetDecisive(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)	{}
};

#endif // !defined(AFX_STATUS_H__C222CF70_66F9_47E2_9E10_88DCF6E9EFFA__INCLUDED_)
