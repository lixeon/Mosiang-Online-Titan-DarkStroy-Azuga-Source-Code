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

BOOL CEngineEffect::Init(char* filename,CObjectBase* pObject,BYTE EngineObjectType,WORD StartFrame,WORD EndFrame)
{	
	Release();
	
	m_NotProcessedAnimateKey = 0;
	m_bMotionPause = FALSE;
	m_pObject = pObject;

	DIRECTORYMGR->SetLoadMode(eLM_Effect);


	//////////////////////////////////////////////////////////////////////////
	// ÀÓ½Ã ¼³Á¤	
	//PreLoadObject(filename);	
	//////////////////////////////////////////////////////////////////////////	

	PreLoadObject( filename );

	m_GXOHandle = GetObjectHandle(filename,MHEffectPROC,this,GXOBJECT_CREATE_TYPE_EFFECT);
	strcpy(m_ObjectName,filename);

	if(m_GXOHandle == NULL)
	{
		FILE* fp = fopen("./Log/MissedObjectList.txt","a+");
		if(fp)
		{
			fprintf(fp,"%s\n",filename);
			fclose(fp);
		}
		DIRECTORYMGR->SetLoadMode(eLM_Root);
		return FALSE;
	}
	
	m_AniInfo = m_staAniMgr.GetAniInfoObj(filename,m_GXOHandle,StartFrame,EndFrame);

	DIRECTORYMGR->SetLoadMode(eLM_Root);

	ChangeCustumMotion(1,StartFrame,EndFrame,TRUE);

	DisablePick();
	if(m_pObject)
	{
		m_pObject->m_EngineObjectType = EngineObjectType;
		if(m_pObject->GetEngineObjectType() == eEngineObjectType_SkillObject)
			EnablePick();
	}
	
	return TRUE;
}

void CEngineEffect::Release()
{
	CEngineObject::Release();
}

DWORD __stdcall MHEffectPROC(I4DyuchiGXExecutive* pExecutive, GXMAP_OBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData)
{
	CEngineEffect* pObject = (CEngineEffect*)pData;
	if(pObject == NULL)
		return 0;
	if(pObject->IsInited() == FALSE)
		return 0;
	
	pObject->Animate(arg1);
	if(pObject->IsEndMotion() && pObject->IsRepeat() == FALSE)
	{
		pObject->Hide();
	}


	return 0;
}