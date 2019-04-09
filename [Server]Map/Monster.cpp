// Monster.cpp: implementation of the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Monster.h"
#include "CharMove.h"
#include "PackedData.h"
#include "GridSystem.h"
#include "Player.h"
#include "UserTable.h"
#include "RegenNpc.h"
#include "ObjectStateManager.h"
#include "AISystem.h"
#include "SkillManager_server.h"
#include "MHError.h"
#include "HelpRequestManager.h"

//SW_TEST
#include "cJackpotManager.h"

#include "TileManager.h"
#include "PathManager.h"

#include "..\[CC]Header\GameResourceManager.h"
#include "RegenPrototype.h"
#include "AIManager.h"
#include "RegenManager.h"
#include "Battle.h"

#include "StatisticsManager.h"
#include "cMonsterSpeechManager.h"
#include "SiegeWarProfitMgr.h"

#include "AbilityManager.h"

#include "GuildManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonster::CMonster()
{
	m_pSInfo = NULL;
	m_pLastAttackPlayer = NULL;
}

CMonster::~CMonster()
{
	m_pSInfo = NULL;
}
BOOL CMonster::Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	CObject::Init(kind, AgentNum, pBaseObjectInfo);
	
	//m_MonsterState = new CStateNPC;
	m_pTObject = NULL;
	m_DropItemId = 0;
	m_dwDropItemRatio = 100;
	
	m_bEventMob = FALSE;

	m_pLastAttackPlayer = NULL;

	return TRUE;
}
void CMonster::Release()
{
	SetTObject(NULL);

	if(m_bEventMob)
	{
		g_pAISystem->ReleaseMonsterID(GetSubID());
		g_pAISystem->ReleaseMonsterID(GetID());
		m_BaseObjectInfo.dwObjectID = 0;
		SetSubID(0);
		m_bEventMob = FALSE;
	}
	m_Distributer.Release();
	
	CObject::Release();
}

void CMonster::InitMonster(MONSTER_TOTALINFO* pTotalInfo)
{
	m_KillerPlayer = 0;
	memcpy(&m_MonsterInfo,pTotalInfo,sizeof(MONSTER_TOTALINFO));
	// 몬스터 스텟 정보 입력
	GAMERESRCMNGR->GetMonsterStats(GetMonsterKind(), &m_mon_stats);


	//m_pState = new CStateMachine;
	//m_pState->InitMachine(this);
	//m_pAIBase = new CAIBase((CObject*)this);
	//m_pAIBase->Init(GAMERESRCMNGR->GetMonsterInfo(GetMonsterKind()));
	//if( GetObjectKind() == eObjectKind_SpecialMonster || GetObjectKind() == eObjectKind_BossMonster)
	{
		m_pSInfo = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind());
	}
	
	m_stateParam.SetState(m_BaseObjectInfo.ObjectState);
	GSTATEMACHINE.SetState( this, eMA_STAND );
	//m_aiParam.Init(GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind()));
	m_aiParam.Init();
	m_MoveInfo.KyungGongIdx = eMA_STAND;

	m_pLastAttackPlayer = NULL;
}

void CMonster::GetMonsterTotalInfo(MONSTER_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_MonsterInfo,sizeof(MONSTER_TOTALINFO));
}


void CMonster::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
{
	SEND_MONSTER_TOTALINFO* pMsg = (SEND_MONSTER_TOTALINFO*)pAddMsg;
	pMsg->AddableInfo.Init();

	pMsg->Category = MP_USERCONN;
	pMsg->Protocol = MP_USERCONN_MONSTER_ADD;
	pMsg->dwObjectID = dwReceiverID;
	GetSendMoveInfo(&pMsg->MoveInfo,&pMsg->AddableInfo);
	GetBaseObjectInfo(&pMsg->BaseObjectInfo);
	GetMonsterTotalInfo(&pMsg->TotalInfo);
	pMsg->bLogin = bLogin;

	*pMsgLen = pMsg->GetMsgLength();
}
DWORD CMonster::GetCurAttackIndex()
{
	return m_pSInfo->AttackIndex[m_aiParam.GetCurAttackKind()];
}

void CMonster::MoveStop()
{
	if(CCharMove::IsMoving(this) == FALSE)
		return;
	
	VECTOR3* pos = CCharMove::GetPosition(this);
	CCharMove::EndMove(this,gCurTime,pos);
	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_STOP;
	msg.dwMoverID = GetID();
	msg.cpos.Compress(pos);

	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
}

// 데미지 입힌 객체 관리 //////////////////////////////
void CMonster::AddDamageObject(CPlayer* pPlayer, DWORD damage, DWORD plusdamage)
{
	if( damage )
		m_Distributer.AddDamageObject(pPlayer, damage, plusdamage);
}

/*
void CMonster::SetPeaceRestTime(DWORD dwMaxTime)
{
	m_dwRestTime = rand()%dwMaxTime;
	
}
void CMonster::SetAttackRestTime(DWORD dwRestTime)
{
	m_dwRestTime = dwRestTime;
	
}
*/

/*
BOOL CMonster::Rest(DWORD dwTickTime)
{
	if(m_dwRestTime == 0)
		return TRUE;
	if(m_dwRestTime < dwTickTime)
	{
		m_dwRestTime = 0;
		return TRUE;
	}
	else
	{
		m_dwRestTime -= dwTickTime;
		return FALSE;
	}
}*/


void CMonster::AddChat(char * str)
{
	TESTMSG pmsg;
	pmsg.Category = MP_CHAT;
	pmsg.Protocol = MP_CHAT_FROMMONSTER_ALL;
	pmsg.dwObjectID = GetID();
	//strcpy(pmsg.Msg, str);
	SafeStrCpy( pmsg.Msg, str, MAX_CHAT_LENGTH+1 );
	//g_Console.LOG(4, pmsg->Msg);

	//CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	//if(!pPlayer) return;

	//PACKEDDATA_OBJ->QuickSend(this,&pmsg,sizeof(pmsg));
	PACKEDDATA_OBJ->QuickSend(this,&pmsg,pmsg.GetMsgLength());	//CHATMSG 040324
}

void CMonster::AddSpeech( DWORD SpeechType, DWORD SpeechIdx )
{
	ASSERT(SpeechIdx>=0);

	MSG_DWORD4 msg;
	msg.Category = MP_CHAT;
	msg.Protocol = MP_CHAT_MONSTERSPEECH;
	msg.dwObjectID = GetID();

	if( SpeechType == eMSpch_Balloon || SpeechType == eMSpch_BallonChat )	//말풍선 or 말풍선+채팅창
	{
		msg.dwData1 = SpeechType;
		msg.dwData2 = SpeechIdx;
		msg.dwData3 = 0;	//사용안함
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(MSG_DWORD4));

		return;
	}
	else if( SpeechType == eMSpch_SmallShout )	//외치기 SMALLSHOUT
	{
		msg.dwData1 = SpeechType;
		msg.dwData2 = SpeechIdx;
		msg.dwData3 = GetID();
		msg.dwData4 = GetMonsterKind();

		g_pUserTable->SetPositionUserHead();
		while( CObject* pObject = g_pUserTable->GetUserData() )
		{
			if( pObject->GetObjectKind() != eObjectKind_Player ) continue;
			
			CPlayer* pReceiver = (CPlayer*)pObject;
			if( GetGridID() == pReceiver->GetChannelID() )
			{
				pReceiver->SendMsg( &msg, sizeof(MSG_DWORD4) );
			}
		}
		return;
	}
	else
	{
		ASSERT(0);
		return;
	}
}

void CMonster::OnStartObjectState(BYTE State,DWORD dwParam)
{
	switch(State)
	{
	case eObjectState_Die:
		m_KillerPlayer = dwParam;
		break;
	case eObjectState_TiedUp_CanSkill:	// 이동 결박
	case eObjectState_TiedUp:	// 이동,공격 모두 결박
		OnStop();
		break;
	}
}

void CMonster::OnEndObjectState(BYTE State)
{
	switch(State)
	{
	case eObjectState_Die:
		g_pServerSystem->RemoveMonster(GetID());		
		break;
	}
}

float CMonster::GetRadius()
{	
	return (float)m_pSInfo->MonsterRadius;
}

void CMonster::StateProcess()
{
	switch(GetState())
	{
	case eObjectState_Die:
		{
			// 2007. 7. 9. CBH - 전문기술 전용 몬스터면 죽었을때 아이탬 드롭 막는다.			
			if( GetObjectKindGroup(this->GetObjectKind()) == eOBJECTKINDGROUP_NONE )
			{
				DWORD RemainTime;
				if(m_ObjectState.State_End_Time >= gCurTime)
					RemainTime = m_ObjectState.State_End_Time - gCurTime;
				else
					RemainTime = 0;
				if(m_KillerPlayer && RemainTime < MONSTERREMOVE_TIME - 1500 )
				{
					CObject* pKiller = g_pUserTable->FindUser(m_KillerPlayer);
					if(pKiller)
						GetBattle()->OnMonsterDistribute(this,pKiller);

					m_KillerPlayer = 0;
				}
			}			
		}
		break;
	}
}

int CMonster::GetObjectTileSize()
{
	float radius = GetRadius();
	if(radius < 50)
		return 0;
	else if(radius < 150)
		return 1;
	else if(radius < 250)
		return 2;
	else
		return 2;
}
BOOL CMonster::SetTObject( CPlayer * pNewTPlayer )
{	
	if(pNewTPlayer)
	{
		ASSERT(pNewTPlayer->GetGridID() == GetGridID());
	}
	
	CPlayer * m_pOldTPlayer = (CPlayer * )m_pTObject;
	if(pNewTPlayer == m_pOldTPlayer) return FALSE;

	if(pNewTPlayer)
	{
		// 데미지 받아 새로운 추적자 설정
		// 검색된 몬스터가 유저 추적

/*//oldplayer는 관심없는데..
		if((m_pOldTPlayer && !m_pOldTPlayer->GetInited() )|| (m_pOldTPlayer && m_pOldTPlayer->GetState() == eObjectState_Die) )
		{
			MHERROR->OutputFile("Debug.txt", 
				MHERROR->GetStringArg("SetTObject old mID:%d,pID:%d,mstate:%d,pstate:%d,mG:%d,pG:%d", 
										GetID(),
										m_pOldTPlayer->GetID(), 
										GetState(), 
										m_pOldTPlayer->GetState(),
										GetGravity(), 
										m_pOldTPlayer->GetGravity()));
			return FALSE;
		}
*/
		if( !pNewTPlayer->GetInited() || pNewTPlayer->GetState() == eObjectState_Die )
		{
			MHERROR->OutputFile("Debug.txt", 
				MHERROR->GetStringArg("SetTObject new mID:%d,pID:%d,mstate:%d,pstate:%d,mG:%d,pG:%d", 
										GetID(),
										pNewTPlayer->GetID(), 
										GetState(), 
										pNewTPlayer->GetState(),
										GetGravity(), 
										pNewTPlayer->GetGravity()));
			return FALSE;
		}

/*
		if(pNewTPlayer->GetGravity() + GetGravity() > 100)
			pNewTPlayer->RemoveFollowAsFarAs(GetGravity());

		if(m_pOldTPlayer)
		{
			m_pOldTPlayer->RemoveFollowList(GetID());
		}
		pNewTPlayer->AddFollowList(this);
*/

//KES 먼저 넣고! 나중에 비중을 계산하도록 변경하자.

		if( pNewTPlayer->AddFollowList(this) == TRUE )
		{
			if(m_pOldTPlayer)
			{
				m_pOldTPlayer->RemoveFollowList(GetID());
			}

			m_pTObject = pNewTPlayer;
		
			DWORD dwGravity = pNewTPlayer->GetGravity();
			if(dwGravity > 100)
			{
				if( pNewTPlayer->RemoveFollowAsFarAs(dwGravity, this) )
					return FALSE;	//자신이 지워졌다.
			}

			//떨어져 나간 몬스터가 자신일 수도 있다!
			
			//SW050903	몬스터대사 관련
			MonSpeechInfo* pTemp = NULL;
			if(m_pSInfo->bForeAttack)	// 선공몹일 경우에만
			{
				pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( GetMonsterKind(), eMon_Speech_ForeAtk );
			}
			else if( !m_pSInfo->bForeAttack && m_stateParam.stateNew == eMA_ATTACK )//!비선공이면서 헬퍼몹이면.. 헬퍼 구분을 어떻게 해야할것인가..
			{
				pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( GetMonsterKind(), eMon_Speech_AboutHelp );
			}
			if( pTemp )
				AddSpeech( pTemp->SpeechType, pTemp->SpeechIndex );
			
			return TRUE;
		}
		
		return FALSE;
	}
	else
	{
		// pNewTPlayer == NULL
		// 몬스터가 추적 포기(rest, walkaround, stand) => 추적 플레이어 줄임
		// 몬스터 죽었을때 => 추적 플레이어 줄임
		if(m_pOldTPlayer)
		{
			m_pOldTPlayer->RemoveFollowList(GetID());
//			m_pOldTPlayer =NULL;
		}

		m_pTObject = NULL;
		m_pLastAttackPlayer = NULL;
		return FALSE;
	}
	return FALSE;

}
void CMonster::DoDamage(CObject* pAttacker,RESULTINFO* pDamageInfo,DWORD beforeLife)
{
	if( pAttacker->GetGridID() != GetGridID() )
	{
//		ASSERT(0);
		return;
	}
	DWORD plusdamage = 0;

//---첫빵
	// 필드보스는 제외
	if(GetObjectKind() != eObjectKind_FieldBossMonster)
		if(beforeLife == GetSMonsterList()->Life && pDamageInfo->RealDamage > 0 )
		{
			plusdamage = (DWORD)(GetSMonsterList()->Life*0.2f);
		}

	if( pDamageInfo->RealDamage > beforeLife)
		pDamageInfo->RealDamage = beforeLife;

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		AddDamageObject((CPlayer*)pAttacker, pDamageInfo->RealDamage, plusdamage);

		if( m_stateParam.stateCur == eMA_REST || m_stateParam.stateCur == eMA_RUNAWAY) return ;

		// 도망?
//		WORD RunawayType = 0;
//		if(CAIManager::IsRunawayStateChange(this, (CPlayer *)pAttacker))
//		{
//			GSTATEMACHINE.SetState(this, eMA_RUNAWAY);
//			return;
//		}

		if(CAIManager::IsTargetChange((CPlayer *)pAttacker, this))
		{
			m_pLastAttackPlayer = (CPlayer*)pAttacker;
			SetTObject((CPlayer *)pAttacker);
		}
	}

	// 도움 요청 
	// YH 헬프 받으면 또 헬프하고 또 헬프 하고 하는 경우가 발생
	// 헬프를 받았을때나 미스(공격거리)때는 핼프요청 안함
	if(pDamageInfo->RealDamage != 0)
		CHelpRequestManager::MonsterRequestProcess(this);

	/*if( m_pTObject )
	{
		if(m_pSInfo->TargetChange)
		{
			SetTObject((CPlayer *)pAttacker);	
		}
	}
	else
	{
		SetTObject((CPlayer *)pAttacker);
	}*/
	
	GSTATEMACHINE.SetState(this, eMA_ATTACK);

}
void CMonster::SetLife(DWORD Life,BOOL bSendMsg)
{
//	ASSERT(Life <= m_pSInfo->Life);
	DWORD maxlife = m_pSInfo->Life;
	if(Life > maxlife)
		Life = maxlife;
	m_MonsterInfo.Life = Life;
}
void CMonster::SetShield(DWORD Shield,BOOL bSendMsg)
{
//	ASSERT(Shield <= m_pSInfo->Shield);
	DWORD maxshield = m_pSInfo->Shield;
	if(Shield > maxshield)
		Shield = maxshield;
	m_MonsterInfo.Shield = Shield;
}
void CMonster::DoDie(CObject* pAttacker)
{
//	g_Console.LOG(4, "Monster Die : GROUPID: %d, ID: %d, NAME: %s", GetMonsterGroupNum(), GetID(), m_pSInfo->Name);

	DWORD AttackerID;
	if(pAttacker == NULL)
		AttackerID = 0;
	else 
		AttackerID = pAttacker->GetID();
	REGENNPC_OBJ->ReleaseMonsterGroup(((CMonster*)this)->GetMonsterGroupNum());
	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,AttackerID);
	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die,MONSTERREMOVE_TIME);

	//SW jackpot

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
//COMMENT:JACKPOT	

		JACKPOTMGR->DoJackpot((CPlayer*)pAttacker, this);

		SWPROFITMGR->AddProfitFromMonster( (CPlayer*)pAttacker, this );

		//SW060719 문파포인트
		GUILDMGR->AddGuildHuntedMonsterCount( (CPlayer*)pAttacker, this );
	}
	
	//SW050902
	MonSpeechInfo* pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( this->GetMonsterKind(), eMon_Speech_Death );
	if( pTemp )
		this->AddSpeech( pTemp->SpeechType, pTemp->SpeechIndex );

	//SW051112 무쌍모드
	//m_pLastAttackPlayer 대신 pAttacker 로..
	if( pAttacker && pAttacker->GetObjectKind()==eObjectKind_Player )
	{
#ifdef _JAPAN_LOCAL_
		((CPlayer*)pAttacker)->AddMussangPointFromLevel(GetLevel());
#else
		// 06. 03 국내무쌍 - 이영준
		if(!(ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_MUSSANG, ((CPlayer*)pAttacker)->GetAbilityGroup())  < 1))
			((CPlayer*)pAttacker)->AddMussangPointFromLevel(GetLevel());
#endif
	}

	m_pLastAttackPlayer = NULL;
	SetTObject(NULL);
	
//	STATISTICSMGR->MonsterDieCount( GetMonsterKind() );	
}

void CMonster::GetSendMoveInfo(SEND_MOVEINFO* pRtInfo,CAddableInfoList* pAddInfoList)
{
	CObject::GetSendMoveInfo(pRtInfo,pAddInfoList);
	pRtInfo->KyungGongIdx = (WORD)m_stateParam.stateCur;	
}

float CMonster::DoGetMoveSpeed()
{
	WORD kind = GetMonsterKind();

	float Speed = 0.0;
	switch(m_stateParam.stateCur)
	{
	case eMA_WALKAROUND:
		{
			Speed = (float)GAMERESRCMNGR->GetMonsterListInfo(kind)->WalkMove;
		}
		break;
	case eMA_RUNAWAY:
		{
			Speed = (float)GAMERESRCMNGR->GetMonsterListInfo(kind)->RunawayMove;
		}
		break;
	case eMA_PERSUIT:
		{
			Speed = (float)GAMERESRCMNGR->GetMonsterListInfo(kind)->RunMove;
		}
		break;
	default:
		{
			Speed = 0;
//			ASSERT(0);
		}
		break;
	}

	return Speed;
}

void CMonster::OnStop()
{
	if(CCharMove::IsMoving(this) == FALSE) return;
	
	VECTOR3* pos = CCharMove::GetPosition(this);
	CCharMove::EndMove(this, gCurTime, pos);
	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_STOP;
	msg.dwMoverID = GetID();
	msg.cpos.Compress(pos);

	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
}

void CMonster::OnMove( VECTOR3 * pPos )
{

	ASSERT(GetState() != eObjectState_Die);

	// 결박 상태이기때문에 못 움직이게 함
	if( GetState() == eObjectState_TiedUp_CanSkill ||
		GetState() == eObjectState_TiedUp )
		return;

	VECTOR3* curpos = CCharMove::GetPosition(this);
	if(m_MoveInfo.bMoving)
	{
		if(m_MoveInfo.GetMaxTargetPosIdx() != 0 && m_MoveInfo.GetTargetPosition(m_MoveInfo.GetMaxTargetPosIdx()-1)->x == pPos->x && m_MoveInfo.GetTargetPosition(m_MoveInfo.GetMaxTargetPosIdx()-1)->z == pPos->z)
			return;
	}
	if(curpos->x == pPos->x && curpos->z == pPos->z)
		return;

	//////////////////////////////////////////////////////////////////////////
	// 타일 및 충돌..
	VECTOR3 tPos;
	BOOL preo = g_pServerSystem->GetMap()->GetTileManager()->IsPreoccupied(this,pPos);
	if(preo)
	{
		tPos = g_pServerSystem->GetMap()->GetTileManager()->GetNextCloserPos(this,pPos,curpos,100,1000);
	}
	else
	{
		tPos = *pPos;
	}

	m_MoveInfo.InitTargetPosition();
//	BOOL bInv = FALSE;
//	if(m_stateParam.stateCur == eMA_PERSUIT)
		//bInv = TRUE;
//gotoPATH:
	switch(PATHMGR->GetPath(&m_MoveInfo.CurPosition,pPos,m_MoveInfo.GetTargetPositionArray(), MAX_CHARTARGETPOSBUF_SIZE, m_MoveInfo.GetMaxTargetPosIdxRef(), this, FALSE))
	{	
	case PATH_BLOCKFAILED:
		{
			GSTATEMACHINE.SetState(this, eMA_RUNAWAY);
			return;

			VECTOR3 IDir;
			IDir.x = -m_MoveInfo.Move_Direction.x;
			IDir.z = -m_MoveInfo.Move_Direction.z;
			BeelinePoint(&m_MoveInfo.CurPosition, &IDir, 1000, pPos);
			OnMove(pPos);
		///	goto gotoPATH;
		}
		break;
	case PATH_FAILED:
		return;
	}

	WORD count = m_MoveInfo.GetMaxTargetPosIdx();

	// jsd 
	if( count == 0 )
		return;
	else if( count == 1 )
	{
		MOVE_ONETARGETPOS_FROMSERVER msg;
		SetProtocol(&msg,MP_MOVE,MP_MOVE_ONETARGET);
		msg.dwObjectID = GetID();
		msg.SetStartPos(curpos);
		msg.SetTargetPos(m_MoveInfo.GetTargetPosition(0));

		PACKEDDATA_OBJ->QuickSend(this,&msg,msg.GetSize());
	}
	else
	{
		MOVE_TARGETPOS msg;
		msg.Init();
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_TARGET;
		msg.dwMoverID = GetID();
		msg.SetStartPos(&m_MoveInfo.CurPosition);
		//msg.tpos.Compress(&TargetPos);
		for(int i=0;i<count;++i)
			msg.AddTargetPos(m_MoveInfo.GetTargetPosition(i));

		PACKEDDATA_OBJ->QuickSend(this,&msg,msg.GetSize());
	}	

	//StartMove(pHero,gCurTime,&TargetPos);
	m_MoveInfo.SetCurTargetPosIdx(0);
	ASSERT(m_MoveInfo.GetCurTargetPosIdx()< m_MoveInfo.GetMaxTargetPosIdx());
	CCharMove::StartMoveEx(this,gCurTime,curpos,m_MoveInfo.GetTargetPosition(0));

}
void CMonster::DoStand()
{
	OnStop();
}
void CMonster::DoRest(BOOL bStart)
{
	OnStop();

}
BOOL CMonster::DoAttack( WORD attackNum )
{
	if( !m_pTObject ) return FALSE;

	OnStop();
	
	// yh
	if( IsInSpecialState(eSpecialState_Stun) )
		return TRUE;

	if( GetState() == eObjectState_TiedUp_CanMove ||
		GetState() == eObjectState_TiedUp )
		return TRUE;

	// LBS 일단 막아봅니다.
	if( GetGridID() != m_pTObject->GetGridID() )
	{
//		ASSERT(0);
		return TRUE;
	}

	if(GetObjectKind() == eObjectKind_FieldBossMonster)
	{
		// 필드보스 - 05.12 이영준
		// 몬스터가 범위 공격을 사용할수 있게 변경
		CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(attackNum);
		VECTOR3 TargetPos, MonPos;
		GetPosition(&MonPos);
		if(pSkillInfo->GetSkillInfo()->TargetRange == 0) // 1 target 
		{
			SKILLMGR->MonsterAttack(attackNum, this, m_pTObject);
			return TRUE;
		}

		if(pSkillInfo->GetSkillInfo()->TargetAreaPivot == 0) // range target
		{			
			TargetPos = *CCharMove::GetPosition(GetTObject());		
			SKILLMGR->MonsterAttack(attackNum, this, &TargetPos,m_pTObject->GetID());
		}
		else // range self
		{			
			SKILLMGR->MonsterAttack(attackNum, this, &MonPos,m_pTObject->GetID());
		}
	}
	else
		SKILLMGR->MonsterAttack(attackNum, this, m_pTObject);

	return TRUE;
}

BOOL CMonster::DoWalkAround()
{
	int ran=0;
	VECTOR3 Target;
	VECTOR3 * pThisPos = CCharMove::GetPosition(this);
	//VECTOR3 domainPoint = RESPAWNMGR->GetOBJ_REGEN(GetID())->vPos;
	//VECTOR3 domainPoint = *GROUPMGR->GetRegenObject(GetMonsterGroupNum(), GetBattleID(), GetID())->GetPos();
	// YH 이벤트리콜이나 수련장등에선 ProtoType 안넣어주는것으로 수정
	CRegenPrototype* pProtoType = REGENMGR->GetPrototype(GetSubID());
	VECTOR3 domainPoint = pProtoType ? pProtoType->vPos : *pThisPos;
	VECTOR3 decisionPoint;
	AdjacentPointForOriginPoint(pThisPos, &domainPoint, (float)10000, &decisionPoint);

	float dx=0,dz=0;
	ran = rand();
	if(m_pSInfo->DomainRange==0)
		m_pSInfo->DomainRange = 1;
	
	dx = float(ran%m_pSInfo->DomainRange) * (ran%2?1:-1);
	ran = rand();
	dz = float(ran%m_pSInfo->DomainRange) * (ran%2?1:-1);
	Target.x = decisionPoint.x + dx;
	Target.z = decisionPoint.z + dz;

	if(Target.x < 10.0f)
		Target.x = 10.0f;
	else if(Target.x > 51200.0f)
		Target.x = 51200.0f;

	if(Target.z < 10.0f)
		Target.z = 10.0f;
	else if(Target.z > 51200.0f)
		Target.z = 51200.0f;
	
	//taiyo 임시처리 
	VECTOR3 CollisonPos;
	
	MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
	if(g_pServerSystem->GetMap()->CollisionCheck(pThisPos,&Target,&CollisonPos,this) == TRUE)
	{
		Target.x = decisionPoint.x - dx;
		Target.z = decisionPoint.z - dz;
	}
	if(g_pServerSystem->GetMap()->CollisionCheck(pThisPos,&Target,&CollisonPos,this) == TRUE)
	{
		Target.x = decisionPoint.x + dx;
		Target.z = decisionPoint.z - dz;
	}
	if(g_pServerSystem->GetMap()->CollisionCheck(pThisPos,&Target,&CollisonPos,this) == TRUE)
	{
		Target.x = decisionPoint.x - dx;
		Target.z = decisionPoint.z + dz;
	}

	//YH2DO
	if(Target.x < 10)
		Target.x = 10;
	else if(Target.x > 51200)
		Target.x = 51200;
	if(Target.z < 10)
		Target.z = 10;
	else if(Target.z > 51200)
		Target.z = 51200;
	
	OnMove(&Target);

	return TRUE;
}

BOOL CMonster::DoPursuit(CObject * pTObject)
{
	CObject * pObj = pTObject;
	if( !m_pTObject)
		return FALSE;
	else
		pObj = m_pTObject;

	if(!pObj) return FALSE;

	VECTOR3 TObjectPos = *CCharMove::GetPosition(pObj);

	OnMove(&TObjectPos);

	return TRUE;
}

void CMonster::DoRunaway()
{
	MONSTEREX_LIST * pEXSInfo = (MONSTEREX_LIST *)m_pSInfo;
	VECTOR3 MovePoint;
	VECTOR3 * pThisPos = CCharMove::GetPosition(this);

	switch(m_aiParam.RunawayType)
	{
	case eRunawayType_Help:
		{
			ASSERT(m_aiParam.pHelperMonster);
			VECTOR3 * pHelperPos = CCharMove::GetPosition(m_aiParam.pHelperMonster);
			MovePoint.x = pHelperPos->x;
			MovePoint.y = 0;
			MovePoint.z = pHelperPos->z;
		}
		break;
	case eRunawayType_General:
		{
			m_aiParam.pHelperMonster = NULL;

			VECTOR3 IDir;
			IDir.x = -m_MoveInfo.Move_Direction.x;
			IDir.z = -m_MoveInfo.Move_Direction.z;
			BeelinePoint(pThisPos, &IDir, pEXSInfo->RunawayDistance, &MovePoint);
		}
		break;
	}
	VECTOR3 Target;

	Target.x = MovePoint.x;
	Target.z = MovePoint.z;

	if(Target.x < 10.0f)
		Target.x = 10.0f;
	else if(Target.x > 51200.0f)
		Target.x = 51200.0f;

	if(Target.z < 10.0f)
		Target.z = 10.0f;
	else if(Target.z > 51200.0f)
		Target.z = 51200.0f;

	//taiyo 임시처리 
	VECTOR3 CollisonPos;
	
	MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
	if(g_pServerSystem->GetMap()->CollisionCheck(pThisPos,&Target,&CollisonPos,this) == TRUE)
	{
		Target.x = MovePoint.x - 100;
		Target.z = MovePoint.z - 100;
	}
	if(g_pServerSystem->GetMap()->CollisionCheck(pThisPos,&Target,&CollisonPos,this) == TRUE)
	{
		Target.x = MovePoint.x + 100;
		Target.z = MovePoint.z - 100;
	}
	if(g_pServerSystem->GetMap()->CollisionCheck(pThisPos,&Target,&CollisonPos,this) == TRUE)
	{
		Target.x = MovePoint.x - 100;
		Target.z = MovePoint.z + 100;
	}


	if(Target.x < 10)
		Target.x = 10;
	if(Target.z < 10)
		Target.z = 10;
	if(Target.x > 51200)
		Target.x = 51200;
	if(Target.z > 51200)
		Target.z = 51200;
	
	OnMove(&Target);
}

CMonster * CMonster::DoFriendSearch(DWORD Range)
{
	return (CMonster *)g_pServerSystem->GetGridSystem()->FindMonsterInRange(CCharMove::GetPosition(this), (float)Range, this, FIND_FIRST );
}

CObject * CMonster::DoSearch()
{
	return g_pServerSystem->GetGridSystem()->FindPlayerInRange(CCharMove::GetPosition(this), (float)m_pSInfo->SearchRange, this,(BYTE)m_pSInfo->TargetSelect );
}

CObject * CMonster::OnCollisionObject()
{
	return g_pServerSystem->GetGridSystem()->FindPlayerInRange(CCharMove::GetPosition(this), (float)m_pSInfo->MonsterRadius, this, FIND_FIRST );
}

void CMonster::Drop()
{
	if( m_MonsterInfo.Group == (WORD)QUESTRECALL_GROUP_ID )	return;

	VECTOR3* KilledPosition = CCharMove::GetPosition(this);
	DWORD TotalLife = GetSMonsterList()->Life;
	m_Distributer.Distribute(m_KillerPlayer,KilledPosition,m_DropItemId, m_dwDropItemRatio, this);
}

void CMonster::AddStatus(CStatus* pStatus)
{
	CObject::AddStatus(pStatus);

	if(pStatus->GetFlag() & STATUS_FLAG_MOVE)
	{
		MoveStop();
		DoPursuit();
	}
}

void CMonster::RemoveStatus(CStatus* pStatus)
{
	CObject::RemoveStatus(pStatus);

	if(pStatus->GetFlag() & STATUS_FLAG_MOVE)
	{
		MoveStop();
		DoPursuit();
	}
}

void CMonster::DistributePerDamage()
{
	m_Distributer.DistributePerDamage(this);
}

// 필드보스 - 05.12 이영준
// 필드보스용 아이템 분배 함수 호출함
void CMonster::DistributeItemPerDamage()
{
	m_Distributer.DistributeItemPerDamage(this);
}

void CMonster::DistributeDamageInit()
{
	m_Distributer.DamageInit();
}

void CMonster::DistributeDeleteDamagedPlayer(DWORD CharacterID)
{
	m_Distributer.DeleteDamagedPlayer(CharacterID);
}


DWORD CMonster::DoGetPhyAttackPowerMin()
{
	BYTE bAttackType = MONSTER_ATTACKTYPE1;

	double physicalAttackPower= 0;

	if(bAttackType == MONSTER_ATTACKTYPE1)
	{
		physicalAttackPower = m_mon_stats.PhysicalAttackMin1;
		int tmp = m_mon_stats.PhysicalAttackMax1-m_mon_stats.PhysicalAttackMin1;
		if(tmp) physicalAttackPower += rand()%tmp;
	}
	else if(bAttackType == MONSTER_ATTACKTYPE2)
	{
		physicalAttackPower = m_mon_stats.PhysicalAttackMin2;
		int tmp = m_mon_stats.PhysicalAttackMax2-m_mon_stats.PhysicalAttackMin2;
		if(tmp) physicalAttackPower += rand()%tmp;
	}

	return (DWORD)physicalAttackPower;
}


DWORD CMonster::DoGetPhyAttackPowerMax()
{
	BYTE bAttackType = MONSTER_ATTACKTYPE1;

	double physicalAttackPower= 0;

	if(bAttackType == MONSTER_ATTACKTYPE1)
	{
		physicalAttackPower = m_mon_stats.PhysicalAttackMin1;
		int tmp = m_mon_stats.PhysicalAttackMax1-m_mon_stats.PhysicalAttackMin1;
		if(tmp) physicalAttackPower += rand()%tmp;
	}
	else if(bAttackType == MONSTER_ATTACKTYPE2)
	{
		physicalAttackPower = m_mon_stats.PhysicalAttackMin2;
		int tmp = m_mon_stats.PhysicalAttackMax2-m_mon_stats.PhysicalAttackMin2;
		if(tmp) physicalAttackPower += rand()%tmp;
	}

	return (DWORD)physicalAttackPower;
}
