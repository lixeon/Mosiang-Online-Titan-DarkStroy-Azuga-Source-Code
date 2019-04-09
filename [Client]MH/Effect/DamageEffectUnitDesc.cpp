// DamageEffectUnitDesc.cpp: implementation of the CDamageEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamageEffectUnitDesc.h"
#include "DamageEffectUnit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoryPoolTempl<CDamageEffectUnit>* CDamageEffectUnitDesc::m_pUnitPool = NULL;

CDamageEffectUnitDesc::CDamageEffectUnitDesc(BOOL bDangledToOperator)
: CEffectUnitDesc(eEffectUnit_Damage,bDangledToOperator)
{
	if(m_pUnitPool == NULL)
	{
		m_pUnitPool = new CMemoryPoolTempl<CDamageEffectUnit>;
		m_pUnitPool->Init(50,5,"CDamageEffectUnit");
	}
	m_DamageRate = 1;
	m_FadeDuration = 1000;
	m_DamageKind = eDamageKind_Front;
}

CDamageEffectUnitDesc::~CDamageEffectUnitDesc()
{
	if(m_pUnitPool)
	{
		m_pUnitPool->Release();
		delete m_pUnitPool;
		m_pUnitPool = NULL;
	}
}

void CDamageEffectUnitDesc::DoParseScript(char* szKey,CMHFile* pFile)
{
	CMD_ST(szKey);
		CMD_CS("#DURATION")
			m_FadeDuration = pFile->GetDword();
			
		CMD_CS("#PERCENT")
			m_DamageRate = pFile->GetFloat() * 0.01f;
			
		CMD_CS("#DAMAGEKIND")
			char* strKind = pFile->GetString();
			m_DamageKind = atoi(strKind);
			if(m_DamageKind == 0)	// 글자로 써놓은 경우
			{
				if(strcmp(strKind,"FRONT")==0)
					m_DamageKind = eDamageKind_Front;
				else if(strcmp(strKind,"LEFT")==0)
					m_DamageKind = eDamageKind_Left;
				else if(strcmp(strKind,"RIGHT")==0)
					m_DamageKind = eDamageKind_Right;
				else
					ASSERT(0);
			}
			/*
		CMD_CS("#RANGEKIND")
			char* strKind = pFile->GetString();
			m_RangeKind = atoi(strKind);
			strupr(strKind);
			if(m_RangeKind == 0)	// 글자로 써놓은 경우
			{
				if(strcmp(strKind,"SAMETIME")==0)
					m_RangeKind = eRangeDamageKind_SameTime;
				else if(strcmp(strKind,"FRONTTOBACK")==0)
					m_RangeKind = eRangeDamageKind_FrontToBack;
				else if(strcmp(strKind,"LEFTTORIGHT")==0)
					m_RangeKind = eRangeDamageKind_LeftToRight;
				else if(strcmp(strKind,"RIGHTTOLEFT")==0)
					m_RangeKind = eRangeDamageKind_RightToLeft;
				else if(strcmp(strKind,"CENTERTOEDGE")==0)
					m_RangeKind = eRangeDamageKind_CenterToEdge;
				else
					ASSERT(0);
			}
			*/

	CMD_EN
}

CEffectUnit* CDamageEffectUnitDesc::GetEffectUnit(EFFECTPARAM* pEffectParam)
{
	CDamageEffectUnit* pEffectUnit = m_pUnitPool->Alloc();
	pEffectUnit->Initialize(this,pEffectParam);
	
	return pEffectUnit;
}