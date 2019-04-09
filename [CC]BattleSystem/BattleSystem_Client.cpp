// BattleSystem.cpp: implementation of the CBattleSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _MHCLIENT_

#include "BattleSystem_Client.h"

#include "BattleFactory.h"
#include "VimuStreet/BattleFactory_VimuStreet.h"
#include "MurimField/BattleFactory_MurimField.h"
#include "MunpaField/BattleFactory_MunpaField.h"
#include "Suryun/BattleFactory_Suryun.h"
#include "GTournament/BattleFactory_GTournament.h"
#include "SiegeWar/BattleFactory_SiegeWar.h"
#include "SiegeWar/Battle_SiegeWar.h"

#include "ObjectManager.h"
#include "VimuManager.h"

#include "ChatManager.h"
#include "MHMap.h"
#include "MainGame.h"

#include "MHCamera.h"
#include "SiegeWarMgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleSystem::CBattleSystem()
{
	m_pCurBattle = &m_DefaultBattle;
	
	m_BattleFactory[eBATTLE_KIND_NONE] = NULL;
	m_BattleFactory[eBATTLE_KIND_VIMUSTREET] = new CBattleFactory_VimuStreet;
	m_BattleFactory[eBATTLE_KIND_MURIMFIELD] = new CBattleFactory_MurimField;
	m_BattleFactory[eBATTLE_KIND_MUNPAFIELD] = NULL;
	m_BattleFactory[eBATTLE_KIND_SURYUN] = new CBattleFactory_Suryun;
	m_BattleFactory[eBATTLE_KIND_GTOURNAMENT] = new CBattleFactory_GTournament;
	m_BattleFactory[eBATTLE_KIND_SIEGEWAR] = new CBattleFactory_SiegeWar;
}

CBattleSystem::~CBattleSystem()
{
	Release();
}

void CBattleSystem::Release()
{
	if(m_pCurBattle != &m_DefaultBattle)
	{
		DeleteBattle(m_pCurBattle);
	}
	
	for(int n=0;n<eBATTLE_KIND_MAX;++n)
	{
		if(m_BattleFactory[n])
		{
			delete m_BattleFactory[n];
			m_BattleFactory[n] = 0;
		}
	}
}

BOOL CBattleSystem::CreateBattle(BATTLE_INFO_BASE* pInfo)
{
	ASSERT(pInfo->BattleKind < eBATTLE_KIND_MAX);
	
	m_pCurBattle = m_BattleFactory[pInfo->BattleKind]->CreateBattle(pInfo, MAP->GetMapNum());
	if(m_pCurBattle == NULL)
		return FALSE;

	return TRUE;
}

void CBattleSystem::DeleteBattle(CBattle* pBattle)
{
	m_BattleFactory[pBattle->GetBattleKind()]->DeleteBattle(pBattle);
}

CBattle* CBattleSystem::GetBattle(CHero* pHero)
{
	return m_pCurBattle;
}

CBattle* CBattleSystem::GetBattle(DWORD BattleID)
{
	return m_pCurBattle;
}

void CBattleSystem::Render()
{
	m_pCurBattle->Render();
	if(m_pCurBattle->IsDestroyed() == TRUE)
	{
		ASSERT(m_pCurBattle != &m_DefaultBattle);
		m_pCurBattle->OnDestroy();
		DeleteBattle(m_pCurBattle);
		m_pCurBattle = &m_DefaultBattle;
	}
}

void CBattleSystem::ReleaseBattle()
{
	if(m_pCurBattle != &m_DefaultBattle)
	{
		m_pCurBattle->OnDestroy();
		DeleteBattle(m_pCurBattle);
		m_pCurBattle = &m_DefaultBattle;
	}
}

void CBattleSystem::SetSiegeWarBattleInfo( SEND_SW_BTGUILDLIST* pInfo )
{
	if( !pInfo || !m_pCurBattle || m_pCurBattle->GetBattleKind() != eBATTLE_KIND_SIEGEWAR )		return;

	((CBattle_SiegeWar*)m_pCurBattle)->SetBattleInfo( pInfo );
}


void CBattleSystem::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	VIMUMGR->NetworkMsgParse( Protocol, pMsg );

	switch(Protocol)
	{
	case MP_BATTLE_INFO:
		{
			MSG_BATTLE_INFO* pmsg = (MSG_BATTLE_INFO*)pMsg;
			CreateBattle((BATTLE_INFO_BASE*)pmsg->info);
		}
		break;
	case MP_BATTLE_START_NOTIFY:
		{
			m_pCurBattle->StartBattle();
		}
		break;
		
	case MP_BATTLE_TEAMMEMBER_ADD_NOTIFY:
		{
			MSG_BATTLE_TEAMMEMBER_ADDDELETE* pmsg = (MSG_BATTLE_TEAMMEMBER_ADDDELETE*)pMsg;
			m_pCurBattle->OnTeamMemberAdd(pmsg->Team,pmsg->Member.MemberID,pmsg->Member.MemberName);

			// observer의 경우 처리 ( 안보이게 : 맞나? )
/*			if( pmsg->Team == eBattleTeam_Observer )
			{
				CObject* pObjcet = OBJECTMGR->GetObject( pmsg->Member.MemberID );
				if( pObjcet->GetObjectKind() != eObjectKind_Player )
					return;
				CPlayer* pPlayer = (CPlayer*)pObjcet;
				pPlayer->GetEngineObject()->HideWithScheduling();
				pPlayer->GetCharacterTotalInfo()->bVisible = false;
			}		
*/
			//-----------------------------------
		}
		break;
	case MP_BATTLE_TEAMMEMBER_DELETE_NOTIFY:
		{
			MSG_BATTLE_TEAMMEMBER_ADDDELETE* pmsg = (MSG_BATTLE_TEAMMEMBER_ADDDELETE*)pMsg;
			m_pCurBattle->OnTeamMemberDelete(pmsg->Team,pmsg->Member.MemberID,pmsg->Member.MemberName);
		}
		break;		
	case MP_BATTLE_TEAMMEMBER_DIE_NOTIFY:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			m_pCurBattle->OnTeamMemberDie(pmsg->dwData1,pmsg->dwData2,pmsg->dwData3);
		}
		break;
	case MP_BATTLE_BATTLEOBJECT_DESTROY_NOTIFY:
		{			
		}
		break;
	case MP_BATTLE_BATTLEOBJECT_CREATE_NOTIFY:
		{			
		}
		break;
	case MP_BATTLE_VICTORY_NOTIFY:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			m_pCurBattle->Victory(pmsg->dwData1,pmsg->dwData2);
		}
		break;
	case MP_BATTLE_DRAW_NOTIFY:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			m_pCurBattle->Draw();
		}
		break;
	case MP_BATTLE_DESTROY_NOTIFY:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			m_pCurBattle->SetDestroyFlag();
		}
		break;
	case MP_BATTLE_CHANGE_OBJECTBATTLE:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(pObject == NULL)
				return;

			
			
			//battle Kind
			if( pmsg->dwData3 == eBATTLE_KIND_VIMUSTREET || pmsg->dwData3 == eBATTLE_KIND_GTOURNAMENT )
			{
				if( pmsg->dwData2 == eBattleTeam1 )			//battle Team
				{
					if( ((CPlayer*)pObject)->InTitan() == TRUE )
					{
						OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_A_s.beff"));	//SW HERO는 옵션 설정 무관하게 이펙트 생성을 위한..설정
						pObject->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
					}
					else
					{
						OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_A_s.beff"));	//SW HERO는 옵션 설정 무관하게 이펙트 생성을 위한..설정
						pObject->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
					}
					((CPlayer*)pObject)->GetCharacterTotalInfo()->bVisible = true;
				}
				else if( pmsg->dwData2 == eBattleTeam2 )
				{
					if( ((CPlayer*)pObject)->InTitan() == TRUE )
					{
						OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_B_s.beff"));
						pObject->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
					}
					else
					{
						OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_B_s.beff"));
						pObject->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
					}
					((CPlayer*)pObject)->GetCharacterTotalInfo()->bVisible = true;
				}
				else if( pmsg->dwData2 == 2 )
				{
					/*if( pObject->GetObjectKind() == eObjectKind_Player )
					{
						((CPlayer*)pObject)->GetCharacterTotalInfo()->bVisible = false;
						if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM && CAMERA->GetCameraMode() != eCM_EyeView )
							pObject->GetEngineObject()->SetAlpha( 0.3f );
						else
							pObject->GetEngineObject()->HideWithScheduling();
					}*/
				}
			}
			else if( pmsg->dwData3 == eBATTLE_KIND_SIEGEWAR &&
				pObject->GetObjectKind() == eObjectKind_Player )
			{
				((CBattle_SiegeWar*)m_pCurBattle)->SetSiegeWarName( pObject, pmsg->dwData2, pmsg->dwData4 );
			}
			else	//default battle로
			{
				pObject->RemoveObjectEffect( BATTLE_TEAMEFFECT_ID );
				if( pObject == HERO )
					pObject->RemoveObjectEffect( BATTLE_STAGEEFFECT_ID );	//비무장지우기
			}

			pObject->SetBattle(pmsg->dwData1,(BYTE)pmsg->dwData2);
		}
		break;

// SiegeWar
	case MP_BATTLE_SIEGEWAR_BATTLEINFO:
		{
			SEND_SW_BTGUILDLIST* pmsg = (SEND_SW_BTGUILDLIST*)pMsg;

			HERO->SetBattleTeam( pmsg->Team );
			if( m_pCurBattle && m_pCurBattle->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )			
				((CBattle_SiegeWar*)m_pCurBattle)->SetBattleInfo( pmsg );
		}
		break;


///////VIMU

	case MP_BATTLE_VIMU_CREATESTAGE:
		{
			MSG_BATTLE_VIMU_CREATESTAGE* pmsg = (MSG_BATTLE_VIMU_CREATESTAGE*)pMsg;

			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(pObject == NULL)
				return;
			
			VECTOR3 vPos;
			pObject->GetPosition( &vPos );
			vPos.x = pmsg->vPosStage.x;
			vPos.z = pmsg->vPosStage.z;

			OBJECTEFFECTDESC desc( FindEffectNum("eff_bmu_side_s.beff"), EFFECT_FLAG_ABSOLUTEPOS, &vPos );
			pObject->AddObjectEffect( BATTLE_STAGEEFFECT_ID, &desc, 1 );
		}
		break;

	case MP_BATTLE_VIMU_START:	//캐릭터의 비무시작
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData);
			if( pObject == NULL )
				break;
		}
		break;
	case MP_BATTLE_VIMU_END: //캐릭터의 비무끝
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData);
			if( pObject == NULL )
				break;
		}
		break;
	}
}


#endif