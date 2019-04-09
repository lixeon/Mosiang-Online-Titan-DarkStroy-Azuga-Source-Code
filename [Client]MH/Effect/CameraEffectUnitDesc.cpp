// CameraEffectUnitDesc.cpp: implementation of the CCameraEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CameraEffectUnitDesc.h"
#include "CameraEffectUnit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoryPoolTempl<CCameraEffectUnit>* CCameraEffectUnitDesc::m_pUnitPool = NULL;

CCameraEffectUnitDesc::CCameraEffectUnitDesc(BOOL bDangledToOperator)
: CEffectUnitDesc(eEffectUnit_Camera,bDangledToOperator)
{
	if(m_pUnitPool == NULL)
	{
		m_pUnitPool = new CMemoryPoolTempl<CCameraEffectUnit>;
		m_pUnitPool->Init(50,5,"CCameraEffectUnit");
	}

	m_CameraNum = -1;
	m_InitAngleY = 0;
	m_InitAngleY = 0;
	m_InitDistance = 500;
}

CCameraEffectUnitDesc::~CCameraEffectUnitDesc()
{
	if(m_pUnitPool)
	{
		m_pUnitPool->Release();
		delete m_pUnitPool;
		m_pUnitPool = NULL;
	}
}

void CCameraEffectUnitDesc::DoParseScript(char* szKey,CMHFile* pFile)
{
	CMD_ST(szKey);
		CMD_CS("#CAMERANUM")
			m_CameraNum = pFile->GetInt();
		
		CMD_CS("#ANGLEX")
			m_InitAngleX = pFile->GetFloat();

		CMD_CS("#ANGLEY")
			m_InitAngleY = pFile->GetFloat();
		
		CMD_CS("#DISTANCE")
			m_InitDistance = pFile->GetFloat();

	CMD_EN
}

CEffectUnit* CCameraEffectUnitDesc::GetEffectUnit(EFFECTPARAM* pEffectParam)
{
	ASSERT(m_CameraNum != -1);
	CCameraEffectUnit* pEffectUnit = m_pUnitPool->Alloc();
	pEffectUnit->Initialize(this,pEffectParam);
	
	return pEffectUnit;
}