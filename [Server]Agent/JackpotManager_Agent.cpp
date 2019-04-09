// JackpotManager_Agent.cpp: implementation of the JackpotManager_Agent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JackpotManager_Agent.h"
#include "ServerSystem.h"
#include "Network.h"
#include "MHTimeManager.h"
#include "AgentDBMsgParser.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(JackpotManager_Agent)

JackpotManager_Agent::JackpotManager_Agent()
{
	Init();
}

JackpotManager_Agent::~JackpotManager_Agent()
{

}

void	JackpotManager_Agent::CheckManager()	//잭팟관리하는 Agent인지
{
	//g_pServerSystem->GetServerNum()
	if( g_pServerSystem->GetServerNum() == 0 )
		m_bManager = TRUE;
	else m_bManager = FALSE;

}
void	JackpotManager_Agent::GetTotalMoneyFrDB()	//DB에 쿼리...생성될때..그리고 정기적으로
{
	if( gCurTime - m_dwLastDBUpdateTime < m_dwUpdateTimeLength ) return;
	
	//DB.Query
	JackpotLoadTotalMoney();

	m_dwLastDBUpdateTime = gCurTime;
}

void	JackpotManager_Agent::SendMsgUserTotalMoney()	//총금액 정보를 유저에게 보낸다. //!맵 접속 유저인지 확인
{
	MSG_DWORD msg;
	//SetProtocol((MSGBASE*)&msg, MP_JACKPOT, MP_JACKPOT_TOTALMONEY_NOTIFY_TO_AGENT);	//다른 AGENT로 보내고
	msg.Category = MP_JACKPOT;
	msg.Protocol = MP_JACKPOT_TOTALMONEY_NOTIFY_TO_AGENT;
	msg.dwData = m_dwTotalMoney;

	g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));

	//SetProtocol((MSGBASE*)&msg, MP_JACKPOT, MP_JACKPOT_TOTALMONEY_NOTIFY);	//this AGENT의 USERS 에게 보낸다.
	msg.Category = MP_JACKPOT;
	msg.Protocol = MP_JACKPOT_TOTALMONEY_NOTIFY;

	USERINFO* info = NULL;	
	g_pUserTable->SetPositionUserHead();
	while( info = g_pUserTable->GetUserData() )	//!g_pUserTable 에는 다 CPlayer object 들인가?
	{
		if( info->wUserMapNum )
		{
			MSG_DWORD msgTemp = msg;
			g_Network.Send2User( info->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
		}
	}

	//MP_JACKPOT_TOTALMONEY_NOTIFY_TO_AGENT
}

void	JackpotManager_Agent::SendMsgAddUserTotalMoney( DWORD CharacterID )
{
	MSG_DWORD msg;

	msg.Category = MP_JACKPOT;
	msg.Protocol = MP_JACKPOT_TOTALMONEY_NOTIFY;
	msg.dwObjectID = CharacterID;
	msg.dwData = m_dwTotalMoney;
	
	g_Network.Send2User((MSGBASE*)&msg, sizeof(msg) );	
}

void	JackpotManager_Agent::Start()
{
	CheckManager();
}

void	JackpotManager_Agent::Process()
{
	if(m_bManager)
	GetTotalMoneyFrDB();
}

void	JackpotManager_Agent::Init()
{
	m_dwTotalMoney			= 0;
	m_dwUpdateTimeLength	= DB_UPDATE_TIMELENGTH;
	m_dwLastDBUpdateTime	= 0;
	m_bManager				= FALSE;

	//!DB.Query 총금액 받아오기
	JackpotLoadTotalMoney();

//	//관리자 서버인지 확인하기
//	CheckManager();
}

void	JackpotManager_Agent::Release()
{

}
