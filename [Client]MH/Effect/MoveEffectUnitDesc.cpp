// MoveEffectUnitDesc.cpp: implementation of the CMoveEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoveEffectUnitDesc.h"
#include "MoveEffectUnit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoryPoolTempl<CMoveEffectUnit>* CMoveEffectUnitDesc::m_pUnitPool = NULL;

CMoveEffectUnitDesc::CMoveEffectUnitDesc(BOOL bDangledToOperator)
: CEffectUnitDesc(eEffectUnit_Move,bDangledToOperator)
{
	if(m_pUnitPool == NULL)
	{
		m_pUnitPool = new CMemoryPoolTempl<CMoveEffectUnit>;
		m_pUnitPool->Init(50,5,"CMoveEffectUnit");
	}

	m_bApplyWeight = TRUE;
	m_bYMove = FALSE;
	m_bTurnChar = FALSE;
}

CMoveEffectUnitDesc::~CMoveEffectUnitDesc()
{
	if(m_pUnitPool)
	{
		m_pUnitPool->Release();
		delete m_pUnitPool;
		m_pUnitPool = NULL;
	}

}

void CMoveEffectUnitDesc::DoParseScript(char* szKey,CMHFile* pFile)
{
	CMD_ST(szKey);
		CMD_CS("#DURATION")
			m_Duration = pFile->GetDword();
		CMD_CS("#APPLYWEIGHT")
			m_bApplyWeight = pFile->GetBool();
		CMD_CS("#YMOVE")
			m_bYMove = pFile->GetBool();
		CMD_CS("#START")
			m_Position.x = pFile->GetFloat();
		CMD_CS("#END")
			m_Position.y = pFile->GetFloat();
		CMD_CS("#TURN")
			m_bTurnChar = pFile->GetBool();		
	CMD_EN
}

CEffectUnit* CMoveEffectUnitDesc::GetEffectUnit(EFFECTPARAM* pEffectParam)
{
	CMoveEffectUnit* pEffectUnit = m_pUnitPool->Alloc();
	pEffectUnit->Initialize(this,pEffectParam);
	
	return pEffectUnit;
}