// SoundEffectUnitDesc.cpp: implementation of the CSoundEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoundEffectUnitDesc.h"
#include "SoundEffectUnit.h"
#include "..\MHFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoryPoolTempl<CSoundEffectUnit>* CSoundEffectUnitDesc::m_pUnitPool = NULL;

CSoundEffectUnitDesc::CSoundEffectUnitDesc(BOOL bDangledToOperator)
: CEffectUnitDesc(eEffectUnit_Sound,bDangledToOperator)
{
	if(m_pUnitPool == NULL)
	{
		m_pUnitPool = new CMemoryPoolTempl<CSoundEffectUnit>;
		m_pUnitPool->Init(50,5,"CSoundEffectUnit");
	}

	m_SoundNum = 0;
}

CSoundEffectUnitDesc::~CSoundEffectUnitDesc()
{
	if(m_pUnitPool)
	{
		m_pUnitPool->Release();
		delete m_pUnitPool;
		m_pUnitPool = NULL;
	}

}

void CSoundEffectUnitDesc::DoParseScript(char* szKey,CMHFile* pFile)
{
	CMD_ST(szKey);
		CMD_CS("#SOUNDNUM")
			m_SoundNum = pFile->GetWord();

	CMD_EN
}

CEffectUnit* CSoundEffectUnitDesc::GetEffectUnit(EFFECTPARAM* pEffectParam)
{
	CSoundEffectUnit* pEffectUnit = m_pUnitPool->Alloc();
	pEffectUnit->Initialize(this,pEffectParam);
	
	return pEffectUnit;
}