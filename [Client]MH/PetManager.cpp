#include "stdafx.h"
#include ".\petmanager.h"
#include "Pet.h"
#include "GameIn.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "PetSpeechManager.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "CheatMsgParser.h"
#include "StatsCalcManager.h"

#include "WindowIdEnum.h"

#include "./Interface/cScriptManager.h"

#include "./interface/cDialog.h"
#include "ObjectGuagen.h"
#include "cStatic.h"
#include "PetStateDlg.h"
#include "PetStateMiniDlg.h"
#include "PetInventoryDlg.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"
#include "CharacterDialog.h"

#include "..\[CC]Header\CommonCalcFunc.h"

GLOBALTON(CPetManager)

CPetManager::CPetManager(void)
{
//	m_HeroOwnPetInfo.Initialize(5);
	m_PetInfoList.Initialize(10);
	m_PetImageList.Initialize(20);

	PetMgrInit();
}

CPetManager::~CPetManager(void)
{
}

void CPetManager::PetMgrInit()
{
	m_dwStateCheckTime		= gCurTime;
	m_dwStaminaDecrease		= 0;
	m_dwOldFriendShipForToolTipChange	= PET_DEFAULT_FRIENDLY;

	m_dwDistCheckTime		= gCurTime;
	m_bValidDistance		= FALSE;
	m_bReadytoMove			= FALSE;

	m_dwSkillRechargeCheckTime	= gCurTime;
	m_dwSkillRechargeAmount		= 0;
	m_bSkillGuageFull		= FALSE;
	m_bReadyToSendSkillMsg	= FALSE;

	m_dwResummonDelayTime	= 0;

	m_pCurSummonPet			= NULL;
	m_bIsPetStateDlgToggle	= FALSE;
	m_pCurPetStateDlg		= NULL;
	m_pStateDlg				= NULL;	
	m_pStateMiniDlg			= NULL;
	m_pInvenDlg				= NULL;

	m_bReadyToSendRestMsg	= TRUE;
}

void CPetManager::PetMgrRelease()
{
//	ReleasePetTable();
	ReleasePetInfoList();
	PetMgrInit();
	RemovePetImage();
}

// void CPetManager::AddHeroPet(CPet* pPet)
// {
// 	m_HeroOwnPetInfo.Add(pPet, pPet->GetID());
// }

void CPetManager::InitPetInfo( PET_TOTALINFO* pPetInfo, int num )
{
	for( int i = 0; i < num; ++i )
	{
		AddPetInfo(&pPetInfo[i]);
	}
}

void CPetManager::AddPetInfo( PET_TOTALINFO* pPetInfo )
{
	PET_TOTALINFO* pInfo = m_PetInfoList.GetData(pPetInfo->PetSummonItemDBIdx);
	if( pInfo )	//재교환시
	{
		memcpy(pInfo, pPetInfo, sizeof(PET_TOTALINFO));
	}
	else
	{
		PET_TOTALINFO* pNewInfo = new PET_TOTALINFO;
		memcpy(pNewInfo, pPetInfo, sizeof(PET_TOTALINFO));
		m_PetInfoList.Add(pNewInfo,pNewInfo->PetSummonItemDBIdx);
	}
}

void CPetManager::RemovePetInfo( DWORD dwSummonItemDBIdx )
{
	PET_TOTALINFO* pInfo = m_PetInfoList.GetData(dwSummonItemDBIdx);

	if( !pInfo )	return;

	SAFE_DELETE(pInfo);

	m_PetInfoList.Remove(dwSummonItemDBIdx);
}

void CPetManager::ReleasePetInfoList()
{
	PET_TOTALINFO* pInfo = NULL;
	m_PetInfoList.SetPositionHead();

	while( pInfo = m_PetInfoList.GetData() )
	{
		SAFE_DELETE(pInfo);
	}
	m_PetInfoList.RemoveAll();
}

PET_TOTALINFO* CPetManager::GetPetInfo(DWORD dwItemDBIdx)
{
	PET_TOTALINFO* pInfo = NULL;

	pInfo = m_PetInfoList.GetData(dwItemDBIdx);

	return pInfo;
}

void CPetManager::UpdateCurPetInfo( CPet* pPet )
{
	DWORD ItemDBIdx = pPet->m_PetTotalInfo.PetSummonItemDBIdx;

	PET_TOTALINFO* pInfo = GetPetInfo(ItemDBIdx);

	if(pInfo)
		memcpy(pInfo, &pPet->m_PetTotalInfo, sizeof(PET_TOTALINFO));
}

void CPetManager::OnPetRemove(CPet* pPet)
{
	if(!pPet->GetMaster())	return;
	else
	{
		pPet->GetMaster()->SetPet(NULL);
	}

	if(HEROID != pPet->GetMaster()->GetID())
	{
		pPet->SetMaster(NULL);
		return;
	}
	//if(HEROID == pPet->GetMaster()->GetID())
	else
	{
		if(!m_pCurSummonPet)	return;

		if(!CheckCurSummonPetKindIs(ePK_EventPet))
		if(TRUE == CheckPetAlive(m_pCurSummonPet->m_PetTotalInfo.PetSummonItemDBIdx))
			UpdateCurPetInfo(pPet);

		//해당아이템 툴팁 다시 찍기
		//DWORD ItemDBIdx = pPet->m_PetTotalInfo.PetSummonItemDBIdx;
		//ITEMMGR->RefreshItemToolTip(ItemDBIdx);

		pPet->SetMaster(NULL);

		m_pCurSummonPet = NULL;

		//펫 지속버프 관련 추가 스탯 갱신
		RefleshPetMaintainBuff();
	}
}

void CPetManager::SetResummonCheckTime()
{
	m_dwResummonDelayTime = gCurTime;
}

BOOL CPetManager::CheckResummonAvailable()
{
#ifdef _CHEATENABLE_
	if(CHEATMGR->IsCheatEnable())
		return TRUE;
#endif
	return ( gCurTime-m_dwResummonDelayTime >= GAMERESRCMNGR->GetPetRule()->ResummonVilidTime );
}

DWORD CPetManager::GetPetResummonRestTime()
{
	return ( GAMERESRCMNGR->GetPetRule()->ResummonVilidTime - (gCurTime - m_dwResummonDelayTime) );
}


BOOL CPetManager::CheckDefaultFriendship(DWORD dwItemDBIdx)
{
	PET_TOTALINFO* pInfo = GetPetInfo(dwItemDBIdx);

	if( pInfo )
	{
		if( pInfo->PetFriendly < GAMERESRCMNGR->GetPetRule()->DefaultFriendship )
			return FALSE;
		else
			return TRUE;
	}
	return FALSE;
}

DWORD CPetManager::GetPetFriendship(DWORD dwItemDBIdx)
{
	PET_TOTALINFO* pInfo = GetPetInfo(dwItemDBIdx);

	if( !pInfo )
	{
		ASSERTMSG(0, "SummonItem have No PetInfo!");
		return 0;
	}

	return pInfo->PetFriendly;
}

BOOL CPetManager::IsCurPetStaminaFull()
{
	if(m_pCurSummonPet)
	{
		return m_pCurSummonPet->IsPetStaminaFull();
	}
	else
	{
		return FALSE;
	}
}

BOOL CPetManager::IsCurPetSummonItem( DWORD dwItemDBIdx )
{
	if(!m_pCurSummonPet)	return FALSE;

	if(m_pCurSummonPet->m_PetTotalInfo.PetSummonItemDBIdx == dwItemDBIdx)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// void CPetManager::RemovePetFromTable( DWORD dwPetID )
// {//
// 	m_HeroOwnPetInfo.Remove(dwPetID);
// }

// CPet* CPetManager::GetHeroPet(DWORD dwPetID)
// {
// 	return m_HeroOwnPetInfo.GetData(dwPetID);
// }

DWORD CPetManager::GetHeroPetID()
{
	if(GetCurSummonPet())
	{
		return GetCurSummonPet()->GetID();
	}
	else
	{
		return 0;
	}
}

BOOL CPetManager::CheckPetSummoned()
{
	if(!GetCurSummonPet())
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1240) );
		return FALSE;
	}
	else
		return TRUE;
}

BOOL CPetManager::CheckPetAlive( DWORD dwSummonItemDBIdx )
{
	PET_TOTALINFO* pInfo = GetPetInfo(dwSummonItemDBIdx);
	if(!pInfo)
	{
		ASSERT(0);
		return FALSE;
	}

	return pInfo->bAlive;
}

void CPetManager::NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	switch( Protocol )
	{
	case MP_PET_VALUEINFO:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			//!!!임시
			if(!m_pCurSummonPet) return;
			if(pmsg->dwData4 != m_pCurSummonPet->GetID())
			{
				ASSERT(0);
				return;
			}

			if(FALSE == CheckPetAlive(m_pCurSummonPet->m_PetTotalInfo.PetSummonItemDBIdx))
				return;

			m_pCurSummonPet->m_PetTotalInfo.PetStamina = pmsg->dwData1;
			m_pCurSummonPet->m_PetTotalInfo.PetFriendly = pmsg->dwData2;

			static Stamina = 0;

			if( Stamina && (0==pmsg->dwData1) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1264));
			}
			//툴팁 갱신	//친밀도 갱신
			DWORD Friendship = pmsg->dwData2 / 1000;
			if( m_dwOldFriendShipForToolTipChange != Friendship )
			{
				UpdateCurPetInfo(m_pCurSummonPet);

				ITEMMGR->RefreshItemToolTip(m_pCurSummonPet->m_PetTotalInfo.PetSummonItemDBIdx);
				m_dwOldFriendShipForToolTipChange = Friendship;
			}

			Stamina = pmsg->dwData1;
			
			SetPetGuageText( pmsg->dwData1, pmsg->dwData2 );
		}
		break;
	case MP_PET_STATEINFO:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if( NULL == pPlayer )	return;

			CPet* pPet = pPlayer->GetPet();
			if( NULL == pPet )	return;

			CPet* pMPet = (CPet*)OBJECTMGR->GetObject(pmsg->dwData1);			
			if( NULL == pMPet )	return;

			if(pPet->GetID() != pmsg->dwData1)
			{
				ASSERTMSG(0,"Error! : Lost Pet");
			}
//			if(pmsg->dwData1 != m_pCurSummonPet->GetID())
//			{
//				ASSERT(0);
//				return;
//			}

			if(pmsg->dwData2)
			{
				if(ePM_SKILL != pmsg->dwData2 || ePM_STAMINA_ZERO != pmsg->dwData2 )	//스태미나 0일 때 모션없음.
				{
					//pPet->m_ObjectState.State_Start_Motion = pmsg->dwData2;
					pPet->ChangeMotion(pmsg->dwData2, FALSE);
				}
			}
			if(pmsg->dwData3)
			{
				//to do
				char* temp = PET_SPEECHMGR->GetPetSpeech(pmsg->dwData3);
				if( temp )
				{
					//CPet* pPet = (CPet*)OBJECTMGR->GetObject(pmsg->dwData1);
					//if( !pPet ) return;
					OBJECTMGR->ShowChatBalloon( pPet, temp );
				}
			}

		}
		break;
	case MP_PET_REST_ON_ACK:
		{
			if(!m_pCurSummonPet)	return;
			m_pCurSummonPet->SetPetRest(TRUE);
			SetReadyToSendRestMsg(TRUE);
			//!!!버튼 설정 확인 (단축키로 휴식설정이 가능)
			//m_pStateDlg->SetBtnClick(ePetRestBtn);
			SetPetStateDlgUseRestInfo(m_pCurSummonPet);

			//펫 지속버프 관련 추가 스탯 갱신
			RefleshPetMaintainBuff();
		}
		break;
	case MP_PET_REST_OFF_ACK:
		{
			if(!m_pCurSummonPet)	return;
			m_pCurSummonPet->SetPetRest(FALSE);
			SetReadyToSendRestMsg(TRUE);
			//m_pStateDlg->SetBtnClick(ePetUseBtn);
			SetPetStateDlgUseRestInfo(m_pCurSummonPet);

			//펫 지속버프 관련 추가 스탯 갱신
			RefleshPetMaintainBuff();
		}
		break;
	case MP_PET_REST_ON_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1272) );
			SetReadyToSendRestMsg(TRUE);
		}
		break;
	case MP_PET_REST_NACK:
		{
			//sync error
			ASSERT(0);
			SetReadyToSendRestMsg(TRUE);
		}
		break;
	case MP_PET_SUMMON_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPet* pPet = (CPet*)OBJECTMGR->GetObject(pmsg->dwData);
			if(!pPet)	return;

			TARGETSET set;
			set.pTarget = pPet;
			EFFECTMGR->StartEffectProcess(eEffect_PetSummon,pPet,&set,0,pPet->GetID());

			if(pPet->GetMaster())
			{
				if(pPet->GetMaster()->GetID() == HEROID)
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1259) );
			}
		}
		break;
	case MP_PET_SEAL_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPet* pPet = (CPet*)OBJECTMGR->GetObject(pmsg->dwData);
			if(!pPet)	return;

			TARGETSET set;
			set.pTarget = pPet;
			EFFECTMGR->StartEffectProcess(eEffect_PetSeal,pPet,&set,0,pPet->GetID());

			if(HEROID == pmsg->dwObjectID)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1260) );
				//펫 정보창 게이지 초기화
				SetPetGuageText(0,0);

				if(!CheckCurSummonPetKindIs(ePK_EventPet))
					SetResummonCheckTime();

				ClosePetAllDlg();
			//SetCurSummonPet(NULL);
				//HERO->SetPet(NULL); //GAMEIN removeobject에서 해주자.
				
			}
		}
		break;
	case MP_PET_USE_SKILL_ACK:
		{
			//스킬 게이지 초기화 & 스킬 준비 상태 초기화
			//m_dwSkillRechargeAmount = 0;
			SetSkillRechargeAmount(0);
			m_bSkillGuageFull		= FALSE;
			//메시지 유효화
			SetReadyToSendSkillMsg(TRUE);
		}
		break;
	case MP_PET_USE_SKILL_NACK:
		{
			//메시지 유효화
			SetReadyToSendSkillMsg(TRUE);
		}
		break;
	case MP_PET_ADDNEW_FROMITEM:
		{
			SEND_ADDPET_FROMITEM* pmsg = (SEND_ADDPET_FROMITEM*)pMsg;
			AddPetInfo(&pmsg->Info);
			ITEMMGR->RefreshItemToolTip(pmsg->Info.PetSummonItemDBIdx);
		}
		break;
	case MP_PET_UPDATE_FRIENDSHIP:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*) pMsg;
			PET_TOTALINFO* pInfo = GetPetInfo(pmsg->dwData1);
			pInfo->PetFriendly = pmsg->dwData2;

			//ITEMMGR->RefreshItemToolTip(pInfo->PetSummonItemDBIdx);
		}
		break;
	case MP_PET_UPGRADE_ACK:
		{
			GAMEIN->GetPetUpgradeDialog()->PetUpgradeAck();
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1273) );
		}
		break;
	case MP_PET_UPGRADE_NACK:
		{
			GAMEIN->GetPetUpgradeDialog()->PetUpgradeNack();
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1274) );
		}
		break;
	case MP_PET_REVIVAL_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			//pmsg->dwData 0은 아이템몰부활, 1,2,3은 게임내 단계별
			GAMEIN->GetPetRevivalDialog()->PetRevivalAck(pmsg->dwData);
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1262) );
			//펫 스킬게이지 초기화
			//펫 휴식상태
// 			m_dwSkillRechargeAmount = 0;
// 			m_bSkillGuageFull = FALSE;
// 			SetReadyToSendSkillMsg(FALSE);
		}
		break;
	case MP_PET_REVIVAL_NACK:
		{
			GAMEIN->GetPetRevivalDialog()->PetRevivalNack();
		}
		break;
// 	case MP_PET_FEED_FAILED:
// 		{
// 			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
// 			switch(pmsg->bData)
// 			{
// 			case ePFR_Unsummoned:
// 				{
// 					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1253) );
// 				}
// 				break;
// 			case ePFR_StaminaFull:
// 				{
// 				}
// 				break;
// 			default:
// 				break;
// 			}
// 		}
// 		break;
	case MP_PET_SUMMON_RESULT:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1255) );
		}
		break;
	case MP_PET_MAXSTAMINA_NOTIFY:
		{
			//스태미나 최대값 변경.
			if(!m_pCurSummonPet)	return;
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			m_pCurSummonPet->SetMaxStamina(pmsg->dwData);
		}
		break;
	case MP_PET_ADDINFO:
		{
			SEND_ADDPET_FROMITEM* pmsg = (SEND_ADDPET_FROMITEM*)pMsg;
			AddPetInfo(&pmsg->Info);		
		}
		break;
	default:
		break;
	}
}

void CPetManager::PetMgrProcess()
{
	if( m_pCurSummonPet == NULL ) return;
	CheckDistWithMaster();

	//m_pCurSummonPet->CalcStamina();
	//CalcSkillRecharge();
}

void CPetManager::CalcSkillRecharge()
{
	//펫인포리스트에는 펫 충전 속도를 가지고 있다.
	//최대치는 고정
	//특정 시간 간격 마다 속도만큼 더해준다.
	//바로 게이지에 보여준다.
	if(!m_pCurSummonPet)	return;
	//!!!휴식 상태면 홀딩.
	if(m_pCurSummonPet->IsPetRest())
	{
		//m_dwSkillRechargeCheckTime = gCurTime;
		return;
	}

    if( gCurTime - m_dwSkillRechargeCheckTime < PET_SKILLCHARGE_CHECKTIME ) return;

	if( 0 != m_pCurSummonPet->m_PetTotalInfo.PetStamina )
	{	
		m_dwSkillRechargeAmount += m_pCurSummonPet->m_pSInfo->SkillRecharge;
	}
	else
	{
		m_dwSkillRechargeAmount = 0;
	}

	if( m_dwSkillRechargeAmount > PET_MAX_SKILL_CHARGE )
	{
		m_dwSkillRechargeAmount = PET_MAX_SKILL_CHARGE;
		m_bSkillGuageFull = TRUE;
		SetReadyToSendSkillMsg(TRUE);
	}

	SetSkillRechargeAmount(m_dwSkillRechargeAmount);

//	float SkillCharge = (float)m_dwSkillRechargeAmount / (float)PET_MAX_SKILL_CHARGE;
//	((CObjectGuagen*)m_pStateDlg->GetWindowForID(PS_GUAGE3))->SetValue( SkillCharge, 0 );
//	((CObjectGuagen*)m_pStateMiniDlg->GetWindowForID(PSMN_GUAGE3))->SetValue( SkillCharge, 0 );

	m_dwSkillRechargeCheckTime = gCurTime;
}
void CPetManager::SetSkillRechargeAmount( DWORD amount )
{
	m_dwSkillRechargeAmount = amount;

	float SkillCharge = 0.f;
	DWORD effectTime = 0;

	if(amount)
	{
		SkillCharge = (float)m_dwSkillRechargeAmount / (float)PET_MAX_SKILL_CHARGE;
		effectTime = 1600*amount/PET_MAX_SKILL_CHARGE;
	}
	
	//((CObjectGuagen*)m_pStateDlg->GetWindowForID(PS_GUAGE3))->SetValue( SkillCharge, 0 );
	//((CObjectGuagen*)m_pStateMiniDlg->GetWindowForID(PSMN_GUAGE3))->SetValue( SkillCharge, 0 );
//	((CObjectGuagen*)m_pStateDlg->GetWindowForID(PS_GUAGE3))->SetValue( SkillCharge, effectTime );
//	((CObjectGuagen*)m_pStateMiniDlg->GetWindowForID(PSMN_GUAGE3))->SetValue( SkillCharge, effectTime );
}
void CPetManager::CheckDistWithMaster()
{
//	if( !m_pCurSummonPet ) return;
	//1초마다 히어로의 현재 위치를 팻 위치와 비교하고
	//유효 거리 이상이면
	//히어로 노드로 부터 펫 노드를 구해
	//서버로 정보 보내고
	//펫 이동시작한다.
	if( gCurTime - m_dwDistCheckTime < PET_DIST_CHECKTIME ) return;

	float dist = CalcDistanceXZ(&m_pCurSummonPet->GetCurPosition(), &HERO->GetCurPosition());

	/*
	if( MAX_LIMIT_PET_DIST < dist )
	{
		//워프메시지. 서버에서 순간이동 처리.
		MOVE_POS	msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_PET_WARP_SYN;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = m_pCurSummonPet->GetID();
		MOVE_INFO* pMoveInfo = HERO->GetBaseMoveInfo();
		msg.cpos.Compress(&pMoveInfo->CurPosition);
		NETWORK->Send(&msg,sizeof(msg));

		//SetPosition(pPet,&pHeroMoveInfo->CurPosition);
	}*/

	m_bValidDistance = (DEFAULT_PET_FOLLOW_DIST < dist);

	if( (m_bValidDistance || MOVEMGR->IsMoving(m_pCurSummonPet)) && m_bReadytoMove )
	{
		MOVEMGR->SendPetMoveMsg();
	}

	m_dwDistCheckTime = gCurTime;
}

void CPetManager::OpenPetStateDlg()
{	//맵이동시에도 먼저 선택한 상태창 유지.

	//펫 소환중 아니면 리턴 & 메시지
	if(!CheckPetSummoned())
		return;

	if(m_pCurPetStateDlg && m_pCurPetStateDlg->IsActive())
		return;

	/*
	if(!m_bIsPetStateDlgToggle)
	{
		m_pCurPetStateDlg = (cDialog*)m_pStateDlg;
// 		if(!m_pStateDlg->IsActive())
// 			m_pStateDlg->SetActive(TRUE);
		m_pStateDlg->SetActive(!m_pStateDlg->IsActive());
	}
	else
	{
// 		if(!m_pStateMiniDlg->IsActive())
// 			m_pStateMiniDlg->SetActive(TRUE);
		m_pStateMiniDlg->SetActive(!m_pStateMiniDlg->IsActive());
	}*/
	//060310 수정
	if(!m_bIsPetStateDlgToggle)
	{
		if(m_pStateMiniDlg)	m_pStateMiniDlg->SetActive(FALSE);
		m_pCurPetStateDlg = (cDialog*)m_pStateDlg;
	}
	else
	{
		if(m_pStateDlg)	m_pStateDlg->SetActive(FALSE);
		m_pCurPetStateDlg = (cDialog*)m_pStateMiniDlg;
	}

	if(m_pCurPetStateDlg)
		m_pCurPetStateDlg->SetActive(TRUE);
}

void CPetManager::OpenPetInvenDlg()
{
	if(!CheckPetSummoned())
		return;
	
	if(!m_pInvenDlg)	return;

	if(m_pInvenDlg->IsItemInfoInit())
	{//처음 여는 것이 아니면.
		if(m_pInvenDlg)
		{
			if(!m_pInvenDlg->IsActive())
			{
				SetPetValidInvenTab();
				m_pInvenDlg->SetActive(TRUE);
			}
			else
				m_pInvenDlg->SetActive(FALSE);
		}		
	}
	else//처음 여는 것이면
	{//맵으로 정보 요청 메시지
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_PETINVEN_INFO_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = GetHeroPetID();

		NETWORK->Send(&msg, sizeof(msg));
	}
}

/*
void CPetManager::SetPetDlgToggle( BOOL bVal )	//FALSE 가 큰창(기본)
{//그냥 쉽게 기본세팅(큰창)기준으로 토글을 한번 해줄 것인가..
	//아니면 현재창 지정을 해놓을 것인가..
	//큰창(기본)->작은창
	m_bIsPetStateDlgToggle = bVal;
}*/

void CPetManager::TogglePetStateDlg()	//현재창 활성화 상태에서 들어온다.
{
//	if(!CheckPetSummoned())
//		return;

	//ASSERT(m_pStateDlg);
	//ASSERT(m_pStateMiniDlg);
	if(!m_pStateDlg || !m_pStateMiniDlg)	return;

	LONG absX = LONG(m_pStateDlg->GetAbsX());
	LONG absY = LONG(m_pStateDlg->GetAbsY());

	if( m_pCurPetStateDlg )
	{
		absX = LONG(m_pCurPetStateDlg->GetAbsX());
		absY = LONG(m_pCurPetStateDlg->GetAbsY());

		m_pCurPetStateDlg->SetActive(FALSE);
	}
	
	if( !m_bIsPetStateDlgToggle )
	{
		m_pCurPetStateDlg = (cDialog*)m_pStateMiniDlg;
	}
	else
	{
		m_pCurPetStateDlg = (cDialog*)m_pStateDlg;
	}

	m_pCurPetStateDlg->SetAbsXY(absX,absY);
	m_pCurPetStateDlg->SetActive(TRUE);

	m_bIsPetStateDlgToggle = !m_bIsPetStateDlgToggle;

	return;
}

void CPetManager::SetPetStateDlgInfo( CPet* pPet )
{
	if(!pPet)	return;
	BASE_PET_LIST* pList = pPet->m_pSInfo;
	PET_TOTALINFO* pInfo = &pPet->m_PetTotalInfo;
	if(!pList||!pInfo)	return;

	//상태창 펫이름과 단계 표시하기
	if(!m_pStateDlg)	return;
	
	cStatic* pName = (cStatic*)m_pStateDlg->GetNameTextWin();
	cStatic* pNameMini = (cStatic*)m_pStateMiniDlg->GetNameTextWin();
	if( pName && pNameMini )
	{
		pName->SetStaticText( pList->Name );
		pNameMini->SetStaticText( pList->Name );
	}

	cStatic* pGrade = (cStatic*)m_pStateDlg->GetGradeTextWin();
	if( pGrade )
	{
		char szValue[32];
		sprintf(szValue, "%d", pInfo->PetGrade);
		pGrade->SetStaticText( szValue );
	}

	cStatic* p2DImage = (cStatic*)m_pStateDlg->Get2DImageWin();
	if( p2DImage )
	{
		p2DImage->SetBasicImage(GetPetImage(pList->PetKind));
	}

	//펫 스킬 정보
	if(pList->BuffList)
	{
		PET_BUFF_LIST* pBuffList = NULL;
		char BuffName[32] = {0,};

		for( int i = 0; i < MAX_PET_BUFF_NUM; ++i )
		{
			cStatic** pText = (cStatic**)m_pStateDlg->GetPetBuffTextWin();

			WORD BuffIdx = pList->BuffList[pInfo->PetGrade-1][i];
			if(0 == BuffIdx)
			{
				if(pText[i])
				{
					pText[i]->SetStaticText("");
				}
				continue;
			}

			pBuffList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);

			if(pBuffList && pBuffList->BuffValueData)
			{
				if( pBuffList->BuffKind == ePB_Demage_Percent )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1270), pBuffList->BuffValueData );
				}
				else if( pBuffList->BuffKind == ePB_Dodge )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1271), pBuffList->BuffSuccessProb );
				}
				else if( pBuffList->BuffKind == ePB_MasterAllStatUp )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1286), pBuffList->BuffValueData );
				}
				else if( pBuffList->BuffKind == ePB_Item_DoubleChance )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1305), pBuffList->BuffSuccessProb );
				}
				else if( pBuffList->BuffKind == ePB_NoForeAtkMonster )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1350) );
				}
				else if( pBuffList->BuffKind == ePB_ReduceCriticalDmg )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1388), pBuffList->BuffValueData );
				}
				else if( pBuffList->BuffKind == ePB_MasterAllStatRound )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1389));
				}
				else if( pBuffList->BuffKind == ePB_Item_RareProbUp )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1476) );
				}
				else if( pBuffList->BuffKind == ePB_MussangTimeIncrease )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1477) );
				}
			}

			if(pText[i])
			{
				pText[i]->SetStaticText(BuffName);
			}
		}
	}
}

void CPetManager::SetPetStateDlgUseRestInfo( CPet* pPet )
{
	if( !pPet )	return;
	cStatic* pState = m_pStateDlg->GetUseRestTextWin();
	cStatic* pStateMini = m_pStateMiniDlg->GetUseRestTextWin();
	if(pState && pStateMini)
	{
		if(pPet->IsPetRest())
		{
			pState->SetStaticText(CHATMGR->GetChatMsg(1257));
			pStateMini->SetStaticText(CHATMGR->GetChatMsg(1257));
		}
		else
		{
			pState->SetStaticText(CHATMGR->GetChatMsg(1256));
			pStateMini->SetStaticText(CHATMGR->GetChatMsg(1256));
		}
	}
}

void CPetManager::ClosePetAllDlg()
{
	m_pStateDlg->SetActive(FALSE);
	m_pStateMiniDlg->SetActive(FALSE);
	m_pInvenDlg->SetActive(FALSE);
}

void CPetManager::SetPetGuageText( DWORD dwStamina, DWORD dwFriendShip )
{
	if(!m_pCurSummonPet)	return;

	char szValue[32];
	
	if(0 < dwFriendShip && dwFriendShip <= 100000)
	{
		sprintf(szValue, "%d%%", 1);
	}
	else
	{
		sprintf(szValue, "%d%%", dwFriendShip/100000);
	}
#ifdef _CHEATENABLE_
	if(CHEATMGR->IsCheatEnable())
	{
		sprintf(szValue, "%.3f", (float)dwFriendShip/100000);
	}
#endif
	((cStatic*)m_pStateDlg->GetFriendShipTextWin())->SetStaticText(szValue);
	((cStatic*)m_pStateMiniDlg->GetFriendShipTextWin())->SetStaticText(szValue);

	sprintf(szValue, "%d / %d", dwStamina, m_pCurSummonPet->GetMaxStamina());
	((cStatic*)m_pStateDlg->GetStaminaTextWin())->SetStaticText(szValue);
	((cStatic*)m_pStateMiniDlg->GetStaminaTextWin())->SetStaticText(szValue);

	float stamina = float(m_pCurSummonPet->m_PetTotalInfo.PetStamina)
		/ float(m_pCurSummonPet->GetMaxStamina());
	float friendShip = float(m_pCurSummonPet->m_PetTotalInfo.PetFriendly)
		/ float(GAMERESRCMNGR->GetPetRule()->MaxFriendship);

	((CObjectGuagen*)m_pStateDlg->GetFriendShipGuage())->SetValue( friendShip, 0 );
	((CObjectGuagen*)m_pStateMiniDlg->GetFriendShipGuage())->SetValue( friendShip, 0 );
	((CObjectGuagen*)m_pStateDlg->GetStaminaGuage())->SetValue( stamina, 0 );
	((CObjectGuagen*)m_pStateMiniDlg->GetStaminaGuage())->SetValue( stamina, 0 );

/*
	char szValue[32];
	cStatic* pFriendShipText = (cStatic*)m_pStateDlg->GetWindowForID(PS_GUAGETEXT1);
	cStatic* pFriendShipTextMini = (cStatic*)m_pStateMiniDlg->GetWindowForID(PSMN_GUAGETEXT1);
	if(pFriendShipText && pFriendShipTextMini)
	{
		sprintf(szValue, "%d%%", dwFriendShip/1000);
#ifdef _CHEATENABLE_
		if(CHEATMGR->IsCheatEnable())
		{
			//sprintf(szValue, "%d.%d", pmsg->dwData2/100, pmsg->dwData2%100);
			sprintf(szValue, "%.3f", (float)dwFriendShip/1000);
		}
#endif
		pFriendShipText->SetStaticText(szValue);
		pFriendShipTextMini->SetStaticText(szValue);
	}

	cStatic* pStaminaText = (cStatic*)m_pStateDlg->GetWindowForID(PS_GUAGETEXT2);
	cStatic* pStaminaTextMini = (cStatic*)m_pStateMiniDlg->GetWindowForID(PSMN_GUAGETEXT2);
	if(pStaminaText && pStaminaTextMini)
	{
		sprintf(szValue, "%d / %d", dwStamina, m_pCurSummonPet->m_pSInfo->StaminaMax[m_pCurSummonPet->GetPetCurGrade()-1]);
		pStaminaText->SetStaticText(szValue);
		pStaminaTextMini->SetStaticText(szValue);
	}

	float stamina = float(m_pCurSummonPet->m_PetTotalInfo.PetStamina)
		/ float(m_pCurSummonPet->m_pSInfo->StaminaMax[m_pCurSummonPet->GetPetGrade()-1]);
	float friendShip = float(m_pCurSummonPet->m_PetTotalInfo.PetFriendly)
		/ float(PET_MAX_FRIENDSHIP);

	((CObjectGuagen*)m_pStateDlg->GetWindowForID(PS_GUAGE2))->SetValue( stamina, 0 );
	((CObjectGuagen*)m_pStateDlg->GetWindowForID(PS_GUAGE1))->SetValue( friendShip, 0 );
	((CObjectGuagen*)m_pStateMiniDlg->GetWindowForID(PSMN_GUAGE2))->SetValue( stamina, 0 );
	((CObjectGuagen*)m_pStateMiniDlg->GetWindowForID(PSMN_GUAGE1))->SetValue( friendShip, 0 );
*/
}

void CPetManager::SetPetStateMiniDlgInfo( CPet* pPet )
{
	if(!pPet)	return;
	
	cStatic* pInvenNum = (cStatic*)m_pStateDlg->GetInvenNumTextWin();

	PET_TOTALINFO* pInfo = &pPet->m_PetTotalInfo;
	BASE_PET_LIST* pList = GAMERESRCMNGR->GetPetListInfo(pPet->GetPetIdx());
	//펫 유효 인벤토리 갯수
	if( pInvenNum )
	{
		char szValue[32];
		DWORD count = pList->InventoryTapNum[pInfo->PetGrade-1];

		if(count)
		{
			wsprintf( szValue, "%d", count);
		}
		else
		{
			wsprintf( szValue, "%s", CHATMGR->GetChatMsg(249));
		}

		pInvenNum->SetStaticText(szValue);
	}

	//펫 스킬 정보

	/*if(pList->BuffList)
	{
		PET_BUFF_LIST* pBuffList = NULL;
		char BuffName[32] = {0,};

		for( int i = 0; i < MAX_PET_BUFF_NUM; ++i )
		{
			cStatic** pText = (cStatic**)m_pStateDlg->GetPetBuffTextWin();

			WORD BuffIdx = pList->BuffList[pPetInfo->PetGrade-1][i];
			if(0 == BuffIdx)
			{
				if(pText[i])
				{
					pText[i]->SetStaticText("");
				}
				continue;
			}

			pBuffList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);

			if(pBuffList && pBuffList->BuffValueData)
			{
				if( pBuffList->BuffKind == ePB_Demage_Percent )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1270), pBuffList->BuffValueData );
				}
				else if( pBuffList->BuffKind == ePB_Dodge )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1271), pBuffList->BuffSuccessProb );
				}
			}

			if(pText[i])
			{
				pText[i]->SetStaticText(BuffName);
			}
		}
	}*/
}

DWORD CPetManager::GetPetValidInvenMaxTabNum()
{
	if(!m_pCurSummonPet)	return 0;

	BASE_PET_LIST* pPetList = NULL;

	pPetList = m_pCurSummonPet->GetSInfo();

	return pPetList->InventoryTapNum[m_pCurSummonPet->GetPetCurGrade()-1];
}

void CPetManager::SetPetValidInvenTab()
{
	if(!m_pInvenDlg)	return;

	DWORD ValidTabNum = GetPetValidInvenMaxTabNum();	//0 ~ 3

	//인벤창 탭 꺼주기 또는 탭 슬롯들 락해주기
	m_pInvenDlg->ApplyValidInvenTab(ValidTabNum);
}

void CPetManager::SetCurPetRest( BOOL bRest )
{
	if(!CheckPetSummoned())
	{
		ASSERT(0);
		return;
	}

	m_pCurSummonPet->SetPetRest(bRest);
	//m_pStateDlg->SetBtnClick(!bRest);
}

void CPetManager::SendPetRestMsg( BOOL bRest )//펫 사용/휴식 정보 보내기
{
	if(!CheckPetSummoned())
		return;

	if(!IsReadyToSendRestMsg())
		return;

	if( 0 == m_pCurSummonPet->m_PetTotalInfo.PetStamina )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1272) );
		return;
	}

	if(m_pCurSummonPet->IsPetRest() == bRest) return;

	//SetCurPetRest(bRest); //리턴받고 처리.

	MSGBASE msg;
	msg.Category = MP_PET;
	
//	if(m_pCurSummonPet->IsPetRest())
	if(bRest)
		msg.Protocol = MP_PET_REST_ON;
	else
		msg.Protocol = MP_PET_REST_OFF;

	msg.dwObjectID = HERO->GetID();
	NETWORK->Send( &msg, sizeof(msg) );

	SetReadyToSendRestMsg(FALSE);
}

void CPetManager::SendPetSealMsg()
{
	if(!CheckPetSummoned())
	{
		return;
	}

	MSGBASE msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_SEAL_SYN;
	msg.dwObjectID = HERO->GetID();
	NETWORK->Send( &msg, sizeof(msg) );
}

void CPetManager::CheckRestNSkillUse()	//무공창 스킬버튼 기능. 휴식상태면 해제시키고 게이지풀이면 스킬 사용. 기획자에게 문의후 알게됨.
{
	if(!CheckPetSummoned())
		return;

	if(m_pCurSummonPet->IsPetRest())
	{
		SendPetRestMsg(FALSE);
		UseCurPetSkill();
	}
	else
	{
		UseCurPetSkill();
	}
}

void CPetManager::InitPetSkillGuage()
{
	SetSkillRechargeAmount(0);
	SetSkillGuageFull(FALSE);
}

void CPetManager::UseCurPetSkill()
{
	if(!CheckPetSummoned())
		return;

	//스킬 게이지 풀 상태 체크
	if(!IsSkillGuageFull())
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1241) );
	}

	if(!IsReadyToSendSkillMsg())
		return;
//	MP_PET_USE_SKILL_SYN,
//	MP_PET_USE_SKILL_ACK,	//캐릭이 사용중인 스킬과 중복이 아니고, 펫 스킬 게이지가 풀 상태.
//	MP_PET_USE_SKILL_NACK,

	MSGBASE msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_USE_SKILL_SYN;
	msg.dwObjectID = HERO->GetID();
	NETWORK->Send( &msg, sizeof(msg) );

	SetReadyToSendSkillMsg(FALSE);
}

void CPetManager::AddMasterStatFromPetBuff( player_calc_stats* pMasterAdditionalStat )
{
	CPet* pCurPet = GetCurSummonPet();
	if(NULL == pCurPet)	return;

	if(pCurPet->IsPetRest())	return;

	BASE_PET_LIST* pInfo = pCurPet->GetSInfo();

	for( int i = 0; i < 3; ++i )
	{
		WORD BuffIdx = pInfo->BuffList[m_pCurSummonPet->GetPetCurGrade()-1][i];
		PET_BUFF_LIST* pList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);

		if(!pList)	continue;

		if(pList->BuffKind == ePB_MasterAllStatUp)
		{
			WORD AddStat = (WORD)pList->BuffValueData;
			pMasterAdditionalStat->GenGol += AddStat;
			pMasterAdditionalStat->MinChub += AddStat;
			pMasterAdditionalStat->CheRyuk += AddStat;
			pMasterAdditionalStat->SimMaek += AddStat;

		}

		//SW060906 신규펫 스탯 반올림
		if(pList->BuffKind == ePB_MasterAllStatRound)
		{
			pMasterAdditionalStat->GenGol += GetAddStatFromRound(HERO->GetGenGol());
			pMasterAdditionalStat->MinChub += GetAddStatFromRound(HERO->GetMinChub());
			pMasterAdditionalStat->CheRyuk += GetAddStatFromRound(HERO->GetCheRyuk());
			pMasterAdditionalStat->SimMaek += GetAddStatFromRound(HERO->GetSimMek());
		}
	}
}

void CPetManager::RefleshPetMaintainBuff()
{//펫 지속버프 관련 추가 스탯 갱신	//펫활성/휴식/소환/봉인시에 한다.	//소환,봉인시는 개념적이 아닌 물리적 추가/제거시. 
	STATSMGR->CalcItemStats( HERO );
	GAMEIN->GetCharacterDialog()->RefreshInfo();	
}

WORD CPetManager::GetPetEquipItemMax( DWORD dwItemDBIdx )
{//펫 장착 아이템 최대갯수
	PET_TOTALINFO* pInfo = GetPetInfo(dwItemDBIdx);

	if( !pInfo )	return 0;

	return pInfo->PetGrade;
}

void CPetManager::LoadPetImage()
{
	BASE_PET_LIST* pPetList = NULL;
	int count = 1;

	while(pPetList = GAMERESRCMNGR->GetPetListInfo(count))
	{
		cImage* pImage = new cImage;
		SCRIPTMGR->GetImage( pPetList->Pet2DIdx, pImage, PFT_ITEMPATH );
		ASSERT(!m_PetImageList.GetData(pPetList->PetKind));
		m_PetImageList.Add(pImage,pPetList->PetKind);
		++count;
	}
}

cImage* CPetManager::GetPetImage( DWORD dwPetIdx )
{
	return m_PetImageList.GetData(dwPetIdx);
}

void CPetManager::RemovePetImage()
{
	cImage* pImage = NULL;

	m_PetImageList.SetPositionHead();
	while(pImage = m_PetImageList.GetData())
	{
		delete pImage;
		pImage = NULL;
	}
	m_PetImageList.RemoveAll();
}

void CPetManager::SetCurSummonPetKind( CPet* pPet )
{
	WORD PetListIdx = pPet->GetPetKind();

	if( PetListIdx == CRISTMAS_EVENTPET )
	{
		m_wPetKind = ePK_EventPet;
	}
	else
	{
		m_wPetKind = ePK_None;
	}

}

BOOL CPetManager::CheckCurSummonPetKindIs( int kind )
{
	BOOL rt = (m_wPetKind & kind);

	return rt;
}
