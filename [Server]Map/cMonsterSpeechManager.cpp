// cMonsterSpeechManager.cpp: implementation of the cMonsterSpeechManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cMonsterSpeechManager.h"
#include "MHTimeManager.h"
#include <time.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(cMonsterSpeechManager)

cMonsterSpeechManager::cMonsterSpeechManager()
{
//	srand((unsigned)time(NULL));
	Init();
}

cMonsterSpeechManager::~cMonsterSpeechManager()
{
	Release();
}

void cMonsterSpeechManager::Init()
{

	m_MonSpeechInfoTable.Initialize(100);
//	m_MonSpeechTable.Initialize(100);
}

void cMonsterSpeechManager::Release()
{
	ReleaseSpeechInfoList();
//	ReleaseSpeechList();
}
	
bool cMonsterSpeechManager::LoadMonSpeechInfoList()
{
	CMHFile fp;
	char szBuf[256] = {0,};

	char filename[256];

#ifdef _FILE_BIN_
//	sprintf(filename,"Resource/MonsterSpeechInfoList.bin");
	sprintf(filename,"Resource/Server/MonsterSpeechInfoList.bin");
	if( !fp.Init( filename, "rb" ) )
		return FALSE;
#else
	sprintf(filename,"Resource/MonsterSpeechInfoList.txt");
	if( !fp.Init( filename, "rt" ) )
		return FALSE;
#endif


	int num = 0;
	int eSpeechKind = -1;
	MonSpeech* pMonSpeech = NULL;
	
	while(TRUE)
	{
		//스크립끝 정지 or 최대량 정지
		if( fp.IsEOF() ||num == MAX_MONSTER_NUM )
			break;

		if( strcmp(szBuf, "#KIND") != 0 )
		{
			fp.GetString(szBuf);
		}
		//주석 처리
		if(szBuf[0] == '@')
		{
			fp.GetLineX(szBuf, 256);
			continue;
		}

		//대문자화
//		strcpy(szBuf,strupr(szBuf));
		strupr(szBuf);

		//생성 & 대입
		if( strcmp(szBuf, "#KIND") == 0 )
		{
			pMonSpeech = new MonSpeech;
			//memset(pMonSpeech, 0, sizeof(MonSpeech));	//초기화 했음.

			pMonSpeech->MonsterIdx = fp.GetDword();
			
			pMonSpeech->MonsterType = fp.GetDword();

			while(TRUE)
			{
				fp.GetString(szBuf);

				strupr(szBuf);

				if( strcmp(szBuf, "#FOREATK" ) == 0 )
				{
					eSpeechKind = eMon_Speech_ForeAtk;
				}
				else if( strcmp(szBuf, "#FORGIVE" ) == 0 )
				{
					eSpeechKind = eMon_Speech_ForgivePursuit;
				}
				else if( strcmp(szBuf, "#HELP" ) == 0 )
				{
					eSpeechKind = eMon_Speech_Help;
				}
				else if( strcmp(szBuf, "#FORHELP" ) == 0 )
				{
					eSpeechKind = eMon_Speech_AboutHelp;
				}
				else if( strcmp(szBuf, "#DEATH" ) == 0 )
				{
					eSpeechKind = eMon_Speech_Death;
				}
				else if( strcmp(szBuf, "#STAND" ) == 0 )
				{
					eSpeechKind = eMon_Speech_Stand;
				}
				else if( strcmp(szBuf, "#WALK" ) == 0 )
				{
					eSpeechKind = eMon_Speech_WalkAround;
				}
				else if( strcmp(szBuf, "#KSTAND" ) == 0 )
				{
					eSpeechKind = eMon_Speech_KeepStand;
				}
				else if( strcmp(szBuf, "#KWALK" ) == 0 )
				{
					eSpeechKind = eMon_Speech_KeepWalkAround;
				}
				else if( strcmp(szBuf, "#KIND" ) == 0 || strcmp(szBuf, "#END") == 0 )
				{
					break;
				}

				ASSERT(eSpeechKind > -1);
				SetSpeechInfo(pMonSpeech, eSpeechKind, &fp);
			}
			ASSERT(!m_MonSpeechInfoTable.GetData(pMonSpeech->MonsterIdx));
			m_MonSpeechInfoTable.Add(pMonSpeech, pMonSpeech->MonsterIdx);
			++num;
		}		
	}//while

	fp.Release();
	return TRUE;
}

void cMonsterSpeechManager::ReleaseSpeechInfoList()
{
	MonSpeech* pList = NULL;
	m_MonSpeechInfoTable.SetPositionHead();
	while( pList = m_MonSpeechInfoTable.GetData() )
	{
		for( int i = 0; i < eMon_Speech_MAX; ++i )
		{
			if(pList->pSpchList[i])
			{
				delete [] pList->pSpchList[i];
				pList->pSpchList[i] = NULL;
			}//SAFE_DELETE
		}
		delete pList;
		pList = NULL;
	}
	m_MonSpeechInfoTable.RemoveAll();
}

MonSpeechInfo* cMonsterSpeechManager::GetCurStateSpeechIndex(DWORD MonKind, DWORD StateKind)
{
	MonSpeech* pMonSpeech = m_MonSpeechInfoTable.GetData(MonKind);

	if( !pMonSpeech ) return NULL; //유효한가?

	if( pMonSpeech->dwProbNoSpeech[StateKind] == 100 ) return NULL;
	ASSERT( pMonSpeech->dwSpeechTotalRate[StateKind] >= 0 );
	
	if( pMonSpeech->dwSpchListCount[StateKind] == 0 ) //해당 대사 카운트가 0 이면
		return NULL;
	
	DWORD Rnd = rand()%100;
	
	if( Rnd < pMonSpeech->dwProbNoSpeech[StateKind] )	//대사 없음 확률
		return NULL;

	DWORD SpeechRate = random(0, pMonSpeech->dwSpeechTotalRate[StateKind]);
	if( !SpeechRate )	return NULL;

	DWORD MinRate = 0;
	DWORD MaxRate = 0;
	for( DWORD i = 0; i < pMonSpeech->dwSpchListCount[StateKind]; ++i )
	{
		MinRate = MaxRate;
		MaxRate += pMonSpeech->pSpchList[StateKind][i].Prob;

		if( MinRate < SpeechRate && SpeechRate <= MaxRate )		//대사 확률 비교
		{
			//몬스터 타입(일반, 중간보스, 보스)이 몬스터 구조체 안에 없으므로..처리가 애매하다.
			//몬스터 타입이라기 보다 채팅 타입으로 보자!
			m_SpeechInfo.SpeechType = pMonSpeech->MonsterType;
			m_SpeechInfo.SpeechIndex = pMonSpeech->pSpchList[StateKind][i].SpeechIdx;
			if(m_SpeechInfo.SpeechIndex == 0)	return NULL;	//스크립터 실수를 위한 예외처리.
				
			return &m_SpeechInfo;
		}
	}
/*
#ifdef _DEBUG
	if(SpeechRate)
	{
		char buf[128];
		sprintf(buf, "!MonsterSpeechError_NonSpeech!	MonsterIdx: %d\t StateKind: %d", MonKind, StateKind);
		ASSERTMSG(0, buf);	
	}
#endif*/
	return NULL;
}


void cMonsterSpeechManager::SetSpeechInfo(MonSpeech* pMonSpeech, DWORD StateKind, CMHFile* fp)
{
	DWORD	dwSpeechNum = fp->GetDword();	//대사 갯수
	pMonSpeech->dwSpchListCount[StateKind] = dwSpeechNum;

	pMonSpeech->dwProbNoSpeech[StateKind] = fp->GetDword();	//대사 안함 확률

	char szBuf[512];
	if( !dwSpeechNum  || pMonSpeech->dwProbNoSpeech[StateKind] >= 100 )
	{

		fp->GetLineX(szBuf, 512);		
		return;
	}

	pMonSpeech->pSpchList[StateKind] = new MonSpeechList[dwSpeechNum];

	for( DWORD i = 0; i < dwSpeechNum; ++i )
	{
		pMonSpeech->pSpchList[StateKind][i].Prob = fp->GetDword();
		pMonSpeech->pSpchList[StateKind][i].SpeechIdx = fp->GetDword();
		pMonSpeech->dwSpeechTotalRate[StateKind] += pMonSpeech->pSpchList[StateKind][i].Prob;
	}
	
	fp->GetLineX(szBuf,512);

}