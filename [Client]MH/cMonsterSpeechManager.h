// cMonsterSpeechManager.h: interface for the cMonsterSpeechManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMONSTERSPEECHMANAGER_H__ED30E693_1F9D_44B8_B98A_47E0637C7E49__INCLUDED_)
#define AFX_CMONSTERSPEECHMANAGER_H__ED30E693_1F9D_44B8_B98A_47E0637C7E49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MHFile.h"

#define MON_SPEECHMGR USINGTON(cMonsterSpeechManager)

enum Monster_Type{ eMonType_General, eMonType_M_Boss, eMonType_H_Boss };

struct MonSpeechStr
{
	MonSpeechStr() : idx(0),pStr(NULL){}
	DWORD idx;
	char* pStr;
};

class cMonsterSpeechManager  
{
	CYHHashTable<MonSpeechStr>	m_MonSpeechTable;	//몬스터 대사 스트링 테이블

public:
	cMonsterSpeechManager();
	virtual ~cMonsterSpeechManager();
	void Init();
	void Release();
	bool LoadSpeechList();
	void ReleaseSpeechList();

	char* GetMonsterSpeech(DWORD Idx);
};

EXTERNGLOBALTON(cMonsterSpeechManager)
#endif // !defined(AFX_CMONSTERSPEECHMANAGER_H__ED30E693_1F9D_44B8_B98A_47E0637C7E49__INCLUDED_)
