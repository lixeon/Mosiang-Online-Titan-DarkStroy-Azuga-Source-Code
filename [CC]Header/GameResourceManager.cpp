// GameResourceManager.cpp: implementation of the CGameResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#ifndef _MURIMNET_

#include "GameResourceManager.h"
#include "MHFile.h"
#include "TacticManager.h"
#include "SkillInfo.h"
#ifdef _MHCLIENT_
#include "SkillManager_client.h"
#include "./Audio/MHAudioManager.h"
#include "GMNotifyManager.h"
#else
#include "SkillManager_server.h"
#include "ItemDrop.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CGameResourceManager);

BOOL g_bTESTSERVER = FALSE;
extern int g_nServerSetNum;


CGameResourceManager::CGameResourceManager()
{
	m_bNpcRegenLoad = FALSE;
	m_bMonsterListLoad = FALSE;
	m_bNpcListLoad = FALSE;
	m_bMonsterDropItem = FALSE;
	m_bLowResolution = FALSE;

	m_TotalNpcGroupCount = 0;
	m_MaxNpcRegenCount = 0;

	m_SMonsterListTable.Initialize(100);
	m_PetListTable.Initialize(50);
	m_pBuffList = NULL;
	
#ifdef _SERVER_RESOURCE_FIELD_
	m_StaticNpcTable.Initialize( 300 );
	m_WareNpcTable.Initialize( 10 );
	m_HideNpcTable.Initialize(3);
	m_TitanMapMosterDropTable.Initialize( 100 );
#else
	m_PreLoadDataTable.Initialize(4);	
	m_PreLoadEffectTable.Initialize(25);	
	
#endif //_SERVER_RESOURCE_FIELD_

#ifdef _CLIENT_RESOURCE_FIELD_
	m_PreLoadItemlist = NULL;

	//2008. 6. 18. CBH - +10 이상 아이탬의 이펙트 정보 테이블 초기화
	m_PlusItemEffectInfoTable.Initialize(10);
#endif	
	m_AvatarEquipTable.Initialize(50);
	m_ShopItemDupOptionTable.Initialize(30);	


	// magi82 //////////////////////////////////////////////////////////////////////////

	m_LevPenaltyPool = new CMemoryPoolTempl<LEV_PENALTY>;
	m_LevPenaltyPool->Init( 99, 10, "LevPenalty" );

	m_LevPenaltyTable.Initialize( 99 );
	
	////////////////////////////////////////////////////////////////////////////////////

	//SW070127 타이탄
	m_TitanListTable.Initialize(4);
	//m_TitanEquipEnduranceInfo.Initialize(16);
	//m_TitanSkillLink.Initialize(64);

	m_PaneltyTime = 0;

	// magi82 - UniqueItem(070625)
	m_UniqueItemListTable.Initialize( 100 );

	// magi82 - UniqueItem(070709)
	m_UniqueItemMixTable.Initialize( 100 );

	m_MapKindStateInfoTable.Initialize(128);

	//2007. 12. 4. CBH - 스킨 아이탬 리스트 테이블 초기화
	m_NomalClothesSkinTable.Initialize(100);
	//2008. 1. 16. CBH - 코스튬 스킨 아이탬 리스트 테이블 초기화
	m_CotumeSkinTable.Initialize(100);	
}

CGameResourceManager::~CGameResourceManager()
{
	SAFE_DELETE_ARRAY(m_NpcChxList);	

	ResetMonsterList();
	ResetPetList();
	ResetPetBuffList();

	//SW070127 타이탄
	RemoveTitanList();
	//RemoveTitanSkillLink();
	//RemoveTitanEquipEnduranceInfo();

#ifdef _CLIENT_RESOURCE_FIELD_
	DeletePreLoadData();	
	RemovePlusItemEffectTable();
#endif

#ifdef _SERVER_RESOURCE_FIELD_
	ReleaseStaticNpc();

	HIDE_NPC_INFO* info = NULL;
	m_HideNpcTable.SetPositionHead();
	while(info = m_HideNpcTable.GetData())
	{
		if(info)
		{
			delete info;
			info = NULL;
		}
	}
	m_HideNpcTable.RemoveAll();

	MONSTERDROP_TIMAP *pdata = NULL;
	m_TitanMapMosterDropTable.SetPositionHead();
	while(pdata = m_TitanMapMosterDropTable.GetData())
		SAFE_DELETE( pdata );
	m_TitanMapMosterDropTable.RemoveAll();
#endif
	//
	AVATARITEM* pItem = NULL;
	m_AvatarEquipTable.SetPositionHead();
	while(pItem = m_AvatarEquipTable.GetData())
	{
		if(pItem)
		{
			delete pItem;
			pItem = NULL;
		}
	}
	m_AvatarEquipTable.RemoveAll();

	SHOPITEMDUP* pDupOption = NULL;
	m_ShopItemDupOptionTable.SetPositionHead();
	while(pDupOption = m_ShopItemDupOptionTable.GetData())
	{
		if(pDupOption)
		{
			delete pDupOption;
			pDupOption = NULL;
		}
	}
	m_ShopItemDupOptionTable.RemoveAll();


	// magi82 //////////////////////////////////////////////////////////////////////////

	m_LevPenaltyTable.SetPositionHead();
	LEV_PENALTY* pLevPenalty = NULL;
	while( pLevPenalty = m_LevPenaltyTable.GetData() )
	{
		m_LevPenaltyPool->Free( pLevPenalty );
	}
	m_LevPenaltyTable.RemoveAll();

	SAFE_DELETE(m_LevPenaltyPool);
	////////////////////////////////////////////////////////////////////////////////////

	// magi82 - UniqueItem(070625)
	m_UniqueItemListTable.SetPositionHead();
	UNIQUE_ITEM_OPTION_INFO* sUniqueItemTemp = NULL;
	while(sUniqueItemTemp = m_UniqueItemListTable.GetData())
	{
		if(sUniqueItemTemp)
		{
			delete sUniqueItemTemp;
			sUniqueItemTemp = NULL;
		}
	}
	m_UniqueItemListTable.RemoveAll();
	//////////////////////////////////////////////////////////////////////////

	// magi82 - UniqueItem(070709)
	m_UniqueItemMixTable.SetPositionHead();
	UNIQUE_ITEM_MIX_INFO* sUniqueItemMixTemp = NULL;
	while(sUniqueItemMixTemp = m_UniqueItemMixTable.GetData())
	{
		// magi82(14) - Memory(071027) 메모리 누수 수정
		SAFE_DELETE_ARRAY(sUniqueItemMixTemp->sUniqueItemMixMaterial);
		SAFE_DELETE(sUniqueItemMixTemp);
	}
	m_UniqueItemMixTable.RemoveAll();
	//////////////////////////////////////////////////////////////////////////

	RemoveNomalClothesSkinList();	// 2007. 12. 4. CBH - 스킨 아이탬 목록 리스트 삭제
	RemoveCostumeSkinList();	// 2008. 1. 16. CBH - 코스튬 스킨 아이탬 목록 리스트 삭제

	m_MapKindStateInfoTable.SetPositionHead();
	while( stMAPKINDINFO* pMapKind = m_MapKindStateInfoTable.GetData() )
	{
		delete pMapKind;
	}
	m_MapKindStateInfoTable.RemoveAll();
}

BOOL CGameResourceManager::LoadPetList()
{
	CMHFile file;

#ifdef _FILE_BIN_
	if(!file.Init("./Resource/PetList.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Reource/PetList.txt", "rt))
		return FALSE;
#endif

	int count = 0;
	BASE_PET_LIST* pList = NULL;

	while(!file.IsEOF())
	{
		if( count == MAX_PET_NUM )
			break;

		pList = new BASE_PET_LIST;
		memset(pList, 0, sizeof(BASE_PET_LIST));

 		pList->PetKind = file.GetWord();
		SafeStrCpy(pList->Name, file.GetString(), MAX_NAME_LENGTH+1);
		pList->MotionID = file.GetDword();
		pList->Pet2DIdx = file.GetWord();
		SafeStrCpy(pList->ChxName_PerGrade[0], file.GetString(), 32);
		SafeStrCpy(pList->ChxName_PerGrade[1], file.GetString(), 32);
		SafeStrCpy(pList->ChxName_PerGrade[2], file.GetString(), 32);
		pList->Scale = file.GetFloat();
		pList->SummonLevel = file.GetWord();
		pList->Tall[0] = file.GetWord();
		pList->Tall[1] = file.GetWord();
		pList->Tall[2] = file.GetWord();
		pList->SpeedFromHero = file.GetFloat();
		pList->SkillIdx[0] = file.GetWord();
		pList->SkillIdx[1] = file.GetWord();
		pList->SkillIdx[2] = file.GetWord();
		pList->SkillInfo[0] = SKILLMGR->GetSkillInfo(pList->SkillIdx[0]);
		pList->SkillInfo[1] = SKILLMGR->GetSkillInfo(pList->SkillIdx[1]);
		pList->SkillInfo[2] = SKILLMGR->GetSkillInfo(pList->SkillIdx[2]);
		pList->SkillRecharge = file.GetDword();
		pList->StaminaDecreasePerSkill[0] = file.GetDword();
		pList->StaminaDecreasePerSkill[1] = file.GetDword();
		pList->StaminaDecreasePerSkill[2] = file.GetDword();
		pList->StaminaMax[0] = file.GetDword();
		pList->StaminaMax[1] = file.GetDword();
		pList->StaminaMax[2] = file.GetDword();
		pList->InventoryTapNum[0] = file.GetWord();
		pList->InventoryTapNum[1] = file.GetWord();
		pList->InventoryTapNum[2] = file.GetWord();
		pList->DieDramaNum[0]	= FindEffectNum(file.GetString());
		pList->DieDramaNum[1]	= FindEffectNum(file.GetString());
		pList->DieDramaNum[2]	= FindEffectNum(file.GetString());

		for( int i = 0; i < 3; ++i )
		{
			pList->BuffList[i][0]	= file.GetWord();
			pList->BuffList[i][1]	= file.GetWord();
			pList->BuffList[i][2]	= file.GetWord();
		}

		ASSERT(!m_PetListTable.GetData(pList->PetKind));

		m_PetListTable.Add(pList, pList->PetKind);

		++count;
	}

	return TRUE;
}

BASE_PET_LIST*  CGameResourceManager::GetPetListInfo(int idx)
{
	return m_PetListTable.GetData(idx);
}

void CGameResourceManager::ResetPetList()
{
	BASE_PET_LIST* pInfo = NULL;
	m_PetListTable.SetPositionHead();
	while(pInfo = m_PetListTable.GetData())
	{
		SAFE_DELETE(pInfo);
	}
	m_PetListTable.RemoveAll();
}

BOOL CGameResourceManager::LoadPetRule()
{
	CMHFile file;
	char szBuf[256] = {0,};

#ifdef _FILE_BIN_
	if(!file.Init("./Resource/PetRule.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/PetRule.txt", "rt"))
		return FALSE;
#endif

	while(!file.IsEOF())
	{
		strcpy(szBuf,strupr(file.GetString()));

		if(strcmp(szBuf,"#PET_RESUMMON_VALID_TIME") == 0)
		{
			m_PetRule.ResummonVilidTime = file.GetDword();
		}

		else if(strcmp(szBuf,"#PET_GRADEUP_PROB_1TO2") == 0)
		{
			m_PetRule.GradeUpProb1to2 = file.GetDword();
		}

		else if(strcmp(szBuf,"#PET_GRADEUP_PROB_2TO3") == 0)
		{
			m_PetRule.GradeUpProb2to3 = file.GetDword();
		}

		else if(strcmp(szBuf,"#MAX_FRIENDSHIP") == 0)
		{
			m_PetRule.MaxFriendship = file.GetDword();
		}

		else if(strcmp(szBuf,"#DEFAULT_FRIENDSHIP") == 0)
		{
			m_PetRule.DefaultFriendship = file.GetDword();
		}

		else if(strcmp(szBuf,"#REVIVAL_FRIENDSHIP") == 0)
		{
			m_PetRule.RevivalFriendship = file.GetDword();
		}

		else if(strcmp(szBuf,"#REVIVAL_FRIENDSHIP_SHOP") == 0)
		{
			m_PetRule.RevivalFriendship_Shop = file.GetDword();
		}

		else if(strcmp(szBuf,"#FRIENDLY_SKILL") == 0)
		{
			m_PetRule.FriendShipPerSkill[0] = file.GetInt();
			m_PetRule.FriendShipPerSkill[1] = file.GetInt();
			m_PetRule.FriendShipPerSkill[2] = file.GetInt();
		}
		

		else if(strcmp(szBuf,"#FRIENDLY_TIME") == 0)
		{
			m_PetRule.FriendShipPer10Sec[0] = file.GetInt();
			m_PetRule.FriendShipPer10Sec[1] = file.GetInt();
			m_PetRule.FriendShipPer10Sec[2] = file.GetInt();
		}

		else if(strcmp(szBuf,"#FRIENDLY_MASTER_DIE") == 0)
		{
			m_PetRule.FriendShipPerMasterDie = file.GetInt();
		}

		else if(strcmp(szBuf,"#FRIENDLY_STAMINA_ZERO_EVERY10SEC") == 0)
		{
			m_PetRule.FriendShipPerStaminaZero = file.GetInt();
		}

		else if(strcmp(szBuf,"#FRIENDLY_TRADE") == 0)
		{
			m_PetRule.FriendShipPerTrade = file.GetInt();
		}

		else if(strcmp(szBuf,"#FRIENDLY_UPGRADE_FAIL") == 0)
		{
			m_PetRule.FriendShipPerUpgradeFail = file.GetInt();
		}

		else if(strcmp(szBuf,"#STAMINA_STAND") == 0)
		{
			m_PetRule.StaminaStand = file.GetInt();
		}

		else if(strcmp(szBuf,"#STAMINA_MOVE") == 0)
		{
			m_PetRule.StaminaMove = file.GetInt();
		}
	}

	return TRUE;
}

BOOL CGameResourceManager::LoadPetBuffList()
{
	CMHFile file;
	char szBuf[256] = {0,};

#ifdef _FILE_BIN_
	if(!file.Init("./Resource/PetBuffList.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/PetBuffList.txt", "rt"))
		return FALSE;
#endif

	PET_BUFF_LIST* pList = NULL;

	int BuffNum = file.GetInt();
	m_pBuffList = new PET_BUFF_LIST[BuffNum];

	int count = 0;
	while(!file.IsEOF())
	{
		m_pBuffList[count].Idx = file.GetWord();
		SafeStrCpy(m_pBuffList[count].BuffName,file.GetString(),MAX_NAME_LENGTH+1);
		m_pBuffList[count].BuffKind = file.GetByte();
		m_pBuffList[count].BuffSuccessProb = file.GetDword();
		m_pBuffList[count].BuffValueData = file.GetDword();
		m_pBuffList[count].BuffAdditionalData = file.GetWord();

		++count;

		if(count == BuffNum)
			break;
	}

	return TRUE;
}

PET_BUFF_LIST* CGameResourceManager::GetPetBuffInfo( int idx )
{
	if(idx == 0)	return NULL;
	return &m_pBuffList[idx-1];
}

void CGameResourceManager::ResetPetBuffList()
{
	if(m_pBuffList)
	{
		delete[] m_pBuffList;
		m_pBuffList = NULL;
	}
}

//SW070127 타이탄
BOOL CGameResourceManager::LoadTitanSpellCostInfo()
{
	CMHFile file;

#ifdef _FILE_BIN_
//	if(!file.Init("./Resource/TitanSpellCostPerMap.bin", "rb"))
	if(!file.Init("./Resource/Server/TitanSpellCostPerMap.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/TitanSpellCostPerMap.txt", "rt"))
		return FALSE;
#endif

	int count = 0;
	WORD mapNum = 0;
	while(!file.IsEOF())
	{
		mapNum = file.GetWord();
		m_TitanSpellCostInfo[mapNum] = file.GetDword();

		++count;
		if( count > MAX_MAP_ENUMCOUNT )
			break;
	}

	return TRUE;
}

DWORD CGameResourceManager::GetTitanSpellCost( WORD MapNum )
{
	return m_TitanSpellCostInfo[MapNum];
}

BOOL CGameResourceManager::LoadTitanList()
{
	CMHFile file;

#ifdef _FILE_BIN_
	if(!file.Init("./Resource/TitanList.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/TitanList.txt", "rt"))
		return FALSE;
#endif

	BASE_TITAN_LIST* pList = NULL;
	TITANINFO_GRADE* pGrade = NULL;

	int totalCount = file.GetInt();

	int count = 0;

	while(!file.IsEOF())
	{
		if( totalCount == count )	break;

		pList = new BASE_TITAN_LIST;

		memset(pList, 0, sizeof(BASE_TITAN_LIST));

		pList->TitanIdx = file.GetWord();
		SafeStrCpy(pList->TitanName, file.GetString(), MAX_NAME_LENGTH+1);
		SafeStrCpy(pList->TitanBaseChxName, file.GetString(), MAX_CHXNAME_LENGTH+1);
		SafeStrCpy(pList->TitanfaceModName, file.GetString(), MAX_CHXNAME_LENGTH+1);
		//pList->Titan2DImage = file.GetWord();
		char tmp[64] = {0,};	//for 파싱시 구분

		float attAtk, attReg;
		attAtk = attReg = 0.f;
		for( int i = 0; i < MAX_TITAN_GRADE; ++i )
		{
			SafeStrCpy(tmp, file.GetString(), MAX_CHXNAME_LENGTH);
			if( strcmp(tmp, "#") != 0)
				ASSERT(0);

			pGrade = &pList->GradeInfo[i];
			//SafeStrCpy(pGrade->BodyChxName, file.GetString(), MAX_CHXNAME_LENGTH);			
			//SafeStrCpy(pGrade->FaceModName, file.GetString(), MAX_CHXNAME_LENGTH);
			//SafeStrCpy(pGrade->CloakModName, file.GetString(), MAX_CHXNAME_LENGTH);
			//SafeStrCpy(pGrade->ShoulderModName, file.GetString(), MAX_CHXNAME_LENGTH);

			pGrade->Speed = file.GetDword();
            pGrade->KyungGongSpeed[0] = file.GetWord();
			pGrade->KyungGongSpeed[1] = file.GetWord();
			pGrade->KyungGongSpeed[2] = file.GetWord();
			pGrade->Scale = file.GetFloat();
			pGrade->Tall = file.GetWord();
			pGrade->MaxFuel = file.GetDword();
			pGrade->MaxSpell = file.GetDword();
			//pGrade->Endurance = file.GetDword();
			pGrade->Critical = file.GetDword();
			pGrade->AttackRange = file.GetFloat();
			pGrade->MeleeAttackPowerMin = file.GetWord();
			pGrade->MeleeAttackPowerMax = file.GetWord();
			pGrade->RangeAttackPowerMin = file.GetWord();
			pGrade->RangeAttackPowerMax = file.GetWord();


			//pGrade->PhyAtk = file.GetFloat();
			pGrade->PhyDef = file.GetFloat();

			float attAtk = file.GetFloat();

			for( int att = ATTR_FIRE; att <= ATTR_MAX; ++att )
			{
				(pGrade->AttAtk).SetElement_Val(att, attAtk);
			}
			for( int att = ATTR_FIRE; att <= ATTR_MAX; ++att )
			{
				(pGrade->AttReg).SetElement_Val(att, file.GetFloat());
			}

			pGrade->RecallReduceTime = file.GetDword();
			pGrade->dwRecallSpellTime = file.GetDword(); //2007. 11. 19. CBH - 타이탄 소환 주문 시간 추가

		}

		if( m_TitanListTable.GetData(pList->TitanIdx) )
		{
			ASSERT(0);
		}

		m_TitanListTable.Add(pList, pList->TitanIdx);

		count++;
	}

	return TRUE;
}

BASE_TITAN_LIST* CGameResourceManager::GetTitanListInfo( DWORD idx )
{
	//return m_TitanListTable.GetData(idx);
	BASE_TITAN_LIST* pList = m_TitanListTable.GetData(idx);

	return pList;
}

TITANINFO_GRADE* CGameResourceManager::GetTitanGradeInfo( DWORD idx, DWORD grade )
{
	BASE_TITAN_LIST* pList = m_TitanListTable.GetData(idx);

	if(!pList)	return NULL;

	if( 3 < grade || grade < 1 )
	{
		ASSERT(0);
		return NULL;
	}

	return &pList->GradeInfo[grade-1];
}

void CGameResourceManager::RemoveTitanList()
{
	BASE_TITAN_LIST* pInfo = NULL;
	m_TitanListTable.SetPositionHead();
	while(pInfo = m_TitanListTable.GetData())
	{
		SAFE_DELETE(pInfo);
	}
	m_TitanListTable.RemoveAll();
}
/*
BOOL CGameResourceManager::LoadTitanSkillLink()
{
	CMHFile file;

#ifdef _FILE_BIN_
	if(!file.Init("./Resource/TitanSkillLink.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/TitanSkillLink.txt", "rt"))
		return FALSE;
#endif

	TITAN_SKILLLINK* pInfo = NULL;

	while(!file.IsEOF())
	{
		if( count >= totalCount )	break;

		pInfo = new TITAN_SKILLLINK;
		memset(pInfo, 0, sizeof(TITAN_SKILLLINK));

		pInfo->CharSkillIdx = file.GetDword();
		pInfo->TitanSkillIdx = file.GetDword();
#ifdef _DEBUG
		SafeStrCpy(pInfo->skillName, file.GetString(), MAX_NAME_LENGTH + 1);
#else
		file.GetString();
#endif
		if( m_TitanSkillLink.GetData(pInfo->CharSkillIdx) )
		{
			ASSERT(0);
			return FALSE;
		}

		m_TitanSkillLink.Add(pInfo, pInfo->CharSkillIdx);

		count++;
	}

	return TRUE;
}

TITAN_SKILLLINK* CGameResourceManager::GetTitanSkillIdx(DWORD charSkillIdx)
{
	TITAN_SKILLLINK* pInfo = (TITAN_SKILLLINK*)m_TitanSkillLink.GetData(charSkillIdx);

	return pInfo;
}

void CGameResourceManager::RemoveTitanSkillLink()
{
	TITAN_SKILLLINK* pInfo = NULL;
	m_TitanSkillLink.SetPositionHead();
	while(pInfo = m_TitanSkillLink.GetData())
	{
		SAFE_DELETE(pInfo);
	}
	m_TitanSkillLink.RemoveAll();
}*/

/*
BOOL CGameResourceManager::LoadTitanEquipEnduranceInfo()
{
	CMHFile file;

#ifdef _FILE_BIN_
	if(!file.Init("./Resource/TitanEndrncInfo_S.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/TitanEndrncInfo_S.txt", "rt"))
		return FALSE;
#endif

	TITAN_EQUIPITEM_ENDURANCE* pInfo = NULL;

	int totalCount = file.GetInt();
	int count = 0;

	while(!file.IsEOF())
	{
		if( count >= totalCount )	break;

		pInfo = new TITAN_EQUIPITEM_ENDURANCE;
		memset(pInfo, 0, sizeof(TITAN_EQUIPITEM_ENDURANCE));

		pInfo->titanEquipItemIdx = file.GetDword();
		pInfo->decreaseEndurance = file.GetDword();

		if( m_TitanEquipEnduranceInfo.GetData(pInfo->titanEquipItemIdx) )
		{
			ASSERT(0);
			return FALSE;
		}

		m_TitanEquipEnduranceInfo.Add(pInfo, pInfo->titanEquipItemIdx);

		count++;
	}

	return TRUE;
}

TITAN_EQUIPITEM_ENDURANCE* CGameResourceManager::GetTitanEquipEndurance( DWORD titanEquipItemIdx )
{
	return (TITAN_EQUIPITEM_ENDURANCE*)m_TitanEquipEnduranceInfo.GetData(titanEquipItemIdx);
}

DWORD CGameResourceManager::GetDecreaseEnduranceFromItemIdx( DWORD titanEquipItemIdx )
{
	TITAN_EQUIPITEM_ENDURANCE* pInfo = m_TitanEquipEnduranceInfo.GetData(titanEquipItemIdx);
	if(pInfo)
	{
		return pInfo->decreaseEndurance;
	}
	else
	{
		return 0;
	}
}

void CGameResourceManager::RemoveTitanEquipEnduranceInfo()
{
	TITAN_EQUIPITEM_ENDURANCE* pInfo = NULL;
	m_TitanEquipEnduranceInfo.SetPositionHead();
	while(pInfo = m_TitanEquipEnduranceInfo.GetData())
	{
		SAFE_DELETE(pInfo);
	}
	m_TitanEquipEnduranceInfo.RemoveAll();
}*/

BOOL CGameResourceManager::LoadTitanRule()
{
	CMHFile file;
	char szBuf[256] = {0,};

#ifdef _FILE_BIN_
	if(!file.Init("./Resource/TitanRule.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/TitanRule.txt", "rt"))
		return FALSE;
#endif

	while(!file.IsEOF())
	{
		strcpy(szBuf,strupr(file.GetString()));

		if(strcmp(szBuf,"#TITAN_RECALL_VALID_TIME_ZEROFUEL") ==0)
		{
			m_TitanRule.TitanRecallValidTime_ZeroFuel = file.GetDword();
		}
		if(strcmp(szBuf,"#TITAN_RECALL_VALID_TIME_SEAL") ==0)
		{
			m_TitanRule.TitanRecallValidTime_Seal = file.GetDword();
		}
		if(strcmp(szBuf,"#TITAN_RECALL_VALID_TIME_GENERAL") ==0)	// magi82(24)
		{
			m_TitanRule.TitanRecallValidTime_General = file.GetDword();
		}
		if(strcmp(szBuf,"#TITAN_VAMP_VALID_TIME") ==0)
		{
			m_TitanRule.TitanVampValidTime = file.GetDword();
		}
		if(strcmp(szBuf,"#TITAN_FUELRATE_FOR_VAMP") ==0)
		{
			m_TitanRule.TitanFuelRate_ForVamp = file.GetDword();
		}
		if(strcmp(szBuf,"#MASTER_LIFERATE_ALLOWFOR_TITANVAMP") ==0)
		{
			m_TitanRule.MasterLifeRate_AllowForTitanVamp = file.GetDword();
		}
		if(strcmp(szBuf,"#MASTER_LIFERATE_FROM_TITANVAMP") ==0)
		{
			m_TitanRule.MasterLifeRate_FromTitanVamp = file.GetDword();
		}
		if(strcmp(szBuf,"#FUEL_CONVERTRATE_FROM_TITANVAMP") ==0)
		{
			m_TitanRule.FuelConvertRate_FromTitanVamp = file.GetDword();
		}
		if(strcmp(szBuf,"#MASTER_LIFERATE_FOR_TITANCALL") ==0)
		{
			m_TitanRule.MasterLifeRate_forTitanCall = file.GetDword();
		}
		if(strcmp(szBuf,"#TITAN_DAMAGE_SHARERATE") ==0)
		{
			m_TitanRule.TitanDamageShareRate = file.GetDword();
		}
		if(strcmp(szBuf,"#PROB_DECREASE_ENDURANCE") ==0)
		{
			m_TitanRule.Prob_DecreaseEndurance = file.GetDword();
		}
		if(strcmp(szBuf,"#PROB_DECREASE_ARMOR_ENDURANCE_ATK") ==0)
		{
			m_TitanRule.Prob_DecreaseArmorEndurance_Atk = file.GetDword();
		}
		if(strcmp(szBuf,"#PROB_DECREASE_ARMOR_ENDURANCE_DEF") ==0)
		{
			m_TitanRule.Prob_DecreaseArmorEndurance_Def = file.GetDword();
		}
		if(strcmp(szBuf,"#TITAN_MAINTAIN_TIME") ==0)	// magi82(25)
		{
			m_TitanRule.TitanMaintainTime = file.GetDword();
		}
	}

	return TRUE;
}

BOOL CGameResourceManager::LoadFlagNpcInfo()
{
	CMHFile file;
	char szBuf[256] = {0,};
	char value[256] = {0,};

#ifdef _FILE_BIN_
	if(!file.Init("./Resource/FlagNpcInfo.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/FlagNpcInfo.txt", "rt"))
		return FALSE;
#endif

	while(!file.IsEOF())
	{
		strcpy(value,strupr(file.GetString()));

		if(strcmp(value,"#GTDAY_FLG") == 0)
		{
			for( int i = 0; i < Week; ++i )
			{
				FlagNpcSet[eGTFlg][i] = file.GetByte();
			}
		}
		if(strcmp(value,"#SGDAY_FLG") == 0)
		{
			for( int i = 0; i < Week; ++i )
			{
				FlagNpcSet[eSGFlg][i] = file.GetByte();
			}
		}
	}

	return TRUE;
}

BYTE CGameResourceManager::GetFlagFromDate( int flgKind, int day )
{
	if( flgKind >= eFlgKind || day >= Week )
	{
		char buf[128];
		sprintf(buf, "Check Date for Flag : Kind(%d), Day(%d)", flgKind, day);
		ASSERTMSG(0, buf);
		return 99;
	}

	BYTE flag = FlagNpcSet[flgKind][day];

	return flag;
}


BOOL CGameResourceManager::LoadDeveloperInfo()
{
	char filename[256];
	char value[256];
	CMHFile file;

#ifdef _MHCLIENT_
	FILE* fp = fopen("./AutopatchOption.opt","rb");

	if( fp != NULL )
	{	
		BOOL bWindowMode = FALSE;
		fscanf( fp, "%d", &bWindowMode );

		//SW061111 저해상도옵션
		fscanf( fp, "%d", &m_bLowResolution );

		fclose(fp);	
		
		if( bWindowMode == TRUE)
			m_GameDesc.dispInfo.dispType = WINDOW_WITH_BLT;
		else
			m_GameDesc.dispInfo.dispType = /*FULLSCREEN_WITH_FLIP;//*/FULLSCREEN_WITH_BLT;

		if( m_bLowResolution )
		{
			m_GameDesc.dispInfo.dwWidth = LOWRSLTN_W;
			m_GameDesc.dispInfo.dwHeight = LOWRSLTN_H;

			NOTIFYMGR->SetPosition( NOTIFYMSG_RSLTN_X, NOTIFYMSG_RSLTN_Y, NOTIFYMSG_DEFAULT_WIDTH );
		}
		else
		{
			m_GameDesc.dispInfo.dwWidth = MIDRSLTN_W;
			m_GameDesc.dispInfo.dwHeight = MIDRSLTN_H;

			NOTIFYMGR->SetPosition( NOTIFYMSG_DEFAULT_X, NOTIFYMSG_DEFAULT_Y, NOTIFYMSG_DEFAULT_WIDTH );
		}
	}
#endif

	sprintf(filename,"../DeveloperWorking/Developer.ini");
	if(!file.Init(filename, "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR))
		return FALSE;

	while(1)
	{
		if(file.IsEOF())
			break;

		strcpy(value,strupr(file.GetString()));
		if(strcmp(value,"*WINDOWMODE") == 0)
		{
#ifdef _MHCLIENT_
			if( file.GetBool() )
				m_GameDesc.dispInfo.dispType = WINDOW_WITH_BLT;
			else
				m_GameDesc.dispInfo.dispType = /*FULLSCREEN_WITH_FLIP;//*/FULLSCREEN_WITH_BLT;
#endif
		}
		else if(strcmp(value,"*ASSERTMSGBOX") == 0)
		{
			g_bAssertMsgBox = file.GetBool();
		}
		else if(strcmp(value,"*TESTSERVER") == 0)
		{
			g_bTESTSERVER = file.GetBool();
		}
	}
	file.Release();

	return TRUE;
}

void CGameResourceManager::ResetMonsterList()
{
	BASE_MONSTER_LIST * pInfo = NULL;
	m_SMonsterListTable.SetPositionHead();
	while(pInfo = m_SMonsterListTable.GetData())
	{
		delete [] pInfo->AttackIndex;
		delete [] pInfo->AttackRate;
		delete [] pInfo->SkillInfo;
		
		pInfo->AttackIndex = NULL;
		pInfo->AttackRate = NULL;
		pInfo->SkillInfo = NULL;
		delete pInfo;
		pInfo = NULL;
	}
	m_SMonsterListTable.RemoveAll();
}

void CGameResourceManager::LoadUserCommendList()
{
	m_UserCommendList.clear();

	CMHFile file;
#ifdef _FILE_BIN_
//	if(!file.Init("./Resource/UserCommandList.bin", "rb"))
	if(!file.Init("./Resource/Client/UserCommandList.bin", "rb"))
		return;
#else
	if(!file.Init("./Resource/UserCommandList.txt", "rt"))
		return;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;

		std::string szUserAction;

		int ActionNumber = file.GetInt();

		szUserAction = file.GetString();
		
		m_UserCommendList.push_back(szUserAction);
	}
}

void CGameResourceManager::LoadItemChangeRateofAttrib()
{
	m_ItemChangeRateAttribList.clear();

	CMHFile file;
#ifdef _FILE_BIN_
//	if(!file.Init("./Resource/AttribItemChangeRato.bin", "rb"))
	if(!file.Init("./Resource/Server/AttribItemChangeRato.bin", "rb"))
		return;
#else
	if(!file.Init("./Resource/AttribItemChangeRato.txt", "rt"))
		return;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;

		ItemChangeRateofAttrib ChangeRateInfo;

		for(int i = 0; i < 5; ++i)
		{
			ChangeRateInfo.attribinfo[i].changeIndex = file.GetInt();
			ChangeRateInfo.attribinfo[i].changeRate = file.GetInt();
		}

		m_ItemChangeRateAttribList.push_back(ChangeRateInfo);
	}

}

void CGameResourceManager::LoadItemChangeRateofLv()
{
	m_ItemChangeRateofLvList.clear();

	CMHFile file;
#ifdef _FILE_BIN_
//	if(!file.Init("./Resource/ItemChangeRateofLv.bin", "rb"))
	if(!file.Init("./Resource/Server/ItemChangeRateofLv.bin", "rb"))
	{
		::MessageBox(NULL,"ItemChangeRateofLv.bin - 파일이 올바로 로드되지 않았습니다.","경고",MB_OK);
		return;
	}
#else
	if(!file.Init("./Resource/ItemChangeRateofLv.txt", "rt"))
		return;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;

		ItemChangeRateofLv ChangeRateInfo;

		ChangeRateInfo.Level = file.GetInt();

		for(int i = 0; i < 10; ++i)
		{
			ChangeRateInfo.Rate[i] = file.GetInt();//변화율 저장
		}

		m_ItemChangeRateofLvList.push_back(ChangeRateInfo);
	}

	

}

int CGameResourceManager::CheckAttribItemIndex(WORD idx)
{
	for(unsigned int i = 0; i < m_ItemChangeRateAttribList.size(); ++i)
	{
		if(m_ItemChangeRateAttribList[i].attribinfo[0].changeIndex == idx)
			return i;
	}

	return -1;
}



void CGameResourceManager::LoadMonsterInfoInMapList()
{
	//파일을 로드하여 정보를 저장한다.
	
	m_MonsterInfoInMapList.clear();

	CMHFile file;
#ifdef _FILE_BIN_
//	if(!file.Init("./Resource/MonsterInfoInMap.bin", "rb"))
	if(!file.Init("./Resource/Client/MonsterInfoInMap.bin", "rb"))
		return;
#else
	if(!file.Init("./Resource/MonsterInfoInMap.txt", "rt"))
		return;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;
		
		MonsterInfoInMap MonsterInfo;

		MonsterInfo.MapNumber = file.GetInt();
		char* s = file.GetString();
		MonsterInfo.MinLevel = file.GetInt();
		MonsterInfo.MaxLevel = file.GetInt();

		m_MonsterInfoInMapList.push_back(MonsterInfo);
	}

	//unsigned int size = m_MonsterInfoInMapList.size();

}

unsigned int CGameResourceManager::GetMinMonsterLvInMap(unsigned int MapNum)
{
	unsigned int MinLv = 0;
	for(unsigned int i = 0; i < m_MonsterInfoInMapList.size(); ++i)
	{
		if(m_MonsterInfoInMapList[i].MapNumber == MapNum)
		{
			MinLv = m_MonsterInfoInMapList[i].MinLevel;
			return MinLv;
		}
	}

	return MinLv;
}

void CGameResourceManager::LoadSMonsterList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("./Resource/SMonsterList.bin", "rb"))
		return;
#else
	if(!file.Init("./Resource/SMonsterList.txt", "rt"))
		return;
#endif

	//SMONSTER_LIST * pSMonsterList = NULL;
	int num = file.GetInt();
	for( int i = 0 ; i < num ; ++i )
	{
		file.GetInt();
#ifndef _FILE_BIN_
		file.Seek(1);
#endif		
		LoadSMonsterInfo(i, file.GetString());
	}
}
BASE_MONSTER_LIST * CGameResourceManager::GetMonsterListInfo(int idx)
{
	return m_SMonsterListTable.GetData(idx);
}

int CGameResourceManager::GetMonsterIndexForName( char* pStrName )
{
	m_SMonsterListTable.SetPositionHead();
	BASE_MONSTER_LIST* pList;
	while( pList = m_SMonsterListTable.GetData() )
	{
		if( strcmp( pList->Name, pStrName ) == 0 )
			return pList->MonsterKind;
	}

	return 0;
}

void CGameResourceManager::LoadSMonsterInfo( int idx, char * fileName )
{
	SMONSTER_LIST * pList = new SMONSTER_LIST;
	memset(pList, 0, sizeof(SMONSTER_LIST));

	CMHFile fp;
	char buff[256];
#ifdef _FILE_BIN_
	sprintf(buff, "./Resource/%s", fileName);
	if(!fp.Init(buff, "rb"))
		return;
#else
	sprintf(buff, "./Resource/%s", fileName);
	if(!fp.Init(buff, "rt"))
		return;
#endif

	WORD aidx=0;
	WORD anum=0;
	DWORD ridx=0;
	DWORD attackIdx = 0;
	while(1)
	{
		fp.GetString(buff);
		if(fp.IsEOF())
			break;
		if(buff[0] == '@')
		{
			fp.GetLineX(buff, 256);
			continue;
		}
		else if(buff[0] == '{' || buff[0] == '}')
		{
			continue;
		}
		CMD_ST(buff)
		CMD_CS("#MONSTERKIND")
			pList->MonsterKind = fp.GetWord();
		CMD_CS("#NAME")
			SafeStrCpy( pList->Name, fp.GetStringInQuotation(), MAX_NAME_LENGTH+1 );
		CMD_CS("#ENGNAME")
			SafeStrCpy( pList->EngName, fp.GetStringInQuotation(), MAX_NAME_LENGTH+1 );			
		CMD_CS("#LEVEL")
			pList->Level = fp.GetLevel();
		CMD_CS("#SCALE")
			pList->Scale = fp.GetFloat();
		CMD_CS("#GRAVITY")
			pList->Gravity = fp.GetDword();
		CMD_CS("#WEIGHT")
			pList->Weight = fp.GetFloat();
		CMD_CS("#MONSTERRADIUS")
			pList->MonsterRadius = fp.GetDword();
		CMD_CS("#LIFE")
			pList->Life = fp.GetDword();
		CMD_CS("#SHIELD")
			pList->Shield = fp.GetDword();
		CMD_CS("#EXPPOINT")
			pList->ExpPoint = fp.GetExpPoint();
		CMD_CS("#TALL")
			pList->Tall = fp.GetWord();
		CMD_CS("#CHXNAME")
			fp.GetString(pList->ChxName);

		CMD_CS("#ATTACKPHYSICALMIN")
			pList->AttackPhysicalMin = fp.GetWord();
		CMD_CS("#ATTACKPHYSICALMAX")
			pList->AttackPhysicalMax = fp.GetWord();
		CMD_CS("#CRITICAL")
			pList->CriticalPercent = fp.GetWord();
		CMD_CS("#PHYSICALDEFENSE")
			pList->PhysicalDefense = fp.GetWord();
		CMD_CS("#PROPERTYRESIST1")
			pList->AttribResist.SetElement_Val(ATTR_FIRE,fp.GetFloat());
		CMD_CS("#PROPERTYRESIST2")
			pList->AttribResist.SetElement_Val(ATTR_WATER,fp.GetFloat());
		CMD_CS("#PROPERTYRESIST3")
			pList->AttribResist.SetElement_Val(ATTR_TREE,fp.GetFloat());
		CMD_CS("#PROPERTYRESIST4")
			pList->AttribResist.SetElement_Val(ATTR_IRON,fp.GetFloat());
		CMD_CS("#PROPERTYRESIST5")
			pList->AttribResist.SetElement_Val(ATTR_EARTH,fp.GetFloat());

		CMD_CS("#WALKMOVE")
			pList->WalkMove = fp.GetWord();
		CMD_CS("#RUNMOVE")
			pList->RunMove = fp.GetWord();
		CMD_CS("#RUNAWAYMOVE")
			pList->RunawayMove = fp.GetWord();
		
		CMD_CS("#STANDTIME")
			pList->StandTime = fp.GetDword();
		CMD_CS("#STANDRATE")
			pList->StandRate = fp.GetByte();
		CMD_CS("#DOMAINRANGE")
			pList->DomainRange = fp.GetDword();
		CMD_CS("#PURSUITFORGIVETIME")
			pList->PursuitForgiveTime = fp.GetDword();
		CMD_CS("#PURSUITFORGIVEDISTANCE")
			pList->PursuitForgiveDistance = fp.GetDword();
		CMD_CS("#FOREATTACK")
			pList->bForeAttack = fp.GetBool();
		CMD_CS("#SEARCHPERIODICTIME")
			pList->SearchPeriodicTime = fp.GetDword();
		CMD_CS("#TARGETSELECT")
			pList->TargetSelect = fp.GetWord();
		CMD_CS("#SEARCHRANGE")
			pList->SearchRange = fp.GetDword();
		CMD_CS("#SEARCHANGLE")
			pList->SearchAngle = fp.GetDword();
		CMD_CS("#TARGETCHANGE")
			pList->TargetChange = fp.GetWord();

		// 수정 되어야 할 부분
		CMD_CS("#MOVEDRAMANUM")
			pList->MoveDramaNum = FindEffectNum(fp.GetString());
		CMD_CS("#MOTIONID");
			pList->MotionID = fp.GetDword();
		CMD_CS("#DAMAGE1DRAMANUM")
			pList->Damage1DramaNum = FindEffectNum(fp.GetString());
		CMD_CS("#DIE1DRAMANUM")
			pList->Die1DramaNum = FindEffectNum(fp.GetString());
		CMD_CS("#DIE2DRAMANUM")
			pList->Die2DramaNum = FindEffectNum(fp.GetString());


		CMD_CS("#ATTACKNUM")
			pList->AttackNum = fp.GetDword();
	
			pList->AttackIndex = new DWORD[pList->AttackNum];
			pList->AttackRate = new DWORD[pList->AttackNum];
			pList->SkillInfo = new CSkillInfo*[pList->AttackNum];
			for(DWORD i = 0 ; i < pList->AttackNum ; ++i)
			{
				pList->AttackIndex[i] = 0;
				pList->AttackRate[i] = 0;
				pList->SkillInfo[i] = 0;
			}
		CMD_CS("#ATTACKKIND")
			pList->AttackKind = fp.GetWord();
		CMD_CS("#ATTACK")
			attackIdx =  fp.GetDword();
			ASSERT( attackIdx < pList->AttackNum );
		CMD_CS("#ATTACKINDEX")
			pList->AttackIndex[attackIdx] = fp.GetDword();
			pList->SkillInfo[attackIdx] = SKILLMGR->GetSkillInfo((WORD)pList->AttackIndex[attackIdx]);
			ASSERT(pList->SkillInfo[attackIdx]);
		CMD_CS("#ATTACKRATE")
			pList->AttackRate[attackIdx] = fp.GetDword();
		
		// ATTACK PATTERN INFO
		CMD_CS("#ATTACKPATTERNNUM")
			pList->AttackPatternClass.Init(fp.GetWord());
		CMD_CS("#ATTACKPATTERN")
			aidx = fp.GetWord();
			anum = fp.GetWord();
		CMD_CS("#ATTACKPATTERNINDEX")
			pList->AttackPatternClass.InitPattern(aidx, anum);
			for(int i = 0 ; i < anum ; ++i )
				pList->AttackPatternClass.SetVal(aidx, i, fp.GetWord());
		
		//RECALL INFOMATION
		CMD_CS("#RECALLNUM")
			pList->RecallInfoClass.Init(fp.GetDword());
		CMD_CS("#RECALL")
			ridx = fp.GetDword();
		CMD_CS("#RECALLTYPE")
			pList->RecallInfoClass.SetRecallType(ridx, fp.GetDword());
		CMD_CS("#RECALLID")
			pList->RecallInfoClass.SetRecallID(ridx, fp.GetDword());
		CMD_CS("#RECALLMONSTERNUM")
			pList->RecallInfoClass.SetRecallMonsterNum(ridx, fp.GetDword());
		CMD_CS("#RECALLRANGE")
			pList->RecallInfoClass.SetRecallRange(ridx, fp.GetDword());
			
		//HELP
		CMD_CS("#INITHELPTYPE")
			pList->InitHelpType = fp.GetWord();
		CMD_CS("#HEARING")
			pList->bHearing = fp.GetBool();
		CMD_CS("#HEARINGDISTANCE")
			pList->HearingDistance = fp.GetDword();

		CMD_EN
	}

	ASSERT(m_SMonsterListTable.GetData(pList->MonsterKind) == NULL);
/*
#ifdef _TESTCLIENT_
		if(m_SMonsterListTable.GetData(pList->MonsterKind))
			m_SMonsterListTable.Remove(pList->MonsterKind);
#endif */

	m_SMonsterListTable.Add(pList, pList->MonsterKind);
}

BOOL CGameResourceManager::LoadMonsterList()
{
	char filename[64];
//	char value[64] = {0,};
	CMHFile file;

#ifdef _FILE_BIN_
	sprintf(filename,"./Resource/MonsterList.bin");	
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"./Resource/MonsterList.txt");	
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif

	int n =0;
	MONSTEREX_LIST * pList = NULL;
	while(1)
	{
		if(n == MAX_MONSTER_NUM)
			break;

		if(file.IsEOF())
			break;
		pList = new MONSTEREX_LIST;
		memset(pList, 0, sizeof(MONSTEREX_LIST));
		pList->MonsterKind				= file.GetWord();		// 번호
//		file.GetString(pList->Name);						// 몬스터이름
//		file.GetString(pList->EngName);						// 영어이름
		SafeStrCpy( pList->Name, file.GetString(), MAX_MONSTER_NAME_LENGTH+1 );
		SafeStrCpy( pList->EngName, file.GetString(), MAX_NAME_LENGTH+1 );
		pList->Level					= file.GetLevel();		// 몬스터레벨	
		pList->MoveDramaNum				= FindEffectNum(file.GetString());	
		pList->MotionID					= file.GetDword();	
//		file.GetString(pList->ChxName);
		SafeStrCpy( pList->ChxName, file.GetString(), 32 );
		pList->Scale					= file.GetFloat();// 몬스터 크기	
		pList->Gravity					= file.GetDword();
#ifdef _HK_LOCAL_
		pList->Gravity	= (DWORD)(100.f / ( ( 100.f / (float)(pList->Gravity) ) + 3.f ));
#endif
		pList->Weight					= file.GetFloat();// 몬스터 무게
		pList->MonsterRadius			= file.GetWord();// 몬스터 	
		pList->Life						= file.GetDword();
		pList->Shield					= file.GetDword();
		pList->ExpPoint					= file.GetExpPoint();// 경험치
		pList->Tall						= file.GetWord();

		pList->AttackPhysicalMin		= file.GetWord();// 공격 최소물리 공격력	
		pList->AttackPhysicalMax		= file.GetWord();// 공격 최대물리 공격력	
		pList->CriticalPercent			= file.GetWord();// 크리티컬 퍼센트
		pList->PhysicalDefense			= file.GetWord();// 물리 방어력	
		pList->AttribResist.SetElement_Val(ATTR_FIRE,file.GetFloat());// 속성저항력(화)% 	
		pList->AttribResist.SetElement_Val(ATTR_WATER,file.GetFloat());// 속성저항력(수)% 	
		pList->AttribResist.SetElement_Val(ATTR_TREE,file.GetFloat());// 속성저항력(목)% 	
		pList->AttribResist.SetElement_Val(ATTR_IRON,file.GetFloat());// 속성저항력(금)% 	
		pList->AttribResist.SetElement_Val(ATTR_EARTH,file.GetFloat());// 속성저항력(토)% 

		pList->WalkMove					= file.GetWord();// 몬스터걷기이동력	
		pList->RunMove					= file.GetWord();// 몬스터뛰기이동력	
		pList->RunawayMove				= file.GetWord();// 몬스터도망이동력
		
		pList->Damage1DramaNum			= FindEffectNum(file.GetString());// 맞음 1 연출 번호	
		pList->Die1DramaNum				= FindEffectNum(file.GetString());// 죽음 1 연출 번호	
		pList->Die2DramaNum				= FindEffectNum(file.GetString());// 죽음 2 연출 번호	

		pList->StandTime				= file.GetDword();
		pList->StandRate				= file.GetByte();
		pList->WalkRate					= file.GetByte();
		pList->RunRate					= file.GetByte();
		pList->DomainRange				= file.GetDword();		// 영역 범위 : 반지름
		pList->PursuitForgiveTime		= file.GetDword();
		pList->PursuitForgiveDistance	= file.GetDword();
		
		pList->bForeAttack				= file.GetBool();					// 선공격
		pList->SearchPeriodicTime		= file.GetDword();				// 탐색 주기
		pList->TargetSelect				= file.GetWord();					// 타겟 선택 : FIND_CLOSE, FIND_FIRST
		pList->SearchRange				= file.GetWord();
		pList->SearchAngle				= file.GetByte();
		pList->TargetChange				= file.GetWord();

		pList->AttackNum				= file.GetWord();
		pList->AttackIndex				= new DWORD[2];
		pList->AttackRate				= new DWORD[2];
		pList->SkillInfo				= new CSkillInfo*[2];
			
		pList->AttackIndex[0]			= file.GetWord();
		pList->AttackIndex[1]			= file.GetWord();
		pList->SkillInfo[0]				= SKILLMGR->GetSkillInfo((WORD)pList->AttackIndex[0]);
		pList->SkillInfo[1]				= SKILLMGR->GetSkillInfo((WORD)pList->AttackIndex[1]);
		pList->AttackRate[0]			= file.GetWord();
		pList->AttackRate[1]			= file.GetWord();

		pList->MinDropMoney				= file.GetDword();// 최소돈 액수	
		pList->MaxDropMoney				= file.GetDword();// 최대 돈 액수	
		ASSERT(pList->MaxDropMoney >= pList->MinDropMoney);
		pList->DropRate[eMONEY]			= file.GetWord();// 돈 확률(%)	
		pList->DropRate[eITEM1]			= file.GetWord();// 아이템 확률(%)	
		pList->DropRate[eITEM2]			= file.GetWord();// 아이템 확률(%)	
		pList->DropRate[eITEM3]			= file.GetWord();// 아이템 확률(%)	
		pList->DropRate[eNOITEM]			= file.GetWord();// 노템 확률(%)	

#ifndef _MHCLIENT_
		DWORD temp = 0;
		for(int t=0;t<eDROPITEMKIND_MAX;++t)
		{
			pList->CurDropRate[t] = pList->DropRate[t];
			temp += pList->DropRate[t];
		}

		if( temp != MAX_DROP_ITEM_PERCENT )
			ASSERTMSG( 0, "drop rate error! in MonsterList" );
#endif
		pList->ItemListNum1				= file.GetWord();// 해당아이템리스트표번호	
		pList->ItemListNum2				= file.GetWord();// 해당아이템리스트표번호	
		pList->ItemListNum3				= file.GetWord();// 해당아이템리스트표번호	

		pList->MaxDropItemNum			= file.GetByte();// 최대아이템 갯수

		pList->fRunawayLifeRate			= file.GetFloat();
		pList->RunawayRate				= file.GetWord();
		pList->RunawayHelpRate			= file.GetWord();
		pList->RunawayDistance			= file.GetWord();	//????

		pList->Talk1					= file.GetWord();				// 대사 1
		pList->Talk2					= file.GetWord();				
		pList->Talk3					= file.GetWord();

		pList->HelpRequestCondition		= file.GetWord();
		pList->HelpRequestDistance		= file.GetWord();
		pList->HelpRequestTargetNum		= file.GetWord();

		pList->RecallCondition			= file.GetWord();
		pList->RecallObjectID			= file.GetWord();
		pList->RecallObjectNum			= file.GetWord();

		pList->InitHelpType				= file.GetWord();
		pList->bHearing					= file.GetBool();
		pList->HearingDistance			= file.GetDword();
#ifdef _CHINA_LOCAL_
		pList->ObjectKind				= 32;
#else
		pList->ObjectKind				= file.GetWord();
#endif

		ASSERT(!m_SMonsterListTable.GetData(pList->MonsterKind));

		m_SMonsterListTable.Add(pList, pList->MonsterKind);

		++n;
	}

	return TRUE;
}

BOOL CGameResourceManager::LoadNpcList(NPC_LIST* pNpcList)
{
	CMHFile fp;
#ifdef _FILE_BIN_
	fp.Init("./Resource/NpcList.bin", "rb");
#else
	fp.Init("./Resource/NpcList.txt", "rt");
#endif
	int idx = 0;
	if(!fp.IsInited()) return FALSE;
	while(1)
	{
		if(fp.IsEOF())
			break;
		
		pNpcList[idx].NpcKind = fp.GetWord();
		//fp.GetString(pNpcList[idx].Name);
		SafeStrCpy(pNpcList[idx].Name, fp.GetString(), MAX_NAME_LENGTH+1 );
		pNpcList[idx].ModelNum = fp.GetWord();
		pNpcList[idx].JobKind = fp.GetWord();
		pNpcList[idx].Scale = fp.GetFloat(); // Npc 크기
		pNpcList[idx].Tall	= fp.GetWord(); // Npc 키
		pNpcList[idx].ShowJob = fp.GetBool();
		
		
		idx++;
	}

	fp.Release();
	
#ifdef _SERVER_RESOURCE_FIELD_
	SetNpcListLoadFlag(TRUE);
#endif
	
	return TRUE;
}

#ifdef _SERVER_RESOURCE_FIELD_
NPC_LIST* CGameResourceManager::GetNpcList(WORD JobKind)
{
	for(int i=0; i<MAX_NPC_NUM; ++i)
	{
		if(m_NpcList[i].NpcKind == JobKind)
			return &m_NpcList[i];
	}
	return NULL;
}
#endif

BOOL CGameResourceManager::LoadMapChangeArea()//060608 - Add by wonju
{
	char filename[64];
	CMHFile file;
#ifdef _FILE_BIN_
	sprintf(filename,"./Resource/MapChange.bin");	
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"./Resource/MapChange.txt");	
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif

	int n =0;
	while(1)
	{
		if(file.IsEOF())
			break;

		m_MapChangeArea[n].Kind = file.GetWord();
		//		file.GetString(m_MapChangeArea[n].CurMapName);
		//		file.GetString(m_MapChangeArea[n].ObjectName);
		SafeStrCpy( m_MapChangeArea[n].CurMapName, file.GetString(), MAX_NAME_LENGTH+1 );
		SafeStrCpy( m_MapChangeArea[n].ObjectName, file.GetString(), MAX_NAME_LENGTH+1 );

		m_MapChangeArea[n].CurMapNum = file.GetWord();
		m_MapChangeArea[n].MoveMapNum = file.GetWord();
		m_MapChangeArea[n].CurPoint.x = file.GetFloat();
		m_MapChangeArea[n].CurPoint.y = 0;
		m_MapChangeArea[n].CurPoint.z = file.GetFloat();
		m_MapChangeArea[n].MovePoint.x = file.GetFloat();
		m_MapChangeArea[n].MovePoint.y = 0;
		m_MapChangeArea[n].MovePoint.z = file.GetFloat();
		m_MapChangeArea[n].chx_num = file.GetWord();

		++n;
	}

	return TRUE;

}

BOOL CGameResourceManager::LoadMapChangeArea(MAPCHANGE_INFO* pMapChangeArea)
{
	char filename[64];
	CMHFile file;
#ifdef _FILE_BIN_
	sprintf(filename,"./Resource/MapChange.bin");	
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"./Resource/MapChange.txt");	
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif

	int n =0;
	while(1)
	{
		if(file.IsEOF())
			break;
		
		pMapChangeArea[n].Kind = file.GetWord();
//		file.GetString(pMapChangeArea[n].CurMapName);
//		file.GetString(pMapChangeArea[n].ObjectName);
		SafeStrCpy( pMapChangeArea[n].CurMapName, file.GetString(), MAX_NAME_LENGTH+1 );
		SafeStrCpy( pMapChangeArea[n].ObjectName, file.GetString(), MAX_NAME_LENGTH+1 );

		pMapChangeArea[n].CurMapNum = file.GetWord();
		pMapChangeArea[n].MoveMapNum = file.GetWord();
		pMapChangeArea[n].CurPoint.x = file.GetFloat();
		pMapChangeArea[n].CurPoint.y = 0;
		pMapChangeArea[n].CurPoint.z = file.GetFloat();
		pMapChangeArea[n].MovePoint.x = file.GetFloat();
		pMapChangeArea[n].MovePoint.y = 0;
		pMapChangeArea[n].MovePoint.z = file.GetFloat();
		pMapChangeArea[n].chx_num = file.GetWord();
		
		++n;
	}

	return TRUE;
}

BOOL CGameResourceManager::LoadLoginPoint(LOGINPOINT_INFO* pLogin)
{
	char filename[64];
	CMHFile file;
#ifdef _FILE_BIN_
//	sprintf(filename,"Resource/LoginPoint.bin");	
	sprintf(filename,"Resource/Server/LoginPoint.bin");
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/LoginPoint.txt");	
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif

	int n =0;
	while(1)
	{
		if(file.IsEOF())
			break;
		pLogin[n].Kind = file.GetWord();
		//file.GetString(pLogin[n].MapName);
		SafeStrCpy( pLogin[n].MapName, file.GetString(), MAX_NAME_LENGTH+1 );
		pLogin[n].MapNum = file.GetWord();
		pLogin[n].MapLoginPointNum = file.GetByte();
		if(pLogin[n].MapLoginPointNum == 0)
		{
			MessageBox(NULL,"LoginPointNum is Zero!!!",0,0);
		}
		if(pLogin[n].MapLoginPointNum > 10)
		{
			MessageBox(NULL,"LoginPointNum is too big!!!",0,0);
		}
		for(int i=0;i<pLogin[n].MapLoginPointNum;++i)
		{
			pLogin[n].CurPoint[i].x = file.GetFloat();
			pLogin[n].CurPoint[i].y = 0;
			pLogin[n].CurPoint[i].z = file.GetFloat();
		}
		pLogin[n].chx_num = file.GetWord();
		++n;
	}

	return TRUE;
}

#ifdef _MAPSERVER_
BOOL CGameResourceManager::CheckCorrectLoginPoint(VECTOR3* pos)
{
	static float maxdist = 1000;
	for( int i = 0; i < MAX_POINT_NUM; i++)
	{
		if(m_LoginPoint[i].MapNum == GetLoadMapNum())
		{
			for(int n=0;n<m_LoginPoint[i].MapLoginPointNum;++n)
			{
				float dist = CalcDistanceXZ(pos,&m_LoginPoint[i].CurPoint[n]);
				if(dist < maxdist)	//임의
					return TRUE;
			}
		}
	}
	for( i = 0; i < MAX_POINT_NUM; i++)
	{
		if(m_MapChangeArea[i].MoveMapNum == GetLoadMapNum())
		{
			float dist = CalcDistanceXZ(pos,&m_MapChangeArea[i].MovePoint);
			if(dist < maxdist)	//임의
				return TRUE;
		}
	}
	
	return FALSE;
}


void CGameResourceManager::GetRegenPoint(VECTOR3& pos,VECTOR3& RandPos,WORD LoginIndex,WORD MapchangeIndex)
{
	if(MapchangeIndex)
	{
		MAPCHANGE_INFO* changeinfo = GetMapChangeInfo(MapchangeIndex);
		if(!changeinfo)
		{
			char temp[128];
			sprintf(temp,"MapChangePoint is abnormal mappoint:%d,loginpoin:%d",MapchangeIndex,LoginIndex);
			ASSERTMSG(0,temp);
			pos.x = 25000;
			pos.y = 0;
			pos.z = 25000;
		}
		else
		{
			if(changeinfo->MoveMapNum != GAMERESRCMNGR->GetLoadMapNum())
				MapchangeIndex = 0;
			else
			{
				pos.x = changeinfo->MovePoint.x;
				pos.y = 0;
				pos.z = changeinfo->MovePoint.z;
			}
		}
	}
	if(MapchangeIndex == 0)
	{
		LOGINPOINT_INFO* LoginPoint = GetLoginPointInfoFromMapNum(GAMERESRCMNGR->GetLoadMapNum());//LoginIndex);
		if(!LoginPoint)
		{
			char temp[128];
			sprintf(temp,"LoginPoint is abnormal mappoint:%d,loginpoin:%d",MapchangeIndex,LoginIndex);
			ASSERTMSG(0,temp);
			pos.x = 25000;
			pos.y = 0;
			pos.z = 25000;
		}
		else
		{
			int rl = rand() % LoginPoint->MapLoginPointNum;
			pos.x = LoginPoint->CurPoint[rl].x;
			pos.y = 0;
			pos.z = LoginPoint->CurPoint[rl].z;
		}
	}
	
	int temp;
	temp = rand() % 500 - 250;
	RandPos.x = pos.x + temp;
	temp = rand() % 500 - 250;
	RandPos.z = pos.z + temp;
	RandPos.y = 0;

	ASSERT(!(pos.x == 0 && pos.z == 0));
}

void CGameResourceManager::GetRegenPointFromEventMap( VECTOR3& pos,VECTOR3& RandPos,WORD LoginIndex,WORD MapchangeIndex,DWORD dwTeam)
{
	if(MapchangeIndex)
	{
		MAPCHANGE_INFO* changeinfo = GetMapChangeInfo(MapchangeIndex);
		if(!changeinfo)
		{
			char temp[128];
			sprintf(temp,"MapChangePoint is abnormal mappoint:%d,loginpoin:%d",MapchangeIndex,LoginIndex);
			ASSERTMSG(0,temp);
			pos.x = 25000;
			pos.y = 0;
			pos.z = 25000;
		}
		else
		{
			if(changeinfo->MoveMapNum != GAMERESRCMNGR->GetLoadMapNum())
				MapchangeIndex = 0;
			else
			{
				pos.x = changeinfo->MovePoint.x;
				pos.y = 0;
				pos.z = changeinfo->MovePoint.z;
			}
		}
	}
	if(MapchangeIndex == 0)
	{
		LOGINPOINT_INFO* LoginPoint = GetLoginPointInfoFromMapNum(GAMERESRCMNGR->GetLoadMapNum());//LoginIndex);
		if(!LoginPoint)
		{
			char temp[128];
			sprintf(temp,"LoginPoint is abnormal mappoint:%d,loginpoin:%d",MapchangeIndex,LoginIndex);
			ASSERTMSG(0,temp);
			pos.x = 25000;
			pos.y = 0;
			pos.z = 25000;
		}
		else
		{
			int rl = dwTeam - 1;
			pos.x = LoginPoint->CurPoint[rl].x;
			pos.y = 0;
			pos.z = LoginPoint->CurPoint[rl].z;
		}
	}
	
	int temp;
	temp = rand() % 500 - 250;
	RandPos.x = pos.x + temp;
	temp = rand() % 500 - 250;
	RandPos.z = pos.z + temp;
	RandPos.y = 0;

	ASSERT(!(pos.x == 0 && pos.z == 0));
}



#endif

//-----------------------------------------------------------------------------------------------------------//
//		서버만 사용하는 부분
#ifdef _SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//

void CGameResourceManager::GetMonsterStats(WORD wMonsterKind, monster_stats * stats)
{
/*	stats->AttributeAttackMin1 = GetMonsterInfo(wMonsterKind)->Attack1Property;
	stats->AttributeAttackMax1 = GetMonsterInfo(wMonsterKind)->Attack2Property;
	stats->AttributeAttackMin2 = GetMonsterInfo(wMonsterKind)->Attack1Property;
	stats->AttributeAttackMax2 = GetMonsterInfo(wMonsterKind)->Attack2Property;*/

	stats->Attack1OccurProportion = (WORD)GetMonsterListInfo(wMonsterKind)->AttackRate[0];				// 공격1 발생 확률
	stats->PhysicalAttackMin1 = GetMonsterListInfo(wMonsterKind)->AttackPhysicalMin;						// 공격1 최소 공격력
	stats->PhysicalAttackMax1 = GetMonsterListInfo(wMonsterKind)->AttackPhysicalMax;
//	stats->PhysicalAttackMin2 = GetMonsterInfo(wMonsterKind)->Attack2PhysicalMin;						// 공격1 최소 공격력
//	stats->PhysicalAttackMax2 = GetMonsterInfo(wMonsterKind)->Attack2PhysicalMax;
	// 방어
	stats->PhysicalDefense = GetMonsterListInfo(wMonsterKind)->PhysicalDefense;
	stats->AttributeResist = GetMonsterListInfo(wMonsterKind)->AttribResist;
}

BOOL CGameResourceManager::LoadNpcRegen(NPC_REGEN pNpcRegen[MAX_NPC_GROUPNUM][MAX_NPC_REGEN_NUM])
{
	char filename[64];
	char value[64] = {0,};
	CMHFile file;
#ifdef _FILE_BIN_
	sprintf(filename,"./Resource/NpcRegen.bin");	
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"./Resource/NpcRegen.txt");	
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif

	int n =0;
	WORD Group = 1;
	WORD count = 1;
	
	while(1)
	{
		if(file.IsEOF())
			break;
		if(Group > MAX_NPC_GROUPNUM || n > MAX_NPC_REGEN_NUM) break;

		if(count > MONSTERGROUPUNIT)
		{
			count = 1;
			n = 1;
			Group++;
			m_TotalNpcGroupCount++;
		}
		pNpcRegen[Group][n].MapNum = file.GetWord();
		if(pNpcRegen[Group][n].MapNum == 0)
			break;

		pNpcRegen[Group][n].NpcKind = file.GetWord();
		//file.GetString(pNpcRegen[Group][n].Name);
		SafeStrCpy( pNpcRegen[Group][n].Name, file.GetString(), MAX_NAME_LENGTH+1 );

		pNpcRegen[Group][n].NpcIndex = (WORD)file.GetDword();
		pNpcRegen[Group][n].Pos.x = file.GetFloat();
		pNpcRegen[Group][n].Pos.y = 0;
		pNpcRegen[Group][n].Pos.z = file.GetFloat();
		pNpcRegen[Group][n].Angle = file.GetFloat();
		count++;
		n++;
		if(n > m_MaxNpcRegenCount)
			m_MaxNpcRegenCount = n;
	}
	SetNpcRegenLoadFlag(TRUE);
	return TRUE;
}


void CGameResourceManager::SetLoadMapNum(WORD Mapnum)
{
	m_LoadingMapNum = Mapnum;
}

BOOL CGameResourceManager::LoadStaticNpc()
{
	CMHFile file;
	char filename[64];
#ifdef _FILE_BIN_
	sprintf( filename, "./Resource/StaticNpc.bin" );	
	if( !file.Init( filename, "rb" ) )
		return FALSE;
#else
	sprintf( filename,"./Resource/StaticNpc.txt" );	
	if( !file.Init( filename, "rt" ) )
		return FALSE;
#endif

	STATIC_NPCINFO Info;
	while( 1 )
	{
		if( file.IsEOF() )
			break;

		Info.MapNum = file.GetWord();
		Info.wNpcJob = file.GetWord();
		//file.GetString( Info.NpcName );
		SafeStrCpy( Info.NpcName, file.GetString(), MAX_NAME_LENGTH+1 );
		Info.wNpcUniqueIdx = file.GetWord();
		Info.vPos.x = file.GetFloat();
		Info.vPos.y = 0.0f;
		Info.vPos.z = file.GetFloat();
		file.GetFloat();

		if( m_LoadingMapNum != Info.MapNum )
			continue;

		STATIC_NPCINFO* pInfo = new STATIC_NPCINFO;
		*pInfo = Info;


		m_StaticNpcTable.Add( pInfo, pInfo->wNpcUniqueIdx );

		if( pInfo->wNpcJob == 10 || pInfo->wNpcJob == 84)
			m_WareNpcTable.Add( pInfo, pInfo->wNpcUniqueIdx );
	}

	file.Release();

#ifdef _FILE_BIN_
//	sprintf( filename, "./Quest/QuestNpcList.bin" );	
	sprintf( filename, "./Resource/QuestScript/QuestNpcList.bin" );
	if( !file.Init( filename, "rb" ) )
		return FALSE;
#else
//	sprintf( filename, "./Quest/QuestNpcList.txt" );	
	sprintf( filename, "./Resource/QuestScript/QuestNpcList.txt" );
	if( !file.Init( filename, "rt" ) )
		return FALSE;
#endif

	while( 1 )
	{
		if( file.IsEOF() )
			break;

		Info.MapNum = file.GetWord();
		Info.wNpcJob = file.GetWord();
//		file.GetString( Info.NpcName );
		SafeStrCpy( Info.NpcName, file.GetString(), MAX_NAME_LENGTH+1 );
		Info.wNpcUniqueIdx = file.GetWord();
		Info.vPos.x = file.GetFloat();
		Info.vPos.y = 0.0f;
		Info.vPos.z = file.GetFloat();
		file.GetFloat();

		if( m_LoadingMapNum != Info.MapNum )
			continue;

		STATIC_NPCINFO* pInfo = new STATIC_NPCINFO;
		*pInfo = Info;
		m_StaticNpcTable.Add( pInfo, pInfo->wNpcUniqueIdx );
	}

	file.Release();

	// 요새전 관련 npc정보 읽음...
#ifdef _FILE_BIN_
	sprintf( filename, "./Resource/Server/FortWarNpcInfo.bin" );
	if( !file.Init( filename, "rb" ) )
		return FALSE;
#else
	sprintf( filename, "./Resource/Server/FortWarNpcInfo.txt" );
	if( !file.Init( filename, "rt" ) )
		return FALSE;
#endif

	while( 1 )
	{
		if( file.IsEOF() )
			break;

		Info.MapNum = file.GetWord();
		Info.wNpcJob = file.GetWord();
		SafeStrCpy( Info.NpcName, file.GetString(), MAX_NAME_LENGTH+1 );
		Info.wNpcUniqueIdx = file.GetWord();
		Info.vPos.x = file.GetFloat();
		Info.vPos.y = 0.0f;
		Info.vPos.z = file.GetFloat();
		file.GetFloat();

		if( m_LoadingMapNum != Info.MapNum )
			continue;

		STATIC_NPCINFO* pInfo = new STATIC_NPCINFO;
		*pInfo = Info;
		m_StaticNpcTable.Add( pInfo, pInfo->wNpcUniqueIdx );
	}

	file.Release();
	//

	return TRUE;
}


BOOL CGameResourceManager::IsInWareNpcPosition( VECTOR3* pvPos )
{
	float dist = 0.0f;
	STATIC_NPCINFO* pInfo = NULL;
	m_WareNpcTable.SetPositionHead();
	while( pInfo = m_WareNpcTable.GetData() )
	{
		dist = CalcDistanceXZ( pvPos, &pInfo->vPos );
		if( dist <= 5000.0f )	return TRUE;
	}

	return FALSE;
}


void CGameResourceManager::ReleaseStaticNpc()
{
	STATIC_NPCINFO* pData = NULL;
	m_StaticNpcTable.SetPositionHead();
	while( pData = m_StaticNpcTable.GetData() )
		delete pData;
	m_StaticNpcTable.RemoveAll();
}

STATIC_NPCINFO* CGameResourceManager::GetStaticNpcInfo( WORD wNpcUniqueIdx )
{
	return m_StaticNpcTable.GetData( wNpcUniqueIdx );	
}

//-----------------------------------------------------------------------------------------------------------//
#endif //_SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
BOOL CGameResourceManager::LoadExpPoint()
{
	char filename[64];
	CMHFile file;
#ifdef _FILE_BIN_
	sprintf(filename,"Resource/characterExpPoint.bin");	
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/characterExpPoint.txt");	
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif

	int idx =0;
	while(1)
	{
		if(idx > MAX_CHARACTER_LEVEL_NUM-1) break;
		if(file.IsEOF())
			break;
		file.GetWord();												// 레벨
		m_CharacterExpPoint[idx] = file.GetExpPoint64();				// 경험치
		idx++;
	}

	file.Release();
	return TRUE;	
}

BOOL CGameResourceManager::LoadBobusangNpcInfo()
{
	CMHFile file;

	if( FALSE == file.Init("resource/Server/BobusangInfo.bin", "rb") )
	{
		MessageBox(NULL,"Can't Open BobusangInfo File", NULL, NULL);
		return FALSE;
	}

	char paserTmp[256] = {0,};
	DWORD	dwTmp = 0;

	file.GetString();	//#REGEN_TIMEGAP
	dwTmp = file.GetDword();	//m_dwAppearTimeMin 
	dwTmp = file.GetDword();	//m_dwAppearTimeMax 

	file.GetString();	//#DURATION_TIMEGAP
	dwTmp = file.GetDword();	//m_dwDurationTimeMin
	dwTmp = file.GetDword();	//m_dwDurationTimeMax 

	int PosNum, MapNum;
	char* pMapNum = NULL;

	while( FALSE == file.IsEOF() )
	{
		PosNum = MapNum = 0;

		file.GetString(paserTmp);
		//strcpy( paserTmp, file.GetString() );

		if( '#' == paserTmp[0] )
		{
			pMapNum = paserTmp;
			++pMapNum;

			MapNum = atoi(pMapNum);

			if( 0 > MapNum || MapNum > MAX_MAP_ENUMCOUNT )
			{
				return FALSE;
			}

			while( PosNum < BOBUSANG_POSNUM_MAX )
			{
				m_PosPerMap[MapNum][PosNum].ApprPos.x	= file.GetFloat();
				m_PosPerMap[MapNum][PosNum].ApprPos.z	= file.GetFloat();
				m_PosPerMap[MapNum][PosNum].ApprDir		= file.GetFloat();
				++PosNum;
			}
		}
	}

	return TRUE;
}

BobusangPosPerMap* CGameResourceManager::GetBobusangPos( DWORD mapNum, DWORD posNum )
{
	BobusangPosPerMap* pPos = &m_PosPerMap[mapNum][posNum];

	return pPos;
}

BOOL CGameResourceManager::LoadPlayerxMonsterPoint()
{
#ifdef _FILE_BIN_
	CMHFile file;
	int idx = 0;
//	if( !file.Init( "Resource/PlayerxMonsterPoint.bin", "rb") )
	if( !file.Init( "Resource/Server/PlayerxMonsterPoint.bin", "rb") )
		return FALSE;
	while(1)
	{
		if(idx > MAX_PLAYERLEVEL_NUM - 1) break;

		char value[64] = {0,};

		// MAX_MONSTERLEVELPOINTRESTRICT_NUM + MONSTERLEVELRESTRICT_LOWSTARTNUM + 1  1은 gap이 0일경우때문에
		for(int levelgap = 0 ; levelgap < MAX_MONSTERLEVELPOINTRESTRICT_NUM + MONSTERLEVELRESTRICT_LOWSTARTNUM + 1 ; levelgap++)
		{
			m_PLAYERxMONSTER_POINT[idx][levelgap] = file.GetInt();
		}
		idx++;
	}
	file.Release();
#else
	FILE * fp = NULL;
	int idx = 0;
	fp = fopen("Resource/PlayerxMonsterPoint.txt", "rt");
	if(!fp) return FALSE;
	while(1)
	{
		if(idx > MAX_PLAYERLEVEL_NUM - 1) break;

		char value[64] = {0,};

		// MAX_MONSTERLEVELPOINTRESTRICT_NUM + MONSTERLEVELRESTRICT_LOWSTARTNUM + 1  1은 gap이 0일경우때문에
		for(int levelgap = 0 ; levelgap < MAX_MONSTERLEVELPOINTRESTRICT_NUM + MONSTERLEVELRESTRICT_LOWSTARTNUM + 1 ; levelgap++)
		{
			fscanf(fp, "%s", value);
			m_PLAYERxMONSTER_POINT[idx][levelgap] = atoi(value);
		}
		idx++;
	}
	fclose(fp);
#endif
	return TRUE;
}

DWORD CGameResourceManager::GetPLAYERxMONSTER_POINT(LEVELTYPE level,int levelgap)
{
	ASSERT(0 < level && level < 255);	// 255 is temp Max Level of Mon
	ASSERT(-MONSTERLEVELRESTRICT_LOWSTARTNUM <= levelgap && 
			levelgap <= MAX_MONSTERLEVELPOINTRESTRICT_NUM );

	return m_PLAYERxMONSTER_POINT[level-1][levelgap+MONSTERLEVELRESTRICT_LOWSTARTNUM];
}
//-----------------------------------------------------------------------------------------------------------//
//		서버만 사용하는 부분
#ifdef _SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
BOOL CGameResourceManager::LoadTaticAbilityInfo()
{
	CMHFile file;
	char filename[64];

#ifdef _FILE_BIN_
//	sprintf(filename,"Resource/tacticabilityinfo.bin");
	sprintf(filename,"Resource/Server/tacticabilityinfo.bin");
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/tacticabilityinfo.txt");
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif
	TATIC_ABILITY_INFO Info;
	int n = 0;
	while(1)
	{
		if( file.IsEOF() )	break;
		n = file.GetInt();
		if( n > MAX_TATIC_ABILITY_NUM )	break;
		Info.wTypeAttack[n-1] = file.GetWord();
		Info.wTypeRecover[n-1] = file.GetWord();
		Info.fTypeBuffRate[n-1] = file.GetFloat();
		Info.wTypeBuff[n-1] = file.GetWord();
	}
	TACTICMGR->AddTaticAbilityInfo(&Info);
	file.Release();
	return TRUE;
}
//-----------------------------------------------------------------------------------------------------------//
#endif //_SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//

BOOL CGameResourceManager::LoadTacticStartInfoList() 
{
	CMHFile file;
	char filename[64];

	TACTICSTART_INFO Info;
#ifdef _FILE_BIN_
	sprintf(filename,"Resource/tacticstartinfo.bin");
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/tacticstartinfo.txt");
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif

	int n =0;
	while(1)
	{
		if(file.IsEOF())
			break;

		Info.TacticID			= file.GetWord();
//		file.GetString(Info.Name);
		SafeStrCpy( Info.Name, file.GetString(), MAX_NAME_LENGTH+1 );
		Info.NeedMemberNum		= file.GetByte();
		Info.WaitingTime		= file.GetWord();
		Info.WaitingNaeryuk		= file.GetWord();
		Info.SuccessNaeryuk		= file.GetWord();
		Info.TacticStartEffect	= FindEffectNum(file.GetString());
		Info.SkillNum = file.GetWord();
		
		for(int i=0; i<6; ++i)
		{
			Info.PosX[i] = file.GetFloat();
			Info.PosZ[i] = file.GetFloat();
		}
		
		TACTICMGR->AddTacticStartInfo(&Info);
		++n;
	}
	file.Release();
	return TRUE;
}

BOOL CGameResourceManager::LoadAvatarEquipList()
{
	CMHFile file;
	char filename[256];
#ifdef _FILE_BIN_
	sprintf(filename,"Resource/AvatarEquip.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/AvatarEquip.txt");
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif

	while(1)
	{
		if(file.IsEOF())
		{
			break;
		}

			int ItemIdx = file.GetWord();
		//if( ItemIdx < 55000 )
			//continue;

		AVATARITEM* pAvatar = new AVATARITEM;
		pAvatar->Gender = file.GetByte();					// 성별
		pAvatar->Position = file.GetByte();					// 착용부위
		pAvatar->Item[eAvatar_Hat] = file.GetWord();		// 모자
		pAvatar->Item[eAvatar_Hair] = file.GetWord();		// 머리
		pAvatar->Item[eAvatar_Face] = file.GetWord();		// 얼굴
		pAvatar->Item[eAvatar_Mask] = file.GetWord();		// 가면
		pAvatar->Item[eAvatar_Glasses] = file.GetWord();	// 안경
		pAvatar->Item[eAvatar_Mustache] = file.GetWord();	// 수염
		pAvatar->Item[eAvatar_Dress] = file.GetWord();		// 옷
		pAvatar->Item[eAvatar_Shoulder] = file.GetWord();	// 어깨
		pAvatar->Item[eAvatar_Back] = file.GetWord();		// 등
		pAvatar->Item[eAvatar_Shoes] = file.GetWord();		// 신발
		pAvatar->Item[eAvatar_Effect] = file.GetWord();		// 이펙트
		pAvatar->Item[eAvatar_Hand] = file.GetWord();		// 손

		pAvatar->Item[eAvatar_Weared_Hair] = file.GetWord();		// 입는 아이템 머리
		pAvatar->Item[eAvatar_Weared_Face] = file.GetWord();		// 입는 아이템 얼굴
		pAvatar->Item[eAvatar_Weared_Hat] = file.GetWord();			// 입는 아이템 머리띠		
		pAvatar->Item[eAvatar_Weared_Dress] = file.GetWord();		// 입는 아이템 옷
		pAvatar->Item[eAvatar_Weared_Shoes] = file.GetWord();		// 입는 아이템 신발		

		pAvatar->Item[eAvatar_Weared_Gum] = file.GetWord();			// 착용무기 검
		pAvatar->Item[eAvatar_Weared_Gwun] = file.GetWord();		// 착용무기 권
		pAvatar->Item[eAvatar_Weared_Do] = file.GetWord();			// 착용무기 도
		pAvatar->Item[eAvatar_Weared_Chang] = file.GetWord();		// 착용무기 창
		pAvatar->Item[eAvatar_Weared_Gung] = file.GetWord();		// 착용무기 궁
		pAvatar->Item[eAvatar_Weared_Amgi] = file.GetWord();		// 착용무기 암기		

		m_AvatarEquipTable.Add( pAvatar, ItemIdx );
	}	

	return TRUE;
}



BOOL CGameResourceManager::LoadShopItemDupList()
{
	CMHFile file;
	char filename[256];
#ifdef _FILE_BIN_
	sprintf(filename,"Resource/ItemdupOption.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/ItemdupOption.txt");
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif

	while(1)
	{
		if(file.IsEOF())
		{
			break;
		}

		char buf[64] = { 0, };
		
		DWORD Index = file.GetDword();
		if( Index == 0 )			break;

		SHOPITEMDUP* pDupOption = new SHOPITEMDUP;		
		pDupOption->Index = Index;
		
		file.GetString( buf );
		if( strcmp( buf, "#CHARM" ) == 0 )
			pDupOption->DupType = eShopItemDupType_Charm;
		else if( strcmp( buf, "#HERB" ) == 0 )
			pDupOption->DupType = eShopItemDupType_Herb;
		else if( strcmp( buf, "#INCANTATION" ) == 0 )
			pDupOption->DupType = eShopItemDupType_Incantation;
		else if( strcmp( buf, "#SUNDRIES" ) == 0 )
			pDupOption->DupType = eShopItemDupType_Sundries;
		else if( strcmp( buf, "#EQUIP" ) == 0 )
			pDupOption->DupType = eShopItemDupType_PetEquip;
		else
		{
			ASSERT(0);
			pDupOption->DupType = eShopItemDupType_None;
		}

		pDupOption->Param = file.GetDword();

		// 주석
		file.GetString( buf );

		m_ShopItemDupOptionTable.Add( pDupOption, pDupOption->Index );
	}

	return TRUE;
}

BOOL CGameResourceManager::LoadUniqueItemOptionList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("./Resource/UniqueItemOptionList.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/UniqueItemOptionList.bin", "rt"))
		return FALSE;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;

		UNIQUE_ITEM_OPTION_INFO* pInfo = new UNIQUE_ITEM_OPTION_INFO;

		pInfo->dwItemIdx = file.GetDword();
		SafeStrCpy(pInfo->strManEffectName, file.GetString(), MAX_CHXNAME_LENGTH+1);
		SafeStrCpy(pInfo->strWomanEffectName, file.GetString(), MAX_CHXNAME_LENGTH+1);
		pInfo->ExchangeFlag = file.GetByte();
		pInfo->MixFlag = file.GetByte();
		pInfo->dwCurseCancellation = file.GetDword();

		pInfo->nHp = file.GetInt();
		pInfo->nMp	 = file.GetInt();
		pInfo->nPhyDamage = file.GetInt();
		pInfo->nCriDamage = file.GetInt();
		pInfo->nCriRate = file.GetInt();
		pInfo->nGengol = file.GetInt();
		pInfo->nMinChub = file.GetInt();
		pInfo->nCheRyuk = file.GetInt();
		pInfo->nSimMek = file.GetInt();
		pInfo->nDefen = file.GetInt();
		pInfo->nRange = file.GetInt();
		pInfo->nAttR = file.GetInt();
		pInfo->nEnemyDefen = file.GetInt();
		pInfo->nShield = file.GetInt();
        
		m_UniqueItemListTable.Add( pInfo, pInfo->dwItemIdx );
	}


	return TRUE;
}

UNIQUE_ITEM_OPTION_INFO* CGameResourceManager::GetUniqueItemOptionList( DWORD dwItemIdx )
{
	UNIQUE_ITEM_OPTION_INFO* pInfo = m_UniqueItemListTable.GetData(dwItemIdx);
	if( !pInfo )
	{
		return NULL;
	}

	return pInfo;
}

BOOL CGameResourceManager::LoadUniqueItemMixList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("./Resource/UniqueItemMixList.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/UniqueItemMixList.bin", "rt"))
		return FALSE;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;

		UNIQUE_ITEM_MIX_INFO* pInfo = new UNIQUE_ITEM_MIX_INFO;
		ZeroMemory(pInfo, sizeof(UNIQUE_ITEM_MIX_INFO));
		pInfo->dwItemIdx = file.GetDword();
		pInfo->wMaterialKindNum = file.GetWord();

        pInfo->sUniqueItemMixMaterial = new UNIQUE_ITEM_MIX_MATERIAL_INFO[pInfo->wMaterialKindNum];

		for( int i = 0; i < pInfo->wMaterialKindNum; i++ )
		{
            pInfo->sUniqueItemMixMaterial[i].dwMaterialIdx = file.GetDword();
			pInfo->sUniqueItemMixMaterial[i].wMaterialNum = file.GetWord();
		}

		for( int i = 0; i < UNIQUE_ITEM_MIX_RESULT_INDEX; i++ )
		{
			pInfo->sUniqueItemMixResult[i].dwResultIdx = file.GetDword();
			pInfo->sUniqueItemMixResult[i].wResultRate = file.GetWord();
		}

		m_UniqueItemMixTable.Add( pInfo, pInfo->dwItemIdx );
	}


	return TRUE;
}

UNIQUE_ITEM_MIX_INFO* CGameResourceManager::GetUniqueItemMixList( DWORD dwItemIdx )
{
	UNIQUE_ITEM_MIX_INFO* pInfo = m_UniqueItemMixTable.GetData(dwItemIdx);
	if( !pInfo )
	{
		return NULL;
	}

	return pInfo;
}

void CGameResourceManager::LoadMapKindInfo()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/MapKindInfo.bin", "rb"))
		return;
#else
	if(!file.Init("Resource/MapKindInfo.txt", "rt"))
		return;
#endif

	stMAPKINDINFO* pInfo = NULL;
	char strTemp[128] = { 0, };

	while(1)
	{
		if(file.IsEOF())
			break;

		DWORD dwMapNumber = file.GetDword();
		if( 0 == dwMapNumber )
			break;

		pInfo = new stMAPKINDINFO;
		ZeroMemory(pInfo, sizeof(stMAPKINDINFO));

		pInfo->dwMapNum				= dwMapNumber;
		SafeStrCpy(pInfo->strMapName, file.GetString(), MAX_NAME_LEN+1);
		//pInfo->dwMapStateBItFlag |= file.GetDword();

		while(1)
		{
			SafeStrCpy(strTemp, file.GetString(), 128);

			if( strcmp(strTemp, "|") == 0 )
				continue;
			else if( strcmp(strTemp, "#") == 0 )
				break;
			else if( strcmp(strTemp, "MAPVIEW") == 0 )
				pInfo->dwMapStateBItFlag |= eMapView;
			else if( strcmp(strTemp, "EVENT") == 0 )
				pInfo->dwMapStateBItFlag |= eEventMap;
			else if( strcmp(strTemp, "RUNNING") == 0 )
				pInfo->dwMapStateBItFlag |= eRunningMap;
			else if( strcmp(strTemp, "SIEGEWAR") == 0 )
				pInfo->dwMapStateBItFlag |= eSiegeWarMap;
			else if( strcmp(strTemp, "BOSS") == 0 )
				pInfo->dwMapStateBItFlag |= eBossMap;
			else if( strcmp(strTemp, "TITAN") == 0 )
				pInfo->dwMapStateBItFlag |= eTitanMap;
			else if( strcmp(strTemp, "QUEST") == 0 )
				pInfo->dwMapStateBItFlag |= eQuestRoom;
			else if( strcmp(strTemp, "TOURNAMENT") == 0 )
				pInfo->dwMapStateBItFlag |= eTournament;
			else if( strcmp(strTemp, "SURVIVAL") == 0 )
				pInfo->dwMapStateBItFlag |= eSurvivalMap;
		}

		m_MapKindStateInfoTable.Add(pInfo, pInfo->dwMapNum);
	}
	
	file.Release();
}

stMAPKINDINFO* CGameResourceManager::GetMapKindInfo(DWORD dwMapNum)
{
	stMAPKINDINFO* pInfo = m_MapKindStateInfoTable.GetData(dwMapNum);
	if( !pInfo )
	{
		//ASSERT(0);
		return NULL;
	}

	return pInfo;
}

//2007. 12. 4. CBH - 의복 스킨 아이탬 관련 함수
BOOL CGameResourceManager::LoadNomalClothesSkinList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	file.Init("Resource/SkinSelectItemList.bin","rb");
#else
	file.Init("Resource/SkinSelectItemList.txt","rt");
#endif
	if(file.IsInited() == FALSE)
	{
		ASSERT(0);
		return FALSE;
	}

	while(1)
	{
		if(file.IsEOF() == TRUE)
			break;

		SKIN_SELECT_ITEM_INFO* pSkinSelectItem = new SKIN_SELECT_ITEM_INFO;
		memset(pSkinSelectItem, 0, sizeof(SKIN_SELECT_ITEM_INFO));

		pSkinSelectItem->dwIndex = file.GetDword();
		memset(pSkinSelectItem->szSkinName, 0, sizeof(char)*(MAX_ITEMNAME_LENGTH+1));		
		SafeStrCpy( pSkinSelectItem->szSkinName, file.GetString(), MAX_ITEMNAME_LENGTH+1 );
		pSkinSelectItem->dwLimitLevel = file.GetDword();
		for(int i = 0 ; i < SKINITEM_LIST_MAX ; i++)
		{
			pSkinSelectItem->wEquipItem[i] = file.GetWord();
		}		

		m_NomalClothesSkinTable.Add(pSkinSelectItem, pSkinSelectItem->dwIndex);		
	}

	return TRUE;
}

SKIN_SELECT_ITEM_INFO* CGameResourceManager::GetNomalClothesSkinList( DWORD dwKeyIndex )
{
	/*
	SKIN_SELECT_ITEM_INFO* pSkinItem = NULL;
	DWORD dwMaxNum = m_NomalClothesSkinTable.GetDataNum();
	
	m_NomalClothesSkinTable.SetPositionHead();
	for(DWORD i = 0 ; i < dwMaxNum ; i++)
	{
		pSkinItem = m_NomalClothesSkinTable.GetData();

		if(pSkinItem == NULL)
			continue;

		if(pSkinItem->dwIndex == dwKeyIndex)
			return pSkinItem;

		pSkinItem = NULL;
	}
	*/

	SKIN_SELECT_ITEM_INFO* pSkinItem = NULL;
	
	m_NomalClothesSkinTable.SetPositionHead();
	while(pSkinItem = m_NomalClothesSkinTable.GetData())
	{
		if(pSkinItem->dwIndex != 0)
		{
			if(pSkinItem->dwIndex == dwKeyIndex)
				return pSkinItem;
		}		

		pSkinItem = NULL;
	}	

	return pSkinItem;
}

DWORD CGameResourceManager::GetNomalClothesSkinListCountNum()
{
	return m_NomalClothesSkinTable.GetDataNum();
}

void CGameResourceManager::RemoveNomalClothesSkinList()
{
	SKIN_SELECT_ITEM_INFO* pSkinSelectItem = NULL;

	m_NomalClothesSkinTable.SetPositionHead();
	while(pSkinSelectItem = m_NomalClothesSkinTable.GetData())
	{
		SAFE_DELETE(pSkinSelectItem);
	}
	m_NomalClothesSkinTable.RemoveAll();
}

CYHHashTable<SKIN_SELECT_ITEM_INFO>* CGameResourceManager::GetNomalClothesSkinTable()
{
	return &m_NomalClothesSkinTable;
}

//2008. 1. 15. CBH - 코스튬 스킨 아이탬 관련 함수
BOOL CGameResourceManager::LoadCostumeSkinList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	file.Init("Resource/CostumeSkinItemList.bin","rb");
#else
	file.Init("Resource/CostumeSkinItemList.txt","rt");
#endif
	if(file.IsInited() == FALSE)
	{
		ASSERT(0);
		return FALSE;
	}

	while(1)
	{
		if(file.IsEOF() == TRUE)
			break;

		SKIN_SELECT_ITEM_INFO* pSkinSelectItem = new SKIN_SELECT_ITEM_INFO;
		memset(pSkinSelectItem, 0, sizeof(SKIN_SELECT_ITEM_INFO));

		pSkinSelectItem->dwIndex = file.GetDword();
		memset(pSkinSelectItem->szSkinName, 0, sizeof(char)*(MAX_ITEMNAME_LENGTH+1));		
		SafeStrCpy( pSkinSelectItem->szSkinName, file.GetString(), MAX_ITEMNAME_LENGTH+1 );
		pSkinSelectItem->dwLimitLevel = file.GetDword();
		pSkinSelectItem->wEquipItem[0] = file.GetWord();

		m_CotumeSkinTable.Add(pSkinSelectItem, pSkinSelectItem->dwIndex);		
	}

	return TRUE;
}

SKIN_SELECT_ITEM_INFO* CGameResourceManager::GetCostumeSkinList( DWORD dwKeyIndex )
{
	/*
	SKIN_SELECT_ITEM_INFO* pSkinItem = NULL;
	DWORD dwMaxNum = m_CotumeSkinTable.GetDataNum();

	m_CotumeSkinTable.SetPositionHead();
	for(DWORD i = 0 ; i < dwMaxNum ; i++)
	{
		pSkinItem = m_CotumeSkinTable.GetData();

		if(pSkinItem == NULL)
			continue;

		if(pSkinItem->dwIndex == dwKeyIndex)
			return pSkinItem;

		pSkinItem = NULL;
	}
	*/

	SKIN_SELECT_ITEM_INFO* pSkinItem = NULL;	

	m_CotumeSkinTable.SetPositionHead();
	while(pSkinItem = m_CotumeSkinTable.GetData())
	{
		if(pSkinItem->dwIndex != 0)
		{
			if(pSkinItem->dwIndex == dwKeyIndex)
				return pSkinItem;
		}		
	}

	return pSkinItem;
}

DWORD CGameResourceManager::GetCostumeSkinListCountNum()
{
	return m_CotumeSkinTable.GetDataNum();
}

void CGameResourceManager::RemoveCostumeSkinList()
{
	SKIN_SELECT_ITEM_INFO* pSkinSelectItem = NULL;

	m_CotumeSkinTable.SetPositionHead();
	while(pSkinSelectItem = m_CotumeSkinTable.GetData())
	{
		SAFE_DELETE(pSkinSelectItem);
	}
	m_CotumeSkinTable.RemoveAll();
}

CYHHashTable<SKIN_SELECT_ITEM_INFO>* CGameResourceManager::GetCostumeSkinTable()
{
	return &m_CotumeSkinTable;
}
//////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------------------------------------//
//		클라이언트만 사용하는 부분
#ifdef _CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
void CGameResourceManager::DeletePreLoadData()
{
	//
	sPRELOAD_INFO* pPreLoadInfo = NULL;
	m_PreLoadDataTable.SetPositionHead();
	while( pPreLoadInfo = m_PreLoadDataTable.GetData() )
	{
		delete pPreLoadInfo;		
	}
	m_PreLoadDataTable.RemoveAll();	

	//
	PRELOAD* pEff = NULL;
	PTRLISTSEARCHSTART(m_PreLoadEfflist, PRELOAD*, pEff);
//		m_PreLoadEfflist.Remove(pEff);
		delete pEff;
		pEff = NULL;
	PTRLISTSEARCHEND
	m_PreLoadEfflist.RemoveAll();

	//
	PRELOAD* pMod = NULL;
	PTRLISTSEARCHSTART(m_PreLoadModlist, PRELOAD*, pMod);
//		m_PreLoadModlist.Remove(pMod);
		delete pMod;
		pMod = NULL;
	PTRLISTSEARCHEND
	m_PreLoadModlist.RemoveAll();

	if(m_PreLoadItemlist)
        delete[] m_PreLoadItemlist;

	sPRELOAD_EFFECT_INFO* pPreLoadEffectInfo = NULL;
	m_PreLoadEffectTable.SetPositionHead();
	while( pPreLoadEffectInfo = m_PreLoadEffectTable.GetData() )
	{
		PRELOAD* pEffect = NULL;
		PTRLISTSEARCHSTART(pPreLoadEffectInfo->Effect, PRELOAD*, pEffect);
		delete pEffect;
		pEffect = NULL;
		PTRLISTSEARCHEND
		pPreLoadEffectInfo->Effect.RemoveAll();
		delete pPreLoadEffectInfo;		
	}
	m_PreLoadEffectTable.RemoveAll();	

}


#include ".\Engine\EngineObject.h"		// for PreLoadObject()
void CGameResourceManager::PreLoadData()
{
//	DWORD n;
	// YH2DO 로딩에 대한 확정 필요

	/* 케릭터는 너무 많아서 막아놓음
	//////////////////////////////////////////////////////////////////////////
	// 케릭터 Preload
	DIRECTORYMGR->SetLoadMode(eLM_Character);

	for(int job=0;job<eJOB_Max;++job)
	{
		CEngineObject::PreLoadObject(m_ModFileList[GENDER_MALE][job].BaseObjectFile);
		CEngineObject::PreLoadObject(m_ModFileList[GENDER_FEMALE][job].BaseObjectFile);
		
		for(n=0;n<m_ModFileList[GENDER_MALE][job].MaxModFile;++n)
			CEngineObject::PreLoadObject(m_ModFileList[GENDER_MALE][job].ModFile[n]);

		for(n=0;n<m_ModFileList[GENDER_FEMALE][job].MaxModFile;++n)
			CEngineObject::PreLoadObject(m_ModFileList[GENDER_FEMALE][job].ModFile[n]);
	}

	for(n=0;n<m_HairModList[GENDER_MALE].MaxModFile;++n)
		CEngineObject::PreLoadObject(m_HairModList[GENDER_MALE].ModFile[n]);
	
	for(n=0;n<m_HairModList[GENDER_FEMALE].MaxModFile;++n)
		CEngineObject::PreLoadObject(m_HairModList[GENDER_FEMALE].ModFile[n]);
	//////////////////////////////////////////////////////////////////////////
	*/
	DIRECTORYMGR->SetLoadMode(eLM_Character);
	CEngineObject::PreLoadObject("Man.chx");
	CEngineObject::PreLoadObject("Woman.chx");
//	DIRECTORYMGR->SetLoadMode(eLM_Titan);
//	CEngineObject::PreLoadObject("Titan_a1.chx");

	/*
	//////////////////////////////////////////////////////////////////////////
	// Npc
	DIRECTORYMGR->SetLoadMode(eLM_Npc);

	for(n=0;n<m_MaxNpcChxList;++n)
		CEngineObject::PreLoadObject(m_NpcChxList[n].FileName);
	//////////////////////////////////////////////////////////////////////////
	*/
	
	DIRECTORYMGR->SetLoadMode(eLM_Root);
}

BOOL CGameResourceManager::LoadModFileList(MOD_LIST pModList[GENDER_MAX])
{
	CMHFile file;
	char filename[64];
	DWORD idx = 0;
	
	//////////////////////////////////////////////////////////////////////////
	// 남자
	idx = 0;
#ifdef _FILE_BIN_
//	sprintf(filename,"Resource/ModList_M.bin");
	sprintf(filename,"Resource/Client/ModList_M.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/ModList_M.txt");
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif
	
	// BaseObjectFile
	pModList[GENDER_MALE].BaseObjectFile = file.GetString();	
	
	pModList[GENDER_MALE].MaxModFile = file.GetDword();
	pModList[GENDER_MALE].ModFile = new StaticString[pModList[GENDER_MALE].MaxModFile];
	while(1)
	{
		if(idx >= pModList[GENDER_MALE].MaxModFile) break;
		pModList[GENDER_MALE].ModFile[idx] = file.GetString();
		
		++idx;
	}		
	file.Release();
	
	
	//////////////////////////////////////////////////////////////////////////
	// 여자
	idx = 0;
#ifdef _FILE_BIN_
//	sprintf(filename,"Resource/ModList_W.bin");
	sprintf(filename,"Resource/Client/ModList_W.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/ModList_W.txt");
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif
	
	// BaseObjectFile
	pModList[GENDER_FEMALE].BaseObjectFile = file.GetString();
	
	pModList[GENDER_FEMALE].MaxModFile = file.GetDword();
	pModList[GENDER_FEMALE].ModFile = new StaticString[pModList[GENDER_FEMALE].MaxModFile];
	while(1)
	{
		if(idx >= pModList[GENDER_FEMALE].MaxModFile) break;
		pModList[GENDER_FEMALE].ModFile[idx] = file.GetString();
				
		++idx;
	}		
	file.Release();

	return TRUE;
}
BOOL CGameResourceManager::LoadFaceModFileList(MOD_LIST pFaceModList[GENDER_MAX]) //pjscode
{
	//얼굴 관련
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

		//////////////////////////////////////////////////////////////////////////
		// 남자
		idx = 0;
#ifdef _FILE_BIN_
//		sprintf(filename,"Resource/FaceList_M.bin");		
		sprintf(filename,"Resource/Client/FaceList_M.bin");
		if(!file.Init(filename,"rb"))
			return FALSE;
#else
		sprintf(filename,"Resource/FaceList_M.txt");	
		if(!file.Init(filename,"r"))
			return FALSE;
#endif
		
		pFaceModList[GENDER_MALE].MaxModFile = file.GetDword();
		pFaceModList[GENDER_MALE].ModFile = new StaticString[pFaceModList[GENDER_MALE].MaxModFile];

		while(1)
		{
			if(idx >= pFaceModList[GENDER_MALE].MaxModFile) break;
			pFaceModList[GENDER_MALE].ModFile[idx] = file.GetString();

			++idx;
		}		
		file.Release();

		
		//////////////////////////////////////////////////////////////////////////
		// 여자
		idx = 0;
#ifdef _FILE_BIN_
//		sprintf(filename,"Resource/FaceList_W.bin");
		sprintf(filename,"Resource/Client/FaceList_W.bin");
		if(!file.Init(filename,"rb"))
			return FALSE;
#else
		sprintf(filename,"Resource/FaceList_W.txt");
		if(!file.Init(filename,"rt"))
			return FALSE;
#endif
		
		pFaceModList[GENDER_FEMALE].MaxModFile = file.GetDword();
		pFaceModList[GENDER_FEMALE].ModFile = new StaticString[pFaceModList[GENDER_FEMALE].MaxModFile];

		
		while(1)
		{
			if(idx >= pFaceModList[GENDER_FEMALE].MaxModFile) break;
			pFaceModList[GENDER_FEMALE].ModFile[idx] = file.GetString();

			++idx;
		}		
	file.Release();
	
	return TRUE;
}

BOOL CGameResourceManager::LoadBodyModFileList(MOD_LIST pBodyModList[GENDER_MAX])
{
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

		//////////////////////////////////////////////////////////////////////////
		// 남자
		idx = 0;
#ifdef _FILE_BIN_
		sprintf(filename,"Resource/BodyList_M.bin");		
		if(!file.Init(filename,"rb"))
			return FALSE;
#else
		sprintf(filename,"Resource/BodyList_M.txt");		
		if(!file.Init(filename,"rt"))
			return FALSE;
#endif
		pBodyModList[GENDER_MALE].MaxModFile = file.GetDword();
		pBodyModList[GENDER_MALE].ModFile = new StaticString[pBodyModList[GENDER_MALE].MaxModFile];

		while(1)
		{
			if(idx >= pBodyModList[GENDER_MALE].MaxModFile) break;
			pBodyModList[GENDER_MALE].ModFile[idx] = file.GetString();

			++idx;
		}		
		file.Release();

		
		//////////////////////////////////////////////////////////////////////////
		// 여자
		idx = 0;
#ifdef _FILE_BIN_
		sprintf(filename,"Resource/BodyList_W.bin");
		if(!file.Init(filename,"rb"))
			return FALSE;
#else
		sprintf(filename,"Resource/BodyList_W.txt");
		if(!file.Init(filename,"rt"))
			return FALSE;
#endif		
		
		pBodyModList[GENDER_FEMALE].MaxModFile = file.GetDword();
		pBodyModList[GENDER_FEMALE].ModFile = new StaticString[pBodyModList[GENDER_FEMALE].MaxModFile];

		
		while(1)
		{
			if(idx >= pBodyModList[GENDER_FEMALE].MaxModFile) break;
			pBodyModList[GENDER_FEMALE].ModFile[idx] = file.GetString();

			++idx;
		}		
	file.Release();
	
	return TRUE;
}


BOOL CGameResourceManager::LoadHairModFileList(MOD_LIST pHairModList[GENDER_MAX])
{
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

		//////////////////////////////////////////////////////////////////////////
		// 남자
		idx = 0;
#ifdef _FILE_BIN_
//		sprintf(filename,"Resource/HairList_M.bin");	
		sprintf(filename,"Resource/Client/HairList_M.bin");
		if(!file.Init(filename,"rb"))
			return FALSE;
#else
		sprintf(filename,"Resource/HairList_M.txt");		
		if(!file.Init(filename,"rt"))
			return FALSE;
#endif		
		pHairModList[GENDER_MALE].MaxModFile = file.GetDword();
		pHairModList[GENDER_MALE].ModFile = new StaticString[pHairModList[GENDER_MALE].MaxModFile];

		while(1)
		{
			if(idx >= pHairModList[GENDER_MALE].MaxModFile) break;
			pHairModList[GENDER_MALE].ModFile[idx] = file.GetString();

			++idx;
		}		
		file.Release();

		
		//////////////////////////////////////////////////////////////////////////
		// 여자
		idx = 0;
#ifdef _FILE_BIN_
//		sprintf(filename,"Resource/HairList_W.bin");
		sprintf(filename,"Resource/Client/HairList_W.bin");
		if(!file.Init(filename,"rb"))
			return FALSE;
#else
		sprintf(filename,"Resource/HairList_W.txt");
		if(!file.Init(filename,"rt"))
			return FALSE;
#endif
		
		pHairModList[GENDER_FEMALE].MaxModFile = file.GetDword();
		pHairModList[GENDER_FEMALE].ModFile = new StaticString[pHairModList[GENDER_FEMALE].MaxModFile];

		
		while(1)
		{
			if(idx >= pHairModList[GENDER_FEMALE].MaxModFile) break;
			pHairModList[GENDER_FEMALE].ModFile[idx] = file.GetString();

			++idx;
		}		
	file.Release();
	
	return TRUE;
}


// Titan
BOOL CGameResourceManager::LoadTitanModFileList(MOD_LIST* pModList)
{
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

#ifdef _FILE_BIN_
	sprintf(filename,"Resource/Client/ModList_T.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/Client/ModList_T.txt");
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif
	idx = 0;

	// BaseObjectFile
	pModList->BaseObjectFile = file.GetString();	

	pModList->MaxModFile = file.GetDword();
	pModList->ModFile = new StaticString[pModList->MaxModFile];
	while(1)
	{
		if(idx >= pModList->MaxModFile) break;
		pModList->ModFile[idx] = file.GetString();

		++idx;
	}		
	file.Release();

	return TRUE;
}


BOOL CGameResourceManager::LoadTitanAttachList(MOD_LIST pAttachModList[TITANKIND_MAX])
{
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

	for(int i=0; i<TITANKIND_MAX; ++i)
	{
#ifdef _FILE_BIN_
		sprintf(filename,"Resource/TitanAttach_%02d.bin", i);
		if(!file.Init(filename,"rb"))
			return FALSE;
#else
		sprintf(filename,"Resource/TitanAttach_%02d.txt", i);
		if(!file.Init(filename,"rt"))
			return FALSE;
#endif
		idx = 0;

		// BaseObjectFile
		pAttachModList[i].BaseObjectFile = file.GetString();	

		pAttachModList[i].MaxModFile = file.GetDword();
		pAttachModList[i].ModFile = new StaticString[pAttachModList[i].MaxModFile];
		while(1)
		{
			if(idx >= pAttachModList[i].MaxModFile) break;
			pAttachModList[i].ModFile[idx] = file.GetString();

			++idx;
		}		
		file.Release();
	}

	return TRUE;
}


/*
BOOL CGameResourceManager::LoadTitanHairModList(MOD_LIST pHairModeList[TITANKIND_MAX])
{
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

	for(int i=0; i<TITANKIND_MAX; ++i)
	{
#ifdef _FILE_BIN_
		sprintf(filename,"Resource/TitanHairList_%02d.bin", i);
		if(!file.Init(filename,"rb"))
			return FALSE;
#else
		sprintf(filename,"Resource/TitanHairList_%02d.txt", i);
		if(!file.Init(filename,"rt"))
			return FALSE;
#endif
		
		idx = 0;
		pHairModeList[i].MaxModFile = file.GetDword();
		pHairModeList[i].ModFile = new StaticString[pHairModeList[i].MaxModFile];

		while(1)
		{
			if(idx >= pHairModeList[i].MaxModFile) break;
			pHairModeList[i].ModFile[idx] = file.GetString();

			++idx;
		}		
		file.Release();
	}


	return TRUE;
}



BOOL CGameResourceManager::LoadTitanFaceModList(MOD_LIST pFaceModList[TITANKIND_MAX])
{
	//얼굴 관련
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

	for(int i=0; i<TITANKIND_MAX; ++i)
	{	
	#ifdef _FILE_BIN_
		sprintf(filename,"Resource/TitanFaceList_%02d.bin", i);		
		if(!file.Init(filename,"rb"))
			return FALSE;
	#else
		sprintf(filename,"Resource/TitanFaceList_%02d.txt");	
		if(!file.Init(filename,"r"))
			return FALSE;
	#endif

		pFaceModList[i].MaxModFile = file.GetDword();
		pFaceModList[i].ModFile = new StaticString[pFaceModList[i].MaxModFile];

		idx = 0;
		while(1)
		{
			if(idx >= pFaceModList[i].MaxModFile) break;
			pFaceModList[i].ModFile[idx] = file.GetString();

			++idx;
		}		
		file.Release();
	}

	return TRUE;
}
*/




BOOL CGameResourceManager::LoadNpcChxList()
{
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

	idx = 0;
#ifdef _FILE_BIN_
//	sprintf(filename,"Resource/NpcChxList.bin");
	sprintf(filename,"Resource/Client/NpcChxList.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/NpcChxList.txt");
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif
	
	m_MaxNpcChxList = file.GetDword();
	m_NpcChxList = new CHXLIST[m_MaxNpcChxList];

	while(1)
	{
		if(idx >= m_MaxNpcChxList) break;
		m_NpcChxList[idx].FileName = file.GetString();
		
		++idx;
	}		
	file.Release();
	
	return TRUE;
}

char* CGameResourceManager::GetNpcChxName(DWORD index)
{
	ASSERT(index > 0 && index < m_MaxNpcChxList);
	return m_NpcChxList[index].FileName;
}

BOOL CGameResourceManager::LoadItemChxList()
{
	CMHFile file;
	char filename[64];
	DWORD idx = 0;

	idx = 0;
#ifdef _FILE_BIN_
	sprintf(filename,"Resource/Client/ItemChxList.bin");
	if(!file.Init(filename,"rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/ItemChxList.txt");
	if(!file.Init(filename,"rt"))
		return FALSE;
#endif

	m_MaxItemChxCount = file.GetDword();
	m_ItemChxList = new CHXLIST[m_MaxItemChxCount];

	while(1)
	{
		if(idx >= m_MaxItemChxCount) break;
		m_ItemChxList[idx].FileName = file.GetString();

		++idx;
	}		
	file.Release();

	return TRUE;
}

char* CGameResourceManager::GetItemChxName(DWORD index)
{
	ASSERT(index < m_MaxItemChxCount);
	return m_ItemChxList[index].FileName;
}

BOOL CGameResourceManager::LoadGameDesc()
{
	char filename[64];
	char value[64] = {0,};
	CMHFile file;
#ifdef _FILE_BIN_
	sprintf(filename,"INI/GameDesc.bin");	
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"INI/GameDesc.ini");	
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;

		strcpy(value,strupr(file.GetString()));

		if(strcmp(value,"*DISPWIDTH") == 0)
		{
			m_GameDesc.dispInfo.dwWidth = file.GetDword();
		}
		else if(strcmp(value,"*DISPHEIGHT") == 0)
		{
			m_GameDesc.dispInfo.dwHeight = file.GetDword();
		}
		else if(strcmp(value,"*BPS") == 0)
		{
			m_GameDesc.dispInfo.dwBPS = file.GetDword();
		}
		else if(strcmp(value,"*WINDOWMODE") == 0)
		{
			if( file.GetBool() )
				m_GameDesc.dispInfo.dispType = WINDOW_WITH_BLT;
			else
				m_GameDesc.dispInfo.dispType = /*FULLSCREEN_WITH_FLIP;//*/FULLSCREEN_WITH_BLT;	//FLIP은 Alt_Tab이 안된다.

//			m_GameDesc.dispInfo.bWindowed = file.GetBool();

		}
		else if(strcmp(value,"*SHADOW") == 0)
		{
			m_GameDesc.bShadow = file.GetByte();
		}
		else if(strcmp(value,"*MAXSHADOWLIGHT") == 0)
		{
			m_GameDesc.MaxShadowNum = file.GetDword();
		}
		else if(strcmp(value,"*SHADOWDETAIL") == 0)
		{
			m_GameDesc.MaxShadowTexDetail = file.GetDword();
		}
		else if(strcmp(value,"*DISTRIBUTESERVERIP") == 0)
		{
			file.GetString(m_GameDesc.DistributeServerIP);
		}
		else if(strcmp(value,"*DISTRIBUTESERVERPORT") == 0)
		{
			m_GameDesc.DistributeServerPort = file.GetWord();
		}
		else if(strcmp(value,"*AGENTSERVERPORT") == 0)
		{
			m_GameDesc.AgentServerPort = file.GetWord();
		}
		
		else if(strcmp(value,"*MOVEPOINT") == 0)
		{
			file.GetString(m_GameDesc.MovePoint);
		}
		else if(strcmp(value,"*FPS") == 0)
		{
			m_GameDesc.FramePerSec = file.GetDword();
			m_GameDesc.TickPerFrame = 1000/(float)m_GameDesc.FramePerSec;
		}
		else if(strcmp(value,"*CAMERAANGLEX") == 0)
		{
			m_GameDesc.CameraMinAngleX = file.GetFloat();
			m_GameDesc.CameraMaxAngleX = file.GetFloat();
		}
		else if(strcmp(value,"*CAMERADISTANCE") == 0)
		{
			m_GameDesc.CameraMinDistance = file.GetFloat();
			m_GameDesc.CameraMaxDistance = file.GetFloat();
		}
		else if(strcmp(value,"*GRAVITY") == 0)
		{
			gAntiGravity = 1/file.GetFloat();
		}		

		// Volume
		else if(strcmp(value,"*MASTERVOLUME") == 0)
		{
			m_GameDesc.MasterVolume = file.GetFloat();
		}
		else if(strcmp(value,"*SOUNDVOLUME") == 0)
		{
			m_GameDesc.SoundVolume = file.GetFloat();
		}
		else if(strcmp(value,"*BGMVOLUME") == 0)
		{
			m_GameDesc.BGMVolume = file.GetFloat();
		}
		//limit
		else if(strcmp(value,"*LIMITDAY") == 0)
		{
			m_GameDesc.LimitDay = file.GetDword();
		}
		else if(strcmp(value,"*LIMITID") == 0)
		{
			file.GetString(m_GameDesc.LimitID);
		}
		else if(strcmp(value,"*LIMITPWD") == 0)
		{
			file.GetString(m_GameDesc.LimitPWD);
		}
		else if(strcmp(value,"*WINDOWTITLE") == 0)
		{
			file.GetString(m_GameDesc.strWindowTitle);
		}
		
	}
	
	file.Release();

	
	gTickPerFrame = m_GameDesc.TickPerFrame;
	AUDIOMGR->SetMasterVolume(m_GameDesc.MasterVolume);
	AUDIOMGR->SetSoundVolume(m_GameDesc.SoundVolume);
	AUDIOMGR->SetBGMVolume(m_GameDesc.BGMVolume);

	return TRUE;
}

BOOL CGameResourceManager::LoadServerList()
{
	m_nMaxServerList = 0;

	char filename[64];
	char value[64] = {0,};
	int index = 0;
	CMHFile file;

#ifdef _FILE_BIN_
	sprintf( filename, "INI/ServerList.bin" );
	if( !file.Init( filename, "rb" ) )
		return FALSE;
#else
	sprintf( filename, "INI/ServerList.ini" );	
	if( !file.Init( filename, "rt" ) )
		return FALSE;
#endif

	while( 1 )
	{
		if( file.IsEOF() )
			break;

		strcpy( value, strupr(file.GetString()) );

		if( strcmp( value,"*USERSERVER" ) == 0 )
		{
			++index;
			if( index > 0 )
			{
				m_ServerList[index-1].bEnter = file.GetBool();
				file.GetString( m_ServerList[index-1].DistributeIP );
				m_ServerList[index-1].DistributePort = file.GetWord();
				file.GetStringInQuotation( m_ServerList[index-1].ServerName );
				m_ServerList[index-1].ServerNo = file.GetWord();
				++m_nMaxServerList;
			}
		}

		else if( strcmp( value,"*TESTSERVER" ) == 0 && g_bTESTSERVER)
		{
			++index;
			if( index > 0 )
			{
				m_ServerList[index-1].bEnter = file.GetBool();
				file.GetString( m_ServerList[index-1].DistributeIP );
				m_ServerList[index-1].DistributePort = file.GetWord();
				file.GetStringInQuotation( m_ServerList[index-1].ServerName );
				m_ServerList[index-1].ServerNo = file.GetWord();
				++m_nMaxServerList;
			}
		}
	}
	
	file.Release();

	return TRUE;
}


char* CGameResourceManager::GetServerSetName()
{
	for(int i=0; i<m_nMaxServerList; ++i)
	{
        if( g_nServerSetNum == m_ServerList[i].ServerNo )
			return m_ServerList[i].ServerName;
	}
	return NULL;
}



#include "ItemManager.h"			// for LoadPreDataTable()
void CGameResourceManager::LoadPreDataTable()
{
	CMHFile file;
#ifdef _FILE_BIN_
//	if(!file.Init("./Resource/PreLoadData.bin", "rb"))
	if(!file.Init("./Resource/Client/PreLoadData.bin", "rb"))
		return;
#else
	if(!file.Init("./Resource/PreLoadData.txt", "rt"))
		return;
#endif

	char Token[64];	

	while( !file.IsEOF() )
	{
		file.GetString(Token);

		if(strcmp(Token, "#EFFECT") == 0)
		{
			PRELOAD* pEff = NULL;
			pEff = new PRELOAD;

			char buf[128];

			file.GetString(buf);
			pEff->FileName = buf;
			
			m_PreLoadEfflist.AddTail(pEff);
		}
		else if(strcmp(Token, "#MOD") == 0)
		{
			PRELOAD* pEff = NULL;
			pEff = new PRELOAD;

			char buf[128];

			file.GetString(buf);
			pEff->FileName = buf;
			
			m_PreLoadModlist.AddTail(pEff);
		}
		else if(strcmp(Token, "#MAPNUM") == 0)
		{
			sPRELOAD_INFO* pPreLoadInfo = new sPRELOAD_INFO;
			pPreLoadInfo->MapNum = file.GetInt();

			// 해당맵의 몬스터 정보 로딩
//			LoadPreMonsterData(pPreLoadInfo, pPreLoadInfo->MapNum);
			
			int Count = file.GetInt();
			int* Level = NULL;

			ITEM_INFO* pItemInfo = NULL;

			/*
			for(int i=0; i<Count; i++)
			{
				Level = file.GetInt();
				// 해당 레벨 아이템 로딩
				ITEMMGR->SetPreItemData(pPreLoadInfo, Level);
			}
			*/


			// Item종류가 워낙 많아서.. 차라리 이게 낫지..
			Level = new int[Count];

			for(int i=0; i<Count; i++)
				Level[i] = file.GetInt();

			ITEMMGR->SetPreItemData(pPreLoadInfo, Level, Count);

			delete Level;

			m_PreLoadDataTable.Add(pPreLoadInfo, pPreLoadInfo->MapNum);
		}
		///////////////////////////////////////////////////////////////////////////////
		// 06. 04. PreLoadData 추가기능 - 이영준
		// 아이템 사전 로드기능 추가
		// #ITEM [수량] [아이템 인덱스]...
		else if(strcmp(Token, "#ITEM") == 0)
		{
			// 있으면 지워준다
			if(m_PreLoadItemlist)
				delete[] m_PreLoadItemlist;

			// 수량을 가져온다
			int Count = file.GetInt();
			
			if(Count > 0)
			{
				// 버퍼 생성
				m_PreLoadItemlist = new WORD[Count + 1];
				// 첫번째 자리에 수량을 넣어준다
				m_PreLoadItemlist[0] = Count;

				WORD ItemIdx = 0;

				for(int i = 1; i < Count + 1; i++)
				{		
					ItemIdx = file.GetWord();
					m_PreLoadItemlist[i] = ItemIdx;
				}
			}
		}
		///////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////
		// 06. 05. PreLoadData 추가기능 - 이영준
		// 맵별 이펙트 사전 로드기능 추가
		// #MAPEFFECT [수량] [파일이름]...
		else if(strcmp(Token, "#MAPEFFECT") == 0)
		{
			sPRELOAD_EFFECT_INFO* pPreLoadInfo = new sPRELOAD_EFFECT_INFO;

			pPreLoadInfo->MapNum = file.GetInt();
			pPreLoadInfo->Count = file.GetInt();
			
			for(int i = 0; i < pPreLoadInfo->Count; i++)
			{
				PRELOAD* pEff = NULL;
				pEff = new PRELOAD;

				char buf[128];

				file.GetString(buf);
				pEff->FileName = buf;

				pPreLoadInfo->Effect.AddTail(pEff);
			}

			m_PreLoadEffectTable.Add(pPreLoadInfo, pPreLoadInfo->MapNum);
		}
		///////////////////////////////////////////////////////////////////////////////

	}
}


void CGameResourceManager::LoadPreMonsterData(sPRELOAD_INFO* pPreLoadInfo, MAPTYPE MapNum)
{
	CMHFile file;
	char filename[256];
#ifdef _FILE_BIN_
	sprintf(filename,"Resource/Monster_%02d.bin",MapNum);
	if(!file.Init(filename,"rb"))
		return ;
#else
	sprintf(filename,"Resource/Monster_%02d.txt",MapNum);
	if(!file.Init(filename,"rt"))
		return ;
#endif

	char buff[256];
	while(1)
	{
		if(file.IsEOF())
		{
			break;
		}
		file.GetString(buff);
		strcpy(buff, _strupr(buff));

		CMD_ST(buff)			
		CMD_CS("#ADD")
			file.GetByte();
			file.GetDword();
			int kind = file.GetDword();
			if(kind)
			{
				// 같은게 있는지 찾고
				for(int i=0; i<pPreLoadInfo->Count[ePreLoad_Monster]; i++)
					if(kind == pPreLoadInfo->Kind[ePreLoad_Monster][i])		break;
				
				// 없으면 새로운 몬스터 추가
				if(i == pPreLoadInfo->Count[ePreLoad_Monster])
				{
					pPreLoadInfo->Kind[ePreLoad_Monster][i] = kind;
					++pPreLoadInfo->Count[ePreLoad_Monster];

					//
					if(pPreLoadInfo->Count[ePreLoad_Monster] >= MAX_KIND_PERMAP)
						break;
				}
			}
		CMD_EN
	}
}


#ifdef _TESTCLIENT_
BOOL CGameResourceManager::LoadTestClientInfo()
{
	char filename[64];
	char value[64] = {0,};
	sprintf(filename,"../DeveloperWorking/TestClient.ini");
	CMHFile file;
	if(!file.Init(filename, "rt"))
		return FALSE;

	while(1)
	{
		if(file.IsEOF())
			break;

		strcpy(value,strupr(file.GetString()));

		if(strcmp(value,"*STARTPOSITION") == 0)
		{
			m_TestClientInfo.x = file.GetFloat();
			m_TestClientInfo.z = file.GetFloat();
		}
		else if(strcmp(value,"*INTITAN") == 0)
		{
			m_TestClientInfo.bInTitan = file.GetWord();
		}
		else if(strcmp(value,"*BOSSMONSTERKIND") == 0)
		{
			m_TestClientInfo.BossMonsterKind = file.GetWord();			
		}
		else if(strcmp(value,"*BOSSMONSTERNUM") == 0)
		{
			m_TestClientInfo.BossMonsterNum = file.GetWord();
		}
		else if(strcmp(value,"*BOSSMONSTERFILE") == 0)
		{
			file.GetString(m_TestClientInfo.BossInfofile);
		}		
		else if(strcmp(value,"*EFFECT") == 0)
		{
			m_TestClientInfo.Effect = FindEffectNum(file.GetString());
		}
		else if(strcmp(value,"*MUGONG") == 0)
		{
			m_TestClientInfo.MugongIdx = file.GetWord();
		}
		else if(strcmp(value,"*WEAPON") == 0)
		{
			m_TestClientInfo.WeaponIdx = file.GetWord();
		}
		else if(strcmp(value,"*SHOES") == 0)
		{
			m_TestClientInfo.ShoesIdx = file.GetWord();		
		}
		else if(strcmp(value,"*HAT") == 0)
		{
			m_TestClientInfo.HatIdx = file.GetWord();		
		}
		else if(strcmp(value,"*DRESS") == 0)
		{
			m_TestClientInfo.DressIdx = file.GetWord();		
		}
		else if(strcmp(value,"*GENDER") == 0)
		{
			m_TestClientInfo.Gender = file.GetByte();
		}
		else if(strcmp(value,"*MONSTERNUM") == 0)
		{
			m_TestClientInfo.MonsterNum = file.GetWord();
		}
		else if(strcmp(value,"*MONSTEREFFECT") == 0)
		{
			m_TestClientInfo.MonsterEffect = FindEffectNum(file.GetString());
		}
		else if(strcmp(value,"*MONSTERDIE") == 0)
		{
			m_TestClientInfo.bMonsterDie = file.GetWord();
		}		
		else if(strcmp(value,"*MONSTERKIND") == 0)
		{
			m_TestClientInfo.MonsterKind = file.GetWord();
		}		
		else if(strcmp(value,"*MAP") == 0)
		{
			m_TestClientInfo.Map = file.GetByte();
		}
		else if(strcmp(value,"*CHARACTERNUM") == 0)
		{
			m_TestClientInfo.CharacterNum = file.GetWord();
		}
		else if(strcmp(value,"*HAIR") == 0)
		{
			m_TestClientInfo.HairType = file.GetByte();
		}
		else if(strcmp(value,"*FACE") == 0)
		{
			m_TestClientInfo.FaceType = file.GetByte();
		}
		else if(strcmp(value,"*LIGHTEFFECT") == 0)
		{
			m_TestClientInfo.LightEffect = file.GetBool();
		}
		//trustpak
		else if(strcmp(value, "*TEST_CHARACTER_NUM") == 0)
		{
			m_TestClientInfo.dwTestCharacterNo = file.GetDword();
		}
		else if(strcmp(value, "*TEST_CHARACTER_INFO") == 0)
		{
			static int s_iTestCharacterCount = 0;

			if (MAX_TEST_CHARACTER_NO > s_iTestCharacterCount)
			{
				m_TestClientInfo.aTestCharacterInfo[s_iTestCharacterCount].byGender		= file.GetByte();
				m_TestClientInfo.aTestCharacterInfo[s_iTestCharacterCount].fX			= file.GetFloat();
				m_TestClientInfo.aTestCharacterInfo[s_iTestCharacterCount].fY			= file.GetFloat();
				m_TestClientInfo.aTestCharacterInfo[s_iTestCharacterCount].fZ			= file.GetFloat();
				m_TestClientInfo.aTestCharacterInfo[s_iTestCharacterCount].fAngle		= DEGTORAD( file.GetFloat() );

				++s_iTestCharacterCount;
			}			
		}
		//
		else if(strcmp(value, "*TITANWEAPON") == 0)
		{
			m_TestClientInfo.titaninfo.WearedItemIdx[eTitanWearedItem_Weapon] = file.GetWord();
		}
		else if(strcmp(value, "*TESTEFFECT") == 0)
		{
			m_TestClientInfo.nTestEffectNum = FindEffectNum(file.GetString());
		}
	}
	
	file.Release();



	return TRUE;
}
#endif

BOOL CGameResourceManager::LoadPlusItemEffect()
{
	CMHFile file;
	char filename[64];

#ifdef _FILE_BIN_	
	sprintf(filename,"Resource/Client/PlusItemEffect.bin");
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/Client/PlusItemEffect.txt");
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif

	PLUSITEM_EFFECT_INFO* pPlusItemEffectInfo = NULL;	

	while(1)
	{
		if( file.IsEOF() )	break;

		pPlusItemEffectInfo = new PLUSITEM_EFFECT_INFO;
		
		pPlusItemEffectInfo->wItemType = file.GetWord();		
		memset(pPlusItemEffectInfo->szCHXName, 0, sizeof(pPlusItemEffectInfo->szCHXName));
		for(int i = 0 ; i < MAX_PLUSITEM_EFFECT_NUM ; i++)
		{
			SafeStrCpy(pPlusItemEffectInfo->szCHXName[i], file.GetString(), MAX_CHXNAME_LENGTH+1);
		}

		m_PlusItemEffectInfoTable.Add(pPlusItemEffectInfo, pPlusItemEffectInfo->wItemType);

	}	
	file.Release();

	return TRUE;
}

void CGameResourceManager::RemovePlusItemEffectTable()
{
	m_PlusItemEffectInfoTable.SetPositionHead();
	while (PLUSITEM_EFFECT_INFO* pInfo = m_PlusItemEffectInfoTable.GetData())
	{
		SAFE_DELETE(pInfo);
	}
	m_PlusItemEffectInfoTable.RemoveAll();
}


PLUSITEM_EFFECT_INFO* CGameResourceManager::GetPlusItemEffectInfo(WORD wItemType)
{
	m_PlusItemEffectInfoTable.SetPositionHead();
	while (PLUSITEM_EFFECT_INFO* pInfo = m_PlusItemEffectInfoTable.GetData())
	{
		if(pInfo->wItemType == wItemType)
			return pInfo;
	}

	return NULL;
}

//-----------------------------------------------------------------------------------------------------------//
#endif //_CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//

// 06. 05 HIDE NPC - 이영준
#ifdef _SERVER_RESOURCE_FIELD_


BOOL CGameResourceManager::LoadHideNpcList()
{
	CMHFile file;
#ifdef _FILE_BIN_
//	if(!file.Init("Resource/HideNpcList.bin","rb"))
	if(!file.Init("Resource/Server/HideNpcList.bin","rb"))
		return FALSE;
#else
	if(!file.Init("Resource/HideNpcList.txt","rt"))
		return FALSE;
#endif
	
	WORD mapnum = GetLoadMapNum();
	WORD index = 0;

	while(!file.IsEOF())
	{
		// 현재 맵에 해당되는 NPC만 읽어들인다
		if(file.GetWord() == mapnum)
		{
			index = file.GetWord();

			HIDE_NPC_INFO* info = new HIDE_NPC_INFO;

			memset(info, 0, sizeof(HIDE_NPC_INFO));

			info->UniqueIdx = index;
			
			m_HideNpcTable.Add(info, index);
		}
	}

	return TRUE;
}

BOOL CGameResourceManager::LoadFullMoonEventRate()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/FullMoonRate.bin","rb"))
		return FALSE;
#else
	if(!file.Init("Resource/FullMoonRate.txt","rt"))
		return FALSE;
#endif

	memset( m_FullMoonEventRate, 0, sizeof(int)*eFULLMOONEVENT_MAX );

	char value[64] = {0,};

	while(!file.IsEOF())
	{
		strcpy(value,strupr(file.GetString()));

		if(strcmp(value,"#NOTHING") == 0)
			m_FullMoonEventRate[eFULLMOONEVENT_NONE] = file.GetInt();
		else if(strcmp(value,"#DUN") == 0)
			m_FullMoonEventRate[eFULLMOONEVENT_BUN] = file.GetInt();
		else if(strcmp(value,"#CHINESECAKE") == 0)
			m_FullMoonEventRate[eFULLMOONEVENT_CHINESECAKE] = file.GetInt();
	}

	return TRUE;
}

int CGameResourceManager::GetFullMoonEventIdx( int Rate )
{
	int tr = 0;
	for(int i=0; i<eFULLMOONEVENT_MAX; ++i)
	{
		tr += m_FullMoonEventRate[i];
		if( tr >= Rate )
			return i;
	}

	return eFULLMOONEVENT_NONE;
}

// magi82 //////////////////////////////////////////////////////////////////////////
BOOL CGameResourceManager::LoadLevPenalty()
{
	LEV_PENALTY* pTemp = NULL;

	CMHFile file;
#ifdef _FILE_BIN_
//	if(!file.Init("./Resource/ExpPenalty.bin", "rb"))
	if(!file.Init("./Resource/Server/ExpPenalty.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/ExpPenalty.txt", "rt"))
		return FALSE;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;

		pTemp = m_LevPenaltyPool->Alloc();

		int Index = file.GetInt();
		pTemp->fNow = file.GetFloat();
		pTemp->fSave = file.GetFloat();

		m_LevPenaltyTable.Add(pTemp, Index);
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////

// magi82 //////////////////////////////////////////////////////////////////////////
LEV_PENALTY* CGameResourceManager::GetLevPenalty(  LEVELTYPE Lev  )
{
	return m_LevPenaltyTable.GetData( Lev );
}

///////////////////////////////////////////////////////////////////////////////////


BOOL CGameResourceManager::LoadPaneltyTime()
{
	char filename[64];
	CMHFile file;
#ifdef _FILE_BIN_
	sprintf(filename,"Resource/PenaltyTime.bin");	
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/PenaltyTime.txt");	
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif

	m_PaneltyTime = file.GetDword(); 

	return TRUE;
}


void CGameResourceManager::LoadTitanMapMonsterDropInfo()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/Server/TitanMapDropInfo.bin", "rb"))
		return;
#else
	if(!file.Init("Resource/TitanMapDropInfo.txt", "rt"))
		return;
#endif

	DWORD level = 0;

	while(1)
	{
		if(file.IsEOF())
			break;

		MONSTERDROP_TIMAP* pdropinfo = new MONSTERDROP_TIMAP;

		level = file.GetDword();

		for(int i=0; i<MAX_PARTY_LISTNUM; ++i)		
		{            
			pdropinfo->stDropInfo[i].wExpPoint = file.GetDword();
			pdropinfo->stDropInfo[i].wMoneyMin = file.GetDword();
			pdropinfo->stDropInfo[i].wMoneyMax = file.GetDword();
		}


		m_TitanMapMosterDropTable.Add( pdropinfo, level );
	}

	file.Release();
}

#endif

#endif
