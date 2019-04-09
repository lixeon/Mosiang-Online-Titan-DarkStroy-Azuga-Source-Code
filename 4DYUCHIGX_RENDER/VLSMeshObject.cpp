#include "CoD3DDevice.h"
#include "VLSMeshObject.h"
#include "../4DyuchiGXGFunc/global.h"
#include "d3d_helper.h"
#include "VLFaceGroup.h"
#include "render_type.h"
#include "VBHeap.h"
#include "../4DYUCHIGRX_MYSELF97/Myself97Common.h"

//DWORD CVLSMeshObject::m_dwVertexShader = 0xffffffff;

CVLSMeshObject::CVLSMeshObject()
{
	memset((char*)this+4,0,sizeof(CVLSMeshObject)-4);
	m_bCanUpdate = TRUE;


	m_fRenderPriorityFactor = RENDER_PRIORITY_FACTOR_VLSOBJECT;

}
BOOL __stdcall CVLSMeshObject::StartInitialize(MESH_DESC* pDesc,IGeometryController* pControl,IGeometryControllerStatic* pControlStatic)
{
	BOOL	bResult = FALSE;
/*
	if (m_dwVertexShader == 0xffffffff)
	{

		DWORD dwDecl[] =
		{
			D3DVSD_STREAM(0),
			D3DVSD_REG(D3DVSDE_POSITION ,D3DVSDT_FLOAT3),
			D3DVSD_STREAM(1),
			D3DVSD_REG(D3DVSDE_DIFFUSE ,D3DVSDT_D3DCOLOR),
			D3DVSD_STREAM(2),
			D3DVSD_REG(D3DVSDE_TEXCOORD0 ,D3DVSDT_FLOAT2),
			D3DVSD_END()
		};


		m_pRenderer->GetDevice()->CreateVertexShader(dwDecl,NULL,&m_dwVertexShader,0);
	}
*/
	m_pMeshController = pControl;
	m_meshFlag = pDesc->meshFlag;
	m_dwVertexNum = pDesc->dwVertexNum;

	m_dwMaxFaceGroupNum = pDesc->dwFaceGroupNum;
	m_pFaceGroup = new CVLFaceGroup[m_dwMaxFaceGroupNum];
	
	
	m_pv3Local = new VECTOR3[m_dwVertexNum];
	memcpy(m_pv3Local,pDesc->pv3LocalList,sizeof(VECTOR3)*m_dwVertexNum);

//	TransformVector3_VPTR2(m_pv3Local,m_pv3World,pDesc->pMatrixWorldInverse,m_dwVertexNum);

	DWORD	dwSize;
	DWORD	dwErrorCode;
	// 아래에서 인터럽트가 걸린 경우, m_dwVertexNum을 보라. 하나의 오브젝트에 버텍스 갯수가 엄하게 나온 경우다.
	if (!m_pRenderer->GetVBVLSMeshHeap()->AllocVB(&dwErrorCode,&m_VB,m_dwVertexNum,&dwSize,&m_VB.m_bEnable,NULL))
	{
		PrintfDebugString("CVLSMeshObject::StartInitialize() 오브젝트 하나에 %d개의 버텍스가 들어왔다. (One object has %d vertices. reduce!)\n", m_dwVertexNum, m_dwVertexNum);
		__asm int 3
	}

	if (!m_pRenderer->GetVBVector3MeshHeap()->AllocVB(&dwErrorCode,&m_VBProjected,m_dwVertexNum,&dwSize,&m_VBProjected.m_bEnable,NULL))
	{
		PrintfDebugString("CVLSMeshObject::StartInitialize() 오브젝트 하나에 %d개의 버텍스가 들어왔다. (One object has %d vertices. reduce!)\n", m_dwVertexNum, m_dwVertexNum);
		__asm int 3
	}


	m_VB.SetVertexPosList(pDesc->pv3WorldList,m_dwVertexNum);
	m_VBProjected.SetVertexPosList(pDesc->pv3WorldList,m_dwVertexNum);

	if (pDesc->pVertexColor)
	{
		m_pVertexColor = pDesc->pVertexColor;
		m_VB.SetVertexColor(m_pVertexColor,DIFFUSE_COORD_VLS_OFFSET,m_dwVertexNum);
	}

	if (pDesc->ptvTexCoordList)
		m_VB.SetTexCoord(pDesc->ptvTexCoordList,TEX1_COORD_OFFSET_VLSMESH,m_dwVertexNum);
	

	


	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL __stdcall CVLSMeshObject::InsertFaceGroup(FACE_DESC* pDesc)
{

	BOOL		bResult = FALSE;
	DIMATERIAL*		pMtl;

	if (!m_pFaceGroup[m_dwFaceGroupNum].CreateFaceGroup(
		m_pRenderer,
		m_pRenderer->GetDevice(),
		m_pv3Local,
		pDesc->pIndex,
		NULL,//m_pVertexColor,
		pDesc->dwFacesNum,CREATE_FACEGROUP_TYPE_STATIC))
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
BOOL __stdcall CVLSMeshObject::Render(
		DWORD dwRefIndex,										// 애니메이션 컨트롤블럭
		DWORD dwAlpha,
		LIGHT_INDEX_DESC* pDynamicLightIndexList,DWORD dwLightNum,
		LIGHT_INDEX_DESC* pSpotLightIndexList,DWORD dwSpotLightNum,
		DWORD dwMtlSetIndex,
		DWORD dwEffectIndex,
		DWORD dwFlag					
		)
{
	DWORD i,j;

	BOOL				bLight = FALSE;
	BOOL				bShadow = FALSE;

	LPDIRECT3DTEXTURE8	pLightTex;
	LIGHT_DESC*			pLight;
	
	LPDIRECT3DDEVICE8	pDevice = m_pRenderer->GetDevice();
	CAMERA_DESC*		pCamera;
	MATRIX4*			pResult = m_pRenderer->GetIdentityMatrix();
	LPDIRECT3DTEXTURE8	pDefaultTex = m_pRenderer->GetDefaultMaterial()->TextureDiffuse.GetTexture();


	if (dwFlag & RENDER_TYPE_NOT_DRAW)
		goto lb_return;

	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

lb_render:


	pDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)pResult);		
	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,0);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);
	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);

	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	pDevice->SetVertexShader(m_VB.GetVertexShader());
	
	pDevice->SetStreamSource( 0,m_VB.GetVertexBuffer(),m_VB.GetVertexSize());


	if (dwFlag & RENDER_TYPE_DISABLE_ZCLIP)
		pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);


	if (m_pRenderer->INL_IsEnableFog())
		pDevice->SetRenderState(D3DRS_FOGENABLE,TRUE);

	if (m_meshFlag.IsDisableZBubfferWrite())
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

	DIMATERIAL*		pMtl;
	for (i=0; i<m_dwFaceGroupNum; i++)
	{
	//	pMtl = m_pRenderer->GetMaterial(1,i);
		pMtl = m_pFaceGroup[i].GetMaterial();

		if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

		if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_SELF_ILLUMIN)
			pDevice->SetMaterial(&pMtl->d3dMtl);


		// 페이스그룹의 해당 매터리얼 상태에 맞게 알파블랜딩 스타트
		BeginAlphaMeshObject(pDevice,1,pMtl->dwAlpha,dwAlpha);

		if (m_pRenderer->GetLightMapFlag() & LIGHTMAP_FLAG_DISABLE_TEXTURE)
			m_pRenderer->GetDefaultMaterial()->TextureDiffuse.SetTexture(pDevice,0);
		else
			pMtl->TextureDiffuse.SetTexture(pDevice,0);


		pDevice->SetIndices(m_pFaceGroup[i].GetIndexBuffer(),m_VB.GetStartIndex());
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBStartIndex(),m_pFaceGroup[i].GetFacesNum());


		// 알파블랜딩 end.
		EndAlphaMeshObject(pDevice);

		if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	}
//	m_pRenderer->SetRenderMode(0);

	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	if (m_pRenderer->INL_IsEnableFog())
		pDevice->SetRenderState(D3DRS_FOGENABLE,FALSE);

	if (m_meshFlag.IsDisableZBubfferWrite())
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

	if ( (dwFlag & RENDER_TYPE_RECV_SHADOW) && dwSpotLightNum)
		bShadow = TRUE;

	if ((dwFlag & RENDER_TYPE_UPDATE_SHADING) && dwLightNum)
		bLight = TRUE;

	BOOL	bCameraUpdate;

	if ( !(bLight | bShadow) )
		goto lb_return;

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);
	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_CAMERASPACEPOSITION );

	pCamera = m_pRenderer->INL_GetCameraDesc();

	pDevice->SetVertexShader(m_VBProjected.GetVertexShader());
	pDevice->SetStreamSource( 0,m_VBProjected.GetVertexBuffer(),m_VBProjected.GetVertexSize());

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
		
		
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
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
				
				DWORD	dwPrjLightIndicesNum;
				dwPrjLightIndicesNum = m_pFaceGroup[i].BuildPrjLightMesh(m_pRenderer->GetIBCache(),j,&pLight->v3Point,bCameraUpdate);
				if (!dwPrjLightIndicesNum)
					continue;

			
//				pDevice->SetIndices(m_pFaceGroup[i].GetProjectedIndexBuffer(),m_VBProjected.GetStartIndex());
//				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_dwVertexNum,m_pFaceGroup[i].GetIBProjectedStartIndex(),m_pFaceGroup[i].GetCulledWithLightFacesNum());

				pDevice->SetIndices(m_pFaceGroup[i].GetIBPrjLightPtr(j)->GetIndexBuffer(),m_VBProjected.GetStartIndex());
				pDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,
					0,
					m_dwVertexNum,
					m_pFaceGroup[i].GetIBPrjLightPtr(j)->GetStartIndex(),
					dwPrjLightIndicesNum);

				m_pRenderer->AddLitFacesCount(dwPrjLightIndicesNum);
			}
		}
	}
lb_exit_dynamic_lit:
	
	if (bShadow)
	{
		MATRIX4				matTex;
		SPOT_LIGHT_DESC*	pSpotDesc;
		
		pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_PROJECTED | D3DTTFF_COUNT3 );
		pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		
		pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
		pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);// 무조건 맞음.

		for (DWORD m=0; m<dwSpotLightNum; m++)
		{
			pSpotDesc = m_pRenderer->INL_GetSpotDesc(pSpotLightIndexList[m].bLightIndex);
			MatrixMultiply2(&matTex,m_pRenderer->INL_GetInverseViewMatriex(),&pSpotDesc->prjDesc.matPrjTex);
		
			pDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + 0),(D3DMATRIX*)&matTex);

			if (pSpotLightIndexList[m].pMtlHandle)
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
				
			VECTOR3*			pShadowLightPos;
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


	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	

	return TRUE;
}


BOOL __stdcall CVLSMeshObject::Update(DWORD dwFlag)
{
	if (!m_bCanUpdate)
		return FALSE;

	if (m_pVertexColor)
		m_VB.SetVertexColor(m_pVertexColor,DIFFUSE_COORD_VLS_OFFSET,m_dwVertexNum);

	return TRUE;
}


void __stdcall CVLSMeshObject::EndInitialize()
{

	// 알파 속성을 가지고 페이스그룹을 소트시킨다.
	SortWithAlpha();
	if (m_pv3Local)
	{
		delete [] m_pv3Local;
		m_pv3Local = NULL;
	}
}
