// AttackCalc.cpp: implementation of the CAttackCalc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttackCalc.h"
#include "..\[CC]Header\CommonCalcFunc.h"
#include <time.h>
#include "PartyManager.h"
#include "Party.h"

#ifdef _JAPAN_LOCAL_
#include "..\[CC]BattleSystem\Battle.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAttackCalc::CAttackCalc()
{
	srand((unsigned)time(NULL));
}

CAttackCalc::~CAttackCalc()
{
	
}
DWORD CAttackCalc::GetPlayerPoint(LEVELTYPE level1, int leve_gab)
{
	//////////////////////////////////////////////////////////////////////////
	// ?Ã“Â½Ãƒ Â¶Â«Â»Â§ //½«µÈ¼¶ÏÞÖÆ99µÄ¹ÖÎï====ÕâÀïËÆºõÓÐÎÊÌâ,´æÒÉ
	if(level1 == MAX_CHARACTER_LEVEL_NUM) //99
		return 0;
	//////////////////////////////////////////////////////////////////////////
	
	ASSERT(level1 >= 1);
	if(leve_gab < -MONSTERLEVELRESTRICT_LOWSTARTNUM)
		leve_gab = -MONSTERLEVELRESTRICT_LOWSTARTNUM;
	else if(leve_gab >= MAX_MONSTERLEVELPOINTRESTRICT_NUM)
		leve_gab  = MAX_MONSTERLEVELPOINTRESTRICT_NUM;

	return GAMERESRCMNGR->GetPLAYERxMONSTER_POINT(level1,leve_gab);
}

DWORD CAttackCalc::GetPlayerExpPoint( int level_gap, DWORD MonsterExp )
{
	//level_gap .. Ä³¸¯ÅÍ ·¹º§ - ¸ó½ºÅÍ ·¹º§

	float Exp = 0.f;
	if( level_gap < -8 )		// -9¡é
	{
		Exp = (float)(MonsterExp * 1.5);
	}
	else if( -9 < level_gap && level_gap < 1 )	// 0 ~ -8
	{
		Exp = MonsterExp + MonsterExp * -level_gap * 0.05f;
	}
	else if( 0 < level_gap && level_gap < 5 )	// 1, 2, 3, 4
	{
		Exp = MonsterExp * ( 5 - level_gap ) * 0.2f;	//°ø½ÄÀÌ...?
	}
	else if( level_gap == 5 )	// 5
	{
		Exp = MonsterExp * 0.1f;
	}
	else if( level_gap > 5 )		// 5¡è
		return 0;
	else
		return 0;

	if( (DWORD)(Exp * 10) > (DWORD)Exp * 10 )	//¼Ò¼öÁ¡ Ã¹Â°ÀÚ¸®ºÎÅÍ ºñ±³ //¿Ã¸²
	{
		return (DWORD)Exp + 1;
	}
	else
		return (DWORD)Exp;
}

BOOL	CAttackCalc::getCritical(CObject* pAttacker,CObject* pTarget,float fCriticalRate)
{
	DWORD attackercritical = pAttacker->GetCritical();

#ifdef _JAPAN_LOCAL_
	LEVELTYPE targetlevel = pTarget->GetLevel();
	LEVELTYPE attackerlevel = pAttacker->GetLevel();

	float fCri = (float)( attackercritical + 20 ) / (float)( targetlevel * 5 + 100 );
	if( fCri > 0.15f ) fCri = 0.15f;

	if( attackerlevel < targetlevel )
	{
		fCri = fCri + fCriticalRate - (float)(targetlevel - attackerlevel)*0.02f;
		if( fCri < 0.f )	fCri = 0.f;
	}
	else
	{
		fCri = fCri + fCriticalRate + (float)(attackerlevel - targetlevel)*0.004f;
	}

	float fRand = (float)(rand()%100) / 100.f;
		
	if( fCri < fRand )
		return FALSE;
	else
		return TRUE;

//	float fCriticalrate = (attackercritical + 20.f) / (targetlevel * 20.f + 300.f);

//	if(fCriticalrate > 0.2f)
//		fCriticalrate = 0.2f;
	
//	WORD wCriticalPercent = (WORD)(fCriticalrate * 100);

//	if(fCriticalRate)
//		wCriticalPercent += GetPercent(fCriticalRate,pAttacker->GetLevel(),pTarget->GetLevel());

//	return (rand()%100 < wCriticalPercent);	

#else
	LEVELTYPE targetlevel = pTarget->GetLevel();

	float fCriticalrate = (attackercritical + 20.f) / (targetlevel * 20.f + 300.f);
	if(fCriticalrate > 0.2f)
		fCriticalrate = 0.2f;
	
	WORD wCriticalPercent = (WORD)(fCriticalrate * 100);

	if(fCriticalRate)
		wCriticalPercent += GetPercent(fCriticalRate,pAttacker->GetLevel(),pTarget->GetLevel());

	// magi82 - UniqueItem(070629)
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		if( ((int)wCriticalPercent + ((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriRate) < 0 )
		{
			wCriticalPercent = 0;
		}
		else
		{
			wCriticalPercent += ((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriRate;
		}
	}

	return (rand()%100 < wCriticalPercent);
#endif
}

BOOL	CAttackCalc::getDecisive(CObject* pAttacker,CObject* pTarget,float fCriticalRate)
{
	DWORD attackercritical = pAttacker->GetDecisive();

#ifdef _JAPAN_LOCAL_
	LEVELTYPE targetlevel = pTarget->GetLevel();
	LEVELTYPE attackerlevel = pAttacker->GetLevel();

	float fCri = (float)( attackercritical + 20 ) / (float)( targetlevel * 5 + 100 );
	if( fCri > 0.15f ) fCri = 0.15f;

	if( attackerlevel < targetlevel )
	{
		fCri = fCri + fCriticalRate - (float)(targetlevel - attackerlevel)*0.02f;
		if( fCri < 0.f )	fCri = 0.f;
	}
	else
	{
		fCri = fCri + fCriticalRate + (float)(attackerlevel - targetlevel)*0.004f;
	}

	float fRand = (float)(rand()%100) / 100.f;
		
	if( fCri < fRand )
		return FALSE;
	else
		return TRUE;

//	float fCriticalrate = (attackercritical + 20.f) / (targetlevel * 20.f + 300.f);

//	if(fCriticalrate > 0.2f)
//		fCriticalrate = 0.2f;
	
//	WORD wCriticalPercent = (WORD)(fCriticalrate * 100);

//	if(fCriticalRate)
//		wCriticalPercent += GetPercent(fCriticalRate,pAttacker->GetLevel(),pTarget->GetLevel());

//	return (rand()%100 < wCriticalPercent);	

#else
	LEVELTYPE targetlevel = pTarget->GetLevel();

	float fCriticalrate = (attackercritical + 20.f) / (targetlevel * 20.f + 300.f);
	if(fCriticalrate > 0.2f)
		fCriticalrate = 0.2f;
	
	WORD wCriticalPercent = (WORD)(fCriticalrate * 100);

	if(fCriticalRate)
		wCriticalPercent += GetPercent(fCriticalRate,pAttacker->GetLevel(),pTarget->GetLevel());

	// magi82 - UniqueItem(070629)
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		if( ((int)wCriticalPercent + ((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriRate) < 0 )
		{
			wCriticalPercent = 0;
		}
		else
		{
			wCriticalPercent += ((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriRate;
		}
	}

	return (rand()%100 < wCriticalPercent);
#endif
}

double CAttackCalc::getPlayerPhysicalAttackPower(CPlayer * pPlayer,float PhyAttackRate, BOOL bCritical )
{
	double physicalAttackPower = 0;
	DWORD minVal,maxVal;
	minVal = pPlayer->GetPhyAttackPowerMin();
	maxVal = pPlayer->GetPhyAttackPowerMax();	

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	// ÀÏ¹Ý °ø°Ý·Â Çâ»ó
	float val = 1 + pPlayer->GetSkillStatsOption()->BaseAtk;

	if(val < 0)
		val = 0.0f;

	minVal = (DWORD)((minVal * val) + 0.5);
	maxVal = (DWORD)((maxVal * val) + 0.5);
	//////////////////////////////////////////////////////////////////////////
	
	if(maxVal<=minVal)
	{
		physicalAttackPower = minVal;
	}
	else
	{		
		DWORD gap = maxVal - minVal + 1;
		physicalAttackPower = minVal+rand()%gap;
	}
	

	physicalAttackPower = physicalAttackPower*PhyAttackRate;

#ifndef _JAPAN_LOCAL_	//ÀÏº»ÀÇ °æ¿ì´Â Å©¸®Æ¼ÄÃÀ» 2.25¹è°¡ ¾Æ´Ñ 1.5¹è·Î Á¤»óÀûÀ¸·Î ¼öÁ¤µÈ´Ù.
	if(bCritical)
	{
		physicalAttackPower = physicalAttackPower*1.5f;			// critical attack
	}
#endif
	
	return physicalAttackPower;
}

double CAttackCalc::getPlayerAttributeAttackPower(CPlayer * pPlayer, 
							WORD Attrib, DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate)
{
	DWORD MinV=0,MaxV=0;

//	ASSERT(AttAttackMax >= AttAttackMin);
	if( AttAttackMax < AttAttackMin )
	{
		char buf[128] = { 0, };
		sprintf( buf, "Min:%d, Max:%d, Attrib:%d, AttAttackRate:%f",
			AttAttackMin, AttAttackMax, Attrib, AttAttackRate );
		ASSERTMSG( 0, buf );
	}

#ifdef _JAPAN_LOCAL_

	if(AttAttackRate > 0 || AttAttackMax > 0 )
	{
		WORD SimMek = pPlayer->GetSimMek();

		DWORD MinLVV = (pPlayer->GetLevel()+5) - 5;		// RateMin = (Level + 5) - 5
		DWORD MaxLVV = (pPlayer->GetLevel()+5) + 5;		// RateMax = (Level + 5) + 5

//--- ÀÏº» ¹æÆÄ½Ã »ó»ý°ü°è¿¡ µû¸¥ °ø°Ý·Â Áõ°¡
/*
		if( pPlayer->IsPartyHelp() && pPlayer->GetBattle()->GetBattleKind() != eBATTLE_KIND_VIMUSTREET )
		{
			MinLVV = MinLVV + MinLVV * 20 / 100;
			MaxLVV = MaxLVV + MaxLVV * 20 / 100;
		}
*/
		//¾ÆÀÌÅÛ + Æ¯±âÀÇ ¼Ó¼º°ø°Ý·Â
		DWORD AttribPlus = (DWORD)pPlayer->GetAttribPlusPercent(Attrib);

		MinV = ( MinLVV + AttribPlus + SimMek/2 ) * AttAttackRate + AttAttackMin;
		MaxV = ( MaxLVV + AttribPlus + SimMek/2 ) * AttAttackRate + AttAttackMax;
	}

#else
	if(AttAttackRate > 0)
	{
		// Â¼Ã“Â¼Âº Percent Â°Ã¸Â°ÃÂ·Ã‚
		//[Â¼Ã“Â¼ÂºRateÂ°Ã¸Â°ÃÂ·Ã‚] * ( ([Â½Ã‰Â¸Ã†]+200) /100 ) + [Â½Ã‰Â¸Ã†]/5
		WORD SimMek = pPlayer->GetSimMek();
		double midtermVal = (double)(SimMek + 200)/(double)100;
		DWORD MinLVV = (pPlayer->GetLevel()+5) - 5;		// RateMin = (Level + 5) - 5
		DWORD MaxLVV = (pPlayer->GetLevel()+5) + 5;		// RateMax = (Level + 5) + 5
		MinV = DWORD((MinLVV * AttAttackRate * midtermVal) + SimMek/5 + min(SimMek-12,25));
		MaxV = DWORD((MaxLVV * AttAttackRate * midtermVal) + SimMek/5 + min(SimMek-12,25));
		
//		double dS = SimMek;
		//Â³Â»Â°Ã¸ Â°Ã¸Â°ÃÂ·Ã‚ = [ Â³Â»Â°Ã¸ Â°Ã¸Â°ÃÂ·Ã‚ * {(Â½Ã‰Â¸Ã† +200)/200}*{(Â½Ã‰Â¸Ã†+1000)/500}+Â½Ã‰Â¸Ã†]*0.6
//		MinV = (MinLVV * ((dS+200)/200)*((dS+1000)/500)+dS)*0.6;
//		MaxV = (MaxLVV * ((dS+200)/200)*((dS+1000)/500)+dS)*0.6;
	}


	// Â¼Ã“Â¼Âº ?Ã½Â´Ã« Â°Ã¸Â°ÃÂ·Ã‚
	MinV += AttAttackMin;
	MaxV += AttAttackMax;

	// Â¼Ã“Â¼Âº Â°Ã¸Â°ÃÂ·Ã‚ Â»Ã³Â½Ã‚ Â´ÃµÃ‡Ã˜ÃÃœ(Â¾Ã†?ÃŒÃ…Ã›,Ã†Â¯Â±Ã¢)
	float AttUp = 1 + pPlayer->GetAttribPlusPercent(Attrib);
	MinV = (DWORD)(AttUp * MinV);
	MaxV = (DWORD)(AttUp * MaxV);
#endif
	
	DWORD AttackPower = random(MinV,MaxV);
	return AttackPower;
}

// monster

double	CAttackCalc::getMonsterPhysicalAttackPower(CMonster * pMonster, float PhyAttackRate, BOOL bCritical)
{
	BYTE bAttackType = MONSTER_ATTACKTYPE1;
	
	double physicalAttackPower= 0;
	DWORD minVal = pMonster->GetPhyAttackPowerMin();
	DWORD maxVal = pMonster->GetPhyAttackPowerMax();	

	if(maxVal<=minVal)
	{
		physicalAttackPower = minVal;
	}
	else
	{		
		DWORD gap = maxVal - minVal + 1;
		physicalAttackPower = minVal+rand()%gap;
	}
	
	return physicalAttackPower;

/*	const monster_stats * mon_stats = pMonster->GetMonsterStats();
	double physicalAttackPower= 0;

	if(bAttackType == MONSTER_ATTACKTYPE1)
	{
		physicalAttackPower = mon_stats->PhysicalAttackMin1;
		int tmp = mon_stats->PhysicalAttackMax1-mon_stats->PhysicalAttackMin1;
		if(tmp) physicalAttackPower += rand()%tmp;
	}
	else if(bAttackType == MONSTER_ATTACKTYPE2)
	{
		physicalAttackPower = mon_stats->PhysicalAttackMin2;
		int tmp = mon_stats->PhysicalAttackMax2-mon_stats->PhysicalAttackMin2;
		if(tmp) physicalAttackPower += rand()%tmp;
	}

	return physicalAttackPower;*/
}

double	CAttackCalc::getMonsterAttributeAttackPower(CMonster * pMonster, WORD Attrib, DWORD AttAttackMin,DWORD AttAttackMax)
{
	monster_stats * mon_stats = pMonster->GetMonsterStats();
	
	ASSERT(AttAttackMax >= AttAttackMin);

	DWORD gap = AttAttackMax - AttAttackMin + 1;
	return AttAttackMin+rand()%gap;
}

//SW070127 Å¸ÀÌÅº
double	CAttackCalc::getTitanPhysicalAttackPower( CTitan* pTitan, CPlayer* pPlayer, float PhyAttackRate, BOOL bCritical )
{
	double physicalAttackPower = 0;
	DWORD minVal,maxVal;

	minVal = pTitan->GetPhyAttackPowerMin();
	maxVal = pTitan->GetPhyAttackPowerMax();

	if(maxVal<=minVal)
	{
		physicalAttackPower = minVal;
	}
	else
	{		
		DWORD gap = maxVal - minVal + 1;
		physicalAttackPower = minVal+rand()%gap;
	}

	physicalAttackPower = physicalAttackPower*PhyAttackRate;

	if(bCritical)
	{
		physicalAttackPower = physicalAttackPower*1.5f;			// critical attack
	}

	return physicalAttackPower;
}

double	CAttackCalc::getTitanAttributeAttackPower( CTitan* pTitan, CPlayer* pPlayer, WORD Attrib, DWORD AttAttackMin, DWORD AttAttackMax, float AttAttackRate )
{
	DWORD MinV=0,MaxV=0;

	if( AttAttackMax < AttAttackMin )
	{
		char buf[128] = { 0, };
		sprintf( buf, "Min:%d, Max:%d, Attrib:%d, AttAttackRate:%f",
			AttAttackMin, AttAttackMax, Attrib, AttAttackRate );
		ASSERTMSG( 0, buf );
	}

	titan_calc_stats* pStats = pTitan->GetItemStats();
	WORD ownerSimMek = pPlayer->GetSimMek();

	DWORD MinPwr = DWORD(( pStats->AttributeAttack.GetElement_Val(Attrib) * (ownerSimMek + 100)/400 + ownerSimMek/5 ) * 0.74f);
	DWORD MaxPwr = DWORD(( pStats->AttributeAttack.GetElement_Val(Attrib) * (ownerSimMek + 100)/400 + ownerSimMek/5 ) * 0.74f);


	DWORD AttackPower = random(MinPwr,MaxPwr);

	//return AttackPower;
	return AttackPower * AttAttackRate;
}

double	CAttackCalc::getPhysicalAttackPower(CObject * pObject,float PhyAttackRate, BOOL bCritical )
{
	/*if(pObject->GetObjectKind() == eObjectKind_Player)
		return getPlayerPhysicalAttackPower((CPlayer*)pObject,PhyAttackRate,bCritical );*/
	//SW070127 Å¸ÀÌÅº
	if( pObject->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pPlayer = (CPlayer*)pObject;
		if(pPlayer->InTitan())
		{
			double titanPwr = getTitanPhysicalAttackPower( pPlayer->GetCurTitan(), pPlayer, PhyAttackRate, bCritical );
			double masterPwr = getPlayerPhysicalAttackPower( pPlayer, PhyAttackRate, bCritical );
			double finalPwr = titanPwr + masterPwr * 0.6f;

			return finalPwr;
		}
		else
		{
			return getPlayerPhysicalAttackPower( pPlayer, PhyAttackRate, bCritical );
		}
	}
	else if(pObject->GetObjectKind() & eObjectKind_Monster)
		return getMonsterPhysicalAttackPower((CMonster*)pObject,PhyAttackRate,bCritical);
	else
	{
		ASSERT(0);
		return 0;
	}
}
double	CAttackCalc::getAttributeAttackPower(CObject * pObject, WORD Attrib, DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate)
{
	/*if(pObject->GetObjectKind() == eObjectKind_Player)
		return getPlayerAttributeAttackPower((CPlayer*)pObject,Attrib,AttAttackMin,AttAttackMax,AttAttackRate);*/
	//SW070127 Å¸ÀÌÅº
	if( pObject->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pPlayer = (CPlayer*)pObject;
		if(pPlayer->InTitan())
		{
			double titanPwr = getTitanAttributeAttackPower( pPlayer->GetCurTitan(), pPlayer, Attrib, AttAttackMin, AttAttackMax, AttAttackRate );
			double masterPwr = getPlayerAttributeAttackPower(pPlayer,Attrib,AttAttackMin,AttAttackMax,AttAttackRate);
			double finalPwr = titanPwr + masterPwr * 0.6f;

			return finalPwr;
		}
		else
			return getPlayerAttributeAttackPower((CPlayer*)pObject,Attrib,AttAttackMin,AttAttackMax,AttAttackRate);
	}
	else if(pObject->GetObjectKind() & eObjectKind_Monster)
		return getMonsterAttributeAttackPower((CMonster*)pObject,Attrib,AttAttackMin,AttAttackMax);
	else
	{
		ASSERT(0);
		return 0;
	}
}

double	CAttackCalc::getPhyDefenceLevel(CObject* pObject, CObject* pAttacker)
{
	double phyDefence = pObject->GetPhyDefense();
	
	// magi82 - UniqueItem(070626)
	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		phyDefence = phyDefence * ( 1.0f - (((CPlayer*)pAttacker)->GetUniqueItemStats()->nEnemyDefen * 0.01f) );
	}

	// RaMa - 05.02.04  -> ´ëÁöÀÇ ºÎÀû
	if( pObject->GetObjectKind() == eObjectKind_Player )
	{
		phyDefence += (phyDefence*((CPlayer*)pObject)->GetShopItemStats()->RegistPhys)/100;

		//////////////////////////////////////////////////////////////////////////
		// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
		// ¹«°ø º¯È¯ Ãß°¡
		// ¹°¸®¹æ¾î·Â
		float val = 1.0f + ((CPlayer*)pObject)->GetSkillStatsOption()->PhyDef;

		if(val < 0)
			val = 0.0f;

		phyDefence = phyDefence * val;
		//////////////////////////////////////////////////////////////////////////

		if( ((CPlayer*)pObject)->GetPartyIdx() )
		{
			CParty* pParty = PARTYMGR->GetParty( ((CPlayer*)pObject)->GetPartyIdx() );
			if( pParty )
			{
				int count = pParty->GetMemberCountofMap( pObject->GetID() );
				if( count && gPartyEvent[ePartyEvent_DefenceRate].Rate[count-1] )
					phyDefence = (DWORD)(phyDefence*gPartyEvent[ePartyEvent_DefenceRate].Rate[count-1]);
			}
		}
	}
	
	LEVELTYPE AttackerLevel = pAttacker->GetLevel();

#ifdef _JAPAN_LOCAL_
	double phyDefenceLevel = ( phyDefence*2.0 ) / ( (double)AttackerLevel*50 );
	if(phyDefenceLevel < 0.0 )
	{
		phyDefenceLevel = 0.0;
	}
	else if(phyDefenceLevel > 0.99)
		phyDefenceLevel = 0.99;
#else
	double phyDefenceLevel = (phyDefence*2.0 + 50) / ( AttackerLevel*20 + 150 );

	if(phyDefenceLevel < 0.0 )
	{
		ASSERT(0);
		phyDefenceLevel = 0;
	}

	if(phyDefenceLevel > 0.9)
		phyDefenceLevel = 0.9;
#endif

	//SW070127 Å¸ÀÌÅº
	if( pObject->GetObjectKind() == eObjectKind_Player )
	{
		if( ((CPlayer*)pObject)->InTitan() )
		{
			WORD titanDefense =	((CPlayer*)pObject)->GetTitanManager()->GetTitanStats()->PhysicalDefense;
			WORD shopItemTitanDefense = 0;	//Ãß°¡ ÀÛ¾÷

			WORD totalTitanDefense = (titanDefense + shopItemTitanDefense);

			DWORD	tmp = ( ( ((CPlayer*)pObject)->GetLevel() * 30 + 75 ) * 2 );
			phyDefenceLevel = (totalTitanDefense + 25) / ( ( pAttacker->GetLevel() * 30 + 75 ) * 2 );

			phyDefenceLevel = (titanDefense > 0.8 ? 0.8 : titanDefense);
		}
	}

	return phyDefenceLevel;
}
