// PatternNPC.cpp: implementation of the CPatternNPC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PatternNPC.h"
#include "Monster.h"
#include <time.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPatternNPC::CPatternNPC()
{
//	m_dwCurSearchTime = NULL;
//	m_dwTmpSearchTime = NULL;
//	m_dwLastSearchTime = NULL;
//	m_bFirstSearch = FALSE;
	srand((unsigned)time(NULL));
}

CPatternNPC::~CPatternNPC()
{

}

BOOL CPatternNPC::MonsterAreaCheck(CMonster* pMonster)
{

//	VECTOR3 RegenPos = pMonster->
	return FALSE;
}

void CPatternNPC::MonsterPatternManager(CMonster* pMonster, BYTE StateFlag)
{
/*	if(!pMonster->GetFirstSearch())
	{
//		m_dwCurSearchTime = m_dwTmpSearchTime = m_dwLastSearchTime = gCurTime;
		pMonster->SetCurSearchTime(gCurTime);
		pMonster->SetTmpSearchTime(gCurTime);
		pMonster->SetLastSearchTime(gCurTime);

		StateFlag = MonsterSearchTarget(pMonster);
		pMonster->SetFirstSearch(TRUE);
	}
	else
	{
		pMonster->SetCurSearchTime(gCurTime);
		pMonster->SetTmpSearchTime(pMonster->GetCurSearchTime() - pMonster->GetLastSearchTime());
		if(pMonster->GetTmpSearchTime() > GAMERESRCMNGR->GetMonsterInfo(pMonster->GetMonsterKind())->SearchTime * 1)
		{
			StateFlag = MonsterSearchTarget(pMonster);
			pMonster->SetLastSearchTime(pMonster->GetCurSearchTime());
		}

	}
	
	//////////////////////////////////////////////////////////////////////////
	// 몬스터도 좀 쉽시다... -_-+ [3/18/2003]
	if(pMonster->Rest(gTickTime) == FALSE)
		return;

	if(pMonster->GetState() == eObjectState_Die) return;
	
	switch(StateFlag)
	{
	case PEACE_STATE:
		PeacePattern(pMonster);
		break;
	case ANGER_STATE:
		AngerPattern(pMonster);
		break;
	case ATTACK_STATE:
		AttackPattern(pMonster);
		break;
	}*/
}

void CPatternNPC::NpcPatternManager(CNpc* pNpc, BYTE StateFlag)
{
/*	switch(StateFlag)
	{
	case PEACE_STATE:
		PeacePattern(pMonster);
		break;
	case ANGER_STATE:
		AngerPattern(pMonster);
		break;
	case ATTACK_STATE:
		AttackPattern(pMonster);
		break;
	}*/
}

BYTE CPatternNPC::MonsterSearchTarget(CMonster* pMonster)
{
//	BYTE state = pMonster->GetMonsterState()->GetCurState();
//	BYTE Kind = pMonster->GetObjectKind();
//	BYTE dist = 9;

//	CPlayer* pPeacePlayer = (CPlayer*)g_pServerSystem->GetGridSystem()->FindPlayerInRange(0, CCharMove::GetPosition(pMonster), GAMERESRCMNGR->GetMonsterInfo(Kind)->MoveArea);
//	CPlayer* pAngerPlayer = (CPlayer*)GAMESYSTEM_OBJ->GetGridSystem()->FindPlayerInRange(0, CCharMove::GetPosition(pMonster), GAMERESRCMNGR->GetMonsterInfo(Kind)->AngryArea);
//	WORD AttDist = GAMERESRCMNGR->GetMonsterInfo(Kind)->Attack1Distance;
//	WORD SearchDist = GAMERESRCMNGR->GetMonsterInfo(Kind)->SearchArea;
//	CPlayer* SearchPlayer = (CPlayer*)g_pServerSystem->GetGridSystem()->FindPlayerInRange(0, CCharMove::GetPosition(pMonster), SearchDist);
//	CPlayer* AttPlayer = (CPlayer*)g_pServerSystem->GetGridSystem()->FindPlayerInRange(0, CCharMove::GetPosition(pMonster), 500);
//	WORD tmp1 = GAMERESRCMNGR->GetMonsterInfo(Kind)->MoveArea;
////	if(pPeacePlayer)
//	{
//		if(pAngerPlayer)
//		{
/*	if(SearchPlayer)
	{
		if(AttPlayer)
		{
			pMonster->SetTargetPlayer(AttPlayer);
			pMonster->SetTargetObjectPos(CCharMove::GetPosition(AttPlayer));
			pMonster->GetMonsterState()->SetCurState(ATTACK_STATE);
		}
		else
		{
//				pMonster->SetTargetPlayer(pAngerPlayer);
//				pMonster->SetTargetPlayer(pPeacePlayer);
//				pMonster->SetTargetObjectPos(CCharMove::GetPosition(pPeacePlayer));
//				pMonster->GetMonsterState()->SetCurState(ANGER_STATE);
			pMonster->GetMonsterState()->SetCurState(PEACE_STATE);
		}
	}
	
//		}
//		else
//		{
//			pMonster->SetTargetPlayer(pPeacePlayer);
//			pMonster->SetTargetObjectPos(CCharMove::GetPosition(pPeacePlayer));
//			pMonster->GetMonsterState()->SetCurState(PEACE_STATE);
//		}
//	}
//	else
//	{
//		pMonster->GetMonsterState()->SetCurState(PEACE_STATE);
//	}*/
	return 1;
}


void CPatternNPC::PeacePattern(CMonster* pMonster)
{
	switch(pMonster->GetMonsterKind())
	{
	case eParakho_L:
	case eParakho:
	case eParakho_S:			ParakhoPeace(pMonster); break;
	case eHucpyobom_L:
	case eHucpyobom:
	case eHucpyobom_S:			HucpyobomPeace(pMonster); break;
	case eCoyente_L:
	case eCoyente:
	case eCoyente_S:			CoyentePeace(pMonster); break;
	case eHyelgwiso_L:
	case eHyelgwiso:
	case eHyelgwiso_S:			HyelgwisoPeace(pMonster); break;
	case eGorilra_L:	
	case eGorilra:	
	case eGorilra_S:			GorilraPeace(pMonster); break;
	case eNokrimdo_L:
	case eNokrimdo:
	case eNokrimdo_S:			NokrimdoPeace(pMonster); break;
	case eGom_L:	
	case eGom:	
	case eGom_S:				GomPeace(pMonster); break;
	case eTochawo_L:
	case eTochawo:
	case eTochawo_S:			TochawoPeace(pMonster); break;
	case eYeti_L:	
	case eYeti:	
	case eYeti_S:				YetiPeace(pMonster); break;
	case eGongGwi_L:
	case eGongGwi:
	case eGongGwi_S:			GongGwiPeace(pMonster); break;
	case eHorangie_L:	
	case eHorangie:	
	case eHorangie_S:			HorangiePeace(pMonster); break;
	case eDongyounginja_L:
	case eDongyounginja:
	case eDongyounginja_S:		DongyounginjaPeace(pMonster); break;
	case eYewa_L:
	case eYewa:
	case eYewa_S:				YewaPeace(pMonster); break;
	case eSanso_L:
	case eSanso:
	case eSanso_S:				SansoPeace(pMonster); break;
	case eAlyou_L:
	case eAlyou:
	case eAlyou_S:				AlyouPeace(pMonster); break;
	case eSagalru_L:
	case eSagalru:
	case eSagalru_S:			SagalruPeace(pMonster); break;
	case ePyobom_L:
	case ePyobom:
	case ePyobom_S:				PyobomPeace(pMonster); break;
	case eYekgwi_L:
	case eYekgwi:
	case eYekgwi_S:				YekgwiPeace(pMonster); break;
	case eImugi_L:
	case eImugi:
	case eImugi_S:				ImugiPeace(pMonster); break;
	case eSamurai_L:	
	case eSamurai:	
	case eSamurai_S:			SamuraiPeace(pMonster); break;
	case eBunwun_L:
	case eBunwun:
	case eBunwun_S:				BunwunPeace(pMonster); break;
	case eMokbigwi_L:	
	case eMokbigwi:	
	case eMokbigwi_S:			MokbigwiPeace(pMonster); break;
	case eSuckgein_L:	
	case eSuckgein:	
	case eSuckgein_S:			SuckgeinPeace(pMonster); break;
	case eHu_L:	
	case eHu:	
	case eHu_S:					HuPeace(pMonster); break;
	case eInmeundanrang_L:
	case eInmeundanrang:
	case eInmeundanrang_S:		InmeundanrangPeace(pMonster); break;
	case eNokrimchejuPe_L:
	case eNokrimchejuPe:
	case eNokrimchejuPe_S:		NokrimchejuPeace(pMonster); break;
	case eMujiki_L:
	case eMujiki:
	case eMujiki_S:				MujikiPeace(pMonster); break;
	case eBarki_L:
	case eBarki:
	case eBarki_S:				BarkiPeace(pMonster); break;
	case eHwan_L:	
	case eHwan:	
	case eHwan_S:				HwanPeace(pMonster); break;
	case eYoumyou_L:
	case eYoumyou:
	case eYoumyou_S:			YoumyouPeace(pMonster); break;
	case eSansin_L:
	case eSansin:
	case eSansin_S:				SansinPeace(pMonster); break;
	case eSagaik_L:
	case eSagaik:
	case eSagaik_S:				SagaikPeace(pMonster); break;
	case eBuHey_L:
	case eBuHey:
	case eBuHey_S:				BuHeyPeace(pMonster); break;
	case eHeulkyojangro_L:
	case eHeulkyojangro:
	case eHeulkyojangro_S:		HeulkyojangroPeace(pMonster); break;
	case eKangsi_L:
	case eKangsi:
	case eKangsi_S:				KangsiPeace(pMonster); break;
	case eHungchun_L:	
	case eHungchun:	
	case eHungchun_S:			HungchunPeace(pMonster); break;
	case eTa_L:
	case eTa:
	case eTa_S:					TaPeace(pMonster); break;
	case eSanjewangPeac_L:
	case eSanjewangPeac:
	case eSanjewangPeac_S:		SanjewangPeace(pMonster);break;
	case eBackho_L:
	case eBackho:
	case eBackho_S:				BackhoPeace(pMonster); break;
	case eKumong_L:
	case eKumong:
	case eKumong_S:				KumongPeace(pMonster); break;
	case eWomawang_L:
	case eWomawang:
	case eWomawang_S:			WomawangPeace(pMonster); break;
	case eJujak_L:
	case eJujak:
	case eJujak_S:				JujakPeace(pMonster); break;
	case eHyunmu_L:
	case eHyunmu:
	case eHyunmu_S:				HyunmuPeace(pMonster); break;
	case eKirin_L:
	case eKirin:
	case eKirin_S:				KirinPeace(pMonster); break;
	case eYoung_L:
	case eYoung:
	case eYoung_S:				YoungPeace(pMonster); break;
	}
}

void CPatternNPC::AngerPattern(CMonster* pMonster)
{
	switch(pMonster->GetMonsterKind())
	{
	case eParakho_L:
	case eParakho:
	case eParakho_S:			ParakhoAnger(pMonster); break;
	case eHucpyobom_L:
	case eHucpyobom:
	case eHucpyobom_S:			HucpyobomAnger(pMonster); break;
	case eCoyente_L:
	case eCoyente:
	case eCoyente_S:			CoyenteAnger(pMonster); break;
	case eHyelgwiso_L:
	case eHyelgwiso:
	case eHyelgwiso_S:			HyelgwisoAnger(pMonster); break;
	case eGorilra_L:	
	case eGorilra:	
	case eGorilra_S:			GorilraAnger(pMonster); break;
	case eNokrimdo_L:
	case eNokrimdo:
	case eNokrimdo_S:			NokrimdoAnger(pMonster); break;
	case eGom_L:	
	case eGom:	
	case eGom_S:				GomAnger(pMonster); break;
	case eTochawo_L:
	case eTochawo:
	case eTochawo_S:			TochawoAnger(pMonster); break;
	case eYeti_L:	
	case eYeti:	
	case eYeti_S:				YetiAnger(pMonster); break;
	case eGongGwi_L:
	case eGongGwi:
	case eGongGwi_S:			GongGwiAnger(pMonster); break;
	case eHorangie_L:	
	case eHorangie:	
	case eHorangie_S:			HorangieAnger(pMonster); break;
	case eDongyounginja_L:
	case eDongyounginja:
	case eDongyounginja_S:		DongyounginjaAnger(pMonster); break;
	case eYewa_L:
	case eYewa:
	case eYewa_S:				YewaAnger(pMonster); break;
	case eSanso_L:
	case eSanso:
	case eSanso_S:				SansoAnger(pMonster); break;
	case eAlyou_L:
	case eAlyou:
	case eAlyou_S:				AlyouAnger(pMonster); break;
	case eSagalru_L:
	case eSagalru:
	case eSagalru_S:			SagalruAnger(pMonster); break;
	case ePyobom_L:
	case ePyobom:
	case ePyobom_S:				PyobomAnger(pMonster); break;
	case eYekgwi_L:
	case eYekgwi:
	case eYekgwi_S:				YekgwiAnger(pMonster); break;
	case eImugi_L:
	case eImugi:
	case eImugi_S:				ImugiAnger(pMonster); break;
	case eSamurai_L:	
	case eSamurai:	
	case eSamurai_S:			SamuraiAnger(pMonster); break;
	case eBunwun_L:
	case eBunwun:
	case eBunwun_S:				BunwunAnger(pMonster); break;
	case eMokbigwi_L:	
	case eMokbigwi:	
	case eMokbigwi_S:			MokbigwiAnger(pMonster); break;
	case eSuckgein_L:	
	case eSuckgein:	
	case eSuckgein_S:			SuckgeinAnger(pMonster); break;
	case eHu_L:	
	case eHu:	
	case eHu_S:					HuAnger(pMonster); break;
	case eInmeundanrang_L:
	case eInmeundanrang:
	case eInmeundanrang_S:		InmeundanrangAnger(pMonster); break;
	case eNokrimchejuPe_L:
	case eNokrimchejuPe:
	case eNokrimchejuPe_S:		NokrimchejuAnger(pMonster); break;
	case eMujiki_L:
	case eMujiki:
	case eMujiki_S:				MujikiAnger(pMonster); break;
	case eBarki_L:
	case eBarki:
	case eBarki_S:				BarkiAnger(pMonster); break;
	case eHwan_L:	
	case eHwan:	
	case eHwan_S:				HwanAnger(pMonster); break;
	case eYoumyou_L:
	case eYoumyou:
	case eYoumyou_S:			YoumyouAnger(pMonster); break;
	case eSansin_L:
	case eSansin:
	case eSansin_S:				SansinAnger(pMonster); break;
	case eSagaik_L:
	case eSagaik:
	case eSagaik_S:				SagaikAnger(pMonster); break;
	case eBuHey_L:
	case eBuHey:
	case eBuHey_S:				BuHeyAnger(pMonster); break;
	case eHeulkyojangro_L:
	case eHeulkyojangro:
	case eHeulkyojangro_S:		HeulkyojangroAnger(pMonster); break;
	case eKangsi_L:
	case eKangsi:
	case eKangsi_S:				KangsiAnger(pMonster); break;
	case eHungchun_L:	
	case eHungchun:	
	case eHungchun_S:			HungchunAnger(pMonster); break;
	case eTa_L:
	case eTa:
	case eTa_S:					TaAnger(pMonster); break;
	case eSanjewangPeac_L:
	case eSanjewangPeac:
	case eSanjewangPeac_S:		SanjewangAnger(pMonster);break;
	case eBackho_L:
	case eBackho:
	case eBackho_S:				BackhoAnger(pMonster); break;
	case eKumong_L:
	case eKumong:
	case eKumong_S:				KumongAnger(pMonster); break;
	case eWomawang_L:
	case eWomawang:
	case eWomawang_S:			WomawangAnger(pMonster); break;
	case eJujak_L:
	case eJujak:
	case eJujak_S:				JujakAnger(pMonster); break;
	case eHyunmu_L:
	case eHyunmu:
	case eHyunmu_S:				HyunmuAnger(pMonster); break;
	case eKirin_L:
	case eKirin:
	case eKirin_S:				KirinAnger(pMonster); break;
	case eYoung_L:
	case eYoung:
	case eYoung_S:				YoungAnger(pMonster); break;
	}
}

void CPatternNPC::AttackPattern(CMonster* pMonster)
{
	switch(pMonster->GetMonsterKind())
	{
	case eParakho_L:
	case eParakho:
	case eParakho_S:			ParakhoAttack(pMonster); break;
	case eHucpyobom_L:
	case eHucpyobom:
	case eHucpyobom_S:			HucpyobomAttack(pMonster); break;
	case eCoyente_L:
	case eCoyente:
	case eCoyente_S:			CoyenteAttack(pMonster); break;
	case eHyelgwiso_L:
	case eHyelgwiso:
	case eHyelgwiso_S:			HyelgwisoAttack(pMonster); break;
	case eGorilra_L:	
	case eGorilra:	
	case eGorilra_S:			GorilraAttack(pMonster); break;
	case eNokrimdo_L:
	case eNokrimdo:
	case eNokrimdo_S:			NokrimdoAttack(pMonster); break;
	case eGom_L:	
	case eGom:	
	case eGom_S:				GomAttack(pMonster); break;
	case eTochawo_L:
	case eTochawo:
	case eTochawo_S:			TochawoAttack(pMonster); break;
	case eYeti_L:	
	case eYeti:	
	case eYeti_S:				YetiAttack(pMonster); break;
	case eGongGwi_L:
	case eGongGwi:
	case eGongGwi_S:			GongGwiAttack(pMonster); break;
	case eHorangie_L:	
	case eHorangie:	
	case eHorangie_S:			HorangieAttack(pMonster); break;
	case eDongyounginja_L:
	case eDongyounginja:
	case eDongyounginja_S:		DongyounginjaAttack(pMonster); break;
	case eYewa_L:
	case eYewa:
	case eYewa_S:				YewaAttack(pMonster); break;
	case eSanso_L:
	case eSanso:
	case eSanso_S:				SansoAttack(pMonster); break;
	case eAlyou_L:
	case eAlyou:
	case eAlyou_S:				AlyouAttack(pMonster); break;
	case eSagalru_L:
	case eSagalru:
	case eSagalru_S:			SagalruAttack(pMonster); break;
	case ePyobom_L:
	case ePyobom:
	case ePyobom_S:				PyobomAttack(pMonster); break;
	case eYekgwi_L:
	case eYekgwi:
	case eYekgwi_S:				YekgwiAttack(pMonster); break;
	case eImugi_L:
	case eImugi:
	case eImugi_S:				ImugiAttack(pMonster); break;
	case eSamurai_L:	
	case eSamurai:	
	case eSamurai_S:			SamuraiAttack(pMonster); break;
	case eBunwun_L:
	case eBunwun:
	case eBunwun_S:				BunwunAttack(pMonster); break;
	case eMokbigwi_L:	
	case eMokbigwi:	
	case eMokbigwi_S:			MokbigwiAttack(pMonster); break;
	case eSuckgein_L:	
	case eSuckgein:	
	case eSuckgein_S:			SuckgeinAttack(pMonster); break;
	case eHu_L:	
	case eHu:	
	case eHu_S:					HuAttack(pMonster); break;
	case eInmeundanrang_L:
	case eInmeundanrang:
	case eInmeundanrang_S:		InmeundanrangAttack(pMonster); break;
	case eNokrimchejuPe_L:
	case eNokrimchejuPe:
	case eNokrimchejuPe_S:		NokrimchejuAttack(pMonster); break;
	case eMujiki_L:
	case eMujiki:
	case eMujiki_S:				MujikiAttack(pMonster); break;
	case eBarki_L:
	case eBarki:
	case eBarki_S:				BarkiAttack(pMonster); break;
	case eHwan_L:	
	case eHwan:	
	case eHwan_S:				HwanAttack(pMonster); break;
	case eYoumyou_L:
	case eYoumyou:
	case eYoumyou_S:			YoumyouAttack(pMonster); break;
	case eSansin_L:
	case eSansin:
	case eSansin_S:				SansinAttack(pMonster); break;
	case eSagaik_L:
	case eSagaik:
	case eSagaik_S:				SagaikAttack(pMonster); break;
	case eBuHey_L:
	case eBuHey:
	case eBuHey_S:				BuHeyAttack(pMonster); break;
	case eHeulkyojangro_L:
	case eHeulkyojangro:
	case eHeulkyojangro_S:		HeulkyojangroAttack(pMonster); break;
	case eKangsi_L:
	case eKangsi:
	case eKangsi_S:				KangsiAttack(pMonster); break;
	case eHungchun_L:	
	case eHungchun:	
	case eHungchun_S:			HungchunAttack(pMonster); break;
	case eTa_L:
	case eTa:
	case eTa_S:					TaAttack(pMonster); break;
	case eSanjewangPeac_L:
	case eSanjewangPeac:
	case eSanjewangPeac_S:		SanjewangAttack(pMonster);break;
	case eBackho_L:
	case eBackho:
	case eBackho_S:				BackhoAttack(pMonster); break;
	case eKumong_L:
	case eKumong:
	case eKumong_S:				KumongAttack(pMonster); break;
	case eWomawang_L:
	case eWomawang:
	case eWomawang_S:			WomawangAttack(pMonster); break;
	case eJujak_L:
	case eJujak:
	case eJujak_S:				JujakAttack(pMonster); break;
	case eHyunmu_L:
	case eHyunmu:
	case eHyunmu_S:				HyunmuAttack(pMonster); break;
	case eKirin_L:
	case eKirin:
	case eKirin_S:				KirinAttack(pMonster); break;
	case eYoung_L:
	case eYoung:
	case eYoung_S:				YoungAttack(pMonster); break;
	}
}

void CPatternNPC::DefaultMonsterPeacePattern(CMonster* pMonster)
{
/*
		WORD data = 10;
		WORD temp = 0;
		if(CCharMove::IsMoving(pMonster))
			return;
		else
		{
			temp = rand() % data;
			if(temp > 0 && temp <= 3)
			{
				pMonster->WalkAround(300);
			}
			else
			{
				pMonster->SetPeaceRestTime(1000);
			}
		}*/
	
		
}

void CPatternNPC::DefaultMonsterAngerPattern(CMonster* pMonster)
{
/*
	if(CCharMove::IsMoving(pMonster))
	{
		pMonster->MoveToPosition(pMonster->GetTargetObjectPos());
//		return;		
	}
	else
	{
		pMonster->MoveToPosition(pMonster->GetTargetObjectPos());
	}*/

}

void CPatternNPC::DefaultMonsterAttackPattern(CMonster* pMonster)
{
/*	if(CCharMove::IsMoving(pMonster))
	{
		return;
		VECTOR3* MonsterPos = CCharMove::GetPosition(pMonster);
		VECTOR3* TargetPos = pMonster->GetTargetObjectPos();
		float Distance = CalcDistanceXZ(TargetPos, MonsterPos);
		float AttackDist = GAMERESRCMNGR->GetMonsterInfo(pMonster->GetMonsterKind())->Attack1Distance;
		if(Distance <= AttackDist)
		{
			// 공격함수 실행
			pMonster->Attack(0);
		}
		// 공격거리보다 멀고 공격영역안에 있으면 다시추적
		else 
		{
			VECTOR3* tpos = pMonster->GetTargetObjectPos();
			pMonster->MoveToPosition(tpos);

		}
	}
	else
	{
		VECTOR3* TargetPos = CCharMove::GetPosition(pMonster);
		VECTOR3* MonsterPos = pMonster->GetTargetObjectPos();
		float Distance = CalcDistanceXZ(TargetPos, MonsterPos);
		float AttackDist = GAMERESRCMNGR->GetMonsterInfo(pMonster->GetMonsterKind())->Attack1Distance;
		if(Distance <= AttackDist)
		{
			// 공격함수 실행
			pMonster->Attack(0);
		}
		// 공격거리보다 멀고 공격영역안에 있으면 다시추적
		else 
		{
			VECTOR3* tpos = pMonster->GetTargetObjectPos();
			pMonster->MoveToPosition(tpos);
	}
	}*/
}

void CPatternNPC::DefaultNpcPeacePattern(CNpc* pNpc)
{

}

void CPatternNPC::DefaultNpcAngerPattern(CNpc* pNpc)
{

}

void CPatternNPC::DefaultNpcAttackPattern(CNpc* pNpc)
{

}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC::ParakhoPeace(CMonster* pMonster)
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: ParakhoAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: ParakhoAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: HucpyobomPeace(CMonster* pMonster)	
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: HucpyobomAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: HucpyobomAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: CoyentePeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: CoyenteAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: CoyenteAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: HyelgwisoPeace(CMonster* pMonster)	
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: HyelgwisoAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: HyelgwisoAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: GorilraPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: GorilraAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: GorilraAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: NokrimdoPeace(CMonster* pMonster)	
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: NokrimdoAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: NokrimdoAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: GomPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: GomAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: GomAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: TochawoPeace(CMonster* pMonster)	
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: TochawoAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: TochawoAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: YetiPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: YetiAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: YetiAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: GongGwiPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: GongGwiAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: GongGwiAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: HorangiePeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: HorangieAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: HorangieAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: DongyounginjaPeace(CMonster* pMonster)	
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: DongyounginjaAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: DongyounginjaAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: YewaPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: YewaAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: YewaAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: SansoPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: SansoAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: SansoAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: AlyouPeace(CMonster* pMonster)			
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: AlyouAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: AlyouAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: SagalruPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: SagalruAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: SagalruAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: PyobomPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: PyobomAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: PyobomAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: YekgwiPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: YekgwiAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: YekgwiAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: ImugiPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: ImugiAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: ImugiAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: SamuraiPeace(CMonster* pMonster)	
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: SamuraiAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: SamuraiAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: BunwunPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: BunwunAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: BunwunAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: MokbigwiPeace(CMonster* pMonster)	
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: MokbigwiAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: MokbigwiAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: SuckgeinPeace(CMonster* pMonster)	
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: SuckgeinAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: SuckgeinAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: HuPeace(CMonster* pMonster)			
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: HuAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: HuAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: InmeundanrangPeace(CMonster* pMonster)
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: InmeundanrangAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: InmeundanrangAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: NokrimchejuPeace(CMonster* pMonster)	
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: NokrimchejuAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: NokrimchejuAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: MujikiPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: MujikiAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: MujikiAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: BarkiPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: BarkiAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: BarkiAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: HwanPeace(CMonster* pMonster)			
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: HwanAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: HwanAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: YoumyouPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: YoumyouAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: YoumyouAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: SansinPeace(CMonster* pMonster)			
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: SansinAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: SansinAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: SagaikPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: SagaikAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: SagaikAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: BuHeyPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: BuHeyAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: BuHeyAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: HeulkyojangroPeace(CMonster* pMonster)	
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: HeulkyojangroAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: HeulkyojangroAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: KangsiPeace(CMonster* pMonster)			
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: KangsiAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: KangsiAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
//////////////////////////////////////////////////////////////////////////
void CPatternNPC:: HungchunPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: HungchunAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: HungchunAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
///////////////////////////////////////////////////////////////////////////

void CPatternNPC:: TaPeace(CMonster* pMonster)			
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: TaAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: TaAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
///////////////////////////////////////////////////////////////////////////

void CPatternNPC:: SanjewangPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: SanjewangAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: SanjewangAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
///////////////////////////////////////////////////////////////////////////

void CPatternNPC:: BackhoPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: BackhoAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: BackhoAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
///////////////////////////////////////////////////////////////////////////

void CPatternNPC:: KumongPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: KumongAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: KumongAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
///////////////////////////////////////////////////////////////////////////

void CPatternNPC:: WomawangPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: WomawangAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: WomawangAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
///////////////////////////////////////////////////////////////////////////

void CPatternNPC:: JujakPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: JujakAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: JujakAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
///////////////////////////////////////////////////////////////////////////

void CPatternNPC:: HyunmuPeace(CMonster* pMonster)		
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: HyunmuAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: HyunmuAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
///////////////////////////////////////////////////////////////////////////

void CPatternNPC:: KirinPeace(CMonster* pMonster)	
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: KirinAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: KirinAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
///////////////////////////////////////////////////////////////////////////

void CPatternNPC:: YoungPeace(CMonster* pMonster)	
{
	DefaultMonsterPeacePattern(pMonster);
}
void CPatternNPC:: YoungAnger(CMonster* pMonster)
{
	DefaultMonsterAngerPattern(pMonster);
}
void CPatternNPC:: YoungAttack(CMonster* pMonster)
{
	DefaultMonsterAttackPattern(pMonster);
}
///////////////////////////////////////////////////////////////////////////


















