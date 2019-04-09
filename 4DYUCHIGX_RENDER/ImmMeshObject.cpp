// DecalMeshObject.cpp: implementation of the CImmMeshObject class.
//
//////////////////////////////////////////////////////////////////////

#include "ImmMeshObject.h"
#include "CoD3DDevice.h"
#include "render_type.h"
#include "VBHeap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


STDMETHODIMP CImmMeshObject::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (refiid == IID_IMM_MESH_OBJECT)
	{
		*ppv = (IDIImmMeshObject*)this;
		AddRef();
		return S_OK;
	}

	*ppv = NULL;
	return E_NOINTERFACE;

}
STDMETHODIMP_(ULONG) CImmMeshObject::AddRef(void)
{
	m_dwRefCount++;
	return m_dwRefCount;
}
STDMETHODIMP_(ULONG) CImmMeshObject::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
	{
		m_pRenderer->DeleteImmMeshObject(this);
		Cleanup();
		
	}

	return 0;
}


CImmMeshObject::CImmMeshObject()
{
	m_fRenderPriorityFactor = RENDER_PRIORITY_FACTOR_IMMMESHOBJECT;
	m_dwWorkMemorySize = 0;
	m_dwTriCount = 0;
	m_pMtlHandle = NULL;
	m_pWorkMemory = NULL;
}

CImmMeshObject immMeshObject;
void CImmMeshObject::Initialize(CoD3DDevice* pRenderer)
{
	CImmMeshObject*	pDummy;
	pDummy = &immMeshObject;
	memcpy(this,pDummy,sizeof(CImmMeshObject));	

	m_pRenderer = pRenderer;
	
/*	
	
	__asm
	{
		; new 를 사용하지 않으므로 더미 인스턴스를 하나 만들어 함수테이블을 복사한다.
		xor		ecx,ecx
		mov		ecx,2
		mov		esi,dword ptr[pDummy]
		mov		edi,dword ptr[this]
		rep		movsd		
	}*/
}
void CImmMeshObject::Cleanup()
{
	if (m_pWorkMemory)
	{
		delete [] m_pWorkMemory;
		m_pWorkMemory = NULL;
	}
	m_VB.Cleanup();

}


BOOL CImmMeshObject::Build(IVERTEX* piv3Tri,DWORD dwTriCount,void* pMtlHandle,DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	m_dwTriCount = dwTriCount;
	m_dwVertexNum = dwTriCount*3;
	
	if (m_pWorkMemory)
		Cleanup();

	DWORD	dwErrorCode;
	if (!m_pRenderer->GetVBHeapForImmMesh()->AllocVB(&dwErrorCode,&m_VB,m_dwVertexNum,&m_dwWorkMemorySize,&m_VB.m_bEnable,NULL))
		goto lb_return;

	m_pWorkMemory = new BYTE[m_dwWorkMemorySize];
	memcpy(m_pWorkMemory,piv3Tri,m_dwWorkMemorySize);
	Update(0);


	m_pMtlHandle = (DIMATERIAL*)pMtlHandle;
	bResult = TRUE;
lb_return:
	return bResult;
}

BOOL __stdcall CImmMeshObject::Render(
		DWORD dwRefIndex,DWORD dwAlpha,
		LIGHT_INDEX_DESC* pDynamicLightIndexList,DWORD dwLightNum,
		LIGHT_INDEX_DESC* pSpotLightIndexList,DWORD dwSpotLightNum,
		DWORD dwMtlSetIndex,
		DWORD dwEffectIndex,
		DWORD dwFlag
		)
{
	LPDIRECT3DDEVICE8	pDevice = m_pRenderer->GetDevice();
	


	
//	m_pD3DDevice->SetVertexShader(D3DFVF_XYZ | D3DFVF_TEX1);
	pDevice->SetVertexShader(m_VB.GetVertexShader());
	
	pDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)m_pRenderer->GetIdentityMatrix());
	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,0);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);
	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);


	DIMATERIAL*	pMtl = m_pMtlHandle;
	if (!pMtl)
		pMtl = m_pRenderer->GetDefaultMaterial();

	D3DMATERIAL8	d3dmtl;
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
	pDevice->SetMaterial(&d3dmtl);

		// 2side 처리

	if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
		pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		


	BeginAlphaMeshObject(pDevice,1,pMtl->dwAlpha,dwAlpha);
	pMtl->TextureDiffuse.SetTexture(pDevice,0);

	pDevice->SetRenderState( D3DRS_ZBIAS, 10);


	pDevice->SetStreamSource( 0,m_VB.GetVertexBuffer(),m_VB.GetVertexSize());

	//trustpak 2005/07/15
	pDevice->DrawPrimitive( D3DPT_TRIANGLELIST,m_VB.GetStartIndex(),m_dwTriCount);		
	/*
	if (TRUE == m_pRenderer->GetRenderWireSolidBothMode())
	{
		DWORD dwLegacyRenderMode = m_pRenderer->GetRenderMode();

		m_pRenderer->SetRenderMode(RENDER_MODE_SOLID);

		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST,m_VB.GetStartIndex(),m_dwTriCount);	


		m_pRenderer->SetRenderMode(RENDER_MODE_WIREFRAME);
		SetColorFactorShader(pDevice, 0xff808080,0);					

		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST,m_VB.GetStartIndex(),m_dwTriCount);	


		RestoreTextureShader(pDevice,0);
		m_pRenderer->SetRenderMode(dwLegacyRenderMode);

	}
	else
	{
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST,m_VB.GetStartIndex(),m_dwTriCount);	
	}
	*/

	///--trustpak end

//	m_pD3DDevice->DrawPrimitiveUP(
//		D3DPT_TRIANGLELIST,
//		1,piv3Tri,sizeof(IVERTEX));

	pDevice->SetRenderState( D3DRS_ZBIAS, 0);

	EndAlphaMeshObject(pDevice);
				
	if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
		pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

	return TRUE;
}

BOOL __stdcall CImmMeshObject::GetTriBufferPtr(BYTE** ppDest,DWORD* pdwSize)
{
	*ppDest = m_pWorkMemory;
	*pdwSize = m_dwWorkMemorySize;

	return TRUE;
}
BOOL __stdcall CImmMeshObject::Update(DWORD dwFlag)
{

	BYTE*	pDest;
	m_VB.LockVertexPtr(&pDest,0);
	memcpy(pDest,m_pWorkMemory,m_dwWorkMemorySize);
	m_VB.UnlockVertexPtr();

	return TRUE;
}
/*
BOOL __stdcall CImmMeshObject::StartInitialize(MESH_DESC* pDesc,IGeometryController* pControl)
{
	return FALSE;
}
void __stdcall CImmMeshObject::EndInitialize()
{
}
BOOL __stdcall CImmMeshObject::InsertFaceGroup(FACE_DESC* pDesc)
{
	return FALSE;
}

BOOL __stdcall CImmMeshObject::RenderShadow(DWORD dwRefIndex,DWORD dwAlpha,BYTE* pSpotLightIndex,DWORD dwViewNum,DWORD dwFlag)
{
	return FALSE;
}

void __stdcall CImmMeshObject::DisableUpdate()
{
}
*/
