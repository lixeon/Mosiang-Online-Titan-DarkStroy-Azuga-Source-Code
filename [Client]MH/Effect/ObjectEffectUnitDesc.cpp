// Objec	tEffectUnitDesc.cpp: implementation of the CObjectEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectEffectUnitDesc.h"
#include "..\Engine\EngineObject.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoryPoolTempl<CObjectEffectUnit>* CObjectEffectUnitDesc::m_pUnitPool = NULL;

CObjectEffectUnitDesc::CObjectEffectUnitDesc(BOOL bDangledToOperator)
: CEffectUnitDesc(eEffectUnit_Object,bDangledToOperator)
{
	if(m_pUnitPool == NULL)
	{
		m_pUnitPool = new CMemoryPoolTempl<CObjectEffectUnit>;
		m_pUnitPool->Init(50,5,"CObjectEffectUnit");
	}

	m_RandomPos.x = 0;
	m_RandomPos.y = 0;
	m_RandomPos.z = 0;
	m_Scale.x = 1;
	m_Scale.y = 1;
	m_Scale.z = 1;
	m_bRepeat = FALSE;
	m_fAlpha = 1;
	m_bApplyHeightField = TRUE;
	m_IllusionMaterial = NULL;
	
	
	m_StartFrame = 0;
	m_EndFrame = -1;

	m_AngleRad = 0;
	m_Illusion = 0;

// jsd test
	m_dwPattern = 0;
	SetVector3( &m_vStartAngle, 0.0f, 0.0f, 0.0f );
	m_fChangeVel = 0.0f;
}

CObjectEffectUnitDesc::~CObjectEffectUnitDesc()
{
	if(m_pUnitPool)
	{
		m_pUnitPool->Release();
		delete m_pUnitPool;
		m_pUnitPool = NULL;
	}

}

void CObjectEffectUnitDesc::DoParseScript(char* szKey,CMHFile* pFile)
{
	CMD_ST(szKey);			
		CMD_CS("#OBJECTNAME")
			m_ObjectName = pFile->GetString();
		
//			if( strcmp(m_ObjectName,TARGET) == 0 ||
//				strcmp(m_ObjectName,OPERATOR) == 0 )
//				return;

//			DIRECTORYMGR->SetLoadMode(eLM_Effect);
//			CEngineObject::PreLoadObject(m_ObjectName);
//			DIRECTORYMGR->SetLoadMode(eLM_Root);

		CMD_CS("#SCALE")
			m_Scale.x = pFile->GetFloat();
			m_Scale.y = pFile->GetFloat();
			m_Scale.z = pFile->GetFloat();

		CMD_CS("#REPEAT")
			m_bRepeat = pFile->GetBool();
		
		CMD_CS("#ALPHA")
			m_fAlpha = pFile->GetByte() * 0.01f;

		CMD_CS("#POSITION")
			if( m_Position.y != 0 )
				m_bApplyHeightField = FALSE;

		CMD_CS("#APPLYHEIGHTFIELD")
			m_bApplyHeightField = pFile->GetBool();
		
			if( m_Position.y != 0 )
				m_bApplyHeightField = FALSE;

		CMD_CS("#SETFRAME")
			m_StartFrame = pFile->GetWord();
			m_EndFrame = pFile->GetWord();

		CMD_CS("#ANGLE")
			m_AngleRad = DEGTORAD(pFile->GetFloat());

		CMD_CS("#PATTERN")
			m_dwPattern = pFile->GetDword();

		CMD_CS("#STARTANGLE")
			m_vStartAngle.x = DEGTORAD(pFile->GetFloat());
			m_vStartAngle.y = DEGTORAD(pFile->GetFloat());
			m_vStartAngle.z = DEGTORAD(pFile->GetFloat());

		CMD_CS("#CHANGEVEL")
			m_fChangeVel = pFile->GetFloat();
		
//		CMD_CS("#ILLUSION")
			m_Illusion = pFile->GetWord();
		
		CMD_CS("#ILLUSIONMATERIAL")
			m_IllusionMaterial = pFile->GetWord();

		CMD_CS("#OBJECTNAMECRITICAL")
			m_ObjectNameCritical = pFile->GetString();
			
	CMD_EN
}

CEffectUnit* CObjectEffectUnitDesc::GetEffectUnit(EFFECTPARAM* pEffectParam)
{
	CObjectEffectUnit* pEffectUnit = m_pUnitPool->Alloc();
	pEffectUnit->Initialize(this,pEffectParam);

	return pEffectUnit;
}