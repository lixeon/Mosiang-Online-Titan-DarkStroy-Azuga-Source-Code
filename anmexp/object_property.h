#pragma once

#include "../4DyuchiGRX_Common/mesh_flag.h"
#include "../4DyuchiGXGFunc/global.h"

struct OBJECT_PROPERTY_ITEM
{
	DWORD						dwPropertyFlag;
	TCHAR						uszObjectName[MAX_NAME_LEN];
};

class CObjectPropertyList
{
	VBHASH_HANDLE				m_pHash;
	DWORD						m_dwObjectNum;
	OBJECT_PROPERTY_ITEM*		m_pObjectTable;
	HWND						m_hDlg;
	DWORD						m_dwMaxItemNum;

public:
	void						SetDlgHandle(HWND hDlg)		{m_hDlg = hDlg;}
	BOOL						Initialize(DWORD dwMaxNum);
	
	BOOL						InsertObject(TCHAR* uszObjName,DWORD flag);

	OBJECT_PROPERTY_ITEM*		GetProperty(char* szObjName);
	BOOL						SetProperty(int iLBIndex,DWORD flag);

	DWORD*						GetPropertyPtr(char* szObjName);
	DWORD*						GetPropertyPtr(int iLBIndex);
	BOOL						DisplayItem(int iLBIndex);

	CObjectPropertyList();
	~CObjectPropertyList();

};
