#pragma once

#include "object.h"
#include "CollisionMeshObjectTree.h"

class CScene;

#pragma pack(push,4)


struct FILE_COLLISION_MESH_OBJECT_HEADER
{
	DWORD				dwFacesNum;
	DWORD				dwProperty[4];
};
#pragma pack(pop)


class CCollisionMeshObject : public CBaseObject
{
	DWORD				m_dwFacesNum;
	DWORD				m_dwProperty[4];
	

	VECTOR3*			m_pv3FaceList;
	PLANE*				m_pPlaneList;

public:	
	BOOL				Render(I4DyuchiGXRenderer* pRenderer,DWORD dwFlag);
	
	DWORD				ReadFile(void* pFP,DWORD dwVersion);
	void				ReleaseFaceList();

	DWORD				GetFaceCount(void){ return m_dwFacesNum;}
	VECTOR3*			GetTriBuffer(void){ return m_pv3FaceList;}
	PLANE*				GetPlaneBuffer(void){ return m_pPlaneList;}
	void				GetProperty( DWORD* pdwOutProperty);

	CCollisionMeshObject();
	~CCollisionMeshObject();
};
