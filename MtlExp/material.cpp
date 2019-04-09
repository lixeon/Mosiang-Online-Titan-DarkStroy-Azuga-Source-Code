#include "material.h"
#include "../4DyuchiGXGFunc/global.h"
#include "maxexp_global.h"

CMaterial::CMaterial()
{
	memset(this,0,sizeof(CMaterial));
}

BOOL CMaterial::SetTexture(TEXTURE_MAP_TYPE type,TCHAR* uszFileName)
{

	BOOL	bResult = FALSE;
	TCHAR	filename[_MAX_PATH];
	memset(filename,0,_MAX_PATH);

	if (type > TEXTURE_MAP_TYPE_MAX_INDEX)
		goto lb_return;


	GetNameRemovePath(filename,uszFileName);

	_tcscpy(&m_uszTexmapFileName[type][0],filename);

	bResult = TRUE;

lb_return:
	return bResult;

}

DWORD CMaterial::WriteFile(FILE* fp)
{
	FILE_MATERIAL_HEADER*	pHeader = (FILE_MATERIAL_HEADER*)&this->m_dwTextureNum;
	DWORD	oldPos = ftell(fp);

	fwrite(pHeader,sizeof(FILE_MATERIAL_HEADER),1,fp);
	
	return (ftell(fp)-oldPos);
}

