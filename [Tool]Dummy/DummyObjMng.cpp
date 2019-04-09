// DummyObjMng.cpp: implementation of the CDummyObjMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DummyObjMng.h"
#include "MoveMng.h"

#include "Console.h"

extern DWORD g_MapChangeCount;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CDummyObjMng)

CDummyObjMng::CDummyObjMng()
{
	m_dwHeroID = 0;
	m_nObjectNum = 0;

	memset( &m_HeroTotalInfo, 0, sizeof(SEND_HERO_TOTALINFO) );
	m_pHeroBaseObjInfo = &m_HeroTotalInfo.BaseObjectInfo;
	m_pHeroChrTotalInfo = &m_HeroTotalInfo.ChrTotalInfo;
	m_pHeroTotalInfo = &m_HeroTotalInfo.HeroTotalInfo;
	m_pSendMoveInfo = &m_HeroTotalInfo.SendMoveInfo;	
//	m_pHeroMugongTotalInfo = &m_HeroTotalInfo.MugongTotalInfo;
//	m_pItemTotalInfo = &m_HeroTotalInfo.ItemTotalInfo;
//	m_pHeroBaseMoveInfo = &m_HeroTotalInfo.BaseMoveInfo;
	memset( &m_HeroMoveInfo, 0, sizeof(MOVE_INFO) );
	m_DummyObjState = eDUMMYOBJSTATE_Alive;

	memset( &m_MapDesc, 0, sizeof(MAPDESC) );
	m_dwCurMapNum = m_dwToMap = m_dwToMapNum = 0;

	m_dwActionStartTime = 0;
	m_dwActionEstimateTime = 10000;

	m_fMoveSpeed = RUNSPEED;

	memset( m_sChat, 0, 256 );	

	m_bPeace = TRUE;
	m_bUngi = FALSE;
	m_bRun = TRUE;
	m_bMapChange = FALSE;
	m_bLogin = FALSE;
	m_bHunt = FALSE;
	m_bRevive = FALSE;
	m_bMustUngi = FALSE;
	m_bFirstMove = FALSE;
	m_bForcedToMapChange = TRUE;

	m_nMoveFlag = 1;

	m_nSkillNum = 7;

	m_nHuntCount = 0;
	m_nMaxHuntCount = 0;

	//
	m_bEnableMapChange = FALSE;
	m_bEnableAttack = FALSE;
	m_bEnableChat = FALSE;

	m_bFollow = FALSE;


}

CDummyObjMng::~CDummyObjMng()
{
}

void CDummyObjMng::Init( int id, CDummyNetwork* pNetwork )
{
	m_nID = id;
	m_pNetwork = pNetwork;
}

void CDummyObjMng::Exit()
{
}

void CDummyObjMng::Process()
{	
	int action = SelectAction();
	
	if( action != -1 ) ExeAction( action );
}

int CDummyObjMng::GetObjectNum()
{
	int objnum = -1;
	if( m_nObjectNum )
		objnum = rand()%m_nObjectNum;

	return objnum;
}

void CDummyObjMng::SetMapNum( DWORD mapnum )
{ 
	m_dwCurMapNum = mapnum;
	m_MapDesc = RESMNG->GetMapDesc( mapnum );
	m_nMoveFlag = 1;
//	m_vOldPosition = m_MapDesc.vMovePoint[0];

	m_nMaxHuntCount = rand()%30 + 10;
	m_nHuntCount = 0;

	m_dwActionEstimateTime = 0;
	m_dwActionStartTime = 0;

	m_bFirstMove = TRUE;
}

void CDummyObjMng::SetHeroTotalInfo( SEND_HERO_TOTALINFO info )
{
	m_HeroTotalInfo = info;
//	m_HeroTotalInfo.BaseMoveInfo.CurTargetPositionIdx = 0;
//	m_HeroTotalInfo.BaseMoveInfo.MaxTargetPositionIdx = 0;
//	m_HeroTotalInfo.BaseMoveInfo.TargetPositions[0] = m_MapDesc.vMovePoint[0];
//	m_vOldPosition = m_HeroTotalInfo.BaseMoveInfo.TargetPositions[0];

	m_HeroMoveInfo.InitTargetPosition();
	VECTOR3 pos;
	pos.x = m_HeroTotalInfo.SendMoveInfo.CurPos.wx;
	pos.y = 0;
	pos.z = m_HeroTotalInfo.SendMoveInfo.CurPos.wz;

	m_HeroMoveInfo.CurPosition = pos;
	m_HeroMoveInfo.TargetPositions[0] = m_MapDesc.vMovePoint[0];
	m_vOldPosition = m_HeroMoveInfo.TargetPositions[0];

	m_nMoveFlag = 1;
	m_bFirstMove = TRUE;

	m_dwHeroID = m_HeroTotalInfo.BaseObjectInfo.dwObjectID;
}	

void CDummyObjMng::SetDummyObjStateDie( DWORD id )
{
	if( m_HeroTotalInfo.BaseObjectInfo.dwObjectID == id )
	{
		m_DummyObjState = eDUMMYOBJSTATE_Die;
		ActionChatting( "헉! 죽었다!!" );
	}
}

int CDummyObjMng::SelectAction()
{
	int action = -1;
	if( m_DummyObjState == eDUMMYOBJSTATE_Alive )
	{
		action = rand()%MAX_ACTION_NUM+1;
	}
	else if( m_DummyObjState == eDUMMYOBJSTATE_Die )
	{		
		m_bRevive = TRUE;
		m_bHunt = FALSE;
		m_bUngi = FALSE;
		m_bLogin = FALSE;
		m_dwActionEstimateTime = 10000;
//		ExeAction( eACTION_Revive );
		action = eACTION_Revive;
	}

	return action;
}

void CDummyObjMng::ExeAction( int action )
{
	DWORD curtime = GetTickCount();
	if( (curtime-m_dwActionStartTime) >= m_dwActionEstimateTime )
	{
		if( m_DummyObjState == eDUMMYOBJSTATE_Die )
		{
			ActionRevive();
			return;
		}
		
		if( m_bUngi ) action = eACTION_Ungi;
		if( m_bMapChange && m_bEnableMapChange )
		{
			ActionMapChange();
			return;
		}
		if( m_bHunt && m_bEnableAttack )
		{
			ActionSkill();
			return;
		}
		if( m_bLogin )
		{
			ActionSaveLogin();
			return;
		}

		if( MOVEMNG->GetMonsterNum() > 0 )
		{
			m_bFirstMove = FALSE;
		}

		if( m_bRevive )
			action = eACTION_Revive;

		if( m_bMustUngi )
			action = eACTION_Ungi;

		if( m_bFirstMove )
			action = 100;

//		if( m_bFollow )
//			action = eACTION_Hunt;
		
		switch( action )
		{
		case eACTION_Peace:
			ActionPeaceWarMode();
			break;

		case eACTION_Run:
			ActionRunMode();
			break;

		case eACTION_Ungi:
			ActionUngiMode();
			break;

		case eACTION_MapChange:
			if( m_bEnableMapChange )
				ActionMoveToMapchangePoint();
			break;

		case eACTION_SaveLogin:
			ActionMoveToLoginPoint();			
			break;

		case eACTION_Revive:
//			ActionRevive();
			break;

		case eACTION_Hunt:
			if( MOVEMNG->GetMonsterNum() > 0 )
			{
				if( m_bEnableAttack )
					ActionMoveToMonster();
			}
			break;		

		default:
			ActionMove();
			break;
		}

//		g_Console.Log( eLogDisplay, 4, "Dummy%d Action : %d - Current MapNum : %d", m_nID, action, m_dwCurMapNum );
	}
}

void CDummyObjMng::ActionPeaceWarMode()
{
	m_dwActionStartTime = GetTickCount();

	MSGBASE msg;
	msg.Category = MP_PEACEWARMODE;
	msg.dwObjectID = m_dwHeroID;
	if( m_bPeace )
	{
		msg.Protocol = MP_PEACEWARMODE_WAR;
		m_bPeace = FALSE;
		ActionChatting( "Fight!!" );
	}
	else
	{
		msg.Protocol = MP_PEACEWARMODE_PEACE;
		m_bPeace = TRUE;
		ActionChatting( "Peace!!" );
	}
	m_pNetwork->Send( &msg, sizeof(msg) );

	m_dwActionEstimateTime = 2000;
}

void CDummyObjMng::ActionRunMode()
{
	m_dwActionStartTime = GetTickCount();

	MSGBASE msg;
	msg.Category = MP_MOVE;
	msg.dwObjectID = m_dwHeroID;
	if( m_bRun )
	{
		msg.Protocol = MP_MOVE_WALKMODE;
		m_fMoveSpeed = WALKSPEED;
		m_bRun = FALSE;
		ActionChatting( "걷자!!" );		
	}
	else
	{
		msg.Protocol = MP_MOVE_RUNMODE;
		m_fMoveSpeed = RUNSPEED;
		m_bRun = TRUE;
		ActionChatting( "뛰자!!" );		
	}
	m_pNetwork->Send( &msg, sizeof(msg) );	

	m_dwActionEstimateTime = 2000;
}

void CDummyObjMng::ActionUngiMode()
{
	m_dwActionStartTime = GetTickCount();

	MSGBASE msg;
	msg.Category = MP_UNGIJOSIK;
	msg.dwObjectID = m_dwHeroID;
	if( m_bUngi )
	{
		msg.Protocol = MP_UNGIJOSIK_END;
		m_bUngi = FALSE;
		m_dwActionEstimateTime = 2000;
		ActionChatting( "이제 일어나야지!!" );
		m_bMustUngi = FALSE;		

		m_bFirstMove = TRUE;
		m_nMoveFlag = -1;
	}
	else
	{
		msg.Protocol = MP_UNGIJOSIK_START;		
		m_bUngi = TRUE;
		m_dwActionEstimateTime = 50000;
		ActionChatting( "운기조식해야돼!!" );
		
	}
	m_pNetwork->Send( &msg, sizeof(msg) );	
}

void CDummyObjMng::ActionMapChange()
{
	m_dwActionStartTime = GetTickCount();

	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANGEMAP_SYN;
	msg.dwObjectID = m_dwHeroID;
	msg.dwData = m_dwToMap;
	m_pNetwork->Send( &msg, sizeof(msg) );

	m_bMapChange = FALSE;

	m_dwActionEstimateTime = 2000;	
}

void CDummyObjMng::ActionSaveLogin()
{
	m_dwActionStartTime = GetTickCount();

	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_SAVEPOINT_SYN;
	msg.dwObjectID = m_dwHeroID;
	msg.dwData = m_MapDesc.nMapNum+2000;
				
	m_pNetwork->Send( &msg, sizeof(msg) );

	m_bLogin = FALSE;	

	m_dwActionEstimateTime = 3000;

	ActionChatting( "LoginPoint 저장!!" );
}

void CDummyObjMng::ActionChatting( char* str )
{
	if( m_bEnableChat )
	{
		sprintf( m_sChat, str );
		
		TESTMSG data;
		data.Category = MP_CHAT;
		data.Protocol = MP_CHAT_ALL;
		data.dwObjectID	= m_dwHeroID;
		strcpy( data.Msg, m_sChat );
		
		m_pNetwork->Send( &data, sizeof(data) );
		
		memset( m_sChat, 0, 256 );
	}
}

void CDummyObjMng::CalMapChangePosition()
{
	int i = rand()%m_MapDesc.nMaxMapChange;
	m_dwToMap = m_MapDesc.MapChangePoint[i].nChangeNum;
	m_dwToMapNum = m_MapDesc.MapChangePoint[i].nToMapNum;
//	m_pHeroBaseMoveInfo->TargetPositions[0] = m_MapDesc.MapChangePoint[i].vMapChange;
	m_HeroMoveInfo.TargetPositions[0] = m_MapDesc.MapChangePoint[i].vMapChange;
	
	CalEstimateTime();
}

void CDummyObjMng::CalLoginPosition()
{
//	m_pHeroBaseMoveInfo->TargetPositions[0] = m_MapDesc.vLoginPoint;
	m_HeroMoveInfo.TargetPositions[0] = m_MapDesc.vLoginPoint;

	CalEstimateTime();
}

void CDummyObjMng::CalMonsterPosition()
{
/*	m_MonsterInfo = MOVEMNG->FindNearMonster( m_pHeroBaseMoveInfo->CurPosition );
	m_pHeroBaseMoveInfo->TargetPositions[0] = m_MonsterInfo.BaseMoveInfo.CurPosition;

	CalEstimateTime();
	*/
}

void CDummyObjMng::CalMovePosition()
{
	if( m_nMoveFlag == 1 )
	{
//		m_pHeroBaseMoveInfo->TargetPositions[0] = m_vOldPosition;//m_MapDesc.vMovePoint[0];
//		m_pHeroBaseMoveInfo->TargetPositions[0].x += m_nMoveFlag*(rand()%100);
//		m_pHeroBaseMoveInfo->TargetPositions[0].z += m_nMoveFlag*(rand()%100);
		m_HeroMoveInfo.TargetPositions[0] = m_vOldPosition;//m_MapDesc.vMovePoint[0];
		m_HeroMoveInfo.TargetPositions[0].x += m_nMoveFlag*(rand()%100);
		m_HeroMoveInfo.TargetPositions[0].z += m_nMoveFlag*(rand()%100);
	}
	else
	{
		int i = rand()%m_MapDesc.nMaxMovePoint;
//		m_pHeroBaseMoveInfo->TargetPositions[0] = m_MapDesc.vMovePoint[i];
//		m_pHeroBaseMoveInfo->TargetPositions[0].x += m_nMoveFlag*(rand()%100);
//		m_pHeroBaseMoveInfo->TargetPositions[0].z += m_nMoveFlag*(rand()%100);
		m_HeroMoveInfo.TargetPositions[0] = m_MapDesc.vMovePoint[i];
		m_HeroMoveInfo.TargetPositions[0].x += m_nMoveFlag*(rand()%100);
		m_HeroMoveInfo.TargetPositions[0].z += m_nMoveFlag*(rand()%100);
	}

	m_nMoveFlag *= -1;

	CalEstimateTime();

	
}

void CDummyObjMng::CalEstimateTime()
{
//	float Distance = CalcDistanceXZ( &m_pHeroBaseMoveInfo->CurPosition, &m_pHeroBaseMoveInfo->TargetPositions[0] );
	float Distance = CalcDistanceXZ( &m_HeroMoveInfo.CurPosition, &m_HeroMoveInfo.TargetPositions[0] );
	m_dwActionEstimateTime = (DWORD)(Distance/m_fMoveSpeed*1000);	
}

void CDummyObjMng::ActionMoveToMapchangePoint()
{
	m_dwActionStartTime = GetTickCount();
	
	CalMapChangePosition();

	MOVE_TARGETPOS msg;
	msg.Init();
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_TARGET;
	msg.dwObjectID = m_dwHeroID;
	msg.dwMoverID = msg.dwObjectID;	
	msg.SetStartPos( &m_HeroMoveInfo.CurPosition );
	msg.AddTargetPos( &m_HeroMoveInfo.TargetPositions[0] );
	m_pNetwork->Send( &msg, msg.GetSize() );

	m_HeroMoveInfo.CurPosition = m_HeroMoveInfo.TargetPositions[0];

	/*
	MOVE_TARGETPOS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_TARGET;
	msg.dwObjectID = m_dwHeroID;
	msg.dwMoverID = msg.dwObjectID;
	msg.Init();
	msg.SetStartPos( &m_pHeroBaseMoveInfo->CurPosition );
	msg.AddTargetPos( &m_pHeroBaseMoveInfo->TargetPositions[0] );
	m_pNetwork->Send( &msg, msg.GetSize() );

	m_pHeroBaseMoveInfo->CurPosition = m_pHeroBaseMoveInfo->TargetPositions[0];
*/

	char* temp;
	temp = GetMapName( m_dwToMapNum );
	sprintf( m_sChat, "%s(으)로 가볼까!!", temp );
	ActionChatting( m_sChat );

	m_bMapChange = TRUE;

	m_bFirstMove = FALSE;
}

void CDummyObjMng::ActionMoveToLoginPoint()
{
	m_dwActionStartTime = GetTickCount();

	CalLoginPosition();

	MOVE_TARGETPOS msg;
	msg.Init();
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_TARGET;
	msg.dwObjectID = m_dwHeroID;
	msg.dwMoverID = msg.dwObjectID;	
	msg.SetStartPos( &m_HeroMoveInfo.CurPosition );
	msg.AddTargetPos( &m_HeroMoveInfo.TargetPositions[0] );
	m_pNetwork->Send( &msg, msg.GetSize() );

	m_HeroMoveInfo.CurPosition = m_HeroMoveInfo.TargetPositions[0];

/*		MOVE_TARGETPOS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_TARGET;
	msg.dwObjectID = m_dwHeroID;
	msg.dwMoverID = msg.dwObjectID;
	msg.Init();
	msg.SetStartPos( &m_pHeroBaseMoveInfo->CurPosition );
	msg.AddTargetPos( &m_pHeroBaseMoveInfo->TargetPositions[0] );
	m_pNetwork->Send( &msg, msg.GetSize() );

	m_pHeroBaseMoveInfo->CurPosition = m_pHeroBaseMoveInfo->TargetPositions[0];
*/

	m_bLogin = TRUE;

	ActionChatting( "LoginPoint 저장하러 가야지!!" );

	m_bFirstMove = FALSE;
}

void CDummyObjMng::ActionMoveToMonster()
{
	if( MOVEMNG->GetMonsterNum() <= 0 ) return;
	
	++m_nHuntCount;

	m_dwActionStartTime = GetTickCount();

	CalMonsterPosition();

	MOVE_TARGETPOS msg;
	msg.Init();
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_TARGET;
	msg.dwObjectID = m_dwHeroID;
	msg.dwMoverID = msg.dwObjectID;	
	msg.SetStartPos( &m_HeroMoveInfo.CurPosition );
	msg.AddTargetPos( &m_HeroMoveInfo.TargetPositions[0] );
	m_pNetwork->Send( &msg, msg.GetSize() );

	m_HeroMoveInfo.CurPosition = m_HeroMoveInfo.TargetPositions[0];

/*		MOVE_TARGETPOS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_TARGET;
	msg.dwObjectID = m_dwHeroID;
	msg.dwMoverID = msg.dwObjectID;
	msg.Init();
	msg.SetStartPos( &m_pHeroBaseMoveInfo->CurPosition );
	msg.AddTargetPos( &m_pHeroBaseMoveInfo->TargetPositions[0] );
	m_pNetwork->Send( &msg, msg.GetSize() );

	m_pHeroBaseMoveInfo->CurPosition = m_pHeroBaseMoveInfo->TargetPositions[0];

	*/
	ActionChatting( "Monster 잡으러 가야지!!" );

	m_bHunt = TRUE;

	m_bFirstMove = FALSE;
}

void CDummyObjMng::ActionMove()
{
	m_dwActionStartTime = GetTickCount();
	
	CalMovePosition();
	
	MOVE_TARGETPOS msg;
	msg.Init();
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_TARGET;
	msg.dwObjectID = m_dwHeroID;
	msg.dwMoverID = msg.dwObjectID;	
	msg.SetStartPos( &m_HeroMoveInfo.CurPosition );
	msg.AddTargetPos( &m_HeroMoveInfo.TargetPositions[0] );
	m_pNetwork->Send( &msg, msg.GetSize() );

	m_HeroMoveInfo.CurPosition = m_HeroMoveInfo.TargetPositions[0];

/*		MOVE_TARGETPOS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_TARGET;
	msg.dwObjectID = m_dwHeroID;
	msg.dwMoverID = msg.dwObjectID;
	msg.Init();
	msg.SetStartPos( &m_pHeroBaseMoveInfo->CurPosition );
	msg.AddTargetPos( &m_pHeroBaseMoveInfo->TargetPositions[0] );
	m_pNetwork->Send( &msg, msg.GetSize() );

	m_pHeroBaseMoveInfo->CurPosition = m_pHeroBaseMoveInfo->TargetPositions[0];
*/

	if( m_bRun )	ActionChatting( "나 달린다!!" );
	else			ActionChatting( "나 걷는다!!" );

	m_bFirstMove = FALSE;
}

void CDummyObjMng::ActionRevive()
{
	m_dwActionStartTime = GetTickCount();

	int i = rand()%2;

	MSGBASE msg;
	msg.Category = MP_CHAR_REVIVE;
	msg.dwObjectID = m_dwHeroID;
	if( i == 0 )
		msg.Protocol = MP_CHAR_REVIVE_PRESENTSPOT_SYN;
	else
		msg.Protocol = MP_CHAR_REVIVE_LOGINSPOT_SYN;
	m_pNetwork->Send( &msg, sizeof(msg) );

	m_dwActionEstimateTime = 15000;

	m_DummyObjState = eDUMMYOBJSTATE_Alive;

	ActionChatting( "다시 살아나야돼!!" );

	m_bRevive = FALSE;

	m_bMustUngi = TRUE;

	m_nSkillNum = 7;
}

void CDummyObjMng::ActionSkill()
{
/*	m_dwActionStartTime = GetTickCount();

	if( MOVEMNG->IsMonster( m_MonsterInfo.BaseObjectInfo.dwObjectID, m_pHeroBaseMoveInfo->CurPosition ) == FALSE )
	{
		m_bHunt = FALSE;
		m_nSkillNum = 7;
		ActionFollowMonster();
		return;
	}

	MSG_SKILL_START_SYN msg;
	MAINTARGET MainTarget;
	memset( &MainTarget, 0, sizeof(MAINTARGET) );
	MainTarget.SetMainTarget( m_MonsterInfo.BaseObjectInfo.dwObjectID );
	msg.InitMsg( m_nSkillNum, &MainTarget, 10.0f, m_dwHeroID );
	
	m_pNetwork->Send( &msg, msg.GetMsgLength() );

	m_dwActionEstimateTime = 500;
	
//	m_bHunt = FALSE;

//	if( m_nSkillNum == 7 )
	{
//		sprintf( m_sChat, "콤보 %d 공격!!", m_nSkillNum );
//		ActionChatting( m_sChat );
		ActionChatting( "공격!!" );
	}

	++m_nSkillNum;
	if( m_nSkillNum > 12 )
		m_nSkillNum = 7;

	m_bFollow = FALSE;

	m_bHunt = TRUE;
	*/
}

void CDummyObjMng::SetLevelUp( int level )
{
	m_pHeroChrTotalInfo->Level = level;

	sprintf( m_sChat, "이제 Level %d이다!!", level );
	ActionChatting( m_sChat );
}

void CDummyObjMng::SetExp( int exp, int flag )
{
	if( flag == 0 )
	{
		sprintf( m_sChat, "경험치 %d(을)를 잃었다.!!", exp );
	}
	else if( flag == 1 )
	{
		sprintf( m_sChat, "경험치 %d(을)를 얻었다.!!", exp );
	}
	
	ActionChatting( m_sChat );
}

void CDummyObjMng::SetMoney( int money, int flag )
{
	if( flag == 0 )
	{
		sprintf( m_sChat, "돈 %d(을)를 얻었다.!!", money );
	}
	else if( flag == 1 )
	{
		sprintf( m_sChat, "돈 %d(을)를 사용했다.!!", money );
	}
	else if( flag == 2 )
	{
		sprintf( m_sChat, "돈 %d(을)를 잃었다.!!", money );
	}
	
	ActionChatting( m_sChat );
}

void CDummyObjMng::SetItem()
{
	sprintf( m_sChat, "아이템을 얻었다.!!" );
	
	ActionChatting( m_sChat );
}

void CDummyObjMng::ForcedToMapChange( int mapnum )
{
	if( m_dwCurMapNum == mapnum )
		return;

	m_dwActionStartTime = GetTickCount();

	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANGEMAP_SYN;
	msg.dwObjectID = m_dwHeroID;

	switch( mapnum )
	{
	case 12: msg.dwData = 1005; break;	// 장안
	case 17: msg.dwData = 1003; break;	// 난주
	case 18: msg.dwData = 1002; break;	// 돈황
	case 19: msg.dwData = 1001; break;	// 옥문관
	case 13: msg.dwData = 1007; break;	// 화청지
	case 6: msg.dwData = 1011; break;	// 낙양
	case 8: msg.dwData = 1015; break;	// 정주
	case 7: msg.dwData = 1023; break;	// 개봉
//	case 2: msg.dwData = 1028; break;	// 태산
	case 1: msg.dwData = 1024; break;	// 북경
	}
	
	m_pNetwork->Send( &msg, sizeof(msg) );

	m_bMapChange = FALSE;

	m_dwActionEstimateTime = 5000;
	
	g_MapChangeCount++;
}

void CDummyObjMng::ActionFollowMonster()
{
/*	if( MOVEMNG->GetMonsterNum() <= 0 ) return;
	
	m_dwActionStartTime = GetTickCount();

	m_MonsterInfo.BaseMoveInfo.CurPosition = MOVEMNG->GetMonsterPosition( m_MonsterInfo.BaseObjectInfo.dwObjectID );
	if( m_MonsterInfo.BaseMoveInfo.CurPosition.x == 0 )
		return;
	
	m_pHeroBaseMoveInfo->TargetPositions[0] = m_MonsterInfo.BaseMoveInfo.CurPosition;

	CalEstimateTime();

	MOVE_TARGETPOS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_TARGET;
	msg.dwObjectID = m_dwHeroID;
	msg.dwMoverID = msg.dwObjectID;
	msg.Init();
	msg.SetStartPos( &m_pHeroBaseMoveInfo->CurPosition );
	msg.AddTargetPos( &m_pHeroBaseMoveInfo->TargetPositions[0] );
	m_pNetwork->Send( &msg, msg.GetSize() );

/*
	MOVE_TARGETPOS msg;
	msg.Init();
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_TARGET;
	msg.dwObjectID = HEROID;
	msg.dwMoverID = HEROID;
	msg.SetStartPos(&pHero->m_MoveInfo.CurPosition);
	//msg.tpos.Compress(&TargetPos);
	for(int i=0;i<count;++i)
		msg.AddTargetPos(&pHero->m_MoveInfo.TargetPositions[i]);

	NETWORK->Send(&msg,msg.GetSize());

	//StartMove(pHero,gCurTime,&TargetPos);
	StartMoveEx(pHero,gCurTime);
	*/
/*
	m_pHeroBaseMoveInfo->CurPosition = m_pHeroBaseMoveInfo->TargetPositions[0];

	ActionChatting( "어디 갔어!!" );

	m_bHunt = TRUE;

	m_bFirstMove = FALSE;

	m_bFollow = TRUE;
	*/
}