// cMonsterSpeechManager.h: interface for the cMonsterSpeechManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMONSTERSPEECHMANAGER_H__980D146A_30B9_4F0C_9A69_4E74D859217A__INCLUDED_)
#define AFX_CMONSTERSPEECHMANAGER_H__980D146A_30B9_4F0C_9A69_4E74D859217A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "CommonGameDefine.h"
#include "MHFile.h"

#define MON_SPEECHMGR USINGTON(cMonsterSpeechManager)

//#define MONSTERLIST_MAXNUM	190		몬스터리스트 로드 에 있는 것으로 쓰자.
#define MAX_SPEECHLIST_NUM	50

enum Mon_SpeechState{
		eMon_Speech_ForeAtk,	//선공격 시작 대사
		eMon_Speech_ForgivePursuit,	//추적 포기 대사
		eMon_Speech_Help,	//도움 요청 대사
		eMon_Speech_AboutHelp,	//도움 요청에 대한 반응 대사
		eMon_Speech_Death,	//죽음 대사
		eMon_Speech_Stand,	//대기 대사
		eMon_Speech_WalkAround,	//이동 대사
		eMon_Speech_KeepStand,
		eMon_Speech_KeepWalkAround,
//		eMon_Speech_Rest,	//휴식 대사 사용안함.
		eMon_Speech_MAX,
};

//enum Monster_Type{ eMonType_General, eMonType_M_Boss, eMonType_H_Boss };
enum Monster_SpeechTpye{eMSpch_Balloon, eMSpch_BallonChat, eMSpch_SmallShout,};	//MonsterType

struct MonSpeechList
{
	MonSpeechList() : Prob(0),SpeechIdx(0){}
	DWORD	Prob;
	DWORD	SpeechIdx;
};

struct MonSpeechInfo
{
	MonSpeechInfo() : SpeechType(0), SpeechIndex(0){}
	DWORD SpeechType;
	DWORD SpeechIndex;
};

struct MonSpeech
{
	MonSpeech() {memset(this,0,sizeof(MonSpeech));}
	DWORD	MonsterIdx;
	DWORD	MonsterType;
	DWORD	dwProbNoSpeech[eMon_Speech_MAX];
	DWORD	dwSpeechTotalRate[eMon_Speech_MAX];
	DWORD	dwSpchListCount[eMon_Speech_MAX];
	MonSpeechList*	pSpchList[eMon_Speech_MAX];
};

class cMonsterSpeechManager  
{
	CYHHashTable<MonSpeech>		m_MonSpeechInfoTable;	//몬스터 대사 정보 테이블
	MonSpeechInfo				m_SpeechInfo;

public:
	cMonsterSpeechManager();
	virtual ~cMonsterSpeechManager();

	void Init();
	void Release();
	bool LoadMonSpeechInfoList();

	void ReleaseSpeechInfoList();

	MonSpeechInfo* GetCurStateSpeechIndex(DWORD MonKind, DWORD StateKind);
	void SetSpeechInfo(MonSpeech* pMonSpeech, DWORD StateKind, CMHFile* fp);
};

EXTERNGLOBALTON(cMonsterSpeechManager)
#endif // !defined(AFX_CMONSTERSPEECHMANAGER_H__980D146A_30B9_4F0C_9A69_4E74D859217A__INCLUDED_)
