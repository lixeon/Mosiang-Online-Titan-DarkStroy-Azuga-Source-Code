// AniInfo.cpp: implementation of the CAniInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AniInfo.h"
#include "MHFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAniInfo::CAniInfo(char* filename,DWORD MaxAniNum)
{
	m_MaxAniNum = MaxAniNum+1;
	m_AniInfo = new ANIINFO[m_MaxAniNum];
	for(WORD n=0;n<m_MaxAniNum;++n)
	{
		m_AniInfo[n].EngineMotionIndex = n;
		m_AniInfo[n].StartFrame = 0;
		m_AniInfo[n].EndFrame = -1;
	}
/*
	DIRECTORYMGR->SetLoadMode(eLM_Resource);

	char ChiFileName[256];
	strcpy(ChiFileName,filename);
	sprintf(&ChiFileName[strlen(filename)-3],"bin");
	CMHFile file;

	if(file.Init(ChiFileName,"rb",
		MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR|
		MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT) == FALSE)
		return;
	
	WORD MotionNum;
	while(MotionNum=file.GetWord())
	{	
		ASSERT(MotionNum <= MaxAniNum);
		m_AniInfo[MotionNum].EngineMotionIndex = file.GetWord();
		m_AniInfo[MotionNum].StartFrame = file.GetWord();
		m_AniInfo[MotionNum].EndFrame = file.GetWord();
	}
	file.Release();
*/
}

CAniInfo::~CAniInfo()
{
	if(m_AniInfo)
	{
		delete [] m_AniInfo;
		m_AniInfo = NULL;
	}
}

