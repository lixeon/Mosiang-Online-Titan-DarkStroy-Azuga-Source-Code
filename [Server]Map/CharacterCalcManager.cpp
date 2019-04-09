// CharacterCalcManager.cpp: implementation of the CCharacterCalcManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterCalcManager.h"
#include "Player.h"
#include "KyungGongManager.h"
#include "CharMove.h"
#include "BossMonster.h"
#include "WeatherManager.h"
#include "SkillManager_Server.h"
#include "PartyManager.h"
#include "Party.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharacterCalcManager::CCharacterCalcManager()
{
}

CCharacterCalcManager::~CCharacterCalcManager()
{

}

void CCharacterCalcManager::StartUpdateLife( CObject* pObject, DWORD plusLife, BYTE recoverInterval, DWORD recoverTime )
{
	DWORD MaxVal = pObject->GetMaxLife();
	DWORD CurVal = pObject->GetLife();
	DWORD DamagedVal = MaxVal-CurVal;
	if(DamagedVal < plusLife)
		plusLife = DamagedVal;

	pObject->m_YYLifeRecoverTime.bStart = TRUE;
	pObject->m_YYLifeRecoverTime.count = recoverInterval;														// 10번에 걸쳐 회복한다
	pObject->m_YYLifeRecoverTime.recoverUnitAmout = plusLife/pObject->m_YYLifeRecoverTime.count;	// 한번에 회복될 양
	pObject->m_YYLifeRecoverTime.recoverDelayTime = recoverTime/pObject->m_YYLifeRecoverTime.count;		// 6초간 회복이 완료된다
	pObject->m_YYLifeRecoverTime.lastCheckTime = 0;													// 처음은 바로 회복한다.

	UpdateLife( pObject );
}

void CCharacterCalcManager::StartUpdateShield( CObject* pObject, DWORD plusShield, BYTE recoverInterval, DWORD recoverTime )
{
	DWORD MaxVal = pObject->GetMaxShield();
	DWORD CurVal = pObject->GetShield();
	DWORD DamagedVal = MaxVal-CurVal;
	if(DamagedVal < plusShield)
		plusShield = DamagedVal;

	pObject->m_YYShieldRecoverTime.bStart = TRUE;
	pObject->m_YYShieldRecoverTime.count = recoverInterval;														// 10번에 걸쳐 회복한다
	pObject->m_YYShieldRecoverTime.recoverUnitAmout = plusShield/pObject->m_YYShieldRecoverTime.count;	// 한번에 회복될 양
	pObject->m_YYShieldRecoverTime.recoverDelayTime = recoverTime/pObject->m_YYShieldRecoverTime.count;		// 6초간 회복이 완료된다
	pObject->m_YYShieldRecoverTime.lastCheckTime = 0;													// 처음은 바로 회복한다.

	UpdateShield( pObject );
}

void CCharacterCalcManager::StartUpdateNaeRyuk( CPlayer* pPlayer, DWORD plusNaeRyuk )
{
	DWORD MaxVal = pPlayer->GetMaxNaeRyuk();
	DWORD CurVal = pPlayer->GetNaeRyuk();
	DWORD DamagedVal = MaxVal-CurVal;
	if(DamagedVal < plusNaeRyuk)
		plusNaeRyuk = DamagedVal;

	pPlayer->m_YYNaeRyukRecoverTime.bStart = TRUE;
	pPlayer->m_YYNaeRyukRecoverTime.count = RECOVERINTERVALUNIT;																// 10번에 걸쳐 회복한다
	pPlayer->m_YYNaeRyukRecoverTime.recoverUnitAmout = plusNaeRyuk/pPlayer->m_YYNaeRyukRecoverTime.count;	// 한번에 회복될 양
	pPlayer->m_YYNaeRyukRecoverTime.recoverDelayTime = RECOVERTIMEUNIT/pPlayer->m_YYNaeRyukRecoverTime.count;		// 1초간 회복이 완료된다
	pPlayer->m_YYNaeRyukRecoverTime.lastCheckTime = 0;														// 처음은 바로 회복한다.

	UpdateNaeRyuk( pPlayer );
}
void CCharacterCalcManager::UpdateNaeRyuk(CPlayer* pPlayer)
{

	if( pPlayer->GetState() == eObjectState_Die )
		return;

	if( pPlayer->m_YYNaeRyukRecoverTime.bStart && pPlayer->m_YYNaeRyukRecoverTime.count )
	{
		if(pPlayer->m_YYNaeRyukRecoverTime.recoverDelayTime + pPlayer->m_YYNaeRyukRecoverTime.lastCheckTime < gCurTime )
		{
			pPlayer->m_YYNaeRyukRecoverTime.lastCheckTime = gCurTime;
			// 내력 증가
			pPlayer->AddNaeRyuk( pPlayer->m_YYNaeRyukRecoverTime.recoverUnitAmout, NULL );
			--pPlayer->m_YYNaeRyukRecoverTime.count;
			if( pPlayer->m_YYNaeRyukRecoverTime.count ==0 )
			{
				pPlayer->m_YYNaeRyukRecoverTime.bStart = FALSE;
				//CObjectEvent::ObjectEvent(pPlayer, OE_LEVELUP, &m_HeroCharacterInfo.Level);
			}
		}
	}
}

void CCharacterCalcManager::UpdateLife(CObject* pObject)
{
	if( pObject->GetState() == eObjectState_Die )
		return;

	if( pObject->m_YYLifeRecoverTime.bStart && pObject->m_YYLifeRecoverTime.count )
	{
		if(pObject->m_YYLifeRecoverTime.recoverDelayTime + pObject->m_YYLifeRecoverTime.lastCheckTime < gCurTime )
		{
			pObject->m_YYLifeRecoverTime.lastCheckTime = gCurTime;
			// 생명력 증가
			pObject->AddLife( pObject->m_YYLifeRecoverTime.recoverUnitAmout, NULL );
			--pObject->m_YYLifeRecoverTime.count;
			if( pObject->m_YYLifeRecoverTime.count ==0 )
			{
				pObject->m_YYLifeRecoverTime.bStart = FALSE;
				//CObjectEvent::ObjectEvent(pObject, OE_LIFERECOVERCOMPLETED, NULL);
			}
		}
	}
}

void CCharacterCalcManager::UpdateShield(CObject* pObject)
{
	if( pObject->GetState() == eObjectState_Die )
		return;

	if( pObject->m_YYShieldRecoverTime.bStart && pObject->m_YYShieldRecoverTime.count )
	{
		if(pObject->m_YYShieldRecoverTime.recoverDelayTime + pObject->m_YYShieldRecoverTime.lastCheckTime < gCurTime )
		{
			pObject->m_YYShieldRecoverTime.lastCheckTime = gCurTime;
			pObject->AddShield( pObject->m_YYShieldRecoverTime.recoverUnitAmout );
			--pObject->m_YYShieldRecoverTime.count;
			if( pObject->m_YYShieldRecoverTime.count ==0 )
			{
				pObject->m_YYShieldRecoverTime.bStart = FALSE;
			}
		}
	}
}

void CCharacterCalcManager::CalcMaxLife(CPlayer * pPlayer)
{
	DWORD MaxLife = 0;
	WORD Sta = pPlayer->GetCheRyuk();
	LEVELTYPE level = pPlayer->GetLevel();

#ifdef _JAPAN_LOCAL_
	WORD Gen = pPlayer->GetGenGol();
	//[레벨] * 20 + [체력] * 5 + [근골] * 2
	MaxLife = ( level * 20 ) + ( Sta * 5 ) + ( Gen * 2 );
#else
	//[레벨] * 5 + [체력] * 10
	MaxLife = ( level * 5 ) + ( Sta * 10 );	
#endif
	
	// + 아이템 가산
	MaxLife += pPlayer->GetItemStats()->MaxLife;

	//2007. 6. 13. CBH -  세트아이탬 가산 
	MaxLife += pPlayer->GetSetItemStats()->dwLife;
	
	// + 특기 가산
	MaxLife += pPlayer->GetAbilityStats()->LifeUp;

	// RaMa - 04.11.17   -> ShopItemOption횄횩째징
	MaxLife += pPlayer->GetShopItemStats()->Life;

	// RaMa - 05.02.16   AvatarOption
	MaxLife += pPlayer->GetAvatarOption()->Life;

	MaxLife += pPlayer->GetSkillStatsOption()->Life;

	// magi82 - UniqueItem(070629)
	if( ((int)MaxLife + pPlayer->GetUniqueItemStats()->nHp) > 0 )
	{
		MaxLife += pPlayer->GetUniqueItemStats()->nHp;
	}
	else
	{
		MaxLife = 1;
	}

	pPlayer->SetMaxLife(MaxLife);
	
	pPlayer->CheckLifeValid();
}
void CCharacterCalcManager::CalcMaxShield(CPlayer * pPlayer)
{
	DWORD MaxShield = 0;
	WORD Simmek = pPlayer->GetSimMek();
	WORD Gengol = pPlayer->GetGenGol();
	WORD Minchub = pPlayer->GetMinChub();
	LEVELTYPE level = pPlayer->GetLevel();

#ifdef _JAPAN_LOCAL_
	// [레벨] * 30 + [심맥] * 4 + [근골] * 6 + [민첩] * 2
	MaxShield = ( level * 30 ) + ( Simmek * 4 ) + ( Gengol * 6 ) + ( Minchub * 2 );
#else
	// [레벨] * 5 + [심맥] * 10
	// [레벨] * 5 + [심맥] * 10 + [근골] * 5 + [민첩] * 5 //- 2005.8.25 내외공밸런스로 공식 수정
	MaxShield = ( level * 5 ) + ( Simmek * 10 ) + ( Gengol * 5 ) + ( Minchub * 5 );
#endif

	// + 아이템 가산
	MaxShield += pPlayer->GetItemStats()->MaxShield;

	//2007. 6. 13. CBH -  세트아이탬 가산 
	MaxShield += pPlayer->GetSetItemStats()->dwShield;
	
	// + 특기 가산
	MaxShield += pPlayer->GetAbilityStats()->ShieldUp;
	
	// RaMa - 04.11.17   -> ShopItemOption횄횩째징
	MaxShield += pPlayer->GetShopItemStats()->Shield;

	// RaMa - 05.02.16   AvatarOption
	MaxShield += pPlayer->GetAvatarOption()->Shield;

	MaxShield += pPlayer->GetSkillStatsOption()->Shield;

	// magi82 - UniqueItem(070629)
	if( ((int)MaxShield + pPlayer->GetUniqueItemStats()->nShield) > 0 )
	{
		MaxShield += pPlayer->GetUniqueItemStats()->nShield;
	}
	else
	{
		MaxShield = 1;
	}

	pPlayer->SetMaxShield(MaxShield);

	pPlayer->CheckShieldValid();
}
void CCharacterCalcManager::CalcMaxNaeRyuk(CPlayer * pPlayer)
{
	DWORD MaxEnergy = 0;
	WORD Simmek = pPlayer->GetSimMek();
	LEVELTYPE level = pPlayer->GetLevel();

#ifdef _JAPAN_LOCAL_
	// [레벨] * 10 + [심맥] * 5
	MaxEnergy = ( level * 10 ) + ( Simmek * 5 );
#else
	// [레벨] * 5 + [심맥] * 10
	MaxEnergy = ( level * 5 ) + ( Simmek * 10 );
#endif

	MaxEnergy += pPlayer->GetItemStats()->MaxNaeRyuk;

	//2007. 6. 13. CBH -  세트아이탬 가산 
	MaxEnergy += pPlayer->GetSetItemStats()->dwNaeRyuk;
	
	// + 특기 가산
	MaxEnergy += pPlayer->GetAbilityStats()->NaeRyukUp;

	// RaMa - 04.11.17   -> ShopItemOption횄횩째징
	MaxEnergy += pPlayer->GetShopItemStats()->Naeryuk;

	// RaMa - 05.02.16   AvatarOption
	MaxEnergy += pPlayer->GetAvatarOption()->Naeruyk;

	MaxEnergy += pPlayer->GetSkillStatsOption()->NaeRyuk;

	// magi82 - UniqueItem(070629)
	if( ((int)MaxEnergy + pPlayer->GetUniqueItemStats()->nMp) > 0 )
	{
		MaxEnergy += pPlayer->GetUniqueItemStats()->nMp;
	}
	else
	{
		MaxEnergy = 1;
	}

	pPlayer->SetMaxNaeRyuk(MaxEnergy);

	pPlayer->CheckNaeRyukValid();
}

void CCharacterCalcManager::ProcessLife(CPlayer * pPlayer)
{
	if(pPlayer->GetState() == eObjectState_Die)
		return;

	DWORD life = pPlayer->GetLife();
	DWORD maxlife = pPlayer->GetMaxLife();
	LEVELTYPE level = pPlayer->GetLevel();

	if(life == maxlife) return;
	
	DWORD curTime = 0;
	if(!pPlayer->m_LifeRecoverTime.bStart)
	{
		curTime = gCurTime;
	}
	else
	{
		curTime = pPlayer->m_LifeRecoverTime.lastCheckTime = gCurTime;
		pPlayer->m_LifeRecoverTime.bStart = FALSE;
		return;
	}


	if(pPlayer->GetState() == eObjectState_Ungijosik)
		Ungi_LifeCount(pPlayer,life,maxlife,curTime);
	else
		General_LifeCount(pPlayer, curTime, level, life, maxlife);

}


void CCharacterCalcManager::General_LifeCount(CPlayer * pPlayer, DWORD curTime, LEVELTYPE level, DWORD life, DWORD maxlife)
{
	DWORD TimeLength = 5000;

#ifdef _JAPAN_LOCAL_
	//SW051112 무쌍모드
	if(pPlayer->IsMussangMode())
	{
		TimeLength = 2500;	//2배 빨라짐.
	}
#endif
	
	if(curTime - pPlayer->m_LifeRecoverTime.lastCheckTime > TimeLength)
	{// 	평상시 회복 (5초당) 체력 : 전체의 1%
		DWORD UpLife = pPlayer->GetRecoverLife();
	
		// 06. 03 국내무쌍 - 이영준
#ifndef _JAPAN_LOCAL_
		float fRate = 0.01f;

		if(pPlayer->IsMussangMode())
		{
			switch(pPlayer->GetStage())
			{
			case eStage_Normal:	
				fRate = 0.015f;
				break;
			case eStage_Hwa:		
			case eStage_Geuk:
				fRate = 0.015f;
				break;
			case eStage_Hyun:
			case eStage_Tal:
				fRate = 0.02f;
				break;
			}
		}
		DWORD CalcLife = life+(DWORD)(maxlife*fRate)+UpLife;
#else
		DWORD CalcLife = life+(DWORD)(maxlife*0.01f)+UpLife;
#endif
			
		pPlayer->SetLife(CalcLife);
		pPlayer->m_LifeRecoverTime.lastCheckTime = curTime; //KES confirm
	}
}

void CCharacterCalcManager::General_LifeCountBoss(CBossMonster * pBoss, DWORD curTime, DWORD life, DWORD maxlife)
{
	// 	평상시 회복 (5초당) 체력 : 전체의 1%
	if(curTime - pBoss->m_LifeRecoverTime.lastCheckTime > 5000)
	{
		pBoss->SetLife(life+(DWORD)(maxlife*0.001f), TRUE);
		pBoss->m_LifeRecoverTime.lastCheckTime = curTime;
	}
}

void CCharacterCalcManager::Ungi_LifeCount(CPlayer* pPlayer, DWORD life, DWORD maxlife, DWORD curTime)
{
	// RaMa - 04.11.24  ->운기조식시간
#ifdef _JAPAN_LOCAL_
	DWORD dwUngiTime = 4000;
#else
	DWORD dwUngiTime = 5000;
#endif
	// dwUngiTime = dwUngiTime*(1/gUngiSpeed);
	dwUngiTime = (DWORD)(dwUngiTime*(1/gEventRate[eEvent_UngiSpeed]));

	// 2005 크리스마스 이벤트 코드
	if( WEATHERMGR->GetWeatherState() == eWS_Snow )
		dwUngiTime /= 2;
	
	//레벨에 상관없이 5초마다 전체량의 10(보정치) + 3% 재충전
	if(curTime - pPlayer->m_LifeRecoverTime.lastCheckTime > dwUngiTime)
	{
		DWORD baseplus = (DWORD)(10 + maxlife*(0.03f)) + pPlayer->m_AbilityStats.UngiUpVal;
		DWORD pluslife = (DWORD)(maxlife*pPlayer->GetUngiPlusRate());
		pPlayer->SetLife(life + baseplus + pluslife);
		pPlayer->m_LifeRecoverTime.lastCheckTime = curTime;
	}
}

void CCharacterCalcManager::ProcessLifeBoss(CBossMonster * pBoss)
{
	if(pBoss->GetState() == eObjectState_Die)
		return;

	DWORD life = pBoss->GetLife();
	DWORD maxlife = pBoss->GetMaxLife();

	if(life == maxlife) return;
	
	DWORD curTime = 0;
	if(!pBoss->m_LifeRecoverTime.bStart)
	{
		curTime = gCurTime;
	}
	else
	{
		curTime = pBoss->m_LifeRecoverTime.lastCheckTime = gCurTime;
		pBoss->m_LifeRecoverTime.bStart = FALSE;
		return;
	}

	General_LifeCountBoss(pBoss, curTime, life, maxlife);
}

void CCharacterCalcManager::ProcessShield(CPlayer * pPlayer)
{
	if(pPlayer->GetState() == eObjectState_Die)
		return;

	DWORD shield = pPlayer->GetShield();
	DWORD maxshield = pPlayer->GetMaxShield();
	LEVELTYPE level = pPlayer->GetLevel();

	if(shield == maxshield) return;
	
	DWORD curTime = 0;
	if(!pPlayer->m_ShieldRecoverTime.bStart)
	{
		curTime = gCurTime;
	}
	else
	{
		curTime = pPlayer->m_ShieldRecoverTime.lastCheckTime = gCurTime;
		pPlayer->m_ShieldRecoverTime.bStart = FALSE;
		return;
	}


	if(pPlayer->GetState() == eObjectState_Ungijosik)
		Ungi_ShieldCount(pPlayer,shield,maxshield,curTime);
	else
		General_ShieldCount(pPlayer, curTime, level, shield, maxshield);
	

}

void CCharacterCalcManager::General_ShieldCount(CPlayer * pPlayer, DWORD curTime, LEVELTYPE level, DWORD Shield, DWORD maxShield)
{
	DWORD TimeLength = 5000;

#ifdef _JAPAN_LOCAL_
	//SW051112 무쌍모드
	if(pPlayer->IsMussangMode())
	{
		TimeLength = 2500;	//2배 빨라짐.
	}
#endif

	// 	평상시 회복 (5초당) 전체의 1%
	if(curTime - pPlayer->m_ShieldRecoverTime.lastCheckTime > TimeLength)
	{
		DWORD UpShield = pPlayer->GetRecoverShield();
		// 06. 03 국내무쌍 - 이영준
#ifndef _JAPAN_LOCAL_
		float fRate = 0.05f;

		if(pPlayer->IsMussangMode())
		{
			switch(pPlayer->GetStage())
			{
			case eStage_Normal:	
				fRate = 0.075f;
				break;
			case eStage_Hwa:		
			case eStage_Geuk:
				fRate = 0.075f;
				break;
			case eStage_Hyun:
			case eStage_Tal:
				fRate = 0.1f;
				break;
			}
		}
		
		DWORD CalcShield = Shield+(DWORD)(maxShield*fRate)+UpShield;
#else
		DWORD CalcShield = Shield+(DWORD)(maxShield*0.05f)+UpShield;
#endif

		pPlayer->SetShield(CalcShield);
		pPlayer->m_ShieldRecoverTime.lastCheckTime = curTime;	//KES이렇게 하면 안되지 않남? 정확히 5초마다가 되지 못한다.
	}
}

void CCharacterCalcManager::Ungi_ShieldCount(CPlayer* pPlayer, DWORD Shield, DWORD maxShield, DWORD curTime)
{
	// RaMa - 04.11.24  ->운기조식시간
#ifdef _JAPAN_LOCAL_
	DWORD dwUngiTime = 4000;
#else
	DWORD dwUngiTime = 5000;
#endif
	// dwUngiTime = dwUngiTime*(1/gUngiSpeed);
	dwUngiTime = (DWORD)(dwUngiTime*(1/gEventRate[eEvent_UngiSpeed]));

	// 2005 크리스마스 이벤트 코드
	if( WEATHERMGR->GetWeatherState() == eWS_Snow )
		dwUngiTime /= 2;

	//레벨에 상관없이 5초마다 전체량의 10(보정치) + 8% 재충전
	if(curTime - pPlayer->m_ShieldRecoverTime.lastCheckTime > dwUngiTime)
	{
		DWORD baseplus = (DWORD)(10 + maxShield*(0.08f)) + pPlayer->m_AbilityStats.UngiUpVal;
		DWORD plusShield = (DWORD)(maxShield*pPlayer->GetUngiPlusRate());
		pPlayer->SetShield(Shield + baseplus + plusShield);
		pPlayer->m_ShieldRecoverTime.lastCheckTime = curTime;
	}
}

void CCharacterCalcManager::ProcessShieldBoss(CBossMonster * pBoss)
{
	if(pBoss->GetState() == eObjectState_Die)
		return;

	DWORD shield = pBoss->GetShield();
	DWORD maxshield = pBoss->GetMaxShield();
	LEVELTYPE level = pBoss->GetLevel();

	if(shield == maxshield) return;
	
	DWORD curTime = 0;
	if(!pBoss->m_ShieldRecoverTime.bStart)
	{
		curTime = gCurTime;
	}
	else
	{
		curTime = pBoss->m_ShieldRecoverTime.lastCheckTime = gCurTime;
		pBoss->m_ShieldRecoverTime.bStart = FALSE;
		return;
	}

	General_ShieldCountBoss(pBoss, curTime, shield, maxshield);
}

void CCharacterCalcManager::General_ShieldCountBoss(CBossMonster * pBoss, DWORD curTime, DWORD Shield, DWORD maxShield)
{
	// 	평상시 회복 (5초당) 전체의 1%
	if(curTime - pBoss->m_ShieldRecoverTime.lastCheckTime > 5000)
	{
		pBoss->SetShield(Shield+(DWORD)(maxShield*0.005f), TRUE);
		pBoss->m_ShieldRecoverTime.lastCheckTime = curTime;
	}
}

void CCharacterCalcManager::ProcessNaeRyuk(CPlayer * pPlayer)
{
	if(pPlayer->GetState() == eObjectState_Die)
		return;
	
	DWORD energy = pPlayer->GetNaeRyuk();
	DWORD maxEnergy = pPlayer->GetMaxNaeRyuk();
	LEVELTYPE level = pPlayer->GetLevel();
	WORD KGIdx = pPlayer->m_MoveInfo.KyungGongIdx;

	if(KGIdx != 0)
	{
		KyungGong_NaeRyukCount(pPlayer,gCurTime,KGIdx);
		return;
	}
	//////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	// 내력소모
	if(SingleSpecialState_NaeRyukCount(pPlayer))
		return;
	//////////////////////////////////////////////////////////////////////

	if(energy == maxEnergy) return;

	DWORD curTime = 0;
	if(!pPlayer->m_NaeRyukRecoverTime.bStart)
	{
		curTime = gCurTime;
	}
	else
	{
		curTime = pPlayer->m_NaeRyukRecoverTime.lastCheckTime = gCurTime;
		pPlayer->m_NaeRyukRecoverTime.bStart = FALSE;
		return;
	}

	if(pPlayer->GetState() == eObjectState_Ungijosik)
		Ungi_NaeRyukCount(pPlayer,curTime,energy,maxEnergy);
	else
		General_NaeRyukCount(pPlayer, curTime, level, energy, maxEnergy);
	

}

void CCharacterCalcManager::General_NaeRyukCount(CPlayer* pPlayer, DWORD curTime, LEVELTYPE level, DWORD energy, DWORD maxEnergy)
{
	DWORD TimeLength = 5000;

#ifdef _JAPAN_LOCAL
	//SW051112 무쌍모드
	if(pPlayer->IsMussangMode())
	{
		TimeLength = 2500;	//2배 빨라짐.
	}
#endif
	 //평상시 회복 (5초당) 내력 : 전체의 1%
	if(curTime - pPlayer->m_NaeRyukRecoverTime.lastCheckTime > TimeLength)
	{
		// RaMa - 04.11.17  ->ShopItemOption?횉 쨀쨩쨌횂횊쨍쨘쨔쨌챗 1.5쨔챔 횁천째징.
		float fRate = 0.01f;
		if( pPlayer->GetShopItemStats()->RecoverRate )
		{
			fRate += pPlayer->GetShopItemStats()->RecoverRate*0.01f;
		}

		// 06. 03 국내무쌍 - 이영준
#ifndef _JAPAN_LOCAL_
		if(pPlayer->IsMussangMode())
		{
			switch(pPlayer->GetStage())
			{
			case eStage_Normal:	
				fRate = fRate * 1.5f;
				break;
			case eStage_Hwa:		
			case eStage_Geuk:
				fRate = fRate * 1.5f;
				break;
			case eStage_Hyun:
			case eStage_Tal:
				fRate = fRate * 2.0f;
				break;
			}
		}
#endif

		pPlayer->SetNaeRyuk(energy+(DWORD)(maxEnergy*fRate));
		pPlayer->m_NaeRyukRecoverTime.lastCheckTime = curTime;
	}
}

void CCharacterCalcManager::Ungi_NaeRyukCount(CPlayer* pPlayer, DWORD curTime, DWORD energy, DWORD maxEnergy)
{	
	// RaMa - 04.11.24  ->운기조식시간
#ifdef _JAPAN_LOCAL_
	DWORD dwUngiTime = 4000;
#else
	DWORD dwUngiTime = 5000;
#endif
	// dwUngiTime = dwUngiTime*(1/gUngiSpeed);
	dwUngiTime = (DWORD)(dwUngiTime*(1/gEventRate[eEvent_UngiSpeed]));

	// 2005 크리스마스 이벤트 코드
	if( WEATHERMGR->GetWeatherState() == eWS_Snow )
		dwUngiTime /= 2;

	//레벨에 상관없이 5초마다 전체량의 10(보정치) + 3% 재충전
	if(curTime - pPlayer->m_NaeRyukRecoverTime.lastCheckTime > dwUngiTime)
	{
		// RaMa - 04.11.17  ->ShopItemOption?횉 쨀쨩쨌횂횊쨍쨘쨔쨌챗 1.5쨔챔 횁천째징.
		DWORD recover = 0;
		recover = (DWORD)(10 + maxEnergy*(0.03f)) + pPlayer->m_AbilityStats.UngiUpVal;
//		recover += (DWORD)(maxEnergy*pPlayer->GetUngiPlusRate());
		recover = (DWORD)(recover + (maxEnergy*pPlayer->GetUngiPlusRate()));

		if( pPlayer->GetShopItemStats()->RecoverRate )
		{
//			recover *= (DWORD)((pPlayer->GetShopItemStats()->RecoverRate*0.01f));
			recover = (DWORD)(recover * ((pPlayer->GetShopItemStats()->RecoverRate*0.01f)));
		}

		pPlayer->SetNaeRyuk(energy + recover);
		pPlayer->m_NaeRyukRecoverTime.lastCheckTime = curTime;
	}
}

void CCharacterCalcManager::KyungGong_NaeRyukCount(CPlayer* pPlayer, DWORD curTime,WORD KyungGongIdx)
{
	ASSERT(KyungGongIdx);
	CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(KyungGongIdx);
	ASSERT(pKGInfo);
		
	if(curTime - pPlayer->m_KyungGongNaeRyuk_LastCheckTime > 3000)
	{
		// RaMa - 06.06.08
		if( pPlayer->GetAvatarOption()->NaeruykspendbyKG == 0 )
			pPlayer->ReduceNaeRyuk(pKGInfo->GetNeedNaeRyuk());

		if(pPlayer->GetNaeRyuk() == 0)
		{
			CCharMove::ForceStopKyungGong(pPlayer);
		}
		pPlayer->m_KyungGongNaeRyuk_LastCheckTime = curTime;
	}
}

//////////////////////////////////////////////////////////////////////
// 06. 06. 2차 전직 - 이영준
// 은신/혜안
// 내력소모
BOOL CCharacterCalcManager::SingleSpecialState_NaeRyukCount(CPlayer* pPlayer)
{
	WORD Count = 0;

	for(WORD State = eSingleSpecialState_Hide; State < eSingleSpecialState_Max; State++)
	{
		if(!pPlayer->GetSingleSpecialState(State))
			continue;

        SPECIAL_STATE_INFO* pInfo = SKILLMGR->GetSpecialStateInfo(State);

		if(!pInfo)
			continue;

		Count++;

		if(pPlayer->m_StateNaeRyukCountTime[State] + pInfo->TickTime >= gCurTime)
			continue;

		pPlayer->m_StateNaeRyukCountTime[State] = gCurTime;

		// 소모내력계산
		WORD MogongNaeRyuk = pInfo->NaeRyuk;

		// 이벤트
		DWORD NeedNaeRyuk = (DWORD)(MogongNaeRyuk*gEventRate[eEvent_NaeRuykRate]);

		if( pPlayer->GetPartyIdx() )
		{
			CParty* pParty = PARTYMGR->GetParty( pPlayer->GetPartyIdx() );
			if( pParty )
			{
				int count = pParty->GetMemberCountofMap( pPlayer->GetID() );
				if( count && gPartyEvent[ePartyEvent_NaeRyukRate].Rate[count-1] )
					NeedNaeRyuk = (DWORD)(NeedNaeRyuk*gPartyEvent[ePartyEvent_NaeRyukRate].Rate[count-1]);
			}
		}

		if(NeedNaeRyuk)
		{
			// 아이템
			WORD NaeryukSpend = pPlayer->GetShopItemStats()->NeaRyukSpend + pPlayer->GetAvatarOption()->NeaRyukSpend 
				+ pPlayer->GetShopItemStats()->PlustimeNaeruyk;
			if( NaeryukSpend >= 100 )
				NeedNaeRyuk = 0;
			else
				NeedNaeRyuk = (DWORD)(NeedNaeRyuk*(1.0f - NaeryukSpend*0.01f));

			// 무쌍모드
#ifdef _JAPAN_LOCAL_
			if( pPlayer->IsMussangMode() )
			{
				NeedNaeRyuk = (DWORD)(NeedNaeRyuk * 0.25f);
			}
#else
			if( pPlayer->IsMussangMode() )
			{
				switch(pPlayer->GetStage())
				{
				case eStage_Normal:	
					NeedNaeRyuk = (DWORD)(NeedNaeRyuk * 0.95f);
				case eStage_Hwa:		
				case eStage_Geuk:
					NeedNaeRyuk = (DWORD)(NeedNaeRyuk * 0.90f);
				case eStage_Hyun:	  		
				case eStage_Tal:	
					NeedNaeRyuk = (DWORD)(NeedNaeRyuk * 0.85f);
				}
			}
#endif //_JAPAN_LOCAL_

			pPlayer->ReduceNaeRyuk(NeedNaeRyuk);

			if(pPlayer->GetNaeRyuk() < NeedNaeRyuk)
			{
				// 남은 내력이 필요 내력보다 적으면 종료
				pPlayer->SetSingleSpecialStateUsedTime(State, gCurTime);
				pPlayer->SetSingleSpecialState(State, false);
				pPlayer->SendSingleSpeicalStateAck(State, false);
			}
		}
	}

	if(Count > 0)
		return TRUE;
	else
		return FALSE;
}
//////////////////////////////////////////////////////////////////////
