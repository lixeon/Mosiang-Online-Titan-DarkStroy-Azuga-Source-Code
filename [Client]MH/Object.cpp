// Object.cpp: implementation of the CObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Object.h"
#include "MoveManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "ObjectStateManager.h"
#include "./interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "ObjectBalloon.h"
#include "OBalloonChat.h"
#include "OBalloonName.h"
#include "StreetStallTitleTip.h"
#include "StunState.h"
#include "AmplifiedPower.h"

#include "ObjectGuagen.h"
#include "GameResourceManager.h"
#include "Status.h"

#include "MOTIONDESC.h"

#include "Hero.h"
#include "BattleSystem_Client.h"

#include "DetectState.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObject::CObject()
{
	m_pSpecialState[eSpecialState_Stun] = new CStunState(this);
	m_pSpecialState[eSpecialState_AmplifiedPowerPhy] = new CAmplifiedPowerPhy(this);
	m_pSpecialState[eSpecialState_AmplifiedPowerAtt] = new CAmplifiedPowerAtt(this);
	m_pObjectBalloon		= NULL;
	m_bObjectBalloonBits	= 0;
	m_StateEffectList.Initialize(3);
	m_pMotionDESC			= NULL;
	m_bSelected				= FALSE;

	m_ObjectKind			= 0;
}

CObject::~CObject()
{
	Release();
	for(int n=0;n<eSpecialState_Max;++n)
		delete m_pSpecialState[n];
}

BOOL CObject::Init(EObjectKind kind,BASEOBJECT_INFO* pBaseObjectInfo)
{
	for(int n=0;n<eSpecialState_Max;++n)
		m_pSpecialState[n]->Init();

	for(int i = 0; i < eSingleSpecialState_Max; i++)
		m_SingleSpecialStateUsedTime[i] = 0;

	m_bMoveSkipCount = 0;
	m_bIsYMoving = FALSE;

	memcpy(&m_BaseObjectInfo,pBaseObjectInfo,sizeof(m_BaseObjectInfo));

	m_ObjectKind = kind;
	m_ObjectState.bEndState = FALSE;

	SetFlag(FALSE);
//m_bDieFlag = FALSE;
	m_bInited = TRUE;

	return TRUE;
}


void CObject::InitObjectBalloon(BYTE bitFlag)
{
	m_pObjectBalloon = new CObjectBalloon;

	m_bObjectBalloonBits = bitFlag;

	BYTE objectKind = GetObjectKind();
	if(objectKind == eObjectKind_Player)
	{
		// 남자, 혹은 여자
		m_pObjectBalloon->InitBalloon(this, 170, bitFlag);		
	}
	else if(objectKind & eObjectKind_Pet)
	{
		CPet* pPet = (CPet*)this;
		m_pObjectBalloon->InitBalloon(this, pPet->GetSInfo()->Tall[(pPet->GetPetCurGrade())-1], bitFlag);
	}
	else if(objectKind & eObjectKind_Monster)
	{
		BASE_MONSTER_LIST* pData = ((CMonster*)this)->GetSInfo();
		m_pObjectBalloon->InitBalloon(this, (LONG)(pData->Tall*pData->Scale), bitFlag);	
	}
	else if(objectKind & eObjectKind_Npc)
	{
		m_pObjectBalloon->InitBalloon(this, ((CNpc *)this)->GetSInfo()->Tall, bitFlag);	
	}
	else
	{
		m_pObjectBalloon->InitBalloon(this, 180, bitFlag);
	}

	if(bitFlag & ObjectBalloon_Name)
	{
		if( (objectKind == eObjectKind_Monster) || (objectKind == eObjectKind_TitanMonster) )
			m_pObjectBalloon->GetOBalloonName()->InitName(((CMonster *)this)->GetSInfo()->Name);
		else
        	m_pObjectBalloon->GetOBalloonName()->InitName(m_BaseObjectInfo.ObjectName);

		if(objectKind == eObjectKind_Player)
		{
			if( ((CPlayer*)this)->GetGuildMarkName() > 0 ) 
				SetGuildMark();
			m_pObjectBalloon->SetNickName(((CPlayer*)this)->GetNickName());
		}

		if(objectKind == eObjectKind_Npc)
		{
			if( ((CNpc*)this)->GetSInfo()->ShowJob )
				m_pObjectBalloon->SetNickName(((CNpc*)this)->GetSInfo()->Name);
			/*
			switch(((CNpc*)this)->GetNpcKind())
			{	// 직업 안보이는 것들
			case 16:	//상징물
			case 22:	//초보자안내인
			case 23:	//보물상자
			case 24:	//황릉영혼
			case 26:	//상자
			case 27:	//퀘스트중맵이동
			case 29:	//맵전환보조
			case 30:	//로그인
			case 31:	//맵전환
			case 32:	//옥문관표국
			case 33:	//명사산표국
			case 34:	//월아천표국
			case 35:	//탑이사표국
			case 36:	//화청지표국
			case 37:	//정주표국
			case 38:	//연무장간판
			case 40:	//상자
			case 43:	//알
			case 44:	//알수없음
				break;
			default:
				m_pObjectBalloon->SetNickName(((CNpc*)this)->GetSInfo()->Name);
			}
			*/
		}

	}

	// chat
	cImage top1, top2, top3, middle1, middle2, middle3, bottom1, bottom2, bottom3, tail; 
	SCRIPTMGR->GetImage(66, &top1);
	SCRIPTMGR->GetImage(67, &top2);
	SCRIPTMGR->GetImage(68, &top3);
	SCRIPTMGR->GetImage(69, &middle1);
	SCRIPTMGR->GetImage(70, &middle2);
	SCRIPTMGR->GetImage(71, &middle3);
	SCRIPTMGR->GetImage(72, &bottom1);
	SCRIPTMGR->GetImage(73, &bottom2);
	SCRIPTMGR->GetImage(74, &bottom3);
	SCRIPTMGR->GetImage(75, &tail);


	if( bitFlag & ObjectBalloon_Chat )
	{
		m_pObjectBalloon->GetOBalloonChat()->InitBalloon(&top1, &top3, &bottom1, &bottom3, &middle1, &middle3, &top2, &bottom2, &middle2, &tail, 8);

		// RaMa - 04.11.29   ->채팅과 노점상은 3번의 렌더로 찍는다.
		cImage left1;
		cImage left2;
		cImage left3;
		cImage center1;
		cImage center2;
		cImage center3;
		cImage right1;
		cImage right2;
		cImage right3;
		
		SCRIPTMGR->GetImage(99, &left1);
		SCRIPTMGR->GetImage(100, &center1);
		SCRIPTMGR->GetImage(101, &right1);
		SCRIPTMGR->GetImage(102, &left2);
		SCRIPTMGR->GetImage(103, &center2);
		SCRIPTMGR->GetImage(104, &right2);
		SCRIPTMGR->GetImage(105, &left3);
		SCRIPTMGR->GetImage(106, &center3);
		SCRIPTMGR->GetImage(107, &right3);

		m_pObjectBalloon->GetOBalloonChat()->InitBalloonForChat(&left1, &center1, &right1,
			&left2, &center2, &right2, &left3, &center3, &right3, 8);
	}

	if( bitFlag & ObjectBalloon_Title)
	{
		m_pObjectBalloon->GetSSTitleTip()->InitBalloon(&top1, &top3, &bottom1, &bottom3, &middle1, &middle3, &top2, &bottom2, &middle2, NULL, 8);
		m_pObjectBalloon->GetSBSTitleTip()->InitBalloon(&top1, &top3, &bottom1, &bottom3, &middle1, &middle3, &top2, &bottom2, &middle2, NULL, 8);

		// StreetStall Title
		{						
			// RaMa - 04.11.29   ->채팅과 노점상은 3번의 렌더로 찍는다.
			cImage left1;
			cImage left2;
			cImage left3;
			cImage center1;
			cImage center2;
			cImage center3;
			cImage right1;
			cImage right2;
			cImage right3;
			
			SCRIPTMGR->GetImage(117, &left1);
			SCRIPTMGR->GetImage(118, &center1);
			SCRIPTMGR->GetImage(119, &right1);
			SCRIPTMGR->GetImage(120, &left2);
			SCRIPTMGR->GetImage(121, &center2);
			SCRIPTMGR->GetImage(122, &right2);
			SCRIPTMGR->GetImage(123, &left3);
			SCRIPTMGR->GetImage(124, &center3);
			SCRIPTMGR->GetImage(125, &right3);		
			
			m_pObjectBalloon->GetSSTitleTip()->InitBalloonForChat(&left1, &center1, &right1,
				&left2, &center2, &right2, &left3, &center3, &right3, 8);
		}
		{						
			// RaMa - 04.11.29   ->채팅과 노점상은 3번의 렌더로 찍는다.
			cImage left1;
			cImage left2;
			cImage left3;
			cImage center1;
			cImage center2;
			cImage center3;
			cImage right1;
			cImage right2;
			cImage right3;
			
			SCRIPTMGR->GetImage(128, &left1);
			SCRIPTMGR->GetImage(129, &center1);
			SCRIPTMGR->GetImage(130, &right1);
			SCRIPTMGR->GetImage(131, &left2);
			SCRIPTMGR->GetImage(132, &center2);
			SCRIPTMGR->GetImage(133, &right2);
			SCRIPTMGR->GetImage(134, &left3);
			SCRIPTMGR->GetImage(135, &center3);
			SCRIPTMGR->GetImage(136, &right3);		
			
			m_pObjectBalloon->GetSBSTitleTip()->InitBalloonForChat(&left1, &center1, &right1,
				&left2, &center2, &right2, &left3, &center3, &right3, 8);
		}
	}	
}

//void CObject::AddChatBalloon( char * chatMsg )
//{
//	ShowChatBalloon( TRUE, chatMsg );
//}

//-----
void CObject::SetOverInfoOption( DWORD dwOption )
{
	if( m_pObjectBalloon )	
		m_pObjectBalloon->SetOverInfoOption( dwOption );
}

void CObject::SetGuildMark()
{
	if( m_pObjectBalloon )
		m_pObjectBalloon->SetGuildMark( ((CPlayer*)this)->GetGuildMarkName() );
}

void CObject::SetNickName()
{
	if( m_pObjectBalloon )
		m_pObjectBalloon->SetNickName( ((CPlayer*)this)->GetNickName() );
}

void CObject::ShowObjectName( BOOL bShow, DWORD dwColor )		//show/hide
{
	if( !m_pObjectBalloon )								return;
	if( !(m_bObjectBalloonBits & ObjectBalloon_Name) )	return;

	m_pObjectBalloon->ShowObjectName( bShow, dwColor );
}

void CObject::ShowChatBalloon( BOOL bShow, char* chatMsg, DWORD dwColor, DWORD dwAliveTime )
{
	if( !m_pObjectBalloon )								return;
	if( !(m_bObjectBalloonBits & ObjectBalloon_Chat) )	return;

	m_pObjectBalloon->ShowChatBalloon( bShow, chatMsg, dwColor, dwAliveTime );
}

//-----

void CObject::InitMove(BASEMOVE_INFO* pMoveInfo)
{
	memset(&m_MoveInfo,0,sizeof(MOVE_INFO));
	memcpy(&m_MoveInfo,pMoveInfo,sizeof(BASEMOVE_INFO));
}

void CObject::Release()
{
	m_StatusList.RemoveAll();
	
	for(int n=0;n<eSpecialState_Max;++n)
		m_pSpecialState[n]->EndState();
	m_bMoveSkipCount = 0;
	m_bInited = FALSE;

	if(m_ShadowObj.IsInited())
	{
		m_ShadowObj.Release();
	}
	RemoveAllObjectEffect();

	m_ObjectKind = 0;
	m_BaseObjectInfo.ObjectState = -1;
	m_EngineObject.Release();
	
	SAFE_DELETE( m_pObjectBalloon );
	m_bObjectBalloonBits = 0;
}

void CObject::GetBaseObjectInfo(BASEOBJECT_INFO* pRtInfo)
{
	memcpy(pRtInfo,&m_BaseObjectInfo,sizeof(BASEOBJECT_INFO));
}

void CObject::GetBaseMoveInfo(BASEMOVE_INFO* pRtInfo)
{
	memcpy(pRtInfo,&m_MoveInfo,sizeof(BASEMOVE_INFO));
}

void CObject::Process()
{
	SpecialStateProcess(gTickTime);
	OBJECTSTATEMGR->StateProcess(this);
	MOVEMGR->MoveProcess(this);

	if(m_ShadowObj.IsInited())
		m_ShadowObj.SetEngObjPosition(&GetCurPosition());
	
	/*
	HEFFPROC hEff;
	m_StateEffectList.SetPositionHead();
	while(hEff = m_StateEffectList.GetData())
		EFFECTMGR->Process(gCurTime,hEff);
	*/
}

void CObject::SetFlag(BOOL val)
{
	m_bDieFlag = val; 
}

void CObject::SetDieFlag()
{
	m_DiedTime = gCurTime;

	// 죽었을때의 처리
	SetFlag(TRUE);
//	m_bDieFlag = TRUE;
	MOVEMGR->MoveStop(this);
	HERO->ClearTarget(this);

	// LBS 03.11.21
	if( OBJECTMGR->GetSelectedObjectID() == GetID() )
	{
		OBJECTMGR->SetSelectedObjectID(0);

		//KES 추가	//여기서 하는게 이상하다. 몬스터게이지를 살려두기위해서 좀 꼬였다.
		OnDeselected();
		if( OBJECTMGR->CanOverInfoChange( this ) )
			ShowObjectName( FALSE );
		else
			ShowObjectName( TRUE, OBJECTMGR->GetObjectNameColor(this) );
		//
	}
	
//	OBJECTSTATEMGR->StartObjectState(this, eObjectState_Die);
	
	//CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(133), );
	// 죽으면 클릭 안되게 함
	if( m_ObjectKind == eObjectKind_Player )
		m_EngineObject.EnablePick();
	else
		m_EngineObject.DisablePick();
}

void CObject::Revive(VECTOR3* pRevivePos)
{
	SetFlag(FALSE);
//	m_bDieFlag = FALSE;
	MOVEMGR->SetPosition(this,pRevivePos);
}

void CObject::SetState(BYTE State)
{
	SetMotionInState(State);
	
	if(m_ObjectState.State_Start_Motion != -1)
	{
//		if(State != m_BaseObjectInfo.ObjectState)
			ChangeMotion(m_ObjectState.State_Start_Motion,FALSE);
		if(m_ObjectState.State_Ing_Motion != -1)
			ChangeBaseMotion(m_ObjectState.State_Ing_Motion);
	}
	m_ObjectState.BeforeState	= m_BaseObjectInfo.ObjectState;
	m_BaseObjectInfo.ObjectState		= State;
	m_ObjectState.State_Start_Time = gCurTime;
	m_ObjectState.bEndState		= FALSE;
}

BYTE CObject::GetState()
{
	return m_BaseObjectInfo.ObjectState;
}

void CObject::SetPosition(VECTOR3* pPos)
{
	GetEngineObject()->SetEngObjPosition(pPos);
}
void CObject::SetAngle(float AngleRad)
{
	GetEngineObject()->SetEngObjAngle(AngleRad);
}
void CObject::GetPosition(VECTOR3* pPos)
{
	GetEngineObject()->GetEngObjPosition(pPos);
	ASSERTMSG(pPos->x > -1000,"이 메세지 도 윤호한테 연락을...");
	ASSERTMSG(pPos->z > -1000,"이 메세지 도 윤호한테 연락을...");
}
float CObject::GetAngle()
{
	return GetEngineObject()->GetEngObjAngle();
}
DIRINDEX CObject::GetDirectionIndex()
{
	float angleRad = GetAngle();
	DIRINDEX dir = RADTODIR(angleRad);
	while(dir >= MAX_DIRECTION_INDEX)
		dir -= MAX_DIRECTION_INDEX;
	return dir;
}

void CObject::AddStatus(CStatus* pStatus)
{
	m_StatusList.AddTail(pStatus);
}
void CObject::RemoveStatus(CStatus* pStatus)
{
	BOOL rt = m_StatusList.Remove(pStatus);
}

void CObject::ChangeMotion(int motion,BOOL bRoop)
{
	GetEngineObject()->ChangeMotion(motion,bRoop);
}
void CObject::ChangeBaseMotion(int motion)
{
	GetEngineObject()->ChangeBaseMotion(motion);
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
#ifndef _JAPAN_LOCAL_	
	if(res > 1)
		res = 1;
#endif
	return res;
}

void CObject::OnSelected()
{
	GetEngineObject()->EnableSelfIllumin();
	m_bSelected = TRUE;
}

void CObject::OnDeselected()
{
	GetEngineObject()->DisableSelfIllumin();
	m_bSelected = FALSE;
}

void CObject::SetBattle(DWORD BattleID,BYTE Team)
{
	m_BaseObjectInfo.BattleID = BattleID;
	m_BaseObjectInfo.BattleTeam = Team;
}

void CObject::AddObjectEffect(DWORD KeyValue,OBJECTEFFECTDESC* EffectNumArray,WORD NumOfEffect,CObject* pSkillOperator)
{
	// test
//	if(GetState() == eObjectState_Die)
//	{
//		int a=0;
//	}

	
	HEFFPROC heff;

	// 기존에 있던 이펙트를 찾는다.
	BOOL bFound = FALSE;
	m_StateEffectList.StartGetMultiData(KeyValue);
	while(heff = (HEFFPROC)m_StateEffectList.GetMultiData())
	{
		EFFECTMGR->IncEffectRefCount(heff);
		bFound = TRUE;
	}

	// 있었으면 리턴		//kes***
	if(bFound == TRUE)
		return;

	// 없었으면 새로 만든다.
	for(int n=0;n<NumOfEffect;++n)
	{
		if(EffectNumArray[n].Effect == 0)
			continue;
		heff = EFFECTMGR->StartSkillMainTainEffect(
			EffectNumArray[n].Effect,this,EffectNumArray[n].Flag,&EffectNumArray[n].Position,pSkillOperator);
		if(heff == NULL)
			continue;
		m_StateEffectList.Add((void*)heff,KeyValue);
	}
}

void CObject::RemoveObjectEffect(DWORD KeyValue)
{
    if( m_bInited != 1 )
	{
		ASSERT(0);
		return;
	}

	HEFFPROC heff;
	// 기존에 있던 이펙트를 찾는다.
	BOOL bFound = FALSE;
	m_StateEffectList.StartGetMultiData(KeyValue);
	
	cPtrList list;

	while(heff = (HEFFPROC)m_StateEffectList.GetMultiData())
	{
		if(EFFECTMGR->ForcedEndEffect(heff) == TRUE)
		{
//			m_StateEffectList.RemoveCurMultiData();
			list.AddTail((void*)KeyValue);
		}
	}

	PTRLISTPOS pos = list.GetHeadPosition();
	while( pos )
	{
		DWORD k = (HEFFPROC)list.GetNext(pos);
		m_StateEffectList.Remove(k);
	}
	list.RemoveAll();

}
void CObject::RemoveAllObjectEffect()
{
	HEFFPROC heff;
	m_StateEffectList.SetPositionHead();
	while(heff = (HEFFPROC)m_StateEffectList.GetData())
	{
		while(EFFECTMGR->ForcedEndEffect(heff) == FALSE)
		{}
	}
	m_StateEffectList.RemoveAll();	//KES 리스트에서 지워도 되나? forceEnd가 안된경우는..confirm
}
int	CObject::GetMotionIDX(int mainMotion, int subMotion)
{
	return m_pMotionDESC->GetMotionIDX(mainMotion, subMotion);
}

void CObject::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive)
{
	for(int n=0;n<eSpecialState_Max;++n)
		m_pSpecialState[n]->EndState();

//	RemoveAllObjectEffect();	
//	m_StatusList.RemoveAll();
}

void CObject::StartSpecialState(DWORD SpecialStateKind,DWORD Time,
								WORD wParam1,WORD wParam2,float fParam3,
								WORD EffectNum,WORD StateIcon,BOOL bHeroOper)
{
	m_pSpecialState[SpecialStateKind]->StartState(Time,wParam1,wParam2,fParam3,EffectNum,StateIcon,bHeroOper);
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

BOOL CObject::IsNpcMark( DWORD dwValue )
{
	HEFFPROC heff = (HEFFPROC)m_StateEffectList.GetData( dwValue );
	if( heff )
		return TRUE;
	return FALSE;
}

void CObject::SetKyungGongLevel( WORD wLevel )
{
	m_MoveInfo.AbilityKyungGongLevel = wLevel;
}

void CObject::SetObjectBalloonTall(LONG Tall)
{
	m_pObjectBalloon->SetTall(Tall);
}

// 
// void CObject::InitObjectBalloon(BYTE bitFlag)
// {
// 	m_pObjectBalloon = new CObjectBalloon;
// 
// 	m_bObjectBalloonBits = bitFlag;
// 
// 	if(GetObjectKind() == eObjectKind_Player)
// 	{
// 		// 남자, 혹은 여자
// 		m_pObjectBalloon->InitBalloon(this, 170, bitFlag);		
// 	}
// 	else if(GetObjectKind() & eObjectKind_Pet)
// 	{
// 		//m_pObjectBalloon->InitBalloon(this, ((CPet*)this)->GetSInfo()->Tall, bitFlag);
// 		CPet* pPet = (CPet*)this;
// 		m_pObjectBalloon->InitBalloon(this, pPet->GetSInfo()->Tall[(pPet->GetPetCurGrade())-1], bitFlag);
// 	}
// 	else if(GetObjectKind() & eObjectKind_Monster)
// 	{
// 		m_pObjectBalloon->InitBalloon(this, ((CMonster *)this)->GetSInfo()->Tall, bitFlag);	
// 	}
// 	else if(GetObjectKind() & eObjectKind_Npc)
// 	{
// 		m_pObjectBalloon->InitBalloon(this, ((CNpc *)this)->GetSInfo()->Tall, bitFlag);	
// 	}
// 	else
// 	{
// 		m_pObjectBalloon->InitBalloon(this, 180, bitFlag);
// 	}
// 
// 	if(bitFlag & ObjectBalloon_Name)
// 	{
// 		if(GetObjectKind() == eObjectKind_Monster)
// 			m_pObjectBalloon->GetOBalloonName()->InitName(((CMonster *)this)->GetSInfo()->Name);
// 		else
// 			m_pObjectBalloon->GetOBalloonName()->InitName(m_BaseObjectInfo.ObjectName);
// 
// 		if(GetObjectKind() == eObjectKind_Player)
// 		{
// 			if( ((CPlayer*)this)->GetGuildMarkName() > 0 ) 
// 				SetGuildMark();
// 			m_pObjectBalloon->SetNickName(((CPlayer*)this)->GetNickName());
// 
// 			/*			cImage left;
// 			cImage center;
// 			cImage right;
// 			SCRIPTMGR->GetImage(99, &left);
// 			SCRIPTMGR->GetImage(100, &center);
// 			SCRIPTMGR->GetImage(101, &right);			
// 			m_pObjectBalloon->GetOBalloonChat()->InitBalloonForChat(&left, &center, &right,
// 			&left, &center, &right, &left, &center, &right, 8);*/
// 		}
// 
// 		if(GetObjectKind() == eObjectKind_Npc)
// 		{
// 			if( ((CNpc*)this)->GetSInfo()->ShowJob )
// 				m_pObjectBalloon->SetNickName(((CNpc*)this)->GetSInfo()->Name);
// 			/*
// 			switch(((CNpc*)this)->GetNpcKind())
// 			{	// 직업 안보이는 것들
// 			case 16:	//상징물
// 			case 22:	//초보자안내인
// 			case 23:	//보물상자
// 			case 24:	//황릉영혼
// 			case 26:	//상자
// 			case 27:	//퀘스트중맵이동
// 			case 29:	//맵전환보조
// 			case 30:	//로그인
// 			case 31:	//맵전환
// 			case 32:	//옥문관표국
// 			case 33:	//명사산표국
// 			case 34:	//월아천표국
// 			case 35:	//탑이사표국
// 			case 36:	//화청지표국
// 			case 37:	//정주표국
// 			case 38:	//연무장간판
// 			case 40:	//상자
// 			case 43:	//알
// 			case 44:	//알수없음
// 			break;
// 			default:
// 			m_pObjectBalloon->SetNickName(((CNpc*)this)->GetSInfo()->Name);
// 			}
// 			*/
// 		}
// 
// 	}
// 
// 	if( bitFlag & ObjectBalloon_Chat )
// 	{
// 		// chat
// 		cImage top1;
// 		cImage top2;
// 		cImage top3;
// 		cImage middle1; 
// 		cImage middle2;
// 		cImage middle3;
// 		cImage bottom1;
// 		cImage bottom2;
// 		cImage bottom3;
// 		cImage tail;
// 
// 		SCRIPTMGR->GetImage(66, &top1);
// 		SCRIPTMGR->GetImage(67, &top2);
// 		SCRIPTMGR->GetImage(68, &top3);
// 		SCRIPTMGR->GetImage(69, &middle1);
// 		SCRIPTMGR->GetImage(70, &middle2);
// 		SCRIPTMGR->GetImage(71, &middle3);
// 		SCRIPTMGR->GetImage(72, &bottom1);
// 		SCRIPTMGR->GetImage(73, &bottom2);
// 		SCRIPTMGR->GetImage(74, &bottom3);
// 		SCRIPTMGR->GetImage(75, &tail);
// 
// 		m_pObjectBalloon->GetOBalloonChat()->InitBalloon(&top1, &top3, &bottom1, &bottom3, &middle1, &middle3, &top2, &bottom2, &middle2, &tail, 8);		
// 
// 
// 		// RaMa - 04.11.29   ->채팅과 노점상은 3번의 렌더로 찍는다.
// 		cImage left1;
// 		cImage left2;
// 		cImage left3;
// 		cImage center1;
// 		cImage center2;
// 		cImage center3;
// 		cImage right1;
// 		cImage right2;
// 		cImage right3;
// 
// 		SCRIPTMGR->GetImage(99, &left1);
// 		SCRIPTMGR->GetImage(100, &center1);
// 		SCRIPTMGR->GetImage(101, &right1);
// 		SCRIPTMGR->GetImage(102, &left2);
// 		SCRIPTMGR->GetImage(103, &center2);
// 		SCRIPTMGR->GetImage(104, &right2);
// 		SCRIPTMGR->GetImage(105, &left3);
// 		SCRIPTMGR->GetImage(106, &center3);
// 		SCRIPTMGR->GetImage(107, &right3);
// 
// 		m_pObjectBalloon->GetOBalloonChat()->InitBalloonForChat(&left1, &center1, &right1,
// 			&left2, &center2, &right2, &left3, &center3, &right3, 8);
// 	}
// 
// 	if( bitFlag & ObjectBalloon_Title)
// 	{
// 		// StreetStall Title
// 		{			
// 			cImage top1;
// 			cImage top2;
// 			cImage top3;
// 			cImage middle1; 
// 			cImage middle2;
// 			cImage middle3;
// 			cImage bottom1;
// 			cImage bottom2;
// 			cImage bottom3;
// 
// 			SCRIPTMGR->GetImage(66, &top1);
// 			SCRIPTMGR->GetImage(67, &top2);
// 			SCRIPTMGR->GetImage(68, &top3);
// 			SCRIPTMGR->GetImage(69, &middle1);
// 			SCRIPTMGR->GetImage(70, &middle2);
// 			SCRIPTMGR->GetImage(71, &middle3);
// 			SCRIPTMGR->GetImage(72, &bottom1);
// 			SCRIPTMGR->GetImage(73, &bottom2);
// 			SCRIPTMGR->GetImage(74, &bottom3);
// 
// 			m_pObjectBalloon->GetSSTitleTip()->InitBalloon(&top1, &top3, &bottom1, &bottom3, &middle1, &middle3, &top2, &bottom2, &middle2, NULL, 8);
// 
// 			// RaMa - 04.11.29   ->채팅과 노점상은 3번의 렌더로 찍는다.
// 			cImage left1;
// 			cImage left2;
// 			cImage left3;
// 			cImage center1;
// 			cImage center2;
// 			cImage center3;
// 			cImage right1;
// 			cImage right2;
// 			cImage right3;
// 
// 			SCRIPTMGR->GetImage(117, &left1);
// 			SCRIPTMGR->GetImage(118, &center1);
// 			SCRIPTMGR->GetImage(119, &right1);
// 			SCRIPTMGR->GetImage(120, &left2);
// 			SCRIPTMGR->GetImage(121, &center2);
// 			SCRIPTMGR->GetImage(122, &right2);
// 			SCRIPTMGR->GetImage(123, &left3);
// 			SCRIPTMGR->GetImage(124, &center3);
// 			SCRIPTMGR->GetImage(125, &right3);		
// 
// 			m_pObjectBalloon->GetSSTitleTip()->InitBalloonForChat(&left1, &center1, &right1,
// 				&left2, &center2, &right2, &left3, &center3, &right3, 8);
// 		}
// 		{			
// 			cImage top1;
// 			cImage top2;
// 			cImage top3;
// 			cImage middle1; 
// 			cImage middle2;
// 			cImage middle3;
// 			cImage bottom1;
// 			cImage bottom2;
// 			cImage bottom3;
// 
// 			SCRIPTMGR->GetImage(66, &top1);
// 			SCRIPTMGR->GetImage(67, &top2);
// 			SCRIPTMGR->GetImage(68, &top3);
// 			SCRIPTMGR->GetImage(69, &middle1);
// 			SCRIPTMGR->GetImage(70, &middle2);
// 			SCRIPTMGR->GetImage(71, &middle3);
// 			SCRIPTMGR->GetImage(72, &bottom1);
// 			SCRIPTMGR->GetImage(73, &bottom2);
// 			SCRIPTMGR->GetImage(74, &bottom3);
// 
// 			m_pObjectBalloon->GetSBSTitleTip()->InitBalloon(&top1, &top3, &bottom1, &bottom3, &middle1, &middle3, &top2, &bottom2, &middle2, NULL, 8);
// 
// 			// RaMa - 04.11.29   ->채팅과 노점상은 3번의 렌더로 찍는다.
// 			cImage left1;
// 			cImage left2;
// 			cImage left3;
// 			cImage center1;
// 			cImage center2;
// 			cImage center3;
// 			cImage right1;
// 			cImage right2;
// 			cImage right3;
// 
// 			SCRIPTMGR->GetImage(128, &left1);
// 			SCRIPTMGR->GetImage(129, &center1);
// 			SCRIPTMGR->GetImage(130, &right1);
// 			SCRIPTMGR->GetImage(131, &left2);
// 			SCRIPTMGR->GetImage(132, &center2);
// 			SCRIPTMGR->GetImage(133, &right2);
// 			SCRIPTMGR->GetImage(134, &left3);
// 			SCRIPTMGR->GetImage(135, &center3);
// 			SCRIPTMGR->GetImage(136, &right3);		
// 
// 			m_pObjectBalloon->GetSBSTitleTip()->InitBalloonForChat(&left1, &center1, &right1,
// 				&left2, &center2, &right2, &left3, &center3, &right3, 8);
// 		}
// 	}	
// }
