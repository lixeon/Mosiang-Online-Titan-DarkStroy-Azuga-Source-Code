// SpecialState.h: interface for the CSpecialState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPECIALSTATE_H__D4A09DE3_E811_4EE8_8ADE_290B0E97494A__INCLUDED_)
#define AFX_SPECIALSTATE_H__D4A09DE3_E811_4EE8_8ADE_290B0E97494A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _MHCLIENT_
#include "..\Effect\EffectManager.h"
#endif

class CObject;

class CSpecialState  
{
protected:
	CObject* m_pParent;
	DWORD m_StateTime;
	WORD m_wParam1;
	WORD m_wParam2;
	float m_fParam3;

#ifdef _MHCLIENT_
	HEFFPROC m_Effect;
	WORD m_EffectNum;
	WORD m_StateIcon;
#endif

public:
	CSpecialState(CObject* pParent);
	~CSpecialState();

	void Init();

	void Process(DWORD tickTime);

	virtual void StartState(DWORD Time,WORD wParam1,WORD wParam2,float fParam3,WORD EffectNum,WORD StateIcon,BOOL bHeroOper = FALSE);
	virtual void EndState();
	
	inline BOOL IsInState()	{	return m_StateTime ? TRUE : FALSE;	}

	virtual WORD GetParam1()	{	return m_wParam1;	}
	virtual WORD GetParam2()	{	return m_wParam2;	}
	virtual float GetParam3()	{	return m_fParam3;	}
};


#endif // !defined(AFX_SPECIALSTATE_H__D4A09DE3_E811_4EE8_8ADE_290B0E97494A__INCLUDED_)
