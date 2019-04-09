#include "material.h"
#include "../4DyuchiGXGFunc/global.h"
#include "global_variable.h"
#include "../4DyuchiGRX_Common/IFileStorage.h"

CMaterial::CMaterial()
{
	m_dwType = OBJECT_TYPE_MATERIAL;
	m_dwTextureNum = 0;
	m_dwDiffuse = 0;
	m_dwAmbient = 0;
	m_dwSpecular = 0;
	m_fTransparency = 0.0f;
	m_fShine = 0.0f;
	m_fShineStrength = 0.0f;
	memset(m_szDiffuseTexmapFileName,0,sizeof(m_szDiffuseTexmapFileName));
	memset(m_szReflectTexmapFileName,0,sizeof(m_szReflectTexmapFileName));
	memset(m_szBumpTexmapFileName,0,sizeof(m_szBumpTexmapFileName));
	

}

DWORD CMaterial::ReadFile(void* pFP)
{
	FILE_MATERIAL_HEADER	header;

	DWORD	dwLen =	g_pFileStorage->FSRead(pFP,&header,sizeof(FILE_MATERIAL_HEADER));
	m_dwTextureNum = 1;//header.dwTextureNum;
	m_dwDiffuse = header.dwDiffuse;
	m_dwAmbient = header.dwAmbient;
	m_dwSpecular = header.dwSpecular;
	m_fTransparency = header.fTransparency;
	m_fShine = header.fShine;
	m_fShineStrength = header.fShineStrength;

	memset(m_szDiffuseTexmapFileName,0,sizeof(m_szDiffuseTexmapFileName));
	lstrcpy(m_szDiffuseTexmapFileName,header.szTexmapFileName[TEXTURE_MAP_TYPE_DIFFUSE]);

	memset(m_szReflectTexmapFileName,0,sizeof(m_szReflectTexmapFileName));
	lstrcpy(m_szReflectTexmapFileName,header.szTexmapFileName[TEXTURE_MAP_TYPE_REFLECT]);

	memset(m_szBumpTexmapFileName,0,sizeof(m_szBumpTexmapFileName));
	lstrcpy(m_szBumpTexmapFileName,header.szTexmapFileName[TEXTURE_MAP_TYPE_BUMP]);

	
	memset(m_szMtlName,0,sizeof(m_szMtlName));
	lstrcpy(m_szMtlName,header.szMtlName);

	m_dwFlag = header.dwFlag;
	m_dwIndex = header.dwIndex;
	
	return dwLen;
}


DWORD CMaterial::WriteSTMMaterial(FILE* fp)
{
	DWORD	oldPos = ftell(fp);
	
	STM_MATERIAL	mtl;
	memcpy(&mtl,this,sizeof(mtl));
	mtl.dwFlag = m_dwFlag;
	fwrite(&mtl,sizeof(STM_MATERIAL),1,fp);

	return (ftell(fp)-oldPos);
}
DWORD CMaterial::ReadSTMMaterial(void* pFP)
{
	DWORD	dwLen = 0;

	STM_MATERIAL	mtl;
	memset(m_szMtlName,0,sizeof(m_szMtlName));

	dwLen += g_pFileStorage->FSRead(pFP,&mtl,sizeof(STM_MATERIAL));
	


	m_dwType = mtl.dwType;
	m_dwIndex = mtl.dwIndex;
	m_dwTextureNum = mtl.dwTextureNum;
	m_dwDiffuse = mtl.dwDiffuse;
	m_dwAmbient = mtl.dwAmbient;
	m_dwSpecular = mtl.dwSpecular;
	m_fTransparency = mtl.fTransparency;
	m_fShine = mtl.fShine;
	m_fShineStrength = mtl.fShineStrength;
	m_dwFlag = mtl.dwFlag;
	
	lstrcpy(m_szDiffuseTexmapFileName,mtl.szTexmapFileName);
//	lstrcpy(m_szReflectTexmapFileName,mtl.szTexmapFileName);
	
	return dwLen;
}


