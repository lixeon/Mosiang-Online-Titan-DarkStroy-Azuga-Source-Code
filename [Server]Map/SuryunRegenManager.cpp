// SuryunRegenManager.cpp: implementation of the CSuryunRegenManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SuryunRegenManager.h"
#include "SuryunRegen.h"
#include "MHFile.h"
#include "SuryunRegenInfo.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSuryunRegenManager::CSuryunRegenManager()
{
	m_SuryunRegen.Initialize(32);
	m_SuryunRegenInfo.Initialize(32);
}

CSuryunRegenManager::~CSuryunRegenManager()
{
	Release();
}

void CSuryunRegenManager::Release()
{
	CSuryunRegen* pInfo;
	m_SuryunRegen.SetPositionHead();
	while(pInfo = m_SuryunRegen.GetData())
		delete pInfo;
	m_SuryunRegen.RemoveAll();

	CSuryunRegenInfo* pRInfo;
	m_SuryunRegenInfo.SetPositionHead();
	while(pRInfo = m_SuryunRegenInfo.GetData())
		delete pRInfo;
	m_SuryunRegenInfo.RemoveAll();
}

void CSuryunRegenManager::LoadSuryunRegenList()
{
	CMHFile file;
#ifdef _FILE_BIN_
//	if(!file.Init("Resource/SuryunRegenList.bin","rb"))
	if(!file.Init("Resource/Server/SuryunRegenList.bin","rb"))
		return ;
#else
	if(!file.Init("Resource/SuryunRegenList.txt","rt"))
		return ;
#endif	
	char filename[128];
	int num;
	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;
		num = file.GetByte();
		file.GetString(filename);
		CSuryunRegenInfo * pRegenInfo = new CSuryunRegenInfo;
		pRegenInfo->Load(filename);
		m_SuryunRegenInfo.Add(pRegenInfo, num);
	}
	file.Release();
}


CSuryunRegen* CSuryunRegenManager::MakeNewRegen(int RegenNum, DWORD SuryunID, WORD* MonsterKindInfo)
{
	CSuryunRegen* pSuRegen;
	if(pSuRegen = m_SuryunRegen.GetData(SuryunID))
	{
		ASSERT(0);
		return NULL;
	}
	pSuRegen = new CSuryunRegen;
	CSuryunRegenInfo* pRegenInfo = m_SuryunRegenInfo.GetData(RegenNum);
	pSuRegen->Init(pRegenInfo, MonsterKindInfo);
	m_SuryunRegen.Add(pSuRegen, SuryunID);
	return pSuRegen;
}

void CSuryunRegenManager::ReleaseRegen(DWORD SuryunID)
{
	CSuryunRegen* pRegen = m_SuryunRegen.GetData(SuryunID);
	if(!pRegen)
	{
		ASSERT(0);
		return;
	}
	m_SuryunRegen.Remove(SuryunID);
	delete pRegen;
}
