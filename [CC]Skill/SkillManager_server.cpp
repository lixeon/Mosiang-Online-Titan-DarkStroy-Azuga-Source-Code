// SkillManager.cpp: implementation of the CSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _MAPSERVER_

#include "SkillManager_server.h"

#include "UserTable.h"
#include "CharMove.h"

#include "Player.h"
#include "Monster.h"
#include "Pet.h"
#include "PackedData.h"

#include "BattleSystem_Server.h"
#include "MapDBMsgParser.h"
#include "MugongManager.h"
#include "ItemManager.h"

#include "ObjectStateManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

enum eSkillReleaseKind
{
	eSkillReleaseKind_RELEASE,
	eSkillReleaseKind_Process,
	eSkillReleaseKind_BATTLE,
};
CSkillManager::CSkillManager()
{
	g_PoolSTLIST.Init(1000,500,"g_PoolSTLIST");
	m_ICHandle = NULL;

	m_HwaKyungSkillTable.Initialize(30);
	m_GeukMaSkillTable.Initialize(30);

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	m_SkillOptionTable.Initialize(30);
	m_SkillOptionByItemTable.Initialize(30);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	m_SpecialStateInfoTable.Initialize(3);
	//////////////////////////////////////////////////////////////////////////	
}

CSkillManager::~CSkillManager()
{
	Release();
	g_PoolSTLIST.Release();
}


void CSkillManager::Init()
{
	m_SkillInfoTable.Initialize(2000);
	m_SkillObjectTable.Initialize(1000);
	LoadSkillInfoList();
	LoadSkillChangeInfoList();

	m_SkillAreaMgr.LoadSkillAreaList();
	
	m_ICHandle = ICCreate();
	ICInitialize(m_ICHandle,SKILLOBJECT_ID_MAX);

	m_JobSkillProbabilityTable.Initialize(MAX_JOBLEVEL_NUM);	// 2007. 6. 28. CBH - 전문기술 확률 테이블 초기화 /////////////////
	LoadJobSkillProbability();				// 2007. 6. 28. CBH - 전문기술 확률 리소스 로딩 /////////////////	
}
void CSkillManager::Release()
{
	CSkillObject* pObject;
	m_SkillObjectTable.SetPositionHead();
	while(pObject = m_SkillObjectTable.GetData())
	{
		ReleaseSkillObject(pObject,eSkillReleaseKind_RELEASE);
	}
	m_SkillObjectTable.RemoveAll();

	CSkillInfo* pSInfo = NULL;
	m_SkillInfoTable.SetPositionHead();
	while(pSInfo = m_SkillInfoTable.GetData())
	{
		delete pSInfo;
	}
	m_SkillInfoTable.RemoveAll();

	m_SkillAreaMgr.Release();
	
	if(m_ICHandle)
	{
		ICRelease(m_ICHandle);
		m_ICHandle = NULL;
	}
	
	PTRLISTSEARCHSTART(m_SkillChangeList,SKILL_CHANGE_INFO*,pInfo)
		delete pInfo;
	PTRLISTSEARCHEND
	m_SkillChangeList.RemoveAll();

	m_HwaKyungSkillTable.RemoveAll();
	m_GeukMaSkillTable.RemoveAll();

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
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
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	SPECIAL_STATE_INFO* pStateInfo = NULL;

	m_SpecialStateInfoTable.SetPositionHead();
	while(pStateInfo = m_SpecialStateInfoTable.GetData())
	{
		delete pStateInfo;
	}
	m_SpecialStateInfoTable.RemoveAll();
	//////////////////////////////////////////////////////////////////////////

	////// 2007. 6. 28. CBH - 전문기술 확률 리스트 삭제 /////////////////
	JOB_SKILL_PROBABILITY_INFO* pJobSkillInfo = NULL;

	m_JobSkillProbabilityTable.SetPositionHead();
	while(pJobSkillInfo = m_JobSkillProbabilityTable.GetData())
	{
		SAFE_DELETE(pJobSkillInfo);		
	}
	m_JobSkillProbabilityTable.RemoveAll();
	////////////////////////////////////////////////////////////////////////
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
		ASSERTMSG(0,"SkillList를 로드하지 못했습니다.");
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		CSkillInfo* pInfo = new CSkillInfo;
		pInfo->InitSkillInfo(&file);

		m_SkillInfoTable.Add(pInfo,pInfo->GetSkillIndex());
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

	// 화경, 극마 스킬리스트
	LoadJobSkillList();
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	LoadSkillOptionList();
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	LoadStateList();
	//////////////////////////////////////////////////////////////////////////
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
		ASSERTMSG(0,"SkillChangeList를 로드하지 못했습니다.");
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;
		SKILL_CHANGE_INFO * pInfo = new SKILL_CHANGE_INFO;
		pInfo->wMugongIdx = file.GetWord();
		pInfo->wChangeRate = file.GetWord();
		pInfo->wTargetMugongIdx = file.GetWord();

		m_SkillChangeList.AddTail(pInfo);
	}
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

//////////////////////////////////////////////////////////////////////////
// 06. 06. 2차 전직 - 이영준
// 무공 변환 추가
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
		ASSERTMSG(0,"SkillOptionList를 로드하지 못했습니다.");
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
// 06. 06. 2차 전직 - 이영준
// 은신/혜안
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
		ASSERTMSG(0,"StateList를 로드하지 못했습니다.");
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

DWORD CSkillManager::GetNewSkillObjectID()
{
	static tempID = SKILLOBJECT_ID_START;
	if(++tempID >= SKILLOBJECT_ID_MAX)
		tempID = SKILLOBJECT_ID_START;
	return tempID;

	//return SKILLOBJECT_ID_START + ICAllocIndex(m_ICHandle);
}
void CSkillManager::ReleaseSkillObjectID(DWORD SkillObjectID)
{
	return;
	
	//ICFreeIndex(m_ICHandle,SkillObjectID - SKILLOBJECT_ID_START);
}

void CSkillManager::OnSkillStartSyn(MSG_SKILL_START_SYN* pmsg)
{
	CSkillObject* pSObj;
	SKILLOBJECT_INFO info;
	VECTOR3 TargetPos;
	float TargetRadius;
	int SkillLevel = 1;
	CBattle* pBattle;
	CObject * pTargetObject = NULL;
	float skillTreeAmp;
	BYTE Err = 0;
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	WORD SkillOptionIndex = 0;
	SKILLOPTION* pSkillOption = NULL;
	//////////////////////////////////////////////////////////////////////////
	
	CObject* pOperator = g_pUserTable->FindUser(pmsg->Operator);

	/// 임시/////////
	if(pmsg->Operator > 1000000)
	{
		/*
		SYSTEMTIME systime;
		char szFile[256] = {0,};
		GetLocalTime( &systime );		
		sprintf( szFile, "./Log/Babo_%02d_%4d%02d%02d.txt", g_pServerSystem->GetMapNum(), systime.wYear, systime.wMonth, systime.wDay );		

		char Strtemp[256];
		FILE* fpstart = fopen(szFile,"a+");
		fprintf(fpstart, "[SkillSyn Object ID Over] - [OperatorID : %u] [SkillIndex : %d] [ObjectID : %u] [%2d:%02d:%02d]\n", pmsg->Operator, pmsg->SkillIdx, pmsg->dwObjectID, systime.wHour, systime.wMinute, systime.wSecond);
		fclose(fpstart);
		*/

		return;
	}
	////////////

	if(pOperator == NULL)
	{
		/*
		/// 임시/////////
		SYSTEMTIME systime;
		char szFile[256] = {0,};
		GetLocalTime( &systime );		
		sprintf( szFile, "./Log/Babo_%02d_%4d%02d%02d.txt", g_pServerSystem->GetMapNum(), systime.wYear, systime.wMonth, systime.wDay );		

		char Strtemp[256];
		FILE* fpstart = fopen(szFile,"a+");
		fprintf(fpstart, "[SkillSyn Object ID Error] - [OperatorID : %u] [SkillIndex : %d] [ObjectID : %u] [%2d:%02d:%02d]\n", pmsg->Operator, pmsg->SkillIdx, pmsg->dwObjectID, systime.wHour, systime.wMinute, systime.wSecond);
		fclose(fpstart);		
		////////////		
		*/

		return;
	}

	CSkillInfo* pSkillInfo = GetSkillInfo(pmsg->SkillIdx);
	ASSERT(pSkillInfo);
	if(pSkillInfo == NULL)
	{
		Err = eErr_SkillInfoNULL;
		goto SkillStartNack;
	}


	if(pOperator->GetObjectKind() == eObjectKind_Player)
	{
		//KES HACK CHECK
		//마지막에 사용한 스킬의 애니메이션 길이를 저장한다.
		//현재 시간을 저장한다.
		//마지막에 사용한 스킬의 에니메이션 길이 시간이 지났는지 검사한다.
		CPlayer* pPlayer = (CPlayer*)pOperator;

		DWORD dwLSAT = pPlayer->GetLastSkillAnimationTime();

		BOOL bHack = FALSE;
		if( gCurTime + 2000 < pPlayer->GetSkillDelayStartTime() + dwLSAT )	//2초까지는 렉을 고려하여 봐준다.
		{
			pPlayer->SpeedHackCheck();
			bHack = TRUE;
		}

		pPlayer->SetSkillDelayStartTime( gCurTime );
		//////////////////////////////////////////////////
		// 06. 07. SAT 변경 - 이영준
		// 남여 구분의 의미가 없으므로
		// 여케릭 데이터를 활용 무초 딜레이 감소에 쓴다
		//int nAT = pSkillInfo->GetSkillOperateAnimationTime( pPlayer->GetGender() );
		int nAT = pSkillInfo->GetSkillOperateAnimationTime( 0 );
		//////////////////////////////////////////////////
		pPlayer->SetLastSkillAnimationTime( nAT );

		if( bHack )
		{
			Err = eErr_NotExcutableSkillState;
			goto SkillStartNack;
		}

		//-----------------
		
		//if( pPlayer->InTitan() && pmsg->SkillIdx > SKILLNUM_TO_TITAN )
		//	SkillLevel = pPlayer->GetMugongLevel(pmsg->SkillIdx - SKILLNUM_TO_TITAN);
		//else
			SkillLevel = pPlayer->GetMugongLevel(pmsg->SkillIdx);
		if(SkillLevel == 0 || SkillLevel == -1)	// 무공이 없는 경우이거나 0성인 경우
		{
			Err = eErr_SkillLvl0;
			goto SkillStartNack;
		}

		//////////////////////////////////////////////////////////////////////////
		// 06. 06. 2차 전직 - 이영준
		// 무공 변환 추가	
		SkillOptionIndex = pPlayer->GetSkillOptionIndex(pmsg->SkillIdx);

		if(SkillOptionIndex)
			pSkillOption = m_SkillOptionTable.GetData(SkillOptionIndex);
		//////////////////////////////////////////////////////////////////////////

		// RaMa - 화경, 극마 체크 추가
		if( pPlayer->GetStage() & eStage_Hwa )
		{
			if( m_GeukMaSkillTable.GetData( pSkillInfo->GetSkillIndex() ) )
			{
				Err = eErr_NotExcutableSkillState;
				goto SkillStartNack;
			}
		}
		else if( pPlayer->GetStage() & eStage_Geuk )
		{
			if( m_HwaKyungSkillTable.GetData( pSkillInfo->GetSkillIndex() ) )
			{
				Err = eErr_NotExcutableSkillState;
				goto SkillStartNack;
			}
		}
		if( pPlayer->GetWeaponEquipType() == WP_EVENT )
		{
			if( pmsg->SkillIdx != COMBO_EVENT_MIN )
				goto SkillStartNack;
		}
		else if( pPlayer->GetWeaponEquipType() == WP_EVENT_HAMMER )
		{
			if( pmsg->SkillIdx != COMBO_EVENT_HAMMER )
				goto SkillStartNack;
		}

		/*
		// 2005 크리스마스 이벤트
		if( pPlayer->GetWeaponEquipType() == WP_EVENT )
		{
			if( pTargetObject->GetObjectKind() == eObjectKind_Monster )
			{
				CMonster *pMonster = (CMonster*)pTargetObject;
				
				if( pMonster->GetMonsterKind() != EVENT_MONSTER_SANTA )
				{
					goto SkillStartNack;
				}
			}
		}
		*/

		// magi82 - Titan(070912) 타이탄 무공업데이트
		// 이제 타이탄 무기와 캐릭터의 무기는 별개이다.(무기가 서로 다르다고 해서 스킬이 안나가는게 아님)
		//if(pPlayer->InTitan())
		//{
		//	WORD weapon = pPlayer->GetWeaponEquipType();
		//	WORD titanweapon = pPlayer->GetTitanWeaponEquipType();
		//	if(weapon != titanweapon)
		//	{
		//		goto SkillStartNack;
		//	}
		//}
	}	

//KES
	if( pOperator->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pOperator, eObjectState_Immortal, 0 );
//	
	
	// RaMa - BattleCheck
	if( pOperator->GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT ||
		pOperator->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{		
		if( pOperator->GetBattleTeam() == 2 )			// Oberver
			goto SkillStartNack;
	}
	//

	if(pSkillInfo->IsExcutableSkillState(pOperator,SkillLevel, pSkillOption) == FALSE)
	{
		Err = eErr_NotExcutableSkillState;
		goto SkillStartNack;
	}

	if(CheckTitanWeapon(pOperator, pSkillInfo->GetSkillInfo()) == FALSE)
	{
		Err = eErr_NotExcutableSkillState;
		goto SkillStartNack;
	}

	if(GetMainTargetPos(&pmsg->MainTarget,&TargetPos, &pTargetObject) == FALSE)	// 타겟이 없다.
	{
		Err = eErr_NoTarget;
		goto SkillStartNack;
	}

	//KES
	//타겟팅 방식이 상대방인데 자신을 타겟으로 정했다.
	if( pSkillInfo->GetSkillInfo()->TargetKind == 0 && pTargetObject == pOperator )
	{
		Err = eErr_NoTarget;
		goto SkillStartNack;
	}
	
	GetMainTargetRadius(&pmsg->MainTarget,&TargetRadius);

	if(pSkillInfo->ComboTest(pOperator) == FALSE)
	{
		Err = eErr_ComboTest;
		goto SkillStartNack;
	}

	// 2007. 7. 3. CBH - 전문스킬발동시 몬스터와의 관계 처리 함수 추가	
	CObject* pTarget = g_pUserTable->FindUser(pmsg->MainTarget.dwMainTargetID);
	if(pTarget != NULL)
	{
		if(!IsJobSkill((CPlayer*)pOperator, pTarget, pSkillInfo))
		{
			Err = eErr_NotExcutableSkillState;
			goto SkillStartNack;
		}	
	}	

	if(pSkillInfo->IsInSkillRange(pOperator,&TargetPos,TargetRadius, pSkillOption) == FALSE)
	{
		if(pTargetObject)
		{
			RESULTINFO DamageInfo;
			DamageInfo.Clear();
			pTargetObject->Damage(pOperator, &DamageInfo);
		}
		Err = eErr_NotInRange;
		goto SkillStartNack;
	}
	
	
	if(pSkillInfo->CheckEvade(&pmsg->MainTarget) == TRUE)
	{
		Err = eErr_CheckEvade;
		goto SkillStartNack;
	}
	
	//스킬 생성 전에 절초 획득
	MUGONGBASE* pCurMugong;	
	pCurMugong = ((CPlayer*)pOperator)->GetMugongBaseByMugongIdx(pmsg->SkillIdx);
	if(pCurMugong)
	if( pCurMugong->Sung == 12 )
	{
		// 절초가 있는 무공이면
		DWORD RandRate;
		PTRLISTSEARCHSTART(m_SkillChangeList,SKILL_CHANGE_INFO*,pChangeInfo)
			if(pChangeInfo->wMugongIdx == pmsg->SkillIdx)
			{
#ifdef _CHINA_LOCAL_
				RandRate = random(0, 15000);	//중국의 경우 절초 확률 1/15000
#else
				RandRate = random(0, 10000);
#endif
//				if( g_pServerSystem->GetNation() == eNATION_CHINA )
//					RandRate = random(0, 15000);	//중국의 경우 절초 확률 1/15000
//				else
//					RandRate = random(0, 10000);
				
				if(0 <= RandRate && RandRate < pChangeInfo->wChangeRate)
				{
					MUGONGMNGR->ChangeMugong((CPlayer*)pOperator, pmsg->SkillIdx, pCurMugong->Position, pChangeInfo->wTargetMugongIdx, eLog_MugongDestroyByGetJulCho);
				}
				break;
			}
		PTRLISTSEARCHEND
	}

	// 스킬 오브젝트 생성
	pSObj = pSkillInfo->GetSkillObject();

	info.SkillIdx = pmsg->SkillIdx;
	info.SkillObjectIdx = GetNewSkillObjectID();
	info.Direction = pmsg->SkillDir;
	info.Operator = pmsg->Operator;
	info.SkillLevel = SkillLevel;

	info.MainTarget = pmsg->MainTarget;

	info.Pos = *pSkillInfo->GetTargetAreaPivotPos(CCharMove::GetPosition(pOperator),&TargetPos);
	info.StartTime = gCurTime;

	// battle
	info.BattleID = pOperator->GetBattleID();
	pBattle = pOperator->GetBattle();
	info.BattleKind = pBattle->GetBattleKind();

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	info.Option = SkillOptionIndex;
	//////////////////////////////////////////////////////////////////////////

	if(pOperator->GetObjectKind() == eObjectKind_Player)
	{
		if(((CPlayer*)pOperator)->IsSkipSkill())
		{
			((CPlayer*)pOperator)->GetDelayGroup()->AddDelay(
				CDelayGroup::eDK_Skill,pSkillInfo->GetSkillIndex(),
				pSkillInfo->GetSkillInfo()->DelayTime - pSkillInfo->GetSkillOperateAnimationTime( 1 ));
		}
		else
		{
			((CPlayer*)pOperator)->GetDelayGroup()->AddDelay(
				CDelayGroup::eDK_Skill,pSkillInfo->GetSkillIndex(),
				pSkillInfo->GetSkillInfo()->DelayTime);
		}
	}
	skillTreeAmp = GetSkillTreeAmp((CPlayer*)pOperator,pSkillInfo);

	pSObj->Init(&info,&pmsg->TargetList,skillTreeAmp,pTargetObject);
	pSObj->SetBattle(pOperator->GetBattleID(),pOperator->GetBattleTeam());
	pSObj->SetGridID(pOperator->GetGridID());

	// 테이블에 등록
	m_SkillObjectTable.Add(pSObj,pSObj->GetID());
	g_pServerSystem->AddSkillObject(pSObj,&info.Pos);

	//SW051129 Pet
	if(pOperator->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pPlayer = (CPlayer*)pOperator;
		CPet* pPet = (CPet*)pPlayer->GetCurPet();
		if( pPet )
		{
			pPet->GetRandMotionNSpeech(ePM_MASTER_SKILL);
		}
	}


#ifdef _KOR_LOCAL_
	if(pOperator->GetObjectKind() == eObjectKind_Player)
		((CPlayer*)pOperator)->SetActionTime();
#endif

	// autonote
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		if( pTargetObject )
        if( pTargetObject->GetObjectKind() & eObjectKind_Monster )		//---타겟이 몬스터일 경우만
			((CPlayer*)pOperator)->SetLastAttackTimeForAutoNote( gCurTime );		
	}
	//


	return;

SkillStartNack:
	if(pOperator->GetObjectKind() != eObjectKind_Player)
		return;

	// for delete AmplifiedPower
	pOperator->GetSpecialStateParam1(eSpecialState_AmplifiedPowerPhy);

	CPlayer* pp = (CPlayer*)pOperator;
	MSG_BYTE msg;
	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_START_NACK;
	msg.dwObjectID = pp->GetID();
	msg.bData = Err;
	pp->SendMsg(&msg,sizeof(msg));	

	return;
}

void CSkillManager::OnAddObjectToSkillObjectArea(DWORD SkillObjectID,DWORD ObjectID)
{
	CSkillObject* pSObj = GetSkillObject(SkillObjectID);
	if(pSObj == NULL)
		return;

	CObject* pObject = g_pUserTable->FindUser(ObjectID);
	if(pObject == NULL)
		return;

	pSObj->AddTargetObject(pObject);
}

void CSkillManager::OnRemoveObjectToSkillObjectArea(DWORD SkillObjectID,DWORD ObjectID)
{
	CSkillObject* pSObj = GetSkillObject(SkillObjectID);
	if(pSObj == NULL)
		return;

	pSObj->RemoveTargetObject(ObjectID);
}

CSkillObject* CSkillManager::GetSkillObject(DWORD SkillObjID)
{
	return (CSkillObject*)m_SkillObjectTable.GetData(SkillObjID);
}
CSkillInfo* CSkillManager::GetSkillInfo(WORD SkillInfoIdx)
{
	return (CSkillInfo*)m_SkillInfoTable.GetData(SkillInfoIdx);
}

void CSkillManager::ReleaseSkillObject(CSkillObject* pSkillObject,int ReleaseKind)
{
	CSkillInfo* pSInfo = GetSkillInfo(pSkillObject->GetSkillIdx());
//	ASSERT(pSInfo);
	if(!pSInfo)
	{
		ASSERTMSG(0,"삑!!!!!! 중요한곳 이거 보면 윤호한테 꼭 연락바람!!!");

		CSkillObject* ptemp = m_SkillObjectTable.GetData(pSkillObject->GetID());
        if( ptemp == pSkillObject )
		{
			m_SkillObjectTable.Remove(pSkillObject->GetID());
			g_pServerSystem->RemoveSkillObject(pSkillObject->GetID());
            ReleaseSkillObjectID(pSkillObject->GetID());
		}
		return;
	}

	/// 2007 .7 .10. CBH - 전문기술 오브젝트 드롭 관련 처리 추가
	pSkillObject->SkillObjectFirstUnitResult();

	m_SkillObjectTable.Remove(pSkillObject->GetID());
	g_pServerSystem->RemoveSkillObject(pSkillObject->GetID());
	ReleaseSkillObjectID(pSkillObject->GetID());

	pSInfo->ReleaseSkillObject(pSkillObject,ReleaseKind);	
}

void CSkillManager::OnSkillStartEffect(MSG_DWORD2* pmsg)
{
	CObject* pObject = g_pUserTable->FindUser(pmsg->dwData1);
	if(pObject == NULL)
		return;
	
#ifdef _KOR_LOCAL_
	if( pObject->GetObjectKind() == eObjectKind_Player )
		((CPlayer*)pObject)->SetActionTime();
#endif

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pObject,pmsg,sizeof(MSG_DWORD2));
}

void CSkillManager::OnSkillOperateSyn(MSG_SKILL_OPERATE* pmsg)
{
	CSkillObject* pSObj = GetSkillObject(pmsg->SkillObjectID);
	if(pSObj == NULL)
		return;

	CObject* pReqObj = g_pUserTable->FindUser(pmsg->RequestorID);
	if(pReqObj == NULL)
		return;
		
	BOOL rt = pSObj->Operate(pReqObj,&pmsg->MainTarget,&pmsg->TargetList);
	ASSERT(rt);
}

void CSkillManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_SKILL_START_SYN:
		{
			MSG_SKILL_START_SYN* pmsg = (MSG_SKILL_START_SYN*)pMsg;
			OnSkillStartSyn(pmsg);
		}
		break;
	case MP_SKILL_ADDOBJECT_TO_SKILLOBJECTAREA_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			OnAddObjectToSkillObjectArea(pmsg->dwData1,pmsg->dwData2);
		}
		break;
	case MP_SKILL_REMOVEOBJECT_FROM_SKILLOBJECTAREA_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			OnRemoveObjectToSkillObjectArea(pmsg->dwData1,pmsg->dwData2);
		}
		break;
	case MP_SKILL_STARTEFFECT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			OnSkillStartEffect(pmsg);			
		}
		break;
	case MP_SKILL_OPERATE_SYN:
		{
			MSG_SKILL_OPERATE* pmsg = (MSG_SKILL_OPERATE*)pMsg;
			OnSkillOperateSyn(pmsg);
		}
		break;
	}
}



void CSkillManager::Process()
{
	CSkillObject* pSObj;
	DWORD rtCode;
	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		rtCode = pSObj->Update();
		if(rtCode == SO_DESTROYOBJECT)
		{
			ReleaseSkillObject(pSObj,eSkillReleaseKind_Process);
		}		
	}
}

void CSkillManager::GetMainTargetRadius(MAINTARGET* pMainTarget,float* pRtRadius)
{
	if(pMainTarget->MainTargetKind == MAINTARGET::MAINTARGETKIND_POS)
		*pRtRadius = 0;
	else
	{
		CObject* pTarget = g_pUserTable->FindUser(pMainTarget->dwMainTargetID);
		*pRtRadius = pTarget->GetRadius();
	}	
}

BOOL gTestMonsterAttack = TRUE;
void CSkillManager::MonsterAttack(WORD SkillIdx,CObject* pMonster,CObject* pTarget)
{
	if(gTestMonsterAttack == FALSE)
		return;

	CBattle* pBattle;
	CSkillInfo* pSkillInfo = GetSkillInfo(SkillIdx);
	ASSERT(pSkillInfo);
	if(pSkillInfo == NULL)
		return;
	
	VECTOR3* pMonPos = CCharMove::GetPosition(pMonster);
	VECTOR3* pTargetPos = CCharMove::GetPosition(pTarget);
	VECTOR3 dir = *pTargetPos - *pMonPos;
	DIRINDEX diridx = RADTODIR(VECTORTORAD(dir));


	// 스킬 오브젝트 생성
	SKILLOBJECT_INFO info;
	CSkillObject* pSObj = pSkillInfo->GetSkillObject();

	info.SkillIdx = SkillIdx;
	info.SkillObjectIdx = GetNewSkillObjectID();
	info.Direction = diridx;
	info.Operator = pMonster->GetID();
	info.MainTarget.MainTargetKind = MAINTARGET::MAINTARGETKIND_OBJECTID;
	info.MainTarget.dwMainTargetID = pTarget->GetID();

	info.Pos = *pSkillInfo->GetTargetAreaPivotPos(pMonPos,pTargetPos);
	info.StartTime = gCurTime;
	info.SkillLevel = 1;

	
	// battle
	info.BattleID = pMonster->GetBattleID();
	pBattle = pMonster->GetBattle();
	info.BattleKind = pBattle->GetBattleKind();



	CTargetList TList;
	TList.Clear();
	CTargetListIterator iter(&TList);
	iter.AddTargetWithNoData(pTarget->GetID(),SKILLRESULTKIND_NEGATIVE);
	iter.Release();

	// 테이블에 등록
	pSObj->Init(&info,&TList);
	pSObj->SetBattle(pMonster->GetBattleID(),pMonster->GetBattleTeam());
	pSObj->SetGridID(pMonster->GetGridID());

	
	m_SkillObjectTable.Add(pSObj,pSObj->GetID());
	g_pServerSystem->AddSkillObject(pSObj,&info.Pos);
}

void CSkillManager::MonsterAttack(WORD SkillIdx,CObject* pMonster,VECTOR3* pTargetPos, DWORD TargetID)
{
	if(gTestMonsterAttack == FALSE)
		return;

	CBattle* pBattle;
	CSkillInfo* pSkillInfo = GetSkillInfo(SkillIdx);
	ASSERT(pSkillInfo);
	if(pSkillInfo == NULL)
		return;
	
	VECTOR3* pMonPos = CCharMove::GetPosition(pMonster);
	VECTOR3 dir = *pTargetPos - *pMonPos;
	DIRINDEX diridx = RADTODIR(VECTORTORAD(dir));


	// 스킬 오브젝트 생성
	SKILLOBJECT_INFO info;
	CSkillObject* pSObj = pSkillInfo->GetSkillObject();

	info.SkillIdx = SkillIdx;
	info.SkillObjectIdx = GetNewSkillObjectID();
	info.Direction = diridx;
	info.Operator = pMonster->GetID();
	info.MainTarget.MainTargetKind = MAINTARGET::MAINTARGETKIND_OBJECTID;
	info.MainTarget.dwMainTargetID = TargetID;

	info.Pos = *pSkillInfo->GetTargetAreaPivotPos(pMonPos,pTargetPos);
	info.StartTime = gCurTime;
	info.SkillLevel = 1;

	
	// battle
	info.BattleID = pMonster->GetBattleID();
	pBattle = pMonster->GetBattle();
	info.BattleKind = pBattle->GetBattleKind();
	
	
	CTargetList tlist;

	WORD Radius = pSkillInfo->GetSkillInfo()->TargetRange;
	WORD AreaNum = pSkillInfo->GetSkillInfo()->TargetAreaIdx;

	/////////////////////////////////////////////////////
	/// 06. 08. 2차 보스 - 이영준
	/// 안전거리 
	/// 이 거리 안쪽은 타겟으로 지정되지 않는다
	WORD SafeRange = pSkillInfo->GetSafeRange();
	/////////////////////////////////////////////////////
	
	if(AreaNum != 0)
	{
		CSkillArea* pSkillArea = GetSkillArea(pMonPos,pTargetPos,pSkillInfo);	// Area의 중심좌표까지 셋팅되어져 온다.
		g_pUserTable->GetTargetInArea(pSkillArea,&tlist);
	}
	else if(Radius != 0)
	{
		/////////////////////////////////////////////////////
		/// 06. 08. 2차 보스 - 이영준
		/// 안전거리 
		///	기존 GetTargetInRange 함수에 안전거리를 인자로 마지막에 추가했다
		g_pUserTable->GetTargetInRange(pSkillInfo->GetTargetAreaPivotPos(pMonPos,pTargetPos),Radius,&tlist,SafeRange);
		/////////////////////////////////////////////////////
	}
	
	
	// 테이블에 등록
	pSObj->Init(&info,&tlist);
	pSObj->SetBattle(pMonster->GetBattleID(),pMonster->GetBattleTeam());
	pSObj->SetGridID(pMonster->GetGridID());
	
	
	m_SkillObjectTable.Add(pSObj,pSObj->GetID());
	g_pServerSystem->AddSkillObject(pSObj,&info.Pos);
}

void CSkillManager::PetOperSkill(WORD SkillIdx,CPet* pPet,CObject* pTarget)
{
	CBattle* pBattle;
	CSkillInfo* pSkillInfo = GetSkillInfo(SkillIdx);
	ASSERT(pSkillInfo);
	if( NULL == pSkillInfo )
		return;

	VECTOR3* pPetPos = CCharMove::GetPosition(pPet);
	VECTOR3* pTargetPos = CCharMove::GetPosition(pTarget);
	VECTOR3 dir = *pTargetPos - *pPetPos;
	DIRINDEX diridx = RADTODIR(VECTORTORAD(dir));

	// 스킬 오브젝트 생성
	SKILLOBJECT_INFO info;
	CSkillObject* pSObj = pSkillInfo->GetSkillObject();

	info.SkillIdx = SkillIdx;
	info.SkillObjectIdx = GetNewSkillObjectID();
	info.Direction = diridx;
	info.Operator = pPet->GetID();
	info.MainTarget.dwMainTargetID = pTarget->GetID();

	info.Pos = *pSkillInfo->GetTargetAreaPivotPos(pPetPos,pTargetPos);
	info.StartTime = gCurTime;
	info.SkillLevel = (BYTE)pPet->GetPetCurGrade();

	// battle
	info.BattleID = pPet->GetBattleID();
	pBattle = pPet->GetBattle();
	info.BattleKind = pBattle->GetBattleKind();

	CTargetList TList;
	TList.Clear();
	CTargetListIterator iter(&TList);
	iter.AddTargetWithNoData(pTarget->GetID(),SKILLRESULTKIND_NEGATIVE);
	iter.Release();

	// 테이블 등록
	pSObj->Init(&info,&TList);
	pSObj->SetBattle(pPet->GetBattleID(),pPet->GetBattleTeam());
	pSObj->SetGridID(pPet->GetGridID());

	m_SkillObjectTable.Add(pSObj,pSObj->GetID());
	g_pServerSystem->AddSkillObject(pSObj,&info.Pos);
}

CSkillArea* CSkillManager::GetSkillArea(VECTOR3* pObjPos, VECTOR3* pTargetPos, CSkillInfo* pSkillInfo)
{
	VECTOR3 DirVec = *pTargetPos - *pObjPos;
	float fRad = VECTORTORAD(DirVec);
	DIRINDEX dir = RADTODIR(fRad);
	CSkillArea* pArea = GetSkillArea(dir,pSkillInfo->GetSkillAreaIndex());
	
	// Area의 중심좌표까지 셋팅되어져 온다.
	VECTOR3* pPos;
	pPos = pSkillInfo->GetTargetAreaPivotPos(pObjPos,pTargetPos);
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

void CSkillManager::ReleaseAllSkillInBattle(DWORD BattleID)
{
	CSkillObject* pSObj;
	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		if(pSObj->GetBattleID() == BattleID)
			ReleaseSkillObject(pSObj,eSkillReleaseKind_BATTLE);
	}	
}

float CSkillManager::GetSkillTreeAmp(CPlayer* pPlayer,CSkillInfo* pSkillInfo)
{
	if(pSkillInfo->GetBeforeSkill() == 0)
		return 1.f;
	
	if(pPlayer->GetMugongLevel(pSkillInfo->GetBeforeSkill()) == 12)
		return 1.2f;

	return 1.f;
}

SKILL_CHANGE_INFO * CSkillManager::GetSkillChangeInfo(WORD wMugongIdx)
{
	PTRLISTSEARCHSTART(m_SkillChangeList, SKILL_CHANGE_INFO *, pInfo)
		if(pInfo->wMugongIdx == wMugongIdx)
			return pInfo;
	PTRLISTSEARCHEND
	return NULL;
}

SKILL_CHANGE_INFO * CSkillManager::GetSkillChangeInfobyDeadlyMugongIdx(WORD wDeadlyMugongIdx)
{
	PTRLISTSEARCHSTART(m_SkillChangeList, SKILL_CHANGE_INFO *, pInfo)
		if(pInfo->wTargetMugongIdx == wDeadlyMugongIdx)
			return pInfo;
	PTRLISTSEARCHEND
	return NULL;
}

////////// 2007. 6. 28. CBH - 전문기술 관련 함수 추가 ////////////////////
BOOL CSkillManager::LoadJobSkillProbability()
{
	CMHFile file;
	
	if(!file.Init("Resource/jobskill.bin", "rb"))
		return FALSE;

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

BOOL CSkillManager::IsJobSkill(CPlayer* pPlayer, CObject* pTarget, CSkillInfo* pSkillInfo)
{	
	// 타겟이 전문기술 오브젝트면 일반 스킬 막는다	
	// 전문기술 레벨보다 오브젝트의 레벨이 높으면 막는다. (메세지 처리)
	// 타겟이 일반 몹이고 전문스킬 시전시 시전 못하게 막는다.
	WORD wSkillKind = pSkillInfo->GetSkillKind();

	if( pTarget->GetObjectKind() == eObjectKind_Player )
	{
		if(CheckSkillKind(wSkillKind) == TRUE)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}	
	
	//성문에서 뻑나는 부분 처리
	if(pTarget->GetObjectKind() == eObjectKind_MapObject)
	{
		return TRUE;
	}
	
	WORD wObjectKind = pTarget->GetObjectKind();	

	if(CheckSkillKind(wSkillKind) == TRUE)	
	{		
		int nSkillLevel = pPlayer->GetMugongLevel(pSkillInfo->GetSkillIndex());		

		if( GetObjectKindGroup(wObjectKind) == eOBJECTKINDGROUP_NONE )
		{				
			return FALSE;
		}

		//타이탄 탑승시 전문스킬 발동 불가 처리
		if(pPlayer->InTitan() == TRUE)
		{			
			return FALSE;
		}

		//스킬종류와 몬스터 종류와의 비교		
		BOOL bJobSkillSuccess = FALSE;
		switch(wSkillKind)
		{
		case SKILLKIND_MINING:	// 채광
			{			
				if( wObjectKind == eObjectKind_Mining )
				{
					bJobSkillSuccess = TRUE;
				}				
			}
			break;
		case SKILLKIND_COLLECTION:	// 채집
			{
				if( wObjectKind == eObjectKind_Collection )
				{
					bJobSkillSuccess = TRUE;
				}				
			}
			break;
		case SKILLKIND_HUNT:	// 사냥
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
			return FALSE;
		}

		CMonster* pMonster = (CMonster*)pTarget;
		if(pMonster == NULL)
		{
			return FALSE;
		}
		if(nSkillLevel < pMonster->GetLevel())
		{		
			return FALSE;
		}
	}
	else if( GetObjectKindGroup(wObjectKind) == eOBJECTKINDGROUP_JOB )
	{			
		return FALSE;
	}	

	return TRUE;
}

WORD CSkillManager::GetJobSkillProbability(WORD wSkillLevel, WORD wTargetLevel)
{
	WORD wProbability = 0;

	m_JobSkillProbabilityTable.SetPositionHead();
	
	JOB_SKILL_PROBABILITY_INFO* pJobSkillInfo = NULL;	
	pJobSkillInfo = m_JobSkillProbabilityTable.GetData(wSkillLevel);
	
	if(pJobSkillInfo == NULL)
	{	
		return wProbability;
	}

	for(WORD i = 0 ; i < MAX_JOBMOB_NUM ; i++)
	{
		if(i+1 == wTargetLevel) //레벨은 1부터 시작 i값은 배열 인덱스....
		{
			wProbability = pJobSkillInfo->ProbabilityArray[i];
			return wProbability;
		}
	}	
			
	return wProbability;
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

///// 2007. 10. 15. CBH - 타이탄 무공과 무기 체크(SkillInfo에서 이동) /////////////
BOOL CSkillManager::CheckTitanWeapon(CObject* pObject, SKILLINFO* SkillInfo)
{
	// magi82 - Titan(070912) 타이탄 무공업데이트
	if(((CPlayer*)pObject)->InTitan())	// 타이탄에 탑승중일때..
	{
		// 타이탄 무공이면 타이탄 무기를 체크한다.
		if(SkillInfo->SkillKind == SKILLKIND_TITAN)
		{
			if( SkillInfo->WeaponKind != 0 )
			{
				if( ((CPlayer*)pObject)->GetTitanWeaponEquipType() != SkillInfo->WeaponKind )
					return FALSE;

				if( ((CPlayer*)pObject)->GetTitanWeaponEquipType() != ((CPlayer*)pObject)->GetWeaponEquipType() )
					return FALSE;
			}
		}
		else	// 타이탄 무공이 아니면 에러
		{
			return FALSE;
		}
	}
	else	// 타이탄에 탑승중이지 않을때..
	{
		// 타이탄 무공이면 에러
		if(SkillInfo->SkillKind == SKILLKIND_TITAN)
		{
			return FALSE;
		}
		else	// 타이탄 무공이 아니면 캐릭터 무기를 체크한다.
		{
			if( SkillInfo->WeaponKind != 0 )
			{
				if( ((CPlayer*)pObject)->GetWeaponEquipType() != SkillInfo->WeaponKind )
					return FALSE;
			}
		}
	}

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////

#endif
