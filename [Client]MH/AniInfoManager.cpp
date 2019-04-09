// AniInfoManager.cpp: implementation of the CAniInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AniInfoManager.h"
#include "Engine/GraphicEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAniInfoManager::CAniInfoManager()
{

}

CAniInfoManager::~CAniInfoManager()
{
	for(int n=0;n<MAX_BYTE_VALUE;++n)
	{
		PTRLISTSEARCHSTART(m_AniInfoList[n],ANIINFOOBJ*,pobj)
			delete pobj->pAniInfo;
			delete pobj;
		PTRLISTSEARCHEND
			
		m_AniInfoList[n].RemoveAll();
	}
}

CAniInfo* CAniInfoManager::FindAniInfoFromPool(char* filename)
{
	BYTE headkey = (BYTE)*filename;
	PTRLISTSEARCHSTART(m_AniInfoList[headkey],ANIINFOOBJ*,pobj)
		if(strcmp(pobj->filename,filename) == 0)
			return pobj->pAniInfo;
	PTRLISTSEARCHEND

	return NULL;
}

void CAniInfoManager::AddAniInfoToPool(char* filename,CAniInfo* pAniInfo)
{
	BYTE headkey = (BYTE)*filename;
	ANIINFOOBJ* pobj = new ANIINFOOBJ(filename,pAniInfo);	
	m_AniInfoList[headkey].AddHead(pobj);
}

CAniInfo* CAniInfoManager::GetAniInfoObj(char* filename,GXOBJECT_HANDLE handle)
{
	CAniInfo* pAniInfo = FindAniInfoFromPool(filename);
	if(pAniInfo)
		return pAniInfo;

	return LoadAniInfoFile(filename,handle);
}

CAniInfo* CAniInfoManager::GetAniInfoObj(char* filename,GXOBJECT_HANDLE handle,WORD StartFrame,WORD EndFrame)
{
	CAniInfo* pAniInfo = GetAniInfoObj(filename,handle);
	ASSERT(pAniInfo->GetMaxAniNum() <= 1);
	ANIINFO* pInfo = pAniInfo->GetAniInfo(1);
	pInfo->StartFrame = StartFrame;
	pInfo->EndFrame = EndFrame;
	return pAniInfo;
}

CAniInfo* CAniInfoManager::LoadAniInfoFile(char* filename,GXOBJECT_HANDLE handle)
{
	DWORD MaxAniNum = g_pExecutive->GXOGetMotionNum(handle,0);	

	CAniInfo* pAniinfo = new CAniInfo(filename,MaxAniNum);
	AddAniInfoToPool(filename,pAniinfo);

	return pAniinfo;
}
