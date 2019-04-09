// SkillObjectAttachUnit_Vampiric.cpp: implementation of the CSkillObjectAttachUnit_Vampiric class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectAttachUnit_Vampiric.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_Vampiric::CSkillObjectAttachUnit_Vampiric()
{

}

CSkillObjectAttachUnit_Vampiric::~CSkillObjectAttachUnit_Vampiric()
{

}

void CSkillObjectAttachUnit_Vampiric::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_VampLifeRate = pSkillInfo->GetVampiricLife(SkillLevel);
	m_VampNaeryukRate = pSkillInfo->GetVampiricNaeryuk(SkillLevel);
		
	//지속효과표시
//	if(m_VampLifeRate != 0)
//	{
//		m_StateIconNum[0] = eStatusIcon_VampiricLife;
//		m_AttachEffectNum[0] = 692;
//	}
//	if(m_VampNaeryukRate != 0)
//	{
//		m_StateIconNum[0] = eStatusIcon_VampiricNaeryuk;
//		m_AttachEffectNum[0] = 698;
//	}
}
	
void CSkillObjectAttachUnit_Vampiric::GetVampiricLife(float& fRate)
{
	if(m_VampLifeRate > fRate)
		fRate = m_VampLifeRate;
}

void CSkillObjectAttachUnit_Vampiric::GetVampiricNaeryuk(float& fRate)
{
	if(m_VampNaeryukRate > fRate)
		fRate = m_VampNaeryukRate;
}



//==============
//CSkillObjectAttachUnit_ReverseVampiric
//=============

CSkillObjectAttachUnit_ReverseVampiric::CSkillObjectAttachUnit_ReverseVampiric()
{
	m_VampLifeRate = 0.0f;
	m_VampNaeryukRate = 0.0f;
}

CSkillObjectAttachUnit_ReverseVampiric::~CSkillObjectAttachUnit_ReverseVampiric()
{

}

void CSkillObjectAttachUnit_ReverseVampiric::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_VampLifeRate		= pSkillInfo->GetReverseVampiricLife(SkillLevel);
	m_VampNaeryukRate	= pSkillInfo->GetReverseVampiricNaeryuk(SkillLevel);

#ifdef _MHCLIENT_	
	//지속효과표시
	if(m_VampLifeRate != 0)
	{
		m_StateIconNum[0] = eStatusIcon_ReverseVampiricLife;
	}
	if(m_VampNaeryukRate != 0)
	{
		m_StateIconNum[0] = eStatusIcon_ReverseVampiricNaeryuk;
	}
#endif
}
	
void CSkillObjectAttachUnit_ReverseVampiric::GetReverseVampiricLife(float& fRate)
{
	fRate = m_VampLifeRate;
}

void CSkillObjectAttachUnit_ReverseVampiric::GetReverseVampiricNaeryuk(float& fRate)
{
	fRate = m_VampNaeryukRate;
}


