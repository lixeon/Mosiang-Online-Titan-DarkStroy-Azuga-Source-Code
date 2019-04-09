// Guild.h: interface for the CGuild class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILD_H__61C4631E_8749_49A3_8F03_8998897B6389__INCLUDED_)
#define AFX_GUILD_H__61C4631E_8749_49A3_8F03_8998897B6389__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtrList.h"
#include "GuildWarehouse.h"

#define GUILD_STUDENT_NUM_MAX	25

enum RTChangeRank
{
	eRankPos_1,			//@부문주
	eRankPos_2,			//@장로1
	eRankPos_3,			//@장로2
	eRankPos_Max,
	eRankPos_Err,
};

//enum GuildPlusTimeKind
//{
//	eSuRyun_OneHalf,
//	eMuGong_OneHalf,
//	eExp_OneHalf,
//	eDamageUp_20Pct,
//	eGuildPlusTime_Max,
//};
enum eGuildPointUseKind{ForGuildPlusTime, ForGuildMugong,};

//struct GUILDUSING_PLUSTIME_INFO
//{
//	//int PlusTimeKind;
//	DWORD PlusTimeEndTime;
//};

class CGuild
{
	GUILDINFO m_GuildInfo;
	cPtrList m_MemberList;
	MARKNAMETYPE m_MarkName;
	
	DWORD m_RankMemberIdx[eRankPos_Max]; // m_RankMemberIdx[0] : vice master, m_RankMemberIdx[1],m_RankMemberIdx[2] : seniors  

	//SW060719 문파포인트
	GUILDPOINT_INFO m_GuildPoint;
	DWORD	m_nMemberOnConnectingThisMap;

	//GUILDPLUSTIME_INFO m_GuildUsingPlusTime[eGuildPlusTime_Max];
		
	CGuildWarehouse m_GuildWare;

	// GuilTournament Info
	DWORD		m_GTBattleID;

	BOOL	m_bItemInfoInited;
	BOOL	m_bWaitingItemInfoFromDB;	//SW 단 한번만 Query..(DB응답이 늦을 경우 대비)

	//SW060713 문하생
	DWORD	m_nStudentCount;

public:
	CGuild(GUILDINFO* pInfo, MONEYTYPE GuildMoney);
	virtual ~CGuild();

	DWORD GetIdx();

	void InitGuildItem(ITEMBASE* pInfo);
	
	BOOL IsMaster(DWORD PlayerIDX);
	BOOL IsMember(DWORD MemberIDX);
	BOOL IsViceMaster(DWORD PlayerIDX);
	
	BOOL AddMember(GUILDMEMBERINFO* pMemberInfo);
	BOOL DeleteMember(DWORD PlayerIDX, BYTE bType);
	char* GetGuildName() { return m_GuildInfo.GuildName;	}
	
	void BreakUp();
	GUILDINFO* GetGuildInfo() { return &m_GuildInfo;	}
	void SendMsgToAll(MSGBASE* msg, int size);
	void SendMsgToAllExceptOne(MSGBASE* msg, int size, DWORD CharacterIdx);
	void SendDeleteMsg(DWORD PlayerIDX, BYTE bType);
	void SendAddMsg(GUILDMEMBERINFO* pInfo, DWORD CharacterIDX);
	void SendNote(CPlayer *pPlayer, char* toName, char* note);
	void SendAllNote(CPlayer *pPlayer, char* note);
	void SendUnionNote(CPlayer *pPlayer, char* note);
	
	MARKNAMETYPE GetMarkName();
	void SetMarkName(MARKNAMETYPE name);
	
	DWORD GetMasterIdx();
	int GetMemberNum() { return m_MemberList.GetCount();	}
	cPtrList* GetMemberList()	{	return &m_MemberList;	}

	void GetTotalMember(GUILDMEMBERINFO* rtInfo);
	BYTE GetLevel();
	BYTE GetMaxMemberNum();

	GUILDMEMBERINFO* GetMemberInfo(DWORD MemberIdx);
	int IsVacancy(BYTE ToRank);
	
	BOOL CanAddMember();
	
	BOOL ChangeMemberRank(DWORD MemberIdx, BYTE ToRank);
	BOOL DoChangeMemberRank(DWORD MemberIdx, int Pos, BYTE Rank);

	void ResetRankMemberInfo(DWORD MemberIdx, BYTE Rank);
	CItemSlot* GetSlot();
	void EnterWareHouse(CPlayer* pPlayer,BYTE TabNum);
	void LeaveWareHouse(DWORD PlayerID,BYTE TabNum);

	MAPTYPE GetLocation();
	void	SetLocation( MAPTYPE MapNum )	{ m_GuildInfo.MapNum = MapNum; }
	
	MONEYTYPE GetMaxPurseMoney();
	MONEYTYPE GetMoney();
	MONEYTYPE SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag );
	void SendtoWarehousePlayerAll(MSGBASE* msg, int size);
	void SetMemberLevel(DWORD PlayerIdx, LEVELTYPE PlayerLvl);
	void SetLogInfo(DWORD PlayerIdx, BOOL vals);  // send msg to all member, broadcasting to other mapserver

	void LevelUp();
	void LevelDown();
	
	// GuilTournament Info
	void SetGTBattleID( DWORD BattleID )			{	m_GTBattleID = BattleID;	}
	DWORD GetGTBattleID()							{	return m_GTBattleID;		}

	void	SetGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwGuildUnionMarkIdx );
	void	SetGuildUnionIdx( DWORD dwGuildUnionIdx )	{ m_GuildInfo.UnionIdx = dwGuildUnionIdx; }
	void	SetGuildUnionMarkIdx( DWORD dwMarkIdx );
	DWORD	GetGuildUnionIdx()							{ return m_GuildInfo.UnionIdx; }
	
	BOOL	IsEmptyGuildWare();

	//SW060526 문파창고 아이템 정보저장 시점 변경
	void	SetItemInfoInited( BOOL bInit )	{	m_bItemInfoInited = bInit;	}
	BOOL	IsItemInfoInited() {	return m_bItemInfoInited;	}
	void	SetWaitingItemInfoFromDB( BOOL bVal )	{	m_bWaitingItemInfoFromDB = bVal;	}
	BOOL	IsWaitngItemInfoFromDB()	{	return m_bWaitingItemInfoFromDB;	}

	CGuildWarehouse* GetWarehouse()	{	return &m_GuildWare;	}

	// 06. 03. 문파공지 - 이영준
	void	SetGuildNotice(char* pNotice) { strcpy(m_GuildInfo.GuildNotice, pNotice); }

	//SW060713 문하생
	DWORD	GetStudentNum() { return m_nStudentCount; }
    BOOL	CanAddStudent();

	//SW060719 문파포인트
	void	AddHuntedMonsterCount(int AddCount);
	void	SetHuntedMonsterCount(int SetCount)	{	m_GuildPoint.GuildHuntedMonsterCount = SetCount;	}
	int		GetHuntedMonsterCount()			{	return m_GuildPoint.GuildHuntedMonsterCount;	}
	void	SetHuntedMonsterTotalCountInfo(int SetTotal, DWORD DBProcessTime);
	int		GetHuntedMonsterTotalCount()	{	return m_GuildPoint.GuildHuntedMonsterTotalCount;	}

	void	InitGuildPointInfo(GUILDPOINT_INFO* pGuildPointInfo);
	GUILDPOINT_INFO*	GetGuildPointInfo()	{	return &m_GuildPoint;	}
	void	SetGuildPoint(int TotalPoint);
	int		GetGuildPoint()	{	return m_GuildPoint.GuildPoint;	}

	void	AddConnectingMapMemberCount(int Val);
	int		GetConnectingMapMemberCount()	{	return m_nMemberOnConnectingThisMap;	}

	void	SendGuildPointAddInfoToAll(DWORD AddPoint, DWORD TotalPoint, int eAddKind, DWORD AddData);
	void	SendGuildPointUseInfoToAll(DWORD TotalPoint, int eUseKind, DWORD KindIdx, DWORD EndTime);
	void	AddGuildPlustime(DWORD plustimeIdx, DWORD endTime);
	void	RemoveGuildPlustime(DWORD plustimeIdx);
	//void	UseGuildPoint(CPlayer* pPlayer, DWORD UseKind, DWORD KindIdx);
	BOOL	IsUsingPlustimeIdx(DWORD plustimeIdx);
	BOOL	IsUsingPlustimeKind(DWORD plustimeKind);

	DWORD	GetGuildUsingPlustimeIdxFromKind(DWORD plustimeKind);
	int		GetGuildPlustimeFlg()	{	return m_GuildPoint.GuildPlusTimeflg;	}
	void	CheckPlustimeEnd(DWORD CurTime);

	void	SetZeroMoney();

	void	SendMsgAgentStudentJoin(DWORD studentIdx, char* studentName,char* Intro);	//magi82 - 문하생 가입편리시스템

	//SW070103 문파포인트개선	//문하생 렙업에 따른 포인트 누적. 축하금
	void	SetStudentLvUpCountInfo(DWORD LvUpCount, BOOL bNeedCheck);
	void	SendMsgForMasterChecking();
	void	SendMsgToMaster(MSGBASE* msg, int size);
};

#endif // !defined(AFX_GUILD_H__61C4631E_8749_49A3_8F03_8998897B6389__INCLUDED_)

