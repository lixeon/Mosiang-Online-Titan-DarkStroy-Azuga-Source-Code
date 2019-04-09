// SkillObject.cpp: implementation of the CSkillObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _MAPSERVER_

#include "SkillObject_server.h"
#include "SkillInfo.h"
#include "SkillObjectTerminator.h"
#include "SkillObjectTargetList.h"
#include "SkillObjectFirstUnit.h"
#include "SkillObjectStateUnit.h"
#include "CharMove.h"

#include "UserTable.h"
#include "ServerSystem.h"
#include "PackedData.h"
#include "Player.h"

#include "BattleSystem_Server.h"
#include "AbilityManager.h"
#include "Monster.h"
#include "SkillManager_server.h"
#include "CharacterCalcManager.h"
#include "GuildManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObject::CSkillObject( CSkillInfo* pSkillInfo,
							CSkillObjectTerminator* pTerminator,
							CSkillObjectTargetList* pTargetList,
							CSkillObjectFirstUnit* pFirstUnit)
{
	m_pSkillInfo = pSkillInfo;
	m_pTerminator = pTerminator;
	m_pTargetList = pTargetList;
	m_pTargetList->SetSkillObject(this);
	m_pFirstUnit = pFirstUnit;
	m_PositiveTargetType = pSkillInfo->GetPositiveResultTargetType();
	m_NegativeTargetType = pSkillInfo->GetNegativeResultTargetType();
	
	m_ObjectKind = eObjectKind_SkillObject;

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	m_OptionIndex = 0;
	//////////////////////////////////////////////////////////////////////////
}

CSkillObject::~CSkillObject()
{
	Release();

	SAFE_DELETE(m_pTerminator);
	SAFE_DELETE(m_pTargetList);

	CSkillObjectSingleUnit* pSingleUnit;
	while(pSingleUnit = (CSkillObjectSingleUnit*)m_SingleUnitList.RemoveTail())
	{
		delete pSingleUnit;
	}

	CSkillObjectStateUnit* pStateUnit;
	while(pStateUnit = (CSkillObjectStateUnit*)m_StateUnitList.RemoveTail())
	{
		delete pStateUnit;
	}

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	m_OptionIndex = 0;
	//////////////////////////////////////////////////////////////////////////
}

void CSkillObject::Init(SKILLOBJECT_INFO* pInfo,CTargetList* pTList,float skillTreeAmp,CObject* pMainTarget)
{
	m_bDieFlag = FALSE;
	m_SkillTreeAmp = skillTreeAmp;

	m_pTerminator->Init(m_pSkillInfo,pInfo->SkillLevel);

	// 생명력 설정
	m_MaxLife = m_CurLife = DWORD(m_pSkillInfo->GetSkillInfo()->Life * m_SkillTreeAmp);
	
	m_BaseObjectInfo.dwObjectID = pInfo->SkillObjectIdx;
	strcpy(m_BaseObjectInfo.ObjectName,"SkillObject");
	m_BaseObjectInfo.ObjectState = eObjectState_None;

	m_pOperator = g_pUserTable->FindUser(pInfo->Operator);
//	ASSERT(m_pOperator);
	if( m_pOperator == NULL ) return;
	
	WORD ResultKind = SKILLRESULTKIND_NONE;
	// SingleUnit Init
	PTRLISTSEARCHSTART(m_SingleUnitList,CSkillObjectSingleUnit*,pSingleUnit)
		pSingleUnit->Init(m_pSkillInfo,pInfo->SkillLevel);
		ResultKind |= pSingleUnit->GetPNTarget();
	PTRLISTSEARCHEND;

	//진법은 어빌리티의 레벨으로 스킬 레벨에 있는 수치를 쓴다.
	BYTE SkillLvl = pInfo->SkillLevel;
	if(m_pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB)
	{
		BYTE stage = ((CPlayer*)m_pOperator)->GetStage();
		if( stage == eStage_Hwa || stage == eStage_Hyun )
			SkillLvl = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_HWA, ((CPlayer*)m_pOperator)->GetAbilityGroup()) + 1;
		else if( stage == eStage_Geuk || stage == eStage_Tal )
			SkillLvl = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_GEUK, ((CPlayer*)m_pOperator)->GetAbilityGroup()) + 1;
	}
	// StateUnit Init
	PTRLISTSEARCHSTART(m_StateUnitList,CSkillObjectStateUnit*,pStateUnit)
		pStateUnit->Init(m_pSkillInfo,SkillLvl,m_pOperator);
		ResultKind |= pStateUnit->GetPNTarget();
	PTRLISTSEARCHEND;

	CObject* pAdditionalTarget = NULL;
	if( ResultKind & SKILLRESULTKIND_POSITIVE && 
		m_pSkillInfo->GetPositiveResultTargetType() == SKILLTARGETTYPE_SELF)
		pAdditionalTarget = m_pOperator;
	if( ResultKind & SKILLRESULTKIND_NEGATIVE && 
		m_pSkillInfo->GetNegativeResultTargetType() == SKILLTARGETTYPE_SELF)
		pAdditionalTarget = m_pOperator;

	memcpy(&m_SkillObjectInfo,pInfo,sizeof(SKILLOBJECT_INFO));
	m_pTargetList->InitTargetList(this,pTList,pAdditionalTarget,&pInfo->MainTarget);
	
	MSG_SKILLOBJECT_ADD msg;
	msg.InitMsg(&m_SkillObjectInfo,TRUE);
	


	PTRLISTSEARCHSTART(m_StateUnitList,CSkillObjectStateUnit*,pStateUnit)
		pStateUnit->StartState(m_pTargetList);
	PTRLISTSEARCHEND;
	
	
	m_pFirstUnit->Init(m_pSkillInfo,pInfo->SkillLevel,m_pOperator);
	int nCount = m_pFirstUnit->ExcuteFirstUnit(m_pOperator,m_pTargetList,&msg,m_SkillTreeAmp);

	if( nCount > 0 || m_pSkillInfo->GetSkillInfo()->TargetKind == 1 )
	{

			//경험치 주기
			if( m_pOperator )
			if( m_pOperator->GetObjectKind() == eObjectKind_Player && 
				g_pServerSystem->GetMap()->IsVillage() == FALSE &&
				m_pOperator->GetBattle()->GetBattleKind() != eBATTLE_KIND_VIMUSTREET)
			{
				CPlayer* pPlayer = (CPlayer*)m_pOperator;
				
				DWORD MugongExp = MUGONG_USE_EXP;
				
				// RaMa - 04.11.24   ->무공경험치는 내력소모비율과 상관없이
				
				// 중국쪽 악명패널티
#ifdef _CHINA_LOCAL_
				if(pPlayer->GetBadFame() >= 60)
					MugongExp = MugongExp/4;
				else if(pPlayer->GetBadFame() >= 30)
					MugongExp = MugongExp/2;
				else
					MugongExp = MugongExp;
#endif
/*				if(g_pServerSystem->GetNation() == eNATION_CHINA)
				{
					if(pPlayer->GetBadFame() >= 60)
						MugongExp = MugongExp/4;
					else if(pPlayer->GetBadFame() >= 30)
						MugongExp = MugongExp/2;
					else
						MugongExp = MugongExp;
				}*/

				
				//이벤트 무공성 경험치

				MugongExp = (DWORD)((float)MugongExp * gEventRate[eEvent_MugongExp]);

				//SW060719 문파포인트
				DWORD AddMugongExpFromGuildPlustime = 0;
				if( pPlayer->GetGuildIdx() )
				{
					AddMugongExpFromGuildPlustime = MugongExp;
					if( FALSE == GUILDMGR->GetGuildPlustimeRt( pPlayer->GetGuildIdx(), eGPT_MuGong, &AddMugongExpFromGuildPlustime ) )
					{
						AddMugongExpFromGuildPlustime = 0;
					}
				}

				if( pPlayer->GetShopItemStats()->AddMugongExp )
				{
//					MugongExp *= (pPlayer->GetShopItemStats()->AddMugongExp*0.01f+1);
					DWORD temp = MugongExp * pPlayer->GetShopItemStats()->AddMugongExp;
					MugongExp = MugongExp + temp / 100 + ( temp > temp / 100 * 100 ? 1 : 0 );//올림
				}
				
				MugongExp += AddMugongExpFromGuildPlustime;

				// 2005 크리스마스 이벤트 코드
				if( pMainTarget )
				{
					if( pMainTarget->GetObjectKind() & eObjectKind_Monster )
					{
						 WORD kind = ((CMonster*)pMainTarget)->GetMonsterKind();
		                 if(kind != EVENT_MONSTER_SANTA1 && kind != EVENT_MONSTER_SANTA2)
							pPlayer->AddExpPointToMugong(m_pSkillInfo->GetSkillIndex(),MugongExp);
					}
#ifdef _HK_LOCAL_
					else if( pMainTarget->GetObjectKind() == eObjectKind_Player )
					{
//						char buf[128];
//						wsprintf( buf, "%d, %d, %d, %d", ResultKind, m_pSkillInfo->GetPositiveResultTargetType(), nCount, m_pSkillInfo->GetSkillInfo()->TargetKind );
//						g_Console.LOG( 4, buf );
						if( !( ResultKind & SKILLRESULTKIND_NEGATIVE ) &&
							( ( m_pSkillInfo->GetPositiveResultTargetType() == SKILLTARGETTYPE_SELF ) ||
							  ( m_pSkillInfo->GetPositiveResultTargetType() == SKILLTARGETTYPE_TEAMMATE ) ||
							  ( m_pSkillInfo->GetPositiveResultTargetType() == SKILLTARGETTYPE_NOTENEMYTARGET ) ) )
							pPlayer->AddExpPointToMugong(m_pSkillInfo->GetSkillIndex(),MugongExp);

					}
#endif
					else
					{
						pPlayer->AddExpPointToMugong(m_pSkillInfo->GetSkillIndex(),MugongExp);
					}
				}
				else
				{
//					char buf[128];
//					wsprintf( buf, "NULL %d, %d, %d, %d", ResultKind, m_pSkillInfo->GetPositiveResultTargetType(), nCount, m_pSkillInfo->GetSkillInfo()->TargetKind );
//					g_Console.LOG( 4, buf );
					pPlayer->AddExpPointToMugong(m_pSkillInfo->GetSkillIndex(),MugongExp);
				}
			}	
//		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	// 보조형(생명력,내력,호신강기,방어력,속성방어력) 적용
	SKILLOPTION* pSkillOption = NULL;

	// 사용자가 플레이어일때
	if(m_pOperator->GetObjectKind() == eObjectKind_Player)
	{
		// 스킬 옵션을 가져온다
		m_OptionIndex = pInfo->Option;
		pSkillOption = SKILLMGR->GetSkillOption(m_OptionIndex);
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 06. 07. 스킬개선 - 이영준
	CObject* pTarget = NULL;
	CYHHashTable<CSkillObject>* pTargetSkillList = NULL;
	CSkillObject* pTargetSkill = NULL;
	PTRLISTPOS pos = NULL;

	m_pTargetList->SetPositionHead();
	
	while(1)
	{
		//타겟을 가져온다
		pTarget = NULL;
		pTarget = m_pTargetList->GetNextTarget();

		//타겟이 없으면 종료
		if(pTarget == NULL)
			break;

		//타겟이 플레이어가 아니면 패스
		if(pTarget->GetObjectKind()	!= eObjectKind_Player)
			continue;

		//타겟에게 영향을 미치는 스킬리스트를 가져온다
		pTargetSkillList = NULL;
		pTargetSkillList = ((CPlayer*)pTarget)->GetSkillList();

		//없으면 패스
		if(!pTargetSkillList) continue;
		if(pTargetSkillList->GetDataNum() != 0)
		{
			pTargetSkillList->SetPositionHead();

			while(1)
			{
				//스킬리스트에서 스킬을 가져온다.
				pTargetSkill = NULL;
				pTargetSkill = (CSkillObject*)pTargetSkillList->GetData();

				//없으면 종료
				if(!pTargetSkill) break;

				//같은 계열이라면
				switch(IsSame(pTargetSkill))
				{
				case 1:	// 레벨 판단 필요한 스킬
					{
						//이미 플레이어에게 걸린 스킬보다 레벨이 낮은 스킬이면 삭제한다
						if(pTargetSkill->GetSkillObjectInfo()->SkillLevel > GetSkillObjectInfo()->SkillLevel)
						{
							DoDie(NULL);
							// 클라이언트에 생성메세지를 보내지 않는다.
							return;
						}
						//아니면 플레이어에게 걸린 스킬을 삭제한다
						else
							pTargetSkill->DoDie(NULL);
					}
					break;
				case 2: // 무조건 치고 들어가는 스킬
					{					
						pTargetSkill->DoDie(NULL);
					}
					break;
				case 3: // 무조건 삭제되는 스킬
					{
						DoDie(NULL);
						return;
					}
					break;
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		// 06. 06. 2차 전직 - 이영준
		// 무공 변환 추가
		// 보조형(생명력,내력,호신강기,방어력,속성방어력) 적용
		// 변환된 무공이면 능력치를 더해준다.
		if(pSkillOption)
		{
			((CPlayer*)pTarget)->AddSkillStatsOption(pSkillOption);

			CCharacterCalcManager::CalcMaxLife((CPlayer*)pTarget);
			CCharacterCalcManager::CalcMaxShield((CPlayer*)pTarget);
			CCharacterCalcManager::CalcMaxNaeRyuk((CPlayer*)pTarget);
		}
		//////////////////////////////////////////////////////////////////////////

		//스킬리스트에 추가해준다.
		pTargetSkillList->Add(this, GetID());
	}
	//////////////////////////////////////////////////////////////////////////
	
	PACKEDDATA_OBJ->QuickSend(m_pOperator,&msg,msg.GetMsgLength());
}

void CSkillObject::Release()
{
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	// 보조형(생명력,내력,호신강기,방어력,속성방어력) 적용
	
	// 사용자가 플레이어일때
	SKILLOPTION* pSkillOption = NULL;

	if(m_pOperator)
	if(m_pOperator->GetObjectKind() == eObjectKind_Player)
	{
		// 스킬 옵션을 가져온다
		pSkillOption = SKILLMGR->GetSkillOption(m_OptionIndex);
	}

	m_pTargetList->SetPositionHead();
	CObject* pTarget = NULL;

	while(1)
	{
		// 타겟을 가져온다
		pTarget = m_pTargetList->GetNextTarget();

		// 없으면 종료
		if(!pTarget)
			break;

		// 타겟이 플레이어가 아니면 다음 타겟으로 패스
		if(pTarget->GetObjectKind() != eObjectKind_Player)
			continue;

		////////////////////////////////////////////////////////////////////////////////
		// 06. 07. 스킬개선 - 이영준
		// 타겟의 스킬리스트에서 삭제해준다
		CYHHashTable<CSkillObject>* pTargetSkillList =((CPlayer*)pTarget)->GetSkillList();
		if(pTargetSkillList)
		{
			// 영향을 주었던 스킬이면
			if(pTargetSkillList->GetData(GetID()))
			{
				// 리스트에서 제거
				pTargetSkillList->Remove(GetID());

				// 스킬 옵션이 있으면 빼준다.
				if(pSkillOption)
				{
					((CPlayer*)pTarget)->RemoveSkillStatsOption(pSkillOption);

					CCharacterCalcManager::CalcMaxLife((CPlayer*)pTarget);
					CCharacterCalcManager::CalcMaxShield((CPlayer*)pTarget);
					CCharacterCalcManager::CalcMaxNaeRyuk((CPlayer*)pTarget);
				}
			}
		}
	}
	m_OptionIndex = 0;

	//////////////////////////////////////////////////////////////////////////

	PTRLISTSEARCHSTART(m_StateUnitList,CSkillObjectStateUnit*,pStateUnit)
		pStateUnit->EndState(m_pTargetList);
	PTRLISTSEARCHEND;

	m_pFirstUnit->Release();
	m_pTargetList->Release(this);
	
	PTRLISTSEARCHSTART(m_SingleUnitList,CSkillObjectSingleUnit*,pSingleUnit)
		pSingleUnit->Release();
	PTRLISTSEARCHEND;
	
	m_pOperator = NULL;
}

void CSkillObject::AddSingleUnit(CSkillObjectSingleUnit* pSingleUnit)
{
	m_SingleUnitList.AddTail(pSingleUnit);
	pSingleUnit->SetSkillObject(this);
}

void CSkillObject::AddStateUnit(CSkillObjectStateUnit* pStateUnit)
{
	m_StateUnitList.AddTail(pStateUnit);
	pStateUnit->SetSkillObject(this);
}

DWORD CSkillObject::Update()
{
	if(m_bDieFlag == TRUE)
		return SO_DESTROYOBJECT;

	////////////////////////////////////////////////////////////////////////////////////////////
	// 06. 04. 스킬 버그 수정 - 이영준
	// 부착(지속) 이펙트를 가진 스킬이면서 범위형 스킬이 아닌경우는
	// 몬스터에 부착되어 몬스터가 죽을때 같이 사라져야 하는 스킬로 판단한다.
	if(m_pSkillInfo->GetSkillInfo()->EffectMineOperate && m_pSkillInfo->GetSkillInfo()->TargetRange == 0)
	{
		m_pTargetList->SetPositionHead();
		WORD die = 0;
		while(CObject* pTarget = m_pTargetList->GetNextTarget())
		{
			if(pTarget->GetState() == eObjectState_Die)
				die++;
		}
		if(m_pTargetList->GetTargetNum() == 0 || m_pTargetList->GetTargetNum() <= die)
			return SO_DESTROYOBJECT;
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//YHTEST
	if(m_pOperator==NULL)
	{
//		MessageBox(NULL,"m_pOperator is NULL (CSkillObject::Update)",0,0);
		return SO_DESTROYOBJECT;
	}
	//////////////////////////////////////////////////////////////////////////
	

	if( m_pOperator->GetInited() == FALSE ||
		m_pOperator->GetID() != m_SkillObjectInfo.Operator)
		return SO_DESTROYOBJECT;

	////////////////////////////////////////////////////////////////////////////////////////////
	// 06. 04. 스킬 버그 수정 - 이영준
	// 사용자가 죽으면 스킬도 삭제한다.
	//if( m_pOperator->GetState() == eObjectState_Die)
	//	return SO_DESTROYOBJECT;
	////////////////////////////////////////////////////////////////////////////////////////////
		
	PTRLISTSEARCHSTART(m_SingleUnitList,CSkillObjectSingleUnit*,pSingleUnit)
		pSingleUnit->Update(&m_SkillObjectInfo,m_pTargetList,m_SkillTreeAmp);
	PTRLISTSEARCHEND;
	
	if(m_pTerminator->CheckTerminate(&m_SkillObjectInfo) == TRUE)
	{
		//종료처리
		return SO_DESTROYOBJECT;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 1인용 무공(철포삼등)일 경우 Operator 와 위치가 달라지면 Operator 위치로 이동시킨다.
	if( m_pSkillInfo->GetSkillInfo()->TargetAreaFix == 0 &&
		m_pTargetList->GetTargetNum() == 1 )
	{
		m_pTargetList->SetPositionHead();
		CObject* pTarget = m_pTargetList->GetNextTarget();
		if( pTarget->GetGridPosition()->x != GetGridPosition()->x ||
			pTarget->GetGridPosition()->z != GetGridPosition()->z )
		{
			CCharMove::SetPosition(this,CCharMove::GetPosition(pTarget));
		}
	}

	return SO_OK;
}

void CSkillObject::UpdateTargetList(CObject* pObject)
{
	m_pTargetList->UpdateTargetList(pObject);
}

void CSkillObject::AddTargetObject(CObject* pObject)
{
	// YH2DO 검사 필요

	BYTE bTargetKind = m_pTargetList->AddTargetObject(pObject,0);

	// yh 추가 됨
	if(bTargetKind == SKILLRESULTKIND_NONE)
		return;

	//////////////////////////////////////////////////////////////////////////
	// YH2DO Positive Negative 타겟 적용
	PTRLISTSEARCHSTART(m_StateUnitList,CSkillObjectStateUnit*,pStateUnit)
		pStateUnit->StartState(pObject,bTargetKind);
	PTRLISTSEARCHEND;
	//////////////////////////////////////////////////////////////////////////
	

	MSG_DWORD2BYTE msg;
	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_ADDOBJECT_TO_SKILLOBJECTAREA_ACK;
	msg.dwData1 = GetID();
	msg.dwData2 = pObject->GetID();
	msg.bData = bTargetKind;
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(this,&msg,sizeof(msg));
}
void CSkillObject::RemoveTargetObject(DWORD ObjectID)
{
	// YH2DO 검사 필요


	BYTE bTargetKind = m_pTargetList->RemoveTargetObject(ObjectID);

	// yh 추가 됨
	if(bTargetKind == SKILLRESULTKIND_NONE)
		return;
	
	CObject* pObject = g_pUserTable->FindUser(ObjectID);
	if(pObject)
	{
		//////////////////////////////////////////////////////////////////////////
		// YH2DO Positive Negative 타겟 적용
		PTRLISTSEARCHSTART(m_StateUnitList,CSkillObjectStateUnit*,pStateUnit)
			pStateUnit->EndState(pObject,bTargetKind);
		PTRLISTSEARCHEND;
		//////////////////////////////////////////////////////////////////////////
	}


	MSG_DWORD2 msg;
	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_REMOVEOBJECT_FROM_SKILLOBJECTAREA_ACK;
	msg.dwData1 = GetID();
	msg.dwData2 = ObjectID;
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(this,&msg,sizeof(msg));
}

void CSkillObject::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
{
	if(bLogin == TRUE)
	{
		*pMsgLen = 0;		// 처음 로그인 했을때 메세지는 InitSkillObject에서 보낸다.
		return;
	}

	MSG_SKILLOBJECT_ADD* pMsg = (MSG_SKILLOBJECT_ADD*)pAddMsg;
	pMsg->InitMsg(&m_SkillObjectInfo,FALSE);
	pMsg->dwObjectID = dwReceiverID;
	CTargetListIterator iter(&pMsg->TargetList);
	m_pTargetList->SetPositionHead();
	STLIST* pList;
	while(pList = m_pTargetList->GetNextTargetList())
	{
		iter.AddTargetWithNoData(pList->pObject->GetID(),pList->bTargetKind);
	}
	iter.Release();
	*pMsgLen = pMsg->GetMsgLength();
}
void CSkillObject::SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID)
{
	MSG_DWORD* pMsg = (MSG_DWORD*)pRemoveMsg;
	pMsg->Category = MP_SKILL;
	pMsg->Protocol = MP_SKILL_SKILLOBJECT_REMOVE;
	pMsg->dwObjectID = dwReceiverID;
	pMsg->dwData = GetID();
	
	*pMsgLen = sizeof(MSG_DWORD);
}

void CSkillObject::GetPosition(VECTOR3* pPos)
{
	*pPos = m_SkillObjectInfo.Pos;
}

BOOL CSkillObject::IsNegativeTarget(CObject* pObject)
{
	return TargetTypeCheck(m_NegativeTargetType,pObject);
}

BOOL CSkillObject::IsPositiveTarget(CObject* pObject)
{
	return TargetTypeCheck(m_PositiveTargetType,pObject);
}

BOOL CSkillObject::TargetTypeCheck(WORD TargetType,CObject* pObject)
{
	switch(TargetType)
	{
	case SKILLTARGETTYPE_ENEMY:
		{
			CBattle* pBattle = pObject->GetBattle();	// m_pOperator 속한 베틀을 얻어온다.
			return pBattle->IsEnemy(m_pOperator,pObject);
		}
		break;
	case SKILLTARGETTYPE_TEAMMATE:
		{
			CBattle* pBattle = pObject->GetBattle();	// m_pOperator 속한 베틀을 얻어온다.
			return pBattle->IsFriend(m_pOperator,pObject);
		}
		break;
	case SKILLTARGETTYPE_SELF:
		{
			if(pObject != m_pOperator)
				return FALSE;
		}
		break;
	case SKILLTARGETTYPE_ENEMYANDTEAM:
		{
			CBattle* pBattle = pObject->GetBattle();	// m_pOperator 속한 베틀을 얻어온다.
			if(pBattle->IsEnemy(m_pOperator,pObject))
				return TRUE;
			if(pBattle->IsFriend(m_pOperator,pObject))
				return TRUE;

			return FALSE;
		}
		break;
	case SKILLTARGETTYPE_NOTENEMYTARGET:
		{
			CBattle* pBattle = BATTLESYSTEM->GetBattle(m_pOperator->GetBattleID());	// m_pOperator 속한 베틀을 얻어온다.
			if(pBattle->IsEnemy(m_pOperator,pObject))
				return FALSE;

			return TRUE;
		}
		break;
	}
	return TRUE;
}

CObject* CSkillObject::GetOperator()
{
	return m_pOperator;
}

void CSkillObject::DoDie(CObject* pAttacker)
{
	m_bDieFlag = TRUE;
}

DWORD CSkillObject::GetLife()
{
	return m_CurLife;	
}
void CSkillObject::SetLife(DWORD Life,BOOL bSendMsg)
{
	if(Life > GetMaxLife()) 
		Life = GetMaxLife(); 
	m_CurLife = Life;
}
DWORD CSkillObject::DoGetMaxLife()
{
	return m_MaxLife;
}

BOOL CSkillObject::IsSame(CSkillObject* pSkill)
{
	// 1. 스킬 인덱스가 같으면 같은 계열이다. - 레벨 판단 필요
	if(pSkill->GetSkillIdx() == GetSkillIdx())
		return 1;

	// 2. 속성능력 증폭 스킬은 모두 같은 계열이다. - 무조건 치고 들어간다
	if(pSkill->GetSkillInfo()->GetAmplifiedPowerAtt(1) && GetSkillInfo()->GetAmplifiedPowerAtt(1))
		return 2;

	// 3. 방어력 증가(철포삼&나한기공)는 같은 계열이다. - 레벨 판단 필요
	if(pSkill->GetSkillInfo()->GetUpPhyDefense(12) > 0.00001f && GetSkillInfo()->GetUpPhyDefense(12) > 0.00001f)
		return 1;

	return FALSE;
}

//// 2007. 7. 10. CBH - 전문기술 관련 함수 추가
void CSkillObject::SkillObjectFirstUnitResult()
{
	if(m_pFirstUnit != NULL)	//예외처리
	{
		m_pFirstUnit->FirstUnitResult();
	}	
}
//////////////////////////////////////////////////
#endif
