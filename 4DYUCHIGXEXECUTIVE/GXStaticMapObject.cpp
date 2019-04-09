#include "GXStaticMapObject.h"
#include "map.h"
#include "executive.h"
#include "global_variable.h"
#include "GXObject.h"

CGXStaticMapObject::~CGXStaticMapObject()
{
	DeleteBSPTree();
}

BOOL CGXStaticMapObject::Initialize(CoExecutive* pExecutive,CGXMap* pMap,DWORD dwIndex,DWORD dwObjIndex)
{
	CGXMapObject::Cleanup();
	m_dwScheduleFlag = 0;
	m_dwPropertyFlag = 0;
	m_dwObjectIndex = dwObjIndex;
	m_pModel = pMap->GetBaseModel();
	m_dwRenderFlag = RENDER_TYPE_UPDATE_SHADING | RENDER_TYPE_RECV_SHADOW | RENDER_TYPE_USE_PROJECTIONLIGHT;
	m_dwObjectType = GX_MAP_OBJECT_TYPE_STRUCT;
	m_pExecutive = pExecutive;
	m_dwAlphaFlag = 0x000000ff;
	m_dwID = 0xfffffff1;
	m_pHashHandle = NULL;

	m_pTree		=	0;

	return TRUE;
}

COLLISION_MESH_OBJECT_DESC*	__stdcall CGXStaticMapObject::GetCollisionMeshObjectDesc()
{
	COLLISION_MESH_OBJECT_DESC* pColMesh = m_pModel->GetCollisionMeshObjectDesc(m_dwObjectIndex);
	return pColMesh;
	
}

DWORD __stdcall	CGXStaticMapObject::OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2)
{
	
	
	
	return 0;
}
BOOL __stdcall CGXStaticMapObject::Render()
{/*
	CoGXObject*	pGXO = m_pExecutive->GetTracedGXObject();
	DWORD	dwAlpha = m_dwAlphaFlag;

	if (pGXO)
	{

		CAMERA_DESC	cameraDesc;
		VECTOR3		v3Ray;
		VECTOR3*	pv3To;
		float		D;

		PLANE	plane;
		m_pExecutive->GetCameraEyePlane(&plane);


		COLLISION_MESH_OBJECT_DESC* pColMesh;
		pColMesh = m_pModel->GetCollisionMeshObjectDesc(m_dwObjectIndex);


		pv3To = &pGXO->GetCollisionMeshObjectDesc()->boundingSphere.v3Point;

		D = DotProduct(&plane.v3Up,pv3To);

		if ( DotProduct(&pColMesh->boundingSphere.v3Point,&plane.v3Up) - D < 0)
		{
			dwAlpha = dwAlpha>>2;
		}
	}
	*/


	if (m_dwScheduleFlag & SCHEDULE_FLAG_NOT_RENDER)
	{
		return TRUE;
	}
	
	return m_pExecutive->INL_GetGeometry()->RenderStaticModel(m_pModel,m_dwObjectIndex,m_dwAlphaFlag,0,m_dwRenderFlag);

}

DWORD __stdcall	CGXStaticMapObject::GetObjectIndexInModel()
{
	return m_dwObjectIndex;
	
}
BOOL __stdcall CGXStaticMapObject::IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
	BOOL		bResult = FALSE;
	DWORD		dwFaceGroupIndex;
	

	if (!m_pModel->IsCollisionMeshObject(pv3IntersectPoint,pfDist,&dwFaceGroupIndex,m_dwObjectIndex,ptCursor,dwViewportIndex,dwFlag))
		goto lb_return;

	*pdwObjIndex = m_dwObjectIndex;
	*pdwModelIndex = 0xffffffff;
	bResult = TRUE;

lb_return:
	return bResult;

}



DWORD __stdcall CGXStaticMapObject::CreateIVertexList(IVERTEX** ppVertex)
{
	return m_pModel->CreateIVertexListWithIndex(ppVertex,m_dwObjectIndex);
}

void __stdcall CGXStaticMapObject::ReleaseIVertexList(IVERTEX*	pVertex)
{
	m_pModel->ReleaseIVertexList(pVertex);
}

BOOL __stdcall CGXStaticMapObject::ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc)
{
	return m_pModel->ShadeLightMapObject(m_dwObjectIndex,pv3,dwFacesNum,pLight,pFunc);
}
void	CGXStaticMapObject::BuildBSPTree()
{
	BOOL	bResult;
	// bsp트리 빌드.
	if( this->m_pTree)
	{
#ifdef _DEBUG
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"CGXStaticMapObject::BuildBSPTree(), if( this->m_pTree), File:%s , Line:%d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
#else
		__asm int 3;
#endif
		this->m_pTree->DeleteAll();

	}
	this->m_pTree		=	new	MStaticObjectTree;
	IVERTEX*	pIVertex	=	0;
	DWORD	dwIVCount	=	m_pModel->CreateIVertexListWithIndex( &pIVertex, m_dwObjectIndex);
	if( dwIVCount <= 0)		// 스테이틱하지만 빌보드같은것일 경우,
	{
//		goto	lbReturn;
	}
	bResult = m_pTree->Init(dwIVCount);
	DWORD	i;
	for( i = 0; i < dwIVCount; i++)
	{
		VECTOR3	pTri[3];
		pTri[0].x	=	pIVertex[i*3].x;
		pTri[0].y	=	pIVertex[i*3].y;
		pTri[0].z	=	pIVertex[i*3].z;
		pTri[1].x	=	pIVertex[i*3+1].x;
		pTri[1].y	=	pIVertex[i*3+1].y;
		pTri[1].z	=	pIVertex[i*3+1].z;
		pTri[2].x	=	pIVertex[i*3+2].x;
		pTri[2].y	=	pIVertex[i*3+2].y;
		pTri[2].z	=	pIVertex[i*3+2].z;

		m_pTree->AddTriangle( pTri);
	}

	m_pTree->BuildTree( 5);

lbReturn:
	if( pIVertex)	m_pModel->ReleaseIVertexList( pIVertex);
	pIVertex	=	0;
}
void	CGXStaticMapObject::DeleteBSPTree()
{
	if( m_pTree != 0)
	{
		delete	m_pTree;
		m_pTree		=	0;
	}
}
