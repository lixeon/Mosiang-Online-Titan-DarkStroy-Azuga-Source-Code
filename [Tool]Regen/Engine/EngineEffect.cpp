// EngineEffect.cpp: implementation of the CEngineEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineEffect.h"
#include "GraphicEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngineEffect::CEngineEffect()
{
	m_ObjectName[0] = 0;
}

CEngineEffect::~CEngineEffect()
{
	Release();
}

BOOL CEngineEffect::Init(char* filename,CObjectBase* pObject)
{
	m_CurMotionNum = -1;
	m_bMotionPause = FALSE;
	m_pObject = pObject;

	DIRECTORYMGR->SetLoadMode(eLM_Effect);
	m_GXOHandle = GetObjectHandle(filename,MHEffectPROC,this,GXOBJECT_CREATE_TYPE_EFFECT);
	strcpy(m_ObjectName,filename);

	if(m_GXOHandle == NULL)
	{		
		char temp[256];
		sprintf(temp,"¿ÀºêÁ§Æ® »ý¼º ½ÇÆÐ : %s",filename);
		MessageBox(NULL,temp,NULL,NULL);
		DIRECTORYMGR->SetLoadMode(eLM_Root);
		return FALSE;		
	}
	
	//g_pExecutive->DisablePick(m_GXOHandle);
	//EnableShadow(FALSE);
	
	m_AniInfo = new ANIINFO[2];

	for(WORD n=0;n<2;++n)
	{
		m_AniInfo[n].EngineMotionIndex = n;
		m_AniInfo[n].StartFrame = 0;
		m_AniInfo[n].EndFrame = -1;
	}

	DIRECTORYMGR->SetLoadMode(eLM_Root);
	return TRUE;
}

void CEngineEffect::Release()
{
	if(m_GXOHandle)
	{
//		Hide();
//		AddPool(m_GXOHandle,m_ObjectName);
		AddGarbageObject(m_GXOHandle);
		m_GXOHandle = NULL;
	}
	if(m_AniInfo)
	{
		delete [] m_AniInfo;
		m_AniInfo = NULL;
	}
}

DWORD __stdcall MHEffectPROC(I4DyuchiGXExecutive* pExecutive, GXMAP_OBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData)
{
	CEngineEffect* pObject = (CEngineEffect*)pData;
	if(pObject == NULL)
		return 0;
	
	pObject->Animate(arg1);
	if(pObject->IsEndMotion())
		pObject->Hide();
	//pObject->m_pObject->Process();


	return 0;
}