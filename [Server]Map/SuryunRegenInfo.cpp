// SuryunRegenInfo.cpp: implementation of the CSuryunRegenInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SuryunRegenInfo.h"
#include "MHFile.h"
#include "SuryunRegenGroupInfo.h"
#include "SuryunRegenGroup.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSuryunRegenInfo::CSuryunRegenInfo()
{
	m_MaxCount = 0;
	m_RegenGroupInfoArray = NULL;
}

CSuryunRegenInfo::~CSuryunRegenInfo()
{
	if( m_RegenGroupInfoArray )
	{
		delete [] m_RegenGroupInfoArray;
	}
}

void CSuryunRegenInfo::Init(char* filename)
{
	CMHFile file;

	if(!file.Init(filename, "rb"))
	{
		ASSERT(0);
		return;
	}
		
	int a = file.GetByte();
	file.Release();

	//regen group info add
}

void CSuryunRegenInfo::TimeRegen(CSuryunRegenGroup* pRegenGroup, int GroupID, DWORD elpsTime, CPlayer* pPlayer, WORD* MonsterKindInfo)
{
	if((GroupID <0) || (GroupID >= m_MaxCount))
	{
		ASSERTMSG(0, "(그룹 맥스아이디)와 (그룹의 총 갯수-1)은 같아야 합니다.");
		return;
	}
	
	m_RegenGroupInfoArray[GroupID].TimeRegen(pRegenGroup, elpsTime, pPlayer, MonsterKindInfo);
}

void CSuryunRegenInfo::RatioRegen(CSuryunRegenGroup* pRegenGroupArray, int GroupID, int CurMonsterNum, CPlayer* pPlayer, WORD* MonsterKindInfo)
{
	if((GroupID <0) || (GroupID >= m_MaxCount))
	{
		ASSERTMSG(0, "(그룹 맥스아이디)와 (그룹의 총 갯수-1)은 같아야 합니다.");
		return;
	}
	
	int SeqGroup;
	for(int n=0;n<5;++n)
	{
		SeqGroup = m_RegenGroupInfoArray[GroupID].ChkRatioRegen(n,CurMonsterNum);
		if(SeqGroup == -1)
			continue;

		m_RegenGroupInfoArray[SeqGroup].RatioRegen(&pRegenGroupArray[SeqGroup], pPlayer, MonsterKindInfo);
	}	
}


#define GROUPKEY	"$GROUP"
#define MAXGROUP	"$MAXGROUP"
#define GROUPNAME	"#GROUPNAME"
void CSuryunRegenInfo::Load(char* filename)
{
	CMHFile file;
	if(!file.Init(filename,"rb"))
	{
		ASSERTMSG(0,filename);
		return;
	}
	char Buf[1024] = {0,};
	char tempBuf[256];
	BOOL bReadGroup = FALSE;
	char GroupID[4];
	
	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		file.GetString(tempBuf);
		if(bReadGroup == FALSE)
		{
			if(strncmp(tempBuf, MAXGROUP, strlen(MAXGROUP)) == 0)
			{
				m_MaxCount = file.GetByte();
				m_RegenGroupInfoArray = new CSuryunRegenGroupInfo[m_MaxCount];
			}
			if(strncmp(tempBuf,GROUPKEY,strlen(GROUPKEY)) == 0)
			{
				bReadGroup = TRUE;
				Buf[0] = 0;
				continue;
			}
		}
		else
		{
			strcat(Buf,tempBuf);
			strcat(Buf," ");
			if(strncmp(tempBuf, GROUPNAME, strlen(GROUPNAME)) == 0)
			{
				file.GetString(GroupID);
				strcat(Buf,GroupID);
				strcat(Buf," ");
				int a = atoi(GroupID);
			}
			if(tempBuf[0] == '}')
			{
			//	CSuryunRegenGroupInfo* pInfo = new CSuryunRegenGroupInfo;
			//	pInfo->fromStr(Buf);
			//	pInfo->AddGroupToListCtrl(&m_listGroup);
			//	m_RegenGroupInfoArray[i].fromStr(Buf);
				m_RegenGroupInfoArray[atoi(GroupID)].fromStr(Buf);
				bReadGroup = FALSE;
			}
		}
	}

	file.Release();
}


int CSuryunRegenInfo::GetMonCountinGroup(int GroupID)
{
	if((GroupID <0) || (GroupID >= m_MaxCount))
	{
		ASSERTMSG(0, "(그룹 맥스아이디)와 (그룹의 총 갯수-1)은 같아야 합니다.");
		return 0;
	}
	
	return m_RegenGroupInfoArray[GroupID].GetMonsterCount();
}