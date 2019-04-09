// Battle_Suryun.cpp: implementation of the CBattle_Suryun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Battle_Suryun.h"

#include "Object.h"
#include "BattleTeam_Suryun.h"
#include "../[CC]Suryun/SuryunDefine.h"

#ifdef _MAPSERVER_
#include "RecallManager.h"
#include "UserTable.h"
#include "Player.h"
#include "Monster.h"
#include "CharMove.h"
#include "../[CC]Header/GameResourceManager.h"
#include "ObjectStateManager.h"
#include "SkillManager_Server.h"
#include "SuryunRegen.h"
#include "SuryunRegenManager.h"
#include "../[CC]Suryun/SuryunManager_server.h"

#else	// _MHCLIENT_
#include "ObjectManager.h"
#include "./interface/cWindowManager.h"
#include "MPMissionDialog.h"
#include "MPNoticeDialog.h"
#include "MPGuageDialog.h"
#include "GameIn.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "WindowIDEnum.h"
#include "../[CC]Suryun/SuryunManager_client.h"
#include "Effect/EffectManager.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define BATTLE_SURYUN_READYTIME		10000
#define BATTLE_SURYUN_RESULTTIME	10000

CBattle_Suryun::CBattle_Suryun()
{
	m_bDestroyed = FALSE;
}

CBattle_Suryun::~CBattle_Suryun()
{
//	if(m_bDestroyed == FALSE)
//		printf("ERROR!!!!! NOT DESTROYED");
}

void CBattle_Suryun::Initialize(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2)
{
	CBattle::Initialize(pCreateInfo,&m_PlayerTeam,&m_MonsterTeam);
	m_bDestroyed = FALSE;
	m_pPlayer = NULL;
	m_BattleInfo.BattleState = eBATTLE_STATE_READY;
}

BOOL CBattle_Suryun::IsEnemy(CObject* pOperator,CObject* pTarget)
{
	if(m_BattleInfo.BattleState == eBATTLE_STATE_RESULT)
		return FALSE;
	
	if(pOperator->GetObjectKind() == eObjectKind_Player)
		return pTarget->GetObjectKind() & eObjectKind_Monster;
	else if(pOperator->GetObjectKind() & eObjectKind_Monster)
		return pTarget->GetObjectKind() == eObjectKind_Player;

	return FALSE;
}
BOOL CBattle_Suryun::IsFriend(CObject* pOperator,CObject* pTarget)
{ 
	if(pOperator->GetObjectKind() == eObjectKind_Player)
		return pTarget->GetObjectKind() & eObjectKind_Monster;

	return TRUE;
}

#ifdef _MAPSERVER_

void CBattle_Suryun::GetBattleInfo(char* pInfo,WORD* size)
{
	memcpy(pInfo,&m_BattleSuryunInfo,sizeof(BATTLE_INFO_SURYUN));
	*size = sizeof(BATTLE_INFO_SURYUN);
}
#endif


// event func
void CBattle_Suryun::OnCreate(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2)
{
#ifdef _MHCLIENT_
	WINDOWMGR->CreateMPMissionDlg();
	WINDOWMGR->CreateMPNoticeDlg();
	WINDOWMGR->CreateMPGuageDlg();
	GAMEIN->GetMPMissionDialog()->SetActive(TRUE);
	
	m_ResultStartTime = 0;
	m_bSetResultTime = FALSE;
#endif

	m_dwCreateTime = gCurTime;
	BATTLE_INFO_SURYUN* pInfo = (BATTLE_INFO_SURYUN*)pCreateInfo;
	memcpy(&m_BattleSuryunInfo,pInfo,sizeof(BATTLE_INFO_SURYUN));
	m_pPlayer = NULL;
	m_bResultFirst = TRUE;
	SURYUNMISSIONINFO* pMissionInfo = SURYUNMGR->GetMissionInfo(pInfo->MugongIdx);
	ASSERT(pMissionInfo);
	if(pMissionInfo == NULL)
		return;

	m_pSuryunInfo = pMissionInfo->GetSuryunInfo(pInfo->CurrentSung);

	m_KilledCount = 0;
	
#ifdef _MAPSERVER_
	m_pRegen = SURYUNREGENMGR->MakeNewRegen(
		m_pSuryunInfo->RegenNum,
		pInfo->BattleID,
		pInfo->MonsterKindInfo);
#endif

	CBattle::OnCreate( pCreateInfo, pTeam1, pTeam2 );	
}

void CBattle_Suryun::OnFightStart()
{
#ifdef _MAPSERVER_
	m_pRegen->StartProcess();
#else
	m_BattleInfo.BattleTime = gCurTime;
	SURYUNMGR->SetActiveDlg(FALSE);
#endif

	CBattle::OnFightStart();
}
void CBattle_Suryun::OnDestroy()
{
	m_bDestroyed = TRUE;
#ifdef _MAPSERVER_
	SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());
	if(m_pPlayer)
	{		
		CCharMove::ReleaseMove(m_pPlayer);
								
		m_pPlayer->SetBattle(m_BattleSuryunInfo.ReturnBattleID,0);
		m_pPlayer->SetGridID(m_BattleSuryunInfo.ReturnGridID);
		m_pPlayer->GetMoveInfo()->CurPosition = m_BattleSuryunInfo.ReturnPosition;
		
		MSG_DWORD msg;
		msg.Category = MP_SURYUN;
		msg.Protocol = MP_SURYUN_RETURNTOREALWORLD;
		msg.dwData = GAMERESRCMNGR->GetLoadMapNum();		
		m_pPlayer->SendMsg(&msg,sizeof(msg));

		if(m_pPlayer->GetState() == eObjectState_Die)
			OBJECTSTATEMGR_OBJ->EndObjectState(m_pPlayer,eObjectState_Die);
		m_pPlayer->SetLife(m_pPlayer->GetMaxLife(),FALSE);
		m_pPlayer->SetShield(m_pPlayer->GetMaxShield(),FALSE);
		m_pPlayer->SetNaeRyuk(m_pPlayer->GetMaxNaeRyuk(),FALSE);
	}
	SURYUNREGENMGR->ReleaseRegen(m_BattleInfo.BattleID);
#endif

	RemoveAllMonster();

	m_PlayerTeam.Release();
	m_MonsterTeam.Release();

	//CBattle::OnDestroy();
}
void CBattle_Suryun::OnTeamMemberAdd(int Team,DWORD MemberID,char* Name)
{
#ifdef _MAPSERVER_
	CObject* pObj = g_pUserTable->FindUser(MemberID);
#else
	CObject* pObj = OBJECTMGR->GetObject(MemberID);
#endif
	
	if(pObj == NULL)
	{
		//MessageBox(NULL,"OnTeamMemberAdd is NULL",NULL,NULL);
		ASSERT(0);
		return;
	}

	if(pObj->GetObjectKind() == eObjectKind_Player)
	{
		ASSERT(m_pPlayer == NULL);
		m_pPlayer = (CPlayer*)pObj;
		m_BattleInfo.BattleTime = gCurTime;
#ifdef _MAPSERVER_
		m_pRegen->SetPlayer(m_pPlayer);
#endif
	}
}
BOOL CBattle_Suryun::OnTeamMemberDelete(int Team,DWORD MemberID,char* Name)
{
	if(IsDestroyed())
		return FALSE;

	ASSERT(m_pPlayer);
	if(MemberID == m_pPlayer->GetID())
	{
		SetDestroyFlag();
		m_pPlayer = NULL;
	}

	return FALSE;
}
BOOL CBattle_Suryun::OnTeamMemberDie(int Team,DWORD VictimMemberID,DWORD KillerID)
{
#ifdef _MAPSERVER_
	CObject* pObj = g_pUserTable->FindUser(VictimMemberID);
	if(Team == eBattleTeam1)	// 플레이어가 죽었을경우
	{
		OBJECTSTATEMGR_OBJ->StartObjectState(pObj,eObjectState_Die,KillerID);	
		((CPlayer*)pObj)->m_bNeedRevive = FALSE;
		Victory(eBattleTeam2,eBattleTeam1);		// 몬스터의 승리
		return TRUE;
	}
	else
	{
		ASSERT(pObj->GetObjectKind() & eObjectKind_Monster);
		CMonster* pMonster = (CMonster*)pObj;
		if(pMonster->GetMonsterKind() == COMPLETELY_ALL_RECOVER)
		{
			m_pPlayer->SetLife(m_pPlayer->GetMaxLife());
			m_pPlayer->SetShield(m_pPlayer->GetMaxShield());
			m_pPlayer->SetNaeRyuk(m_pPlayer->GetMaxNaeRyuk());
		}
		else if(pMonster->GetMonsterKind() == NORMAL_LIFE_RECOVER)
			m_pPlayer->SetLife((DWORD)(m_pPlayer->GetLife()+m_pPlayer->GetMaxLife() *0.4));
		else if(pMonster->GetMonsterKind() == NORMAL_SHIELD_RECOVER)
			m_pPlayer->SetShield((DWORD)(m_pPlayer->GetShield()+m_pPlayer->GetMaxShield() *0.4));
		else if(pMonster->GetMonsterKind() == NORMAL_NAERYUK_RECOVER)
			m_pPlayer->SetNaeRyuk((DWORD)(m_pPlayer->GetNaeRyuk()+m_pPlayer->GetMaxNaeRyuk() *0.4));
		else
		{
			++m_KilledCount;
			m_pRegen->DecreaseMonster(pMonster->GetSuryunGroup());
			MSG_DWORD msg;
			SetProtocol(&msg,MP_SURYUN,MP_SURYUN_SUCCESSPERCENT);
			msg.dwData = (100*m_KilledCount) / m_pSuryunInfo->MonNum;
			if(msg.dwData > 100)
				msg.dwData = 100;
			m_pPlayer->SendMsg(&msg,sizeof(msg));
		}
	}
#endif
	
	return FALSE;
}
void CBattle_Suryun::OnTeamObjectCreate(int Team,CBattleObject* pBattleObject)
{
}
void CBattle_Suryun::OnTeamObjectDestroy(int Team,CBattleObject* pBattleObject)
{
}
void CBattle_Suryun::OnTeamMemberRevive(int Team,CObject* pReviver)
{
}
void CBattle_Suryun::OnMonsterDistribute(CMonster* pMonster,CObject* pKiller)
{
}

void CBattle_Suryun::OnTick()
{
	DWORD AliveTime = gCurTime - m_dwCreateTime;
	if(AliveTime > 10*60*1000)
	{
		//YH임시
		m_BattleInfo.BattleState = eBATTLE_STATE_RESULT;
		m_BattleInfo.BattleTime = gCurTime + 1;
	}

	ySWITCH(m_BattleInfo.BattleState)
		yCASE(eBATTLE_STATE_READY)			
			if(m_pPlayer == NULL)
				return;

			DWORD ElapTime = gCurTime - m_BattleInfo.BattleTime;
			if( ElapTime > BATTLE_SURYUN_READYTIME)
			{
				StartBattle();
			}

		yCASE(eBATTLE_STATE_FIGHT)
#ifdef _MAPSERVER_
			m_pRegen->Process();

			Judge();
#endif

		yCASE(eBATTLE_STATE_RESULT)
#ifdef _MAPSERVER_
			DWORD ElapTime = gCurTime - m_BattleInfo.BattleTime;
			if(m_bResultFirst && ElapTime > BATTLE_SURYUN_RESULTTIME)
			{
				SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());
				RemoveAllMonster();
				m_bResultFirst = FALSE;
				SetDestroyFlag();
			}

#endif
	
	yENDSWITCH
}

// 승패 판정
BOOL CBattle_Suryun::Judge()
{
	if(m_KilledCount >= m_pSuryunInfo->MonNum)
	{
		Victory(eBattleTeam1,eBattleTeam2);
		return TRUE;
	}
	if(gCurTime - m_BattleInfo.BattleTime > m_pSuryunInfo->LimitTime)
	{
		Draw();
		return TRUE;
	}

	return FALSE;	
}
void CBattle_Suryun::Victory(int WinnerTeamNum,int LoserTeamNum)
{
#ifdef _MAPSERVER_
	SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());
	if(WinnerTeamNum == eBattleTeam1)
	{
		m_pPlayer->AddExpPointToMugong(m_BattleSuryunInfo.MugongIdx,1,TRUE);		
	}
	else
	{
#ifdef _CHINA_LOCAL_
//		if( g_pServerSystem->GetNation() == eNATION_CHINA )
		if( m_pPlayer->GetMugongNum() == 0 )
			m_pPlayer->SetMoney( m_pSuryunInfo->Fee, MONEY_ADDITION, 0, 
				eItemTable_Inventory, eMoneyLog_GetSuryunFail, m_BattleSuryunInfo.MugongIdx );
#endif
	}	
#endif
	CBattle::Victory(WinnerTeamNum,LoserTeamNum);
}
void CBattle_Suryun::Draw()
{
#ifdef _MAPSERVER_

#ifdef _CHINA_LOCAL_
//	if( g_pServerSystem->GetNation() == eNATION_CHINA )
	if( m_pPlayer->GetMugongNum() == 0 )
		m_pPlayer->SetMoney( m_pSuryunInfo->Fee, MONEY_ADDITION, 0, 
			eItemTable_Inventory, eMoneyLog_GetSuryunFail, m_BattleSuryunInfo.MugongIdx );
#endif

	SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());
#endif
	CBattle::Draw();
}

void CBattle_Suryun::RemoveAllMonster()
{
#ifdef _MAPSERVER_
	ToEachTeam(pTeam)
		YHTPOSITION pos = pTeam->GetPositionHead();
		while(CObject* pObject = pTeam->GetNextTeamMember(&pos))
		{
			if(pObject->GetObjectKind() & eObjectKind_Monster)
			{
				g_pServerSystem->RemoveMonster(pObject->GetID());
			}
		}
	EndToEachTeam	
#endif		

}


// Render
#ifdef _MHCLIENT_


#include "interface/cFont.h"
void CBattle_Suryun::Render()
{
	DWORD ElapsedTime = gCurTime-m_BattleInfo.BattleTime;
//	RECT rect;
//	SetRect(&rect,30,100,500,150);
//	char temp[256];
//	sprintf(temp,"Suryun Battle");
//	CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);
	ySWITCH(m_BattleInfo.BattleState)
//		yCASE(eBATTLE_STATE_READY)
//			rect.top += 15;
//			rect.bottom += 15;
//			sprintf(temp,"Ready");
//			CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);
			
		yCASE(eBATTLE_STATE_FIGHT)
//			rect.top += 15;
//			rect.bottom += 15;
//			sprintf(temp,"Fight");
//			CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);
			
			DWORD A = m_pSuryunInfo->LimitTime;
			DWORD RemainTime = A > ElapsedTime ? A - ElapsedTime : 0;
			GAMEIN->GetMPGuageDialog()->SetTime(RemainTime);

		yCASE(eBATTLE_STATE_RESULT)
//			rect.top += 15;
//			rect.bottom += 15;
//			sprintf(temp,"Result");
			
			if(m_bSetResultTime == FALSE)
			{
				m_ResultStartTime = gCurTime;
				m_bSetResultTime = TRUE;
				m_bResultEffect = FALSE;
			}
//			CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);
			if(gCurTime - m_ResultStartTime > MSGBOXTIME)
			{
				if(m_WinnerTeam == eBattleTeam1)
				{
					if(m_bResultEffect == FALSE)
					{
						// magi82(4) - Titan(071023) 타이탄일때 이펙트 처리(수련장 성공이펙트)
						if(HERO->InTitan() == TRUE)
							EFFECTMGR->StartEffectProcess(eEffect_TitanLevelUpSentence,HERO,NULL,0,0);
						else
							EFFECTMGR->StartEffectProcess(eEffect_LevelUpSentence,HERO,NULL,0,0);

						m_bResultEffect = TRUE;
					}
					WINDOWMGR->MsgBox( MBI_MPNOTICE_SUCCESS, MBT_NOBTN, CHATMGR->GetChatMsg(663) );
				}
				else	
					WINDOWMGR->MsgBox( MBI_MPNOTICE_FAIL, MBT_NOBTN, CHATMGR->GetChatMsg(664) );
			}

	yENDSWITCH	
}

#endif
