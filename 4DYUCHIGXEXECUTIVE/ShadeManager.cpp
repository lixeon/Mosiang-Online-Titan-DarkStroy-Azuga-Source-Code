#include "ShadeManager.h"
#include "GXMapObject.h"
#include <process.h>
#include "global_variable.h"

UINT WINAPI ShadeThread(LPVOID lpVoid);



CShadeManager::CShadeManager()
{
	memset(this,0,sizeof(CShadeManager));
	Initialize();
}
	
BOOL CShadeManager::Initialize()
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	m_dwMaxThreadNum = systemInfo.dwNumberOfProcessors;
	
	

	UINT		uiThreadID;
	for (DWORD i=0; i<m_dwMaxThreadNum; i++)
	{
		m_hThreadSuspend[i] = CreateEvent(NULL,FALSE,FALSE,NULL);
		m_hThreadStandby[i] = CreateEvent(NULL,FALSE,FALSE,NULL);
		
		m_Arg[i].dwAction = 0;
		m_Arg[i].dwIndex = i;
		m_Arg[i].hSuspend = m_hThreadSuspend[i];
		m_Arg[i].hStandby = m_hThreadStandby[i];

		m_hThread[i] = (HANDLE)_beginthreadex(NULL,NULL,ShadeThread,(void*)(m_Arg+i),NULL,&uiThreadID);
	}

	
	return TRUE;
}
BOOL CShadeManager::InsertShadeItem(CGXMapObject* pMapObj,VECTOR3* pv3,DWORD dwTriCount,LIGHT_DESC* pLight,SHADE_FUNC pFunc)
{
	
	DWORD dwIndex = WaitForMultipleObjects(m_dwMaxThreadNum,m_hThreadStandby,FALSE,INFINITE);
#ifdef _DEBUG
	if (dwIndex == 0xffffffff)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"CShadeManager::InsertShadeItem(), if (dwIndex == 0xffffffff), File:%s , Line:%d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif

	m_Arg[dwIndex].dwAction = 0;
	m_Arg[dwIndex].pLight = pLight;
	m_Arg[dwIndex].pMapObj = pMapObj;
	m_Arg[dwIndex].pShadeFunc = pFunc;

	if (dwTriCount)
	{
		m_Arg[dwIndex].dwTriCount = dwTriCount;
		m_Arg[dwIndex].pv3 = new VECTOR3[dwTriCount*3];
		memcpy(m_Arg[dwIndex].pv3,pv3,sizeof(VECTOR3)*dwTriCount*3);
	}
	
	SetEvent(m_Arg[dwIndex].hSuspend);


		
	return TRUE;
}
void CShadeManager::WaitShading()
{
	WaitForMultipleObjects(m_dwMaxThreadNum,m_hThreadStandby,TRUE,INFINITE);
	for (DWORD i=0; i<m_dwMaxThreadNum; i++)
	{
		SetEvent(m_hThreadStandby[i]);
	}
}

CShadeManager::~CShadeManager()
{
	for (DWORD i=0; i<m_dwMaxThreadNum; i++)
	{
		m_Arg[i].dwAction = 0xffffffff;
		SetEvent(m_hThreadSuspend[i]);
	}
	WaitForMultipleObjects(m_dwMaxThreadNum,m_hThread,TRUE,INFINITE);

	for (i=0; i<m_dwMaxThreadNum; i++)
	{
		CloseHandle(m_hThread[i]);
		CloseHandle(m_hThreadStandby[i]);
		CloseHandle(m_hThreadSuspend[i]);
	}
}

UINT WINAPI ShadeThread(LPVOID lpVoid)
{
	SHADE_ITEM_ARG*	pArg = (SHADE_ITEM_ARG*)lpVoid;
	SetEvent(pArg->hStandby);

	while (TRUE)
	{
		WaitForSingleObject(pArg->hSuspend,INFINITE);
		if (pArg->dwAction == 0xffffffff)
			goto lb_exit;

		
		pArg->pMapObj->ShadeLightMapObject(pArg->pv3,pArg->dwTriCount,pArg->pLight,pArg->pShadeFunc);

		if (pArg->pv3)
		{
			delete [] pArg->pv3;
			pArg->pv3 = NULL;
		}

		SetEvent(pArg->hStandby);

	}
lb_exit:
	_endthreadex(0);

	return 0;
}