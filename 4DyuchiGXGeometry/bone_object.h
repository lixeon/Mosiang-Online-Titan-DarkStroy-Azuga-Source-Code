#pragma once

#include "object.h"
#include "../4DyuchiGRX_common/typedef.h"

class CoModel;

class CBoneObject : public CBaseObject
{
	COLLISION_MESH_OBJECT_DESC	m_colMeshDesc;
	
public:
	COLLISION_MESH_OBJECT_DESC*		GetColMeshObjectDesc() {return &m_colMeshDesc;}
	BOOL							Initialize(CoModel* pModel);

	DWORD					ReadFile(void* pFP,DWORD dwVersion);
	CBoneObject();
	~CBoneObject();

};
