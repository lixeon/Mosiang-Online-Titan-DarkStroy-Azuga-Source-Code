#ifndef GXEVENT_TRIGGER_OBJECT_H
#define GXEVENT_TRIGGER_OBJECT_H

#include "../4DyuchiGRX_Common/IRenderer.h"
#include "../4DyuchiGRX_Common/IGeometry.h"
#include "GXMapObject.h"
#include "executive.h"

class CoGXEventTrigger : public CGXMapObject
{
	
	COLLISION_MESH_OBJECT_DESC	m_colMeshDesc;
	PLANE						m_pPlane[6];				// 충돌 메쉬 객체 기술자와 같은 면 집합. 면의 윗면은 중심에서 바깥쪽으로 향해있다.
	GXSchedulePROC				m_pProc;
	EVENT_TRIGGER_DESC			m_EventTriggerDesc;
	VECTOR3						m_v3DefaultLength;
	
	void				Cleanup();
public:
	CoGXEventTrigger();

	BOOL				Initialize(CoExecutive* pExecutive,GXSchedulePROC pProc,DWORD dwFlag);
	void				SetPosition(VECTOR3* pv3Pos);
	void				SetRotation(VECTOR3* pv3Rot);
	void				SetScale(VECTOR3* pv3Scale);
	void				GetEventTriggerDesc(EVENT_TRIGGER_DESC* pEVDesc);
	void				DefaultProc();
	void				BuildMesh();
	DWORD				Release();

	GXSchedulePROC		GetProc(){	return	m_pProc;}

	PLANE*				GetPlanes(){return	m_pPlane;}
	
	COLLISION_MESH_OBJECT_DESC*	__stdcall	GetCollisionMeshObjectDesc();
	DWORD						__stdcall	OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2);
	DWORD						__stdcall	GetObjectIndexInModel();
	BOOL						__stdcall	Render();
	DWORD						__stdcall	CreateIVertexList(IVERTEX** ppVertex);
	void						__stdcall	ReleaseIVertexList(IVERTEX*	pVertex);
	BOOL						__stdcall	ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc);

	BOOL						__stdcall	IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
};

#endif
