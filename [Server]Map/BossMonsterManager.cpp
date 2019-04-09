// BossMonsterManager.cpp: implementation of the CBossMonsterManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BossMonsterManager.h"
#include "BossMonster.h"
#include "MHFile.h"
#include "BossMonsterInfo.h"
#include "SummonInfo.h"
#include "AIGroupManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


#define MONSTERKIND		"$MONSTERKIND"
	#define REGENTIME		"*REGENTIME"
	#define	SUMMONFILE		"*SUMMONFILENUM"

	#define FIRSTATTACK		"*FIRSTATTACK"
	#define ATTACK			"$ATTACK"
		#define NEXTATTACK	"#NEXTATTACK"

	#define MAXSTATE		"*MAXSTATE"
	#define STATE			"$STATE"
		#define STATECOUNT		"#COUNT"
		#define LIFELESSTHAN	"#LIFELESSTHAN"
		#define ACTION_RECOVER	"#ACTION_RECOVER"
		#define ACTION_SUMMON	"#ACTION_SUMMON"
	
	#define DIESUMMON	"*DIESUMMON"
	#define UNGIVALUE	"*UNGIVALUE"
	#define CHANGETARGETRANGE "*CHANGETARGETRANGE"
	
	/// 06. 08. 2차 보스 - 이영준
	/// 타겟팅 방식
	#define TARGETINGTYPE "*TARGETINGTYPE"

	/// 06. 09. 2차 보스 - 이영준
	/// 소멸시간
	#define DELETETIME "*DELETETIME"


#define MONSTERGROUP		"$GROUP"

CBossMonsterManager::CBossMonsterManager()
{
	Init();	
}

CBossMonsterManager::~CBossMonsterManager()
{
	Release();
}

void CBossMonsterManager::Init()
{
	m_BossMonsterInfoTable.Initialize(2);
	m_SummonInfoTable.Initialize(2);
}

void CBossMonsterManager::Release()
{	
	CBossMonsterInfo* pBossInfo;
	m_BossMonsterInfoTable.SetPositionHead();	
	while(pBossInfo = m_BossMonsterInfoTable.GetData())
	{
		pBossInfo->Release();
		SAFE_DELETE(pBossInfo);
	}
	m_BossMonsterInfoTable.RemoveAll();	

	CSummonInfo* pSummonInfo;
	m_SummonInfoTable.SetPositionHead();
	while(pSummonInfo = m_SummonInfoTable.GetData())
	{
		pSummonInfo->Release();
		SAFE_DELETE(pSummonInfo);
	}
	m_SummonInfoTable.RemoveAll();

	m_BossList.RemoveAll();
}

void CBossMonsterManager::LoadBossMonsterInfoList()
{
	///////////////////////////////////////////////
	//ex)	1	resource/BossMonster1
	//		2	resource/BossMonster2
	///////////////////////////////////////////////
	CMHFile file;
//	file.Init("Resource/BossMonsterfileList.bin", "rb");
	file.Init("Resource/Server/BossMonsterfileList.bin", "rb");
	if(file.IsInited() == FALSE)
	{
		MessageBox(0, "BossMonsterfileList.bin is not found!", 0, MB_OK);
		return;
	}
	char BossFilename[256];
	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;
		file.GetInt();
		file.GetString(BossFilename);
		LoadBossMonster(BossFilename);
	}
	
	file.Release();
}

void CBossMonsterManager::LoadBossMonster(char* BossFile)
{
	CMHFile file;
	char tempBuf[256];
	char bossfilename[256];
	int i=0;
	WORD MonsterKind;
	int MaxStateNum;
	DWORD CurAttackIdx;

#ifdef _FILE_BIN_
	sprintf(bossfilename,"%s.bin",BossFile);
	if(!file.Init(bossfilename,"rb"))
		return ;
#else
	sprintf(bossfilename,"%s.txt",BossFile);
	if(!file.Init(bossfilename,"rt"))
		return ;
#endif
	
	CBossMonsterInfo * pInfo = new CBossMonsterInfo;
	while(1)
	{
		file.GetString(tempBuf);
		if(file.IsEOF())
			break;
		if((tempBuf[0] == '}') || (tempBuf[0] == '{'))
			continue;
		else if(tempBuf[0] == '@')
		{
			file.GetLine(tempBuf, 256);
			continue;
		}
		
		CMD_ST(tempBuf)
		CMD_CS(MONSTERKIND)
		{
			MonsterKind = file.GetWord();
			pInfo->SetMonsterKind(MonsterKind);
		}
		CMD_CS(REGENTIME)
		{
			DWORD MinTime = file.GetDword();
			DWORD MaxTime = file.GetDword();
			pInfo->SetRegenDelayTime(MinTime, MaxTime);
		}
		CMD_CS(MAXSTATE)
		{
			MaxStateNum = file.GetInt();
			pInfo->SetMaxStateNum(MaxStateNum);
		}
		CMD_CS(STATE)
		{
			int num = file.GetInt() - 1;
			if((num < 0) || (num > MaxStateNum-1))
			{
				ASSERTMSG(0, "BossInfo Error! State Num is abnormal");
				return;
			}
			LoadEventState(num, &file, pInfo->GetEventStateInfo());
		}
		CMD_CS(FIRSTATTACK)
		{
			pInfo->SetFirstAttackIdx(file.GetWord());
		}
		CMD_CS(ATTACK)
		{
			NEXTATTACKINFO* pNewNextAttackInfo = new NEXTATTACKINFO;
			CurAttackIdx = file.GetDword();
			LoadAttackInfo(pNewNextAttackInfo,CurAttackIdx, &file);
			pInfo->AddAttackInfo(CurAttackIdx, pNewNextAttackInfo);
		}
		CMD_CS(SUMMONFILE)
			// summon init 
			pInfo->SetSummonFileNum(file.GetWord());
		CMD_CS(DIESUMMON)
			pInfo->SetDieGroupID(file.GetWord());
		CMD_CS(UNGIVALUE)
			pInfo->SetBossUngijosik(file.GetDword(), file.GetDword(), file.GetFloat(), file.GetFloat());
		CMD_CS(CHANGETARGETRANGE)
			pInfo->SetChangeTargetRange(file.GetFloat(), file.GetFloat());
		///////////////////////////////////////////////////////////
		/// 06. 08. 2차 보스 - 이영준
		/// 타겟팅 방식
		/// 0: 기존 방식 1: 매 공격시 랜덤으로 타겟 변경
		CMD_CS( TARGETINGTYPE )
			pInfo->SetTargetingType( file.GetWord() );
		///////////////////////////////////////////////////////////
		/// 06. 09. 2차 보스 - 이영준
		/// 소멸시간
		CMD_CS( DELETETIME )
			pInfo->SetDeleteTime( file.GetDword() * 60000 );
		CMD_EN
	}

	file.Release();
	
	m_BossMonsterInfoTable.Add(pInfo, pInfo->GetMonsterKind());
}

void CBossMonsterManager::LoadEventState(int num, CMHFile * fp, BOSSEVENTSTATE* pRtBossState)
{
	char buf[256];
	while(1)
	{
		fp->GetString(buf);
		if(buf[0] == '{')
			continue;
		else if(buf[0] == '}')
			break;
		else if(buf[0] == '@')
			fp->GetLineX(buf, 256);
		
		CMD_ST(buf)
		CMD_CS(LIFELESSTHAN)
			pRtBossState[num].Condition = eBOSSCONDITION_LIFE;
			pRtBossState[num].ConditionValue = fp->GetWord();
		CMD_CS(ACTION_RECOVER)
			pRtBossState[num].Action = eBOSSACTION_RECOVER;
			pRtBossState[num].ActionValue = fp->GetWord();
		CMD_CS(ACTION_SUMMON)
			pRtBossState[num].Action = eBOSSACTION_SUMMON;
			pRtBossState[num].ActionValue = fp->GetWord();
		CMD_CS(STATECOUNT)
			pRtBossState[num].Count = fp->GetByte();

		CMD_EN
	}
	
}


void CBossMonsterManager::LoadAttackInfo(NEXTATTACKINFO* pRtNextAttack, DWORD CurAttackIdx, CMHFile * fp)
{
	char buf[256];
	while(1)
	{
		fp->GetString(buf);
		if(buf[0] == '{')
			continue;
		else if(buf[0] == '}')
			break;
		else if(buf[0] == '@')
			fp->GetLineX(buf, 256);
		
		
		CMD_ST(buf)
		CMD_CS(NEXTATTACK)
		{
			pRtNextAttack->AddNextAttack(fp->GetWord(), fp->GetDword());
		}		
		CMD_EN
	}
}

void CBossMonsterManager::SetBossInfo(CBossMonster* pBossMonster)
{
	CBossMonsterInfo * pBossInfo = m_BossMonsterInfoTable.GetData(pBossMonster->GetMonsterKind());
	if(pBossInfo)
	{
		pBossMonster->SetBossInfo(pBossInfo);
		return;
	}
	ASSERT(0);
}

//////////////////////////////////////////////////////////////////////////
//summon
void CBossMonsterManager::LoadSummonInfoList()
{
	CMHFile file;

#ifdef _FILE_BIN_
//	file.Init("Resource/SummonList.bin", "rb");
	file.Init("Resource/Server/SummonList.bin", "rb");
	if(!file.IsInited())
	{
		ASSERTMSG(0, "SummonList.bin is not found!");
		return;
	}
#else
	file.Init("Resource/SummonList.txt", "rb");
	if(!file.IsInited())
	{
		ASSERTMSG(0, "SummonList.txt is not found!");
		return;
	}
#endif
	
	int num;
	char summonfile[256];
	while(1)
	{
		if(file.IsEOF())
			break;
		num = file.GetInt();
		file.GetString(summonfile);

		LoadSummonInfo(num, summonfile);
	}
	
	file.Release();
}

void CBossMonsterManager::LoadSummonInfo(int num, char* summonFile)
{
	CMHFile file;
	char summonfilename[256];
#ifdef _FILE_BIN_
	sprintf(summonfilename,"%s.bin",summonFile);
	if(!file.Init(summonfilename,"rb"))
		return ;
#else
	sprintf(summonfilename,"%s.txt",summonFile);
	if(!file.Init(summonfilename,"rt"))
		return ;
#endif

	char tempBuf[256];
	CSummonInfo * pSummonInfo = new CSummonInfo;

	while(1)
	{
		file.GetString(tempBuf);
		if(file.IsEOF())
			break;
		if((tempBuf[0] == '}') || (tempBuf[0] == '{'))
			continue;
		else if(tempBuf[0] == '@')
		{
			file.GetLine(tempBuf, 256);
			continue;
		}
		
		CMD_ST(tempBuf)
		CMD_CS(MONSTERGROUP)
		pSummonInfo->GroupInit(&file);
		CMD_EN
	}
	
	m_SummonInfoTable.Add(pSummonInfo , num);
	file.Release();
}

CSummonInfo* CBossMonsterManager::GetSummonInfo(int num)
{
	CSummonInfo* pInfo;
	pInfo = m_SummonInfoTable.GetData(num);
	return pInfo;
}

void CBossMonsterManager::RegenGroup(CBossMonster* pBoss, WORD SummonFileNum, int GroupNum)
{
	CSummonInfo * pInfo = m_SummonInfoTable.GetData(SummonFileNum);
	if(!pInfo)
	{
		ASSERTMSG(0, "No SummonInfo");
		return;
	}
	pInfo->RegenGroup(pBoss, GroupNum);
}


void CBossMonsterManager::SetBossRandRegenChannel(WORD MonsterKind, DWORD GridID, WORD MonsterGroupNum)
{
	CBossMonsterInfo* pBossInfo = m_BossMonsterInfoTable.GetData(MonsterKind);
	if(pBossInfo == 0)
	{
		ASSERTMSG(0, "NO Boss Info.");
		return;
	}
	
	DWORD delaytime;
	
	delaytime = pBossInfo->CalcRegenDelayTime();
	CAIGroup* pAIGroup = GROUPMGR->GetGroup(MonsterGroupNum, GridID);
	
	if(!pAIGroup) return;
	
	pAIGroup->GetRegenInfo()->SetRegenDelayTime(delaytime);
}

void CBossMonsterManager::UserLogOut(DWORD CharacterID, DWORD GridID)
{	
	PTRLISTSEARCHSTART(m_BossList, CBossMonster*, pBoss)
		if( pBoss )
		{
			if(pBoss->GetGridID() == GridID)
				pBoss->DistributeDeleteDamagedPlayer(CharacterID);
		}
	PTRLISTSEARCHEND
}

void CBossMonsterManager::AddBossMonster(CBossMonster* pBoss)
{
	m_BossList.AddTail(pBoss);
}

void CBossMonsterManager::DeleteBossMonster(CBossMonster* pBoss)
{
	m_BossList.Remove((void*)pBoss);
}
