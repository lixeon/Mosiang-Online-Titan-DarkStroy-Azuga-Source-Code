// LightEffectUnitDesc.cpp: implementation of the CLightEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LightEffectUnitDesc.h"

#include "LightEffectUnit.h"
#include "..\MHFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMemoryPoolTempl<CLightEffectUnit>* CLightEffectUnitDesc::m_pUnitPool = NULL;


CLightEffectUnitDesc::CLightEffectUnitDesc(BOOL bDangledToOperator)
: CEffectUnitDesc(eEffectUnit_Light,bDangledToOperator)
{
	if(m_pUnitPool == NULL)
	{
		m_pUnitPool = new CMemoryPoolTempl<CLightEffectUnit>;
		m_pUnitPool->Init(50,5,"CLightEffectUnit");
	}

	m_RandomPos.x = 0;
	m_RandomPos.y = 0;
	m_RandomPos.z = 0;

	m_ColorIndex = 0;
}

CLightEffectUnitDesc::~CLightEffectUnitDesc()
{
	if(m_pUnitPool)
	{
		m_pUnitPool->Release();
		delete m_pUnitPool;
		m_pUnitPool = NULL;
	}
}

void CLightEffectUnitDesc::DoParseScript(char* szKey,CMHFile* pFile)
{
	CMD_ST(szKey);			
		CMD_CS("#COLORINDEX")
			m_ColorIndex = pFile->GetDword();
			
		CMD_CS("#RADIUS")
			m_fRadius = pFile->GetFloat();
	CMD_EN
}

CEffectUnit* CLightEffectUnitDesc::GetEffectUnit(EFFECTPARAM* pEffectParam)
{
	CLightEffectUnit* pEffectUnit = m_pUnitPool->Alloc();
	pEffectUnit->Initialize(this,pEffectParam);
	
	return pEffectUnit;
}