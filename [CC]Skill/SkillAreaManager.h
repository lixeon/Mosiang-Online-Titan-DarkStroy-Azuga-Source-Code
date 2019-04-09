// SkillAreaManager.h: interface for the CSkillAreaManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLAREAMANAGER_H__1FDD7F51_E7AE_49A5_BAE8_811E7364A582__INCLUDED_)
#define AFX_SKILLAREAMANAGER_H__1FDD7F51_E7AE_49A5_BAE8_811E7364A582__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillArea.h"
#include "SkillAreaData.h"

struct SKILLAREADATASET		// ¹æÇâ¶§¹®¿¡ ÀÎµ¦½ºº°·Î ¸ð¾Ò´Ù.
{
private:
	CSkillAreaData SkillAreaDataArray[MAX_DIRECTION_INDEX];
public:
	void SetAreaData(CMHFile* pFile);
	CSkillAreaData* GetAreaData(DIRINDEX dir);
};

class CSkillAreaManager  
{
	int m_MaxSkillArea;
	CYHHashTable<SKILLAREADATASET> m_SkillAreaDataTable;

public:
//	MAKESINGLETON(CSkillAreaManager);

	CSkillAreaManager();
	virtual ~CSkillAreaManager();

	BOOL LoadSkillAreaList();
	void Release();

	CSkillArea* GetSkillArea(int SkillAreaIndex,DIRINDEX DirIdx);
	void ReleaseSkillArea(CSkillArea* pSkillArea);
};

#endif // !defined(AFX_SKILLAREAMANAGER_H__1FDD7F51_E7AE_49A5_BAE8_811E7364A582__INCLUDED_)
