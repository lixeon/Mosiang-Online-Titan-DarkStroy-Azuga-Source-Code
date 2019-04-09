// LoadList.cpp: implementation of the CLoadList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegenTool.h"
#include "LoadList.h"
#include "./Engine/EngineObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoadList* g_pLoadList = NULL;

CLoadList::CLoadList()
{
}

CLoadList::~CLoadList()
{
}
/*
BOOL CLoadList::LoadMapList(MAP_LIST* pMapList)
{
	char filename[64];
	char value[64] = {0,};
	sprintf(filename,"./Resource/MapList.txt");
	CMHFile file;
	if(!file.Init(filename, "rt"))
		return FALSE;

	int n =0;
	while(1)
	{
		if(file.IsEOF())
			break;

		pMapList[n].MapKind = file.GetWord();
		file.GetString(pMapList[n].FieldName);
		file.GetString(pMapList[n].LocalName);
		file.GetString(pMapList[n].PositionName);
		file.GetString(pMapList[n].FileName);
		++n;
	}
	return TRUE;
}
*/
BOOL CLoadList::LoadMonsterList()
{
	char filename[64];
	char value[64] = {0,};
	sprintf(filename,"./Resource/MonsterList.bin");
	CMHFile file;
	if(!file.Init(filename, "rb"))
		return FALSE;

	memset(m_MonsterList,0,sizeof(BASE_MONSTER_LIST)*MAX_MONSTER_NUM);

	BASE_MONSTER_LIST* pList;
	int n =0;
	while(1)
	{
		if(n == MAX_MONSTER_NUM)
			break;

		if(file.IsEOF())
			break;
		
		pList = &m_MonsterList[n];
		memset(pList, 0, sizeof(BASE_MONSTER_LIST));
		pList->MonsterKind				= file.GetWord();		// 번호
		file.GetString(pList->Name);						// 몬스터이름	
		file.GetString(pList->EngName);						// 영어이름
		pList->Level					= file.GetLevel();		// 몬스터레벨	
		pList->MoveDramaNum				= file.GetInt();	
		pList->MotionID					= file.GetDword();	
		file.GetString(pList->ChxName);

		char temp[1024];
		file.GetLine(temp,1024);
		++n;
	}
	return TRUE;
}
