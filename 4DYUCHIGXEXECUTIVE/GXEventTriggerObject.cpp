#include "GXEventTriggerObject.h"

#include "../4DYUCHIGRX_MYSELF97/Myself97Common.h"

CoGXEventTrigger::CoGXEventTrigger()
{
}
void CoGXEventTrigger::Cleanup()
{

}
BOOL CoGXEventTrigger::Initialize(CoExecutive* pExecutive,GXSchedulePROC pProc,DWORD dwFlag)
{
	CGXMapObject::Cleanup();
	m_pExecutive = pExecutive;
	m_EventTriggerDesc.dwColor = 0xffffffff;
	SET_VECTOR3(&m_EventTriggerDesc.v3Pos,0.0f);
	SET_VECTOR3(&m_EventTriggerDesc.v3Rot,0.0f);
	SET_VECTOR3(&m_EventTriggerDesc.v3Scale,1.0f);
	SET_VECTOR3(&m_v3DefaultLength,100.0f);
	BuildMesh();

	m_dwID = 0xfffffff4;
	m_pHashHandle = NULL;
	m_dwPropertyFlag = dwFlag;
	m_dwObjectType = GX_MAP_OBJECT_TYPE_TRIGGER;

	m_pProc	=	pProc;

	return TRUE;
}
void CoGXEventTrigger::SetPosition(VECTOR3* pv3Pos)
{
	m_EventTriggerDesc.v3Pos = *pv3Pos;
	BuildMesh();
}
void CoGXEventTrigger::SetRotation(VECTOR3* pv3Rot)
{
	m_EventTriggerDesc.v3Rot = *pv3Rot;
	BuildMesh();
}
void CoGXEventTrigger::SetScale(VECTOR3* pv3Scale)
{
	m_EventTriggerDesc.v3Scale = *pv3Scale;
	BuildMesh();
}
void CoGXEventTrigger::BuildMesh()
{
	MATRIX4		matScale;
	MATRIX4		matPos;
	MATRIX4		matRot,matRotX,matRotY,matRotZ;
	MATRIX4		matWorld;

	VECTOR3		v3PointList[2];
	v3PointList[0] = m_v3DefaultLength;
	VECTOR3_MUL_FLOAT(v3PointList+1,&m_v3DefaultLength,-1.0f);

	CalcBoundingMesh(v3PointList,2,&m_colMeshDesc);
	
	SetScaleMatrix(&matScale,&m_EventTriggerDesc.v3Scale);

	SetRotationXMatrix(&matRotX,m_EventTriggerDesc.v3Rot.x);
	SetRotationYMatrix(&matRotY,m_EventTriggerDesc.v3Rot.y);
	SetRotationZMatrix(&matRotZ,m_EventTriggerDesc.v3Rot.z);
		
	MatrixMultiply2(&matRot,&matRotX,&matRotY);
	MatrixMultiply2(&matRot,&matRot,&matRotZ);

	TranslateMatrix(&matPos,&m_EventTriggerDesc.v3Pos);
	
	MatrixMultiply2(&matWorld,&matRot,&matPos);
	MatrixMultiply2(&matWorld,&matScale,&matWorld);

	TransformVector3_VPTR1(m_colMeshDesc.boundingBox.v3Oct,&matWorld,8);
	TransformVector3_VPTR1(&m_colMeshDesc.boundingSphere.v3Point,&matWorld,1);

	CGXMap*	pMap = m_pExecutive->GetGXMap();
	if (pMap)
	{
		if (m_dwClipperIndex != 0xffffffff)
			pMap->UpdateDynamicObject(m_dwClipperIndex);
	}
	
	// 트리거와 개체의 충돌을 위해 면을 계산해놓는다.

	// 인덱스 순서.
	// 326	x
	// 451	-x
	// 034	y
	// 561	-y
	// 012	z
	// 654	-z
	DWORD	PlaneIndex[18]=	{ 3, 2, 6, 4, 5, 1, 0, 3, 4, 5, 6, 1, 0, 1, 2, 6, 5, 4	};

	DWORD	i;
	for( i = 0; i < 6; i++)
	{
		VECTOR3	v3Tri[3];
		v3Tri[0] = m_colMeshDesc.boundingBox.v3Oct[PlaneIndex[i*3+0]];
		v3Tri[1] = m_colMeshDesc.boundingBox.v3Oct[PlaneIndex[i*3+1]];
		v3Tri[2] = m_colMeshDesc.boundingBox.v3Oct[PlaneIndex[i*3+2]];

		CalcPlaneEquation(&m_pPlane[i],v3Tri);
//		MakePLANE( &m_pPlane[i], &m_colMeshDesc.boundingBox.v3Oct[PlaneIndex[i*3+0]], &m_colMeshDesc.boundingBox.v3Oct[PlaneIndex[i*3+1]], &m_colMeshDesc.boundingBox.v3Oct[PlaneIndex[i*3+2]]);
	}

}
void CoGXEventTrigger::GetEventTriggerDesc(EVENT_TRIGGER_DESC* pEVDesc)
{
	*pEVDesc = m_EventTriggerDesc;
}
void CoGXEventTrigger::DefaultProc()
{
}
DWORD CoGXEventTrigger::Release()
{
	CGXMap*	pMap = m_pExecutive->GetGXMap();
	if (pMap)
	{
		if (m_dwIndexInInitialTable != 0xffffffff)
			pMap->DeleteInitialGXTrigger(m_dwIndexInInitialTable);

		if (m_dwClipperIndex != 0xffffffff)
			pMap->DeleteDynamicObject(m_dwClipperIndex);
	}

	Cleanup();
	CGXMapObject::Cleanup();
	return 0;
}

COLLISION_MESH_OBJECT_DESC*	__stdcall CoGXEventTrigger::GetCollisionMeshObjectDesc()
{
	return &m_colMeshDesc;
}
DWORD __stdcall CoGXEventTrigger::OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2)
{
	return 0;
}
DWORD __stdcall	CoGXEventTrigger::GetObjectIndexInModel()
{
	return 0xffffffff;
}
BOOL __stdcall CoGXEventTrigger::Render()
{
	m_pExecutive->INL_GetRenderer()->RenderBox(m_colMeshDesc.boundingBox.v3Oct,m_EventTriggerDesc.dwColor);
	return TRUE;
}
DWORD __stdcall CoGXEventTrigger::CreateIVertexList(IVERTEX** ppVertex)
{
	return 0;
}
void __stdcall CoGXEventTrigger::ReleaseIVertexList(IVERTEX* pVertex)
{

}
BOOL __stdcall CoGXEventTrigger::ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc)
{
	return FALSE;
}

BOOL __stdcall CoGXEventTrigger::IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
	MATRIX4	matView,matProj;
	WORD	wWidth,wHeight;
	SHORT_RECT		rect;

	m_pExecutive->INL_GetGeometry()->GetViewMatrix(&matView,dwViewportIndex);
	m_pExecutive->INL_GetGeometry()->GetProjectionMatrix(&matProj,dwViewportIndex);
	m_pExecutive->INL_GetGeometry()->GetViewport(&rect,&wWidth,&wHeight,dwViewportIndex);
	
	*pdwModelIndex = 0xffffffff;
	*pdwObjIndex = 0xffffffff;

	return IsCollisionMeshWithScreenCoord(pv3IntersectPoint,pfDist,&m_colMeshDesc,&matView,&matProj,ptCursor,&rect);
	
}