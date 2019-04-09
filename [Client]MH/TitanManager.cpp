// TitanManager.cpp: implementation of the CTitanManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Titan.h"
#include "TitanManager.h"
#include "MHFile.h"
#include "GameResourceStruct.h"
#include "GameResourceManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "TitanInventoryDlg.h"
#include "ObjectManager.h"
#include "AppearanceManager.h"
#include "TitanGuageDlg.h"
#include "TitanRecallDlg.h"
#include "MugongManager.h"
#include "CheatMsgParser.h"
#include "MHCamera.h"
#include "ObjectBalloon.h"
#include "ObjectStateManager.h"
#include "MHMap.h"
#include "StatsCalcManager.h"
#include "QuickDialog.h"
#include "PeaceWarModeManager.h"
#include "BattleSystem_Client.h"
#include "StatusIconDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CTitanManager)
CTitanManager::CTitanManager()
{
	// 파츠 제작 테이블
	m_TPMItemTable.Initialize(MAX_ITEM_NUM);

	// 파츠 속성 테이블
	m_TitanPartsKindPool = new CMemoryPoolTempl<TITAN_PARTS_KIND>;
	m_TitanPartsKindPool->Init( 100, 10, "TitanPartsKind" );
	m_TitanPartsKindTable.Initialize( 100 );

	// 타이탄 업그레이드 테이블
	m_TitanUpgradeInfoTable.Initialize(100);

	// 타이탄 분해 테이블
	m_TitanBreakInfoTable.Initialize(MAX_ITEM_NUM);

	m_TitanInfoList.Initialize(4);
	m_ItemEnduranceList.Initialize(4);
	m_ItemUsingEnduranceList.Initialize(4);

	TitanMgrInit();
}

CTitanManager::~CTitanManager()
{
	// 파츠 제작 테이블 릴리즈
	ITEM_MIX_INFO* pInfo;
	m_TPMItemTable.SetPositionHead();
	while(pInfo = m_TPMItemTable.GetData())
	{
		if(pInfo->psResultItemInfo)
		{
			for(int i = 0 ; i < pInfo->wResultItemNum ; ++i)
			{
				if(pInfo->psResultItemInfo[i].wResItemIdx != 0)
				{
					if(pInfo->psResultItemInfo[i].psMaterialItemInfo)
					{
						delete [] pInfo->psResultItemInfo[i].psMaterialItemInfo;
						pInfo->psResultItemInfo[i].psMaterialItemInfo = NULL;
					}
				}
			}

			delete [] pInfo->psResultItemInfo;
			pInfo->psResultItemInfo = NULL;
		}
		delete pInfo;
		pInfo = NULL;
	}
	m_TPMItemTable.RemoveAll();
	//////////////////////////////////////////////////////////////////////////


	// 파츠 속성 테이블 릴리즈
	m_TitanPartsKindTable.SetPositionHead();
	TITAN_PARTS_KIND* pTitanPartsKind = NULL;
	while( pTitanPartsKind = m_TitanPartsKindTable.GetData() )
	{
		m_TitanPartsKindPool->Free( pTitanPartsKind );
	}
	m_TitanPartsKindTable.RemoveAll();

	SAFE_DELETE(m_TitanPartsKindPool);
	//////////////////////////////////////////////////////////////////////////

	// 타이탄 업그레이드 테이블 릴리즈
	TITAN_UPGRADEINFO* pTitanInfo;
	m_TitanUpgradeInfoTable.SetPositionHead();
	while(pTitanInfo = m_TitanUpgradeInfoTable.GetData())
	{
		delete pTitanInfo;
		pTitanInfo = NULL;
	}
	m_TitanUpgradeInfoTable.RemoveAll();
	//////////////////////////////////////////////////////////////////////////

	// 타이탄 분해 테이블 릴리즈
	TITAN_BREAKINFO* pTitanBreakInfo;
	m_TitanBreakInfoTable.SetPositionHead();
	while(pTitanBreakInfo = m_TitanBreakInfoTable.GetData())
	{
		delete pTitanBreakInfo;
		pTitanBreakInfo = NULL;
	}
	m_TitanBreakInfoTable.RemoveAll();
	//////////////////////////////////////////////////////////////////////////

	Release();
}

void CTitanManager::TitanMgrInit()
{
	// 타이탄 분해 확률변수 초기화
	m_TitanBreakRate = 0;
	m_RegistedTitanItemDBIdx = 0;	// 등록된 타이탄아이템의 DB인덱스 초기화
	m_TitanRefairItem = NULL;

	m_dwRecallCheckTime = 0;
	m_bAvaliableEndurance = FALSE;

	ZeroMemory(&m_TitanShopitemOption, sizeof(TITAN_SHOPITEM_OPTION));	// magi82(26)
}

void CTitanManager::TitanMgrRelease()
{
	TitanMgrInit();
	Release();
	m_ItemUsingEnduranceList.RemoveAll();
	ReleaseTitanEquipInfo();
	ReleaseTitanInfoList();

	SAFE_DELETE(m_pCurRidingTitan);

	ZeroMemory(&m_TitanShopitemOption, sizeof(TITAN_SHOPITEM_OPTION));	// magi82(26)
}

void CTitanManager::Release()
{

}

void CTitanManager::LoadTPMResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo)
{
	pResInfo->wResItemIdx = fp->GetWord();
	SafeStrCpy(pResInfo->szResItemName, fp->GetString(), MAX_ITEMNAME_LENGTH+1);
	pResInfo->wMixKind = fp->GetWord();
	ASSERT(pResInfo->wMixKind < 7);
	pResInfo->Money = fp->GetDword();
	pResInfo->SuccessRatio = fp->GetWord();
	pResInfo->wMaterialItemNum = fp->GetWord();
	pResInfo->psMaterialItemInfo = new ITEM_MIX_MATERIAL[pResInfo->wMaterialItemNum];
	for( int i = 0 ; i < pResInfo->wMaterialItemNum ; ++i)
	{
		pResInfo->psMaterialItemInfo[i].wMatItemIdx = fp->GetWord();
		SafeStrCpy(pResInfo->psMaterialItemInfo[i].szMatItemName, fp->GetString(), MAX_ITEMNAME_LENGTH+1);
		pResInfo->psMaterialItemInfo[i].wMatItemNum = fp->GetWord();
	}
}
void CTitanManager::LoadTPMList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/TitanMixList.bin", "rb"))
		return;
#else
	if(!file.Init("Resource/TitanMixList.txt", "rt"))
		return;
#endif

	ITEM_MIX_INFO * pInfo = NULL;
	WORD wTmpItemIdx = 0;
	WORD wTmpRestIdx = 0;
	WORD wTmpResultNum = 0;
	//	WORD count = 0;
	while(1)
	{
		if(file.IsEOF())
			break;

		//		count++;

		wTmpItemIdx = file.GetWord();
		pInfo = m_TPMItemTable.GetData(wTmpItemIdx);
		if(!pInfo)
		{
			pInfo = new ITEM_MIX_INFO;
			memset(pInfo, 0, sizeof(ITEM_MIX_INFO));
			pInfo->wItemIdx = wTmpItemIdx;
			SafeStrCpy(pInfo->szItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
			wTmpResultNum = file.GetWord();
			pInfo->wResultItemNum = wTmpResultNum;		
		}
		else
		{
			ASSERT(pInfo->wItemIdx == wTmpItemIdx);
			pInfo->wItemIdx = wTmpItemIdx;
			SafeStrCpy(pInfo->szItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
			wTmpResultNum = file.GetWord();
			ASSERT(pInfo->wResultItemNum == wTmpResultNum);
			pInfo->wResultItemNum = wTmpResultNum;
		}

		if(!pInfo->psResultItemInfo)
		{
			pInfo->psResultItemInfo = new ITEM_MIX_RES[pInfo->wResultItemNum];
			memset(pInfo->psResultItemInfo, 0, sizeof(ITEM_MIX_RES)*pInfo->wResultItemNum);
		}
		for(wTmpRestIdx = 0 ; wTmpRestIdx < pInfo->wResultItemNum ; ++wTmpRestIdx)
		{	
			if(pInfo->psResultItemInfo[wTmpRestIdx].wResItemIdx == 0)
			{
				LoadTPMResultItemInfo(&file, &pInfo->psResultItemInfo[wTmpRestIdx]);
				break;
			}
		}

		if(!m_TPMItemTable.GetData(pInfo->wItemIdx))
			m_TPMItemTable.Add(pInfo, pInfo->wItemIdx);
		pInfo = NULL;

	}
	file.Release();
}

ITEM_MIX_INFO * CTitanManager::GetTPMItemInfo(WORD wItemIdx)
{
	return m_TPMItemTable.GetData(wItemIdx);
}

BOOL CTitanManager::LoadTitanPartsKindList()
{
	TITAN_PARTS_KIND* pTemp = NULL;

	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("./Resource/TitanPartsKind.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/TitanPartsKind.txt", "rt"))
		return FALSE;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;

		pTemp = m_TitanPartsKindPool->Alloc();
		ZeroMemory(pTemp, sizeof(TITAN_PARTS_KIND));

		pTemp->dwPartsIdx = file.GetDword();
		pTemp->dwPartsKind = file.GetDword();
		pTemp->dwResultTitanIdx = file.GetDword();

		m_TitanPartsKindTable.Add(pTemp, pTemp->dwPartsIdx);
	}

	return TRUE;
}

TITAN_PARTS_KIND* CTitanManager::GetTitanPartsKind(  DWORD partsIdx  )
{
	return m_TitanPartsKindTable.GetData( partsIdx );
}

void CTitanManager::LoadTitanUpgradeInfoResultItemInfo(CMHFile * fp, TITAN_UPGRADE_MATERIAL * pMaterialInfo)
{
	pMaterialInfo->dwIndex = fp->GetDword();
	pMaterialInfo->dwCount = fp->GetDword();
}
void CTitanManager::LoadTitanUpgradeInfoList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/TitanUpgradeInfo.bin", "rb"))
		return;
#else
	if(!file.Init("Resource/TitanUpgradeInfo.txt", "rt"))
		return;
#endif

	TITAN_UPGRADEINFO * pInfo = NULL;
	DWORD dwTitanIdx = 0;

	while(1)
	{
		if(file.IsEOF())
			break;

		dwTitanIdx = file.GetDword();

		pInfo = new TITAN_UPGRADEINFO;
		ZeroMemory(pInfo, sizeof(TITAN_UPGRADEINFO));
		pInfo->dwTitanIdx = dwTitanIdx;
		pInfo->dwNextTitanIdx = file.GetDword();
		pInfo->dwMoney = file.GetDword();
		pInfo->wMaterial_Num = file.GetWord();

		if(pInfo->wMaterial_Num)
		{
			pInfo->pTitanUpgradeMaterial = new TITAN_UPGRADE_MATERIAL[pInfo->wMaterial_Num];
			ZeroMemory(pInfo->pTitanUpgradeMaterial, sizeof(TITAN_UPGRADE_MATERIAL)*pInfo->wMaterial_Num);
		}

		for( int i = 0; i < pInfo->wMaterial_Num; i++ )
			LoadTitanUpgradeInfoResultItemInfo(&file, &pInfo->pTitanUpgradeMaterial[i]);

		m_TitanUpgradeInfoTable.Add(pInfo, pInfo->dwTitanIdx);

		pInfo = NULL;
	}
	file.Release();
}

TITAN_UPGRADEINFO * CTitanManager::GetTitanUpgradeInfoItemInfo(DWORD titanIdx)
{
	return m_TitanUpgradeInfoTable.GetData(titanIdx);
}

void CTitanManager::LoadTitanBreakInfoMaterialInfo(CMHFile * fp, TITAN_BREAK_MATERIAL * pMaterialInfo)
{
	pMaterialInfo->dwMaterialIdx = fp->GetDword();
	pMaterialInfo->wMaterCnt = fp->GetWord();
	pMaterialInfo->wRate = fp->GetWord();
}

void CTitanManager::LoadTitanBreakInfoList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/TitanBreakList.bin", "rb"))
		return;
#else
	if(!file.Init("Resource/TitanBreakList.txt", "rt"))
		return;
#endif

	TITAN_BREAKINFO * pInfo = NULL;
	DWORD dwIdx = 0;

	while(1)
	{
		if(file.IsEOF())
			break;

		if(!m_TitanBreakRate)
			m_TitanBreakRate = file.GetDword();

		dwIdx = file.GetDword();

		pInfo = new TITAN_BREAKINFO;
		ZeroMemory(pInfo, sizeof(TITAN_BREAKINFO));
		pInfo->dwIdx = dwIdx;
		pInfo->dwMoney = file.GetDword();
		pInfo->wTotalCnt = file.GetWord();
		pInfo->wGetCnt = file.GetWord();

		if(pInfo->wTotalCnt)
		{
			pInfo->pTitanBreakMaterial = new TITAN_BREAK_MATERIAL[pInfo->wTotalCnt];
			ZeroMemory(pInfo->pTitanBreakMaterial, sizeof(TITAN_BREAK_MATERIAL)*pInfo->wTotalCnt);
		}

		for( int i = 0; i < pInfo->wTotalCnt; i++ )
			LoadTitanBreakInfoMaterialInfo(&file, &pInfo->pTitanBreakMaterial[i]);

		m_TitanBreakInfoTable.Add(pInfo, pInfo->dwIdx);

		pInfo = NULL;
	}
	file.Release();
}

TITAN_BREAKINFO * CTitanManager::GetTitanBreakInfoItemInfo(DWORD dwIdx)
{
	return m_TitanBreakInfoTable.GetData(dwIdx);
}

DWORD CTitanManager::GetCallItemDBIdx()
{
	if(GetCurRidingTitan())
	{
		TITAN_TOTALINFO* pInfo = GetCurRidingTitan()->GetTitanTotalInfo();
		if(pInfo)
			return pInfo->TitanCallItemDBIdx;
	}

	return 0;
}

void CTitanManager::ChangeTitanGuageFuel(int changeAmount)
{
	//DWORD newfuel = m_pCurRidingTitan->GetLife() + changeAmount;

	m_pCurRidingTitan->SetLife(changeAmount);
}

void CTitanManager::ChangeTitanGuageSpell(int changeAmount)
{
	//DWORD newSpell = m_pCurRidingTitan->GetNaeRyuk() + changeAmount;

	m_pCurRidingTitan->SetNaeRyuk(changeAmount);
}

BOOL CTitanManager::CheckRecallAvailable()
{
#ifdef _CHEATENABLE_
	if(CHEATMGR->IsCheatEnable())
		return TRUE;
#endif
	//BOOL bAvailable = FALSE;
	// 시간 제한
	//bAvailable = CheckRecallAvailable();
	// 시간 외 제한
	return CheckTimeRecallAvailable();
}

void CTitanManager::SetRecallRemainTime(DWORD RecallState, DWORD stTimeValue /*= 0*/)
{
	stTIME recall, client;

	client.SetTime(GetCurTime());

	// magi82(24)
	if( stTimeValue )
	{
		if( m_serverT.value < stTimeValue )
		{
			recall.SetTime(stTimeValue);

			recall -= m_serverT;	//gap
		}
		else
		{
			return;
		}
	}

	if( recall.value )
	{
		client += recall;	// 보정 완료.
		m_dwRecallCheckTime = client.value;
	}
	else
	{
		if(!m_RegistedTitanItemDBIdx)
		{
			m_dwRecallCheckTime = 0;
			return;
		}

		TITAN_TOTALINFO* pInfo = m_pCurRidingTitan->GetTitanTotalInfo();

		if(!pInfo)
		{
			ASSERT(0);
			return;
		}

		stTIME tmp,penalTime;

		DWORD penalSec = 0;
		DWORD penalMin = 0;

		TITANINFO_GRADE* pGrade = GAMERESRCMNGR->GetTitanGradeInfo(pInfo->TitanKind, pInfo->TitanGrade);
		if( !pGrade )
			return;

		// magi82(24)
		//if( MAP->GetMapNum() == 101
		//	|| MAP->GetMapNum() == 102
		//	|| MAP->GetMapNum() == 104
		//	|| MAP->GetMapNum() == 105 )
		if( MAP->IsMapKind(eTitanMap) )
		{
			// 판타지맵 일때
			if( RecallState == eExhaustFuel || RecallState == eMasterLifeRate )		// magi82(18)
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
			// 중원맵 일때
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
	}
}

BOOL CTitanManager::CheckTimeRecallAvailable()
{	
	return (GetCurTime() > m_dwRecallCheckTime);
	/*
	BOOL bAble = FALSE;

	if(!m_RegistedTitanItemDBIdx)	return bAble;

	TITAN_TOTALINFO* pInfo = GetTitanInfo(m_RegistedTitanItemDBIdx);

	if(!pInfo)
	{
		ASSERT(0);
		return bAble;
	}

	// 체력조건으로 해제된 경우
	if( 0 == pInfo->Fuel || 0 == pInfo->Spell )
		bAble = (gCurTime - m_dwRecallCheckTime > GAMERESRCMNGR->GetTitanRule()->TitanRecallValidTime_ZeroFuel);
	else	// 봉인으로 타이탄해제된 경우
		bAble = (gCurTime - m_dwRecallCheckTime > GAMERESRCMNGR->GetTitanRule()->TitanRecallValidTime_Seal);

	return bAble;*/
}

// magi82 - Titan(070604)
stTIME CTitanManager::GetTitanResummonTime()
{
	stTIME tmp,curTime;
	tmp.SetTime(m_dwRecallCheckTime);
	curTime.SetTime(GetCurTime());
	tmp -= curTime;
	return tmp;
	/*
	DWORD remainTime = 999;
	if(!m_RegistedTitanItemDBIdx)	return remainTime;

	TITAN_TOTALINFO* pInfo = GetTitanInfo(m_RegistedTitanItemDBIdx);

	if(!pInfo)
	{
		ASSERT(0);
		return remainTime;
	}
	// 체력조건으로 해제된 경우
	if( 0 == pInfo->Fuel || 0 == pInfo->Spell )
		remainTime = ( GAMERESRCMNGR->GetTitanRule()->TitanRecallValidTime_ZeroFuel - (gCurTime - m_dwRecallCheckTime) );
	else	// 봉인으로 타이탄해제된 경우
		remainTime = ( GAMERESRCMNGR->GetTitanRule()->TitanRecallValidTime_Seal - (gCurTime - m_dwRecallCheckTime) );

	return remainTime;*/
}

BOOL CTitanManager::IsRegistedTitan( DWORD dwCallItemDBIdx )
{
	TITAN_TOTALINFO* pInfo = GetTitanInfo(dwCallItemDBIdx);
	if(!pInfo)
	{
		ASSERT(0);
		return FALSE;
	}

	return (pInfo->RegisterCharacterIdx == HERO->GetID());
	//return pInfo->bRegistered;
}

DWORD CTitanManager::GetTitanGrade( DWORD dwCallItemDBIdx )
{
	TITAN_TOTALINFO* pInfo = GetTitanInfo(dwCallItemDBIdx);
	if(!pInfo)
	{
		ASSERT(0);
		return 0;
	}

	return pInfo->TitanGrade;
}

void CTitanManager::InitTitanInfo( TITAN_TOTALINFO* pTitanInfo, int num )
{
	TITANMGR->SetRegistedTitanItemDBIdx(0);	// 맵이동을 하면 타이탄을 다시 셋팅하므로 등록여부 DBIdx값도 초기해야함

	for( int i = 0; i < num; ++i )
	{
		AddTitanInfo(&pTitanInfo[i]);

		// magi82 - Titan(070420)
		if( pTitanInfo[i].RegisterCharacterIdx == HERO->GetID() )
		{
			if(TITANMGR->GetRegistedTitanItemDBIdx() > 0)
				ASSERT(0);
	
			TITANMGR->SetRegistedTitanItemDBIdx(pTitanInfo[i].TitanCallItemDBIdx);

			if(pTitanInfo[i].RecallTime)
			{
				SetRecallRemainTime(NULL, pTitanInfo[i].RecallTime);	// magi82(18)
			}
		}
	}
}

void CTitanManager::AddTitanInfo( TITAN_TOTALINFO* pTitanInfo )
{
	TITAN_TOTALINFO* pInfo = m_TitanInfoList.GetData(pTitanInfo->TitanCallItemDBIdx);
	if( pInfo )
	{
		memcpy(pInfo, pTitanInfo, sizeof(TITAN_TOTALINFO));
	}
	else
	{
		TITAN_TOTALINFO* pNewInfo = new TITAN_TOTALINFO;
		memcpy(pNewInfo, pTitanInfo, sizeof(TITAN_TOTALINFO));
		m_TitanInfoList.Add(pNewInfo,pNewInfo->TitanCallItemDBIdx);
	}
}

void CTitanManager::RemoveTitanInfo( DWORD dwCallItemDBIdx )
{
	TITAN_TOTALINFO* pInfo = m_TitanInfoList.GetData(dwCallItemDBIdx);

	if( !pInfo )	return;

	SAFE_DELETE(pInfo);

	m_TitanInfoList.Remove(dwCallItemDBIdx);
}

void CTitanManager::ReleaseTitanInfoList()
{
	TITAN_TOTALINFO* pInfo = NULL;
	m_TitanInfoList.SetPositionHead();

	while( pInfo = m_TitanInfoList.GetData() )
	{
		SAFE_DELETE(pInfo);
	}

	m_TitanInfoList.RemoveAll();
}

TITAN_TOTALINFO* CTitanManager::GetTitanInfo( DWORD dwCallItemDBIdx )
{
	TITAN_TOTALINFO* pInfo = m_TitanInfoList.GetData(dwCallItemDBIdx);

	return pInfo;
}

void CTitanManager::UpdateCurTitanInfo( CTitan* pTitan )
{
	//!!!
}

void CTitanManager::OnTitanRemove( CTitan* pTitan )
{
	//!!!
}

void CTitanManager::InitTitanEquipItemEnduranceInfo( TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo, int num )
{
	CItem* ptitanEnduranceItem = NULL;
	for( int i = 0; i < num; ++i )
	{
		AddTitanEquipItemEnduranceInfo(&pEnduranceInfo[i]);
	}
}

void CTitanManager::AddTitanEquipItemEnduranceInfo( TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo )
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = m_ItemEnduranceList.GetData(pEnduranceInfo->ItemDBIdx);

	if( pInfo )
	{
		memcpy(pInfo, pEnduranceInfo, sizeof(TITAN_ENDURANCE_ITEMINFO));
	}
	else
	{
		TITAN_ENDURANCE_ITEMINFO* pNewInfo = new TITAN_ENDURANCE_ITEMINFO;
		memcpy(pNewInfo, pEnduranceInfo, sizeof(TITAN_ENDURANCE_ITEMINFO));
		m_ItemEnduranceList.Add(pNewInfo, pNewInfo->ItemDBIdx);
	}

	SetTitanEnduranceView(pEnduranceInfo->ItemDBIdx);
}

void CTitanManager::RemoveTitanEquipInfo( DWORD dwEquipItemDBIdx )
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = m_ItemEnduranceList.GetData(dwEquipItemDBIdx);

	if( !pInfo )	return;

	SAFE_DELETE(pInfo);

	m_ItemEnduranceList.Remove(dwEquipItemDBIdx);
}

void CTitanManager::ReleaseTitanEquipInfo()
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
	m_ItemEnduranceList.SetPositionHead();

	while( pInfo = m_ItemEnduranceList.GetData() )
	{
		SAFE_DELETE(pInfo);
	}

	m_ItemEnduranceList.RemoveAll();
}

TITAN_ENDURANCE_ITEMINFO* CTitanManager::GetTitanEnduranceInfo( DWORD dwEquipItemDBIdx )
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;

	pInfo = m_ItemEnduranceList.GetData(dwEquipItemDBIdx);

	return pInfo;
}

void CTitanManager::AddTitanUsingEquipItemList( TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo )
{
	if( !pEnduranceInfo )	return;

	TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
	if( pInfo = m_ItemUsingEnduranceList.GetData(pEnduranceInfo->ItemDBIdx) )
	{
		*pInfo = *pEnduranceInfo;
		return;
	}

	m_ItemUsingEnduranceList.Add(pEnduranceInfo, pEnduranceInfo->ItemDBIdx);

	// magi82 - Titan(070423) 타이탄 인벤창에도 갱신해야함
	if(GAMEIN->GetTitanInventoryDlg()->IsActive())
	{
		GAMEIN->GetTitanInventoryDlg()->SetTitanEquipInfo();
	}
}

void CTitanManager::RemoveTitanUsingEquipItemList( DWORD equipItemDBIdx )
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = m_ItemUsingEnduranceList.GetData(equipItemDBIdx);

	//ASSERT(pInfo);

	m_ItemUsingEnduranceList.Remove(equipItemDBIdx);

	// magi82 - Titan(070423) 타이탄 인벤창에도 갱신해야함
	if(GAMEIN->GetTitanInventoryDlg()->IsActive())
	{
		GAMEIN->GetTitanInventoryDlg()->SetTitanEquipInfo();
	}
}

BOOL CTitanManager::CheckUsingEquipItemNum()
{// 갯수. 장착템 전부 있는지
	BOOL bTrue = (m_ItemUsingEnduranceList.GetDataNum() == eTitanWearedItem_Max);
	return bTrue;
}

BOOL CTitanManager::CheckUsingEquipItemEndurance()
{
	// 내구도 1% 2개 이하인지
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

// magi82 - Titan(070423)
int CTitanManager::GetTitanEquipTotalInfo(DWORD* temp)
{
	int i = 0;
	TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
	m_ItemUsingEnduranceList.SetPositionHead();

	while( pInfo = m_ItemUsingEnduranceList.GetData() )
	{
		if(pInfo)
		{
			temp[i] = pInfo->ItemDBIdx;
			i++;
		}
	}

	return i;
}

// magi82 - Titan(070424)
void CTitanManager::SetTitanEnduranceView( DWORD dwEquipItemDBIdx )
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = m_ItemEnduranceList.GetData(dwEquipItemDBIdx);

	if( pInfo )
	{
		CItem* pItem = ITEMMGR->GetItem(pInfo->ItemDBIdx);
		if(pItem)
		{
			if(pInfo->Endurance <= 1)
			{
				pItem->SetTitanEndurance(TRUE);
			}
			else
			{
				pItem->SetTitanEndurance(FALSE);
			}

			if(GAMEIN->GetTitanInventoryDlg()->IsActive())
			{
				GAMEIN->GetTitanInventoryDlg()->SetTitanEquipInfo();
			}
		}	
	}
	else
	{
		ASSERT(0);
		return;
	}
}

// magi82 - Titan(070424)
void CTitanManager::SetTitanTotalEnduranceView()
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
	m_ItemEnduranceList.SetPositionHead();

	while( pInfo = m_ItemEnduranceList.GetData() )
	{
		if(pInfo)
		{
			CItem* pItem = ITEMMGR->GetItem(pInfo->ItemDBIdx);
			if(pItem)
			{
				if(pInfo->Endurance <= 1)
				{
					pItem->SetTitanEndurance(TRUE);
				}
				else
				{
					pItem->SetTitanEndurance(FALSE);
				}
			}
		}
		else
		{
			ASSERT(0);
			return;
		}
	}
}

BOOL CTitanManager::CheckMasterLifeAmountForRiding()
{
	// 주인 체력 체크

	DWORD perc = HERO->GetLife() * 100 / HERO->GetMaxLife();
	if( perc < GAMERESRCMNGR->GetTitanRule()->MasterLifeRate_forTitanCall )
		return FALSE;

	return TRUE;
}

void CTitanManager::CheckNotifyMsg( int checkKind )
{
	//탑승 상태가 아니면 back
	if( !HERO->InTitan() )
	{
		return;
	}

	switch(checkKind)
	{
	case TNM_MASTER_LIFE:
		{
			//주인 체력이 30% 미만이면
			DWORD curLife = HERO->GetLife();
			DWORD maxLife = HERO->GetMaxLife();

			DWORD lifeRate = curLife * 100 / maxLife;

			if( lifeRate < 30 && curLife )
			{
				CHATMGR->AddMsg( CTC_ALERT_RED, CHATMGR->GetChatMsg(9999) );	//체력이 30% 미만 경고
			}
		}
		break;
	case TNM_EQUIP_ENDURANCE:
		{
			CHATMGR->AddMsg( CTC_ALERT_RED, CHATMGR->GetChatMsg(9999) );	//타이탄 장착템 수리 경고
		}
		break;
	case TNM_EQUIP_NUM:
		{
			CHATMGR->AddMsg( CTC_ALERT_RED, CHATMGR->GetChatMsg(9999) );	//타이탄 장착템 갯수 경고
		}
		break;
	case TNM_CANNOT_DO:
		{
			CHATMGR->AddMsg( CTC_ALERT_RED, CHATMGR->GetChatMsg(9999) );	//타이탄 탑승시 불가.
		}
		break;
	}
}

void CTitanManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_TITAN_ADDNEW_FROMITEM:
		{
			SEND_ADDTITAN_FROMITEM* pmsg = (SEND_ADDTITAN_FROMITEM*)pMsg;
			AddTitanInfo(&pmsg->Info);
			if(pmsg->Info.RegisterCharacterIdx)
			{
				TITANMGR->SetRegistedTitanItemDBIdx(pmsg->Info.TitanCallItemDBIdx);
				STATSMGR->CalcTitanStats(pmsg->Info.TitanCallItemDBIdx);
				CItem* pItem = ITEMMGR->GetItem(pmsg->Info.TitanCallItemDBIdx);
				if(!pItem)
				{
					ASSERT(0);
					return;
				}
				ITEMPARAM Param = pItem->GetItemBaseInfo()->ItemParam;
				if(!(Param & ITEM_PARAM_TITAN_REGIST))
				{
					Param |= ITEM_PARAM_TITAN_REGIST;
					pItem->SetItemParam( Param );
				}	
			}
			ITEMMGR->RefreshItemToolTip(pmsg->Info.TitanCallItemDBIdx);
		}
		break;
	case MP_TITAN_ADDNEW_EQUIP_FROMITEM:
		{
			SEND_ADDTITANEQUIP_FROMITEM* pmsg = (SEND_ADDTITANEQUIP_FROMITEM*)pMsg;
			AddTitanEquipItemEnduranceInfo(&pmsg->Info);
			ITEMMGR->RefreshItemToolTip(pmsg->Info.ItemDBIdx);
		}
		break;
	case MP_TITAN_RIDEIN_ACK:
		{
			MSG_TITAN_RIDEIN* pmsg = (MSG_TITAN_RIDEIN*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
			if( !pPlayer )		return;

			//if( pPlayer->GetState() == eObjectState_TitanRecall )
			OBJECTSTATEMGR->EndObjectState(pPlayer, eObjectState_TitanRecall);			

			EFFECTMGR->StartEffectProcess(eEffect_Titan_RideIn,pPlayer,NULL,0,0);

			pPlayer->RidingTitan( TRUE );
			pPlayer->SetTitanAppearanceInfo( &pmsg->AppearanceInfo );
			APPEARANCEMGR->InitAppearance( pPlayer );

			if( HERO == pPlayer )
			{
				CTitan* pNewTitan = new CTitan;
				pNewTitan->InitTitan(&pmsg->TitanTotalInfo);
				pPlayer->SetCurTitan(pNewTitan);

				// magi82 - Titan(070531)
				GAMEIN->GetTitanGuageDlg()->SetLife(pmsg->TitanTotalInfo.Fuel);
				GAMEIN->GetTitanGuageDlg()->SetNaeRyuk(pmsg->TitanTotalInfo.Spell);
				GAMEIN->GetTitanGuageDlg()->SetActive(TRUE);
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1546));

				// magi82(23)
				//if( MAP->GetMapNum() != 101
				//	&& MAP->GetMapNum() != 102
				//	&& MAP->GetMapNum() != 104
				//	&& MAP->GetMapNum() != 105
				//	)
				if( FALSE == MAP->IsMapKind(eTitanMap) )
				{
					STATUSICONDLG->AddIcon(HERO, eStatusIcon_Titan_General, 0, pmsg->MaintainTime);
				}
				else
				{
					STATUSICONDLG->AddIcon(HERO, eStatusIcon_Titan_Fantasy, 0, 0);
				}

				// magi82 - Titan(070611) - 타이탄에 탑승하는 순간 모든 무공의 툴팁에 타이탄 내용이 추가된다.
				MUGONGMGR->RefreshMugong();

				// magi82 - Titan(071018) - 타이탄 탑승시 아이템 갱신
				ITEMMGR->RefreshAllItem();

				GAMEIN->GetQuickDialog()->RefreshIcon();	// magi82 - Titan(070911) 타이탄 무공업데이트

				// Camera 수정			
				CAMERA->InitCamera( 0, CAMERA->GetCameraDesc()->m_AngleX.ToDeg(), CAMERA->GetCameraDesc()->m_AngleY.ToDeg(),
					2500, HERO, TRUE );
				MAP->ToggleSunPosForShadow( TRUE );

				GAMEIN->GetTitanRecallDlg()->SetActive(FALSE);
			}			
			
			if( HERO == pPlayer )
				PEACEWARMGR->ToggleHeroPeace_WarMode();

			// magi82(3) - Titan(071022) 타이탄 탑승시 이펙트 변경
			DWORD dwBattleKind = BATTLESYSTEM->GetBattle( pPlayer->GetBattleID() )->GetBattleKind();
			if(dwBattleKind == eBATTLE_KIND_VIMUSTREET)
			{
				pPlayer->RemoveObjectEffect(BATTLE_TEAMEFFECT_ID);
				BYTE team = pPlayer->GetBattleTeam();
				if( team == eBattleTeam1 )
				{
					OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_A_s.beff"));
					pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
				}
				else if( team == eBattleTeam2 )
				{
					OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_B_s.beff"));
					pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
				}
			}

			if( pPlayer->IsPKMode() == TRUE )
			{
				pPlayer->RemoveObjectEffect(PK_EFFECT_ID);

				OBJECTEFFECTDESC desc(FindEffectNum("t_maintain_PK_S.beff"));
				pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1, HERO);
			}

			pPlayer->SetBaseMotion();
			if(pPlayer->GetState() == eObjectState_None)
				OBJECTSTATEMGR->EndObjectState(pPlayer,eObjectState_None);			
		}
		break;
	case MP_TITAN_GETOFF_ACK:	// 타이탄 탑승 해제
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			CPlayer* pOwner = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);

			if(!pOwner)
			{
				ASSERT(0);
				return;
			}

			switch(pmsg->dwData2)
			{
			case eNormal:
			case eFromUser:
				{
					EFFECTMGR->StartEffectProcess(eEffect_Titan_GetOff,pOwner,NULL,0,0);
				}
				break;
			case eMasterLifeRate:
			case eExhaustFuel:
			case eExhaustSpell:
				{
					EFFECTMGR->StartEffectProcess(eEffect_Titan_ForcedGetOff,pOwner,NULL,0,0);
				}
				break;
			default:
				ASSERT(0);
			}

			pOwner->RidingTitan( FALSE );

			APPEARANCEMGR->InitAppearance( pOwner );

			if( HERO == pOwner )
			{
				CTitan* pTitan = pOwner->GetCurTitan();

				SetRecallRemainTime(pmsg->dwData2);	// magi82(18)

				SAFE_DELETE(pTitan);

				pOwner->SetCurTitan(pTitan);

				// magi82(23)
				//if( MAP->GetMapNum() != 101
				//	&& MAP->GetMapNum() != 102
				//	&& MAP->GetMapNum() != 104
				//	&& MAP->GetMapNum() != 105
				//	)
				if( FALSE == MAP->IsMapKind(eTitanMap) )
				{
					STATUSICONDLG->RemoveIcon(HERO, eStatusIcon_Titan_General, 0);
				}
				else
				{
					STATUSICONDLG->RemoveIcon(HERO, eStatusIcon_Titan_Fantasy, 0);
				}

				// magi82 - Titan(070913) 타이탄 증서 딜레이
				stTIME stTime;
				stTime = GetTitanResummonTime();
				DWORD dwRemainTime = 0;
				dwRemainTime = Convert2MilliSecond( stTime.GetDay(), stTime.GetHour(), stTime.GetMinute(), stTime.GetSecond() );
				CItem* pItem = ITEMMGR->GetItem(m_RegistedTitanItemDBIdx);
				if( pItem )
				{
					HERO->GetDelayGroup()->AddDelay(CDelayGroup::eDK_Item, pItem->GetItemIdx(), dwRemainTime);
				}

				// magi82 - Titan(070531)
				GAMEIN->GetTitanGuageDlg()->SetActive(FALSE);

				//msg.dwData2 = reason;	// chatmsg 해제이유
				switch(pmsg->dwData2)
				{
				case eNormal:
				case eFromUser:
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1547));
					}
					break;
				case eMasterLifeRate:
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1586));
					}
					break;
				case eExhaustFuel:
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1585));
					}
					break;
				case eExhaustSpell:
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1584));
					}
					break;
				default:
					ASSERT(0);
					//return;
				}

				// magi82 - Titan(070611) - 타이탄에 내리는 순간 모든 무공의 툴팁에 타이탄 내용이 삭제된다.
				MUGONGMGR->RefreshMugong();

				// magi82 - Titan(071018) - 타이탄 탑승 해제시 아이템 갱신
				ITEMMGR->RefreshAllItem();

				GAMEIN->GetQuickDialog()->RefreshIcon();	// magi82 - Titan(070911) 타이탄 무공업데이트

				//
				CAMERA->Init(HERO,CAMERA->GetCameraDesc()->m_AngleX.ToDeg(),CAMERA->GetCameraDesc()->m_AngleY.ToDeg(),1000);
				MAP->ToggleSunPosForShadow( FALSE );				
			}
			
			pOwner->SetBaseMotion();
			if(pOwner->GetState() == eObjectState_None)
				OBJECTSTATEMGR->EndObjectState(pOwner,eObjectState_None);

			// magi82(3) - Titan(071022) 타이탄 탑승해제시 이펙트 변경
			DWORD dwBattleKind = BATTLESYSTEM->GetBattle( pOwner->GetBattleID() )->GetBattleKind();
			if(dwBattleKind == eBATTLE_KIND_VIMUSTREET)
			{
				pOwner->RemoveObjectEffect(BATTLE_TEAMEFFECT_ID);
				BYTE team = pOwner->GetBattleTeam();
				if( team == eBattleTeam1 )
				{
					OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_A_s.beff"));
					pOwner->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
				}
				else if( team == eBattleTeam2 )
				{
					OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_B_s.beff"));
					pOwner->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
				}
			}

			if( pOwner->IsPKMode() == TRUE )
			{
				pOwner->RemoveObjectEffect(PK_EFFECT_ID);

				OBJECTEFFECTDESC desc(FindEffectNum("maintain_PK_S.beff"));
				pOwner->AddObjectEffect( PK_EFFECT_ID, &desc, 1, HERO);
			}			
		}
		break;
	case MP_TITAN_FUEL_ACK:
		{
			MSG_INT* pmsg = (MSG_INT*)pMsg;
			
			TITAN_TOTALINFO* pInfo = GetTitanInfo(m_RegistedTitanItemDBIdx);
			if(!pInfo)
			{
				ASSERT(0);
				return;
			}

			pInfo->Fuel = pmsg->nData;

			ChangeTitanGuageFuel((DWORD)pmsg->nData);
		}
		break;
	case MP_TITAN_SPELL_ACK:
		{
			MSG_INT* pmsg = (MSG_INT*)pMsg;
			
			TITAN_TOTALINFO* pInfo = GetTitanInfo(m_RegistedTitanItemDBIdx);
			if(!pInfo)
			{
				ASSERT(0);
				return;
			}
			pInfo->Spell = pmsg->nData;

			ChangeTitanGuageSpell((DWORD)pmsg->nData);
		}
		break;
	case MP_TITAN_VALUEINFO:
		{/*
			MSG_DWORD2	msg;
			msg.Category = MP_TITAN;
			msg.Protocol = MP_TITAN_VALUEINFO;
			msg.dwObjectID = m_pOwner->GetID();
			msg.dwData1 = m_TitanTotalInfo.Fuel;
			msg.dwData2 = m_TitanTotalInfo.Spell;*/
		}
		break;
	case MP_TITAN_ENDURANCE_UPDATE:
		{/*
			MSG_DWORD2 msg;
			msg.Category = MP_TITAN;
			msg.Protocol = MP_TITAN_ENDURANCE_UPDATE;
			msg.dwObjectID = m_pMaster->GetID();
			msg.dwData1 = pInfo->ItemDBIdx;
			msg.dwData2 = pInfo->Endurance;*/
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			TITAN_ENDURANCE_ITEMINFO* pInfo = GetTitanEnduranceInfo(pmsg->dwData1);
			if(!pInfo)	return;

			pInfo->Endurance = pmsg->dwData2;

			ITEMMGR->RefreshItemToolTip(pmsg->dwData1);
		}
		break;
	case MP_TITAN_STATINFO:
		{
			// !!!!!! 이제 안씀(클라이언트 자체 해결 계산) !!!!!!
			//SEND_TITANSTATINFO* pmsg = (SEND_TITANSTATINFO*)pMsg;
			//SetTitanStats(&pmsg->Info);
			//GAMEIN->GetTitanPartsChangeDlg()->SetTitanStatsInfo();
			//GAMEIN->GetTitanInventoryDlg()->SetTitanInvenInfo();	// magi82 - Titan(070423)
		}
		break;
	case MP_TITAN_RECALL_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*) pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
			if( !pPlayer )		return;

			BYTE state = pPlayer->GetState();
			pPlayer->OnEndObjectState(state);

			if(pPlayer == HERO)
			{
				CTitanRecallDlg* pTitianRecallDlg = GAMEIN->GetTitanRecallDlg();
				pTitianRecallDlg->SetActive(TRUE);				
				
				TITAN_TOTALINFO* pTitanInfo = TITANMGR->GetTitanInfo(TITANMGR->GetRegistedTitanItemDBIdx());								
				if( pTitanInfo == NULL )
					return;

				TITANINFO_GRADE* pGrade = GAMERESRCMNGR->GetTitanGradeInfo(pTitanInfo->TitanKind, pTitanInfo->TitanGrade);
				if( pGrade == NULL )
					return;

				pTitianRecallDlg->SetSuccessTime(pGrade->dwRecallSpellTime - GetTitanShopitemOption()->dwSummonReduceTime);	// magi82(26)
				pTitianRecallDlg->StartProgress();
			}
	
			OBJECTSTATEMGR->StartObjectState( pPlayer, eObjectState_TitanRecall );
		}
		break;
	case MP_TITAN_RECALL_CANCEL_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*) pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
			if( !pPlayer )		return;

			//if( pPlayer->GetState() == eObjectState_TitanRecall )
			OBJECTSTATEMGR->EndObjectState(pPlayer, eObjectState_TitanRecall);

			if(pPlayer == HERO)
				GAMEIN->GetTitanRecallDlg()->SetActive(FALSE);			

			pPlayer->ChangeMotion(eMotion_Engrave_End, FALSE);			
		}
		break;
	case MP_TITAN_RECALL_CANCEL_NACK:
		{

		}
		break;
	default:
		{
			ASSERT(0);
		}
		break;
	}
}

void CTitanManager::SetTitanRepairTotalEquipItemLock( BOOL bFlag )
{
	m_ItemEnduranceList.SetPositionHead();

	TITAN_ENDURANCE_ITEMINFO* pEndurance = NULL;
	while(pEndurance = m_ItemEnduranceList.GetData())
	{
		CItem* pItem = ITEMMGR->GetItem(pEndurance->ItemDBIdx);
		if(bFlag == TRUE)
		{
			pItem->SetLock(TRUE);
		}
		else
		{
			pItem->SetLock(FALSE);
		}
	}
}

// magi82 - Titan(070619)
DWORD CTitanManager::GetTitanEnduranceTotalInfo(MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN* msg, BOOL bFlag)
{
	TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
	m_ItemEnduranceList.SetPositionHead();
	DWORD dwMoney = 0;

	while( pInfo = m_ItemEnduranceList.GetData() )
	{
		if(pInfo)
		{
			CItem* pItem = ITEMMGR->GetItem(pInfo->ItemDBIdx);
			if(pItem)
			{
				BYTE eTable = ITEMMGR->GetTableIdxForAbsPos(pItem->GetPosition());
				if( eTable == eItemTable_Inventory || eTable == eItemTable_Titan )
				{
					if(bFlag)	// 수리비
					{
						float fEnduranceRate = (float)pInfo->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
						if( fEnduranceRate > 0.0f && fEnduranceRate < 1.0f )
						{
							fEnduranceRate = 1.0f;
						}
						DWORD dwRepairPrice = (DWORD)( (100 - (DWORD)fEnduranceRate) * (pItem->GetItemInfo()->BuyPrice / 100) * 0.09f );

						dwMoney += dwRepairPrice;
					}
					else	// 수리할 아이템 골라내기
					{
						msg->AddRepairInfo(pItem->GetItemIdx(), pItem->GetPosition());
					}
				}
			}
		}
		else
		{
			ASSERT(0);
			return 0;
		}
	}

	return dwMoney;
}

void CTitanManager::SetTitanPaperDelayTime()
{
	// magi82 - Titan(070914) 타이탄 증서 딜레이
	stTIME stTime;
	stTime = TITANMGR->GetTitanResummonTime();
	if( stTime.value > 0 )
	{
		DWORD dwRemainTime = 0;
		DWORD dwStartTime = 0;
		dwRemainTime = Convert2MilliSecond( stTime.GetDay(), stTime.GetHour(), stTime.GetMinute(), stTime.GetSecond() );

		CItem* pItem = ITEMMGR->GetItem(TITANMGR->GetRegistedTitanItemDBIdx());
		if( !pItem )
			return;

		TITAN_TOTALINFO* pInfo = TITANMGR->GetTitanInfo(TITANMGR->GetRegistedTitanItemDBIdx());
		if( !pInfo )
			return;

		TITANINFO_GRADE* pGrade = GAMERESRCMNGR->GetTitanGradeInfo(pInfo->TitanKind, pInfo->TitanGrade);
		if( !pGrade )
			return;

		// magi82(24)
		//if( MAP->GetMapNum() == 101
		//	|| MAP->GetMapNum() == 102
		//	|| MAP->GetMapNum() == 104
		//	|| MAP->GetMapNum() == 105 )
		//if( MAP->IsMapKind(eTitanMap) )
		//{
		//	// 판타지 맵일때
		//	if( pInfo->Fuel == 0 || pInfo->Spell == 0 )	// 강제 해제일 경우..
		//	{
		//		dwStartTime = GAMERESRCMNGR->GetTitanRule()->TitanRecallValidTime_ZeroFuel - pGrade->RecallReduceTime;
		//	}
		//	else	// 일반 해제일 경우..
		//	{
		//		dwStartTime = GAMERESRCMNGR->GetTitanRule()->TitanRecallValidTime_Seal - pGrade->RecallReduceTime;
		//	}
		//}
		//else
		//{
		//	// 중원맵 일때
		//	dwStartTime = GAMERESRCMNGR->GetTitanRule()->TitanRecallValidTime_General - pGrade->RecallReduceTime;
		//}

		if( pItem )
		{
			HERO->GetDelayGroup()->AddDelay(CDelayGroup::eDK_Item, pItem->GetItemIdx(), dwRemainTime);
		}
	}
}

void CTitanManager::SetServerLogInTime()
{
	SYSTEMTIME slt = GAMEIN->GetLoginTime();	// 서버 로그인 시각.
	m_serverT.SetTime( slt.wYear-2000, slt.wMonth, slt.wDay, slt.wHour, slt.wMinute, slt.wSecond );
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
