#include "colmeshobj.h"
#include "global_variable.h"

#include	"../4DyuchiGRX_myself97/CollisionTest/CollisionTestMovingEllipsoidMeetTriangle.h"

CCollisionMeshObject::CCollisionMeshObject()
{
	m_dwFacesNum = 0;
	m_pv3FaceList = NULL;
	
	m_dwProperty[0] = 0;
	m_dwProperty[1] = 0;
	m_dwProperty[2] = 0;
	m_dwProperty[3] = 0;
}


DWORD CCollisionMeshObject::ReadFile(void* pFP,DWORD dwVersion)
{
	DWORD	dwLen = 0;

	dwLen += CBaseObject::ReadFile(pFP,dwVersion);
	FILE_COLLISION_MESH_OBJECT_HEADER*	pHeader = (FILE_COLLISION_MESH_OBJECT_HEADER*)&this->m_dwFacesNum;
	dwLen += g_pFileStorage->FSRead(pFP,pHeader,sizeof(FILE_COLLISION_MESH_OBJECT_HEADER));

	m_pv3FaceList = new VECTOR3[m_dwFacesNum*3];
	m_pPlaneList = new PLANE[m_dwFacesNum];

	dwLen += g_pFileStorage->FSRead(pFP,m_pv3FaceList,sizeof(VECTOR3)*m_dwFacesNum*3);
	dwLen += g_pFileStorage->FSRead(pFP,m_pPlaneList,sizeof(PLANE)*m_dwFacesNum);


	return dwLen;
	

}

BOOL CCollisionMeshObject::Render(I4DyuchiGXRenderer* pRenderer,DWORD dwFlag)
{
	return  pRenderer->RenderTriVector3(m_pv3FaceList,m_dwFacesNum,0);
}
void CCollisionMeshObject::ReleaseFaceList()
{
	if (m_pv3FaceList)
	{
		delete []m_pv3FaceList;
		m_pv3FaceList = NULL;
	}
	if (m_pPlaneList)
	{
		delete [] m_pPlaneList;
		m_pPlaneList = NULL;
	}
}

CCollisionMeshObject::~CCollisionMeshObject()
{
	ReleaseFaceList();

}

void CCollisionMeshObject::GetProperty( DWORD* pdwOutProperty)
{
	DWORD	i;
	for( i = 0; i < 4; i++)
	{
		pdwOutProperty[i]	=	m_dwProperty[i];
	}
}

