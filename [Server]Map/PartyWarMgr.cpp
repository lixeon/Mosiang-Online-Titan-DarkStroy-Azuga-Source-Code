// PartyWarMgr.cpp: implementation of the CPartyWarMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyWarMgr.h"
#include "UserTable.h"
#include "Player.h"
#include "Party.h"
#include "PartyManager.h"
#include "PackedData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPartyWarTeam::CPartyWarTeam()
{
	m_dwPartyIdx = 0;
	memset( m_Member, 0, sizeof(sPWMember)*MAX_PARTY_LISTNUM );
	m_nAliveNum = 0;

	m_bLock = m_bReady = FALSE;
	m_vWarPos.x = m_vWarPos.y = m_vWarPos.z = 0.0f;
}

CPartyWarTeam::~CPartyWarTeam()
{
	m_dwPartyIdx = 0;
	memset( m_Member, 0, sizeof(sPWMember)*MAX_PARTY_LISTNUM );
	m_nAliveNum = 0;

	m_bLock = m_bReady = FALSE;
	m_vWarPos.x = m_vWarPos.y = m_vWarPos.z = 0.0f;
}

BOOL CPartyWarTeam::IsAddableMember( DWORD dwMemberIdx, int nIndex )
{	
	if( nIndex >= MAX_PARTY_LISTNUM )	return FALSE;
	if( m_Member[nIndex].dwMemberIdx != dwMemberIdx )	return FALSE;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( dwMemberIdx );
	if( !pPlayer )	return FALSE;
	if( pPlayer->IsVimuing() || pPlayer->IsPKMode() )	return FALSE;
	if( pPlayer->GetState() != eObjectState_None && pPlayer->GetState() != eObjectState_Move )	return FALSE;

	return TRUE;
}

BOOL CPartyWarTeam::IsWarMember( DWORD dwMemberIdx )
{	
	for( int i = 0; i < MAX_PARTY_LISTNUM; ++i )
	{
		if( m_Member[i].bEnableWar && m_Member[i].dwMemberIdx == dwMemberIdx )
			return TRUE;
	}
	return FALSE;
}

void CPartyWarTeam::InitMember( DWORD dwMemberIdx, int nIndex )
{
	if( nIndex >= MAX_PARTY_LISTNUM )	return;

	m_Member[nIndex].dwMemberIdx = dwMemberIdx;
	m_Member[nIndex].bEnableWar = FALSE;

	if( nIndex == 0 )
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( m_Member[nIndex].dwMemberIdx );
		if( pPlayer )
			pPlayer->GetPosition( &m_vWarPos );
	}
}

void CPartyWarTeam::AddMember( DWORD dwMemberIdx, int nIndex )
{
	if( nIndex >= MAX_PARTY_LISTNUM )	return;
	if( m_Member[nIndex].dwMemberIdx != dwMemberIdx )	return;

	m_Member[nIndex].bEnableWar = TRUE;
		
	++m_nAliveNum;
	if( m_nAliveNum > MAX_PARTY_LISTNUM )	m_nAliveNum = 7;
}

void CPartyWarTeam::RemoveMember( DWORD dwMemberIdx, int nIndex )
{
	if( nIndex >= MAX_PARTY_LISTNUM )	return;
	if( m_Member[nIndex].dwMemberIdx != dwMemberIdx )	return;

	m_Member[nIndex].bEnableWar = FALSE;

	--m_nAliveNum;
	if( m_nAliveNum < 0 )	m_nAliveNum = 0;
}

BOOL CPartyWarTeam::MemberDie( DWORD dwMemberIdx )
{
	for( int i = 0; i < MAX_PARTY_LISTNUM; ++i )
	{
		if( m_Member[i].bEnableWar && m_Member[i].dwMemberIdx == dwMemberIdx )
		{
			--m_nAliveNum;
			if( m_nAliveNum < 0 )	m_nAliveNum = 0;
			return TRUE;
		}
	}
	return FALSE;
}

void CPartyWarTeam::SendMsgToAllMember( MSGBASE* pMsg, int nLength )
{
	for( int i = 0; i < MAX_PARTY_LISTNUM; ++i )
	{
		if( m_Member[i].dwMemberIdx != 0 )
		{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( m_Member[i].dwMemberIdx );
			if( pPlayer )
				pPlayer->SendMsg( pMsg, nLength );
		}
	}
}

void CPartyWarTeam::SendMsgToAllMemberInWar( MSGBASE* pMsg, int nLength )
{
	for( int i = 0; i < MAX_PARTY_LISTNUM; ++i )
	{
		if( m_Member[i].bEnableWar && m_Member[i].dwMemberIdx )
		{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( m_Member[i].dwMemberIdx );
			if( pPlayer )
				pPlayer->SendMsg( pMsg, nLength );
		}
	}
}

void CPartyWarTeam::SendMsgToMaster( MSGBASE* pMsg, int nLength )
{
	CParty* pParty = PARTYMGR->GetParty( m_dwPartyIdx );
	if( !pParty )	return;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pParty->GetMasterID() );
	if( !pPlayer )	return;

	pPlayer->SendMsg( pMsg, nLength );
}
	
void CPartyWarTeam::SendMsgRevive()
{
	for( int i = 0; i < MAX_PARTY_LISTNUM; ++i )
	{
		if( m_Member[i].bEnableWar && m_Member[i].dwMemberIdx )
		{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( m_Member[i].dwMemberIdx );
			if( pPlayer )
			if( pPlayer->GetState() == eObjectState_Die )
			if( FALSE == pPlayer->IsPenaltyByDie() )
				pPlayer->ReviveAfterVimu();
		}
	}
}

void CPartyWarTeam::SendMsgToGridInWar( MSGBASE* pMsg, int nLength )
{
	for( int i = 0; i < MAX_PARTY_LISTNUM; ++i )
	{
		if( m_Member[i].bEnableWar && m_Member[i].dwMemberIdx )
		{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( m_Member[i].dwMemberIdx );
			if( pPlayer )
			{
				pMsg->dwObjectID = m_Member[i].dwMemberIdx;
				PACKEDDATA_OBJ->QuickSend( pPlayer, pMsg, nLength );
			}
		}
	}
}

void CPartyWarTeam::SendMsgToGridInOne( MSGBASE* pMsg, int nLength )
{
	for( int i = 0; i < MAX_PARTY_LISTNUM; ++i )
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( m_Member[i].dwMemberIdx );
		if( pPlayer )
		{
			pMsg->dwObjectID = m_Member[i].dwMemberIdx;
			PACKEDDATA_OBJ->QuickSend( pPlayer, pMsg, nLength );
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPartyWar::CPartyWar()
{
	m_dwIdx = 0;
	m_nState = ePartyWar_Null;
	m_dwWarTime = 0;
}

CPartyWar::~CPartyWar()
{
}

void CPartyWar::Init(DWORD dwPartyIdx1, DWORD dwPartyIdx2, DWORD dwIdx )
{
	m_dwIdx = dwIdx;

	m_Team1.SetPartyIdx( dwPartyIdx1 );
	m_Team2.SetPartyIdx( dwPartyIdx2 );

	m_nState = ePartyWar_PreWait;
	m_dwWarTime = gCurTime;
	m_dwWinner = 0;
}

int CPartyWar::IsMemberInPartyWar( DWORD dwPlayerIdx, DWORD dwPartyIdx )
{
	if( m_Team1.GetPartyIdx() == dwPartyIdx )
	{
		if( m_Team1.IsWarMember( dwPlayerIdx ) )	return 1;
		else										return 0;
	}
	else if( m_Team2.GetPartyIdx() == dwPartyIdx )
	{
		if( m_Team2.IsWarMember( dwPlayerIdx ) )	return 2;
		else										return 0;
	}
	
	return 0;
}

BOOL CPartyWar::IsEnemy( CPlayer* pTarget, DWORD dwTargetPartyIdx )
{
	if( m_Team1.GetPartyIdx() == dwTargetPartyIdx )
		return m_Team1.IsWarMember( pTarget->GetID() );
	else if( m_Team2.GetPartyIdx() == dwTargetPartyIdx )
		return m_Team2.IsWarMember( pTarget->GetID() );
	
	return FALSE;
}

BOOL CPartyWar::PlayerDie( DWORD dwPlayerIdx, DWORD dwPartyIdx )
{
	if( m_Team1.GetPartyIdx() == dwPartyIdx )
		return m_Team1.MemberDie( dwPlayerIdx );
	else if( m_Team2.GetPartyIdx() == dwPartyIdx )
		return m_Team2.MemberDie( dwPlayerIdx );
	return FALSE;
}

void CPartyWar::RemovePlayer( DWORD dwPlayerIdx, DWORD dwPartyIdx )
{
	if( m_Team1.GetPartyIdx() == dwPartyIdx )
		m_Team1.MemberDie( dwPlayerIdx );
	else if( m_Team2.GetPartyIdx() == dwPartyIdx )
		m_Team2.MemberDie( dwPlayerIdx );

	if( m_nState == ePartyWar_PreWait || m_nState == ePartyWar_Wait )
	{
		// sendmsg
		MSG_DWORD Msg;
		Msg.Category = MP_PARTYWAR;
		Msg.Protocol = MP_PARTYWAR_NACK;
		Msg.dwData = 99;
			
		m_Team1.SendMsgToMaster( &Msg, sizeof(Msg) );
		m_Team2.SendMsgToMaster( &Msg, sizeof(Msg) );

		m_nState = ePartyWar_End;
		m_dwWarTime = gCurTime;
	}
}

void CPartyWar::Process()
{
	switch( m_nState )
	{
	case ePartyWar_PreWait:		ProcessPreWait();	break;
	case ePartyWar_Wait:		ProcessWait();		break;
	case ePartyWar_Ready:		ProcessReady();		break;
	case ePartyWar_Fight:		ProcessFight();		break;
	case ePartyWar_Result:		ProcessResult();	break;
	}
}

void CPartyWar::ProcessPreWait()
{
	DWORD dwRemainTime = gCurTime - m_dwWarTime;
	if( dwRemainTime > 10000 )
	{
		// sendmsg
		MSG_DWORD Msg;
		Msg.Category = MP_PARTYWAR;
		Msg.Protocol = MP_PARTYWAR_NACK;
		Msg.dwData = 99;
			
		m_Team1.SendMsgToMaster( &Msg, sizeof(Msg) );
		m_Team2.SendMsgToMaster( &Msg, sizeof(Msg) );

		m_nState = ePartyWar_End;
		m_dwWarTime = gCurTime;
	}
}

void CPartyWar::ProcessWait()
{
	if( m_Team1.IsReady() && m_Team2.IsReady() )
	{
		// sendmsg
		MSG_BATTLE_VIMU_CREATESTAGE Msg;
		Msg.Category = MP_PARTYWAR;
		Msg.Protocol = MP_PARTYWAR_READY;
		Msg.vPosStage = *m_Team1.GetWarPosition();
		
//		m_Team1.SendMsgToAllMemberInWar( &Msg, sizeof(Msg) );
//		m_Team2.SendMsgToAllMemberInWar( &Msg, sizeof(Msg) );

		Msg.dwBattleID = 1;
		m_Team1.SendMsgToGridInWar( &Msg, sizeof(Msg) );
		Msg.dwBattleID = 2;
		m_Team2.SendMsgToGridInWar( &Msg, sizeof(Msg) );

		m_nState = ePartyWar_Ready;
		m_dwWarTime = gCurTime;		
	}
	else
	{
		DWORD dwRemainTime = gCurTime - m_dwWarTime;
		if( dwRemainTime > 180000 )
		{
			// sendmsg
			MSG_DWORD Msg;
			Msg.Category = MP_PARTYWAR;
			Msg.Protocol = MP_PARTYWAR_NACK;
			Msg.dwData = 99;
			
			m_Team1.SendMsgToAllMember( &Msg, sizeof(Msg) );
			m_Team2.SendMsgToAllMember( &Msg, sizeof(Msg) );

			m_nState = ePartyWar_End;
			m_dwWarTime = gCurTime;
		}
	}
}

void CPartyWar::ProcessReady()
{
	DWORD dwRemainTime = gCurTime - m_dwWarTime;
	if( dwRemainTime > 10000 )
	{
		// sendmsg
		MSGBASE Msg;
		Msg.Category = MP_PARTYWAR;
		Msg.Protocol = MP_PARTYWAR_FIGHT;
		
		m_Team1.SendMsgToAllMemberInWar( &Msg, sizeof(Msg) );
		m_Team2.SendMsgToAllMemberInWar( &Msg, sizeof(Msg) );

		m_nState = ePartyWar_Fight;
		m_dwWarTime = gCurTime;
	}
}

void CPartyWar::ProcessFight()
{
	MSG_DWORD Msg;
	Msg.Category = MP_PARTYWAR;
	Msg.Protocol = MP_PARTYWAR_RESULT;

	BOOL bTeam1 = m_Team1.IsAlive();
	BOOL bTeam2 = m_Team2.IsAlive();	

	if( bTeam1 && bTeam2 )
	{
		DWORD dwRemainTime = gCurTime - m_dwWarTime;
		if( dwRemainTime > 120000 )
		{
			Msg.dwData = 2;		// draw

			m_Team1.SendMsgToGridInWar( &Msg, sizeof(Msg) );
			m_Team2.SendMsgToGridInWar( &Msg, sizeof(Msg) );

			m_nState = ePartyWar_Result;
			m_dwWarTime = gCurTime;
			m_dwWinner = 0;
		}
	}
	else if( bTeam1 && !bTeam2 )
	{
		// sendmsg
		Msg.dwData = 1;		// win
		m_Team1.SendMsgToGridInWar( &Msg, sizeof(Msg) );
		Msg.dwData = 0;		// lose
		m_Team2.SendMsgToGridInWar( &Msg, sizeof(Msg) );

		m_nState = ePartyWar_Result;
		m_dwWarTime = gCurTime;
		m_dwWinner = 1;
	}
	else if( !bTeam1 && bTeam2 )
	{
		// sendmsg
		Msg.dwData = 0;		// lose
		m_Team1.SendMsgToGridInWar( &Msg, sizeof(Msg) );
		Msg.dwData = 1;		// win
		m_Team2.SendMsgToGridInWar( &Msg, sizeof(Msg) );

		m_nState = ePartyWar_Result;
		m_dwWarTime = gCurTime;
		m_dwWinner = 2;
	}
	else
	{
		Msg.dwData = 2;		// draw
		m_Team1.SendMsgToGridInWar( &Msg, sizeof(Msg) );
		m_Team2.SendMsgToGridInWar( &Msg, sizeof(Msg) );

		m_nState = ePartyWar_Result;
		m_dwWarTime = gCurTime;
		m_dwWinner = 0;
	}
}

void CPartyWar::ProcessResult()
{
	DWORD dwRemainTime = gCurTime - m_dwWarTime;
	if( dwRemainTime > 5000 )
	{
		// sendmsg
		m_Team1.SendMsgRevive();
		m_Team2.SendMsgRevive();

		MSG_NAME2_DWORD Msg;
		Msg.Category = MP_PARTYWAR;
		Msg.Protocol = MP_PARTYWAR_END;
		SafeStrCpy( Msg.Name1, m_Team1.GetMasterName(), MAX_NAME_LENGTH+1 );
		SafeStrCpy( Msg.Name2, m_Team2.GetMasterName(), MAX_NAME_LENGTH+1 );
		Msg.dwData = m_dwWinner;
		if( m_dwWinner == 0 || m_dwWinner == 1 )
			m_Team1.SendMsgToGridInOne( &Msg, sizeof(Msg) );
		else if( m_dwWinner == 2 )
			m_Team2.SendMsgToGridInOne( &Msg, sizeof(Msg) );

		m_nState = ePartyWar_End;
		m_dwWarTime = gCurTime;
	}
}

void CPartyWar::PartyWarSuggestAccept( DWORD dwChannel )
{
	CParty* pParty1 = PARTYMGR->GetParty( m_Team1.GetPartyIdx() );
	CParty* pParty2 = PARTYMGR->GetParty( m_Team2.GetPartyIdx() );
	if( pParty1 == NULL || pParty2 == NULL )	return;

	// set party info
	PARTYMEMBER* pPartyInfo1 = pParty1->GetPartyMember();
	PARTYMEMBER* pPartyInfo2 = pParty2->GetPartyMember();
	PARTY_INFO Msg1, Msg2;
	Msg1.Category = Msg2.Category = MP_PARTYWAR;
	Msg1.Protocol = Msg2.Protocol = MP_PARTYWAR_SUGGEST_ACCEPT;
	Msg1.PartyDBIdx = pParty1->GetPartyIdx();
	Msg2.PartyDBIdx = pParty2->GetPartyIdx();
	for( int i = 0; i < MAX_PARTY_LISTNUM; ++i )
	{
		Msg1.Member[i].bLogged = pPartyInfo1[i].bLogged;
		Msg1.Member[i].dwMemberID = pPartyInfo1[i].MemberID;
		Msg1.Member[i].Level = pPartyInfo1[i].Level;
		Msg1.Member[i].LifePercent = pPartyInfo1[i].LifePercent;
		Msg1.Member[i].NaeRyukPercent = pPartyInfo1[i].NaeRyukPercent;
		SafeStrCpy( Msg1.Member[i].Name, pPartyInfo1[i].Name, MAX_NAME_LENGTH+1 );
		Msg1.Member[i].ShieldPercent = pPartyInfo1[i].ShieldPercent;
		
		Msg2.Member[i].bLogged = pPartyInfo2[i].bLogged;
		Msg2.Member[i].dwMemberID = pPartyInfo2[i].MemberID;
		Msg2.Member[i].Level = pPartyInfo2[i].Level;
		Msg2.Member[i].LifePercent = pPartyInfo2[i].LifePercent;
		Msg2.Member[i].NaeRyukPercent = pPartyInfo2[i].NaeRyukPercent;
		SafeStrCpy( Msg2.Member[i].Name, pPartyInfo2[i].Name, MAX_NAME_LENGTH+1 );
		Msg2.Member[i].ShieldPercent = pPartyInfo2[i].ShieldPercent;
	}

	// init member
	CPlayer* pPlayer = NULL;
	for( i = 0; i < MAX_PARTY_LISTNUM; ++i )
	{
		pPlayer = (CPlayer*)g_pUserTable->FindUser( pPartyInfo1[i].MemberID );
		if( pPlayer )
		{
			if( pPlayer->GetChannelID() == dwChannel )
			{
				m_Team1.InitMember( pPartyInfo1[i].MemberID, i );
			}
		}
		pPlayer = (CPlayer*)g_pUserTable->FindUser( pPartyInfo2[i].MemberID );
		if( pPlayer )
		{
			if( pPlayer->GetChannelID() == dwChannel )
			{
				m_Team2.InitMember( pPartyInfo2[i].MemberID, i );
			}
		}
	}	
	// set master name
	m_Team1.SetMasterName( pPartyInfo1[0].Name );
	m_Team2.SetMasterName( pPartyInfo2[0].Name );	

	// send msg
	m_Team1.SendMsgToAllMember( (MSGBASE*)&Msg2, sizeof(Msg2) );
	m_Team2.SendMsgToAllMember( (MSGBASE*)&Msg1, sizeof(Msg1) );

	m_nState = ePartyWar_Wait;
	m_dwWarTime = gCurTime;
}

void CPartyWar::PartyWarSuggestDeny( DWORD dwPartyIdx )
{
	if( m_Team1.GetPartyIdx() == dwPartyIdx )
	{
		// sendmsg
		MSGBASE Msg;
		Msg.Category = MP_PARTYWAR;
		Msg.Protocol = MP_PARTYWAR_SUGGEST_DENY;
		m_Team2.SendMsgToMaster( &Msg, sizeof(Msg) );

		m_nState = ePartyWar_End;
		m_dwWarTime = gCurTime;
	}
	else if( m_Team2.GetPartyIdx() == dwPartyIdx )
	{
		// sendmsg
		MSGBASE Msg;
		Msg.Category = MP_PARTYWAR;
		Msg.Protocol = MP_PARTYWAR_SUGGEST_DENY;
		m_Team1.SendMsgToMaster( &Msg, sizeof(Msg) );

		m_nState = ePartyWar_End;
		m_dwWarTime = gCurTime;
	}
}

void CPartyWar::PartyWarAddMember( DWORD dwPartyIdx, DWORD dwMemberIdx, int nIndex )
{
	MSG_DWORD3 Msg;
	Msg.Category = MP_PARTYWAR;
	Msg.dwData1 = dwPartyIdx;
	Msg.dwData2 = dwMemberIdx;
	Msg.dwData3 = nIndex;
	
	if( m_Team1.GetPartyIdx() == dwPartyIdx )
	{
		if( m_Team1.IsAddableMember( dwMemberIdx, nIndex ) )
		{
			Msg.Protocol = MP_PARTYWAR_ADDMEMBER_ACK;
			m_Team1.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );
			m_Team2.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );

			m_Team1.AddMember( dwMemberIdx, nIndex );			
		}
		else
		{
			Msg.Protocol = MP_PARTYWAR_ADDMEMBER_NACK;
			m_Team1.SendMsgToMaster( (MSGBASE*)&Msg, sizeof(Msg) );
		}		
	}
	else if( m_Team2.GetPartyIdx() == dwPartyIdx )
	{
		if( m_Team2.IsAddableMember( dwMemberIdx, nIndex ) )
		{
			Msg.Protocol = MP_PARTYWAR_ADDMEMBER_ACK;
			m_Team1.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );
			m_Team2.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );

			m_Team2.AddMember( dwMemberIdx, nIndex );			
		}
		else
		{
			Msg.Protocol = MP_PARTYWAR_ADDMEMBER_NACK;
			m_Team2.SendMsgToMaster( (MSGBASE*)&Msg, sizeof(Msg) );
		}
	}
}

void CPartyWar::PartyWarRemoveMember( DWORD dwPartyIdx, DWORD dwMemberIdx, int nIndex )
{
	MSG_DWORD3 Msg;
	Msg.Category = MP_PARTYWAR;
	Msg.Protocol = MP_PARTYWAR_REMOVEMEMBER_ACK;
	Msg.dwData1 = dwPartyIdx;
	Msg.dwData2 = dwMemberIdx;
	Msg.dwData3 = nIndex;

	if( m_Team1.GetPartyIdx() == dwPartyIdx )
	{
		m_Team1.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );
		m_Team2.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );

		m_Team1.RemoveMember( dwMemberIdx, nIndex );		
	}
	else if( m_Team2.GetPartyIdx() == dwPartyIdx )
	{
		m_Team1.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );
		m_Team2.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );

		m_Team2.RemoveMember( dwMemberIdx, nIndex );		
	}
}

void CPartyWar::PartyWarLock( DWORD dwPartyIdx )
{
	MSG_DWORD Msg;
	Msg.Category = MP_PARTYWAR;
	Msg.Protocol = MP_PARTYWAR_LOCK;
	Msg.dwData = dwPartyIdx;

	if( m_Team1.GetPartyIdx() == dwPartyIdx )
	{
		if( !m_Team1.IsAlive() )	return;
		if( m_Team1.IsLock() )		return;

		m_Team1.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );
		m_Team2.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );		

		m_Team1.SetLock( TRUE );		
	}
	else if( m_Team2.GetPartyIdx() == dwPartyIdx )
	{
		if( !m_Team2.IsAlive() )	return;
		if( m_Team2.IsLock() )		return;

		m_Team1.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );
		m_Team2.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );		

		m_Team2.SetLock( TRUE );		
	}
}

void CPartyWar::PartyWarUnLock( DWORD dwPartyIdx )
{
	MSG_DWORD Msg;
	Msg.Category = MP_PARTYWAR;
	Msg.Protocol = MP_PARTYWAR_UNLOCK;
	Msg.dwData = dwPartyIdx;

	if( m_Team1.GetPartyIdx() == dwPartyIdx )
	{
		m_Team1.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );
		m_Team2.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );
		
		m_Team1.SetLock( FALSE );	m_Team1.SetReady( FALSE );
		m_Team2.SetLock( FALSE );	m_Team2.SetReady( FALSE );
	}
	else if( m_Team2.GetPartyIdx() == dwPartyIdx )
	{
		m_Team1.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );
		m_Team2.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );		

		m_Team1.SetLock( FALSE );	m_Team1.SetReady( FALSE );
		m_Team2.SetLock( FALSE );	m_Team2.SetReady( FALSE );
	}
}

void CPartyWar::PartyWarStart( DWORD dwPartyIdx )
{
	MSG_DWORD Msg;
	Msg.Category = MP_PARTYWAR;
	Msg.Protocol = MP_PARTYWAR_NACK;
	Msg.dwData = 107;

	if( m_Team1.GetPartyIdx() == dwPartyIdx )
	{
		if( m_Team1.IsLock() )
			m_Team1.SetReady( TRUE );
		else
			m_Team1.SendMsgToMaster( &Msg, sizeof(Msg) );
	}
	else if( m_Team2.GetPartyIdx() == dwPartyIdx )
	{
		if( m_Team2.IsLock() )
			m_Team2.SetReady( TRUE );
		else
			m_Team2.SendMsgToMaster( &Msg, sizeof(Msg) );
	}
}

void CPartyWar::PartyWarCancel( DWORD dwPartyIdx )
{
	MSGBASE Msg;
	Msg.Category = MP_PARTYWAR;
	Msg.Protocol = MP_PARTYWAR_CANCEL;

	if( m_Team1.GetPartyIdx() == dwPartyIdx || m_Team2.GetPartyIdx() == dwPartyIdx )
	{
		m_Team1.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );
		m_Team2.SendMsgToAllMember( (MSGBASE*)&Msg, sizeof(Msg) );
		
		m_nState = ePartyWar_End;
		m_dwWarTime = gCurTime;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPartyWarMgr::CPartyWarMgr()
{
	m_PartyWarIdxTable.Initialize( 200 );
	m_PartyWarTable.Initialize( 100 );
	m_dwPartyWarTableIdx = 0;
}

CPartyWarMgr::~CPartyWarMgr()
{
	Release();
}

void CPartyWarMgr::Init() 
{
}

void CPartyWarMgr::Release()
{
	DWORD* pPartyWarIdx = NULL;
	m_PartyWarIdxTable.SetPositionHead();
	while( pPartyWarIdx = m_PartyWarIdxTable.GetData() )
		delete pPartyWarIdx;
	m_PartyWarIdxTable.RemoveAll();

	CPartyWar* pPartyWar = NULL;
	m_PartyWarTable.SetPositionHead();
	while( pPartyWar = m_PartyWarTable.GetData() )
		delete pPartyWar;
	m_PartyWarTable.RemoveAll();

	m_dwPartyWarTableIdx = 0;
}

BOOL CPartyWarMgr::IsEnemy( CPlayer* pOper, CPlayer* pTarget )
{
	DWORD dwOperPartyIdx = pOper->GetPartyIdx();
	DWORD dwTargetPartyIdx = pTarget->GetPartyIdx();

	if( dwOperPartyIdx == 0 || dwTargetPartyIdx == 0 )
		return FALSE;

	DWORD* pIdx1 = m_PartyWarIdxTable.GetData( dwOperPartyIdx );
	if( !pIdx1 )	return FALSE;
	DWORD* pIdx2 = m_PartyWarIdxTable.GetData( dwTargetPartyIdx );
	if( !pIdx2 )	return FALSE;
	if( *pIdx1 != *pIdx2 )	return FALSE;

	CPartyWar* pPartyWar = m_PartyWarTable.GetData( *pIdx1 );
	if( !pPartyWar )	return FALSE;

	return pPartyWar->IsEnemy( pTarget, dwTargetPartyIdx );
}

BOOL CPartyWarMgr::PlayerDie( CPlayer* pPlayer, CPlayer* pAttacker )
{
	DWORD dwPlayerPartyIdx = pPlayer->GetPartyIdx();
	DWORD dwAttackerPartyIdx = pAttacker->GetPartyIdx();
	if( dwPlayerPartyIdx == 0 || dwAttackerPartyIdx == 0 )	return FALSE;

	DWORD* pIdx1 = m_PartyWarIdxTable.GetData( dwPlayerPartyIdx );
	DWORD* pIdx2 = m_PartyWarIdxTable.GetData( dwAttackerPartyIdx );
	if( pIdx1 == NULL || pIdx2 == NULL )	return FALSE;
	if( *pIdx1 != *pIdx2 )	return FALSE;
	
	CPartyWar* pPartyWar = m_PartyWarTable.GetData( *pIdx1 );
	if( pPartyWar == NULL )	return FALSE;

	return pPartyWar->PlayerDie( pPlayer->GetID(), dwPlayerPartyIdx );
}

void CPartyWarMgr::RemovePlayer( CPlayer* pPlayer )
{
	DWORD dwPartyIdx = pPlayer->GetPartyIdx();
	if( dwPartyIdx == 0 )	return;

	DWORD* pIdx1 = m_PartyWarIdxTable.GetData( dwPartyIdx );
	if( pIdx1 == NULL )	return;
	
	CPartyWar* pPartyWar = m_PartyWarTable.GetData( *pIdx1 );
	if( pPartyWar == NULL )	return;

	pPartyWar->RemovePlayer( pPlayer->GetID(), dwPartyIdx );
}

void CPartyWarMgr::RegistPartyWar( DWORD dwPartyIdx1, DWORD dwPartyIdx2 )
{
	++m_dwPartyWarTableIdx;

	DWORD* pIdx1 = new DWORD;
	*pIdx1 = m_dwPartyWarTableIdx;
	m_PartyWarIdxTable.Add( pIdx1, dwPartyIdx1 );
	DWORD* pIdx2 = new DWORD;
	*pIdx2 = m_dwPartyWarTableIdx;
	m_PartyWarIdxTable.Add( pIdx2, dwPartyIdx2 );
	
	CPartyWar* pPartyWar = m_PartyWarTable.GetData( m_dwPartyWarTableIdx );
	if( !pPartyWar )
	{
		pPartyWar = new CPartyWar;
		m_PartyWarTable.Add( pPartyWar, m_dwPartyWarTableIdx );
	}
	pPartyWar->Init( dwPartyIdx1, dwPartyIdx2, m_dwPartyWarTableIdx );
}

void CPartyWarMgr::UnRegistPartyWar( CPartyWar* pPartyWar )
{
	DWORD dwIdx, dwTeam1, dwTeam2;
	dwIdx = pPartyWar->GetIndex();
	pPartyWar->GetPartyIdx( &dwTeam1, &dwTeam2 );

	delete pPartyWar;
	m_PartyWarTable.Remove( dwIdx );

	DWORD* pTeam1 = m_PartyWarIdxTable.GetData( dwTeam1 );
	if( pTeam1 )
	{
		delete pTeam1;
		m_PartyWarIdxTable.Remove( dwTeam1 );
	}
	DWORD* pTeam2 = m_PartyWarIdxTable.GetData( dwTeam2 );
	{
		delete pTeam2;
		m_PartyWarIdxTable.Remove( dwTeam2 );
	}
}

void CPartyWarMgr::Process()
{
	CPartyWar* pPartyWar = NULL;
	m_PartyWarTable.SetPositionHead();
	cPtrList list;
	while( pPartyWar = m_PartyWarTable.GetData() )
	{
		if( pPartyWar->GetState() == ePartyWar_End )
			list.AddTail( pPartyWar );
		else
			pPartyWar->Process();
	}

	PTRLISTPOS pos = list.GetHeadPosition();
	while( pos )
	{
		CPartyWar* p = (CPartyWar*)list.GetNext( pos );
		UnRegistPartyWar( p );	
	}
	list.RemoveAll();
	
}

void CPartyWarMgr::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_PARTYWAR_SUGGEST:			Msg_PartyWarSuggest( pMsg );		break;		
	case MP_PARTYWAR_SUGGEST_ACCEPT:	Msg_PartyWarSuggestAccept( pMsg );	break;
	case MP_PARTYWAR_SUGGEST_DENY:		Msg_PartyWarSuggestDeny( pMsg );	break;
	case MP_PARTYWAR_ADDMEMBER_SYN:		Msg_PartyWarAddMemberSyn( pMsg );		break;
	case MP_PARTYWAR_REMOVEMEMBER_SYN:	Msg_PartyWarRemoveMemberSyn( pMsg );	break;
	case MP_PARTYWAR_LOCK:				Msg_PartyWarLock( pMsg );			break;
	case MP_PARTYWAR_UNLOCK:			Msg_PartyWarUnLock( pMsg );			break;
	case MP_PARTYWAR_START:				Msg_PartyWarStart( pMsg );			break;
	case MP_PARTYWAR_CANCEL:			Msg_PartyWarCancel( pMsg );			break;
	}	
}

void CPartyWarMgr::Msg_PartyWarSuggest( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	CPlayer* pReceiver = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData );
	if( pSender == NULL || pReceiver == NULL )	return;

	// check party
	DWORD dwSenderPartyIdx = pSender->GetPartyIdx();
	CParty* pSenderParty = PARTYMGR->GetParty( dwSenderPartyIdx );
	if( pSenderParty == NULL )
	{
		SendNackMsg( pSender, 1 );		// 1 = sender is not in party
		return;
	}
	DWORD dwReceiverPartyIdx = pReceiver->GetPartyIdx();
	CParty* pReceiverParty = PARTYMGR->GetParty( dwReceiverPartyIdx );
	if( pReceiverParty == NULL )
	{
		SendNackMsg( pSender, 2 );		// 2 = receiver is not in party
		return;
	}	
	if( dwSenderPartyIdx == dwReceiverPartyIdx )
	{
		SendNackMsg( pSender, 3 );		// 3 = same party
		return;
	}
	// check master
	DWORD dwSenderPartyMasterIdx = pSenderParty->GetMasterID();
	if( dwSenderPartyMasterIdx != pmsg->dwObjectID )
	{
		SendNackMsg( pSender, 4 );		// 4 = sender is not partymaster
		return;
	}
	DWORD dwReceiverPartyMasterIdx = pReceiverParty->GetMasterID();
	CPlayer* pReceiverPartyMaster = (CPlayer*)g_pUserTable->FindUser( dwReceiverPartyMasterIdx );
	if( pReceiverPartyMaster == NULL )
	{
		SendNackMsg( pSender, 5 );		// 5 = receiverpartymaster is not map
		return;
	}
	else
	{
		if( pSender->GetBattleID() != pReceiverPartyMaster->GetBattleID() )
		{
			SendNackMsg( pSender, 6 );	// 6 = receiverpartymaster is not same channel
			return;
		}
	}
	// check partywar
	if( m_PartyWarIdxTable.GetData( dwSenderPartyIdx ) )
	{
		SendNackMsg( pSender, 7 );		// 7 = sender is in partywar
		return;
	}
	if( m_PartyWarIdxTable.GetData( dwReceiverPartyIdx ) )
	{
		SendNackMsg( pSender, 8 );		// 8 = receiver is in partywar
		return;
	}
	// check state
	if( pSender->IsPKMode() || pSender->IsVimuing() )
	if( pSender->GetState() != eObjectState_None && pSender->GetState() != eObjectState_Move )
	{
		SendNackMsg( pSender, 9 );		// 9 = sender is not in right state
		return;
	}
	if( pReceiverPartyMaster->IsPKMode() || pReceiverPartyMaster->IsVimuing() )
	if( pReceiverPartyMaster->GetState() != eObjectState_None && pReceiverPartyMaster->GetState() != eObjectState_Move )
	{
		SendNackMsg( pSender, 9 );		// 9 = receiverpartymaster is not in right state
		return;
	}
	// check distance
	VECTOR3 vSenderPos, vReceiverPos;
	pSender->GetPosition( &vSenderPos );
	pReceiverPartyMaster->GetPosition( &vReceiverPos );
	if( CalcDistanceXZ( &vSenderPos, &vReceiverPos ) > 1000.0f )
	{
		SendNackMsg( pSender, 10 );		// 10 = receiverpartymaster is not in right distance
		return;
	}	
	//if( g_pServerSystem->GetMapNum() == RUNNINGMAP )
	if( g_pServerSystem->GetMap()->IsMapSame(eRunningMap) )
	{
		SendNackMsg( pSender, 9 );		// 9 = 달리기맵에서 방파전금지
		return;
	}
//	if( g_pServerSystem->GetMapNum() == PKEVENTMAP )
//	{
//		SendNackMsg( pSender, 9 );
//		return;
//	}
	//if( g_pServerSystem->CheckMapKindIs(eSurvival) )
	if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )
	{
		SendNackMsg( pSender, 10 );
		return;
	}
	// sendmsg
	MSGBASE ToSender;
	ToSender.Category = MP_PARTYWAR;
	ToSender.Protocol = MP_PARTYWAR_SUGGEST_WAIT;
	pSender->SendMsg( &ToSender, sizeof(ToSender) );
	MSG_NAME ToReceiver;
	ToReceiver.Category = MP_PARTYWAR;
	ToReceiver.Protocol = MP_PARTYWAR_SUGGEST;
	SafeStrCpy( ToReceiver.Name, pSender->GetObjectName(), MAX_NAME_LENGTH+1 );
	pReceiverPartyMaster->SendMsg( &ToReceiver, sizeof(ToReceiver) );

	// regist partywar
	RegistPartyWar( dwSenderPartyIdx, dwReceiverPartyIdx );
}

void CPartyWarMgr::Msg_PartyWarSuggestAccept( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;	

	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( pSender == NULL )	return;

	DWORD* pIdx = m_PartyWarIdxTable.GetData( pSender->GetPartyIdx() );
	if( !pIdx )	return;
	CPartyWar* pPartyWar = m_PartyWarTable.GetData( *pIdx );
	if( !pPartyWar )	return;

	pPartyWar->PartyWarSuggestAccept( pSender->GetChannelID() );
}

void CPartyWarMgr::Msg_PartyWarSuggestDeny( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;	

	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pSender )	return;
	DWORD* pIdx = m_PartyWarIdxTable.GetData( pSender->GetPartyIdx() );
	if( !pIdx )	return;
	CPartyWar* pPartyWar = m_PartyWarTable.GetData( *pIdx );
	if( !pPartyWar )	return;

	pPartyWar->PartyWarSuggestDeny( pSender->GetPartyIdx() );
}

void CPartyWarMgr::Msg_PartyWarAddMemberSyn( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pSender )	return;
	DWORD* pIdx = m_PartyWarIdxTable.GetData( pSender->GetPartyIdx() );
	if( !pIdx )	return;
	CPartyWar* pPartyWar = m_PartyWarTable.GetData( *pIdx );
	if( !pPartyWar )	return;

	pPartyWar->PartyWarAddMember( pSender->GetPartyIdx(), pmsg->dwData1, pmsg->dwData2 );
}

void CPartyWarMgr::Msg_PartyWarRemoveMemberSyn( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pSender )	return;
	DWORD* pIdx = m_PartyWarIdxTable.GetData( pSender->GetPartyIdx() );
	if( !pIdx )	return;
	CPartyWar* pPartyWar = m_PartyWarTable.GetData( *pIdx );
	if( !pPartyWar )	return;

	pPartyWar->PartyWarRemoveMember( pSender->GetPartyIdx(), pmsg->dwData1, pmsg->dwData2 );
}

void CPartyWarMgr::Msg_PartyWarLock( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pSender )	return;
	DWORD* pIdx = m_PartyWarIdxTable.GetData( pSender->GetPartyIdx() );
	if( !pIdx )	return;
	CPartyWar* pPartyWar = m_PartyWarTable.GetData( *pIdx );
	if( !pPartyWar )	return;

	pPartyWar->PartyWarLock( pSender->GetPartyIdx() );
}

void CPartyWarMgr::Msg_PartyWarUnLock( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pSender )	return;

	DWORD* pIdx = m_PartyWarIdxTable.GetData( pSender->GetPartyIdx() );
	if( !pIdx )	return;

	CPartyWar* pPartyWar = m_PartyWarTable.GetData( *pIdx );
	if( !pPartyWar )	return;

	pPartyWar->PartyWarUnLock( pSender->GetPartyIdx() );
}

void CPartyWarMgr::Msg_PartyWarStart( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pSender )	return;

	DWORD* pIdx = m_PartyWarIdxTable.GetData( pSender->GetPartyIdx() );
	if( !pIdx )	return;
	
	CPartyWar* pPartyWar = m_PartyWarTable.GetData( *pIdx );
	if( !pPartyWar )	return;

	pPartyWar->PartyWarStart( pSender->GetPartyIdx() );
}

void CPartyWarMgr::Msg_PartyWarCancel( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pSender )	return;

	DWORD* pIdx = m_PartyWarIdxTable.GetData( pSender->GetPartyIdx() );
	if( !pIdx )	return;

	CPartyWar* pPartyWar = m_PartyWarTable.GetData( *pIdx );
	if( !pPartyWar )	return;

	pPartyWar->PartyWarCancel( pSender->GetPartyIdx() );
}

void CPartyWarMgr::SendNackMsg( CPlayer* pPlayer, DWORD dwSate )
{
	MSG_DWORD Nack;
	Nack.Category = MP_PARTYWAR;
	Nack.Protocol = MP_PARTYWAR_NACK;
	Nack.dwData = dwSate;
	
	pPlayer->SendMsg( &Nack, sizeof(Nack) );	
}

int CPartyWarMgr::IsMemberInPartyWar( CPlayer* pPlayer )
{
	DWORD dwPartyIdx = pPlayer->GetPartyIdx();
	if( dwPartyIdx == 0 )	return 0;

	DWORD* pIdx = m_PartyWarIdxTable.GetData( dwPartyIdx );
	if( !pIdx )	return 0;

	CPartyWar* pPartyWar = m_PartyWarTable.GetData( *pIdx );
	if( !pPartyWar )	return 0;

	return pPartyWar->IsMemberInPartyWar( pPlayer->GetID(), dwPartyIdx );
}

BOOL CPartyWarMgr::IsinSamePartyWar( CPlayer* pPlayer1, CPlayer* pPlayer2 )
{
	DWORD dwPartyIdx1 = pPlayer1->GetPartyIdx();
	if( dwPartyIdx1 == 0 )	return FALSE;
	DWORD* pPartyWarIdx1 = m_PartyWarIdxTable.GetData( dwPartyIdx1 );
	if( !pPartyWarIdx1 )	return FALSE;
	CPartyWar* pPartyWar1 = m_PartyWarTable.GetData( *pPartyWarIdx1 );
	if( !pPartyWar1 )		return FALSE;

	DWORD dwPartyIdx2 = pPlayer2->GetPartyIdx();
	if( dwPartyIdx2 == 0 )	return FALSE;
	DWORD* pPartyWarIdx2 = m_PartyWarIdxTable.GetData( dwPartyIdx2 );
	if( !pPartyWarIdx2 )	return FALSE;
	CPartyWar* pPartyWar2 = m_PartyWarTable.GetData( *pPartyWarIdx2 );
	if( !pPartyWar2 )		return FALSE;

	return (*pPartyWarIdx1==*pPartyWarIdx2)?TRUE:FALSE;
}
