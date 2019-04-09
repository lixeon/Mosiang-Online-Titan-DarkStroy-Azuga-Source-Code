// EffectUnitDesc.cpp: implementation of the CEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectUnitDesc.h"

#include "EffectUnit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectUnitDesc::CEffectUnitDesc(DWORD EffectUnitType,BOOL bDangledToOperator)
{
	m_EffectUnitType = EffectUnitType;
	
	m_bDangledToOperator = bDangledToOperator;
	
	m_Position.x = 0;
	m_Position.y = 0;
	m_Position.z = 0;

	m_RandomPos.x = 0;
	m_RandomPos.y = 0;
	m_RandomPos.z = 0;
	
	m_bRelatedCoordinate = 1;
	
	m_bOneTargetPos = FALSE;
}

CEffectUnitDesc::~CEffectUnitDesc()
{

}


void CEffectUnitDesc::ParseScript(CMHFile* pFile)
{
#ifdef _DEBUG
	strcpy(m_EffFilename,pFile->GetFileName());
#endif

	char buf[128];
	pFile->GetString(buf);	

	while(1)
	{
		pFile->GetString(buf);
		

		CMD_ST(buf);
			CMD_CS("}")
				break;
			CMD_CS("#POSITION")
				m_Position.x = pFile->GetFloat();
				m_Position.y = pFile->GetFloat();
				m_Position.z = pFile->GetFloat();

			CMD_CS("#RANDOM")
				m_RandomPos.x = pFile->GetFloat();
				m_RandomPos.y = pFile->GetFloat();
				m_RandomPos.z = pFile->GetFloat();

			CMD_CS("#COORDINATE")
				m_bRelatedCoordinate = pFile->GetByte();

			CMD_CS("#ONETARGET")
				m_bOneTargetPos = pFile->GetBool();
	
		CMD_EN

		DoParseScript(buf,pFile);
	}	
}