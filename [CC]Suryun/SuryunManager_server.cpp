// SuryunManager.cpp: implementation of the CSuryunManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _MAPSERVER_
#include "SuryunManager_server.h"
#include "UserTable.h"
#include "Player.h"
#include "CharMove.h"
#include "RecallManager.h"
#include "MapDBMsgParser.h"
#include "SkillManager_server.h"

#include "..\[CC]BattleSystem\BattleSystem_Server.h"
#include "..\[CC]BattleSystem\Suryun\Battle_Suryun.h"

#include "Network.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSuryunManager::CSuryunManager()
{
	m_MissionInfo.Initialize(64);
	m_MonsterInfo.Initialize(64);
	m_ChangeLevelTable.Initialize(64);
	LoadSuryunInfo();
	LoadMonsterInfo();
}

CSuryunManager::~CSuryunManager()
{
	SURYUNMISSIONINFO* pInfo;
	m_MissionInfo.SetPositionHead();
	while(pInfo = m_MissionInfo.GetData())
		delete pInfo;
	m_MissionInfo.RemoveAll();	
	
	SURYUNMONSTERINFO* pMInfo;
	m_MonsterInfo.SetPositionHead();
	while(pMInfo = m_MonsterInfo.GetData())
		delete pMInfo;
	m_MonsterInfo.RemoveAll();
	
	SURYUNLEVELCHANGE* pChange;
	m_ChangeLevelTable.SetPositionHead();
	while(pChange = m_ChangeLevelTable.GetData())
		delete pChange;
	m_ChangeLevelTable.RemoveAll();
}

#define OnError(errstate,reason_code,goto_target)	\
if(errstate)	{	err_reason = reason_code;	goto goto_target;	}
void CSuryunManager::OnGoSuryunSyn(CPlayer* pPlayer,WORD MugongIdx)
{
	int err_reason = 0;
	BOOL rt;
	MAPTYPE mapnum = SURYUNMAP;
	BATTLE_INFO_SURYUN BattleInfo;
	SURYUNMISSIONINFO* pMission;
	SURYUNINFO* pSuryunInfo;
	SURYUNMONSTERINFO* pMonsterInfo;
	MSG_DWORD msg;
	int adlv;
	MONEYTYPE SuryunFee;
	int sung = pPlayer->GetMugongLevel(MugongIdx);
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(MugongIdx);

	OnError(sung == -1,1,OnGoSuryunSyn_NACK)

	pMission = GetMissionInfo(MugongIdx);
	OnError(pMission == NULL,2,OnGoSuryunSyn_NACK);
	pSuryunInfo = pMission->GetSuryunInfo(sung);
	OnError(pSuryunInfo == NULL,3,OnGoSuryunSyn_NACK);
	
	adlv = GetChangedLevel(pPlayer->GetLevel(),sung) + pSuryunInfo->AdjustLevel;	
	if( adlv <=0 )
		adlv = 1;
	
	pMonsterInfo = GetMonsterInfo((LEVELTYPE)adlv);
	OnError(pMonsterInfo == NULL,4,OnGoSuryunSyn_NACK);

	SuryunFee = GetSuryunFee(pSuryunInfo,pPlayer);
	OnError(pPlayer->GetMoney() < SuryunFee, 5,OnGoSuryunSyn_NACK);
	OnError(pSuryunInfo->LimitTime == 0,6,OnGoSuryunSyn_NACK);
	OnError(pSuryunInfo->RegenNum == 0,7,OnGoSuryunSyn_NACK);
	OnError(pSuryunInfo->MonNum == 0,8,OnGoSuryunSyn_NACK);
	OnError(pSkillInfo == NULL,9,OnGoSuryunSyn_NACK);
	// magi82 - Titan(070911) 타이탄 무공업데이트
	OnError(pSkillInfo->GetSkillKind() == SKILLKIND_TITAN,10,OnGoSuryunSyn_NACK);
	OnError(
		pPlayer->GetWeaponEquipType() != pSkillInfo->GetWeaponType() &&
		pSkillInfo->GetSkillKind() == SKILLKIND_OUTERMUGONG
		,10,OnGoSuryunSyn_NACK);
	
	
	
	BattleInfo.BattleID		= BATTLESYSTEM->MakeNewBattleID();
	BattleInfo.BattleKind	= eBATTLE_KIND_SURYUN;
	BattleInfo.BattleState	= eBATTLE_STATE_READY;
	BattleInfo.BattleTime	= 0;
	BattleInfo.ReturnBattleID = pPlayer->GetBattleID();
	BattleInfo.ReturnGridID = pPlayer->GetGridID();
	BattleInfo.ReturnPosition = *CCharMove::GetPosition(pPlayer);
	BattleInfo.CurrentSung = sung;
	pMonsterInfo->GetMonsterArray(BattleInfo.MonsterKindInfo);
	BattleInfo.MugongIdx = MugongIdx;
	rt = BATTLESYSTEM->CreateBattle(&BattleInfo,mapnum);
	OnError(rt == FALSE,6,OnGoSuryunSyn_NACK);

	if(SuryunFee)
	{
		pPlayer->SetMoney( SuryunFee, MONEY_SUBTRACTION, 0, 
			eItemTable_Inventory, eMoneyLog_LoseSuryunFee, MugongIdx );
	}
	
	CCharMove::ReleaseMove(pPlayer);
	
	pPlayer->SetLife(pPlayer->GetMaxLife());
	pPlayer->SetShield(pPlayer->GetMaxShield());
	pPlayer->SetNaeRyuk(pPlayer->GetMaxNaeRyuk());
	
	pPlayer->SetBattle(BattleInfo.BattleID,eBattleTeam1);
	pPlayer->SetGridID(BattleInfo.BattleID);
	
	msg.Category = MP_SURYUN;
	msg.Protocol = MP_SURYUN_GOSURYUNMAP_ACK;
	msg.dwData = mapnum;
	pPlayer->SendMsg(&msg,sizeof(msg));
	
	return;
OnGoSuryunSyn_NACK:
	msg.Category = MP_SURYUN;
	msg.Protocol = MP_SURYUN_GOSURYUNMAP_NACK;
	msg.dwData = err_reason;
	pPlayer->SendMsg(&msg,sizeof(msg));

}

void CSuryunManager::NetworkMsgParse(DWORD dwConnectionIndex, BYTE Protocol,void* pMsg)
{
	ySWITCH(Protocol)
		yCASE(MP_SURYUN_GOSURYUNMAP_SYN)
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)	return;
			OnGoSuryunSyn(pPlayer,pmsg->wData);

		yCASE(MP_SURYUN_ENTER_SYN)
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)	return;

			ASSERT(pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SURYUN);

			//베틀이 잘못되었다. 끊어버리자.
			if( pPlayer->GetBattle()->GetBattleKind() != eBATTLE_KIND_SURYUN )
			{
				MSG_DWORD msg;
				msg.Category	= MP_USERCONN;
				msg.Protocol	= MP_USERCONN_GAMEIN_NACK;
				msg.dwData		= pmsg->dwObjectID;
				g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );					
				return;
			}

			VECTOR3 StartPosition;
			SetVector3(&StartPosition,SURYUNENTER_POSITION_X,0,SURYUNENTER_POSITION_Z);

			CCharMove::InitMove(pPlayer,&StartPosition);
			pPlayer->SetInitState(
				PLAYERINITSTATE_MUGONG_INFO |
				PLAYERINITSTATE_ITEM_INFO |
				PLAYERINITSTATE_ITEM_RARE_OPTION_INFO |
				PLAYERINITSTATE_ITEM_OPTION_INFO |
				PLAYERINITSTATE_ABILITY_INFO |
//				PLAYERINITSTATE_GMCONTROL_INFO |
				PLAYERINITSTATE_HERO_INFO |
				PLAYERINITSTATE_PYOGUK_INFO |
				PLAYERINITSTATE_SHOPITEM_USEDINFO |
				PLAYERINITSTATE_PET_INFO|
				PLAYERINITSTATE_TITAN_INFO,
				MP_SURYUN_ENTER_SYN);

			

			// do nothing

		yCASE(MP_SURYUN_LEAVE_SYN)
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)	return;

			CCharMove::InitMove(pPlayer,&pPlayer->GetMoveInfo()->CurPosition);
			pPlayer->SetInitState(
				PLAYERINITSTATE_MUGONG_INFO |
				PLAYERINITSTATE_ITEM_INFO |
				PLAYERINITSTATE_ITEM_RARE_OPTION_INFO |
				PLAYERINITSTATE_ITEM_OPTION_INFO |
				PLAYERINITSTATE_PET_INFO |
				PLAYERINITSTATE_ABILITY_INFO |
//				PLAYERINITSTATE_GMCONTROL_INFO |
				PLAYERINITSTATE_HERO_INFO |
				PLAYERINITSTATE_PYOGUK_INFO |
				PLAYERINITSTATE_SHOPITEM_USEDINFO|
				PLAYERINITSTATE_PET_INFO|
				PLAYERINITSTATE_TITAN_INFO,
				MP_SURYUN_LEAVE_SYN);

// MP_USERCONN_GAMEIN_SYN에서 처럼 quest정보 얻기....(jsd04.09.16)
			// 항상 서브퀘스트 정보부터 읽어야 함...
//			QuestMainQuestLoad(pmsg->dwObjectID);
			QuestSubQuestLoad(pmsg->dwObjectID);
//			QuestItemload(pmsg->dwObjectID);
//---------------------------------------------------------------
		
	yENDSWITCH
}

MONEYTYPE CSuryunManager::GetSuryunFee(SURYUNINFO* pInfo,CPlayer* pPlayer)
{
#ifdef _CHINA_LOCAL_
	return pInfo->Fee;
#endif	
//	if( g_pServerSystem->GetNation() == eNATION_CHINA )
//		return pInfo->Fee;

	if(pPlayer->GetMugongNum() == 0)
		return 0;
	else
		return pInfo->Fee;
}

#endif

