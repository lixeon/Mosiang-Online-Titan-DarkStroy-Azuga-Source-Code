// GuildManager.h: interface for the CGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDMANAGER_H__E299C841_F0B0_413D_9DD7_F63232F5D31A__INCLUDED_)
#define AFX_GUILDMANAGER_H__E299C841_F0B0_413D_9DD7_F63232F5D31A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Guild.h"

class CPlayer;

enum eInviteKind{KindNone, AsMember, AsStudent};

enum eGuildPointUseKind{ForGuildPlusTime, ForGuildMugong,};

enum GuildPointAddKind
{
	eGM_CheatGuildPoint,
	eGuildHuntedMonsterTotalCountConverting,
	eGuildMemberMaxLevelUp,
	eGuildTournamentMarks,
	eGuildPlusTimeUse,
	eGuildSkillUse,
};

struct GUILDPLUSTIME_INFO
{
	DWORD PlusTimeIdx;
	DWORD PlusTimeKind;
	char PlustimeName[MAX_ITEMNAME_LENGTH+1];
	DWORD UsableTimeLen;
	DWORD NeedPoint;
	DWORD AddData;
};

#define GUILDMGR CGuildManager::GetInstance()
class CGuildManager  
{
	GUILDPLUSTIME_INFO* m_pGuildPlustimeInfo;

	CGuild m_Guild;
	BOOL m_bWaiting;
	DWORD m_RequestPlayerID;
	DWORD m_SelectedMemberID;
	BOOL m_bActiveFunc;
	int m_SelectedNum;
	WORD	m_wTempLocation;

	int m_InvitedKind;

	BOOL	m_bWaitingGuildPointUse;

	DWORD	m_dwSOSTargetMapNum;	// magi82 - SOS(070724)
	DWORD	m_dwSOSTargetPos;		// magi82 - SOS(070724)
	DWORD	m_dwSOSDieMemberIdx;	// magi82 - SOS(070725)
	int		m_nSOSChannelIdx;		// magi82 - SOS(070807) 채널은 0부터 시작이므로 초기값을 -1로 함

	MONEYTYPE m_LevelUpInfo[MAX_GUILD_LEVEL];
	void SetWaiting(BOOL val);
	BOOL IsWaiting() { return m_bWaiting; }

	void SetWaitingGPUse(BOOL val);
	BOOL IsWaitingGPUse()	{	return m_bWaitingGuildPointUse;	}
	
	void ShowErrMsg(BYTE bType);
	void MarkChange(MSG_DWORD3 * pInfo);
	void LoadLevelUpInfo();
	MONEYTYPE GetLevelUpMoney(BYTE GuildLevel);
	
	void SetPlayerNickName(CObject* pObject, char* NickName);
	void SetPlayerGuildName(CObject* pObject, DWORD dwGuildIdx, char* GuildName);
public:	
	MAKESINGLETON(CGuildManager);
	CGuildManager();
	virtual ~CGuildManager();
	
	void Init();
	void InitGameIn();
	void Release();
	void NetworkMsgParse(BYTE Protocol,void* pMsg);	
	
	void CreateGuildSyn(char* GuildName, char* Intro);
	void BreakUpGuildSyn();
	void DeleteMemberSyn();

	void CreateGuildResult(DWORD GuildIdx, char* GuildName);
	void BreakUpGuildResult();

	void AddMemberSyn(DWORD PlayerIDX);
	void SetRequestPlayerId(DWORD PlayerIDX);

	void AddMemberAccept();
	void AddMemberDeny();
	
	void SecedeSyn();

	char* GetGuildName();
	BYTE GetLevel();
	char* GetMasterName();
	BYTE GetMemberNum();
	MAPTYPE GetLocation();
	char* GetMemberName(DWORD MemberID);

	void LevelUpSyn();	
	BOOL CanEntryGuild(char* date);

	void SetSelectedMemberNum(int SelectedNum);
	DWORD GetSelectedMemberID();
	
	char* GetSelectedMemberName();
	
	BYTE GetGuildLevel();
	void SetGuildLevel( BYTE bLevel );
	
	void ChangeMemberRank(BYTE Rank);
	
	void GiveNickNameSyn(char* NickName);
	char* GetRankName(BYTE rank);

	void SetActiveFunc(BOOL val);
	BOOL IsActiveFunc();

	void GuildWarehouseInfoSyn(BYTE TabNum);
	void GuildWarehouseLeave(BYTE TabNum);
	
	BOOL CanUseWarehouse();

	BOOL	SendChangeLocation();
	void	SetTempLocation( WORD wMapNum )	{ m_wTempLocation = wMapNum; }
	WORD	GetTempLocation()	{ return m_wTempLocation; }
	BOOL	CheckChangeLocation( int nIdx );
	
	// guildfieldwar
	CGuild*	GetGuild();

	// 06. 03. 문파공지 - 이영준
	void SetGuildNotice(char* str);
	char* GetGuildNotice() { return m_Guild.GetGuildNotice(); }

	void AddStudentSyn(DWORD TargetPlayerID, DWORD TargetPlayerLevel);
	void AddStudentAccept();
	void SetInvitedKind(int Kind)	{ m_InvitedKind = Kind; }
	int	GetInvitedKind()	{	return m_InvitedKind;	}

	//SW060719 문파포인트
	BOOL	LoadGuildPointPlustimeList();
	GUILDPLUSTIME_INFO* GetGuildPointPlustimeList()	{	return m_pGuildPlustimeInfo;	}

	BOOL	CheckGuildPlustimeUse(DWORD GuildPlustimeIdx);
	void	UseGuildPointSyn(int eUseKind, DWORD ListIdx);

	void	GetTotalHuntedMonsterCountFromMapServer();

	void	ShowGuildPointChangedChatMessage(BOOL bAdd, DWORD AddPoint, DWORD Kind, DWORD AddData);

	void	AddGuildPlustime(DWORD GuildPlustimeIdx, DWORD Endtime);
	void	RemoveGuildPlustime(DWORD GuildPlustimeIdx);

	void	AddGuildPlustimeStatusIcon(DWORD GuildPlustimeIdx);
	void	RemoveGuildPlustimeStatusIcon(DWORD GuildPlustimeIdx);
	DWORD	GetGuildPlustimeKindFromIdx(DWORD GuildPlustimeIdx);
	WORD	GetGuildPlustimeBuffIconIdxFromKind(DWORD Kind);

	DWORD	GetGuildPlustimeEndTimeFromKind(DWORD Kind);
	void	GetGuildPlustimeNameFromKind(DWORD Kind, char* pName);
	
	// magi82 - SOS(070724)
	DWORD	GetSOSTargetMapNum()	{	return m_dwSOSTargetMapNum;	}
	void	SetSOSTargetMapNum(DWORD num)	{	m_dwSOSTargetMapNum = num;	}

	DWORD	GetSOSTargetPos()	{	return m_dwSOSTargetPos;	}
	void	SetSOSTargetPos(DWORD pos)	{	m_dwSOSTargetPos = pos;	}
	//////////////////////////////////////////////////////////////////////////

	// magi82 - SOS(070725)
	DWORD	GetSOSDieMemberIdx()	{	return m_dwSOSDieMemberIdx;	}
	void	SetSOSDieMemberIdx(DWORD idx)	{	m_dwSOSDieMemberIdx = idx;	}

	// magi82 - SOS(070807)
	int	GetSOSChannelIdx()	{	return m_nSOSChannelIdx;	}
	void	SetSOSChannelIdx(int idx)	{	m_nSOSChannelIdx = idx;	}

	//SW080515 함수 추가.
	BOOL	IsSameGuild(CPlayer* pPlayerA, CPlayer* pPlayerB);
};

#endif // !defined(AFX_GUILDMANAGER_H__E299C841_F0B0_413D_9DD7_F63232F5D31A__INCLUDED_)
