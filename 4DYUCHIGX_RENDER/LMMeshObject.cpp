#include "CoD3DDevice.h"
#include "VBHeap.h"
#include "../4DyuchiGXGFunc/global.h"
#include "LMMeshObject.h"
#include "d3d_helper.h"
#include "LMFaceGroup.h"
#include "render_type.h"


CLMMeshObject::CLMMeshObject()
{
	memset((char*)this+4,0,sizeof(CLMMeshObject)-4);

	m_bCanUpdate = TRUE;
	m_fRenderPriorityFactor = RENDER_PRIORITY_FACTOR_LMOBJECT;
}

BOOL __stdcall CLMMeshObject::StartInitialize(MESH_DESC* pDesc,IGeometryController* pControl,IGeometryControllerStatic* pControlStatic)
{
	BOOL		bResult = FALSE;
	
	m_pMeshController = pControl;
	m_meshFlag = pDesc->meshFlag;
	
	m_dwMaxFaceGroupNum = pDesc->dwFaceGroupNum;
	m_pFaceGroup = new CLMFaceGroup[m_dwMaxFaceGroupNum];
	m_pv3Local = new VECTOR3[pDesc->dwVertexNum];

	
	m_dwVertexNum = pDesc->dwVertexNum;
	TransformVector3_VPTR2(m_pv3Local,pDesc->pv3WorldList,pDesc->pMatrixWorldInverse,pDesc->dwVertexNum);
	
	m_ptvTex = pDesc->ptvTexCoordList;
	
	m_pImageBuffer = pDesc->LightTexture.pBits;
	m_dwTexturePlaneNum = pDesc->LightTexture.dwTexPlaneNum;
	m_pTexturePlane = pDesc->LightTexture.pTexPlane;
	m_dwSurfaceWidth = pDesc->LightTexture.dwSurfaceWidth;
	m_dwSurfaceHeight = pDesc->LightTexture.dwSurfaceHeight;

	if (pDesc->LightTexture.dwSurfaceWidth*pDesc->LightTexture.dwSurfaceHeight*pDesc->LightTexture.dwBPS)
	{
		CreateStaticLightTexture(
			pDesc->LightTexture.pBits,
			pDesc->LightTexture.dwSurfaceWidth,
			pDesc->LightTexture.dwSurfaceHeight,
			pDesc->LightTexture.dwBPS);
	}
	
	
	DWORD	dwSize;
	DWORD	dwErrorCode;
	if (!m_pRenderer->GetVBVector3MeshHeap()->AllocVB(&dwErrorCode,&m_VBProjected,pDesc->dwVertexNum,&dwSize,&m_VBProjected.m_bEnable,NULL))
		__asm int 3
	m_VBProjected.SetVertexPosList(m_pv3Local,pDesc->dwVertexNum);

	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL __stdcall CLMMeshObject::InsertFaceGroup(FACE_DESC* pDesc)
{
	BOOL			bResult = FALSE;

	DIMATERIAL*		pMtl;
	if (!m_pFaceGroup[m_dwFaceGroupNum].CreateFaceGroup(
		m_pRenderer,
		m_pRenderer->GetDevice(),
		m_pv3Local,m_ptvTex,
		pDesc->pIndex,
		pDesc->ptUVLight1,
		pDesc->dwFacesNum,
		CREATE_FACEGROUP_TYPE_STATIC))
		goto lb_return;

	pMtl = m_pRenderer->AllocMaterial(pDesc->pMaterial,NULL,NULL,0);
	m_pFaceGroup[m_dwFaceGroupNum].SetMaterial(pMtl,pDesc->dwMtlIndex);

	m_dwFaceGroupNum++;
	m_dwTotalFacesNum += pDesc->dwFacesNum;

	bResult = TRUE;

lb_return:
	return bResult;
}

void __stdcall CLMMeshObject::EndInitialize()
{
	SortWithAlpha();


	if (m_pv3Local)
	{
		delete [] m_pv3Local;
		m_pv3Local = NULL;
	}
}


BOOL __stdcall CLMMeshObject::Render(
		DWORD dwRefIndex,										// 애니메이션 컨트롤블럭
		DWORD dwAlpha,
		LIGHT_INDEX_DESC* pDynamicLightIndexList,DWORD dwLightNum,
		LIGHT_INDEX_DESC* pSpotLightIndexList,DWORD dwSpotLightNum,
		DWORD dwMtlSetIndex,
		DWORD dwEffectIndex,
		DWORD dwFlag					
		)
{	

	CAMERA_DESC*		pCamera;
	DWORD				i,j;
	LPDIRECT3DDEVICE8	pDevice = m_pRenderer->GetDevice();
	BOOL				bLight = FALSE;
	BOOL				bShadow = FALSE;
	LIGHT_DESC*			pLight;

	
	LPDIRECT3DTEXTURE8	pLightTex;
	MATRIX4*			pResult = m_pRenderer->GetIdentityMatrix();
	LPDIRECT3DTEXTURE8	pDefaultTex = m_pRenderer->GetDefaultMaterial()->TextureDiffuse.GetTexture();
	

	if (dwFlag & RENDER_TYPE_NOT_DRAW)
		goto lb_return;


	if (dwFlag & RENDER_TYPE_DISABLE_ZCLIP)
		pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);


	m_pMeshController->ApplyResultMatrix(&pResult,dwRefIndex,0);
	pDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)pResult);
	
	
	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,0);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);
	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);

	pDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX,1);
	pDevice->SetTextureStageState( 1, D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
	pDevice->SetTextureStageState( 1, D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);
	pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
	pDevice->SetVertexShader( D3DFVF_D3DDUALVERTEX);


	pDevice->SetTextureStageState( 2, D3DTSS_COLOROP,   D3DTOP_DISABLE );

	if (m_pRenderer->INL_GetLightMapFlag())
	{
		pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		m_TexStaticLight.SetTexture(pDevice,1);
	}
	else
	{
		pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	}

	if (m_pRenderer->INL_IsEnableFog())
		pDevice->SetRenderState(D3DRS_FOGENABLE,TRUE);

	if (m_meshFlag.IsDisableZBubfferWrite())
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

	DIMATERIAL*	pMtl;
	for (i=0; i<m_dwFaceGroupNum; i++)
	{
	//	pMtl = m_pRenderer->GetMaterial(1,i);
		pMtl = m_pFaceGroup[i].GetMaterial();

			
		if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

		if (m_pRenderer->GetLightMapFlag() & LIGHTMAP_FLAG_DISABLE_TEXTURE)
			m_pRenderer->GetDefaultMaterial()->TextureDiffuse.SetTexture(pDevice,0);
		else
			pMtl->TextureDiffuse.SetTexture(pDevice,0);
		
//		m_pRenderer->GetDefaultMaterial()->Texture.SetTexture(pDevice,0);

		// 알파블랜딩 시작
		BeginAlphaMeshObject(pDevice,2,pMtl->dwAlpha,dwAlpha);
	
		pDevice->SetStreamSource( 0,m_pFaceGroup[i].GetVertexBuffer(),m_pFaceGroup[i].GetVertexSize());		
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST,m_pFaceGroup[i].GetVBStartIndex(),m_pFaceGroup[i].GetFacesNum());		

		if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

		// 알파블랜딩 끝
		EndAlphaMeshObject(pDevice);
	}
	
	if (m_pRenderer->INL_IsEnableFog())
		pDevice->SetRenderState(D3DRS_FOGENABLE,FALSE);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
	
	if (m_meshFlag.IsDisableZBubfferWrite())
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);



	if ((dwFlag & RENDER_TYPE_UPDATE_SHADING) && dwLightNum)
		bLight = TRUE;

	if ( (dwFlag & RENDER_TYPE_RECV_SHADOW) && dwSpotLightNum)
		bShadow = TRUE;

	BOOL	bCameraUpdate;
	
	if ( !(bLight | bShadow) )
		goto lb_return;


	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);
	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_CAMERASPACEPOSITION );

	//////////////////////////////////////////////////////
	// 카메라가 바뀌지 않았다면 이 부분도 스킵할 수 있다.
	//////////////////////////////////////////////////////

	pCamera = m_pRenderer->INL_GetCameraDesc();

	pDevice->SetStreamSource( 0,m_VBProjected.GetVertexBuffer(),m_VBProjected.GetVertexSize());
	pDevice->SetVertexShader(m_VBProjected.GetVertexShader());

	float	fDist;
	fDist = CalcDistance(&pCamera->v3From,&m_v3CameraPointPrv);
	if (fDist < MIN_UNIT*2.0f)
	{
		bCameraUpdate = FALSE;
		goto lb_light;
	}
	m_v3CameraPointPrv = pCamera->v3From;
	bCameraUpdate = TRUE;

	for (i=0; i<m_dwFaceGroupNum; i++)
	{
		m_pFaceGroup[i].CullFaceListWithCameraEye(&pCamera->v3From);
	}

lb_light:
	if (bLight)
	{
		pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2 );
		pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2 );
		pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );

		pDevice->SetTextureStageState( 1, D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
		pDevice->SetTextureStageState( 1, D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);
		pDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_CAMERASPACEPOSITION );
		
		
		pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
		pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
		

		

		MATRIX4		matAdjustUV0,matAdjustUV1;
		for (j=0; j<dwLightNum; j++)
		{

			pLight = m_pRenderer->INL_GetRTLightDesc((DWORD)pDynamicLightIndexList[j].bLightIndex);
			pLightTex = m_pRenderer->GetDynamicLightTexture(pLight->dwProjTexIndex);
			if (!pLightTex)
				continue;

			pDevice->SetTexture(0,pLightTex);
			pDevice->SetTexture(1,pLightTex);
			// uvw좌표는 각각 다음과 같이 구해진다.
			// u = ((v3World.x - v3Light.x) / fRs ) / 2.0f +0.5f;
			// v = ((v3World.y - v3Light.y) / fRs ) / 2.0f +0.5f;
			// w = ((v3World.z - v3Light.z) / fRs ) / 2.0f +0.5f;
		
			SetLightTexMatrix(&matAdjustUV0,&pLight->v3Point,pLight->fRs);



			// 두번째 텍스쳐를 위한 좌표는 첫번째 좌표의 w성분을 u로 넣고 v는 0.5로 채운다.2차원 텍스쳐 한장으로 1차원까지 구현하기 위해
			memset(&matAdjustUV1,0,sizeof(MATRIX4));
			matAdjustUV1._31 = matAdjustUV0._11;
			matAdjustUV1._41 = matAdjustUV0._43;
			matAdjustUV1._42 = 0.5f;
			matAdjustUV1._44 = 1.0f;

			MatrixMultiply2(&matAdjustUV0,m_pRenderer->INL_GetInverseViewMatriex(),&matAdjustUV0);
			MatrixMultiply2(&matAdjustUV1,m_pRenderer->INL_GetInverseViewMatriex(),&matAdjustUV1);
			pDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + 0),(D3DMATRIX*)&matAdjustUV0);
			pDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + 1),(D3DMATRIX*)&matAdjustUV1);

			


			for (i=0; i<m_dwFaceGroupNum; i++)
			{
				if (!m_pFaceGroup[i].GetCulledWithCameraFacesNum())
					continue;
				
				DWORD	dwPrjLightFacesNum;
				dwPrjLightFacesNum = m_pFaceGroup[i].BuildPrjLightMesh(m_pRenderer->GetIBCache(),j,&pLight->v3Point,bCameraUpdate);
				if (!dwPrjLightFacesNum)
					continue;

				
//					pDevice->SetIndices(m_pFaceGroup[i].GetProjectedIndexBuffer(),m_VBProjected.GetStartIndex());
//					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBProjectedStartIndex(),m_pFaceGroup[i].GetCulledWithLightFacesNum());

				pDevice->SetIndices(m_pFaceGroup[i].GetIBPrjLightPtr(j)->GetIndexBuffer(),m_VBProjected.GetStartIndex());				
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
					0,
					m_dwVertexNum,
					m_pFaceGroup[i].GetIBPrjLightPtr(j)->GetStartIndex(),
					dwPrjLightFacesNum);

				m_pRenderer->AddLitFacesCount(dwPrjLightFacesNum);
			
			}
//			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
		}
	}

lb_exit_dynamic_lit:
	if (bShadow)
	{
		MATRIX4	matTex;
		SPOT_LIGHT_DESC*	pSpotDesc;
		
	
	    pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_PROJECTED | D3DTTFF_COUNT3 );
		pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		   	
		pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
		pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);


		for (DWORD m=0; m<dwSpotLightNum; m++)
		{	
			pSpotDesc = m_pRenderer->INL_GetSpotDesc(pSpotLightIndexList[m].bLightIndex);

			MatrixMultiply2(&matTex,m_pRenderer->INL_GetInverseViewMatriex(),&pSpotDesc->prjDesc.matPrjTex);

			pDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + 0),(D3DMATRIX*)&matTex);

			if (pSpotDesc->pMtl)
		//	if (pSpotLightIndexList[m].pMtlHandle)
			{
				// 라이트가 매터리얼 핸들을 가지고 있으므로 이미지 프로젝션 라이트다.
				if (pSpotLightIndexList[m].bTexOP == 0xff)
				{
					pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
					pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
				}

				DIMATERIAL*	pMtl = (DIMATERIAL*)pSpotLightIndexList[m].pMtlHandle;
				pMtl->TextureDiffuse.SetTexture(pDevice,0);
			}
			else
			{
				LPDIRECT3DTEXTURE8	pTex;
				if (pSpotDesc->pRenderTexture)
					pTex = pSpotDesc->pRenderTexture->GetTexture();
				else
					pTex = pDefaultTex;
				
				pDevice->SetTexture( 0,pTex);
			}
			VECTOR3*	pShadowLightPos;
			BOUNDING_SPHERE*	pShadowSenderPos;


			pShadowLightPos = &pSpotDesc->prjDesc.v3From;
			pShadowSenderPos = &pSpotDesc->prjDesc.sphereSenderShadow;
		

			for (i=0; i<m_dwFaceGroupNum; i++)
			{
				if (!m_pFaceGroup[i].GetCulledWithCameraFacesNum())
					continue;
				
				DWORD	dwPrjShadowFacesNum;
				dwPrjShadowFacesNum = m_pFaceGroup[i].BuildPrjShadowMesh(m_pRenderer->GetIBCache(),m,pShadowLightPos,pShadowSenderPos,bCameraUpdate);
				if (!dwPrjShadowFacesNum)
					continue;

				pDevice->SetIndices(m_pFaceGroup[i].GetIBPrjShadowPtr(m)->GetIndexBuffer(),m_VBProjected.GetStartIndex());				
				pDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,
					0,
					m_dwVertexNum,
					m_pFaceGroup[i].GetIBPrjShadowPtr(m)->GetStartIndex(),
					dwPrjShadowFacesNum);

				m_pRenderer->AddShadowedFacesCount(dwPrjShadowFacesNum);
			}
			if (pSpotLightIndexList[m].pMtlHandle && pSpotLightIndexList[m].bTexOP == 0xff)
			{
				pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
				pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
			}
		}
	}

lb_return:
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );

	if (dwFlag & RENDER_TYPE_DISABLE_ZCLIP)
		pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

	return TRUE;
}
BOOL CLMMeshObject::Render(DWORD dwColor)
{
	DWORD	i;
		
	LPDIRECT3DDEVICE8	pDevice = m_pRenderer->GetDevice();
	SetColorFactorShader(pDevice,dwColor,0);

	for (i=0; i<m_dwFaceGroupNum; i++)
	{
		pDevice->SetStreamSource( 0,m_pFaceGroup[i].GetVertexBuffer(),m_pFaceGroup[i].GetVertexSize());
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST,m_pFaceGroup[i].GetVBStartIndex(),m_pFaceGroup[i].GetFacesNum());
	}

	RestoreTextureShader(pDevice,0);

	return TRUE;
}
void CLMMeshObject::SortWithAlpha()
{
	CLMFaceGroup*	pFaceGroupTemp = (CLMFaceGroup*)malloc(sizeof(CLMFaceGroup)*m_dwFaceGroupNum);
	DWORD			dwAlphaIndex = m_dwFaceGroupNum-1;
	DWORD			dwNotAlphaIndex = 0;

	DIMATERIAL*	pMtl;
	for (DWORD i=0; i<m_dwFaceGroupNum; i++)
	{
		pMtl = m_pFaceGroup[i].GetMaterial();
		if (pMtl->dwAlpha)
		{
			memcpy(&pFaceGroupTemp[dwAlphaIndex],&m_pFaceGroup[i],sizeof(CLMFaceGroup));
			dwAlphaIndex--;
		}
		else
		{
			memcpy(&pFaceGroupTemp[dwNotAlphaIndex],&m_pFaceGroup[i],sizeof(CLMFaceGroup));
			dwNotAlphaIndex++;
		}
	}
	memcpy(m_pFaceGroup,pFaceGroupTemp,sizeof(CLMFaceGroup)*m_dwFaceGroupNum);
	free(pFaceGroupTemp);
}

BOOL __stdcall CLMMeshObject::Update(DWORD dwFlag)
{
	if (!m_bCanUpdate)
		return FALSE;

	if (m_TexStaticLight.GetTexture())
	{
	
		m_pRenderer->GetD3DResourceManager()->SetSurfaceWithBitmap(			
			0,
			m_TexStaticLight.GetTexture(),
			m_pImageBuffer,
			m_dwSurfaceWidth,
			m_dwSurfaceHeight,
			D3DFMT_R5G6B5);
	
	//	RestoreLightTexture(m_pStaticLightTexture,m_pTexturePlane,m_dwTexturePlaneNum);
	}

	return TRUE;
}
BOOL CLMMeshObject::CreateStaticLightTexture(char* pBits,DWORD dwWidth,DWORD dwHeight,DWORD dwBPS)
{
	dwBPS = 2;
	BOOL bResult = FALSE;


	if (pBits)
	{
		bResult = m_pRenderer->GetTextureManager()->AllocRawTexture(&m_TexStaticLight,dwWidth,dwHeight,1,0,D3DFMT_R5G6B5,D3DPOOL_MANAGED);
		if (!bResult)
			goto lb_return;

		m_pRenderer->GetD3DResourceManager()->SetSurfaceWithBitmap(
			0,
			m_TexStaticLight.GetTexture(),
			pBits,
			dwWidth,
			dwHeight,
			D3DFMT_R5G6B5);
	}
	bResult = TRUE;

lb_return:
	return bResult;
}

CLMMeshObject::~CLMMeshObject()
{
	CBaseMeshObject::~CBaseMeshObject();

	DIMATERIAL*	pMtl;
	for (DWORD i=0; i<m_dwFaceGroupNum; i++)
	{
		m_pFaceGroup[i].ReleasePrjMesh(m_pRenderer->GetIBCache());

		pMtl = m_pFaceGroup[i].GetMaterial();
		if (pMtl)
		{
			m_pRenderer->FreeMaterial(pMtl);
		}
		
	}
//	if (m_pStaticLightTexture)
//	{
//		m_pRenderer->GetD3DResourceManager()->Release(m_pStaticLightTexture);
//		m_pStaticLightTexture = NULL;
//	}

	if (m_pFaceGroup)
	{
		delete [] m_pFaceGroup;
		m_pFaceGroup = NULL;
	}
}
