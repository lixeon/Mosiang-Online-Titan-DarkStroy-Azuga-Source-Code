// SkillDalayManager.cpp: implementation of the CSkillDalayManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillDalayManager.h"

#include "MHFile.h"
#include "Network.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CSkillDelayManager);


CSkillDelayManager::CSkillDelayManager()
{
	m_htSkillUse.Initialize( 500 );

	m_mpSkillUse = new CMemoryPoolTempl<SKILLUSE>;
	m_mpSkillUse->Init( 500, 10, "SKILLUSE" );

	m_htPrimereSkill.Initialize( 10 );
}

CSkillDelayManager::~CSkillDelayManager()
{
	m_htSkillUse.SetPositionHead();

	SKILLUSE* pSkillUse = NULL;
	while( pSkillUse = m_htSkillUse.GetData() )
	{
		m_mpSkillUse->Free( pSkillUse );
	}

	m_htSkillUse.RemoveAll();

	m_htPrimereSkill.SetPositionHead();
	PRIMERESKILL* pPrimereSkill = NULL;

	while( pPrimereSkill = m_htPrimereSkill.GetData() )
	{
		delete pPrimereSkill;
	}

	m_htPrimereSkill.RemoveAll();

	m_mpSkillUse->Release();
	SAFE_DELETE( m_mpSkillUse );
}

void CSkillDelayManager::LoadSkillUseInfo()
{
	CMHFile file;	

	if( !file.Init( ".\\Resource\\PremierSkill.bin", "rb" ) )
	{
		return;
	}

	while(1)
	{
		if(file.IsEOF())
			break;

		DWORD dwGroupNum	= file.GetDword();
		DWORD dwSkillIndex	= file.GetDword();
		DWORD dwDelay		= file.GetDword();
		
		PRIMERESKILL* pPrimereSkill = new PRIMERESKILL;
		pPrimereSkill->dwSkillIndex = dwSkillIndex;
		pPrimereSkill->dwDelay		= dwDelay;
		m_htPrimereSkill.Add( pPrimereSkill, dwSkillIndex );
	}

	file.Release();
}

BOOL CSkillDelayManager::IsPrimereSkill( DWORD dwSkillIndex )
{
	if( m_htPrimereSkill.GetData( dwSkillIndex ) == NULL ) return FALSE;

	return TRUE;
}

//스킬을 사용할 수 있으면 return TRUE
BOOL CSkillDelayManager::AddSkillUse( DWORD dwCharacterID, DWORD dwSkillIndex, BOOL bForce )
{
	//상승무공이 아니면
	PRIMERESKILL* pPrimereSkill = m_htPrimereSkill.GetData( dwSkillIndex );
	if( pPrimereSkill == NULL )
		return TRUE;

	DWORD dwDelay = pPrimereSkill->dwDelay;

	SKILLUSE* pOld = m_htSkillUse.GetData( dwCharacterID );

	//스킬을 사용한 적이 있다면
	if( pOld )
	{
		if( bForce )	//메세지 받은 것이라 강제로 세팅
		{
			//정보 갱신
			ASSERT( dwCharacterID == pOld->dwCharacterID );
			pOld->dwDelay		= dwDelay;
			pOld->dwSkillIndex	= dwSkillIndex;
			pOld->dwStartTime	= gCurTime;

			return TRUE;
		}
		else
		{
			//dwDelay시간이 지났다면
			if( gCurTime - pOld->dwStartTime + 5000 >= pOld->dwDelay )	//렉을 고려한다.(5초)
			{
				//정보 갱신
				ASSERT( dwCharacterID == pOld->dwCharacterID );
				pOld->dwDelay		= dwDelay;
				pOld->dwSkillIndex	= dwSkillIndex;
				pOld->dwStartTime	= gCurTime;
				
				//다른 에이젼트로 정보 갱신 내용을 보내주어야 한다.
				SendMsgToAgentServer( dwCharacterID, dwSkillIndex );
				
				return TRUE;
			}
			else	//아직 delay가 남았다면
			{
				return FALSE;
			}
		}
	}
	else	//스킬을 사용한 적이 없다면
	{
		SKILLUSE* pSkillUse = m_mpSkillUse->Alloc();
		
		pSkillUse->dwCharacterID	= dwCharacterID;
		pSkillUse->dwSkillIndex		= dwSkillIndex;
		pSkillUse->dwDelay			= dwDelay;
		pSkillUse->dwStartTime		= gCurTime;
		
		m_htSkillUse.Add( pSkillUse, dwCharacterID );

		//다른에이젼트로 정보를 보내야 한다.
		if( !bForce )	//다른에이젼트로 부터 받은 메세지 일경우는 안보낸다.
			SendMsgToAgentServer( dwCharacterID, dwSkillIndex );

		return TRUE;
	}
}
/*
void CSkillDelayManager::RemoveSkillUse( DWORD dwCharacterID )
{
	SKILLUSE* pOld = m_htSkillUse.GetData( dwCharacterID );

	if( pOld )
	{
		m_htSkillUse.Remove( dwCharacterID );
		m_mpSkillUse->Free( pOld );
	}
}
*/

//send other agent!
void CSkillDelayManager::SendMsgToAgentServer( DWORD dwCharacterID, DWORD dwSkillIndex )
{
	MSG_DWORD2 msg;
	msg.Protocol	= MP_SKILL;
	msg.Category	= MP_SKILL_START_SYN;
	msg.dwObjectID	= dwCharacterID;
	msg.dwData1		= dwCharacterID;
	msg.dwData2		= dwSkillIndex;

	g_Network.Broadcast2AgentServerExceptSelf( (char*)&msg, sizeof(msg) );
}


void CSkillDelayManager::SendSkillDelayMsgToClient( DWORD dwCharacterID )
{

	SKILLUSE* pOld = m_htSkillUse.GetData( dwCharacterID );
	
	if( pOld )
	{
		//dwDelay시간이 남아 있다면
		if( gCurTime - pOld->dwStartTime + 5000 < pOld->dwDelay )	//렉을 고려한다.(5초)
		{
			ASSERT( dwCharacterID == pOld->dwCharacterID );
			//클라이언트로 딜레이를 보내준다.			
			MSG_DWORD2 msg;
			msg.Category	= MP_SKILL;
			msg.Protocol	= MP_SKILL_DELAY_NOTIFY;
			msg.dwObjectID	= dwCharacterID;
			msg.dwData1		= pOld->dwDelay;	//원래 딜레이시간			
			msg.dwData2		= pOld->dwDelay - ( gCurTime - pOld->dwStartTime );	//남은시간

			g_Network.Send2User( (MSGBASE*)&msg, sizeof(msg) );
		}
	}
}
