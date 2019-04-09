// SummonGroup.h: interface for the CSummonGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUMMONGROUP_H__97FB70DD_604F_4455_84AA_BDEF84D7D83D__INCLUDED_)
#define AFX_SUMMONGROUP_H__97FB70DD_604F_4455_84AA_BDEF84D7D83D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;
class CMHFile;
struct SUMMONMONSTERINFO
{
	BYTE ObjectKind;
	WORD MonsterKind;
	float x;
	float z;

	SUMMONMONSTERINFO(BYTE ObjectKind, WORD MonsterKind, float x, float z) 
		: ObjectKind(ObjectKind), MonsterKind(MonsterKind), x(x), z(z) {}
};

class CSummonGroup  
{
	vector<SUMMONMONSTERINFO*> m_vecSummonMonster;
	VECTOR3	m_SeedPos;
	
public:
	CSummonGroup();
	virtual ~CSummonGroup();

	void Release();

	void AddSummonMonsterInfo(BYTE ObjectKind, WORD MonsterKind, float x, float z);
	void RegenGroup(CBossMonster* pBoss);
	void SetSeedPos(POSTYPE x, POSTYPE z);
	VECTOR3 GetSeedPos();
};

#endif // !defined(AFX_SUMMONGROUP_H__97FB70DD_604F_4455_84AA_BDEF84D7D83D__INCLUDED_)
