#include "StdAfx.h"
#include "./Pet.h"
#include "./Player.h"
#include "Network.h"
#include "PackedData.h"
#include "../[CC]Header/GameResourceManager.h"
#include "SkillManager_server.h"
#include "CharMove.h"
#include "PetSpeechManager.h"
#include "MapDBMsgParser.h"

CPet::CPet(void)
{
	//memset(&m_PetTotalInfo,0,sizeof(PET_TOTALINFO));
	m_pMaster		= NULL;
	m_pBaseInfo		= NULL;
//	m_bSeal			= TRUE;		//초기값 봉인.
//	m_bRest			= TRUE;

	m_dwMaxStamina		= 0;

	m_dwInfoCheckTime	= gCurTime;
	m_CurBasicState		= ePBS_NONE;

	m_dwStateCheckTime	= gCurTime;
	m_dwRndGapTime		= 0;

	m_dwStaminaDecrease	= 0;
	m_wFrameCounter		= 0;
}

CPet::~CPet(void)
{
}

void CPet::DoDie(CObject* pAttacker)
{

}

//void CPet::OnStartObjectState(BYTE State,DWORD dwParam)
//{
//	//to do
//	switch(State)
//	{
//	case eObjectState_SkillUsing:
//		{
//
//		}
//		break;
//	case eObjectState_Ungijosik:
//		{
//
//		}
//		break;
//	case eObjectState_Die:
//		{
//			
//		}
//		break;
//	}
//	CObject::SetState(State);	//baseobjectinfo.. 이거 필요없을듯. 
//}

//void CPet::OnEndObjectState(BYTE State)
//{
//}

void CPet::Release()
{
	if(m_pMaster)
	{
		CPetManager* pPetMgr = m_pMaster->GetPetManager();
		pPetMgr->SetCurSummonPetNull();
	}

	m_pMaster = NULL;

	CObject::Release();
}

void CPet::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
{
	SEND_PET_TOTALINFO* pMsg = (SEND_PET_TOTALINFO*)pAddMsg;
	pMsg->AddableInfo.Init();

	pMsg->Category = MP_USERCONN;
	pMsg->Protocol = MP_USERCONN_PET_ADD;
	pMsg->dwObjectID = dwReceiverID;
	pMsg->MasterID = m_pMaster->GetID();
	GetSendMoveInfo(&pMsg->MoveInfo,&pMsg->AddableInfo);
	GetBaseObjectInfo(&pMsg->BaseObjectInfo);
	GetPetTotalInfoRt(&pMsg->TotalInfo);
	GetPetMasterNameRt(pMsg->MasterName);
	pMsg->bLogin = bLogin;

	*pMsgLen = pMsg->GetMsgLength();
}

float CPet::DoGetMoveSpeed()
{
	ASSERT(m_pMaster);
	if(!m_pMaster)	return 400;

	if(m_pMaster->GetKyungGongIdx())	//주인경공시
	{
		GetRandMotionNSpeech(ePM_KGONG);
	}
	return m_pMaster->DoGetMoveSpeed() * m_pBaseInfo->SpeedFromHero;
}

void CPet::GetPetTotalInfoRt(PET_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_PetTotalInfo, sizeof(PET_TOTALINFO));
}

void CPet::GetPetMasterNameRt(char* pRtMasterName)
{
	if(!m_pMaster)	return;
	SafeStrCpy(pRtMasterName, m_pMaster->GetObjectName(), MAX_NAME_LENGTH+1);
}

void CPet::InitPet(PET_TOTALINFO* pTotalinfo)
{
	memcpy(&m_PetTotalInfo, pTotalinfo, sizeof(PET_TOTALINFO));
	m_pBaseInfo = GAMERESRCMNGR->GetPetListInfo(GetPetKind());

	m_dwRndGapTime		= 10000; //10sec
//	m_bRest			= TRUE;
}

void CPet::SetPetStamina( DWORD dwStamina )
{
	if(m_pBaseInfo)
	{
		DWORD MaxStamina = m_pBaseInfo->StaminaMax[GetPetCurGrade()-1];
		if( MaxStamina < dwStamina )
		{
			m_PetTotalInfo.PetStamina = MaxStamina;
		}
		else
		{
			m_PetTotalInfo.PetStamina = dwStamina;
		}
	}

	SendPetInfoMsg();
}

void CPet::SetPetMaxStamina(DWORD val, BOOL bSendMsg/* = FALSE*/)
{
	m_dwMaxStamina = val;

	if(m_PetTotalInfo.PetStamina > val)
		m_PetTotalInfo.PetStamina = val;

	//!!!SEND_MSG to master.
	if(bSendMsg)
	{
		MSG_DWORD msg;
		msg.Category = MP_PET;
		msg.Protocol = MP_PET_MAXSTAMINA_NOTIFY;
		msg.dwObjectID = m_pMaster->GetID();
		msg.dwData = m_dwMaxStamina;

		m_pMaster->SendMsg(&msg, sizeof(msg));
	}
}

void CPet::SetPetFriendShip( DWORD dwFriendShip )
{//현재는 치트에 사용

	DWORD max_friendly = GAMERESRCMNGR->GetPetRule()->MaxFriendship;

	if( max_friendly < dwFriendShip )
		dwFriendShip = max_friendly;

	m_PetTotalInfo.PetFriendly = dwFriendShip;

	SendPetInfoMsg();
}

void CPet::PetGradeUp( WORD wPetLevel )
{
	if(m_PetTotalInfo.PetGrade<PET_MAX_GRADE)
	{m_PetTotalInfo.PetGrade = wPetLevel;}
}

void CPet::CheckPetMoving()
{
	float fMoveTime = ((gCurTime - m_MoveInfo.Move_StartTime)*0.001f);
	//예상 경과 시간보다 움직이기 시작한 시간이 더 클때...
	if(m_MoveInfo.Move_EstimateMoveTime < fMoveTime)
	{
		if( m_MoveInfo.GetCurTargetPosIdx() < m_MoveInfo.GetMaxTargetPosIdx() )
			m_MoveInfo.bMoving = TRUE;
		//return m_MoveInfo.bMoving = TRUE;
		else 
		{
			 m_MoveInfo.bMoving = FALSE;
		}
	}
	else
		m_MoveInfo.bMoving = TRUE;
}

//void CPet::CheckCurBasicState()
//{
//	if(m_pMaster->GetState() == eObjectState_Ungijosik)
//	{
//		m_CurBasicState = ePBS_UNGI;
//	}
//	else if(m_MoveInfo.bMoving)
//	{
//		m_CurBasicState = ePBS_MOVING;
//	}
//	else
//	{
//		m_CurBasicState = ePBS_NONE;
//	}
//}

void CPet::GetRandMotionNSpeech( DWORD state/*=0*/, BYTE frequency/*=ePMF_RANDOM*/ )
{//state 를 받아서 확률에 의해 해당 모션/대사 정보를 보낸다.
	//During Test!

	//if( state != ePM_MASTER_DIE && 
	//	state != ePM_DIE &&
	//	state != ePM_STAMINA_ZERO )
	if(frequency != ePMF_ALWAYS)
	{
		if(gCurTime - m_dwStateCheckTime < m_dwRndGapTime)	return;
	}

	if(state)
	{
		m_dwRndGapTime = random(5000,10000);
		//m_dwRndGapTime += random(5000,10000);
		//if(m_dwRndGapTime>60000)	m_dwRndGapTime = 60000;
	}
	else	//기본 상태들
	{
		m_dwRndGapTime = random(10000,30000);
	}

	m_dwStateCheckTime = gCurTime;

	
	if(frequency == ePMF_RANDOM)
	{
		DWORD RndValue = rand()%(5 - GetPetCurGrade());
		if(RndValue) return;
	}

// 	if(ePM_MASTER_DIE == state && 0 == m_PetTotalInfo.PetFriendly)
// 		state = ePM_DIE;

	/*//process로..
	if(m_CurBasicState != ePBS_UNGI)
	{
		if(IsPetMoving())
			m_CurBasicState = ePBS_MOVING;
		else
			m_CurBasicState = ePBS_NONE;
	}*/

	MSG_DWORD4 msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_STATEINFO;
	msg.dwObjectID = m_pMaster->GetID();
	msg.dwData1	= GetID();
	if(!state)
	{//기본 상태에서의 모션 구하기
		state = GetMotionNumFromBasicState();
	}
	msg.dwData2 = state;
	//if(!state) return; ....이동시 대사 있음.

	PetSpeechInfo rtSpeechInfo;
	if(PET_SPEECHMGR->GetPetCurStateSpeechIndex(GetPetKind(),state,GetPetCurGrade(),&rtSpeechInfo))
	{
		msg.dwData3 = rtSpeechInfo.SpeechIndex;	//대사 인덱스
//	msg.dwData4 = rtSpeechInfo.SpeechType;	//대사 타입 / 우선 필요 없을듯.
	}
	else
	{
		msg.dwData3 = 0;
		if(!state)	//모션도 없으면
			return;
	}

	PACKEDDATA_OBJ->QuickSend(m_pMaster, &msg, sizeof(MSG_DWORD4));
}

BYTE CPet::GetMotionNumFromBasicState()
{
	switch(GetPetBasicState())
	{
	case ePBS_NONE:	//기본상태에서는 보조모션.
		{
			return ePM_SUBSTAND;
		}
	case ePBS_MOVING:	//이동모션은 이벤트적 모션없음.
		{
			return ePM_MOVE;
		}
	case ePBS_UNGI:	//운기중 이벤트.
		{
			return ePM_UNGI;
		}
	default:
		break;
	}
	return 0;
}


void CPet::GetPetMotionFromBasicState()
{
//	if(gCurTime - m_dwStateCheckTime < m_dwRndGapTime)	return;

	GetRandMotionNSpeech();

//	m_dwStateCheckTime = gCurTime;
}

void CPet::Process()
{
//	if(FALSE == m_bSeal)	//봉인해제
//	{
//		CheckCurBasicState();
		if(m_PetTotalInfo.bAlive == FALSE)	return;

		CalcStamina();
		//CalcSkillRecharge();	//펫 메니져로.. 펫 속성이라기 보다 유저에 의한 명령 타이밍.
		
		//CheckPetMoving();
		
		if(m_CurBasicState != ePBS_UNGI)
		{
			if(IsPetMoving())
				m_CurBasicState = ePBS_MOVING;
			else
				m_CurBasicState = ePBS_NONE;
		}
		//10초 마다 계산
		if(gCurTime - m_dwInfoCheckTime < PET_STATE_CHECK_TIME) return;

		GetPetMotionFromBasicState();

		ASSERT(m_dwStaminaDecrease);

		//스테미나 감소
		//SW060406 펫 장착
		if(m_pMaster)
		{
			CPetManager* pPetMgr = m_pMaster->GetPetManager();
			PETEQUIP_ITEMOPTION* pEquipInfo = pPetMgr->GetPetEquipOption();
			DWORD PSDecrease = pEquipInfo->iPetStaminaReductionDecrease;

			if(PSDecrease)
			{
				m_dwStaminaDecrease -= DWORD(m_dwStaminaDecrease * PSDecrease / 100);
			}
		}		
		m_dwStaminaDecrease /= m_wFrameCounter;	//소수점 버림.
		//SW070927 기획 추가 : 펫 3단계 친밀도 100% 시 스태미나 감소 없음.
		if( (PET_MAX_GRADE == GetPetCurGrade()) && (PET_MAX_FRIENDLY == GetPetFriendShip()) )
		{
		}
		else
			AddStamina( -1 * m_dwStaminaDecrease );

		//친밀도 
		CalcFriendship();

		//!!!서버렉 심화의 문제가 될 수 있다.
		SendPetInfoMsg();

		m_dwStaminaDecrease	= m_wFrameCounter	= 0;
		
		m_dwInfoCheckTime = gCurTime;
//	}
}

void CPet::CalcPetMaxStamina()
{
	if(!m_pBaseInfo)	return;
	if(m_PetTotalInfo.PetGrade < 1)
	{
		ASSERT(0);
		return;
	}

	DWORD MaxStamina = m_pBaseInfo->StaminaMax[m_PetTotalInfo.PetGrade-1];
	//SW060406 펫 장착
	if(m_pMaster)
	{
		CPetManager* pPetMgr = m_pMaster->GetPetManager();
		PETEQUIP_ITEMOPTION* pEquipInfo = pPetMgr->GetPetEquipOption();
		DWORD PSMaxIncrease = pEquipInfo->iPetStaminaMaxIncreaseAmount;

		if(PSMaxIncrease)
		{
			SetPetMaxStamina(MaxStamina + PSMaxIncrease, TRUE);
			return;
		}
	}

	SetPetMaxStamina(MaxStamina);
}

void CPet::CalcStamina()
{//매 프레임마도 펫 상태에 따른 값을 더하고 10초가 되면 총 누적값을 프레임수로 나누어 10초 평균 값을 구한다.
	++m_wFrameCounter;

	static DWORD _stand = ((PET_RULE*)GAMERESRCMNGR->GetPetRule())->StaminaStand;
	static DWORD _move = ((PET_RULE*)GAMERESRCMNGR->GetPetRule())->StaminaMove;

	BOOL decreaseTwice = FALSE;
	if(!IsPetRest())	//펫 활성화 상태에서 소모 2배
		decreaseTwice = TRUE;

	switch(GetPetBasicState())
	{
	case ePBS_NONE:
	case ePBS_UNGI:
		{
//			m_dwStaminaDecrease += eSDA_STAND;
			m_dwStaminaDecrease += _stand + _stand * decreaseTwice;
		}
		break;
	case ePBS_MOVING:
		{
//			m_dwStaminaDecrease += eSDA_MOVE;
			m_dwStaminaDecrease += _move + _move * decreaseTwice;
		}
		break;
	default:
		break;
	}
}

void CPet::AddStamina(int stamina, BOOL bSendMsg/* = FALSE*/)
{//10초마다
	//if( 0 == stamina ) return; >> friendship msg 때문에 주석처리.

	if( 0 > stamina )
	{
		//빼려는 값보다 크면
		if( m_PetTotalInfo.PetStamina > DWORD(-stamina) )
		{
			m_PetTotalInfo.PetStamina += stamina;
		}
		else
		{
			if( m_PetTotalInfo.PetStamina )
			{
				//펫 스태미나 0일 때 대사.
				GetRandMotionNSpeech(ePM_STAMINA_ZERO, ePMF_ALWAYS);
			}
			m_PetTotalInfo.PetStamina = 0;
		}
	}
	else	//펫먹이 줄때 마다
	{
		//스테미너 회복 포션 러쉬를 막기위한...
		if( m_PetTotalInfo.PetStamina == m_dwMaxStamina )
			return;

		//현재펫 스태미너 값과의 합이 최대값 보다 크면
		if( m_PetTotalInfo.PetStamina + stamina > m_dwMaxStamina )
		{
			m_PetTotalInfo.PetStamina = m_dwMaxStamina;
		}
		else
		{
			m_PetTotalInfo.PetStamina += stamina;
		}
	}

	if(bSendMsg)
		SendPetInfoMsg();
}

//void CPet::PlusStamina(DWORD amount)
//{
//	m_PetTotalInfo.PetStamina += amount;
//	DWORD dwCurMaxStamina = m_pBaseInfo->StaminaMax[m_PetTotalInfo.PetGrade-1];
//	if( m_PetTotalInfo.PetStamina > dwCurMaxStamina )
//	{m_PetTotalInfo.PetStamina = dwCurMaxStamina;}
//}

void CPet::AddFriendship( int friendship, BOOL bSendMsg/* = FALSE*/)
{
	////SW061211 크리스마스이벤트이벤트 펫은 친밀도 영향없다.
	if(m_pMaster)
	{
		CPetManager* pPetMgr = m_pMaster->GetPetManager();
		if( pPetMgr->CheckCurSummonPetKindIs(ePK_EventPet) )
			return;
	}

	if( 0 == friendship ) return;

	DWORD max_friendly = GAMERESRCMNGR->GetPetRule()->MaxFriendship;

	if( 0 > friendship )
	{
		if( m_PetTotalInfo.PetFriendly < DWORD(-friendship) )
		{
			m_PetTotalInfo.PetFriendly = 0;
			//펫 죽음 처리.
			SetPetAlive(FALSE);
			if(m_pMaster->GetState() != eObjectState_Die)
				GetRandMotionNSpeech(ePM_DIE,ePMF_ALWAYS);
			//DBUpdate는 하지 않는다
			SendPetDieMsg();
			//PetUpdateToDB(m_pMaster->GetID(), m_PetTotalInfo);			
			m_pMaster->GetPetManager()->ReleaseMoveWithDelay(5000);
			//m_pMaster->SetCurPet(NULL);

			const PET_TOTALINFO* pInfo = GetPetTotalInfo();
			LogPet(m_pMaster->GetID(),m_pMaster->GetUserID(),ePetLog_PetDie, pInfo->PetDBIdx, pInfo->PetSummonItemDBIdx,
				pInfo->PetGrade,pInfo->PetStamina,pInfo->PetFriendly,pInfo->bAlive);

		}
		else
			m_PetTotalInfo.PetFriendly += friendship;
	}
	else
	{
		if( max_friendly < m_PetTotalInfo.PetFriendly + friendship )
			m_PetTotalInfo.PetFriendly = max_friendly;	//렙업 조건
		else
			m_PetTotalInfo.PetFriendly += friendship;
	}

	//친밀도 맥스면 메시지 보내라 -> 다른 곳에서 보낸다. 빈도 줄이자.
	//	if(IsPetMaxFriendship())
	//	{
	//		SendPetInfoMsg();
	//	}

	if(bSendMsg)
		SendPetInfoMsg();
}

void CPet::CalcFriendship()
{
	//10초마다 체크하여 스태미나 0 이면 친밀도 감소
	//if(IsPetStaminaZero())
	if( 0 == m_PetTotalInfo.PetStamina )
	{
		int Petfriendly = GAMERESRCMNGR->GetPetRule()->FriendShipPerStaminaZero;
		AddFriendship(Petfriendly);
	}
	else
	{
		DWORD friendShip = 0;
		PET_RULE* pRule = GAMERESRCMNGR->GetPetRule();
		ASSERT(pRule);

		if(!IsPetRest())
		{
			friendShip = pRule->FriendShipPerSkill[GetPetCurGrade()-1];
		}
		else
		{
			friendShip = pRule->FriendShipPer10Sec[GetPetCurGrade()-1];
		}

		//SW060406 펫 장착
		if(m_pMaster)
		{
			CPetManager* pPetMgr = m_pMaster->GetPetManager();
			PETEQUIP_ITEMOPTION* pEquipInfo = pPetMgr->GetPetEquipOption();
			DWORD PFIncrease = pEquipInfo->iPetFriendshipIncrease;

			if(PFIncrease)
			{
				friendShip += DWORD(friendShip * PFIncrease / 100);
			}

		}

		AddFriendship(friendShip);
	}
}

BOOL CPet::IsPetMaxFriendship()
{
	DWORD max_friendly = GAMERESRCMNGR->GetPetRule()->MaxFriendship;
	return (max_friendly == m_PetTotalInfo.PetFriendly);
}

BOOL CPet::IsPetStaminaFull()
{
	if(m_pBaseInfo)
		return (m_dwMaxStamina == m_PetTotalInfo.PetStamina);
	else
	{
		ASSERT(0);
		return FALSE;
	}
}

//void CPet::CalcSkillRecharge()
//{
//	if(m_bRest)
//		return;
//	else
//	{
//		//to do 장착 아이템과 펫 종류에 따라 게이지 충전 속도 차이.
//	}
//}

BOOL CPet::UsePetSkill()
{
	//주인과 거리 체크
	//스킬딜레이 체크

	//친밀도 증가
	//스태미나 감소
	//정지 시켜줘야하나..

	SKILLMGR->PetOperSkill(m_pBaseInfo->SkillIdx[GetPetCurGrade()-1], this, m_pMaster);

	//친밀도 증가
	int friendShip = 0;
	PET_RULE* pRule = GAMERESRCMNGR->GetPetRule();
	ASSERT(pRule);

	friendShip = pRule->FriendShipPerSkill[GetPetCurGrade()-1];

	AddFriendship(friendShip);

	//!!!펫 종류와 장착 템에 따라 팻 스태미나 감소
	int stamina = 0;
	stamina = m_pBaseInfo->StaminaDecreasePerSkill[GetPetCurGrade()-1];
	AddStamina(-stamina,TRUE);

	GetRandMotionNSpeech(ePM_SKILL);	//스킬리스트의 공격모션을 안쓰려면 지정.

	return TRUE;
}

void CPet::SendPetInfoMsg()
{
	if( m_pMaster == NULL )
	{
		char temp[256] = {0,};
		sprintf(temp, "Error!! <SendPetInfoMsg> pMaster is NULL. MasterId : %d	PetDBId : %d",
			m_PetTotalInfo.PetMasterUserID, m_PetTotalInfo.PetDBIdx);
		ASSERTMSG(0, temp);
		return;
	}

	////SW061211 크리스마스이벤트
	if(m_pMaster)
	{
		CPetManager* pPetMgr = m_pMaster->GetPetManager();
		if( pPetMgr->CheckCurSummonPetKindIs(ePK_EventPet) )
			return;
	}

	//펫 정보를 해당 주인에게 보낸다.
	MSG_DWORD4	msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_VALUEINFO;
	msg.dwObjectID = m_pMaster->GetID();
	msg.dwData1	= m_PetTotalInfo.PetStamina;
	msg.dwData2 = m_PetTotalInfo.PetFriendly;
	//msg.dwData3 = m_PetTotalInfo.	//스킬 준비 상태 또는 게이지 관련.
	msg.dwData4 = GetID();

	//g_Network.Send2Server(m_pMaster->GetAgentNum(),(char*)&msg,sizeof(msg));
	m_pMaster->SendMsg(&msg, sizeof(msg));
}

void CPet::SendPetDieMsg()
{
	MSG_DWORD2 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_PET_DIE;

	msg.dwData1 = m_pMaster->GetID();
	msg.dwData2 = GetID();

	PACKEDDATA_OBJ->QuickSend(m_pMaster,&msg,sizeof(msg));
}
