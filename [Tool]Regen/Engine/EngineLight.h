// EngineLight.h: interface for the CEngineLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINELIGHT_H__FAC16136_501A_42C7_8168_EB3122A654A4__INCLUDED_)
#define AFX_ENGINELIGHT_H__FAC16136_501A_42C7_8168_EB3122A654A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphicEngine.h"

class CEngineObject;

class CEngineLight  
{
	GXLIGHT_HANDLE	m_GXLHandle;
	DWORD m_StartTime;
	DWORD m_LightTime;

public:
	CEngineLight();
	virtual ~CEngineLight();

	BOOL InitNormalLight(int lightindex,float radius,VECTOR3* pPos,DWORD lighttime = 0xffffffff);	// 0xffffffff ÀÌ¸é ¹«ÇÑ´ë·Î ÄÑÀÖÀ½..
	BOOL InitShadowLight(int lightindex,VECTOR3* pFrom,VECTOR3* pTo,float fFov,float Radius);
	void Release();

	BOOL LinkToObject(CEngineObject* pObject,char* szObjectName,VECTOR3* pLinkOffset);
	BOOL AttachToObject(CEngineObject* pObject,char* szObjectName);
	void DetachFromObject(CEngineObject* pObject);

	void GetLightDesc(LIGHT_DESC* pDesc);
	void SetLightDesc(LIGHT_DESC* pDesc);

	void SetPosition(VECTOR3* pPos);
	void SetRadius(float radius);

	friend DWORD __stdcall MHLightPROC(I4DyuchiGXExecutive* pExecutive, GXLIGHT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData);
};

#endif // !defined(AFX_ENGINELIGHT_H__FAC16136_501A_42C7_8168_EB3122A654A4__INCLUDED_)
