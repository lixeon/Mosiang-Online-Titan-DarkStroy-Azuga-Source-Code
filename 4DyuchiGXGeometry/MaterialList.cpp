#include "MaterialList.h"
#include "global_variable.h"
#include "../4DyuchiGRX_Common/IFileStorage.h"

CMaterialList::CMaterialList()
{
	memset(this,0,sizeof(CMaterialList));
}


BOOL CMaterialList::Initialize(DWORD dwMtlNum)
{
	m_dwMaxMtlNum = dwMtlNum;
	dwMtlNum++;

	DWORD	dwSize = sizeof(MTL_HANDLE)*dwMtlNum;

	m_pMtlList = new MTL_HANDLE[dwMtlNum];
	memset(m_pMtlList,0,sizeof(MTL_HANDLE)*dwMtlNum);

	return TRUE;
}
BOOL CMaterialList::BeginBulidMaterialList(DWORD dwMaxMtlNum)
{
	m_pHashMtl = VBHCreate();
	VBHInitialize(m_pHashMtl,40,sizeof(CMaterial),dwMaxMtlNum);

	return TRUE;
}

BOOL CMaterialList::SetMaterial(CMaterial* pMtl,DWORD dwMtlIndex)
{
	BOOL		bResult = FALSE;

	if (dwMtlIndex >= m_dwMaxMtlNum)
		goto lb_return;

	m_pMtlList[dwMtlIndex].Mtl = *pMtl;

	bResult = TRUE;

lb_return:
	return bResult;
}
DWORD CMaterialList::AddMaterial(MATERIAL* pMtl)
{
	DWORD		dwMtlIndex = 0xffffffff;

	if (!VBHSelect(m_pHashMtl,&dwMtlIndex,1,(void*)pMtl,sizeof(MATERIAL)))
	{
		if (m_dwMtlNum >= m_dwMaxMtlNum)
			goto lb_return;

		m_pMtlList[m_dwMtlNum].Mtl.SetMateiralInfo(pMtl);
		
		dwMtlIndex = m_dwMtlNum;
		m_pMtlList[m_dwMtlNum].Mtl.SetIndex(dwMtlIndex);
		
		VBHInsert(m_pHashMtl,dwMtlIndex,pMtl,sizeof(MATERIAL));
		m_dwMtlNum++;
	}	

lb_return:
	return dwMtlIndex;
}
CMaterial* CMaterialList::GetMaterial(DWORD dwMtlIndex)
{
	CMaterial* pMtl = NULL;

	if (dwMtlIndex >= m_dwMaxMtlNum)
		goto lb_return;

	pMtl = &m_pMtlList[dwMtlIndex].Mtl;

lb_return:
	return pMtl;
}
DWORD CMaterialList::CreateMaterialTable(MATERIAL_TABLE** ppMtlTableList)
{
	DWORD	dwNum = 0;
	MATERIAL_TABLE*	pMtlTable = (MATERIAL_TABLE*)malloc(sizeof(MATERIAL_TABLE)*m_dwMtlNum);
	
	for (DWORD i=0; i<m_dwMaxMtlNum; i++)
	{

		if (OBJECT_TYPE_MATERIAL == m_pMtlList[i].Mtl.GetType())
		{	
#ifdef _DEBUG
			if (dwNum >= m_dwMtlNum)
			{
				char	txt[512];
				memset(txt,0,512);
				wsprintf(txt,"CMaterialList::CreateMaterialTable(), if (dwNum >= m_dwMtlNum), File:%s , Line:%d \n",__FILE__,__LINE__);
				DWORD	dwAddr;
				GetEIP(&dwAddr);
				g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
			}
#endif

			pMtlTable[dwNum].dwMtlIndex = i;
			pMtlTable[dwNum].pMtl = m_pMtlList[i].Mtl.GetMaterialInfo();
			dwNum++;
		}
	}
	*ppMtlTableList = pMtlTable;
	return dwNum;
}


void CMaterialList::ReleaseMaterialTable(MATERIAL_TABLE* pMtlTableList)
{
	free(pMtlTableList);
}

void CMaterialList::Release()
{
	

	if (m_pHashMtl)
	{
		VBHRelease(m_pHashMtl);
		m_pHashMtl = NULL;
	}

	if (m_pMtlList)
	{
		delete [] m_pMtlList;
		m_pMtlList = NULL;
	}
	m_dwMtlNum = 0;
	m_dwMaxMtlNum = 0;
}
DWORD CMaterialList::WriteFile(FILE* fp)
{
	DWORD	oldPos = ftell(fp);
	
	// AddMaterial()로 추가된 매터리얼들을 세이브하게된다.
	// 따라서 매터리얼들은 완전 선형적으로 중간 빈공간 없이 존재한다.

	fwrite(&m_dwMtlNum,sizeof(DWORD),1,fp);
	for (DWORD i=0; i<m_dwMtlNum; i++)
	{
		m_pMtlList[i].Mtl.WriteSTMMaterial(fp);

	}
	

	return (ftell(fp)-oldPos);
}
DWORD CMaterialList::GetIndex(char* szMtlName)
{
	DWORD dwResult = 0xffffffff;
	DWORD dwLen;
	for (DWORD i=0; i<m_dwMaxMtlNum; i++)
	{
		dwLen = lstrlen(m_pMtlList[i].Mtl.GetName());
		if (!dwLen)
			continue;

		if (!lstrcmp(szMtlName,m_pMtlList[i].Mtl.GetName()))
		{
			dwResult = i;
			goto lb_return;
		}
	}
lb_return:
	return dwResult;
}

DWORD CMaterialList::ReadFile(void* pFP)
{
	DWORD	dwLen = 0;

	Release();

	dwLen += g_pFileStorage->FSRead(pFP,&m_dwMtlNum,sizeof(DWORD));
	
	Initialize(m_dwMtlNum);
	CMaterial	mtl;
	for (DWORD i=0; i<m_dwMaxMtlNum; i++)
	{
		dwLen += mtl.ReadSTMMaterial(pFP);
		mtl.SetIndex(i);
		SetMaterial(&mtl,mtl.GetIndex());
		//fwrite(m_pMtlList,sizeof(MTL_HANDLE),m_dwMtlNum,fp);
		
	}
	return dwLen;
	
}
CMaterialList::~CMaterialList()
{
	Release();

}