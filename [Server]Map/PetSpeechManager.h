#pragma once

#include "MHFile.h"

#define PET_SPEECHMGR	USINGTON(CPetSpeechManager)

enum Pet_SpeechState{
	ePet_Speech_Stand,	//대기 시작 대사
	ePet_Speech_Move,	//이동 시작 대사
	ePet_Speech_Substand,	//대기 보조 대사
	ePet_Speech_Kgong,	//경공 대사
	ePet_Speech_Ungi,	//운기 상태중 이벤트 적 대사
	ePet_Speech_Master_Skill,	//주인 스킬 사용시 대사
	ePet_Speech_Master_Die,		//주인 죽음시 대사
	ePet_Speech_Pet_Skill,		//펫 스킬 사용 대사
	ePet_Speech_Pet_Die,	//펫 죽음 대사
	ePet_Speech_Pet_StaminaZero,	//펫 스태미나 0일 때 대사
	ePet_Speech_MAX,
};

enum SPEECH_TYPE{BALLOON,BALLOON_CHAT,CHANEL_SHOUT,};

struct SpeechList
{
	SpeechList() : Prob(0),SpeechIdx(0){}
	DWORD	Prob;
	DWORD	SpeechIdx;
};

struct PetSpeech
{
	PetSpeech() {memset(this,0,sizeof(PetSpeech));}

	DWORD	PetIdx;
	DWORD	PetSpeechType;
	DWORD	dwProbNoSpeech[ePet_Speech_MAX];
	DWORD	dwSpeechTotalRate[ePet_Speech_MAX];
	DWORD	dwSpchLIstCount[ePet_Speech_MAX];
	SpeechList* pSpchList[ePet_Speech_MAX];
};

struct PetSpeechInfo
{
	PetSpeechInfo() : SpeechType(0), SpeechIndex(0){}
	DWORD SpeechType;
	DWORD SpeechIndex;
};

class CPetSpeechManager
{
	CYHHashTable<PetSpeech>		m_PetSpeechInfoTable;	//펫 대사 정보 테이블

public:
	CPetSpeechManager(void);
	virtual ~CPetSpeechManager(void);

	void Init();
	void Release();

	BOOL LoadPetSpeechInfoList();
	void ReleasePetSpeechInfoList();

	BOOL GetPetCurStateSpeechIndex(DWORD PetKind, DWORD StateKind, DWORD PetGrade, PetSpeechInfo* pRtSpeechInfo);

	void SetPetSpeechInfo(PetSpeech* pPetSpeech, DWORD StateKind, CMHFile* fp);

};

EXTERNGLOBALTON(CPetSpeechManager)