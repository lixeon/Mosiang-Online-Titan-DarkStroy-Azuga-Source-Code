#include "CoGeometry.h"
#include "RenderType.h"
#include "geometry_global.h"
#include "../4DyuchiGXGFunc/global.h"
#include "SkyMesh.h"
#include <crtdbg.h>

extern LONG			g_lComponent;


MATRIX4 CoGeometry::m_matIdentity;
ErrorHandleProc		g_pErrorHandleFunc = NULL;
I4DyuchiFileStorage*	g_pFileStorage = NULL;

DWORD __stdcall DefaultErrorHandleProc(ERROR_TYPE type,DWORD dwErrorPriority,void* pCodeAddress,char* szStr)
{
	/*
	char szFileName[] = __FILE__;
	int iLineNumber = __LINE__;*/
		
	char	txt[512];
	memset(txt,0,512);

	char*	pMsg;
	if (ERROR_TYPE_FILE_NOT_FOUND == type)
	{
		wsprintf(txt,"File not found, %s\n",szStr);
		pMsg = szStr;
	}
	else
		pMsg = szStr;


	OutputDebugString(pMsg);

	if (!dwErrorPriority)
		__asm int 3

	return 0;
}

BOOL __stdcall Render_CoModel(CoGeometry* pGeometry,RENDER_MODEL_DESC* pDesc)
{
	BOOL	bResult;
	
	bResult = pDesc->pModel->Render(
		pDesc->dwRefIndex,
		pDesc->pMatParent,
		pDesc->pMatTransform,
		pDesc->dwFrame,
		pDesc->dwMotionIndex,
		pDesc->dwAlpha,
		pDesc->dwMtlSetIndex,
		pDesc->dwEffectIndex,
		pDesc->dwFlag);

	return bResult;
}
BOOL __stdcall Render_CoStaticModel(CoGeometry* pGeometry,RENDER_STATIC_MODEL_DESC* pDesc)
{
	
	BOOL	bResult;
	bResult = pDesc->pStaticModel->RenderObject(pDesc->dwObjIndex,pDesc->dwAlpha,pDesc->dwMtlSetIndex,pDesc->dwFlag);
	return bResult;
}
BOOL __stdcall Render_CoHeightField(CoGeometry* pGeometry,RENDER_HFIELD_DESC* pDesc)
{
	BOOL bResult;

	bResult = pDesc->pHField->RenderObject(pDesc->dwObjIndex,pDesc->dwAlpha,pDesc->dwFlag);
		
	return bResult;

}

DWORD __stdcall	CoGeometry::GetReadCountInMOD(char* szFileName)
{
	DWORD	dwCount = 0;

	void* pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_BINARY);
	if (!pFP)
		goto lb_return;

	FILE_SCENE_HEADER header;

	g_pFileStorage->FSRead(pFP,&header,sizeof(FILE_SCENE_HEADER));
	dwCount = header.dwObjectNum;
	g_pFileStorage->FSCloseFile(pFP);

lb_return:
	return dwCount;
}

DWORD __stdcall	CoGeometry::GetReadCountInSTM(char* szFileName)
{
	DWORD	dwCount = 0;

	void*	pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_BINARY);
	
	if (!pFP)
		goto lb_return;

	g_pFileStorage->FSSeek(pFP,sizeof(DWORD),FSFILE_SEEK_CUR);
	SkipMaterialListInSTM(pFP);
	dwCount++;
		
	DWORD	dwObjectNum;
	dwObjectNum = 0;
	g_pFileStorage->FSRead(pFP,&dwObjectNum,sizeof(DWORD));

	dwCount += dwObjectNum;
	g_pFileStorage->FSCloseFile(pFP);

lb_return:
	return dwCount;

}

DWORD __stdcall	CoGeometry::GetReadCountInHField(char* szFileName)
{
	DWORD	dwCount = 0;
	
	void*	pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_BINARY);
	if (!pFP)
		goto lb_return;

	g_pFileStorage->FSSeek(pFP,sizeof(DWORD),FSFILE_SEEK_CUR);


	HFIELD_DESC	hfDesc;
	g_pFileStorage->FSRead(pFP,&hfDesc,sizeof(HFIELD_DESC));


	dwCount = hfDesc.dwObjNumX*hfDesc.dwObjNumZ;

	g_pFileStorage->FSCloseFile(pFP);
	

lb_return:
	return dwCount;
}

STDMETHODIMP CoGeometry::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (IID_IUnknown == refiid)
		*ppv = this;
	else if (IID_4DyuchiGXGeometry == refiid)
		*ppv = (I4DyuchiGXGeometry*)this;
	else 
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();
	return NOERROR;
	
}
STDMETHODIMP_(ULONG) CoGeometry::AddRef(void)
{
	m_dwRefCount++;
	return 0;
	
}
STDMETHODIMP_(ULONG) CoGeometry::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}


CoGeometry::CoGeometry()
{
	InterlockedIncrement( &g_lComponent);

	memset((char*)this+4,0,sizeof(CoGeometry)-4);
	m_dwLightFlag = 1;
	m_bShadowUpdatePerFrame = TRUE;
	
#ifdef _DEBUG
	int	flag = _CRTDBG_ALLOC_MEM_DF |_CRTDBG_LEAK_CHECK_DF;

	_CrtSetDbgFlag(flag);
#endif

	//trustpak 2005/06/24

	m_dwLodUsingMode = 0;
	m_dwLODUsingLevel = 0;

	///
}

void __stdcall CoGeometry::SetRadiusForLoadResource(float fRS)
{
	m_fRSForLoadResource = fRS;
}

BOOL __stdcall CoGeometry::AddLight(LIGHT_DESC* pLight,DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	
	
	// 라이트 플래그가 0이면 애초에 라이트를 넣을수 없다..
	if (!m_dwLightFlag)	
		goto lb_return;


	if (dwFlag & GXLIGHT_TYPE_ENABLE_IMAGE_PROJECTION)
	{
		if (!pLight->pMtlHandle)
			goto lb_return;

		AddSpotLight(pLight,SPOT_LIGHT_TYPE_PRJIMAGE);
		bResult = TRUE;
		goto lb_return;
	}
	// 만일을 대비해서 매터리얼 핸들은 초기화시켜놓는다.
	pLight->pMtlHandle =  NULL;
	if (dwFlag & GXLIGHT_TYPE_ENABLE_SHADOW)
	{
		// 쉐도우 플래그가 0이면 쉐도우 라이트를 넣을수 없다.
		if (!m_dwShadowFlag)
			goto lb_return;
		
		AddSpotLight(pLight,SPOT_LIGHT_TYPE_SHADOW);
		bResult = TRUE;
		goto lb_return;
	}
	

	if (GXLIGHT_TYPE_STATIC & dwFlag)
	{
		// 모델에만 적용될 라이트..
		if (m_dwStaticRTLightNum >= MAX_STATIC_RTLIGHT_NUM_PER_SCENE)
			goto lb_return;

		m_RTLightList[STATIC_RTLIGHT_START_INDEX + m_dwStaticRTLightNum] = *pLight;
		m_pRenderer->SetRTLight(pLight,STATIC_RTLIGHT_START_INDEX + m_dwStaticRTLightNum,0);
		m_dwStaticRTLightNum++;
		bResult = TRUE;
		goto lb_return;
	}

	if (m_dwDynamicRTLightNum >= MAX_DYNAMIC_RTLIGHT_NUM_PER_SCENE)
		goto lb_return;

	m_RTLightList[m_dwDynamicRTLightNum] = *pLight;
	m_pRenderer->SetRTLight(pLight,m_dwDynamicRTLightNum,0);
	m_dwDynamicRTLightNum++;

	bResult = TRUE;

lb_return:
	return bResult;

}

BOOL __stdcall CoGeometry::BeginRender(DWORD dwViewportIndex,DWORD dwColor,DWORD dwFlag)
{
	BOOL bResult = FALSE;
	if (!m_pRenderer)
		goto lb_return;


	m_dwCurrentViewportIndex = dwViewportIndex;

	Clear(m_pRenderQ);
	m_bIsBeginRender = TRUE;


	m_LightIndexPool.FreeAll();
	
	memset(m_ShadowSender,0,sizeof(m_ShadowSender));
	
	m_dwStaticRTLightNum = 0;
	m_dwDynamicRTLightNum = 0;
	m_dwSpotLightNum[0] = 0;
	m_dwSpotLightNum[1] = 0;
	m_dwTotalSpotLightNum = 0;

	m_MatrixPool.FreeAll();

	if (m_pViewport[dwViewportIndex].fogDesc.bEnable)
	{
		m_pRenderer->EnableFog(
			m_pViewport[dwViewportIndex].fogDesc.fStart,
			m_pViewport[dwViewportIndex].fogDesc.fEnd,
			m_pViewport[dwViewportIndex].fogDesc.fDensity,
			m_pViewport[dwViewportIndex].fogDesc.dwColor,
			0);
	}
	else
		m_pRenderer->DisableFog();
	
		
	if (m_pViewport[dwViewportIndex].dirLightDesc.bEnable)
	{
		m_pRenderer->EnableDirectionalLight(&m_pViewport[dwViewportIndex].dirLightDesc,0);
	}
	else
		m_pRenderer->DisableDirectionalLight();
	

	m_pRenderer->SetAmbientColor(m_pViewport[dwViewportIndex].dwAmbientColor);
	m_pRenderer->SetEmissiveColor(m_pViewport[dwViewportIndex].dwEmissiveColor);
	m_pRenderer->BeginRender(&m_pViewport[dwViewportIndex].rcClip,dwColor,dwFlag);	


	m_pRenderer->SetViewFrusturm(
		&m_pViewport[dwViewportIndex].ViewVolume,
		&m_pViewport[dwViewportIndex].cameraDesc,
		&m_pViewport[dwViewportIndex].matView,
		&m_pViewport[dwViewportIndex].matProj,
		&m_pViewport[dwViewportIndex].matForBillBoard);
	

lb_return:
	return bResult;
}

void __stdcall CoGeometry::SetShadowUpdatePerFrame(BOOL bSwitch)
{
	m_bShadowUpdatePerFrame = bSwitch;
	
}
void __stdcall CoGeometry::EndRender()
{

	if (!m_pRenderer)
		return;

	DWORD		dwTypeID;
	char*		pItem;
	void*		pFunc;
	DWORD		i;


	if (!m_bGameFrame)
	{
		if (m_bShadowUpdatePerFrame)
			goto lb_skip_render_texture;
	}

	if (!m_dwTotalSpotLightNum)
		goto lb_skip_render_texture;

	
	if (!m_dwShadowFlag)
		goto lb_skip_render_texture;
	
	m_pRenderer->SetRenderTextureMustUpdate(TRUE);
	m_pRenderer->BeginShadowMap();
	
	

	// 그림자 생성....
	DWORD		dwNum;
	dwNum = GetItemNumWithType(m_pRenderQ,TYPEID_RENDER_MODEL);
	for (i=0; i<dwNum; i++)
	{
		DWORD	dwFlag;
		pItem = GetItem(m_pRenderQ,&pFunc,TYPEID_RENDER_MODEL,i);
		RENDER_MODEL_DESC*		pModelDesc = (RENDER_MODEL_DESC*)pItem;

		dwFlag = pModelDesc->dwFlag;
		if (dwFlag & RENDER_TYPE_SEND_SHADOW)
		{
			pModelDesc->pModel->RenderShadow(
				pModelDesc->dwRefIndex,
				pModelDesc->pMatParent,pModelDesc->pMatTransform,pModelDesc->dwFrame,pModelDesc->dwMotionIndex,
				m_spotLightList,m_vvSpot,
				m_dwTotalSpotLightNum,		// 스폿라이트 총개수
				pModelDesc->dwAlpha,
				dwFlag);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//거울면 이미지 생성
	////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_pRenderer->EndShadowMap();
	m_pRenderer->SetRenderTextureMustUpdate(FALSE);

lb_skip_render_texture:

	BOUNDING_SPHERE	senderSphere;

	senderSphere.v3Point.y = -90000.0f;

	float	fDist;
	float	fMaxDist;
	fMaxDist = 900000.0f;

	for (i=0; i<m_dwTotalSpotLightNum; i++)
	{
		if (m_ShadowSender[i].dwPointNum)
		{
			for (DWORD j=0; j<m_ShadowSender[i].dwPointNum; j++)
			{
				fDist = CalcDistance(&m_ShadowSender[i].sphere[j].v3Point,&m_ShadowSender[i].v3LightPoint);

				if (fDist < fMaxDist)
				{
					senderSphere = m_ShadowSender[i].sphere[j];
					fMaxDist = fDist;
					
				}
			}
			m_pRenderer->SetShadowLightSenderPosition(&senderSphere,i);
		}
	}
	// 실제 렌더링..


//	m_pRenderer->BeginRender();	

	while (pItem = GetItemSeq(m_pRenderQ,&dwTypeID,&pFunc))
	{
		__asm
		{
			mov			eax,pItem
			push		eax
			mov			eax,this
			push		eax		
			call		pFunc
		}
	}

	m_pRenderer->EndRender();	
	m_bIsBeginRender = FALSE;

}
void __stdcall CoGeometry::Present(HWND hWnd)
{
	if (m_pRenderer)
		m_pRenderer->Present(hWnd);
}

void __stdcall CoGeometry::SetDrawDebugFlag(DWORD dwFlag)
{
	m_dwDrawDebugFlag = dwFlag;
}
DWORD __stdcall	CoGeometry::GetDrawDebugFlag()
{
	return m_dwDrawDebugFlag;
}

void __stdcall CoGeometry::CalcViewVolume(VIEW_VOLUME* pVV,RECT* pRect,DWORD dwViewportIndex)
{
	BOOL		bResult	=	FALSE;
	VECTOR3		v3PickRayToLeftUp,v3PickRayToLeftDown,v3PickRayToRightUp,v3PickRayToRightDown;
    MATRIX4	matProj,matView;

	GetProjectionMatrix(&matProj,dwViewportIndex);
	GetViewMatrix(&matView,dwViewportIndex);
	VIEWPORT*	pVP = m_pViewport + dwViewportIndex;
		

	PLANE	plane;
	
	VECTOR3	v3Dir;
	VECTOR3_SUB_VECTOR3(&v3Dir,&m_pViewport[dwViewportIndex].cameraDesc.v3To,&m_pViewport[dwViewportIndex].cameraDesc.v3From);
	Normalize(&v3Dir,&v3Dir);
	VECTOR3_MUL_FLOAT(&v3Dir,&v3Dir,80000.0f);
	VECTOR3	v3NewTo;
	VECTOR3_ADD_VECTOR3(&v3NewTo,&m_pViewport[dwViewportIndex].cameraDesc.v3From,&v3Dir);
	plane.D =  -1.0f * DotProduct(&v3Dir,&v3NewTo);
	plane.v3Up = v3Dir;



	POINT	point;
	point.x = pRect->left;
	point.y = pRect->top;
	GetIntersectPointWithScreenCoord(&v3PickRayToLeftUp,&plane,&point,dwViewportIndex);

	point.x = pRect->left;
	point.y = pRect->bottom;
	GetIntersectPointWithScreenCoord(&v3PickRayToLeftDown,&plane,&point,dwViewportIndex);

	point.x = pRect->right;
	point.y = pRect->bottom;
	GetIntersectPointWithScreenCoord(&v3PickRayToRightDown,&plane,&point,dwViewportIndex);

	point.x = pRect->right;
	point.y = pRect->top;
	GetIntersectPointWithScreenCoord(&v3PickRayToRightUp,&plane,&point,dwViewportIndex);

	
	MakeViewVolumeWithSelectRange(pVV,&m_pViewport[dwViewportIndex].cameraDesc.v3From,
		&v3PickRayToLeftUp,
		&v3PickRayToRightUp,
		&v3PickRayToRightDown,
		&v3PickRayToLeftDown);
}


void __stdcall CoGeometry::SetShadowFlag(DWORD dwFlag)
{
	m_dwShadowFlag = dwFlag;

	
	if (m_pRenderer)
		m_pRenderer->SetShadowFlag(dwFlag);

}
DWORD __stdcall	CoGeometry::GetShadowFlag()
{
	return m_dwShadowFlag;
}
void __stdcall CoGeometry::SetLightFlag(DWORD dwFlag)
{
	m_dwLightFlag = dwFlag;
}
DWORD __stdcall CoGeometry::GetLightFlag()
{
	return m_dwLightFlag;
}
DWORD __stdcall CoGeometry::CreateDynamicLight(DWORD dwRS,DWORD dwColor,char* szFileName)
{
	DWORD	dwResult = 0;
	if (!m_pRenderer)
		goto lb_return;

	dwResult = m_pRenderer->CreateDynamicLight(dwRS,dwColor,szFileName);
lb_return:
	return dwResult;

}
BOOL __stdcall CoGeometry::DeleteDynamicLight(DWORD dwIndex)
{
	DWORD	dwResult = 0;
	if (!m_pRenderer)
		goto lb_return;

	dwResult = m_pRenderer->DeleteDynamicLight(dwIndex);
lb_return:
	return dwResult;
}
BOOL __stdcall CoGeometry::RenderSkyMesh(ISkyMesh* pSkyMesh,DWORD dwFlag)
{
	// 스카이매쉬는 가장 먼저 그려져야 하므로 큐에 넣지 않고 바로 그려버린다.
	return pSkyMesh->Render(dwFlag);
}
BOOL __stdcall CoGeometry::RenderModel(I3DModel* pModel,DWORD dwRefIndex,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwFrame,DWORD dwMotionIndex,DWORD dwAlpha,DWORD dwMtlSetIndex,DWORD dwEffectIndex,DWORD dwFlag)
{

	BOOL			bResult = FALSE;
	

	COLLISION_MODEL_DESC*	pColModelDesc;
	MATRIX4*		pMatTemp;


	if (!m_ModelRefManager.GetColModelDesc(&pColModelDesc,dwRefIndex))
		goto lb_return;

	if (dwAlpha >255)
		dwAlpha = 255;

	if (dwFlag & RENDER_TYPE_IGNORE_VIEWVOLUME_CLIP)
		goto lb_push;
	

	if (!ClipRenderObjectWithViewVolume(&m_pViewport[m_dwCurrentViewportIndex].ViewVolume,&pColModelDesc->colMeshModel,dwMtlSetIndex))
		goto lb_return;
		
lb_push:
/*	ppMatEntry = &pParentMat;

	for (i=0; i<2; i++)
	{
		if (ppMatEntry[i])
		{
			pMatTemp = (MATRIX4*)m_MatrixPool.Alloc(1);
			
			*pMatTemp = *ppMatEntry[i];
			ppMatEntry[i] = pMatTemp;
		}
	}*/
	if (pTransformMat)
	{
		pMatTemp = (MATRIX4*)m_MatrixPool.Alloc(1);
		*pMatTemp = *pTransformMat;
		pTransformMat = pMatTemp;
	}
	/*
	if (pParentMat)
	{
		pMatTemp = (MATRIX4*)m_MatrixPool.Alloc(1);
		*pMatTemp = *pParentMat;
		pParentMat = pMatTemp;
	}
	*/
	//	RENDER_MODEL_DESC*	pItem;
//	pItem = (RENDER_MODEL_DESC*)&pModel;
	RENDER_MODEL_DESC	itemDesc;
	itemDesc.pModel = (CoModel*)pModel;
	itemDesc.dwRefIndex = dwRefIndex;
	itemDesc.pMatParent = pParentMat;
	itemDesc.pMatTransform = pTransformMat;
	itemDesc.dwFrame = dwFrame;
	itemDesc.dwMotionIndex = dwMotionIndex;
	itemDesc.dwAlpha = dwAlpha;
	itemDesc.dwMtlSetIndex = dwMtlSetIndex;
	itemDesc.dwEffectIndex = dwEffectIndex;
	itemDesc.dwFlag = dwFlag;
	

	bResult = PushItem(m_pRenderQ,TYPEID_RENDER_MODEL,(char*)&itemDesc);
lb_return:
	return bResult;

}

BOOL __stdcall CoGeometry::RenderStaticModel(I3DStaticModel* pModel,DWORD dwObjIndex,DWORD dwAlpha,DWORD dwMtlSetIndex,DWORD dwFlag)
{
	if (dwAlpha >255)
		dwAlpha = 255;

	RENDER_STATIC_MODEL_DESC	itemDesc;
	itemDesc.pStaticModel = (CoStaticModel*)pModel;
	itemDesc.dwObjIndex = dwObjIndex;
	itemDesc.dwAlpha = dwAlpha;
	itemDesc.dwMtlSetIndex = dwMtlSetIndex;
	itemDesc.dwFlag = dwFlag;

//	RENDER_STATIC_MODEL_DESC*	pItem = (RENDER_STATIC_MODEL_DESC*)&pModel;
	
	return PushItem(m_pRenderQ,TYPEID_RENDER_STATIC_MODEL,(char*)&itemDesc);

}
BOOL __stdcall CoGeometry::RenderHField(IHeightField* pHField,DWORD dwObjIndex,DWORD dwAlpha,DWORD dwFlag)
{
	BOOL	bResult = FALSE;

	if (!((CoHeightField*)pHField)->GetIDIHFieldManager())
		goto lb_return;

	RENDER_HFIELD_DESC	itemDesc;

	itemDesc.pHField = (CoHeightField*)pHField;
	itemDesc.dwObjIndex = dwObjIndex;
	itemDesc.dwAlpha = dwAlpha;
	itemDesc.dwFlag = dwFlag;
	
//	RENDER_HFIELD_DESC*	pItem = (RENDER_HFIELD_DESC*)&pHField;

	COLLISION_MESH_OBJECT_DESC*		pColMeshDesc;
	
	if (dwObjIndex >= pHField->GetObjectNum())
		goto lb_return;

	
	// 오브젝트가 뷰볼륨안에 들어가지 않는 경우 클립
	pColMeshDesc = pHField->GetCollisionMeshObjectDesc(dwObjIndex);
	if (!ClipRenderObjectWithViewVolume(&m_pViewport[m_dwCurrentViewportIndex].ViewVolume,pColMeshDesc,0))
		goto lb_return;

	// 디테일 조정..
//	dwDetail = ((CoHeightField*)pHField)->CalcDetail(&pColMeshDesc->boundingSphere.v3Point,pColMeshDesc->boundingSphere.fRs,&m_cameraDesc.v3From,m_ViewVolume.fFar);
//	((CoHeightField*)pHField)->SetDetailLevel(dwObjIndex,dwDetail);

//	((CoHeightField*)pHField)->CalcDetail(dwObjIndex,&m_matViewProj);
	((CoHeightField*)pHField)->CalcDetail(dwObjIndex,&m_pViewport[m_dwCurrentViewportIndex].cameraDesc.v3From);

	PushItem(m_pRenderQ,TYPEID_RENDER_HFIELD,(char*)&itemDesc);
	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL __stdcall CoGeometry::RenderSprite(IDISpriteObject* pSprite,VECTOR2* pv2Scaling,float fRot,VECTOR2* pv2Trans,RECT* pRect,DWORD dwColor,int iZOrder,DWORD dwFlag)
{
	return m_pRenderer->RenderSprite(pSprite,pv2Scaling,fRot,pv2Trans,pRect,dwColor,iZOrder,dwFlag);
}
BOOL __stdcall CoGeometry::RenderFont(IDIFontObject* pFont,TCHAR* str,DWORD dwLen,RECT* pRect,DWORD dwColor,CHAR_CODE_TYPE type,int iZOrder,DWORD dwFlag)
{
	return m_pRenderer->RenderFont(pFont,str,dwLen,pRect,dwColor,type,iZOrder,dwFlag);
}
BOOL __stdcall CoGeometry::RenderLight(LIGHT_DESC* pLightDesc,DWORD dwAlphaFlag,DWORD dwRenderFlag)
{
	RenderSphere(&pLightDesc->v3Point,pLightDesc->fRs,pLightDesc->dwDiffuse);
	return TRUE;
}
void __stdcall CoGeometry::RenderBox(VECTOR3* pv3Oct,DWORD dwColor)
{
	m_pRenderer->RenderBox(pv3Oct,dwColor);
}

void __stdcall CoGeometry::RenderSphere(VECTOR3* pv3Point,float fRs,DWORD dwColor)
{		  
	WORD wClipWidth  = m_pViewport[m_dwCurrentViewportIndex].wClipWidth / 2;
    WORD wClipHeight = m_pViewport[m_dwCurrentViewportIndex].wClipHeight / 2;

	MATRIX4			matPos,matWorld,matViewProj;
	//,matView;
	

	VECTOR3			v3[2];
	VECTOR4			v4[2];
	VECTOR4			v4Screen[2];
	DWORD			i;
	

	memset(v3,0,sizeof(VECTOR3)*2);
	v3[1].x = fRs;

	

	// 월드 매트릭스 
	TranslateMatrix(&matPos,pv3Point);
	MatrixMultiply2(&matWorld,GetBillBoardMatrix(),&matPos);

	TransformVector3_VPTR1(v3,&matWorld,2);

	// 뷰매트릭스 
	MatrixMultiply2(&matViewProj,&m_pViewport[m_dwCurrentViewportIndex].matView,&m_pViewport[m_dwCurrentViewportIndex].matProj);
	
	TransformV3TOV4(v4,v3,&matViewProj,2);

	for (i=0; i<2; i++)
	{
		v4Screen[i].x = ( 1.0f + (v4[i].x / v4[i].w) ) * wClipWidth;
		v4Screen[i].y = ( 1.0f - (v4[i].y / v4[i].w) ) * wClipHeight;
		v4Screen[i].z = v4[i].z / v4[i].w;
		v4Screen[i].w = 1.0f / v4[i].w;
	}


	fRs = sqrtf(
		(v4Screen[1].x - v4Screen[0].x) * (v4Screen[1].x - v4Screen[0].x) +
		 (v4Screen[1].y - v4Screen[0].y) * (v4Screen[1].y - v4Screen[0].y));
	
	m_pRenderer->RenderPoint(pv3Point,0xffffffff);
	v4Screen[0].x += (float)m_pViewport[m_dwCurrentViewportIndex].rcClip.left;
	v4Screen[0].y += (float)m_pViewport[m_dwCurrentViewportIndex].rcClip.top;
	
	m_pRenderer->RenderCircle((VECTOR2*)&v4Screen[0],fRs,dwColor);
	

}/*

HRESULT TransformVertices( LPDIRECT3DDEVICE3 pd3dDevice,
                           D3DTLVERTEX* pvVertices, DWORD dwNumVertices )
{
    // Get a ptr to the current viewport
    LPDIRECT3DVIEWPORT3 pvViewport;
    pd3dDevice->GetCurrentViewport( &pvViewport );

    // Get the width and height of the viewport. This is needed to scale the
    // transformed vertices to fit the render window.
    D3DVIEWPORT2 vp;
    vp.dwSize = sizeof(D3DVIEWPORT2);
    pvViewport->GetViewport2( &vp );
    DWORD dwClipWidth  = vp.dwWidth/2;
    DWORD dwClipHeight = vp.dwHeight/2;

    // Get the current matrix set. This is needed for the transformation.
    D3DMATRIX matWorld, matView, matProj;
    pd3dDevice->GetTransform( D3DTRANSFORMSTATE_WORLD,      &matWorld );
    pd3dDevice->GetTransform( D3DTRANSFORMSTATE_VIEW,       &matView );
    pd3dDevice->GetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProj );
    D3DMATRIX matSet = matWorld * matView * matProj;

    // Transform each vertex through the current matrix set
    for( DWORD i=0; i<dwNumVertices; i++ )
    {
        // Get the untransformed vertex position
        FLOAT x = pvVertices[i].sx;
        FLOAT y = pvVertices[i].sy;
        FLOAT z = pvVertices[i].sz;

        // Transform it through the current matrix set
        FLOAT xp = matSet._11*x + matSet._21*y + matSet._31*z + matSet._41;
        FLOAT yp = matSet._12*x + matSet._22*y + matSet._32*z + matSet._42;
        FLOAT zp = matSet._13*x + matSet._23*y + matSet._33*z + matSet._43;
        FLOAT wp = matSet._14*x + matSet._24*y + matSet._34*z + matSet._44;

        // Finally, scale the vertices to screen coords. This step first 
        // "flattens" the coordinates from 3D space to 2D device coordinates,
        // by dividing each coordinate by the wp value. Then, the x- and
        // y-components are transformed from device coords to screen coords.
        // Note 1: device coords range from -1 to +1 in the viewport.
        // Note 2: the sz-coordinate will be used in the z-buffer.
        pvVertices[i].sx  = ( 1.0f + (xp/wp) ) * dwClipWidth;
        pvVertices[i].sy  = ( 1.0f - (yp/wp) ) * dwClipHeight;
        pvVertices[i].sz  = zp / wp;
        pvVertices[i].rhw = wp;
    }

    return S_OK;
}*/


BOOL __stdcall CoGeometry::Initialize(I4DyuchiGXRenderer* pRenderer,I4DyuchiFileStorage* pFileStorage,DWORD dwViewportNum,DWORD dwMaxFileNum,DWORD dwMaxBucketNum,ErrorHandleProc pErrorHandleFunc)
{
	BOOL			bResult = FALSE;

	g_pErrorHandleFunc = pErrorHandleFunc;

	if (!g_pErrorHandleFunc)
		g_pErrorHandleFunc = DefaultErrorHandleProc;

	SetIdentityMatrix(&m_matIdentity);


	m_dwViewportNum = dwViewportNum;

	if (!m_dwViewportNum || m_dwViewportNum > MAX_VIEWPORT_NUM)
		m_dwViewportNum = MAX_VIEWPORT_NUM;
		
	m_pViewport = new VIEWPORT[m_dwViewportNum];
	memset(m_pViewport,0,sizeof(VIEWPORT)*m_dwViewportNum);

	m_bViewVolumeClip = TRUE;

	m_pRenderer = pRenderer;
	m_pFileStorage = pFileStorage;
	m_pFileStorage->AddRef();
	g_pFileStorage = m_pFileStorage;

	if (pRenderer)
	{
		pRenderer->AddRef();
		pRenderer->GetClientRect(&m_rcWindow,&m_wScreenWidth,&m_wScreenHeight);
		
		for (DWORD i=0; i<m_dwViewportNum; i++)
		{
			VECTOR3	v3Eye;
			v3Eye.x = 0.0f;
			v3Eye.y = -1000.0f;
			v3Eye.z = 0.0f;

			ResetCamera(&v3Eye,DEFAULT_NEAR,DEFAULT_FAR,DEFAULT_FOV,i);

			SetIdentityMatrix(&m_pViewport[i].matForBillBoard);
			m_pViewport[i].dwAmbientColor = DEFAULT_AMBIENT_COLOR;
			SetViewport(NULL,i);
		}
	}
	else
	{
		m_wScreenWidth = 640;
		m_wScreenHeight = 480;
	}



	

	m_pFileNameHash = VBHCreate();
	VBHInitialize(m_pFileNameHash,dwMaxBucketNum,MAX_NAME_LEN,dwMaxFileNum);

	m_pFileList = new FILE_HANDLE[dwMaxFileNum];
	memset(m_pFileList,0,sizeof(FILE_HANDLE)*dwMaxFileNum);
	m_dwMaxFileNum = dwMaxFileNum;

	

	m_pIC = ICCreate();

	ICInitialize(m_pIC,dwMaxFileNum-1);
	m_dwMaxFileNum--;

	m_pRenderQ = CreateItemQueue();
	BeginInitialize(m_pRenderQ,3);
	
	SetType(m_pRenderQ,TYPEID_RENDER_MODEL,sizeof(RENDER_MODEL_DESC),Render_CoModel,MAX_RENDER_MODEL_NUM);
	SetType(m_pRenderQ,TYPEID_RENDER_STATIC_MODEL,sizeof(RENDER_STATIC_MODEL_DESC),Render_CoStaticModel,MAX_RENDER_STATICMODEL_OBJ_NUM);
	SetType(m_pRenderQ,TYPEID_RENDER_HFIELD,sizeof(RENDER_HFIELD_DESC),Render_CoHeightField,MAX_RENDER_HFIELD_OBJ_NUM);

	EndInitialize(m_pRenderQ);
	m_LightIndexPool.Initialize(sizeof(LIGHT_INDEX_DESC),MAX_LIGHT_INDEX_NUM_IN_POOL);
	m_MatrixPool.Initialize(sizeof(MATRIX4),MAX_MATRIX_NUM_IN_POOL);

	m_ModelRefManager.Initialize(m_pRenderer,MAX_MODEL_REF_INDEX_NUM);
	bResult = TRUE;


lb_return:
	return bResult;

}
ULONG __stdcall CoGeometry::CreateStaticModel(I3DStaticModel** ppStaticModel,DWORD dwMaxObjectNum,DWORD dwMaxMtlNum,DWORD dwFlag)
{
	CoStaticModel* pStaticModel = new CoStaticModel;
	pStaticModel->Initialize(m_pRenderer,dwMaxObjectNum,dwMaxMtlNum);
	pStaticModel->SetGeometry(this);
	pStaticModel->AddRef();

	*ppStaticModel = pStaticModel;

	DWORD	dwItemIndex;
	dwItemIndex = ICAllocIndex(m_pIC);
#ifdef _DEBUG
	char txt[512];
	if (!dwItemIndex)
	{
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		wsprintf(txt,"Fail to CreateStaticModel, Fail to ICAllocIndex\n");
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif
	m_pFileList[dwItemIndex].pSearchHandle = VBHInsert(m_pFileNameHash,dwItemIndex,"STM",lstrlen("STM"));
	m_pFileList[dwItemIndex].pFileItem = (void*)pStaticModel;
	m_pFileList[dwItemIndex].type = FILE_ITEM_TYPE_STATIC_MODEL;
	pStaticModel->SetIndexInGeometry(dwItemIndex);

	m_dwFileNum++;
	
	return 0;
}
BOOL __stdcall CoGeometry::DisableUnloadPreLoadedResource(char* szFileName)
{
	BOOL	bResult = FALSE;

	DWORD	dwIndexOut;
	
	char		szSearchFileName[256];
	GetNameRemovePath(szSearchFileName,szFileName);

	DWORD			dwSearchNameLen = lstrlen(szSearchFileName);
	CharToSmallASCII(szSearchFileName,szSearchFileName,dwSearchNameLen);

	if (!VBHSelect(m_pFileNameHash,&dwIndexOut,1,szSearchFileName,dwSearchNameLen))
		goto lb_return;

	bResult = DisableUnloadPreLoadedResource(dwIndexOut);

lb_return:
	return bResult;
}


BOOL __stdcall CoGeometry::EnableUnloadPreLoadedResource(char* szFileName)
{
	BOOL	bResult = FALSE;

	DWORD	dwIndexOut;
	
	char		szSearchFileName[256];
	GetNameRemovePath(szSearchFileName,szFileName);

	DWORD			dwSearchNameLen = lstrlen(szSearchFileName);
	CharToSmallASCII(szSearchFileName,szSearchFileName,dwSearchNameLen);

	if (!VBHSelect(m_pFileNameHash,&dwIndexOut,1,szSearchFileName,dwSearchNameLen))
		goto lb_return;

	bResult = EnableUnloadPreLoadedResource(dwIndexOut);
lb_return:
	return bResult;
}
BOOL CoGeometry::DisableUnloadPreLoadedResource(DWORD dwIndex)
{
	BOOL	bResult = FALSE;
	if (m_pFileList[dwIndex].dwFlag != FILE_HANDLE_FLAG_PRELOADED)
		goto lb_return;

	m_pFileList[dwIndex].dwFlag = FILE_HANDLE_FLAG_PRELOADED_BUT_DISABLE_UNLOAD;
	
	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL CoGeometry::EnableUnloadPreLoadedResource(DWORD dwIndex)
{
	BOOL	bResult = FALSE;
	if (m_pFileList[dwIndex].dwFlag != FILE_HANDLE_FLAG_PRELOADED_BUT_DISABLE_UNLOAD)
		goto lb_return;

	m_pFileList[dwIndex].dwFlag = FILE_HANDLE_FLAG_PRELOADED;

	bResult = TRUE;
lb_return:
	return bResult;
}

BOOL __stdcall CoGeometry::IsLoadedModel(I3DModel** ppModel,char* szFileName)
{
	BOOL	bResult = FALSE;

	DWORD	dwIndexOut;
	*ppModel = NULL;

	char		szSearchFileName[256];
	GetNameRemovePath(szSearchFileName,szFileName);

	DWORD			dwSearchNameLen = lstrlen(szSearchFileName);
	CharToSmallASCII(szSearchFileName,szSearchFileName,dwSearchNameLen);

	if (VBHSelect(m_pFileNameHash,&dwIndexOut,1,szSearchFileName,dwSearchNameLen))
	{
		*ppModel = (I3DModel*)m_pFileList[dwIndexOut].pFileItem;
		bResult = TRUE;
	}
	
	return bResult;
}


ULONG __stdcall CoGeometry::LoadModel(I3DModel** ppModel,char* szFileName,DWORD dwFlag)
{
	
	ULONG		ulResult = 0xffffffff;

	CoModel*	pModel = NULL;

	DWORD		dwIndexOut;
	DWORD		dwItemIndex;

	if (m_dwFileNum >= m_dwMaxFileNum)
		goto lb_return;

	char		szSearchFileName[256];
	GetNameRemovePath(szSearchFileName,szFileName);

	DWORD	dwSearchNameLen;
	dwSearchNameLen = lstrlen(szSearchFileName);
	CharToSmallASCII(szSearchFileName,szSearchFileName,dwSearchNameLen);
#ifdef _DEBUG
	char txt[512];
#endif

	if (VBHSelect(m_pFileNameHash,&dwIndexOut,1,szSearchFileName,dwSearchNameLen))
	{
		pModel = (CoModel*)m_pFileList[dwIndexOut].pFileItem;
#ifdef _DEBUG
		wsprintf(txt,"Load from Memory, %s\n",szFileName);
		OutputDebugString(txt);
#endif
		goto lb_success;
	}
	pModel = new CoModel;
	pModel->SetGeometry(this);
	if (!pModel->ReadFile(szFileName,dwFlag))
	{
		delete pModel;
		pModel = NULL;
		goto lb_return;
	}

	dwItemIndex = ICAllocIndex(m_pIC);

#ifdef _DEBUG
	if (!dwItemIndex)
	{
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		wsprintf(txt,"Fail to LoadModel %s,Fail to ICAllocIndex\n",szFileName);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif

	m_pFileList[dwItemIndex].pSearchHandle = VBHInsert(m_pFileNameHash,dwItemIndex,szSearchFileName,dwSearchNameLen);
	m_pFileList[dwItemIndex].pFileItem = (void*)pModel;
	m_pFileList[dwItemIndex].type = FILE_ITEM_TYPE_MODEL;
	
	
	pModel->SetIndexInGeometry(dwItemIndex);

	m_dwFileNum++;

lb_success:
	pModel->AddRef();
	ulResult = 0;

	
lb_return:
	*ppModel = pModel;
	return ulResult;

}

CMotion* CoGeometry::LoadMotion(char* szFileName)
{
	CMotion*	pMotion;

	DWORD	dwIndexOut;
	DWORD	dwItemIndex;

	char		szSearchFileName[256];
	GetNameRemovePath(szSearchFileName,szFileName);

	DWORD dwSearchNameLen;
	dwSearchNameLen = lstrlen(szSearchFileName);
	CharToSmallASCII(szSearchFileName,szSearchFileName,dwSearchNameLen);

	if (VBHSelect(m_pFileNameHash,&dwIndexOut,1,szSearchFileName,dwSearchNameLen))
	{
		pMotion = (CMotion*)m_pFileList[dwIndexOut].pFileItem;
		/*
#ifdef _DEBUG
		char txt[512];
		wsprintf(txt,"Load from Memory, %s\n",szFileName);
		OutputDebugString(txt);
#endif
		*/
		goto	lb_success;
	}
	pMotion = new CMotion;
	if (!pMotion->ReadFile(szFileName))
	{
		delete pMotion;
		pMotion = NULL;
		goto lb_return;
	}
	dwItemIndex = ICAllocIndex(m_pIC);
		
	m_pFileList[dwItemIndex].pSearchHandle = VBHInsert(m_pFileNameHash,dwItemIndex,szSearchFileName,dwSearchNameLen);
	m_pFileList[dwItemIndex].pFileItem = (void*)pMotion;
	m_pFileList[dwItemIndex].type = FILE_ITEM_TYPE_MOTION;

	pMotion->SetGeometry(this);
	pMotion->SetIndexInGeometry(dwItemIndex);
	
	m_dwFileNum++;

lb_success:	
	pMotion->AddRef();
		
lb_return:
	return pMotion;
}

BOOL CoGeometry::IsLoadedMotion(CMotion** ppMotion,char* szFileName)
{
	BOOL	bResult = FALSE;

	DWORD	dwIndexOut;

	char		szSearchFileName[256];
	GetNameRemovePath(szSearchFileName,szFileName);

	DWORD			dwSearchNameLen = lstrlen(szSearchFileName);
	CharToSmallASCII(szSearchFileName,szSearchFileName,dwSearchNameLen);

	if (!VBHSelect(m_pFileNameHash,&dwIndexOut,1,szSearchFileName,dwSearchNameLen))
		goto lb_return;

	*ppMotion = (CMotion*)m_pFileList[dwIndexOut].pFileItem;
	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CoGeometry::IsLoadedMaterial(CMaterialLib** ppMtlLib,char* szFileName)
{
	BOOL	bResult = FALSE;

	DWORD	dwIndexOut;
	
	char		szSearchFileName[256];
	GetNameRemovePath(szSearchFileName,szFileName);

	DWORD			dwSearchNameLen = lstrlen(szSearchFileName);
	CharToSmallASCII(szSearchFileName,szSearchFileName,dwSearchNameLen);

	if (!VBHSelect(m_pFileNameHash,&dwIndexOut,1,szSearchFileName,dwSearchNameLen))
		goto lb_return;

	*ppMtlLib = (CMaterialLib*)m_pFileList[dwIndexOut].pFileItem;
	bResult = TRUE;

lb_return:
	return bResult;
}


BOOL CoGeometry::DeleteFileItem(DWORD dwIndex)
{
	BOOL bResult;
	m_dwFileNum--;
	
	ICFreeIndex(m_pIC,dwIndex);
	bResult = VBHDelete(m_pFileNameHash,m_pFileList[dwIndex].pSearchHandle);
	m_pFileList[dwIndex].pFileItem = NULL;
	m_pFileList[dwIndex].pSearchHandle = NULL;
	m_pFileList[dwIndex].dwFlag = 0;
	
	return bResult;
}
BOOL CoGeometry::UnloadPreLoadedItem(DWORD dwIndex)
{
	BOOL bResult = FALSE;
	if (!(m_pFileList[dwIndex].dwFlag & FILE_HANDLE_FLAG_PRELOADED))
		goto lb_return;

	if (m_pFileList[dwIndex].pFileItem)
		((IUnknown*)m_pFileList[dwIndex].pFileItem)->Release();
			
	m_pFileList[dwIndex].dwFlag = 0;
	bResult = TRUE;
lb_return:
	return bResult;
}
CMaterialLib* CoGeometry::LoadMaterial(char* szFileName)
{
	DWORD				dwResult = 0xffffffff;

	CMaterialLib*		pMtlLib;

	DWORD	dwIndexOut;
	DWORD	dwItemIndex;
	
	char		szSearchFileName[256];
	GetNameRemovePath(szSearchFileName,szFileName);

	DWORD			dwSearchNameLen = lstrlen(szSearchFileName);
	CharToSmallASCII(szSearchFileName,szSearchFileName,dwSearchNameLen);

	if (VBHSelect(m_pFileNameHash,&dwIndexOut,1,szSearchFileName,dwSearchNameLen))
	{
		pMtlLib = (CMaterialLib*)m_pFileList[dwIndexOut].pFileItem;
		goto	lb_success;
	}
	pMtlLib = new CMaterialLib;
	if (!pMtlLib->ReadFile(szFileName))
	{
		delete pMtlLib;
		pMtlLib = NULL;
		goto lb_return;
	}
	dwItemIndex = ICAllocIndex(m_pIC);
		
	m_pFileList[dwItemIndex].pSearchHandle = VBHInsert(m_pFileNameHash,dwItemIndex,szSearchFileName,dwSearchNameLen);
	m_pFileList[dwItemIndex].pFileItem = (void*)pMtlLib;
	m_pFileList[dwItemIndex].type = FILE_ITEM_TYPE_MTL;

	pMtlLib->SetGeometry(this);
	pMtlLib->SetIndexInGeometry(dwItemIndex);
	pMtlLib->Initialize(m_pRenderer);
	
	m_dwFileNum++;

lb_success:	
	pMtlLib->AddRef();

lb_return:
	return pMtlLib;
}	
ULONG __stdcall	CoGeometry::CreateSkyMesh(ISkyMesh** ppSkyMesh,char* szFileName,DWORD dwFlag)
{
	CoSkyMesh*	pSkyMesh;

	ULONG		ulResult = 0xffffffff;
	I3DModel*	pModel;
		
	if (LoadModel((I3DModel**)&pModel,szFileName,0) == 0xffffffff)
		goto lb_return;

	pModel->Initialize(m_pRenderer,0);
	pSkyMesh = new CoSkyMesh;
	pSkyMesh->Initialize(this,pModel,dwFlag);
	pSkyMesh->AddRef();

	pModel->Release();
	*ppSkyMesh = pSkyMesh;
	
	ulResult++;

lb_return:
	return ulResult;
}

ULONG __stdcall CoGeometry::CreateHeightField(IHeightField** ppHeightField,DWORD dwFlag)
{
	ULONG	ulResult = 0xffffffff;

	CoHeightField* pHField = new CoHeightField;
	pHField->AddRef();
	pHField->SetGeometry(this);
		
	if (!pHField->Initialize(this,m_pRenderer))
	{
		delete pHField;
		goto lb_return;
	}

	*ppHeightField = pHField;

	DWORD	dwItemIndex;
	dwItemIndex = ICAllocIndex(m_pIC);
#ifdef _DEBUG
	char txt[512];
	if (!dwItemIndex)
	{
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		wsprintf(txt,"Fail to CreateHeightField, Fail to ICAllocIndex\n");
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif
	m_pFileList[dwItemIndex].pSearchHandle = VBHInsert(m_pFileNameHash,dwItemIndex,"HFL",lstrlen("HFL"));
	m_pFileList[dwItemIndex].pFileItem = (void*)pHField;
	m_pFileList[dwItemIndex].type = FILE_ITEM_TYPE_HFIELD;
	pHField->SetIndexInGeometry(dwItemIndex);
	m_dwFileNum++;

	ulResult = 0;
lb_return:
	return ulResult;
}

BOOL __stdcall CoGeometry::AddMirror(LIGHT_DESC* pMirror,DWORD dwFlag)
{
	return AddSpotLight(pMirror,SPOT_LIGHT_TYPE_MIRROR);
	
}
DWORD CoGeometry::AddSpotLight(LIGHT_DESC* pLight,SPOT_LIGHT_TYPE  type)
{

	BOOL	bOrtho;
	if (m_dwSpotLightNum[type] >= MAX_SHADOW_LIGHT_NUM_PER_SCENE)
		goto lb_return;

	bOrtho = FALSE;
	pLight->type = type;
	m_spotLightList[m_dwTotalSpotLightNum] = *pLight;
	if (pLight->fFar <= 0.0f || pLight->fFar > 80000.0f)
		pLight->fFar = pLight->fRs;

	if (pLight->fNear <= 0.0f || pLight->fNear > 80000.0f)
		pLight->fNear = 10.0f;

	

//	m_spotLightList[m_dwTotalSpotLightNum].fFar = 10000.0f;
	m_spotLightList[m_dwTotalSpotLightNum].fNear = 10.0f;
	
	if (SPOT_LIGHT_TYPE_SHADOW == type)
	{

		MakeViewVolume(&m_vvSpot[m_dwTotalSpotLightNum],
			&m_spotLightList[m_dwTotalSpotLightNum].v3Point,
			&m_spotLightList[m_dwTotalSpotLightNum].v3To,
			&m_spotLightList[m_dwTotalSpotLightNum].v3Up,
			m_spotLightList[m_dwTotalSpotLightNum].fFov,
			1.0f,
			m_spotLightList[m_dwTotalSpotLightNum].fRs);
	}
	else if ((SPOT_LIGHT_TYPE_PRJIMAGE == type) || (SPOT_LIGHT_TYPE_MIRROR == type))
	{

		bOrtho = TRUE;
		MakeViewVolumeWithOrtho(&m_vvSpot[m_dwTotalSpotLightNum],
			&m_spotLightList[m_dwTotalSpotLightNum].v3Point,
			&m_spotLightList[m_dwTotalSpotLightNum].v3To,
			&m_spotLightList[m_dwTotalSpotLightNum].v3Up,
			m_spotLightList[m_dwTotalSpotLightNum].fWidth,
			1.0f,
			m_spotLightList[m_dwTotalSpotLightNum].fRs);
	}

	BOUNDING_SPHERE	lightSphere;
	lightSphere.v3Point = pLight->v3Point;
	lightSphere.fRs = pLight->fRs;

	SetShadowLightSenderSphere(&lightSphere,m_dwTotalSpotLightNum);
	m_pRenderer->SetSpotLightDesc(
			&pLight->v3Point,&pLight->v3To,&pLight->v3Up,
			pLight->fFov,
			pLight->fNear,pLight->fFar,
			pLight->fWidth,
			bOrtho,
			pLight->pMtlHandle,
			pLight->dwProjTexIndex,
			m_dwTotalSpotLightNum,
			type);


	m_dwSpotLightNum[type]++;
	m_dwTotalSpotLightNum++;
	
lb_return:
	return m_dwTotalSpotLightNum;
}
void __stdcall CoGeometry::EnableViewVolumeClip()
{
	m_bViewVolumeClip = TRUE;
}
void __stdcall CoGeometry::DisableViewVolumeClip()
{
	m_bViewVolumeClip = FALSE;
}
void __stdcall CoGeometry::GetViewVolume(VIEW_VOLUME* pVolume,DWORD dwViewportIndex)
{
	*pVolume = m_pViewport[dwViewportIndex].ViewVolume;
}
BOOL __stdcall CoGeometry::MoveCamera(VECTOR3* pv3Trans,DWORD dwViewportIndex)
{
	
	VECTOR3_ADDEQU_VECTOR3(&m_pViewport[dwViewportIndex].cameraDesc.v3From,pv3Trans);
	VECTOR3_ADDEQU_VECTOR3(&m_pViewport[dwViewportIndex].cameraDesc.v3To,pv3Trans);

	SetCameraDesc(&m_pViewport[dwViewportIndex].cameraDesc,dwViewportIndex);
	

	return TRUE;
}
BOOL __stdcall CoGeometry::ResetCamera(VECTOR3* pv3Eye,float fNear,float fFar,float fFovX,DWORD dwViewportIndex)
{
	BOOL	bResult = FALSE;

	if (dwViewportIndex >= m_dwViewportNum)
		goto lb_return;

	m_pViewport[dwViewportIndex].cameraDesc.v3From = *pv3Eye;
	m_pViewport[dwViewportIndex].cameraDesc.v3To = *pv3Eye;
	m_pViewport[dwViewportIndex].cameraDesc.v3To.z += 1000.0f;
		
	m_pViewport[dwViewportIndex].cameraDesc.v3Up.x = 0.0f;
	m_pViewport[dwViewportIndex].cameraDesc.v3Up.y = 1.0f;
	m_pViewport[dwViewportIndex].cameraDesc.v3Up.z = 0.0f;
	m_pViewport[dwViewportIndex].cameraDesc.fNear = fNear;
	m_pViewport[dwViewportIndex].cameraDesc.fFar = fFar;
	m_pViewport[dwViewportIndex].cameraDesc.fFovX = fFovX;
	m_pViewport[dwViewportIndex].cameraDesc.fXRot = 0.0f;
	m_pViewport[dwViewportIndex].cameraDesc.fYRot = 0.0f;
	m_pViewport[dwViewportIndex].cameraDesc.fZRot = 0.0f;
	
	SetCameraDesc(&m_pViewport[dwViewportIndex].cameraDesc,dwViewportIndex);
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CoGeometry::SetCameraPos(VECTOR3* pv3Eye,DWORD dwViewportIndex)
{
	BOOL	bResult = FALSE;

	VECTOR3	v3Offset;

	if (dwViewportIndex >= m_dwViewportNum)
		goto lb_return;

	VECTOR3_SUB_VECTOR3(&v3Offset,pv3Eye,&m_pViewport[dwViewportIndex].cameraDesc.v3From);
	VECTOR3_ADDEQU_VECTOR3(&m_pViewport[dwViewportIndex].cameraDesc.v3From,&v3Offset);
	VECTOR3_ADDEQU_VECTOR3(&m_pViewport[dwViewportIndex].cameraDesc.v3To,&v3Offset);
	
	SetCameraDesc(&m_pViewport[dwViewportIndex].cameraDesc,dwViewportIndex);
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CoGeometry::SetCameraViewDistance(float fNear,float fFar,DWORD dwViewportIndex)
{
	BOOL	bResult = FALSE;


	if (dwViewportIndex >= m_dwViewportNum)
		goto lb_return;

	m_pViewport[dwViewportIndex].cameraDesc.fFar = fFar;
	m_pViewport[dwViewportIndex].cameraDesc.fNear = fNear;

	SetCameraDesc(&m_pViewport[dwViewportIndex].cameraDesc,dwViewportIndex);
	bResult = TRUE;

lb_return:
	return bResult;

}
void __stdcall CoGeometry::UpdateWindowSize()
{
	
	if (m_pRenderer)
	{
		m_pRenderer->UpdateWindowSize();
		m_pRenderer->GetClientRect(&m_rcWindow,&m_wScreenWidth,&m_wScreenHeight);
		
		for (DWORD i=0; i<m_dwViewportNum; i++)
		{
			if (m_pViewport[i].bFullScreen)
				SetViewport(NULL,i);
		}
	}
//	SetCamera(&m_cameraDesc.v3From,&m_cameraDesc.v3To,&m_cameraDesc.v3Up,m_cameraDesc.fNear,m_cameraDesc.fFar);
}
void __stdcall CoGeometry::SetFog(FOG_DESC* pFogDesc,DWORD dwViewportIndex)
{
	m_pViewport[dwViewportIndex].fogDesc.fDensity = pFogDesc->fDensity;
	m_pViewport[dwViewportIndex].fogDesc.fStart = pFogDesc->fStart;
	m_pViewport[dwViewportIndex].fogDesc.fEnd = pFogDesc->fEnd;
	m_pViewport[dwViewportIndex].fogDesc.dwColor = pFogDesc->dwColor;
}
void __stdcall CoGeometry::EnableFog(DWORD dwViewportIndex)
{
	m_pViewport[dwViewportIndex].fogDesc.bEnable = TRUE;
}
void __stdcall CoGeometry::DisableFog(DWORD dwViewportIndex)
{
	m_pViewport[dwViewportIndex].fogDesc.bEnable = FALSE;
}

void __stdcall CoGeometry::SetDirectionalLight(DIRECTIONAL_LIGHT_DESC* pLight,DWORD dwViewportIndex)
{
	m_pViewport[dwViewportIndex].dirLightDesc.dwAmbient = pLight->dwAmbient;
	m_pViewport[dwViewportIndex].dirLightDesc.dwDiffuse = pLight->dwDiffuse;
	m_pViewport[dwViewportIndex].dirLightDesc.dwSpecular = pLight->dwSpecular;
	m_pViewport[dwViewportIndex].dirLightDesc.v3Dir = pLight->v3Dir;
}
void __stdcall CoGeometry::GetDirectionalLight(DIRECTIONAL_LIGHT_DESC* pLight,DWORD dwViewportIndex)
{
	*pLight = m_pViewport[dwViewportIndex].dirLightDesc;
}
void __stdcall CoGeometry::EnableDirectionalLight(DWORD dwViewportIndex)
{
	m_pViewport[dwViewportIndex].dirLightDesc.bEnable = TRUE;
}
void __stdcall CoGeometry::DisableDirectionalLight(DWORD dwViewportIndex)
{
	m_pViewport[dwViewportIndex].dirLightDesc.bEnable = FALSE;
}

BOOL __stdcall CoGeometry::GetIntersectPointWithScreenCoord(VECTOR3* pv3Pos,PLANE* pPlane,POINT* ptCursor,DWORD dwViewportIndex)
{
	BOOL		bResult = FALSE;	
	VECTOR3		v3PickRayDir,v3PickRayOrig,v3To;
	float		t;

	POINT p;
	if (ptCursor->x > m_pViewport[dwViewportIndex].rcClip.right || ptCursor->y > m_pViewport[dwViewportIndex].rcClip.bottom)
		goto lb_return;

	p.x = ptCursor->x - m_pViewport[dwViewportIndex].rcClip.left;
	p.y = ptCursor->y - m_pViewport[dwViewportIndex].rcClip.top;

	if (p.x < 0 || p.y < 0)
		goto lb_return;

	CalcRay(
		&v3PickRayOrig,
		&v3PickRayDir,
		p.x,p.y,
		(DWORD)m_pViewport[dwViewportIndex].wClipWidth,(DWORD)m_pViewport[dwViewportIndex].wClipHeight,
		&m_pViewport[dwViewportIndex].matProj,
		&m_pViewport[dwViewportIndex].matView);
    VECTOR3_ADD_VECTOR3(&v3To,&v3PickRayOrig,&v3PickRayDir);
	
	bResult = CalcIntersectPointLineAndPlane(pv3Pos,pPlane,&v3PickRayOrig,&v3To,&t);

lb_return:
	return bResult;
}/*
BOOL __stdcall CoGeometry::SetCamera(VECTOR3* pEyePt,VECTOR3* pLookAtPt,VECTOR3* pUp,float fNear,float fFar,float fFovX,DWORD dwViewportIndex)
{
//	3.0f / 4.0f;

	m_pViewport[dwViewportIndex].cameraDesc.fFovX = fFovX;
	m_pViewport[dwViewportIndex].cameraDesc.fFar = fFar;
	m_pViewport[dwViewportIndex].cameraDesc.fNear = fNear;
	

	m_pViewport[dwViewportIndex].cameraDesc.v3From = *pEyePt;
	m_pViewport[dwViewportIndex].cameraDesc.v3To = *pLookAtPt;
	m_pViewport[dwViewportIndex].cameraDesc.v3Up = *pUp;

	m_pViewport[dwViewportIndex].cameraDesc.fAspect = (float)m_pViewport[dwViewportIndex].wClipHeight / (float)m_pViewport[dwViewportIndex].wClipWidth;
	m_pViewport[dwViewportIndex].cameraDesc.fFovY = 2.0f * (float)atan(m_pViewport[dwViewportIndex].cameraDesc.fAspect * (float)tan(m_pViewport[dwViewportIndex].cameraDesc.fFovX/ 2.0f));

	SetCameraDesc(&m_pViewport[dwViewportIndex].cameraDesc,dwViewportIndex);

	return TRUE;
}
*/
/*
void SetParallelProjectionMatrix(MATRIX4* pMat,DWORD dwWidth,DWORD dwHeight,float fFar,float fNear)
{

	float	fn = (fFar - fNear);

	memset(pMat,0,sizeof(MATRIX4));
	pMat->_11 = 2.0f / dwWidth;
	pMat->_22 = 2.0f / dwHeight;
	pMat->_33 = 1.0f / fn;
	pMat->_43 = -1.0f * fNear / fn;
	pMat->_44 = 1.0f;


}	 
*/
void __stdcall CoGeometry::SetCameraDesc(CAMERA_DESC* pCamera,DWORD dwViewportIndex)
{	

	m_pViewport[dwViewportIndex].cameraDesc.fAspect = (float)m_pViewport[dwViewportIndex].wClipHeight / (float)m_pViewport[dwViewportIndex].wClipWidth;
	m_pViewport[dwViewportIndex].cameraDesc.fFovY = 2.0f * (float)atan(m_pViewport[dwViewportIndex].cameraDesc.fAspect * (float)tan(m_pViewport[dwViewportIndex].cameraDesc.fFovX/ 2.0f));



	m_pViewport[dwViewportIndex].cameraDesc = *pCamera;

	MakeViewVolume(&m_pViewport[dwViewportIndex].ViewVolume,
		&m_pViewport[dwViewportIndex].cameraDesc.v3From,
		&m_pViewport[dwViewportIndex].cameraDesc.v3To,
		&m_pViewport[dwViewportIndex].cameraDesc.v3Up,
		m_pViewport[dwViewportIndex].cameraDesc.fFovX,
		1.0f / m_pViewport[dwViewportIndex].cameraDesc.fAspect,
		m_pViewport[dwViewportIndex].cameraDesc.fFar);

	::SetViewMatrix(
		&m_pViewport[dwViewportIndex].matView,
		&m_pViewport[dwViewportIndex].cameraDesc.v3From,
		&m_pViewport[dwViewportIndex].cameraDesc.v3To,
		&m_pViewport[dwViewportIndex].cameraDesc.v3Up);

	::SetProjectionMatrix(
		&m_pViewport[dwViewportIndex].matProj,
		m_pViewport[dwViewportIndex].cameraDesc.fFovY,
		m_pViewport[dwViewportIndex].cameraDesc.fAspect,
		m_pViewport[dwViewportIndex].cameraDesc.fNear,
		m_pViewport[dwViewportIndex].cameraDesc.fFar);

	SetInverseMatrix(&m_pViewport[dwViewportIndex].matViewInverse,&m_pViewport[dwViewportIndex].matView);

	VECTOR3	v3Dir;
	VECTOR3_SUB_VECTOR3(
		&m_pViewport[dwViewportIndex].cameraDesc.v3EyeDir,
		&m_pViewport[dwViewportIndex].cameraDesc.v3To,
		&m_pViewport[dwViewportIndex].cameraDesc.v3From
		);
	CreatePlane(&m_pViewport[dwViewportIndex].planeCameraEye,&m_pViewport[dwViewportIndex].cameraDesc.v3EyeDir,&m_pViewport[dwViewportIndex].cameraDesc.v3From);

/*
	MakeViewVolumeWithOrtho(&m_ViewVolume,
		&m_cameraDesc.v3From,
		&m_cameraDesc.v3To,
		&m_cameraDesc.v3Up,
		m_dwWidth,
		1.0f/m_cameraDesc.fAspect,
		80000.0f);
*/


	MATRIX4	matRotX,matRotY,matRotZ;

//	float	x	=	-1.0f * m_pViewport[dwViewportIndex].cameraDesc.fXRot * 3.1415926f / 18-0.0f;
//	float	y	=	m_pViewport[dwViewportIndex].cameraDesc.fYRot * 3.1415926f / 18-0.0f;
//	float	z	=	-1.0f * m_pViewport[dwViewportIndex].cameraDesc.fZRot * 3.1415926f / 18-0.0f;

	float	x	=	-1.0f * m_pViewport[dwViewportIndex].cameraDesc.fXRot;;
	float	y	=	m_pViewport[dwViewportIndex].cameraDesc.fYRot;;
	float	z	=	-1.0f * m_pViewport[dwViewportIndex].cameraDesc.fZRot;;


	SetRotationZMatrix(&matRotZ,z);
	SetRotationXMatrix(&matRotX,x);
	SetRotationYMatrix(&matRotY,y);
	
	
	MatrixMultiply2(&m_pViewport[dwViewportIndex].matForBillBoard,&matRotZ,&matRotX);
	MatrixMultiply2(&m_pViewport[dwViewportIndex].matForBillBoard,&m_pViewport[dwViewportIndex].matForBillBoard,&matRotY);

	if (m_pRenderer)
	{

		m_pRenderer->SetViewFrusturm(
			&m_pViewport[dwViewportIndex].ViewVolume,
			&m_pViewport[dwViewportIndex].cameraDesc,
			&m_pViewport[dwViewportIndex].matView,
			&m_pViewport[dwViewportIndex].matProj,
			&m_pViewport[dwViewportIndex].matForBillBoard);
	}

}

void __stdcall CoGeometry::SetAmbientColor(DWORD dwViewportIndex,DWORD dwColor)
{
	m_pViewport[dwViewportIndex].dwAmbientColor = dwColor;
}
DWORD __stdcall CoGeometry::GetAmbientColor(DWORD dwViewportIndex)
{
	return m_pViewport[dwViewportIndex].dwAmbientColor;
}
void __stdcall CoGeometry::SetEmissiveColor(DWORD dwViewportIndex,DWORD dwColor)
{
	m_pViewport[dwViewportIndex].dwEmissiveColor = dwColor;
}
DWORD __stdcall CoGeometry::GetEmissiveColor(DWORD dwViewportIndex)
{
	return m_pViewport[dwViewportIndex].dwEmissiveColor;
}

void __stdcall CoGeometry::GetProjectionMatrix(MATRIX4* pMatrix,DWORD dwViewportIndex)
{
	*pMatrix = m_pViewport[dwViewportIndex].matProj;
}
void __stdcall CoGeometry::GetViewMatrix(MATRIX4* pMatrix,DWORD dwViewportIndex)
{
	*pMatrix = m_pViewport[dwViewportIndex].matView;
}
void __stdcall CoGeometry::GetViewport(SHORT_RECT* pClipRect,WORD* pwWidth,WORD* pwHeight,DWORD dwViewportIndex)
{
	*pClipRect = m_pViewport[dwViewportIndex].rcClip;
	*pwWidth = m_pViewport[dwViewportIndex].wClipWidth;
	*pwHeight = m_pViewport[dwViewportIndex].wClipHeight;
}

void __stdcall CoGeometry::GetCameraAttachedMatrix(MATRIX4* pMat,DWORD dwViewportIndex,float fDist)
{
	MATRIX4		matTrans;

	SetIdentityMatrix(&matTrans);
	MATRIX4*	pMatBillboard = &m_pViewport[dwViewportIndex].matForBillBoard;
	

	CAMERA_DESC*	pCameraDesc = &m_pViewport[dwViewportIndex].cameraDesc;
	
	VECTOR3	v3CameraDir;
	VECTOR3_SUB_VECTOR3(&v3CameraDir,&pCameraDesc->v3To,&pCameraDesc->v3From);
	Normalize(&v3CameraDir,&v3CameraDir);
	VECTOR3_MUL_FLOAT(&v3CameraDir,&v3CameraDir,pCameraDesc->fNear + fDist);
	VECTOR3_ADD_VECTOR3((VECTOR3*)&matTrans._41,&pCameraDesc->v3From,&v3CameraDir);

	MatrixMultiply2(pMat,pMatBillboard,&matTrans);

}
void __stdcall CoGeometry::GetCameraDesc(CAMERA_DESC* pDesc,DWORD dwViewportIndex)
{
	*pDesc = m_pViewport[dwViewportIndex].cameraDesc;
}
void __stdcall CoGeometry::GetCameraEyePlane(PLANE* pPlane,DWORD dwViewportIndex)
{
	*pPlane = m_pViewport[dwViewportIndex].planeCameraEye;
}
void __stdcall CoGeometry::GetBillBoardMatrix(MATRIX4* pMat,DWORD dwViewportIndex)
{
	*pMat = m_pViewport[dwViewportIndex].matForBillBoard;
}
void __stdcall CoGeometry::GetSystemStatus(SYSTEM_STATUS* pStatus)
{
	if (m_pRenderer)
		m_pRenderer->GetSystemStatus(pStatus);
}

void __stdcall CoGeometry::SetTickCount(DWORD dwTickCount,BOOL bGameFrame)
{
	m_dwTickCount = dwTickCount;
	m_bGameFrame = bGameFrame;
	if (m_pRenderer)
		m_pRenderer->SetTickCount(dwTickCount,bGameFrame);

}
// 카메라 회전용 추가함수.
void __stdcall CoGeometry::SetCameraAngleRad( VECTOR3* pv3Rotate,DWORD dwViewportIndex)
{
//	float	x	=	pv3Rotate->x * 3.1415926f / 18-0.0f;
//	float	y	=	pv3Rotate->y * 3.1415926f / 18-0.0f;
//	float	z	=	pv3Rotate->z * 3.1415926f / 18-0.0f;

	float	x	=	pv3Rotate->x;
	float	y	=	pv3Rotate->y;
	float	z	=	pv3Rotate->z;


/*
	if( x > PI)
	{
		fTemp	=	x / PI;
		x		=	PI * fTemp
	}
*/
	VECTOR3	To, Up;
	To.x	=	this->m_pViewport[dwViewportIndex].cameraDesc.v3To.x - m_pViewport[dwViewportIndex].cameraDesc.v3From.x;
	To.y	=	this->m_pViewport[dwViewportIndex].cameraDesc.v3To.y - m_pViewport[dwViewportIndex].cameraDesc.v3From.y;
	To.z	=	this->m_pViewport[dwViewportIndex].cameraDesc.v3To.z - m_pViewport[dwViewportIndex].cameraDesc.v3From.z;
	float	fLength	=	VECTOR3Length(&To);
	To.x = 0;
	To.y = 0;
	To.z = fLength;
//	Up.x	=	m_cameraDesc.v3Up.x;
//	Up.y	=	m_cameraDesc.v3Up.y;
//	Up.z	=	m_cameraDesc.v3Up.z;
	Up.x	=	0;
	Up.y	=	1;
	Up.z	=	0;

	VECTOR3	NewTo, NewUp;
	// z, x, y순으로 적용.
	// z축.
	NewTo.x = cosf(z)*To.x + sinf(z)*To.y;
	NewTo.y = -1 * sinf(z)*To.x + cosf(z)*To.y;
	NewTo.z = To.z;
	To.x = NewTo.x;
	To.y = NewTo.y;
	To.z = NewTo.z;

	NewUp.x = cosf(z)*Up.x + sinf(z)*Up.y;
	NewUp.y = -1 * sinf(z)*Up.x + cosf(z)*Up.y;
	NewUp.z = Up.z;
	Up.x = NewUp.x;
	Up.y = NewUp.y;
	Up.z = NewUp.z;

	//	x축 돌리기.
	NewTo.x = To.x;
	NewTo.y = cosf(x)*To.y + sinf(x)*To.z;
	NewTo.z = -1*sinf(x)*To.y + cosf(x)*To.z;
	To.x = NewTo.x;
	To.y = NewTo.y;
	To.z = NewTo.z;

	NewUp.x = Up.x;
	NewUp.y = cosf(x)*Up.y + sinf(x)*Up.z;
	NewUp.z = -1*sinf(x)*Up.y + cosf(x)*Up.z;
	Up.x = NewUp.x;
	Up.y = NewUp.y;
	Up.z = NewUp.z;

	// y축 돌리기.
	NewTo.x = cosf(y)*To.x + sinf(y)*To.z;
	NewTo.y = To.y;
	NewTo.z = -1 * sinf(y) * To.x + cosf(y) * To.z;

	NewUp.x = cosf(y)*Up.x + sinf(y)*Up.z;
	NewUp.y = Up.y;
	NewUp.z = -1 * sinf(y) * Up.x + cosf(y) * Up.z;
	// y,x,z 회전행렬 곱.;
//	NewTo.x = (To.x * (cos(y)*cos(z) - sin(x)*sin(y)*sin(z))) + (To.y * (cos(x)*sin(z))) + (To.z * (sin(y)*cos(z) + sin(x)*cos(y)*sin(z)));
//	NewTo.y = (To.x * (-1*cos(y)*sin(z)-sin(x)*sin(y)*cos(z))) + (To.y * (cos(x)*cos(z))) + (To.z * (-1*sin(y)*sin(z) + sin(x)*cos(y)*cos(z)));
//	NewTo.z = (To.x * (-1*cos(x)*sin(y))) + (To.y * (-1*sin(x))) + (To.z * (cos(x)*cos(y)));

	this->m_pViewport[dwViewportIndex].cameraDesc.v3To.x = m_pViewport[dwViewportIndex].cameraDesc.v3From.x + NewTo.x;
	this->m_pViewport[dwViewportIndex].cameraDesc.v3To.y = m_pViewport[dwViewportIndex].cameraDesc.v3From.y + NewTo.y;
	this->m_pViewport[dwViewportIndex].cameraDesc.v3To.z = m_pViewport[dwViewportIndex].cameraDesc.v3From.z + NewTo.z;

	this->m_pViewport[dwViewportIndex].cameraDesc.v3Up.x	=	NewUp.x;
	this->m_pViewport[dwViewportIndex].cameraDesc.v3Up.y	=	NewUp.y;
	this->m_pViewport[dwViewportIndex].cameraDesc.v3Up.z	=	NewUp.z;
	

//	if( m_pViewport[dwViewportIndex].cameraDesc.fXRot > PI/2.0f )	
//		m_pViewport[dwViewportIndex].cameraDesc.fXRot	=	PI / 2.0f;
//	else if( m_pViewport[dwViewportIndex].cameraDesc.fXRot < -1*PI / 2.0f)
//		m_pViewport[dwViewportIndex].cameraDesc.fXRot =	-1*PI / 2.0f;
//	else	
		m_pViewport[dwViewportIndex].cameraDesc.fXRot	=	x;
	
//	if( m_pViewport[dwViewportIndex].cameraDesc.fYRot > 2.0f*PI)
//		m_pViewport[dwViewportIndex].cameraDesc.fYRot =	m_pViewport[dwViewportIndex].cameraDesc.fYRot - 2.0f*PI;
//	else if( m_pViewport[dwViewportIndex].cameraDesc.fYRot < 0.0f)
//		m_pViewport[dwViewportIndex].cameraDesc.fYRot =	m_pViewport[dwViewportIndex].cameraDesc.fYRot + 2.0f*PI;
//	else	
		m_pViewport[dwViewportIndex].cameraDesc.fYRot	=	y;

//	if(m_pViewport[dwViewportIndex].cameraDesc.fZRot > PI)
//		m_pViewport[dwViewportIndex].cameraDesc.fZRot	=	PI;
//	else if(m_pViewport[dwViewportIndex].cameraDesc.fZRot < -1.0f*PI)
//		m_pViewport[dwViewportIndex].cameraDesc.fZRot	=	-1.0f*PI;
//	else	
		m_pViewport[dwViewportIndex].cameraDesc.fZRot	=	z;


	this->m_bUpIsValid	=	TRUE;
	this->SetCameraDesc(&m_pViewport[dwViewportIndex].cameraDesc,dwViewportIndex);
	this->m_bUpIsValid	=	FALSE;
}

void __stdcall CoGeometry::RotateCameraRad( VECTOR3* pv3Rotate,DWORD dwViewportIndex)
{
	m_pViewport[dwViewportIndex].cameraDesc.fXRot += pv3Rotate->x;
	m_pViewport[dwViewportIndex].cameraDesc.fYRot += pv3Rotate->y;
	m_pViewport[dwViewportIndex].cameraDesc.fZRot += pv3Rotate->z;

	VECTOR3	Rotate;
	Rotate.x = m_pViewport[dwViewportIndex].cameraDesc.fXRot;
	Rotate.y = m_pViewport[dwViewportIndex].cameraDesc.fYRot;
	Rotate.z = m_pViewport[dwViewportIndex].cameraDesc.fZRot;

	this->SetCameraAngleRad( &Rotate,dwViewportIndex);

}

void __stdcall CoGeometry::GetCameraAngleRad(VECTOR3* pv3Angle,DWORD dwViewportIndex)
{
	pv3Angle->x = m_pViewport[dwViewportIndex].cameraDesc.fXRot;
	pv3Angle->y = m_pViewport[dwViewportIndex].cameraDesc.fYRot;
	pv3Angle->z = m_pViewport[dwViewportIndex].cameraDesc.fZRot;
}

void __stdcall CoGeometry::SetViewport(SHORT_RECT* pRect,DWORD dwViewportIndex)
{
	if (pRect)
	{
		if (pRect->left < m_rcWindow.left)
			pRect->left = m_rcWindow.left;

		if (pRect->top < m_rcWindow.top)
			pRect->top = m_rcWindow.top;

		if (pRect->right > m_rcWindow.right)
			pRect->right = m_rcWindow.right;

		if (pRect->bottom > m_rcWindow.bottom)
			pRect->bottom = m_rcWindow.bottom;


		m_pViewport[dwViewportIndex].rcClip = *pRect;
	
		m_pViewport[dwViewportIndex].wClipWidth = m_pViewport[dwViewportIndex].rcClip.right - m_pViewport[dwViewportIndex].rcClip.left;
		m_pViewport[dwViewportIndex].wClipHeight = m_pViewport[dwViewportIndex].rcClip.bottom - m_pViewport[dwViewportIndex].rcClip.top;

		m_pViewport[dwViewportIndex].cameraDesc.fAspect = (float)m_pViewport[dwViewportIndex].wClipHeight / (float)m_pViewport[dwViewportIndex].wClipWidth;
		m_pViewport[dwViewportIndex].cameraDesc.fFovY = 2.0f * (float)atan(m_pViewport[dwViewportIndex].cameraDesc.fAspect * tanf(m_pViewport[dwViewportIndex].cameraDesc.fFovX / 2.0f));
		SetCameraDesc(&m_pViewport[dwViewportIndex].cameraDesc,dwViewportIndex);
		m_pViewport[dwViewportIndex].bFullScreen = FALSE;
	}
	else
	{
		SHORT_RECT		rect;
		WORD			wWidth,wHeight;
		
		if (m_pRenderer)
			m_pRenderer->GetClientRect(&rect,&wWidth,&wHeight);
		
	
		m_pViewport[dwViewportIndex].bFullScreen = TRUE;
		m_pViewport[dwViewportIndex].rcClip.left = rect.left;
		m_pViewport[dwViewportIndex].rcClip.top = rect.top;
		m_pViewport[dwViewportIndex].rcClip.right = rect.right;
		m_pViewport[dwViewportIndex].rcClip.bottom = rect.bottom;
		m_pViewport[dwViewportIndex].wClipWidth = wWidth;
		m_pViewport[dwViewportIndex].wClipHeight = wHeight;
		

	}
	

	SetCameraDesc(&m_pViewport[dwViewportIndex].cameraDesc,dwViewportIndex);
	
}
BOOL __stdcall CoGeometry::SetCameraFitModel(I3DModel* pModel,DWORD dwRefIndex,float fNear,float fFar,float fFovX,DWORD dwViewportIndex)
{
	BOOL	bResult = FALSE;
	COLLISION_MODEL_DESC*		pColModelDesc;
	
	if (!m_ModelRefManager.GetColModelDesc(&pColModelDesc,dwRefIndex))
		goto lb_return;

	bResult = SetCameraFitCollisionMesh(&pColModelDesc->colMeshModel,fNear,fFar,fFovX,dwViewportIndex);

lb_return:
	return bResult;

}
BOOL __stdcall CoGeometry::SetCameraFitCollisionMesh(COLLISION_MESH_OBJECT_DESC* pColMeshDesc,float fNear,float fFar,float fFovX,DWORD dwViewportIndex)
{
	BOOL		bResult = FALSE;
	VECTOR3		v3EyePos;
	float		fZDist,fWidth,fHeight;


	fWidth = pColMeshDesc->boundingBox.v3Oct[7].x - pColMeshDesc->boundingBox.v3Oct[5].x;
	fHeight = pColMeshDesc->boundingBox.v3Oct[7].y - pColMeshDesc->boundingBox.v3Oct[5].y;

	v3EyePos.x = pColMeshDesc->boundingBox.v3Oct[5].x + (fWidth / 2.0f);
	v3EyePos.y = pColMeshDesc->boundingBox.v3Oct[5].y + (fHeight / 2.0f);
		
	if (fWidth > fHeight)
		fZDist = fWidth;
	else 
		fZDist = fHeight;
	
	fZDist /= 2.0f;		// fov 따라서 계산해야하나 일단 90도로 설정

	// min_z - fZDist
	v3EyePos.z = pColMeshDesc->boundingBox.v3Oct[4].z - fZDist;
	ResetCamera(&v3EyePos,fNear,fFar,fFovX,dwViewportIndex);

	/*
	pDesc->boundingBox.v3Oct[0].x = min_x;
	pDesc->boundingBox.v3Oct[0].y = max_y;
	pDesc->boundingBox.v3Oct[0].z = max_z;

	pDesc->boundingBox.v3Oct[1].x = min_x;
	pDesc->boundingBox.v3Oct[1].y = min_y;
	pDesc->boundingBox.v3Oct[1].z = max_z;

	pDesc->boundingBox.v3Oct[2].x = max_x;
	pDesc->boundingBox.v3Oct[2].y = min_y;
	pDesc->boundingBox.v3Oct[2].z = max_z;

	pDesc->boundingBox.v3Oct[3].x = max_x;
	pDesc->boundingBox.v3Oct[3].y = max_y;
	pDesc->boundingBox.v3Oct[3].z = max_z;

	pDesc->boundingBox.v3Oct[4].x = min_x;
	pDesc->boundingBox.v3Oct[4].y = max_y;
	pDesc->boundingBox.v3Oct[4].z = min_z;

	pDesc->boundingBox.v3Oct[5].x = min_x;
	pDesc->boundingBox.v3Oct[5].y = min_y;
	pDesc->boundingBox.v3Oct[5].z = min_z;

	pDesc->boundingBox.v3Oct[6].x = max_x;
	pDesc->boundingBox.v3Oct[6].y = min_y;
	pDesc->boundingBox.v3Oct[6].z = min_z;

	pDesc->boundingBox.v3Oct[7].x = max_x;
	pDesc->boundingBox.v3Oct[7].y = max_y;
	pDesc->boundingBox.v3Oct[7].z = min_z;
	*/
	return TRUE;
}
BOOL __stdcall CoGeometry::PreLoadModel(char* szFileName)
{
	BOOL		bResult = TRUE;
	CoModel*	pModel;

	if (IsLoadedModel((I3DModel**)&pModel,szFileName))
	{
#ifdef _DEBUG
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		char txt[512];
		wsprintf(txt,"Fail to PreLoadModel %s,Alreay loaded\n",szFileName);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,1,(void*)dwAddr,txt);
#endif
		bResult = FALSE;
		goto lb_return;
	}

	if (LoadModel((I3DModel**)&pModel,szFileName,0) == 0xffffffff)
	{
		bResult = FALSE;
		goto lb_return;
	}
	pModel->Initialize(m_pRenderer,0);
	m_pFileList[pModel->GetFileIndex()].dwFlag = FILE_HANDLE_FLAG_PRELOADED;

#ifdef _DEBUG
	char txt[512];
	wsprintf(txt,"PreLoadModel %s\n",szFileName);
	OutputDebugString(txt);
#endif
	
lb_return:
	return bResult;

}

BOOL __stdcall CoGeometry::PreLoadMotion(char* szFileName)
{
	BOOL		bResult = TRUE;
	CMotion*	pMotion;
	
	if (IsLoadedMotion(&pMotion,szFileName))
	{
#ifdef _DEBUG
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		char txt[512];
		wsprintf(txt,"Fail to PreLoadMotion %s,Alreay loaded\n",szFileName);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,1,(void*)dwAddr,txt);
#endif
		bResult = FALSE;
		goto lb_return;
	}

	pMotion = LoadMotion(szFileName);
	if (!pMotion)
	{
		bResult = FALSE;
		goto lb_return;
	}
	m_pFileList[pMotion->GetFileIndex()].dwFlag = FILE_HANDLE_FLAG_PRELOADED;
	
lb_return:
	return bResult;
}
BOOL __stdcall CoGeometry::PreLoadMaterial(char* szFileName)
{
	BOOL			bResult = TRUE;
	CMaterialLib	*pMtlLib;
	
	if (IsLoadedMaterial(&pMtlLib,szFileName))
	{
#ifdef _DEBUG
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		char txt[512];
		wsprintf(txt,"Fail to PreLoadMaterial %s,Alreay loaded\n",szFileName);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,1,(void*)dwAddr,txt);
#endif
		bResult = FALSE;
		goto lb_return;
	}

	pMtlLib = LoadMaterial(szFileName);
	if (!pMtlLib)
	{
		bResult = FALSE;
		goto lb_return;
	}
	pMtlLib->Initialize(m_pRenderer);
	m_pFileList[pMtlLib->GetFileIndex()].dwFlag = FILE_HANDLE_FLAG_PRELOADED;

	
lb_return:
	return bResult;

}
void __stdcall CoGeometry::UnloadAllPreLoadedItem(DWORD dwFlag)
{

	DWORD dwFileItemFlag;
	dwFileItemFlag = FILE_HANDLE_FLAG_PRELOADED;
	
	if (UNLOAD_PRELOADED_RESOURCE_TYPE_ALL_PRELOADED == dwFlag)
		dwFileItemFlag |= FILE_HANDLE_FLAG_PRELOADED_BUT_DISABLE_UNLOAD;
	
	for (DWORD i=0; i<m_dwMaxFileNum; i++)
	{
		if (m_pFileList[i].dwFlag & dwFileItemFlag)
		{
			if (m_pFileList[i].pFileItem)
				((IUnknown*)m_pFileList[i].pFileItem)->Release();
			
			m_pFileList[i].dwFlag = 0;
		}
	}
}
void CoGeometry::SetShadowLightSenderSphere(BOUNDING_SPHERE* pSphere,DWORD dwIndex)
{

	if (0.0f < pSphere->v3Point.y)
	{
		if (m_ShadowSender[dwIndex].dwPointNum >= MAX_SENDER_POINT_PER_SHADOWLIGHT)
			return;

		m_ShadowSender[dwIndex].sphere[m_ShadowSender[dwIndex].dwPointNum] = *pSphere;
		m_ShadowSender[dwIndex].dwPointNum++;

	//	m_v3ShadowSenderPos = *pv3Point;
	}
}


BOOL __stdcall CoGeometry::GetRayWithScreenCoord(VECTOR3* pv3Pos,VECTOR3* pv3Dir,POINT* pt,DWORD dwViewportIndex)
{

	CalcRay(
		pv3Pos,pv3Dir,
		pt->x,pt->y,
		m_pViewport[dwViewportIndex].wClipWidth,m_pViewport[dwViewportIndex].wClipHeight,
		&m_pViewport[dwViewportIndex].matProj,&m_pViewport[dwViewportIndex].matView);

	return TRUE;
}

void CoGeometry::ResourceCheck()
{

#ifdef _DEBUG
	DWORD			dwIndexList[2048];
	memset(dwIndexList,0,sizeof(DWORD));


	DWORD	dwNum = VBHGetAllItem(m_pFileNameHash,dwIndexList,2048);
	if (dwNum)
	{
		char	txt[512];
		wsprintf(txt,"\n\n\n%s\n","Geometry Resource Leak!! Look the VC++'s Output Window");
		OutputDebugString(txt);
		for (DWORD i=0; i<dwNum; i++)
		{
			memset(txt,0,sizeof(txt));
			if (m_pFileList[dwIndexList[i]].type == FILE_ITEM_TYPE_MODEL)
			{
				CoModel* pModel = (CoModel*)m_pFileList[dwIndexList[i]].pFileItem;
				wsprintf(txt,"Model %s not freed\n",pModel->GetModelFileName());

			}
			else if (m_pFileList[dwIndexList[i]].type == FILE_ITEM_TYPE_MTL)
			{
				CMaterialLib* pMtl = (CMaterialLib*)m_pFileList[dwIndexList[i]].pFileItem;
				wsprintf(txt,"Material %s not freed\n",pMtl->GetMtlFileName());
			}
			else if (m_pFileList[dwIndexList[i]].type == FILE_ITEM_TYPE_MOTION)
			{
				CMotion* pMotion = (CMotion*)m_pFileList[dwIndexList[i]].pFileItem;
				wsprintf(txt,"Motion %s not freed\n",pMotion->GetMotionFileName());
			}
			else if (m_pFileList[dwIndexList[i]].type == FILE_ITEM_TYPE_STATIC_MODEL)
			{
				CoStaticModel* pStaticModel = (CoStaticModel*)m_pFileList[dwIndexList[i]].pFileItem;
				wsprintf(txt,"StaticModel %s not freed\n",pStaticModel->GetStaticModelFileName());
			}
			else if (m_pFileList[dwIndexList[i]].type == FILE_ITEM_TYPE_HFIELD)
			{
				CoHeightField* pHField = (CoHeightField*)m_pFileList[dwIndexList[i]].pFileItem;
				wsprintf(txt,"HeightField %s not freed\n",pHField->GetHFieldFileName());
			}
			
			OutputDebugString(txt);
		}
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_RESOURCE_LEAK,0,(void*)dwAddr,txt);
	}
#endif 
}
CoGeometry::~CoGeometry()
{
	ResourceCheck();

	if (m_pViewport)
	{
		delete [] m_pViewport;
		m_pViewport = NULL;
	}
	if (m_pIC)
	{
		ICRelease(m_pIC);
		m_pIC = NULL;
	}
	if (m_pRenderQ)
	{
		ReleaseItemQueue(m_pRenderQ);
		m_pRenderQ = NULL;
	}
	if (m_pFileList)
	{
		delete [] m_pFileList;
		m_pFileList = NULL;
	}
	if (m_pFileNameHash)
	{
		VBHRelease(m_pFileNameHash);
		m_pFileNameHash = NULL;
	}

	if (m_pRenderer)
	{
		m_pRenderer->Release();
		m_pRenderer = NULL;
	}
	if (m_pFileStorage)
	{
		m_pFileStorage->Release();
		m_pFileStorage = NULL;
	}
//	FreeLibrary(m_hModGFunc);

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
	InterlockedDecrement( &g_lComponent);

}



//trustpak 2005/06/24

void __stdcall CoGeometry::SetModelLODUsingMode(DWORD dwLodUsingMode, DWORD dwLevel)
{
	m_dwLodUsingMode  = dwLodUsingMode;
	m_dwLODUsingLevel = dwLevel;
}

///

/*
void CoGeometry::SetSpotLight(LIGHT_DESC* pLight,DWORD dwLightIndex,BOOL bOrtho)
{
	CAMERA_DESC						camera;
	MATRIX4	matLightProj,matLightView,matProj;
	MATRIX4 matAdjLightProj;

	float	fvpWidth,fvpHeight;
	
	fvpWidth = (float)m_dwShadowTexSize;
	fvpHeight = (float)m_dwShadowTexSize;

	

	camera.v3From = pLight->v3Point;
	camera.v3To = pLight->v3To;
	camera.v3Up = pLight->v3Up;
	camera.fFovX = camera.fFovY = pLight->fFov;
	camera.fNear = pLight->fNear;
	camera.fFar = pLight->fFar;
	camera.fAspect = 1.0f;
	
	::SetViewMatrix(&matLightView,&camera.v3From,&camera.v3To,&camera.v3Up);
	::SetProjectionMatrix(&matProj,camera.fFovY,camera.fAspect,DEFAULT_NEAR,DEFAULT_FAR );



	
	// Set the texel aligning projection matrix.
	SetAdjustedProjectionMatrix(
		&matAdjLightProj,pLight->fFov, 1.0f,
		pLight->fNear,pLight->fFar,
		0.5f, -0.5f, fvpWidth, fvpHeight );

	// Set the light projection matrix.
	if (bOrtho)
	{
		D3DXMatrixOrthoLH((D3DXMATRIX*)&matLightProj,fWidth,fHeight,DEFAULT_NEAR,DEFAULT_FAR);
	}
	else
	{
		SetAdjustedProjectionMatrix(
			&matLightProj,pLight->fFov, 1.0f,
			pLight->fNear,pLight->fFar,
			0.0f, 0.0f, fvpWidth,fvpHeight);
	}

	
	MATRIX4		matTexScale;
	MATRIX4		matTex;



    // Create texture scaling matrix:
    // | 0.5      0        0        0 |
    // | 0        -0.5     0        0 |
    // | 0        0        0        0 |
    // | 0.5      0.5      1        1 |

    // This will scale and offset -1 to 1 range of x, y
    // coords output by projection matrix to 0-1 texture
    // coord range.
	memset(&matTexScale,0,sizeof(matTexScale));

	matTexScale._11 = 0.5f;
    matTexScale._22 = -0.5f;
    matTexScale._33 = 0.0f; 
    matTexScale._41 = 0.5f; 
    matTexScale._42 = 0.5f;
    matTexScale._43 = 1.0f; 
    matTexScale._44 = 1.0f;

    MATRIX4	mat, mat2;
	MatrixMultiply2(&mat,&matLightProj,&matTexScale);
	MatrixMultiply2(&mat2,&matLightView,&mat);
	MatrixMultiply2(&matTex,&m_pViewport[m_dwCurrentViewportIndex].matViewInverse,&mat2);
	
	m_pRenderer->SetShadowMatrixSet(&matLightView,&matTex,&matProj,&pLight->v3Point,dwLightIndex);
	



}
*/

/*
BOOL __stdcall CoGeometry::SetCameraEye(VECTOR3* pEye)
{
	m_cameraDesc.v3From = *pEye;
	SetCameraDesc(&m_cameraDesc);
	return TRUE;

}

BOOL __stdcall CoGeometry::SetCameraLook(VECTOR3* pLookAt)
{
	m_cameraDesc.v3To = *pLookAt;
	SetCameraDesc(&m_cameraDesc);
	return TRUE;
}*/