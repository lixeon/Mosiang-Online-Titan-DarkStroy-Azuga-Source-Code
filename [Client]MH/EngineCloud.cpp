// EngineCloud.cpp: implementation of the CEngineCloud class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineCloud.h"
#include "MHFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DWORD CEngineCloud::m_MaxCloudFileNum;
StaticString* CEngineCloud::m_CloudFileName = NULL;
VECTOR3 CEngineCloud::m_CloudMoveVector;
int CEngineCloud::m_CloudHeightMax;
int CEngineCloud::m_CloudHeightMin;

CEngineCloud::CEngineCloud()
{
	m_AniInfo = NULL;
}

CEngineCloud::~CEngineCloud()
{

}

void CEngineCloud::LoadCloudList(char* listFile)
{
	ReleaseCloudList();
	
	DIRECTORYMGR->SetLoadMode(eLM_Cloud);

	CMHFile file;
	BOOL rt = file.Init(listFile,"ET",
		MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);
	if(rt == FALSE)
	{
		m_MaxCloudFileNum = 0;
		DIRECTORYMGR->SetLoadMode(eLM_Root);
		return;
	}

	m_MaxCloudFileNum = file.GetDword();
	m_CloudFileName = new StaticString[m_MaxCloudFileNum];
	for(DWORD n=0;n<m_MaxCloudFileNum;++n)
	{
		m_CloudFileName[n] = file.GetString();
	}

	DIRECTORYMGR->SetLoadMode(eLM_Root);
}

void CEngineCloud::ReleaseCloudList()
{
	if(m_CloudFileName)
	{
		delete [] m_CloudFileName;
		m_CloudFileName = NULL;
	}
	m_MaxCloudFileNum = 0;
}

void CEngineCloud::SetCloudVelocity(VECTOR3* pVel)
{
	m_CloudMoveVector = *pVel;
}

void CEngineCloud::SetCloudHeight(int minv,int maxv)
{
	m_CloudHeightMax = maxv;
	m_CloudHeightMin = minv;
}

BOOL CEngineCloud::Init(DWORD CloudNum,float x,float z)
{
	ASSERT(CloudNum < m_MaxCloudFileNum);

	DIRECTORYMGR->SetLoadMode(eLM_Cloud);
	m_GXOHandle = GetObjectHandle(m_CloudFileName[CloudNum],MHCloudPROC,this,GXOBJECT_CREATE_TYPE_EFFECT);

	if(m_GXOHandle == NULL)
	{
		char temp[256];
		sprintf(temp,"Fail To Create Cloud : %s",m_CloudFileName[CloudNum]);
		MessageBox(NULL,temp,NULL,NULL);
		DIRECTORYMGR->SetLoadMode(eLM_Root);
		return FALSE;		
	}
		
	DIRECTORYMGR->SetLoadMode(eLM_Root);

	m_LastCalcedTime = gCurTime;
//	m_Position.x = float(rand()%51200);
	m_Position.x = float((rand()%512)*100);
	int gap = m_CloudHeightMax - m_CloudHeightMin + 1;
	m_Position.y = float(rand()%gap)+m_CloudHeightMin;
//	m_Position.z = float(rand()%51200);
	m_Position.z = float((rand()%512)*100);

	SetEngObjPosition(&m_Position);
	
	return TRUE;
}

void CEngineCloud::Release()
{
	if(m_GXOHandle)
	{
		AddGarbageObject(m_GXOHandle);
		m_GXOHandle = NULL;
	}
	if(m_AniInfo)
	{
		m_AniInfo = NULL;
	}
}

DWORD __stdcall MHCloudPROC(I4DyuchiGXExecutive* pExecutive, GXOBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData)
{
	CEngineCloud* pObject = (CEngineCloud*)pData;
	if(pObject == NULL)
		return 0;

	// Cloud has no animation
	// pObject->Animate();

	DWORD elapsed = gCurTime - pObject->m_LastCalcedTime;
	float ftime = elapsed*0.001f;
	pObject->m_LastCalcedTime = gCurTime;
	
	pObject->m_Position = pObject->m_CloudMoveVector * ftime + pObject->m_Position;


#define MAP_WIDTH 51200
#define SIGHT_DIS 10000 
	if(pObject->m_Position.x > MAP_WIDTH+SIGHT_DIS)
		pObject->m_Position.x = -SIGHT_DIS;
	if(pObject->m_Position.z > MAP_WIDTH+SIGHT_DIS)
		pObject->m_Position.z = -SIGHT_DIS;

	if(pObject->m_Position.x < -SIGHT_DIS)
		pObject->m_Position.x = MAP_WIDTH+SIGHT_DIS;
	if(pObject->m_Position.z < -SIGHT_DIS)
		pObject->m_Position.z = MAP_WIDTH+SIGHT_DIS;

	pObject->SetEngObjPosition(&pObject->m_Position);
	
	return 0;
}
