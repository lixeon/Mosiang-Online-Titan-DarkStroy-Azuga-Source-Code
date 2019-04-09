#include "GXLight.h"
#include "GXObject.h"
#include "GXDefault.h"
#include "../4DyuchiGXGFunc/global.h"

BOOL CoGXLight::Initialize(CoExecutive* pExecutive,LIGHT_DESC* pDesc,MATERIAL* pMtl,GXSchedulePROC pProc,DWORD dwFlag)
{
	memset(this,0,sizeof(CoGXLight));
	CGXMapObject::Cleanup();

	m_pProc = pProc;

//	if (!m_pProc)
//		m_pProc = GXDefaultLightSchedulePROC;

	m_pExecutive = pExecutive;
	memcpy(&m_LightDesc,pDesc,sizeof(LIGHT_DESC));
	m_LightDesc.pMtlHandle = NULL;

	VECTOR3_SUB_VECTOR3(&m_v3Dir,&m_LightDesc.v3To,&m_LightDesc.v3Point);
	
	m_LightDesc.pMtlHandle = NULL;

	
	if (dwFlag & GXLIGHT_TYPE_ENABLE_IMAGE_PROJECTION)
	{
		I4DyuchiGXRenderer*	pRenderer = m_pExecutive->GetRenderer();
		if (pRenderer)
		{
		
			if (pMtl)
			{
				m_LightDesc.pMtlHandle = pRenderer->CreateMaterial(pMtl,&m_dwWidth,&m_dwHeight,0);
				// 텍스쳐 어드레스중 보더모드를 지원하지 않는 비됴카드를 고려해서 미리 보더를 설정해둔다.
				DWORD	dwBorderColor;
				if ((0x000000ff & pDesc->dwProjTexIndex) == 0xff)
				{
					// add 모드 
					dwBorderColor = 0x00000000;
				}
				else
				{
					// multiply모드
					dwBorderColor = 0xffffffff;

				}
				pRenderer->SetMaterialTextureBorder(m_LightDesc.pMtlHandle,dwBorderColor);
			}
		}
	}

	if (dwFlag & GXLIGHT_TYPE_STATIC)
	{
		DisableSchedule();
	//	DisableRender();
	}
	if (dwFlag & GXLIGHT_TYPE_ENABLE_DYNAMIC_LIGHT)
	{
		EnableDynamicLight();
	}
	
//	if (dwFlag & GXLIGHT_TYPE_ACCESS_LIGHT_MAP)
//	{
//		m_LightDesc.dwProjTexIndex = 0xffffffff;
//	}
	
	m_pSymbolModel = m_pExecutive->GetSymbolModel(SYMBOL_TYPE_LIGHT);
	
	if (m_pSymbolModel)
	{
		m_dwModelRefIndex = m_pSymbolModel->Reference();
		m_pSymbolModel->SetZOrder(9,m_dwModelRefIndex);
	}

	m_dwPropertyFlag = dwFlag;
	m_dwRenderFlag = RENDER_TYPE_DISABLE_ZCLIP;
	m_dwObjectType = GX_MAP_OBJECT_TYPE_LIGHT;
	m_dwID = 0xfffffff3;
	m_pHashHandle = NULL;

	MATRIX4	matTransform;
	SetIdentityMatrix(&matTransform);
	*(VECTOR3*)&matTransform._41 = m_LightDesc.v3Point;

	m_colMeshDesc.boundingSphere.v3Point = m_LightDesc.v3Point;
	m_colMeshDesc.boundingSphere.fRs = m_LightDesc.fRs;

	if (m_pSymbolModel)
	{
		m_pSymbolModel->UpdateColMeshDesc(m_dwModelRefIndex,m_pParentMatrix,&matTransform,0,0,0);
	}
	return TRUE;
}
DWORD __stdcall CoGXLight::GetObjectIndexInModel()
{
	return 0xffffffff;
}

BOOL __stdcall CoGXLight::Render()
{

	BOOL	bResult = FALSE;
	MATRIX4	matTransform;
	SetIdentityMatrix(&matTransform);
	*(VECTOR3*)&matTransform._41 = m_LightDesc.v3Point;


	if (m_pSymbolModel)
		bResult = m_pExecutive->INL_GetGeometry()->RenderModel(m_pSymbolModel,m_dwModelRefIndex,m_pParentMatrix,&matTransform,0,0,255,0,0,m_dwRenderFlag);

	bResult = m_pExecutive->INL_GetGeometry()->RenderLight(&m_LightDesc,m_dwAlphaFlag,m_dwRenderFlag);
	return bResult;
}

void CoGXLight::SetPosition(VECTOR3* pv3Pos) 
{
	m_LightDesc.v3Point = *pv3Pos;
	VECTOR3_ADD_VECTOR3(&m_LightDesc.v3To,&m_LightDesc.v3Point,&m_v3Dir);

	UpdateCollisionMesh();

}
void CoGXLight::SetLightDesc(LIGHT_DESC* pLightDesc)
{
	// 매터리얼 핸들을 가지고 있을 경우 정상적인 절차로만 세팅해야하므로..
	// 무시한다.

	void* pMtlHandle = NULL;
	if (m_LightDesc.pMtlHandle)
		pMtlHandle = m_LightDesc.pMtlHandle;
	
	m_LightDesc = *pLightDesc;
	m_LightDesc.pMtlHandle = pMtlHandle;

	VECTOR3_SUB_VECTOR3(&m_v3Dir,&m_LightDesc.v3To,&m_LightDesc.v3Point);
	UpdateCollisionMesh();

}
void CoGXLight::MovePosition(VECTOR3* pv3Pos) 
{
	VECTOR3_ADDEQU_VECTOR3(&m_LightDesc.v3Point,pv3Pos);
	VECTOR3_ADDEQU_VECTOR3(&m_LightDesc.v3To,pv3Pos);

//	m_LightDesc.v3Point.x += pv3Pos->x; 
//	m_LightDesc.v3Point.y += pv3Pos->y; 
//	m_LightDesc.v3Point.z += pv3Pos->z;
	VECTOR3_SUB_VECTOR3(&m_v3Dir,&m_LightDesc.v3To,&m_LightDesc.v3Point);
	UpdateCollisionMesh();
}
void CoGXLight::UpdateCollisionMesh()
{

	MATRIX4	matTransform;
	SetIdentityMatrix(&matTransform);
	*(VECTOR3*)&matTransform._41 = m_LightDesc.v3Point;

	m_colMeshDesc.boundingSphere.v3Point = m_LightDesc.v3Point;
	m_colMeshDesc.boundingSphere.fRs = m_LightDesc.fRs;

	if (m_pSymbolModel)
	{
		m_pSymbolModel->UpdateColMeshDesc(m_dwModelRefIndex,m_pParentMatrix,&matTransform,0,0,0);
	}

	CGXMap*	pMap = m_pExecutive->GetGXMap();
	if (pMap)
	{
		if (m_dwClipperIndex != 0xffffffff)
			pMap->UpdateDynamicObject(m_dwClipperIndex);
	}

	
}
void CoGXLight::Cleanup()
{
	
	if (m_LightDesc.pMtlHandle)
	{
		m_pExecutive->GetRenderer()->DeleteMaterial(m_LightDesc.pMtlHandle);
		m_LightDesc.pMtlHandle = NULL;

	}
	if (m_pSymbolModel)
	{
		m_pSymbolModel->UnReference(m_dwModelRefIndex);
	}
	if (m_pParentGXObject)
	{
		m_pParentGXObject->DetachLight(this);
	}
	ReleaseID();
}
COLLISION_MESH_OBJECT_DESC*	__stdcall CoGXLight::GetCollisionMeshObjectDesc()
{
	return &m_colMeshDesc;
}
DWORD __stdcall	CoGXLight::OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2)
{
	if (m_pProc)
		m_pProc(pExecutive,this,msg,arg1,arg2,m_pData);

	DefaultProc();
	return 0;
}
void CoGXLight::DefaultProc()
{
	
	if (m_pParentMatrix)
	{

		if (m_dwPropertyFlag & ATTACH_TYPE_LINK)
		{
			VECTOR3_ADD_VECTOR3(&m_LightDesc.v3Point,(VECTOR3*)&m_pParentMatrix->_41,&m_v3FromDistancePivot);
			
			
		}
		else 
		{
			m_LightDesc.v3Point = *(VECTOR3*)&m_pParentMatrix->_41;
//			m_LightDesc.v3To = *(VECTOR3*)&m_pParentMatrixTo->_41;

		}
	}
	VECTOR3_ADD_VECTOR3(&m_LightDesc.v3To,&m_LightDesc.v3Point,&m_v3Dir);
	if (memcmp(&m_colMeshDesc.boundingSphere.v3Point,&m_LightDesc.v3Point,sizeof(VECTOR3)))
	{
		UpdateCollisionMesh();
	}
}
BOOL __stdcall CoGXLight::IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
	DWORD	dwIndex = 0xffffffff;
	DWORD	dwFaceIndex;
	BOOL	bResult = FALSE;

	MATRIX4	mat4;
	SetIdentityMatrix(&mat4);

	if (!m_pSymbolModel)
		goto lb_return;


	*(VECTOR3*)&mat4._41 = m_LightDesc.v3Point;

	dwIndex = m_pSymbolModel->GetMeshObjectWithScreenCoord(pv3IntersectPoint,pfDist,&dwFaceIndex,m_dwModelRefIndex,NULL,&mat4,0,0,ptCursor,dwViewportIndex,dwFlag);
	if (dwIndex == 0xffffffff)
		goto lb_return;

	*pdwObjIndex = 0xffffffff;
	*pdwModelIndex = 0xffffffff;
	bResult = TRUE;

lb_return:
	return bResult;
}

void CoGXLight::PostAttach(CoGXObject* pObj,MATRIX4* pMat,VECTOR3* pv3Offset,DWORD dwAttachType)
{
	
	m_dwPropertyFlag = (m_dwPropertyFlag & GXLIGHT_ATTACH_TYPE_MASK_INVERSE) | dwAttachType;
	m_pParentGXObject = pObj;
	m_pParentMatrix = pMat;

	if (dwAttachType == ATTACH_TYPE_LINK)
	{
		if (pv3Offset)
		{
			m_v3FromDistancePivot = *pv3Offset;
		}
		else
		{
			VECTOR3_SUB_VECTOR3(&m_v3FromDistancePivot,&m_LightDesc.v3Point,(VECTOR3*)&m_pParentMatrix->_41);
		}
	}
	DefaultProc();

}


void CoGXLight::PostDetach()
{
	m_pParentGXObject = NULL;
	m_pParentMatrix = NULL;
	DefaultProc();

}

DWORD CoGXLight::Release()
{
	CGXMap*	pMap = m_pExecutive->GetGXMap();
	if (pMap)
	{
		if (m_dwIndexInInitialTable != 0xffffffff)
			pMap->DeleteInitialGXLight(m_dwIndexInInitialTable);

		if (m_dwClipperIndex != 0xffffffff)
			pMap->DeleteDynamicObject(m_dwClipperIndex);
	}

	Cleanup();
	CGXMapObject::Cleanup();
	return 0;
}


DWORD __stdcall CoGXLight::CreateIVertexList(IVERTEX** ppVertex)
{
	return 0;
}

void __stdcall CoGXLight::ReleaseIVertexList(IVERTEX*	pVertex)
{
}

BOOL __stdcall CoGXLight::ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc)
{
	return FALSE;
}


