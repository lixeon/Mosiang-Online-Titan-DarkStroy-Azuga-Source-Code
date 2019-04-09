#include "stdafx.h"
#include "SuryunDefine.h"
#include "MHFile.h"

#ifdef _MAPSERVER_
#include "SuryunManager_Server.h"
#include "SkillManager_server.h"
#else
#include "SuryunManager_Client.h"
#include "SkillManager_client.h"
#endif


void CSuryunManager::LoadMonsterInfo()
{
	CMHFile file;
	if(!file.Init("./Resource/SuryunMonster.bin", "rb"))
		return;

	while(!file.IsEOF())
	{
		SURYUNMONSTERINFO * pInfo = new SURYUNMONSTERINFO;
		pInfo->Level = file.GetLevel();
		ASSERT(pInfo->Level);
		for(int i=0; i<MAX_MONSTER_PERLEVEL; ++i)
		{
			int count =0;
			for(int j=0;j<MAX_MONIDX_PERMONSTERINFO;++j)
			{
				pInfo->MonsterIdxArray[i].MonsterIdx[count] = file.GetByte();
				if(pInfo->MonsterIdxArray[i].MonsterIdx[count])
					++count;
			}
			pInfo->MonsterIdxArray[i].count = count;
		}
		ASSERT(m_MonsterInfo.GetData(pInfo->Level) == NULL);
		m_MonsterInfo.Add(pInfo, pInfo->Level);
	}
	file.Release();
}

void CSuryunManager::LoadSuryunInfo()
{
	CMHFile file;
	if(!file.Init("./Resource/SuryunData.bin", "rb"))
		return;

	while(!file.IsEOF())
	{
		SURYUNMISSIONINFO * pInfo = new SURYUNMISSIONINFO;
		file.GetString();	// 무공이름
		pInfo->MugongIdx = file.GetWord();
		ASSERT(pInfo->MugongIdx);
		for(int i=0; i<MAX_MUGONG_PRACTICE; ++i)
		{
			pInfo->MissionInfo[i].AimSung = file.GetByte();
			pInfo->MissionInfo[i].LimitTime = file.GetDword();
			pInfo->MissionInfo[i].MonKind = file.GetInt();
			pInfo->MissionInfo[i].MonNum = file.GetInt();
			pInfo->MissionInfo[i].Fee = file.GetDword();
			pInfo->MissionInfo[i].RegenNum = file.GetInt();
			pInfo->MissionInfo[i].AdjustLevel = file.GetInt();
		}
		ASSERT(m_MissionInfo.GetData(pInfo->MugongIdx) == NULL);
		m_MissionInfo.Add(pInfo, pInfo->MugongIdx);
	}
	file.Release();


	if(!file.Init("./Resource/SuryunLevelChange.bin", "rb"))
		return;

	while(!file.IsEOF())
	{		
		LEVELTYPE lv = file.GetLevel();
		
		SURYUNLEVELCHANGE* pChange = new SURYUNLEVELCHANGE;
		pChange->ChangeLevel[0] = file.GetInt();
		pChange->ChangeLevel[1] = file.GetInt();
		pChange->ChangeLevel[2] = file.GetInt();
		ASSERT(m_ChangeLevelTable.GetData(lv) == NULL);
		m_ChangeLevelTable.Add(pChange, lv);
	}
	file.Release();
}





void SURYUNMONSTERINFO::GetMonsterArray(WORD* pMonsterArray)
{
	for(int kind=0;kind<MAX_MONSTER_PERLEVEL;++kind)
	{
		SURYUNMONSTER* pMonsterInfo = &MonsterIdxArray[kind];
		ASSERT(pMonsterInfo->count != 0);
		if(pMonsterInfo->count == 0)
		{
			pMonsterArray[kind] = 0;
			continue;
		}

		int rv = rand() % pMonsterInfo->count;
		pMonsterArray[kind] = pMonsterInfo->MonsterIdx[rv];
	}
}

BOOL CSuryunManager::NeedSuryun(WORD MugongIdx,BYTE Sung,DWORD Exp)
{	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(MugongIdx);
	ASSERT(pSkillInfo);
	if(pSkillInfo == NULL)
		return FALSE;
	
	DWORD maxpoint = pSkillInfo->GetNeedExp(Sung);

	//BOOL bSungCheck = (Sung == 0 || Sung == 6 || Sung == 11);
	//BOOL bExpCheck = Exp >= maxpoint-1 || Sung == 0;

	BOOL bSungCheck = (Sung == 13 );
	BOOL bExpCheck = Exp >= maxpoint-1 || Sung == 13;
	BOOL bJobSkillCheck = SKILLMGR->CheckSkillKind(pSkillInfo->GetSkillKind());
	//2007. 10. 8. CBH - 전문기술은 6, 11성일때 그냥 레벨업한다.
	//2007. 10. 23. CBH - 타이탄 무공은 그냥 레벨업한다.
	if((bSungCheck == TRUE) && (bExpCheck == TRUE) && (bJobSkillCheck == FALSE) && (pSkillInfo->GetSkillKind() != SKILLKIND_TITAN))
	{	
		return TRUE;
	}

	return FALSE;
}


LEVELTYPE CSuryunManager::GetChangedLevel(LEVELTYPE adjustedLevel,BYTE sung)
{
	SURYUNLEVELCHANGE* pChange = m_ChangeLevelTable.GetData(adjustedLevel);
	if(pChange == 0)
		return adjustedLevel;

	int plusLevel;
	if(sung == 0)	plusLevel = pChange->ChangeLevel[0];
	else if(sung == 6)	plusLevel = pChange->ChangeLevel[1];
	else if(sung == 11)	plusLevel = pChange->ChangeLevel[2];
	else plusLevel = 0;
	return adjustedLevel+plusLevel;
}

