#pragma once

#include "../4DyuchiGRX_common/stdafx.h"
#include "motion.h"
#include "../4DyuchiGXGFunc/global.h"
#include "CoGeometry.h"
#include "global_variable.h"

CMotion::CMotion()
{
	memset((char*)this+4,0,sizeof(CMotion)-4);

}

STDMETHODIMP_(ULONG) CMotion::AddRef(void)
{
	m_dwRefCount++;
	return 0;
}
STDMETHODIMP_(ULONG) CMotion::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
	{
		delete this;
	}
	return 0;
}

STDMETHODIMP CMotion::QueryInterface(REFIID refiid, PPVOID ppv)
{

	return E_NOINTERFACE;
	
}
BOOL CMotion::GetMotionDesc(MOTION_DESC* pDesc)
{
	pDesc->dwTicksPerFrame = m_dwTicksPerFrame;
	pDesc->dwFirstFrame = m_dwFirstFrame;
	pDesc->dwLastFrame = m_dwLastFrame;
	pDesc->dwFrameSpeed = m_dwFrameSpeed;
	pDesc->dwKeyFrameStep = m_dwKeyFrameStep;
	lstrcpy(pDesc->szMotionName,m_szMotionFileName);
	
	return TRUE;
}
CMotionObject* CMotion::AllocMotionObject()
{
	CMotionObject*	pMotObj = new CMotionObject;
	pMotObj->m_pNextMotionObject = m_pBaseMotionObject;
	m_pBaseMotionObject = pMotObj;
	m_dwMotionObjectNum++;

	return pMotObj;
}
BOOL CMotion::ReadFile(char* szFileName)
{
	DWORD		i;
	BOOL		bResult = FALSE;
	DWORD		size;
	DWORD		type;
	CMotionObject*	pMotObj;
	DWORD		dwNameLen;

	void* pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_BINARY);
	if (!pFP)
	{
		// FILE_NOT_FOUND /////////////////////////////////////////////////////////
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_FILE_NOT_FOUND,1,(void*)dwAddr,szFileName);
		///////////////////////////////////////////////////////////////////////////

		goto lb_return;
	}

	FILE_MOTION_HEADER	header;
	
	g_pFileStorage->FSRead(pFP,&header,sizeof(FILE_MOTION_HEADER));

	m_dwTicksPerFrame = header.dwTicksPerFrame;
	m_dwFirstFrame = header.dwFirstFrame;
	m_dwLastFrame = header.dwLastFrame;
	m_dwFrameSpeed = header.dwFrameSpeed;
	m_dwKeyFrameStep = header.dwKeyFrameStep;
	m_dwMotionObjectNum = header.dwMotionObjectNum;

//	lstrcpy(m_szMotionName,header.szMotionName);
	lstrcpy(m_szMotionFileName,szFileName);
	

	for (i=0; i<header.dwMotionObjectNum; i++)
	{
		g_pFileStorage->FSRead(pFP,&type,sizeof(DWORD));

		pMotObj = AllocMotionObject();

		g_pFileStorage->FSRead(pFP,&size,sizeof(DWORD));

		pMotObj->ReadFile(pFP);
		pMotObj->SetKeyFrameStep(m_dwKeyFrameStep);
		dwNameLen = lstrlen(pMotObj->GetObjectName());

		if (m_dwMaxNameLen < dwNameLen)
			m_dwMaxNameLen = dwNameLen;

	}

	g_pFileStorage->FSCloseFile(pFP);



	m_pHashObjName = VBHCreate();
	VBHInitialize(m_pHashObjName,m_dwMotionObjectNum,m_dwMaxNameLen,m_dwMotionObjectNum+1);
	
	pMotObj = m_pBaseMotionObject;
	void*	pHashHandle;
	while(pMotObj)
	{
		pHashHandle = VBHInsert(m_pHashObjName,(DWORD)pMotObj,pMotObj->GetObjectName(),pMotObj->GetNameLen());
#ifdef _DEBUG
		if (!pHashHandle)
		{
			char	txt[512];
			memset(txt,0,512);
			wsprintf(txt,"CMotion::ReadFile(), if (!pHashHandle), File:%s , Line:%d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
#endif

		pMotObj = pMotObj->m_pNextMotionObject;
	}

	bResult = TRUE;
lb_return:
	return bResult;
}

CMotionObject* CMotion::GetMotionObject(char* szObjName,DWORD dwSize)
{
	DWORD	dwResult = 0;
	CMotionObject*	pMotObj = NULL;
	
 	dwResult = VBHSelect(m_pHashObjName,(DWORD*)&pMotObj,1,szObjName,dwSize);

	return pMotObj;


}
BOOL CMotion::Unload()
{
	BOOL	bResult = FALSE;
	if (!m_pGeometry)
		goto lb_return;
	
	if (!m_dwIndexInGeometry)
		goto lb_return;

	
	bResult = m_pGeometry->UnloadPreLoadedItem(m_dwIndexInGeometry);
lb_return:
	return bResult;
	
}
	
void CMotion::ReleaseObjects()
{
	CMotionObject* pCur = m_pBaseMotionObject;
	CMotionObject* pNext;
	
	while (pCur)
	{
		pNext = pCur->m_pNextMotionObject;
		delete pCur;
		pCur = pNext;
	}
	m_pBaseMotionObject = NULL;
	m_dwMotionObjectNum = 0;
}

CMotion::~CMotion()
{

	if (m_pGeometry)
	{
		if (m_pGeometry->INL_GetRenderer())
			m_pGeometry->INL_GetRenderer()->ClearCacheWithMotionUID(this);
	}
	
	if (m_pHashObjName)
		VBHRelease(m_pHashObjName);

	ReleaseObjects();
	if (m_dwIndexInGeometry)
		m_pGeometry->DeleteFileItem(m_dwIndexInGeometry);
}