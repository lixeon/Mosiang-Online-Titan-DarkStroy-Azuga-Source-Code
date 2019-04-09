// EngineCloud.h: interface for the CEngineCloud class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINECLOUD_H__4A9669C4_B5F4_402E_BE45_EEA5FEB9388C__INCLUDED_)
#define AFX_ENGINECLOUD_H__4A9669C4_B5F4_402E_BE45_EEA5FEB9388C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ENGINE\EngineObject.h"

class CEngineCloud : public CEngineObject  
{
	static DWORD m_MaxCloudFileNum;
	static StaticString* m_CloudFileName;
	static VECTOR3 m_CloudMoveVector;
	static int m_CloudHeightMax;
	static int m_CloudHeightMin;

	DWORD m_LastCalcedTime;
	VECTOR3 m_Position;
public:
	CEngineCloud();
	virtual ~CEngineCloud();

	static void LoadCloudList(char* listFile);
	static void ReleaseCloudList();
	static void SetCloudVelocity(VECTOR3* pVel);
	static void SetCloudHeight(int minv,int maxv);
	
	static DWORD GetMaxCloudKindNum()	{	return m_MaxCloudFileNum;	}

	BOOL Init(DWORD CloudNum,float x,float z);
	void Release();

	friend DWORD __stdcall MHCloudPROC(I4DyuchiGXExecutive* pExecutive, GXOBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData);
};

#endif // !defined(AFX_ENGINECLOUD_H__4A9669C4_B5F4_402E_BE45_EEA5FEB9388C__INCLUDED_)
