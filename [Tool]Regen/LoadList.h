// LoadList.h: interface for the CLoadList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADLIST_H__7DB30271_0CE8_4AA9_AE43_E00854FB0381__INCLUDED_)
#define AFX_LOADLIST_H__7DB30271_0CE8_4AA9_AE43_E00854FB0381__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DefineStruct.h"
#include "MHFile.h"
#include "./Engine/EngineObject.h"

class CLoadList  
{

//	CEngineObject m_EngineObject;
	char m_szTileName[MAX_MAP_NUM][MAX_FILE_NAME];
public:
//	MOD_LIST			m_ModFileList[GENDER_MAX][eJOB_Max];
//	MOD_LIST			m_HairModList[GENDER_MAX];
	
	CLoadList();
	virtual ~CLoadList();

//	MAP_LIST m_MapList[MAX_MAP_NUM];
	BASE_MONSTER_LIST m_MonsterList[MAX_MONSTER_NUM];
	
	BOOL LoadMonsterList();

	char* GetTileName(WORD MapNum){return m_szTileName[MapNum];}
};

extern CLoadList* g_pLoadList;
#endif // !defined(AFX_LOADLIST_H__7DB30271_0CE8_4AA9_AE43_E00854FB0381__INCLUDED_)
