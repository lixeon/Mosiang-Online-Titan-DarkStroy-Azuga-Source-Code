// MunpaMarkManager.cpp: implementation of the CMunpaMarkManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MunpaMarkManager.h"
#include "MHFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMunpaMarkManager)
CMunpaMarkManager::CMunpaMarkManager()
{
	m_bIsInited = FALSE;
	m_DescTable.Initialize(64);
	m_MunpaMarkNumTable.Initialize(64);
}

CMunpaMarkManager::~CMunpaMarkManager()
{
	Release();
}

BOOL CMunpaMarkManager::IsInited()
{
	return m_bIsInited;
}

void CMunpaMarkManager::Init()
{
	if(!LoadMunpaMarkNum())
	{
		ASSERTMSG(0, "Fail To Load a File : MunpaMarkNumList");
		return;
	}

	CMHFile file;
#ifdef _FILE_BIN_
	if(file.Init("image/munpamarkList.bin","rb") == FALSE )
		return;
#else
	if(file.Init("image/munpamarkList.txt","rt") == FALSE )
		return;
#endif

	int marknum;
	char fileimagename[64];
	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;
		
		marknum = file.GetInt();
		file.GetString(fileimagename);
		
		// 약간의 검사 과정

		MUNPAMARKDESC* pDesc = new MUNPAMARKDESC;
		pDesc->MarkNum = marknum;
		strcpy(pDesc->MarkFileName,fileimagename);

		BOOL rt = pDesc->Mark.Init(pDesc->MarkFileName);
		ASSERT(rt);

		m_DescTable.Add(pDesc,pDesc->MarkNum);	
	}

	m_bIsInited = TRUE;
}

BOOL CMunpaMarkManager::LoadMunpaMarkNum()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(file.Init("image/MunpaMarkNumList.bin", "rb") == FALSE)
		return FALSE;
#else
	if(file.Init("image/MunpaMarkNumList.txt", "rt") == FALSE)
		return FALSE;
#endif

	DWORD munpaidx;
	int imgnum;
	
	if(!file.IsInited()) return FALSE;

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;
		munpaidx = file.GetDword();
		imgnum = file.GetInt();
		
		MUNPAMARKNUM* pNum = new MUNPAMARKNUM;
		pNum->MunpaID = munpaidx;
		pNum->MarkNum = imgnum;
		
		m_MunpaMarkNumTable.Add(pNum, pNum->MunpaID);
	}

	file.Release();
	return TRUE;
}

void CMunpaMarkManager::Release()
{
	m_bIsInited = FALSE;
	MUNPAMARKDESC* pDescTable = NULL;
	m_DescTable.SetPositionHead();
	while(pDescTable = m_DescTable.GetData())
	{
		delete pDescTable;
	}
	m_DescTable.RemoveAll();


	MUNPAMARKNUM* pMarkTable = NULL;
	m_MunpaMarkNumTable.SetPositionHead();
	while(pMarkTable = m_MunpaMarkNumTable.GetData())
	{
		delete pMarkTable;
	}
	m_MunpaMarkNumTable.RemoveAll();
}

CMunpaMark* CMunpaMarkManager::GetMunpaMark(int MarkNum)
{
//	ASSERT(IsInited());
	if(IsInited())
	{	
		MUNPAMARKDESC* pMunpaMarkDesc = m_DescTable.GetData(MarkNum);
		ASSERT(pMunpaMarkDesc);
		return &pMunpaMarkDesc->Mark;
	}
	else 
		return NULL;
}

CMunpaMark* CMunpaMarkManager::GetMunpaMark(DWORD MunpaID)
{
	if(MunpaID == 0)
		return NULL;
	MUNPAMARKNUM* p = m_MunpaMarkNumTable.GetData(MunpaID);
	if(!p)
		return NULL;	//GetMunpaMark(0); //기본 이미지
//	ASSERT(p);
	else 
		return GetMunpaMark(p->MarkNum);
}
