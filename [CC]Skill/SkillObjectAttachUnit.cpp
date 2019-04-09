// SkillObjectAttachUnit.cpp: implementation of the CSkillObjectAttachUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectAttachUnit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit::CSkillObjectAttachUnit()
{
#ifdef _MHCLIENT_	
	for(int n=0;n<MAX_SKILLMAINTAIN_EFFECT;++n)
	{
		m_AttachEffectNum[n] = 0;
		m_StateIconNum[n] = 0;
	}
#endif
	
	m_nAttachUnitKind = eATTACH_UNIT_KIND_UNKNOWN;
}

CSkillObjectAttachUnit::~CSkillObjectAttachUnit()
{

}
