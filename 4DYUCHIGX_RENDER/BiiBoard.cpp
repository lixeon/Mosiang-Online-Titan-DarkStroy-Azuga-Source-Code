#include "BillBoard.h"
#include "CoD3DDevice.h"
#include "../4DyuchiGXGFunc/global.h"
#include "d3d_helper.h"
#include "render_type.h"
#include "MaterialManager.h"


CBillBoardObject::CBillBoardObject()
{
	memset((char*)this+4,0,sizeof(CBillBoardObject)-4);
	m_bCanUpdate = TRUE;

	
	m_fRenderPriorityFactor = RENDER_PRIORITY_FACTOR_BILLBOARDOBJECT;

}



BOOL __stdcall CBillBoardObject::StartInitialize(MESH_DESC* pDesc,IGeometryController* pControl,IGeometryControllerStatic* pControlStatic)
{
	BOOL		bResult = FALSE;
	
	m_meshFlag = pDesc->meshFlag;
	m_pMeshController = pControl;

	m_pv3Local = new VECTOR3[pDesc->dwVertexNum];
	m_ptvTex = pDesc->ptvTexCoordList;

	TransformVector3_VPTR2(m_pv3Local,pDesc->pv3WorldList,pDesc->pMatrixWorldInverse,pDesc->dwVertexNum);

	
	
	m_ptvTex = pDesc->ptvTexCoordList;

	bResult = TRUE;

lb_return:
	return bResult;
}



BOOL __stdcall CBillBoardObject::InsertFaceGroup(FACE_DESC* pDesc)
{
	BOOL			bResult = FALSE;

	CreateRenderPrimitive(m_pv3Local,m_ptvTex,pDesc->pIndex,pDesc->dwFacesNum);
	m_pMaterial = m_pRenderer->AllocMaterial(pDesc->pMaterial,NULL,NULL,0);

	bResult = TRUE;

lb_return:
	return bResult;
}
void __stdcall CBillBoardObject::EndInitialize()
{
	if (m_pv3Local)
	{
		delete [] m_pv3Local;
		m_pv3Local = NULL;
	}
}


void __stdcall CalcInverseRotate(MATRIX4* pResult,MATRIX4* pTransformMat)
{
	VECTOR3		v3Front;
	
	v3Front.x = 0.0f;
	v3Front.y = 0.0f;
	v3Front.z = -1.0f;

	MATRIX4		matRot = *pTransformMat;
	matRot._41 = 0.0f;
	matRot._42 = 0.0f;
	matRot._43 = 0.0f;


	VECTOR3		v3Dir;
	TransformVector3_VPTR2(&v3Dir,&v3Front,&matRot,1);
	float ang = CalcAngle(&v3Front,&v3Dir);
	
	VECTOR3		v3Axis;
	CrossProduct(&v3Axis,&v3Dir,&v3Front);
	Normalize(&v3Axis,&v3Axis);
	if (ang == 0.0f )
		SetIdentityMatrix(pResult);
	else
		SetRotationMatrix(pResult,&v3Axis,-1.0f*ang);

	TransformVector3_VPTR1(&v3Dir,pResult,1);
}

BOOL __stdcall CBillBoardObject::Render(
		DWORD dwRefIndex,										// 애니메이션 컨트롤블럭
		DWORD dwAlpha,
		LIGHT_INDEX_DESC* pDynamicLightIndexList,DWORD dwLightNum,
		LIGHT_INDEX_DESC* pSpotLightIndexList,DWORD dwSpotLightNum,
		DWORD dwMtlSetIndex,
		DWORD dwEffectIndex,
		DWORD dwFlag					
		)
{	
	LPDIRECT3DDEVICE8	pDevice = m_pRenderer->GetDevice();

	BOOL		bResult = FALSE;
	MATRIX4*	pResult;

	
	VECTOR3*		pv3Local;
	TVERTEX*		ptvAnimated;
	
	

	m_pMeshController->ApplyResultMatrix(&pResult,dwRefIndex,RESULT_MATRIX_ALIGN_VIEW);
	m_pMeshController->ApplyMorph(&pv3Local,&ptvAnimated,dwRefIndex,0);

	if (ptvAnimated)
		m_VB.SetTexCoord(ptvAnimated,m_pIndex,TEX1_COORD_OFFSET_BBOARD,m_dwVertexNum);

	
	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,0);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);
	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);

	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);	
	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	
	
	pDevice->SetVertexShader(m_VB.GetVertexShader());
	pDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)pResult);
	


	if (dwFlag & RENDER_TYPE_DISABLE_ZCLIP)
		pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);


	if (m_meshFlag.IsDisableZBubfferWrite())
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
		

	
	D3DMATERIAL8	d3dmtl = m_pMaterial->d3dMtl;
		
	if (!(m_pMaterial->dwProperty & DIMATERIAL_PROPERTY_TYPE_SELF_ILLUMIN))
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

	
	pDevice->SetMaterial(&d3dmtl);

	// 2side 처리
	if (m_pMaterial->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
		pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		
		
	BeginAlphaMeshObject(pDevice,1,m_pMaterial->dwAlpha,dwAlpha);
	
		
	if (m_pMaterial->TextureDiffuse.HasTexture())
	{
		
		m_pMaterial->TextureDiffuse.SetTexture(pDevice,0);
	}

	///////////////////////////////////////////////////////////////////////////////////////
	pDevice->SetStreamSource( 0,m_VB.GetVertexBuffer(),m_VB.GetVertexSize());

	//trustpak 2005/07/12
	//pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0,m_dwTotalFacesNum);

	if (TRUE == m_pRenderer->GetRenderWireSolidBothMode())
	{
		DWORD dwLegacyRenderMode = m_pRenderer->GetRenderMode();
		
		// 먼저 solid 로 한번 그리고,
		m_pRenderer->SetRenderMode(RENDER_MODE_SOLID);
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0,m_dwTotalFacesNum);

		// wire frame을 다시 그려준다. 

		m_pRenderer->SetRenderMode(RENDER_MODE_WIREFRAME);
		SetColorFactorShader(pDevice, 0xff808080,0);

		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0,m_dwTotalFacesNum);

		RestoreTextureShader(pDevice,0);
		m_pRenderer->SetRenderMode(dwLegacyRenderMode);
	}
	else
	{
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0,m_dwTotalFacesNum);
	}	
	///--trustpak end

	///////////////////////////////////////////////////////////////////////////////////////

	// 알파블랜딩 end.
	EndAlphaMeshObject(pDevice);
				
	if (m_pMaterial->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
		pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	
	if (m_meshFlag.IsDisableZBubfferWrite())
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

	if (dwFlag & RENDER_TYPE_DISABLE_ZCLIP)
		pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

	
	if (m_pRenderer->INL_IsEnableFog())
		pDevice->SetRenderState(D3DRS_FOGENABLE,FALSE);



lb_return:
	return bResult;

}


BOOL __stdcall CBillBoardObject::Update(DWORD dwFlag)
{
	if (!m_bCanUpdate)
		return FALSE;

	return TRUE;
}

BOOL CBillBoardObject::CreateRenderPrimitive(VECTOR3* pv3,TVERTEX* ptv,WORD* pIndex,DWORD dwFacesNum)
{
	BOOL		bResult = FALSE;
	LPDIRECT3DDEVICE8	pDevice = m_pRenderer->GetDevice();

	m_dwTotalFacesNum = dwFacesNum;
	m_dwVertexNum = dwFacesNum*3;

	m_pIndex = new WORD[dwFacesNum*3];
	memcpy(m_pIndex,pIndex,sizeof(WORD)*dwFacesNum*3);

	m_VB.CreateVertexBuffer(m_pRenderer->GetD3DResourceManager(),sizeof(D3DBBOARDVERTEX),D3DUSAGE_WRITEONLY,D3DFVF_BBOARDVERTEX,D3DPOOL_MANAGED,m_dwVertexNum);
	m_VB.SetVertexPosList(pIndex,pv3,m_dwVertexNum);
	if (ptv)
		m_VB.SetTexCoord(ptv,pIndex,TEX1_COORD_OFFSET_BBOARD,m_dwVertexNum);
	
	bResult = TRUE;

lb_return:
	return bResult;
}
/*
BOOL CBillBoardObject::SetMaterial(MATERIAL* pMtlSrc)
{
	BOOL		bResult = FALSE;



//	m_VB.SetVertexColor(pMaterial->GetDiffuse(),DIFFUSE_COORD_OFFSET_XYZ,m_dwVertexNum);
	if (!m_pRenderer->AllocTexture(&m_Texture,pMaterial->GetDiffuseTexmapName(),0,TRUE,TRUE))
	{
		m_pRenderer->AllocDefaultTexture(&m_Texture);
	}
	
	if (pMaterial->dwFlag & TRANSP_TYPE_FILTER)
	{
		m_dwAlpha = TEX_FLAG_ALPHA_TRANSP_FILTER;
		m_dwAlpha |= (BYTE)( (1.0f - pMaterial->fTransparency)* 255.0f);
		
	}
	if (pMaterial->dwFlag & TRANSP_TYPE_SUBTRACTIVE)
	{
		m_dwAlpha = TEX_FLAG_ALPHA_TRANSP_SUB;
		m_dwAlpha |= (BYTE)( (1.0f - pMaterial->fTransparency)* 255.0f);
	}
	else if (pMaterial->dwFlag & TRANSP_TYPE_ADDITIVE)
	{
		m_dwAlpha = TEX_FLAG_ALPHA_TRANSP_ADD;
		m_dwAlpha |= (BYTE)( (1.0f - pMaterial->fTransparency)* 255.0f);
	}
	
	if (m_Texture.GetTexFlag() & TEX_FLAG_ALPHA_OPASITY)
	{
		m_dwAlpha |= TEX_FLAG_ALPHA_OPASITY;
		m_dwAlpha |= (BYTE)( (1.0f - pMaterial->fTransparency)* 255.0f);

	}

	if ((pMaterial->GetFlag() & MATERIAL_TYPE_2SIDE_MASK) == MATERIAL_TYPE_2SIDE)
		Set2Side(TRUE);
	else 
		Set2Side(FALSE);

	bResult = TRUE;

lb_return:
	return bResult;
}
*/
	
/*
	memset(pMtlResult,0,sizeof(DIMATERIAL));
	DWORDtoCOLOR(
		pMtlSrc->GetDiffuse(),
		&pMtlResult->d3dMtl.Diffuse.r,
		&pMtlResult->d3dMtl.Diffuse.g,
		&pMtlResult->d3dMtl.Diffuse.b,
		&pMtlResult->d3dMtl.Diffuse.a);

	DWORDtoCOLOR(
		pMtlSrc->GetAmbient(),
		&pMtlResult->d3dMtl.Ambient.r,
		&pMtlResult->d3dMtl.Ambient.g,
		&pMtlResult->d3dMtl.Ambient.b,
		&pMtlResult->d3dMtl.Ambient.a);

	DWORDtoCOLOR(
		pMtlSrc->GetSpecular(),
		&pMtlResult->d3dMtl.Specular.r,
		&pMtlResult->d3dMtl.Specular.g,
		&pMtlResult->d3dMtl.Specular.b,
		&pMtlResult->d3dMtl.Specular.a);

			
	pMtlResult->d3dMtl.Power = pMtlSrc->fShineStrength * MAXSHINESTR_TO_SS3D_VAL;
	if (m_pRenderer->INL_IsEnableSpecular())
	{
		if (0.0f == pMtlSrc->fShineStrength)
			pMtlResult->d3dMtl.Power = m_pRenderer->INL_GetDefaultSpecularPower();
	}

	pMtlResult->d3dMtl.Diffuse.a = 1.0f;
	pMtlResult->d3dMtl.Ambient.a = 1.0f;
	pMtlResult->d3dMtl.Specular.a = 1.0f;

	if (pMtlSrc->dwFlag & SELF_ILLUNUM)
	{
		pMtlResult->dwProperty |= DIMATERIAL_PROPERTY_TYPE_SELF_ILLUMIN;
		pMtlResult->d3dMtl.Emissive.a = 1.0f;
		pMtlResult->d3dMtl.Emissive.r = 1.0f;
		pMtlResult->d3dMtl.Emissive.g = 1.0f;
		pMtlResult->d3dMtl.Emissive.b = 1.0f;
	}
	pMtlResult->dwAlpha = 255;
	if (pMtlSrc->dwFlag & TRANSP_TYPE_FILTER)
	{
		pMtlResult->dwAlpha = TEX_FLAG_ALPHA_TRANSP_FILTER;
		pMtlResult->dwAlpha |= (BYTE)( (1.0f - pMtlSrc->fTransparency)* 255.0f);
		if (pMtlResult->dwAlpha & 0x000000ff == 255)
			pMtlResult->dwAlpha = 255;		
	}
	else if (pMtlSrc->dwFlag & TRANSP_TYPE_SUBTRACTIVE)
	{
		pMtlResult->dwAlpha = TEX_FLAG_ALPHA_TRANSP_SUB;
		pMtlResult->dwAlpha |= (BYTE)( (1.0f - pMtlSrc->fTransparency)* 255.0f);
	}
	else if (pMtlSrc->dwFlag & TRANSP_TYPE_ADDITIVE)
	{
		pMtlResult->dwAlpha = TEX_FLAG_ALPHA_TRANSP_ADD;
		pMtlResult->dwAlpha |= (BYTE)( (1.0f - pMtlSrc->fTransparency)* 255.0f);
	}
	
	// diffuse텍스쳐 할당 

	char*	pTexFileName = pMtlSrc->GetDiffuseTexmapName();
	if (m_pRenderer->AllocTexture(&pMtlResult->TextureDiffuse,pTexFileName,0,TRUE,TRUE))
	{

		if (pMtlResult->TextureDiffuse.GetTexFlag() & TEX_FLAG_ALPHA_OPASITY)
		{
			pMtlResult->dwAlpha |= TEX_FLAG_ALPHA_OPASITY;
			pMtlResult->dwAlpha |= (BYTE)( (1.0f - pMtlSrc->fTransparency)* 255.0f);

		}
	}
	else
	{
		m_pRenderer->AllocDefaultTexture(&pMtlResult->TextureDiffuse);
	}

	// reflect 
	pTexFileName = pMtlSrc->GetReflectTexmapName();
	m_pRenderer->AllocTexture(&pMtlResult->TextureReflect,pTexFileName,0,TRUE,TRUE);


	if (pMtlSrc->GetFlag() &  MATERIAL_TYPE_2SIDE)
		pMtlResult->dwProperty |= DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE;
}*/

CBillBoardObject::~CBillBoardObject()
{
	if (m_pMaterial)
	{
		m_pRenderer->DeleteMaterial(m_pMaterial);
		m_pMaterial = NULL;

	}
	if (m_pIndex)
	{
		delete [] m_pIndex;
		m_pIndex = NULL;
	}
	
	CBaseMeshObject::~CBaseMeshObject();
}
