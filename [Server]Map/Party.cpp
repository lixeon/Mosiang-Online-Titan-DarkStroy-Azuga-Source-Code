// Party.cpp: implementation of the CParty class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Party.h"
#include "UserTable.h"
#include "Network.h"
#include "Distribute_Random.h"
#include "Distribute_Damage.h"
#include "PartyManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//2008. 5. 21. CBH - 파티생성 추가 옵션 관련 수정
/*
CParty::CParty(DWORD id,DWORD MasterID,char* strMasterName,CPlayer* pMaster,BYTE Option) :
m_PartyIDx(id)
{
	m_Member[0].SetMember(MasterID,strMasterName,NULL, 0);
	m_TacticObjectID = 0;
	m_MasterChanging = FALSE;
	m_Option = Option;
	m_OldSendtime = 0;
	switch(m_Option)
	{
	case ePartyOpt_Random:
		m_pDistributeWay = DISTRIBUTERAND; break;
	case ePartyOpt_Damage:
		m_pDistributeWay = DISTRIBUTEDAM; break;
	case ePartyOpt_Sequence:
		{
			m_pDistributeWay = DISTRIBUTERAND;
			ASSERT(0);
		}
		break;
	default:
		{
			m_pDistributeWay = DISTRIBUTERAND;
			ASSERT(0);
		}		
		break;
	}
}
*/
CParty::CParty(DWORD id,DWORD MasterID,char* strMasterName,CPlayer* pMaster,PARTY_ADDOPTION* pAddOption) :
m_PartyIDx(id)
{
	m_Member[0].SetMember(MasterID,strMasterName,NULL, 0);
	m_TacticObjectID = 0;
	m_MasterChanging = FALSE;	
	memcpy(&m_AddPotion, pAddOption, sizeof(m_AddPotion));
	m_Option = pAddOption->bOption;
	m_OldSendtime = 0;
	switch(m_Option)
	{
	case ePartyOpt_Random:
		m_pDistributeWay = DISTRIBUTERAND; break;
	case ePartyOpt_Damage:
		m_pDistributeWay = DISTRIBUTEDAM; break;
	case ePartyOpt_Sequence:
		{
			m_pDistributeWay = DISTRIBUTERAND;
			ASSERT(0);
		}
		break;
	default:
		{
			m_pDistributeWay = DISTRIBUTERAND;
			ASSERT(0);
		}		
		break;
	}

	m_dwRequestPlayerID = 0;
	m_dwRequestProcessTime = 0;
}

CParty::~CParty()
{

}

void CParty::SetMaster(CPlayer* pMaster)
{
	m_Member[0].SetMember(pMaster->GetID(), pMaster->GetObjectName(), pMaster, pMaster->GetLevel());
}

void CParty::SendPartyInfo(CPlayer* pToPlayer)
{
	PARTY_INFO msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_INFO;
	msg.dwObjectID = pToPlayer->GetID();
	msg.PartyDBIdx = m_PartyIDx;
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		GetMemberInfo(n,&msg.Member[n]);
		CPlayer* pMember = (CPlayer*)g_pUserTable->FindUser(msg.Member[n].dwMemberID);
		if(pMember)
		if(pToPlayer->GetGridID() != pMember->GetGridID())
		{
			msg.Member[n].LifePercent = 0;
			msg.Member[n].NaeRyukPercent = 0;
		}
	}
	msg.Option = GetOption();
	msg.PartyAddOption = m_AddPotion;	//2008. 5. 22. CBH - 방파 추가 옵션 셋팅
	pToPlayer->SendMsg(&msg,sizeof(msg));
}

BOOL CParty::AddPartyMember(DWORD AddMemberID,char* Name,CPlayer* pPlayer, LEVELTYPE lvl)
{
	if(m_Member[0].MemberID == 0)
		ASSERTMSG(0, "파티 정보가 없는데 AddPartyMember함");
	for(int n=1;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].MemberID == 0)
		{
			m_Member[n].SetMember(AddMemberID,Name,pPlayer,lvl);
			//2008. 6. 16. CBH - 방파 매칭 시스탬 추가로 서로 다른맵에서 추가할때
			//신청자의 정보가 없어서 로그인 상태가 되지 않아서 방파 멤버 추가시
			//로그인상태로 설정해준다.
			m_Member[n].bLogged = TRUE;
			if(pPlayer)
			{				
				SendPartyInfo(pPlayer);
			}
			return TRUE;
		}
	}
	ASSERTMSG(0,"파티가 꽉 찾는데 Add하려 했습니다");

	return FALSE;
}

BOOL CParty::RemovePartyMember(DWORD MemberID) 
{
	for(int n=1;n<MAX_PARTY_LISTNUM;++n) 
	{
		if(m_Member[n].MemberID == MemberID) 
		{			
			m_Member[n].MemberID = 0;
			m_Member[n].Name[0] = 0;
			m_Member[n].bLogged = FALSE;
			m_Member[n].LifePercent = 0;
			m_Member[n].NaeRyukPercent = 0;
			m_Member[n].Level = 0;

			return TRUE;
		}
	}
	return FALSE;
} 

BOOL CParty::ChangeMaster(DWORD FromID,DWORD ToID)
{
	ASSERTMSG(m_Member[0].MemberID != ToID,"자기자신에게 리더를 이양하려 했습니다.");
	ASSERTMSG(m_Member[0].MemberID == FromID,"리더가 아닌 사람이 이양하려 했습니다.");


	for(int n=1; n<MAX_PARTY_LISTNUM; ++n) 
	{
		if(m_Member[n].MemberID == ToID) 
		{
			swap(m_Member[0], m_Member[n]);
			return TRUE;
		} //if
	} // for
	
	return FALSE;
} 

BOOL CParty::IsPartyMember(DWORD PlayerID) {
	for(int n=0; n<MAX_PARTY_LISTNUM; ++n) 
		if(m_Member[n].MemberID == PlayerID) return TRUE;
	return FALSE;	
	
}

BOOL CParty::IsMemberLogIn(int n)
{
	return m_Member[n].bLogged;
}

void CParty::UserLogIn(CPlayer* pPlayer,BOOL bNotifyUserLogin) 
{	
	int n = SetMemberInfo(pPlayer->GetID(), pPlayer->GetObjectName(), pPlayer, pPlayer->GetLevel(), TRUE);
	if(n == -1)
	{
		ASSERT(0); 
		return;
	}
	// 기존 파티의 정보를 로그인한 유저에게 보내준다.
	SendPartyInfo(pPlayer);

	if(bNotifyUserLogin)
	{
		// 로그인한 유저의 정보를 기존 파티원에게 보내준다.
		SendPlayerInfoToOtherMembers(n, pPlayer->GetGridID());
		
		SEND_CHANGE_PARTY_MEMBER_INFO cmsg;
		cmsg.Category = MP_PARTY;
		cmsg.Protocol = MP_PARTY_NOTIFYMEMBER_LOGIN_TO_MAPSERVER;
		cmsg.dwObjectID = pPlayer->GetID();
		cmsg.PartyID = pPlayer->GetPartyIdx();
		cmsg.Level = pPlayer->GetLevel();
		SafeStrCpy(cmsg.Name,this->GetMemberName(pPlayer->GetID()), MAX_NAME_LENGTH+1);
		
		g_Network.Send2AgentServer((char*)&cmsg,sizeof(cmsg));
	}

	//2008. 5. 27. CBH - 파티가 있는 유저가 접속하면 모든 맵서버에 파티정보를 보낸다
	PARTYMGR->NotifyPartyInfoSyn(pPlayer->GetPartyIdx());
}

void CParty::NotifyUserLogIn(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{
	int n = SetMemberInfo(pmsg->dwObjectID, pmsg->Name, NULL, pmsg->Level, TRUE);
	SendPlayerInfoToOtherMembers(n, 0);
}

void CParty::UserLogOut(DWORD PlayerID) 
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].MemberID == PlayerID)
		{
			m_Member[n].bLogged = FALSE;
			m_Member[n].LifePercent = 0;
			m_Member[n].NaeRyukPercent = 0;
			m_Member[n].ShieldPercent = 0;
		}
	}
}

void CParty::SendMsgUserLogOut(DWORD PlayerID)
{
	// 로그아웃한 유저의 정보를 다른 파티원에게 보내준다.
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBER_LOGOUT;
	msg.dwData = PlayerID;
	SendMsgToAll(&msg,sizeof(msg));
}

void CParty::SendMsgToAll(MSGBASE* msg, int size) 
{
	CPlayer* pPlayer;
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if(m_Member[i].bLogged == TRUE ) 
		{
			msg->dwObjectID = m_Member[i].MemberID;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(m_Member[i].MemberID);
			if(pPlayer == NULL)
				continue;

			pPlayer->SendMsg(msg, size);
		}
	}
} 

void CParty::SendMsgExceptOne(MSGBASE* msg, int size, DWORD PlayerID) 
{
	CPlayer* pPlayer;
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if(m_Member[i].MemberID == PlayerID )
			continue;
		else if(m_Member[i].bLogged == TRUE ) 
		{
			msg->dwObjectID = m_Member[i].MemberID;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(m_Member[i].MemberID);
			if(pPlayer == NULL)
				continue;
			
			pPlayer->SendMsg(msg, size);
		}
	}
}

void CParty::SendMsgExceptOneinChannel(MSGBASE* msg, int size, DWORD PlayerID, DWORD GridID)
{
	CPlayer* pPlayer;
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if(m_Member[i].MemberID == PlayerID )
			continue;
		else if(m_Member[i].bLogged == TRUE ) 
		{
			msg->dwObjectID = m_Member[i].MemberID;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(m_Member[i].MemberID);
			if(pPlayer == NULL)
				continue;
			if(pPlayer->GetGridID() != GridID)
				continue;
			pPlayer->SendMsg(msg, size);
		}
	}
}

void CParty::BreakUp()
{
	CPlayer* pPlayer;
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if(m_Member[i].MemberID != NULL ) 
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser(m_Member[i].MemberID);
			if(pPlayer == NULL)
				continue;
			
			pPlayer->SetPartyIdx(0);
		}
	}
}

void CParty::GetMemberInfo(int n, PARTY_MEMBER* pRtInfo)
{	
	pRtInfo->dwMemberID = m_Member[n].MemberID;
	SafeStrCpy(pRtInfo->Name,m_Member[n].Name, MAX_NAME_LENGTH+1);
	pRtInfo->bLogged = m_Member[n].bLogged;
	pRtInfo->Level = m_Member[n].Level;
	if(m_Member[n].MemberID != NULL && m_Member[n].bLogged == TRUE)
	{
		pRtInfo->LifePercent = m_Member[n].LifePercent;
		pRtInfo->NaeRyukPercent = m_Member[n].NaeRyukPercent;
		
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->GetData(m_Member[n].MemberID);
		if(pPlayer)
		{
			VECTOR3 vPos;
			pPlayer->GetPosition(&vPos);
			pRtInfo->posX = (WORD)vPos.x;
			pRtInfo->posZ = (WORD)vPos.z;
		}
	}
	else
	{
		pRtInfo->LifePercent = 0;
		pRtInfo->NaeRyukPercent = 0;
		pRtInfo->posX = 0;
		pRtInfo->posZ = 0;
	}
}

DWORD CParty::GetFirstOnlineMemberID()
{
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if( m_Member[i].bLogged == TRUE ) 
		{
			return m_Member[i].MemberID;
		}
	}
	return 0;
}

char* CParty::GetMemberName(DWORD MemberID)
{
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if(m_Member[i].MemberID == MemberID ) 
		{
			return m_Member[i].Name;
		} //if
	} //for
//	ASSERTMSG(0,"맴버가 아닌 사람의 아이디를 찾으려 했습니다.");
	return "";
}

void CParty::SendMemberLogInMsg(DWORD PlayerID)
{
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBER_LOGINMSG;
	msg.dwData = PlayerID;
	SendMsgToAll(&msg, sizeof(msg)); 
}

void CParty::NotifyMemberLogin(DWORD PlayerID)
{
	MSG_DWORD bmsg;
	bmsg.Category = MP_PARTY;
	bmsg.Protocol = MP_PARTY_NOTIFYMEMBER_LOGINMSG;
	bmsg.dwObjectID = PlayerID;
	bmsg.dwData = GetPartyIdx();
	g_Network.Send2AgentServer((char*)&bmsg, sizeof(bmsg));
}

int CParty::SetMemberInfo(DWORD MemberID, char* strName, CPlayer* pPlayer, LEVELTYPE lvl, BOOL bLog)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].MemberID == MemberID)
		{
			m_Member[n].SetMember(MemberID, strName, pPlayer, lvl);
			m_Member[n].bLogged = bLog;
			return n;
		}
	}
	ASSERT( n < MAX_PARTY_LISTNUM );
	return -1;
}

void CParty::SendPlayerInfoToOtherMembers(int n, DWORD GridID)
{
	SEND_PARTY_MEMBER_INFO msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBER_LOGIN;
	GetMemberInfo(n,&msg.MemberInfo);
	msg.PartyID = GetPartyIdx();
	SendMsgLoginMemberInfo(&msg, GridID);
}

void CParty::SendMsgLoginMemberInfo(SEND_PARTY_MEMBER_INFO* msg, DWORD GridID)
{
	CPlayer* pPlayer;
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if(m_Member[i].bLogged == TRUE ) 
		{
			msg->dwObjectID = m_Member[i].MemberID;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(m_Member[i].MemberID);
			if(pPlayer == NULL)
				continue;
			if(pPlayer->GetGridID() != GridID)
			{
				msg->MemberInfo.LifePercent = 0;
				msg->MemberInfo.NaeRyukPercent = 0;
			}
			pPlayer->SendMsg(msg, sizeof(SEND_PARTY_MEMBER_INFO));
		}
	}
}

void CParty::SetMasterChanging(BOOL val)
{
	m_MasterChanging = val;
}

BOOL CParty::IsMasterChanging(CPlayer* pPlayer, BYTE Protocol)
{
	if(m_MasterChanging == TRUE)
	{
		MSG_INT msg;
		msg.Category = MP_PARTY;
		msg.Protocol = Protocol;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return TRUE;
	}
	return FALSE;
}

void CParty::SetMemberLevel(DWORD PlayerID, LEVELTYPE lvl)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].MemberID == PlayerID)
		{
			m_Member[n].SetLevel(lvl);
			return;
		}
	}
	char buff[256] = {0, };
	sprintf(buff, "PartyID : %d, PlayerID : %d, level : %d", m_PartyIDx, PlayerID, lvl);
	ASSERTMSG(0, buff);
}

void CParty::SendMemberPos(DWORD MoverID, BASEMOVE_INFO* pMoveInfo)
{
	CPlayer* pPlayer = NULL;

	SEND_PARTYICON_MOVEINFO msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_SENDPOS;
	msg.MoveInfo.dwMoverID = MoverID;
	msg.MoveInfo.tgCount = pMoveInfo->GetMaxTargetPosIdx();
	msg.MoveInfo.cPos.posX = (WORD)pMoveInfo->CurPosition.x;
	msg.MoveInfo.cPos.posZ = (WORD)pMoveInfo->CurPosition.z;
	msg.MoveInfo.KyungGongIdx = pMoveInfo->KyungGongIdx;
	msg.MoveInfo.MoveState = pMoveInfo->MoveMode;
	
	for(int i=0; i<msg.MoveInfo.tgCount; i++)
	{
		VECTOR3* pTgPos = pMoveInfo->GetTargetPosition(i);
		msg.MoveInfo.tgPos[i].posX = (WORD)pTgPos->x;
		msg.MoveInfo.tgPos[i].posZ = (WORD)pTgPos->z;
	}
	
	for(i=0; i<MAX_PARTY_LISTNUM; i++)
	{
		if(m_Member[i].bLogged && m_Member[i].MemberID != MoverID)
//		if(m_Member[i].bLogged)
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser(m_Member[i].MemberID);
			if(!pPlayer)			continue;

			pPlayer->SendMsg(&msg, msg.GetSize());
		}
	}
}

void CParty::SetOption(BYTE Option)
{
	m_Option = Option;
}

BYTE CParty::GetOption()
{
	return m_Option;
}

void CParty::SendAbil( LEVELTYPE MonsterLevel, PARTY_RECEIVE_MEMBER* pMemberInfo, LEVELTYPE MaxLevel )
{
	m_pDistributeWay->CalcAbilandSend(MonsterLevel, pMemberInfo, MaxLevel);
}

void CParty::SendItem( PARTY_RECEIVE_MEMBER* pRealMember, WORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel )
{
	m_pDistributeWay->SendItem(pRealMember, DropItemId, DropItemRatio, pMonInfo, MonsterKind, MaxLevel);
}

int CParty::GetMemberCountofMap( DWORD dwPlayerID )
{	
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( dwPlayerID );
	if( pPlayer == NULL ) return FALSE;

#ifndef _HK_LOCAL_
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser( GetMasterID() );
	if( pMaster == NULL )
		return 0;
	else if( pPlayer->GetChannelID() != pMaster->GetChannelID() )
		return 0;
#endif

	int count = 0;
	for( int i = 0 ; i< MAX_PARTY_LISTNUM ; ++i )
	{
		CPlayer* pOther = (CPlayer*)g_pUserTable->FindUser( m_Member[i].MemberID );
		if( pOther == NULL ) continue;

		if( m_Member[i].bLogged && pPlayer->GetChannelID() == pOther->GetChannelID() )
             ++count;
	}

	return count;
}

#ifdef _JAPAN_LOCAL_
BOOL CParty::IsHelpPartyMember( DWORD dwPlayerID )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( dwPlayerID );
	if( pPlayer == NULL ) return FALSE;

	for( int i = 0 ; i< MAX_PARTY_LISTNUM ; ++i )
	{
		if( m_Member[i].bLogged && m_Member[i].MemberID != dwPlayerID )
		{
			CPlayer* pOther = (CPlayer*)g_pUserTable->FindUser( m_Member[i].MemberID );
			if( pOther == NULL ) continue;

			if( pPlayer->GetChannelID() != pOther->GetChannelID() ) continue;
			
			if( pPlayer->WhatIsAttrRelation( pOther->GetMainCharAttr() ) == eCAR_Need_Help )
			{
				VECTOR3 pMyPos, pOtherPos;
				pPlayer->GetPosition( &pMyPos );
				pOther->GetPosition( &pOtherPos );
				float dist = roughGetLength( pOtherPos.x - pMyPos.x, pOtherPos.z - pMyPos.z );
				if( dist <= 3000.0f )
					return TRUE;
			}
		}
	}

	return FALSE;
}
#endif

DWORD CParty::GetMemberNum()
{
	DWORD num=0;
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].MemberID != 0)
			++num;
	}
	return num;
}

//2008. 5. 21. CBH - 파티생성 추가 옵션 관련 수정
void CParty::SetAddOption(PARTY_ADDOPTION* pAddOption)
{
	memcpy(&m_AddPotion, pAddOption, sizeof(m_AddPotion));
	m_Option = pAddOption->bOption;
}

PARTY_ADDOPTION* CParty::GetAddOption()
{
	return &m_AddPotion;
}

void CParty::Process()
{
	if(m_dwRequestPlayerID == 0)
		return;

	if(m_dwRequestProcessTime < gCurTime)
	{
		PARTYMGR->MasterToPartyRequestErr(m_dwRequestPlayerID, eErr_Request_NotState);
		PARTYMGR->MasterToPartyRequestErr(m_Member[0].MemberID, eErr_Request_TimeExcess);
		m_dwRequestPlayerID = 0;
		m_dwRequestProcessTime = 0;
	}
}

void CParty::SetRequestPlayerID(DWORD dwID)
{
	m_dwRequestPlayerID = dwID;
}

DWORD CParty::GetRequestPlayerID()
{
	return m_dwRequestPlayerID;
}

void CParty::StartRequestProcessTime()
{
	m_dwRequestProcessTime = gCurTime + eDICISION_TIME;
}

void CParty::InitRequestTime()
{
	m_dwRequestProcessTime = 0;
}
