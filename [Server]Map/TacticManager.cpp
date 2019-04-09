// TacticManager.cpp: implementation of the CTacticManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TacticManager.h"
#include "PartyManager.h"
#include "Party.h"
#include "UserTable.h"
#include "TacticStartInfo.h"
#include "Player.h"
#include "TacticObject.h"
#include "ObjectStateManager.h"
#include "TaticAbilityInfo.h"
#include "SkillManager_Server.h"
#include "AbilityManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DWORD TacticErrorCode = 0;
#define GotoTacticError(tag,ErrorCode)	{							\
									TacticErrorCode = ErrorCode;	\
									goto tag;						\
										}
		

CTacticManager::CTacticManager()
{
	m_TacticInfoTable.Initialize(64);
	m_GarbageTacticTable.Initialize(64);
}

CTacticManager::~CTacticManager()
{
	m_TacticInfoTable.SetPositionHead();
	while(CTacticStartInfo* pTacticInfo = m_TacticInfoTable.GetData())
	{
		delete pTacticInfo;
	}
	m_TacticInfoTable.RemoveAll();

	delete m_TacticAbilityInfo;
}

void CTacticManager::AddTacticStartInfo(TACTICSTART_INFO* pList)
{
	ASSERT(m_TacticInfoTable.GetData(pList->TacticID) == NULL);

	CTacticStartInfo* pInfo = new CTacticStartInfo(pList);
	m_TacticInfoTable.Add(pInfo,pList->TacticID);
}

CTacticStartInfo* CTacticManager::GetTacticStartInfo(WORD TacticID)
{
	CTacticStartInfo* pInfo = m_TacticInfoTable.GetData(TacticID);
	return pInfo;
}

void CTacticManager::AddTaticAbilityInfo(TATIC_ABILITY_INFO* pList)
{
	CTaticAbilityInfo* pInfo = new CTaticAbilityInfo(pList);
	m_TacticAbilityInfo = pInfo;
}

void CTacticManager::OnTacticStartReceived(CPlayer* pPlayer, SEND_TACTIC_START* pmsg)
{
	ASSERT(pPlayer);
	CTacticStartInfo* pTacticInfo = GetTacticStartInfo(pmsg->TacticID);
	
	CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());
	if(pParty == NULL)
		GotoTacticError(tacticstart_failed,eTSEC_NoParty);

	if(pParty->GetTacticObjectID() != 0)
		GotoTacticError(tacticstart_failed,eTSEC_AlreadyPartyDoingTactic);
		
	if(pTacticInfo->IsTacticableState(pPlayer) == FALSE)
		GotoTacticError(tacticstart_failed,eTSEC_NotTacticableState);

	if(pTacticInfo->IsTacticablePosition(pPlayer,DIRTODEG(pmsg->Direction)) == FALSE)
		GotoTacticError(tacticstart_failed,eTSEC_NotTacticablePosition);
	
	RegistTacticObject(pTacticInfo,pPlayer,pmsg->Direction);
	
	OBJECTSTATEMGR_OBJ->StartObjectState(pPlayer, eObjectState_Tactic, 0);
	
	return;

tacticstart_failed:
	MSG_DWORD msg;
	msg.Category = MP_TACTIC;
	msg.Protocol = MP_TACTIC_START_NACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.dwData = TacticErrorCode;
	pPlayer->SendMsg(&msg,sizeof(msg));
}

void CTacticManager::OnTacticJoinReceived(CPlayer* pPlayer, SEND_TACTIC_JOIN* pmsg)
{
	CParty* pParty;
	CTacticObject* pTacticObj;

	if(pPlayer->GetPartyIdx() == 0)
		GotoTacticError(tacticjoin_failed,eTJEC_NoPartyIdx);

	pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());
	if(pParty == NULL)
		GotoTacticError(tacticjoin_failed,eTJEC_NoPartyInstance);

	if(pParty->GetTacticObjectID() == 0)
		GotoTacticError(tacticjoin_failed,eTJEC_NotPartyTacticState);

	if(pParty->GetTacticObjectID() != pmsg->TacticObjectID)
		GotoTacticError(tacticjoin_failed,eTJEC_NotEqualBetweenMSGnPartyTactic);

	pTacticObj = GetTacticObject(pmsg->TacticObjectID);
	if(pTacticObj == NULL)
		GotoTacticError(tacticjoin_failed,eTJEC_NoTacticObject);

	pTacticObj->TacticJoin(pPlayer,pmsg->Position);
		
	OBJECTSTATEMGR_OBJ->StartObjectState(pPlayer, eObjectState_Tactic, 0);

	return;

tacticjoin_failed:
	MSG_DWORD msg;
	msg.Category = MP_TACTIC;
	msg.Protocol = MP_TACTIC_JOIN_NACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.dwData = TacticErrorCode;
	pPlayer->SendMsg(&msg,sizeof(msg));	
}


void CTacticManager::RegistTacticObject(CTacticStartInfo* pInfo,CPlayer* pOperator,DIRINDEX Direction)
{
	BASEOBJECT_INFO binfo;
	binfo.dwObjectID = GetNewTacticObjectIdx();
	//strcpy(binfo.ObjectName,pInfo->GetTacticName());
	SafeStrCpy(binfo.ObjectName,pInfo->GetTacticName(),MAX_NAME_LENGTH+1);
	
	binfo.ObjectState = eObjectState_None;

	g_pServerSystem->AddTacticObject(&binfo,pInfo,pOperator,Direction);
}

CTacticObject* CTacticManager::GetTacticObject(DWORD TacticObjectID)
{
	CObject* pObj = g_pUserTable->FindUser(TacticObjectID);
	if(pObj == NULL)
		return NULL;
	if(pObj->GetObjectKind() != eObjectKind_Tactic)
		return NULL;

	return (CTacticObject*)pObj;
}

BOOL CTacticManager::TacticProcess(CTacticObject* pTacObj)
{
	if(pTacObj->CheckFail() == TRUE)
	{
		AddGarbageTactic(pTacObj);
		return TRUE;
	}
	if( pTacObj->CheckExecute() == TRUE)
	{
		//혹시나 안지워지는게 있으면
		if(gCurTime - pTacObj->GetTacticStartTime() > 30000)
		{
			ASSERTMSG(0, "AddGarbageTactic!");
			AddGarbageTactic(pTacObj);
		}
	}
	return FALSE;
/* hs 스킬 발동하고 지우게 수정
	else if(pTacObj->CheckExecute() == TRUE)
	{
		AddGarbageTactic(pTacObj); 
		return TRUE;
	}

	
	else
	{
		return FALSE;
	}
*/

}

void CTacticManager::AddGarbageTactic(CTacticObject* pTacObj)
{
	m_GarbageTacticTable.Add(pTacObj,pTacObj->GetID());
}

void CTacticManager::ProcessGarbageTactic()
{
	m_GarbageTacticTable.SetPositionHead();
	CTacticObject* pObj;
	while(pObj = m_GarbageTacticTable.GetData())
	{
		g_pServerSystem->RemoveTacticObject(pObj->GetID());
	}
	m_GarbageTacticTable.RemoveAll();
}

DWORD CTacticManager::GetAttackValue( CPlayer* pOperator, DWORD AttackValue )
{
	CParty* pParty = PARTYMGR->GetParty( pOperator->GetPartyIdx() );
	if( pParty == NULL )
		return 0;

	DWORD TacticObjectID = pParty->GetTacticObjectID();

	CObject* pObj = g_pUserTable->FindUser(TacticObjectID);
	if( pObj == NULL )
		return 0;	

	//////////////////////////////////////////////////////////////////////////
	// 진법에 참여한 캐릭터의 레벨 합 * 진법 변수 + 진법 최소 공격력 + 진법 시전자의 주술력
	BYTE AbilityLevel = 0;
	BYTE stage = pOperator->GetStage();
	if( stage == eStage_Hwa || stage == eStage_Hyun )
		AbilityLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_HWA, pOperator->GetAbilityGroup());
	else if( stage == eStage_Geuk || stage == eStage_Tal )
		AbilityLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_GEUK, pOperator->GetAbilityGroup());

	DWORD TacticValue = m_TacticAbilityInfo->GetTacticAttackAbilityValue( AbilityLevel );
	
	return ( (CTacticObject*)pObj)->GetJoinLevel() * AttackValue + TacticValue;
}



/*
DWORD CTacticManager::GetAttackMaxValue( CPlayer* pOperator, WORD SkillIdx )
{
	DWORD TacticObjectID = PARTYMGR->GetParty(pOperator->GetPartyIdx())->GetTacticObjectID();

	CObject* pObj = g_pUserTable->FindUser(TacticObjectID);
	if( !pObj)	return 0;	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( SkillIdx );
	if( !pSkillInfo )	return 0;
	
	//////////////////////////////////////////////////////////////////////////
	// 진법에 참여한 캐릭터의 레벨 합 * 진법 변수 + 진법 최소 공격력 + 진법 시전자의 주술력
//	BYTE AbilityLevel = ABILITYMGR->GetAbilityLevel( ABILITYINDEX_TACTIC, pOperator->GetAbilityGroup() );
	BYTE AbilityLevel = 0;
	BYTE stage = pOperator->GetStage();
	if( stage == eStage_Hwa || stage == eStage_Hyun )
		AbilityLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_HWA, pOperator->GetAbilityGroup());
	else if( stage == eStage_Geuk || stage == eStage_Tal )
		AbilityLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_GEUK, pOperator->GetAbilityGroup());

	DWORD TacticValue = m_TacticAbilityInfo->GetTacticAttackAbilityValue( AbilityLevel );
	
	return ( (CTacticObject*)pObj)->GetJoinLevel() * pSkillInfo->GetSkillInfo()->FirstPhyAttack[0] +
			 pSkillInfo->GetSkillInfo()->FirstAttAttackMax[0] + TacticValue;

/*	BYTE AbilityLevel =  1; // ((CTacticObject*)pObj)->GetOperatorAbilityLevel();
	DWORD TacticValue = m_TacticAbilityInfo->GetTacticAttackAbilityValue(AbilityLevel);

	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(SkillIdx);
	
	return ((CTacticObject*)pObj)->GetJoinLevel() * 
			pSkillInfo->GetFirstAttAttack(0) +
			pSkillInfo->GetSkillInfo()->FirstAttAttackMax[0]+ TacticValue;
			*\/
}

DWORD CTacticManager::GetAttackMinValue( CPlayer* pOperator, WORD SkillIdx )
{
	DWORD TacticObjectID = PARTYMGR->GetParty(pOperator->GetPartyIdx())->GetTacticObjectID();

	CObject* pObj = g_pUserTable->FindUser(TacticObjectID);
	if( !pObj)	return 0;	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( SkillIdx );
	if( !pSkillInfo )	return 0;

	//////////////////////////////////////////////////////////////////////////
	// 진법에 참여한 캐릭터의 레벨 합 * 진법 변수 + 진법 최소 공격력 + 진법 시전자의 주술력
//	BYTE AbilityLevel = ABILITYMGR->GetAbilityLevel( ABILITYINDEX_TACTIC, pOperator->GetAbilityGroup() );
	BYTE AbilLevel = 0;
	BYTE stage = pOperator->GetStage();
	if( stage == eStage_Hwa || stage == eStage_Hyun )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_HWA, pOperator->GetAbilityGroup());
	else if( stage == eStage_Geuk || stage == eStage_Tal )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_GEUK, pOperator->GetAbilityGroup());

	DWORD TacticValue = m_TacticAbilityInfo->GetTacticAttackAbilityValue( AbilLevel );
	
	return ( (CTacticObject*)pObj)->GetJoinLevel() * pSkillInfo->GetSkillInfo()->FirstPhyAttack[0] +
			 pSkillInfo->GetSkillInfo()->FirstAttAttackMin[0] + TacticValue;
	/*
	BYTE AbilityLevel =  ((CTacticObject*)pObj)->GetOperatorAbilityLevel();
	DWORD TacticValue = m_TacticAbilityInfo->GetTacticAttackAbilityValue(AbilityLevel);

	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(SkillIdx);
	
	return ((CTacticObject*)pObj)->GetJoinLevel() * 
			pSkillInfo->GetFirstAttAttack(0) +
			pSkillInfo->GetSkillInfo()->FirstAttAttackMin[0]+ TacticValue;
	*\/
}
*/


void CTacticManager::GetRecoverValue( CPlayer* pOperator, WORD SkillIdx, WORD &Life, WORD &Naeryuk )
{
	CParty* pParty = PARTYMGR->GetParty(pOperator->GetPartyIdx());
	if( pParty == NULL ) return;

	DWORD TacticObjectID = pParty->GetTacticObjectID();

	CObject* pObj = g_pUserTable->FindUser(TacticObjectID);
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( SkillIdx );
	if( !pObj || !pSkillInfo )
	{
		Life = 0;
		Naeryuk = 0;
		return;	
	}
	
	//////////////////////////////////////////////////////////////////////////
	//진법 자체의 회복 수치 + 진법 시전자의 주술력
//	BYTE AbilityLevel = ABILITYMGR->GetAbilityLevel( ABILITYINDEX_TACTIC, pOperator->GetAbilityGroup() );
	BYTE AbilLevel = 0;
	BYTE stage = pOperator->GetStage();
	if( stage == eStage_Hwa || stage == eStage_Hyun )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_HWA, pOperator->GetAbilityGroup());
	else if( stage == eStage_Geuk || stage == eStage_Tal )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_GEUK, pOperator->GetAbilityGroup());

	DWORD TacticValue = m_TacticAbilityInfo->GetTacticRecoverAbilityValue( AbilLevel );
	
	Life = (WORD)(pSkillInfo->GetSkillInfo()->FirstRecoverLife[0] + TacticValue);
	Naeryuk = (WORD)(pSkillInfo->GetSkillInfo()->FirstRecoverNaeRyuk[0] + TacticValue);

/*	BYTE AbilityLevel =  ((CTacticObject*)pObj)->GetOperatorAbilityLevel();
	WORD TacticValue = m_TacticAbilityInfo->GetTacticRecoverAbilityValue(AbilityLevel);

	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(SkillIdx);

	Naeryuk = pSkillInfo->GetSkillInfo()->FirstRecoverNaeRyuk[0] + TacticValue;
	Life = pSkillInfo->GetSkillInfo()->FirstRecoverLife[0] + TacticValue;
	*/
}

float CTacticManager::GetBuffRateForPhyAttack( CPlayer* pOperator, WORD SkillIdx )
{
	CParty* pParty = PARTYMGR->GetParty(pOperator->GetPartyIdx());
	if( pParty == NULL )
		return 0.0f;

	DWORD TacticObjectID = pParty->GetTacticObjectID();

	CObject* pObj = g_pUserTable->FindUser(TacticObjectID);
	if( !pObj)	return 0;	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( SkillIdx );
	if( !pSkillInfo )	return 0;	
	
	//////////////////////////////////////////////////////////////////////////
	//진법 자체의 버프상승 수치 + 진법 시전자의 주술력
//	BYTE AbilityLevel = ABILITYMGR->GetAbilityLevel( ABILITYINDEX_TACTIC, pOperator->GetAbilityGroup() );
	BYTE AbilLevel = 0;
	BYTE stage = pOperator->GetStage();
	if( stage == eStage_Hwa || stage == eStage_Hyun )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_HWA, pOperator->GetAbilityGroup());
	else if( stage == eStage_Geuk || stage == eStage_Tal )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_GEUK, pOperator->GetAbilityGroup());

	float TacticValue = m_TacticAbilityInfo->GetTacticBuffRateValue( AbilLevel );

	return pSkillInfo->GetSkillInfo()->UpPhyAttack[0] + TacticValue;
}

float CTacticManager::GetBuffRateForPhyDefense( CPlayer* pOperator, WORD SkillIdx )
{
	CParty* pParty = PARTYMGR->GetParty(pOperator->GetPartyIdx());

	if( pParty == NULL )
		return 0.0f;

	DWORD TacticObjectID = pParty->GetTacticObjectID();

	CObject* pObj = g_pUserTable->FindUser(TacticObjectID);
	if( !pObj)	return 0;	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( SkillIdx );
	if( !pSkillInfo )	return 0;	
	
	//////////////////////////////////////////////////////////////////////////
	//진법 자체의 버프상승 수치 + 진법 시전자의 주술력
//	BYTE AbilityLevel = ABILITYMGR->GetAbilityLevel( ABILITYINDEX_TACTIC, pOperator->GetAbilityGroup() );
	BYTE AbilLevel = 0;
	BYTE stage = pOperator->GetStage();
	if( stage == eStage_Hwa || stage == eStage_Hyun )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_HWA, pOperator->GetAbilityGroup());
	else if( stage == eStage_Geuk || stage == eStage_Tal )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_GEUK, pOperator->GetAbilityGroup());

	float TacticValue = m_TacticAbilityInfo->GetTacticBuffRateValue( AbilLevel );

	return pSkillInfo->GetSkillInfo()->UpPhyDefence[0] + TacticValue;
}

float CTacticManager::GetBuffRateForAttrDefense( CPlayer* pOperator, WORD SkillIdx )
{
	CParty* pParty = PARTYMGR->GetParty(pOperator->GetPartyIdx());

	if( pParty == NULL )
		return 0.0f;

	DWORD TacticObjectID = pParty->GetTacticObjectID();

	CObject* pObj = g_pUserTable->FindUser(TacticObjectID);
	if( !pObj)	return 0;	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( SkillIdx );
	if( !pSkillInfo )	return 0;	
	
	//////////////////////////////////////////////////////////////////////////
	//진법 자체의 버프상승 수치 + 진법 시전자의 주술력
//	BYTE AbilityLevel = ABILITYMGR->GetAbilityLevel( ABILITYINDEX_TACTIC, pOperator->GetAbilityGroup() );
	BYTE AbilLevel = 0;
	BYTE stage = pOperator->GetStage();
	if( stage == eStage_Hwa || stage == eStage_Hyun )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_HWA, pOperator->GetAbilityGroup());
	else if( stage == eStage_Geuk || stage == eStage_Tal )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_GEUK, pOperator->GetAbilityGroup());

	float TacticValue = m_TacticAbilityInfo->GetTacticBuffRateValue( AbilLevel );

	return pSkillInfo->GetSkillInfo()->UpAttDefence[0] + TacticValue;
}

DWORD CTacticManager::GetBuffValueForNaeryuk( CPlayer* pOperator, WORD SkillIdx )
{
	CParty* pParty = PARTYMGR->GetParty(pOperator->GetPartyIdx());

	if( pParty == NULL )
		return 0;

	DWORD TacticObjectID = pParty->GetTacticObjectID();

	CObject* pObj = g_pUserTable->FindUser(TacticObjectID);
	if( !pObj)	return 0;	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( SkillIdx );
	if( !pSkillInfo )	return 0;	
	
	//////////////////////////////////////////////////////////////////////////
	//진법 자체의 버프상승 수치 + 진법 시전자의 주술력
//	BYTE AbilityLevel = ABILITYMGR->GetAbilityLevel( ABILITYINDEX_TACTIC, pOperator->GetAbilityGroup() );
	BYTE AbilLevel = 0;
	BYTE stage = pOperator->GetStage();
	if( stage == eStage_Hwa || stage == eStage_Hyun )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_HWA, pOperator->GetAbilityGroup());
	else if( stage == eStage_Geuk || stage == eStage_Tal )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_GEUK, pOperator->GetAbilityGroup());

	DWORD TacticValue = m_TacticAbilityInfo->GetTacticBuffValue( AbilLevel );

	return pSkillInfo->GetSkillInfo()->UpMaxNaeRyuk[0] + TacticValue;
}

DWORD CTacticManager::GetBuffValueForLife( CPlayer* pOperator, WORD SkillIdx )
{
	CParty* pParty = PARTYMGR->GetParty(pOperator->GetPartyIdx());

	if( pParty == NULL )
		return 0;

	DWORD TacticObjectID = pParty->GetTacticObjectID();

	CObject* pObj = g_pUserTable->FindUser(TacticObjectID);
	if( !pObj)	return 0;	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( SkillIdx );
	if( !pSkillInfo )	return 0;	
	
	//////////////////////////////////////////////////////////////////////////
	//진법 자체의 버프상승 수치 + 진법 시전자의 주술력
//	BYTE AbilityLevel = ABILITYMGR->GetAbilityLevel( ABILITYINDEX_TACTIC, pOperator->GetAbilityGroup() );
	BYTE AbilLevel = 0;
	BYTE stage = pOperator->GetStage();
	if( stage == eStage_Hwa || stage == eStage_Hyun )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_HWA, pOperator->GetAbilityGroup());
	else if( stage == eStage_Geuk || stage == eStage_Tal )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_GEUK, pOperator->GetAbilityGroup());

	DWORD TacticValue = m_TacticAbilityInfo->GetTacticBuffValue( AbilLevel );

	return pSkillInfo->GetSkillInfo()->UpMaxLife[0] + TacticValue;

}

DWORD CTacticManager::GetBuffValueForShield( CPlayer* pOperator, WORD SkillIdx )
{
	CParty* pParty = PARTYMGR->GetParty(pOperator->GetPartyIdx());

	if( pParty == NULL )
		return 0;

	DWORD TacticObjectID = pParty->GetTacticObjectID();

	CObject* pObj = g_pUserTable->FindUser(TacticObjectID);
	if( !pObj)	return 0;	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( SkillIdx );
	if( !pSkillInfo )	return 0;	
	
	//////////////////////////////////////////////////////////////////////////
	//진법 자체의 버프상승 수치 + 진법 시전자의 주술력
//	BYTE AbilityLevel = ABILITYMGR->GetAbilityLevel( ABILITYINDEX_TACTIC, pOperator->GetAbilityGroup() );
	BYTE AbilLevel = 0;
	BYTE stage = pOperator->GetStage();
	if( stage == eStage_Hwa || stage == eStage_Hyun )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_HWA, pOperator->GetAbilityGroup());
	else if( stage == eStage_Geuk || stage == eStage_Tal )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_GEUK, pOperator->GetAbilityGroup());

	DWORD TacticValue = m_TacticAbilityInfo->GetTacticBuffValue( AbilLevel );

	return pSkillInfo->GetSkillInfo()->UpMaxShield[0] + TacticValue;
}

void CTacticManager::AddGarbageTactic(CPlayer* pOperator)
{
	CParty* pParty = PARTYMGR->GetParty(pOperator->GetPartyIdx());
	if(pParty == 0)
	{
		ASSERTMSG(0, "AddGarbageTactic NULL Party.");
		return; 
	}
	DWORD TacticObjectID = pParty->GetTacticObjectID();
	CObject* pObj = g_pUserTable->FindUser(TacticObjectID);

	if(pObj == NULL)
	{
		ASSERT(0);
		return;
	}
	if(pObj->GetObjectKind() != eObjectKind_Tactic)
	{
		ASSERT(0);
		return;
	}

	AddGarbageTactic((CTacticObject*)pObj);

	PARTYMGR->SetTacticObjectID(pOperator->GetPartyIdx(),0);
}

DWORD toidTemp = 0;
DWORD CTacticManager::GetNewTacticObjectIdx()
{
	return TACTICOBJECT_ID_START+ toidTemp++;
}
