#pragma once

#include "MHFile.h"

#define PET_SPEECHMGR USINGTON(CPetSpeechManager)

struct PetSpeechStr
{
	PetSpeechStr() : Idx(0), pStr(NULL) {}
	DWORD Idx;
	char* pStr;
};

class CPetSpeechManager
{
	CYHHashTable<PetSpeechStr>	m_PetSpeechTable;

public:
	CPetSpeechManager(void);
	~CPetSpeechManager(void);

	void Init();
	void Release();
	BOOL LoadPetSpeechList();
	void ReleasePetSpeechList();

	char* GetPetSpeech(DWORD strIdx);
};

EXTERNGLOBALTON(CPetSpeechManager)