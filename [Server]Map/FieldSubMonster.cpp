#include "stdafx.h"
#include "fieldsubmonster.h"
#include "PackedData.h"
#include "CharMove.h"
#include "../[CC]Header/GameResourceManager.h"

CFieldSubMonster::CFieldSubMonster(void)
{
	m_nRegenPosIndex = 0;
}

CFieldSubMonster::~CFieldSubMonster(void)
{
}

void CFieldSubMonster::DoDie(CObject* pAttacker)
{
	CMonster::DoDie(pAttacker);	

	// CFieldBossMonsterManager에 죽었다는것을 통보해준다
	FIELDBOSSMONMGR->SubDead(this);
}

void CFieldSubMonster::SetLife(DWORD Life,BOOL bSendMsg)
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

void CFieldSubMonster::SetShield(DWORD Shield,BOOL bSendMsg)
{
	CMonster::SetShield(Shield, bSendMsg);	
	
	if(bSendMsg)
	{
		MSG_DWORD3 msg;
		msg.Category = MP_BOSSMONSTER;
		msg.Protocol = MP_FIELD_SHIELD_NOTIFY;
		msg.dwData1 = GetLife();
		msg.dwData2 = GetMonsterKind();
		msg.dwData3 = GetID();
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
	}
}

BOOL CFieldSubMonster::IsBattle()
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

BOOL CFieldSubMonster::IsDistruct()
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

void CFieldSubMonster::Recover()	
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
			}
			if(curshield < maxshield)
			{
				DWORD plusshield = (DWORD)(maxshield * m_Info.m_fShieldRate);
				SetShield(curshield+plusshield, TRUE);
			}
		}
	}	
}

VECTOR3* CFieldSubMonster::GetRegenPos()
{
	int ran=0;
	VECTOR3* pThisPos = CCharMove::GetPosition(this);
	VECTOR3 vTargetPos;

	VECTOR3 domainPoint = FIELDBOSSMONMGR->GetRegenPosition(this->GetRegenPosIndex());

	if(m_pSInfo->DomainRange==0)
		m_pSInfo->DomainRange = 1;

	int dwRange = (int)m_pSInfo->DomainRange;

	//보스 리젠위치의 셋팅된 반경범위로 랜덤하게 움직인다.
	vTargetPos.x = domainPoint.x + ( (rand() % 2) ? -1 : 1 ) * (rand() % dwRange) * 100;
	vTargetPos.z = domainPoint.z + ( (rand() % 2) ? -1 : 1 ) * (rand() % dwRange) * 100;	

	//vTargetPos.x = domainPoint.x + ( (rand() % 2) ? -1 : 1 ) * (rand() % 10) * 100;
	//vTargetPos.z = domainPoint.z + ( (rand() % 2) ? -1 : 1 ) * (rand() % 10) * 100;	

	return &vTargetPos;
}

BOOL CFieldSubMonster::DoWalkAround()
{
	int ran=0;
	VECTOR3 Target;
	VECTOR3 * pThisPos = CCharMove::GetPosition(this);
	
	VECTOR3 domainPoint = *GetRegenPos();
	VECTOR3 decisionPoint;
	AdjacentPointForOriginPoint(pThisPos, &domainPoint, (float)10000, &decisionPoint);	

	float dx=0,dz=0;
	ran = rand();
	if(m_pSInfo->DomainRange==0)
		m_pSInfo->DomainRange = 1;

	//dx = float(ran%m_pSInfo->DomainRange) * (ran%2?1:-1);
	//ran = rand();
	//dz = float(ran%m_pSInfo->DomainRange) * (ran%2?1:-1);
	//Target.x = decisionPoint.x + dx;
	//Target.z = decisionPoint.z + dz;

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
