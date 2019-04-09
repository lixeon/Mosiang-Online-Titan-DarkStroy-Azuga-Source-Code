#include "vertexbuffer.h"
#include "d3d_helper.h"
#include "D3DResourceManager.h"

void inline INL_CalcXZ(float* px,float* pz,DWORD dwPosX,DWORD dwPosY,float top,float left,float bottom,float right,float width,float height,DWORD dwFacesNumX,DWORD dwFacesNumZ)
{
	DWORD		dwIndex;
	
	dwIndex = dwPosY*(dwFacesNumX+1)+dwPosX;
	*px = (float)dwPosX/(float)dwFacesNumX * width + left;
	*pz = (float)dwPosY/(float)dwFacesNumZ * height + top;
}



CVertexBuffer::CVertexBuffer()
{
	m_dwSize = 0;
	m_pVB = NULL;
	m_dwStartIndex = 0;
	m_dwVerticesNum = 0;
	m_dwVertexShader = 0;
	m_pResourceManager = NULL;
	
	// for vertexbuffer pool & heap
	m_pVBManager = NULL;
	m_pVBHandle = NULL;
	m_bEnable = FALSE;
}

void CVertexBuffer::SetTimeStamp()
{
	if (m_pVBManager)
		m_pVBManager->SetTimeStamp(m_pVBHandle);
	
}
BOOL CVertexBuffer::CreateVertexBuffer(CD3DResourceManager* pResourceManager,DWORD dwSize,DWORD dwUsage,DWORD FVF,D3DPOOL pool,DWORD dwNum)
{
	BOOL	bResult = FALSE;
	HRESULT	hr;

	m_dwVerticesNum = dwNum;
	m_dwSize = dwSize;
	
	m_dwVertexShader = FVF;
	hr =  pResourceManager->CreateVertexBuffer(dwSize*dwNum,dwUsage,FVF, pool,&m_pVB);
	
	if (hr != D3D_OK)
		goto lb_return;
	
	m_pResourceManager = pResourceManager;
	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CVertexBuffer::SetVertexPosList(VECTOR3* pv3,DWORD dwNum)
{
	DWORD	i;
	BYTE*	pDest;
	
	BOOL	bResult = FALSE;

	if (!m_pVB)
		goto lb_return;

//HRESULT Lock(
//  UINT OffsetToLock,
//  UINT SizeToLock,
//  BYTE** ppbData,
//  DWORD Flags
//);

	m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*dwNum, (BYTE**)&pDest, 0 );
	for (i=0; i<dwNum; i++)
	{
		((VECTOR3*)pDest)->x = (pv3+i)->x;
		((VECTOR3*)pDest)->y = (pv3+i)->y;
		((VECTOR3*)pDest)->z = (pv3+i)->z;
		
		pDest += m_dwSize;
	}
	
	m_pVB->Unlock();
	bResult = TRUE;

lb_return:
	return bResult;

}
BOOL CVertexBuffer::SetVertexAndUVList(VECTOR3* pv3,TVERTEX* ptv,TEX_COORD_OFFSET offset,DWORD dwNum)
{
	BYTE*	pDest;
	BYTE*	pDestBackup;
	BOOL	bResult = FALSE;

	if (!m_pVB)
		goto lb_return;

	m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*dwNum, (BYTE**)&pDest, 0 );

	pDestBackup = pDest;
	
	if (pv3)
	{

		for (DWORD i=0; i<dwNum; i++)
		{
			((VECTOR3*)pDest)->x = (pv3+i)->x;
			((VECTOR3*)pDest)->y = (pv3+i)->y;
			((VECTOR3*)pDest)->z = (pv3+i)->z;
			
			pDest += m_dwSize;
		}
	}

	pDest = pDestBackup;

	if (ptv)
	{

		for (DWORD i=0; i<dwNum; i++)
		{
			*(float*)(pDest + offset) = (ptv+i)->u;
			*(float*)(pDest + offset + 4) = (ptv+i)->v;
			
			pDest += m_dwSize;
		}
	}
	
	m_pVB->Unlock();
	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CVertexBuffer::SetVertexPosList(VECTOR4* pv4,DWORD dwNum)
{
	BYTE*	pDest;
	BOOL	bResult = FALSE;
	DWORD	i;

	if (!m_pVB)
		goto lb_return;

	m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*dwNum, (BYTE**)&pDest, 0 );
	for (i=0; i<dwNum; i++)
	{
		*(VECTOR4*)pDest = *(pv4+i);
		pDest += m_dwSize;
	}
	m_pVB->Unlock();
	bResult = TRUE;

lb_return:
	return bResult;

}

BOOL CVertexBuffer::SetVertexPosList(WORD* pIndex,VECTOR3* pv3,DWORD dwNum)
{
	BYTE*	pDest;
	DWORD	i;
	BOOL	bResult = FALSE;

	if (!m_pVB)
		goto lb_return;

	m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*dwNum, (BYTE**)&pDest, 0 );
	for (i=0; i<dwNum; i++)
	{
		*(VECTOR3*)pDest = *(pv3 + pIndex[i]);
		pDest += m_dwSize;	
	}

	m_pVB->Unlock();
	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CVertexBuffer::SetVertexPosList(DWORD dwDestPitch,DWORD dwSrcPosX,DWORD dwSrcPosY,HFIELD_DESC* pHFDesc,DWORD dwSampleStep)
{
	BYTE*	pDest;
	
	BOOL	bResult = FALSE;
	if (!m_pVB)
		goto lb_return;

	m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*m_dwVerticesNum, (BYTE**)&pDest, 0 );	
	bResult = SetVertexPosListWithManualLock(pDest,dwDestPitch,dwSrcPosX,dwSrcPosY,pHFDesc,dwSampleStep);
	m_pVB->Unlock();

	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CVertexBuffer::SetVertexPosListForScreenFiltier(DWORD dwDestPitch,float left,float right,float top,float bottom,DWORD dwFacesNumX,DWORD dwFacesNumY)
{
	BYTE*	pDest;
	
	BOOL	bResult = FALSE;
	if (!m_pVB)
		goto lb_return;

	m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*m_dwVerticesNum, (BYTE**)&pDest, 0 );	
	bResult = SetVertexPosListWithManualLockForScreenFiltier(pDest,dwDestPitch,top,left,bottom,right,dwFacesNumX,dwFacesNumY);
	m_pVB->Unlock();

	bResult = TRUE;

lb_return:
	return bResult;
}


BOOL CVertexBuffer::SetTLVERTEXList(VECTOR4* pv4Rect,DWORD dwColor,DWORD dwNum)
{
	DWORD	i;
	BYTE*	pDest;
	BOOL	bResult = FALSE;

	if (!m_pVB)
		goto lb_return;

	m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*dwNum, (BYTE**)&pDest, 0 );

	for (i=0; i<dwNum; i++)
	{
		*(VECTOR4*)pDest = pv4Rect[i];
		((D3DTLVERTEX*)(pDest))->color = dwColor;
		
		pDest += m_dwSize;
	}
	m_pVB->Unlock();

	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CVertexBuffer::SetVertexColor(DWORD dwColor,DIFFUSE_COORD_OFFSET offset,DWORD dwNum)
{
	DWORD	i;
	BYTE*	pDest;
	BOOL	bResult = FALSE;

	if (!m_pVB)
		goto lb_return;

	m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*dwNum, (BYTE**)&pDest, 0 );

	for (i=0; i<dwNum; i++)
	{
		*(DWORD*)(pDest+offset) = dwColor;
		pDest += m_dwSize;
	}
	m_pVB->Unlock();

	bResult = TRUE;

lb_return:
	return bResult;

}
BOOL CVertexBuffer::SetVertexColor(DWORD* pVertexColor,DIFFUSE_COORD_OFFSET offset,DWORD dwNum)
{
	BYTE*	pDest;
	
	BOOL	bResult = FALSE;
	if (!m_pVB)
		goto lb_return;

	m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*dwNum, (BYTE**)&pDest, 0 );
	bResult = SetVertexColorWithManualLock(pDest,pVertexColor,offset,dwNum);
	m_pVB->Unlock();

	bResult = TRUE;

lb_return:
	return bResult;
}


BOOL CVertexBuffer::SetVertexPosListWithManualLock(BYTE* pDest,DWORD dwDestPitch,DWORD dwSrcPosX,DWORD dwSrcPosY,HFIELD_DESC* pHFDesc,DWORD dwSampleStep)
{
	DWORD	z,x,xpos,zpos;
	for (z=0; z<dwDestPitch; z++)
	{
		for (x=0; x<dwDestPitch; x++)
		{
			xpos = dwSrcPosX+(x*dwSampleStep);
			zpos = dwSrcPosY+(z*dwSampleStep);

			INL_CalcXZ(
				&((VECTOR3*)pDest)->x,
				&((VECTOR3*)pDest)->z,
				xpos,zpos,
				pHFDesc->top,
				pHFDesc->left,
				pHFDesc->bottom,
				pHFDesc->right,
				pHFDesc->width,
				pHFDesc->height,
				pHFDesc->dwFacesNumX,
				pHFDesc->dwFacesNumZ);

				((VECTOR3*)pDest)->y = pHFDesc->pyfList[xpos + zpos*pHFDesc->dwYFNumX];

			pDest += m_dwSize;	
		}
	}
	return TRUE;
}

BOOL CVertexBuffer::SetVertexPosListWithManualLockForScreenFiltier(BYTE* pDest,DWORD dwDestPitch,float left,float right,float top,float bottom,DWORD dwFacesNumX,DWORD dwFacesNumY)
{
	float	width,height;
	width = right-left;
	height = bottom - top;
	DWORD	y,x;
	for (y=0; y<dwDestPitch; y++)
	{
		for (x=0; x<dwDestPitch; x++)
		{
			INL_CalcXZ(
				&((VECTOR3*)pDest)->x,
				&((VECTOR3*)pDest)->y,
				x,y,
				top,
				left,
				bottom,
				right,
				width,
				height,
				dwFacesNumX,
				dwFacesNumY);

				((VECTOR3*)pDest)->z = 1.0f;
				

			pDest += m_dwSize;	
		}
	}
	return TRUE;
}
BOOL CVertexBuffer::SetVertexColorWithManualLock(BYTE* pDest,DWORD* pVertexColor,DIFFUSE_COORD_OFFSET offset,DWORD dwNum)
{
	DWORD i;
	for (i=0; i<dwNum; i++)
	{
		*(DWORD*)(pDest+offset) = pVertexColor[i];
		pDest += m_dwSize;
	}
	return TRUE;
}


HRESULT CVertexBuffer::LockVertexPtr(BYTE** ppVPtr,DWORD dwFlag)
{
	return m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*m_dwVerticesNum, (BYTE**)ppVPtr,dwFlag);
	
}
void CVertexBuffer::UnlockVertexPtr()
{
	m_pVB->Unlock();
}


BOOL CVertexBuffer::SetTexCoord(TVERTEX* ptv,TEX_COORD_OFFSET offset,DWORD dwNum)
{
	BOOL	bResult = FALSE;

	DWORD	i;
	BYTE*	pDest;

	if (!m_pVB)
		goto lb_return;

	m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*dwNum, (BYTE**)&pDest, 0 );
	
	for (i=0; i<dwNum; i++)
	{
		*(float*)(pDest + offset) = (ptv+i)->u;
		*(float*)(pDest + offset + 4) = (ptv+i)->v;
		pDest += m_dwSize;
	}
	m_pVB->Unlock();
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CVertexBuffer::SetTexCoord(TVERTEX* ptv,WORD* pIndex,TEX_COORD_OFFSET offset,DWORD dwNum)
{
	DWORD	i;
	BYTE*	pDest;

	BOOL	bResult = FALSE;
	if (!m_pVB)
		goto lb_return;

	m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*dwNum, (BYTE**)&pDest, 0 );


	
	for (i=0; i<dwNum; i++)
	{
		*(float*)(pDest + offset) = (ptv+pIndex[i])->u;
		*(float*)(pDest + offset + 4) = (ptv+pIndex[i])->v;
		pDest += m_dwSize;
	}
	m_pVB->Unlock();
	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CVertexBuffer::SetNormal(VECTOR3* pnv,DWORD dwNum)
{
	DWORD		i;
	BYTE*		pDest;

	BOOL	bResult = FALSE;
	if (!m_pVB)
		goto lb_return;

	m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*dwNum, (BYTE**)&pDest, 0 );

	for (i=0; i<dwNum; i++)
	{
		((D3DVLVERTEX*)pDest)->nx = (pnv+i)->x;
		((D3DVLVERTEX*)pDest)->ny = (pnv+i)->y;
		((D3DVLVERTEX*)pDest)->nz = (pnv+i)->z;
		pDest += m_dwSize;
	} 

	m_pVB->Unlock();
	bResult = TRUE;

lb_return:
	return bResult;

}

BOOL CVertexBuffer::SetTangentU(VECTOR3* pnv,DWORD dwNum)
{
	DWORD		i;
	BYTE*		pDest;

	BOOL	bResult = FALSE;
	if (!m_pVB)
		goto lb_return;

	m_pVB->Lock(m_dwStartIndex*m_dwSize,m_dwSize*dwNum, (BYTE**)&pDest, 0 );

	for (i=0; i<dwNum; i++)
	{
		((D3DPLVERTEX*)pDest)->tanu_x = (pnv+i)->x;
		((D3DPLVERTEX*)pDest)->tanu_y = (pnv+i)->y;
		((D3DPLVERTEX*)pDest)->tanu_z = (pnv+i)->z;
		pDest += m_dwSize;
	} 

	m_pVB->Unlock();
	bResult = TRUE;

lb_return:
	return bResult;

}

void CVertexBuffer::Cleanup()
{
	if (m_pVBManager)
	{
		m_pVBManager->ReleaseVB(m_pVBHandle);
	}
	else
	{
		if (m_pVB)
		{
			m_pResourceManager->Release(m_pVB);
		}
	}
	m_dwSize = 0;
	m_pVB = NULL;
	m_dwStartIndex = 0;
	m_dwVerticesNum = 0;
	m_dwVertexShader = 0;
	m_pResourceManager = NULL;
	
	// for vertexbuffer pool & heap
	m_pVBManager = NULL;
	m_pVBHandle = NULL;
	
}
CVertexBuffer::~CVertexBuffer()
{
	Cleanup();
}
