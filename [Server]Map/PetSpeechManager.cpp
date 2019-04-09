#include "stdafx.h"
#include "MHTimeManager.h"
#include <time.h>
#include ".\petspeechmanager.h"

GLOBALTON(CPetSpeechManager)

CPetSpeechManager::CPetSpeechManager(void)
{
	Init();	//->ServerSystem->Start
}

CPetSpeechManager::~CPetSpeechManager(void)
{
	Release();
}

void CPetSpeechManager::Init()
{
	m_PetSpeechInfoTable.Initialize(100);
}

void CPetSpeechManager::Release()
{
	ReleasePetSpeechInfoList();
}

BOOL CPetSpeechManager::LoadPetSpeechInfoList()
{
	CMHFile fp;
	char szBuf[256] = {0,};

	char filename[256];
	
#ifdef _FILE_BIN_
//	sprintf(filename, "Resource/PetSpeechInfoList.bin");
	sprintf(filename, "Resource/Server/PetSpeechInfoList.bin");
	if( !fp.Init( filename, "rb" ) )
		return FALSE;
#else
	sprintf(filename, "Resource/PetSpeechInfoList.txt");
	if( !fp.Init( filename, "rt" ) )
		return FALSE;
#endif

	int num = 0;
	int eSpeechKind = -1;
	PetSpeech* pPetSpeech = NULL;

	while(TRUE)
	{
		if( fp.IsEOF() || num == MAX_PET_NUM )
			break;

		if( strcmp(szBuf, "#KIND") != 0 )
		{
			fp.GetString(szBuf);
		}

		if(szBuf[0] == '@')
		{
			fp.GetLineX(szBuf, 256);
			continue;
		}

		strupr(szBuf);

		if( 0 == strcmp(szBuf, "#KIND") )
		{
			pPetSpeech = new PetSpeech;
			pPetSpeech->PetIdx = fp.GetDword();
			pPetSpeech->PetSpeechType = fp.GetDword();

			while(TRUE)
			{
				fp.GetString(szBuf);

				strupr(szBuf);

				if( 0 == strcmp(szBuf, "#STAND" ) )
				{
					eSpeechKind = ePet_Speech_Stand;
				}
				else if( 0 == strcmp(szBuf, "#MOVE" ) )
				{
					eSpeechKind = ePet_Speech_Move;
				}
				else if( 0 == strcmp(szBuf, "#SUB" ) )
				{
					eSpeechKind = ePet_Speech_Substand;
				}
				else if( 0 == strcmp(szBuf, "#KGONG" ) )
				{
					eSpeechKind = ePet_Speech_Kgong;
				}
				else if( 0 == strcmp(szBuf, "#UNGI" ) )
				{
					eSpeechKind = ePet_Speech_Ungi;
				}
				else if( 0 == strcmp(szBuf, "#CSKILL" ) )
				{
					eSpeechKind = ePet_Speech_Master_Skill;
				}
				else if( 0 == strcmp(szBuf, "#CDIE" ) )
				{
					eSpeechKind = ePet_Speech_Master_Die;
				}
				else if( 0 == strcmp(szBuf, "#PSKILL" ) )
				{
					eSpeechKind = ePet_Speech_Pet_Skill;
				}
				else if( 0 == strcmp(szBuf, "#PDIE" ) )
				{
					eSpeechKind = ePet_Speech_Pet_Die;
				}
				else if( 0 == strcmp(szBuf, "#PSTZERO" ) )
				{
					eSpeechKind = ePet_Speech_Pet_StaminaZero;
				}
				else if( 0 == strcmp(szBuf, "#KIND") || 0 == strcmp(szBuf, "#END") )
				{
					break;
				}

				ASSERT(eSpeechKind > -1);
				SetPetSpeechInfo(pPetSpeech, eSpeechKind, &fp);
			}
			ASSERT(!m_PetSpeechInfoTable.GetData(pPetSpeech->PetIdx));
			m_PetSpeechInfoTable.Add(pPetSpeech, pPetSpeech->PetIdx);
			++num;
		}
	}//while

	fp.Release();

	return TRUE;
}
	
void CPetSpeechManager::ReleasePetSpeechInfoList()
{
	PetSpeech* pList = NULL;
	m_PetSpeechInfoTable.SetPositionHead();
	while( pList = m_PetSpeechInfoTable.GetData() )
	{
		for( int i = 0; i < ePet_Speech_MAX; ++ i )
		{
			if(pList->pSpchList[i])
			{
				delete [] pList->pSpchList[i];
				pList->pSpchList[i] = NULL;
			}
		}
		delete pList;
		pList = NULL;
	}
	m_PetSpeechInfoTable.RemoveAll();
}

BOOL CPetSpeechManager::GetPetCurStateSpeechIndex(DWORD PetKind, DWORD StateKind, DWORD PetGrade, PetSpeechInfo* pRtSpeechInfo)
{
	PetSpeech* pPetSpeech = m_PetSpeechInfoTable.GetData(PetKind);
	if( !pPetSpeech ) return FALSE;

	--StateKind;	//모션 인덱스는 1부터..

	if( 100 == pPetSpeech->dwProbNoSpeech[StateKind] ) return FALSE;
	ASSERT( pPetSpeech->dwSpchLIstCount[StateKind] >= 0);

	if( 0 == pPetSpeech->dwSpchLIstCount[StateKind] ) return FALSE;

	DWORD Rnd = rand()%100;

	//펫 단계별로 확률 증가한다. 10% -> 20% -> 30%	//pPetSpeech->dwProbNoSpeech[StateKind] 는 기본 100%?
	Rnd += (DWORD)(Rnd * (PetGrade * 0.1f));
	if( Rnd < pPetSpeech->dwProbNoSpeech[StateKind] ) return FALSE;
	//if( Rnd < pPetSpeech->dwProbNoSpeech[StateKind] - PetGrade) return FALSE;

	DWORD SpeechRate = random(0, pPetSpeech->dwSpeechTotalRate[StateKind]);
	DWORD MinRate = 0;
	DWORD MaxRate = 0;
	for( DWORD i = 0; i < pPetSpeech->dwSpchLIstCount[StateKind]; ++i )
	{
		MinRate = MaxRate;
		MaxRate += pPetSpeech->pSpchList[StateKind][i].Prob;

		if( MinRate < SpeechRate && SpeechRate <= MaxRate )
		{
			pRtSpeechInfo->SpeechType = pPetSpeech->PetSpeechType;
			pRtSpeechInfo->SpeechIndex = pPetSpeech->pSpchList[StateKind][i].SpeechIdx;

			return TRUE;
		}
	}

#ifdef _DEBUG
	if(SpeechRate)
	{
		char buf[128];
		sprintf(buf, "!PetSpeechError_NonSpeech!	PetIdx: %d/t StateKind: %d", PetKind, StateKind);
		ASSERTMSG(0, buf);	
	}
#endif
	return FALSE;
}

void CPetSpeechManager::SetPetSpeechInfo(PetSpeech* pPetSpeech, DWORD StateKind, CMHFile* fp)
{
	DWORD	dwSpeechNum = fp->GetDword();	//대사 갯수
	pPetSpeech->dwSpchLIstCount[StateKind] = dwSpeechNum;

	pPetSpeech->dwProbNoSpeech[StateKind] = fp->GetDword();	//대사 안함 확률

	char szBuf[512];
	if( !dwSpeechNum || 100 <= pPetSpeech->dwProbNoSpeech[StateKind] )
	{
		fp->GetLineX(szBuf, 512);
		return;
	}

	pPetSpeech->pSpchList[StateKind] = new SpeechList[dwSpeechNum];

	for( DWORD i = 0; i < dwSpeechNum; ++i )
	{
		pPetSpeech->pSpchList[StateKind][i].Prob = fp->GetDword();
		pPetSpeech->pSpchList[StateKind][i].SpeechIdx = fp->GetDword();
		pPetSpeech->dwSpeechTotalRate[StateKind] += pPetSpeech->pSpchList[StateKind][i].Prob;
	}

	fp->GetLineX(szBuf,512);

}
