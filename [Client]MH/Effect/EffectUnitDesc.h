// EffectUnitDesc.h: interface for the CEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTUNITDESC_H__F1978154_6E2A_4368_94DC_A56976FB8BDF__INCLUDED_)
#define AFX_EFFECTUNITDESC_H__F1978154_6E2A_4368_94DC_A56976FB8BDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEffectUnit;
class CMHFile;

struct EFFECTPARAM;

enum
{
	eEffectUnit_Light,
	eEffectUnit_Object,
	eEffectUnit_Move,
	eEffectUnit_Sound,
	eEffectUnit_Camera,
	eEffectUnit_Damage,
	eEffectUnit_Animation,
};
class CEffectUnitDesc  
{
protected:
	BOOL m_bDangledToOperator;
	BOOL m_bOneTargetPos;

	VECTOR3		m_Position;
	VECTOR3		m_RandomPos;
	
	BYTE m_bRelatedCoordinate;
public:
#ifdef _DEBUG
	char m_EffFilename[MAX_PATH];
#endif
	
	CEffectUnitDesc(DWORD EffectUnitType,BOOL bDangledToOperator);
	virtual ~CEffectUnitDesc();

	void ParseScript(CMHFile* pFile);
	virtual void DoParseScript(char* szKey,CMHFile* pFile) {};
	virtual CEffectUnit* GetEffectUnit(EFFECTPARAM* pEffectParam) = 0;

	BOOL IsDangledToOperator()	{	return m_bDangledToOperator;	}
	
	friend class CEffectUnit;
	friend class CEffect;

	DWORD m_EffectUnitType;
};

#endif // !defined(AFX_EFFECTUNITDESC_H__F1978154_6E2A_4368_94DC_A56976FB8BDF__INCLUDED_)
