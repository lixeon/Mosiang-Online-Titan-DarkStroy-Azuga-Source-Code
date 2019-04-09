// EffectTrigger.h: interface for the CEffectTrigger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGER_H__413120EB_3D74_4DB8_918F_4A3F26CCF717__INCLUDED_)
#define AFX_EFFECTTRIGGER_H__413120EB_3D74_4DB8_918F_4A3F26CCF717__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEffectTriggerUnitDesc;
class CEffectUnit;
class CEffect;

#define MAX_TRIGGER_NUM		64

class CEffectTrigger  
{
	int m_TriggerUnitNum;
	CEffectTriggerUnitDesc** m_ppEffectTriggerUnitArray;
	bool m_pExecuted[MAX_TRIGGER_NUM];

public:
	CEffectTrigger();
	virtual ~CEffectTrigger();
	
	void Init(int TriggerNum,CEffectTriggerUnitDesc** pEffectTriggerUnitArray);
	void Release();

	friend class CEffectTriggerDesc;

	void Process(DWORD ElapseTime,CEffect* pEffect);
	void ExecuteAll(CEffect* pEffect);
	
	inline int GetUnitNum()	{	return m_TriggerUnitNum;	}
	inline CEffectTriggerUnitDesc* GetEffectTriggerUnitDesc(int n)	{	return m_ppEffectTriggerUnitArray[n];	}
	
	void Reset();
};

#endif // !defined(AFX_EFFECTTRIGGER_H__413120EB_3D74_4DB8_918F_4A3F26CCF717__INCLUDED_)
