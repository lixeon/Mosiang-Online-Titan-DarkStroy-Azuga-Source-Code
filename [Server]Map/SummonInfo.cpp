// SummonInfo.cpp: implementation of the CSummonInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SummonInfo.h"
//#include "MHFile.h"
#include "SummonGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define ADDMONSTER "#ADD"
#define SEEDPOS	"#SEEDPOS"

CSummonInfo::CSummonInfo()
{
}

CSummonInfo::~CSummonInfo()
{

}

void CSummonInfo::Release()
{
	map<int, CSummonGroup*>::iterator it;
	for(it = m_mapSummonGroup.begin(); it != m_mapSummonGroup.end(); ++it)
	{
		CSummonGroup* pInfo = it->second;
		delete pInfo;
		pInfo = NULL;
	}
	m_mapSummonGroup.clear();
}

void CSummonInfo::AddSummonGroup(int num, CSummonGroup* pInfo)
{
	m_mapSummonGroup[num] = pInfo;
}

void CSummonInfo::GroupInit(CMHFile * fp)
{
	char buf[256];
	int GroupNum;
	BYTE ObjectKind;
	GroupNum = fp->GetInt();
	WORD MonsterKind ;
	float x, z;
	CSummonGroup * pInfo = new CSummonGroup;
	while(1)
	{
		fp->GetString(buf);

		if(buf[0] == '}')
			return;
		else if(buf[0] == '{')
			continue;
		else if(buf[0] == '@')
		{
			fp->GetLineX(buf, 256);
			continue;
		}

		CMD_ST(buf)
		CMD_CS(ADDMONSTER)
		{
			ObjectKind = fp->GetByte();
			fp->GetDword(); // id
			MonsterKind = fp->GetWord();
			x = fp->GetFloat();
			z= fp->GetFloat();
			pInfo->AddSummonMonsterInfo(ObjectKind, MonsterKind, x, z);
			AddSummonGroup(GroupNum, pInfo);
		}
		CMD_CS(SEEDPOS)
			x = fp->GetFloat();
			z = fp->GetFloat();
			pInfo->SetSeedPos((POSTYPE)x,(POSTYPE)z);
		CMD_EN

	}
}

CSummonGroup* CSummonInfo::GetSummonGroup(int num)
{
	return m_mapSummonGroup[num];
}

void CSummonInfo::RegenGroup(CBossMonster* pBoss, int GroupNum)
{
	CSummonGroup* pSummonGroup = GetSummonGroup(GroupNum);
	if(!pSummonGroup)
	{
		ASSERTMSG(0,"No SummonGroup!");
		return;
	}
	pSummonGroup->RegenGroup(pBoss);
}