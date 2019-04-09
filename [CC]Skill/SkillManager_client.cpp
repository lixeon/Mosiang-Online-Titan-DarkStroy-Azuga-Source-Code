// SkillManager.cpp: implementation of the CSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _MHCLIENT_

#include "SkillManager_client.h"
#include "ActionTarget.h"

#include "Hero.h"
#include "MoveManager.h"
#include "QuickManager.h"
#include "Gamein.h"
#include "QuickItem.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "TacticManager.h"

#include "ChatManager.h"
#include "ExchangeManager.h"

#include "GameResourceManager.h"
#include "QuickDialog.h"

#include "PKManager.h"
#include "ObjectActionManager.h"
#include "BattleSystem_Client.h"
#include "../[CC]BattleSystem/GTournament/Battle_GTournament.h"

#include "PeaceWarModeManager.h"

#include "MAINGAME.h"

#include "SkillDelayManager.h"

#include "PeaceWarModeManager.h"
#include "InventoryExDialog.h"
#include "Item.h"
#include "PartyWar.h"
#include "ItemManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//GLOBALTON(CSkillManager);
CSkillManager::CSkillManager()
{
	g_PoolSTLIST.Init(100,50,"g_PoolSTLIST");
	m_pSkillInfo = NULL;

	m_HwaKyungSkillTable.Initialize(30);
	m_GeukMaSkillTable.Initialize(30);

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	m_SkillOptionTable.Initialize(30);
	m_SkillOptionByItemTable.Initialize(30);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// Àº½Å/Çý¾È
	m_SpecialStateInfoTable.Initialize(3);
	//////////////////////////////////////////////////////////////////////////

	// debug¿ë
	m_nSkillUseCount = 0;
}

CSkillManager::~CSkillManager()
{
	Release();
	g_PoolSTLIST.Release();
}

void CSkillManager::Init()
{
	m_SkillInfoTable.Initialize(2000);
//	m_DummySkillInfoTable.Initialize(64);
	m_SkillObjectTable.Initialize(512);
	LoadSkillInfoList();
	LoadSkillChangeInfoList();

	m_SkillAreaMgr.LoadSkillAreaList();

	m_JobSkillProbabilityTable.Initialize(MAX_JOBLEVEL_NUM);	// 2007. 6. 28. CBH - Àü¹®±â¼ú È®·ü Å×ÀÌºí ÃÊ±âÈ­ /////////////////
	LoadJobSkillProbability();				// 2007. 6. 28. CBH - Àü¹®±â¼ú È®·ü ¸®¼Ò½º ·Îµù /////////////////

	// debug¿ë
	m_nSkillUseCount = 0;
}
void CSkillManager::Release()
{
	CSkillInfo* pSInfo = NULL;

	m_SkillInfoTable.SetPositionHead();
	while(pSInfo = m_SkillInfoTable.GetData())
	{
		delete pSInfo;
	}
	m_SkillInfoTable.RemoveAll();
/*	
	m_DummySkillInfoTable.SetPositionHead();
	while(p = m_DummySkillInfoTable.GetData())
	{
		delete p;
	}
	m_DummySkillInfoTable.RemoveAll();
*/	
	CSkillObject* pSObj = NULL;

	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		OBJECTMGR->AddGarbageObject((CObject*)pSObj);
		delete pSObj;
	}
	m_SkillObjectTable.RemoveAll();

	m_SkillAreaMgr.Release();

	PTRLISTSEARCHSTART(m_SkillChangeList,SKILL_CHANGE_INFO*,pInfo)
		delete pInfo;
	PTRLISTSEARCHEND
	m_SkillChangeList.RemoveAll();

	m_HwaKyungSkillTable.RemoveAll();
	m_GeukMaSkillTable.RemoveAll();

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	SKILLOPTION* pSOpt = NULL;

	m_SkillOptionTable.SetPositionHead();
	while(pSOpt = m_SkillOptionTable.GetData())
	{
		delete pSOpt;
	}
	m_SkillOptionTable.RemoveAll();
	m_SkillOptionByItemTable.RemoveAll();
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// Àº½Å/Çý¾È
	SPECIAL_STATE_INFO* pStateInfo = NULL;

	m_SpecialStateInfoTable.SetPositionHead();
	while(pStateInfo = m_SpecialStateInfoTable.GetData())
	{
		delete pStateInfo;
	}
	m_SpecialStateInfoTable.RemoveAll();
	//////////////////////////////////////////////////////////////////////////

	////// 2007. 6. 28. CBH - Àü¹®±â¼ú È®·ü ¸®½ºÆ® »èÁ¦ /////////////////
	JOB_SKILL_PROBABILITY_INFO* pJobSkillInfo = NULL;

	m_JobSkillProbabilityTable.SetPositionHead();
	while(pJobSkillInfo = m_JobSkillProbabilityTable.GetData())
	{
		SAFE_DELETE(pJobSkillInfo);		
	}
	m_JobSkillProbabilityTable.RemoveAll();
	////////////////////////////////////////////////////////////////////////

	// debug¿ë
	m_nSkillUseCount = 0;
}

void CSkillManager::LoadSkillInfoList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	file.Init("Resource/SkillList.bin","rb");
#else
	file.Init("Resource/SkillList.txt","rt");
#endif
	if(file.IsInited() == FALSE)
	{
		//ASSERTMSG(0,"SkillList¸¦ ·ÎµåÇÏÁö ¸øÇß½À´Ï´Ù.");
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		CSkillInfo* pInfo = new CSkillInfo;
//		CSkillInfo* pDummyInfo = new CSkillInfo;
		pInfo->InitSkillInfo(&file);

		//	2005 Å©¸®½º¸¶½º ÀÌº¥Æ® ÄÚµå
		//////////////////////////////////////////////////////////////////////////
		//	½ºÅ³ Àß µé¾î°¡³ª Ã¼Å©Áß...
		WORD SkillIndex = pInfo->GetSkillIndex();
	
		//////////////////////////////////////////////////////////////////////////
		
//		pDummyInfo->InitDummySkillInfo(pInfo);

		ASSERT(m_SkillInfoTable.GetData(pInfo->GetSkillIndex()) == NULL);
		m_SkillInfoTable.Add(pInfo,pInfo->GetSkillIndex());
//		m_DummySkillInfoTable.Add(pDummyInfo,pInfo->GetSkillIndex());
	}

	file.Release();

	file.Init("Resource/SAT.bin","rb");
	if(file.IsInited() == FALSE)
	{
		MessageBox(0,"SAT.bin is not found",0,0);
		return;
	}

	int count = file.GetDword();
	for(int n=0;n<count;++n)
	{
		WORD skillIdx = file.GetWord();
		DWORD aniTimeMale = file.GetDword();
		DWORD aniTimeFemale = file.GetDword();

		CSkillInfo* pSkillInfo = GetSkillInfo(skillIdx);
		if(pSkillInfo == NULL)
			continue;

		pSkillInfo->SetSkillOperateAnimationTime(aniTimeMale,aniTimeFemale);
	}
	file.Release();

	LoadSkillTreeList();

	// È­°æ, ±Ø¸¶ ¸®½ºÆ®
	LoadJobSkillList();
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	LoadSkillOptionList();
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// Àº½Å/Çý¾È
	LoadStateList();
	//////////////////////////////////////////////////////////////////////////
}

void CSkillManager::LoadSkillTreeList()
{
	CMHFile file;
	file.Init("Resource/SkillTree.bin","rb");

	if(file.IsInited() == FALSE)
	{
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		WORD Array[5];

		file.GetString();
		file.GetWord(Array,5);
		
		for(int n=0;n<5;++n)
		{
			WORD Before = 0,After = 0;
			WORD Cur = Array[n];
			if(n != 0) Before = Array[n-1];
			if(n != 4) After = Array[n+1];
			
			CSkillInfo* pInfo = GetSkillInfo(Cur);
			if(pInfo == NULL)
				continue;

			pInfo->SetSkillTree(Before,After,Array);
		}
	}
	file.Release();
}


void CSkillManager::LoadJobSkillList()
{
	CMHFile file;
	file.Init("Resource/JobSkillList.bin","rb");

	if(file.IsInited() == FALSE)
	{
		return;
	}


	char buf[32];
	int Count = 0;
	DWORD SkillIdx = 0;

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		file.GetString(buf);

		if( strcmp( buf, "#HWAKUNG" ) == 0 )
		{
			Count = file.GetInt();

			for(int i=0; i<Count; ++i)
			{
				SkillIdx = file.GetDword();
				CSkillInfo* pInfo = m_SkillInfoTable.GetData( SkillIdx );
				if( !pInfo )		continue;

				m_HwaKyungSkillTable.Add( pInfo, SkillIdx );
			}
		}
		if( strcmp( buf, "#GEUKMA" ) == 0 )
		{
			Count = file.GetInt();

			for(int i=0; i<Count; ++i)
			{
				SkillIdx = file.GetDword();
				CSkillInfo* pInfo = m_SkillInfoTable.GetData( SkillIdx );
				if( !pInfo )		continue;

				m_GeukMaSkillTable.Add( pInfo, SkillIdx );
			}
		}
	}

	file.Release();
}


void CSkillManager::LoadSkillChangeInfoList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	file.Init("Resource/SkillChangeList.bin","rb");
#else
	file.Init("Resource/SkillChangeList.txt","rt");
#endif
	if(file.IsInited() == FALSE)
	{
		ASSERTMSG(0,"SkillChangeList¸¦ ·ÎµåÇÏÁö ¸øÇß½À´Ï´Ù.");
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;
		SKILL_CHANGE_INFO * pInfo = new SKILL_CHANGE_INFO;
		pInfo->wMugongIdx = file.GetWord();
		pInfo->wChangeRate = file.GetByte();
		pInfo->wTargetMugongIdx = file.GetWord();

		m_SkillChangeList.AddTail(pInfo);
	}
}

//////////////////////////////////////////////////////////////////////////
// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
// ¹«°ø º¯È¯ Ãß°¡
void CSkillManager::LoadSkillOptionList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	file.Init("Resource/SkillOptionList.bin","rb");
#else
	file.Init("Resource/SkillOptionList.txt","rt");
#endif
	if(file.IsInited() == FALSE)
	{
		ASSERTMSG(0,"SkillOptionList¸¦ ·ÎµåÇÏÁö ¸øÇß½À´Ï´Ù.");
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		SKILLOPTION* pSOpt = new SKILLOPTION;

		memset(pSOpt, 0, sizeof(SKILLOPTION));

		pSOpt->Index		= file.GetWord();
		pSOpt->SkillKind	= file.GetWord();
		pSOpt->OptionKind	= file.GetWord();
		pSOpt->OptionGrade	= file.GetWord();
		pSOpt->ItemIndex	= file.GetWord();
		
		for(int i = 0; i < MAX_SKILLOPTION_COUNT; i++)
		{
			WORD Kind = file.GetWord();
			
			switch(Kind)
			{
			case eSkillOption_Range:
				pSOpt->Range = file.GetInt();
				break;
			
			case eSkillOption_ReduceNaeRyuk:
				pSOpt->ReduceNaeRyuk = file.GetFloat();
				break;
			
			case eSkillOption_PhyAtk:
				pSOpt->PhyAtk = file.GetFloat();
				break;
				
			case eSkillOption_BaseAtk:
				pSOpt->BaseAtk = file.GetFloat();
				break;
			
			case eSkillOption_AttAtk:
				pSOpt->AttAtk = file.GetFloat();
				break;

			case eSkillOption_Life:
				pSOpt->Life = file.GetInt();
				break;

			case eSkillOption_NaeRyuk:
				pSOpt->NaeRyuk = file.GetInt();
				break;

			case eSkillOption_Shield:
				pSOpt->Shield = file.GetInt();
				break;

			case eSkillOption_PhyDef:
				pSOpt->PhyDef = file.GetFloat();
				break;

			case eSkillOption_AttDef:
				pSOpt->AttDef = file.GetFloat();
				break;

			case eSkillOption_Duration:
				pSOpt->Duration = file.GetDword();
				break;

			case eSkillOption_None:
			default:
				file.GetWord();
				break;
			}
		}

		m_SkillOptionTable.Add(pSOpt, pSOpt->Index);
		m_SkillOptionByItemTable.Add(pSOpt, pSOpt->ItemIndex);
	}
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
// Àº½Å/Çý¾È
void CSkillManager::LoadStateList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	file.Init("Resource/StateInfo.bin","rb");
#else
	file.Init("Resource/StateInfo.txt","rt");
#endif
	if(file.IsInited() == FALSE)
	{
		ASSERTMSG(0,"StateList¸¦ ·ÎµåÇÏÁö ¸øÇß½À´Ï´Ù.");
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		SPECIAL_STATE_INFO* pInfo = new SPECIAL_STATE_INFO;

		memset(pInfo, 0, sizeof(SPECIAL_STATE_INFO));

		pInfo->Index = file.GetWord();
		pInfo->IconIdx = file.GetWord();
		pInfo->TickTime = file.GetDword();
		pInfo->NaeRyuk = file.GetWord();
		pInfo->DelayTime = file.GetDword();
		pInfo->DurationTime = file.GetDword();

		m_SpecialStateInfoTable.Add(pInfo, pInfo->Index);
	}
}
//////////////////////////////////////////////////////////////////////////

CSkillInfo*	CSkillManager::GetSkillInfo(WORD SkillInfoIdx)
{
	return (CSkillInfo*)m_SkillInfoTable.GetData(SkillInfoIdx);
}
/*
CSkillInfo*	CSkillManager::GetDummySkillInfo(WORD SkillInfoIdx)
{
	return (CSkillInfo*)m_DummySkillInfoTable.GetData(SkillInfoIdx);
}
*/
WORD CSkillManager::GetSkillTooltipInfo(WORD SkillInfoIdx)
{
	SKILLINFO* skInfo = ((CSkillInfo*)m_SkillInfoTable.GetData(SkillInfoIdx))->GetSkillInfo();
	if(!skInfo)
		return 0;
	return skInfo->SkillTooltipIdx;
}
CSkillObject* CSkillManager::GetSkillObject(DWORD SkillObjectID)
{
	return (CSkillObject*)m_SkillObjectTable.GetData(SkillObjectID);
}
WORD CSkillManager::GetComboSkillIdx(CHero* pHero)
{
	WORD SkillNum = 0;
	int WeaponEquipType = pHero->GetWeaponEquipType();
	int CurComboNum = pHero->GetCurComboNum();
	if( CurComboNum == SKILL_COMBO_NUM || 
		CurComboNum == MAX_COMBO_NUM ||
		CurComboNum >= pHero->GetLevel() * 2)
	{
		CurComboNum = 0;
		pHero->SetCurComboNum(0);
	}

	// ÇöÀç ÄÞº¸+1 ¹øÀÇ ÄÞº¸ÀÇ ¾îÅÃ¹øÈ£¸¦ ¾ò¾î¿Í¾ß ÇÏÁö¸¸
	// SkillNum = COMBO_???_MIN + ÄÞº¸¹øÈ£ - 1 = COMBO_???_MIN + CurComboNum +1 -1
	// so... COMBO_???_MIN + CurComboNum ÀÌ´Ù.
	switch(WeaponEquipType)
	{
	case WP_GUM:
		SkillNum = COMBO_GUM_MIN + CurComboNum;
		break;
	case WP_GWUN:
		SkillNum = COMBO_GWUN_MIN + CurComboNum;
		break;
	case WP_DO:
		SkillNum = COMBO_DO_MIN + CurComboNum;
		break;
	case WP_CHANG:
		SkillNum = COMBO_CHANG_MIN + CurComboNum;
		break;
	case WP_GUNG:
		SkillNum = COMBO_GUNG_MIN + CurComboNum;
		break;
	case WP_AMGI:
		SkillNum = COMBO_AMGI_MIN + CurComboNum;
		break;
	//	2005 Å©¸®½º¸¶½º ÀÌº¥Æ® ÄÚµå
	case WP_EVENT:
		SkillNum = COMBO_EVENT_MIN;
		break;
	// 2006 Ãß¼®
	case WP_EVENT_HAMMER:
		SkillNum = COMBO_EVENT_HAMMER;
		break;
	}

	// !!!!!!!!!!! magi82 - ¿ø·¡´Â Å¸ÀÌÅº ÄÞº¸¸¦ µû·Î ¸¸µé¾î¾ßÇÏÁö¸¸ Áö±ÝÀº ±ÞÇØ¼­ ÀÏ´Ü ÀÌ·¸°Ô ÀÓ½Ã·Î ¾¸ !!!!!!!!!!!!!!!!!!1
	if(pHero->InTitan())
		SkillNum += 10000;

	return SkillNum;
}

//////////////////////////////////////////////////////////////////////////
// ¹«°ø Á¤º¸¸¦ Äü¸Å´ÏÁ®¿¡¼­ ¼±ÅÃµÈ Á¤º¸¸¦ °¡Á®¿Í
// ¹«°øÃ¢¿¡¼­ ´õºíÅ¬¸¯À¸·Î »ç¿ëÇÒ½Ã ´ÜÃàÃ¢¿¡¼­ »ç¿ëµÇ¾ú´ø ¹«°øÀÌ
// »ç¿ëµÇ´Â ¹ö±×·Î ½ºÅ³ ¸Å´ÏÀú°¡ ¸¶Áö¸·À¸·Î »ç¿ëÇÑ ½ºÅ³À» ÀúÀåÇÏµµ·Ï ÇÏ¿©
// »ç¿ëÇÏÁö ¾Ê´Â ÇÔ¼ö
/*
WORD CSkillManager::GetMugongSkillIdx(CHero* pHero)
{	
	WORD SkillNum = 0;
#ifdef _TESTCLIENT_
	SkillNum = GAMERESRCMNGR->m_TestClientInfo.MugongIdx;
#else
	int abspos = QUICKMGR->GetSelectedQuickAbsPos();
	CQuickItem* pQuickItem;
	pQuickItem = GAMEIN->GetQuickDialog()->GetQuickItem(abspos);
	if(pQuickItem == NULL)
		return FALSE;
	SkillNum = pQuickItem->GetSrcIdx();
#endif
	return SkillNum;
}
*/
//////////////////////////////////////////////////////////////////////////

// ½ºÅ³À» »ç¿ë
BOOL CSkillManager::ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,BOOL bMugong)
{
/*
	WORD SkillNum;
	if(bMugong == FALSE)
		SkillNum = GetComboSkillIdx(pHero);
	else
		SkillNum = GetMugongSkillIdx(pHero);

	CSkillInfo* pSkillInfo = GetSkillInfo(SkillNum);
*/
	WORD SkillNum;
	CSkillInfo* pSkillInfo;

	if(bMugong == FALSE)
	{
		SkillNum = GetComboSkillIdx(pHero);
		pSkillInfo = GetSkillInfo(SkillNum);
	}
	else
		pSkillInfo = m_pSkillInfo;

	//ASSERT(pSkillInfo);
	if(pSkillInfo == NULL) 
		return FALSE;

	return ExcuteSkillSYN(pHero,pTarget,pSkillInfo);
}

BOOL CSkillManager::ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,CSkillInfo* pSkillInfo)
{
	CActionTarget target;
	target.CopyFrom(pTarget);
	SKILLOPTION* pSkillOption = NULL;

	//ASSERT(pSkillInfo);
	if(pSkillInfo == NULL)
		return FALSE;
	
	int SkillLevel = 0;
	//if( pHero->InTitan() && pSkillInfo->GetSkillIndex() > SKILLNUM_TO_TITAN )
	//	SkillLevel = pHero->GetMugongLevel( pSkillInfo->GetSkillIndex() - SKILLNUM_TO_TITAN );
	//else
		SkillLevel = pHero->GetMugongLevel(pSkillInfo->GetSkillIndex());
	
	//SW070127 Å¸ÀÌÅº
	if( 0 == SkillLevel )
		return FALSE;

	WORD wSkillKind = pSkillInfo->GetSkillKind();

	// magi82 - Titan(070912) Å¸ÀÌÅº ¹«°ø¾÷µ¥ÀÌÆ®
	// ÀÌÁ¦ Å¸ÀÌÅº ¹«±â¿Í Ä³¸¯ÅÍÀÇ ¹«±â´Â º°°³ÀÌ´Ù.(¹«±â°¡ ¼­·Î ´Ù¸£´Ù°í ÇØ¼­ ½ºÅ³ÀÌ ¾È³ª°¡´Â°Ô ¾Æ´Ô)
	//if( (pHero->InTitan() == TRUE) && (CheckSkillKind(wSkillKind) == FALSE) )
	//{
	//	WORD weapon = pHero->GetWeaponEquipType();
	//	WORD titanWeapon = pHero->GetTitanWeaponEquipType();
	//	if(weapon != titanWeapon)
	//	{
	//		pHero->DisableAutoAttack(); //Àü¹® ½ºÅ³À» ¾²¸é ¹«Á¶°Ç ÀÚµ¿¾îÅÃ ±â´ÉÀ» ²ö´Ù.		
	//		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1644));
	//		return FALSE;
	//	}
	//}

	WORD SkillOptionIndex = pHero->GetSkillOptionIndex(pSkillInfo->GetSkillIndex());
	
	if(SkillOptionIndex)
		pSkillOption = m_SkillOptionTable.GetData(SkillOptionIndex);

	if(pSkillInfo->IsExcutableSkillState(pHero,SkillLevel,pSkillOption) == FALSE)
	{
		WORD wSkillKind = pSkillInfo->GetSkillKind();
		if( CheckSkillKind(wSkillKind) == FALSE )
		{
			pHero->SetCurComboNum(0);
		}	
		//"°ø°Ý¸øÇØ!"
		return FALSE;
	}
	
	// Áø¹ý »ç¿ëÀÌ¸é Áø¹ý ¸Þ´ÏÁ® ÂÊÀ¸·Î µ¹¸°´Ù.
	if(pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB)
	{
/*
				if( pTarget->GetTargetID() != 0 )
				{
					CObject* pTargetObj = OBJECTMGR->GetObject( pTarget->GetTargetID() );
					if( pTargetObj )
					{
		                if( !PARTYWAR->IsEnemy( (CPlayer*)pTargetObj ) )	return FALSE;
					}
				}
*/		

		// Áø¹ýStart¹øÈ£¿Í Áø¹ýSkill¹øÈ£´Â °°´Ù.
		TACTICMGR->HeroTacticStart(pSkillInfo->GetSkillIndex());
		return FALSE;
	}

	if(pSkillInfo->ComboTest(pHero) == FALSE)
	{
		return FALSE;
	}
	
	if(pSkillInfo->ConvertTargetToSkill(pHero,&target) == FALSE)
	{
		return FALSE;
	}

	// 2007. 7. 3. CBH - Àü¹®½ºÅ³¹ßµ¿½Ã ¸ó½ºÅÍ¿ÍÀÇ °ü°è Ã³¸® ÇÔ¼ö Ãß°¡
	if(!IsJobSkill(pHero, pTarget, pSkillInfo))
	{
		return FALSE;
	}

	if(pSkillInfo->IsValidTarget(pHero, &target) == FALSE)
		return FALSE;

	// magi82(5) - Titan(071023) Å¸ÀÌÅº ¹«°ø½Ã ¹«±â Ã¼Å©ÇÒ¶§ Å¸°Ù Ã¼Å©º¸´Ù µÚ¿¡ ³õ¾Æ¾ß Å¸°ÙÀÌ ¸ó½ºÅÍ°¡ ¾Æ´Ò¶§ Ã¤ÆÃ¸Þ¼¼Áö°¡ ¶ßÁö¾Ê´Â´Ù.
	if(CheckTitanWeapon(pHero, pSkillInfo->GetSkillInfo()) == FALSE)
	{
		return FALSE;
	}

//KES 040308

	if(PEACEWARMGR->IsPeaceMode(pHero) == TRUE)		//KES¿Å±è
		PEACEWARMGR->ToggleHeroPeace_WarMode();
	
// RaMa È­°æ, ±Ø¸¶ Ã¼Å©
	if( pHero->GetStage() & eStage_Hwa )
	{
		if( m_GeukMaSkillTable.GetData( pSkillInfo->GetSkillIndex() ) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1144), CHATMGR->GetChatMsg(892) );
			return FALSE;
		}
	}
	else if( pHero->GetStage() & eStage_Geuk )
	{
		if( m_HwaKyungSkillTable.GetData( pSkillInfo->GetSkillIndex() ) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1144), CHATMGR->GetChatMsg(890) );
			return FALSE;
		}
	}

	
//
	VECTOR3* pTargetPos = target.GetTargetPosition();
	if(pTargetPos == NULL)
		return FALSE;
	
	if(pSkillInfo->IsInSkillRange(pHero,&target,pSkillOption) == TRUE)
	{	// ¼º°ø		
		MOVEMGR->HeroMoveStop();
		
		//ÀÚ½ÅÀÌ Å¸°ÙÀÏ °æ¿ì¿¡´Â º¸´Â ¹æÇâÀ» ¹Ù²ÙÁö ¾Ê´Â´Ù.
		if( pSkillInfo->GetSkillInfo()->TargetKind != 1 )
			MOVEMGR->SetLookatPos(pHero,pTargetPos,0,gCurTime);
				
		pHero->SetMovingAction(NULL);		

		////////////////////////////////////////////////////////
		//06. 06 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
		//ÀÌÆåÆ® »ý·«(¹«ÃÊ)
		//¹«ÃÊ »óÅÂ¿¡¼­ ¹«ÃÊ °¡´É ¹«°øÀ» ¾µ¶§ ½ÃÀü¿¬Ãâ »ý·«
		if( pHero->IsSkipSkill() == eSkipEffect_Start && pSkillInfo->CanSkipEffect() )
			return RealExcuteSkillSYN(pHero,&target,pSkillInfo);
		////////////////////////////////////////////////////////		

		if( pSkillInfo->GetSkillInfo()->EffectStart != 0 &&
			pSkillInfo->GetSkillInfo()->EffectStartTime != 0)	// ½ÃÀü¿¬ÃâÀÌ ÀÖÀ» °æ¿ì
		{
			return HeroSkillStartEffect(pHero,&target,pSkillInfo);
		}
		else
		{
			return RealExcuteSkillSYN(pHero,&target,pSkillInfo);
		}
	}
	else
	{	// ½ÇÆÐ
		pHero->SetCurComboNum(0);
		CAction MoveAction;
		if(pSkillInfo->IsMugong() == FALSE)
		{
			pSkillInfo = GetSkillInfo(GetComboSkillIdx(pHero));
		}
		MoveAction.InitSkillAction(pSkillInfo,&target);
		MOVEMGR->SetHeroActionMove(&target,&MoveAction);
		return FALSE;
	}
	

	return TRUE;
}

BOOL CSkillManager::ExcuteTacticSkillSYN(CHero* pHero,TACTIC_TOTALINFO* pTInfo,BYTE OperNum)
{
	CActionTarget target;
	target.InitActionTarget(&pTInfo->Pos,NULL);

	CSkillInfo* pSkillInfo = GetSkillInfo(pTInfo->TacticId);
	//ASSERT(pSkillInfo);
	if(pSkillInfo == NULL)
		return FALSE;

	return RealExcuteSkillSYN(pHero,&target,pSkillInfo);
}

void CSkillManager::GetMultiTargetList(CSkillInfo* pSkillInfo,CHero* pHero,CActionTarget* pTarget)
{
	WORD Radius = pSkillInfo->GetSkillInfo()->TargetRange;
	WORD AreaNum = pSkillInfo->GetSkillInfo()->TargetAreaIdx;
	if(AreaNum != 0)
	{
		CSkillArea* pSkillArea = GetSkillArea(pHero,pTarget,pSkillInfo);	// AreaÀÇ Áß½ÉÁÂÇ¥±îÁö ¼ÂÆÃµÇ¾îÁ® ¿Â´Ù.
		pTarget->SetTargetObjectsInArea(pHero,pSkillArea);
	}
	else if(Radius != 0)
	{
		/// 06. 08. ÀÚ±âÁß½É¹üÀ§Çü ½ºÅ³ ¹ö±× ¼öÁ¤ - ÀÌ¿µÁØ
		/// ÀÚ±â Áß½É ¹üÀ§ÀÏ °æ¿ì¿¡ ½ºÅÝ°ú ¹«°ø º¯È¯¿¡ ÀÇÇÑ »çÁ¤°Å¸® È¿°ú°¡
		/// ¹«°ø ¹üÀ§¿¡ Àû¿ëµÇ¾î¾ß ÇÑ´Ù.
		if( pSkillInfo->GetSkillInfo()->TargetAreaPivot == 1 && pSkillInfo->GetSkillInfo()->TargetRange != 0 )
		{
			Radius += (WORD)HERO->GetAddAttackRange();

			WORD SkillOptionIndex = HERO->GetSkillOptionIndex(pSkillInfo->GetSkillIndex());
			SKILLOPTION* pSkillOption = NULL;

			if(SkillOptionIndex)
			{
				pSkillOption = m_SkillOptionTable.GetData(SkillOptionIndex);
			}

			if(pSkillOption)
			{
				Radius += pSkillOption->Range;
			}	
		}

		pTarget->SetTargetObjectsInRange(pSkillInfo->GetTargetAreaPivotPos(&pHero->GetCurPosition(),pTarget->GetTargetPosition()),Radius);
	}
}

BOOL CSkillManager::RealExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,CSkillInfo* pSkillInfo)
{
/*
	CSkillInfo* pDummyInfo = GetDummySkillInfo(pSkillInfo->GetSkillIndex());
	if(pDummyInfo->CheckOriginal(pSkillInfo) == FALSE)
	{
		MSGBASE msg;
		SetProtocol(&msg,MP_USERCONN,MP_USERCONN_CHEAT_USING);
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));

//		MAINGAME->SetGameState(eGAMESTATE_END);
		
		return FALSE;
	}
*/
	SKILLOPTION* pSkillOption = NULL;

	if(pSkillInfo == NULL)
		return FALSE;
		
	int SkillLevel = pHero->GetMugongLevel(pSkillInfo->GetSkillIndex());

	WORD SkillOptionIndex = pHero->GetSkillOptionIndex(pSkillInfo->GetSkillIndex());
	
	if(SkillOptionIndex)
		pSkillOption = m_SkillOptionTable.GetData(SkillOptionIndex);

	if(pSkillInfo->IsExcutableSkillState(pHero,SkillLevel,pSkillOption) == FALSE)
	{
		pHero->SetCurComboNum(0);
		//"°ø°Ý¸øÇØ!"
		return FALSE;
	}

	// 2005 Å©¸®½º¸¶½º ÀÌº¥Æ®
	if( pHero->GetWeaponEquipType() == WP_EVENT && pSkillInfo->GetSkillIndex() == COMBO_EVENT_MIN )
	{
		//CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemLike( EVENT_ITEM_SNOW );
		//SW061211 Å©¸®½º¸¶½ºÀÌº¥Æ®
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetPriorityItemForCristmasEvent();
		
		if( !pItem )
		{
		//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(583) );
			return	FALSE;
		}

//		GAMEIN->GetInventoryDialog()->UseItem( pItem );
		
		// ´õºí Å¬¸¯À¸·Î »ç¿ëµÇ¾îÁö´Â °Í¶§¹®¿¡ Á÷Á¢ Àü¼Û
		MSG_ITEM_USE_SYN msg;

		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.wItemIdx = pItem->GetItemIdx();
		msg.TargetPos = pItem->GetPosition();

		NETWORK->Send(&msg,sizeof(msg));

		// debug¿ë
		ITEMMGR->m_nItemUseCount++;
	}

	// Áø¹ý½Ã ³»·ÂºÎÁ·
	if(pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB && pHero->GetNaeRyuk() < pSkillInfo->GetNeedNaeRyuk(1))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(401) );		
		return FALSE;
	}

	//°ø°ÝÇÏ¸é pkÁö¼Ó½Ã°£À» ¿¬ÀåÇÑ´Ù.
	if( HERO->IsPKMode() )
	{
		CObject* pObject = OBJECTMGR->GetObject(pTarget->GetTargetID());
		if( pObject )
		if( pObject->GetObjectKind() == eObjectKind_Player )
		{
			int SkillLevel = pHero->GetMugongLevel(pSkillInfo->GetSkillIndex());
			if( pSkillInfo->GetFirstPhyAttack( SkillLevel ) ||
				pSkillInfo->GetFirstAttAttack( SkillLevel ) )
			{
				PKMGR->SetPKStartTimeReset();
			}
		}
	}

	HERO->SetNextAction(NULL);

	if(pSkillInfo->IsMultipleTargetSkill() == TRUE)
	{
		pTarget->ConvertMainTargetToPosition(pHero,pSkillInfo->GetSkillRange());
		GetMultiTargetList(pSkillInfo,pHero,pTarget);
	}

	CSkillObject* pSObj = CreateTempSkillObject(pSkillInfo,pHero,pTarget);
	if(pSObj == NULL)
		return FALSE;
	
	pSkillInfo->SendMsgToServer(pHero,pTarget);
	
	pHero->SetSkillCoolTime(pSkillInfo->GetSkillInfo()->DelayTime);

	if(pHero->IsSkipSkill())
	{
		pHero->GetDelayGroup()->AddDelay(
			CDelayGroup::eDK_Skill,pSkillInfo->GetSkillIndex(),
			pSkillInfo->GetSkillInfo()->DelayTime - pSkillInfo->GetSkillOperateAnimationTime( 1 ));
	}
	else
	{
		pHero->GetDelayGroup()->AddDelay(
			CDelayGroup::eDK_Skill,pSkillInfo->GetSkillIndex(),
			pSkillInfo->GetSkillInfo()->DelayTime);
	}
	//¿©±â¼­ ³ª¸ÓÁö ½ºÅ³µµ »¡°²°Ô ¸¸µé¾îÁÖÀÚ.
	SKILLDELAYMGR->AddSkillDelay( pSkillInfo->GetSkillIndex() );


	pHero->SetCurComboNum(pSkillInfo->GetSkillInfo()->ComboNum);

	m_pSkillInfo = NULL;

#ifdef _TESTCLIENT_
	static IDDDD = 0;
	MSG_SKILLOBJECT_ADD msg;
	SKILLOBJECT_INFO info;
	memcpy(&info,pSObj->GetSkillObjectInfo(),sizeof(SKILLOBJECT_INFO));
	info.SkillObjectIdx = SKILLOBJECT_ID_START+IDDDD++;
	info.StartTime = gCurTime;
	info.SkillLevel = 5;
	msg.InitMsg(&info,TRUE);
	CTargetListIterator iter(&msg.TargetList);
	pTarget->SetPositionFirstTargetObject();
	CBattle* pBattle = BATTLESYSTEM->GetBattle(HERO);
	while(CObject* pObject = pTarget->GetNextTargetObject())
	{
		RESULTINFO dinfo;
		dinfo.Clear();
		if(pBattle->IsEnemy(pHero,pObject) == TRUE)
		{
			dinfo.bCritical = 1;//rand()%5 ? FALSE : TRUE;
			dinfo.RealDamage = rand() % 50;
			dinfo.ShieldDamage = rand() % 30;
			dinfo.CounterDamage = 0;
			dinfo.StunTime = 1000;//(rand() % 30 == 0) ? 2000 : 0;
			iter.AddTargetWithResultInfo(pObject->GetID(),SKILLRESULTKIND_NEGATIVE,&dinfo);
		}
		else
		{
			iter.AddTargetWithResultInfo(pObject->GetID(),SKILLRESULTKIND_POSITIVE,&dinfo);
		}

		
	}

	iter.Release();

	NetworkMsgParse(MP_SKILL_SKILLOBJECT_ADD,&msg);
#endif

	//SW05810 ÆòÈ­¸ðµå ÀÚµ¿ÀüÈ¯ ÀÛ¾÷
	PEACEWARMGR->SetCheckTime(gCurTime);

	return TRUE;
}

// ´ÙÀ½ ÄÞº¸ ½ºÅ³À» ÁöÁ¤
void CSkillManager::SetNextComboSkill(CHero* pHero,CActionTarget* pTarget,BOOL bMugong)
{
	CSkillInfo* pNextSkill;
	WORD NextSkillIdx;
/*
	if(bMugong)
		NextSkillIdx = GetMugongSkillIdx(pHero);
	else
		NextSkillIdx = GetComboSkillIdx(pHero);
	pNextSkill = GetSkillInfo(NextSkillIdx);
*/ //GetMugongSkillIdx() ÇÔ¼ö¸¦ »ç¿ëÇÏÁö ¾Ê°Ô µÇ¾î ¼öÁ¤
	if(bMugong)
		pNextSkill = m_pSkillInfo;
	else
	{
		NextSkillIdx = GetComboSkillIdx(pHero);
		pNextSkill = GetSkillInfo(NextSkillIdx);
	}
	
	if(pNextSkill == NULL)
		return;
	CAction act;
	act.InitSkillAction(pNextSkill,pTarget);
	pHero->SetNextAction(&act);
}

// ÀÌº¥Æ® ÇÚµé ÇÔ¼öµé
BOOL CSkillManager::OnSkillCommand(CHero* pHero,CActionTarget* pTarget,BOOL bMugong)
{
	//////////////////////////////////////////////////////////////////////////
	// Á×Àº »ç¶÷Àº °ø°ÝÇÒ¼ö ¾øÀ½
	if(pTarget->GetTargetID() != 0)
	{
		CObject* pTObj = OBJECTMGR->GetObject(pTarget->GetTargetID());
		if(pTObj == NULL)
			return FALSE;
		if(pTObj->IsDied() == TRUE)
			return FALSE;

		if( pHero->GetWeaponEquipType() != WP_EVENT )
		{
			if( pTObj->GetObjectKind() & eObjectKind_Monster )
			{
				CMonster* pMonster = (CMonster*)pTObj;
				if(	pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_SM || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_SM
					|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_MD || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_MD
					|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_LG || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_LG
/*					|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_SM
					|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_MD
					|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_LG*/ )
					return FALSE;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	

	// Guild Tournament³ª °ø¼ºÀü¿¡¼­ ObserverÀÌ¸é »ç¿ëºÒ°¡
	CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
	if( pBattle && pBattle->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT ||
		pBattle && pBattle->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		if( pHero->GetBattleTeam() == 2 )
			return FALSE;	
	}

	// 2005 Å©¸®½º¸¶½º ÀÌº¥Æ® ÄÚµå
	// ÀÌº¥Æ® ¹«±â ÀåÂø½Ã ´«µ¢ÀÌ°¡ ¾øÀ¸¸é °ø°ÝºÒ°¡ ³»°ø »ç¿ë ºÒ°¡
	if( pHero->GetWeaponEquipType() == WP_EVENT )
	{
		if( bMugong )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(586) );	
			return FALSE;
		}
		else
		{
			//CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemLike( EVENT_ITEM_SNOW );
			//SW061211 Å©¸®½º¸¶½ºÀÌº¥Æ®
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetPriorityItemForCristmasEvent();

			if( !pItem )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(583) );
				return	FALSE;
			}
		}
	}
	else if( pHero->GetWeaponEquipType() == WP_EVENT_HAMMER )
	{
		if( bMugong )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(798) );	
			return FALSE;
		}
	}

	if( pHero->GetState() == eObjectState_SkillSyn ||
		pHero->GetState() == eObjectState_SkillUsing)
	{
		// 2007. 7. 6. CBH - Àü¹®½ºÅ³Àº ÀÚµ¿ °ø°ÝÀ» ¸·¾Æ¾ßÇÑ´Ù.
		CObject* pObject = OBJECTMGR->GetObject(pTarget->GetTargetID());
		if(pObject == NULL) //¿¹¿ÜÃ³¸®
		{
			return FALSE;
		}

		if( GetObjectKindGroup(pObject->GetObjectKind()) == eOBJECTKINDGROUP_JOB )
		{
			pHero->SetStage(eObjectState_None);
			return FALSE;
		}
		else
		{
			SetNextComboSkill(pHero,pTarget,bMugong);
		}		
	}
	else
		ExcuteSkillSYN(pHero,pTarget,bMugong);		//return FALSE Ã³¸®....¾ø´Ù.. ±¦ÂúÀ»±î?
	return TRUE;
}
DWORD GetComboDelayTime(WORD WeaponKind)
{
	DWORD time = 0;
	ySWITCH(WeaponKind)
		yCASE(WP_GUM)	time = 120;
		yCASE(WP_GWUN)	time = 150;
		yCASE(WP_DO)	time = 150;
		yCASE(WP_CHANG)	time = 150;
		yCASE(WP_GUNG)	time = 50;
		yCASE(WP_AMGI)	time = 80;
	yENDSWITCH
	return time;
}
void CSkillManager::OnComboTurningPoint(CHero* pHero)
{
	if(pHero->GetNextAction()->HasAction())
	{
		if(pHero->GetNextAction()->GetActionKind() != eActionKind_Skill)
			pHero->SetCurComboNum(0);

		pHero->GetNextAction()->ExcuteAction(pHero);
		pHero->GetNextAction()->Clear();
	}
	else
	{		
		if(pHero->IsAutoAttacking())
		{
			if(pHero->GetCurComboNum() < 2)	// ÀÚµ¿°ø°ÝÀº ÄÞº¸ 2±îÁö¸¸	12/3ÀÏ È¸ÀÇ °á°ú 3¿¡¼­ 2·Î ¹Ù²ñ
			{
				if(SKILLMGR->OnSkillCommand(pHero,pHero->GetAutoAttackTarget(),FALSE) == FALSE)
					pHero->DisableAutoAttack();
			}
			else
			{
				OBJECTSTATEMGR->StartObjectState(pHero,eObjectState_SkillDelay);
				OBJECTSTATEMGR->EndObjectState(pHero,eObjectState_SkillDelay,GetComboDelayTime(pHero->GetWeaponEquipType()));
				pHero->SetCurComboNum(0);
			}
		}
	}
}
void CSkillManager::OnExcuteSkillNACKed(SKILLOBJECT_INFO* pInfo)
{
	//ASSERT(0);
}

// SkillObject µî·Ï ¹× ÇØÁ¦
void CSkillManager::DoCreateSkillObject(CSkillObject* pSObj,SKILLOBJECT_INFO* pSOInfo,CTargetList* pTList)
{
	pSObj->Init(pSOInfo,pTList);
	
	CSkillObject* pPreObj = m_SkillObjectTable.GetData(pSObj->GetID());
	//ASSERT(pPreObj == NULL);	
	if(pPreObj != NULL)
	{
		ReleaseSkillObject(pPreObj);
	}

	m_SkillObjectTable.Add(pSObj,pSObj->GetID());
	OBJECTMGR->AddSkillObject(pSObj);
}

CSkillObject* CSkillManager::CreateSkillObject(MSG_SKILLOBJECT_ADD* pSkillObjectAddInfo)
{
	CObject* pOperator = OBJECTMGR->GetObject(pSkillObjectAddInfo->SkillObjectInfo.Operator);

	////////////////////////////////////////////////////////////////////
	/// 06. 08. 2Â÷ º¸½º - ÀÌ¿µÁØ
	/// ÀÏºÎ ½ºÅ³ »ç¿ë½Ã ¹æÇâÀ» ´õ Æ²±â À§ÇØ
	/// ¹Ì¸® ½ºÅ³ Á¤º¸¸¦ °¡Á®¿Í¾ß ÇÑ´Ù
	WORD SkillIdx = pSkillObjectAddInfo->SkillObjectInfo.SkillIdx;
	CSkillInfo* pSkillInfo = GetSkillInfo(SkillIdx);
	CSkillObject* pSObj = pSkillInfo->GetSkillObject();
	////////////////////////////////////////////////////////////////////

	if(pOperator == NULL)
	{
		OBJECTACTIONMGR->ApplyTargetList(pOperator,&pSkillObjectAddInfo->TargetList,eDamageKind_Front);
		return NULL;
	}
	if(pOperator->GetObjectKind() & eObjectKind_Monster)
	{
		VECTOR3 MainTargetPos;
		GetMainTargetPos(&pSkillObjectAddInfo->SkillObjectInfo.MainTarget,&MainTargetPos,NULL);
		/////////////////////////////////////////////////////////////////////////////////////
		/// 06. 08. 2Â÷ º¸½º - ÀÌ¿µÁØ
		/// ±âÁ¸ SetLookatPos ÇÔ¼ö ¸¶Áö¸· ÀÎÀÚ¿¡ Ãß°¡ È¸Àü°¢À» Ãß°¡Çß´Ù.
		MOVEMGR->SetLookatPos(pOperator,&MainTargetPos,0,gCurTime, pSkillInfo->GetAddDegree());
		/////////////////////////////////////////////////////////////////////////////////////
	}		
	else
		MOVEMGR->SetAngle(pOperator,DIRTODEG(pSkillObjectAddInfo->SkillObjectInfo.Direction),0);


	DoCreateSkillObject(pSObj,&pSkillObjectAddInfo->SkillObjectInfo,
						&pSkillObjectAddInfo->TargetList);

	return pSObj;
}


CSkillObject* CSkillManager::CreateTempSkillObject(CSkillInfo* pSkillInfo,CHero* pHero,CActionTarget* pTarget)
{
	///////////////////////////////////////////////////////////////////////////
	// 06. 04. ½ºÅ³ ¹ö±× ¼öÁ¤ - ÀÌ¿µÁØ
	// ÀÓ½Ã°´Ã¼°¡ ³²¾ÆÀÖ´Ù¸é ½ºÅ³À» »ý¼ºÇÏÁö ¾Ê´Â´Ù
	CSkillObject* pTempObj = GetSkillObject(TEMP_SKILLOBJECT_ID);
	if(pTempObj)
	{
//		ASSERT(0);
		return NULL;
	}
	///////////////////////////////////////////////////////////////////////////

	CSkillObject* pSObj = pSkillInfo->GetSkillObject();
	
	SKILLOBJECT_INFO sinfo;
	sinfo.SkillIdx = pSkillInfo->GetSkillIndex();
	sinfo.SkillObjectIdx = TEMP_SKILLOBJECT_ID;
	sinfo.Direction = DEGTODIR(pHero->GetAngleDeg());
	pTarget->GetMainTarget(&sinfo.MainTarget);
	sinfo.Operator = pHero->GetID();
	VECTOR3* pHeroPos = &pHero->GetCurPosition();
	VECTOR3* pTargetPos = pTarget->GetTargetPosition();
	if(pTargetPos == NULL)
		return NULL;
	sinfo.Pos = *(pSkillInfo->GetTargetAreaPivotPos(pHeroPos,pTargetPos));
	sinfo.StartTime = gCurTime;

	CTargetList tlist;
	pTarget->GetTargetArray(&tlist);

	DoCreateSkillObject(pSObj,&sinfo,&tlist);

	// for debug
	
	return pSObj;
}

void CSkillManager::ChangeTempObjToRealObj(MSG_SKILLOBJECT_ADD* pSkillObjectAddInfo)
{
	CSkillObject* pTempObj = GetSkillObject(TEMP_SKILLOBJECT_ID);
//	//ASSERT(pTempObj);

	if(pTempObj == NULL)
	{
		return;
	}

	m_SkillObjectTable.Remove(pTempObj->GetID());
	OBJECTMGR->AddGarbageObject(pTempObj);
	
	DoCreateSkillObject(pTempObj,&pSkillObjectAddInfo->SkillObjectInfo,
							&pSkillObjectAddInfo->TargetList);

	// debug¿ë
	m_nSkillUseCount--;
}

void CSkillManager::ReleaseSkillObject(CSkillObject* pSkillObject)
{
	CSkillInfo* pSInfo = GetSkillInfo(pSkillObject->GetSkillIdx());
	if(pSInfo == NULL)
	{
		char temp[256];
		sprintf(temp,"skillkind:%d  SkillName:%s  oper:%d",
			pSkillObject->GetSkillIdx(),
			pSkillObject->GetSkillInfo()->GetSkillName(),
			pSkillObject->GetOperator() ? pSkillObject->GetOperator()->GetID() : 0);
		ASSERTMSG(0,temp);
		return;
	}
	//ASSERT(pSInfo);

	m_SkillObjectTable.Remove(pSkillObject->GetID());
	OBJECTMGR->AddGarbageObject(pSkillObject);
	
	pSkillObject->ReleaseSkillObject();
	pSInfo->ReleaseSkillObject(pSkillObject,0);
	
}
void CSkillManager::OnReceiveSkillObjectAdd(MSG_SKILLOBJECT_ADD* pmsg)
{
	//////////////////////////////////////////////////////////////////
	// 06. 04. ½ºÅ³ ¹ö±× ¼öÁ¤ - ÀÌ¿µÁØ
	// ÀÚ±â°¡ ¾´ ½ºÅ³ÀÌ¶óµµ 
	// ½ºÅ³ ÃÖÃÊ »ý¼º½Ã¿¡´Â ChangeTempObjToRealObj¸¦ È£ÃâÇÏÁö¸¸
	// ±×µÚ ±×¸®µå ÀÌµ¿µîÀÇ ÀÌÀ¯·Î Add¸¸ µÇ´Â °æ¿ì¿¡´Â
	// CreateSkillObject¸¦ È£ÃâÇØ¼­ »ý¼ºÇØ¾ßÇÑ´Ù.
    //
	//if(pmsg->SkillObjectInfo.Operator == HEROID)
	if(pmsg->SkillObjectInfo.Operator == HEROID && pmsg->bCreate)
		ChangeTempObjToRealObj(pmsg);
	else
		CreateSkillObject(pmsg);
}
void CSkillManager::OnReceiveSkillObjectRemove(MSG_DWORD* pmsg)
{
	CSkillObject* pSObj = GetSkillObject(pmsg->dwData);
	if(pSObj == NULL)
	{
		//ASSERTMSG(0,"SkillObject Remove Failed");
		return;
	}
	ReleaseSkillObject(pSObj);
}

void CSkillManager::OnReceiveSkillStartNack()
{
	HERO->SetCurComboNum(0);
	CSkillObject* pSObj = GetSkillObject(TEMP_SKILLOBJECT_ID);
	//ASSERT(pSObj);
	if(pSObj == NULL)
		return;

	if( HERO == pSObj->GetOperator() )
	{
		m_nSkillUseCount--;
	}

	pSObj->SetMissed();
	ReleaseSkillObject(pSObj);

}

void CSkillManager::OnReceiveSkillSingleResult(MSG_SKILL_SINGLE_RESULT* pmsg)
{
	CSkillObject* pSObj = GetSkillObject(pmsg->SkillObjectID);
	if(pSObj == NULL)
	{
		OBJECTACTIONMGR->ApplyTargetList(NULL,&pmsg->TargetList,eDamageKind_ContinueDamage);
		return;
	}

	pSObj->OnReceiveSkillSingleResult(pmsg);
}

void CSkillManager::OnReceiveSkillObjectTargetListAdd(DWORD SkillObjectID,DWORD AddedObjectID,BYTE bTargetKind)
{
	CSkillObject* pSObj = GetSkillObject(SkillObjectID);
	//ASSERT(pSObj);
	if(pSObj == NULL)
	{
		//ASSERTMSG(0,"SkillObject Target Add Failed(No SObj)");
		return;
	}
	CObject* pObject = OBJECTMGR->GetObject(AddedObjectID);
	if(pObject == NULL)
	{
		//ASSERTMSG(0,"SkillObject Target Add Failed(No Target)");
		return;
	}

	pSObj->AddTargetObject(pObject,bTargetKind);
}
void CSkillManager::OnReceiveSkillObjectTargetListRemove(DWORD SkillObjectID,DWORD RemovedObjectID)
{
	CSkillObject* pSObj = GetSkillObject(SkillObjectID);
	//ASSERT(pSObj);
	if(pSObj == NULL)
	{		
		//ASSERTMSG(0,"SkillObject Target Remove Failed");
		return;
	}

	pSObj->RemoveTargetObject(RemovedObjectID);
}

void CSkillManager::OnReceiveSkillStartEffect(DWORD SkillOperator,DWORD SkillIdx)
{
	CObject* pOperator = OBJECTMGR->GetObject(SkillOperator);
	if(pOperator == NULL)
	{
		//ASSERTMSG(0,"SkillObject StartEffect Failed(No Operator)");
		return;
	}
	
	CSkillInfo* pSkillInfo = GetSkillInfo((WORD)SkillIdx);
	if(pSkillInfo == NULL)
	{		
		//ASSERTMSG(0,"SkillObject StartEffect Failed(No SkillInfo)");
		return;
	}

	SkillStartEffect(pOperator,pSkillInfo);
}

void CSkillManager::OnReceiveSkillObjectOperate(MSG_SKILL_OPERATE* pmsg)
{
	CSkillObject* pSObj = GetSkillObject(pmsg->SkillObjectID);
	//ASSERT(pSObj);
	if(pSObj == NULL)
	{		
		//ASSERTMSG(0,"SkillObject Operate Failed(No SObj)");
		return;
	}

	CObject* pReqObj = OBJECTMGR->GetObject(pmsg->RequestorID);
	pSObj->Operate(pReqObj,&pmsg->MainTarget,&pmsg->TargetList);

}

void CSkillManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	printf("[%d] %d // %d\n", ((MSGBASE*)pMsg)->CheckSum, Protocol, ((MSGBASE*)pMsg)->dwObjectID);

	switch(Protocol)
	{
	case MP_SKILL_SKILLOBJECT_ADD:
		{
			MSG_SKILLOBJECT_ADD* pmsg = (MSG_SKILLOBJECT_ADD*)pMsg;
			printf("%d\n", pmsg->SkillObjectInfo.SkillIdx);
			OnReceiveSkillObjectAdd(pmsg);
		}
		break;
	case MP_SKILL_SKILLOBJECT_REMOVE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			OnReceiveSkillObjectRemove(pmsg);
		}
		break;
	case MP_SKILL_START_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			OnReceiveSkillStartNack();
		}
		break;
	case MP_SKILL_SKILL_SINGLE_RESULT:
		{
			MSG_SKILL_SINGLE_RESULT* pmsg = (MSG_SKILL_SINGLE_RESULT*)pMsg;
			OnReceiveSkillSingleResult(pmsg);
		}
		break;

		// TargetList Update
	case MP_SKILL_ADDOBJECT_TO_SKILLOBJECTAREA_ACK:
		{
			MSG_DWORD2BYTE* pmsg = (MSG_DWORD2BYTE*)pMsg;
			OnReceiveSkillObjectTargetListAdd(pmsg->dwData1,pmsg->dwData2,pmsg->bData);
		}
		break;
	case MP_SKILL_REMOVEOBJECT_FROM_SKILLOBJECTAREA_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			OnReceiveSkillObjectTargetListRemove(pmsg->dwData1,pmsg->dwData2);
		}
		break;

	case MP_SKILL_STARTEFFECT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			OnReceiveSkillStartEffect(pmsg->dwData1,pmsg->dwData2);
		}
		break;

	case MP_SKILL_OPERATE_ACK:
		{
			MSG_SKILL_OPERATE* pmsg = (MSG_SKILL_OPERATE*)pMsg;
			OnReceiveSkillObjectOperate(pmsg);
		}
		break;
		
	case MP_SKILL_DELAY_NOTIFY:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			SKILLDELAYMGR->ContinueSkillDelay( pmsg->dwData1, pmsg->dwData2 );
		}
		break;
	case MP_SKILL_JOB_NACK:
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "½ºÅ³ ½ÇÆÐ.");
		}
		break;
	}
}

CSkillArea* CSkillManager::GetSkillArea(CObject* pObject, CActionTarget* pTarget, CSkillInfo* pSkillInfo)
{
	CSkillArea* pArea = GetSkillArea(pObject->GetDirectionIndex(),pSkillInfo->GetSkillAreaIndex());
	
	// AreaÀÇ Áß½ÉÁÂÇ¥±îÁö ¼ÂÆÃµÇ¾îÁ® ¿Â´Ù.
	VECTOR3* pPos;
	pPos = pSkillInfo->GetTargetAreaPivotPos(&pObject->GetCurPosition(),pTarget->GetTargetPosition());
	pArea->SetCenterPos(pPos);

	return pArea;	
}

CSkillArea* CSkillManager::GetSkillArea(DIRINDEX dir,WORD SkillAreaIndex)
{
	CSkillArea* pArea = m_SkillAreaMgr.GetSkillArea(SkillAreaIndex,dir);
	
	return pArea;
}

void CSkillManager::ReleaseSkillArea(CSkillArea* pSkillArea)
{
	m_SkillAreaMgr.ReleaseSkillArea(pSkillArea);
}

void CSkillManager::UpdateSkillObjectTargetList(CObject* pObject)
{
	CSkillObject* pSObj;
	DWORD rtCode;
	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		rtCode = pSObj->Update();		// ÀÚ±â°¡ ¾²°í ÀÖ´Â ½ºÅ³µé¿¡ ´ëÇÑ ¸ó½ºÅÍÀÇ ¾÷µ¥ÀÌÆ®
		pSObj->UpdateTargetList(pObject);	// ÁÖÀÎ°ø ¾÷µ¥ÀÌÆ®
#ifdef _TESTCLIENT_
		if(rtCode == SO_DESTROYOBJECT)
		{
			m_SkillObjectTable.Remove(pSObj->GetID());
			ReleaseSkillObject(pSObj);
		}
#endif
	}
}

void CSkillManager::RemoveTarget(CObject* pObject,BYTE bTargetKind)
{
	CSkillObject* pSObj;
	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		pSObj->RemoveTarget(pObject,bTargetKind);
	}
}

void CSkillManager::SkillStartEffect(CObject* pObject,CSkillInfo* pSkillInfo)
{
	TARGETSET set;
	set.pTarget = pObject;
	DWORD flag = 0;
	if(pObject->GetID() == HEROID)
		flag |= EFFECT_FLAG_HEROATTACK;
	EFFECTMGR->StartEffectProcess(pSkillInfo->GetSkillInfo()->EffectStart,pObject,
								&set,1,pObject->GetID(),flag);
}

BOOL CSkillManager::HeroSkillStartEffect(CHero* pHero,CActionTarget* pTarget,CSkillInfo* pSkillInfo)
{
	SkillStartEffect(pHero,pSkillInfo);
	OBJECTSTATEMGR->StartObjectState(pHero,eObjectState_SkillStart);
	OBJECTSTATEMGR->EndObjectState(pHero,eObjectState_SkillStart,pSkillInfo->GetSkillInfo()->EffectStartTime);
	CAction act;
	act.InitSkillActionRealExecute(pSkillInfo,pTarget);
	pHero->SetSkillStartAction(&act);

	// ¼­¹ö¿¡ ¸Þ¼¼Áö º¸³½´Ù.
	MSG_DWORD2 msg;
	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_STARTEFFECT;
	msg.dwData1 = pHero->GetID();
	msg.dwData2 = pSkillInfo->GetSkillIndex();
	NETWORK->Send(&msg,sizeof(msg));

	return TRUE;
}

void CSkillManager::MakeSAT()
{
	FILE* fp = fopen("SAT.txt","w");
	fprintf(fp,"%d\n",m_SkillInfoTable.GetDataNum());
	
	CEngineObject man,woman;
	man.Init("man.chx",NULL,eEngineObjectType_Character);
	woman.Init("woman.chx",NULL,eEngineObjectType_Character);
	CSkillInfo* pSkillInfo;
	m_SkillInfoTable.SetPositionHead();
	while(pSkillInfo = m_SkillInfoTable.GetData())
	{
		DWORD StateEndTimeMan = 0;
		DWORD StateEndTimeWoman = 0;
		BOOL bBinding = pSkillInfo->GetSkillInfo()->BindOperator != 0;
		if(!bBinding)
		{
			WORD effectuse = pSkillInfo->GetSkillInfo()->EffectUse;
			StateEndTimeMan = EFFECTMGR->GetOperatorAnimatioEndTime(effectuse,eEffectForMan,&man);
			StateEndTimeWoman = EFFECTMGR->GetOperatorAnimatioEndTime(effectuse,eEffectForWoman,&woman);
			if(StateEndTimeMan == 0)	StateEndTimeMan = 500;
			if(StateEndTimeWoman == 0)	StateEndTimeWoman = 500;			
		}

		fprintf(fp,"%d\t%d\t%d\n",pSkillInfo->GetSkillIndex(),StateEndTimeMan,StateEndTimeWoman);
	}

	man.Release();
	woman.Release();

	fclose(fp);
}

SKILL_CHANGE_INFO * CSkillManager::GetSkillChangeInfo(WORD wMugongIdx)
{
	PTRLISTSEARCHSTART(m_SkillChangeList, SKILL_CHANGE_INFO *, pInfo)
		if(pInfo->wMugongIdx == wMugongIdx)
			return pInfo;
	PTRLISTSEARCHEND
	return NULL;
}

BOOL CSkillManager::IsDeadlyMugong(WORD wMugongIdx)
{
	PTRLISTSEARCHSTART(m_SkillChangeList, SKILL_CHANGE_INFO *, pInfo)
		if(pInfo->wTargetMugongIdx == wMugongIdx)
			return TRUE;
	PTRLISTSEARCHEND
	return FALSE;
}

///////////////////////////////////////////////////////////////////
// 06. 04. ½ºÅ³ ¹ö±× ¼öÁ¤ - ÀÌ¿µÁØ
// ÀÓ½Ã½ºÅ³°´Ã¼¸¦ °­Á¦·Î Áö¿öÁÖ´Â ÇÔ¼ö
void CSkillManager::DeleteTempSkill()
{
	CSkillObject* pSObj = GetSkillObject(TEMP_SKILLOBJECT_ID);

	ReleaseSkillObject(pSObj);
}
///////////////////////////////////////////////////////////////////

////////// 2007. 6. 28. CBH - Àü¹®±â¼ú È®·ü °ü·Ã ÇÔ¼ö Ãß°¡ ////////////////////
BOOL CSkillManager::LoadJobSkillProbability()
{
	CMHFile file;
	if(!file.Init("Resource/jobskill.bin","rb"))
	{
		return FALSE;
	}

	JOB_SKILL_PROBABILITY_INFO* pJobSkillInfo;
	while(!file.IsEOF())
	{
		pJobSkillInfo = new JOB_SKILL_PROBABILITY_INFO;
		memset( pJobSkillInfo, 0, sizeof(JOB_SKILL_PROBABILITY_INFO) );

		pJobSkillInfo->wSkillLevel = file.GetWord();
		file.GetWord(pJobSkillInfo->ProbabilityArray, MAX_JOBMOB_NUM);

		ASSERT(!m_JobSkillProbabilityTable.GetData(pJobSkillInfo->wSkillLevel));

		m_JobSkillProbabilityTable.Add(pJobSkillInfo, pJobSkillInfo->wSkillLevel);
		pJobSkillInfo = NULL;
	}
	file.Release();

	return TRUE;
}

BOOL CSkillManager::IsJobSkill(CHero* pHero,CActionTarget* pTarget, CSkillInfo* pSkillInfo)
{	
	// Å¸°ÙÀÌ Àü¹®±â¼ú ¿ÀºêÁ§Æ®¸é ÀÏ¹Ý ½ºÅ³ ¸·´Â´Ù	
	// Àü¹®±â¼ú ·¹º§º¸´Ù ¿ÀºêÁ§Æ®ÀÇ ·¹º§ÀÌ ³ôÀ¸¸é ¸·´Â´Ù. (¸Þ¼¼Áö Ã³¸®)
	// Å¸°ÙÀÌ ÀÏ¹Ý ¸÷ÀÌ°í Àü¹®½ºÅ³ ½ÃÀü½Ã ½ÃÀü ¸øÇÏ°Ô ¸·´Â´Ù.
	CObject* pObject = OBJECTMGR->GetObject(pTarget->GetTargetID());

	if(pObject == NULL)
	{
		return FALSE;
	}
	
	WORD wSkillKind = pSkillInfo->GetSkillKind();
	//Å¸°ÙÀÌ ÇÃ·¹ÀÌ¾î ÀÏ¶§ ¹«°øÀÏ¶§ Ã³¸®
	if( pObject->GetObjectKind() == eObjectKind_Player )		
	{
		if(CheckSkillKind(wSkillKind) == TRUE)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "¥Í¬¡§Þ¯à¡A¤£¯à§ðÀ»¹ï¤è .");
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	//¼º¹®¿¡¼­ »¶³ª´Â ºÎºÐ Ã³¸®
	if(pObject->GetObjectKind() == eObjectKind_MapObject)
	{
		return TRUE;
	}

	WORD wObjectKind = pObject->GetObjectKind();	

	if(CheckSkillKind(wSkillKind) == TRUE)	
	{
		pHero->DisableAutoAttack(); //Àü¹® ½ºÅ³À» ¾²¸é ¹«Á¶°Ç ÀÚµ¿¾îÅÃ ±â´ÉÀ» ²ö´Ù.

		int nSkillLevel = pHero->GetMugongLevel(pSkillInfo->GetSkillIndex());

		if( GetObjectKindGroup(wObjectKind) == eOBJECTKINDGROUP_NONE )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "¥Í¬¡§Þ¯à¡A¤£¯à§ðÀ»¹ï¤è ");						
			return FALSE;
		}

		//Å¸ÀÌÅº Å¾½Â½Ã Àü¹®½ºÅ³ ¹ßµ¿ ºÒ°¡ Ã³¸®
		if(pHero->InTitan() == TRUE)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1657) );
			return FALSE;
		}

		//½ºÅ³Á¾·ù¿Í ¸ó½ºÅÍ Á¾·ù¿ÍÀÇ ºñ±³
		BOOL bJobSkillSuccess = FALSE;
		switch(wSkillKind)
		{
		case SKILLKIND_MINING:	// Ã¤±¤
			{			
				if( wObjectKind == eObjectKind_Mining )
				{
					bJobSkillSuccess = TRUE;
				}				
			}
			break;
		case SKILLKIND_COLLECTION:	// Ã¤Áý
			{
				if( wObjectKind == eObjectKind_Collection )
				{
					bJobSkillSuccess = TRUE;
				}				
			}
			break;
		case SKILLKIND_HUNT:	// »ç³É
			{
				if( wObjectKind == eObjectKind_Hunt )
				{
					bJobSkillSuccess = TRUE;
				}				
			}
			break;
		}

		if(bJobSkillSuccess == FALSE)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "¤£¬O¹ïÀ³ªº§Þ¯à ");								
			return FALSE;
		}

		CMonster* pMonster = (CMonster*)pObject;
		if(pMonster == NULL)	//¿¹¿ÜÃ³¸®
		{				
			return FALSE;
		}

		if(nSkillLevel < pMonster->GetMonsterLevel())
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "§Þ¯àµ¥¯Å¹L§C ");
			return FALSE;
		}		
	}
	else if( GetObjectKindGroup(wObjectKind) == eOBJECTKINDGROUP_JOB )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "¥Ø¼Ð¤£¯à¨Ï¥Î ");
		pHero->DisableAutoAttack(); //Àü¹® ½ºÅ³À» ¾²¸é ¹«Á¶°Ç ÀÚµ¿¾îÅÃ ±â´ÉÀ» ²ö´Ù.		
		return FALSE;
	}	
	
	return TRUE;
}

BOOL CSkillManager::CheckSkillKind(WORD wSkillKind)
{
	if((wSkillKind == SKILLKIND_MINING) || (wSkillKind == SKILLKIND_COLLECTION) || (wSkillKind == SKILLKIND_HUNT))	
	{
		return TRUE;
	}

	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////

///// 2007. 10. 15. CBH - Å¸ÀÌÅº ¹«°ø°ú ¹«±â Ã¼Å©(SkillInfo¿¡¼­ ÀÌµ¿) /////////////
BOOL CSkillManager::CheckTitanWeapon(CHero* pHero, SKILLINFO* SkillInfo)
{
	// magi82 - Titan(070912) Å¸ÀÌÅº ¹«°ø¾÷µ¥ÀÌÆ®
	// Å¸ÀÌÅº¿¡ Å¾½ÂÁßÀÏ¶§..
	if( pHero->InTitan() == TRUE )
	{
		// Å¸ÀÌÅº ¹«°øÀÌ¸é Å¸ÀÌÅº ¹«±â¸¦ Ã¼Å©ÇÑ´Ù.
		if( SkillInfo->SkillKind == SKILLKIND_TITAN )
		{
			if( SkillInfo->WeaponKind != 0 )
			{
				if( pHero->GetTitanWeaponEquipType() != SkillInfo->WeaponKind )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(151) );
					pHero->DisableAutoAttack();
					return FALSE;
				}

				if( pHero->GetWeaponEquipType() != pHero->GetTitanWeaponEquipType() )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1644));
					pHero->DisableAutoAttack();
					return FALSE;
				}
			}
		}
		else	// Å¸ÀÌÅº ¹«°øÀÌ ¾Æ´Ï¸é ¿¡·¯
		{
			if(SKILLMGR->CheckSkillKind(SkillInfo->SkillKind) == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1657) );
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1653) );
			}			
			return FALSE;
		}
	}
	else	// Å¸ÀÌÅº¿¡ Å¾½ÂÁßÀÌÁö ¾ÊÀ»¶§..
	{
		// Å¸ÀÌÅº ¹«°øÀÌ¸é ¿¡·¯
		if( SkillInfo->SkillKind == SKILLKIND_TITAN )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1668) );
			return FALSE;
		}
		else	// Å¸ÀÌÅº ¹«°øÀÌ ¾Æ´Ï¸é Ä³¸¯ÅÍ ¹«±â¸¦ Ã¼Å©ÇÑ´Ù.
		{
			if( SkillInfo->WeaponKind != 0 )
			{
				if( pHero->GetWeaponEquipType() != SkillInfo->WeaponKind )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(151) );
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////

#endif
