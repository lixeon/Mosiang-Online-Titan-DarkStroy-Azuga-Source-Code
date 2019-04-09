// AnimationEffectUnitDesc.cpp: implementation of the CAnimationEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AnimationEffectUnitDesc.h"
#include "AnimationEffectUnit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////.

CMemoryPoolTempl<CAnimationEffectUnit>* CAnimationEffectUnitDesc::m_pUnitPool = NULL;

CAnimationEffectUnitDesc::CAnimationEffectUnitDesc(BOOL bDangledToOperator)
: CEffectUnitDesc(eEffectUnit_Animation,bDangledToOperator)
{
	if(m_pUnitPool == NULL)
	{
		m_pUnitPool = new CMemoryPoolTempl<CAnimationEffectUnit>;
		m_pUnitPool->Init(50,5,"CAnimationEffectUnit");
	}
}

CAnimationEffectUnitDesc::~CAnimationEffectUnitDesc()
{
	if(m_pUnitPool)
	{
		m_pUnitPool->Release();
		delete m_pUnitPool;
		m_pUnitPool = NULL;
	}
}

void CAnimationEffectUnitDesc::DoParseScript(char* szKey,CMHFile* pFile)
{
	CMD_ST(szKey);
		CMD_CS("#MOTION")
			m_MotionNum = pFile->GetDword();
			m_CustumMotion = FALSE;
			m_bRepeat = FALSE;
		CMD_CS("#REPEAT")
			m_bRepeat = pFile->GetBool();
		CMD_CS("#FRAME")
			m_CustumMotion = pFile->GetBool();
			m_StartFrame = pFile->GetInt();
			m_EndFrame = pFile->GetInt();

	CMD_EN
}

CEffectUnit* CAnimationEffectUnitDesc::GetEffectUnit(EFFECTPARAM* pEffectParam)
{
	CAnimationEffectUnit* pEffectUnit = m_pUnitPool->Alloc();
	pEffectUnit->Initialize(this,pEffectParam);
	
	return pEffectUnit;
}