// TargetListIterator.cpp: implementation of the CTargetListIterator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TargetListIterator.h"

#include "TargetList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTargetListIterator::CTargetListIterator()
{
	m_CurMode = TLMODE_NONE;
}
CTargetListIterator::CTargetListIterator(CTargetList* pTargetList)
{
	Init(pTargetList);
}

CTargetListIterator::~CTargetListIterator()
{
	ASSERT(m_CurMode == TLMODE_NONE);
}

void CTargetListIterator::Init(CTargetList* pTargetList)
{
	m_pTargetList = pTargetList;
	m_PositionOfList = 0;
	m_pCurTargetData = NULL;
	m_DataNum = 0;

	m_CurMode = TLMODE_INITED;
}

void CTargetListIterator::Release()
{
	m_pTargetList->SetBufLen(m_PositionOfList);
	m_CurMode = TLMODE_NONE;
}

void CTargetListIterator::AddTarget(DWORD id,WORD Flag,BYTE kind)
{
	ShiftToNext();
	
	m_pCurTargetData->SetTarget(id,Flag,kind);
	m_DataNum = 0;
}

void CTargetListIterator::AddTargetData(DWORD data)
{
	ASSERT(m_pCurTargetData);
	m_pCurTargetData->SetTargetData(m_DataNum++,data);

	m_PositionOfList += m_pCurTargetData->GetOneDataSize();
}


void CTargetListIterator::AddTargetWithNoData(DWORD id,BYTE kind)
{
	AddTarget(id,0,kind);
}
	
void CTargetListIterator::AddTargetWithResultInfo(DWORD id,BYTE kind,RESULTINFO* pResultInfo)
{
	WORD flag = 0;
	if(pResultInfo->bDispelFromEvil)	flag |= TARGET_FLAG_DISPELFROMEVIL;
	if(pResultInfo->bDodge)	flag |= TARGET_FLAG_DODGE;
	if(pResultInfo->bCritical)	flag |= TARGET_FLAG_CRITICAL;
	if(pResultInfo->RealDamage)	flag |= TARGET_FLAG_LIFEDAMAGE;
	if(pResultInfo->ShieldDamage)	flag |= TARGET_FLAG_SHIELDDAMAGE;
	if(pResultInfo->CounterDamage)	flag |= TARGET_FLAG_COUNTERDAMAGE;
	if(pResultInfo->HealLife)	flag |= TARGET_FLAG_HEALLIFE;
	if(pResultInfo->RechargeNaeryuk)	flag |= TARGET_FLAG_RECHARGE;
	if(pResultInfo->StunTime)	flag |= TARGET_FLAG_STUN;
	if(pResultInfo->Vampiric_Life)	flag |= TARGET_FLAG_VAMPIRIC_LIFE;
	if(pResultInfo->Vampiric_Naeryuk)	flag |= TARGET_FLAG_VAMPIRIC_NR;
	if(pResultInfo->bDecisive)	flag |= TARGET_FLAG_DECISIVE;
	if(pResultInfo->TitanObsorbDamage)	flag |= TARGET_FLAG_TITAN_OBSORB_DAMAGE;
	
	AddTarget(id,flag,kind);
	
	if(flag&TARGET_FLAG_LIFEDAMAGE)		AddTargetData(pResultInfo->RealDamage);
	if(flag&TARGET_FLAG_SHIELDDAMAGE)	AddTargetData(pResultInfo->ShieldDamage);
	if(flag&TARGET_FLAG_COUNTERDAMAGE)	AddTargetData(pResultInfo->CounterDamage);
	if(flag&TARGET_FLAG_HEALLIFE)		AddTargetData(pResultInfo->HealLife);
	if(flag&TARGET_FLAG_RECHARGE)		AddTargetData(pResultInfo->RechargeNaeryuk);
	if(flag&TARGET_FLAG_STUN)			AddTargetData(pResultInfo->StunTime);
	if(flag&TARGET_FLAG_VAMPIRIC_LIFE)	AddTargetData(pResultInfo->Vampiric_Life);
	if(flag&TARGET_FLAG_VAMPIRIC_NR)	AddTargetData(pResultInfo->Vampiric_Naeryuk);
	if(flag&TARGET_FLAG_TITAN_OBSORB_DAMAGE)	AddTargetData(pResultInfo->TitanObsorbDamage);
}

BOOL CTargetListIterator::GetNextTarget()
{
	if(m_PositionOfList >= m_pTargetList->GetBufLen())
		return FALSE;

	ShiftToNext();
	
	WORD flag = GetTargetFlag();

	if(flag&TARGET_FLAG_LIFEDAMAGE)		m_PositionOfList += m_pCurTargetData->GetOneDataSize();
	if(flag&TARGET_FLAG_SHIELDDAMAGE)	m_PositionOfList += m_pCurTargetData->GetOneDataSize();
	if(flag&TARGET_FLAG_COUNTERDAMAGE)	m_PositionOfList += m_pCurTargetData->GetOneDataSize();
	if(flag&TARGET_FLAG_HEALLIFE)		m_PositionOfList += m_pCurTargetData->GetOneDataSize();
	if(flag&TARGET_FLAG_RECHARGE)		m_PositionOfList += m_pCurTargetData->GetOneDataSize();
	if(flag&TARGET_FLAG_STUN)			m_PositionOfList += m_pCurTargetData->GetOneDataSize();
	if(flag&TARGET_FLAG_VAMPIRIC_LIFE)	m_PositionOfList += m_pCurTargetData->GetOneDataSize();
	if(flag&TARGET_FLAG_VAMPIRIC_NR)	m_PositionOfList += m_pCurTargetData->GetOneDataSize();
	if(flag&TARGET_FLAG_TITAN_OBSORB_DAMAGE)	m_PositionOfList += m_pCurTargetData->GetOneDataSize();
	
	m_DataNum = 0;
	return TRUE;
}

DWORD CTargetListIterator::GetTargetID()
{
	ASSERT(m_pCurTargetData);
	return m_pCurTargetData->GetTargetID();	
}

WORD CTargetListIterator::GetTargetFlag()
{
	ASSERT(m_pCurTargetData);
	return m_pCurTargetData->GetTargetFlag();
}

BYTE CTargetListIterator::GetTargetKind()
{
	ASSERT(m_pCurTargetData);
	return m_pCurTargetData->GetTargetKind();
}

void CTargetListIterator::GetTargetData(RESULTINFO* pResultInfo)
{
	pResultInfo->Clear();
	WORD flag = GetTargetFlag();
	if(flag&TARGET_FLAG_DISPELFROMEVIL)	pResultInfo->bDispelFromEvil = TRUE;
	if(flag&TARGET_FLAG_DODGE)			pResultInfo->bDodge = TRUE;
	if(flag&TARGET_FLAG_CRITICAL)		pResultInfo->bCritical = TRUE;
	if(flag&TARGET_FLAG_LIFEDAMAGE)		pResultInfo->RealDamage = m_pCurTargetData->GetTargetData(m_DataNum++);
	if(flag&TARGET_FLAG_SHIELDDAMAGE)	pResultInfo->ShieldDamage = m_pCurTargetData->GetTargetData(m_DataNum++);
	if(flag&TARGET_FLAG_COUNTERDAMAGE)	pResultInfo->CounterDamage = m_pCurTargetData->GetTargetData(m_DataNum++);
	if(flag&TARGET_FLAG_HEALLIFE)		pResultInfo->HealLife = m_pCurTargetData->GetTargetData(m_DataNum++);
	if(flag&TARGET_FLAG_RECHARGE)		pResultInfo->RechargeNaeryuk = m_pCurTargetData->GetTargetData(m_DataNum++);
	if(flag&TARGET_FLAG_STUN)			pResultInfo->StunTime = m_pCurTargetData->GetTargetData(m_DataNum++);
	if(flag&TARGET_FLAG_VAMPIRIC_LIFE)	pResultInfo->Vampiric_Life = m_pCurTargetData->GetTargetData(m_DataNum++);
	if(flag&TARGET_FLAG_VAMPIRIC_NR)	pResultInfo->Vampiric_Naeryuk = m_pCurTargetData->GetTargetData(m_DataNum++);
	if(flag&TARGET_FLAG_DECISIVE)		pResultInfo->bDecisive = TRUE;
	if(flag&TARGET_FLAG_TITAN_OBSORB_DAMAGE)	pResultInfo->TitanObsorbDamage = m_pCurTargetData->GetTargetData(m_DataNum++);
}

void CTargetListIterator::ShiftToNext()
{
	m_pCurTargetData = m_pTargetList->GetTargetData(m_PositionOfList);
	m_PositionOfList += m_pCurTargetData->GetBaseTargetDataSize();
}
