#include "CoD3DDevice.h"
#include "VLMeshObject.h"
#include "../4DyuchiGXGFunc/global.h"
#include "d3d_helper.h"
#include "VLFaceGroup.h"
#include "render_type.h"
#include "VBHeap.h"


CVLMeshObject::CVLMeshObject()
{
	memset((char*)this+4,0,sizeof(CVLMeshObject)-4);
	m_bCanUpdate = TRUE;
	m_fRenderPriorityFactor = RENDER_PRIORITY_FACTOR_VLOBJECT;
}

BOOL __stdcall CVLMeshObject::StartInitialize(MESH_DESC* pDesc,IGeometryController* pControl,IGeometryControllerStatic* pControlStatic)
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
		bResult = m_pRenderer->GetVBVLMeshHeap()->AllocVB(&dwErrorCode,&m_VB,m_dwVertexNum,&dwSize,&m_VB.m_bEnable,NULL);

		if (!bResult)
			goto lb_return;

		m_VB.SetVertexPosList(pDesc->pv3LocalList,m_dwVertexNum);
		

		if (pDesc->ptvTexCoordList)
			m_VB.SetTexCoord(pDesc->ptvTexCoordList,TEX1_COORD_OFFSET_VLMESH,m_dwVertexNum);

		if (pDesc->pv3NormalLocal)
			m_VB.SetNormal(pDesc->pv3NormalLocal,m_dwVertexNum);
	}
	

	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL __stdcall CVLMeshObject::InsertFaceGroup(FACE_DESC* pDesc)
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


BOOL __stdcall CVLMeshObject::Render(
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
	LIGHT_DESC*	pLight;
	BYTE*		pVertex;
	CRenderTexture*		pRenderTexture = NULL;
	LPDIRECT3DDEVICE8	pDevice = m_pRenderer->GetDevice();

	DWORD				dwPhysiqueFlag = 0;

	D3DLIGHT8			d3dLight;
	VECTOR3*	pv3AnimatedLocal = NULL;
	TVERTEX*	ptvAnimated = NULL;

	CVertexBuffer*	pRenderVB = &m_VB;
	MATRIX4*	pResult = m_pRenderer->GetIdentityMatrix();
	MATRIX4		matWorldForPhysique;


	if (dwFlag & RENDER_TYPE_NOT_DRAW)
		goto lb_return;

	if (dwFlag & RENDER_TYPE_UPDATE_SHADING)
	{
	
		if (dwLightNum)
		{
			// 포인트라이트가 있는 경우
			if (dwLightNum > 7)
				dwLightNum = 7;

	   		memset(&d3dLight,0,sizeof(d3dLight));
			d3dLight.Type = D3DLIGHT_POINT;
			d3dLight.Attenuation0 = m_pRenderer->INL_GetAttentuation0();
			bLight = TRUE;
			dwPhysiqueFlag = APPLY_PHYSIQUE_RECALC_NORMAL;
		}
		
	}
	else 
	{
		dwLightNum = 0;
	}


	if (m_pRenderer->INL_IsEnableDrectionalLight())
	{
		// 방향성 라이트가 설정된 경우..
		bLight = TRUE;
		dwPhysiqueFlag = APPLY_PHYSIQUE_RECALC_NORMAL;
	}

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
			m_pMeshController->ApplyPhysique(pVertex,sizeof(D3DVLVERTEX),dwRefIndex,dwPhysiqueFlag);
			pRenderVB->UnlockVertexPtr();
			pResult = &matWorldForPhysique;
		}
		m_pMeshController->ApplyWorldMatrixForPhysique(&matWorldForPhysique,dwRefIndex,dwPhysiqueFlag);
		pResult = &matWorldForPhysique;
	}
		


lb_render:

	pDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)pResult);		

	pDevice->SetTextureStageState( 1, D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
	pDevice->SetTextureStageState( 1, D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);

	pDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0);
	pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE );
	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE);	
			

	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);

	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);


	MATRIX4	matTex;
	m_pRenderer->SetSphereMapMatrix(&matTex,pResult,m_pRenderer->INL_GetViewMatrix());
	pDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + 0),(D3DMATRIX*)&matTex);



	pDevice->SetStreamSource( 0,pRenderVB->GetVertexBuffer(),pRenderVB->GetVertexSize());

	if (dwFlag & RENDER_TYPE_DISABLE_ZCLIP)
		pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);

//	if (m_pRenderer->INL_IsEnableFog())
//		pDevice->SetRenderState(D3DRS_FOGENABLE,TRUE);

	if (m_meshFlag.IsDisableZBubfferWrite())
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

//	dwFlag |= RENDER_TYPE_USE_EFFECT;
//	dwEffectIndex = 0;


	if (bLight)
	{
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS ,TRUE);
		for (i=0; i<dwLightNum; i++)
		{
			pLight = m_pRenderer->INL_GetRTLightDesc(pDynamicLightIndexList[i].bLightIndex);
			DWORDtoCOLOR(pLight->dwDiffuse,
				&d3dLight.Diffuse.r,
				&d3dLight.Diffuse.g,
				&d3dLight.Diffuse.b,
				&d3dLight.Diffuse.a);

			*(VECTOR3*)&d3dLight.Position.x = *(VECTOR3*)&pLight->v3Point.x;
		
			d3dLight.Range = pLight->fRs;
			pDevice->SetLight(i,&d3dLight);
			pDevice->LightEnable(i,TRUE);
		}
	}
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

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


	D3DMATERIAL8	d3dmtl;
	DIMATERIAL*	pMtl;
	for (i=0; i<m_dwFaceGroupNum; i++)
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
		if (  (DWORD)d3dmtl.Power & m_pRenderer->INL_GetSpecularMask() )
			pDevice->SetRenderState(D3DRS_SPECULARENABLE,TRUE);
		else
			pDevice->SetRenderState(D3DRS_SPECULARENABLE,FALSE);


		
		if (m_pRenderer->INL_IsEnableFog())
		{
			if (pMtl->dwAlpha & TEX_FLAG_ALPHA_TRANSP_ADD)
				pDevice->SetRenderState(D3DRS_FOGENABLE,FALSE);
			else
				pDevice->SetRenderState(D3DRS_FOGENABLE,TRUE);
		}




		pDevice->SetMaterial(&d3dmtl);

		// 2side 처리
		if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		
		// 페이스그룹의 해당 매터리얼 상태에 맞게 알파블랜딩 스타트
		BeginAlphaMeshObject(pDevice,2,pMtl->dwAlpha,dwAlpha);
		pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);	
		pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE);	
	//	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		
		if (m_pRenderer->GetLightMapFlag() & LIGHTMAP_FLAG_DISABLE_TEXTURE )
		{
			pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			goto lb_skip_tex_render;
		}
		if (pMtl->TextureDiffuse.HasTexture())
		{
			
			pMtl->TextureDiffuse.SetTexture(pDevice,1);
			//	m_pRenderer->GetDefaultMaterial()->TextureDiffuse.SetTexture(pDevice,1);
		}
		else
		{
			pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		//	m_pRenderer->GetDefaultMaterial()->TextureDiffuse.SetTexture(pDevice,1);
			
		}
		if (pMtl->TextureReflect.HasTexture())
		{
			pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2 );
			pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
			pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);	
			pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE4X);	
			pMtl->TextureReflect.SetTexture(pDevice,0);
		}
		else
		{
		//	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0);
		//	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE );
			m_pRenderer->GetDefaultMaterial()->TextureDiffuse.SetTexture(pDevice,0);

		}/*
		if (pMtl->TextureBump.HasTexture())
		{

			VECTOR3		v3Light;
			v3Light.x = -1.0f;
			v3Light.y = -1.0f;
			v3Light.z = -1.0f;


			DWORD dwFactor = VectortoRGBA( &v3Light, 0.0f );
			dwFactor = 0xFFFFFFFF;
			pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwFactor );

			pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
			pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);
			pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0);
			pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE );
//			pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);	
//			pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);	
			pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_DOTPRODUCT3 );
			pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED2X);	
			
//			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );

			pMtl->TextureBump.SetTexture(pDevice,0);
//			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	
		//	pDevice->SetTextureStageState( 2, D3DTSS_COLORARG1, D3DTA_CURRENT );
		//	pDevice->SetTextureStageState( 2, D3DTSS_COLORARG2, D3DTA_TEXTURE );
				
        }
		*/
	
lb_skip_tex_render:

		pDevice->SetVertexShader(pRenderVB->GetVertexShader());
		pDevice->SetIndices(m_pFaceGroup[i].GetIndexBuffer(),pRenderVB->GetStartIndex());
		
		//trustpak 2005/07/15
		//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBStartIndex(),m_pFaceGroup[i].GetFacesNum());

		if (TRUE == m_pRenderer->GetRenderWireSolidBothMode())
		{
			DWORD dwLegacyRenderMode = m_pRenderer->GetRenderMode();

			m_pRenderer->SetRenderMode(RENDER_MODE_SOLID);

			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBStartIndex(),m_pFaceGroup[i].GetFacesNum());

			m_pRenderer->SetRenderMode(RENDER_MODE_WIREFRAME);
			SetColorFactorShader(pDevice, 0xff808080,0);					

			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBStartIndex(),m_pFaceGroup[i].GetFacesNum());

			RestoreTextureShader(pDevice,0);
			m_pRenderer->SetRenderMode(dwLegacyRenderMode);

		}
		else
		{
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBStartIndex(),m_pFaceGroup[i].GetFacesNum());
		}

		///---trustpak end

		// 알파블랜딩 end.
		EndAlphaMeshObject(pDevice);
				
		if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	}
lb_return:	



	if (m_meshFlag.IsDisableZBubfferWrite())
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS ,FALSE);

	if (dwFlag & RENDER_TYPE_DISABLE_ZCLIP)
		pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	for (i=0; i<dwLightNum; i++)
		pDevice->LightEnable(i,FALSE);

	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	
	if (m_pRenderer->INL_IsEnableFog())
		pDevice->SetRenderState(D3DRS_FOGENABLE,FALSE);

	return TRUE;
}


BOOL __stdcall CVLMeshObject::RenderEffect(
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
	pDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0);
	pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE );

	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);
	pDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0),(D3DMATRIX*)&matTex);



	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);	
	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE4X);	

	pEffect->m_texture.SetTexture(pDevice,0);
	
	pDevice->SetStreamSource( 0,pRenderVB->GetVertexBuffer(),pRenderVB->GetVertexSize());
	pDevice->SetVertexShader(pRenderVB->GetVertexShader());

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
			pMtl->TextureDiffuse.SetTexture(pDevice,1);
		}
		else
		{
			pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		//	m_pRenderer->GetDefaultMaterial()->TextureDiffuse.SetTexture(pDevice,1);

		}

		// 2side 처리
		if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		
		// 페이스그룹의 해당 매터리얼 상태에 맞게 알파블랜딩 스타트
		BeginAlphaMeshObject(pDevice,2,pMtl->dwAlpha,dwAlpha);

		pDevice->SetIndices(m_pFaceGroup[i].GetIndexBuffer(),pRenderVB->GetStartIndex());
		
		//trustpak 2005/07/15
		//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBStartIndex(),m_pFaceGroup[i].GetFacesNum());

		if (TRUE == m_pRenderer->GetRenderWireSolidBothMode())
		{
			DWORD dwLegacyRenderMode = m_pRenderer->GetRenderMode();

			m_pRenderer->SetRenderMode(RENDER_MODE_SOLID);

			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBStartIndex(),m_pFaceGroup[i].GetFacesNum());

			m_pRenderer->SetRenderMode(RENDER_MODE_WIREFRAME);
			SetColorFactorShader(pDevice, 0xff808080,0);	
			
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBStartIndex(),m_pFaceGroup[i].GetFacesNum());

			RestoreTextureShader(pDevice,0);
			m_pRenderer->SetRenderMode(dwLegacyRenderMode);

		}
		else
		{
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBStartIndex(),m_pFaceGroup[i].GetFacesNum());
		}

		///---trustpak end

		if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);

		EndAlphaMeshObject(pDevice);

		
	}
	

	bResult = TRUE;

lb_return:
	return bResult;
}


BOOL __stdcall CVLMeshObject::RenderProjection(
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
			m_pMeshController->ApplyPhysique(pVertex,sizeof(D3DVLVERTEX),dwRefIndex,APPLY_PHYSIQUE_RECALC_NORMAL | APPLY_PHYSIQUE_WRITE_UV);
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

BOOL CVLMeshObject::Render(CVertexBuffer* pVB,DWORD dwColor)
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

BOOL __stdcall CVLMeshObject::Update(DWORD dwFlag)
{
	if (!m_bCanUpdate)
		return FALSE;

	return TRUE;
}





void __stdcall CVLMeshObject::EndInitialize()
{

	// 알파 속성을 가지고 페이스그룹을 소트시킨다.
	SortWithAlpha();

	
}
void CVLMeshObject::SortWithAlpha()
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


CVLMeshObject::~CVLMeshObject()
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
/*	if (m_pVertexColor)
	{
		delete [] m_pVertexColor;
		m_pVertexColor = NULL;
	}*/

//	ReleaseVertexShader(m_pRenderer->GetDevice(),&m_vsHandle);
}
		/*
	if (dwFlag & RENDER_TYPE_RECV_SHADOW)
	{
		pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
		pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);

		for (m=0; m<dwSpotLightNum; m++)
		{
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );

			pRenderTexture = m_pRenderer->INL_GetShadowTexture(pSpotLightIndex[m]);

			// 자기 자신의 그림자를 흰색으로 칠한다...
			pRenderTexture->BeginRender(0);
				
			pDevice->SetTransform(D3DTS_VIEW,(D3DMATRIX*)m_pRenderer->INL_GetShadowViewMatrix(pSpotLightIndex[m]));
			pDevice->SetTransform(D3DTS_PROJECTION,(D3DMATRIX*)m_pRenderer->INL_GetShadowProjMatrix(pSpotLightIndex[m]));
			pDevice->SetTexture( 0,NULL);

			pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
			Render(0xffffffff);
			pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

			pRenderTexture->EndRender();

			pDevice->SetTransform(D3DTS_VIEW,(D3DMATRIX*)m_pRenderer->INL_GetViewMatrix());
			pDevice->SetTransform(D3DTS_PROJECTION,(D3DMATRIX*)m_pRenderer->INL_GetProjMatrix());
			/////////////////////////////////////////////////////////////////////////////////////////////



			// 자기 자신을 제외한 그림자를 렌더링...
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
		
			pDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + 0),(D3DMATRIX*)m_pRenderer->INL_GetShadowMapMatrix(pSpotLightIndex[m]));
			SetProjTextureMODDiffuseShader(pDevice,0,0xffffffff,0);
			pDevice->SetTexture( 0,pRenderTexture->GetTexture());

			
			for (i=0; i<m_dwFaceGroupNum; i++)
			{
			
				if (m_pFaceGroup[i].Get2SideFlag())
					pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

				pDevice->SetIndices(m_pFaceGroup[i].GetIndexBuffer(),0);
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,0,m_pFaceGroup[i].GetFacesNum());

				if (m_pFaceGroup[i].Get2SideFlag())
					pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////

			// 자기 자신의 그림자를 그림자색으로 칠한다...
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			pRenderTexture->BeginRender(0);
	
			pDevice->SetTransform(D3DTS_VIEW,(D3DMATRIX*)m_pRenderer->INL_GetShadowViewMatrix(pSpotLightIndex[m]));
			pDevice->SetTransform(D3DTS_PROJECTION,(D3DMATRIX*)m_pRenderer->INL_GetShadowProjMatrix(pSpotLightIndex[m]));
			DWORD	sr = 255 - dwAlpha;
			DWORD	sg = 255 - dwAlpha;
			DWORD	sb = 255 - dwAlpha;
			dwShadowColor =  0xff000000 | (sb<<16) | (sg<<8) | sr;

			pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
			Render(dwShadowColor);
			pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
			pRenderTexture->EndRender();
			
			pDevice->SetTransform(D3DTS_VIEW,(D3DMATRIX*)m_pRenderer->INL_GetViewMatrix());
			pDevice->SetTransform(D3DTS_PROJECTION,(D3DMATRIX*)m_pRenderer->INL_GetProjMatrix());
			/////////////////////////////////////////////////////////////////////////////////////////////
			
		}
	}*/

	/*

	//-----------------------------------------------------------------------------
// Name: ApplySphereMapToObject()
// Desc: Uses the current orientation of the vertices to calculate the object's
//       spheremapped texture coords.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::ApplySphereMapToObject( D3DVERTEX* pvVertices,
                                                   DWORD dwNumVertices )
{
    // Get the current world-view matrix
    D3DMATRIX matWorld, matView, matWV;
    m_pd3dDevice->GetTransform( D3DTRANSFORMSTATE_VIEW,  &matView );
    m_pd3dDevice->GetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
    D3DMath_MatrixMultiply( matWV, matWorld, matView );

    // Extract world-view matrix elements for speed
    FLOAT m11 = matWV._11,   m21 = matWV._21,   m31 = matWV._31;
    FLOAT m12 = matWV._12,   m22 = matWV._22,   m32 = matWV._32;
    FLOAT m13 = matWV._13,   m23 = matWV._23,   m33 = matWV._33;

    // Loop through the vertices, transforming each one and calculating
    // the correct texture coordinates.
    for( WORD i = 0; i < dwNumVertices; i++ )
    {
        FLOAT nx = pvVertices[i].nx;
        FLOAT ny = pvVertices[i].ny;
        FLOAT nz = pvVertices[i].nz;

        // Check the z-component, to skip any vertices that face backwards
        if( nx*m13 + ny*m23 + nz*m33 > 0.0f )
            continue;

        // Assign the spheremap's texture coordinates
        pvVertices[i].tu = 0.5f * ( 1.0f + ( nx*m11 + ny*m21 + nz*m31 ) );
        pvVertices[i].tv = 0.5f * ( 1.0f - ( nx*m12 + ny*m22 + nz*m32 ) );
    }

    return S_OK;
}
*/
/*	MATRIX4		matWorld,matScale;
	VECTOR3		v3Scale;
	v3Scale.z = v3Scale.x = 1.05f;
	v3Scale.y = 1.0f;

	SetScaleMatrix(&matScale,&v3Scale);
	MatrixMultiply2(&matWorld,&matScale,pMatWorld);

	pDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)&matWorld);
*/


/*
이 부분 수정해야할지도 모름...방향땜시..

  subps			xmm0,xmm2				; u = m_pv3World[pIndex[j*3+2]] - m_pv3World[pIndex[j*3+0]];
subps			xmm1,xmm3				; v = m_pv3World[pIndex[j*3+2]] - m_pv3World[pIndex[j*3+1]];

void CVLMeshObject::SetVertexNormal()
{
	WORD*			pIndex;
	DWORD			i,j,dwFacesNum;

	VECTOR3*		pv3World = m_pv3World;
	VECTOR3*		pv3Normal = m_pv3Normal;
	DWORD			dwVertexNum = m_dwVertexNum;


	DWORD			dwFaceGroupNum = m_dwFaceGroupNum;

	memset(m_pv3Normal,0,sizeof(VECTOR3)*m_dwVertexNum);

	if (!m_dwFaceGroupNum)
		goto lb_return;


	for (i=0; i<m_dwFaceGroupNum; i++)
	{
		pIndex = m_pFaceGroup[i].GetIndexPtr();
		dwFacesNum = m_pFaceGroup[i].GetFacesNum();

		__asm
		{
				mov				ecx,dword ptr[dwFacesNum]
				mov				edi,dword ptr[pIndex]
				mov				ebx,12
				or				ecx,ecx
				jz				lb_cross_end

				
lb_loop:
				xor				edx,edx					; clean edx
				mov				esi,dword ptr[pv3World]

				movzx			eax,word ptr[edi]		; index 0
				mul				ebx						 
				movups			xmm2,[esi+eax]			; m_pv3World[pIndex[j*3+0]]
				push			eax						; backup index 0

				movzx			eax,word ptr[edi+2]		; index 1
				mul				ebx
				movups			xmm3,[esi+eax]			; m_pv3World[pIndex[j*3+1]]
				push			eax						; backup index 1
	
				movzx			eax,word ptr[edi+4]		; index 2
				mul				ebx
				movups			xmm0,[esi+eax]			; m_pv3World[pIndex[j*3+2]]
				push			eax						; bakup index 2
				
				movaps			xmm5,xmm2				; backup m_pv3World[pIndex[j*3+0]]
				movaps			xmm4,xmm0				; backup m_pv3World[pIndex[j*3+2]]
				
				movaps			xmm1,xmm0				;
				subps			xmm0,xmm2				; u = m_pv3World[pIndex[j*3+2]] - m_pv3World[pIndex[j*3+0]];
				subps			xmm1,xmm3				; v = m_pv3World[pIndex[j*3+2]] - m_pv3World[pIndex[j*3+1]];

				; cross vector c = u*v
				movaps			xmm2,xmm0			;	?	|	z1	|	y1	|	x1

				shufps			xmm0,xmm0,146		;	z1	|	y1	|	x1	|	z1	
				shufps			xmm1,xmm1,41		;	x2	|	z2	|	z2	|	y2
				mulps			xmm0,xmm1			; z1x2	| y1z2	| x1z2	| z1y2

				shufps			xmm1,xmm1,48		;	?	|	x2	|	?	|	y2
				shufps			xmm2,xmm2,16		;	?	|	y1	|	?	|	x1
				mulps			xmm1,xmm2			;	?	| y1x2	|	?	| x1y2

				movhlps			xmm2,xmm0			;	?		|	?		| z1x2		| y1z2
				subps			xmm2,xmm0			;	?		|	?		| z1x2-x1z2	| y1z2-z1y2

				movhlps			xmm0,xmm1			;	?	|	?	|	?	| y1x2
				subss			xmm1,xmm0			;	?	|	?	|	?	| x1y2-y1x2

				shufps			xmm2,xmm1,4			;   ?	|	z	|	y	|	x
				; result		c = xmm2	

				; normalize
				
				; xmm5 = m_pv3World[pIndex[j*3+0]]
				; xmm3 = m_pv3World[pIndex[j*3+1]]
				; xmm4 = m_pv3World[pIndex[j*3+2]]

				mov				esi,dword ptr[pv3Normal]
				
				pop				eax
				movups			xmm5,[esi+eax]		; m_pv3Normal[pIndex[j*3+2]] += c
				addps			xmm5,xmm2			;
				movlps			[esi+eax],xmm5
				movhlps			xmm5,xmm5
				movss			[esi+eax+8],xmm5

				pop				eax
				movups			xmm4,[esi+eax]		; m_pv3Normal[pIndex[j*3+1]] += c
				addps			xmm4,xmm2			;
				movlps			[esi+eax],xmm4
				movhlps			xmm4,xmm4
				movss			[esi+eax+8],xmm4		
				
				pop				eax
				movups			xmm3,[esi+eax]		; m_pv3Normal[pIndex[j*3+0]] += c
				addps			xmm3,xmm2			;
				movlps			[esi+eax],xmm3	
				movhlps			xmm3,xmm3
				movss			[esi+eax+8],xmm3

lb_loop_end:
				add				edi,6
				dec				ecx
				jnz				lb_loop
lb_cross_end:
		}
	}
	__asm
	{
				mov				ecx,dwVertexNum
				mov				edi,dword ptr[pv3Normal]
lb_loop_normalize:
				movups			xmm2,[edi]
			
				movaps			xmm0,xmm2
				mulps			xmm0,xmm0			;	?	|	zz	|	yy	|	xx
				movhlps			xmm1,xmm0			;	?	|	?	|	?	|	zz
				addss			xmm1,xmm0			;	?	|	?	|	?	| xx+zz
				shufps			xmm0,xmm0,1			;	?	|	?	|	?	|	yy
				addss			xmm1,xmm0			;	?	|	?	|	?	| xx+yy+zz
				sqrtss			xmm0,xmm1
				shufps			xmm0,xmm0,0			
				divps			xmm2,xmm0

				movlps			[edi],xmm2
				movhlps			xmm2,xmm2
				movss			[edi+8],xmm2
				
				add				edi,12
				loop			lb_loop_normalize
	}
	m_VB.SetNormal(m_pv3Normal,m_dwVertexNum);
lb_return:
	return;
}*/
/*
void CVLMeshObject::SetVertexNormal()
{
	VECTOR3			u,v,c;
	WORD*			pIndex;
	DWORD			i,j,k;

//	VECTOR3 zero;
//	zero.x = 0.0f;
//	zero.y = 0.0f;
//	zero.z = 0.0f;
	VECTOR3*		pv3World = m_pv3World;

	memset(m_pv3Normal,0,sizeof(VECTOR3)*m_dwVertexNum);

	for (i=0; i<m_dwFaceGroupNum; i++)
	{
		pIndex = m_pFaceGroup[i].GetIndexPtr();

		for (j=0; j<m_pFaceGroup[i].GetFacesNum(); j++)
		{
			VECTOR3_SUB_VECTOR3(&u,&pv3World[pIndex[j*3+1]],&pv3World[pIndex[j*3+0]]);
			VECTOR3_SUB_VECTOR3(&v,&pv3World[pIndex[j*3+2]],&pv3World[pIndex[j*3+0]]);

			CrossProduct(&c,&u,&v);
//			Normalize(&c,&c);

			for (k=0; k<3; k++)
			{
				VECTOR3_ADDEQU_VECTOR3(&m_pv3Normal[pIndex[j*3+k]],&c);
			}
		}
	}
	if (m_dwFaceGroupNum)
	{
		for (i=0; i<m_dwVertexNum; i++)
		{
			Normalize(&m_pv3Normal[i],&m_pv3Normal[i]);
		}
	}
	m_VB.SetNormal(m_pv3Normal,m_dwVertexNum);
}*/


	//////////// vertex shader //////////////////////////////////////////////
/*
	MatrixMultiply2(&matResult,m_pRenderer->INL_GetViewMatrix(),m_pRenderer->INL_GetProjMatrix());
	
	pDevice->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&matResult);
	pDevice->SetTransform( D3DTS_VIEW, (D3DXMATRIX*)m_pRenderer->GetIdentityMatrix());
	pDevice->SetTransform( D3DTS_PROJECTION, (D3DXMATRIX*)m_pRenderer->GetIdentityMatrix());
	bNeedRestoreView = TRUE;

	Transpose(&matResult);
	pDevice->SetVertexShaderConstant(0, &matResult,4 );
	pDevice->SetVertexShader(m_pRenderer->GetShader(VL_TEST));
*/