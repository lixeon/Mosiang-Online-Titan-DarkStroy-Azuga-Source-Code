#ifndef SHADE_MANAGER_H
#define SHADE_MANAGER_H

#include "../4DyuchiGXGFunc/global.h"

#define MAX_THREAD_NUM	32

class CGXMapObject;

struct SHADE_ITEM_ARG
{
	CGXMapObject*	pMapObj;
	VECTOR3*		pv3;
	DWORD			dwTriCount;
	LIGHT_DESC*		pLight;
	SHADE_FUNC		pShadeFunc;

	DWORD			dwAction;
	DWORD			dwIndex;
	HANDLE			hSuspend;
	HANDLE			hStandby;
};

class CShadeManager
{
	SHADE_ITEM_ARG	m_Arg[MAX_THREAD_NUM];
	HANDLE	m_hThread[MAX_THREAD_NUM];
	HANDLE	m_hThreadStandby[MAX_THREAD_NUM];
	HANDLE	m_hThreadSuspend[MAX_THREAD_NUM];
	DWORD	m_dwMaxThreadNum;
	
	BOOL CShadeManager::Initialize();
public:
	
	BOOL	InsertShadeItem(CGXMapObject* pMapObj,VECTOR3* pv3,DWORD dwTriCount,LIGHT_DESC* pLight,SHADE_FUNC pFunc);
	void	WaitShading();
	CShadeManager();
	~CShadeManager();

	

		

};
#endif

