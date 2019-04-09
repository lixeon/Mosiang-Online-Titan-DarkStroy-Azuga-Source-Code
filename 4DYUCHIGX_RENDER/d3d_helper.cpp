#include "d3d_helper.h"
#include "../4DyuchiGRX_myself97/My2DLib.h"
#include "../4DyuchiGXGFunc/global.h"
#include <io.h>
#include "IndexBuffer.h"
#include "CoD3DDevice.h"
#include "global_variable.h"





BOOL InitializeShader(LPDIRECT3DDEVICE8 pDevice,SHADER_HANDLE* pHandle,DWORD* pdwDecl,char* szFileName,SHADER_TYPE type)
{
	BOOL	bResult = FALSE;

	char	szDefaultPath[_MAX_PATH] = "d:\\4DyuchiGX\\4DyuchiGX_Shader\\";
	if (pHandle->dwRefCount)
	{
		goto lb_already;
	}
	
	DWORD	dwFailCount;
	dwFailCount = 0;

lb_try_open:
	FILE* fp;
	fp = fopen(szFileName,"rb");
	if (!fp)
	{
		if (dwFailCount)
			goto lb_return;

	
		char	szPureFileName[_MAX_PATH];
		GetNameRemovePath(szPureFileName,szFileName);
		lstrcat(szDefaultPath,szPureFileName);
		szFileName = szDefaultPath;
		dwFailCount++;
		goto lb_try_open;
	}

	int		iFileSize;
	iFileSize = filelength(fileno(fp));

	pHandle->pdwCodeMemAddr = (DWORD*)new char[iFileSize];
	memset(pHandle->pdwCodeMemAddr,0,sizeof(iFileSize));
	fread(pHandle->pdwCodeMemAddr,sizeof(char),iFileSize,fp);
	fclose(fp);

	HRESULT	hr;

	if (type == SHADER_TYPE_VERTEX_SHADER)
	{
		hr = pDevice->CreateVertexShader(pdwDecl,pHandle->pdwCodeMemAddr,&pHandle->dwShader, 0 );
		pHandle->type = SHADER_TYPE_VERTEX_SHADER;
	}
	else
	{
		hr = pDevice->CreatePixelShader(pHandle->pdwCodeMemAddr,&pHandle->dwShader);
		pHandle->type = SHADER_TYPE_PIXEL_SHADER;
	}
	if (D3D_OK != hr)
	{
		delete [] pHandle->pdwCodeMemAddr;
		pHandle->pdwCodeMemAddr = NULL;
		goto lb_return;
	}
	bResult = TRUE;

lb_already:
	bResult = TRUE;
	pHandle->dwRefCount++;

lb_return:
	return bResult;

}

void ReleaseShader(LPDIRECT3DDEVICE8 pDevice,SHADER_HANDLE* pHandle)
{
	pHandle->dwRefCount--;
	if (!pHandle->dwRefCount)
	{
		if (pHandle->type == SHADER_TYPE_VERTEX_SHADER)
		{
			pDevice->DeleteVertexShader(pHandle->dwShader);
		}
		else
		{
			pDevice->DeletePixelShader(pHandle->dwShader);
		}
		delete [] pHandle->pdwCodeMemAddr;
		pHandle->pdwCodeMemAddr = NULL;
		pHandle->dwShader = 0xffffffff;
	}
}


HRESULT __stdcall AllocVBFunc(CD3DResourceManager* pResourceManager,IDirect3DResource8** ppBuffer,DWORD dwFVF,D3DPOOL pool,DWORD dwUsage,DWORD dwSize)
{
	return pResourceManager->CreateVertexBuffer(dwSize,dwUsage,dwFVF,pool,(IDirect3DVertexBuffer8**)ppBuffer);

}

HRESULT __stdcall AllocIBFunc(CD3DResourceManager* pResourceManager,IDirect3DResource8** ppBuffer,DWORD dwFVF,D3DPOOL pool,DWORD dwUsage,DWORD dwSize)
{
	return pResourceManager->CreateIndexBuffer(dwSize,dwUsage,D3DFMT_INDEX16 ,pool,(IDirect3DIndexBuffer8**)ppBuffer);
}

void __stdcall EnableAlphaSetting(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex,DWORD dwAlpha)
{
	dwAlpha = dwAlpha <<24;
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
	pDevice->SetRenderState(D3DRS_TEXTUREFACTOR , dwAlpha);

	pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );
	pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
}

void __stdcall BeginAlphaMeshObject(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageNum,DWORD dwMtlAlpha,DWORD dwObjAlpha)
{
	DWORD dwResultAlpha = dwMtlAlpha & 0x000000ff;

	dwResultAlpha = (DWORD)( (float)dwResultAlpha * ((float)dwObjAlpha / 255.0f) );
	
	DWORD	i;
	if (dwResultAlpha < 255)
	{
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );

		dwResultAlpha = dwResultAlpha <<24;
		pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		pDevice->SetRenderState(D3DRS_TEXTUREFACTOR , dwResultAlpha);

		for (i=0; i<dwTexStageNum; i++)
		{
			pDevice->SetTextureStageState( i, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );
			pDevice->SetTextureStageState( i, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		}
	}
	else 
	{
		for (i=0; i<dwTexStageNum; i++)
		{
			pDevice->SetTextureStageState( i, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			pDevice->SetTextureStageState( i, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
		}

	}

	if (dwMtlAlpha & ALPHA_TEX_PROPERTY_MASK)
	{
		if (dwMtlAlpha & TEX_FLAG_ALPHA_OPASITY)
		{
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
			pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

			pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );			
		} 
		if (dwMtlAlpha & TEX_FLAG_ALPHA_TRANSP_ADD)
		{
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
			pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);


		}
		else if (dwMtlAlpha & TEX_FLAG_ALPHA_TRANSP_SUB)
		{
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
			pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
			pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
		}
	}
}

void __stdcall BeginAlphaMeshObjectPerStageIndex(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex,DWORD dwMtlAlpha,DWORD dwObjAlpha)
{
	DWORD dwResultAlpha = dwMtlAlpha & 0x000000ff;

	dwResultAlpha = (DWORD)( (float)dwResultAlpha * ((float)dwObjAlpha / 255.0f) );
	
	if (dwResultAlpha < 255)
	{
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );

		dwResultAlpha = dwResultAlpha <<24;
		pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		pDevice->SetRenderState(D3DRS_TEXTUREFACTOR , dwResultAlpha);

		pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );
		pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		
	}
	else 
	{
		pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
		
	}

	if (dwMtlAlpha & ALPHA_TEX_PROPERTY_MASK)
	{
		if (dwMtlAlpha & TEX_FLAG_ALPHA_OPASITY)
		{
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
			pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

			pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );			
		} 
		if (dwMtlAlpha & TEX_FLAG_ALPHA_TRANSP_ADD)
		{
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
			pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);


		}
		else if (dwMtlAlpha & TEX_FLAG_ALPHA_TRANSP_SUB)
		{
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
			pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
			pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
		}
	}
}

void __stdcall EndAlphaMeshObject(LPDIRECT3DDEVICE8 pDevice)
{
//	if (!dwObjAlpha)
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
//	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
//	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);

}
void SetColorFactorShader(LPDIRECT3DDEVICE8 pDevice,D3DCOLOR color,DWORD dwTexStageIndex )
{
    pDevice->SetRenderState( D3DRS_TEXTUREFACTOR , color );

    pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_COLORARG2, D3DTA_TFACTOR );
    pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 );
    pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
}

void RestoreTextureShader(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex)
{
	pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    
}


void SetHFieldTexMatrix(MATRIX4* pMat,VECTOR3* pv3Rect)
{
	// uvw좌표는 각각 다음과 같이 구해진다.
	// u = x-x0 / width
	// v = z-z0 / height

	float	width = pv3Rect[3].x - pv3Rect[0].x;
	float	height = pv3Rect[1].z - pv3Rect[0].z;

	memset(pMat,0,sizeof(MATRIX4));
	pMat->_11 = 1.0f / width;
	pMat->_32 = 1.0f / height;
	pMat->_41 = -1.0f * pv3Rect[0].x / width;
	pMat->_42 = -1.0f * pv3Rect[0].z / height;
	pMat->_44 = 1.0f;
}

/*
void SetHFieldTexMatrix0(MATRIX4* pMat,VECTOR3* pv3Rect,float fFaceSize,DWORD dwWidthHeight)
{
	// uvw좌표는 각각 다음과 같이 구해진다.
	// u = x-x0 / fFaceSize
	// v = z-z0 / fFaceSize

	pMat->_11 = 1.0f / fFaceSize;
	pMat->_12  = 0.0f;
	pMat->_13  = 0.0f;
	pMat->_14  = 0.0f;
	
	pMat->_21  = 0.0f;
	pMat->_22  = 0.0f;
	pMat->_23  = 0.0f;
	pMat->_24  = 0.0f;

	pMat->_31  = 0.0f;
	pMat->_32 = 1.0f / fFaceSize;
	pMat->_33  = 0.0f;
	pMat->_34  = 0.0f;

	pMat->_41 = -1.0f * pv3Rect[0].x / fFaceSize;
	pMat->_42 = -1.0f * pv3Rect[0].z / fFaceSize;
	pMat->_43  = 0.0f;
	pMat->_44 = 1.0f;

}
void SetHFieldTexMatrix1(MATRIX4* pMat,VECTOR3* pv3Rect,float fFaceSize,DWORD dwWidthHeight)
{
	// uvw좌표는 각각 다음과 같이 구해진다.
	// u = x-x0 / fFaceSize
	// v = z-z0 / fFaceSize

	pMat->_11 = 1.0f / fFaceSize;
	pMat->_12  = 0.0f;
	pMat->_13  = 0.0f;
	pMat->_14  = 0.0f;
	
	pMat->_21  = 0.0f;
	pMat->_22  = 0.0f;
	pMat->_23  = 0.0f;
	pMat->_24  = 0.0f;

	pMat->_31  = 0.0f;
	pMat->_32 = 1.0f / fFaceSize;
	pMat->_33  = 0.0f;
	pMat->_34  = 0.0f;

	pMat->_41 = -1.0f * pv3Rect[0].x / fFaceSize - 1.0f;
	pMat->_42 = -1.0f * pv3Rect[0].z / fFaceSize;
	pMat->_43  = 0.0f;
	pMat->_44 = 1.0f;

}
void SetHFieldTexMatrix2(MATRIX4* pMat,VECTOR3* pv3Rect,float fFaceSize,DWORD dwWidthHeight)
{
	// uvw좌표는 각각 다음과 같이 구해진다.
	// u = x-x0 / fFaceSize
	// v = z-z0 / fFaceSize

	pMat->_11 = 1.0f / fFaceSize;
	pMat->_12  = 0.0f;
	pMat->_13  = 0.0f;
	pMat->_14  = 0.0f;
	
	pMat->_21  = 0.0f;
	pMat->_22  = 0.0f;
	pMat->_23  = 0.0f;
	pMat->_24  = 0.0f;

	pMat->_31  = 0.0f;
	pMat->_32 = 1.0f / fFaceSize;
	pMat->_33  = 0.0f;
	pMat->_34  = 0.0f;

	pMat->_41 = -1.0f * pv3Rect[0].x / fFaceSize;
	pMat->_42 = -1.0f * pv3Rect[0].z / fFaceSize - 1.0f;
	pMat->_43  = 0.0f;
	pMat->_44 = 1.0f;

}
void SetHFieldTexMatrix3(MATRIX4* pMat,VECTOR3* pv3Rect,float fFaceSize,DWORD dwWidthHeight)
{
	// uvw좌표는 각각 다음과 같이 구해진다.
	// u = x-x0 / fFaceSize
	// v = z-z0 / fFaceSize

	pMat->_11 = 1.0f / fFaceSize;
	pMat->_12  = 0.0f;
	pMat->_13  = 0.0f;
	pMat->_14  = 0.0f;
	
	pMat->_21  = 0.0f;
	pMat->_22  = 0.0f;
	pMat->_23  = 0.0f;
	pMat->_24  = 0.0f;

	pMat->_31  = 0.0f;
	pMat->_32 = 1.0f / fFaceSize;
	pMat->_33  = 0.0f;
	pMat->_34  = 0.0f;

	pMat->_41 = -1.0f * pv3Rect[0].x / fFaceSize - 1.0f;
	pMat->_42 = -1.0f * pv3Rect[0].z / fFaceSize - 1.0f;
	pMat->_43  = 0.0f;
	pMat->_44 = 1.0f;

}*/
/*
void SetHFieldTexMatrix4(MATRIX4* pMatQuad,VECTOR3* pv3Rect,float fFaceSize,DWORD dwWidthHeight,DWORD dwDirFlag)
{
	// uvw좌표는 각각 다음과 같이 구해진다.
	// u = x-x0 / fFaceSize
	// v = z-z0 / fFaceSize

	pMatQuad->_11 = 1.0f / fFaceSize;
	pMatQuad->_12  = 0.0f;
	pMatQuad->_13  = 0.0f;
	pMatQuad->_14  = 0.0f;
	
	pMatQuad->_21  = 0.0f;
	pMatQuad->_22  = 0.0f;
	pMatQuad->_23  = 0.0f;
	pMatQuad->_24  = 0.0f;

	pMatQuad->_31  = 0.0f;
	pMatQuad->_32 = 1.0f / fFaceSize;
	pMatQuad->_33  = 0.0f;
	pMatQuad->_34  = 0.0f;

	pMatQuad->_41 = -1.0f * pv3Rect[0].x / fFaceSize;
	pMatQuad->_42 = -1.0f * pv3Rect[0].z / fFaceSize;
	pMatQuad->_43  = 0.0f;
	pMatQuad->_44 = 1.0f;

	pMatQuad[3] = pMatQuad[2] = pMatQuad[1] = pMatQuad[0];  

	DWORD	dwRelativeIndex[4];

	// dwDirFlag 는 0부터 3까지...
	DWORD	dwPivotIndex = dwDirFlag;
//	dwRelativeIndex[0] = dwDirFlag;
//	dwRelativeIndex[1] = (dwDirFlag+1)%4;
//	dwRelativeIndex[2] = (dwDirFlag+2)%4;;
//	dwRelativeIndex[3] = (dwDirFlag+3)%4;;


	

	pMatQuad[1]._41 = -1.0f * pv3Rect[0].x / fFaceSize - 1.0f;
	pMatQuad[1]._42 = -1.0f * pv3Rect[0].z / fFaceSize;

	pMatQuad[2]._41 = -1.0f * pv3Rect[0].x / fFaceSize;
	pMatQuad[2]._42 = -1.0f * pv3Rect[0].z / fFaceSize - 1.0f;

	pMatQuad[3]._41 = -1.0f * pv3Rect[0].x / fFaceSize - 1.0f;
	pMatQuad[3]._42 = -1.0f * pv3Rect[0].z / fFaceSize - 1.0f;

}
*/

void SetHFieldTexMatrix4(MATRIX4* pMatQuad,VECTOR3* pv3Rect,float fFaceSize,DWORD dwWidthHeight,DWORD dwDirFlag)
{
	VECTOR3	v3Rect[4];
	memcpy(v3Rect,pv3Rect,sizeof(v3Rect));


	// uvw좌표는 각각 다음과 같이 구해진다.
	// u = x-x0 / fFaceSize
	// v = z-z0 / fFaceSize

	pMatQuad->_11 = 1.0f / fFaceSize;
	pMatQuad->_12  = 0.0f;
	pMatQuad->_13  = 0.0f;
	pMatQuad->_14  = 0.0f;
	
	pMatQuad->_21  = 0.0f;
	pMatQuad->_22  = 0.0f;
	pMatQuad->_23  = 0.0f;
	pMatQuad->_24  = 0.0f;

	pMatQuad->_31  = 0.0f;
	pMatQuad->_32 = 1.0f / fFaceSize;
	pMatQuad->_33  = 0.0f;
	pMatQuad->_34  = 0.0f;


	pMatQuad->_43  = 0.0f;
	pMatQuad->_44 = 1.0f;

	pMatQuad[3] = pMatQuad[2] = pMatQuad[1] = pMatQuad[0];  


	pMatQuad[0]._41 = -1.0f * v3Rect[0].x / fFaceSize;
	pMatQuad[0]._42 = -1.0f * v3Rect[0].z / fFaceSize;

	pMatQuad[1]._41 = -1.0f * v3Rect[0].x / fFaceSize - 1.0f;
	pMatQuad[1]._42 = -1.0f * v3Rect[0].z / fFaceSize;

	pMatQuad[2]._41 = -1.0f * v3Rect[0].x / fFaceSize;
	pMatQuad[2]._42 = -1.0f * v3Rect[0].z / fFaceSize - 1.0f;

	pMatQuad[3]._41 = -1.0f * v3Rect[0].x / fFaceSize - 1.0f;
	pMatQuad[3]._42 = -1.0f * v3Rect[0].z / fFaceSize - 1.0f;



	MATRIX4	matAdjust;
	memset(&matAdjust,0,sizeof(matAdjust));
	
	switch (dwDirFlag)
	{
	case 0:
		// 0도
		matAdjust._11 = 1.0f;
		matAdjust._22 = 1.0f;
		matAdjust._33 = 1.0f;
		matAdjust._44 = 1.0f;
		break;
	case 1:
		// 90도
		matAdjust._12 = 1.0f;
		matAdjust._21 = 1.0f;
		matAdjust._33 = 1.0f;
		matAdjust._41 = 0.0f;
		matAdjust._42 = 1.0f;
		matAdjust._44 = 1.0f;
		break;
	case 2:
		// 180도
		matAdjust._11 = 1.0f;
		matAdjust._22 = 1.0f;
		matAdjust._33 = 0.0f;
		matAdjust._41 = 1.0f;
		matAdjust._42 = 1.0f;
		matAdjust._44 = 1.0f;
		break;
	case 3:
		// 270도
		matAdjust._12 = 1.0f;
		matAdjust._21 = 1.0f;
		matAdjust._33 = 1.0f;
		matAdjust._41 = 1.0f;
		matAdjust._42 = 0.0f;
		matAdjust._44 = 1.0f;
		break;
	}
	for (DWORD i=0; i<4; i++)
	{
		MatrixMultiply2(pMatQuad+i,pMatQuad+i,&matAdjust);
	}
/*
	MatrixMultiply2(pMatQuad+0,pMatQuad+0,&matAdjust);
	MatrixMultiply2(pMatQuad+1,pMatQuad+1,&matAdjust);
	MatrixMultiply2(pMatQuad+2,pMatQuad+2,&matAdjust);
	MatrixMultiply2(pMatQuad+3,pMatQuad+3,&matAdjust);
	*/

}



HRESULT SetAdjustedProjectionMatrix( MATRIX4* mat, FLOAT fFOV, FLOAT fAspect,
                                     FLOAT fNearPlane, FLOAT fFarPlane,
                                     FLOAT fPixDx, FLOAT fPixDy, FLOAT fVPWidth, 
                                     FLOAT fVPHeight )
{
    if( fabs(fFarPlane-fNearPlane) < 0.01f )
        return E_INVALIDARG;
    if( fabs(sin(fFOV/2)) < 0.01f )
        return E_INVALIDARG;

    float h =   1.0f  * ( cosf(fFOV/2)/sinf(fFOV/2) );

    float fTop = fNearPlane/h;
    float fBottom = -fTop;
    float fRight = fTop * fAspect;
    float fLeft = -fRight;

    float fXWSize = fRight - fLeft;
    float fYWSize = fTop - fBottom;

    float fDx = -( fPixDx*fXWSize/fVPWidth );
    float fDy = -( fPixDy*fYWSize/fVPHeight );
    
    SetFrustumMatrix( mat, fLeft+fDx, fRight+fDx, fTop+fDy, fBottom+fDy, fNearPlane,
        fFarPlane );

    return S_OK;
}



//-----------------------------------------------------------------------------
// Name: SetFrustumMatrix()
// Desc: Creates a projection matrix base on frustum dimension paramters.
//-----------------------------------------------------------------------------
void SetFrustumMatrix( MATRIX4* mat, 
                                     FLOAT fLeft, FLOAT fRight, FLOAT fTop,
                                     FLOAT fBottom, FLOAT fNearPlane, FLOAT fFarPlane )
{
    float Q = fFarPlane / ( fFarPlane - fNearPlane );

    ZeroMemory( mat, sizeof(MATRIX4) );
    mat->_11 = ( 2.0f*fNearPlane )/( fRight - fLeft );
    mat->_22 = ( 2.0f*fNearPlane )/( fTop - fBottom );
    mat->_31 = ( fRight + fLeft )/ (fRight - fLeft );
    mat->_32 = ( fTop + fBottom )/ (fTop - fBottom );
    mat->_33 = Q;
    mat->_34 = 1.0f;
    mat->_43 = -Q*fNearPlane;
}


void RestoreLightTexture(LPDIRECT3DTEXTURE8 pLightTexture,TEXTURE_PLANE* pTexPlane,DWORD dwTexPlaneNum)
{
	D3DLOCKED_RECT	rect;
	pLightTexture->LockRect(0,&rect,NULL,0);
	SetBitmapFromTexturePlane((char*)rect.pBits,rect.Pitch,pTexPlane,dwTexPlaneNum);
	pLightTexture->UnlockRect(0);
}


void OutputErrorMsg(CoD3DDevice* pRenderer,HRESULT hr,char* szMsg)
{
	char	txt[512];
	char	e_out_of_memeory[] = "E_OUTOFMEMORY";
	char	out_of_video_memeory[] = "D3DERR_OUTOFVIDEOMEMORY";
	char	invalid_call[] = "D3DERR_INVALIDCALL";
	char	unknown[] = "Unknown Error";

	char*	pRet = unknown;

	
	if (hr == D3DERR_OUTOFVIDEOMEMORY)
		pRet = out_of_video_memeory;
	else if (hr == E_OUTOFMEMORY)
		pRet = e_out_of_memeory;
	else if (hr ==  D3DERR_INVALIDCALL )
		pRet = invalid_call;

	wsprintf(txt,"%s - %s",szMsg,pRet);
	MessageBox(NULL,txt,"Error",MB_OK);
	DWORD	dwAddr;
	GetEIP(&dwAddr);
	g_pErrorHandleFunc(ERROR_TYPE_DEVICE_NOT_SUPPROT,0,(void*)dwAddr,txt);
}


//	스케일 다운.
void ScaleDownTexture565( WORD* pTex, WORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale)
{
	DWORD	dwImageWidth	=	dwWidth/dwScale;
	DWORD	dwImageHeight	=	dwHeight/dwScale;
//	DWORD	dwImageBPP		=	16;		// 지금은 무조껀 16비트.
	
	// 작아진 이미지의 한 도트는 원본 이미지의 dwScale*dwScale만큼의 도트를 평균 낸것이다.

	DWORD	x = 0, y = 0;
	for( y = 0; y < dwImageHeight; y++)	// 다운 스케일된 이미지를 기준으로 루프를 돈다.
	{
		for( x = 0; x < dwImageWidth; x++)
		{
			// dwScale이 2일 경우 결과좌표 x,y의 점은 원본좌표 x,y; x+1,y; x,y+1; x+1,y+1의 각 요소의 평균값이다.
			// 원본 텍스쳐에서 dwScale*dwScale만큼 샘플링.
			DWORD	ComR	=	0;
			DWORD	ComG	=	0;
			DWORD	ComB	=	0;
			DWORD	u = 0, v = 0;
			for(v = 0; v < dwScale; v++)
			{
				for( u = 0; u < dwScale; u++)
				{
//					WORD	Component = pTex[(x*dwScale) + u + ((y+v)*dwScale*(dwWidth))];
//					WORD	Component = pTex[(x*dwScale) + u + ((y*dwScale+v)*(dwWidth))];
					WORD	Component = pTex[   (y*dwScale+v)*dwWidth    + (x*dwScale)+u];
					ComR = ComR + ((Component&0xf800) >> 11);		// 비트 배열이 바뀌면 여길 수정.
					ComG = ComG + ((Component&0x07e0) >> 5);
					ComB = ComB + ((Component&0x001f) );
				}
			}
			ComR = (ComR /(dwScale*dwScale)) << 11;		// dwScale*dwScale로 나누고 11비트 왼쪽으로.
			ComG = (ComG /(dwScale*dwScale)) << 5;		// dwScale*dwScale로 나누고 5비트 왼쪽으로.
			ComB = ComB /(dwScale*dwScale);				// dwScale*dwScale로 나누기만..
			pDest[x+y*dwImageWidth] = unsigned short (ComR + ComG + ComB);
		}
	}

}

void ScaleDownTexture4444( WORD* pTex, WORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale)
{
	DWORD	dwImageWidth	=	dwWidth/dwScale;
	DWORD	dwImageHeight	=	dwHeight/dwScale;
//	DWORD	dwImageBPP		=	16;		// 지금은 무조껀 16비트.
	
	// 작아진 이미지의 한 도트는 원본 이미지의 dwScale*dwScale만큼의 도트를 평균 낸것이다.

	DWORD	x = 0, y = 0;
	for( y = 0; y < dwImageHeight; y++)	// 다운 스케일된 이미지를 기준으로 루프를 돈다.
	{
		for( x = 0; x < dwImageWidth; x++)
		{
			// dwScale이 2일 경우 결과좌표 x,y의 점은 원본좌표 x,y; x+1,y; x,y+1; x+1,y+1의 각 요소의 평균값이다.
			// 원본 텍스쳐에서 dwScale*dwScale만큼 샘플링.
			DWORD	ComR	=	0;
			DWORD	ComG	=	0;
			DWORD	ComB	=	0;
			DWORD	ComA	=	0;
			DWORD	u = 0, v = 0;
			for(v = 0; v < dwScale; v++)
			{
				for( u = 0; u < dwScale; u++)
				{
					WORD	Component = pTex[   (y*dwScale+v)*dwWidth    + (x*dwScale)+u];
					ComR	=	ComR	+	((Component&0xf000) >> 12);
					ComG	=	ComG	+	((Component&0x0f00) >> 8);
					ComB	=	ComB	+	((Component&0x00f0) >> 4);
					ComA	=	ComA	+	((Component&0x000f));
				}
			}
			ComR	=	(ComR / (dwScale*dwScale)) << 12;
			ComG	=	(ComG / (dwScale*dwScale)) << 8;
			ComB	=	(ComB / (dwScale*dwScale)) << 4;
			ComA	=	(ComA / (dwScale*dwScale));
			pDest[x+y*dwImageWidth]	=	unsigned short (ComR + ComG + ComB + ComA);
		}
	}

}

void ScaleDownTexture8888(DWORD* pTex, DWORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale)
{
	DWORD	dwImageWidth	=	dwWidth/dwScale;
	DWORD	dwImageHeight	=	dwHeight/dwScale;
//	DWORD	dwImageBPP		=	16;		// 지금은 무조껀 16비트.
	
	// 작아진 이미지의 한 도트는 원본 이미지의 dwScale*dwScale만큼의 도트를 평균 낸것이다.

	DWORD	x = 0, y = 0;
	for( y = 0; y < dwImageHeight; y++)	// 다운 스케일된 이미지를 기준으로 루프를 돈다.
	{
		for( x = 0; x < dwImageWidth; x++)
		{
			// dwScale이 2일 경우 결과좌표 x,y의 점은 원본좌표 x,y; x+1,y; x,y+1; x+1,y+1의 각 요소의 평균값이다.
			// 원본 텍스쳐에서 dwScale*dwScale만큼 샘플링.
			DWORD	ComR	=	0;
			DWORD	ComG	=	0;
			DWORD	ComB	=	0;
			DWORD	ComA	=	0;
			DWORD	u = 0, v = 0;
			for(v = 0; v < dwScale; v++)
			{
				for( u = 0; u < dwScale; u++)
				{
					DWORD	Component = pTex[   (y*dwScale+v)*dwWidth    + (x*dwScale)+u];
					ComR	=	ComR	+	((Component & 0xff000000)>>24);
					ComG	=	ComG	+	((Component & 0x00ff0000)>>16);
					ComB	=	ComB	+	((Component & 0x0000ff00)>>8);
					ComA	=	ComA	+	(Component & 0x000000ff);
				}
			}
			ComR	=	(ComR / (dwScale*dwScale)) << 24;
			ComG	=	(ComG / (dwScale*dwScale)) << 16;
			ComB	=	(ComB / (dwScale*dwScale)) << 8;
			ComA	=	(ComA / (dwScale*dwScale));
			pDest[x+y*dwImageWidth]	=	(ComR | ComG | ComB | ComA);
		}
	}

}





//----------------------------------------------------------------------------
// Name : CFont::DIB24To16
// Description :
//---------------------------------------------------------------------------
BOOL DIB24To16(BYTE* pIn, BYTE* pOut, int iWidth, int iHeight)
{
	// Pixel converting.

	int iPitch = (((iWidth * 24) + 31) & ~31) >> 3;
	
	int i = 0;
	int j = 0;
	int iCount = iHeight * iWidth -1;

	for (i = 0; i < iHeight; i++)
	{		
		for (j = iWidth -1; j >= 0; j--)
		{
			// save to turn around pixel image		

			*((WORD*)pOut + iCount) = 0xF000 | (pIn[iPitch * i + (j * 3) + 0] >> 4) << 8 |
											   (pIn[iPitch * i + (j * 3) + 1] >> 4) << 4 |
											   (pIn[iPitch * i + (j * 3) + 2] >> 4);

			// Set Alpha value.

			if (0xF000 == *((WORD*)pOut + iCount))
			{
				*((WORD*)pOut + iCount) = 0;
			}

			--iCount;
		}
	}

	return TRUE;
}

/*
void ShadeLightTexture(LPDIRECT3DTEXTURE8 pLightTexture,TEXTURE_PLANE* pTexPlane,DWORD dwTexPlaneNum,LIGHT_DESC* pLight,DWORD dwLightNum,DWORD dwOffsetX,DWORD dwOffsetY)
{
	
	VECTOR3		dir;
	float		cosang;
	DWORD		i,j,m;


	DWORD		dwCount = 0;

	if (dwLightNum > MAX_PLANE_APPLIED_LIGHT_NUM)
		dwLightNum = MAX_PLANE_APPLIED_LIGHT_NUM;

//	RestoreLightTexture();


	D3DLOCKED_RECT	rect;
	pLightTexture->LockRect(0,&rect,NULL,0);
	
	DWORD		color[4] = {0xff,0xff,0x00,0x00};	// a r g b
	DWORD		lit_color[4];
	DWORD		light_color[4];
	DWORD		src_color[4];
	float		distance;

	DWORD		k;
		
	lit_color[3] = 0xffffffff;

	PATCH*			pPatch;
	VECTOR3*		pv3Normal;
	

	for (m=0; m<dwTexPlaneNum; m++)
	{
		pv3Normal = &pTexPlane[m].v3Normal;

		pPatch = pTexPlane[m].pPatch;
		for (i=0; i<pTexPlane[m].dwPatchNum; i++)
		{
			src_color[0] = (pPatch[i].dwColor & 0x000000ff);
			src_color[1] = (pPatch[i].dwColor & 0x0000ff00)>>8;
			src_color[2] = (pPatch[i].dwColor & 0x00ff0000)>>16;
					
			lit_color[0] = 0;
			lit_color[1] = 0;
			lit_color[2] = 0;


			
			for (j=0; j<dwLightNum; j++)
			{
				dir = pLight[j].v3Point - pPatch[i].v3Point;
				distance = (float)sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
				if (distance > pLight[j].fRs)
					goto lb_add_light;

				dir = dir/distance;

				light_color[0] = (pLight[j].dwColor & 0x000000ff);
				light_color[1] = (pLight[j].dwColor & 0x0000ff00)>>8;
				light_color[2] = (pLight[j].dwColor & 0x00ff0000)>>16;

				cosang = pv3Normal->x*dir.x + pv3Normal->y*dir.y + pv3Normal->z*dir.z;

				if (cosang > 0.0f)
				{					
					cosang = cosang*((pLight[j].fRs - distance) / pLight[j].fRs)*1.5f;
					for (k=0; k<3; k++)
					{
						lit_color[k] += (DWORD)(cosang*(float)light_color[k]);
					}
				}
			}
			
lb_add_light:
			for (j=0; j<3; j++)
			{
				// 순서 이거 맞음...광도가 255넘어가도 텍스쳐 버퍼에 넣을때 어차피 짤라낼것이므로..
				pPatch[i].dwLightColor[j] = lit_color[j];
				lit_color[j] += src_color[j];

				if (lit_color[j]>0xff)
					lit_color[j]= 0xff;
			}
						
			*(WORD*)((char*)rect.pBits + (pPatch[i].sx+dwOffsetX)*2 + (pPatch[i].sy+dwOffsetY)*rect.Pitch) = 
				(WORD)( (lit_color[2]>>3<<11) | (lit_color[1]>>2<<5) | (lit_color[0]>>3) );



		}
	}
	pLightTexture->UnlockRect(0);
lb_return:
	return;
}
*/

/*
BOOL SetTextureWithBitmap(LPDIRECT3DDEVICE8 pDevice,LPDIRECT3DTEXTURE8 pTexDest,IMAGE_DESC* pImgDesc,D3DFORMAT format)
{
	BOOL				bResult = FALSE;
	DWORD				i,j,div_const,width,height;

	DWORD	miplevelnum = pTexDest->GetLevelCount();

	char*	pTemp = new char[pImgDesc->imgHeader.dwWidth * pImgDesc->imgHeader.dwHeight * pImgDesc->imgHeader.dwBPS];
	memset(pTemp,0,pImgDesc->imgHeader.dwWidth * pImgDesc->imgHeader.dwHeight * pImgDesc->imgHeader.dwBPS);

	for (i=0; i<miplevelnum; i++)
	{
		char*		pImg = pImgDesc->pBits;
		div_const =1;
		if (!i)
			goto lb_set_pos;
		
		for (j=0; j<i; j++)
			div_const *= 2;

lb_set_pos:
		width = pImgDesc->imgHeader.dwWidth / div_const;
		height = pImgDesc->imgHeader.dwHeight / div_const;
		
		if (div_const > 1)
		{
			// 맵맵에 텍스쳐를 바른다..
			pImg = pTemp;
			if (D3DFMT_A4R4G4B4 == format)
			{
				ScaleDownTexture4444(
					(WORD*)pImgDesc->pBits,
					(WORD*)pImg,
					pImgDesc->imgHeader.dwWidth,
					pImgDesc->imgHeader.dwHeight,
					div_const);
			}
			else
			{
				ScaleDownTexture565(
					(WORD*)pImgDesc->pBits,
					(WORD*)pImg,
					pImgDesc->imgHeader.dwWidth,
					pImgDesc->imgHeader.dwHeight,
					div_const);
			}
		}		
		SetSurfaceWithBitmap(pDevice,i,pTexDest,pImg,width,height,format);
	}

	delete [] pTemp;
	bResult = TRUE;

lb_return:
	return bResult;
	
}
*/

/*
void DefaultAlphaSetting(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex)
{
	pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	pDevice->SetTextureStageState( dwTexStageIndex, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
}
*/