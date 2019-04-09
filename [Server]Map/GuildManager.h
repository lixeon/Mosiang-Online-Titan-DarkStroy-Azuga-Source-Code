// GuildManager.h: interface for the CGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDMANAGER_H__FE162F77_2547_40A8_8C25_54C49BC0ED05__INCLUDED_)
#define AFX_GUILDMANAGER_H__FE162F77_2547_40A8_8C25_54C49BC0ED05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GUILDMGR CGuildManager::GetInstance()
#define OnErr(pPlayer, Protocol, errstate) \
if(errstate) { SendNackMsg(pPlayer, Protocol, errstate); return;}

#define GUILD_STUDENT_MAX_LEVEL	40

class CGuild;
class CGuildMark;
class CItemSlot;

//SW060719 문파포인트
enum GuildPointAddKind
{
	eGM_CheatGuildPoint,
	eGuildHuntedMonsterTotalCountConverting,
	eGuildMemberMaxLevelUp,
	eGuildTournamentMarks,
	eGuildPlusTimeUse,
	eGuildSkillUse,
	eGuildFortWar,
};

enum eSectionForGuildHuntedMonsterCount
{
	eHuntedMonsterCount0_999,
	eHuntedMonsterCount1000_1999,
	eHuntedMonsterCount2000_2999,
	eHuntedMonsterCount3000_3999,
	eHuntedMonsterCount4000_4999,
	eSectionForGuildHuntedMonsterCount_MAX,
};

enum eGuildPointPerGuildTournamentResult
{
	eGDT_32,
	eGDT_16,
	eGDT_8,
	eGDT_4,
	eGDT_3,
	eGDT_2,
	eGDT_1,
	eGDT_MAX,
};

struct GUILDPOINT_RULE
{
	int NOMAL_MONSTER_HUNTCOUNT;
	int FIELDBOSS_MONSTER_HUNTCOUNT;
	int BOSS_MONSTER_HUNTCOUNT;
	int QUEST_MONSTER_HUNTCOUNT;

	int GUILDPOINT_SECTION[eSectionForGuildHuntedMonsterCount_MAX];
	int GUILDPOINT_GT_RESULT[eGDT_MAX];

	int HUNTCOUNT_LV_1_19;
	int HUNTCOUNT_LV_20_29;
	int HUNTCOUNT_LV_30_39;

	int LEVELUPTIME_FOR_GIFTMONEY;
	int GUILD_GIFTMONEY;
};


//=============GuildPlusTime=================
//enum GuildPlusTimeKind
//{
//	eSuRyun_OneHalf,
//	eMuGong_OneHalf,
//	eExp_OneHalf,
//	eDamageUp_20Pct,
//	eGuildPlusTime_Max,
//};

struct GUILDPLUSTIME_INFO
{
	DWORD PlusTimeIdx;
	DWORD PlusTimeKind;
	char PlustimeName[MAX_ITEMNAME_LENGTH+1];
	DWORD UsableTimeLen;
	DWORD NeedPoint;
	DWORD AddData;
};

//===============================================


class CGuildManager  
{
	//SW060719 문파포인트
	static int	m_gPlustimeKindNum;
	GUILDPOINT_RULE	m_GuildPointRule;
	DWORD	m_CHECKTIMELEN_GuildHuntedMonsterCounter;	//맵 누적 몬스터사냥카운트 DB갱신 주기
	DWORD	m_dwLastDBUpdateTime;	//이전 갱신 시각
	WORD	m_wCurDateCheckForConverCount;	//몬스터 사냥 포인트 정산을 위한 날짜 체크.

	DWORD	m_CheckTimeForGuildPlustimeEnd;

	//GUILDPLUSTIME_INFO m_pGuildPlustimeInfo[eGuildPlusTime_Max];
	GUILDPLUSTIME_INFO* m_pGuildPlustimeInfo;

	cPtrList	m_GuildPlustimeUsingGuildList;

	//magi82 - 문하생 가입편리시스템 /////////////////////////////////////////
	cPtrList m_GuildTraineeList;
	cPtrList m_GuildTraineeGuildList;
	cPtrList m_GuildTraineeGuildIntroInfoList;
	//////////////////////////////////////////////////////////////////////////


	CYHHashTable<CGuild> m_GuildRoomHashTable;
	CYHHashTable<CGuildMark> m_GuildMarkTable;
	CYHHashTable<ITEM_OPTION_INFO> m_ItemOptionList;
	CYHHashTable<ITEM_RARE_OPTION_INFO> m_ItemRareOptionList;

	MONEYTYPE m_LevelUpInfo[MAX_GUILD_STEP];
	CGuildManager();
	BOOL IsVaildGuildName(char* GuildName);

	BYTE DoBreakUp(DWORD GuildIdx);
	void RegistMarkSyn(DWORD GuildIdx, char* ImgData, DWORD PlayerIdx);
	void RequestMark(CPlayer* pPlayer, MARKNAMETYPE MarkName);
	int convertCharToInt(char c);
	BYTE HexToByte(char* pStr);
	void LoadLevelUpInfo();
	MONEYTYPE GetLevelUpMoney(BYTE GuildLevel);
	BOOL CanEntryGuild(char* date);
public:
	GETINSTANCE(CGuildManager);	
	virtual ~CGuildManager();

	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	
	CGuild* RegistGuild(GUILDINFO* pGuildInfo, MONEYTYPE GuildMoney);
	void RegistGuildItem(DWORD GuildIdx, ITEMBASE* Guilditem);
	void LoadMembers(GUILDMEMBERINFO_ID* pInfo);
	void LoadMark(MARKNAMETYPE MarkName, char* pImgData);
	
	void UserLogIn(CPlayer* pPlayer);
	void UserLogOut(CPlayer* pPlayer);

	void CreateGuildSyn(CPlayer* pPlayer, char* GuildName, char* Intro);
	void CreateGuildResult(LEVELTYPE MasterLvl, GUILDINFO* pGuildInfo);
	
	void BreakUpGuildSyn(CPlayer* pMaster);
	void BreakUpGuildResult(CPlayer* pMaster, DWORD GuildIdx, char* EntryDate);

	void DeleteMemberSyn(CPlayer* pMaster, DWORD MemberIDX);
	BYTE DoDeleteMember(DWORD GuildIDX, DWORD MemberIDX, BYTE bType, char* EntryDay);
	void DeleteMemberResult(DWORD GuildIDX, DWORD MemberIDX, char* EntryDay);
	
	void AddMemberSyn(CPlayer* pMaster, DWORD TargetIdx);
	void AddMemberResult(CPlayer* pTarget, DWORD MasterIdx);
	void DeleteMunhaJoinInfo(DWORD TargetIdx);	// magi82 - 문하생관련(070125)
	
	CGuild* GetGuild(DWORD GuildIdx);
	CGuild* GetGuildFromMasterIdx( DWORD dwMasterIdx );
	CGuildMark* GetMark(MARKNAMETYPE MarkName);
	CGuild* GetGuildIndex( char* GuildName );

	void SendNackMsg(CPlayer* pPlayer, BYTE Protocol, BYTE errstate);
	
	void SecedeSyn(CPlayer* pPlayer);
	void SecedeResult(DWORD GuildIdx, DWORD MemberIDX, char* EntryDay);
	
	void SendChatMsg(DWORD GuildIdx, DWORD PlayerIdx, char* pMsg);
	
	void RegistMarkResult(CPlayer* pPlayer, DWORD GuildIdx, MARKNAMETYPE MarkName, char * pImgData);
	void DoRegistMark(CGuild* pGuild, MARKNAMETYPE MarkName, char* imgData);

	void DeleteMark(MARKNAMETYPE MarkName);
	void MarkChange(CPlayer* pPlayer, DWORD GuildIdx, MARKNAMETYPE MarkName);

	void LevelUpSyn(CPlayer* pPlayer);
	
	void SendGuildEntryDate(CPlayer* pPlayer, char* day);
	
	void ChangeMemberRank(CPlayer* pMaster, DWORD MemberIdx, BYTE Rank);

	void GiveNickNameSyn(CPlayer* pMaster, DWORD TargetId, char* NickName);
	
	CItemSlot* GetSlot(DWORD GuildIdx);
	void GuildWarehouseInfo(CPlayer* pPlayer, BYTE TabNum);
	void LeaveWareHouse(CPlayer* pPlayer, BYTE TabNum);

	//SW050920 Rare
	void AddGuildItemRareOption(ITEM_RARE_OPTION_INFO* pRareInfo);
	void RemoveGuildItemRareOption(DWORD dwRareIdx);
	ITEM_RARE_OPTION_INFO* GetItemRareOption(DWORD dwRareIdx);

	void AddGuildItemOption(ITEM_OPTION_INFO * pInfo);
	void RemoveGuildItemOption(DWORD dwOptionIdx);
	ITEM_OPTION_INFO * GetItemOption(DWORD dwOptionIdx);
	
	void PutInMoneyGuild(CPlayer* pPlayer, DWORD Money);
	void PutOutMoneyGuild(CPlayer* pPlayer, DWORD Money);
	
	void MemberLevelUp(DWORD GuildIdx, DWORD PlayerIdx, LEVELTYPE PlayerLvl);
	
	void SendGuildName(CPlayer* pPlayer, DWORD dwGuildIdx, char * GuildName); // ban, secede, breakup 시는 안보냄. 길드 이름이 없으므로 클라이언트에서 markchange 0 에서 처리
	
	void SetLogInfo(CGuild* pGuild, DWORD PlayerIdx, BOOL vals);
	
	BOOL	CheckHack( CPlayer* pPlayer ); 
	
	void	MsgChangeLocationSyn( void* pMsg );
	void	MsgChangeLocationNotifyToMap( void* pMsg );
	void	MsgGuildNoteSyn( void* pMsg );
	// 06. 03. 문파공지 - 이영준
	void	MsgGuildNoticeSyn( void* pMsg );
	void	MsgGuildNoticeNotify( void* pMsg );
	void	RegistGuildNotice(DWORD GuildIdx, char* Notice);

	//SW060526 문파창고 아이템 정보저장 시점 변경
	void	SetGuildItemInfoInited(DWORD dwGuildIdx, BOOL bInit);	//초기화 상태 세팅
	void	SendGuildItemInfoToAllWarehouseMember(DWORD dwGuildIdx);	//초기정보 대기중인 문파원들에게 아이템정보 보낸다.

	//SW060713 문하생
	void	AddStudentSyn(CPlayer* pMaster, DWORD TargetPlayerID);
	BOOL	CanEntryGuildForStudent(char* date);
	void	AddStudentResult(CPlayer* pTarget, DWORD MasterID);
	//magi82 - 문하생 가입편리시스템 /////////////////////////////////////////
	void	AddStudentResult(DWORD TargetID, DWORD MasterID);
	//////////////////////////////////////////////////////////////////////////

	//SW060719 문파포인트
	BOOL	LoadGuildPointRule();
	GUILDPLUSTIME_INFO* GetGuildPointPlustimeList()	{	return m_pGuildPlustimeInfo;	}

	void	AddGuildHuntedMonsterCount(CPlayer* pPlayer, CObject* pHuntedMonster);
	void	SendGuildHuntedMonsterTotalCountToMapServerExceptOneself(DWORD GuildIdx, int nTotalCount/*, DWORD DBProcessTime*/);
	void	SetGuildHuntedMonsterTotalCountInfo(DWORD GuildIdx, int nTotalCount, DWORD DBProcessTime);
	void	GuildConvertHuntedMonsterTotalCountToGuildPoint(DWORD GuildIdx, int nTotalCount);
	int		GuildPointFromHuntedMonsterTotalCount(int nTotalCount);
	void	GuildMemberLevelUpToGuildPoint(CPlayer* pPlayer);
	int		GuildPointFromGuildMemberLevelUp(CPlayer* pPlayer);
	void	GuildTournamentResultToGuildPoint(DWORD GuildIdx, WORD wRank);
	int		GuildPointFromGuildTournamentRank(WORD wRank);
	void	GuildPointFromFortWar( DWORD dwGuildIdx, DWORD dwGuildPoint );

	BOOL	LoadGuildPointPlustimeList();

	void	InitGuildPointInfo(DWORD GuildIdx, GUILDPOINT_INFO* pGuildPointInfo);
	void	InitGuildPlustimeInfo(DWORD GuildIdx, DWORD GuildUsePlustimeIdx, DWORD GuildIUsePlustimeEndtime);

	void	UseGuildPoint(CPlayer* pPlayer, DWORD UseKind, DWORD ListIdx);

	void	AddGuildPointFromDBReturn(DWORD GuildIdx, DWORD AddPoint, DWORD TotalPoint, int eAddKind, DWORD AdditionalData);
	void	UseGuildPointFromDBReturn(DWORD GuildIdx, DWORD UsePoint, DWORD TotalPoint, int eUseKind, DWORD KindIdx, DWORD Endtime);
	void	SetGuildPoint(DWORD GuildIdx, DWORD TotalPoint);

	void	AddGuildPlustimeUsingGuildList(CGuild* pGuild);
	void	RemoveGuildPlustimeUsingGuildList(CGuild* pGuild);
	void	EndGuildPlustimeDBReturn(DWORD GuildIdx, DWORD PlustimeIdx);
	void	EndGuildPlustime(DWORD GuildIdx, DWORD PlustimeIdx);

	DWORD	GetGuildPlustimeKindFromIdx(DWORD GuildPlustimeIdx);

	BOOL	GetGuildPlustimeRt(DWORD GuildIdx, int PlustimeKind, void* pData);

	void	GUILDMGR_Process();
	void	GUILDMGR_5MinProcess();

	void	GuildHuntedMonsterCountDBUpdate();
	void	CheckDateForGuildHuntedMonsterCountConvertToGuildPoint();
	void	CheckGuildPlustimeEnd();

	BOOL	CheckValidPlustimeKind(GUILDPOINT_INFO* pGPInfo, DWORD GuildIdx, DWORD plusTimeKind);
	//magi82 - 문하생 가입편리시스템 /////////////////////////////////////////
	void AddGuildTraineeInfo(GUILD_TRAINEE_LIST* temp);
	void AddGuildTraineeGuildInfo(GUILD_TRAINEE_GUILDLIST* temp);
	void AddGuildTraineeGuildIntroInfo(GUILD_INTRO_INFO* temp);
	//////////////////////////////////////////////////////////////////////////

	//SW070103 문파포인트개선	//문하생 렙업에 따른 포인트 누적. 축하금
	//누적포인트 DB업데이트
	void GuildStudentLvUpPointDBUpdate(CPlayer* pPlayer);
	void SetGuildStudentLvUpCount(DWORD GuildIdx, DWORD GuildStudentLvUpCount, BOOL bMasterChecking);

	void GiveGuildCerebrationMoney(DWORD GuildIdx);
	void PutInGuildCerabrationMoney(CGuild* pGuild, DWORD Money);

	cPtrList * GetGuildTraineeList()	{ return &m_GuildTraineeList; };

	//SW080515 함수 추가.
	BOOL	IsSameGuild(CPlayer* pPlayerA, CPlayer* pPlayerB);
};

#endif // !defined(AFX_GUILDMANAGER_H__FE162F77_2547_40A8_8C25_54C49BC0ED05__INCLUDED_)
