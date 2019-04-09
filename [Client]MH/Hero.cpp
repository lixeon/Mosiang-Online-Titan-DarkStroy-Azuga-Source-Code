// Hero.cpp: implementation of the CHero class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Hero.h"
#include "GameIn.h"
#include "MHCamera.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "ObjectStateManager.h"
//#include "MinimapDialog.h"
#include "ItemManager.h"
#include "StatsCalcManager.h"
#include "SkillManager_Client.h"
#include "KyungGongManager.h"
#include "TacticManager.h"
#include "PartyManager.h"
#include "MugongManager.h"
#include "ObjectGuagen.h"
#include "./Audio/MHAudioManager.h"
#include "StreetStallManager.h"
#include "CheatMsgParser.h"
#include "interface/cWindowManager.h"
#include "OptionManager.h"
#include "WantedManager.h"
#include "ChatManager.h"
#include "TitanManager.h"

#include "MixDialog.h"
#include "ReinforceDlg.h"
#include "UpgradeDlg.h"
#include "DissolutionDialog.h"

#ifdef _DEBUG
#include "interface\cFont.h"
#include "ChatManager.h"
#endif


#include "WindowIdEnum.h"

#include "ReviveDialog.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "DealDialog.h"
#include "CharStateDialog.h"

#include "mhMap.h"
#include "GameEventManager.h"

#include "BattleSystem_Client.h"
#include "NpcScriptDialog.h"
#include "CommonCalcFunc.h"
#include "AppearanceManager.h"

#include "AbilityInfo.h"

#include "SkillPointRedist.h"
#include "SkillPointNotify.h"
#include "SuryunDialog.h"
#include "AbilityManager.h"

#include "ChaseinputDialog.h"
#include "ShoutDialog.h"
#include "MoveDialog.h"

#include "QuickDialog.h"
#include "PeaceWarModeManager.h"
#include "GuageDialog.h"
#include "MussangManager.h"
#include "WeatherManager.h"
#include "PetManager.h"
#include "InventoryExDialog.h"
#include "PyoGukDialog.h"
#include "cMsgBox.h"

#include "EventMapInfo.h"
#include "SiegeWarMgr.h"
#include "PartyWar.h"
#include "GuildFieldWar.h"
#include "GuildManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VECTOR3 gHeroPos;

CHero::CHero()
{
	m_NextAction.Clear();
	m_CurComboNum = 0;
	m_maxExpPoint = 0;
	m_PyogukNum = 0;	
	m_bUngiFlag = FALSE;
	m_AbilityGroup.SetOwenerObject(this);
}

CHero::~CHero()
{
	Release();
}

void CHero::InitHero(HERO_TOTALINFO * pHeroTotalInfo)
{	
	m_DelayGroup.Init();

	m_bSkillCooling = 0;
	m_MovingAction.Clear();
	m_NextAction.Clear();
	m_SkillStartAction.Clear();
	m_bIsAutoAttackMode = TRUE;
	m_bIsAutoAttacking = FALSE;
	m_bIsKyungGongMode = FALSE;

	memcpy(&m_HeroTotalInfo, pHeroTotalInfo, sizeof(HERO_TOTALINFO));
		
	m_maxExpPoint = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
	MUSSANGMGR->SetMussangReady(FALSE);
	MUSSANGMGR->SetMussangMaxPoint(pHeroTotalInfo->MaxMussangPoint);
//	m_HeroTotalInfo.KyungGongGrade = OPTIONMGR->GetKyungGongIdx();	//***
//	m_bIsKyungGongMode = OPTIONMGR->GetKyungGongMode();				//***

	//ÀÎÅÍÆäÀÌ½º¿¡ Àû¿ë
	//ApplyInterface(); RegisterHero·Î ¿Å±è
	STATSMGR->Clear(&m_itemStats);
	HERO->ClearSetitemOption();		// 2007. 6. 18. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ ±¸Á¶Ã¼ ÃÊ±âÈ­	

	m_dwLastSocietyAction = gCurTime;
	m_bActionPenalty = TRUE;
}


#define SKILL_PERSUIT_TICK	1000

void CHero::Process()
{
	static DWORD	dwSkillPersuitTime	= 0;

	CPlayer::Process();
	
	CAMERA->Process();	//? GaminÀÇ ÇÁ·Î¼¼½º¿¡ ÀÖ´Ù. KES 040406 //Çä.. ¿©±â¼­ ¾ÈÇÏ´Ï Ä«¸Þ¶ó°¡ Èçµé¸°´Ù.
	WEATHERMGR->Process();
	if(m_MovingAction.HasAction())
	{
		if(m_MovingAction.CheckTargetDistance(&GetCurPosition()) == TRUE)
		{
			MOVEMGR->HeroMoveStop();

			m_MovingAction.ExcuteAction(this);
			m_MovingAction.Clear();
		}
		else
		{
			if( m_MovingAction.GetActionKind() == eActionKind_Skill )
			{
				if( gCurTime - dwSkillPersuitTime > SKILL_PERSUIT_TICK )
				{
					m_MovingAction.ExcuteAction(this);
					dwSkillPersuitTime = gCurTime;
				}
			}
			else if( MOVEMGR->IsMoving(this) == FALSE )
			{
				m_MovingAction.Clear();
			}
			
//			if(MOVEMGR->IsMoving(this) == FALSE)
//			{
//				if(m_MovingAction.GetActionKind() == eActionKind_Skill)
//				{
//					m_MovingAction.ExcuteAction(this);
//				}
//				else
//					m_MovingAction.Clear();
//			}			
		}
		

	}
	else
	{
		if(m_bIsAutoAttacking)
		{
			if(m_NextAction.HasAction() == FALSE)
			{
				if(GetCurComboNum() < 2 || GetCurComboNum() == SKILL_COMBO_NUM)	// ÀÚµ¿°ø°ÝÀº ÄÞº¸ 2±îÁö¸¸	12/3ÀÏ È¸ÀÇ °á°ú 3¿¡¼­ 2·Î ¹Ù²ñ
				{
					if(SKILLMGR->OnSkillCommand(this,&m_AutoAttackTarget,FALSE) == FALSE)
						DisableAutoAttack();
				}
			}
		}
	}

	SKILLMGR->UpdateSkillObjectTargetList(this);

	// °æ°ø
	if(m_bIsKyungGongMode)
	{
		if( MOVEMGR->IsMoving(this) &&
			m_MoveInfo.KyungGongIdx == 0 &&
			GetKyungGongGrade() &&
			GetMoveSpeed() != 0 &&
			GetState() != eObjectState_Ungijosik)			
		{
			if(m_MoveInfo.m_bEffectMoving == FALSE)
			{
				// RaMa - 05.09.05 - °æ°øµô·¹ÀÌ°¡ ¾ø´Ù.
				if( GetAvatarOption()->bKyungGong || GetShopItemStats()->bKyungGong )
					MOVEMGR->ToggleHeroKyungGongMode();
/*	//SW º¢²É ÀÌº¥Æ® °ü·Ã ÀÓ½Ã ÇÏµå ÄÚµå
// 2005 Å©¸®½º¸¶½º ÀÌº¥Æ® ÄÚµå
				else if((WEATHERMGR->GetWeatherState() == eWS_Snow ) &&
					   ((GetShopItemStats()->Avatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS && GetShopItemStats()->Avatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT) ||
						(GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS && GetShopItemStats()->Avatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT) ||
						(GetShopItemStats()->Avatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS2 && GetShopItemStats()->Avatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT2) ||
						(GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS2 && GetShopItemStats()->Avatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT2) ||
						(GetShopItemStats()->Avatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS && GetShopItemStats()->Avatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT2) ||
						(GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS && GetShopItemStats()->Avatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT2) ||
						(GetShopItemStats()->Avatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS2 && GetShopItemStats()->Avatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT) ||
						(GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS2 && GetShopItemStats()->Avatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT) ||
						GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_SNOWMAN1_HK || GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_SNOWMAN2_HK ||
						GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_SNOWMAN3_HK || GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP1_HK ||
						GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP2_HK || GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP3_HK ))
*/
#define SHOPITEM_COS_CAT_HAT		56019
#define SHOPITEM_COS_CAT_DRESS		56020
#define SHOPITEM_COS_WEDDING_MAN	56021
#define SHOPITEM_COS_WEDDING_WOMAN	56022
				else if( (WEATHERMGR->GetWeatherState() == eWS_Snow ) &&
						( GetShopItemStats()->Avatar[eAvatar_Dress] ==  SHOPITEM_COS_CAT_DRESS ||
						GetShopItemStats()->Avatar[eAvatar_Dress] ==  SHOPITEM_COS_WEDDING_MAN ||
						GetShopItemStats()->Avatar[eAvatar_Dress] ==  SHOPITEM_COS_WEDDING_WOMAN )
						)
					MOVEMGR->ToggleHeroKyungGongMode();
				else
				{
					#ifdef TAIWAN_LOCAL
						MOVEMGR->ToggleHeroKyungGongMode();
					#else
						CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(GetKyungGongGrade());
						DWORD ElapsedTime = gCurTime - m_ObjectState.State_Start_Time;
						if(ElapsedTime > pKGInfo->GetChangeTime())
						{
							MOVEMGR->ToggleHeroKyungGongMode();
						}
					#endif		// TAWAN_LOCAL
				}
			}
		}
	}

	// ½ºÅ³ ÄðÅ¸ÀÓ
	if(m_bSkillCooling)
	{
		if(gCurTime - m_SkillCoolTimeStart > m_SkillCoolTimeDuration)
			m_bSkillCooling = FALSE;
	}

	VECTOR3 pos;
	float angle;
	GetPosition(&pos);
	angle = GetAngle();

	// ±×¸²ÀÚ¿ë ¶óÀÌÆ® ÀÌµ¿À» À§ÇØ	
	MAP->SetShadowPivotPos(&pos);

	// 3D »ç¿îµåÀÇ Listener ¼³Á¤À» À§ÇØ
	AUDIOMGR->SetListenerPosition(&pos,angle);
	
	// Á×¾úÀ»¶§ ÀÏÁ¤ÇÑ ½Ã°£ÀÌ Áö³ª¸é Ã¢ ¶ç¿î´Ù.
//	if(GetState() == eObjectState_Die)
//	{	
//		if( gCurTime - GetStateStartTime() > PLAYERREVIVE_TIME ) //ÀÏÁ¤ ½Ã°£¸¸ µé¾î°¡°Ô
//		{
			//if( ) ºñ¹«·Î Á×¾ú´Ù. ¾Æ´Ï´Ù.
			//È®ÀÎ¿ä¸Á! confirm						//process°¡ ´À·È´Ù¸é NONEÀÏ ¼öµµ ÀÖ´Ù.
//			if( BATTLESYSTEM->GetBattle(this)->GetBattleKind() == eBATTLE_KIND_NONE )
//			{
//				WANTEDMGR->SetActiveDialog();
//			}
//		}
//	}

	if(GetState() == eObjectState_SkillUsing)
	{
		if(gCurTime - GetStateStartTime() > 10000)	//¼ýÀÚ¸¦ ÁÙÀÌ±â¿£ Áø¹ýºÎºÐÀÌ °É¸°´Ù.
		{
			OBJECTSTATEMGR->EndObjectState(this,eObjectState_SkillUsing);
			//////////////////////////////////////////////////////////////////////////
			// 06. 04. ½ºÅ³ ¹ö±× ¼öÁ¤ - ÀÌ¿µÁØ
			// ½ºÅ³ »ç¿ë »óÅÂ¸¦ °­Á¦·Î Á¾·áÇÒ¶§ ¸Þ¼¼Áö¸¦ ¸ø¹Þ¾Æ
			// »ý¼ºÇÏÁö ¸øÇÑ ÀÓ½Ã½ºÅ³°´Ã¼¸¦ Áö¿öÁØ´Ù.
			CSkillObject* pSObj = SKILLMGR->GetSkillObject(TEMP_SKILLOBJECT_ID);
			if(pSObj)
				SKILLMGR->DeleteTempSkill();
			//////////////////////////////////////////////////////////////////////////
		}
	}

	gHeroPos = pos;
	
	// Play Time ±â·Ï	
	m_HeroTotalInfo.Playtime  += gTickTime/1000;
	
	//SW050810 ÆòÈ­¸ðµå ÀÚµ¿ º¯È¯
	PEACEWARMGR->DoCheckForPeaceMode(this);

	//SW051129 Pet
	PETMGR->PetMgrProcess();

}

void CHero::GetHeroTotalInfo(HERO_TOTALINFO * info)
{
	ASSERT(info);
	memcpy(info, &m_HeroTotalInfo, sizeof(HERO_TOTALINFO));
}


void CHero::ApplyInterface()
{
#ifndef _TESTCLIENT_
	SetMoney(m_HeroTotalInfo.Money);
	SetLevel(m_CharacterInfo.Level);
	SetMaxLife(m_CharacterInfo.MaxLife);
	SetLife(m_CharacterInfo.Life, 0);
	SetMaxNaeRyuk(m_HeroTotalInfo.wMaxNaeRyuk);
	SetNaeRyuk(m_HeroTotalInfo.wNaeRyuk, 0);
	SetMaxShield(m_CharacterInfo.MaxShield);
	SetShield(m_CharacterInfo.Shield, 0);
	SetMaxExpPoint(m_maxExpPoint);

	SetExpPoint(m_HeroTotalInfo.ExpPoint);
	
	SetGenGol(m_HeroTotalInfo.wGenGol);
	SetMinChub(m_HeroTotalInfo.wMinChub);
	SetCheRyuk(m_HeroTotalInfo.wCheRyuk);
	SetSimMek(m_HeroTotalInfo.wSimMek);
	
	SetPartyIdx(m_HeroTotalInfo.PartyID);
	SetMunpaMemberRank(m_CharacterInfo.PositionInMunpa);
	SetMunpaID(m_CharacterInfo.MunpaID);
	SetGuildName(m_CharacterInfo.GuildName);

	SetGuageName(m_BaseObjectInfo.ObjectName);
	SetMunpa();
	SetBadFame(m_CharacterInfo.BadFame);

	SetStage(m_CharacterInfo.Stage);

	// RaMa
	GAMEIN->GetCharacterDialog()->RefreshPointInfo();//RefreshLevelupPoint();
	GAMEIN->GetSkPointDlg()->RefreshAbilityPoint();
#endif
}

void CHero::SetGuageName(char * szName)
{
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAME);
	if( ps )	ps->SetStaticText( szName );
}

void CHero::SetMaxLife(DWORD maxlife)
{
	CPlayer::SetMaxLife(maxlife);

	DWORD newMaxLife = GetMaxLife();
	if(newMaxLife == 0)
	{
		newMaxLife = 1;
	}
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFE))->SetValue(
		(float)m_CharacterInfo.Life/(float)newMaxLife, 0 );

	// ÀÎÅÍÆäÀÌ½º °ÔÀÌÁö º¯°æ
	GAMEIN->GetCharacterDialog()->SetLife(m_CharacterInfo.Life);

	// ÀÎÅÍÆäÀÌ½º : ¼öÄ¡ Ç¥½Ã
	char szValue[50];
	sprintf( szValue, "%d / %d", GetLife(), newMaxLife );

	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFETEXT);
	if( ps )	ps->SetStaticText( szValue );	
}


void CHero::SetMaxShield(DWORD maxShield)
{
	CPlayer::SetMaxShield(maxShield);

	DWORD newMaxShield = GetMaxShield();
	if(newMaxShield == 0)
	{
		newMaxShield = 1;
	}
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGESHIELD))->SetValue(
		(float)m_CharacterInfo.Shield/(float)newMaxShield, 0 );

	// ÀÎÅÍÆäÀÌ½º °ÔÀÌÁö º¯°æ
	GAMEIN->GetCharacterDialog()->SetShield(m_CharacterInfo.Shield);
	// ÀÎÅÍÆäÀÌ½º : ¼öÄ¡ Ç¥½Ã
	char szValue[50];
	sprintf( szValue, "%d / %d", GetShield(), newMaxShield );

	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGESHIELDTEXT);
	if( ps )	ps->SetStaticText( szValue );
}
void CHero::Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal)
{
	ChangeLife(HealVal);
}
void CHero::ChangeLife(int changeval)
{
	DWORD newlife = GetLife() + changeval;

	SetLife(newlife);
	
	if( m_bUngiFlag == FALSE )
	if( newlife )
	if( (GetMaxLife()/newlife) > 2 )
	{
		GAMEEVENTMGR->AddEvent(eGameEvent_Ability, 401);
		m_bUngiFlag = TRUE;
	}
}
void CHero::ChangeShield(int changeval)
{
	if(changeval < 0)
	{
		ASSERT((int)GetShield() >= (-changeval));
	}
	
	DWORD newShield = GetShield() + changeval;
	DWORD maxShield = GetMaxShield();

	SetShield(newShield);
}
void CHero::SetLife(DWORD life, BYTE type)
{
	CPlayer::SetLife(life);

	// ÀÎÅÍÆäÀÌ½º °ÔÀÌÁö º¯°æ
	DWORD MaxLife = GetMaxLife();
	if(MaxLife == 0)
	{
		MaxLife = 1;
	}
	
	
	if(GetState() == eObjectState_Die)
		life = 0;

	//SW070127 Å¸ÀÌÅº
	// Å¸ÀÌÅº Å¾½Â»óÅÂ¿¡¼­ Ã¼·Â 30% ¹Ì¸¸½Ã °æ°í
	TITANMGR->CheckNotifyMsg(TNM_MASTER_LIFE);

	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFE))->SetValue(
		(float)life/(float)MaxLife, 
		(type == 0 ? 0 : (2000/MaxLife)*life) );
	GAMEIN->GetCharacterDialog()->SetLife(life);
	// ÀÎÅÍÆäÀÌ½º : ¼öÄ¡ Ç¥½Ã
	char szValue[50];
	sprintf( szValue, "%d / %d", life, GetMaxLife() );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFETEXT);
	if( ps )	ps->SetStaticText( szValue );
}

void CHero::SetShield(DWORD Shield, BYTE type)
{
	CPlayer::SetShield(Shield);

	// ÀÎÅÍÆäÀÌ½º °ÔÀÌÁö º¯°æ
	DWORD MaxShield = GetMaxShield();
	if(MaxShield == 0)
	{
		MaxShield = 1;
	}

	
	if(GetState() == eObjectState_Die)
		Shield = 0;

	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGESHIELD))->SetValue(
		(float)Shield/(float)MaxShield, 
		(type == 0 ? 0 : (2000/MaxShield)*Shield) );
	GAMEIN->GetCharacterDialog()->SetShield(Shield);
	// ÀÎÅÍÆäÀÌ½º : ¼öÄ¡ Ç¥½Ã
	char szValue[50];
	sprintf( szValue, "%d / %d", Shield, GetMaxShield() );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGESHIELDTEXT);
	if( ps )	ps->SetStaticText( szValue );
}

void CHero::SetMoney(MONEYTYPE Money)
{
	m_HeroTotalInfo.Money = Money;
	if(GAMEIN->GetInventoryDialog())
		GAMEIN->GetInventoryDialog()->SetMoney(Money);
}

void CHero::SetKyungGongGrade(WORD grade)
{	
	m_HeroTotalInfo.KyungGongGrade = grade;
	
//	if(grade != 0)
//		OPTIONMGR->SetKyungGongIdx(HERO->GetKyungGongGrade());//***
	GAMEIN->GetCharacterDialog()->UpdateData();
}

void CHero::SetMaxExpPoint(EXPTYPE dwPoint)
{
	m_maxExpPoint = dwPoint;

	//Èç¹û·¢ÏÖ³ý0´íÎó,ÒÔ¼°µ±µÈ¼¶>=100,Éý¼¶¾­ÑéÎª40ÒÚ
	if (m_maxExpPoint == 0 || GetLevel()>= 100)
	{
		m_maxExpPoint = 4000000000;
	}

	double value = (double)GetExpPoint()/(double)m_maxExpPoint;

	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINT))->SetValue( (GUAGEVAL)value, 0 );
	GAMEIN->GetCharacterDialog()->SetExpPointPercent( (float)(value*100) );
	
	// ÀÎÅÍÆäÀÌ½º : ¼öÄ¡ Ç¥½Ã
	char szValue[50];
	sprintf( szValue, "%I64d / %I64d", GetExpPoint(), dwPoint );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINTTEXT);
	if( ps )	ps->SetStaticText( szValue );
}

void CHero::SetExpPoint(EXPTYPE dwPoint, BYTE type)
{
	m_HeroTotalInfo.ExpPoint = dwPoint;

	// ÀÎÅÍÆäÀÌ½º °ÔÀÌÁö º¯°æ
	double value = (double)dwPoint/(double)m_maxExpPoint;
	// magi82 //////////////////////////////////////////////////////////////////////////
	double value2 = value / 0.2f;
	double value3 = value2 - (EXPTYPE)value2;
	static double value4 = 0.0f;


	DWORD valueTime = (type == 0)? 0 : (DWORD)((2000/m_maxExpPoint)*dwPoint);

	// °æÇèÄ¡°¡ ½×ÀÏ¶§¸¶´Ù ´©Àû¹Ù ÃÊ±âÈ­
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT1))->SetActive(FALSE);
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT2))->SetActive(FALSE);
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT3))->SetActive(FALSE);
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT4))->SetActive(FALSE);

	if(GetLevel() == 99 || type == 0 )
		((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINT))->SetValue( 1, 0 );
	else
	{
		ySWITCH((int)value2)
		yCASE(1)
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT1))->SetActive(TRUE);
		yCASE(2)
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT1))->SetActive(TRUE);
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT2))->SetActive(TRUE);
		yCASE(3)
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT1))->SetActive(TRUE);
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT2))->SetActive(TRUE);
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT3))->SetActive(TRUE);
		yCASE(4)
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT1))->SetActive(TRUE);
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT2))->SetActive(TRUE);
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT3))->SetActive(TRUE);
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(EXP_POINT4))->SetActive(TRUE);
		yENDSWITCH

		if((EXPTYPE)value4 == (EXPTYPE)value2)
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINT))->SetValue( (GUAGEVAL)value3, valueTime );
		else
		{
			((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINT))->SetValue( (GUAGEVAL)value3, 0 );
			value4 = value2;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////


	// ÀÎÅÍÆäÀÌ½º : Ä³¸¯ÅÍ Ã¢
	CObjectGuagen* ps = (CObjectGuagen *)GAMEIN->GetCharacterDialog()->GetWindowForID(CI_EXPPOINTGUAGE);
	if( ps )
	{
		if(type == 0)
			ps->SetValue( (GUAGEVAL)value, 0 );
		else
			ps->SetValue( (GUAGEVAL)value, valueTime );
	}	

	GAMEIN->GetCharacterDialog()->SetExpPointPercent((float)(value*100));
	// ÀÎÅÍÆäÀÌ½º : ¼öÄ¡ Ç¥½Ã

	char szValue[50];
	if(GetLevel() >= 121)//µ±µÈ¼¶>=99Ê±,Ð´Íê×Ö,ÏÖÔÚ¸ÄÎª121¼¶
		sprintf( szValue, "[ %s ]", CHATMGR->GetChatMsg(179) );	//"[ èÇ ]"
	else
		sprintf( szValue, "%.2f%% / %.2f%%", value3*100, value*100 );	// magi82

#ifdef _CHEATENABLE_	
	if(CHEATMGR->IsCheatEnable())
	{
		char buf1[66];
		wsprintf(buf1, " %I64d/%I64d", dwPoint, GetMaxExpPoint());
		strcat(szValue, buf1);
	}
#endif

	cStatic* ps2 = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGEEXPPOINTTEXT);
	if( ps2 )	ps2->SetStaticText( szValue );
}

void CHero::SetLevel(LEVELTYPE level)
{
	// ÀÎÅÍÆäÀÌ½º Ã¢ Ç¥½Ã ¼öÄ¡ º¯°æ
	GAMEIN->GetCharacterDialog()->SetLevel(level);
	
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELEVEL);
	if( ps )	ps->SetStaticValue( level );

	// ·¹º§¾÷½Ã ÀÌÆåÆ®Ç¥½Ã
	if(level > CPlayer::GetLevel())
	{
		// magi82(4) - Titan(071023) Å¸ÀÌÅºÀÏ¶§ ÀÌÆåÆ® Ã³¸®(·¹º§¾÷)
		if(InTitan() == TRUE)
			EFFECTMGR->StartEffectProcess(eEffect_TitanLevelUpSentence,HERO,NULL,0,0);
		else
			EFFECTMGR->StartEffectProcess(eEffect_LevelUpSentence,HERO,NULL,0,0);
	}

	CPlayer::SetLevel(level);

	//»ö º¯È¯
	ITEMMGR->RefreshAllItem();
	MUGONGMGR->RefreshMugong();
	if( GAMEIN->GetQuickDialog() )
		GAMEIN->GetQuickDialog()->RefreshIcon();

}

void CHero::SetMaxNaeRyuk(DWORD val)
{
	m_HeroTotalInfo.wMaxNaeRyuk = val;
	
	// ÀÎÅÍÆäÀÌ½º °ÔÀÌÁö º¯°æ
	DWORD newMaxNaeRyuk = GetMaxNaeRyuk();
	if(newMaxNaeRyuk == 0)
	{
		newMaxNaeRyuk = 1;
	}
	((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAERYUK))->SetValue(
		(float)m_HeroTotalInfo.wNaeRyuk/(float)newMaxNaeRyuk, 0 );

	GAMEIN->GetCharacterDialog()->SetNaeRyuk(m_HeroTotalInfo.wNaeRyuk);

	// ÀÎÅÍÆäÀÌ½º : ¼öÄ¡ Ç¥½Ã
	char szValue[50];
	sprintf( szValue, "%d / %d", GetNaeRyuk(), newMaxNaeRyuk );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAERYUKTEXT);
	if( ps )	ps->SetStaticText( szValue );
}

void CHero::SetNaeRyuk(DWORD val, BYTE type)
{
	if(val > GetMaxNaeRyuk()) 
	{
//		ASSERT(0);
		val = GetNaeRyuk();
	}
	
	// ÀÎÅÍÆäÀÌ½º Ã¢ Ç¥½Ã ¼öÄ¡ º¯°æ
	DWORD dwMaxNaeRyuk = GetMaxNaeRyuk();
//	if( dwMaxNaeRyuk == 0 )
//		dwMaxNaeRyuk = 1;

	if(m_HeroTotalInfo.wMaxNaeRyuk != 0)
		((CObjectGuagen*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAERYUK))->SetValue(
		(float)val/(float)dwMaxNaeRyuk, 
		( type == 0 ? 0 : (1000/dwMaxNaeRyuk)*val) );

	GAMEIN->GetCharacterDialog()->SetNaeRyuk(val);

	// ÀÎÅÍÆäÀÌ½º : ¼öÄ¡ Ç¥½Ã
	char szValue[50];
	sprintf( szValue, "%d / %d", val, dwMaxNaeRyuk );
	cStatic* ps = (cStatic *)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAERYUKTEXT);
	if( ps )	ps->SetStaticText( szValue );
	
	// ¼öÄ¡º¯°æ
	m_HeroTotalInfo.wNaeRyuk = val;
}
void CHero::SetGenGol(WORD val)
{
	m_HeroTotalInfo.wGenGol = val;

	//SW060906 ÆêÀÛ¾÷À¸·Î º¯°æ
	STATSMGR->CalcItemStats(this);
	//STATSMGR->CalcCharStats(this);
	
	// ÀÎÅÍÆäÀÌ½º Ã¢ Ç¥½Ã ¼öÄ¡ º¯°æ
	GAMEIN->GetCharacterDialog()->SetGenGol();
	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
}
void CHero::SetMinChub(WORD val)
{
	m_HeroTotalInfo.wMinChub = val;

	STATSMGR->CalcItemStats(this);
	//STATSMGR->CalcCharStats(this);
	

	// ÀÎÅÍÆäÀÌ½º Ã¢ Ç¥½Ã ¼öÄ¡ º¯°æ
	GAMEIN->GetCharacterDialog()->SetMinChub();
	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
}
void CHero::SetCheRyuk(WORD val)
{
	m_HeroTotalInfo.wCheRyuk = val;

	STATSMGR->CalcItemStats(this);
	//STATSMGR->CalcCharStats(this);
	
	// ÀÎÅÍÆäÀÌ½º Ã¢ Ç¥½Ã ¼öÄ¡ º¯°æ
	GAMEIN->GetCharacterDialog()->SetCheRyuk();
	GAMEIN->GetCharacterDialog()->SetDefenseRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
}
void CHero::SetSimMek(WORD val)
{
	m_HeroTotalInfo.wSimMek = val;

	STATSMGR->CalcItemStats(this);
	//STATSMGR->CalcCharStats(this);
	
	// ÀÎÅÍÆäÀÌ½º Ã¢ Ç¥½Ã ¼öÄ¡ º¯°æ
	GAMEIN->GetCharacterDialog()->SetSimMek();
	GAMEIN->GetCharacterDialog()->UpdateData();
	GAMEIN->GetCharacterDialog()->SetShield(GetShield());
	GAMEIN->GetCharacterDialog()->SetNaeRyuk(GetNaeRyuk());
}
void CHero::SetMunpa()
{
	GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
}

void CHero::SetFame(FAMETYPE val)
{
	m_HeroTotalInfo.Fame = val;
	GAMEIN->GetCharacterDialog()->SetFame(GetFame());
}

void CHero::SetBadFame( FAMETYPE val )
{
	m_CharacterInfo.BadFame = val;
	GAMEIN->GetCharacterDialog()->SetBadFame( val );	
}

BOOL CHero::StartSocietyAct( int nStartMotion, int nIngMotion, int nEndMotion, BOOL bHideWeapon )
{
	if( GetState() != eObjectState_None && GetState() != eObjectState_Immortal )
	{
		if( GetState() == eObjectState_Society )
		{
			if( m_ObjectState.State_End_Motion != -1 )	//¾É±â µ¿ÀÛÀÇ °æ¿ì
			{
				if( m_ObjectState.bEndState == TRUE )
					return FALSE;
				if( gCurTime - GetStateStartTime() < GetEngineObject()->GetAnimationTime(m_ObjectState.State_Start_Motion ) )
					return FALSE;

				OBJECTSTATEMGR->EndObjectState( this, GetState(), GetEngineObject()->GetAnimationTime( m_ObjectState.State_End_Motion ) );
				return TRUE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	m_ObjectState.State_Start_Motion	= (short int)nStartMotion;
	m_ObjectState.State_Ing_Motion		= (short int)nIngMotion;
	m_ObjectState.State_End_Motion		= (short int)nEndMotion;

	if( nEndMotion != -1 )
		m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(nEndMotion);
	else
		m_ObjectState.State_End_MotionTime = 0;


	if(m_ObjectState.State_Start_Motion != -1)
	{
		ChangeMotion( m_ObjectState.State_Start_Motion, FALSE );

		if( m_ObjectState.State_Ing_Motion != -1 )
			ChangeBaseMotion( m_ObjectState.State_Ing_Motion );
	}

	// magi82 - UniqueItem(071010) ±ÇÀÌ ¾Æ´Ò¶§¸¸ ¹«±â¸¦ ¼û±ä´Ù.
	if( bHideWeapon && GetWeaponEquipType() != WP_GWUN)
		APPEARANCEMGR->HideWeapon( this );

	m_ObjectState.BeforeState		= m_BaseObjectInfo.ObjectState;
	m_BaseObjectInfo.ObjectState	= eObjectState_Society;
	m_ObjectState.State_Start_Time	= gCurTime;
	m_ObjectState.bEndState			= FALSE;
	
	if( m_ObjectState.State_Ing_Motion == -1 )
		OBJECTSTATEMGR->EndObjectState( this, eObjectState_Society, GetEngineObject()->GetAnimationTime( m_ObjectState.State_Start_Motion ) );

	return TRUE;
}

BOOL CHero::EndSocietyAct()
{
	if( m_ObjectState.State_Ing_Motion != -1 )
	{
		return FALSE;
	}
	return TRUE;
}

void CHero::StartSocietyActForQuest( int nStartMotion, int nIngMotion /* = -1 */, int nEndMotion /* = -1 */, BOOL bHideWeapon /* = FALSE  */ )
{
	m_ObjectState.State_Start_Motion = (short int)nStartMotion;
	m_ObjectState.State_Ing_Motion = (short int)nIngMotion;
	m_ObjectState.State_End_Motion = (short int)nEndMotion;

	if( nEndMotion != -1 )
		m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(nEndMotion);
	else
		m_ObjectState.State_End_MotionTime = 0;

	if( m_ObjectState.State_Start_Motion != -1 )
	{
		ChangeMotion( m_ObjectState.State_Start_Motion, FALSE );
		if( m_ObjectState.State_Ing_Motion != -1 )
			ChangeBaseMotion( m_ObjectState.State_Ing_Motion );
	}

	if( bHideWeapon )
		APPEARANCEMGR->HideWeapon( this );
}

void CHero::OnStartObjectState(BYTE State)
{
	CPlayer::OnStartObjectState(State);
}
void CHero::OnEndObjectState(BYTE State)
{
	switch(State)
	{
	case eObjectState_Deal:
		{
			HERO->HeroStateNotify(eObjectState_Deal);
		}
		break;
		
	case eObjectState_SkillUsing:
	case eObjectState_SkillBinding:
		SKILLMGR->OnComboTurningPoint(this);
		break;
		
	case eObjectState_SkillSyn:
		{
#ifndef _TESTCLIENT_
			if(SKILLMGR->GetSkillObject(TEMP_SKILLOBJECT_ID) != NULL)	// ¼­¹ö¿¡¼­ ÀÀ´äÀÌ ¾È¿Â°æ¿ì
				OBJECTSTATEMGR->StartObjectState(this,eObjectState_SkillUsing);
			else				
				SKILLMGR->OnComboTurningPoint(this);
#else
			SKILLMGR->OnComboTurningPoint(this);
#endif
		}
		break;
		
	case eObjectState_Move:
		{
			if(PARTYMGR->GetTacticObjectID())
			{
				TACTICMGR->HeroTacticJoin();
			}
		}
		break;
	case eObjectState_SkillStart:
		{
			if(m_SkillStartAction.GetActionKind() != eActionKind_Skill_RealExecute)
				break;
			
			m_SkillStartAction.ExcuteAction(this);
			m_SkillStartAction.Clear();
		}
		break;
	}
	CPlayer::OnEndObjectState(State);
}

//////////////////////////////////////////////////////////////////////////
// Çàµ¿ ÇÔ¼öµé..
void CHero::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive)
{
//	ASSERT(GetLife() == 0);
	
	WINDOWMGR->CloseAllMsgBox();

	GAMEIN->GetDealDialog()->SetDisable(FALSE);
	GAMEIN->GetDealDialog()->SetActive(FALSE);	// »óÁ¡Ã¢ ´Ý±â
	
	GAMEIN->GetNpcScriptDialog()->SetDisable(FALSE);	//NPCÃ¢µµ ´Ý±â
	GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);

	//°­È­ ºÐÇØ µî±Þ¾÷Ã¢ ´Ý±â -- ·çÆÃ´çÇÒ ¼ö µµ ÀÖÀ¸´Ï..

	if( GAMEIN->GetUpgradeDialog()->IsActive() &&
		!GAMEIN->GetUpgradeDialog()->IsDisable() )
	{
		GAMEIN->GetUpgradeDialog()->Release();
		GAMEIN->GetUpgradeDialog()->SetActiveRecursive( FALSE );
	}
	if( GAMEIN->GetMixDialog()->IsActive() &&
		!GAMEIN->GetMixDialog()->IsDisable() )
	{
		GAMEIN->GetMixDialog()->Release();
		GAMEIN->GetMixDialog()->SetActiveRecursive( FALSE );
	}
	if( GAMEIN->GetReinforceDialog()->IsActive() &&
		!GAMEIN->GetReinforceDialog()->IsDisable() )
	{
		GAMEIN->GetReinforceDialog()->Release();
		GAMEIN->GetReinforceDialog()->SetActiveRecursive( FALSE );
	}
	if( GAMEIN->GetDissolutionDialog()->IsActive() &&
		!GAMEIN->GetDissolutionDialog()->IsDisable() )
	{
		GAMEIN->GetDissolutionDialog()->SetActive( FALSE );
	}
	//
	if( GAMEIN->GetSkPointDlg()->IsActive() )
	{
		GAMEIN->GetSkPointDlg()->SetActive( FALSE );
	}

	SetMovingAction(NULL);
	DisableAutoAttack();					// ¿ÀÅä°ø°Ý ²ô±â
	SetLife(0);
	
// LBS 03.10.04 Á×¾úÀ»¶§ ³ëÁ¡»óÀ» ´Ý´Â´Ù.	//¿©±â¼­? Á×À¸¸é ¸Ê¼­¹ö¿¡¼­ Ãë¼ÒÇØÁÖ´Â °ÍÀÌ ÁÁÀ»ÅÙµ¥?
	STREETSTALLMGR->CloseStreetStall();
	
	if( pAttacker )
	if(pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
//		GAMEEVENTMGR->AddEvent(eGameEvent1_HeroDie);
		if( GetLevel() < 5 )
			GAMEEVENTMGR->AddEvent(eGameEvent_Die);
		else
			GAMEEVENTMGR->AddEvent(eGameEvent_DieAfter5);
	}

	if(GAMEIN->GetCharStateDialog())
		GAMEIN->GetCharStateDialog()->SetUngiMode( FALSE );

	DWORD time = 0;
	if( bFatalDamage )
		time = HERO->GetEngineObject()->GetAnimationTime(eMotion_Died_Fly);
	else
		time = HERO->GetEngineObject()->GetAnimationTime(eMotion_Died_Normal);
	CAMERA->SetCharState( eCS_Die, time );

	CPlayer::Die(pAttacker,bFatalDamage,bCritical,bDecisive);
	
	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->SetDefenseRate();
	GAMEIN->GetCharacterDialog()->UpdateData();

	// magi82 - SOS(070719)
	// magi82(36)
	// »ç¿ëÁßÀÎ ½ºÅ³ÀÌ °ø°ÝÈ¿°ú Å¸°Ù Å¸ÀÔ(NegativeResultTargetType)ÀÌ°Å³ª ¹æ¾îÈ¿°ú Å¸°Ù Å¸ÀÔ(PositiveResultTargetType)ÀÌ¸é
	// Å¸°Ù ¸®½ºÆ®¿¡ ÀÚ±â ÀÚ½Åµµ Ãß°¡°¡ µÇ´Âµ¥(°¢ È¿°ú¸¦ Àû¿ë ½ÃÅ°±â À§ÇØ¼­) ±× ¼ø°£ Á×À¸¸é
	// Attacker°¡ ÀÚ±â ÀÚ½ÅÀ¸·Î µÈ´Ù.
	// ±×·± °æ¿ì¸¦ ¹æÁöÇÏ±â À§ÇØ Attacker°¡ ÀÚ±â ÀÚ½ÅÀÎÁö Ã¼Å©ÇØ¾ßÇÑ´Ù.
	if( HEROID != pAttacker->GetID() )
	{
		if(pAttacker->GetObjectKind() == eObjectKind_Player)
		{
			CBattle* pTargetBattle = BATTLESYSTEM->GetBattle( HERO );
			CBattle* pAttackerBattle = BATTLESYSTEM->GetBattle( pAttacker->GetID() );
			WORD MapNum = MAP->GetMapNum();
			DWORD dwCheckBit = eBossMap | eEventMap | eSurvivalMap | eTournament;

			if(HERO->GetMunpaIdx())
			{
				if( HERO->IsPKMode() == TRUE
					|| (pTargetBattle->GetBattleKind() == eBATTLE_KIND_VIMUSTREET && pTargetBattle->GetBattleID() == pAttackerBattle->GetBattleID())
					|| (PARTYWAR->IsPartyWar() && PARTYWAR->IsEnemy((CPlayer*)pAttacker))
					|| GUILDFIELDWAR->IsEnemy((CPlayer*)pAttacker)
					|| MapNum == SIEGEMGR->GetSiegeWarMapNum()
					|| MAP->IsMapKind(dwCheckBit) )
					//|| MapNum == BOSSMONSTER_MAP
					//|| MapNum == BOSSMONSTER_2NDMAP || MapNum == RUNNINGMAP
					//|| MapNum == PKEVENTMAP || MAP->CheckMapKindIs(eSurvival)
					//|| EVENTMAP->IsEventMap() || MapNum == Tournament )
				{
					int a = 0;
				}
				else
				{
					WINDOWMGR->MsgBox( MBI_SOS, MBT_YESNO, CHATMGR->GetChatMsg(1633) );
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	SKILLMGR->RemoveTarget(this, SKILLRESULTKIND_POSITIVE|SKILLRESULTKIND_NEGATIVE);

	if( m_bActionPenalty == FALSE )
	{
		WINDOWMGR->MsgBox( MBI_ACTION_PENALTY, MBT_OK, CHATMGR->GetChatMsg(1606) );
		m_bActionPenalty = TRUE;
	}
}

void CHero::Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage)
{
	//////////////////////////////////////////////////////////////////////////
	// Ä«¸Þ¶ó µÚ·Î ¹Ð¸²
	VECTOR3 push;
	MATRIX4 my;
	SetVector3(&push,0,0,30);
	SetRotationYMatrix(&my,-m_RotateInfo.Angle.ToRad());
	TransformVector3_VPTR2(&push,&push,&my,1);
	CAMERA->SetPushedVibration(&push,200);
	//////////////////////////////////////////////////////////////////////////
	
	CPlayer::Damage(pAttacker,DamageKind,Damage,ShieldDamage,bCritical,bDecisive,titanObserbDamage);
	
}

void CHero::OnHitTarget(CObject* pMainTarget)
{
	//////////////////////////////////////////////////////////////////////////
	// ÁÖÀÎ°ø°ú µ¥¹ÌÁö ÀÔ´Â ¸ÞÀÎÅ¸°Ù°ú °¡±î¿ì¸é Á¶±Ý¾¿ ¶³¾îÆ®¸²	
	VECTOR3 targetpos = pMainTarget->GetCurPosition();
	VECTOR3 Curpos = HERO->GetCurPosition();

	float WeaponMinDistance;
	switch(GetWeaponEquipType())
	{
	case WP_GUM:	WeaponMinDistance = 150;	break;
	case WP_GWUN:	WeaponMinDistance = 100;	break;
	case WP_DO:		WeaponMinDistance = 150;	break;
	case WP_CHANG:	WeaponMinDistance = 150;	break;
	case WP_GUNG:	WeaponMinDistance = 100;	break;
	case WP_AMGI:	WeaponMinDistance = 100;	break;	
	default: ASSERT(0);
	}

	if(CalcDistanceXZ(&targetpos,&Curpos) < WeaponMinDistance)
	{
		VECTOR3 pos;	pos.x = 0;	pos.y = 0;	pos.z = 10.f;
		float angle = HERO->GetAngle();
		TransToRelatedCoordinate(&pos,&pos,angle);
		pos = pos + Curpos;
		MOVEMGR->HeroEffectMove(HERO,&pos,0,0);
	}
}


void CHero::ClearTarget(CObject* pObject)
{
	if(m_AutoAttackTarget.GetTargetID() == pObject->GetID())
		DisableAutoAttack();
	if(m_NextAction.GetTargetID() == pObject->GetID())
	{
		m_NextAction.Clear();
	}
}


void CHero::Revive(VECTOR3* pRevivePos)
{
	DisableAutoAttack();

	CAMERA->SetCharState( eCS_Normal, 0 );	//ÀÏ¾î³ª´Â µ¿ÀÛÀÌ ¾ø´Ù.
	
	CPlayer::Revive(pRevivePos);
}

BOOL CHero::GetKyungGongMode()
{
	return m_bIsKyungGongMode;
}

void CHero::SetKyungGongMode(BOOL bMode)
{
	m_bIsKyungGongMode = bMode;

	if( m_bIsKyungGongMode == FALSE )
	{
		if(MOVEMGR->IsMoving(this) && m_MoveInfo.KyungGongIdx != 0)
			MOVEMGR->ToggleHeroKyungGongMode();
	}
	else
	{
		if(MOVEMGR->IsMoving(this) && m_MoveInfo.KyungGongIdx != 0)
			MOVEMGR->ToggleHeroKyungGongMode();
	}

//KES CHARSTATEDLG 031016
	if(GAMEIN->GetCharStateDialog())
		GAMEIN->GetCharStateDialog()->SetKyungGongMode( m_bIsKyungGongMode );
//
	
//	OPTIONMGR->SetKyungGongMode(m_bIsKyungGongMode);//***
}


int CHero::GetMugongLevel(WORD MugongIdx)
{
	//if(MugongIdx < 100)		// ÄÞº¸
	//	return 1;

	// !!!!!!!!!!! magi82 - ¿ø·¡´Â Å¸ÀÌÅº ÄÞº¸¸¦ µû·Î ¸¸µé¾î¾ßÇÏÁö¸¸ Áö±ÝÀº ±ÞÇØ¼­ ÀÏ´Ü ÀÌ·¸°Ô ÀÓ½Ã·Î ¾¸ !!!!!!!!!!!!!!!!!!1
	if(MugongIdx < 100 || ( 10000 <= MugongIdx && MugongIdx < 10000 + 100 ))		// ÄÞº¸
		return 1;

	CMugongBase* pMugong = MUGONGMGR->GetMugongByMugongIdx(MugongIdx);
	if(pMugong == NULL)
		return 0;

	return pMugong->GetSung();
}

//////////////////////////////////////////////////////////////////////////
// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
// ¹«°ø º¯È¯ Ãß°¡
WORD CHero::GetSkillOptionIndex(WORD MugongIdx)
{
	if(MugongIdx < 100)		// ÄÞº¸
		return 0;

	//if( MugongIdx > SKILLNUM_TO_TITAN && InTitan() )
	//	MugongIdx -= SKILLNUM_TO_TITAN;

	CMugongBase* pMugong = MUGONGMGR->GetMugongByMugongIdx(MugongIdx);
	if(pMugong == NULL)
		return 0;

	return pMugong->GetOption();
}
//////////////////////////////////////////////////////////////////////////

void CHero::AddStatus(CStatus* pStatus)
{
	CObject::AddStatus(pStatus);
	
	MOVEMGR->RefreshHeroMove();

	SetMaxLife(m_CharacterInfo.MaxLife);
	SetMaxShield(m_CharacterInfo.MaxShield);
	SetMaxNaeRyuk(m_HeroTotalInfo.wMaxNaeRyuk);
	
	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->SetDefenseRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
}
void CHero::RemoveStatus(CStatus* pStatus)
{
	CObject::RemoveStatus(pStatus);

	MOVEMGR->RefreshHeroMove();
/*

	((cGuage*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAERYUK))->SetMaxValue(GetMaxNaeRyuk());
	GAMEIN->GetCharacterDialog()->SetNaeRyuk(m_HeroTotalInfo.wNaeRyuk);
	((cGuage*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFE))->SetMaxValue(GetMaxLife());
	GAMEIN->GetCharacterDialog()->SetLife(m_CharacterInfo.Life);
*/

	GAMEIN->GetCharacterDialog()->SetAttackRate();
	GAMEIN->GetCharacterDialog()->SetDefenseRate();
	GAMEIN->GetCharacterDialog()->UpdateData();
}

DWORD CHero::DoGetPhyAttackPowerMin()
{
	const player_calc_stats * pChar_stats = GetCharStats();
	const player_calc_stats * pItem_stats = GetItemStats();
	const SET_ITEM_OPTION* pSetItem_Stats = GetSetItemStats(); //2007. 6. 13. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ Ãß°¡

	WORD WeaponKind = GetWeaponEquipType();
	if(WeaponKind == WP_GUNG || WeaponKind == WP_AMGI)
	{
		WORD MinChub = (WORD)(pChar_stats->MinChub + GetAbilityStats()->StatMin + pItem_stats->MinChub + GetShopItemStats()->Minchub + GetAvatarOption()->Minchub + pSetItem_Stats->wMinChub);

#ifdef _JAPAN_LOCAL_
		
		WORD RareWeapon = pItem_stats->RangeAttackPowerMin;

		if( PARTYMGR->IsHelpPartyMember() && BATTLESYSTEM->GetBattle(this)->GetBattleKind() != eBATTLE_KIND_VIMUSTREET )
			RareWeapon = RareWeapon + RareWeapon * 20 / 100;

		WORD WeaponAttack = RareWeapon
						  * ( 1.f + pItem_stats->RangeAttackPowerAddRateMin )
						  + GetAvatarOption()->Attack;

		DWORD dwPhyAttackPowerMin = CalcRangeAttackPower( MinChub, WeaponAttack )
								  + GetLevel() * 4
								  + GetAbilityStats()->GetPhyAttackUp( WeaponKind );

		return dwPhyAttackPowerMin;
#else
		return (DWORD)((CalcRangeAttackPower(MinChub,pItem_stats->RangeAttackPowerMin+GetAvatarOption()->Attack+pSetItem_Stats->wRangeAttackMin)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->RangeAttackPowerAddRateMin));
#endif
	}
	else
	{
		WORD GenGol = (WORD)(pChar_stats->GenGol + GetAbilityStats()->StatGen + pItem_stats->GenGol + GetShopItemStats()->Gengol + GetAvatarOption()->Gengol + pSetItem_Stats->wGenGol);
		WORD WeaponItemIdx = GetWearedItemIdx(eWearedItem_Weapon);
		WORD DefaultPower = 0;
		if( WeaponItemIdx == 0 )
			DefaultPower = 5;
		
#ifdef _JAPAN_LOCAL_

		WORD RareWeapon = pItem_stats->MeleeAttackPowerMin + DefaultPower;

		if( PARTYMGR->IsHelpPartyMember() && BATTLESYSTEM->GetBattle(this)->GetBattleKind() != eBATTLE_KIND_VIMUSTREET )
			RareWeapon = RareWeapon + RareWeapon * 20 / 100;

		WORD WeaponAttack = RareWeapon
							* ( 1.f + pItem_stats->MeleeAttackPowerAddRateMin )
							+ GetAvatarOption()->Attack;

		DWORD dwPhyAttackPowerMin = CalcMeleeAttackPower( GenGol, WeaponAttack )
								  + GetLevel() * 4
								  + GetAbilityStats()->GetPhyAttackUp( WeaponKind );

		return dwPhyAttackPowerMin;
#else
		return (DWORD)((CalcMeleeAttackPower(GenGol,pItem_stats->MeleeAttackPowerMin+DefaultPower+GetAvatarOption()->Attack+pSetItem_Stats->wMeleeAttackMin)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->MeleeAttackPowerAddRateMin));
#endif
	}
}
DWORD CHero::DoGetPhyAttackPowerMax()
{
	const player_calc_stats * pChar_stats = GetCharStats();
	const player_calc_stats * pItem_stats = GetItemStats();
	const SET_ITEM_OPTION* pSetItem_Stats = GetSetItemStats(); //2007. 6. 13. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ Ãß°¡

	WORD WeaponKind = GetWeaponEquipType();
	if(WeaponKind == WP_GUNG || WeaponKind == WP_AMGI)
	{
		WORD MinChub = (WORD)(pChar_stats->MinChub + GetAbilityStats()->StatMin + pItem_stats->MinChub + GetShopItemStats()->Minchub + GetAvatarOption()->Minchub + pSetItem_Stats->wMinChub);
#ifdef _JAPAN_LOCAL_

		WORD RareWeapon = pItem_stats->RangeAttackPowerMax;

		if( PARTYMGR->IsHelpPartyMember() && BATTLESYSTEM->GetBattle(this)->GetBattleKind() != eBATTLE_KIND_VIMUSTREET )
			RareWeapon = RareWeapon + RareWeapon * 20 / 100;

		WORD WeaponAttack = RareWeapon
						  * ( 1.f + pItem_stats->RangeAttackPowerAddRateMax )
						  + GetAvatarOption()->Attack;

		DWORD dwPhyAttackPowerMax = CalcRangeAttackPower( MinChub, WeaponAttack )
								  + GetLevel() * 4
								  + GetAbilityStats()->GetPhyAttackUp( WeaponKind );

		return dwPhyAttackPowerMax;
#else
		return (DWORD)((CalcRangeAttackPower(MinChub,pItem_stats->RangeAttackPowerMax+GetAvatarOption()->Attack+pSetItem_Stats->wRangeAttackMax)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->RangeAttackPowerAddRateMax));
#endif
	}
	else
	{
		WORD GenGol = (WORD)(pChar_stats->GenGol + GetAbilityStats()->StatGen + pItem_stats->GenGol + GetShopItemStats()->Gengol + GetAvatarOption()->Gengol + pSetItem_Stats->wGenGol);

		WORD WeaponItemIdx = GetWearedItemIdx(eWearedItem_Weapon);
		WORD DefaultPower = 0;
		if( WeaponItemIdx == 0 )
			DefaultPower = 5;
#ifdef _JAPAN_LOCAL_

		WORD RareWeapon = pItem_stats->MeleeAttackPowerMax+DefaultPower;

		if( PARTYMGR->IsHelpPartyMember() && BATTLESYSTEM->GetBattle(this)->GetBattleKind() != eBATTLE_KIND_VIMUSTREET )
			RareWeapon = RareWeapon + RareWeapon * 20 / 100;

		WORD WeaponAttack = RareWeapon
							* ( 1.f + pItem_stats->MeleeAttackPowerAddRateMax )
							+ GetAvatarOption()->Attack;

		DWORD dwPhyAttackPowerMax = CalcMeleeAttackPower( GenGol, WeaponAttack )
								  + GetLevel() * 4
								  + GetAbilityStats()->GetPhyAttackUp( WeaponKind );

		return dwPhyAttackPowerMax;
#else		
		return (DWORD)((CalcMeleeAttackPower(GenGol,pItem_stats->MeleeAttackPowerMax+DefaultPower+GetAvatarOption()->Attack+pSetItem_Stats->wMeleeAttackMax)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->MeleeAttackPowerAddRateMax));
#endif
	}
}

/* Á¾·áÇØ¾ßÇÒ »óÅÂ°ªÀ» ¼­¹ö¿¡µµ ¾Ë¸°´Ù. */
void CHero::HeroStateNotify(BYTE EndState)
{
	MSG_BYTE msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_STATE_NOTIFY;
	msg.dwObjectID = GetID();
	msg.bData = EndState;

	NETWORK->Send(&msg, sizeof(msg));
}

BYTE CHero::GetNaeRyukPercent()
{
	return (BYTE)((GetNaeRyuk()*100) / GetMaxNaeRyuk());
}

DWORD CHero::DoGetMaxNaeRyuk()
{
	return m_HeroTotalInfo.wMaxNaeRyuk;
}

void CHero::ClearGuildInfo()
{
//	SafeStrCpy(m_HeroTotalInfo.MunpaName, "", MAX_GUILD_NAME+1);
	CPlayer::ClearGuildInfo();
}

char* CHero::GetGuildEntryDate()
{
	return m_HeroTotalInfo.MunpaCanEntryDate;
}

void CHero::SetGuildEntryDate(char* day)
{
	SafeStrCpy(m_HeroTotalInfo.MunpaCanEntryDate, day, 11);
}

void CHero::CalcShopItemOption( WORD wIdx, BOOL bAdd, DWORD Param )
{
	if( wIdx == 0 )
		return;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wIdx );
	if(!pItemInfo)
	{
		char buf[256]= { 0, };
		sprintf(buf, "S ItemInfo°¡ ¾ø´Ù. PlayerIdx[%d] Idx[%d]", GetID(), wIdx);
		ASSERTMSG(0, buf);
		return;
	}

	int calc = -1;
	if(bAdd)	calc=1;

	//////////////////////////////////////////////////////
	// 06. 07 ³»°ø ÀûÁß(ÀÏ°Ý) - ÀÌ¿µÁØ
	if( pItemInfo->NaeRyukRecover )
	{
		m_ShopItemOption.Decisive += (pItemInfo->NaeRyukRecover*calc);
		if(m_ShopItemOption.Decisive <= 0)
			m_ShopItemOption.Decisive=0;
	}
	//////////////////////////////////////////////////////

	if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
	{
		// °æÇèÄ¡   GenGol
		if( pItemInfo->GenGol>0 )
		{
			m_ShopItemOption.AddExp += (pItemInfo->GenGol*calc);
			if(m_ShopItemOption.AddExp <= 0)
				m_ShopItemOption.AddExp=0;
			
		}
		// ¾ÆÀÌÅÛ µå¶øÈ®·ü
		if( pItemInfo->MinChub>0 )
		{
			m_ShopItemOption.AddItemDrop += (pItemInfo->MinChub*calc);
			if(m_ShopItemOption.AddItemDrop <= 0)
				m_ShopItemOption.AddItemDrop=0;
		}
		// Á×¾úÀ»¶§ °æÇèÄ¡¿Í µ· ÆÐ³ÎÆ¼ ¾ø´Â°Í		
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
	{
		// Á¶ÇÕÈ®·ü
		/*if( pItemInfo->LimitJob )
		{
			m_ShopItemOption.ItemMixSuccess = (char)pItemInfo->LimitJob;
			if(m_ShopItemOption.ItemMixSuccess < 0)
				m_ShopItemOption.ItemMixSuccess=0;
		}		
		else */
		if( pItemInfo->GenGol )
		{
			if( bAdd )
			{
				m_ShopItemOption.StatePoint += pItemInfo->GenGol;
				GAMEIN->GetCharacterDialog()->RefreshPointInfo();
			}
		}
		else if( pItemInfo->Life )
		{
			m_ShopItemOption.SkillPoint += pItemInfo->Life;
			GAMEIN->GetSkPointDlg()->RefreshAbilityPoint();
			GAMEIN->GetSuryunDialog()->RedistBtnActive();			
			GAMEIN->GetSkPointNotifyDlg()->SetActive( TRUE );
		}
		else if( pItemInfo->CheRyuk )
		{
			if( bAdd )
				m_ShopItemOption.ProtectCount = (char)pItemInfo->CheRyuk;
			else
				m_ShopItemOption.ProtectCount = (char)Param;
		}
		else if( ( pItemInfo->ItemIdx == eIncantation_Tracking ||
				   pItemInfo->ItemIdx == eIncantation_Tracking7 ||
				   pItemInfo->ItemIdx == eIncantation_Tracking7_NoTrade ) && bAdd )
		{
			//if( MAP->GetMapNum() == RUNNINGMAP || MAP->GetMapNum() == PKEVENTMAP )
			if( MAP->IsMapSame(eRunningMap) || MAP->GetMapNum() == PKEVENTMAP )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1457) );
				return;
			}

			GAMEIN->GetChaseinputDlg()->SetActive( TRUE );
			GAMEIN->GetChaseinputDlg()->SetItemIdx( pItemInfo->ItemIdx );
		}
		else if( pItemInfo->ItemIdx == eIncantation_ChangeName && bAdd )
		{
/*			GAMEIN->GetNameChangeNotifyDlg()->SetActive( TRUE );
			GAMEIN->GetInventoryDialog()->SetDisable( TRUE );*/
		}
		else if( pItemInfo->LimitJob )			// ·¹º§Á¦ÇÑÇØÁ¦ ÁÖ¹®¼­
		{
			if( bAdd )
				m_ShopItemOption.EquipLevelFree += (BYTE)pItemInfo->LimitJob;
			else
			{
				m_ShopItemOption.EquipLevelFree -= (BYTE)pItemInfo->LimitJob;
			}
			STATSMGR->CalcItemStats( HERO );
			GAMEIN->GetCharacterDialog()->RefreshInfo();
			ITEMMGR->RefreshAllItem();
		}
		else if( pItemInfo->ItemIdx == eIncantation_MugongExtend && bAdd )
		{
#ifdef _JAPAN_LOCAL_
			 ++m_CharacterInfo.ExtraMugongSlot;
#elif defined _HK_LOCAL_
			 ++m_CharacterInfo.ExtraMugongSlot;
#elif defined _TL_LOCAL_
			 ++m_CharacterInfo.ExtraMugongSlot;
#else
			GAMEIN->GetMugongDialog()->SetMugongExtend( TRUE );
#endif
			if( GAMEIN->GetMugongDialog()->IsActive() )
				GAMEIN->GetMugongDialog()->SetActive( TRUE );
		}
		else if( pItemInfo->ItemIdx == eIncantation_MemoryMoveExtend || pItemInfo->ItemIdx == eIncantation_MemoryMoveExtend7 || pItemInfo->ItemIdx == eIncantation_MemoryMove2 ||
			     pItemInfo->ItemIdx == eIncantation_MemoryMoveExtend30 )
		{
			GAMEIN->GetMoveDialog()->SetExtend( bAdd );
		}
#ifdef _JAPAN_LOCAL_
		else if( pItemInfo->ItemIdx == eIncantation_InvenExtend )
		{
			if( bAdd )
			{
				m_CharacterInfo.ExtraInvenSlot += 1;
				if( GAMEIN->GetInventoryDialog()->IsActive() )
					GAMEIN->GetInventoryDialog()->SetActive( TRUE );
			}
		}
		else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend )
		{
			if( bAdd )
			{
				m_CharacterInfo.ExtraPyogukSlot += 1;
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1302) );
			}
		}
#endif
#ifdef _HK_LOCAL_
		else if( pItemInfo->ItemIdx == eIncantation_InvenExtend )
		{
			if( bAdd )
			{
				m_CharacterInfo.ExtraInvenSlot += 1;
				if( GAMEIN->GetInventoryDialog()->IsActive() )
					GAMEIN->GetInventoryDialog()->SetActive( TRUE );
			}
		}
		else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend )
		{
			if( bAdd )
			{
				m_CharacterInfo.ExtraPyogukSlot += 1;
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1302) );
			}
		}
#endif
#ifdef _TL_LOCAL_
		else if( pItemInfo->ItemIdx == eIncantation_InvenExtend )
		{
			if( bAdd )
			{
				m_CharacterInfo.ExtraInvenSlot += 1;
				if( GAMEIN->GetInventoryDialog()->IsActive() )
					GAMEIN->GetInventoryDialog()->SetActive( TRUE );
			}
		}
		else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend )
		{
			if( bAdd )
			{
				m_CharacterInfo.ExtraPyogukSlot += 1;
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1302) );
			}
		}
#endif
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
	{
		if( pItemInfo->GenGol>0 )
		{
			m_ShopItemOption.Gengol += (pItemInfo->GenGol*calc);
			if(m_ShopItemOption.Gengol <= 0)
				m_ShopItemOption.Gengol=0;

			STATSMGR->CalcItemStats(HERO);
			GAMEIN->GetCharacterDialog()->SetGenGol();
			GAMEIN->GetCharacterDialog()->SetAttackRate();
		}
		if( pItemInfo->MinChub>0 )
		{
			m_ShopItemOption.Minchub += (pItemInfo->MinChub*calc);
			if(m_ShopItemOption.Minchub <= 0)
				m_ShopItemOption.Minchub=0;

			STATSMGR->CalcItemStats(HERO);
			GAMEIN->GetCharacterDialog()->SetMinChub();
			GAMEIN->GetCharacterDialog()->SetAttackRate();
		}
		if( pItemInfo->CheRyuk>0 )
		{		
			m_ShopItemOption.Cheryuk += (pItemInfo->CheRyuk*calc);
			if(m_ShopItemOption.Cheryuk <= 0)
				m_ShopItemOption.Cheryuk=0;

			STATSMGR->CalcItemStats(this);

			GAMEIN->GetCharacterDialog()->SetCheRyuk();
			GAMEIN->GetCharacterDialog()->SetDefenseRate();
		}
		if( pItemInfo->SimMek>0 )
		{
			m_ShopItemOption.Simmek += (pItemInfo->SimMek*calc);
			if(m_ShopItemOption.Simmek <= 0)
				m_ShopItemOption.Simmek=0;

			STATSMGR->CalcItemStats(HERO);
			GAMEIN->GetCharacterDialog()->SetSimMek();
		}
		// ³»°øµ¥¹ÌÁö   Life
		if( pItemInfo->Life>0 )
		{
			m_ShopItemOption.NeagongDamage += (char)(pItemInfo->Life*calc);
			if(m_ShopItemOption.NeagongDamage <= 0)
				m_ShopItemOption.NeagongDamage=0;
		}
		// ¿Ü°ø´ë¹ÌÁö   Shield
		if( pItemInfo->Shield>0 )
		{
			m_ShopItemOption.WoigongDamage += (char)(pItemInfo->Shield*calc);
			if(m_ShopItemOption.WoigongDamage <= 0)
				m_ShopItemOption.WoigongDamage=0;
		}
		// ¹«°øÆ¯±âÄ¡ 1¾¿  Plus_MugongIdx
		if( pItemInfo->NaeRyuk>0 )
		{
			m_ShopItemOption.AddSung += (pItemInfo->NaeRyuk*calc);
			if( m_ShopItemOption.AddSung < 0 )
				m_ShopItemOption.AddSung = 0;
		}
		// ±âº»µ¥¹ÌÁö  Plus_Value
		if( pItemInfo->LimitJob>0 )
		{
			m_ShopItemOption.ComboDamage += (pItemInfo->LimitJob*calc);
			if(m_ShopItemOption.ComboDamage <= 0)
				m_ShopItemOption.ComboDamage=0;
		}
		// Å©¸®Æ¼ÄÃ Áõ°¡·®   CriticalPercent
		if( pItemInfo->LimitGender>0 )
		{
			m_ShopItemOption.Critical += (pItemInfo->LimitGender*calc);
			if(m_ShopItemOption.Critical <= 0)
				m_ShopItemOption.Critical=0;

			GAMEIN->GetCharacterDialog()->SetCritical();
		}
		// Å©¸®Æ¼ÄÃ½Ã ½ºÅÏÈ®·ü    RangeAttackMin
		if( pItemInfo->LimitLevel>0 )
		{
			m_ShopItemOption.StunByCri += (pItemInfo->LimitLevel*calc);
			if(m_ShopItemOption.StunByCri <= 0)
				m_ShopItemOption.StunByCri=0;
		}
		// ¹°¸®¹æ¾î·Â
		if( pItemInfo->LimitGenGol>0 )
		{
			m_ShopItemOption.RegistPhys += (pItemInfo->LimitGenGol*calc);
			if(m_ShopItemOption.RegistPhys < 0)
				m_ShopItemOption.RegistPhys = 0;
			
			GAMEIN->GetCharacterDialog()->SetDefenseRate();
		}
		// ¼Ó¼º¹æ¾î·Â
		if( pItemInfo->LimitMinChub>0 )
		{
			m_ShopItemOption.RegistAttr += (pItemInfo->LimitMinChub*calc);
			if(m_ShopItemOption.RegistAttr < 0)
				m_ShopItemOption.RegistAttr = 0;

			GAMEIN->GetCharacterDialog()->UpdateData();
		}
		// ³»·Â¼Ò¸ð°¨¼Ò
		if( pItemInfo->LimitCheRyuk>0 )
		{
			m_ShopItemOption.NeaRyukSpend += (pItemInfo->LimitCheRyuk*calc);
			if(m_ShopItemOption.NeaRyukSpend < 0 )
				m_ShopItemOption.NeaRyukSpend = 0;
		}
		// ÃÖ´ë»ý¸í·Â Áõ°¡Ä¡
		if( pItemInfo->Plus_MugongIdx )
		{
			m_ShopItemOption.Life += (pItemInfo->Plus_MugongIdx*calc);
			if( m_ShopItemOption.Life <= 0 )
				m_ShopItemOption.Life = 0;	
		}
		// ÃÖ´ëÈ£½Å°­±â Áõ°¡Ä¡
		if( pItemInfo->Plus_Value )
		{
			m_ShopItemOption.Shield += (pItemInfo->Plus_Value*calc);
			if( m_ShopItemOption.Shield <= 0 )
				m_ShopItemOption.Shield = 0;
		}
		// ÃÖ´ë³»·Â Áõ°¡Ä¡
		if( pItemInfo->AllPlus_Kind )
		{
			m_ShopItemOption.Naeryuk += (pItemInfo->AllPlus_Kind*calc);
			if( m_ShopItemOption.Naeryuk <= 0 )
				m_ShopItemOption.Naeryuk = 0;
		}
		// °æ°øµô·¹ÀÌ
		if( pItemInfo->RangeAttackMin )
		{
			m_ShopItemOption.bKyungGong += (pItemInfo->RangeAttackMin*calc);
		}
		// °æ°ø¼Óµµ »ó½Â
		if( pItemInfo->RangeAttackMax )
		{
			m_ShopItemOption.KyungGongSpeed += (pItemInfo->RangeAttackMax*calc);
		}
		// °­È­ ÁõÆø ¼öÄ¡
		if( pItemInfo->CriticalPercent )
		{
			m_ShopItemOption.ReinforceAmp += (pItemInfo->CriticalPercent*calc);

			STATSMGR->CalcItemStats(this);
			GAMEIN->GetCharacterDialog()->RefreshInfo();
		}
		// ¾ÆÀÌÅÛµå¶øÈ®·ü 
		if( pItemInfo->PhyDef )
		{
			m_ShopItemOption.AddItemDrop += (pItemInfo->PhyDef*calc);
		}
		// Å©¸®Æ¼ÄÃ Áõ°¡·®   CriticalPercent
		if( pItemInfo->NaeRyukRecover>0 )
		{
			m_ShopItemOption.Decisive += (pItemInfo->NaeRyukRecover*calc);
			if(m_ShopItemOption.Decisive <= 0)
				m_ShopItemOption.Decisive=0;

			GAMEIN->GetCharacterDialog()->SetCritical();
		}
		// ³ëÁ¡»ó ²Ù¹Ì±â(³ëÁ¡»ó²Ù¹Ì±âÀÌ¸é ¼¥¾ÆÀÌÅÛ ¿É¼Ç¿¡ ÇØ´ç ¾ÆÀÌÅÛ ÀÎµ¦½º¸¦ ³Ö´Â´Ù.
		if( (DWORD)(pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE)) > 0 )
		{
			MSG_DWORD2 msg;
			msg.Category = MP_ITEMEXT;
			msg.Protocol = MP_ITEMEXT_SHOPITEM_DECORATION_ON;
			msg.dwObjectID = HEROID;
			msg.dwData2 = STREETSTALLMGR->GetStallKind();
			
			if(bAdd == FALSE)
			{
				msg.dwData1 = 0;
				m_ShopItemOption.dwStreetStallDecoration = 0;
				OBJECTMGR->SetRemoveDecorationInfo(HERO);
			}
			else
			{
				msg.dwData1 = pItemInfo->ItemIdx;
				m_ShopItemOption.dwStreetStallDecoration = pItemInfo->ItemIdx;


				if( GetState() == eObjectState_StreetStall_Owner)
                    STREETSTALLMGR->AddStreetStallDecoration(HERO, STREETSTALLMGR->GetStallKind());
			}
			
			NETWORK->Send(&msg,sizeof(msg));
		}
		
/*		// ³»·ÂÈ¸º¹·ê 1.5¹è
		if( pItemInfo->NaeRyukRecoverRate>0 )
		{
			m_ShopItemOption.RecoverRate += (pItemInfo->NaeRyukRecoverRate*calc);
			if( m_ShopItemOption.RecoverRate<=0 )
				m_ShopItemOption.RecoverRate = 0;

		}*/
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
	{
		// ÃÖ´ë»ý¸í·Â Áõ°¡Ä¡
		if( pItemInfo->Life > 0 )
		{
			m_ShopItemOption.Life += (WORD)(pItemInfo->Life*calc);
			if( m_ShopItemOption.Life <= 0 )
				m_ShopItemOption.Life = 0;		
		}
		// ÃÖ´ëÈ£½Å°­±â Áõ°¡Ä¡
		if( pItemInfo->Shield > 0 )
		{
			m_ShopItemOption.Shield += (WORD)(pItemInfo->Shield*calc);
			if( m_ShopItemOption.Shield <= 0 )
				m_ShopItemOption.Shield = 0;
		}
		// ÃÖ´ë³»·Â Áõ°¡Ä¡
		if( pItemInfo->NaeRyuk > 0 )
		{
			m_ShopItemOption.Naeryuk += (pItemInfo->NaeRyuk*calc);
			if( m_ShopItemOption.Naeryuk <= 0 )
				m_ShopItemOption.Naeryuk = 0;
		}

		// ½ºÅÈ Àû¿ë
		// STATSMGR->CalcCharStats( pPlayer );
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
	{
//		APPEARANCEMGR->SetAvatarItem( HERO, pItemInfo->ItemIdx, bAdd );
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
	{
		/*
		if( pItemInfo->ItemIdx == eSundries_Shout && bAdd )
			GAMEIN->GetShoutDlg()->SetActive( TRUE );
		*/


		// ³ëÁ¡°³¼³
		if( pItemInfo->CheRyuk )
			m_ShopItemOption.bStreetStall += (pItemInfo->CheRyuk*calc);
	}	
}


void CHero::ActiveOptionInfoToInterface()
{
	if( m_ShopItemOption.Gengol )
		GAMEIN->GetCharacterDialog()->SetGenGol();
	if( m_ShopItemOption.Minchub )
		GAMEIN->GetCharacterDialog()->SetMinChub();
	if( m_ShopItemOption.Cheryuk )
		GAMEIN->GetCharacterDialog()->SetCheRyuk();
	if( m_ShopItemOption.Simmek )
		GAMEIN->GetCharacterDialog()->SetSimMek();
}


void CHero::CheckShopItemUseInfoToInventory( WORD ItemIdx, WORD ItemPos )
{
	CItem*	pItem = NULL;
	
	AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if(!pAvatarItem)	return;

	for(int i=0; i<eAvatar_Max; i++)
	{
		// ¾Æ¹ÙÅ¸ ¾ÆÀÌÅÛÀÌ ºüÁ®¾ß ÇÏ´Â°æ¿ìÀÌ°Å³ª
		// °°Àº ÀÚ¸®¿¡ ¾Æ¹ÙÅ¸ ¾ÆÀÌÅÛÀÌ ÀÖ´Â °æ¿ì¿£.
		if( (!pAvatarItem->Item[i] && m_ShopItemOption.Avatar[i]) ||
			(m_ShopItemOption.Avatar[i] && i==pAvatarItem->Position) )
		{
			for(int n=0; n<SLOT_SHOPINVEN_NUM/2; n++)
			{
				pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( n+TP_SHOPINVEN_START );
				if(!pItem)		continue;

				if( pItem->GetItemIdx()==m_ShopItemOption.Avatar[i] && pItem->GetUseParam() )
				{
					pItem->SetUseParam( 0 );
					break;
				}				
			}
		}
	}
}


void CHero::RefreshLevelupPoint()
{
	if( m_ShopItemOption.StatePoint==0 )		return;

	// Hard Coding
	WORD point = m_HeroTotalInfo.LevelUpPoint + (30 - m_ShopItemOption.StatePoint);	
	GAMEIN->GetCharacterDialog()->RefreshPointInfo();//( TRUE, point );
}

void CHero::SetMunpaName(char* Name)
{
	strcpy(m_CharacterInfo.GuildName, Name);
}

void CHero::SetGuildName(char* Name)	
{ 
	SafeStrCpy(m_CharacterInfo.GuildName, Name, MAX_GUILD_NAME+1);	
}

char* CHero::GetGuildName()			
{ 
	return m_CharacterInfo.GuildName;
}

BOOL CHero::CanSocietyAction( DWORD curTime )
{
	if( GetState() != eObjectState_None && GetState() != eObjectState_Society &&
		GetState() != eObjectState_Immortal )
		return FALSE;

	DWORD dwElapsedTime = gCurTime - m_dwLastSocietyAction;
	if( dwElapsedTime > 2000 ) //2ÃÊ
	{
		m_dwLastSocietyAction = gCurTime;
		return TRUE;
	}

	return FALSE;
}

void CHero::SetStage( BYTE Stage )
{
	CPlayer::SetStage( Stage );

	GAMEIN->GetCharacterDialog()->SetStage( Stage );

//	GAMEIN->GetInventoryDialog()->ref
	ABILITYMGR->UpdateAbilityState( ABILITYUPDATE_CHARACTERSTAGE_CHANGED, 0, GetAbilityGroup() );
}

#ifdef _JAPAN_LOCAL_
DWORD CHero::DoGetAttAttackPowerMax( WORD Attrib )
{
	DWORD MaxV = 0;

	WORD SimMek = GetSimMek();
	DWORD MaxLVV = ( GetLevel()+5 ) + 5;		// RateMax = (Level + 5) + 5

	if( PARTYMGR->IsHelpPartyMember() && BATTLESYSTEM->GetBattle(this)->GetBattleKind() != eBATTLE_KIND_VIMUSTREET )
		MaxLVV = MaxLVV + MaxLVV * 20 / 100;

	//¾ÆÀÌÅÛ + Æ¯±âÀÇ ¼Ó¼º°ø°Ý·Â
	DWORD AttribPlus = GetAttribPlusPercent( Attrib );

	MaxV = MaxLVV + AttribPlus + SimMek/2;

	return MaxV;
}

DWORD CHero::DoGetAttAttackPowerMin( WORD Attrib )
{
	DWORD MinV = 0;

	WORD SimMek = GetSimMek();
	DWORD MinLVV = ( GetLevel()+5 ) - 5;		// RateMax = (Level + 5) - 5

	if( PARTYMGR->IsHelpPartyMember() && BATTLESYSTEM->GetBattle(this)->GetBattleKind() != eBATTLE_KIND_VIMUSTREET )
		MinLVV = MinLVV + MinLVV * 20 / 100;

	//¾ÆÀÌÅÛ + Æ¯±âÀÇ ¼Ó¼º°ø°Ý·Â
	DWORD AttribPlus = GetAttribPlusPercent( Attrib );

	MinV = MinLVV + AttribPlus + SimMek/2;

	return MinV;
}

DWORD CHero::GetAttribPlusPercent( WORD Attrib )
{
	return GetItemStats()->AttributeAttack.GetElement_Val(Attrib) + 
		   GetAbilityStats()->AttAttackUp.GetElement_Val(Attrib);
}

#endif

////////////////////////////////////////////////////////
//06. 06 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
//ÀÌÆåÆ® »ý·«(¹«ÃÊ)
BOOL CHero::IsSkipSkill()
{
	//Çö°æ Å»¸¶°¡ ¾Æ´Ï¸é »ç¿ë¸øÇÔ
	if(	m_CharacterInfo.Stage != eStage_Hyun &&
		m_CharacterInfo.Stage != eStage_Tal )
		return FALSE;

	//¹«ÃÊ»óÅÂ°¡ °É·ÁÀÖ´ÂÁö °Ë»ç
	WORD SkipKind = 0;

	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetSkipEffectKind(SkipKind);
	PTRLISTSEARCHEND;

	if(SkipKind == 0)
		return FALSE;
	else
		return (BOOL)SkipKind;
}
////////////////////////////////////////////////////////

//SW070127 Å¸ÀÌÅº
BOOL CHero::CanUseTitanSkill()
{
	BOOL canUse = FALSE;
	//±âÈ¹º¯°æ //Àåºñ¿Í ¹«°ü
	//canUse = ( HERO->InTitan() && TITANMGR->CheckUsingEquipItemNum() );
	canUse = HERO->InTitan();

	return canUse;
}

////// 2007. 6. 14. CBH - ¼¼Æ®¾ÆÀÌÅÆ ´É·ÂÄ¡ °è»ê ÇÔ¼ö Ãß°¡
void CHero::AddSetitemOption(SET_ITEM_OPTION* pSetItemOption)
{
	float attrvalue = 0;

	/// ±Ù°ñ
	m_setItemStats.wGenGol += pSetItemOption->wGenGol;	
	//¹ÎÃ¸
	m_setItemStats.wMinChub += pSetItemOption->wMinChub;
	//Ã¼·Â
	m_setItemStats.wCheRyuk += pSetItemOption->wCheRyuk;
	//½É¸Æ
	m_setItemStats.wSimMek += pSetItemOption->wSimMek;
	//ÃÖ´ë»ý¸í·Â
	m_setItemStats.dwLife += pSetItemOption->dwLife;
	//È£½Å°­±â
	m_setItemStats.dwShield += pSetItemOption->dwShield;
	//ÃÖ´ë³»·Â
	m_setItemStats.dwNaeRyuk += pSetItemOption->dwNaeRyuk;
	//¼Ó¼º ÀúÇ×·Â	
	m_setItemStats.AttrRegist.AddATTRIBUTE_VAL(pSetItemOption->AttrRegist,1);	
	//±Ù°Å¸® ÃÖ¼Ò °ø°Ý·Â
	m_setItemStats.wMeleeAttackMin += pSetItemOption->wMeleeAttackMin;
	//±Ù°Å¸® ÃÖ´ë °ø°Ý·Â
	m_setItemStats.wMeleeAttackMax += pSetItemOption->wMeleeAttackMax;
	//¿ø°Å¸® ÃÖ¼Ò °ø°Ý·Â
	m_setItemStats.wRangeAttackMin += pSetItemOption->wRangeAttackMin;
	//¿ø°Å¸® ÃÖ´ë °ø°Ý·Â	
	m_setItemStats.wRangeAttackMax += pSetItemOption->wRangeAttackMax;
	//Å©¸®Æ¼ÄÃ
	m_setItemStats.wCriticalPercent += pSetItemOption->wCriticalPercent;	
	//¼Ó¼º °ø°Ý·Â	
	m_setItemStats.AttrAttack.AddATTRIBUTE_VAL(pSetItemOption->AttrAttack,1);		
	//¹°¸®¹æ¾î·Â
	m_setItemStats.wPhyDef += pSetItemOption->wPhyDef;
	//»ý¸í·Â È¸º¹ (°íÁ¤)	
	m_setItemStats.wLifeRecover += pSetItemOption->wLifeRecover;
	//»ý¸í·Â È¸º¹
	m_setItemStats.fLifeRecoverRate += pSetItemOption->fLifeRecoverRate;
	//³»·Â È¸º¹ (°íÁ¤)
	m_setItemStats.wNaeRyukRecover += pSetItemOption->wNaeRyukRecover;
	//³»·Â È¸º¹
	m_setItemStats.fNaeRyukRecoverRate += pSetItemOption->fNaeRyukRecoverRate;
}

void CHero::ClearSetitemOption()
{
	memset(&m_setItemStats, 0, sizeof(SET_ITEM_OPTION));
}
///////////////////////////////////////////////////////////////////////////////////

// magi82 - UniqueItem(070626)
void CHero::AddUniqueItemOption(UNIQUE_ITEM_OPTION_INFO* pUniqueOption)
{
	m_UniqueItemStats.nHp += pUniqueOption->nHp;
	m_UniqueItemStats.nMp += pUniqueOption->nMp;
	m_UniqueItemStats.nShield += pUniqueOption->nShield;
	m_UniqueItemStats.nPhyDamage += pUniqueOption->nPhyDamage;
	m_UniqueItemStats.nCriDamage += pUniqueOption->nCriDamage;
	m_UniqueItemStats.nCriRate += pUniqueOption->nCriRate;
	m_UniqueItemStats.nGengol += pUniqueOption->nGengol;
	m_UniqueItemStats.nMinChub += pUniqueOption->nMinChub;
	m_UniqueItemStats.nCheRyuk += pUniqueOption->nCheRyuk;
	m_UniqueItemStats.nSimMek += pUniqueOption->nSimMek;
	m_UniqueItemStats.nDefen += pUniqueOption->nDefen;
	m_UniqueItemStats.nRange += pUniqueOption->nRange;
	m_UniqueItemStats.nAttR += pUniqueOption->nAttR;
	m_UniqueItemStats.nEnemyDefen += pUniqueOption->nEnemyDefen;
}
//////////////////////////////////////////////////////////////////////////
