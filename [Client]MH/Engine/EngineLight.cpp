// EngineLight.cpp: implementation of the CEngineLight class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineLight.h"
#include "GraphicEngine.h"
#include "EngineObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngineLight::CEngineLight()
{
	m_GXLHandle = NULL;
}

CEngineLight::~CEngineLight()
{
	Release();
}

BOOL CEngineLight::InitNormalLight(int lightindex,float radius,VECTOR3* pPos,DWORD lighttime)
{
	Release();
	LIGHT_DESC	light;
	light.fRs = radius;
	m_GXLHandle = g_pExecutive->CreateGXLight(&light,MHLightPROC,this,lightindex,NULL,GXLIGHT_TYPE_TEX_PROJECTION);
	if(m_GXLHandle == NULL)
		return FALSE;
	
	g_pExecutive->GXLEnableDynamicLight(m_GXLHandle);
	m_StartTime = gCurTime;
	m_LightTime = lighttime;
	g_pExecutive->GXLSetPosition(m_GXLHandle,pPos);

	return TRUE;
}

BOOL CEngineLight::InitShadowLight(int lightindex,VECTOR3* pFrom,VECTOR3* pTo,float fFov,float Radius)
{
	LIGHT_DESC	shadowlight;
	shadowlight.dwAmbient = 0xffffffff;
	shadowlight.dwDiffuse = 0xffffffff;
	shadowlight.dwSpecular = 0xffffffff;
	shadowlight.v3Point = *pFrom;
	shadowlight.fFov = fFov;
	shadowlight.v3Up.x = 0.0f;
	shadowlight.v3Up.y = 1.0f;
	shadowlight.v3Up.z = 0.0f;
	shadowlight.fRs = Radius;
	shadowlight.v3To = *pTo;
	
	if(lightindex == -1)
		m_GXLHandle = g_pExecutive->CreateGXLight(&shadowlight,NULL,NULL,0,NULL,GXLIGHT_TYPE_ENABLE_SHADOW | GXLIGHT_TYPE_DISABLE_LIGHT_COLOR);
	else
		m_GXLHandle = g_pExecutive->CreateGXLight(&shadowlight,NULL,NULL,0,NULL,GXLIGHT_TYPE_ENABLE_SHADOW);

	if(m_GXLHandle == NULL)
		return FALSE;

	
	g_pExecutive->GXLEnableDynamicLight(m_GXLHandle);
	
	return TRUE;
}

void CEngineLight::Release()
{
	if(m_GXLHandle == NULL)
		return;
	if(m_GXLHandle && g_pExecutive)
	{
		g_pExecutive->DeleteGXLight(m_GXLHandle);
		m_GXLHandle = NULL;
	}
}

BOOL CEngineLight::LinkToObject(CEngineObject* pObject,char* szObjectName,VECTOR3* pLinkOffset)
{
	if(m_GXLHandle == NULL)
		return FALSE;
	return g_pExecutive->GXOAttachLight(pObject->m_GXOHandle,szObjectName,m_GXLHandle,pLinkOffset,ATTACH_TYPE_LINK);
}

BOOL CEngineLight::AttachToObject(CEngineObject* pObject,char* szObjectName)
{
	if(m_GXLHandle == NULL)
		return FALSE;
	VECTOR3 pos;
	pos.x = pos.y = pos.z = 0;
	return g_pExecutive->GXOAttachLight(pObject->m_GXOHandle,szObjectName,m_GXLHandle,&pos,ATTACH_TYPE_ATTACH);
}

void CEngineLight::DetachFromObject(CEngineObject* pObject)
{
	if(m_GXLHandle == NULL)
		return;
	g_pExecutive->GXODetachLight(pObject->m_GXOHandle,m_GXLHandle);
}

void CEngineLight::SetPosition(VECTOR3* pPos)
{
	if(m_GXLHandle == NULL)
		return;
	g_pExecutive->GXLSetPosition(m_GXLHandle,pPos);
}

void CEngineLight::GetLightDesc(LIGHT_DESC* pDesc)
{
	if(m_GXLHandle == NULL)
		return;
	g_pExecutive->GXLGetLightDesc(m_GXLHandle,pDesc);
}
void CEngineLight::SetLightDesc(LIGHT_DESC* pDesc)
{
	if(m_GXLHandle == NULL)
		return;
	g_pExecutive->GXLSetLightDesc(m_GXLHandle,pDesc);
}

void CEngineLight::SetRadius(float radius)
{
	if(m_GXLHandle == NULL)
		return;

	LIGHT_DESC Desc;	
	g_pExecutive->GXLGetLightDesc(m_GXLHandle,&Desc);
	Desc.fRs = radius;
	g_pExecutive->GXLSetLightDesc(m_GXLHandle,&Desc);
}

DWORD __stdcall MHLightPROC(I4DyuchiGXExecutive* pExecutive, GXLIGHT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData)
{
	CEngineLight* pLight = (CEngineLight*)pData;
	if(pLight == NULL)
		return 0;

	if(pLight->m_LightTime == 0xffffffff)
		return 0;

	DWORD timegap = gCurTime - pLight->m_StartTime;
	
	if(timegap > pLight->m_LightTime)
	{
		pLight->Release();
	}

	return 0;
}

