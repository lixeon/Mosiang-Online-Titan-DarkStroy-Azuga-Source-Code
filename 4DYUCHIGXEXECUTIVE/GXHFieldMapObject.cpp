#include "GXHFieldMapObject.h"
#include "executive.h"

BOOL CGXHFieldMapObject::Initialize(CoExecutive* pExecutive,CGXMap* pMap,DWORD dwIndex,DWORD dwObjIndex)
{
	CGXMapObject::Cleanup();
	m_dwScheduleFlag = 0;
	m_dwPropertyFlag = 0;
	m_dwObjectIndex = dwObjIndex;
	m_pHField = pMap->GetHField();
	m_dwRenderFlag = RENDER_TYPE_UPDATE_SHADING | RENDER_TYPE_RECV_SHADOW;
	m_dwObjectType = GX_MAP_OBJECT_TYPE_HFIELD;
	m_pExecutive = pExecutive;
	m_dwAlphaFlag = 0x000000ff;
	m_dwID = 0xfffffff2;
	m_pHashHandle = NULL;

	return TRUE;
}

COLLISION_MESH_OBJECT_DESC*	__stdcall CGXHFieldMapObject::GetCollisionMeshObjectDesc()
{
	COLLISION_MESH_OBJECT_DESC* pColMesh = m_pHField->GetCollisionMeshObjectDesc(m_dwObjectIndex);
	return pColMesh;
	
}

DWORD __stdcall	CGXHFieldMapObject::OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2)
{
	
	
	
	return 0;
}
BOOL __stdcall CGXHFieldMapObject::Render()
{
	return m_pExecutive->INL_GetGeometry()->RenderHField(m_pHField,m_dwObjectIndex,m_dwAlphaFlag,m_dwRenderFlag);
}

DWORD __stdcall	CGXHFieldMapObject::GetObjectIndexInModel()
{
	return m_dwObjectIndex;
	
}
BOOL _stdcall CGXHFieldMapObject::IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
	*pdwModelIndex = 0xffffffff;
	*pdwObjIndex = 0xffffffff;
	return FALSE;
}

DWORD __stdcall	CGXHFieldMapObject::CreateIVertexList(IVERTEX** ppVertex)
{
	return 0;
}

void __stdcall CGXHFieldMapObject::ReleaseIVertexList(IVERTEX*	pVertex)
{
}
BOOL __stdcall CGXHFieldMapObject::ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc)
{
	return m_pHField->ShadeLightMapObject(m_dwObjectIndex,pv3,dwFacesNum,pLight,pFunc);
}
