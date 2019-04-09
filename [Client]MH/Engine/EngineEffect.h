// EngineEffect.h: interface for the CEngineEffect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINEEFFECT_H__047F1400_E930_4094_986E_D092224D5173__INCLUDED_)
#define AFX_ENGINEEFFECT_H__047F1400_E930_4094_986E_D092224D5173__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EngineObject.h"
#include <PtrList.h>

#define MAX_EFFECT_ANI_NUM	2

class CEngineEffect  : public CEngineObject
{
	BOOL m_bIsRepeat;  
	char m_ObjectName[64];

	BOOL IsRepeat()	{	return m_bIsRepeat;	}
public:
	CEngineEffect();
	virtual ~CEngineEffect();

	void SetRepeat(BOOL bv)	{	m_bIsRepeat = bv;	}

	BOOL Init(char* filename,CObjectBase* pObject,BYTE EngineObjectType,WORD StartFrame,WORD EndFrame);
	void Release();

	friend DWORD __stdcall MHEffectPROC(I4DyuchiGXExecutive* pExecutive, GXOBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData);
};

#endif // !defined(AFX_ENGINEEFFECT_H__047F1400_E930_4094_986E_D092224D5173__INCLUDED_)
