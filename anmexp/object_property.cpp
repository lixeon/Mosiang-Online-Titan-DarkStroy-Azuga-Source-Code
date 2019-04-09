#include "object_property.h"
#include "resource.h"
#include "../max_common/UserDefine.h"
#include "../4DyuchiGRX_Common/motion_flag.h"

CObjectPropertyList::CObjectPropertyList()
{
	memset(this,0,sizeof(CObjectPropertyList));
}

BOOL CObjectPropertyList::InsertObject(TCHAR* uszObjName,DWORD flag)
{
	TCHAR	txt[512];
	memset(txt,0,512);

	OBJECT_PROPERTY_ITEM*	pItem = m_pObjectTable + m_dwObjectNum;
	
	_tcscpy(pItem->uszObjectName,uszObjName);
	pItem->dwPropertyFlag = *(DWORD*)&flag;

	if (!VBHInsert(m_pHash,(DWORD)pItem,uszObjName,lstrlen(uszObjName)))
		__asm int 3
	
	CreateMotionTextItem(txt,*(CMotionFlag*)&flag,uszObjName);
	SendDlgItemMessage(m_hDlg,IDC_LIST_OBJECTS, LB_ADDSTRING, 0,(DWORD)txt);         

	m_dwObjectNum++;

	return TRUE;
}
BOOL CObjectPropertyList::Initialize(DWORD dwMaxNum)
{
	m_pObjectTable = new OBJECT_PROPERTY_ITEM[dwMaxNum];
	memset(m_pObjectTable,0,sizeof(OBJECT_PROPERTY_ITEM)*dwMaxNum);

	m_dwMaxItemNum = dwMaxNum;
	
	m_pHash = VBHCreate();
	VBHInitialize(m_pHash,64,MAX_NAME_LEN,m_dwMaxItemNum);

	return TRUE;
}

OBJECT_PROPERTY_ITEM* CObjectPropertyList::GetProperty(char* szObjName)
{
	OBJECT_PROPERTY_ITEM*	pItem = NULL;

	VBHSelect(m_pHash,(DWORD*)&pItem,1,szObjName,lstrlen(szObjName));
	return pItem;

}


BOOL CObjectPropertyList::SetProperty(int iLBIndex,DWORD flag)
{
	OBJECT_PROPERTY_ITEM*	pItem = NULL;

	char	szFlag[MAX_NAME_LEN+4];
	memset(szFlag,0,sizeof(szFlag));

	char	szObjName[MAX_NAME_LEN];
	memset(szObjName,0,sizeof(szObjName));

	if (LB_ERR == SendDlgItemMessage(m_hDlg,IDC_LIST_OBJECTS,LB_GETTEXT,iLBIndex,(LPARAM)szFlag))
		__asm int 3


//	char	seps[]   = "-";
//	lstrcpy(szObjName,szFlag);
//	strtok( szObjName, seps );
	memcpy(szObjName,szFlag,lstrlen(szFlag)-4);
	
	if (!VBHSelect(m_pHash,(DWORD*)&pItem,1,szObjName,lstrlen(szObjName)))
		return FALSE;

//	iLBIndex = SendDlgItemMessage(m_hDlg,IDC_LIST_OBJECTS,LB_FINDSTRING,0,(LPARAM)szFlag);         


	memset(szFlag,0,sizeof(szFlag));
	CreateMotionTextItem(szFlag,*(CMotionFlag*)&flag,szObjName);
	pItem->dwPropertyFlag = *(DWORD*)&flag;
	SendDlgItemMessage(m_hDlg,IDC_LIST_OBJECTS,LB_DELETESTRING,iLBIndex,0);         
	SendDlgItemMessage(m_hDlg,IDC_LIST_OBJECTS,LB_INSERTSTRING,iLBIndex,(DWORD)szFlag);         

	

	return TRUE;
	
}


BOOL CObjectPropertyList::DisplayItem(int iLBIndex)
{
	OBJECT_PROPERTY_ITEM*	pItem = NULL;

	char	szFlag[MAX_NAME_LEN+4];
	memset(szFlag,0,sizeof(szFlag));

	char	szObjName[MAX_NAME_LEN];
	memset(szObjName,0,sizeof(szObjName));

	if (LB_ERR == SendDlgItemMessage(m_hDlg,IDC_LIST_OBJECTS,LB_GETTEXT,iLBIndex,(LPARAM)szFlag))
		__asm int 3


//	char	seps[]   = "-";
//	lstrcpy(szObjName,szFlag);
//	strtok( szObjName, seps );
	memcpy(szObjName,szFlag,lstrlen(szFlag)-4);

	
	if (!VBHSelect(m_pHash,(DWORD*)&pItem,1,szObjName,lstrlen(szObjName)))
		return FALSE;

//	iLBIndex = SendDlgItemMessage(m_hDlg,IDC_LIST_OBJECTS,LB_FINDSTRING,0,(LPARAM)szFlag);         


	memset(szFlag,0,sizeof(szFlag));
	CreateMotionTextItem(szFlag,*(CMotionFlag*)&pItem->dwPropertyFlag,szObjName);
	
	SendDlgItemMessage(m_hDlg,IDC_LIST_OBJECTS,LB_DELETESTRING,iLBIndex,0);         
	SendDlgItemMessage(m_hDlg,IDC_LIST_OBJECTS,LB_INSERTSTRING,iLBIndex,(DWORD)szFlag);         

	

	return TRUE;
	
}
DWORD* CObjectPropertyList::GetPropertyPtr(char* szObjName)
{
	OBJECT_PROPERTY_ITEM*	pItem = NULL;

	VBHSelect(m_pHash,(DWORD*)&pItem,1,szObjName,lstrlen(szObjName));
	return &pItem->dwPropertyFlag;

}

DWORD* CObjectPropertyList::GetPropertyPtr(int iLBIndex)
{
	OBJECT_PROPERTY_ITEM*	pItem = NULL;
	
	char	szFlag[MAX_NAME_LEN+4];
	memset(szFlag,0,sizeof(szFlag));

	char	szObjName[MAX_NAME_LEN];
	memset(szObjName,0,sizeof(szObjName));

	if (LB_ERR == SendDlgItemMessage(m_hDlg,IDC_LIST_OBJECTS,LB_GETTEXT,iLBIndex,(LPARAM)szFlag))
		__asm int 3



	memcpy(szObjName,szFlag,lstrlen(szFlag)-4);
//	char	seps[]   = "-";
//	lstrcpy(szObjName,szFlag);
//	strtok( szObjName, seps );
	
	if (!VBHSelect(m_pHash,(DWORD*)&pItem,1,szObjName,lstrlen(szObjName)))
		return NULL;

	
	return &pItem->dwPropertyFlag;

}
CObjectPropertyList::~CObjectPropertyList()
{
	if (m_pHash)
	{

		VBHDeleteAll(m_pHash);
		VBHRelease(m_pHash);
	}

	if (m_pObjectTable)
	{
		delete [] m_pObjectTable;
		m_pObjectTable = NULL;
	}
}