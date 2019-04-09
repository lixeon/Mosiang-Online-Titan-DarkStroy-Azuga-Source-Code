#include "stdafx.h"
#include "fieldbossmonster.h"
#include "PackedData.h"
#include "ObjectStateManager.h"
#include "CharMove.h"
#include "../[CC]Header/GameResourceManager.h"

CFieldBossMonster::CFieldBossMonster(void)
{
}

CFieldBossMonster::~CFieldBossMonster(void)
{
}

void CFieldBossMonster::AddStatus(CStatus* pStatus)
{
}

void CFieldBossMonster::StartSpecialState(DWORD SpecialStateKind,DWORD Time,
						WORD wParam1,WORD wParam2,float fParam3,
						WORD EffectNum,WORD StateIcon)
{
}

BOOL CFieldBossMonster::Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	CMonster::Init(kind, AgentNum, pBaseObjectInfo);
	
	// 분배 시스템 초기화
	DistributeDamageInit();

	m_nRegenPosIndex = 0;
	
	return TRUE;
}

void CFieldBossMonster::DoDie(CObject* pAttacker)
{
	// 수련치 경험치 돈을 분배한다
	DistributePerDamage();
	// 아이템을 분배한다
	DistributeItemPerDamage();

	// 일단 클라이언트에 죽음을 연출해준다
	DWORD AttackerID;
	if(pAttacker == NULL)
		AttackerID = 0;
	else 
		AttackerID = pAttacker->GetID();

	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,AttackerID);
	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die,MONSTERREMOVE_TIME+3000);

	SetTObject(NULL);
	SetLastAttackPlayer( NULL );

	// CFieldBossMonsterManager에 죽었다는것을 통보해준다
	FIELDBOSSMONMGR->BossDead(this);

	//CMonster::DoDie(pAttacker);	
}

void CFieldBossMonster::SetLife(DWORD Life,BOOL bSendMsg)
{
	CMonster::SetLife(Life, bSendMsg);	
	
	if(bSendMsg)
	{
		MSG_DWORD3 msg;
		msg.Category = MP_BOSSMONSTER;
		msg.Protocol = MP_FIELD_LIFE_NOTIFY;
		msg.dwData1 = GetLife();
		msg.dwData2 = GetMonsterKind();
		msg.dwData3 = GetID();
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
	}
}

void CFieldBossMonster::SetShield(DWORD Shield,BOOL bSendMsg)
{
	CMonster::SetShield(Shield, bSendMsg);	
	
	if(bSendMsg)
	{
		MSG_DWORD3 msg;
		msg.Category = MP_BOSSMONSTER;
		msg.Protocol = MP_FIELD_SHIELD_NOTIFY;
		msg.dwData1 = GetShield();
		msg.dwData2 = GetMonsterKind();
		msg.dwData3 = GetID();
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
	}
}

BOOL CFieldBossMonster::IsBattle()
{
	// 평화 모드 일때 FALSE 리턴
	if( m_stateParam.stateCur < eMA_PERSUIT )
		return FALSE;

	// 전투 모드 일때 소멸시간 카운트 리셋
	m_Info.m_dwCountTime = m_Info.m_dwDistructTime;
	// 회복시간도 리셋
	m_Info.m_dwPieceTime = gCurTime;

	// TRUE 리턴
	return TRUE;
}

BOOL CFieldBossMonster::IsDistruct()
{
	BOOL rt = FALSE;
	
	// 마지막 체크 시간이 0일 경우 현재 시간으로 설정하고 FALSE 리턴
	if( m_Info.m_dwLastCheckTime != 0 )
	{
		if( ( gCurTime - m_Info.m_dwLastCheckTime )  <  m_Info.m_dwCountTime )
		{
			m_Info.m_dwCountTime -= ( gCurTime - m_Info.m_dwLastCheckTime );
		}
		else
		{
			m_Info.m_dwCountTime = 0;

			rt = TRUE;
		}
	}

	m_Info.m_dwLastCheckTime = gCurTime;

	return rt;
}

void CFieldBossMonster::Recover()	
{
	if(m_Info.m_dwPieceTime + m_Info.m_dwRecoverStartTime < gCurTime)
	{
		DWORD maxlife = GetMaxLife();
		DWORD curlife = GetLife();
		DWORD maxshield = GetMaxShield();
		DWORD curshield = GetShield();
		
		if(gCurTime - m_LifeRecoverTime.lastCheckTime > m_Info.m_dwRecoverDelayTime)
		{
			if(curlife < maxlife)
			{
				DWORD pluslife = (DWORD)(maxlife * m_Info.m_fLifeRate);
				SetLife(curlife + pluslife, TRUE);
				m_LifeRecoverTime.lastCheckTime = gCurTime;

				// Life를 모두 회복했으면
				if(curlife + pluslife >= maxlife)
					// 분배 시스템 초기화
					DistributeDamageInit();
			}
			if(curshield < maxshield)
			{
				DWORD plusshield = (DWORD)(maxshield * m_Info.m_fShieldRate);
				SetShield(curshield+plusshield, TRUE);
			}
		}
	}	
}

BOOL CFieldBossMonster::DoWalkAround()
{
	int ran=0;
	VECTOR3 Target;
	VECTOR3 * pThisPos = CCharMove::GetPosition(this);

	VECTOR3 domainPoint = FIELDBOSSMONMGR->GetRegenPosition(this->GetRegenPosIndex());
	VECTOR3 decisionPoint;
	AdjacentPointForOriginPoint(pThisPos, &domainPoint, (float)10000, &decisionPoint);	

	float dx=0,dz=0;
	ran = rand();
	if(m_pSInfo->DomainRange==0)
		m_pSInfo->DomainRange = 1;	

	Target.x = decisionPoint.x;
	Target.z = decisionPoint.z;

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
