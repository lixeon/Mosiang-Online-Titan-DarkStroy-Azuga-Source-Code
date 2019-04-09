// MaterialManager.cpp: implementation of the CMaterialManager class.
//
//////////////////////////////////////////////////////////////////////

#include "MaterialManager.h"
#include "CoD3DDevice.h"
#include "global_variable.h"






CMaterialManager::CMaterialManager()
{
	memset(this,0,sizeof(CMaterialManager));

}



BOOL CMaterialManager::Initialize(CoD3DDevice* pRenderer,DWORD dwMaxMtlNum,DWORD dwMaxMtlSetNum,DWORD dwFlag)
{
	

	m_pRenderer = pRenderer;
	m_dwMaxMtlSetNum = dwMaxMtlSetNum;
	m_dwMaxMtlNum = dwMaxMtlNum;

	m_ppMtlHandleList = new MTL_HANDLE*[dwMaxMtlNum];
	memset(m_ppMtlHandleList,0,sizeof(MTL_HANDLE*)*dwMaxMtlNum);

	m_pMtlSetList = new MATERIAL_SET[dwMaxMtlSetNum+1];
	memset(m_pMtlSetList,0,sizeof(MATERIAL_SET)*(dwMaxMtlSetNum+1));

	m_pMtlHash = VBHCreate();
	VBHInitialize(m_pMtlHash,1024,sizeof(MATERIAL),dwMaxMtlNum);

	m_pMtlMemoryPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pMtlMemoryPool,sizeof(MTL_HANDLE),64,dwMaxMtlNum);

	m_pICMtl = ICCreate();
	ICInitialize(m_pICMtl,dwMaxMtlNum);

	m_pICMtlSet = ICCreate();
	ICInitialize(m_pICMtlSet,dwMaxMtlSetNum);

	MATERIAL	mtl;
	memset(&mtl,0,sizeof(mtl));
	/*
	mtl.dwAmbient = 0;
	mtl.dwDiffuse = 0;
	mtl.dwSpecular = 0;
	mtl.dwFlag = 0;
	memset(mtl.szDiffuseTexmapFileName,0,sizeof(mtl.szDiffuseTexmapFileName));*/
//	wsprintf(mtl.szTexmapFileName,"%s","psteam.tga");
	mtl.dwTextureNum = 1;


	m_pDefaultMtlHandle = Alloc(&mtl,NULL,NULL,0);

	
	return TRUE;
}


MTL_HANDLE* CMaterialManager::Add(DWORD* pdwIndexResult,MATERIAL* pMtl,DWORD dwFlag)
{
	DWORD	dwItem;
	DWORD	dwIndex = 0xffffffff;
	void*	pSearchHandle;
	MTL_HANDLE*	pMtlHandle = NULL;

	if (VBHSelect(m_pMtlHash,&dwItem,1,pMtl,sizeof(MATERIAL)))
	{
		*pdwIndexResult = dwItem;
		pMtlHandle = m_ppMtlHandleList[dwItem];
		goto lb_return;
	}

	dwIndex = ICAllocIndex(m_pICMtl);
	if (!dwIndex)
	{
		dwIndex--;
		goto lb_return;
	}
	
	pMtlHandle = (MTL_HANDLE*)LALAlloc(m_pMtlMemoryPool);

	if (!pMtlHandle)
	{
#ifdef _DEBUG
		char	txt[512];
		DWORD	dwAddr;
		memset(txt,0,512);
		wsprintf(txt,"CMaterialManager::Add(), Fail to LALAlloc(), File:%s , Line:%d \n",__FILE__,__LINE__);
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
#endif
		goto lb_return;
	}

	pMtlHandle->dwIndex = dwIndex;
	
	pMtlHandle->dwRefCount = 0;
	SetMaterial(&pMtlHandle->mtlData,pMtl);
	m_ppMtlHandleList[dwIndex] = pMtlHandle;
	
	pSearchHandle = VBHInsert(m_pMtlHash,dwIndex,pMtl,sizeof(MATERIAL));

	if (!pSearchHandle)
	{
#ifdef _DEBUG
		char	txt[512];
		DWORD	dwAddr;
		memset(txt,0,512);
		GetEIP(&dwAddr);
		wsprintf(txt,"CMaterialManager::Add(), Fail to VBHInsert(),File : %s , Line : %d \n",__FILE__,__LINE__);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
#endif
		goto lb_return;
	}
	pMtlHandle->pSearchHandle = pSearchHandle;
	*pdwIndexResult = dwIndex;
	
	m_dwMtlNum++;
lb_return:
	return pMtlHandle;
}
DIMATERIAL* CMaterialManager::Alloc(MATERIAL* pMtl,DWORD* pdwWidth,DWORD* pdwHeight,DWORD dwFlag)
{
	DIMATERIAL*	pResult = NULL;
	
	if (!pMtl)
	{
		// 디폴트 매터리얼 할당..
		pResult = &m_defaultMtlHandle.mtlData;
		goto lb_return;
		
	}
	

	DWORD	dwIndex;
	
	MTL_HANDLE*	pMtlHandle;

	if (!VBHSelect(m_pMtlHash,&dwIndex,1,pMtl,sizeof(MATERIAL)))
	{
		pMtlHandle = Add(&dwIndex,pMtl,dwFlag);
		if (!pMtlHandle)
			goto lb_return;
	}
	pMtlHandle = m_ppMtlHandleList[dwIndex];
	pMtlHandle->dwRefCount++;

	if (pdwWidth && pdwHeight)
	{
		*pdwWidth = 0;
		*pdwHeight = 0;
		IMAGE_HEADER*		pImgHeader = pMtlHandle->mtlData.TextureDiffuse.GetImageHeader();
		if (pImgHeader)
		{
			*pdwWidth = pImgHeader->dwWidth;
			*pdwHeight = pImgHeader->dwHeight;
		}
		
	}
	pResult = &pMtlHandle->mtlData;

lb_return:
	return pResult;

}
void CMaterialManager::SetMaterialTextureBorder(DIMATERIAL* pMtl,DWORD dwColor)
{
	if (pMtl)
	{
		if (pMtl->TextureDiffuse.GetTexture())
		{
			m_pRenderer->GetD3DResourceManager()->SetTextureBorder(pMtl->TextureDiffuse.GetTexture(),dwColor);
		}
	}
}
void CMaterialManager::Free(DIMATERIAL* pMtl)
{
	MTL_HANDLE*	pMtlHandle = (MTL_HANDLE*)( (char*)pMtl - DIMATERIAL_OFFSET );

	// 해쉬핸들이 NULL이면 삭제대상에서 제외한다..디폴트 매터리얼 및 예외사항을 위해서..
	if (!pMtlHandle->pSearchHandle)
	{
		return;
	}

	pMtlHandle->dwRefCount--;
	if (!pMtlHandle->dwRefCount)
	{
		m_ppMtlHandleList[pMtlHandle->dwIndex] = NULL;
		ICFreeIndex(m_pICMtl,pMtlHandle->dwIndex);
		VBHDelete(m_pMtlHash,pMtlHandle->pSearchHandle);

		pMtlHandle->mtlData.TextureDiffuse.Release();
		pMtlHandle->mtlData.TextureReflect.Release();
		pMtlHandle->mtlData.TextureBump.Release();

		LALFree(m_pMtlMemoryPool,pMtlHandle);
		m_dwMtlNum--;
	}

}

void CMaterialManager::SetMaterial(DIMATERIAL* pMtlResult,MATERIAL* pMtlSrc)
{

//	memset(&pMtlResult->d3dMtl,0,sizeof(pMtlResult->d3dMtl));
	memset(pMtlResult,0,sizeof(DIMATERIAL));
	DWORDtoCOLOR(
		pMtlSrc->GetDiffuse(),
		&pMtlResult->d3dMtl.Diffuse.r,
		&pMtlResult->d3dMtl.Diffuse.g,
		&pMtlResult->d3dMtl.Diffuse.b,
		&pMtlResult->d3dMtl.Diffuse.a);

	DWORDtoCOLOR(
		pMtlSrc->GetAmbient(),
		&pMtlResult->d3dMtl.Ambient.r,
		&pMtlResult->d3dMtl.Ambient.g,
		&pMtlResult->d3dMtl.Ambient.b,
		&pMtlResult->d3dMtl.Ambient.a);

	DWORDtoCOLOR(
		pMtlSrc->GetSpecular(),
		&pMtlResult->d3dMtl.Specular.r,
		&pMtlResult->d3dMtl.Specular.g,
		&pMtlResult->d3dMtl.Specular.b,
		&pMtlResult->d3dMtl.Specular.a);

			
	pMtlResult->d3dMtl.Power = pMtlSrc->fShineStrength * MAXSHINESTR_TO_SS3D_VAL;
	if (m_pRenderer->INL_IsEnableSpecular())
	{
		if (0.0f == pMtlSrc->fShineStrength)
			pMtlResult->d3dMtl.Power = m_pRenderer->INL_GetDefaultSpecularPower();
	}

	pMtlResult->d3dMtl.Diffuse.a = 1.0f;
	pMtlResult->d3dMtl.Ambient.a = 1.0f;
	pMtlResult->d3dMtl.Specular.a = 1.0f;

	if (pMtlSrc->dwFlag & SELF_ILLUNUM)
	{
		pMtlResult->dwProperty |= DIMATERIAL_PROPERTY_TYPE_SELF_ILLUMIN;
		pMtlResult->d3dMtl.Emissive.a = 1.0f;
		pMtlResult->d3dMtl.Emissive.r = 1.0f;
		pMtlResult->d3dMtl.Emissive.g = 1.0f;
		pMtlResult->d3dMtl.Emissive.b = 1.0f;
	}
	pMtlResult->dwAlpha = 255;
	if (pMtlSrc->dwFlag & TRANSP_TYPE_FILTER)
	{
		pMtlResult->dwAlpha = TEX_FLAG_ALPHA_TRANSP_FILTER;
		pMtlResult->dwAlpha |= (BYTE)( (1.0f - pMtlSrc->fTransparency)* 255.0f);
		if ((pMtlResult->dwAlpha & 0x000000ff) == 255)
			pMtlResult->dwAlpha = 255;		
	}
	else if (pMtlSrc->dwFlag & TRANSP_TYPE_SUBTRACTIVE)
	{
		pMtlResult->dwAlpha = TEX_FLAG_ALPHA_TRANSP_SUB;
		pMtlResult->dwAlpha |= (BYTE)( (1.0f - pMtlSrc->fTransparency)* 255.0f);
	}
	else if (pMtlSrc->dwFlag & TRANSP_TYPE_ADDITIVE)
	{
		pMtlResult->dwAlpha = TEX_FLAG_ALPHA_TRANSP_ADD;
		pMtlResult->dwAlpha |= (BYTE)( (1.0f - pMtlSrc->fTransparency)* 255.0f);
	}
	
	// diffuse텍스쳐 할당 

	char*	pTexFileName = pMtlSrc->GetDiffuseTexmapName();
	if (m_pRenderer->AllocTexture(&pMtlResult->TextureDiffuse,pTexFileName,0,TRUE,TRUE,TEXTURE_MAP_TYPE_DIFFUSE))
	{

		if (pMtlResult->TextureDiffuse.GetTexFlag() & TEX_FLAG_ALPHA_OPASITY)
		{
			pMtlResult->dwAlpha |= TEX_FLAG_ALPHA_OPASITY;
			pMtlResult->dwAlpha |= (BYTE)( (1.0f - pMtlSrc->fTransparency)* 255.0f);

		}
	}
	else
	{
		m_pRenderer->AllocDefaultDiffuseTexture(&pMtlResult->TextureDiffuse);
	}

	// reflect 
	pTexFileName = pMtlSrc->GetReflectTexmapName();
	m_pRenderer->AllocTexture(&pMtlResult->TextureReflect,pTexFileName,0,TRUE,TRUE,TEXTURE_MAP_TYPE_DIFFUSE);

	// bump
	pTexFileName = pMtlSrc->GetBumpTexmapName();
	if (m_pRenderer->AllocTexture(&pMtlResult->TextureBump,pTexFileName,0,TRUE,FALSE,TEXTURE_MAP_TYPE_BUMP))
	{
	}
	else
	{
		m_pRenderer->AllocDefaultBumpTexture(&pMtlResult->TextureBump);
	}

	if (pMtlSrc->GetFlag() &  MATERIAL_TYPE_2SIDE)
		pMtlResult->dwProperty |= DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE;
}
DWORD CMaterialManager::CreateMaterialSet(MATERIAL_TABLE* pMtlEntry,DWORD dwNum)
{
	
	DWORD		i;
	DWORD		dwResult;
	dwResult = ICAllocIndex(m_pICMtlSet);

	if (0xffffffff == dwResult)
	{
		dwResult--;
		goto lb_return;
	}
		
	DWORD	dwMaxIndex;
	dwMaxIndex = 0;
	for (i=0; i<dwNum; i++)
	{
		if (dwMaxIndex < pMtlEntry[i].dwMtlIndex)
			dwMaxIndex = pMtlEntry[i].dwMtlIndex;

	}
	m_pMtlSetList[dwResult].dwMtlNum = dwMaxIndex+1;
	m_pMtlSetList[dwResult].ppMtl = new DIMATERIAL*[m_pMtlSetList[dwResult].dwMtlNum];
	memset(m_pMtlSetList[dwResult].ppMtl,0,sizeof(DIMATERIAL*)*m_pMtlSetList[dwResult].dwMtlNum);


	DIMATERIAL*		pMtl;
	for (i=0; i<dwNum; i++)
	{
		if (!m_pMtlSetList[dwResult].ppMtl[pMtlEntry[i].dwMtlIndex])
		{
			pMtl = Alloc(pMtlEntry[i].pMtl,NULL,NULL,0);
			m_pMtlSetList[dwResult].ppMtl[pMtlEntry[i].dwMtlIndex] = pMtl;
		}
		
	}

lb_return:
	return dwResult;
}

DIMATERIAL*	CMaterialManager::GetMaterial(DWORD dwMtlSetIndex,DWORD dwMtlIndex)
{
	DIMATERIAL* pMtl = NULL;

	if (dwMtlSetIndex >= m_dwMaxMtlSetNum)
		goto lb_return;

	if (dwMtlIndex >= m_pMtlSetList[dwMtlSetIndex].dwMtlNum)
		goto lb_return;

	pMtl =  m_pMtlSetList[dwMtlSetIndex].ppMtl[dwMtlIndex];

lb_return:
	return pMtl;
}
void CMaterialManager::DeleteMaterialSet(DWORD dwMtlSetIndex)
{

	if (m_pMtlSetList[dwMtlSetIndex].ppMtl)
	{

		for (DWORD i=0; i<m_pMtlSetList[dwMtlSetIndex].dwMtlNum; i++)
		{
			if (m_pMtlSetList[dwMtlSetIndex].ppMtl[i])
			{
				Free(m_pMtlSetList[dwMtlSetIndex].ppMtl[i]);
			}
		}
		m_pMtlSetList[dwMtlSetIndex].dwMtlNum = 0;

		delete [] m_pMtlSetList[dwMtlSetIndex].ppMtl;
		m_pMtlSetList[dwMtlSetIndex].ppMtl = NULL;
		
//		dwMtlSetIndex++;
		
		ICFreeIndex(m_pICMtlSet,dwMtlSetIndex);
	}
}
CMaterialManager::~CMaterialManager()
{

	if (m_pDefaultMtlHandle)
	{
		Free(m_pDefaultMtlHandle);
		m_pDefaultMtlHandle = NULL;
	}

	if (m_pMtlSetList)
	{
		for (DWORD i=0; i<=m_dwMaxMtlSetNum; i++)
		{
			DeleteMaterialSet(i);
		}
		delete [] m_pMtlSetList;
		m_pMtlSetList = NULL;
	}
	if (m_pICMtl)
	{
		ICRelease(m_pICMtl);
		m_pICMtl = NULL;
	}
	if (m_pICMtlSet)
	{
		ICRelease(m_pICMtlSet);
		m_pICMtlSet = NULL;
	}
	if (m_pMtlHash)
	{
		VBHRelease(m_pMtlHash);
		m_pMtlHash = NULL;

	}

	if (m_pMtlMemoryPool)
	{
		ReleaseStaticMemoryPool(m_pMtlMemoryPool);
		m_pMtlMemoryPool = NULL;
	}
	if (m_ppMtlHandleList)
	{
		delete [] m_ppMtlHandleList;
		m_ppMtlHandleList = NULL;
	}


	
}
