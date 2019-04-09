// Guild.cpp: implementation of the CGuild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Guild.h"
#include "Player.h"
#include "UserTable.h"
#include "GuildManager.h"
#include "MapDBMsgParser.h"
#include "NetWork.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuild::CGuild(GUILDINFO* pInfo, MONEYTYPE GuildMoney)
{
	m_GuildInfo.GuildIdx = pInfo->GuildIdx;
	m_GuildInfo.MasterIdx = pInfo->MasterIdx;
	m_GuildInfo.MapNum = pInfo->MapNum;
	SafeStrCpy(m_GuildInfo.GuildName, pInfo->GuildName, MAX_GUILD_NAME+1);
	m_GuildInfo.UnionIdx = pInfo->UnionIdx;
	m_GuildInfo.GuildLevel = pInfo->GuildLevel;

	//SW070103 문파포인트개선
	m_GuildInfo.LvUpCounter = pInfo->LvUpCounter;
	m_GuildInfo.bNeedMasterChecking = pInfo->bNeedMasterChecking;

	SafeStrCpy(m_GuildInfo.MasterName, pInfo->MasterName, MAX_NAME_LENGTH+1);
	SafeStrCpy(m_GuildInfo.GuildNotice, pInfo->GuildNotice, 150);
	m_MarkName = pInfo->MarkName;
	for(int i=0; i<eRankPos_Max; ++i)
		m_RankMemberIdx[i] = 0;

	if( m_GuildInfo.MapNum != g_pServerSystem->GetMapNum() )
		m_GuildWare.Init(0);
	else
		m_GuildWare.Init(GuildMoney);
	m_GTBattleID = 0;

	m_bItemInfoInited = FALSE;
	m_bWaitingItemInfoFromDB = FALSE;

	m_nStudentCount = 0;	//문하생 인원 카운트

	memset(&m_GuildPoint, 0, sizeof(GUILDPOINT_INFO));
	m_nMemberOnConnectingThisMap = 0;
}

CGuild::~CGuild()
{
//KES delete추가 MEM
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;
	while(pos)
	{
		pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		SAFE_DELETE(pInfo);
	}
//-------
	m_MemberList.RemoveAll();
}


BOOL CGuild::IsMaster(DWORD PlayerIDX)
{
	if(m_GuildInfo.MasterIdx != PlayerIDX)
		return FALSE;
	else
		return TRUE;
}

BOOL CGuild::IsViceMaster(DWORD PlayerIDX)
{
	if(m_RankMemberIdx[0] != PlayerIDX)
		return FALSE;
	else
		return TRUE;
}

void CGuild::BreakUp()
{
// set member info clear
	CPlayer* pPlayer = NULL;
	MSGBASE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_BREAKUP_ACK;
	
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		if( pInfo )
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser(pInfo->MemberIdx);
			if(pPlayer)
			{
				pPlayer->SetGuildInfo(0, GUILD_NOTMEMBER, "", 0);
				pPlayer->SetNickName("");
				pPlayer->SendMsg(&msg, sizeof(msg));
				if(GetMarkName())
					GUILDMGR->MarkChange(pPlayer, 0, 0);
				else
					GUILDMGR->SendGuildName(pPlayer, 0, "");
			}
			
		}

		SAFE_DELETE(pInfo);
	}
	m_MemberList.RemoveAll();
}


BOOL CGuild::AddMember(GUILDMEMBERINFO* pMemberInfo)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

		if(pInfo == NULL) return FALSE;
 		if(pInfo->MemberIdx == pMemberInfo->MemberIdx)
 		{
 			return FALSE;
		}
	}
	
	pInfo = new GUILDMEMBERINFO;

	pInfo->InitInfo(pMemberInfo->MemberIdx, pMemberInfo->MemberName, pMemberInfo->Memberlvl, pMemberInfo->Rank);
	m_MemberList.AddTail(pInfo);
	if(pInfo->Rank > GUILD_MEMBER)
	{
		if(pInfo->Rank == GUILD_VICEMASTER)
			m_RankMemberIdx[0] = pInfo->MemberIdx;
		else if(pInfo->Rank == GUILD_SENIOR)
		{
			int pos = IsVacancy(GUILD_SENIOR);
			if((pos <0) || (pos > eRankPos_Max))
			{
				ASSERTMSG(0, "Load GuildMemberInfo - Set Ranked Member");
				return TRUE;
			}
			m_RankMemberIdx[pos] = pInfo->MemberIdx;
		}
	}

	//SW00713 문하생
	if(pInfo->Rank == GUILD_STUDENT)
	{
		++m_nStudentCount;
	}

	SendAddMsg(pInfo, pMemberInfo->MemberIdx);
	return TRUE;
}

BOOL CGuild::DeleteMember(DWORD PlayerIDX, BYTE bType)
{
	CPlayer* pPlayer = NULL;
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;

	while(pos)
	{
		pInfo = (GUILDMEMBERINFO*)m_MemberList.GetAt(pos);
		if(pInfo)
		{
			if(pInfo->MemberIdx == PlayerIDX)
			{
				if(pInfo->Rank == GUILD_STUDENT)
				{
					--m_nStudentCount;
				}

				ResetRankMemberInfo(PlayerIDX, pInfo->Rank);
				SAFE_DELETE(pInfo);
				m_MemberList.RemoveAt(pos);

				return TRUE;
			}
		}
		m_MemberList.GetNext(pos);
	}

	return FALSE;
}

void CGuild::SendDeleteMsg(DWORD PlayerIDX, BYTE bType)
{
	MSG_DWORDBYTE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_DELETEMEMBER_ACK;
	msg.dwData = PlayerIDX;
	msg.bData = bType;
	SendMsgToAll(&msg, sizeof(msg));
}

void CGuild::SendAddMsg(GUILDMEMBERINFO* pInfo, DWORD CharacterIDX)
{
	SEND_GUILD_MEMBER_INFO msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_ADDMEMBER_ACK;
	msg.GuildIdx = GetIdx();
	msg.MemberInfo.InitInfo(pInfo->MemberIdx, pInfo->MemberName, pInfo->Memberlvl, pInfo->Rank, pInfo->bLogged);

	SendMsgToAllExceptOne(&msg, sizeof(msg), CharacterIDX);
}

void CGuild::SendMsgToAll(MSGBASE* msg, int size)
{
	CPlayer* pPlayer = NULL;
	
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();

	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

		if( pInfo )
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser(pInfo->MemberIdx);
			if(pPlayer)
			{
				pPlayer->SendMsg(msg, size);
			}
		}
	}
}

void CGuild::SendMsgToAllExceptOne(MSGBASE* msg, int size, DWORD CharacterIdx)
{
	CPlayer* pPlayer = NULL;

	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		
		if( pInfo )
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser(pInfo->MemberIdx);
			if(pPlayer)
			{
				if(pPlayer->GetID() == CharacterIdx)
					continue;
				pPlayer->SendMsg(msg, size);
			}
		}
	}
}

BOOL CGuild::IsMember(DWORD MemberIDX)
{
	CPlayer* pMember = (CPlayer*)g_pUserTable->FindUser(MemberIDX);
	if(pMember)
	{
		if(pMember->GetGuildIdx() == GetIdx())
			return TRUE;
	}
	else
	{
		PTRLISTPOS pos = m_MemberList.GetHeadPosition();
		while(pos)
		{
			GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

			if(pInfo)
			if(pInfo->MemberIdx == MemberIDX)
				return TRUE;
		}
	}
	return FALSE;
}

DWORD CGuild::GetIdx()
{
	return m_GuildInfo.GuildIdx;
}

void CGuild::GetTotalMember(GUILDMEMBERINFO* pRtInfo)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;
	int i=0;
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

		if(pInfo)
		{
			if(i == MAX_GUILDMEMBER_NUM )
				break;
			pRtInfo[i].InitInfo(pInfo->MemberIdx, pInfo->MemberName, pInfo->Memberlvl, pInfo->Rank, pInfo->bLogged);
			i++;
		}
	}
}


MARKNAMETYPE CGuild::GetMarkName()
{
	return m_MarkName;
}

void CGuild::SetMarkName(MARKNAMETYPE name)
{
	m_MarkName = name;
}

BYTE CGuild::GetLevel()
{
	return m_GuildInfo.GuildLevel;
}

DWORD CGuild::GetMasterIdx()
{
	return m_GuildInfo.MasterIdx;
}

BYTE CGuild::GetMaxMemberNum()
{
	if(( m_GuildInfo.GuildLevel <= 0) || ( m_GuildInfo.GuildLevel > GUILD_5LEVEL))
	{
		char buf[64];
		sprintf(buf, "Wrong Member Num, GuildIdx : %u", m_GuildInfo.GuildIdx);
		ASSERTMSG(0, buf);
		return 0;
	}
#ifdef _CHINA_LOCAL_
	switch( m_GuildInfo.GuildLevel )
	{
	case 1:	return 10;
	case 2:	return 15;
	case 3:	return 20;
	case 4:	return 30;
	case 5:	return 50;
	}
	return 0;
#else
	return 10 + ( (m_GuildInfo.GuildLevel-1) * 10 ); 
#endif
}

BOOL CGuild::CanAddMember()
{	
	if( GetMemberNum() - GetStudentNum() < GetMaxMemberNum() )
		return TRUE;
	else
		return FALSE;
}

BOOL CGuild::ChangeMemberRank(DWORD MemberIdx, BYTE ToRank)
{
	int rt = IsVacancy(ToRank);
	if(rt == eRankPos_Err)
		return FALSE;
	if( DoChangeMemberRank(MemberIdx, rt, ToRank) == FALSE)
		return FALSE;

	MSG_DWORDBYTE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_CHANGERANK_ACK;
	msg.dwData = MemberIdx;
	msg.bData = ToRank;
	SendMsgToAll(&msg, sizeof(MSG_DWORDBYTE));

	return TRUE;
}

int CGuild::IsVacancy(BYTE ToRank)
{
	int end = 0;
	int start = 0;
	switch(ToRank)
	{
	case GUILD_VICEMASTER:
		{
			start = 0; end = 1;
		}
		break;
	case GUILD_SENIOR:
		{
			start = 1; end = 3;
		}
		break;
	case GUILD_MEMBER:
	case GUILD_STUDENT:
		return eRankPos_Max;
		
	default:
		ASSERTMSG(0, "IsVancancy Err.");
		return eRankPos_Err;
	}

	BOOL rt = eRankPos_Err;
	for(int i=start; i<end; ++i)
	{
		if( m_RankMemberIdx[i] == 0 )
		{
			return i;
		}
	}
	return rt;
}

GUILDMEMBERINFO* CGuild::GetMemberInfo(DWORD MemberIdx)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;

	while(pos)
	{
		pInfo = (GUILDMEMBERINFO*)m_MemberList.GetAt(pos);
		if(pInfo)
		{
			if(pInfo->MemberIdx == MemberIdx)
			{
				return pInfo;
			}
		}
		m_MemberList.GetNext(pos);
	}
	char buf[64];
	sprintf(buf, "ERR, NO MemberInfo. GuildIdx: %d, MemberIdx: %d", GetIdx(), MemberIdx);
	ASSERTMSG(0, buf);
	return NULL;
}

BOOL CGuild::DoChangeMemberRank(DWORD MemberIdx, int Pos, BYTE Rank)
{
	GUILDMEMBERINFO* pMemberInfo = GetMemberInfo(MemberIdx);
	if(pMemberInfo == 0)
	{
		ASSERTMSG(0, "Change Rank - No Member Data");
		return FALSE;
	}
	if((Pos <0) || (Pos >eRankPos_Max))
	{
		ASSERTMSG(0, "change rank pos boundary error");
		return FALSE;
	}
	ResetRankMemberInfo(MemberIdx, pMemberInfo->Rank);

	if(pMemberInfo->Rank == GUILD_STUDENT && Rank != GUILD_STUDENT)
	{
		--m_nStudentCount;
	}
	//없는경우이다.
	//else if(pMemberInfo->Rank != GUILD_STUDENT && Rank == GUILD_STUDENT)
	//{
	//	++m_nStudentCount;
	//}
	pMemberInfo->Rank = Rank;
	if(Pos == eRankPos_Max)
		return TRUE;
	m_RankMemberIdx[Pos] = MemberIdx;
	return TRUE;
}

void CGuild::ResetRankMemberInfo(DWORD MemberIdx, BYTE Rank)
{
	switch(Rank)
	{
	case GUILD_VICEMASTER:
		m_RankMemberIdx[0] = 0; break;
	case GUILD_SENIOR:
		{
			if(m_RankMemberIdx[1] == MemberIdx)
				m_RankMemberIdx[1] = 0;
			else if(m_RankMemberIdx[2] == MemberIdx)
				m_RankMemberIdx[2] = 0;
			else 
				ASSERTMSG(0, "ResetRankMemberInfo() - No Rank");
		}
		break;
	}
}

void CGuild::InitGuildItem(ITEMBASE* pInfo)
{
	m_GuildWare.InitGuildItem(pInfo);
}

CItemSlot* CGuild::GetSlot()
{
	return &m_GuildWare;
}

void CGuild::EnterWareHouse(CPlayer* pPlayer,BYTE TabNum)
{
	m_GuildWare.EnterWareHouse(pPlayer, TabNum);
}

void CGuild::LeaveWareHouse(DWORD PlayerID,BYTE TabNum)
{
	m_GuildWare.LeaveWareHouse(PlayerID, TabNum);
}

MAPTYPE CGuild::GetLocation()
{
	return m_GuildInfo.MapNum;
}

MONEYTYPE CGuild::GetMaxPurseMoney()
{
	return m_GuildWare.GetMaxMoney();
}

MONEYTYPE CGuild::GetMoney()
{
	return m_GuildWare.GetMoney();
}

MONEYTYPE CGuild::SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag )
{
	return m_GuildWare.SetMoney(ChangeValue, bOper, MsgFlag);
}

void CGuild::SetZeroMoney()
{
	CPurse* purse = m_GuildWare.GetPurse();
	if( purse )
		purse->SetZeroMoney();
}

void CGuild::SendtoWarehousePlayerAll(MSGBASE* msg, int size)
{
	m_GuildWare.SendtoWarehousePlayerAll(msg, size);
}

void CGuild::SetMemberLevel(DWORD PlayerIdx, LEVELTYPE PlayerLvl)
{
	GUILDMEMBERINFO* pMemberInfo = GetMemberInfo(PlayerIdx);
	if( pMemberInfo == NULL )
	{
		return;
	}

	pMemberInfo->Memberlvl = PlayerLvl;
	MSG_DWORD2 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_MEMBERLEVEL_NOTIFY;
	msg.dwData1 = PlayerIdx;
	msg.dwData2 = PlayerLvl;
	SendMsgToAll(&msg, sizeof(msg));
}

void CGuild::SetLogInfo(DWORD PlayerIdx, BOOL vals)
{
	GUILDMEMBERINFO* pMemberInfo = GetMemberInfo(PlayerIdx);
	if( pMemberInfo == NULL )
	{
		return;
	}
	pMemberInfo->bLogged = vals;

	SEND_GUILDMEMBER_LOGININFO msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_LOGINFO;
	msg.MemberIdx = PlayerIdx;
	msg.bLogIn = vals;
	SendMsgToAllExceptOne(&msg, sizeof(msg), PlayerIdx);
}

void CGuild::LevelUp()
{
	if(m_GuildInfo.GuildLevel >= MAX_GUILD_LEVEL)
		return;
	m_GuildInfo.GuildLevel += 1;

	MSG_BYTE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_LEVELUP_ACK;
	msg.bData = m_GuildInfo.GuildLevel;
	SendMsgToAll(&msg, sizeof(msg));

	LogGuild(0, GetIdx(), eGuildLog_GuildLevelUp, GetLevel());
}

void CGuild::LevelDown()
{
	if(m_GuildInfo.GuildLevel <= GUILD_1LEVEL)
		return;
	m_GuildInfo.GuildLevel -= 1;

	LogGuild(0, GetIdx(), eGuildLog_GuildLevelDown, GetLevel());
}

void CGuild::SetGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwGuildUnionMarkIdx )
{
	m_GuildInfo.UnionIdx = dwGuildUnionIdx;

	CPlayer* pPlayer = NULL;
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();

	while( pos )
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		if( pInfo )
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser( pInfo->MemberIdx );
			if( pPlayer )
				pPlayer->SetGuildUnionInfo( dwGuildUnionIdx, pGuildUnionName, dwGuildUnionMarkIdx );
		}
	}
}

void CGuild::SetGuildUnionMarkIdx( DWORD dwMarkIdx )
{
	CPlayer* pPlayer = NULL;
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();

	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

		if( pInfo )
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser( pInfo->MemberIdx );
			if( pPlayer )
				pPlayer->SetGuildUnionMarkIdx( dwMarkIdx );
		}
	}
}

BOOL CGuild::IsEmptyGuildWare()
{
	return m_GuildWare.IsEmptyAndMoney();
}

void CGuild::SendAllNote(CPlayer *pPlayer, char* note)
{
	CPlayer* pReceiver = NULL;

	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		
		if( pInfo )
		{
			SendNote(pPlayer, pInfo->MemberName, note);
		}
	}
}

void CGuild::SendUnionNote(CPlayer *pPlayer, char* note)
{
	SendNote(pPlayer, m_GuildInfo.MasterName, note);

	if(m_RankMemberIdx[0])
	{
		GUILDMEMBERINFO* pInfo = GetMemberInfo( m_RankMemberIdx[0] );
		SendNote(pPlayer, pInfo->MemberName, note);
	}
}

void CGuild::SendNote(CPlayer *pPlayer, char* toName, char* note)
{
	MSG_FRIEND_SEND_NOTE msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_SENDNOTE_SYN;
	msg.FromId = pPlayer->GetID();
	
	SafeStrCpy(msg.FromName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
	SafeStrCpy(msg.ToName, toName, MAX_NAME_LENGTH+1);
	SafeStrCpy(msg.Note, note, MAX_NOTE_LENGTH+1);	

	g_Network.Broadcast2AgentServer((char*)&msg, msg.GetMsgLength());
}

BOOL CGuild::CanAddStudent()
{
//#define GUILD_STUDENT_NUM_MAX	25
	return ( GetStudentNum() < GUILD_STUDENT_NUM_MAX );
}

void CGuild::AddHuntedMonsterCount( int AddCount )
{
	m_GuildPoint.GuildHuntedMonsterCount += AddCount;
}

void CGuild::SetHuntedMonsterTotalCountInfo( int SetTotal, DWORD DBProcessTime )
{
	m_GuildPoint.GuildHuntedMonsterTotalCount = SetTotal;
	m_GuildPoint.DBProcessTime = DBProcessTime;
}

void CGuild::InitGuildPointInfo( GUILDPOINT_INFO* pGuildPointInfo )
{
	//memcpy(&m_GuildPoint, pGuildPointInfo, sizeof(GUILDPOINT_INFO));

	m_GuildPoint.GuildPoint = pGuildPointInfo->GuildPoint;
	m_GuildPoint.GuildHuntedMonsterCount = 0;
	m_GuildPoint.GuildHuntedMonsterTotalCount = pGuildPointInfo->GuildHuntedMonsterTotalCount;
	m_GuildPoint.DBProcessTime = GetCurTime();
	
	//for( int plustimeKind = 0; plustimeKind < eGPT_Kind_Max; ++plustimeKind )
	//{
	//	AddGuildPlustime(plustimeKind, pGuildPointInfo->GuildUsingPlusTimeInfo[plustimeKind].PlusTimeEndTime);
	//}

}

void CGuild::SetGuildPoint( int TotalPoint )
{
	m_GuildPoint.GuildPoint = TotalPoint;

	//문파원 전파
	MSG_DWORD msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_POINT;
	//msg.dwData1 = GuildIdx;
	msg.dwData = TotalPoint;

	SendMsgToAll(&msg, sizeof(MSG_DWORD));
}

void CGuild::AddConnectingMapMemberCount( int Val )
{
	m_nMemberOnConnectingThisMap += Val;

	if(m_nMemberOnConnectingThisMap < 0)
	{
		ASSERT(0);
	}
}

void CGuild::SendGuildPointAddInfoToAll( DWORD AddPoint, DWORD TotalPoint, int eAddKind, DWORD AddData )
{
	//맵 전파
	MSG_DWORD4 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_POINT_ADD;
	msg.dwData1 = AddPoint;
	msg.dwData2 = TotalPoint;
	msg.dwData3 = eAddKind;
	msg.dwData4 = AddData;
	//pAdditionalData
	//문원렙업 -> 레벨
	//토너먼트 -> 순위
	//몬스터카운트 -> 총카운트

	SendMsgToAll(&msg, sizeof(MSG_DWORD4));
}

void CGuild::SendGuildPointUseInfoToAll( DWORD TotalPoint, int eUseKind, DWORD KindIdx, DWORD EndTime )
{
	//문원 전파
	MSG_DWORD4 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_POINT_USEINFO;
	msg.dwData1 = TotalPoint;
	msg.dwData2 = eUseKind;
	msg.dwData3 = KindIdx;
	msg.dwData4 = EndTime;

	SendMsgToAll(&msg, sizeof(MSG_DWORD4));
}

void CGuild::AddGuildPlustime( DWORD plustimeIdx, DWORD endTime )
{
	//중복 체크
	if( TRUE == IsUsingPlustimeIdx(plustimeIdx) )
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! GuildPlustime: Dupl_UsingPlustime GuildIdx: %d PlustimeIdx :%d", GetIdx(), plustimeIdx );
		ASSERTMSG(0, buf);
	}

	//문파원전파
	MSG_DWORD2 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_PLUSTIME_ADD;
	msg.dwData1 = plustimeIdx;
	msg.dwData2 = endTime;

	SendMsgToAll(&msg, sizeof(MSG_DWORD2));


	DWORD Kind = GUILDMGR->GetGuildPlustimeKindFromIdx(plustimeIdx);

	if( Kind < 0 || plustimeIdx < 1)
	{
		ASSERT(0);
		return;
	}

	m_GuildPoint.GuildPlusTimeflg |= (1<<Kind);
	m_GuildPoint.GuildUsingPlusTimeInfo[Kind].PlusTimeIdx = plustimeIdx;
	m_GuildPoint.GuildUsingPlusTimeInfo[Kind].PlusTimeEndTime = endTime;
}

void CGuild::RemoveGuildPlustime( DWORD plustimeIdx )
{
	if( FALSE == IsUsingPlustimeIdx(plustimeIdx) )
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! GuildPlustime: End_NotUsingPlustime GuildIdx: %d PlustimeIdx: %d", GetIdx(), plustimeIdx );
		ASSERTMSG(0, buf);
		return;
	}

	//문파원전파
	MSG_DWORD msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_PLUSTIME_END;
	msg.dwData = plustimeIdx;

	SendMsgToAll(&msg, sizeof(MSG_DWORD));


	DWORD Kind = GUILDMGR->GetGuildPlustimeKindFromIdx(plustimeIdx);

	//if(m_GuildPoint.GuildPlusTimeflg & (1<<Kind))
	//	m_GuildPoint.GuildPlusTimeflg ^= (1<<Kind);
	m_GuildPoint.GuildPlusTimeflg &= ~(1<<Kind);

	m_GuildPoint.GuildUsingPlusTimeInfo[Kind].PlusTimeIdx = 0;
	m_GuildPoint.GuildUsingPlusTimeInfo[Kind].PlusTimeEndTime = 0;
}

BOOL CGuild::IsUsingPlustimeIdx( DWORD plustimeIdx )
{
	DWORD Kind = GUILDMGR->GetGuildPlustimeKindFromIdx(plustimeIdx);
	return ( m_GuildPoint.GuildPlusTimeflg & (1<<Kind) );
}

BOOL CGuild::IsUsingPlustimeKind( DWORD plustimeKind )
{
	return ( m_GuildPoint.GuildPlusTimeflg & (1<<plustimeKind) );
}

DWORD CGuild::GetGuildUsingPlustimeIdxFromKind( DWORD plustimeKind )
{
	return m_GuildPoint.GuildUsingPlusTimeInfo[plustimeKind].PlusTimeIdx;
}

void CGuild::CheckPlustimeEnd( DWORD CurTime )
{
	for( int i = 0; i < eGPT_Kind_Max; ++i )
	{
		if(m_GuildPoint.GuildUsingPlusTimeInfo[i].PlusTimeIdx)
		if( m_GuildPoint.GuildUsingPlusTimeInfo[i].PlusTimeEndTime < CurTime )
		{
			//DB 정보 초기화
			GuildPlustimeEnd( m_GuildInfo.GuildIdx, m_GuildPoint.GuildUsingPlusTimeInfo[i].PlusTimeIdx );
		}
	}
}

//magi82 - 문하생 가입편리시스템
void CGuild::SendMsgAgentStudentJoin(DWORD studentIdx, char* studentName, char* Intro)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

		if( pInfo )
		{
			if(pInfo->Rank == GUILD_SENIOR || pInfo->Rank == GUILD_VICEMASTER)
			{
				MSG_MUNHA_NAME2_DWORD_NOTE Sendmsg;
				SetProtocol( &Sendmsg, MP_GUILD, MP_GUILD_MUNPA_JOIN_SYN );
				Sendmsg.dwObjectID = studentIdx;
				Sendmsg.dwData = pInfo->MemberIdx;
				SafeStrCpy( Sendmsg.Name1, studentName, MAX_NAME_LENGTH+1 );
				SafeStrCpy( Sendmsg.Name2, pInfo->MemberName, MAX_NAME_LENGTH+1 );
				SafeStrCpy( Sendmsg.Intro, Intro, MAX_CHAT_LENGTH+1 );
				g_Network.Send2AgentServer((char*)&Sendmsg, sizeof(Sendmsg));
			}
		}
	}
}

void CGuild::SetStudentLvUpCountInfo(DWORD LvUpCount, BOOL bNeedCheck)
{//다른맵으로부터
	m_GuildInfo.LvUpCounter = (BYTE)LvUpCount;

	//체크변수가 유효값으로 바뀌었으면 접속중인 문주를 찾는다.
	if( 0 == m_GuildInfo.bNeedMasterChecking && bNeedCheck )
	{
		SendMsgForMasterChecking();
	}

	m_GuildInfo.bNeedMasterChecking = bNeedCheck;
}

void CGuild::SendMsgForMasterChecking()
{
	if(FALSE == m_GuildInfo.bNeedMasterChecking)
		return;

	MSGBASE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_CELEBRATIONMONEY_MASTERCHECKING;
	
	SendMsgToMaster(&msg, sizeof(msg));
}

void CGuild::SendMsgToMaster( MSGBASE* msg, int size )
{
	DWORD GuildMasterID = m_GuildInfo.MasterIdx;

	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(GuildMasterID);

	if(pMaster)
	{
		pMaster->SendMsg(msg,size);
	}
}
