#include "object.h"
#include "../4DyuchiGXGFunc/global.h"

CBaseObject::CBaseObject()
{
	m_objType = OBJECT_TYPE_UNKNOWN;
	m_dwIndex = 0;
	m_pParentObject = NULL;
	m_pChildObjectIndexList = NULL;
	m_dwChildObjectNum = 0;

	SetIdentityMatrix(&m_matResult);
	

	m_ppMatrixList = NULL;
	m_dwMatrixNum = 0;

	memset(&m_TM,0,sizeof(NODE_TM));
	memset(m_uszObjName,0,sizeof(m_uszObjName));
	memset(m_uszParentObjectName,0,sizeof(m_uszParentObjectName));
	m_dwParentObjectIndex = 0xffffffff;
}
BOOL CBaseObject::CreateChildObjectList()
{
	if (!m_dwChildObjectNum)
		return FALSE;

	m_pChildObjectIndexList = new DWORD[m_dwChildObjectNum];
	memset(m_pChildObjectIndexList,0,sizeof(DWORD)*m_dwChildObjectNum);
	m_dwChildObjectNum = 0;
	return TRUE;
}
BOOL CBaseObject::InsertChildObjectIndex(DWORD dwIndex)
{
	m_pChildObjectIndexList[m_dwChildObjectNum] = dwIndex;
	m_dwChildObjectNum++;
	return TRUE;
}
DWORD CBaseObject::WriteFile(FILE* fp)
{
	DWORD	oldPos = ftell(fp);
	
	FILE_BASE_OBJECT_HEADER* pHeader = (FILE_BASE_OBJECT_HEADER*)&(this->m_dwIndex);
	fwrite(pHeader,sizeof(FILE_BASE_OBJECT_HEADER),1,fp);
	if (m_dwChildObjectNum)
		fwrite(m_pChildObjectIndexList,sizeof(DWORD),m_dwChildObjectNum,fp);
	
	return (ftell(fp)-oldPos);
	
}
CBaseObject::~CBaseObject()
{
	if (m_pChildObjectIndexList)
	{
		delete [] m_pChildObjectIndexList;
		m_pChildObjectIndexList = NULL;
	}
}

