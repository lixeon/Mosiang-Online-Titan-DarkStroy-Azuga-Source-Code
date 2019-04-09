// HeroMove.cpp: implementation of the CHeroMove class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifdef _CHEATENABLE_
#include "CheatMsgParser.h"
#endif	//_CHEATENABLE_
#include "HeroMove.h"
#include "SkillManager_Client.h"
#include "MoveManager.h"
#include "ObjectManager.h"
#include "ChatDialog.h"
#include "InventoryExDialog.h"
#include "GameIn.h"
#include "MousePointer.h"
//#include "WindowIdEnum.h"
//#include "interface\cWindowManager.h"
//#include "./Interface/cListDialog.h"
#include "Interface/cEditBox.h"

#include "GameResourceManager.h"
#include "ExchangeManager.h"
//#include "cMsgBox.h"
#include "chatmanager.h"
#include "NpcScriptManager.h"

#include "BattleSystem_Client.h"

#include "Input/UserInput.h"
#include "MouseCursor.h"
#include "Interface/cWindowManager.h"
#include "PartyManager.h"

#include "PetBase.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHeroMove::CHeroMove()
{

}

CHeroMove::~CHeroMove()
{

}

void CHeroMove::OnMouseOver( CObject* pObject )
{
	if( !pObject )	return;
	OBJECTMGR->OnMouseOver( pObject );
	
	if( HERO )
	{
		CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
		if( pBattle->IsEnemy( HERO, pObject ) )
		{
			//// 2007. 7. 11. CBH - 전문기술 전용 몬스터 커서 처리 추가
			BYTE bObjectKind = pObject->GetObjectKind();
			if( bObjectKind == eObjectKind_Mining )
			{
				CURSOR->SetCursor( eCURSOR_MINING );
			}
			else if( bObjectKind == eObjectKind_Collection )
			{
				CURSOR->SetCursor( eCURSOR_COLLECTION );
			}
			else if( bObjectKind == eObjectKind_Hunt )
			{
				CURSOR->SetCursor( eCURSOR_HUNT );
			}
			///////////////////////////////////////////////////////////////
			else
			{
				CURSOR->SetCursor( eCURSOR_ATTACK );
			}
		}
		else
		{
			if( pObject->GetObjectKind() == eObjectKind_Npc )
			{
				// magi82 - Titan(070511) 타이탄 장착아이템 수리할때
				if( CURSOR->GetCursor() != eCURSOR_TITANREPAIR )
				{
					if( ((CNpc*)pObject)->GetNpcKind() != 30 && ((CNpc*)pObject)->GetNpcKind() != 31 )
						CURSOR->SetCursor( eCURSOR_DEALER );
				}
			}
			// magi82 - Titan(070511) 타이탄 장착아이템 수리할때
			else if( CURSOR->GetCursor() != eCURSOR_TITANREPAIR )
                CURSOR->SetCursor( eCURSOR_DEFAULT );
		}			
	}
}

void CHeroMove::OnMouseLeave( CObject* pObject )
{
	if( !pObject )	return;
	OBJECTMGR->OnMouseLeave( pObject );
}

void CHeroMove::OnClickPlayer(DWORD MouseEvent,CPlayer* pPlayer)
{
	if(HEROID == pPlayer->GetID()) return;
	
	// RaMa - 05.05.27  -> Guild Tournament에 Observer로 참가한 경우 클릭 안되게..
/*	if( BATTLESYSTEM->GetBattle( HERO )->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
	if( BATTLESYSTEM->GetBattle( HERO )->GetBattleTeamID( HERO ) == 2 )			// Observer
		return;*/

//귓말아이디 등록
	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_MENU ) )
	if( GAMEIN->GetChatDialog() )
	{
		static char str[MAX_NAME_LENGTH+3];
		wsprintf( str, "/%s ", pPlayer->GetObjectName() );
		GAMEIN->GetChatDialog()->GetChatEditBox()->SetEditText( str );
		GAMEIN->GetChatDialog()->GetChatEditBox()->SetFocusEdit( TRUE );
		return;
	}

	OnAttack( pPlayer, MouseEvent );

/*
	CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
	
	if( pBattle->GetBattleKind() == eBATTLE_KIND_NONE )
	{
		if( g_keyboard.GetKeyPressed( KEY_CONTROL ) )
		{
//			OnAttack( pPlayer, MouseEvent );
		}
		else
		{
			if( MouseEvent & MOUSEEVENT_LCLICK )
				OBJECTMGR->SetSelectedObject( pPlayer );
		}
	}
	//KES VIMU 031020 문파전, 비무중일 때
	else if( pBattle->GetBattleState() == eBATTLE_STATE_FIGHT )
	{
		if( pBattle->IsEnemy( HERO, pPlayer ) )
		{
			OnAttack( pPlayer, MouseEvent );
		}
		else
		{
			if( MouseEvent & MOUSEEVENT_LCLICK )
				OBJECTMGR->SetSelectedObject( pPlayer );
		}
	}
*/
}

void CHeroMove::OnClickMonster(DWORD MouseEvent,CMonster* pMonster)
{
	// RaMa - 05.05.27  -> Guild Tournament에 Observer로 참가한 경우 클릭 안되게..
/*	if( BATTLESYSTEM->GetBattle( HERO )->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
	if( BATTLESYSTEM->GetBattle( HERO )->GetBattleTeamID( HERO ) == 2 )			// Observer
		return;*/

#ifdef _TESTCLIENT_
	if(KEYBOARD->GetKeyPressed(KEY_CONTROL))
	{
		if(MouseEvent & MOUSEEVENT_LCLICK)
		{
			TARGETSET Target;
			Target.pTarget = pMonster;
			memset(&Target.ResultInfo,0,sizeof(RESULTINFO));
			Target.ResultInfo.bCritical = rand()%2 ? TRUE : FALSE;
			Target.ResultInfo.RealDamage = rand() % 50;
			Target.ResultInfo.ShieldDamage = rand() % 30;
			Target.ResultInfo.CounterDamage = 0;
			//Target.TargetData = rand() % 100;
			EFFECTMGR->StartEffectProcess(GAMERESRCMNGR->m_TestClientInfo.Effect,HERO,&Target,1,pMonster->GetID(),EFFECT_FLAG_HEROATTACK);
		}
		else if(MouseEvent == MOUSEEVENT_RCLICK)
		{
			TARGETSET Target;
			Target.pTarget = HERO;
			memset(&Target.ResultInfo,0,sizeof(RESULTINFO));
			Target.ResultInfo.bCritical = rand()%2 ? TRUE : FALSE;
			Target.ResultInfo.RealDamage = rand() % 50;
			Target.ResultInfo.ShieldDamage = rand() % 30;
			Target.ResultInfo.CounterDamage = 0;
			//Target.TargetData = rand() % 100;
			//Target.TargetData = 30;
			EFFECTMGR->StartEffectProcess(GAMERESRCMNGR->m_TestClientInfo.MonsterEffect,pMonster,&Target,1,HERO->GetID(),EFFECT_FLAG_HEROATTACK);
		}
		return;
	}
	else
	{
		CActionTarget Target;
		Target.InitActionTarget(pMonster,NULL);
		BOOL bMugong = FALSE;
		if(MouseEvent & MOUSEEVENT_RCLICK)
		{
			bMugong = TRUE;
			SKILLMGR->OnSkillCommand(HERO, &Target, bMugong);
		}
	}
#endif

	OnAttack( pMonster, MouseEvent );
}

void CHeroMove::OnClickNpc(DWORD MouseEvent, CNpc* pNpc)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;
	
	BYTE BattleKind = BATTLESYSTEM->GetBattle(HERO)->GetBattleKind();
	if( BattleKind != eBATTLE_KIND_NONE && BattleKind != eBATTLE_KIND_SIEGEWAR )	//비무중
		return;

	if( MouseEvent & MOUSEEVENT_DBLCLICK )
	{
		////////////////////////////////////////////////////////////////////////////////
		// 06. 06. 2차 전직 - 이영준
		// 은신 / 혜안
		// 은신 패널티
		if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
			return;
		}
		////////////////////////////////////////////////////////////////////////////////
		
		CActionTarget Target;
		Target.InitActionTarget( pNpc, NULL );

		CAction MoveAction;
		float fDistance = NPC_TALKING_DISTANCE;
		if( pNpc->GetNpcJob() == SYMBOL_ROLE )
			fDistance = NPC_TALKING_DISTANCE/2;
		MoveAction.InitAction( eActionKind_TalkToNpc, NULL, fDistance, &Target );
		MOVEMGR->SetHeroActionMove( &Target, &MoveAction );		
	}	
	else if( MouseEvent & MOUSEEVENT_LCLICK )
	{
		if( OBJECTMGR->GetSelectedObjectID() == pNpc->GetID() )
		{
			////////////////////////////////////////////////////////////////////////////////
			// 06. 06. 2차 전직 - 이영준
			// 은신 / 혜안
			// 은신 패널티
			if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
				return;
			}
			////////////////////////////////////////////////////////////////////////////////
//			float dist = MOVEMGR->CalcDistanceXZ(pNpc,HERO);
//			if(dist > NPC_TALKING_DISTANCE)
//			{
				CActionTarget Target;
				Target.InitActionTarget( pNpc, NULL );

				CAction MoveAction;
				float fDistance = NPC_TALKING_DISTANCE;
				if( pNpc->GetNpcJob() == SYMBOL_ROLE )
					fDistance = NPC_TALKING_DISTANCE/2;
				MoveAction.InitAction( eActionKind_TalkToNpc, NULL, fDistance, &Target );
				MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
//			}
/*
			else
			{
				switch( pNpc->GetNpcKind() )
				{
				case 31:
					{
						char buf[32] = {0,};
						int nLen = strlen( pNpc->GetObjectName() ) - 6; //"가는길"제외
						strncpy( buf, pNpc->GetObjectName(), nLen );
						buf[nLen] = 0;

						cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_MAPCHANGE, MBT_YESNO, CHATMGR->GetChatMsg(360), buf );

						if( pBox )
						{
							pBox->SetParam( pNpc->GetID() );							
							MOVEMGR->HeroMoveStop();
							HERO->SetState(eObjectState_Deal);
						}
					}
					break;
				case 30:
					{
						cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_SELECTLOGINPOINT, MBT_YESNO, CHATMGR->GetChatMsg(361), pNpc->GetObjectName() );
						if( pBox )
						{
							pBox->SetParam( pNpc->GetID() );
							MOVEMGR->HeroMoveStop();
							HERO->SetState(eObjectState_Deal);
						}
					}
					break;
				default:
					{
						NPCSCRIPTMGR->StartNpcScript( pNpc );
						MOVEMGR->HeroMoveStop();
						HERO->SetState(eObjectState_Deal);
					}
					break;
				}

				pNpc->OnClicked();
			}
*/
		}
//		else			
//		{
//			OBJECTMGR->SetSelectedObject( pNpc );
//		}
	}
}

void CHeroMove::OnClickSkillObject(DWORD MouseEvent,CSkillObject* pObj)
{
	OnAttack( pObj, MouseEvent );
}

void CHeroMove::OnClickGround(DWORD MouseEvent,VECTOR3* pPos)
{

#ifdef _TESTCLIENT_

	if(KEYBOARD->GetKeyPressed(KEY_MENU) && KEYBOARD->GetKeyPressed(KEY_SPACE))
	{
		VECTOR3 pos;
		pos.x = pPos->x;
		pos.y = 0;
		pos.z = pPos->z;
		MOVEMGR->SetPosition(HERO,&pos);
	}
	else
	{
		if( MouseEvent & MOUSEEVENT_LCLICK )
			MOVEMGR->SetHeroTarget(pPos);
		else
			MOVEMGR->SetHeroTarget(pPos,FALSE);

		HERO->DisableAutoAttack();
	}
#else

#ifdef _CHEATENABLE_
	if(KEYBOARD->GetKeyPressed(KEY_MENU) && KEYBOARD->GetKeyPressed(KEY_SPACE))
	{
#ifndef _GMTOOL_
		if( CHEATMGR->IsCheatEnable() )	
#endif
		{
			MOVE_POS msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_MOVE_SYN;
			msg.dwObjectID = HEROID;
			msg.dwMoverID = HEROID;
			msg.cpos.Compress(pPos);
			NETWORK->Send(&msg,sizeof(msg));
		}
	}
	else
#endif	//_CHEATENABLE_
	{
		if(HERO)
		{
			if( MouseEvent & MOUSEEVENT_LCLICK )
				MOVEMGR->SetHeroTarget(pPos);
			else
				MOVEMGR->SetHeroTarget(pPos,FALSE);

			HERO->DisableAutoAttack();
			HERO->SetCurComboNum(0);
			HERO->SetMovingAction(NULL);
		}
	}

#endif	//_TESTCLIENT_

}

void CHeroMove::OnClickPet(DWORD MouseEvent, CPetBase* pPet)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;

	if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_NONE )	//비무중
		return;

	if( MouseEvent & MOUSEEVENT_DBLCLICK )
	{
		CActionTarget Target;
		Target.InitActionTarget( pPet, NULL );

		CAction MoveAction;
		MoveAction.InitAction( eActionKind_TalkToPet, NULL, NPC_TALKING_DISTANCE, &Target );
		MOVEMGR->SetHeroActionMove( &Target, &MoveAction );		
	}	
	else if( MouseEvent & MOUSEEVENT_LCLICK )
	{
		if( OBJECTMGR->GetSelectedObjectID() == pPet->GetID() )
		{
			CActionTarget Target;
			Target.InitActionTarget( pPet, NULL );

			CAction MoveAction;
			MoveAction.InitAction( eActionKind_TalkToPet, NULL, NPC_TALKING_DISTANCE, &Target );
			MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
		}
	}
}


void CHeroMove::OnClickMapObject(DWORD MouseEvent, CObject* pObj)
{

}



#define ATTACK_TICK	400

// 2005 크리스마스 이벤트 코드
BOOL IsEventAttackable(CObject* pTargetObj)
{
	if( HERO->GetWeaponEquipType() == WP_EVENT )
	{
		// 인벤토리에 눈덩이가 없으면 공격 불가...
		
		//CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemLike( EVENT_ITEM_SNOW );
		//SW061211 크리스마스이벤트
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetPriorityItemForCristmasEvent();
		
		if( !pItem )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(583) );
			return	FALSE;
		}
	}
	else
	{
		// 이벤트 무기를 장착하지 않으면
		// 산타의요정은 공격 불가...
		
		if( pTargetObj->GetObjectKind() & eObjectKind_Monster )
		{
			CMonster* pMonster = (CMonster*)pTargetObj;
			
			if( pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 || pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2
				|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_SM || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_SM
				|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_MD || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_MD
				|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_LG || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_LG
				//|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_SM
				//|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_MD
				//|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_LG
				)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(584) );
				return FALSE;
			}
		}
	}

	return TRUE;
}

void CHeroMove::OnAttack(CObject* pTargetObj,DWORD MouseEvent)
{	
	static CObject* pLastObj		= NULL;
	static DWORD	dwAttackTime	= 0;
	
	if( HERO && pTargetObj )
	{
		if( HERO->GetCharacterTotalInfo()->bVisible )	//안보일때는 공격이 안되도록 수정 //KES
		{
			if( pLastObj != pTargetObj || gCurTime - dwAttackTime > ATTACK_TICK )
			{
				// RaMa - 2005.01.31   -> 죽은애는 공격할 수 없다.
				if( pTargetObj->GetState() == eObjectState_Die )
					return;

				if( MouseEvent & MOUSEEVENT_LCLICK )
				{
					if( OBJECTMGR->GetSelectedObjectID() == pTargetObj->GetID() )
					{
						////////////////////////////////////////////////////////////////////////////////
						// 06. 06. 2차 전직 - 이영준
						// 은신 / 혜안
						// 은신 패널티
						if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
						{
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
							return;
						}
						////////////////////////////////////////////////////////////////////////////////
						// 2005 크리스마스 이벤트 코드
						if( !IsEventAttackable(pTargetObj) )
							return;

						if( HERO->IsPKMode() )
						if( PARTYMGR->IsPartyMember( pTargetObj->GetID() ) )						
						{ //범위공격으로 파티원을 공격할때는 메세지 없게 하기 위해
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(630) );
							return;
						}
						
						CActionTarget Target;
						Target.InitActionTarget(pTargetObj,NULL);
						
						HERO->EnableAutoAttack(&Target);		
						SKILLMGR->OnSkillCommand(HERO, &Target, FALSE);	
						pLastObj		= pTargetObj;
						dwAttackTime	= gCurTime;
					}
				}
				else if( MouseEvent & MOUSEEVENT_DBLCLICK )
				{
					////////////////////////////////////////////////////////////////////////////////
					// 06. 06. 2차 전직 - 이영준
					// 은신 / 혜안
					// 은신 패널티
					if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
						return;
					}
					////////////////////////////////////////////////////////////////////////////////
					// 2005 크리스마스 이벤트 코드
					if( !IsEventAttackable(pTargetObj) )
						return;

					if( HERO->IsPKMode() )
					if( PARTYMGR->IsPartyMember( pTargetObj->GetID() ) )						
					{ //범위공격으로 파티원을 공격할때는 메세지 없게 하기 위해
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(630) );
						return;
					}
					CActionTarget Target;
					Target.InitActionTarget(pTargetObj,NULL);
					
					HERO->EnableAutoAttack(&Target);		
					SKILLMGR->OnSkillCommand(HERO, &Target, FALSE);	
					pLastObj		= pTargetObj;
					dwAttackTime	= gCurTime;
				}
			}
		}
	}
}

// LBS 노점상 관련 03.09.30
void CHeroMove::OnDbClickPlayer(DWORD MouseEvent,CPlayer* pPlayer)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;

	if( OBJECTMGR->GetSelectedObjectID() )
	{
		if( OBJECTMGR->GetSelectedObjectID() == pPlayer->GetID() )
		{
			//노점상일 경우에만 메세지를 보내자.
			if( pPlayer->GetState() == eObjectState_StreetStall_Owner )
			{
				CActionTarget Target;
				Target.InitActionTarget( pPlayer, NULL );

				CAction MoveAction;
				MoveAction.InitStallAction( STREETSTALL_ENTER_DISTANCE, &Target );
				MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
			}
			else
			{
				OnAttack( pPlayer, MOUSEEVENT_DBLCLICK );
			}
		}
	}
}

void CHeroMove::OnDbClickMonster(DWORD MouseEvent,CMonster* pMonster)
{
	OnAttack( pMonster, MouseEvent );
}

void CHeroMove::OnDbClickNpc(DWORD MouseEvent, CNpc* pNpc)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;

	////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신 / 혜안
	// 은신 패널티
	if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return;
	}	////////////////////////////////////////////////////////////////////////////////

	CActionTarget Target;
	Target.InitActionTarget( pNpc, NULL );
				
	CAction MoveAction;
	float fDistance = NPC_TALKING_DISTANCE;
	if( pNpc->GetNpcJob() == SYMBOL_ROLE )
		fDistance = NPC_TALKING_DISTANCE/2;
	MoveAction.InitAction( eActionKind_TalkToNpc, NULL, fDistance, &Target );
	MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
}

void CHeroMove::AttackSelectedObject()
{
	CObject* pTargetObj = OBJECTMGR->GetSelectedObject();
	if( !pTargetObj ) return;

	OnAttack(pTargetObj,MOUSEEVENT_LCLICK);
}

// LBS 03.10.09
void CHeroMove::SelectLoginPoint( WORD LoginPointIndex )
{
	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_SAVEPOINT_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = LoginPointIndex;
				
	NETWORK->Send(&msg, sizeof(msg));

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(161) );
}

// magi82(35) MapChange State
// 맵이동 할때 어떤 상태의 맵이동인지 체크하게 수정
void CHeroMove::MapChange( WORD MapIndex, DWORD ItemIdx, DWORD ItemPos,DWORD state )
{
	MSG_DWORD4 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANGEMAP_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1 = MapIndex;
	msg.dwData2 = ItemIdx;
	msg.dwData3 = ItemPos;
	msg.dwData4 = state;
	
	NETWORK->Send(&msg, sizeof(msg));	
	
	g_UserInput.SetInputFocus(FALSE);
}
