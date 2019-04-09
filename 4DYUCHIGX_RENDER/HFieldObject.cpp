#include "HFieldObject.h"
#include "HFieldManager.h"
#include "CoD3DDevice.h"
#include "../4DyuchiGRX_myself97/My2DLib.h"
#include "render_type.h"
#include "StaticVBPool.h"


STDMETHODIMP CHFieldObject::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (refiid == IID_HFIELD_OBJECT)
	{
		*ppv = (IDIHFieldObject*)this;
		AddRef();
		return S_OK;
	}

	*ppv = NULL;
	return E_NOINTERFACE;
}
STDMETHODIMP_(ULONG) CHFieldObject::AddRef(void)
{
	m_dwRefCount++;
	return m_dwRefCount;
}
STDMETHODIMP_(ULONG) CHFieldObject::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}
CHFieldObject::CHFieldObject(CHFieldManager* pHFieldManager,DWORD dwFlag)
{

	m_fDistanceFromViewPoint = 900000.0f;
	m_bCanUpdate = TRUE;
	m_dwRefCount = 0;

	m_fRenderPriorityFactor = RENDER_PRIORITY_FACTOR_HFIELD;
	m_pHFieldManager = pHFieldManager;
	m_pRenderer = pHFieldManager->GetRenderer();
}
BOOL __stdcall CHFieldObject::Create(DWORD dwPosX,DWORD dwPosZ,DWORD dwDetailLevel,DWORD dwFacesNumPerX,DWORD dwFacesNumPerZ,VECTOR3* pv3Rect,HFIELD_DESC* pHFDesc)
{
	m_dwPosX = dwPosX;
	m_dwPosZ = dwPosZ;

//	DWORD dwTilePosX = dwPosX / pHFDesc->dwFacesNumPerTileAxis;
//	DWORD dwTilePosZ = dwPosZ / pHFDesc->dwFacesNumPerTileAxis;
//	m_TileObject.Initialize(dwTilePosX,dwTilePosZ,pHFDesc);

	m_pv3Rect = pv3Rect;
	DWORD	dwVertexNum = (dwFacesNumPerX+1)*(dwFacesNumPerZ+1);
	
	return TRUE;



}
BOOL __stdcall CHFieldObject::IsIncludeViewVolume()
{
	return m_pControllerStatic->IsIncludeViewVolume();
}
	
BOOL __stdcall CHFieldObject::OnOut()
{
	return m_pControllerStatic->OnOut();
	
}

BOOL __stdcall CHFieldObject::OnFault()
{
	DWORD	dwErrorCode;
	BOOL	bResult = m_pHFieldManager->GetStaticVBPool()->AllocVB(&dwErrorCode,&m_VB,0,NULL,&m_VB.m_bEnable,this);
	bResult = m_pControllerStatic->OnFault();
	return bResult;
}
BOOL __stdcall CHFieldObject::SetYFactor(DWORD dwDestPitch,HFIELD_DESC* pHFDesc)
{
	BOOL	bResult = FALSE;
	if (!m_VB.m_bEnable)
		goto lb_return;

	bResult = m_VB.SetVertexPosList(dwDestPitch,m_dwPosX,m_dwPosZ,pHFDesc,1);

lb_return:
	return bResult;

}


BOOL __stdcall CHFieldObject::StartInitialize(MESH_DESC* pDesc,IGeometryController* pControl,IGeometryControllerStatic* pControlStatic)
{
	m_pControllerStatic = pControlStatic;
	return TRUE;
}
BOOL __stdcall CHFieldObject::InsertFaceGroup(FACE_DESC* pDesc)
{
	return FALSE;
}
void __stdcall CHFieldObject::EndInitialize()
{

}

void __stdcall CHFieldObject::SetDistanceFromViewPoint(float fDistance)
{	
	m_fDistanceFromViewPoint = fDistance;

}
BOOL __stdcall CHFieldObject::SetDetailLevel(DWORD dwDetailLevel)
{
	BOOL	bResult = FALSE;
	
	
	if (dwDetailLevel >= m_pHFieldManager->GetDetailLevelNum())
		goto lb_return;

	if (dwDetailLevel != m_dwDetailLevel)
	{
		m_dwDetailLevel = dwDetailLevel;
		m_bMustUpdate = TRUE;
	}
	
	
	bResult = TRUE;

lb_return:
	return bResult;
}
void __stdcall CHFieldObject::SetPositionMask(DWORD dwPosMask)
{
	// 포지션 마스크가 바뀌면 인덱스버퍼를 업데이트해야한다.
	if (m_dwPositionMask != dwPosMask)
	{
		m_dwPositionMask = dwPosMask;
		m_bMustUpdate = TRUE;
	}
	
}


void __stdcall CHFieldObject::SetMustUpdate()
{
	m_bMustUpdate = TRUE;
//	return m_TileObject.UpdateTileInfo(m_pHFieldManager->GetHFieldDesc());
	

}
BOOL __stdcall CHFieldObject::SetVertexColorAll(DWORD dwColor)
{
	BOOL	bResult = FALSE;
	if (!m_VB.m_bEnable)
		goto lb_return;

	bResult = m_VB.SetVertexColor(dwColor,DIFFUSE_COORD_OFFSET_HFIELD,m_pHFieldManager->GetVerticesNumPerObj());

lb_return:
	return bResult;

}
BOOL __stdcall CHFieldObject::SetVertexColor(DWORD* pdwColor,DWORD dwVerticesNum)
{
	BOOL	bResult = FALSE;
	if (!m_VB.m_bEnable)
		goto lb_return;

	bResult = m_VB.SetVertexColor(pdwColor,DIFFUSE_COORD_OFFSET_HFIELD,dwVerticesNum);
lb_return:
	return bResult;
}
BOOL __stdcall CHFieldObject::Render(
		DWORD dwRefIndex,										// 애니메이션 컨트롤블럭
		DWORD dwAlpha,
		LIGHT_INDEX_DESC* pDynamicLightIndexList,DWORD dwLightNum,
		LIGHT_INDEX_DESC* pSpotLightIndexList,DWORD dwSpotLightNum,
		DWORD dwMtlSetIndex,
		DWORD dwEffectIndex,
		DWORD dwFlag					
		)
{
	DWORD				i,j;
	
	BOOL				bLight = FALSE;
	BOOL				bShadow = FALSE;
	LPDIRECT3DTEXTURE8	pLightTex;
	LIGHT_DESC*			pLight;
	LPDIRECT3DDEVICE8	pDevice = m_pRenderer->GetDevice();
	LPDIRECT3DTEXTURE8	pDefaultTex = m_pRenderer->GetDefaultMaterial()->TextureDiffuse.GetTexture();

	BOOL	bResult = FALSE;
	
	if (dwFlag & RENDER_TYPE_NOT_DRAW)
		goto lb_return;

	if (!m_VB.m_bEnable)
		OnFault();

	m_VB.SetTimeStamp();
	m_pRenderer->IncRenderHFObjNum();

	// 인덱스버퍼를 새로 할당받아 갱신을 하든가..아니면 이전 렌더링 시에 할당받은걸 그대로 쓰던가...
	if (!m_IB.m_bEnable)
		goto lb_update_indexbuffer;

	if (m_bMustUpdate)
	{
		ReleaseIndexBuffer();	
		m_bMustUpdate = FALSE;

		goto lb_update_indexbuffer;
	}

	

	goto lb_render;

lb_update_indexbuffer:
	INDEX_LIST_DESC		quadIndexList[4];
	m_pHFieldManager->AllocIndexBuffer(&m_dwTriNum,this,m_dwDetailLevel,m_dwPositionMask,quadIndexList);
	m_pRenderer->IncCacheMissRenderHFObjNum();

//	m_pHFieldManager->GetIndexBuffer(&m_pIB,&m_dwTriNum,this,0,0);

	
lb_render:
	pDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)m_pRenderer->GetIdentityMatrix());
	
	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );


	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_MIRROR );
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_MIRROR );
	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_CAMERASPACEPOSITION );
	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2 );

	pDevice->SetVertexShader(m_VB.GetVertexShader());
	pDevice->SetStreamSource( 0,m_VB.GetVertexBuffer(),m_VB.GetVertexSize());

	if (m_pRenderer->INL_IsEnableFog())
		pDevice->SetRenderState(D3DRS_FOGENABLE,TRUE);

	for (i=0; i<m_pControllerStatic->GetTileBufferDescNum(); i++)
	{
		TILE_BUFFER_DESC*	pTileBufferDesc;
		pTileBufferDesc = m_pControllerStatic->GetTileBufferDesc(i);

		IMAGE_HEADER*	pHeader;	
		DWORD			dwTileIndex = pTileBufferDesc->GetTileIndex();
		DWORD			dwTileDir = pTileBufferDesc->GetTileDir();
		

		m_pHFieldManager->SetIndexedTexture(dwTileIndex,0,&pHeader);

		MATRIX4	matAdjustUV[4];
		DWORD	dwTexWidth;
		if (pHeader)
			dwTexWidth = pHeader->dwWidth;
		else
			dwTexWidth = 0;
		

		SetHFieldTexMatrix4(matAdjustUV,m_pv3Rect,m_pHFieldManager->GetTileSize(),dwTexWidth,dwTileDir);

		//////////////////// 알파맵 임시.../////////////////////////
		pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
		if (pTileBufferDesc->alphaMapDesc.pAlphaMapBits)
		{
			//if (pTileBufferDesc->alphaMapDesc.pVoidExt)
lb_alpha_tex_check:
			IDirect3DTexture8*	pAlphaTex = (IDirect3DTexture8*)pTileBufferDesc->alphaMapDesc.pVoidExt;
			if (!pAlphaTex)
			{
				UpdateAlphaMap(pTileBufferDesc);
				goto lb_alpha_tex_check;
			}

			
#ifdef _DEBUG
			if (!m_pRenderer->GetD3DResourceManager()->IsValid(pAlphaTex))
				__asm int 3
#endif
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
			pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			pDevice->SetTextureStageState( 1, D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP );
			pDevice->SetTextureStageState( 1, D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP );
			pDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_CAMERASPACEPOSITION );
			pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2 );

			pDevice->SetTexture(1,pAlphaTex);
			MATRIX4	matAdustUV1;
			SetHFieldTexMatrix(&matAdustUV1,m_pv3Rect);

			MatrixMultiply2(&matAdustUV1,m_pRenderer->INL_GetInverseViewMatriex(),&matAdustUV1);
			pDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + 1),(D3DMATRIX*)&matAdustUV1);
		}
		

		///////////////////////////////////////////////////////////

		pDevice->SetIndices(m_IB.GetIndexBuffer(),m_VB.GetStartIndex());
		
		BeginAlphaMeshObject(pDevice,2,255,dwAlpha);

		for (DWORD j=0; j<4; j++)
		{
			MatrixMultiply2(&matAdjustUV[j],m_pRenderer->INL_GetInverseViewMatriex(),&matAdjustUV[j]);
			pDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + 0),(D3DMATRIX*)&matAdjustUV[j]);

			
			if (m_pControllerStatic->GetTileBufferDesc(i)->indexEntryDesc[j].dwTriNum)
			{			


				// Solid 위에 Wire Frame을 겹쳐 그리는 모드라면, 
				if (TRUE == m_pRenderer->GetRenderWireSolidBothMode())
				{				
					DWORD dwLegacyRenderMode = m_pRenderer->GetRenderMode();
					
					// Solid 로 한번 그리고, 

					m_pRenderer->SetRenderMode(RENDER_MODE_SOLID);

					pDevice->DrawIndexedPrimitive(
						D3DPT_TRIANGLELIST,
						0,
						m_pHFieldManager->GetVerticesNumPerObj(),
						m_IB.GetStartIndex() + m_pControllerStatic->GetTileBufferDesc(i)->indexEntryDesc[j].dwStartIndex,
						m_pControllerStatic->GetTileBufferDesc(i)->indexEntryDesc[j].dwTriNum);

					m_pRenderer->AddFacesNum(m_pControllerStatic->GetTileBufferDesc(i)->indexEntryDesc[j].dwTriNum);



					// Wireframe으로 겹쳐 그린다. 

					m_pRenderer->SetRenderMode(RENDER_MODE_WIREFRAME);
					SetColorFactorShader(pDevice, 0xff808080,0);					

					pDevice->DrawIndexedPrimitive(
						D3DPT_TRIANGLELIST,
						0,
						m_pHFieldManager->GetVerticesNumPerObj(),
						m_IB.GetStartIndex() + m_pControllerStatic->GetTileBufferDesc(i)->indexEntryDesc[j].dwStartIndex,
						m_pControllerStatic->GetTileBufferDesc(i)->indexEntryDesc[j].dwTriNum
						);
					m_pRenderer->AddFacesNum(m_pControllerStatic->GetTileBufferDesc(i)->indexEntryDesc[j].dwTriNum);
					

					RestoreTextureShader(pDevice,0);
					m_pRenderer->SetRenderMode(dwLegacyRenderMode);
				}
				else
				{

					// 정상 동작을 수행. 

					pDevice->DrawIndexedPrimitive(
						D3DPT_TRIANGLELIST,
						0,
						m_pHFieldManager->GetVerticesNumPerObj(),
						m_IB.GetStartIndex() + m_pControllerStatic->GetTileBufferDesc(i)->indexEntryDesc[j].dwStartIndex,
						m_pControllerStatic->GetTileBufferDesc(i)->indexEntryDesc[j].dwTriNum);
	
					m_pRenderer->AddFacesNum(m_pControllerStatic->GetTileBufferDesc(i)->indexEntryDesc[j].dwTriNum);


				}

				
				
			}
		}
	}
	
	EndAlphaMeshObject(pDevice);
	if (m_pRenderer->INL_IsEnableFog())
		pDevice->SetRenderState(D3DRS_FOGENABLE,FALSE);

	if ((dwFlag & RENDER_TYPE_UPDATE_SHADING) && dwLightNum)
		bLight = TRUE;

	if ((dwFlag & RENDER_TYPE_RECV_SHADOW) && dwSpotLightNum )
		bShadow = TRUE;
	

	if (!(bLight | bShadow))
		goto lb_exit_shadow;


	pDevice->SetVertexShader(D3DFVF_XYZ);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);
	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_CAMERASPACEPOSITION );
	


	if (bLight)
	{
		bLight=!bLight;
	}
	if (bLight)
	{
		pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2 );
		pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2 );
		pDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_CAMERASPACEPOSITION );
		
		pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );

		pDevice->SetTextureStageState( 1, D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
		pDevice->SetTextureStageState( 1, D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);

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

			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				m_pHFieldManager->GetVerticesNumPerObj(),
				m_IB.GetStartIndex(),
				m_dwTriNum);
		}
	}

lb_exit_dynamic_lit:

//	if (!m_pRenderer->INL_GetShadowFlag())
//		goto lb_return;
	
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

			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				m_pHFieldManager->GetVerticesNumPerObj(),
				m_IB.GetStartIndex(),
				m_dwTriNum);

			if (pSpotLightIndexList[m].pMtlHandle && pSpotLightIndexList[m].bTexOP == 0xff)
			{
				pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
				pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
			}
		}
	}
lb_exit_shadow:
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
	
lb_return:
	return TRUE;

}
BOOL CHFieldObject::UpdateAlphaMap(TILE_BUFFER_DESC* pTileBufferDesc)
{
	////////// 일단 테스트.. ///////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	ALPHAMAP_DESC*	pAlphaMapDesc;
	pAlphaMapDesc = &pTileBufferDesc->alphaMapDesc;
	if (pAlphaMapDesc->pAlphaMapBits)
	{
		IDirect3DTexture8*	pAlphaTexture = (IDirect3DTexture8*)pAlphaMapDesc->pVoidExt;
		if (!pAlphaTexture)
		{
			m_pHFieldManager->GetRenderer()->GetD3DResourceManager()->CreateTexture(
				pAlphaMapDesc->wWidthHeight,
				pAlphaMapDesc->wWidthHeight,
				1,
				0,
				D3DFMT_A4R4G4B4,
				D3DPOOL_MANAGED,
				&pAlphaTexture);

			pAlphaMapDesc->pVoidExt = (void*)pAlphaTexture;
		}

		D3DLOCKED_RECT	lockRect;
		pAlphaTexture->LockRect(0,&lockRect,NULL,0);

		Convert2BitTo16BitImageWithPitch(
											(char*)lockRect.pBits,
											pAlphaMapDesc->pAlphaMapBits,
											pAlphaMapDesc->wWidthHeight,
											pAlphaMapDesc->wWidthHeight,
											lockRect.Pitch);
		pAlphaTexture->UnlockRect(0);
	}
	return TRUE;
	//////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
}
	
void CHFieldObject::ReleaseIndexBuffer()
{
	// m_bIBEnable이 FALSE가 되는지 확인..
	m_IB.Cleanup();
}

void CHFieldObject::ReleaseVertexBuffer()
{
	// m_bIBEnable이 FALSE가 되는지 확인..
	m_VB.Cleanup();
}


BOOL __stdcall CHFieldObject::ReBuildMesh(DWORD dwDestPitch,HFIELD_DESC* pHFDesc , DWORD* pdwColor)
{
	BOOL	bResult = FALSE;
	if (!m_VB.m_bEnable)
		goto lb_return;

	BYTE*	pDest;
	m_VB.LockVertexPtr(&pDest,0);
	bResult = m_VB.SetVertexPosListWithManualLock(pDest,dwDestPitch,m_dwPosX,m_dwPosZ,pHFDesc,1);
	bResult = m_VB.SetVertexColorWithManualLock(pDest,pdwColor,DIFFUSE_COORD_OFFSET_HFIELD,pHFDesc->dwVerticesNumPerObj);
	m_VB.UnlockVertexPtr();
	
//	m_pHFieldControlObject->ReBuildMesh(m_pHField->GetFacesNumPerObjAxis()+1,&hfDesc , m_pdwVertexColor);
lb_return:
	return bResult;
}
DWORD __stdcall CHFieldObject::GetAlphaFlag()
{
	return 0;
}

BOOL __stdcall CHFieldObject::Update(DWORD dwFlag)
{
	if (!m_bCanUpdate)
		return FALSE;


	return TRUE;

}

void __stdcall CHFieldObject::CleanupAlphaMap()
{
	DWORD	i;
	for (i=0; i<m_pControllerStatic->GetTileBufferDescNum(); i++)
	{
		TILE_BUFFER_DESC*	pTileBufferDesc = m_pControllerStatic->GetTileBufferDesc(i);
		IDirect3DTexture8*	pAlphaTexture = (IDirect3DTexture8*)pTileBufferDesc->alphaMapDesc.pVoidExt;
		
		if (pAlphaTexture)
		{
			m_pHFieldManager->GetRenderer()->GetD3DResourceManager()->Release(pAlphaTexture);
			pTileBufferDesc->alphaMapDesc.pVoidExt = NULL;
		}
	}
}



CHFieldObject::~CHFieldObject()
{
	CleanupAlphaMap();
	



	

	
	
	CBaseMeshObject::~CBaseMeshObject();
}