// Stats.cpp: implementation of the CStats class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Stats.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStats::CStats()
{
	Clear();
}

CStats::~CStats()
{
	
}

void CStats::Clear()
{
	memset(&m_Stats,0,sizeof(player_calc_stats));
}

void CStats::AddOtherStats(CStats* pOtherStats)
{
	/*
	m_Stats.MeleeAttackMin += pOtherStats->m_Stats.MeleeAttackMax;		
	m_Stats.MeleeAttackMax += pOtherStats->m_Stats.MeleeAttackMax;
	m_Stats.RangeAttackMin += pOtherStats->m_Stats.RangeAttackMin;
	m_Stats.RangeAttackMax += pOtherStats->m_Stats.RangeAttackMax;
	m_Stats.AttributeAttack.FireElement += pOtherStats->m_Stats.AttributeAttack.FireElement;
	m_Stats.AttributeAttack.WaterElement += pOtherStats->m_Stats.AttributeAttack.WaterElement;
	m_Stats.AttributeAttack.TreeElement += pOtherStats->m_Stats.AttributeAttack.TreeElement;
	m_Stats.AttributeAttack.IronElement += pOtherStats->m_Stats.AttributeAttack.IronElement;
	m_Stats.AttributeAttack.EarthElement += pOtherStats->m_Stats.AttributeAttack.EarthElement;
	m_Stats.AttributeAttack.PoisonElement += pOtherStats->m_Stats.AttributeAttack.PoisonElement;
	m_Stats.PhysicalDefense += pOtherStats->m_Stats.PhysicalDefense;
	m_Stats.AttributeResist.FireElement += pOtherStats->m_Stats.AttributeResist.FireElement;
	m_Stats.AttributeResist.WaterElement += pOtherStats->m_Stats.AttributeResist.WaterElement;
	m_Stats.AttributeResist.TreeElement += pOtherStats->m_Stats.AttributeResist.TreeElement;
	m_Stats.AttributeResist.IronElement += pOtherStats->m_Stats.AttributeResist.IronElement;
	m_Stats.AttributeResist.EarthElement += pOtherStats->m_Stats.AttributeResist.EarthElement;
	m_Stats.AttributeResist.PoisonElement += pOtherStats->m_Stats.AttributeResist.PoisonElement;
	m_Stats.GenGol += pOtherStats->m_Stats.GenGol;
	m_Stats.MinChub += pOtherStats->m_Stats.MinChub;
	m_Stats.CheRyuk += pOtherStats->m_Stats.CheRyuk;

	m_Stats.ResolveLifePercent += pOtherStats->m_Stats.ResolveLifePercent;
	m_Stats.ResolveEnergyPercent += pOtherStats->m_Stats.ResolveEnergyPercent;	
	*/
}

void CStats::AddItemInfoStats(const ITEM_INFO* pItemInfo)
{
	/*
	switch(pItemInfo->EquipType)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_Stats.RangeKind = 0;
		break;
	case 4:
	case 5:
		m_Stats.RangeKind = 1;
		break;
	default:
		ASSERT(0);
	}
	
	m_Stats.MeleeAttackMin += pItemInfo->MeleeAttackMax;		
	m_Stats.MeleeAttackMax += pItemInfo->MeleeAttackMax;
	m_Stats.RangeAttackMin += pItemInfo->RangeAttackMin;
	m_Stats.RangeAttackMax += pItemInfo->RangeAttackMax;
	m_Stats.AttributeAttack.FireElement += pItemInfo->IncFireElementAttack;
	m_Stats.AttributeAttack.WaterElement += pItemInfo->IncWaterElementAttack;
	m_Stats.AttributeAttack.TreeElement += pItemInfo->IncTreeElementAttack;
	m_Stats.AttributeAttack.IronElement += pItemInfo->IncIronElementAttack;
	m_Stats.AttributeAttack.EarthElement += pItemInfo->IncEarthElementAttack;
	m_Stats.AttributeAttack.PoisonElement += pItemInfo->IncPoisonElementAttack;
	m_Stats.PhysicalDefense += pItemInfo->IncPhisicalDefence;
	m_Stats.AttributeResist.FireElement += pItemInfo->IncFireElementRegist;
	m_Stats.AttributeResist.WaterElement += pItemInfo->IncWaterElementRegist;
	m_Stats.AttributeResist.TreeElement += pItemInfo->IncTreeElementRegist;
	m_Stats.AttributeResist.IronElement += pItemInfo->IncIronElementRegist;
	m_Stats.AttributeResist.EarthElement += pItemInfo->IncEarthElementRegist;
	m_Stats.AttributeResist.PoisonElement += pItemInfo->IncPoisonElementRegist;
	m_Stats.GenGol += pItemInfo->IncGeunGol;
	m_Stats.MinChub += pItemInfo->IncMinChub;
	m_Stats.CheRyuk += pItemInfo->IncCheRyuk;

	m_Stats.ResolveLifePercent += pItemInfo->ResolveLifePercent;
	m_Stats.ResolveEnergyPercent += pItemInfo->ResolveEnergyPercent;
	*/
}

