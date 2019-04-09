// SkillInfo.cpp: implementation of the CSkillInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillInfo.h"


#include "SkillObjectFactory.h"
#include "ItemManager.h"

#ifdef _MAPSERVER_
#include "SkillObject_server.h"

#include "CharMove.h"

#else

#include "SkillObject_Client.h"

#include "ActionTarget.h"

#include "GameIn.h" 
#include "Hero.h"

#include "MoveManager.h"
#include "ObjectStateManager.h"
#include "ChatManager.h"
#include "BattleSystem_client.h"
#include "ObjectManager.h"
#include "SkillManager_client.h"

#include "MussangManager.h"
#include "VimuManager.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillInfo::CSkillInfo()
{
	//////////////////////////////////////////////////
	// 06. 07. SAT 변경 - 이영준
	// 남여 구분의 의미가 없으므로
	// 여케릭 데이터를 활용 무초 딜레이 감소에 쓴다
	m_SkillOperateAnimationTime[GENDER_MALE] = 10000;
	m_SkillOperateAnimationTime[GENDER_FEMALE] = 0;
	//////////////////////////////////////////////////

	m_BeforeSkill = 0;
	m_AfterSkill = 0;
	memset(m_SkillTree,0,sizeof(WORD)*5);
}

CSkillInfo::~CSkillInfo()
{
	PTRLISTSEARCHSTART(m_SkillObjectPool,CSkillObject*,pSObj)
		delete pSObj;
	PTRLISTSEARCHEND;
	m_SkillObjectPool.RemoveAll();
}

BOOL CSkillInfo::InitSkillInfo(CMHFile* pFile)
{
	ASSERT(pFile->IsInited());
	ASSERT(pFile->IsEOF() == FALSE);

	memset(&m_SkillInfo,0,sizeof(m_SkillInfo));

	m_SkillInfo.SkillIdx = pFile->GetWord();

//	pFile->GetString(m_SkillInfo.SkillName);
	SafeStrCpy( m_SkillInfo.SkillName, pFile->GetString(), MAX_NAME_LENGTH+1 );
	m_SkillInfo.SkillTooltipIdx = pFile->GetWord();
	m_SkillInfo.RestrictLevel = pFile->GetWord();
	m_SkillInfo.LowImage = pFile->GetInt();
	m_SkillInfo.HighImage = pFile->GetInt();
	m_SkillInfo.SkillKind = pFile->GetWord();
	m_SkillInfo.WeaponKind = pFile->GetWord();
	m_SkillInfo.SkillRange = pFile->GetWord();
	m_SkillInfo.TargetKind = pFile->GetWord();
	m_SkillInfo.TargetRange = pFile->GetWord();
	m_SkillInfo.TargetAreaIdx = pFile->GetWord();
	m_SkillInfo.TargetAreaPivot = pFile->GetWord();
	m_SkillInfo.TargetAreaFix = pFile->GetWord();
	m_SkillInfo.MoveTargetArea = pFile->GetWord();
	m_SkillInfo.MoveTargetAreaDirection = pFile->GetWord();
	m_SkillInfo.MoveTargetAreaVelocity = pFile->GetFloat();
	m_SkillInfo.Duration = pFile->GetDword();
	m_SkillInfo.Interval = pFile->GetWord();
	m_SkillInfo.DelaySingleEffect = pFile->GetWord();	
	m_SkillInfo.ComboNum = pFile->GetWord();
	m_SkillInfo.Life = pFile->GetWord();
	m_SkillInfo.BindOperator = pFile->GetWord();
	m_SkillInfo.EffectStartTime = pFile->GetWord();
	m_SkillInfo.EffectStart = FindEffectNum(pFile->GetString());
	m_SkillInfo.EffectUse = FindEffectNum(pFile->GetString());
	m_SkillInfo.EffectSelf = FindEffectNum(pFile->GetString());
	m_SkillInfo.EffectMapObjectCreate = FindEffectNum(pFile->GetString());
	m_SkillInfo.EffectMineOperate = FindEffectNum(pFile->GetString());
	m_SkillInfo.DelayTime = pFile->GetDword();
	m_SkillInfo.FatalDamage = pFile->GetWord();
	for(int n=0;n<12;++n)
		m_SkillInfo.NeedExp[n] = pFile->GetExpPoint();
	pFile->GetWord(m_SkillInfo.NeedNaeRyuk,12);
	m_SkillInfo.Attrib = pFile->GetWord();
	m_SkillInfo.NegativeResultTargetType = pFile->GetWord();
	m_SkillInfo.TieUpType = pFile->GetWord();
	m_SkillInfo.ChangeSpeedType = pFile->GetWord();
	m_SkillInfo.ChangeSpeedRate = pFile->GetWord();
	m_SkillInfo.Dispel = pFile->GetWord();
	m_SkillInfo.PositiveResultTargetType = pFile->GetWord();
	m_SkillInfo.Immune = pFile->GetWord();
	m_SkillInfo.AIObject = pFile->GetWord();
	m_SkillInfo.MineCheckRange = pFile->GetWord();
	m_SkillInfo.MineCheckStartTime = pFile->GetWord();
	
	m_SkillInfo.CounterDodgeKind = pFile->GetWord();
	m_SkillInfo.CounterEffect = FindEffectNum(pFile->GetString());

//041213 KES decrease skill power
	m_SkillInfo.DamageDecreaseForDist = pFile->GetWord();
	
	WORD AdditiveAttr;
	WORD temp[12];
	
	for(n=0;n<6;++n)
	{
		AdditiveAttr = pFile->GetWord();
		ySWITCH(AdditiveAttr)
			yCASE(0)	pFile->GetWord(temp,12);
			yCASE(11)	pFile->GetFloat(m_SkillInfo.FirstPhyAttack,12);
			yCASE(12)	pFile->GetFloat(m_SkillInfo.UpPhyAttack,12);
			yCASE(13)	pFile->GetWord(m_SkillInfo.FirstAttAttackMin,12);
			yCASE(14)	pFile->GetWord(m_SkillInfo.FirstAttAttackMax,12);
			yCASE(15)	pFile->GetWord(m_SkillInfo.ContinueAttAttack,12);

			yCASE(20)	pFile->GetFloat(m_SkillInfo.CounterPhyAttack,12);
			yCASE(21)	pFile->GetFloat(m_SkillInfo.CounterAttAttack,12);
			yCASE(22)	pFile->GetFloat(m_SkillInfo.UpPhyDefence,12);
			yCASE(23)	pFile->GetFloat(m_SkillInfo.UpAttDefence,12);
			yCASE(24)	pFile->GetWord(m_SkillInfo.UpMaxNaeRyuk,12);
			yCASE(25)	pFile->GetWord(m_SkillInfo.UpMaxLife,12);
			yCASE(26)	pFile->GetWord(m_SkillInfo.ContinueRecoverNaeRyuk,12);
			yCASE(27)	pFile->GetWord(m_SkillInfo.FirstRecoverNaeRyuk,12);
			yCASE(28)	pFile->GetWord(m_SkillInfo.ContinueRecoverLife,12);
			yCASE(29)	pFile->GetWord(m_SkillInfo.FirstRecoverLife,12);
			yCASE(30)	pFile->GetFloat(m_SkillInfo.CounterDodgeRate,12);

			yCASE(31)	pFile->GetFloat(m_SkillInfo.CriticalRate,12);
			yCASE(32)	pFile->GetFloat(m_SkillInfo.StunRate,12);
			yCASE(33)	pFile->GetWord(m_SkillInfo.StunTime,12);
			
			
			yCASE(34)	pFile->GetWord(m_SkillInfo.UpMaxShield,12);			
			yCASE(35)	pFile->GetWord(m_SkillInfo.AmplifiedPowerPhy,12);			
			yCASE(36)	pFile->GetFloat(m_SkillInfo.VampiricLife,12);
			yCASE(37)	pFile->GetFloat(m_SkillInfo.VampiricNaeryuk,12);			
			yCASE(38)	pFile->GetFloat(m_SkillInfo.RecoverStateAbnormal,12);
					
			yCASE(39)	pFile->GetFloat(m_SkillInfo.DispelAttackFeelRate,12);
			yCASE(40)	pFile->GetFloat(m_SkillInfo.ChangeSpeedProbability,12);
			
			yCASE(41)	pFile->GetWord(m_SkillInfo.DownMaxLife,12);			
			yCASE(42)	pFile->GetWord(m_SkillInfo.DownMaxNaeRyuk,12);			
			yCASE(43)	pFile->GetWord(m_SkillInfo.DownMaxShield,12);
			yCASE(44)	pFile->GetFloat(m_SkillInfo.DownPhyDefence,12);			
			yCASE(45)	pFile->GetFloat(m_SkillInfo.DownAttDefence,12);	
			yCASE(46)	pFile->GetFloat(m_SkillInfo.DownPhyAttack,12);
	
			yCASE(47)	pFile->GetDword(m_SkillInfo.SkillAdditionalTime,12);
			yCASE(48)	pFile->GetWord(m_SkillInfo.AmplifiedPowerAtt,12);
			yCASE(49)	pFile->GetFloat(m_SkillInfo.AmplifiedPowerAttRate,12);

			yCASE(50)	pFile->GetFloat(m_SkillInfo.FirstAttAttack,12);
			yCASE(51)	pFile->GetFloat(m_SkillInfo.ContinueAttAttackRate,12);
			
			yCASE(52)	pFile->GetFloat(m_SkillInfo.DamageRate,12);
			yCASE(53)	pFile->GetFloat(m_SkillInfo.AttackRate,12);
			yCASE(54)	pFile->GetWord(m_SkillInfo.ContinueRecoverShield,12);
			yCASE(55)	pFile->GetFloat(m_SkillInfo.AttackLifeRate,12);
			yCASE(56)	pFile->GetFloat(m_SkillInfo.AttackShieldRate,12);
			yCASE(57)	pFile->GetFloat(m_SkillInfo.AttackSuccessRate,12);
			yCASE(58)	pFile->GetFloat(m_SkillInfo.VampiricReverseLife,12);
			yCASE(59)	pFile->GetFloat(m_SkillInfo.VampiricReverseNaeryuk,12);
			yCASE(60)	pFile->GetDword(m_SkillInfo.AttackPhyLastUp,12);
			yCASE(61)	pFile->GetDword(m_SkillInfo.AttackAttLastUp,12);
//			yCASE(62)	pFile->GetFloat(m_SkillInfo.UpCriticalRate,12);

			////////////////////////////////////////////////////////
			//06. 06 2차 전직 - 이영준
			//이펙트 생략(무초)
			yCASE(100)	
				m_SkillInfo.SkipEffect = pFile->GetWord();
				for(int i = 0; i < 11; i++)
					pFile->GetWord();
			////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////
			// 06. 06. 2차 전직 - 이영준
			// 은신/혜안
			// 경공과 같이 On/Off 형식의 자신의 상태 변화용
			yCASE(101)	
				m_SkillInfo.SpecialState = pFile->GetWord();
				for(int i = 0; i < 11; i++)
					pFile->GetWord();
			////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////
			// 06. 08. 2차 보스 - 이영준
			// 추가 회전 각
			// 2차 보스 스킬중 공격하는 대상보다 조금 틀어진 방향을 향해야 하는 스킬을 위해 추가
			// 일반 몬스터도 사용가능
			yCASE(200)	
				m_SkillInfo.AddDegree = pFile->GetInt();
				for(int i = 0; i < 11; i++)
					pFile->GetInt();
			////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////
			// 06. 08. 2차 보스 - 이영준
			// 안전거리
			// 안전거리 안쪽은 타겟이 아니다
			yCASE(201)	
				m_SkillInfo.SafeRange = pFile->GetWord();
				for(int i = 0; i < 11; i++)
					pFile->GetWord();
			////////////////////////////////////////////////////////
		yENDSWITCH
	}	

	////////////////////////////////////////////////////////
	//06. 06 2차 전직 - 이영준
	//이펙트 생략(무초) 가능여부

	m_SkillInfo.CanSkipEffect = pFile->GetBool();
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환
	// 변환 유형 - 0: 변환불가, 1: 공격형, 2:보조형, 3:모두가능
	m_SkillInfo.ChangeKind = pFile->GetWord();
	////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 임시 교정
	
	// 자기중심범위무공인데 사정거리보다 범위가 작은경우
	if( m_SkillInfo.TargetAreaPivot == 1 && 
		m_SkillInfo.TargetRange != 0 &&
		m_SkillInfo.SkillRange > m_SkillInfo.TargetRange )
		m_SkillInfo.TargetRange = m_SkillInfo.SkillRange;

	//////////////////////////////////////////////////////////////////////////

	//SW070127 타이탄
	m_SkillInfo.LinkSkillIdx = pFile->GetWord();
	
	return TRUE;
}

#define SKILLDUMMYCHECK 0x74
BOOL CSkillInfo::InitDummySkillInfo(CSkillInfo* pOriginalSkillInfo)
{
	memcpy(&m_SkillInfo,&pOriginalSkillInfo->m_SkillInfo,sizeof(SKILLINFO));
	char* p = (char*)&m_SkillInfo;
	for(int n=0;n<sizeof(SKILLINFO);++n)
	{
		p[n] ^= SKILLDUMMYCHECK;
	}

	return TRUE;
}

BOOL CSkillInfo::CheckOriginal(CSkillInfo* pSkillInfo)
{
	char* pOriginal = (char*)&pSkillInfo->m_SkillInfo;
	char* pDummy = (char*)&m_SkillInfo;

	for(int n=0;n<sizeof(SKILLINFO);++n)
	{
		if(pOriginal[n] != ( pDummy[n] ^ SKILLDUMMYCHECK ))
			return FALSE;
	}
	return TRUE;
}


#ifdef _MAPSERVER_
#include "Battle.h"
#include "Player.h"
#include "Titan.h"
#include "PartyManager.h"
#include "Party.h"

BOOL CSkillInfo::IsExcutableSkillState(CObject* pObject,int SkillLevel, SKILLOPTION* pOption)
{	
	if(pObject->GetLevel() < m_SkillInfo.RestrictLevel)
	{
		return FALSE;
	}
	////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신 / 혜안
	// 은신 패널티
	if(m_SkillInfo.SpecialState)
	{
		if(m_SkillInfo.SpecialState == eSingleSpecialState_Hide)
		{
			if(pObject->GetObjectKind() == eObjectKind_Player)
			{
				CPlayer* pPlayer = (CPlayer*)pObject;
				if( pPlayer->GetPetManager()->GetCurSummonPet() )
					return FALSE;

				if( pPlayer->IsVimuing() )
					return FALSE;
			}
		}
		else if(m_SkillInfo.SpecialState == eSingleSpecialState_Detect)
		{
			if(pObject->GetSingleSpecialState(eSingleSpecialState_DetectItem))
				return FALSE;
		}
	}
	else if(pObject->GetSingleSpecialState(eSingleSpecialState_Hide))
	{
		return FALSE;
	}

	// 타이탄 탑승중이면
	if( pObject->GetObjectKind() == eObjectKind_Player && ((CPlayer*)pObject)->InTitan() )
	{
		CPlayer* pPlayer = (CPlayer*)pObject;

		if( !pPlayer->CanUseTitanSkill() )
			return FALSE;

		if(pOption)	// 타이탄에게도 적용
		{
			if((int)((pPlayer->GetCurTitan())->GetMaxNaeRyuk()) < pOption->NaeRyuk * -1)	return FALSE;
			if((int)((pPlayer->GetCurTitan())->GetMaxLife()) < pOption->Life * -1) return FALSE;
		}
	}
	else
	{
		////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////
		// 06. 06. 2차 전직 - 이영준
		// 무공 변환
		// 제한사항
		if(pOption)
		{
			if((int)(pObject->GetMaxNaeRyuk()) < pOption->NaeRyuk * -1)	return FALSE;
			if((int)(pObject->GetMaxLife()) < pOption->Life * -1) return FALSE;
			if((int)(pObject->GetMaxShield()) < pOption->Shield * -1) return FALSE;
		}
		////////////////////////////////////////////////////////////////////////////////
	}
	if(pObject->IsInSpecialState(eSpecialState_Stun) == TRUE)
		return FALSE;
/*
	if(pObject->GetState() != eObjectState_None &&
		pObject->GetState() != eObjectState_Move &&
		pObject->GetState() != eObjectState_TiedUp_CanSkill )
		return FALSE;
*/	
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		////////////////////////////////////////////////////////////////////////////
		// 06. 07. 상태강제변경 - 이영준
		// 일정이상 스킬 사용실패시 특별한 상태를 제외한 나머지 상태는 모두 초기화
		if(!((CPlayer*)pObject)->CanSkillState())
			return FALSE;
		////////////////////////////////////////////////////////////////////////////

		if( ((CPlayer*)pObject)->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Skill,GetSkillIndex()) != 0)
		{
//			((CPlayer*)pObject)->SpeedHackCheck();
			return FALSE;
		}

		//// 장착된 무기로 사용불가 스킬일 경우 리턴
		//if( m_SkillInfo.WeaponKind != 0 )
		//{
		//	if( ((CPlayer*)pObject)->GetWeaponEquipType() != m_SkillInfo.WeaponKind )
		//		return FALSE;
		//}
	}

	WORD MogongNaeRyuk = GetNeedNaeRyuk(SkillLevel);
	// RaMa - 04.11.24  ->내력소모비율
	DWORD NeedNaeRyuk = (DWORD)(MogongNaeRyuk*gEventRate[eEvent_NaeRuykRate]);

	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		if( ((CPlayer*)pObject)->GetPartyIdx() )
		{
			CParty* pParty = PARTYMGR->GetParty( ((CPlayer*)pObject)->GetPartyIdx() );
			if( pParty )
			{
				int count = pParty->GetMemberCountofMap( pObject->GetID() );
				if( count && gPartyEvent[ePartyEvent_NaeRyukRate].Rate[count-1] )
					NeedNaeRyuk = (DWORD)(NeedNaeRyuk*gPartyEvent[ePartyEvent_NaeRyukRate].Rate[count-1]);
			}
		}

		// RaMa - 05.02.07   ->정신의 부적
		// NeedNaeRyuk *= (1.0f - ((CPlayer*)pObject)->GetShopItemStats()->NeaRuykSpend*0.01f);
		// AvatarItem에 내력소모감소옵션 추가
		WORD NaeryukSpend = ((CPlayer*)pObject)->GetShopItemStats()->NeaRyukSpend + ((CPlayer*)pObject)->GetAvatarOption()->NeaRyukSpend 
			+ ((CPlayer*)pObject)->GetShopItemStats()->PlustimeNaeruyk;
		if( NaeryukSpend >= 100 )
			NeedNaeRyuk = 0;
		else
			NeedNaeRyuk = (DWORD)(NeedNaeRyuk*(1.0f - NaeryukSpend*0.01f));


#ifdef _JAPAN_LOCAL_
		//SW051112 무쌍모드
		if( ((CPlayer*)pObject)->IsMussangMode() )
		{
			NeedNaeRyuk = (DWORD)(NeedNaeRyuk * 0.25f);
		}
#else
		// 06. 03 국내무쌍 - 이영준
		if( ((CPlayer*)pObject)->IsMussangMode() )
		{
			switch(((CPlayer*)pObject)->GetStage())
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


		//////////////////////////////////////////////////////////////////////////
		// 06. 06. 2차 전직 - 이영준
		// 무공 변환 추가
		// 내력소모
		if(pOption)
		{
			float ReduceNaeRyuk = 0.0f;

			ReduceNaeRyuk += (((CPlayer*)pObject)->GetSkillStatsOption()->ReduceNaeRyuk + pOption->ReduceNaeRyuk);

			NeedNaeRyuk += (DWORD)((NeedNaeRyuk * ReduceNaeRyuk) + 0.5);

			if(NeedNaeRyuk < 0)
				NeedNaeRyuk = 0;
		}	
		//////////////////////////////////////////////////////////////////////////
	}

	// magi82 - Titan(070914) 타이탄 무공업데이트 -> 타이탄 탑승유무를 떠나서 무조건 캐릭터의 내력을 소모하는걸로 바꿈
	// 타이탄 탑승상태이면
	//if( pObject->GetObjectKind() == eObjectKind_Player && ((CPlayer*)pObject)->InTitan() )
	//{
	//	if( NeedNaeRyuk )
	//	{
	//		CPlayer* pPlayer = (CPlayer*)pObject;

	//		CTitan* pTitan = pPlayer->GetCurTitan();
	//		if( pTitan->GetTotalInfo()->Spell < NeedNaeRyuk )
	//			return FALSE;

	//		pTitan->ReduceSpell( NeedNaeRyuk );
	//	}
	//}
	//else
	{
		if(NeedNaeRyuk)
		{
			if(pObject->GetNaeRyuk() < NeedNaeRyuk)
			{
				return FALSE;
			}

			pObject->ReduceNaeRyuk(NeedNaeRyuk);

		}	//if( HERO->InTitan() ){} else{

		/*//옮김! 해킹때문에
		if( pObject->GetObjectKind() == eObjectKind_Player && 
		g_pServerSystem->GetMap()->IsVillage() == FALSE &&
		pObject->GetBattle()->GetBattleKind() != eBATTLE_KIND_VIMUSTREET)
		{
		CPlayer* pPlayer = (CPlayer*)pObject;

		DWORD MugongExp = MogongNaeRyuk;

		// RaMa - 04.11.24   ->무공경험치는 내력소모비율과 상관없이

		// 중국쪽 악명패널티
		if(g_pServerSystem->GetNation() == eNATION_CHINA)
		{
		if(pPlayer->GetBadFame() >= 60)
		MugongExp = MogongNaeRyuk/4;
		else if(pPlayer->GetBadFame() >= 30)
		MugongExp = MogongNaeRyuk/2;
		else
		MugongExp = MogongNaeRyuk;
		}

		pPlayer->AddExpPointToMugong(GetSkillIndex(),MugongExp);
		}
		*/
	}
/*
	if( pObject->GetLevel() < m_SkillInfo.RestrictLevel &&
		m_SkillInfo.ComboNum != SKILL_COMBO_NUM )		// 일단무공은 레벨제한 뺀다.
		return FALSE;
*/
	
	return TRUE;
}

BOOL CSkillInfo::IsInSkillRange(CObject* pObject,VECTOR3* pTargetPos,float TargetRadius, SKILLOPTION* pOption)
{
	VECTOR3* p1 = CCharMove::GetPosition(pObject);
	VECTOR3* p2 = pTargetPos;
	p1->y = p2->y = 0;

	// 공격자와 맞는 넘의 타겟거리를 뺀다.
	float dist = CalcDistanceXZ(p1,p2) - TargetRadius;
	float addRange = 0.f;
	
	if(m_SkillInfo.ComboNum != SKILL_COMBO_NUM &&
		(m_SkillInfo.WeaponKind == WP_AMGI || m_SkillInfo.WeaponKind == WP_GUNG))		// 콤보가 아니면 스탯에 따라서 사정거리 늘어남
		addRange = pObject->GetAddAttackRange();

	// 공격자가 플레이어 이면 사정거리를 300늘려준다. (오차 고려)
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		dist -= 300.f;

		//////////////////////////////////////////////////////////////////////////
		// 06. 06. 2차 전직 - 이영준
		// 무공 변환 추가	
		// 사정거리
		if(pOption)
			addRange += (((CPlayer*)pObject)->GetSkillStatsOption()->Range + pOption->Range);
		//////////////////////////////////////////////////////////////////////////

		// magi82 UniqueItem(070627)
		addRange += ((CPlayer*)pObject)->GetUniqueItemStats()->nRange;

		// magi82(16) - Titan(071101) 타이탄 아이템 옵션중 사정거리 적용
		if( ((CPlayer*)pObject)->InTitan() == TRUE )
		{
			CItemSlot* pSlot = ((CPlayer*)pObject)->GetSlot(eItemTable_Titan);
			ITEMBASE* pItemBase = (ITEMBASE*)pSlot->GetItemInfoAbs(TP_TITANWEAR_START + eTitanWearedItem_Weapon);
			WORD wTitanWeaponIdx = pItemBase->wIconIdx;
			if( wTitanWeaponIdx )
			{
				ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wTitanWeaponIdx);
				if( pItemInfo && pItemInfo->MugongNum > 0 )
				{
					addRange += pItemInfo->MugongNum;
				}
			}
		}
	}

	float dis = (float)m_SkillInfo.SkillRange+addRange;
	if(dis < 0)
		dis = 0;

	if( dis < dist)	// 서버는 
		return FALSE;
		
	return TRUE;
}

BOOL CSkillInfo::ComboTest(CObject* pObject)
{
	return TRUE;
}

BOOL CSkillInfo::CheckEvade(MAINTARGET* pMainTarget)
{
	return FALSE;
}
#else //_MAPSERVER_

BOOL CSkillInfo::IsExcutableSkillState(CHero* pHero,int SkillLevel, SKILLOPTION* pOption)
{
	if(pHero->GetLevel() < m_SkillInfo.RestrictLevel)
	{
		// 레벨이 부족하여 무공을 시전하실 수 없습니다.
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(999) );
		return FALSE;
	}
	////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신 / 혜안
	// 은신 패널티
	if(m_SkillInfo.SpecialState)
	{
        DWORD time = pHero->GetSingleSpecialStateUsedTime(m_SkillInfo.SpecialState);
 		SPECIAL_STATE_INFO* pInfo = SKILLMGR->GetSpecialStateInfo(m_SkillInfo.SpecialState);

		if(!pInfo)
			return FALSE;

		int delay = (time + pInfo->DelayTime) - gCurTime;
		if(time != 0 && delay > 0)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1355), delay / 1000);
			return FALSE;
		}

		if(m_SkillInfo.SpecialState == eSingleSpecialState_Hide)
		{
			if(pHero->GetPet())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1330));
				return FALSE;
			}

			if(VIMUMGR->IsVimuing())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1330));
				return FALSE;
			}

			if(pHero->IsPKMode())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1330));
				return FALSE;
			}
		}
		else if(m_SkillInfo.SpecialState == eSingleSpecialState_Detect)
		{
			if(pHero->GetSingleSpecialState(eSingleSpecialState_DetectItem))
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1332));
				return FALSE;
			}
		}

	}
	else if(pHero->GetSingleSpecialState(eSingleSpecialState_Hide))
	{
		return FALSE;
	}
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환
	// 제한사항
	if(pOption)
	{
		BOOL bVal = FALSE;

		if( pHero->InTitan() )
		{
			if( !HERO->CanUseTitanSkill() )
			{
				// CHATMSG
				return FALSE;
			}
			//!!! 임시
			/*
			CTitan* pHeroTitan = pHero->GetCurTitan();
			if((int)pHeroTitan->GetMaxLife() < pOption->Life * -1)	bVal = TRUE;
			if((int)pHeroTitan->GetMaxNaeRyuk() < pOption->NaeRyuk * -1) bVal = TRUE;
			//if((int)(pHeroTitan->GetMaxShield()) < pOption->Shield * -1)	bVal = TRUE;
			*/

			if(bVal)
			{
				//CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(798());
				return FALSE;
			}

		}
		else
		{
			if((int)(pHero->GetMaxNaeRyuk()) < pOption->NaeRyuk * -1)	bVal = TRUE;
			if((int)(pHero->GetMaxLife()) < pOption->Life * -1)	bVal = TRUE;
			if((int)(pHero->GetMaxShield()) < pOption->Shield * -1)	bVal = TRUE;

			if(bVal)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(798));
				return FALSE;
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////
	/*
	if(pHero->IsSkillCooling() == TRUE)
	{
		// 쿨타임이 끝나야 무공을 다시 시전하실수 있습니다.
		return FALSE;
	}
	*/
	
	if(pHero->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Skill,GetSkillIndex()) != 0)
		return FALSE;

	//if( m_SkillInfo.WeaponKind != 0 )
	//{
	//	if( pHero->GetWeaponEquipType() != m_SkillInfo.WeaponKind )
	//	{
	//		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(151) );
	//		return FALSE;
	//	}
	//}
	
#ifndef _TESTCLIENT_
	//////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 내력소모 감소효과 체크 - 이영준
	// 클라이언트는 내력소모 감소효과 체크를 안하던 것을 추가 
	
	DWORD NeedNaeRyuk = GetNeedNaeRyuk(SkillLevel);
	
	if(NeedNaeRyuk)
	{
		if( HERO->InTitan() )
		{
			if( pOption )
			{
				float ReduceNaeRyuk = 0.0f;

				ReduceNaeRyuk += (pHero->GetSkillStatsOption()->ReduceNaeRyuk + pOption->ReduceNaeRyuk);

				NeedNaeRyuk += (DWORD)((NeedNaeRyuk * ReduceNaeRyuk) + 0.5);

				if(NeedNaeRyuk < 0)
					NeedNaeRyuk = 0;
			}

			// magi82(6) - Titan(071024) 티이탄 무공을 쓸때도 캐릭터의 내력을 체크하게 수정
			if( pHero->GetNaeRyuk() < NeedNaeRyuk )		// 내력검사
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(401) );
				return FALSE;
			}
		}
		else
		{
			WORD NaeryukSpend = HERO->GetShopItemStats()->NeaRyukSpend + HERO->GetAvatarOption()->NeaRyukSpend 
				+ HERO->GetShopItemStats()->PlustimeNaeruyk;
			NeedNaeRyuk = (DWORD)(NeedNaeRyuk*(1.0f - NaeryukSpend*0.01f));

#ifdef _JAPAN_LOCAL_
			if( HERO->IsMussangMode() )
			{
				NeedNaeRyuk = (DWORD)(NeedNaeRyuk * 0.25f);
			}
#else
			if( MUSSANGMGR->IsMussangMode() )
			{
				switch(HERO->GetStage())
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
			//////////////////////////////////////////////////////////////////////////
			// 06. 06. 2차 전직 - 이영준
			// 무공 변환 추가	
			// 내력소모
			if(pOption)
			{
				float ReduceNaeRyuk = 0.0f;

				ReduceNaeRyuk += (pHero->GetSkillStatsOption()->ReduceNaeRyuk + pOption->ReduceNaeRyuk);

				NeedNaeRyuk += (DWORD)((NeedNaeRyuk * ReduceNaeRyuk) + 0.5);

				if(NeedNaeRyuk < 0)
					NeedNaeRyuk = 0;
			}
			//////////////////////////////////////////////////////////////////////////

			//if( pHero->GetNaeRyuk() < GetNeedNaeRyuk(SkillLevel) )		// 내력검사
			if( pHero->GetNaeRyuk() < NeedNaeRyuk )		// 내력검사
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(401) );
				return FALSE;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////////
#endif //_TESTCLIENT_

//	if( pHero->GetNaeRyuk() < GetNeedNaeRyuk(SkillLevel) )		// 내력검사
//	{
//		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(401) );
//		return FALSE;
//	}
	if( pHero->IsInSpecialState(eSpecialState_Stun) == TRUE)
		return FALSE;

	if( pHero->GetState() != eObjectState_None &&
		pHero->GetState() != eObjectState_Move &&
		pHero->GetState() != eObjectState_TiedUp_CanSkill &&
		pHero->GetState() != eObjectState_Society
		) 
	{

		if( pHero->GetState() != eObjectState_SkillSyn &&
			pHero->GetState() != eObjectState_SkillUsing &&
			pHero->GetState() != eObjectState_SkillBinding &&
			pHero->GetState() != eObjectState_SkillStart &&			
			pHero->GetState() != eObjectState_SkillDelay )
		{
			//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(141) );
			pHero->DisableAutoAttack();
		}

		return FALSE;
	}

	if( pHero->GetState() == eObjectState_Society )
	{
		if( pHero->EndSocietyAct() == FALSE )
		{
			pHero->DisableAutoAttack();
			return FALSE;
		}
	}

	// 레벨 제한
	if( pHero->GetLevel() < m_SkillInfo.RestrictLevel &&
		m_SkillInfo.ComboNum != SKILL_COMBO_NUM) // 일단 무공은 레벨 제한 없음
		return FALSE;

	//if PeaceMode == TRUE then Can't Attack
//KES 막음.. 평화모드시엔 자동으로 전투모드로 바꾸어서 공격!	
//	if(pHero->GetCharacterTotalInfo()->bPeace == TRUE)
//	{
////		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(142) );
//		return FALSE;
//	}

	return TRUE;
}

BOOL CSkillInfo::IsInSkillRange(CHero* pHero,CActionTarget* pTarget, SKILLOPTION* pOption)
{
	VECTOR3 p1;
	pHero->GetPosition(&p1);
	VECTOR3* p2 = pTarget->GetTargetPosition();
	VECTOR3 hpos;
	pHero->GetPosition(&hpos);
	if(p2 == NULL)
		return FALSE;

	p1.y = p2->y = 0;
	
	// 공격자와 맞는 넘의 타겟거리를 뺀다.
	float dist = CalcDistanceXZ(&p1,p2) - pTarget->GetRadius();
	float addRange = 0;

	if( m_SkillInfo.ComboNum != SKILL_COMBO_NUM &&
		(m_SkillInfo.WeaponKind == WP_AMGI || m_SkillInfo.WeaponKind == WP_GUNG))		// 콤보가 아니면 스탯에 따라서 사정거리 늘어남
		addRange = pHero->GetAddAttackRange();

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가	
	// 사정거리
	if(pOption)
		addRange += (pHero->GetSkillStatsOption()->Range + pOption->Range);
	//////////////////////////////////////////////////////////////////////////

	// magi82 UniqueItem(070627)
	addRange += pHero->GetUniqueItemStats()->nRange;

	// magi82(16) - Titan(071101) 타이탄 아이템 옵션중 사정거리 적용
	if( pHero->InTitan() == TRUE )
	{
		WORD wTitanWeaponIdx = pHero->GetTitanWearedItemIdx(eTitanWearedItem_Weapon);
		if( wTitanWeaponIdx )
		{
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wTitanWeaponIdx);
			if( pItemInfo && pItemInfo->MugongNum > 0 )
			{
				addRange += pItemInfo->MugongNum;
			}
		}
	}

	float dis = m_SkillInfo.SkillRange+addRange;
	if(dis < 0)
		dis = 0;

	if( dis < dist)
		return FALSE;
		
	return TRUE;
}

BOOL CSkillInfo::ComboTest(CHero* pHero)
{
#ifdef _TESTCLIENT_
	return TRUE;
#endif
	//	2005 크리스마스 이벤트 코드
	if( GetSkillIndex() == COMBO_EVENT_MIN || GetSkillIndex() == COMBO_EVENT_HAMMER )
		return TRUE;
	
	DWORD curcombo = pHero->GetCurComboNum();
	if( m_SkillInfo.ComboNum == SKILL_COMBO_NUM &&
		curcombo != SKILL_COMBO_NUM )
		return TRUE;
	if( m_SkillInfo.ComboNum == curcombo +1 )
		return TRUE;

	return FALSE;
}

void CSkillInfo::SendMsgToServer(CHero* pOperator,CActionTarget* pTarget)
{
	MSG_SKILL_START_SYN msg;
	MAINTARGET MainTarget;	
	pTarget->GetMainTarget(&MainTarget);

	/////임시  /////////
	WORD wSkillIndex = GetSkillIndex();
	if( (wSkillIndex < 1) || (wSkillIndex > 10036) )
	{
		char str[256] ={0,};
		sprintf( str, "SkillIndex = %d의 범위가 벗어났습니다.", wSkillIndex );
		WRITEDEBUGFILE( str );
//#ifdef _GMTOOL_
//		printf("[Skill Index Error] : [%d] [%d]\n", pOperator->GetID(), msg.SkillIdx);
//#endif	
		return;
	}

//	CObject* pObjectTemp = OBJECTMGR->GetObject(pOperator->GetID());
//	if(NULL == pObjectTemp)
//	{
//#ifdef _GMTOOL_
//		printf("[Object ID Error] : [%d] [%d]\n", pOperator->GetID(), msg.SkillIdx);
//#endif	
//		return;
//	}
	///////////////////

	msg.InitMsg(GetSkillIndex(),&MainTarget,pOperator->GetAngleDeg(),pOperator->GetID());
	pTarget->GetTargetArray(&msg.TargetList);	

	NETWORK->Send(&msg,msg.GetMsgLength());

	// debug용
	if( SKILLMGR->m_nSkillUseCount != 0 )
	{
		char str[256] ={0,};
		sprintf( str, "SkillIndex = %d 스킬(%s) 사용횟수가 잘못되었습니다.", wSkillIndex, m_SkillInfo.SkillName );
		WRITEDEBUGFILE( str );
	}
	SKILLMGR->m_nSkillUseCount++;
}

BOOL CSkillInfo::IsValidTarget(CHero* pHero,CActionTarget* pTarget)
{
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pHero);
	CObject* pTargetObj = NULL;
	if(pTarget->GetTargetID() != 0)
		pTargetObj = OBJECTMGR->GetObject(pTarget->GetTargetID());

	if(pTargetObj == NULL)
		return TRUE;

	if( pTargetObj->GetObjectKind() == eObjectKind_Npc )
		return FALSE;

	if( pTargetObj->GetObjectKind() == eObjectKind_Pet )
		return FALSE;

    //2007. 10. 25. CBH - 전문기술일때 타겟 따라가는 현상 막는다
	if( SKILLMGR->CheckSkillKind(m_SkillInfo.SkillKind) == TRUE )
	{
		if( GetObjectKindGroup(pTargetObj->GetObjectKind()) == eOBJECTKINDGROUP_NONE )
			return FALSE;
	}

	if( m_SkillInfo.FirstPhyAttack[11] != 0 ||
		m_SkillInfo.FirstAttAttackMin[11] != 0 ||
		m_SkillInfo.FirstAttAttackMax[11] != 0 )		// 첫공격이 있으면
	{
		if( pBattle->IsEnemy(pHero,pTargetObj) == FALSE )
			return FALSE;
	}
	
	return TRUE;
}

BOOL CSkillInfo::ConvertTargetToSkill(CHero* pHero,CActionTarget* pTarget)
{
	if(GetSkillInfo()->TargetKind == 1) // 자신
	{
		pTarget->SetOneTarget(pHero);
		return TRUE;
	}
	else if(GetSkillInfo()->TargetKind == 0) // 상대
	{
		pTarget->SetOneTarget(NULL);
		return TRUE;
	}
	else
	{
		return TRUE;
	}
}

#endif

CSkillObject* CSkillInfo::GetSkillObject()
{
	CSkillObject* pSObj;
	if(m_SkillObjectPool.GetCount() == 0)
		pSObj = CSkillObjectFactory::MakeNewSkillObject(this);
	else
		pSObj = (CSkillObject*)m_SkillObjectPool.RemoveTail();
	return pSObj;
}

void CSkillInfo::ReleaseSkillObject(CSkillObject* pSObj,int kind)
{
	PTRLISTSEARCHSTART(m_SkillObjectPool,CSkillObject*,pPreObj)
		if(pSObj == pPreObj)
		{			
			return;
		}
	PTRLISTSEARCHEND

#ifdef _MAPSERVER_
	pSObj->releaseKind = kind;
#endif
	
	m_SkillObjectPool.AddHead(pSObj);
}

VECTOR3* CSkillInfo::GetTargetAreaPivotPos(VECTOR3* pOperatorPos,VECTOR3* pTargetPos)
{	
	if(IsTargetAreaFitToHero())
		return pOperatorPos;
	else
		return pTargetPos;
}

BOOL CSkillInfo::IsMultipleTargetSkill()
{
	if( m_SkillInfo.TargetRange != 0 ||
		m_SkillInfo.TargetAreaIdx != 0)
		return TRUE;
	else
		return FALSE;
}

DWORD CSkillInfo::GetSkillOperateAnimationTime(int gender)
{
	if(gender >= GENDER_MAX)
	{
		ASSERT(0);
		gender = GENDER_MALE;
	}

	return m_SkillOperateAnimationTime[gender];
}

void CSkillInfo::SetSkillTree(WORD Before,WORD After,WORD* pArray)
{
	m_AfterSkill = After;
	m_BeforeSkill = Before;
	memcpy(&m_SkillTree,pArray,sizeof(WORD)*5);
}
