#ifndef _BATTLE_VIMUSTREET_H
#define _BATTLE_VIMUSTREET_H

#include "battle.h"
#include "BattleTeam_VimuStreet.h"

#ifndef _MAPSERVER_
#include "../ImageNumber.h"
#endif

#define BATTLE_VIMUSTREET_READYTIME		6000	//실제 클라이언트가 느끼기엔 5초..
#define BATTLE_VIMUSTREET_FIGHTTIME		60000
#define BATTLE_VIMUSTREET_RESULTTIME	10000

enum eVIMUSTREET_TEAM
{
	VIMUTEAM_BLUE,
	VIMUTEAM_RED,
};


struct BATTLE_INFO_VIMUSTREET : public BATTLE_INFO_BASE
{
	DWORD		Character[eBattleTeam_Max];
	VECTOR3		vStgPos;
};

class CBattle_VimuStreet : public CBattle
{	
	BATTLE_INFO_VIMUSTREET m_VimuStreetInfo;

	CBattleTeam_VimuStreet m_Team[2];
	
//	BOOL JudgeOneTeamWinsOtherTeam(int TheTeam,int OtherTeam);
#ifndef _MAPSERVER_
	CImageNumber	m_ImageNumber;

//	cImage			m_ImageReady;
	cImage			m_ImageStart;
	cImage			m_ImageWin;
	cImage			m_ImageLose;
	cImage			m_ImageDraw;
	cImage*			m_pCurShowImage;

	VECTOR2			m_vTitlePos;
	VECTOR2			m_vTitleScale;

////효과 임시
	DWORD	m_dwFadeOutStartTime;
	BOOL	m_bFadeOut;


#else
	/////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 동시에 죽을시 2번 Player는 비무 죽음 처리 안되는 문제 해결 - 이영준
	// 변수가 하나밖에 없어서 두번째 케릭 처리 불가하여
	// 각각의 Player마다 따로 처리 하도록 변경
	BOOL			m_bDieByOp[2];	//상대방에게 죽었나?(몬스터한테 죽으면 FALSE)

#endif
	
public:
	CBattle_VimuStreet();
	virtual ~CBattle_VimuStreet();

	void Initialize(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2);
	// Battle 정보 관련
#ifdef _MAPSERVER_
	virtual void GetBattleInfo(char* pInfo,WORD* size);
//	virtual void GetBattleInfo(BATTLE_INFO_BASE*& pInfo,int& size);
#endif

	// 적,아군 구별
	virtual BOOL IsEnemy(CObject* pOperator,CObject* pTarget);
	virtual BOOL IsFriend(CObject* pOperator,CObject* pTarget);
	
	// event func
	virtual void OnCreate(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2);
	virtual void OnFightStart();
	virtual void OnDestroy();
	virtual void OnTeamMemberAdd( int Team, DWORD MemberID, char* Name );
	virtual BOOL OnTeamMemberDie(int Team,DWORD VictimMemberID,DWORD KillerID);
	virtual BOOL OnTeamMemberDelete(int Team,DWORD MemberID,char* Name);

	virtual void OnTeamObjectCreate(int Team,CBattleObject* pBattleObject);
	virtual void OnTeamObjectDestroy(int Team,CBattleObject* pBattleObject);


	virtual void OnTick();

	// 승패 판정
	virtual BOOL Judge();
	BOOL JudgeOneTeamWinsOtherTeam(int TheTeam,int OtherTeam);
	virtual void Victory(int WinnerTeamNum,int LoserTeamNum);
	virtual void Draw();

	

	

#ifdef _MHCLIENT_
	// Render		(클라이언트만 사용함)
	virtual void Render();
#endif	
};


#endif
