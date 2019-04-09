#include "bone_object.h"
#include "model.h"
#include "geom_obj.h"
#include "../4DyuchiGXGFunc/global.h"
#include "global_variable.h"


CBoneObject::CBoneObject()
{
	m_objType = OBJECT_TYPE_BONE;

}

DWORD CBoneObject::ReadFile(void* pFP,DWORD dwVersion)
{
	DWORD	dwLen = 0;

	dwLen += CBaseObject::ReadFile(pFP,dwVersion);
//	if (lstrcmp(m_szObjName,
	COLLISION_MESH_OBJECT_DESC_SAVELOAD	colMeshSaveLoad;

	dwLen += g_pFileStorage->FSRead(pFP,&colMeshSaveLoad,sizeof(colMeshSaveLoad));

	memcpy(&m_colMeshDesc.boundingBox,&colMeshSaveLoad.boundingBox,sizeof(m_colMeshDesc.boundingBox));
	memcpy(&m_colMeshDesc.boundingSphere,&colMeshSaveLoad.boundingSphere,sizeof(m_colMeshDesc.boundingSphere));
	m_colMeshDesc.dwObjIndex = colMeshSaveLoad.dwObjIndex;
	
	return dwLen;
}
BOOL CBoneObject::Initialize(CoModel* pModel)
{
	m_pModel = pModel;

/*	
	// offset = 월드 버텍스 * 뼈대 오브젝트 인버스 매트릭스 
	for (DWORD i=0; i<m_dwPhyVertexSetNum; i++)
	{
		m_pPhyVertexSetList[i].pPrimitiveObject = (CMeshObject*)pModel->SelectObject(m_pPhyVertexSetList[i].dwPrimitiveObjectIndex);
		for (DWORD j=0; j<m_pPhyVertexSetList[i].dwBindedVertexNum; j++)
		{
			m_pPhyVertexSetList[i].pPhysiqueVertexList[j].pv3ResultOut = 
				m_pPhyVertexSetList[i].pPrimitiveObject->GetVertexWorldPtr() +
				(DWORD)m_pPhyVertexSetList[i].pPhysiqueVertexList[j].pv3ResultOut;
			
			TransformVector3_VPTR2(&m_pPhyVertexSetList[i].pPhysiqueVertexList[j].v3Offset,
				m_pPhyVertexSetList[i].pPhysiqueVertexList[j].pv3ResultOut,
				&m_TM.mat4Inverse,1);
		}
	}*/
	TransformVector3_VPTR1(&m_colMeshDesc.boundingSphere.v3Point,&m_TM.mat4Inverse,1);
	TransformVector3_VPTR1(m_colMeshDesc.boundingBox.v3Oct,&m_TM.mat4Inverse,8);


	return TRUE;
}
/*
BOOL CBoneObject::PhysiqueTransform()
{
	for (DWORD i=0; i<m_dwPhyVertexSetNum; i++)
	{
		TransformPhysiqueVertex(m_pPhyVertexSetList[i].pPhysiqueVertexList,&m_matResult,m_pPhyVertexSetList[i].dwBindedVertexNum);
	}

	return TRUE;
}
*/

CBoneObject::~CBoneObject()
{
	CBaseObject::~CBaseObject();
}