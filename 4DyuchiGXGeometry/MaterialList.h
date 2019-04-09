#ifndef MATERIAL_LIST_H
#define MATERIAL_LIST_H

#include "material.h"
#include "../4DyuchiGXGFunc/global.h"


struct MTL_HANDLE
{
	CMaterial	Mtl;
};

class CMaterialList
{
	DWORD				m_dwMtlNum;
	DWORD				m_dwMaxMtlNum;
	MTL_HANDLE*			m_pMtlList;
	VBHASH_HANDLE		m_pHashMtl;

public:
	void				SetMaterialNum(DWORD dwNum) {m_dwMtlNum = dwNum;}
	DWORD				GetMaterialNum() {return m_dwMtlNum;}

	DWORD				AddMaterial(MATERIAL* pMtl);
	BOOL				SetMaterial(CMaterial* pMtl,DWORD dwMtlIndex);
	CMaterial*			GetMaterial(DWORD dwMtlIndex);
	
	BOOL				Initialize(DWORD dwMaxMtlNum);
	BOOL				BeginBulidMaterialList(DWORD dwMaxMtlNum);

	DWORD				CreateMaterialTable(MATERIAL_TABLE** ppMtlTableList);
	void				ReleaseMaterialTable(MATERIAL_TABLE* pMtlTableList);

	DWORD				GetIndex(char* szMtlName);

	void				Release();
	DWORD				WriteFile(FILE* fp);
	DWORD				ReadFile(void* pFP);
	
	
	CMaterialList();
	~CMaterialList();

};

#endif
