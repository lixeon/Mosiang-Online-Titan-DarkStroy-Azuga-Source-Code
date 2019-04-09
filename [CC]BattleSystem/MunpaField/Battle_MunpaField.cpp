// Battle_MunpaField.cpp: implementation of the CBattle_MunpaField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Battle_MunpaField.h"
#include "BattleTeam_MunpaField.h"
#include "BattleObject.h"
#include "Object.h"
#include "Player.h"

#ifdef _MAPSERVER_
#include "UserTable.h"
#include "ServerTable.h"
#include "CharMove.h"
#include "ObjectStateManager.h"
#include "PackedData.h"
#include "SkillManager_Server.h"

#else
#include "ChatManager.h"
#endif

#define BATTLE_MUNPAFIELD_READYTIME		30000
#define BATTLE_MUNPAFIELD_FIGHTTIME		300000
#define BATTLE_MUNPAFIELD_RESULTTIME	10000


VECTOR3 team1Pos,team2Pos;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattle_MunpaField::CBattle_MunpaField()
{
	m_BattleFlag = BATTLE_FLAG_LOGINBATTLE;
	SetVector3(&team1Pos,23000,0,25000);
	SetVector3(&team2Pos,27000,0,25000);
}

CBattle_MunpaField::~CBattle_MunpaField()
{

}


//////////////////////////////////////////////////////////////////////////
// { start virtual func 반드시 오버라이딩 해야함

// Battle 정보 관련
#ifdef _MAPSERVER_
void CBattle_MunpaField::GetBattleInfo(char* pInfo,WORD* size)
{
	MSG_BATTLESETTING_SYN_MUNPAFIELD* info = (MSG_BATTLESETTING_SYN_MUNPAFIELD*)pInfo;
	memcpy(info,&m_BattleInfo,sizeof(BATTLE_INFO_BASE));
	
	ToEachTeam(pTeam)
		CBattleTeam_MunpaField* pTeamMF = (CBattleTeam_MunpaField*)pTeam;
		pTeamMF->GetTeamInfo(&info->TeamInfo[TeamNumber]);
	EndToEachTeam
	
	*size = sizeof(MSG_BATTLESETTING_SYN_MUNPAFIELD);
}
#endif

// 적,아군 구별
BOOL CBattle_MunpaField::IsEnemy(CObject* pOperator,CObject* pTarget)
{
	if(m_BattleInfo.BattleState != eBATTLE_STATE_FIGHT)
		return FALSE;
		
	if(pOperator->GetBattleID() != pTarget->GetBattleID())
		return FALSE;
	
	if(pOperator->GetBattleTeam() == pTarget->GetBattleTeam())
		return FALSE;
	
	return TRUE;
}
BOOL CBattle_MunpaField::IsFriend(CObject* pOperator,CObject* pTarget)
{
	if(m_BattleInfo.BattleState != eBATTLE_STATE_FIGHT)
		return FALSE;

#ifdef _MAPSERVER_
	if(pOperator->GetBattleID() != pTarget->GetBattleID())
		return FALSE;
	
	if(pOperator->GetBattleTeam() != pTarget->GetBattleTeam())
		return FALSE;
	
	return TRUE;

#else if _MHCLIENT_
	
	return FALSE;
#endif
}

// event func
void CBattle_MunpaField::OnCreate(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2)
{
#ifdef _MAPSERVER_
	MSG_BATTLESETTING_SYN_MUNPAFIELD* pInfo = (MSG_BATTLESETTING_SYN_MUNPAFIELD*)pCreateInfo;
//	m_RequestMapNum = pInfo->RequestMapNum;
#endif

	CBattle::OnCreate(pCreateInfo,pTeam1,pTeam2);
}
void CBattle_MunpaField::OnFightStart()
{
	CBattle::OnFightStart();
}
void CBattle_MunpaField::OnDestroy()
{
}
void CBattle_MunpaField::OnTeamMemberAdd(int Team,DWORD MemberID,char* Name)
{
#ifdef _MAPSERVER_
	CObject* pObject = g_pUserTable->FindUser(MemberID);
	ASSERT(pObject);
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pPlayer = (CPlayer*)pObject;

		if(Team == eBattleTeam1)
		{
			CCharMove::SetPosition(pPlayer,&team1Pos);
		}
		else
		{
			CCharMove::SetPosition(pPlayer,&team2Pos);
		}
		CCharMove::CorrectPlayerPosToServer(pPlayer);
	}
	
#else
	CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(451), Name, Team );
#endif
	
	CBattleTeam_MunpaField* pTeam = GetTeam(Team);
	MUNPAFIELD_TEAMINFO* pTeamInfo = pTeam->GetTeamInfo();
	BATTLE_TEAMMEMBER_INFO* pMemberInfo = pTeamInfo->FindMember(MemberID);

	for(int n=0;n<MAX_MUNPAFIELD_MEMBERNUM;++n)
	{
		if(pTeamInfo->TeamMember[n].MemberID == MemberID)
		{
			pTeamInfo->TeamMember[n].set(MemberID,Name);
			break;
		}
	}
}
BOOL CBattle_MunpaField::OnTeamMemberDelete(int Team,DWORD MemberID,char* Name)
{
#ifdef _MHCLIENT_
	CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(452), Name, Team );
#endif
	//////////////////////////////////////////////////////////////////////////
	// 임시 테스트용
	CBattleTeam_MunpaField* pTeam = GetTeam(Team);
	MUNPAFIELD_TEAMINFO* pTeamInfo = pTeam->GetTeamInfo();
	BATTLE_TEAMMEMBER_INFO* pMemberInfo = pTeamInfo->FindMember(MemberID);
	ASSERT(pMemberInfo);
	pMemberInfo->MemberID = 0;
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}
BOOL CBattle_MunpaField::OnTeamMemberDie(int Team,DWORD VictimMemberID,DWORD KillerID)
{
	ASSERT(Team < eBattleTeam_Max);
	
#ifdef _MAPSERVER_
	CObject* pVictimMember = g_pUserTable->FindUser(VictimMemberID);
	CObject* pKiller = g_pUserTable->FindUser(KillerID);
	if(pVictimMember == NULL || pKiller == NULL)
	{
		ASSERT(0);
		return FALSE;
	}
	
	ASSERT(pVictimMember->GetBattleID() == GetBattleID());
	if(pKiller->GetBattleID() != GetBattleID())
		return FALSE;

	OBJECTSTATEMGR_OBJ->StartObjectState(pVictimMember,eObjectState_Die,pKiller->GetID());
	OBJECTSTATEMGR_OBJ->EndObjectState(pVictimMember,eObjectState_Die,PLAYERREVIVE_TIME);	
	
	MSG_DWORD3 msg;
	SetProtocol(&msg,MP_BATTLE,MP_BATTLE_TEAMMEMBER_DIE_NOTIFY);
	msg.dwData1 = Team;
	msg.dwData2 = VictimMemberID;
	msg.dwData3 = KillerID;
	ToEachTeam(pTeam)
		pTeam->SendTeamMsg(&msg,sizeof(msg));
	EndToEachTeam
#endif	

	MUNPAFIELD_TEAMINFO* pKillTeam = GetTeam(GetOtherTeamNum(Team))->GetTeamInfo();
	MUNPAFIELD_TEAMINFO* pDiedTeam = GetTeam(Team)->GetTeamInfo();

	++pKillTeam->TotalKill;
	++pDiedTeam->TotalDied;

	BATTLE_TEAMMEMBER_INFO* pKillerInfo = pKillTeam->FindMember(KillerID);
	BATTLE_TEAMMEMBER_INFO* pVictimInfo = pDiedTeam->FindMember(VictimMemberID);

	ASSERT(pKillerInfo);
	ASSERT(pVictimInfo);

	++pKillerInfo->MemberKillNum;
	++pVictimInfo->MemberDieNum;


	return TRUE;
}
void CBattle_MunpaField::OnTeamMemberRevive(int Team,CObject* pReviver)
{
#ifdef _MAPSERVER_
	VECTOR3 pos;
	if(Team == eBattleTeam1)
		pos = team1Pos;
	else
		pos = team2Pos;

	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = pReviver->GetID();
	msg.dwMoverID = pReviver->GetID();
	msg.cpos.Compress(&pos);
	
	CCharMove::SetPosition(pReviver,&pos);

	DWORD MaxLife = pReviver->GetMaxLife();
	DWORD MaxNaeryuk = pReviver->GetMaxNaeRyuk();
	pReviver->SetLife((DWORD)(MaxLife));
	pReviver->SetNaeRyuk((DWORD)(MaxNaeryuk));
	
	ToEachTeam(pTeam)
		pTeam->SendTeamMsg(&msg,sizeof(msg));
	EndToEachTeam
#endif
}
void CBattle_MunpaField::OnTeamObjectCreate(int Team,CBattleObject* pBattleObject)
{
}
void CBattle_MunpaField::OnTeamObjectDestroy(int Team,CBattleObject* pBattleObject)
{
}


void CBattle_MunpaField::OnTick()
{
	ySWITCH(m_BattleInfo.BattleState)
		yCASE(eBATTLE_STATE_READY)
			DWORD ElapTime = gCurTime - m_BattleInfo.BattleTime;
			if( ElapTime > BATTLE_MUNPAFIELD_READYTIME)
			{
				/*
				ToEachTeam(pTeam)
					if(pTeam->GetAliveTeamMemberNum() == 0)
						return;
				EndToEachTeam
				*/
				StartBattle();
			}

		yCASE(eBATTLE_STATE_FIGHT)
			Judge();

		yCASE(eBATTLE_STATE_RESULT)
#ifdef _MAPSERVER_
			DWORD ElapTime = gCurTime - m_BattleInfo.BattleTime;
			if( ElapTime > BATTLE_MUNPAFIELD_RESULTTIME)
			{
				MSGBASE msg;
				msg.Category = MP_MURIMNET;
				msg.Protocol = MP_MURIMNET_RETURNTOMURIMNET_ACK;
				
				ToEachTeam(pTeam)
					pTeam->SendTeamMsg(&msg,sizeof(msg));
				EndToEachTeam
				
				ToEachTeam(pTeam)
					YHTPOSITION pos = pTeam->GetPositionHead();
					while(CObject* pObject = pTeam->GetNextTeamMember(&pos))
					{
						g_pServerSystem->RemovePlayer(pObject->GetID());
					}
				EndToEachTeam

				SetDestroyFlag();
			}
#endif
	
	yENDSWITCH
}

// 승패 판정
BOOL CBattle_MunpaField::JudgeOneTeamWinsOtherTeam(int TheTeam,int OtherTeam)
{
	CBattleTeam_MunpaField* pTheTeam = GetTeam(TheTeam);
	CBattleTeam_MunpaField* pOtherTeam = GetTeam(OtherTeam);
	MUNPAFIELD_TEAMINFO* pTheTeamInfo = pTheTeam->GetTeamInfo();
	MUNPAFIELD_TEAMINFO* pOtherTeamInfo = pOtherTeam->GetTeamInfo();
	/*
	if(pOtherTeam->GetAliveTeamMemberNum() == 0)
	{
		Victory(TheTeam,OtherTeam);
		return TRUE;
	}
	*/
	if(pTheTeamInfo->TotalKill >= 50)
	{
		Victory(TheTeam,OtherTeam);
		return TRUE;
	}

	return FALSE;
}

BOOL CBattle_MunpaField::Judge()
{
	if(JudgeOneTeamWinsOtherTeam(eBattleTeam1,eBattleTeam2) == TRUE)
		return TRUE;

	if(JudgeOneTeamWinsOtherTeam(eBattleTeam2,eBattleTeam1) == TRUE)
		return TRUE;

	DWORD ElapsedTime = gCurTime - m_BattleInfo.BattleTime;
	if(ElapsedTime > BATTLE_MUNPAFIELD_FIGHTTIME)
		Draw();

	return FALSE;
}
void CBattle_MunpaField::Victory(int WinnerTeamNum,int LoserTeamNum)
{
	CBattle::Victory(WinnerTeamNum,LoserTeamNum);

#ifdef _MAPSERVER_	
	SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());
	
	MSG_DWORD2 msg;
	msg.Category = MP_MURIMNET;
	msg.Protocol = MP_MURIMNET_NOTIFYTOMN_GAMEEND;
	msg.dwData1 = GetBattleID();
	msg.dwData2 = WinnerTeamNum;

	g_pServerTable->SetPositionHead();
	SERVERINFO* pAgentInfo = g_pServerTable->GetNextAgentServer();
	if(pAgentInfo == NULL)
	{
		ASSERT(0);
		return;
	}

	PACKEDDATA_OBJ->SendToMapServer(pAgentInfo->dwConnectionIndex,99,&msg,sizeof(msg));

#endif
}
void CBattle_MunpaField::Draw()
{
	CBattle::Draw();

#ifdef _MAPSERVER_
	SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());

	MSG_DWORD2 msg;
	msg.Category = MP_MURIMNET;
	msg.Protocol = MP_MURIMNET_NOTIFYTOMN_GAMEEND;
	msg.dwData1 = GetBattleID();
	msg.dwData2 = 2;
	
	g_pServerTable->SetPositionHead();
	SERVERINFO* pAgentInfo = g_pServerTable->GetNextAgentServer();
	if(pAgentInfo == NULL)
	{
		ASSERT(0);
		return;
	}

	PACKEDDATA_OBJ->SendToMapServer(pAgentInfo->dwConnectionIndex,99,&msg,sizeof(msg));
#endif
}



#ifdef _MHCLIENT_

#include "interface/cFont.h"
void CBattle_MunpaField::Render()
{
	RECT rect;
	SetRect(&rect,30,100,500,150);
	char temp[256];
	sprintf(temp,"MunpaField Battle");
	CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);

	SetRect(&rect,30,120,500,150);
	DWORD ElapsedTime = gCurTime-m_BattleInfo.BattleTime;
	ySWITCH(m_BattleInfo.BattleState)
		yCASE(eBATTLE_STATE_READY)
			DWORD RemainTime = BATTLE_MUNPAFIELD_READYTIME > ElapsedTime ? BATTLE_MUNPAFIELD_READYTIME - ElapsedTime : 0;
			sprintf(temp,"Battle Start in %d sec",RemainTime / 1000);
		yCASE(eBATTLE_STATE_FIGHT)
			DWORD RemainTime = BATTLE_MUNPAFIELD_FIGHTTIME > ElapsedTime ? BATTLE_MUNPAFIELD_FIGHTTIME - ElapsedTime : 0;
			sprintf(temp,"Battle End in %d sec",RemainTime / 1000);
		yCASE(eBATTLE_STATE_RESULT)
			if(m_WinnerTeam == eBattleTeam_Max)
				sprintf(temp,"Battle Result Draw");
			else
				sprintf(temp,"Battle Result %d Team Wins",m_WinnerTeam);
	yENDSWITCH
	
	CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);


	MUNPAFIELD_TEAMINFO* pTeam1 = GetTeam(eBattleTeam1)->GetTeamInfo();
	MUNPAFIELD_TEAMINFO* pTeam2 = GetTeam(eBattleTeam2)->GetTeamInfo();

	sprintf(temp, "Total Score is %d(%d) : %d(%d)",
		pTeam1->TotalKill,pTeam1->TotalDied,
		pTeam2->TotalKill,pTeam2->TotalDied);
	SetRect(&rect,450,30,600,50);	

	CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);

	int pos,n;
	//////////////////////////////////////////////////////////////////////////
	// team1 member
	pos = 0;
	for(n=0;n<MAX_MUNPAFIELD_MEMBERNUM;++n)
	{
		BATTLE_TEAMMEMBER_INFO* pMemberInfo = &pTeam1->TeamMember[n];
		if(pMemberInfo->MemberID)
		{
			sprintf(temp,"%s (%d-%d)",
				pMemberInfo->MemberName,pMemberInfo->MemberKillNum,pMemberInfo->MemberDieNum);
			SetRect(&rect,750,pos*15+15,850,pos*15+30);	
			CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);
			pos++;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// team2 member
	pos = 0;
	for(n=0;n<MAX_MUNPAFIELD_MEMBERNUM;++n)
	{
		BATTLE_TEAMMEMBER_INFO* pMemberInfo = &pTeam2->TeamMember[n];
		if(pMemberInfo->MemberID)
		{
			sprintf(temp,"%s (%d-%d)",
				pMemberInfo->MemberName,pMemberInfo->MemberKillNum,pMemberInfo->MemberDieNum);
			SetRect(&rect,850,pos*15+15,950,pos*15+30);	
			CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);
			pos++;
		}
	}
}

#endif
// } end virtual func
//////////////////////////////////////////////////////////////////////////