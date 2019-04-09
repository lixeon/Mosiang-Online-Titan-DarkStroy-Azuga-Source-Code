#include "CoD3DDevice.h"
#include "PLMeshObject.h"
#include "../4DyuchiGXGFunc/global.h"
#include "d3d_helper.h"
#include "VLFaceGroup.h"
#include "render_type.h"
#include "VBHeap.h"
#include <io.h>

SHADER_HANDLE	CPLMeshObject::m_vsHandle;
SHADER_HANDLE	CPLMeshObject::m_psHandle;
SHADER_HANDLE	CPLMeshObject::m_psDot3Handle;
SHADER_HANDLE	CPLMeshObject::m_psDot3ReflectHandle;
SHADER_HANDLE	CPLMeshObject::m_psOpasityHandle;


/*
    // Create vertex shader for the dolphin
    DWORD dwDolphinVertexDecl[] =
    {
        D3DVSD_STREAM( 0 ),
        D3DVSD_REG( 0, D3DVSDT_FLOAT3 ), // Position of first mesh
        D3DVSD_REG( 3, D3DVSDT_FLOAT3 ), // Normal
        D3DVSD_REG( 6, D3DVSDT_FLOAT2 ), // Tex coords
        D3DVSD_STREAM( 1 ),
        D3DVSD_REG( 1, D3DVSDT_FLOAT3 ), // Position of second mesh
        D3DVSD_REG( 4, D3DVSDT_FLOAT3 ), // Normal
        D3DVSD_REG( 7, D3DVSDT_FLOAT2 ), // Tex coords
        D3DVSD_STREAM( 2 ),
        D3DVSD_REG( 2, D3DVSDT_FLOAT3 ), // Position of second mesh
        D3DVSD_REG( 5, D3DVSDT_FLOAT3 ), // Normal
        D3DVSD_REG( 8, D3DVSDT_FLOAT2 ), // Tex coords
        D3DVSD_END()
    };
*/

BOOL CPLMeshObject::InitializeVertexShader()
{

	DWORD dwDecl[] = 
    {
        D3DVSD_STREAM(0),
        D3DVSD_REG(0,   D3DVSDT_FLOAT3), // position
        D3DVSD_REG(3,   D3DVSDT_FLOAT3), // normal
        D3DVSD_REG(7,   D3DVSDT_FLOAT2), // texture
        D3DVSD_REG(8,   D3DVSDT_FLOAT3), // tangent
        D3DVSD_END()
    };
	
	return InitializeShader(m_pRenderer->GetDevice(),&m_vsHandle,dwDecl,"diffDot3Pix.vso",SHADER_TYPE_VERTEX_SHADER);
}

BOOL CPLMeshObject::InitializePixelShader()
{	
	InitializeShader(m_pRenderer->GetDevice(),&m_psHandle,NULL,"diff.pso",SHADER_TYPE_PIXEL_SHADER);
	InitializeShader(m_pRenderer->GetDevice(),&m_psDot3Handle,NULL,"diffDot3.pso",SHADER_TYPE_PIXEL_SHADER);
	InitializeShader(m_pRenderer->GetDevice(),&m_psDot3ReflectHandle,NULL,"diffDot3Reflect.pso",SHADER_TYPE_PIXEL_SHADER);
	InitializeShader(m_pRenderer->GetDevice(),&m_psOpasityHandle,NULL,"diffOpasity.pso",SHADER_TYPE_PIXEL_SHADER);
	
	return TRUE;
}


CPLMeshObject::CPLMeshObject()
{
	memset((char*)this+4,0,sizeof(CPLMeshObject)-4);
	m_bCanUpdate = TRUE;
	m_fRenderPriorityFactor = RENDER_PRIORITY_FACTOR_VLOBJECT;
}

BOOL __stdcall CPLMeshObject::StartInitialize(MESH_DESC* pDesc,IGeometryController* pControl,IGeometryControllerStatic* pControlStatic)
{
	BOOL	bResult = FALSE;

	m_pMeshController = pControl;

	m_meshFlag = pDesc->meshFlag;
	m_dwVertexNum = pDesc->dwVertexNum;
	pDesc->pv3WorldList;

	m_dwMaxFaceGroupNum = pDesc->dwFaceGroupNum;
	m_pFaceGroup = new CVLFaceGroup[m_dwMaxFaceGroupNum];
	
	if (m_meshFlag.GetTransformType() != TRANSFORM_TYPE_NOT_SOLID)
	{
		
		DWORD	dwSize;
		DWORD	dwErrorCode;
		bResult = m_pRenderer->GetVBPLMeshHeap()->AllocVB(&dwErrorCode,&m_VB,m_dwVertexNum,&dwSize,&m_VB.m_bEnable,NULL);

		if (!bResult)
			goto lb_return;

		m_VB.SetVertexPosList(pDesc->pv3LocalList,m_dwVertexNum);
		

		if (pDesc->ptvTexCoordList)
			m_VB.SetTexCoord(pDesc->ptvTexCoordList,TEX1_COORD_OFFSET_VLMESH,m_dwVertexNum);

		if (pDesc->pv3NormalLocal)
			m_VB.SetNormal(pDesc->pv3NormalLocal,m_dwVertexNum);

		if (pDesc->pv3TangentULocal)
			m_VB.SetTangentU(pDesc->pv3TangentULocal,m_dwVertexNum);
	}
	
	InitializeVertexShader();
	InitializePixelShader();

	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL __stdcall CPLMeshObject::InsertFaceGroup(FACE_DESC* pDesc)
{

	BOOL		bResult = FALSE;
	
	DIMATERIAL*		pMtl;

	if (!m_pFaceGroup[m_dwFaceGroupNum].CreateFaceGroup(
		m_pRenderer,
		m_pRenderer->GetDevice(),
		NULL,
		pDesc->pIndex,
		NULL,//m_pVertexColor,
		pDesc->dwFacesNum,
		0))
		goto lb_return;
	

	pMtl = m_pRenderer->AllocMaterial(pDesc->pMaterial,NULL,NULL,0);
	m_pFaceGroup[m_dwFaceGroupNum].SetMaterial(pMtl,pDesc->dwMtlIndex);
	
//	m_pFaceGroup[m_dwFaceGroupNum].SetMaterial(pDesc->pMaterial,m_pRenderer);

	m_dwTotalFacesNum += m_pFaceGroup[m_dwFaceGroupNum].GetFacesNum();
	m_dwFaceGroupNum++;

lb_return:
	bResult = TRUE;
	
	return bResult;
}

BOOL __stdcall CPLMeshObject::Render(
		DWORD dwRefIndex,										// 애니메이션 컨트롤블럭
		DWORD dwAlpha,
		LIGHT_INDEX_DESC* pDynamicLightIndexList,DWORD dwLightNum,
		LIGHT_INDEX_DESC* pSpotLightIndexList,DWORD dwSpotLightNum,
		DWORD dwMtlSetIndex,
		DWORD dwEffectIndex,
		DWORD dwFlag					
		)
{

	DWORD i;
	BOOL		bLight = FALSE;

	BYTE*		pVertex;
	LPDIRECT3DDEVICE8	pDevice = m_pRenderer->GetDevice();

	DWORD				dwPhysiqueFlag = 0;


	VECTOR3*	pv3AnimatedLocal = NULL;
	TVERTEX*	ptvAnimated = NULL;

	CVertexBuffer*	pRenderVB = &m_VB;
	MATRIX4*	pResult = m_pRenderer->GetIdentityMatrix();
	MATRIX4		matWorldForPhysique;


	if (dwFlag & RENDER_TYPE_NOT_DRAW)
		goto lb_return;

				
	dwPhysiqueFlag = APPLY_PHYSIQUE_RECALC_NORMAL;

	if (m_meshFlag.GetTransformType() != TRANSFORM_TYPE_NOT_SOLID)
	{
		m_pMeshController->ApplyResultMatrix(&pResult,dwRefIndex,0);
		m_pMeshController->ApplyMorph(&pv3AnimatedLocal,&ptvAnimated,dwRefIndex,0);

		// 한방에 uv애니와 버텍스 애니를 처리하자..
		if (pv3AnimatedLocal || ptvAnimated)
			m_VB.SetVertexAndUVList(pv3AnimatedLocal,ptvAnimated,TEX1_COORD_OFFSET_VLMESH,m_dwVertexNum);
	}
	else
	{
		VBCACHE_KEY		key;
		MOTION_STATUS	motStatus;
		m_pMeshController->ApplyMotionStatus(&motStatus,dwRefIndex,0);
		key.dwFrame = (WORD)motStatus.dwFrame;
		key.pMotionUID = motStatus.pMotionUID;
		key.pObject = (void*)this;
//		key.dwObjCount = m_dwObjCount;

		
		BOOL	bNeedUpdate;
		CVertexBuffer* pVB;
		
		pVB = m_pRenderer->GetVBCache()->GetVB(&bNeedUpdate,&key,dwRefIndex,m_dwVertexNum);

		pRenderVB = pVB;
		if (bNeedUpdate)
		{
			dwPhysiqueFlag |= APPLY_PHYSIQUE_WRITE_UV;
			pRenderVB->LockVertexPtr(&pVertex,0  );
			m_pMeshController->ApplyPhysique(pVertex,sizeof(D3DPLVERTEX),dwRefIndex,dwPhysiqueFlag);
			pRenderVB->UnlockVertexPtr();
			pResult = &matWorldForPhysique;
		}
		m_pMeshController->ApplyWorldMatrixForPhysique(&matWorldForPhysique,dwRefIndex,dwPhysiqueFlag);
		pResult = &matWorldForPhysique;
	}
		


lb_render:
	

	if (dwFlag & RENDER_TYPE_DISABLE_ZCLIP)
		pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);

	if (m_meshFlag.IsDisableZBubfferWrite())
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

	pDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)pResult);		

	for (i=0; i<2; i++)
	{
		pDevice->SetTextureStageState( i, D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
		pDevice->SetTextureStageState( i,D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);
	}
		
	/////////// vertex shader //////////////////////////////////////////////////////////////////////////////////////////
	// light direction
	VECTOR4	v4DirLight;
	v4DirLight.x = 1.0f;
	v4DirLight.y = -1.0f;
	v4DirLight.z = 1.0f;

	v4DirLight.w = 0.0f;
	Normalize((VECTOR3*)&v4DirLight,(VECTOR3*)&v4DirLight);
	pDevice->SetVertexShaderConstant(12,&v4DirLight,  1 );	// light direction

	
	MATRIX4	matTemp;
	matTemp = *pResult;
	TransposeMatrix(&matTemp);
	pDevice->SetVertexShaderConstant(0, &matTemp, 4);	


	MatrixMultiply2(&matTemp,pResult,m_pRenderer->INL_GetViewMatrix());
	MatrixMultiply2(&matTemp,&matTemp,m_pRenderer->INL_GetProjMatrix());
	TransposeMatrix(&matTemp);
	pDevice->SetVertexShaderConstant(8, &matTemp, 4);	



	// constant values
	float fHalf[4];
	fHalf[0] = 0.5f;
	fHalf[1] = 0.5f;
	fHalf[2] = 0.5f;
	fHalf[3] = 0.5f;

	pDevice->SetVertexShaderConstant(33, &fHalf, 1);

//	dwAlpha = 125;

	DWORD	dwAlphaRef;
	dwAlphaRef = m_pRenderer->INL_GetAlphaRefVaule();

	float fAlpha[8];
	fAlpha[0] = 1.0f;
	fAlpha[1] = 1.0f;
	fAlpha[2] = 1.0f;
	fAlpha[3] = (float)dwAlpha / 255.0f;
	fAlpha[4] = 1.0f;
	fAlpha[5] = 1.0f;
	fAlpha[6] = 1.0f;
	fAlpha[7] = (float)dwAlphaRef / 255.0f;
	
	

	pDevice->SetPixelShaderConstant(0, &fAlpha, 2);


	CAMERA_DESC* pCameraDesc;
	pCameraDesc = m_pRenderer->INL_GetCameraDesc();
	pDevice->SetVertexShaderConstant(24, &pCameraDesc->v3EyeDir, 1);		

	pDevice->SetStreamSource( 0,pRenderVB->GetVertexBuffer(),pRenderVB->GetVertexSize());
	pDevice->SetVertexShader(m_vsHandle.dwShader);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (dwFlag & RENDER_TYPE_USE_EFFECT)
	{
		VLMESH_EFFECT_DESC*		pEffect;
		pEffect = m_pRenderer->INL_GetVLMeshEffect(dwEffectIndex);
		if (pEffect)
		{
			if (pEffect->bSuccess)
			{
				RenderEffect(pRenderVB,m_pFaceGroup,m_dwFaceGroupNum,dwAlpha,pEffect,pResult,dwMtlSetIndex,dwFlag);
				goto lb_return;
			}
		}
	}

	MATRIX4	matTex;
	m_pRenderer->SetSphereMapMatrix(&matTex,pResult,m_pRenderer->INL_GetViewMatrix());
	
	pDevice->SetVertexShaderConstant(25, &matTex, 4);	
		
	D3DMATERIAL8	d3dmtl;
	DIMATERIAL*	pMtl;
	for (i=0; i<m_dwFaceGroupNum; i++)
	{
		pDevice->SetPixelShader(m_psDot3Handle.dwShader);
//		pDevice->SetPixelShader(m_psHandle.dwShader);
		if (dwMtlSetIndex)
		{
			pMtl = m_pRenderer->GetMaterial(dwMtlSetIndex, m_pFaceGroup[i].GetMtlIndex());
			if (!pMtl)
				goto lb_default_mtl;
		}
		else
		{
lb_default_mtl:
			pMtl = m_pFaceGroup[i].GetMaterial();
		}
		d3dmtl = pMtl->d3dMtl;
		
	
		if (!(pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_SELF_ILLUMIN))
		{

			if (dwFlag & RENDER_TYPE_SELF_ILLUMIN)
			{
				d3dmtl.Emissive = d3dmtl.Diffuse;
			}
			else
			{
				m_pRenderer->INL_GetEmissiveColor(&d3dmtl.Emissive);
			}
			
		}

		// 2side 처리
		if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

				
		// 페이스그룹의 해당 매터리얼 상태에 맞게 알파블랜딩 스타트
		
		BeginAlphaMeshObject(pDevice,1,pMtl->dwAlpha,dwAlpha);
		if (pMtl->dwAlpha & TEX_FLAG_ALPHA_OPASITY)
		{
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);			
//			pDevice->SetPixelShader(m_psOpasityHandle.dwShader);
		}
		if (m_pRenderer->INL_IsEnableFog())
		{
			if (pMtl->dwAlpha & TEX_FLAG_ALPHA_TRANSP_ADD)
				pDevice->SetRenderState(D3DRS_FOGENABLE,FALSE);
			else
				pDevice->SetRenderState(D3DRS_FOGENABLE,TRUE);
		}

//		pDevice->SetMaterial(&d3dmtl);
//		set material color
//		float fMaterial[4] = {1,1,1,1};
//		pDevice->SetVertexShaderConstant(4, fMaterial, 1);


		
		if (pMtl->TextureDiffuse.HasTexture())
		{
//			m_pRenderer->GetDefaultMaterial()->TextureDiffuse.SetTexture(pDevice,0);
			pMtl->TextureDiffuse.SetTexture(pDevice,0);
		} 
		else
		{
			m_pRenderer->GetDefaultMaterial()->TextureDiffuse.SetTexture(pDevice,0);
		}


		if (pMtl->TextureBump.HasTexture())
		{			
			pMtl->TextureBump.SetTexture(pDevice,1);
			
		}
		else
			m_pRenderer->GetDefaultMaterial()->TextureBump.SetTexture(pDevice,1);
		

		if (pMtl->TextureReflect.HasTexture())
		{
			pMtl->TextureReflect.SetTexture(pDevice,2);
			pDevice->SetPixelShader(m_psDot3ReflectHandle.dwShader);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		

		pDevice->SetIndices(m_pFaceGroup[i].GetIndexBuffer(),pRenderVB->GetStartIndex());
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBStartIndex(),m_pFaceGroup[i].GetFacesNum());

		EndAlphaMeshObject(pDevice);
		
		if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	}
	
lb_return:	
	pDevice->SetPixelShader(NULL);
	if (m_meshFlag.IsDisableZBubfferWrite())
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS ,FALSE);

	if (dwFlag & RENDER_TYPE_DISABLE_ZCLIP)
		pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);


	return TRUE;
}



BOOL __stdcall CPLMeshObject::RenderEffect(
		CVertexBuffer*		pRenderVB,
		CVLFaceGroup*		pFaceGroup,DWORD dwFaceGroupNum,
		DWORD				dwAlpha,
		VLMESH_EFFECT_DESC*	pEffect,
		MATRIX4*			pMatWorld,
		DWORD				dwMtlSetIndex,
		DWORD				dwFlag					
		)
{


	BOOL	bResult = FALSE;
	
	LPDIRECT3DDEVICE8	pDevice = m_pRenderer->GetDevice();

	MATRIX4		matTex,matWave;
	DWORD		dwTexGen;

	
	dwTexGen = 0;

	m_pRenderer->SetSphereMapMatrix(&matTex,pMatWorld,m_pRenderer->INL_GetViewMatrix());
	if (pEffect->method == TEXGEN_METHOD_REFLECT_SPHEREMAP)
	{
	
	} 
	else if (pEffect->method == TEXGEN_METHOD_WAVE)
	{
		m_pRenderer->SetWaveTexMatrix(&matWave);
		MatrixMultiply2(&matTex,&matTex,&matWave);
	}
	pDevice->SetVertexShaderConstant(25, &matTex, 4);	

	pEffect->m_texture.SetTexture(pDevice,2);
	pDevice->SetPixelShader(m_psDot3ReflectHandle.dwShader);
	
	
//	pDevice->SetStreamSource( 0,pRenderVB->GetVertexBuffer(),pRenderVB->GetVertexSize());
//	pDevice->SetVertexShader(pRenderVB->GetVertexShader());

	DWORD	i;
	DIMATERIAL*	pMtl;
	D3DMATERIAL8	d3dmtl;


	for (i=0; i<dwFaceGroupNum; i++)
	{
		if (dwMtlSetIndex)
		{
			pMtl = m_pRenderer->GetMaterial(dwMtlSetIndex, m_pFaceGroup[i].GetMtlIndex());
			if (!pMtl)
				goto lb_default_mtl;
		}
		else
		{
lb_default_mtl:
			pMtl = m_pFaceGroup[i].GetMaterial();
		}
		d3dmtl = pMtl->d3dMtl;
		pDevice->SetMaterial(&d3dmtl);

		if (pMtl->TextureDiffuse.HasTexture())
		{
			pMtl->TextureDiffuse.SetTexture(pDevice,0);
		} 
		else
		{
			m_pRenderer->GetDefaultMaterial()->TextureDiffuse.SetTexture(pDevice,0);
		}


		if (pMtl->TextureBump.HasTexture())
		{			
			pMtl->TextureBump.SetTexture(pDevice,1);
			
		}
		else
			m_pRenderer->GetDefaultMaterial()->TextureBump.SetTexture(pDevice,1);
		


		// 2side 처리
		if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		
		// 페이스그룹의 해당 매터리얼 상태에 맞게 알파블랜딩 스타트
		BeginAlphaMeshObject(pDevice,1,pMtl->dwAlpha,dwAlpha);
		if (pMtl->dwAlpha & TEX_FLAG_ALPHA_OPASITY)
		{
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);			
//			pDevice->SetPixelShader(m_psOpasityHandle.dwShader);
		}


		pDevice->SetIndices(m_pFaceGroup[i].GetIndexBuffer(),pRenderVB->GetStartIndex());
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBStartIndex(),m_pFaceGroup[i].GetFacesNum());

		if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);

		EndAlphaMeshObject(pDevice);

		
	}
	

	bResult = TRUE;

lb_return:
	return bResult;
}


BOOL __stdcall CPLMeshObject::RenderProjection(
		DWORD		dwRefIndex,		// 애니메이션 컨트롤블럭
		DWORD		dwAlpha,
		BYTE*		pSpotLightIndex,
		DWORD		dwViewNum,
		DWORD		dwFlag
		)
{
	DWORD i;

	BYTE*		pVertex;
	LPDIRECT3DDEVICE8	pDevice = m_pRenderer->GetDevice();
	VECTOR3*	pv3AnimatedLocal = NULL;
	TVERTEX*	ptvAnimated = NULL;
	CVertexBuffer*	pRenderVB = &m_VB;
	MATRIX4*	pResult = m_pRenderer->GetIdentityMatrix();
	MATRIX4		matWorldForPhysique;

	if (m_meshFlag.GetTransformType() != TRANSFORM_TYPE_NOT_SOLID)
	{
		m_pMeshController->ApplyResultMatrix(&pResult,dwRefIndex,0);
		m_pMeshController->ApplyMorph(&pv3AnimatedLocal,&ptvAnimated,dwRefIndex,0);

		// 한방에 uv애니와 버텍스 애니를 처리하자..
		if (pv3AnimatedLocal || ptvAnimated)
			m_VB.SetVertexAndUVList(pv3AnimatedLocal,ptvAnimated,TEX1_COORD_OFFSET_VLMESH,m_dwVertexNum);
	}
	else
	{
		VBCACHE_KEY		key;
		MOTION_STATUS	motStatus;
		m_pMeshController->ApplyMotionStatus(&motStatus,dwRefIndex,0);
		key.dwFrame = (WORD)motStatus.dwFrame;
		key.pMotionUID = motStatus.pMotionUID;
		key.pObject = (void*)this;
//		key.dwObjCount = m_dwObjCount;
	

		BOOL	bNeedUpdate;
		CVertexBuffer* pVB;
		pVB = m_pRenderer->GetVBCache()->GetVB(&bNeedUpdate,&key,dwRefIndex,m_dwVertexNum);

		pRenderVB = pVB;
		if (bNeedUpdate)
		{
			pRenderVB->LockVertexPtr(&pVertex,0  );
			m_pMeshController->ApplyPhysique(pVertex,sizeof(D3DPLVERTEX),dwRefIndex,APPLY_PHYSIQUE_RECALC_NORMAL | APPLY_PHYSIQUE_WRITE_UV);
			pRenderVB->UnlockVertexPtr();
			
		}
		m_pMeshController->ApplyWorldMatrixForPhysique(&matWorldForPhysique,dwRefIndex,APPLY_PHYSIQUE_RECALC_NORMAL | APPLY_PHYSIQUE_WRITE_UV);
		pResult = &matWorldForPhysique;
	}

lb_render:
	pDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)pResult);
	pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,0);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);
	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);

	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);

	

	pDevice->SetVertexShader(pRenderVB->GetVertexShader());
//	pDevice->SetVertexShader(D3DFVF_XYZ);
	
	pDevice->SetStreamSource( 0,pRenderVB->GetVertexBuffer(),pRenderVB->GetVertexSize());
	
	dwAlpha = dwAlpha >> 1;
	DWORD	sr = 255 - dwAlpha;
	DWORD	sg = 255 - dwAlpha;
	DWORD	sb = 255 - dwAlpha;
	DWORD	dwShadowColor =  0xff000000 | (sb<<16) | (sg<<8) | sr;
//	DWORD	dwShadowColor =  0xffCCCCCC;


	SPOT_LIGHT_DESC*	pSpotDesc;
	
	for (i=0; i<dwViewNum; i++)
	{
		pSpotDesc = m_pRenderer->INL_GetSpotDesc(pSpotLightIndex[i]);
		
		if (!pSpotDesc->pRenderTexture)
			continue;

		if (!pSpotDesc->pRenderTexture->BeginRender(0))
			continue;


	//	if (!m_pRenderer->BeginCreateProjectionMap(pSpotLightIndex[i],0))
	//		continue;

	//	m_pRenderer->BeginCreateShadowMap(pSpotLightIndex[i],RENDER_TEXTURE_CLEAR);

		
		
		
		pDevice->SetTransform(D3DTS_VIEW,(D3DMATRIX*)&pSpotDesc->prjDesc.matPrjView);
		pDevice->SetTransform(D3DTS_PROJECTION,(D3DMATRIX*)&pSpotDesc->prjDesc.matPrjProj);
		
//		pDevice->SetTransform(D3DTS_VIEW,(D3DMATRIX*)m_pRenderer->INL_GetShadowViewMatrix(pSpotLightIndex[i]));
//		pDevice->SetTransform(D3DTS_PROJECTION,(D3DMATRIX*)m_pRenderer->INL_GetShadowProjMatrix(pSpotLightIndex[i]));
		Render(pRenderVB,dwShadowColor);
		
	//	m_pRenderer->EndCreateProjectionMap(pSpotLightIndex[i]);

		pSpotDesc->pRenderTexture->EndRender();
	//	pSpotDesc->pRenderTexture->SetNotMustUpdate();
		
	}
	pDevice->SetTransform(D3DTS_VIEW,(D3DMATRIX*)m_pRenderer->INL_GetViewMatrix());
	pDevice->SetTransform(D3DTS_PROJECTION,(D3DMATRIX*)m_pRenderer->INL_GetProjMatrix());
	pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
lb_return:	
	return TRUE;
}

BOOL CPLMeshObject::Render(CVertexBuffer* pVB,DWORD dwColor)
{
	DWORD	i;
		
	LPDIRECT3DDEVICE8	pDevice = m_pRenderer->GetDevice();
	SetColorFactorShader(pDevice,dwColor,0);

//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
	
	for (i=0; i<m_dwFaceGroupNum; i++)
	{
		pDevice->SetIndices(m_pFaceGroup[i].GetIndexBuffer(),pVB->GetStartIndex());
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBStartIndex(),m_pFaceGroup[i].GetFacesNum());
	}
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

	RestoreTextureShader(pDevice,0);

	return TRUE;
}

BOOL __stdcall CPLMeshObject::Update(DWORD dwFlag)
{
	if (!m_bCanUpdate)
		return FALSE;

	return TRUE;
}





void __stdcall CPLMeshObject::EndInitialize()
{

	// 알파 속성을 가지고 페이스그룹을 소트시킨다.
	SortWithAlpha();

	
}
void CPLMeshObject::SortWithAlpha()
{
	CVLFaceGroup*	pFaceGroupTemp = (CVLFaceGroup*)malloc(sizeof(CVLFaceGroup)*m_dwFaceGroupNum);
	DWORD			dwAlphaIndex = m_dwFaceGroupNum-1;
	DWORD			dwNotAlphaIndex = 0;

	DIMATERIAL*		pMtl;
	for (DWORD i=0; i<m_dwFaceGroupNum; i++)
	{
		pMtl = m_pFaceGroup[i].GetMaterial();
		if (pMtl->dwAlpha)
		{
			memcpy(&pFaceGroupTemp[dwAlphaIndex],&m_pFaceGroup[i],sizeof(CVLFaceGroup));
			dwAlphaIndex--;
		}
		else
		{

			memcpy(&pFaceGroupTemp[dwNotAlphaIndex],&m_pFaceGroup[i],sizeof(CVLFaceGroup));
			dwNotAlphaIndex++;
		}
	}
	memcpy(m_pFaceGroup,pFaceGroupTemp,sizeof(CVLFaceGroup)*m_dwFaceGroupNum);
	free(pFaceGroupTemp);
}


CPLMeshObject::~CPLMeshObject()
{
	m_pRenderer->GetVBCache()->ClearCacheWithIDIMeshObject(this);
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
	if (m_pFaceGroup)
	{
		delete [] m_pFaceGroup;
		m_pFaceGroup = NULL;
	}

	CBaseMeshObject::~CBaseMeshObject();


	ReleaseShader(m_pRenderer->GetDevice(),&m_vsHandle);
	ReleaseShader(m_pRenderer->GetDevice(),&m_psHandle);
	ReleaseShader(m_pRenderer->GetDevice(),&m_psDot3Handle);
	ReleaseShader(m_pRenderer->GetDevice(),&m_psDot3ReflectHandle);
	ReleaseShader(m_pRenderer->GetDevice(),&m_psOpasityHandle);

}
