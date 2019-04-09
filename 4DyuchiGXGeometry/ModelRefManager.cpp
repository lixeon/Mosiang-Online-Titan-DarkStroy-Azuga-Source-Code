// ModelRefManager.cpp: implementation of the CModelRefManager class.
//
//////////////////////////////////////////////////////////////////////

#include "ModelRefManager.h"
#include "model.h"
#include "global_variable.h"
#include "geom_obj.h"
#include "../4DyuchiGRX_Common/IRenderer.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModelRefManager::CModelRefManager()
{
	memset(this,0,sizeof(CModelRefManager));

}
BOOL CModelRefManager::Initialize(I4DyuchiGXRenderer* pRenderer,DWORD dwMaxRefNum)
{
	m_pRenderer = pRenderer;
	m_dwMaxRefNum = dwMaxRefNum;
	m_ppModelRefDescList = new MODEL_REF_DESC*[dwMaxRefNum+1];
	memset(m_ppModelRefDescList,0,sizeof(MODEL_REF_DESC*)*(dwMaxRefNum+1));

	m_pIC = ICCreate();
	ICInitialize(m_pIC,m_dwMaxRefNum-1);

	return TRUE;
}	
DWORD CModelRefManager::AllocRefMemory(DWORD dwColMeshSize,DWORD dwObjNum,CoModel* pModel)
{
	DWORD dwRefIndex = 0xffffffff;

	if (m_dwRefNum >= m_dwMaxRefNum)
		goto lb_return;

	dwRefIndex = ICAllocIndex(m_pIC);
	if (!dwRefIndex)
	{
		dwRefIndex--;
		goto lb_return;
	}
	
	DWORD	dwSize;
	char*	pEntry;
	MODEL_REF_DESC*	pModelRefDesc;
	dwSize = 
		sizeof(MODEL_REF_DESC) + 
		sizeof(MODEL_STATUS) + 
		sizeof(SET_FRAME_ARGS) +
		sizeof(MOTION_LIST_DESC) +
		dwObjNum*sizeof(MATRIX4) +	// matrix entry
		dwObjNum*sizeof(MATRIX4) +	// matrix physique entry
		sizeof(MATERIAL_LIST_DESC) + 
		dwColMeshSize;
	pEntry = new char[dwSize];
	memset(pEntry,0,dwSize);

	pModelRefDesc = (MODEL_REF_DESC*)pEntry;
	pModelRefDesc->pModel = pModel;
	pModelRefDesc->dwSize = dwSize;
	
	pModelRefDesc->dwDataSize = dwSize - sizeof(MODEL_REF_DESC);
	pModelRefDesc->pDataEntry = pEntry + sizeof(MODEL_REF_DESC);

	pModelRefDesc->dwColMeshSize = dwColMeshSize;
	pModelRefDesc->dwRefIndex = dwRefIndex;

	pModelRefDesc->pModelStatus = (MODEL_STATUS*)( pEntry + sizeof(MODEL_REF_DESC) );
	pModelRefDesc->pArgsSetFrame = (SET_FRAME_ARGS*)( pEntry + sizeof(MODEL_REF_DESC) + sizeof(MODEL_STATUS) );
	pModelRefDesc->pMotionListDesc = (MOTION_LIST_DESC*)( pEntry + sizeof(MODEL_REF_DESC) + sizeof(MODEL_STATUS) + sizeof(SET_FRAME_ARGS) );
	pModelRefDesc->pMaterialListDesc = (MATERIAL_LIST_DESC*)( pEntry + sizeof(MODEL_REF_DESC) + sizeof(MODEL_STATUS) + sizeof(SET_FRAME_ARGS) + sizeof(MOTION_LIST_DESC)); 
	pModelRefDesc->pMatrixEntry = (MATRIX4*)( pEntry + sizeof(MODEL_REF_DESC) + sizeof(MODEL_STATUS) + sizeof(SET_FRAME_ARGS) + sizeof(MOTION_LIST_DESC) + sizeof(MATERIAL_LIST_DESC)); 
	pModelRefDesc->pMatrixEntryForPhysiqueLocal  = (MATRIX4*)( pEntry + sizeof(MODEL_REF_DESC) + sizeof(MODEL_STATUS) + sizeof(SET_FRAME_ARGS) + sizeof(MOTION_LIST_DESC) + sizeof(MATERIAL_LIST_DESC) + dwObjNum*sizeof(MATRIX4)); 

	pModelRefDesc->pColModelDesc = (COLLISION_MODEL_DESC*)( pEntry + sizeof(MODEL_REF_DESC) + sizeof(MODEL_STATUS) + sizeof(SET_FRAME_ARGS) + sizeof(MOTION_LIST_DESC) + sizeof(MATERIAL_LIST_DESC) + dwObjNum*sizeof(MATRIX4)*2 );	

	pModelRefDesc->dwRefIndex = dwRefIndex;

	m_ppModelRefDescList[dwRefIndex] = pModelRefDesc;
//	pModelRefDesc->pModelStatus->dwFrame = 0xffffffff;
//	pModelRefDesc->pModelStatus->dwMotionIndex = 0xffffffff;
//	pModelRefDesc->pArgsSetFrame->dwFrame = 0xffffffff;
//	pModelRefDesc->pArgsSetFrame->dwMotionIndex = 0xffffffff;
//	memset(&pModelRefDesc->pArgsSetFrame->matTransform,0xff,sizeof(MATRIX4));

	m_dwRefNum++;
lb_return:
	return dwRefIndex;
}

BOOL CModelRefManager::InitializeIllusionEffect(DWORD dwRefIndex,DWORD dwMaxIllusionKeyNum,CMeshObject* pMeshObj,void* pMtlHandle,DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	
	MODEL_REF_DESC*		pModelRefDesc;

	

	if (dwRefIndex > m_dwMaxRefNum)
		goto lb_return;

	if (!pMeshObj)
		goto lb_return;

	// 기본적으로 잔상은 2프레임이상 사이에서만 만들어질수 있다.
	if (dwMaxIllusionKeyNum < 2)
		goto lb_return;

	// 또한 버텍스가 2개이상 되어야 면을 만들수 있다.
	if (pMeshObj->GetVertexNum() < 2)
		goto lb_return;

	pModelRefDesc = m_ppModelRefDescList[dwRefIndex];
	if (pModelRefDesc->pIllusionDesc)
	{
		goto lb_return;
	}
	DWORD	dwIllusionKeySize,dwIllusionDescSize,dwResultVerticesNum,dwFacesNum;

	dwFacesNum =( pMeshObj->GetVertexNum()-1)*2 * (dwMaxIllusionKeyNum-1);
	dwResultVerticesNum = dwFacesNum*3;
	
	
	
	IVERTEX*	pIVList;
	void*		pTriBufferHandle;

	pTriBufferHandle = m_pRenderer->AllocRenderTriBuffer(&pIVList,dwFacesNum,1);
	if (!pIVList)
		goto lb_return;

	
	dwIllusionKeySize = ( pMeshObj->GetVertexNum() * sizeof(VECTOR3) + sizeof(ILLUSION_KEY_DESC) - sizeof(VECTOR3) );

	dwIllusionDescSize = sizeof(ILLUSION_DESC) + dwIllusionKeySize * dwMaxIllusionKeyNum;
	
	pModelRefDesc->pIllusionDesc = (ILLUSION_DESC*)new char[dwIllusionDescSize];
	memset(pModelRefDesc->pIllusionDesc,0,dwIllusionDescSize);

	pModelRefDesc->pIllusionDesc->dwResultVerticesNum = dwResultVerticesNum;
	pModelRefDesc->pIllusionDesc->pMeshObj = pMeshObj;
	pModelRefDesc->pIllusionDesc->dwVerticesNum = pMeshObj->GetVertexNum();
	pModelRefDesc->pIllusionDesc->dwMaxIllutionKeyNum = dwMaxIllusionKeyNum;
	pModelRefDesc->pIllusionDesc->bEnable = FALSE;
	pModelRefDesc->pIllusionDesc->pTriBufferHandle = pTriBufferHandle;
	pModelRefDesc->pIllusionDesc->pIVertexList = pIVList;

	if (!pMtlHandle)
	{
		pModelRefDesc->pIllusionDesc->pMtlHandleAllocated = m_pRenderer->CreateMaterial(pMeshObj->GetDefaultMaterial(),NULL,NULL,0);
		pModelRefDesc->pIllusionDesc->pMtlHandle = pModelRefDesc->pIllusionDesc->pMtlHandleAllocated;
	}
	else
	{
		pModelRefDesc->pIllusionDesc->pMtlHandle = pMtlHandle;
	}
	


	ILLUSION_KEY_DESC*	pIllusionKeyEntry;
	pIllusionKeyEntry = (ILLUSION_KEY_DESC*)( (char*)pModelRefDesc->pIllusionDesc + sizeof(ILLUSION_DESC) );

	pModelRefDesc->pIllusionDesc->pIllusionKeyHead = pIllusionKeyEntry;
	
	// 원형큐 구조로 만든다.
	
	char*	pEntry;
	pEntry = (char*)pIllusionKeyEntry;
	
	int i;
	for (i=0; i<(int)dwMaxIllusionKeyNum-1; i++)
	{
		pIllusionKeyEntry = (ILLUSION_KEY_DESC*)(pEntry + i*dwIllusionKeySize);
		pIllusionKeyEntry->pNext = (ILLUSION_KEY_DESC*)( (char*)pIllusionKeyEntry + dwIllusionKeySize );
	}
	pIllusionKeyEntry->pNext->pNext = pModelRefDesc->pIllusionDesc->pIllusionKeyHead;

	pModelRefDesc->pIllusionDesc->pIllusionKeyTail = pModelRefDesc->pIllusionDesc->pIllusionKeyHead;
	
	bResult = TRUE;
lb_return:
	return bResult;
	
}
BOOL CModelRefManager::DuplicateStatus(DWORD dwDestRefIndex,DWORD dwSrcRefIndex)
{
	BOOL	bResult = FALSE;
	char*	pSrcPtr;
	char*	pDestPtr;
	DWORD	dwSrcSize;
	DWORD	dwDestSize;

	if (!GetModelRefData(&pSrcPtr,&dwSrcSize,dwSrcRefIndex))
	{
#ifdef _DEBUG
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"CModelRefManager::DuplicateStatus(), if (!GetModelRefData()), File:%s , Line:%d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
#endif
		goto lb_return;
	}

	if (!GetModelRefData(&pDestPtr,&dwDestSize,dwDestRefIndex))
	{
#ifdef _DEBUG
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"CModelRefManager::DuplicateStatus(), if (!GetModelRefData()), File:%s , Line:%d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
#endif
		goto lb_return;
	}

	if (dwSrcSize != dwDestSize)
	{
#ifdef _DEBUG
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"CModelRefManager::DuplicateStatus(), if (dwSrcSize != dwDestSize), File:%s , Line:%d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
#endif
		goto lb_return;
	}

	memcpy(pDestPtr,pSrcPtr,dwDestSize);
	
	bResult = TRUE;

lb_return:
	return bResult;

}

void CModelRefManager::FreeRefMemory(DWORD dwRefIndex)
{
	
	char* pEntry = (char*)m_ppModelRefDescList[dwRefIndex];
#ifdef _DEBUG
	if (!pEntry)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"CModelRefManager::FreeRefMemory(), if (!pEntry), File:%s , Line:%d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif
	if (m_ppModelRefDescList[dwRefIndex]->pIllusionDesc)
	{
		if (m_ppModelRefDescList[dwRefIndex]->pIllusionDesc->pMtlHandleAllocated)
		{
			m_pRenderer->DeleteMaterial(m_ppModelRefDescList[dwRefIndex]->pIllusionDesc->pMtlHandleAllocated);
		}
		m_pRenderer->FreeRenderTriBuffer(m_ppModelRefDescList[dwRefIndex]->pIllusionDesc->pTriBufferHandle);
		delete [] m_ppModelRefDescList[dwRefIndex]->pIllusionDesc;


	}

	delete [] pEntry;
	m_ppModelRefDescList[dwRefIndex] = NULL;
	ICFreeIndex(m_pIC,dwRefIndex);

	m_dwRefNum--;
}
BOOL CModelRefManager::IsValidRefIndex(DWORD dwRefIndex,DWORD dwColMeshSize)
{
	BOOL	bResult = FALSE;
	if (dwRefIndex > m_dwMaxRefNum)
		goto lb_return;

	if (m_ppModelRefDescList[dwRefIndex]->dwColMeshSize != dwColMeshSize)
		goto lb_return;


	bResult = TRUE;

	
lb_return:
#ifdef _DEBUG
	if (!bResult)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"CModelRefManager::IsValidRefIndex(), if (!bResult), File:%s , Line:%d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif
	return bResult;
}
BOOL CModelRefManager::GetModelRefDesc(MODEL_REF_DESC* pModelRefDesc,DWORD dwRefIndex)
{
	
	BOOL	bResult = FALSE;
	

	if (dwRefIndex > m_dwMaxRefNum)
		goto lb_return;

	*pModelRefDesc = *m_ppModelRefDescList[dwRefIndex];
	bResult = TRUE;

lb_return:
	return bResult;
}
ILLUSION_DESC* CModelRefManager::GetModelIllusionDesc(DWORD dwRefIndex)
{
	return m_ppModelRefDescList[dwRefIndex]->pIllusionDesc;

}
BOOL CModelRefManager::GetModelSetFrameArgs(SET_FRAME_ARGS** ppArgs,DWORD dwRefIndex)
{
	
	BOOL	bResult = FALSE;
	

	if (dwRefIndex > m_dwMaxRefNum)
		goto lb_return;

	*ppArgs = m_ppModelRefDescList[dwRefIndex]->pArgsSetFrame;
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CModelRefManager::GetModelRefData(char** ppDataEntry,DWORD* pdwSize,DWORD dwRefIndex)
{
	
	BOOL	bResult = FALSE;

	if (dwRefIndex > m_dwMaxRefNum)
		goto lb_return;

	*ppDataEntry = m_ppModelRefDescList[dwRefIndex]->pDataEntry;
	*pdwSize = m_ppModelRefDescList[dwRefIndex]->dwDataSize;
	
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CModelRefManager::GetColModelDesc(COLLISION_MODEL_DESC**	ppColModelDesc,DWORD dwRefIndex)
{
	BOOL	bResult = FALSE;
	*ppColModelDesc = NULL;

	if (dwRefIndex > m_dwMaxRefNum)
		goto lb_return;

	*ppColModelDesc = m_ppModelRefDescList[dwRefIndex]->pColModelDesc;
	
	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CModelRefManager::GetModelStatus(MODEL_STATUS** ppModelStatus,DWORD dwRefIndex)
{
	BOOL	bResult = FALSE;
	*ppModelStatus = NULL;

	if (dwRefIndex > m_dwMaxRefNum)
		goto lb_return;

	*ppModelStatus = m_ppModelRefDescList[dwRefIndex]->pModelStatus;
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CModelRefManager::GetMatrixEntry(MATRIX4** ppMatrixEntry,MATRIX4** ppMatrixEntryForPhysiqueLocal,DWORD dwRefIndex)
{
	BOOL	bResult = FALSE;
	*ppMatrixEntry = NULL;

	if (dwRefIndex > m_dwMaxRefNum)
		goto lb_return;

	*ppMatrixEntry = m_ppModelRefDescList[dwRefIndex]->pMatrixEntry;
	*ppMatrixEntryForPhysiqueLocal = m_ppModelRefDescList[dwRefIndex]->pMatrixEntryForPhysiqueLocal;
	
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CModelRefManager::GetMotionListDesc(MOTION_LIST_DESC** ppMotionListDesc,DWORD dwRefIndex)
{
	BOOL	bResult = FALSE;
	*ppMotionListDesc = NULL;

	if (dwRefIndex > m_dwMaxRefNum)
		goto lb_return;

	*ppMotionListDesc = m_ppModelRefDescList[dwRefIndex]->pMotionListDesc;
	bResult = TRUE;

lb_return:
	return bResult;
	
}
BOOL CModelRefManager::GetMaterialListDesc(MATERIAL_LIST_DESC** ppMaterialListDesc,DWORD dwRefIndex)
{
	BOOL	bResult = FALSE;
	*ppMaterialListDesc = NULL;

	if (dwRefIndex > m_dwMaxRefNum)
		goto lb_return;

	*ppMaterialListDesc = m_ppModelRefDescList[dwRefIndex]->pMaterialListDesc;
	
	bResult = TRUE;

lb_return:
	return bResult;

}
DWORD CModelRefManager::GetRefIndexWithModelPtr(DWORD* pdwIndexList,CoModel* pModel,DWORD dwMaxNum)
{
	if (dwMaxNum >m_dwRefNum)
		dwMaxNum = m_dwRefNum;

	DWORD	dwCount = 0;
	for (DWORD i=0; i<m_dwMaxRefNum; i++)
	{
		if (m_ppModelRefDescList[i])
		{
			if (m_ppModelRefDescList[i]->pModel == pModel)
			{
				if (dwCount >= dwMaxNum)
					goto lb_return;

				pdwIndexList[dwCount] = m_ppModelRefDescList[i]->dwRefIndex;
				dwCount++;
			}
		}
	}
lb_return:
	return dwCount;
}
CModelRefManager::~CModelRefManager()
{
	if (m_ppModelRefDescList)
		{
		for (DWORD i=0; i<=m_dwMaxRefNum; i++)
		{
			if (m_ppModelRefDescList[i])
			{
				delete [] (char*)m_ppModelRefDescList[i];
				m_ppModelRefDescList[i] = NULL;

			}
		}
	}
	if (m_ppModelRefDescList)
	{
		delete [] m_ppModelRefDescList;
		m_ppModelRefDescList = NULL;
	}
	ICRelease(m_pIC);

}

