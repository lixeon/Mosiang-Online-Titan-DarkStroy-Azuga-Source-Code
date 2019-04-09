// BossMonsterManager.h: interface for the CBossMonsterManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOSSMONSTERMANAGER_H__57C614FD_215F_4B05_B820_6CCBF71CBB58__INCLUDED_)
#define AFX_BOSSMONSTERMANAGER_H__57C614FD_215F_4B05_B820_6CCBF71CBB58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBossMonster;
class CBossMonsterInfo;

#define BOSSMONMGR CBossMonsterManager::GetInstance()

struct BOSSEVENTSTATE;
struct BOSSATTACK;
struct NEXTATTACKINFO;
class CMHFile;
class CSummonInfo;

class CBossMonsterManager  
{
	CBossMonsterManager();

	CYHHashTable<CBossMonsterInfo> m_BossMonsterInfoTable;
	CYHHashTable<CSummonInfo> m_SummonInfoTable;
	cPtrList m_BossList;
public:
	GETINSTANCE(CBossMonsterManager);
	virtual ~CBossMonsterManager();

	void Init();
	void Release();
	
	void LoadBossMonsterInfoList();
	void LoadBossMonster(char* BossFile);
	void LoadSummonInfoList();
	void LoadSummonInfo(int num, char* summonFile);	

	void SetBossInfo(CBossMonster* pBossMonster);
	
	void LoadEventState(int num, CMHFile * fp, BOSSEVENTSTATE* pRtBossState);
	void LoadAttackInfo(NEXTATTACKINFO* pRtNextAttack, DWORD CurAttackIdx, CMHFile * fp);

	CSummonInfo* GetSummonInfo(int num);
	void RegenGroup(CBossMonster* pBoss, WORD SummonFileNum, int GroupNum);
	
	void SetBossRandRegenChannel(WORD MonsterKind, DWORD GridID, WORD MonsterGroupNum);
	void UserLogOut(DWORD CharacterID, DWORD GridID);

	void AddBossMonster(CBossMonster* pBoss);
	void DeleteBossMonster(CBossMonster* pBoss);
};

#endif // !defined(AFX_BOSSMONSTERMANAGER_H__57C614FD_215F_4B05_B820_6CCBF71CBB58__INCLUDED_)
