// CharacterCalcManager.cpp: implementation of the CCharacterCalcManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterCalcManager.h"
#include "GameIn.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "GameResourceManager.h"
#include "./Audio/MHAudioManager.h"
#include "AbilityManager.h"

#include "CharacterDialog.h"
#include "GameEventManager.h"
#include "ObjectStateManager.h"
#include "ObjectActionManager.h"

#include "ExitManager.h"

#include "GameEventManager.h"
#include "QuestManager.h"
#include "SuryunDialog.h"
#include "JournalManager.h"

#include "MainBarDialog.h"
#include "SkillPointRedist.h"

#include "AppearanceManager.h"
#include "ItemManager.h"
#include "MussangManager.h"
#include "SkillPointResetDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CCharacterCalcManager)
CCharacterCalcManager::CCharacterCalcManager()
{

}

CCharacterCalcManager::~CCharacterCalcManager()
{

}

void CCharacterCalcManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol) 
	{
	case MP_CHAR_LIFE_ACK:
		{
			MSG_INT* msg = (MSG_INT*)pMsg;
			//((cGuage*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGELIFE))->SetCurValue(msg->dwData);
			if(HERO)
			{
				HERO->ChangeLife(msg->nData);
//				CHATMGR->AddMsg(CTC_TOWHOLE,"생명력 변화량 : %d",msg->nData);	//for Debug
			}
		}
		break;
	case MP_CHAR_LIFE_NACK:
		{
			ASSERT(0);
		}
		break;
	case MP_CHAR_SHIELD_ACK:
		{
			MSG_INT* msg = (MSG_INT*)pMsg;
			if(HERO)
			{
				HERO->ChangeShield(msg->nData);
			}
		}
		break;
	case MP_CHAR_SHIELD_NACK:
		{
			ASSERT(0);
		}
		break;
	case MP_CHAR_NAERYUK_ACK:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			//((cGuage*)GAMEIN->GetGuageDlg()->GetWindowForID(CG_GUAGENAERYUK))->SetCurValue(msg->dwData);
			if(HERO)
			{
				HERO->SetNaeRyuk(msg->dwData);
			}		
		}
		break;
	case MP_CHAR_NAERYUK_NACK:
		{
			ASSERT(0);
		}
		break;
	case MP_CHAR_EXPPOINT_ACK:
		{
			MSG_EXPPOINT * msg = (MSG_EXPPOINT *)pMsg;	

			if(!HERO) return;
			
			if(msg->ExpKind == MSG_EXPPOINT::EXPPOINTKIND_ACQUIRE)
			{
				EXPTYPE diff = msg->ExpPoint - HERO->GetExpPoint();
				if(diff<0)
				{
					diff = GAMERESRCMNGR->GetMaxExpPoint(HERO->GetLevel()) - HERO->GetExpPoint() + msg->ExpPoint;
					HERO->SetExpPoint(0);
				}
				HERO->SetExpPoint(msg->ExpPoint);
	
				CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg( 132 ), diff );
			}
			else if(msg->ExpKind == MSG_EXPPOINT::EXPPOINTKIND_DIE)
			{
				EXPTYPE diff = HERO->GetExpPoint() - msg->ExpPoint;
				if(diff == 0)
					return;
				
				if(diff<0)	// 렙다시에...
				{
					diff = GAMERESRCMNGR->GetMaxExpPoint(HERO->GetLevel()) - msg->ExpPoint + HERO->GetExpPoint();
				}

				HERO->SetExpPoint(msg->ExpPoint);

				CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg( 134 ), diff );
			}
		}
		break;
	case MP_CHAR_EXPPOINT_NACK:
		{
			ASSERT(0);
		}
		break;		
	case MP_CHAR_MAXLIFE_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetMaxLife(msg->dwData);
			}
		}
		break;
	case MP_CHAR_MAXSHIELD_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetMaxShield(msg->dwData);
			}
		}
		break;
	case MP_CHAR_MAXNAERYUK_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetMaxNaeRyuk(msg->dwData);
			}

		}
		break;
	case MP_CHAR_PLAYERLEVELUP_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			CPlayer * pPlayer = (CPlayer *)OBJECTMGR->GetObject(msg->dwData);
			if(!pPlayer) return;

			// magi82(4) - Titan(071023) 타이탄일때 이펙트 처리(레벨업)
			if(pPlayer->InTitan() == TRUE)
				EFFECTMGR->StartEffectProcess(eEffect_TitanLevelUpSentence,pPlayer,NULL,0,0);
			else
				EFFECTMGR->StartEffectProcess(eEffect_LevelUpSentence,pPlayer,NULL,0,0);
		}
		break;
	case MP_CHAR_LEVEL_NOTIFY:
		{
			MSG_LEVEL * msg = (MSG_LEVEL*)pMsg;
			// 레벨 셋팅 msg->dwData1
			if(HERO)
			{
				EXPTYPE diff = GAMERESRCMNGR->GetMaxExpPoint(HERO->GetLevel()) - HERO->GetExpPoint();
				diff += msg->CurExpPoint;				
				
				if(msg->Level > HERO->GetLevel())
					CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg( 132 ), diff );

				HERO->SetLevel(msg->Level);
				HERO->SetMaxExpPoint(msg->MaxExpPoint);
				HERO->SetExpPoint(msg->CurExpPoint);
				if( msg->Level > HERO->GetMaxLevel() )
					HERO->SetMaxLevel( msg->Level );
			//	AUDIOMGR->Play(64, HERO);

				//SW051112 무쌍모드
//				MUSSANGMGR->SetMussangMaxPointFromLevel( msg->Level );
				
				ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_CHARACTERLEVEL_CHANGED,HERO->GetLevel(),HERO->GetAbilityGroup());

				// Levelup Event Add
//				GAMEEVENTMGR->AddEvent( eGameEvent1_LevelUp, HERO->GetLevel() );
				GAMEEVENTMGR->AddEvent( eGameEvent_LevelUp, HERO->GetLevel() );
				QUESTMGR->ProcessNpcMark();
//				JOURNALMGR->GetListSyn();
			}
		}
		break;
	case MP_CHAR_LEVELUPPOINT_NOTIFY:
		{
			MSG_WORD * msg = (MSG_WORD *)pMsg;
			// 레벨 셋팅 msg->dwData1
			if(HERO)
			{
				if(msg->wData != 0)
				{				
					GAMEIN->GetCharacterDialog()->SetPointLeveling(TRUE, msg->wData);		// 포인트 배분
					if( !GAMEIN->GetCharacterDialog()->IsActive() )
						GAMEIN->GetMainInterfaceDialog()->SetAlram( OPT_CHARACTERDLGICON, TRUE );
				}
				else
 				{
 					GAMEIN->GetCharacterDialog()->SetPointLevelingHide();
 					//GAMEIN->GetCharacterDialog()->SetPointLeveling(FALSE);		// 포인트 배분
 				}
			}
		}
		break;
	case MP_CHAR_GENGOL_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetGenGol((WORD)msg->dwData);
			}
		}
		break;
	case MP_CHAR_MINCHUB_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetMinChub((WORD)msg->dwData);
			}
		}
		break;
	case MP_CHAR_CHERYUK_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetCheRyuk((WORD)msg->dwData);
			}
		}
		break;
	case MP_CHAR_SIMMEK_NOTIFY:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			if(HERO)
			{
				HERO->SetSimMek((WORD)msg->dwData);
			}
		}
		break;
	case MP_CHAR_FAME_NOTIFY:
		{
			MSG_FAME * pmsg = (MSG_FAME*)pMsg;
			if(HERO)
			{
				HERO->SetFame(pmsg->Fame);
			}
		}
		break;
	case MP_CHAR_BADFAME_NOTIFY:
		{
			MSG_FAME * pmsg = (MSG_FAME*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
			if(pPlayer)
			{
				pPlayer->SetBadFame(pmsg->Fame);
			}
		}
		break;
	case MP_CHAR_BADFAME_ACK:
		{
			MSG_FAME* pmsg = (MSG_FAME*)pMsg;

			// 채팅창에 메세지 추가
			char badfame[16] = { 0, };
			char money[16] = { 0, };
			strcpy(badfame, AddComma(pmsg->Fame));
			strcpy(money, AddComma(pmsg->Fame*BAIL_PRICE));
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(646), money, badfame);
		}
		break;
	case MP_CHAR_BADFAME_NACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			// 악명치를 낮추는데 실패하였습니다
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(647));
		}
		break;
	case MP_CHAR_BADFAME_CHANGED:
		{
			MSG_FAME* pmsg = (MSG_FAME*)pMsg;

			if(HERO)
			{
				HERO->SetFame(pmsg->Fame);
			}
		}
		break;
	case MP_CHAR_PLAYTIME_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if(HERO)
			{

			}
		}
		break;
	case MP_CHAR_PLAYTIME_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if(HERO)
			{
				HERO->SetPlayTime(pmsg->dwData);
			}
		}
		break;
	case MP_CHAR_ABILITYEXPPOINT_ACK:
		{
			MSG_ABILLITY_EXPPOINT * msg = (MSG_ABILLITY_EXPPOINT *)pMsg;	

			if(!HERO) return;
			
			if(msg->ExpKind == MSG_ABILLITY_EXPPOINT::EXPPOINTKIND_ACQUIRE)
			{
				int diff = msg->ExpPoint - HERO->GetAbilityExp();
				HERO->SetAbilityExp(msg->ExpPoint);
	
				CHATMGR->AddMsg( CTC_GETABILITYEXP, CHATMGR->GetChatMsg( 135 ), diff );

				ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_ABILITYEXP_CHANGED,0,HERO->GetAbilityGroup());

			}
			else if(msg->ExpKind == MSG_ABILLITY_EXPPOINT::EXPPOINTKIND_ABILITYUSE)
			{
				ASSERT(0);
				//MessageBox(0,"Ung? -_-a",0,0);
			}
		}
		break;

	case MP_CHAR_ABILITY_UPGRADE_ACK:
		{
			GAMEIN->GetSuryunDialog()->SetDisable(FALSE);
			GAMEIN->GetSkPointDlg()->SetDisable(FALSE);

			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);

			GAMEIN->GetSuryunDialog()->SetSelectAbilityIDX( pInfo->GetIdx() );

			ABILITYMGR->UpgradeAbility(pmsg->wData,HERO->GetAbilityGroup());			
			CHATMGR->AddMsg( CTC_ABILITYUPGRADE_SUCCESS, CHATMGR->GetChatMsg( 137 ), pInfo->GetInfo()->Ability_name );

			// event
			GAMEEVENTMGR->AddEvent( eGameEvent_Ability, pInfo->GetInfo()->Ability_index );

			//
			GAMEIN->GetSkPointDlg()->RefreshAbilityPoint();
			GAMEIN->GetSkPointDlg()->RefreshAbilityIcons();

			// magi82(42) - 샵아이템 추가(수련치 초기화)
			GAMEIN->GetSkillPointResetDlg()->RefreshAbilityIcons();
		}
		break;
	case MP_CHAR_ABILITY_UPGRADE_NACK:
		{
			GAMEIN->GetSuryunDialog()->SetDisable(FALSE);
			GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
			
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);
			CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 138 ), pInfo->GetInfo()->Ability_name );
		}
		break;
	case MP_CHAR_ABILITY_UPGRADE_SKPOINT_ACK:
		{
			GAMEIN->GetSuryunDialog()->SetDisable(FALSE);
			GAMEIN->GetSkPointDlg()->SetDisable(FALSE);

			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);

			GAMEIN->GetSuryunDialog()->SetSelectAbilityIDX( pInfo->GetIdx() );

			ABILITYMGR->UpgradeAbilitySkPoint(pmsg->wData,HERO->GetAbilityGroup());			
			CHATMGR->AddMsg( CTC_ABILITYUPGRADE_SUCCESS, CHATMGR->GetChatMsg( 137 ), pInfo->GetInfo()->Ability_name );

			// event
			GAMEEVENTMGR->AddEvent( eGameEvent_Ability, pInfo->GetInfo()->Ability_index );

			//
			GAMEIN->GetSkPointDlg()->RefreshAbilityPoint();
			GAMEIN->GetSkPointDlg()->RefreshAbilityIcons();
		}
		break;
	case MP_CHAR_ABILITY_UPGRADE_SKPOINT_NACK:
		{
			GAMEIN->GetSuryunDialog()->SetDisable(FALSE);
			GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
			
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);
			CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 138 ), pInfo->GetInfo()->Ability_name );
		}
		break;
	case MP_CHAR_ABILITY_DOWNGRADE_SKPOINT_ACK:
		{
			GAMEIN->GetSuryunDialog()->SetDisable(FALSE);
			GAMEIN->GetSkPointDlg()->SetDisable(FALSE);

			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);

			GAMEIN->GetSuryunDialog()->SetSelectAbilityIDX( pInfo->GetIdx() );

			ABILITYMGR->DowngradeAbilitySkPoint(pmsg->wData,HERO->GetAbilityGroup());
			CHATMGR->AddMsg( CTC_ABILITYUPGRADE_SUCCESS, CHATMGR->GetChatMsg( 740 ), pInfo->GetInfo()->Ability_name );

			// event
			GAMEEVENTMGR->AddEvent( eGameEvent_Ability, pInfo->GetInfo()->Ability_index );

			//
			GAMEIN->GetSkPointDlg()->RefreshAbilityPoint();
			GAMEIN->GetSkPointDlg()->RefreshAbilityIcons();

#ifndef _JAPAN_LOCAL_
			// 06. 03. 국내무쌍 - 이영준
			if(pmsg->wData == ABILITYINDEX_TACTIC_MUSSANG)
				MUSSANGMGR->SetMussangPoint(0);
#endif
		}
		break;
	case MP_CHAR_ABILITY_DOWNGRADE_SKPOINT_NACK:
		{
			GAMEIN->GetSuryunDialog()->SetDisable(FALSE);
			GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
			
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);
			CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 741 ), pInfo->GetInfo()->Ability_name );
		}
		break;
		// magi82(42) - 샵아이템 추가(수련치 초기화)
	case MP_CHAR_ABILITY_RESET_SKPOINT_ACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CAbilityInfo* pInfo = ABILITYMGR->GetAbilityInfo(pmsg->wData);

			ABILITYMGR->ResetAbilitySkPoint(pmsg->wData,HERO->GetAbilityGroup());

			GAMEIN->GetSkillPointResetDlg()->SetActive(FALSE);

#ifndef _JAPAN_LOCAL_
			if(pmsg->wData == ABILITYINDEX_TACTIC_MUSSANG)
				MUSSANGMGR->SetMussangPoint(0);
#endif
		}
		break;
	case MP_CHAR_EXITSTART_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			//종료카운트다운시작
			EXITMGR->StartExit();			
		}
		break;
	case MP_CHAR_EXITSTART_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;			
			EXITMGR->RejectExit( pmsg->bData );
		}
		break;
	case MP_CHAR_EXIT_ACK:
		{
			//종료
			EXITMGR->Exit();
		}
		break;
	case MP_CHAR_EXIT_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;			
			EXITMGR->CancelExit( pmsg->bData );
		}
		break;
	case MP_CHAR_YOUAREDIED:
		{
			if( HERO->GetState() != eObjectState_Die )
			{
				ASSERT(HERO->IsDied());

				if(gCurTime - HERO->GetDiedTime() < 10000)
					return;

				OBJECTACTIONMGR->Die(HERO,HERO,FALSE,FALSE,FALSE);
				GAMEIN->GetSkPointDlg()->SetActive( FALSE );
			}
		}
		break;
	case MP_CHAR_POINTMINUS_ACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			--HERO->GetShopItemStats()->StatePoint;
			++HERO->GetShopItemStats()->UseStatePoint;
			GAMEIN->GetCharacterDialog()->RefreshPointInfo();
		}
		break;
	case MP_CHAR_POINTMINUS_NACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(756) );
		}
		break;
	case MP_CHAR_STAGE_NOTIFY:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			if( pmsg->dwObjectID == HEROID )
			{
				HERO->SetStage( pmsg->bData );	
				char temp[64];
				switch( pmsg->bData )
				{
				case eStage_Hwa:
					{
						sprintf( temp, CHATMGR->GetChatMsg(890) );
						EFFECTMGR->StartEffectProcess( eEffect_ChangeStage_Hwa, HERO, NULL, 0, 0 );
					}
					break;
				case eStage_Hyun:
					{
						sprintf( temp, CHATMGR->GetChatMsg(891) );
						EFFECTMGR->StartEffectProcess( eEffect_ChangeStage_Hwa, HERO, NULL, 0, 0 );
					}
					break;
				case eStage_Geuk:
					{
						sprintf( temp, CHATMGR->GetChatMsg(892) );
						EFFECTMGR->StartEffectProcess( eEffect_ChangeStage_Geuk, HERO, NULL, 0, 0 );
					}
					break;
				case eStage_Tal:
					{
						sprintf( temp, CHATMGR->GetChatMsg(893) );
						EFFECTMGR->StartEffectProcess( eEffect_ChangeStage_Geuk, HERO, NULL, 0, 0 );
					}
					break;
				}
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(900), temp );

				APPEARANCEMGR->AddCharacterPartChange( HEROID );
			}
			else
			{
				CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
				if( !pPlayer )	return;
				pPlayer->SetStage( pmsg->bData );

				switch( pmsg->bData )
				{
				case eStage_Hwa:
				case eStage_Hyun:
					{
						EFFECTMGR->StartEffectProcess( eEffect_ChangeStage_Hwa, pPlayer, NULL, 0, 0 );
					}
					break;
				case eStage_Geuk:
				case eStage_Tal:
					{
						EFFECTMGR->StartEffectProcess( eEffect_ChangeStage_Geuk, pPlayer, NULL, 0, 0 );
					}
					break;
				}
				APPEARANCEMGR->AddCharacterPartChange( pmsg->dwObjectID );
			}						
		}
		break;
		//SW051112 무쌍모드
	case MP_CHAR_MUSSANG_INFO:
		{
			MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;

			if( !HERO ) return;

			if( pmsg->bData & eMSS_LevelUp )
			{
				MUSSANGMGR->SetMussangMaxPoint(pmsg->dwData);
				break;
			}
				
			MUSSANGMGR->SetMussangPoint(pmsg->dwData);
			
			if( pmsg->bData & eMSS_Ready )
			{
				if( !MUSSANGMGR->IsMussangReady() )
				{
					MUSSANGMGR->SetMussangReady(TRUE);	//게이지 깜박임과 버튼 활성화 
					MUSSANGMGR->SetValidMsg(TRUE);
				}
			}
		}
		break;
	case MP_CHAR_MUSSANG_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(!pPlayer) return;
				
			if( HERO == pPlayer )
			{
				MUSSANGMGR->SetMussangReady(FALSE);
				MUSSANGMGR->SetMussangMode(TRUE, pmsg->dwData);	// 내부 상태 아이콘 표시/해제
			}

			if((pPlayer->GetCharacterTotalInfo()->bVisible == FALSE)
				||(!pPlayer->GetSingleSpecialState(eSingleSpecialState_Hide)))
			{
#ifdef _JAPAN_LOCAL_
				OBJECTEFFECTDESC desc(FindEffectNum("maintain_mussang.beff"));
				pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID, &desc, 1, pPlayer);
#else
				// 06. 03. 국내무쌍 - 이영준
				switch(pPlayer->GetStage())
				{
				case eStage_Normal:	
					{
						OBJECTEFFECTDESC desc(1035);
						pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID1, &desc, 1, pPlayer);
					}
					break;
				case eStage_Hwa:		
				case eStage_Hyun:
					{
						OBJECTEFFECTDESC desc(888);
						pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID2, &desc, 1, pPlayer);
					}
					break;
				case eStage_Geuk:
				case eStage_Tal:
					{
						OBJECTEFFECTDESC desc(889);
						pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID3, &desc, 1, pPlayer);
					}
					break;
				}
#endif		
			}
		
		}
		break;
	case MP_CHAR_MUSSANG_NACK:
		{
			MUSSANGMGR->SetValidMsg(TRUE);
			//무쌍 수치 부족으로 인한 경우 클라이언트에서 처리하고 서버로 메시지 안보내니 무효!
		}
		break;
	case MP_CHAR_MUSSANG_END:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(!pPlayer) return;
#ifdef _JAPAN_LOCAL_
			pPlayer->RemoveObjectEffect( MUSSANG_EFFECT_ID );
#else
			// 06. 03. 국내무쌍 - 이영준
			switch(pPlayer->GetStage())
			{
			case eStage_Normal:	
				{
					pPlayer->RemoveObjectEffect( MUSSANG_EFFECT_ID1 );
				}
				break;
			case eStage_Hwa:		
			case eStage_Hyun:
				{
					pPlayer->RemoveObjectEffect( MUSSANG_EFFECT_ID2 );
				}
				break;
			case eStage_Geuk:
			case eStage_Tal:
				{
					pPlayer->RemoveObjectEffect( MUSSANG_EFFECT_ID3 );
				}
				break;
			}
#endif		
			
			if( HERO == pPlayer )
			{
				MUSSANGMGR->SetMussangMode(FALSE);	// 내부 상태 아이콘 표시/해제
			}
		}
		break;

#ifdef _JAPAN_LOCAL_
	case MP_CHAR_CHANGE_SUBATTR_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( HEROID == pmsg->dwObjectID )
			{
				HERO->SetMainCharAttr( LOWORD(pmsg->dwData) );
				HERO->SetSubCharAttr( HIWORD(pmsg->dwData) );
				switch( HERO->GetSubCharAttr() )
				{
				case eCA_Fire:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1192), CHATMGR->GetChatMsg(1187) );	break;
				case eCA_Earth:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1192), CHATMGR->GetChatMsg(1188) );	break;
				case eCA_Iron:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1192), CHATMGR->GetChatMsg(1189) );	break;
				case eCA_Water:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1192), CHATMGR->GetChatMsg(1190) );	break;
				case eCA_Tree:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1192), CHATMGR->GetChatMsg(1191) );	break;
				}
				ITEMMGR->RefreshAllItem();
				GAMEIN->GetCharacterDialog()->UpdateData();
			}
		}
		break;
	case MP_CHAR_CHANGE_SUBATTR_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( HEROID == pmsg->dwObjectID )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1193) );
		}
		break;

#endif
	case MP_CHAR_SINGLE_SPECIAL_STATE_NOTIFY:
		{
			MSG_SINGLE_SPECIAL_STATE* pmsg = (MSG_SINGLE_SPECIAL_STATE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->Operator);

			OBJECTMGR->ApplySingleSpecialState(pPlayer, pmsg->State, pmsg->bVal);
		}
		break;
	case MP_CHAR_SINGLE_SPECIAL_STATE_ACK:
		{
			MSG_SINGLE_SPECIAL_STATE* pmsg = (MSG_SINGLE_SPECIAL_STATE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->Operator);

			switch(pmsg->State)
			{
			// 은신 사용
			case eSingleSpecialState_Hide:
				{
					if(pmsg->bVal)
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1327));
					else
					{
						if(pmsg->Forced)
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1328));
						// 일반 해제
						else
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1331));
					}
				}
				break;
			// 혜안 스킬 사용
			case eSingleSpecialState_Detect:
				{
					if(pmsg->bVal)
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1337));
					else
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1334));

				}
				break;
			// 혜안 물약 사용
			case eSingleSpecialState_DetectItem:
				{
					if(pmsg->bVal)
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1333));
					else
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1334));
				}
				break;
			}

			OBJECTMGR->ApplySingleSpecialState(pPlayer, pmsg->State, pmsg->bVal);
		}
		break;
	case MP_CHAR_SINGLE_SPECIAL_STATE_NACK:
		{
			MSG_SINGLE_SPECIAL_STATE_NACK* pmsg = (MSG_SINGLE_SPECIAL_STATE_NACK*)pMsg;

			switch(pmsg->ErrCode)
			{
			case eSingleSpecialState_Error_NaeRyuk:
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(401));
				}
				break;
			case eSingleSpecialState_Error_Delay:
				{
					switch(pmsg->State)
					{
					case eSingleSpecialState_Hide:
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1330));
						break;
					case eSingleSpecialState_Detect:
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1332));
						break;
					}				
				}
				break;
			}
		}
		break;
	case MP_CHAR_FULLMOONEVENT_CHANGE:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
			if( !pPlayer )		return;

			pPlayer->SetFullMoonEventIndex( pmsg->bData );
			APPEARANCEMGR->InitAppearance( pPlayer );

			if( pPlayer->GetID() == HEROID )
			{
				if( pmsg->bData == eFULLMOONEVENT_BUN )
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1453));
				else if( pmsg->bData == eFULLMOONEVENT_CHINESECAKE )
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1458));
				else
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1454));
			}
		}		
		break;
	case MP_CHAR_NOACTIONPANELTY_NOTIFY:
		{
            CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1606) );
			HERO->SetNoActionPenalty();
		}
		break;
	case MP_CHAR_POINTADD_ACK:
		{
			if( HERO->GetShopItemStats()->UseStatePoint )
				--HERO->GetShopItemStats()->UseStatePoint;
			GAMEIN->GetCharacterDialog()->RefreshPointInfo();
		}
		break;
	case MP_CHAR_POINTADD_NACK:
		{
		}
		break;
	}
}
