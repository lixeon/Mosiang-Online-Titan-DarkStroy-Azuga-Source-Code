#pragma once

#include "../max_common/stdafx.h"
#include "phyexp.h"
#include "object.h"

class CBoneObject : public CBaseObject
{
	COLLISION_MESH_OBJECT_DESC_SAVELOAD	m_colMeshDesc;
	
	
public:
	void						SetColMeshDesc(COLLISION_MESH_OBJECT_DESC_SAVELOAD* pColMeshDesc) {m_colMeshDesc = *pColMeshDesc;}
	DWORD						WriteFile(FILE* fp);
	CBoneObject();
	virtual				~CBoneObject();

};
