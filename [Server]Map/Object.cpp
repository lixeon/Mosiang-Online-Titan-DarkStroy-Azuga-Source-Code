// Object.cpp: implementation of the CObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Object.h"
#include "CharMove.h"
//#include "ServerSystem.h"
#include "ObjectStateManager.h"
#include "BattleSystem_Server.h"
#include "PackedData.h"
#include "Network.h"
#include "..\[CC]Header\CommonCalcFunc.h"
#include "StunState.h"
#include "AmplifiedPower.h"
//for event - hs //
#include "Monster.h"
//for event - hs //
#include "Player.h"

#include "DetectState.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CObject::CObject()
{
	m_pSpecialState[eSpecialState_Stun] = new CStunState(this);
	m_pSpecialState[eSpecialState_AmplifiedPowerPhy] = new CAmplifiedPowerPhy(this);
	m_pSpecialState[eSpecialState_AmplifiedPowerAtt] = new CAmplifiedPowerAtt(this);
	m_pSpecialState[eSpecialState_DetectItem] = new CDetectState(this);
	m_bInited = FALSE;
	m_GridID = 0;
//	m_MapNum = 0;
//	m_tempMapNum = 0;
}

CObject::~CObject()
{
	for(int n=0;n<eSpecialState_Max;++n)
		delete m_pSpecialState[n];
}

BOOL CObject::Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	for(int n=0;n<eSpecialState_Max;++n)
		m_pSpecialState[n]->Init();
	SetBattle(pBaseObjectInfo->BattleID,0);

	for(int i = 0; i < eSingleSpecialState_Max; i++)
	{
		m_SingleSpecialStateUsedTime[i] = 0;
		m_StateNaeRyukCountTime[i] = 0;
	}
	
	m_AgentNum = AgentNum;
	m_ObjectKind = kind;
	

	if(pBaseObjectInfo)
		memcpy(&m_BaseObjectInfo,pBaseObjectInfo,sizeof(m_BaseObjectInfo));
	else
		memset(&m_BaseObjectInfo,0,sizeof(BASEOBJECT_INFO));

	memset( &m_MoveInfo, 0, sizeof(MOVE_INFO) );
	
	OBJECTSTATEMGR_OBJ->InitObjectState(this);

	return TRUE;
}

void CObject::Release()
{
	for(int n=0;n<eSpecialState_Max;++n)
		m_pSpecialState[n]->EndState();
	
	m_ObjectKind = 0;
	m_bInited = FALSE;
	m_StatusList.RemoveAll();
	m_bGridInited = FALSE;
}

void CObject::GetBaseObjectInfo(BASEOBJECT_INFO* pRtInfo)
{
	memcpy(pRtInfo,&m_BaseObjectInfo,sizeof(BASEOBJECT_INFO));
}

void CObject::GetSendMoveInfo(SEND_MOVEINFO* pRtInfo,CAddableInfoList* pAddInfoList,BOOL bSetDir /*= FALSE*/)
{
	pRtInfo->CurPos.Compress(CCharMove::GetPosition(this));
	pRtInfo->KyungGongIdx = m_MoveInfo.KyungGongIdx;
	pRtInfo->MoveMode = m_MoveInfo.MoveMode;
	pRtInfo->AbilityKyungGongLevel = m_MoveInfo.AbilityKyungGongLevel;

	if(CCharMove::IsMoving(this))
	{
		if(pAddInfoList == NULL)
		{
			char temp[256];
			sprintf(temp,"Object %s,  Invalid Move State (Not Inited But Moving)");
			ASSERTMSG(0,temp);
			for(int i=0;i<m_MoveInfo.GetMaxTargetPosIdx();++i)
			{
				VECTOR3* pPos = m_MoveInfo.GetTargetPosition(i);
				sprintf(temp,"tpos %d,   (%4.2f,  %4.2f)",i,pPos->x,pPos->z);
				ASSERTMSG(0,temp);
			}
			return;
		}

		COMPRESSEDTARGETPOS tpos;
		tpos.Init();
		for(int i=0;i<m_MoveInfo.GetMaxTargetPosIdx();++i)
			tpos.AddTargetPos(m_MoveInfo.GetTargetPosition(i));
		pAddInfoList->AddInfo(CAddableInfoList::MoveInfo,tpos.GetSize(),&tpos);
	}

	//SW071129 방향 추가
	if( bSetDir )
	{
		pRtInfo->Move_Direction.x = m_MoveInfo.Move_Direction.x;
		pRtInfo->Move_Direction.y = m_MoveInfo.Move_Direction.z;
	}
}
/*
VECTOR3 * CObject::GetCurTargetPosition()
{
	return &m_MoveInfo.TargetPositions[m_MoveInfo.CurTargetPositionIdx];
}*/
void CObject::SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID)
{
	MSG_DWORD* pMsg = (MSG_DWORD*)pRemoveMsg;
	pMsg->Category = MP_USERCONN;
	pMsg->Protocol = MP_USERCONN_OBJECT_REMOVE;
	pMsg->dwObjectID = dwReceiverID;
	pMsg->dwData = GetID();
	
	*pMsgLen = sizeof(MSG_DWORD);
}

void CObject::SetInited()
{
	DoSetInited();
	if(GetObjectKind() != eObjectKind_Player)
	{
		/*
		if(GridPos.x == -1 ||
			GridPos.z == -1 ||
			GridPos.LastX == -1 ||
			GridPos.LastZ == -1)
		{
			ASSERT(0);
		}
		*/
	}

	m_bInited = TRUE;
}

void CObject::SetNotInited()
{
	m_bInited = FALSE;
}

void CObject::GetPosition(VECTOR3* pos)
{
	*pos = *CCharMove::GetPosition(this);
}

void CObject::SetState(BYTE state)
{
	m_ObjectState.BeforeState	= m_BaseObjectInfo.ObjectState;
	m_BaseObjectInfo.ObjectState		= state;
	m_ObjectState.State_Start_Time = gCurTime;
	m_ObjectState.bEndState		= FALSE;
}

int CObject::GetObjectTileSize()
{
	return 0;
}

void CObject::CalcCounterDamage(WORD Attrib,DWORD AttrDamage,DWORD PhyDamage,RESULTINFO* pDamageInfo)
{
	DWORD CounterDamage = 0;
	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetCounterAttackDamage(Attrib,AttrDamage,PhyDamage,pDamageInfo,CounterDamage);
	PTRLISTSEARCHEND;
	pDamageInfo->CounterDamage = CounterDamage;
}

void CObject::CalcVampiric(RESULTINFO* pDamageInfo)
{
	float VampiricLifeRate = 0;
	float VampiricNaeryukRate = 0;
	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetVampiricLife(VampiricLifeRate);
		pSL->GetVampiricNaeryuk(VampiricNaeryukRate);
	PTRLISTSEARCHEND;
	pDamageInfo->Vampiric_Life = (DWORD)(pDamageInfo->RealDamage * VampiricLifeRate);
	pDamageInfo->Vampiric_Naeryuk = (DWORD)(pDamageInfo->RealDamage * VampiricNaeryukRate);

	if( pDamageInfo->Vampiric_Life )
	{
		DWORD CurLife = GetLife();
		DWORD MaxLife = GetMaxLife();
//		ASSERT(CurLife <= MaxLife);
		if(CurLife + pDamageInfo->Vampiric_Life > MaxLife)
		{
			pDamageInfo->Vampiric_Life = MaxLife - CurLife;
		}
		SetLife(CurLife+pDamageInfo->Vampiric_Life,FALSE);
	}
	
	if( (pDamageInfo->Vampiric_Naeryuk) )
	{
		DWORD CurNaeryuk = GetNaeRyuk();
		DWORD MaxNaeryuk = GetMaxNaeRyuk();
//		ASSERT(CurNaeryuk <= MaxNaeryuk);
		if(CurNaeryuk + pDamageInfo->Vampiric_Naeryuk > MaxNaeryuk)
		{
			pDamageInfo->Vampiric_Naeryuk = MaxNaeryuk - CurNaeryuk;
		}
		SetNaeRyuk(CurNaeryuk+pDamageInfo->Vampiric_Naeryuk,FALSE);
	}

}

void CObject::CalcReverseVampiric(RESULTINFO* pDamageInfo)
{
	float VampiricLifeRate = 0.0f;
	float VampiricNaeryukRate = 0.0f;
	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetReverseVampiricLife(VampiricLifeRate);
		pSL->GetReverseVampiricNaeryuk(VampiricNaeryukRate);
	PTRLISTSEARCHEND;
	DWORD VampiricLife		= (DWORD)(pDamageInfo->RealDamage * VampiricLifeRate);
	DWORD VampiricNaeryuk	= (DWORD)(pDamageInfo->RealDamage * VampiricNaeryukRate);

	if( VampiricLife )
	{
		DWORD CurLife = GetLife();
		DWORD MaxLife = GetMaxLife();
//		ASSERT(CurLife <= MaxLife);
		if(CurLife + VampiricLife > MaxLife)
		{
			VampiricLife = MaxLife - CurLife;
		}
		SetLife(CurLife+VampiricLife,TRUE);
	}
	
	if( VampiricNaeryuk )
	{
		DWORD CurNaeryuk = GetNaeRyuk();
		DWORD MaxNaeryuk = GetMaxNaeRyuk();
//		ASSERT(CurNaeryuk <= MaxNaeryuk);
		if(CurNaeryuk + VampiricNaeryuk > MaxNaeryuk)
		{
			VampiricNaeryuk = MaxNaeryuk - CurNaeryuk;
		}
		SetNaeRyuk(CurNaeryuk+VampiricNaeryuk,TRUE);
	}
}

void CObject::CalcRealAttack(CObject* pTarget, DWORD PhyDamage, DWORD AttrDamage, RESULTINFO* pDamageInfo, BOOL bIsContinueAttack  )
{
	DWORD Ori = PhyDamage + AttrDamage;
	DWORD Up = 0,Down = 0;
	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetAttackDamage(Ori,Up,Down);
	PTRLISTSEARCHEND;
//	pDamageInfo->RealDamage = (WORD)STATUSCALC(Ori,Up,Down);	

//	Up = 0;	Down = 0;
	DWORD PhyUp = 0, AttrUp = 0;
	DWORD PhyDown = 0, AttrDown = 0;

	// 2005 크리스마스 이벤트
	if( GetObjectKind() == eObjectKind_Player )
	{
		if( pTarget->GetObjectKind() & eObjectKind_Monster )
		{
			CMonster* pMonster = (CMonster*)pTarget;
			if( pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 || pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2)
			{
				if(  ((CPlayer*)this)->GetWeaponEquipType() == WP_EVENT )
				{
					pDamageInfo->RealDamage = EVENT_DAMAGE;
					return;
				}
				else
				{
					pDamageInfo->RealDamage = 0;
					return;
				}
			}
			else if( pMonster->GetMonsterKind() == EVENT_MONSTER_RABBIT1 || pMonster->GetMonsterKind() == EVENT_MONSTER_RABBIT2 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_RABBIT3 )
			{
				pDamageInfo->RealDamage = EVENT_DAMAGE_RABBIT;
				return;
			}
			//SW080125	구정 이벤트
			else if( pMonster->GetMonsterKind() == EVENT_MONSTER_RAT1 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_RAT2 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_RAT3 )
			{
				pDamageInfo->RealDamage = EVENT_DAMAGE_NEWYEAR;
				return;
			}
//#ifndef _HK_LOCAL_
			else if( pMonster->GetMonsterKind() == EVENT_MONSTER_PUMPHEAD1 || pMonster->GetMonsterKind() == EVENT_MONSTER_PUMPHEAD2 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_PUMPHEAD3 || pMonster->GetMonsterKind() == EVENT_MONSTER_PUMPHEAD4 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_HONEYPIG1 || pMonster->GetMonsterKind() == EVENT_MONSTER_HONEYPIG2 || 
				pMonster->GetMonsterKind() == EVENT_MONSTER_HONEYPIG3 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_CARNATION1 || pMonster->GetMonsterKind() == EVENT_MONSTER_CARNATION2 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_CARNATION3 )
			{
				pDamageInfo->RealDamage = EVENT_DAMAGE_PUMPHEAD;
				return;
			}
//#endif
			else if( ((CPlayer*)this)->GetWeaponEquipType() == WP_EVENT_HAMMER )
			{
				pDamageInfo->RealDamage = EVENT_DAMAGE_HAMMER;
				return;
			}
		}
		else if( ((CPlayer*)this)->GetWeaponEquipType() == WP_EVENT || ((CPlayer*)this)->GetWeaponEquipType() == WP_EVENT_HAMMER )
		{
			pDamageInfo->RealDamage = 0;
			return;
		}
	}

	if( bIsContinueAttack == FALSE )	//지속데미지 공격이 아닌 경우에만 더해준다.
	{
		if( PhyDamage )
		{
			PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
				pSL->GetAttackPhyDamagePlus(PhyDamage,PhyUp,PhyDown);
			PTRLISTSEARCHEND;
		}
		else
		{
			PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
				pSL->GetAttackAttDamagePlus(AttrDamage,AttrUp,AttrDown);
			PTRLISTSEARCHEND;
		}
	}

	DWORD TotalUp = Up + PhyUp + AttrUp;
	DWORD TotalDown = Down + PhyDown + AttrDown;

	pDamageInfo->RealDamage = STATUSCALC(Ori,TotalUp,TotalDown);
}


void CObject::CalcRealDamage(CObject* pOperator,DWORD PhyDamage,DWORD AttrDamage,RESULTINFO* pDamageInfo)
{//CPlayer 에 재정의
	
	// 2005 크리스마스 이벤트
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		if(GetObjectKind() & eObjectKind_Monster)
		{
			CMonster* pMonster = (CMonster*)this;

			if( pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 || pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2)
			{
				if( ((CPlayer*)pOperator)->GetWeaponEquipType() == WP_EVENT )
				{
					pDamageInfo->RealDamage = EVENT_DAMAGE;
					return;
				}
				else
				{
					pDamageInfo->RealDamage = 0;
					return;
				}
			}
			else if( pMonster->GetMonsterKind() == EVENT_MONSTER_RABBIT1 || pMonster->GetMonsterKind() == EVENT_MONSTER_RABBIT2 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_RABBIT3 )
			{
				pDamageInfo->RealDamage = EVENT_DAMAGE_RABBIT;
				return;
			}
			//SW080125	구정 이벤트
			else if( pMonster->GetMonsterKind() == EVENT_MONSTER_RAT1 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_RAT2 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_RAT3 )
			{
				pDamageInfo->RealDamage = EVENT_DAMAGE_NEWYEAR;
				return;
			}
			//SW061211 크리스마스이벤트
			else if( ((CPlayer*)pOperator)->GetWeaponEquipType() == WP_EVENT )
			{
				if( pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_SM
					|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_MD
					|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_LG
					|| pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_SM
					|| pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_MD
					|| pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_LG )
				{
					WORD lv = ((CPlayer*)pOperator)->GetLevel();
					float dmg = ( ( ( (float)lv / 5.0f ) + 1 ) * 750 ) / 8 - 100;

					if( ((CPlayer*)pOperator)->GetEventSnowIdx() == EVENT_ITEM_SNOW )
					{
						pDamageInfo->RealDamage = (DWORD)(dmg * 0.3f);
					}
					else if( ((CPlayer*)pOperator)->GetEventSnowIdx() == EVENT_ITEM_HARDSNOW )
					{
						pDamageInfo->RealDamage = (DWORD)(dmg * 0.6f);
					}
					return;
				}
				else if( pMonster->GetMonsterKind() == EVENT_SNOWMAN_SM
					|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_MD
					|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_LG )
				{
					pDamageInfo->RealDamage = EVENT_DAMAGE;
					return;
				}
				else
				{
					pDamageInfo->RealDamage = 1;
					return;
				}
			}
			else if( pMonster->GetMonsterKind() == EVENT_SNOWMAN_SM
				|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_MD
				|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_LG )
			{
				pDamageInfo->RealDamage = EVENT_DAMAGE;
				return;
			}
			//SW080125	구정 이벤트
			else if( pMonster->GetMonsterKind() == EVENT_MONSTER_RAT1 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_RAT2 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_RAT3 )
			{
				pDamageInfo->RealDamage = EVENT_DAMAGE_NEWYEAR;
				return;
			}
#ifndef _HK_LOCAL_
			else if( pMonster->GetMonsterKind() == EVENT_MONSTER_PUMPHEAD1 || pMonster->GetMonsterKind() == EVENT_MONSTER_PUMPHEAD2 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_PUMPHEAD3 || pMonster->GetMonsterKind() == EVENT_MONSTER_PUMPHEAD4 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_HONEYPIG1 || pMonster->GetMonsterKind() == EVENT_MONSTER_HONEYPIG2 || 
				pMonster->GetMonsterKind() == EVENT_MONSTER_HONEYPIG3 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_CARNATION1 || pMonster->GetMonsterKind() == EVENT_MONSTER_CARNATION2 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_CARNATION3 )
			{
				pDamageInfo->RealDamage = EVENT_DAMAGE_PUMPHEAD;
				return;
			}
#endif
			else if( ((CPlayer*)pOperator)->GetWeaponEquipType() == WP_EVENT_HAMMER )
			{
				pDamageInfo->RealDamage = EVENT_DAMAGE_HAMMER;
				return;
			}
		}
		else if( ((CPlayer*)pOperator)->GetWeaponEquipType() == WP_EVENT || ((CPlayer*)pOperator)->GetWeaponEquipType() == WP_EVENT_HAMMER )
		{
			pDamageInfo->RealDamage = 0;
			return;
		}
	}	
	//SW060330 이전에 쉴드데미지 계산이 이루어진다.
	/*
//회피 게산
	float fDodgeRate = GetDodgeRate();
	if(fDodgeRate != 0)
	{
		if(CheckRandom(fDodgeRate,GetLevel(),pOperator->GetLevel()) == TRUE)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}*/

	// 운기조식중일 경우에는 데미지를 50% 추가한다.
	if(GetState() == eObjectState_Ungijosik)
	{
		PhyDamage = (DWORD)(PhyDamage*1.5f);
		AttrDamage = (DWORD)(AttrDamage*1.5f);		
	}
	
	DWORD Ori = PhyDamage + AttrDamage;
	DWORD Up = 0,Down = 0;
	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetDamage(Ori,Up,Down);
	PTRLISTSEARCHEND;

	pDamageInfo->RealDamage = STATUSCALC(Ori,Up,Down);
}

#ifdef _JAPAN_LOCAL_
DWORD CObject::CalcShieldDamage(DWORD ShieldDamage)
{
	DWORD Shield = GetShield();

	if(Shield < ShieldDamage)
	{
		SetShield(0,FALSE);
		return Shield;
	}
	else
	{
		SetShield(Shield-ShieldDamage,FALSE);
		return ShieldDamage;
	}
}
#else
// 06. 03 국내무쌍 - 이영준
DWORD CObject::CalcShieldDamage(DWORD ShieldDamage, DWORD &RealShieldDamage)
{
	DWORD Shield = GetShield();
	RealShieldDamage = ShieldDamage;

	if(Shield < ShieldDamage)
	{
		SetShield(0,FALSE);
		return (WORD)Shield;
	}
	else
	{
		SetShield(Shield-ShieldDamage,FALSE);
		return ShieldDamage;
	}
}
#endif

void CObject::Die(CObject* pAttacker)
{
	m_StatusList.RemoveAll();

	// 현재 속해 있는 Battle에서 처리해보고 처리하지 못했다면(return FALSE),
	// 각 클래스들에게 처리를 맡긴다.
	DWORD AttackerID;
	if(pAttacker == NULL)
		AttackerID = 0;
	else 
		AttackerID = pAttacker->GetID();
	if(m_pBattle->OnTeamMemberDie(GetBattleTeam(),GetID(),AttackerID) == FALSE)
		DoDie(pAttacker);
		
	for(int n=0;n<eSpecialState_Max;++n)
		m_pSpecialState[n]->EndState();
}

//주의: CPlayer::Damage를 만들었다. 수정사항이 있다면 CPlayer::Damage도 수정하도록 하시오. KES
DWORD CObject::Damage(CObject* pAttacker,RESULTINFO* pDamageInfo)
{
	DWORD life = GetLife();
	DWORD beforelife = life;
	
	//for event - hs //
	if(GetObjectKind() & eObjectKind_Monster)
	{
		CMonster* pMonster = (CMonster*)this;
		if( pMonster->GetMonsterKind() == 143 || pMonster->GetMonsterKind() == 139 
			|| pMonster->GetMonsterKind() == 141 )
		{
			pDamageInfo->RealDamage = 66;
		}
		else if( pMonster->GetMonsterKind() == 144 || pMonster->GetMonsterKind() == 140
			|| pMonster->GetMonsterKind() == 142 )
		{
			pDamageInfo->RealDamage = 88;
		}
		else if( pMonster->GetMonsterKind() == 170 || pMonster->GetMonsterKind() == 171 )
		{
			pDamageInfo->RealDamage = 1000;
		}
		else if( pMonster->GetMonsterKind() == 172 || pMonster->GetMonsterKind() == 173		// 초코바
			|| pMonster->GetMonsterKind() == 174 || pMonster->GetMonsterKind() == 175 )		// 발렌타인
		{
			pDamageInfo->RealDamage = 10;
		}
		else if( pMonster->GetMonsterKind() == 176 || pMonster->GetMonsterKind() == 177 || pMonster->GetMonsterKind() == 178 )	// 차바차바
		{
			pDamageInfo->RealDamage = 7;
		}
	}

	
	//	2005 크리스마스 이벤트 코드
	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pPlayer = (CPlayer*) pAttacker;
		
		if(GetObjectKind() & eObjectKind_Monster)
		{
			CMonster* pMonster = (CMonster*)this;

			if( pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 || pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2)
			{
				if( pPlayer->GetWeaponEquipType() == WP_EVENT )
					pDamageInfo->RealDamage = EVENT_DAMAGE;
				else
					pDamageInfo->RealDamage = 0;
			}
			else if( pMonster->GetMonsterKind() == EVENT_MONSTER_RABBIT1 || pMonster->GetMonsterKind() == EVENT_MONSTER_RABBIT2 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_RABBIT3 )
				pDamageInfo->RealDamage = EVENT_DAMAGE_RABBIT;
			//SW080125	구정 이벤트
			else if( pMonster->GetMonsterKind() == EVENT_MONSTER_RAT1 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_RAT2 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_RAT3 )
			{
				pDamageInfo->RealDamage = EVENT_DAMAGE_NEWYEAR;
			}
#ifndef _HK_LOCAL_
			else if( pMonster->GetMonsterKind() == EVENT_MONSTER_PUMPHEAD1 || pMonster->GetMonsterKind() == EVENT_MONSTER_PUMPHEAD2 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_PUMPHEAD3 || pMonster->GetMonsterKind() == EVENT_MONSTER_PUMPHEAD4 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_HONEYPIG1 || pMonster->GetMonsterKind() == EVENT_MONSTER_HONEYPIG2 || 
				pMonster->GetMonsterKind() == EVENT_MONSTER_HONEYPIG3 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_CARNATION1 || pMonster->GetMonsterKind() == EVENT_MONSTER_CARNATION2 ||
				pMonster->GetMonsterKind() == EVENT_MONSTER_CARNATION3 )
				pDamageInfo->RealDamage = EVENT_DAMAGE_PUMPHEAD;
#endif
			else if( pPlayer->GetWeaponEquipType() == WP_EVENT_HAMMER )
				pDamageInfo->RealDamage = EVENT_DAMAGE_HAMMER;
		}
		else if( pPlayer->GetWeaponEquipType() == WP_EVENT || pPlayer->GetWeaponEquipType() == WP_EVENT_HAMMER )
			pDamageInfo->RealDamage = 0;
	}

	// 2007. 7. 6. CBH - 전문기술 몬스터 데미지 추가 (횟수와 관계 : 300 고정) 
	if( GetObjectKindGroup(GetObjectKind()) == eOBJECTKINDGROUP_JOB )
	{
		pDamageInfo->RealDamage = JOBSKILL_DEMAGE;
	}
	
	//for event//

	if(life > pDamageInfo->RealDamage)
		life -= pDamageInfo->RealDamage;
	else
		life = 0;
	
	SetLife(life,FALSE);

	DoDamage(pAttacker,pDamageInfo,beforelife);

	return life;
}

void CObject::AddLife(DWORD val,DWORD* realAddLife,BOOL bSendMsg)
{	
	DWORD life = GetLife();
	DWORD maxlife = GetMaxLife();
	
	if(life+val > maxlife)
	{
		val = maxlife - life;
	}
	SetLife(life+val,bSendMsg);

	if(realAddLife)
		*realAddLife = val;
}

// RaMa 04.11.04
void CObject::AddShield(DWORD val)
{
	DWORD shield = GetShield();
	DWORD maxshield = GetMaxShield();

	if( shield+val > maxshield )
		val = maxshield - shield;

	SetShield(shield+val);
}

void CObject::AddNaeRyuk(DWORD val,DWORD* realAddNaeRyuk)
{
	DWORD naeryuk = GetNaeRyuk();
	DWORD maxnaeryuk = GetMaxNaeRyuk();
	
	if(naeryuk+val > maxnaeryuk)
	{
		val = maxnaeryuk - naeryuk;
	}
	SetNaeRyuk(naeryuk+val);

	if(realAddNaeRyuk)
		*realAddNaeRyuk = val;
}

void CObject::ReduceNaeRyuk(DWORD val)
{
	if(GetNaeRyuk() < val)
		SetNaeRyuk(0);
	else
		SetNaeRyuk(GetNaeRyuk()-val);
}

void CObject::CheckLifeValid()
{
	DWORD MaxLife = GetMaxLife();
	if(GetLife() > MaxLife)
		SetLife(MaxLife);
}
void CObject::CheckNaeRyukValid()
{
	DWORD MaxNaeRyuk = GetMaxNaeRyuk();
	if(GetNaeRyuk() > MaxNaeRyuk)
		SetNaeRyuk(MaxNaeRyuk);
}
void CObject::CheckShieldValid()
{
	DWORD maxShield = GetMaxShield();
	if(GetShield() > maxShield)
		SetShield(maxShield);
}

//////////////////////////////////////////////////////////////////////////
// Status
void CObject::AddStatus(CStatus* pStatus)
{
	m_StatusList.AddTail(pStatus);

	CheckLifeValid();
	CheckNaeRyukValid();
	CheckShieldValid();
}
void CObject::RemoveStatus(CStatus* pStatus)
{
	BOOL rt = m_StatusList.Remove(pStatus);

	CheckLifeValid();
	CheckNaeRyukValid();
	CheckShieldValid();
}


float CObject::GetAttDefense(WORD Attrib)
{
	if(Attrib == ATTR_NOATTR)		// 무속성은 속성방어력이 0이다.
		return 0;

	float Ori = DoGetAttDefense(Attrib);
	float Up = 0,Down = 0;
	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetAttDefense(Attrib,Ori,Up,Down);
	PTRLISTSEARCHEND;
	float res = STATUSCALC(Ori,Up,Down);
	if(res > 1)
		res = 1;	
	return res;
}

//////////////////////////////////////////////////////////////////////////
// battle
void CObject::SetBattle( DWORD BattleID, BYTE Team, DWORD Param )
{
	m_BaseObjectInfo.BattleID = BattleID;
	m_BaseObjectInfo.BattleTeam = Team;
	m_pBattle = BATTLESYSTEM->GetBattle(BattleID);
	
	if(m_pBattle == NULL)
	{
		ASSERT(0);
		return;
	}

	if(m_bInited == FALSE)
		return;

	MSG_DWORD4 msg;
	msg.Category	= MP_BATTLE;
	msg.Protocol	= MP_BATTLE_CHANGE_OBJECTBATTLE;
	msg.dwObjectID	= GetID();
	msg.dwData1		= BattleID;
	msg.dwData2		= Team;
	msg.dwData3		= m_pBattle->GetBattleKind();
	msg.dwData4		= Param;

	PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof(msg) );
}


void CObject::SendMsg(MSGBASE* pMsg,int MsgLen)
{
	pMsg->dwObjectID = GetID();
	g_Network.Send2Server(GetAgentNum(),(char*)pMsg,MsgLen);
}


void CObject::StartSpecialState(DWORD SpecialStateKind,DWORD Time,
								WORD wParam1,WORD wParam2,float fParam3,
								WORD EffectNum,WORD StateIcon)
{
	m_pSpecialState[SpecialStateKind]->StartState(Time,wParam1,wParam2,fParam3,EffectNum,StateIcon);
}
void CObject::EndSpecialState(DWORD SpecialStateKind)
{
	m_pSpecialState[SpecialStateKind]->EndState();
}
BOOL CObject::IsInSpecialState(DWORD SpecialStateKind)
{
	return m_pSpecialState[SpecialStateKind]->IsInState();
}
void CObject::SpecialStateProcess(DWORD TickTime)
{
	for(int n=0;n<eSpecialState_Max;++n)
	{
		m_pSpecialState[n]->Process(TickTime);
	}
}
WORD CObject::GetSpecialStateParam1(DWORD SpecialStateKind)
{
	return m_pSpecialState[SpecialStateKind]->GetParam1();
}
WORD CObject::GetSpecialStateParam2(DWORD SpecialStateKind)
{
	return m_pSpecialState[SpecialStateKind]->GetParam2();
}
float CObject::GetSpecialStateParam3(DWORD SpecialStateKind)
{
	return m_pSpecialState[SpecialStateKind]->GetParam3();
}

DWORD CObject::GetRecoverLife()
{
	DWORD UpLife = 0;
	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
	pSL->GetRecoverLife(UpLife);
	PTRLISTSEARCHEND;

	return UpLife;
}

DWORD CObject::GetRecoverShield()
{
	DWORD UpShield = 0;
	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
	pSL->GetRecoverShield(UpShield);
	PTRLISTSEARCHEND;

	return UpShield;
}

DWORD CObject::GetRecoverNaeryuk()
{
	DWORD UpNaryuk = 0;
	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
	pSL->GetRecoverNaeryuk(UpNaryuk);
	PTRLISTSEARCHEND;

	return UpNaryuk;
}

void CObject::SendSingleSpeicalStateAck(WORD State, bool bVal, bool Forced)
{
	MSG_SINGLE_SPECIAL_STATE msg;

	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_SINGLE_SPECIAL_STATE_ACK;
	msg.Operator = GetID();
	msg.bVal = bVal;
	msg.State = State;
	msg.Forced = Forced;

	SendMsg(&msg, sizeof(msg));

	msg.Protocol = MP_CHAR_SINGLE_SPECIAL_STATE_NOTIFY;

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(this, &msg, sizeof(msg));
}

void CObject::SendSingleSpeicalStateNack(WORD State, WORD Err)
{
	MSG_SINGLE_SPECIAL_STATE_NACK msg;

	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_SINGLE_SPECIAL_STATE_NACK;
	msg.State = State;
	msg.ErrCode = Err;

	SendMsg(&msg, sizeof(msg));
}
