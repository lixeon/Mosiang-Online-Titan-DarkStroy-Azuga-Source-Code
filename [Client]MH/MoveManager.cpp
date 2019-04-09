// MoveManager.cpp: implementation of the CMoveManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoveManager.h"
#include "Object.h"
#include "Hero.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "GameResourceManager.h"
#include "KyungGongManager.h"
#include "GameIn.h"

#include ".\Engine\EngineObject.h"
#include "MHMap.h"

#include "MovePoint.h"

#include "CAction.h"
#include "ActionTarget.h"


#include "CharStateDialog.h"
#include "PathManager.h"
#include "TileManager.h"
#include "PartyIconManager.h"
#include "PetBase.h"
#include "Pet.h"
#include "PetManager.h"

#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMoveManager)
CMoveManager::CMoveManager()
{

}

CMoveManager::~CMoveManager()
{

}

void CMoveManager::InitMove(CObject* pObject,BASEMOVE_INFO* pMoveInfo,float AngleDeg)
{
	pObject->m_RotateInfo.Angle.SetAngleDeg(AngleDeg);
	pObject->m_RotateInfo.bRotating = FALSE;

	if(pMoveInfo)
	{
		memcpy(&pObject->m_MoveInfo,pMoveInfo,sizeof(BASEMOVE_INFO));
		if(pMoveInfo->bMoving)
		{
			//StartMove(pObject,gCurTime,&pMoveInfo->TargetPosition);
			StartMoveEx(pObject,NULL, gCurTime);
		}
	}
	else
	{
		memset(&pObject->m_MoveInfo,0,sizeof(BASEMOVE_INFO));
	}
	pObject->m_MoveInfo.m_bLastMoving = FALSE;

	pObject->SetPosition(&pObject->m_MoveInfo.CurPosition);

	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pObject)->SetBaseMotion();
	}
}

void CMoveManager::SetPosition(CObject* pObject,VECTOR3* pPos)
{
	//if(pObject->GetObjectKind() == eObjectKind_Pet)
		//printf("MOVEMGR_SetPosition: %d	%f %f \n", pObject->GetID(), pPos->x, pPos->z);
	// Move_bMoving이 TRUE 이 아닐때에만 가능.. ASSERT(Move_bMoving == FALSE); 걸어두자
	//YH2DO
	//ASSERT(pObject->m_MoveInfo.bMoving == FALSE);
	pObject->m_MoveInfo.CurPosition = *pPos;
	pObject->SetPosition(pPos);
}

void CMoveManager::EndYMove(CObject* pObject)
{
	pObject->m_bIsYMoving = FALSE;
	pObject->GetEngineObject()->ApplyHeightField(TRUE);
}

void CMoveManager::EndMove(CObject* pObject)
{
	if(pObject->GetState() == eObjectState_Move)
		OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_Move);

	pObject->m_MoveInfo.m_bEffectMoving = FALSE;
	pObject->m_MoveInfo.bMoving = FALSE;
	pObject->m_MoveInfo.InitTargetPosition();

	if(pObject->m_MoveInfo.KyungGongIdx)
	{
		if(pObject == HERO)
			ToggleHeroKyungGongMode();
	}
}

// 부분 타겟 업데이트 
int CMoveManager::TargetUpdate(CObject* pObject, MOVE_INFO * pMoveInfo, VECTOR3 * TargetPos)
{
	//VECTOR3 * TargetPos = &pMoveInfo->TargetPositions[pMoveInfo->CurTargetPositionIdx];
	// 타겟 거리의 합
	float dx = TargetPos->x - pMoveInfo->CurPosition.x;
	float dz = TargetPos->z - pMoveInfo->CurPosition.z;
	float Distance = sqrtf(dx*dx+dz*dz);
	if(Distance <= 0.01)
	{
		EndMove(pObject);
		return 1;
	}

	float Speed = pObject->GetMoveSpeed();
	float ttt = Speed / Distance;

	pMoveInfo->Move_StartTime			= gCurTime;
	pMoveInfo->Move_StartPosition		= pMoveInfo->CurPosition;
	pMoveInfo->Move_Direction.x			= dx*ttt;
	pMoveInfo->Move_Direction.y			= 0;
	pMoveInfo->Move_Direction.z			= dz*ttt;
	pMoveInfo->Move_EstimateMoveTime	= Speed==0 ? 999999999 : Distance / Speed;

//	if(pObject->GetObjectKind() == eObjectKind_Pet)
//		printf("[PET]%5.2d\t%5.2d\n", gCurTime,pMoveInfo->Move_EstimateMoveTime);
//	if(pObject == HERO)
//		printf("[HERO]%5.2d\t%5.2d\n", gCurTime,pMoveInfo->Move_EstimateMoveTime);

	return 0;
}
void CMoveManager::StartMoveEx(CObject* pObject, VECTOR3* pStartPos ,DWORD CurTime)
{
	//YH 몬스터 튀는것때문에 주석처리함
	//CalcPositionEx(pObject, CurTime);
	if(pObject->GetMoveSpeed() == 0 && pStartPos)
	{
		if(IsMoving(pObject))
			EndMove(pObject);
		SetPosition(pObject,pStartPos);
		return;
	}

	ASSERT( pObject->m_MoveInfo.GetCurTargetPosIdx() <= pObject->m_MoveInfo.GetMaxTargetPosIdx() );	//0000
	StartMoveEx(pObject, CurTime, pObject->m_MoveInfo.GetTargetPosition(pObject->m_MoveInfo.GetCurTargetPosIdx()));
}


void CMoveManager::StartMoveEx(CObject* pObject, DWORD CurTime, VECTOR3 * pTargetPos)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;

	if(pObject->GetState() == eObjectState_Die)
	{
		pMoveInfo->bMoving = FALSE;
		return;
	}

	if(pObject->m_bIsYMoving)
	{
		EndYMove(pObject);
	}

	pMoveInfo->bMoving = TRUE;
	pObject->m_MoveInfo.m_bEffectMoving = FALSE;
	
	if(TargetUpdate(pObject, pMoveInfo, pTargetPos) != 0)
	{
		return;
	}
	
	float TargetAngle = RADTODEG(VECTORTORAD(pMoveInfo->Move_Direction));
	pObject->m_RotateInfo.Angle.SetTargetDeg(TargetAngle);
	float fChangedAngle = pObject->m_RotateInfo.Angle.GetChangeAmountDeg();
	if(fChangedAngle < 0)
		fChangedAngle*=-1.f;
	pObject->m_RotateInfo.EstimatedRotateTime = 0.1f + fChangedAngle * 0.2f / 180.f;
	pObject->m_RotateInfo.Rotate_StartTime = CurTime;
	pObject->m_RotateInfo.bRotating = TRUE;
	
	if(pObject->GetState() != eObjectState_Move)
	{
		if(OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_Move) == TRUE)
			OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_Move,(DWORD)(pMoveInfo->Move_EstimateMoveTime*1000));		
	}
	else
	{
		pObject->SetMotionInState(eObjectState_Move);
		OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_Move,(DWORD)(pMoveInfo->Move_EstimateMoveTime*1000));
	}
	/*
	char temp[256] = {0,};
	if( pObject->GetObjectKind() == eObjectKind_Player)
	{
		sprintf(temp, "Player MoveRemain Time: %d\n", pMoveInfo->Move_EstimateMoveTime*1000);
	}
	if( pObject->GetObjectKind() == eObjectKind_Pet)
	{
		sprintf(temp, "Pet MoveRemain Time: %d\n", pMoveInfo->Move_EstimateMoveTime*1000);
	}
	printf(temp);*/
/*
	if( pObject->GetObjectKind() == eObjectKind_Player && ((CPlayer*)pObject)->GetPet() )
	{		
		MOVE_INFO* pminfo = &((CPlayer*)pObject)->GetPet()->m_MoveInfo;
		pminfo->SetTargetPosition( pObject->m_MoveInfo.GetTargetPositionArray() );
		
		VECTOR3 vPos = ((CPlayer*)pObject)->GetPet()->GetCurPosition();
		VECTOR3 tgPos;
		
		pminfo->SetCurTargetPosIdx( pObject->m_MoveInfo.GetCurTargetPosIdx() );

		if( pObject->m_MoveInfo.GetCurTargetPosIdx() == 0 )
		{			
			pminfo->SetMaxTargetPosIdx( pObject->m_MoveInfo.GetMaxTargetPosIdx() );
			
			for(int r=0; r<MAX_CHARTARGETPOSBUF_SIZE; r++)
			{
				if( pObject->m_MoveInfo.GetTargetPosition(r)->x || 
					pObject->m_MoveInfo.GetTargetPosition(r)->y || 
					pObject->m_MoveInfo.GetTargetPosition(r)->z )
				{
					VECTOR3 tp;
					VECTOR3 tPos = *pObject->m_MoveInfo.GetTargetPosition(r);
					tp = tPos;
					tp = tp - vPos;
					Normalize( &tp, &tp );
					tp = tp * 150;
					
					
					VECTOR3 tt = tPos - tp;
					
					printf("[%d]player - x:%6.2f y:%6.2f z:%6.2f\n", r, tPos.x, tPos.y, tPos.z);
					printf("[%d]pet    - x:%6.2f y:%6.2f z:%6.2f\n", r, tt.x, tt.y, tt.z);
					
					pminfo->SetTargetPosition( r, tt );
					
					vPos = tPos;
					tgPos = tt;
				}
				else
					break;
			}
			printf("\n");
		}

		StartMoveEx(((CPlayer*)pObject)->GetPet(), CurTime, &tgPos);
	}
*/
}

#define MOVESKIPCOUNT2	1
BOOL CMoveManager::CalcPositionEx(CObject* pObject,DWORD CurTime)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	
	if(pMoveInfo->bMoving)
	{
		if(IsGameSlow() == TRUE && pObject != HERO)
		{
			if(pObject->m_bMoveSkipCount < MOVESKIPCOUNT2)
			{
				++pObject->m_bMoveSkipCount;
				return FALSE;
			}
			else
				pObject->m_bMoveSkipCount = 0;
		}

		BOOL bVal = FALSE;
		float fMoveTime = ((CurTime - pMoveInfo->Move_StartTime)*0.001f);
		if(pMoveInfo->Move_EstimateMoveTime < fMoveTime)
		{
			// 다음 타겟으로 변경
			if(pMoveInfo->GetCurTargetPosIdx()< pMoveInfo->GetMaxTargetPosIdx()- 1)
			{
				ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
				pMoveInfo->CurPosition.x = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x;
				pMoveInfo->CurPosition.z = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z;
				pMoveInfo->SetCurTargetPosIdx(pMoveInfo->GetCurTargetPosIdx()+1);
				ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
				StartMoveEx(pObject, CurTime, pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx()));
			}
			else
			{
				ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
				pMoveInfo->CurPosition.x = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x;
				pMoveInfo->CurPosition.z = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z;
				bVal = TRUE;
			}
		}
		else
		{
			pMoveInfo->CurPosition.x = pMoveInfo->Move_StartPosition.x + pMoveInfo->Move_Direction.x * fMoveTime;
			pMoveInfo->CurPosition.z = pMoveInfo->Move_StartPosition.z + pMoveInfo->Move_Direction.z * fMoveTime;
		}

		pObject->SetPosition(&pObject->m_MoveInfo.CurPosition);

		return bVal;
	}


	return FALSE;
}
/*

// 이동중인 상태로 표시하고 시작지점과 끝지점을 기억해놓는다.
// 이동시작시간도 기억해놓는다.
// 현재위치를 계산해봐서 StartPos 랑 차이가 많이 나면 조치-_-;를 취한다.
void CMoveManager::StartMove(CObject* pObject,DWORD CurTime,VECTOR3* TargetPos)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;	
	if(pObject->m_bIsYMoving)
	{
		EndYMove(pObject);
	}


	pMoveInfo->bMoving = TRUE;

	float Speed = pObject->GetMoveSpeed();
	
	pMoveInfo->Move_StartTime = CurTime;

	// 여러개의 타겟 처리
	pMoveInfo->Move_StartPosition.x = pMoveInfo->CurPosition.x;
	pMoveInfo->Move_StartPosition.z = pMoveInfo->CurPosition.z;
	pMoveInfo->TargetPosition.x = TargetPos->x;
	pMoveInfo->TargetPosition.z = TargetPos->z;
	pMoveInfo->Move_StartPosition.y = 0;
	pMoveInfo->TargetPosition.y = 0;

	// 타겟 거리의 합
	float dx = pMoveInfo->TargetPosition.x - pMoveInfo->Move_StartPosition.x;
	float dz = pMoveInfo->TargetPosition.z - pMoveInfo->Move_StartPosition.z;
	float Distance = sqrtf(dx*dx+dz*dz);
	if(Distance <= 0.01)
	{
		pMoveInfo->bMoving = FALSE;
		return;
	}
	float ttt = Speed / Distance;

	// 여러 타겟의 방향 설정 ( 배열혹은 할때마다 셋팅? )
	pMoveInfo->Move_Direction.x = dx*ttt;
	pMoveInfo->Move_Direction.y = 0;
	pMoveInfo->Move_Direction.z = dz*ttt;
	// 예상 시간 설정
	pMoveInfo->Move_EstimateMoveTime = Distance / Speed;
	
	pObject->m_MoveInfo.CurPosition.y = 0;
	float TargetAngle = RADTODEG(VECTORTORAD(pMoveInfo->Move_Direction));
	pObject->m_RotateInfo.Angle.SetTargetDeg(TargetAngle);
	float fChangedAngle = pObject->m_RotateInfo.Angle.GetChangeAmountDeg();
	if(fChangedAngle < 0)
		fChangedAngle*=-1.f;
	pObject->m_RotateInfo.EstimatedRotateTime = 0.1f + fChangedAngle * 0.2f / 180.f;
	pObject->m_RotateInfo.Rotate_StartTime = CurTime;
	pObject->m_RotateInfo.bRotating = TRUE;

	if(pObject->GetState() != eObjectState_Move)
		OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_Move);
	OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_Move,(DWORD)(pMoveInfo->Move_EstimateMoveTime*1000));
}

// 현재 위치를 계산해서 Move_Position에 셋팅하고 넘겨준다.
// 계속해서 계산하지 않고 필요할때만 계산한다.
// Move_bMoving이 FALSE이면 바루 리턴한다.
// 시간 변수에 대한 OverFlow를 고려해야 한다.
#define MOVESKIPCOUNT	1
BOOL CMoveManager::CalcPosition(CObject* pObject,DWORD CurTime)
{
	if(pObject->m_MoveInfo.bMoving)
	{
		if(IsGameSlow() == TRUE)
		{
			if(pObject->m_bMoveSkipCount < MOVESKIPCOUNT)
			{
				++pObject->m_bMoveSkipCount;
				return FALSE;
			}
			else
				pObject->m_bMoveSkipCount = 0;
		}

		BOOL bVal = FALSE;
		float fMoveTime = ((CurTime - pObject->m_MoveInfo.Move_StartTime)*0.001f);
		if(pObject->m_MoveInfo.Move_EstimateMoveTime < fMoveTime)
		{
			pObject->m_MoveInfo.CurPosition.x = pObject->m_MoveInfo.TargetPosition.x;
			pObject->m_MoveInfo.CurPosition.z = pObject->m_MoveInfo.TargetPosition.z;
			bVal = TRUE;
		}
		else
		{
			pObject->m_MoveInfo.CurPosition.x = pObject->m_MoveInfo.Move_StartPosition.x + 
				pObject->m_MoveInfo.Move_Direction.x * fMoveTime;
			pObject->m_MoveInfo.CurPosition.z = pObject->m_MoveInfo.Move_StartPosition.z + 
				pObject->m_MoveInfo.Move_Direction.z * fMoveTime;
		}
		
		pObject->SetPosition(&pObject->m_MoveInfo.CurPosition);
		return bVal;
	}

	return FALSE;
}
*/
BOOL CMoveManager::CalcYPosition(CObject* pObject,DWORD CurTime)
{
	if(pObject->m_bIsYMoving)
	{
		MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;

		BOOL bVal = FALSE;
		float fMoveTime = ((CurTime - pMoveInfo->Move_StartTime)*0.001f);
		if(pMoveInfo->Move_EstimateMoveTime < fMoveTime)
		{
			pMoveInfo->CurPosition.y = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->y;
			bVal = TRUE;
		}
		else
		{
			pMoveInfo->CurPosition.y = pMoveInfo->Move_StartPosition.y + pMoveInfo->Move_Direction.y * fMoveTime;
		}
		
		pObject->SetPosition(&pMoveInfo->CurPosition);

		return bVal;
	}

	return FALSE;
}

void CMoveManager::CalcAngle(CObject* pObject,DWORD CurTime)
{	
	if(pObject->m_RotateInfo.bRotating)
	{
		float fRotateTime = ((CurTime - pObject->m_RotateInfo.Rotate_StartTime)*0.001f);
		if(fRotateTime >= pObject->m_RotateInfo.EstimatedRotateTime)
		{
			pObject->m_RotateInfo.Angle.Process(1);
			pObject->m_RotateInfo.bRotating = FALSE;
		}
		else
		{
			pObject->m_RotateInfo.Angle.Process(fRotateTime / pObject->m_RotateInfo.EstimatedRotateTime);
		}
		pObject->SetAngle(pObject->m_RotateInfo.Angle.ToRad());
	}
}

void CMoveManager::MoveProcess(CObject* pObject)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	if(pObject->m_bIsYMoving)
	{
		if(CalcYPosition(pObject,gCurTime) == TRUE)
			EndYMove(pObject);
	}
	else
	{
		if(CalcPositionEx(pObject,gCurTime) == TRUE)
			EndMove(pObject);
	}
	CalcAngle(pObject,gCurTime);
	pMoveInfo->m_bLastMoving = pMoveInfo->bMoving;

}

void CMoveManager::SetHeroTarget(VECTOR3* pPos,BOOL bMousePointDisplay)
{
	CHero* pHero = OBJECTMGR->GetHero();

	if(pHero->IsInSpecialState(eSpecialState_Stun))
		return;

	if( pHero->GetState() != eObjectState_None &&
		pHero->GetState() != eObjectState_Move &&
		pHero->GetState() != eObjectState_TiedUp_CanMove &&
		pHero->GetState() != eObjectState_Immortal  )
	{
		if( pHero->GetState() == eObjectState_Society )
		{
			if( pHero->EndSocietyAct() == FALSE )
			{
				CAction act;
				act.InitMoveAction(pPos);
				pHero->SetNextAction(&act);
				pHero->DisableAutoAttack();
				return;
			}
		}
		else
		{
			CAction act;
			act.InitMoveAction(pPos);
			pHero->SetNextAction(&act);
			pHero->DisableAutoAttack();
			return;
		}
	}
	if( pHero->GetState() == eObjectState_Move &&
		pHero->GetEngineObject()->GetCurMotion() == pHero->m_ObjectState.State_End_Motion)
		return;

	
	if(IsMoving(pHero))
		CalcPositionEx(pHero,gCurTime);
	
	//////////////////////////////////////////////////////////////////////////
	// 50cm 타일의 중앙에 서게 한다.
	/*
	pPos->x = int(pPos->x / TileWidth) * TileWidth + TileWidth * 0.5f;
	pPos->z = int(pPos->z / TileWidth) * TileWidth + TileWidth * 0.5f;
	*/
	pPos->x = (int(pPos->x / TILECOLLISON_DETAIL) + 0.5f) * TILECOLLISON_DETAIL;
	pPos->y = 0;
	pPos->z = (int(pPos->z / TILECOLLISON_DETAIL) + 0.5f) * TILECOLLISON_DETAIL;
	//////////////////////////////////////////////////////////////////////////
	
	



	pHero->m_MoveInfo.InitTargetPosition();
	switch(PATHMGR->GetPath(&pHero->m_MoveInfo.CurPosition,pPos,pHero->m_MoveInfo.GetTargetPositionArray(), MAX_CHARTARGETPOSBUF_SIZE, pHero->m_MoveInfo.GetMaxTargetPosIdxRef(), pHero, FALSE, 100))
	{
	case PATH_BLOCKFAILED:
	case PATH_FAILED:
		//__asm int 3;
		return;
	}
	WORD count = pHero->m_MoveInfo.GetMaxTargetPosIdx();

	// 길찾기 테스트 용
	/*
	EFFECTMGR->RemoveAllEffect();
	for(int k = 0 ; k < count ; ++k)
	{
		EFFECTMGR->StartEffectProcess(1, HERO, 0,0,0,EFFECT_FLAG_ABSOLUTEPOS, &pHero->m_MoveInfo.TargetPositions[k]);
	}*/


	if(bMousePointDisplay)
		MOVEPOINT->SetPoint(pPos);

	// jsd 
	if( count == 0 )
		return;
	else if( count == 1 )
	{
		MOVE_ONETARGETPOS msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_ONETARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = HEROID;
		msg.SetStartPos( &pHero->m_MoveInfo.CurPosition );
		msg.SetTargetPos( pHero->m_MoveInfo.GetTargetPosition(0) );

		NETWORK->Send(&msg,msg.GetSize());
	}
	else
	{
		MOVE_TARGETPOS msg;
		msg.Init();
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_TARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = HEROID;
		msg.SetStartPos(&pHero->m_MoveInfo.CurPosition);
		//msg.tpos.Compress(&TargetPos);
		ASSERT( count <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
		for(int i=0;i<count;++i)
		{
			msg.AddTargetPos(pHero->m_MoveInfo.GetTargetPosition(i));
		}

		NETWORK->Send(&msg,msg.GetSize());
	}

	//StartMove(pHero,gCurTime,&TargetPos);
	StartMoveEx(pHero,NULL,gCurTime);

	SetPetTarget(&pHero->m_MoveInfo, count);
}

void CMoveManager::SetPetTarget( MOVE_INFO* pHeroMoveInfo, WORD wPosCount )
{
	CPet* pPet = HERO->GetPet();
	if(!pPet) return;

	if(IsMoving(pPet))
	{
		CalcPositionEx(pPet,gCurTime);
	}

	VECTOR3 PetLastPos;
	PetLastPos.x = PetLastPos.y = PetLastPos.z = 0;

	VECTOR3* pHeroLastPos = pHeroMoveInfo->GetTargetPosition(wPosCount-1);

	//펫과 주인의 현재 거리가 10m 가 넘는가?워프메시지
	float Indist = CalcDistanceXZ(&pPet->m_MoveInfo.CurPosition, &pHeroMoveInfo->CurPosition);
	/*
	if( MAX_LIMIT_PET_DIST < Indist )
	{
		//워프메시지. 서버에서 순간이동 처리.
		MOVE_POS	msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_PET_WARP_SYN;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = pPet->GetID();
		msg.cpos.Compress(&pHeroMoveInfo->CurPosition);
		NETWORK->Send(&msg,sizeof(msg));

		return;
		//SetPosition(pPet,&pHeroMoveInfo->CurPosition);
	}*/

	if( IsMoving(pPet) )
	{
		//펫이 주인보다 타겟에 가깝고 펫 과 주인의 거리가 4m 이내이면
		float HeroToTargetDist = CalcDistanceXZ(&pHeroMoveInfo->CurPosition, pHeroLastPos);
		float PetToTargetDist = CalcDistanceXZ(&pPet->m_MoveInfo.CurPosition, pHeroLastPos);
		if( (HeroToTargetDist - PetToTargetDist) >= 0 )
			//&& DEFAULT_PET_FOLLOW_DIST > Indist )
		{
			//정지!
			//MoveStop(pPet,&pPet->m_MoveInfo.CurPosition);
			PetMoveStop();
			return;
		}
	}

	float dist = 0.f;
	if( 0 == wPosCount )	return;

	if( 1 == wPosCount )
	{
		//주인의 노드가 1개이면서 주인의 현재와 최종 노드의 거리가 4 이상이면 
		dist = CalcDistanceXZ(&pHeroMoveInfo->CurPosition, pHeroLastPos);
		if( DEFAULT_PET_FOLLOW_DIST < dist )
		{
			if(!GetPetLastPos(&PetLastPos,pHeroLastPos,&pHeroMoveInfo->CurPosition))
				return;
		}
		else
		{
			PetLastPos = pHeroMoveInfo->CurPosition;
		}
	}
	else
	{//마지막 노드만 검사해서
		VECTOR3* pHeroLastBeforePos = pHeroMoveInfo->GetTargetPosition(wPosCount-2);
		dist = CalcDistanceXZ(pHeroLastPos, pHeroLastBeforePos);

		if( DEFAULT_PET_FOLLOW_DIST < dist )
		{
			if(!GetPetLastPos(&PetLastPos,pHeroLastPos,pHeroLastBeforePos))
				return;
		}
		else
		{
			PetLastPos = *pHeroLastBeforePos;
		}
	}

	pPet->m_MoveInfo.InitTargetPosition();
	switch(PATHMGR->GetPath(&pPet->m_MoveInfo.CurPosition,&PetLastPos,pPet->m_MoveInfo.GetTargetPositionArray(), MAX_CHARTARGETPOSBUF_SIZE, pPet->m_MoveInfo.GetMaxTargetPosIdxRef(), pPet, FALSE, 100))
	{
	case PATH_BLOCKFAILED:
	case PATH_FAILED:
		return;
	}

	
	WORD count = pPet->m_MoveInfo.GetMaxTargetPosIdx();
	if(TargetUpdate(pPet,&pPet->m_MoveInfo,pPet->m_MoveInfo.GetTargetPosition(count-1)) != 0)
		return;

//	pPet->SetMotionInState(eObjectState_Move);
	OBJECTSTATEMGR->EndObjectState(pPet,eObjectState_Move,(DWORD)(pPet->m_MoveInfo.Move_EstimateMoveTime*1000));

	float TargetAngle = RADTODEG(VECTORTORAD(pPet->m_MoveInfo.Move_Direction));
	pPet->m_RotateInfo.Angle.SetTargetDeg(TargetAngle);
	float fChangedAngle = pPet->m_RotateInfo.Angle.GetChangeAmountDeg();
	if(fChangedAngle < 0)
		fChangedAngle*=-1.f;
	pPet->m_RotateInfo.EstimatedRotateTime = 0.1f + fChangedAngle * 0.2f / 180.f;
	pPet->m_RotateInfo.Rotate_StartTime = gCurTime;
	pPet->m_RotateInfo.bRotating = TRUE;
	
	//펫 타겟과 주인 타겟의 거리가 4m 이상이면 움직인다.
	Indist = CalcDistanceXZ(pPet->m_MoveInfo.GetTargetPosition(count-1), pHeroLastPos);
	//펫과 주인의 타겟의 거리가 4m 이상이거나
	dist = CalcDistanceXZ(&pPet->m_MoveInfo.CurPosition, pHeroLastPos);

 	if( DEFAULT_PET_FOLLOW_DIST < Indist || DEFAULT_PET_FOLLOW_DIST < dist)
 	{
		//SendPetMoveMsg();
 		PETMGR->SetMoveReady(TRUE);
 	}
	
}

void CMoveManager::SendPetMoveMsg()
{
	CPet* pPet = HERO->GetPet();
	if(!pPet) return;

	WORD count = pPet->m_MoveInfo.GetMaxTargetPosIdx();

	if( 0 == count ) return;
	else if( 1 == count )
	{
		MOVE_ONETARGETPOS msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_PET_ONETARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = pPet->GetID();
		msg.SetStartPos( &pPet->m_MoveInfo.CurPosition );
		msg.SetTargetPos( pPet->m_MoveInfo.GetTargetPosition(0) );

		NETWORK->Send(&msg,msg.GetSize());
	}
	else
	{
		MOVE_TARGETPOS msg;
		msg.Init();
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_PET_TARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = pPet->GetID();
		msg.SetStartPos( &pPet->m_MoveInfo.CurPosition );
		ASSERT( count <= MAX_CHARTARGETPOSBUF_SIZE );
		for( int i = 0; i < count; ++i )
		{
			msg.AddTargetPos(pPet->m_MoveInfo.GetTargetPosition(i));
		}
		NETWORK->Send(&msg,msg.GetSize());
	}

//	StartMoveEx(pPet,&pPet->m_MoveInfo.CurPosition,gCurTime);
	StartMoveEx(pPet,NULL,gCurTime);

	PETMGR->SetMoveReady(FALSE);
}

/*
void CMoveManager::GetPetPath( MOVE_INFO* pHeroMoveInfo, WORD wPosCount )
{
//	제안2:
//	펫거리가 10m 이상인가
//	최종지점구하고
// 	펫길찾기

	CPet* pPet = PETMGR->GetCurSummonPet();
	if(!pPet) return;

	VECTOR3* HeroLastPos = pHeroMoveInfo->GetTargetPosition(wPosCount-1);

	//펫과 주인의 현재 거리가 10m 가 넘는가?워프메시지
	float dist = CalcDistanceXZ(&pPet->m_MoveInfo.CurPosition, &pHeroMoveInfo->CurPosition);
	if(MAX_LIMIT_PET_DIST < dist)
	{
		//워프메시지. 서버에서 순간이동 처리.
	}

	if(wPosCount==1)
	{
		dist = CalcDistanceXZ(&pHeroMoveInfo->CurPosition,pHeroMoveInfo->GetTargetPosition(0));
		//펫의 첫번째 타겟포즈는 주인의 현재 포즈로
		pPet->m_MoveInfo.SetTargetPosition(0, pHeroMoveInfo->CurPosition);
		if(DEFAULT_PET_FOLLOW_DIST < dist)
		{
			//펫의 두번째 타겟포즈는 주인 타겟의 4m 후방 지점으로
			GetPetLastPos(pPet->m_MoveInfo.GetTargetPosition(1),HeroLastPos,&pHeroMoveInfo->CurPosition);
		}
	}
	else if( MAX_CHARTARGETPOSBUF_SIZE == wPosCount)
	{
		//워프를 하거나 마지막 노드 처리.
		ASSERT(0);
		return;
	}
	else
	{
		VECTOR3* HeroLastBeforePos = pHeroMoveInfo->GetTargetPosition(wPosCount-2);
		//펫의 첫번째 타겟포즈는 주인의 현재 포즈로
		pPet->m_MoveInfo.SetTargetPosition(0, pHeroMoveInfo->CurPosition);
		int i = 0;
		for( i = 1; i < wPosCount; ++i )
		{
			pPet->m_MoveInfo.SetTargetPosition(i,*pHeroMoveInfo->GetTargetPosition(i-1));
		}
		//제일 마지막 노드의 길이를 구해서
		dist = CalcDistanceXZ(HeroLastPos,HeroLastBeforePos);
		if(DEFAULT_PET_FOLLOW_DIST < dist)
		{
			GetPetLastPos(pPet->m_MoveInfo.GetTargetPosition(i),HeroLastPos,HeroLastBeforePos);
		}
		else
		{
			pPet->m_MoveInfo.SetTargetPosition(i-1,*HeroLastBeforePos);
		}
	}

	
	MOVE_TARGETPOS msg;
	msg.Init();
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_PET_TARGET;
	msg.dwObjectID = HEROID;
	msg.dwMoverID = pPet->GetID();
	msg.SetStartPos(&pPet->m_MoveInfo.CurPosition);
	//msg.tpos.Compress(&TargetPos);
	ASSERT( wPosCount <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
	for(int i=0;i<wPosCount;++i)
	{
		msg.AddTargetPos(pPet->m_MoveInfo.GetTargetPosition(i));
	}

	NETWORK->Send(&msg,msg.GetSize());

	StartMoveEx(pPet,NULL,gCurTime);
}*/

BOOL CMoveManager::GetPetLastPos( VECTOR3* pSrcPos, VECTOR3* pFromPos, VECTOR3* pToPos )
{//pFromPos 는 끝점, pToPos 끝점 전 지점. 펫의 이동 타겟지점을 구한다.
	VECTOR3 Pos = *pToPos - *pFromPos;
	float dist = sqrtf(Pos.x*Pos.x + Pos.z*Pos.z);

	//if(dist < DEFAULT_PET_FOLLOW_DIST)
	if(dist < DEFAULT_PET_FOLLOW_DIST)
	{
		ASSERT(0);
		printf("ASSERT %f\n",dist);
		return FALSE;
	}

	pSrcPos->x = pFromPos->x + (Pos.x / dist * DEFAULT_PET_FOLLOW_DIST);
	pSrcPos->z = pFromPos->z + (Pos.z / dist * DEFAULT_PET_FOLLOW_DIST);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////
/// 06. 08. 2차 보스 - 이영준
/// 기존 SetLookatPos 함수 마지막 인자에 추가 회전각을 추가했다.
void CMoveManager::SetLookatPos(CObject* pObject, VECTOR3* pPos,float TurnDuration, DWORD CurTime, int AddDeg)
{
	pObject->m_MoveInfo.CurPosition.y = 0;
	VECTOR3 PositionGap = *pPos - pObject->m_MoveInfo.CurPosition;
	float Distance = VECTOR3Length(&PositionGap);
	if(Distance == 0)
		return;
	VECTOR3 TDirection = PositionGap / Distance;
	/// 추가 회전각을 더해준다
	float angle = RADTODEG(VECTORTORAD(TDirection)) + AddDeg;
	SetAngle(pObject,angle,TurnDuration);
}
/////////////////////////////////////////////////////////////////////////////////////

void CMoveManager::SetAngle(CObject* pObject,float AngleDeg,float TurnDuration)
{
	if(TurnDuration == 0)
	{
		pObject->m_RotateInfo.Angle.SetAngleDeg(AngleDeg);
		pObject->m_RotateInfo.bRotating = FALSE;
		pObject->SetAngle(pObject->m_RotateInfo.Angle.ToRad());
	}
	else
	{
		pObject->m_RotateInfo.Angle.SetAngleDeg(AngleDeg);
		pObject->m_RotateInfo.EstimatedRotateTime = TurnDuration;
		pObject->m_RotateInfo.Rotate_StartTime = gCurTime;
		pObject->m_RotateInfo.bRotating = TRUE;
	}
}

void CMoveManager::SetHeroActionMove(CActionTarget* pTarget,CAction* pAction)
{
	VECTOR3* pTargetPos = pTarget->GetTargetPosition();
	if(pTargetPos == NULL)
		return;
	SetHeroTarget(pTargetPos,FALSE);

	CHero* pHero = OBJECTMGR->GetHero();
	pHero->SetMovingAction(pAction);
	
	pHero->SetCurComboNum(0);
}

void CMoveManager::MoveStop(CObject* pObject,VECTOR3* pStopPos)
{
	if(pObject->m_MoveInfo.bMoving)
		EndMove(pObject);
	if(pStopPos)
	{
		pObject->m_MoveInfo.CurPosition = *pStopPos;
		pObject->SetPosition(pStopPos);
	}
	else
	{
		pObject->SetPosition(&pObject->m_MoveInfo.CurPosition);
	}
}

void CMoveManager::HeroMoveStop()
{

	CHero* pHero = OBJECTMGR->GetHero();
	if(pHero->m_MoveInfo.bMoving == FALSE)
		return;
	
	CalcPositionEx(pHero,gCurTime);
	
	MoveStop(pHero,&pHero->m_MoveInfo.CurPosition);
	// 상태 변경
	if(pHero->GetState() == eObjectState_Die)
		return;

	if( !OBJECTSTATEMGR->StartObjectState(pHero, eObjectState_None) )
		return;	

	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_STOP;
	msg.dwObjectID = HEROID;
	msg.dwMoverID = HEROID;
	msg.cpos.Compress(&pHero->m_MoveInfo.CurPosition);

	NETWORK->Send(&msg,sizeof(MOVE_POS));

	if(PETMGR->GetCurSummonPet())
	{
		//PETMGR->SetMoveReady(FALSE);
		//if(IsMoving(PETMGR->GetCurSummonPet()))
			PetMoveStop();
	}
}

void CMoveManager::PetMoveStop()
{
	CPet* pPet = HERO->GetPet();
	if(!pPet) return;
	if( FALSE == pPet->m_MoveInfo.bMoving )
		return;

	CalcPositionEx(pPet, gCurTime);

	MoveStop(pPet, &pPet->m_MoveInfo.CurPosition);

	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_PET_STOP;
	msg.dwObjectID = HEROID;
	msg.dwMoverID = pPet->GetID();
	msg.cpos.Compress(&pPet->m_MoveInfo.CurPosition);

	NETWORK->Send(&msg,sizeof(msg));
}

float CMoveManager::CalcDistance(CObject* pObject1,CObject* pObject2)
{
	float tx = pObject1->m_MoveInfo.CurPosition.x - pObject2->m_MoveInfo.CurPosition.x;
	float tz = pObject1->m_MoveInfo.CurPosition.z - pObject2->m_MoveInfo.CurPosition.z;

	return sqrtf(tx*tx+tz*tz);
}


void CMoveManager::SetWalkMode(CObject* pObject)
{
	pObject->m_MoveInfo.MoveMode = eMoveMode_Walk;
	
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pObject)->SetBaseMotion();
	}
}
void CMoveManager::SetRunMode(CObject* pObject)
{
	pObject->m_MoveInfo.MoveMode = eMoveMode_Run;
	
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pObject)->SetBaseMotion();
	}
}

BOOL CMoveManager::SetKyungGong(CObject* pObject,WORD KGIdx)
{
	if(IsValidKyungGong(pObject,KGIdx) == FALSE)
		return FALSE;

	pObject->m_MoveInfo.KyungGongIdx = KGIdx;

	
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pObject)->SetBaseMotion();
	}


	return TRUE;
}
BOOL CMoveManager::SetMonsterMoveType(CObject* pObject,WORD KGIdx)
{
	if(!pObject) return FALSE;
	if((pObject->GetObjectKind() & eObjectKind_Monster) == FALSE)
	{
		ASSERT(0);
		return FALSE;
	}
	
	pObject->m_MoveInfo.KyungGongIdx = KGIdx;
	return TRUE;
}

BOOL CMoveManager::IsValidKyungGong(CObject* pObject,WORD KGIdx)
{
	return TRUE;
}

float CMoveManager::GetKyungGongSpeed(WORD KyungGongIdx)
{
	ASSERT(KyungGongIdx);
	CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(KyungGongIdx);
	ASSERT(pKGInfo);
	if(pKGInfo == NULL)
		return 0;

	return pKGInfo->GetSpeed();
}

void CMoveManager::EffectYMove(CObject* pObject,float StartY,float EndY,DWORD Duration)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;

	pMoveInfo->bMoving = TRUE;
	pObject->m_bIsYMoving = TRUE;

	float Speed;	
	
	pMoveInfo->Move_StartTime = gCurTime;

	VECTOR3 CurPos;
	pObject->GetPosition(&CurPos);

	pMoveInfo->Move_StartPosition.x = CurPos.x;
	pMoveInfo->Move_StartPosition.z = CurPos.z;
	pMoveInfo->Move_StartPosition.y = CurPos.y + StartY;

	ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x = CurPos.x;
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z = CurPos.z;
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->y = CurPos.y + EndY;
	//taiyoe
	/*
	pMoveInfo->TargetPosition.x = CurPos.x;
	pMoveInfo->TargetPosition.z = CurPos.z;
	pMoveInfo->TargetPosition.y = CurPos.y + EndY;
	*/

	float Distance = EndY - StartY;
	if(Duration == 0)
		Duration = 1;
	float fTime = Duration * 0.001f;
	Speed = Distance / fTime;
	pMoveInfo->Move_Direction.x = 0;
	pMoveInfo->Move_Direction.y = Speed;
	pMoveInfo->Move_Direction.z = 0;
	pMoveInfo->Move_EstimateMoveTime = fTime;
	
	pObject->GetEngineObject()->ApplyHeightField(FALSE);
}

void CMoveManager::EffectMove(CObject* pObject,VECTOR3* TargetPos,DWORD Duration)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	if(pObject->m_bIsYMoving)
	{
		EndYMove(pObject);
	}

	VECTOR3 repos;
	if( MAP->CollisonCheck(&pMoveInfo->CurPosition,TargetPos,&repos,pObject) == TRUE )
		*TargetPos = repos;



	pMoveInfo->bMoving = TRUE;
	pMoveInfo->m_bEffectMoving = TRUE;

	float Speed;	
	
	pMoveInfo->Move_StartTime = gCurTime;

	pMoveInfo->SetCurTargetPosIdx(0);
	pMoveInfo->SetMaxTargetPosIdx(1);

	pMoveInfo->Move_StartPosition.x = pMoveInfo->CurPosition.x;
	pMoveInfo->Move_StartPosition.z = pMoveInfo->CurPosition.z;
	pMoveInfo->Move_StartPosition.y = 0;

	ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x = TargetPos->x;
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z = TargetPos->z;
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->y = 0;
/*
	pMoveInfo->TargetPosition.x = TargetPos->x;
	pMoveInfo->TargetPosition.z = TargetPos->z;
	pMoveInfo->TargetPosition.y = 0;
*/
	

	float dx = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x - pMoveInfo->Move_StartPosition.x;
	float dz = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z - pMoveInfo->Move_StartPosition.z;
	float Distance = sqrtf(dx*dx+dz*dz);
	if(Distance <= 0.01)
	{
		EndMove(pObject);
		return;
	}
	if(Duration == 0)
		Duration = 1;
	float fTime = Duration * 0.001f;
	Speed = Distance / fTime;
	float ttt = Speed / Distance;
	pMoveInfo->Move_Direction.x = dx*ttt;
	pMoveInfo->Move_Direction.y = 0;
	pMoveInfo->Move_Direction.z = dz*ttt;
	pMoveInfo->Move_EstimateMoveTime = fTime;
}

void CMoveManager::HeroEffectMove(CObject* pObject,VECTOR3* TargetPos,DWORD Duration,CObject* pOperator)
{
	if( pObject->GetObjectKind() == eObjectKind_SkillObject ||
		pObject->GetObjectKind() == eObjectKind_Tactic ||
		pObject->GetObjectKind() == eObjectKind_Item )
		return;

	if(pObject != HERO && pOperator != HERO)
	{
		EffectMove(pObject,TargetPos,Duration);
		return;
	}

	if(pObject == HERO)
	{
		HeroMoveStop();
	}
	
	pObject->m_MoveInfo.SetCurTargetPosIdx(0);
	pObject->m_MoveInfo.SetMaxTargetPosIdx(1);
	EffectMove(pObject,TargetPos,Duration);

	if(pObject->IsDied() == FALSE)
	{
		MOVE_POS msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_EFFECTMOVE;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = pObject->GetID();
		msg.cpos.Compress(TargetPos);
		NETWORK->Send(&msg,sizeof(msg));	
	}
}

void CMoveManager::EffectMoveInterpolation(CObject* pObject,VECTOR3* TargetPos)
{
	if(pObject->IsDied())
		return;

	
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	DWORD dwTime = (DWORD)(pMoveInfo->Move_EstimateMoveTime * 1000);
	DWORD ElapsedTime = gCurTime - pMoveInfo->Move_StartTime;
	if(ElapsedTime >= dwTime)
	{
		if(pObject->m_MoveInfo.bMoving == FALSE)
			SetPosition(pObject,TargetPos);
	}
	else
	{
		DWORD RemainTime = dwTime - ElapsedTime;
		EffectMove(pObject,TargetPos,RemainTime);
	}
	
}

void CMoveManager::ToggleHeroMoveMode()
{
	VECTOR3 TargetPos;
	BOOL bTarget = FALSE;
	if(IsMoving(HERO))
	{
		bTarget = TRUE;
		TargetPos = *HERO->m_MoveInfo.GetTargetPosition(HERO->m_MoveInfo.GetCurTargetPosIdx());
	}

	HeroMoveStop();
	
	MSGBASE msg;
	msg.Category = MP_MOVE;
	msg.dwObjectID = HEROID;
	if(HERO->m_MoveInfo.MoveMode == eMoveMode_Walk)
	{
		msg.Protocol = MP_MOVE_RUNMODE;
//KES CHARSTATEDLG 031016
		if(GAMEIN->GetCharStateDialog())
			GAMEIN->GetCharStateDialog()->SetMoveMode( FALSE );
//
	}
	else
	{
		msg.Protocol = MP_MOVE_WALKMODE;
//KES CHARSTATEDLG 031016
		if(GAMEIN->GetCharStateDialog())
			GAMEIN->GetCharStateDialog()->SetMoveMode( TRUE );
//
	}
	NETWORK->Send(&msg,sizeof(msg));

	if(HERO->m_MoveInfo.MoveMode == eMoveMode_Walk)
		SetRunMode(HERO);
	else
		SetWalkMode(HERO);

	if(bTarget)
	{
		SetHeroTarget(&TargetPos,FALSE);
	}
}

void CMoveManager::ToggleHeroKyungGongMode()
{
	WORD kgidx = HERO->m_MoveInfo.KyungGongIdx;
	MSG_DWORD2 msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_KYUNGGONG_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1 = HEROID;


	VECTOR3 TargetPos;
	BOOL bTarget = FALSE;
	if(IsMoving(HERO))
	{
		bTarget = TRUE;
		TargetPos = *HERO->m_MoveInfo.GetTargetPosition(HERO->m_MoveInfo.GetCurTargetPosIdx());
	}
	
	HeroMoveStop();

	if(kgidx == 0)
	{
		msg.dwData2 = HERO->GetKyungGongGrade();
		SetKyungGong(HERO,HERO->GetKyungGongGrade());
	}
	else
	{
		msg.dwData2 = 0;
		SetKyungGong(HERO,0);
	}
	NETWORK->Send(&msg,sizeof(msg));


	if(bTarget)
	{
		SetHeroTarget(&TargetPos,FALSE);
	}
}


BYTE CMoveManager::GetMoveMode(CObject* pObject)
{
	return pObject->m_MoveInfo.MoveMode;
}

BOOL CMoveManager::IsMoving(CObject* pObject)
{
	return pObject->m_MoveInfo.bMoving;
}


void CMoveManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_MOVE_TARGET:
		{
			MOVE_TARGETPOS* pmsg = (MOVE_TARGETPOS*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
			if(pObject)
			{
				CalcPositionEx(pObject,gCurTime);
				pmsg->GetTargetInfo(&pObject->m_MoveInfo);
				VECTOR3 spos;
				pmsg->spos.Decompress(&spos);
				StartMoveEx(pObject, &spos, gCurTime);
				
/*
				GAMEIN->GetDebugDlg()->DebugMsgParser(DBG_MOVE,"MOVE_START ID: %d",pmsg->dwMoverID);
				GAMEIN->GetDebugDlg()->DebugMsgParser(DBG_MOVE,"sx : %4.2f    sz : %4.2f",pmsg->startx,pmsg->startz);
				GAMEIN->GetDebugDlg()->DebugMsgParser(DBG_MOVE,"tx : %4.2f    tz : %4.2f",pmsg->targetx,pmsg->targetz);
*/
				
			}
			else
			{
//				GAMEIN->GetDebugDlg()->DebugMsgParser(DBG_MOVE,"MOVE_START_NOOBJECT ID: %d",pmsg->dwMoverID);
#ifdef _GMTOOL_
				//쓸데없는 메세지를 보내나 확인해보자.
				++GMTOOLMGR->m_nNullMove;
#endif				
			}
		}
		break;
// jsd
	case MP_MOVE_ONETARGET:
		{
			MOVE_ONETARGETPOS_FROMSERVER* pmsg = (MOVE_ONETARGETPOS_FROMSERVER*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(pObject)
			{
				CalcPositionEx(pObject,gCurTime);
				pmsg->GetTargetInfo(&pObject->m_MoveInfo);
				VECTOR3 spos;
				pmsg->GetStartPos(&spos);
				StartMoveEx(pObject, &spos, gCurTime);
			}
			else
			{
#ifdef _GMTOOL_
				//쓸데없는 메세지를 보내나 확인해보자.
			++GMTOOLMGR->m_nNullMove;
#endif
			}
		}
		break;
//
	case MP_MOVE_PET_STOP:
	case MP_MOVE_STOP:
		{
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
			if(pObject)
			{
/*
				GAMEIN->GetDebugDlg()->DebugMsgParser(DBG_MOVE,"MOVE_STOP ID: %d",pmsg->dwMoverID);
				GAMEIN->GetDebugDlg()->DebugMsgParser(DBG_MOVE,"cx : %4.2f    cz : %4.2f",pObject->GetCurPosition().x,pObject->GetCurPosition().z);
				GAMEIN->GetDebugDlg()->DebugMsgParser(DBG_MOVE,"sx : %4.2f    sz : %4.2f",pmsg->x,pmsg->z);
*/

				VECTOR3 pos;
				pmsg->cpos.Decompress(&pos);
				CMoveManager::MoveStop(pObject,&pos);
			}
			else
			{
//				GAMEIN->GetDebugDlg()->DebugMsgParser(DBG_MOVE,"MOVE_STOP_NOOBJECT ID: %d",pmsg->dwMoverID);
			}
		}
		break;

	case MP_MOVE_WALKMODE:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if( pPlayer )
				SetWalkMode(pPlayer);

			PICONMGR->SetMoveState(pmsg->dwObjectID, eMoveMode_Walk);
		}
		break;
	
	case MP_MOVE_RUNMODE:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if( pPlayer )
				SetRunMode(pPlayer);

			PICONMGR->SetMoveState(pmsg->dwObjectID, eMoveMode_Run);
		}
		break;
	case MP_MOVE_MONSTERMOVE_NOTIFY:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CMonster* pMob = (CMonster*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			BOOL rt = SetMonsterMoveType(pMob, pmsg->wData);
			//ASSERT(rt);
			
		}
		break;
	case MP_MOVE_KYUNGGONG_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			PICONMGR->SetKyungGong(pmsg->dwData1, pmsg->dwData2);

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);
			if(pPlayer == NULL)
				return;

			BOOL rt = SetKyungGong(pPlayer,(WORD)pmsg->dwData2);
			ASSERT(rt);			
		}
		break;

	case MP_MOVE_KYUNGGONG_NACK:
		{
 			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
 			
 			//abil hack check
 			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);
 			if(pPlayer == NULL)
 				return;
 
 			if( pPlayer == HERO )
 			{
 				SetKyungGong(pPlayer,0);
 				HeroMoveStop();
 			}
 
 //			ASSERT(0);
		}
		break;
	case MP_MOVE_EFFECTMOVE:
		{
			/*
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
			if(pObject == NULL)
			{
				return;
			}
			VECTOR3 pos;
			pmsg->cpos.Decompress(&pos);
			EffectMoveInterpolation(pObject,&pos);
			*/
			//ASSERT(0);
		}
		break;
	case MP_MOVE_PET_CORRECTION:
	case MP_MOVE_CORRECTION:
		{	
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
			if(pObject == NULL)
			{
				return;
			}

			VECTOR3 pos;
			pmsg->cpos.Decompress(&pos);

			if(IsMoving(pObject))
				MoveStop(pObject,&pos);
			else
				SetPosition(pObject,&pos);
		}
		break;
	case MP_MOVE_FORCESTOPKYUNGGONG:
		{
			HERO->SetKyungGongMode(FALSE);
		}
		break;
		
	case MP_MOVE_WARP:
		{
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
			if(pObject == NULL)
				return;

			VECTOR3 pos;
			pmsg->cpos.Decompress(&pos);
			MoveStop( pObject, &pos );
			//warp에 대한 이펙트 사용?(로그인 용으로..)
		}
		break;

	case MP_MOVE_PET_WARP_ACK:
		{
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			CPet* pPet = (CPet*)OBJECTMGR->GetObject(pmsg->dwMoverID);
			if(pPet == NULL)
				return;

			VECTOR3 pos;
			pmsg->cpos.Decompress(&pos);


			CalcPositionEx(pPet,gCurTime);

			MoveStop(pPet,&pos);

// 			SetPosition(pPet,&pos);
 
// 			EndMove(pPet);
			TARGETSET set;
			set.pTarget = pPet;
			EFFECTMGR->StartEffectProcess(eEffect_PetWarp,pPet,&set,0,pPet->GetID());

			if( PETMGR->GetCurSummonPet() != pPet )	return;

			if( IsMoving(HERO) )
			{
				WORD count = HERO->m_MoveInfo.GetMaxTargetPosIdx();

				if(count)
					SetPetTarget(&HERO->m_MoveInfo,count);
			}
		}
		break;
	}
}

void CMoveManager::RefreshHeroMove()
{
	if(IsMoving(HERO) == FALSE)
		return;

	VECTOR3 TargetPos = *HERO->m_MoveInfo.GetTargetPosition(HERO->m_MoveInfo.GetCurTargetPosIdx());
	SetHeroTarget(&TargetPos,FALSE);
}
