// SkillAreaManager.cpp: implementation of the CSkillAreaManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillAreaManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// SKILLAREADATASET Func
void SKILLAREADATASET::SetAreaData(CMHFile* pFile)
{
	SkillAreaDataArray[0].LoadAreaData(pFile);
	for(DIRINDEX n=1;n<MAX_DIRECTION_INDEX;++n)
	{
		SkillAreaDataArray[0].GetRotatedAreaData(&SkillAreaDataArray[n],n);
	}	
}
CSkillAreaData* SKILLAREADATASET::GetAreaData(DIRINDEX dir)
{
	return &SkillAreaDataArray[dir];
}
//////////////////////////////////////////////////////////////////////////


CSkillAreaManager::CSkillAreaManager()
{
	m_MaxSkillArea = 0;
	m_SkillAreaDataTable.Initialize(64);
}

CSkillAreaManager::~CSkillAreaManager()
{
	Release();
}

BOOL CSkillAreaManager::LoadSkillAreaList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	file.Init("Resource/SkillAreaList.bin","rb");
#else
	file.Init("Resource/SkillAreaList.txt","rt");
#endif
	if(file.IsInited() == FALSE)
		return FALSE;

	DWORD areanum;
	SKILLAREADATASET* pSet;
	
	CMHFile areafile;
	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		areanum = file.GetDword();
#ifdef _FILE_BIN_
		areafile.Init(file.GetString(),"rb");
#else
		areafile.Init(file.GetString(),"r");
#endif

		if(areafile.IsInited() == FALSE)
			continue;

		pSet = new SKILLAREADATASET;
		pSet->SetAreaData(&areafile);

		m_SkillAreaDataTable.Add(pSet,areanum);
		
		areafile.Release();
	}

	file.Release();
	
	return TRUE;
}
void CSkillAreaManager::Release()
{
	m_SkillAreaDataTable.SetPositionHead();
	while (SKILLAREADATASET* pSet = m_SkillAreaDataTable.GetData()) {
		delete pSet;
	}
	m_SkillAreaDataTable.RemoveAll();
}

CSkillArea* CSkillAreaManager::GetSkillArea(int SkillAreaIndex,DIRINDEX DirIdx)
{
	SKILLAREADATASET* pSet = m_SkillAreaDataTable.GetData(SkillAreaIndex);
	ASSERT(pSet);
	CSkillAreaData* pAreaData = pSet->GetAreaData(DirIdx);
	CSkillArea* pRotatedArea = new CSkillArea(pAreaData);
	return pRotatedArea;
}

void CSkillAreaManager::ReleaseSkillArea(CSkillArea* pSkillArea)
{
	delete pSkillArea;
}