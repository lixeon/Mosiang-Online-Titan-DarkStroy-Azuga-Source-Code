// Action.cpp: implementation of the CAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CAction.h"

#include "SkillManager_Client.h"
#include "SkillInfo.h"
#include "StreetStallManager.h"

#include "ObjectManager.h"
#include "NpcScriptManager.h"
#include "cMsgBox.h"
#include "./interface/cWindowManager.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "MoveManager.h"
#include "objectstatemanager.h"
#include "ItemManager.h"
#include "Gamein.h"
#include "InventoryExDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAction::CAction()
{
	Clear();
}

CAction::~CAction()
{

}

BOOL CAction::HasAction()
{
	return m_ActionKind==eActionKind_None ? FALSE : TRUE;
}

void CAction::CopyFrom(CAction* pSrc)
{
	if(pSrc == NULL)
		Clear();
	else
		memcpy(this,pSrc,sizeof(CAction));
}

void CAction::InitSkillAction(CSkillInfo* pSkillInfo,CActionTarget* pTarget)
{
	int OptionRange = 0;
	WORD OptionIdx = HERO->GetSkillOptionIndex(pSkillInfo->GetSkillIndex());
	SKILLOPTION* pOption = NULL;
	if(OptionIdx)
		pOption = SKILLMGR->GetSkillOption(OptionIdx);

	if(pOption)
		OptionRange = pOption->Range;

	InitAction(eActionKind_Skill,pSkillInfo,pSkillInfo->GetSkillRange()-50 + OptionRange,pTarget);	//
}
void CAction::InitSkillActionRealExecute(CSkillInfo* pSkillInfo,CActionTarget* pTarget)
{
	int OptionRange = 0;
	WORD OptionIdx = HERO->GetSkillOptionIndex(pSkillInfo->GetSkillIndex());
	SKILLOPTION* pOption = NULL;
	if(OptionIdx)
		pOption = SKILLMGR->GetSkillOption(OptionIdx);

	if(pOption)
		OptionRange = pOption->Range;

	InitAction(eActionKind_Skill_RealExecute,pSkillInfo,pSkillInfo->GetSkillRange()-50 + OptionRange,pTarget);	//
}

void CAction::InitStallAction( float Distance, CActionTarget* pTarget )
{
	InitAction(eActionKind_TalkToOwner, NULL, Distance, pTarget);	//
}

void CAction::InitMoveAction(VECTOR3* pToPos)
{
	m_ActionTarget.InitActionTarget(pToPos,0);
	InitAction(eActionKind_Move, NULL, 0, &m_ActionTarget);
}



void CAction::InitAction(int ActionKind,void* pActionInfo,float Distance,CActionTarget* pTarget)
{
	m_ActionKind = ActionKind;
	m_Distance = Distance;
	m_pActionInfo = pActionInfo;
	m_ActionTarget.CopyFrom(pTarget);
}
void CAction::Clear()
{
	m_ActionKind = eActionKind_None;
}

BOOL CAction::CheckTargetDistance(VECTOR3* pCurPos)
{
	VECTOR3* pTargetPos = m_ActionTarget.GetTargetPosition();
	if(pTargetPos == NULL)
		return FALSE;

	float dist = CalcDistanceXZ(pCurPos,pTargetPos);
	if(dist <= m_Distance)
		return TRUE;
	else
		 return FALSE;
}

void CAction::ExcuteAction(CHero* pOperator)
{
	switch(m_ActionKind) {
	case eActionKind_Skill:
		{
			//////////////////////////////////////////////////////////////////////////
			// 06. 06. 2차 전직 - 이영준
			// 무공 변환 추가
			CSkillInfo* pNextSkill = (CSkillInfo*)m_pActionInfo;

			WORD SkillOptionIndex = HERO->GetSkillOptionIndex(pNextSkill->GetSkillIndex());
			SKILLOPTION* pSkillOption = NULL;

			if(SkillOptionIndex)
				pSkillOption = SKILLMGR->GetSkillOption(SkillOptionIndex);
			//////////////////////////////////////////////////////////////////////////

			if(pNextSkill->IsInSkillRange(pOperator,&m_ActionTarget,pSkillOption) == TRUE)
			{	// 성공
				SKILLMGR->ExcuteSkillSYN(pOperator,&m_ActionTarget,pNextSkill);
			}
			else
			{	// 실패
				SKILLMGR->ExcuteSkillSYN(pOperator,&m_ActionTarget,pNextSkill->IsMugong());
			}			
		}
		break;
		
	case eActionKind_Skill_RealExecute:
		SKILLMGR->RealExcuteSkillSYN(pOperator,&m_ActionTarget,(CSkillInfo*)m_pActionInfo);
		break;

	case eActionKind_TalkToNpc:
		{
			CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject( m_ActionTarget.GetTargetID() );
			if( pNpc == NULL )	return;
			if((pNpc->GetNpcJob() == BOMUL_ROLE) && (pNpc->GetState() == eObjectState_Die))
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(918));
				return;
			}


			MSG_DWORD2 msg;
			msg.Category = MP_NPC;
			msg.Protocol = MP_NPC_SPEECH_SYN;
			msg.dwObjectID = HERO->GetID();
			msg.dwData1 = m_ActionTarget.GetTargetID();	//NPC Idx
			msg.dwData2 = pNpc->GetNpcUniqueIdx();

			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);

			NETWORK->Send(&msg, sizeof(msg));
		
			pNpc->OnClicked();
		}
		break;
	case eActionKind_TalkToOwner:
//노점기능 정상화 될때까지 패치올리자.
//		CHATMGR->AddMsg( CTC_SYSMSG, "노점상 기능이 일시적으로 중단되었습니다. 이용에 불편을 드려 죄송합니다." );
		STREETSTALLMGR->EnterStreetStall( m_ActionTarget.GetTargetID() );
		break;
	case eActionKind_Move:
		MOVEMGR->SetHeroTarget( m_ActionTarget.GetTargetPosition(), TRUE);
		break;
	case eActionKind_EquipItem:
		{
			CItem* pItem = ITEMMGR->GetItem((DWORD)m_pActionInfo);		
			if(pItem == NULL)
				return;
			GAMEIN->GetInventoryDialog()->UseItem(pItem);
		}
		break;
	case eActionKind_TalkToPet:
		{
			CPetBase* pPet = (CPetBase*)OBJECTMGR->GetObject( m_ActionTarget.GetTargetID() );
			if( !pPet )		return;

			pPet->OnClicked();
		}
		break;
	default:
		ASSERT(0);
		break;
	}
}
