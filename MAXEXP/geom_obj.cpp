#include "geom_obj.h"
#include "scene.h"
#include "phyexp.h"
#include "phymod.h"
#include "../max_common/helper.h"

CMeshObject::CMeshObject()
{
	m_dwExtVertexNum = 0;
	m_pdwExtVertexIndex = 0;
	m_pBaseFaceGroup = NULL;
	
	m_dwFaceGroupNum = 0;
	m_dwVertexNum = 0;


	m_dwTexVertexNum = 0;
	m_dwMaxVertexNum = 0;

	m_pv3TangentU = NULL;
	m_pv3Point = NULL;
	m_pv3Normal = NULL;
	m_ptUVW = NULL;
	
	m_dwMtlIndex = 0xffffffff;
	m_dwGridIndex = 0xffffffff;

}
CFaceGroup* CMeshObject::AllocFaceGroup()
{
	CFaceGroup*		pFaceGroup = new CFaceGroup;
	memset(pFaceGroup,0,sizeof(CFaceGroup));
	
	pFaceGroup->m_pNextFaceGroup = m_pBaseFaceGroup;
	m_pBaseFaceGroup = pFaceGroup;

	m_dwFaceGroupNum++;

	return pFaceGroup;
}
VECTOR3* CMeshObject::CreateVertexList(DWORD dwNum)
{
	m_pv3Point = new VECTOR3[dwNum];
	m_dwMaxVertexNum = dwNum;

	return m_pv3Point;
}
TVERTEX* CMeshObject::CreateTexVertexList(DWORD dwNum)
{
	m_ptUVW = new TVERTEX[dwNum];
	return m_ptUVW;
}
VECTOR3* CMeshObject::CreateNormalVertexList(VECTOR3* pv3Normal)
{
	m_pv3Normal = new VECTOR3[m_dwVertexNum];
	if (pv3Normal)
	{
        memcpy(m_pv3Normal,pv3Normal,sizeof(VECTOR3)*m_dwOriginalVertexNum);
		for (DWORD i=0; i<m_dwExtVertexNum; i++)
		{
            *(m_pv3Normal+m_dwOriginalVertexNum + i) = pv3Normal[m_pdwExtVertexIndex[i]];
		}


	}
//	SetVertexNormal(m_pv3Normal,m_pv3Point,m_dwVertexNum);
	return m_pv3Normal;
}

VECTOR3* CMeshObject::CreateTangentULIst()
{
    m_pv3TangentU = new VECTOR3[m_dwVertexNum];
    SetVertexTangentU(m_pv3TangentU,m_pv3Point,m_ptUVW,m_dwVertexNum);
	return m_pv3TangentU;
}
BOOL CMeshObject::InsertVertex(VECTOR3* pv3)
{
	BOOL		bResult = FALSE;

	if (m_dwVertexNum >= m_dwMaxVertexNum)
		goto lb_return;

	m_pv3Point[m_dwVertexNum] = *pv3;
	m_dwVertexNum++;

lb_return:
	return bResult;
}
BOOL CMeshObject::InsertTexVertex(TVERTEX* ptUVW)
{
	BOOL		bResult = FALSE;

	if (m_dwTexVertexNum >= m_dwMaxVertexNum)
		goto lb_return;

	m_ptUVW[m_dwTexVertexNum] = *ptUVW;
	m_dwTexVertexNum++;

lb_return:
	return bResult;
}


CFaceGroup* CMeshObject::GetFaceGroup(DWORD dwUniqueIndex)
{
	CFaceGroup*		pCur = m_pBaseFaceGroup;
	while (pCur)
	{
		if (dwUniqueIndex == pCur->GetUniqueIndex())
			goto lb_return;

		pCur = pCur->m_pNextFaceGroup;
	}
lb_return:
	return pCur;
}
void CMeshObject::ReleaseVertexList()
{
	if (m_pv3Point)
	{
		delete [] m_pv3Point;
		m_pv3Point = NULL;
		m_dwVertexNum = 0;
	}
}

void CMeshObject::ReleaseTexVertexList()
{
	if (m_ptUVW)
	{
		delete [] m_ptUVW;
		m_ptUVW = NULL;
		m_dwTexVertexNum = 0;
	}
}
void CMeshObject::ReleaseFaceGroups()
{
	CFaceGroup*		pCur = m_pBaseFaceGroup;
	CFaceGroup*		pNext;

	while(pCur)
	{
		pNext = pCur->m_pNextFaceGroup;
		delete pCur;
		pCur = pNext;
	}
	m_pBaseFaceGroup = NULL;
	m_dwFaceGroupNum = 0;
}
/*
DWORD CMeshObject::SetVertexNormal(VECTOR3* pv3NormalResult,VECTOR3* pv3VertexList,DWORD dwVertexNum)
{
	DWORD	i;
	memset(pv3NormalResult,0,sizeof(VECTOR3)*dwVertexNum);
	
	CFaceGroup*		pCur = m_pBaseFaceGroup;
	

	while(pCur)
	{
		SetFaceNormalToVertex(pv3NormalResult,pv3VertexList,dwVertexNum,pCur->GetFacesPtr(),pCur->GetFacesNum());
		pCur = pCur->m_pNextFaceGroup;

	}
	DWORD	dwCount = 0;

	for (i=0; i<dwVertexNum; i++)
	{
		if (!(pv3NormalResult[i].x == 0.0f && 
			pv3NormalResult[i].y == 0.0f && 
			pv3NormalResult[i].z == 0.0f)) 
		{

			Normalize(&pv3NormalResult[i],&pv3NormalResult[i]);
			dwCount++;
		}
	}
	return dwCount;
}
*/
DWORD CMeshObject::SetVertexTangentU(VECTOR3* pv3TangentUResult,VECTOR3* pv3VertexList,TVERTEX* ptvList,DWORD dwVertexNum)
{
	DWORD	i;
	memset(pv3TangentUResult,0,sizeof(VECTOR3)*dwVertexNum);
	
	CFaceGroup*		pCur = m_pBaseFaceGroup;
	while(pCur)
	{
		SetFaceTangentToVertex(pv3TangentUResult,pv3VertexList,ptvList,dwVertexNum,pCur->GetFacesPtr(),pCur->GetFacesNum());
		pCur = pCur->m_pNextFaceGroup;
	}
	DWORD	dwCount = 0;

	for (i=0; i<dwVertexNum; i++)
	{
		if (!(pv3TangentUResult[i].x == 0.0f && 
			pv3TangentUResult[i].y == 0.0f && 
			pv3TangentUResult[i].z == 0.0f)) 
		{

			Normalize(&pv3TangentUResult[i],&pv3TangentUResult[i]);
			dwCount++;
		}
	}
	return dwCount;
}
DWORD CMeshObject::WriteFile(FILE* fp)
{
	
	DWORD	oldPos = ftell(fp);
	
	FILE_MESH_HEADER*	pHeader = (FILE_MESH_HEADER*)&(this->m_dwMaxVertexNum);
	CBaseObject::WriteFile(fp);
	fwrite(pHeader,sizeof(FILE_MESH_HEADER),1,fp);		// 헤더부터 쓰고...
	
	if (m_dwVertexNum)
		fwrite(m_pv3Point,sizeof(VECTOR3),m_dwVertexNum,fp);
	
	if (m_dwTexVertexNum)
		fwrite(m_ptUVW,sizeof(TVERTEX),m_dwTexVertexNum,fp);

	if (m_dwExtVertexNum)
		fwrite(m_pdwExtVertexIndex,sizeof(DWORD),m_dwExtVertexNum,fp);


	CFaceGroup* pCur = m_pBaseFaceGroup;
	for (DWORD i=0; i<m_dwFaceGroupNum; i++)
	{
		pCur->WriteFile(fp);
		pCur = pCur->m_pNextFaceGroup;
		
	}
	
	m_Physique.Build(m_pv3Point,m_pv3Normal,m_pv3TangentU);
	m_Physique.WriteFile(fp);

	if (m_meshFlag.GetTransformType() != TRANSFORM_TYPE_ALIGN_VIEW)
		fwrite(m_pv3Normal,sizeof(VECTOR3),m_dwVertexNum,fp);
	

	return (ftell(fp)-oldPos);
	
}
BOOL CMeshObject::CreatePhysique(INode* pNode)
{
	INode*				Bone;
	DWORD				dwTotalBonesNum = 0;
	VECTOR3				v3Offset;

	Modifier *phyMod = FindPhysiqueModifier(pNode);
	if (!phyMod)
		return FALSE;
	
	ObjectState os = pNode->EvalWorldState(0);

	// create a Physique Export Interface for the given Physique Modifier
	IPhysiqueExport *phyExport = (IPhysiqueExport *)phyMod->GetInterface(I_PHYINTERFACE);
	
	if (!phyExport)
		return FALSE;

	IPhyContextExport *mcExport = (IPhyContextExport *)phyExport->GetContextInterface(pNode);

	if (!mcExport)
		return FALSE;

	mcExport->ConvertToRigid(TRUE);
	
	for (int i = 0; i < os.obj->NumPoints();  i++)
	{
		
		// 피지크 버텍스 생성
		IPhyVertexExport *vtxExport = mcExport->GetVertexInterface(i);
		//,HIERARCHIAL_VERTEX);
	
		if (vtxExport)
		{

			//need to check if vertex has blending
			if (vtxExport->GetVertexType() & BLENDED_TYPE)
			{
				IPhyBlendedRigidVertex *vtxBlend = (IPhyBlendedRigidVertex *)vtxExport;
				dwTotalBonesNum += vtxBlend->GetNumberNodes();

			}
			else 
			{
				dwTotalBonesNum ++;
				
		
			}
			mcExport->ReleaseVertexInterface(vtxExport);
			vtxExport = NULL;
		}
	}
	if (m_dwVertexNum < os.obj->NumPoints())
		__asm int 3

	m_Physique.CreatePhysiqueVertex(os.obj->NumPoints(),dwTotalBonesNum,m_dwExtVertexNum,m_pdwExtVertexIndex);

	for (i = 0; i < os.obj->NumPoints();  i++)
	{
		
		// 피지크 버텍스 생성
		IPhyVertexExport *vtxExport = mcExport->GetVertexInterface(i);
	
		if (vtxExport)
		{
			//need to check if vertex has blending
			if (vtxExport->GetVertexType() & BLENDED_TYPE)
			{
				IPhyBlendedRigidVertex *vtxBlend = (IPhyBlendedRigidVertex *)vtxExport;
		
				// 피지크 버텍스당 본리스트 생성
				m_Physique.CreateBoneList(i,vtxBlend->GetNumberNodes());

				for (int n = 0; n < vtxBlend->GetNumberNodes(); n++)
				{
					Bone = vtxBlend->GetNode(n);
										
					v3Offset.x = vtxBlend->GetOffsetVector(n).x;
					v3Offset.y = vtxBlend->GetOffsetVector(n).z;
					v3Offset.z = vtxBlend->GetOffsetVector(n).y;

					m_Physique.SetBone(i,n,Bone,vtxBlend->GetWeight(n),&v3Offset);
				}
			}
			else 
			{
				m_Physique.CreateBoneList(i,1);

				IPhyRigidVertex *vtxNoBlend = (IPhyRigidVertex *)vtxExport;
				Bone = vtxNoBlend->GetNode();

				v3Offset.x = vtxNoBlend->GetOffsetVector().x;
				v3Offset.y = vtxNoBlend->GetOffsetVector().z;
				v3Offset.z = vtxNoBlend->GetOffsetVector().y;

				m_Physique.SetBone(i,0,Bone,1.0f,&v3Offset);
			}
			mcExport->ReleaseVertexInterface(vtxExport);
			vtxExport = NULL;	

		}
	}
	phyExport->ReleaseContextInterface(mcExport);
	phyMod->ReleaseInterface(I_PHYINTERFACE, phyExport);
	
	return TRUE;
}
DWORD CMeshObject::CreateVertexList(VECTOR3** ppVertexList)
{
	VECTOR3* p;
	VECTOR3* pv3Src = p = new VECTOR3[m_dwTotalFacesNum*3];
	CFaceGroup* pCur = m_pBaseFaceGroup;

	DWORD	dwCount = 0;
	WORD*	pIndex;
	while(pCur)
	{
		pIndex = pCur->GetFacesPtr();
		for (DWORD i=0; i<pCur->GetFacesNum()*3; i++)
		{
			*p = m_pv3Point[pIndex[i]];
			p++;
			dwCount++;

		}
		pCur = pCur->m_pNextFaceGroup;
	}
	*ppVertexList = pv3Src;
	return dwCount;
}

void CMeshObject::ReleaseVertexList(VECTOR3* pVertexList)
{
	delete [] pVertexList;
}




CMeshObject::~CMeshObject()
{
	ReleaseFaceGroups();
	ReleaseVertexList();
	ReleaseTexVertexList();

	if (m_pv3Normal)
	{
		delete [] m_pv3Normal;
		m_pv3Normal = NULL;
	}
	if (m_pv3TangentU)
	{
		delete [] m_pv3TangentU;
		m_pv3TangentU = NULL;
	}
	if (m_pdwExtVertexIndex)
	{
		delete [] m_pdwExtVertexIndex;
		m_pdwExtVertexIndex = NULL;
		m_dwExtVertexNum = 0;
	}

}
