
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../[CC]Header/GameResourceManager.h"
#include "TitanManager.h"
#include "ServerSystem.h"
#include "PackedData.h"
#include "MHFile.h"
#include "Titan.h"
#include "Player.h"
#include "MapDBMsgParser.h"
#include "ItemManager.h"
#include "CheckRoutine.h"
#include "StatsCalcManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//DWORD CTitanManager::m_gTitanSpellDecrease = 0;

CTitanManager::CTitanManager()
{
	m_TitanInfoList.Initialize(4);
	m_ItemEnduranceList.Initialize(4);
	m_ItemUsingEnduranceList.Initialize(4);

	m_ItemEndurancePool.Init( 50, 10, "TitanEnduranceItem" );

	Init(NULL);
	/*
	m_pCurRidingTitan	= NULL;
	m_pMaster			= NULL;

	m_dwCurRegistTitanCallItemDBIdx = 0;

	m_dwRecallCheckTime = 0;

	TitanScaleForNewOne = 1000;

	m_bAvaliableEndurance = FALSE;*/

	m_dwTitanRecallProcessTime = 0;	
	m_dwCurrentTime = 0;
	m_bTitanRecall = FALSE;	
	m_bTitanRecallClient = FALSE;


	m_dwTitanEPTime = 0;
	m_dwTitanMaintainTime = 0;	// magi82(23)
}

CTitanManager::~CTitanManager()
{

}

void CTitanManager::Init( CPlayer* pPlayer )
{
	m_pMaster = pPlayer;
	m_pCurRidingTitan	= NULL;

	m_dwCurRegistTitanCallItemDBIdx = 0;

	m_dwRecallCheckTime = 0;

	TitanScaleForNewOne = 100;

	m_bAvaliableEndurance = FALSE;


	memset( m_TitanWearedInfo, 0, sizeof(TitanWearedInfo)*eTitanWearedItem_Max );

	ZeroMemory(&m_TitanShopitemOption, sizeof(TITAN_SHOPITEM_OPTION));	// magi82(26)
}

void CTitanManager::Release()
{
	UpDateCurTitanAndEquipItemInfo();

	m_TitanInfoList.SetPositionHead();
	TITAN_TOTALINFO* pTotalInfo = NULL;
	while( pTotalInfo = m_TitanInfoList.GetData() )
	{
		//delete pTotalInfo;
		//pTotalInfo = NULL;
		g_pServerSystem->RemoveTitanInfo(pTotalInfo);
	}
	m_TitanInfoList.RemoveAll();

	m_ItemUsingEnduranceList.RemoveAll();

	m_ItemEnduranceList.SetPositionHead();
	TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = NULL;
	while( pEnduranceInfo = m_ItemEnduranceList.GetData() )
	{
		delete pEnduranceInfo;
		pEnduranceInfo = NULL;
	}
	m_ItemEnduranceList.RemoveAll();

	if( m_pCurRidingTitan )
	{
		//delete m_pCurRidingTitan;
		//m_pCurRidingTitan = NULL;
		RemoveTitan();
	}

	ZeroMemory(&m_TitanShopitemOption, sizeof(TITAN_SHOPITEM_OPTION));	// magi82(26)
}

void CTitanManager::RemoveTitan()
{
	if(!m_pCurRidingTitan)	return;

	g_pServerSystem->RemoveTitan(m_pCurRidingTitan);

	m_pCurRidingTitan = NULL;
}

void CTitanManager::AddTitanTotalInfo( TITAN_TOTALINFO* pTitanInfo, int flagSendMsgTo/* = eServerOnly*/ )
{
	if( !pTitanInfo )	return;

	TITAN_TOTALINFO* pInfo = NULL;
	if( pInfo = m_TitanInfoList.GetData(pTitanInfo->TitanCallItemDBIdx) )
	{
		*pInfo = *pTitanInfo;
		return;
	}

	if(eServerNClient == flagSendMsgTo)
	{
		SEND_ADDTITAN_FROMITEM msg;
		memcpy(&msg.Info, pTitanInfo, sizeof(TITAN_TOTALINFO));
		msg.Category = MP_TITAN;
		msg.Protocol = MP_TITAN_ADDNEW_FROMITEM;
		msg.dwObjectID = m_pMaster->GetID();

		m_pMaster->SendMsg(&msg, sizeof(SEND_ADDTITAN_FROMITEM));
	}

	//TITAN_TOTALINFO* pNewInfo = new TITAN_TOTALINFO;
	//memcpy(pNewInfo, pTitanInfo, sizeof(TITAN_TOTALINFO));
	TITAN_TOTALINFO* pNewInfo = g_pServerSystem->MakeTitanInfo(pTitanInfo);
	if( NULL == pNewInfo )
	{
		ASSERT(0);
		return;
	}
	if( FALSE == AddTitanTotalInfoList(pNewInfo) )
	{
		g_pServerSystem->RemoveTitanInfo( pNewInfo );
	}
}

BOOL CTitanManager::AddTitanTotalInfoList( TITAN_TOTALINFO* pTitanInfo )
{
	if(m_TitanInfoList.GetData(pTitanInfo->TitanCallItemDBIdx))
	{
		ASSERT(0);
		return FALSE;
	}

	m_TitanInfoList.Add(pTitanInfo, pTitanInfo->TitanCallItemDBIdx);
	return TRUE;
}


TITAN_TOTALINFO* CTitanManager::GetTitanTotalInfo( DWORD callItemDBIdx )
{
	TITAN_TOTALINFO* pTitanInfo = m_TitanInfoList.GetData(callItemDBIdx);

	return pTitanInfo;
}

void CTitanManager::RemoveTitanTotalInfo( DWORD callItemDBIdx )
{// 교환시 리스트 상에서만 삭제
	TITAN_TOTALINFO* pInfo = m_TitanInfoList.GetData(callItemDBIdx);

	ASSERT(pInfo);

	m_TitanInfoList.Remove(callItemDBIdx);
}

void CTitanManager::DeleteTitanTotalInfo( DWORD callItemDBIdx )
{// 소환아이템삭제, 로그아웃시.
	TITAN_TOTALINFO* pInfo = m_TitanInfoList.GetData(callItemDBIdx);

	ASSERT(pInfo);
	//SAFE_DELETE(pInfo);
	if(pInfo)
		g_pServerSystem->RemoveTitanInfo(pInfo);

	m_TitanInfoList.Remove(callItemDBIdx);
}

WORD CTitanManager::GetTitanInfoList( TITAN_TOTALINFO* RtInfo )
{
	WORD TitanCount = 0;
	m_TitanInfoList.SetPositionHead();
	TITAN_TOTALINFO* pInfo = NULL;
	while(pInfo = m_TitanInfoList.GetData())
	{
		memcpy(&RtInfo[TitanCount], pInfo, sizeof(TITAN_TOTALINFO));
		++TitanCount;
	}

	return TitanCount;
}

void CTitanManager::DeleteTitan( DWORD callItemDBIdx )
{//타이탄 소환 아이템 삭제시. DB 데이터 포함 삭제
	DeleteTitanTotalInfo( callItemDBIdx );

	TitanDeleteToDB( callItemDBIdx );
}

void CTitanManager::CreateNewTitan( CPlayer* pMaster, DWORD dwItemIdx, DWORD dwCallItemDBIdx, WORD wTitanKind, WORD wTitanGrade /*= 1*/ )
{
	BASE_TITAN_LIST* pTitanListInfo = NULL;

	pTitanListInfo = GAMERESRCMNGR->GetTitanListInfo(wTitanKind);
	if( NULL == pTitanListInfo )	return;

	TITAN_TOTALINFO TotalInfo;
	TotalInfo.TitanCallItemDBIdx = dwCallItemDBIdx;
	TotalInfo.TitanKind = wTitanKind;

	//현 기획서상 최초 등급이 1이 아닌경우는 없다. 하지만 등급별로 아이템이 따로 존재하기때문에...
	//ASSERT(1 == wTitanGrade);
	if( 1 > wTitanGrade || 3 < wTitanGrade )
	{
		ASSERT(0);
		wTitanGrade = 1;
	}

	TotalInfo.Fuel = pTitanListInfo->GradeInfo[wTitanGrade-1].MaxFuel;
	TotalInfo.Spell = pTitanListInfo->GradeInfo[wTitanGrade-1].MaxSpell;
	TotalInfo.TitanGrade = wTitanGrade;
	TotalInfo.TitanMasterUserID = pMaster->GetUserID();
	TotalInfo.RegisterCharacterIdx = 0;
	TotalInfo.RecallTime = 0;
	TotalInfo.bRiding = FALSE;
	//TotalInfo.bRegistered = FALSE;
	TotalInfo.Scale = TitanScaleForNewOne;

	TitanInsertToDB(pMaster->GetUserID(), pMaster->GetID(), dwItemIdx, &TotalInfo);
}

void CTitanManager::CreateUpgradeTitan( CPlayer* pMaster, DWORD dwItemIdx, DWORD dwCallItemDBIdx )
{
	// 인덱스와 등급은 dwItemIdx로 부터 itemlist 상의 정보로부터 가져온다.

	BASE_TITAN_LIST* pTitanListInfo = NULL;

	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( (WORD)dwItemIdx );

	if(!pInfo)
	{
		ASSERT(0);
		return;
	}

	WORD wTitanKind = pInfo->Plus_MugongIdx;
	WORD wTitanGrade = pInfo->Plus_Value;

	pTitanListInfo = GAMERESRCMNGR->GetTitanListInfo(wTitanKind);
	if( NULL == pTitanListInfo )	return;

	TITAN_TOTALINFO TotalInfo;
	TotalInfo.TitanCallItemDBIdx = dwCallItemDBIdx;
	TotalInfo.TitanKind = wTitanKind;

	//현 기획서상 최초 등급이 1이 아닌경우는 없다. 하지만 등급별로 아이템이 따로 존재하기때문에...
	//ASSERT(1 == wTitanGrade);
	if( 1 > wTitanGrade || 3 < wTitanGrade )
	{
		ASSERT(0);
		wTitanGrade = 1;
	}

	TotalInfo.Fuel = pTitanListInfo->GradeInfo[wTitanGrade-1].MaxFuel;
	TotalInfo.Spell = pTitanListInfo->GradeInfo[wTitanGrade-1].MaxSpell;
	TotalInfo.RecallTime = 0;
	TotalInfo.TitanGrade = wTitanGrade;
	TotalInfo.TitanMasterUserID = pMaster->GetUserID();
	TotalInfo.RegisterCharacterIdx = pMaster->GetID();
	TotalInfo.bRiding = FALSE;
	//TotalInfo.bRegistered = FALSE;
	TotalInfo.Scale = (WORD)TitanScaleForNewOne;

	TitanInsertToDB(pMaster->GetUserID(), pMaster->GetID(), dwItemIdx, &TotalInfo);
}

DWORD CTitanManager::GetTitanSpellDecrease()
{
//	static DWORD titanSpellDecrease = 0;
//	titanSpellDecrease = GAMERESRCMNGR->GetTitanSpellCost(g_pServerSystem->GetMapNum());
//	// 타이탄 장착 옵션으로 부터 가감
//	return titanSpellDecrease;

	return GAMERESRCMNGR->GetTitanSpellCost(g_pServerSystem->GetMapNum());
}

void CTitanManager::ReduceTitanSpell( DWORD spell )
{
	if(m_pCurRidingTitan)
	{
		m_pCurRidingTitan->ReduceSpell(spell);
	}
}

// 타이탄 프로세스
// 마력관련처리
// 장착아이템 내구도 처리
void CTitanManager::TitanProcess()
{
	if( m_pCurRidingTitan
		&& FALSE == g_pServerSystem->GetMap()->IsMapKind(eTitanMap) )
		//&& GAMERESRCMNGR->GetLoadMapNum() != 101
		//&& GAMERESRCMNGR->GetLoadMapNum() != 102
		//&& GAMERESRCMNGR->GetLoadMapNum() != 104
		//&& GAMERESRCMNGR->GetLoadMapNum() != 105 )
	{
		// magi82(23)
		if( GetCurTime() > m_dwTitanMaintainTime )
		{
            GetOffTitan();
			m_dwTitanMaintainTime = 0;
		}
		//m_pCurRidingTitan->Process();
	}

	TitanRecallProcess();
}

void CTitanManager::UpDateCurTitanAndEquipItemInfo()
{
	UpdateCurTitanInfo();

	UpdateUsingEquipItemEnduranceInfo();
}

void CTitanManager::UpdateCurTitanInfo()
{
	if( !m_pCurRidingTitan )	return;

	const TITAN_TOTALINFO* pCurTitanInfo = m_pCurRidingTitan->GetTotalInfo();
	DWORD dwCallItemDBIdx = pCurTitanInfo->TitanCallItemDBIdx;
	TITAN_TOTALINFO* TInfo = GetTitanTotalInfo(dwCallItemDBIdx);
	if(!TInfo)
	{
		ASSERT(0);
		return;
	}

	*TInfo = *pCurTitanInfo;

	// 맵이동시 다음 소환 딜레이
	if( m_dwRecallCheckTime > GetCurTime() )
	{
		TInfo->RecallTime = m_dwRecallCheckTime;
	}
	else
	{
		TInfo->RecallTime = 0;
	}

	// magi82(23)
	//if( GAMERESRCMNGR->GetLoadMapNum() == 101
	//	|| GAMERESRCMNGR->GetLoadMapNum() == 102
	//	|| GAMERESRCMNGR->GetLoadMapNum() == 104
	//	|| GAMERESRCMNGR->GetLoadMapNum() == 105 )
	if( g_pServerSystem->GetMap()->IsMapKind(eTitanMap) )
	{
		stTIME stMaintainTime, stCurTime;
		DWORD dwMilliSec = GAMERESRCMNGR->GetTitanRule()->TitanMaintainTime + m_pMaster->GetAbilityStats()->dwTitanRidingPlusTime;
		DWORD dwSec = 0;
		DWORD dwMin = 0;

		dwSec = dwMilliSec / SECTOMILLISEC;
		dwMin = dwSec / MINUTETOSECOND;
		dwSec = dwSec % MINUTETOSECOND;

		stMaintainTime.SetTime(0,0,0,0,dwMin,dwSec);
		stCurTime.SetTime(GetCurTime());
		stCurTime += stMaintainTime;
		m_dwTitanMaintainTime = stCurTime.value;

		TInfo->MaintainTime = m_dwTitanMaintainTime;
	}

	TitanUpdateToDB(m_pMaster->GetUserID(), TInfo);
}

// 타이탄 탑승
BOOL CTitanManager::RideInTitan( DWORD callItemDBIdx, BOOL bSummonFromUser/* = TRUE*/ )
{
	if( FALSE == CheckBeforeRideInTitan(callItemDBIdx) )
		return FALSE;

	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);

	if(!pInfo)
	{
		char szBuf[128] = {0,};
		sprintf(szBuf, "Player Have No TitanData with Item! PlayerID :%d\t ItemDBIdx:%d\n"
			,m_pMaster->GetID(),callItemDBIdx);
		ASSERTMSG(0,szBuf);
		return FALSE;
	}

	if(NULL == m_pCurRidingTitan)
	{
		//m_pCurRidingTitan = new CTitan;	// 메모리풀로 바꾸던지..
		m_pCurRidingTitan = g_pServerSystem->MakeTitan(m_pMaster);
	}
	else
	{
		ASSERT(0);
	}

	// 소환 가능 시간 초기화.
	//pInfo->RecallTime = 0;

	// magi82(23)
	m_dwTitanMaintainTime = pInfo->MaintainTime;
    
	m_pCurRidingTitan->InitTitan(pInfo, m_pMaster);

	//클라이언트로 타이탄 정보 보내기 전 세팅.
	SetTitanStats();

	//m_pCurRidingTitan->SendTitanInfoMsg();

	//SendTitanStats();	//주인에게 보내는 타이탄 세부 정보

	// 유효 그리드 정보 보내기
	MSG_TITAN_RIDEIN msg;
	msg.Category = MP_TITAN;
	msg.Protocol = MP_TITAN_RIDEIN_ACK;
	msg.dwObjectID = m_pMaster->GetID();
	msg.OwnerID = m_pMaster->GetID();
	msg.TitanTotalInfo = *pInfo;
	GetAppearanceInfo(&msg.AppearanceInfo);

	// magi82(23)
	stTIME stCurTime, stMaintainTime;
	stMaintainTime.SetTime(m_dwTitanMaintainTime);
	stCurTime.SetTime(GetCurTime());
	stMaintainTime -= stCurTime;
	msg.MaintainTime = Convert2MilliSecond( stMaintainTime.GetDay(), stMaintainTime.GetHour(), stMaintainTime.GetMinute(), stMaintainTime.GetSecond() );

	if(TRUE == bSummonFromUser)
	{
		PACKEDDATA_OBJ->QuickSend( m_pMaster, &msg, sizeof(msg) );
	}
	else
	{
		m_pMaster->SendMsg(&msg, sizeof(msg));
	}

	return TRUE;
}

void CTitanManager::CheckRidingTitan()
{
	TITAN_TOTALINFO* pInfo = NULL;

	m_TitanInfoList.SetPositionHead();
	while( pInfo = m_TitanInfoList.GetData() )
	{
		if( TRUE == pInfo->bRiding )
		{
			RideInTitan(pInfo->TitanCallItemDBIdx, FALSE);

			SetTitanRiding(FALSE);

			return;
		}
	}
}

void CTitanManager::SetTitanRiding( BOOL bVal )
{
	if(!m_pCurRidingTitan)	return;

	m_pCurRidingTitan->SetTitanRiding(bVal);
}

// 타이탄 탑승 조건 검사
BOOL CTitanManager::CheckBeforeRideInTitan( DWORD callItemDBIdx )
{
	// 등록된 타이탄 인지
	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);
	if(!pInfo)
		return FALSE;

	if( m_pMaster->GetID() != pInfo->RegisterCharacterIdx )
		return FALSE;

	// 현재 탑승중
	if( m_pCurRidingTitan )
	{
		if( m_pCurRidingTitan->GetCallItemDBIdx() == callItemDBIdx )
			GetOffTitan(eFromUser);
		else
		{	//Client 에서 처리.
			ASSERT(0);
		}

		return FALSE;
	}

	// 재소환 딜레이 검사
	// 타이탄 연료가 있는지, 또는 연료 바닥으로 회복 재소환 딜레이 중인지
	if( !CheckTimeRecallAvailable() )
		return FALSE;

	// 주인의 체력이 10% 이상인지
	DWORD perc = m_pMaster->GetLife() * 100 / m_pMaster->GetMaxLife();
	if( perc < GAMERESRCMNGR->GetTitanRule()->MasterLifeRate_forTitanCall )
		return FALSE;

	// 타이탄 장착 아이템이 전부 있는지
	// 타이탄 장착 아이템 중 내구력이 1% 미만인 아이템이 3개 미만인지
	// 070621 기획변경
	/*if( !CheckEquipState() )
		return FALSE;*/

	return TRUE;
}

void CTitanManager::SetRecallCheckTime(DWORD checkTime /*= 0*/)
{
	if(checkTime)
	{
		if(m_dwRecallCheckTime < checkTime)
			m_dwRecallCheckTime = checkTime;	// RCharacterTitanInfo로 부터 중복 시 갱신
	}
	else
	{
		if(!m_dwCurRegistTitanCallItemDBIdx)
		{
			m_dwRecallCheckTime = checkTime;	// 등록 해제후 이동.
			return;
		}
		
		//TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(m_dwCurRegistTitanCallItemDBIdx);
		TITAN_TOTALINFO* pInfo = m_pCurRidingTitan->GetTotalInfo();

		if(!pInfo)
		{
			ASSERT(0);
			return;
		}

		stTIME tmp,penalTime;

		DWORD penalSec = 0;
		DWORD penalMin = 0;
		
		// magi82 - Titan(070914) 타이탄 증서 딜레이
		TITANINFO_GRADE* pGrade = GAMERESRCMNGR->GetTitanGradeInfo(pInfo->TitanKind, pInfo->TitanGrade);
		if( !pGrade )
			return;

		// magi82(24) - 
		//if( GAMERESRCMNGR->GetLoadMapNum() == 101
		//	 || GAMERESRCMNGR->GetLoadMapNum() == 102
		//	 || GAMERESRCMNGR->GetLoadMapNum() == 104
		//	 || GAMERESRCMNGR->GetLoadMapNum() == 105 )
		if( g_pServerSystem->GetMap()->IsMapKind(eTitanMap) )
		{
			// 판타지 맵일때
			// 체력조건으로 해제된 경우
			if( 0 == pInfo->Fuel || 0 == pInfo->Spell )
			{
				penalSec = GAMERESRCMNGR->GetTitanRule()->TitanRecallValidTime_ZeroFuel - pGrade->RecallReduceTime;
			}
			else
			{
				penalSec = GAMERESRCMNGR->GetTitanRule()->TitanRecallValidTime_Seal - pGrade->RecallReduceTime;
			}
		}
		else
		{
			// 중원 맵일때
			penalSec = GAMERESRCMNGR->GetTitanRule()->TitanRecallValidTime_General - pGrade->RecallReduceTime;
		}

		if(!penalSec)
			return;

		// magi82(26)
		if( GetTitanShopitemOption()->dwRecallReduceRate > 0.0f)
		{
			penalSec = (DWORD)(penalSec * GetTitanShopitemOption()->dwRecallReduceRate);
		}

		penalSec /= SECTOMILLISEC;
		penalMin = penalSec / MINUTETOSECOND;
		penalSec %= MINUTETOSECOND;
		penalTime.SetTime(GetCurTime());
		tmp.SetTime(0,0,0,0,penalMin,penalSec);
		penalTime += tmp;
		m_dwRecallCheckTime = penalTime.value;

		pInfo->RecallTime = m_dwRecallCheckTime;
	}
}

BOOL CTitanManager::CheckTimeRecallAvailable()
{
	BOOL bAble = FALSE;

	if(!m_dwCurRegistTitanCallItemDBIdx)	return bAble;

	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(m_dwCurRegistTitanCallItemDBIdx);

	if(!pInfo)
	{
		ASSERT(0);
		return bAble;
	}

	bAble = (GetCurTime() > m_dwRecallCheckTime);

	// 체력조건으로 해제된 경우
	if( TRUE == bAble )
	{
		if( 0 == pInfo->Fuel )//|| 0 == pInfo->Spell )
		{
			//체력 회복
			if(bAble)
			{
				BASE_TITAN_LIST* pList = GAMERESRCMNGR->GetTitanListInfo(pInfo->TitanKind);
				//pInfo->Fuel = (pList->GradeInfo[pInfo->TitanGrade-1]).MaxFuel;
				//pInfo->Spell = (pList->GradeInfo[pInfo->TitanGrade-1]).MaxSpell;
				pInfo->Fuel = GetTitanStats()->MaxFuel;
			}
		}
	}
	
	//개발자는 시간 체크 통과
	if( m_pMaster->GetUserLevel() < eUSERLEVEL_GM )
		bAble = TRUE;

	return bAble;
}

BOOL CTitanManager::CheckEquipState()
{
	BOOL bPass = ( CheckUsingEquipItemNum() && CheckUsingEquipItemEndurance() );
	return bPass;
}

// 타이탄 탑승해제
BOOL CTitanManager::GetOffTitan( int reason  /*= eNormal*/)
{
	if( !m_pCurRidingTitan )
		return FALSE;

	SetRecallCheckTime();

	UpdateCurTitanInfo();

	// 클라이언트로 해제 정보 보내기
	MSG_DWORD2 msg;
	msg.Category = MP_TITAN;
	msg.Protocol = MP_TITAN_GETOFF_ACK;
	msg.dwObjectID = m_pMaster->GetID();
	msg.dwData1 = m_pMaster->GetID();
	msg.dwData2 = reason;
	PACKEDDATA_OBJ->QuickSend( m_pMaster, &msg, sizeof(msg) );

	//SAFE_DELETE(m_pCurRidingTitan);
	g_pServerSystem->RemoveTitan(m_pCurRidingTitan);

	m_pCurRidingTitan = NULL;

	return TRUE;
}

void CTitanManager::ExchangeTitan( DWORD callItemDBIdx, CPlayer* pNewMaster )
{
	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);
	if(!pInfo)
	{
		ASSERT(0);
		return;
	}

	if( pInfo->TitanMasterUserID != m_pMaster->GetUserID() )
	{
		ASSERT(0);
	}

	pInfo->TitanMasterUserID = pNewMaster->GetUserID();

	TitanUpdateToDB(pNewMaster->GetUserID(), pInfo);

	pNewMaster->GetTitanManager()->AddTitanTotalInfoList(pInfo);

	RemoveTitanTotalInfo(pInfo->TitanCallItemDBIdx);
}

// 타이탄 등급업
// 아이템 관련처리(등급별 아이템이 다름.기존 소환아이템 지우기.다음 등급 소환아이템 생성하기)
// 타이탄 등급 가산처리(DB)
void CTitanManager::UpgradeTitan( DWORD callItemDBIdx, DWORD newCallItemDBIdx )
{
	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);
	if(!pInfo)
	{
		ASSERT(0);
		return;
	}

	if( pInfo->TitanGrade >= MAX_TITANGRADE )
	{
		ASSERT(0);
		return;
	}
	pInfo->TitanGrade++;

	pInfo->TitanCallItemDBIdx = newCallItemDBIdx;

	TitanUpdateToDB(m_pMaster->GetUserID(), pInfo);
}

// 타이탄 등록
// 타이탄 등록처리(DB)
BOOL CTitanManager::RegistTitan( DWORD callItemDBIdx)
{
	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);
	if(!pInfo)
	{
		ASSERT(0);
		return FALSE;
	}

	if( pInfo->RegisterCharacterIdx )
	{
		ASSERT(0);
		return FALSE;
	}

	//등록한 캐릭터 idx 
	pInfo->RegisterCharacterIdx = m_pMaster->GetID();

	//등록중인 인증서 DBIdx
	//m_dwCurRegistTitanCallItemDBIdx = callItemDBIdx;
	SetRegistTitanCallItemDBIdx(callItemDBIdx);

	TitanUpdateToDB(m_pMaster->GetUserID(), pInfo);
	
	return TRUE;
}

// 타이탄 등록해제
// 타이탄 등록해제처리(DB)
BOOL CTitanManager::CancleTitanRegister( DWORD callItemDBIdx )
{
	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);
	if(!pInfo)
	{
		ASSERT(0);
		return FALSE;
	}

	if( 0 == pInfo->RegisterCharacterIdx )
	{
		ASSERT(0);
		return FALSE;
	}

	//등록한 캐릭터 idx 초기화
	pInfo->RegisterCharacterIdx = 0;

	// 재소환 가능 시간 초기화
	pInfo->RecallTime = 0;

	//등록중인 인증서 DBIdx 초기화
	m_dwCurRegistTitanCallItemDBIdx = 0;

	TitanUpdateToDB(m_pMaster->GetUserID(), pInfo);

	return TRUE;
}

#define TITAN_POTION_DELAY 29000	// 클라이언트보다 1초 빠르게.. (동기화)
int CTitanManager::ApplyYoungYakItemToCurTitan( ITEM_INFO* pItemInfo )
{
	if(NULL == m_pCurRidingTitan)
	{
		char tmp[128];
		ASSERTMSG(0,tmp);
		return eItemUseErr_Err;
	}

	if( gCurTime - m_dwTitanEPTime <= TITAN_POTION_DELAY )
		return eItemUseErr_TitanItemTime;

	m_dwTitanEPTime = (DWORD)(gCurTime - (TITAN_POTION_DELAY * GetTitanShopitemOption()->dwEPReduceRate));	// magi82(26)

	if(pItemInfo->LifeRecoverRate)
	{
		//연료 최대치 회복
		AddCurTitanFuelAsRate( pItemInfo->LifeRecoverRate );
	}
	else if(pItemInfo->LifeRecover)
	{
		//타이탄 연료 추가
		AddCurTitanFuel(pItemInfo->LifeRecover);
	}

	if(pItemInfo->NaeRyukRecoverRate)
	{
		//내력 최대치 회복
		AddCurTitanSpellAsRate( pItemInfo->NaeRyukRecoverRate );
	}
	else if(pItemInfo->NaeRyukRecover)
	{
		//타이탄 마력 추가
		AddCurTitanSpell(pItemInfo->NaeRyukRecover);
	}

	return eItemUseSuccess;
}

void CTitanManager::AddCurTitanFuel( WORD amount )
{
	if(!m_pCurRidingTitan)	return;

	DWORD curFuel = m_pCurRidingTitan->GetLife();

	m_pCurRidingTitan->SetLife(curFuel + amount);
}

void CTitanManager::AddCurTitanSpell( WORD amount )
{
	if(!m_pCurRidingTitan)	return;

	DWORD curSpell = m_pCurRidingTitan->GetNaeRyuk();

	m_pCurRidingTitan->SetNaeRyuk(curSpell + amount);
}

void CTitanManager::AddCurTitanFuelAsRate( float recoverRate )
{
	if(!m_pCurRidingTitan)	return;
	
	DWORD maxFuel = m_pCurRidingTitan->GetMaxLife();

	DWORD recover = DWORD(maxFuel * recoverRate);

	m_pCurRidingTitan->SetLife(m_pCurRidingTitan->GetLife() + recover);
}

void CTitanManager::AddCurTitanSpellAsRate( float recoverRate )
{
	if(!m_pCurRidingTitan)	return;

	DWORD maxSpell = m_pCurRidingTitan->GetMaxNaeRyuk();

	DWORD recover = DWORD(maxSpell * recoverRate);

	m_pCurRidingTitan->SetNaeRyuk(m_pCurRidingTitan->GetNaeRyuk() + recover);
}

void CTitanManager::SetTitanStats()
{
	titan_calc_stats* pStats = GetTitanItemStats();

	m_titanStats = *pStats;

	// 타이탄 탑승 중이면 타이탄 기본 스탯을 더해준다.
	if( m_pCurRidingTitan )
	{
		const BASE_TITAN_LIST* pList = m_pCurRidingTitan->GetTitanBaseInfo();

		DWORD grade = m_pCurRidingTitan->GetTitanGrade();

		if(pList)
		{
			const TITANINFO_GRADE* pGradeInfo = &pList->GradeInfo[grade -1];

			m_titanStats.MaxFuel	+= pGradeInfo->MaxFuel;
			m_titanStats.MaxSpell	+= pGradeInfo->MaxSpell;

			m_titanStats.Critical	+= pGradeInfo->Critical;
			m_titanStats.AttackRange	+= pGradeInfo->AttackRange;

			m_titanStats.MoveSpeed	+= pGradeInfo->Speed;

			m_titanStats.MeleeAttackPowerMin	+= pGradeInfo->MeleeAttackPowerMin;
			m_titanStats.MeleeAttackPowerMax	+= pGradeInfo->MeleeAttackPowerMax;
			m_titanStats.RangeAttackPowerMin	+= pGradeInfo->RangeAttackPowerMin;
			m_titanStats.RangeAttackPowerMax	+= pGradeInfo->RangeAttackPowerMax;
			m_titanStats.PhysicalDefense	+=	(WORD)pGradeInfo->PhyDef;
	
			ATTRIBUTEATTACK	atk = pGradeInfo->AttAtk;
			ATTRIBUTEREGIST reg = pGradeInfo->AttReg;
			// 속성 공격력부분.
			m_titanStats.AttributeAttack.AddATTRIBUTE_VAL(atk);	//ApplyRate = 1 // 렙차에 따른 비율. 기획상 타이탄 렙차이 계산 없음.
			m_titanStats.AttributeResist.AddATTRIBUTE_VAL(reg);
		}
	}
}

//=================================================================================
//내구도 처리 관련
//=================================================================================

// 장착아이템 내구도 정보 생성
void CTitanManager::MakeNewEnduranceInfo( CPlayer* pOwner, ITEMBASE* pItemInfo, EnduranceException eException )
{
	// DBInsert 리턴후 처리로..

	TITAN_ENDURANCE_ITEMINFO EnduranceInfo;

	// magi82(33)
	if( eException == eExceptionPyoguk )
        EnduranceInfo.CharacterID = 0;	// 창고에 들어갈때는 0 으로
	else
		EnduranceInfo.CharacterID = pOwner->GetID();

	EnduranceInfo.ItemDBIdx = pItemInfo->dwDBIdx;
	EnduranceInfo.ItemIdx = pItemInfo->wIconIdx;
	//EnduranceInfo.bEquip = FALSE;
	EnduranceInfo.Endurance = TITAN_EQUIPITEM_ENDURANCE_MAX;

	// magi82(33)
	if( eException == eExceptionPyoguk )
		EnduranceInfo.UserIdx = pOwner->GetUserID();	// 창고에 들어갈때만 세팅한다.
	else
		EnduranceInfo.UserIdx = 0;
	

	//EnduranceInfo.OwnerUserID = pOwner->GetUserID();
	//EnduranceInfo.OwnerCharacterID = pOwner->GetID();

	// magi82(33)
	if( eException == eExceptionNone )
	{
		// 게임중 정상적으로 타이탄 장착 아이템을 획득 하였을때..
		TitanEquipItemInfoInsertToDB(&EnduranceInfo);
	}
	else if( eException == eExceptionPyoguk )
	{
		// 이벤트 등 다른 예외사항으로 창고를 열어서 타이탄 장착아이템을 DB에 기록 해야할때..
		TitanEquipInsertExceptionPyogukToDB(pOwner->GetID(), &EnduranceInfo);
	}
	else
	{
		// 이벤트 등 다른 예외사항으로 게임에 접속시 타이탄 장착아이템을 DB에 기록 해야할때..
		TitanEquipInsertExceptionToDB(&EnduranceInfo);
	}
	//AddTitanEquipItemEnduranceInfo(&EnduranceInfo);
}

void CTitanManager::DeleteTitanEquip( DWORD itemDBIdx )
{//타이탄 장착 아이템 삭제시. DB 데이터 포함 삭제
	TitanEquipItemInfoDeleteToDB(itemDBIdx);

	RemoveTitanUsingEquipItemList(itemDBIdx);
	DeleteTitanEquipItemInfo(itemDBIdx);
}

// 타이탄 장착 아이템 내구도처리 관련
void CTitanManager::AddTitanEquipItemEnduranceInfo( TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo, int flagSendMsgTo /*= eServerOnly*/ )
{
	if( !pEnduranceInfo )	return;

	if( m_ItemEnduranceList.GetData(pEnduranceInfo->ItemDBIdx) )
	{
		return;
	}

	if(eServerNClient == flagSendMsgTo)
	{
		SEND_ADDTITANEQUIP_FROMITEM msg;
		memcpy(&msg.Info, pEnduranceInfo, sizeof(TITAN_ENDURANCE_ITEMINFO));
		msg.Category = MP_TITAN;
		msg.Protocol = MP_TITAN_ADDNEW_EQUIP_FROMITEM;
		msg.dwObjectID = m_pMaster->GetID();

		m_pMaster->SendMsg(&msg, sizeof(SEND_ADDTITANEQUIP_FROMITEM));
	}

	//TITAN_ENDURANCE_ITEMINFO* pNewInfo = m_ItemEndurancePool.Alloc();
	TITAN_ENDURANCE_ITEMINFO* pNewInfo = new TITAN_ENDURANCE_ITEMINFO;
	if( NULL == pNewInfo )
	{
		ASSERT(0);
		return;
	}
	memcpy(pNewInfo, pEnduranceInfo, sizeof(TITAN_ENDURANCE_ITEMINFO));

	if( FALSE == AddTitanEquipItemEnduranceInfoList(pNewInfo) )
	{
		SAFE_DELETE(pNewInfo);
	}
}

BOOL CTitanManager::AddTitanEquipItemEnduranceInfoList( TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo )
{
	if(m_ItemEnduranceList.GetData(pEnduranceInfo->ItemDBIdx))
	{
		return FALSE;
	}

	m_ItemEnduranceList.Add(pEnduranceInfo, pEnduranceInfo->ItemDBIdx);
	return TRUE;
}


TITAN_ENDURANCE_ITEMINFO* CTitanManager::GetTitanItemEnduranceInfo(DWORD equipItemDBIdx)
{
	return m_ItemEnduranceList.GetData(equipItemDBIdx);
}

void CTitanManager::RemoveTitanEquipItemInfo( DWORD equipItemDBIdx )
{// 교환시 리스트 상에서만 삭제
	TITAN_ENDURANCE_ITEMINFO* pInfo = m_ItemEnduranceList.GetData(equipItemDBIdx);

	ASSERT(pInfo);

	m_ItemEnduranceList.Remove(equipItemDBIdx);
}

void CTitanManager::DeleteTitanEquipItemInfo( DWORD equipItemDBIdx )
{// 장착아이템 파괴, 또는 삭제시.
	TITAN_ENDURANCE_ITEMINFO* pInfo = m_ItemEnduranceList.GetData(equipItemDBIdx);

	ASSERT(pInfo);
	SAFE_DELETE(pInfo);

	m_ItemEnduranceList.Remove(equipItemDBIdx);
}

WORD CTitanManager::GetTitanEquipItemInfoList( TITAN_ENDURANCE_ITEMINFO* RtInfo )
{
	WORD infoCount = 0;
	m_ItemEnduranceList.SetPositionHead();
	TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
	while(pInfo = m_ItemEnduranceList.GetData())
	{
		memcpy(&RtInfo[infoCount], pInfo, sizeof(TITAN_ENDURANCE_ITEMINFO));
		++infoCount;
	}

	return infoCount;
}

void CTitanManager::DiscardTitanEquipItem( POSTYPE whatPos, WORD whatItemIdx, DWORD titanEquipItemDBIdx )
{
	if( EI_TRUE == ITEMMGR->DiscardItem(m_pMaster, whatPos, whatItemIdx, 1) )
	{
		DeleteTitanEquip(titanEquipItemDBIdx);

		MSG_ITEM_DISCARD_ACK msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_TITAN_DISCARD_EQUIPITEM;
		msg.wItemIdx = whatItemIdx;
		msg.TargetPos = whatPos;
		msg.ItemNum = 1;
		m_pMaster->SendMsg( &msg, sizeof(msg) );
	}
	else
	{
		ASSERT(0);		
	}
}

//== 타이탄 사용중 장착 아이템 관련 (for DBUpdate)
void CTitanManager::AddTitanUsingEquipItemList(TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo)
{
	if( !pEnduranceInfo )	return;

	if( m_ItemUsingEnduranceList.GetData(pEnduranceInfo->ItemDBIdx) )
	{
		return;
	}

	m_ItemUsingEnduranceList.Add(pEnduranceInfo, pEnduranceInfo->ItemDBIdx);
}

void CTitanManager::RemoveTitanUsingEquipItemList(DWORD equipItemDBIdx)
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = m_ItemUsingEnduranceList.GetData(equipItemDBIdx);

	//ASSERT(pInfo);

	if(pInfo)
		m_ItemUsingEnduranceList.Remove(equipItemDBIdx);

}

void CTitanManager::UpdateUsingEquipItemEnduranceInfo()
{
	m_ItemUsingEnduranceList.SetPositionHead();
	TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
	while(pInfo = m_ItemUsingEnduranceList.GetData())
	{
		TitanEquipItemInfoUpdateToDB(pInfo);
	}
}

void CTitanManager::GetAppearanceInfo( TITAN_APPEARANCEINFO* RtInfo )
{
	if( m_pCurRidingTitan )
	{
		RtInfo->TitanKind = (WORD)(m_pCurRidingTitan->GetTitanKind());
		RtInfo->TitanGrade = (WORD)(m_pCurRidingTitan->GetTitanGrade());
		RtInfo->TitanScale = (WORD)(m_pCurRidingTitan->GetTotalInfo()->Scale);
	}
	for( int i = 0; i < eTitanWearedItem_Max; ++ i )
	{
		RtInfo->WearedItemIdx[i] = GetWearedInfo( i )->TitanEquipItemIdx;
	}
}

BOOL CTitanManager::CheckUsingEquipItemNum()
{// 갯수. 장착템 전부 있는지
	BOOL bTrue = (m_ItemUsingEnduranceList.GetDataNum() == eTitanWearedItem_Max);
	return bTrue;
}

BOOL CTitanManager::CheckUsingEquipItemEndurance()
{// 내구도 1% 2개 이하인지
	DWORD perc = 0;
	int cnt = 0;
	TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
	m_ItemUsingEnduranceList.SetPositionHead();
	while(pInfo = m_ItemUsingEnduranceList.GetData())
	{
		perc = pInfo->Endurance * 10000 / TITAN_EQUIPITEM_ENDURANCE_MAX ;
		if( perc < 100 )	// 1% 미만
			cnt++;
	}

	m_bAvaliableEndurance = ( cnt < 3 );
	return m_bAvaliableEndurance;
}

// 장착아이템 내구도 증감
void CTitanManager::DoRandomDecrease( int flgPoint )
{
	DWORD Rnd = rand()%100;

	// 1단계 확률.
	if( Rnd > GAMERESRCMNGR->GetTitanRule()->Prob_DecreaseEndurance )
		return;

	Rnd = rand()%100;

	DWORD targetItemDBIdx = 0;

	if( AtAtk == flgPoint )
	{// 2단계 확률.
		if( Rnd > GAMERESRCMNGR->GetTitanRule()->Prob_DecreaseArmorEndurance_Atk )
		{
			//무기 내구력 하락
			targetItemDBIdx = GetWearedInfo(eTitanWearedItem_Weapon)->TitanEquipItemDBIdx;
		}
		else
		{
			//방어구 내구력 하락
			targetItemDBIdx = GetWearedInfo( rand()% eTitanWearedItem_Weapon )->TitanEquipItemDBIdx;
		}
	}
	else if( AtDef == flgPoint )
	{
		if( Rnd > GAMERESRCMNGR->GetTitanRule()->Prob_DecreaseArmorEndurance_Def )
		{
			//무기 내구력 하락
			targetItemDBIdx = GetWearedInfo(eTitanWearedItem_Weapon)->TitanEquipItemDBIdx;
		}
		else
		{
			//방어구 내구력 하락
			targetItemDBIdx = GetWearedInfo( rand()% eTitanWearedItem_Weapon )->TitanEquipItemDBIdx;
		}	
	}

	if( 0 == targetItemDBIdx )
	{
		int i = 0;
		while(!targetItemDBIdx)
		{
			targetItemDBIdx = GetWearedInfo(i)->TitanEquipItemDBIdx;
			++i;

			if(i > eTitanWearedItem_Max)
				break;
		}
	}

	if(targetItemDBIdx)
	{
		TITAN_ENDURANCE_ITEMINFO* pInfo = GetTitanItemEnduranceInfo(targetItemDBIdx);

		if(pInfo)
		{
			DWORD amount = GetDecreaseEnduranceFromItemIdx(pInfo->ItemIdx);
			ASSERT(amount);
			MinusItemEndurance( targetItemDBIdx, amount );
		}
	}
}

DWORD CTitanManager::GetDecreaseEnduranceFromItemIdx( DWORD titanEquipItemIdx )
{
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)titanEquipItemIdx );

	if(pItemInfo)
	{
		return pItemInfo->AllPlus_Kind;
	}
	else
		return 0;
}

void CTitanManager::PlusItemEndurance( DWORD titanEquipItemDBIdx, DWORD increaseAmount )
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = GetTitanItemEnduranceInfo(titanEquipItemDBIdx);

	if(!pInfo)
	{
		ASSERT(0);
		return;
	}

	if( 0 == increaseAmount )
		return;

	// 070621 기획변경
	DWORD tmp = pInfo->Endurance;
	
	pInfo->Endurance += increaseAmount;

	if(pInfo->Endurance > TITAN_EQUIPITEM_ENDURANCE_MAX)
		pInfo->Endurance = TITAN_EQUIPITEM_ENDURANCE_MAX;

	//TitanEquipItemInfoUpdateToDB(pInfo);

	//if( increaseAmount ) %바뀔때만 보내도록 변경
	if( m_pMaster->GetUserLevel() > eUSERLEVEL_GM )
	if( (tmp*100)/TITAN_EQUIPITEM_ENDURANCE_MAX != ((pInfo->Endurance)*100)/TITAN_EQUIPITEM_ENDURANCE_MAX )
	{
		TitanEquipItemInfoUpdateToDB(pInfo);
		// 내구도 정보 보내기
		SendItemEnduranceInfo(pInfo);
	}
}

void CTitanManager::MinusItemEndurance( DWORD titanEquipItemDBIdx, DWORD decreaseAmount )
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = GetTitanItemEnduranceInfo(titanEquipItemDBIdx);

	if(!pInfo)
	{
		ASSERT(0);
		return;
	}

	if( 0 == decreaseAmount )
		return;

	DWORD tmp = pInfo->Endurance;
	// 070621 기획변경
	/*

	if( tmp - decreaseAmount > pInfo->Endurance )
	{
		pInfo->Endurance = 0;
		// 해당 아이템 삭제.
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)pInfo->ItemIdx );
		POSTYPE pos = TP_TITANWEAR_START + (pItemInfo->ItemKind - eTITAN_EQUIPITEM_HELMET);

		DiscardTitanEquipItem(pos,pItemInfo->ItemIdx,titanEquipItemDBIdx);
	}
	else
		pInfo->Endurance -= decreaseAmount;*/
	if( pInfo->Endurance < decreaseAmount )
	{
		pInfo->Endurance = 0;
		STATSMGR->CalcTitanItemStats(m_pMaster);
	}
	else
		pInfo->Endurance -= decreaseAmount;

	//TitanEquipItemInfoUpdateToDB(pInfo);

	//if( decreaseAmount )
	if( m_pMaster->GetUserLevel() > eUSERLEVEL_GM )
	if( (tmp*100)/TITAN_EQUIPITEM_ENDURANCE_MAX != ((pInfo->Endurance)*100)/TITAN_EQUIPITEM_ENDURANCE_MAX )
	{
		TitanEquipItemInfoUpdateToDB(pInfo);
		// 내구도 정보 보내기
		SendItemEnduranceInfo(pInfo);
	}
}

void CTitanManager::SendItemEnduranceInfo( TITAN_ENDURANCE_ITEMINFO* pInfo )
{
	MSG_DWORD2 msg;
	msg.Category = MP_TITAN;
	msg.Protocol = MP_TITAN_ENDURANCE_UPDATE;
	msg.dwObjectID = m_pMaster->GetID();
	msg.dwData1 = pInfo->ItemDBIdx;
	msg.dwData2 = pInfo->Endurance;

	m_pMaster->SendMsg(&msg, sizeof(msg));
}

void CTitanManager::SetWearedInfo( POSTYPE pos, ITEMBASE* pItemBase, BOOL bIn )
{
	WORD wearNum = pos - TP_TITANWEAR_START;

	ASSERT(wearNum>=0 && wearNum<eTitanWearedItem_Max);

	WORD itemIdx = 0;
	DWORD itemDBIdx = 0;

	if(pItemBase)
	{
		itemIdx = pItemBase->wIconIdx;
		itemDBIdx = pItemBase->dwDBIdx;
	}
	else
	{
		itemIdx = 0;
		itemDBIdx = 0;
	}

	TITAN_ENDURANCE_ITEMINFO* pInfo = GetTitanItemEnduranceInfo(pItemBase->dwDBIdx);
	if(!pInfo)
	{
///		ASSERT(0);
	}

	if(bIn)
	{
		AddTitanUsingEquipItemList(pInfo);
		m_TitanWearedInfo[wearNum].TitanEquipItemIdx = itemIdx;
		m_TitanWearedInfo[wearNum].TitanEquipItemDBIdx = itemDBIdx;
	}
	else
	{
		RemoveTitanUsingEquipItemList(pItemBase->dwDBIdx);
		m_TitanWearedInfo[wearNum].TitanEquipItemIdx = 0;
		m_TitanWearedInfo[wearNum].TitanEquipItemDBIdx = 0;
	}
}

TitanWearedInfo* CTitanManager::GetWearedInfo( int equipKind )
{
	return &m_TitanWearedInfo[equipKind];
}

void CTitanManager::ExchangeTitanEquipItem( DWORD dwItemDBIdx, CPlayer* pNewOwner )
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = GetTitanItemEnduranceInfo(dwItemDBIdx);
	if(!pInfo)
	{
		ASSERT(0);
		return;
	}

	if( pInfo->CharacterID != m_pMaster->GetID() )
	{
		ASSERT(0);
	}

	pInfo->CharacterID = pNewOwner->GetID();

	TitanEquipItemInfoUpdateToDB(pInfo);	// magi82 - DB에 저장

	pNewOwner->GetTitanManager()->AddTitanEquipItemEnduranceInfoList(pInfo);	// magi82 - 새로운 아이템 주인의 리스트에 추가

	RemoveTitanEquipItemInfo(pInfo->ItemDBIdx);		// magi82 - 예전주인의 리스트에서 삭제
}

void CTitanManager::SendTitanStats()
{
	SEND_TITANSTATINFO msg;
	msg.Category = MP_TITAN;
	msg.Protocol = MP_TITAN_STATINFO;
	msg.dwObjectID = m_pMaster->GetID();
	msg.Info = m_titanItemStats;

	m_pMaster->SendMsg(&msg, sizeof(SEND_TITANSTATINFO));
}

void CTitanManager::SendTitanPartsChange()
{
	SEND_TITAN_APPEARANCEINFO InfoMsg;
	InfoMsg.Category = MP_ITEM;
	InfoMsg.Protocol = MP_ITEM_TITAN_APPEARANCE_CHANGE;
	InfoMsg.OwnerID = m_pMaster->GetID();
    GetAppearanceInfo(&InfoMsg.titanAppearanceInfo);

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(m_pMaster, &InfoMsg, sizeof(InfoMsg));
}

// magi82 - Titan(070619)
void CTitanManager::SetTitanRepairEquipItem( CPlayer* pPlayer, MSG_DWORD2* pmsg )
{
	// 아이템 유효성 검사
	if( !CHKRT->ItemOf(pPlayer, (POSTYPE)pmsg->dwData2, (WORD)pmsg->dwData1, 0, 0, CB_EXIST|CB_ICONIDX) )
	{
		ASSERT(0);
		return;
	}

	ITEMBASE* pItemBase = (ITEMBASE*)ITEMMGR->GetItemInfoAbsIn(pPlayer, (POSTYPE)pmsg->dwData2);
	DWORD dwDBIdx = pItemBase->dwDBIdx;
	TITAN_ENDURANCE_ITEMINFO* pEndurance = GetTitanItemEnduranceInfo(dwDBIdx);
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);

	if(pEndurance->Endurance < TITAN_EQUIPITEM_ENDURANCE_MAX)
	{
        // magi82 - Titan(070619) 수리비 관련
		float fEnduranceRate = (float)pEndurance->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
		if( fEnduranceRate > 0.0f && fEnduranceRate < 1.0f )
		{
			fEnduranceRate = 1.0f;
		}
		DWORD dwRepairPrice = (DWORD)( (100 - (DWORD)fEnduranceRate) * (pInfo->BuyPrice / 100) * 0.09f );

		if( pPlayer->GetMoney() < dwRepairPrice )
		{
			// 클라에게 수리비가 부족하다고 보냄
			MSG_DWORD msg;
			SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_NACK );
			msg.dwObjectID = pPlayer->GetID();
			msg.dwData = 0;	// 0이면 수리비 부족, 1이면 해당아이템의 내구도 풀, 2면 모든 아이템의 내구도 풀
			pPlayer->SendMsg(&msg,sizeof(msg));
		}
		else
		{
			// 수리비 지불
			pPlayer->SetMoney(dwRepairPrice, MONEY_SUBTRACTION);

			// 수리
			pEndurance->Endurance = TITAN_EQUIPITEM_ENDURANCE_MAX;

			// DB에 갱신
			TitanEquipItemInfoUpdateToDB(pEndurance);

			// 클라에게 성공메세지 보냄
			MSG_TITAN_REPAIR_EQUIPITEM_SYN msg;
			SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_ACK );
			msg.dwObjectID = pPlayer->GetID();
			CopyMemory( &msg.RepairInfo, pEndurance, sizeof(TITAN_ENDURANCE_ITEMINFO) );
			pPlayer->SendMsg(&msg,sizeof(msg));
		}
	}
	else
	{
		// 클라에게 이미 내구도가 풀인 아이템이라고 알려줌
		MSG_DWORD msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_NACK );
		msg.dwObjectID = pPlayer->GetID();
		msg.dwData = 1;	// 0이면 수리비 부족, 1이면 해당아이템의 내구도 풀, 2면 모든 아이템의 내구도 풀
		pPlayer->SendMsg(&msg,sizeof(msg));
	}
}

// magi82 - Titan(070619)
void CTitanManager::SetTitanRepairTotalEquipItem( CPlayer* pPlayer, MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN* pmsg )
{
	// 아이템 유효성 검사
	for( WORD i = 0; i < pmsg->wRepairNum; i++ )
	{
		if( !CHKRT->ItemOf(pPlayer, (POSTYPE)pmsg->sRepairInfo[i].dwItemPos, (WORD)pmsg->sRepairInfo[i].dwItemIdx, 0, 0, CB_EXIST|CB_ICONIDX) )
		{
			ASSERT(0);
			return;
		}
	}

	MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_ACK msg;
	msg.Init();
	SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_TOTAL_ACK );
	msg.dwObjectID = pPlayer->GetID();
	DWORD dwMoney = 0;

	// 수리비 계산
	ITEMBASE* pItemBase = NULL;
	TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = NULL;
	ITEM_INFO* pInfo = NULL;

	for( WORD i = 0; i < pmsg->wRepairNum; i++ )
	{
		pItemBase = (ITEMBASE*)ITEMMGR->GetItemInfoAbsIn(pPlayer, pmsg->sRepairInfo[i].dwItemPos);
		if( !pItemBase )
		{
			ASSERT(0);
			continue;
		}

		pEnduranceInfo = GetTitanItemEnduranceInfo(pItemBase->dwDBIdx);
		if( !pEnduranceInfo )
		{
			ASSERT(0);
			continue;
		}

		pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
		if( !pInfo )
		{
			ASSERT(0);
			continue;
		}

		if(pEnduranceInfo->Endurance < TITAN_EQUIPITEM_ENDURANCE_MAX)
		{
			float fEnduranceRate = (float)pEnduranceInfo->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
			if( fEnduranceRate > 0.0f && fEnduranceRate < 1.0f )
			{
				fEnduranceRate = 1.0f;
			}
			DWORD dwRepairPrice = (DWORD)( (100 - (DWORD)fEnduranceRate) * (pInfo->BuyPrice / 100) * 0.09f );

			dwMoney += dwRepairPrice;
		}
	}

	if( pPlayer->GetMoney() < dwMoney )
	{
		// 클라에게 수리비가 부족하다고 보냄
		MSG_DWORD msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_NACK );
		msg.dwObjectID = pPlayer->GetID();
		msg.dwData = 0;	// 0이면 수리비 부족, 1이면 해당아이템의 내구도 풀, 2면 모든 아이템의 내구도 풀
		pPlayer->SendMsg(&msg,sizeof(msg));
		return;
	}

	// 수리
	for( WORD i = 0; i < pmsg->wRepairNum; i++ )
	{
		ITEMBASE* pItemBase = (ITEMBASE*)ITEMMGR->GetItemInfoAbsIn(pPlayer, pmsg->sRepairInfo[i].dwItemPos);
		TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = GetTitanItemEnduranceInfo(pItemBase->dwDBIdx);

		if(pEnduranceInfo->Endurance < TITAN_EQUIPITEM_ENDURANCE_MAX)
		{
			// 수리
            pEnduranceInfo->Endurance = TITAN_EQUIPITEM_ENDURANCE_MAX;

			// DB에 갱신
			TitanEquipItemInfoUpdateToDB(pEnduranceInfo);

			msg.AddRepairInfo(pEnduranceInfo);
		}
	}

	if(msg.wRepairNum == 0)
	{
		// 클라에게 이미 내구도가 풀인 아이템이라고 알려줌
		MSG_DWORD msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_NACK );
		msg.dwObjectID = pPlayer->GetID();
		msg.dwData = 2;// 0이면 수리비 부족, 1이면 해당아이템의 내구도 풀, 2면 모든 아이템의 내구도 풀
		pPlayer->SendMsg(&msg,sizeof(msg));
	}
	else
	{
		// 수리비 지불
		pPlayer->SetMoney(dwMoney, MONEY_SUBTRACTION);
		pPlayer->SendMsg(&msg,msg.GetSize());
	}
}
//////////////////////////////////////////////////////////////////////////

///// 2007. 9. 13. CBH
BOOL CTitanManager::TitanRecallStartSyn( DWORD callItemDBIdx, BOOL bSummonFromUser/* = TRUE*/ )	//타이탄 소환 시작
{
	if( FALSE == CheckBeforeRideInTitan(callItemDBIdx) )
		return FALSE;

	MSGBASE msg;
	msg.Category = MP_TITAN;
	msg.Protocol = MP_TITAN_RECALL_ACK;
	msg.dwObjectID = m_pMaster->GetID();
			
	PACKEDDATA_OBJ->QuickSend(m_pMaster, &msg, sizeof(msg));
	
	return TRUE;
}

void CTitanManager::TitanRecallProcess()
{
	if(m_bTitanRecall == FALSE)
	{
		return;
	}

	m_dwCurrentTime = gCurTime;
	if(m_dwTitanRecallProcessTime < m_dwCurrentTime)		
	{
		//클라이언트에서 카운트가 완료되면 탑승
		if(m_bTitanRecallClient == TRUE)
		{
			RideInTitan();
			InitTitanRecall();		
		}		
	}
}

void CTitanManager::InitTitanRecall()
{
	m_dwTitanRecallProcessTime = 0;
	m_dwCurrentTime = 0;
	m_bTitanRecall = FALSE;
	m_bTitanRecallClient = FALSE;
}

void CTitanManager::SetRecallProcessTime(DWORD dwRecallProcessTime)
{
	m_dwTitanRecallProcessTime = dwRecallProcessTime;
}

void CTitanManager::StartTitanRecall()
{	
	m_dwCurrentTime = gCurTime;
	m_bTitanRecall = TRUE;
}

BOOL CTitanManager::IsTitanRecall()
{
	return m_bTitanRecall;
}

// 타이탄 탑승
BOOL CTitanManager::RideInTitan()
{
	DWORD callItemDBIdx = GetRegistTitanCallItemDBIdx();
	TITAN_TOTALINFO* pInfo = GetTitanTotalInfo(callItemDBIdx);	

	if( pInfo == NULL)
	{
		return FALSE;
	}

	if(NULL == m_pCurRidingTitan)
	{
		//m_pCurRidingTitan = new CTitan;	// 메모리풀로 바꾸던지..
		m_pCurRidingTitan = g_pServerSystem->MakeTitan(m_pMaster);
	}
	else
	{
		ASSERT(0);
	}

	// magi82(23)
	stTIME stMaintainTime, stCurTime;
	DWORD dwMilliSec = GAMERESRCMNGR->GetTitanRule()->TitanMaintainTime + m_pMaster->GetAbilityStats()->dwTitanRidingPlusTime;
	DWORD dwSec = 0;
	DWORD dwMin = 0;

	dwSec = dwMilliSec / SECTOMILLISEC;
	dwMin = dwSec / MINUTETOSECOND;
	dwSec = dwSec % MINUTETOSECOND;

	stMaintainTime.SetTime(0,0,0,0,dwMin,dwSec);
	stCurTime.SetTime(GetCurTime());
	stCurTime += stMaintainTime;
	m_dwTitanMaintainTime = stCurTime.value;

	pInfo->MaintainTime = m_dwTitanMaintainTime;
	m_pCurRidingTitan->InitTitan(pInfo, m_pMaster);

	//클라이언트로 타이탄 정보 보내기 전 세팅.
	SetTitanStats();

	// 유효 그리드 정보 보내기
	MSG_TITAN_RIDEIN msg;
	msg.Category = MP_TITAN;
	msg.Protocol = MP_TITAN_RIDEIN_ACK;
	msg.dwObjectID = m_pMaster->GetID();
	msg.OwnerID = m_pMaster->GetID();
	msg.MaintainTime = dwMilliSec;
	msg.TitanTotalInfo = *pInfo;
	GetAppearanceInfo(&msg.AppearanceInfo);

	PACKEDDATA_OBJ->QuickSend( m_pMaster, &msg, sizeof(msg) );
	
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////

void CTitanManager::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_TITAN_RECALL_SYN:
		{
			m_bTitanRecallClient = TRUE;
		}		
		break;
	case MP_TITAN_RECALL_CANCEL_SYN:
		{
			//타이탄 소환 카운트중이면 취소 완료
			if(m_bTitanRecall == TRUE)
			{
				InitTitanRecall();

				MSGBASE msg;
				msg.Category = MP_TITAN;
				msg.Protocol = MP_TITAN_RECALL_CANCEL_ACK;
				msg.dwObjectID = m_pMaster->GetID();
								
				PACKEDDATA_OBJ->QuickSend(m_pMaster, &msg, sizeof(msg));
			}
			else
			{
				MSGBASE msg;
				msg.Category = MP_TITAN;
				msg.Protocol = MP_TITAN_RECALL_CANCEL_NACK;
				msg.dwObjectID = m_pMaster->GetID();

				m_pMaster->SendMsg(&msg, sizeof(msg));				
			}
		}
		break;	
	default:
		break;
	}
}

void CTitanManager::MoveTitanEquipItemUpdateToDB(ITEMBASE* pFromItem, ITEMBASE* pToItem, POSTYPE FromPos, POSTYPE ToPos)
{
	// From -> To
	ITEM_INFO* pFromItemInfo = ITEMMGR->GetItemInfo(pFromItem->wIconIdx);
	if(pFromItemInfo && pFromItemInfo->ItemKind & eTITAN_EQUIPITEM)
	{
		TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = m_pMaster->GetTitanManager()->GetTitanItemEnduranceInfo(pFromItem->dwDBIdx);
		if( pEnduranceInfo )
		{
			BOOL bFlag = FALSE;

			// 창고에서 인벤으로 옮길때(옮기려는 인벤의 슬롯은 비어있다.)
			if( (TP_INVENTORY_START <= pFromItem->Position && pFromItem->Position < TP_INVENTORY_END) && (TP_PYOGUK_START <= FromPos && FromPos < TP_PYOGUK_END) )
			{
				pEnduranceInfo->CharacterID = m_pMaster->GetID();
				pEnduranceInfo->UserIdx = 0;	// 창고에 있을때만 세팅
				bFlag = TRUE;
			}
			// 인벤에서 창고로 옮길때(옮기려는 창고의 슬롯은 비어있다.)
			else if( (TP_PYOGUK_START <= pFromItem->Position && pFromItem->Position < TP_PYOGUK_END) && (TP_INVENTORY_START <= FromPos && FromPos < TP_INVENTORY_END) )
			{
				pEnduranceInfo->CharacterID = 0;	// 캐릭이 가지고 있을때만 세팅
				pEnduranceInfo->UserIdx = m_pMaster->GetUserID();
				bFlag = TRUE;
			}

			if( bFlag == TRUE )
			{
				TitanEquipItemInfoUpdateToDB( pEnduranceInfo );
			}
		}
	}

	// ToPos -> FromPos
	ITEM_INFO* pToItemInfo = ITEMMGR->GetItemInfo(pToItem->wIconIdx);
	if(pToItemInfo && pToItemInfo->ItemKind & eTITAN_EQUIPITEM)
	{
		TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = m_pMaster->GetTitanManager()->GetTitanItemEnduranceInfo(pToItem->dwDBIdx);
		if( pEnduranceInfo )
		{
			BOOL bFlag = FALSE;

			// 창고에서 인벤으로 옮길때(옮기려는 인벤의 슬롯은 비어있다.)
			if( (TP_INVENTORY_START <= pToItem->Position && pToItem->Position < TP_INVENTORY_END) && (TP_PYOGUK_START <= ToPos && ToPos < TP_PYOGUK_END) )
			{
				pEnduranceInfo->CharacterID = m_pMaster->GetID();
				pEnduranceInfo->UserIdx = 0;	// 창고에 있을때만 세팅
				bFlag = TRUE;
			}
			// 인벤에서 창고로 옮길때(옮기려는 창고의 슬롯은 비어있다.)
			else if( (TP_PYOGUK_START <= pToItem->Position && pToItem->Position < TP_PYOGUK_END) && (TP_INVENTORY_START <= ToPos && ToPos < TP_INVENTORY_END) )
			{
				pEnduranceInfo->CharacterID = 0;	// 캐릭이 가지고 있을때만 세팅
				pEnduranceInfo->UserIdx = m_pMaster->GetUserID();
				bFlag = TRUE;
			}

			if( bFlag == TRUE )
			{
				TitanEquipItemInfoUpdateToDB( pEnduranceInfo );
			}
		}
	}
}

// magi82(26)
void CTitanManager::SetTitanShopitemOption( DWORD dwItemIdx, BOOL bAdd )
{
    ITEM_INFO* pInfo = ITEMMGR->GetItemInfo((WORD)dwItemIdx);
	if( !pInfo || pInfo->ItemKind != eSHOP_ITEM_TITAN_EQUIP )
		return;

	// bAdd가 TRUE 이면 더해주고 FALSE 이면 빼준다.
	int nAddMark = 1;
	if( bAdd == FALSE )
		nAddMark = -1;

	if( pInfo->GenGol )
	{
		m_TitanShopitemOption.dwSummonReduceTime += (pInfo->GenGol * nAddMark);

		if( m_TitanShopitemOption.dwSummonReduceTime < 0 )
			m_TitanShopitemOption.dwSummonReduceTime = 0;
	}

	if( pInfo->AttrRegist.GetElement_Val(ATTR_FIRE) )
	{
        m_TitanShopitemOption.dwRecallReduceRate += (pInfo->AttrRegist.GetElement_Val(ATTR_FIRE) * nAddMark);

		if( m_TitanShopitemOption.dwRecallReduceRate < 0.0f )
			m_TitanShopitemOption.dwRecallReduceRate = 0.0f;
	}

	if( pInfo->AttrRegist.GetElement_Val(ATTR_WATER) )
	{
        m_TitanShopitemOption.dwEPReduceRate += (pInfo->AttrRegist.GetElement_Val(ATTR_WATER) * nAddMark);

		if( m_TitanShopitemOption.dwEPReduceRate < 0.0f )
			m_TitanShopitemOption.dwEPReduceRate = 0.0f;
	}
}
