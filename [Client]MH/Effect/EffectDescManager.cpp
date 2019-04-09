// EffectDescManager.cpp: implementation of the CEffectDescManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectDescManager.h"

#include "Effect.h"
#include "EffectDesc.h"
#include "..\MHFile.h"

#include "..\DirectoryManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectDescManager::CEffectDescManager()
{
	m_MaxEffectDesc = 0;
	m_pEffectDescArray = NULL;

}

CEffectDescManager::~CEffectDescManager()
{
	Release();
}

void CEffectDescManager::Release()
{
	if(m_pEffectDescArray)
	{
		delete [] m_pEffectDescArray;
		m_pEffectDescArray = NULL;
	}
}

BOOL CEffectDescManager::LoadEffectList(int EffectKind,CMHFile* pFile)
{
	m_MaxEffectDesc = pFile->GetDword();
	m_pEffectDescArray = new CEffectDesc[m_MaxEffectDesc];
	
	DWORD effnum;
	char efffile[256];
	CMHFile EffFile;
	BOOL bSuc;
	for(DWORD n=0;n<m_MaxEffectDesc;++n)
	{
		//ASSERT(!pFile->IsEOF());

		effnum = pFile->GetDword();
		pFile->GetString(efffile);
		if(strcmp(efffile,"NULL") == 0)
			continue;
		
		DIRECTORYMGR->SetLoadMode(eLM_EffectScript);

#ifdef _FILE_BIN_
		bSuc = EffFile.Init(efffile,"rb",
			MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR|
			MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);
#else
		bSuc = EffFile.Init(efffile,"r",
			MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR|
			MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);
#endif	
		DIRECTORYMGR->SetLoadMode(eLM_Root);

		
		if(bSuc == FALSE)
			continue;
		
//		if(effnum == 1321)
//		{
//			int a=0;
//		}
		if(m_pEffectDescArray[effnum].LoadEffectDesc(EffectKind,&EffFile) == FALSE)
		{
			char mes[256];
			sprintf(mes,"Error In Loding '%s' File... plz Check it!!",efffile);
			MessageBox(NULL,mes,0,0);
		}
		
		EffFile.Release();
	}

	return TRUE;
}

BOOL CEffectDescManager::GetEffect(int EffectDescNum,EFFECTPARAM* pParam,CEffect* pOutEffect,BOOL bWarning)
{
	if(EffectDescNum == -1)
		return FALSE;

	if((DWORD)EffectDescNum >= m_MaxEffectDesc)
	{
		ASSERTMSG(0,"연출번호가 이펙트 리스트의 최대갯수보다 큽니다. 확인해주세요");//pjslocal
		return FALSE;
	}

	if(!(m_pEffectDescArray[EffectDescNum].GetMaxEffectUnitDesc() != 0 &&
		m_pEffectDescArray[EffectDescNum].GetMaxEffectTriggerDesc() != 0))
	{
		if(bWarning)
		{
			char temp[256];
			sprintf(temp, "비어있는 연출 파일(%d)을 참조했습니다. 확인해주세요",EffectDescNum);
			ASSERTMSG(0,temp);
		}
		return FALSE;
	}
	
	return m_pEffectDescArray[EffectDescNum].GetEffect(pParam,pOutEffect);
}


CEffectDesc* CEffectDescManager::GetEffectDesc(int EffectDescNum)
{
	if(EffectDescNum == -1)
		return NULL;

	if((DWORD)EffectDescNum >= m_MaxEffectDesc)
	{
		return NULL;
	}

	if(!(m_pEffectDescArray[EffectDescNum].GetMaxEffectUnitDesc() != 0 &&
		m_pEffectDescArray[EffectDescNum].GetMaxEffectTriggerDesc() != 0))
	{
		return NULL;
	}
	
	return &m_pEffectDescArray[EffectDescNum];
}
