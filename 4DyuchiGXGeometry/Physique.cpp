#include "physique.h"
#include "Model.h"
#include "../4DyuchiGXGFunc/global.h"
#include "global_variable.h"

CPhysique::CPhysique()
{
	memset(this,0,sizeof(CPhysique));
}

BOOL CPhysique::CreatePhysiqueVertex(DWORD dwVertexNum,DWORD dwTotalBonesNum)
{
	m_dwVertexNum = dwVertexNum;
	m_pPhyVertexList = new PHYSIQUE_VERTEX[dwVertexNum];
	memset(m_pPhyVertexList,0,sizeof(PHYSIQUE_VERTEX)*dwVertexNum);

	m_dwTotalBonesNum = dwTotalBonesNum;
	m_pBoneList = new BONE[dwTotalBonesNum];
	memset(m_pBoneList,0,sizeof(BONE)*dwTotalBonesNum);
	
	return TRUE;
}

BOOL CPhysique::Initialize(CoModel* pModel)
{
	BOOL	bResult = FALSE;

	DWORD	i;
	if (!m_dwVertexNum)
		goto lb_return;


	for (i=0; i<m_dwTotalBonesNum; i++)
	{
		m_pBoneList[i].pBone = (void*)pModel->SelectObject((DWORD)m_pBoneList[i].pBone);
	}

	for (i=0; i<m_dwVertexNum; i++)
	{
		

		m_pPhyVertexList[i].pBoneList = m_pBoneList + (DWORD)m_pPhyVertexList[i].pBoneList;
		/*
		matInverse = ((CBaseObject*)m_pPhyVertexList[i].pBoneList[0].pBone)->GetTM()->mat4Inverse;
		matInverse._41 = 0.0f;
		matInverse._42 = 0.0f;
		matInverse._43 = 0.0f;

		TransformVector3_VPTR2(
			&pv3NormalLocal[i],
			&pv3NormalWorld[i],
			&matInverse,1);
*/
//		for (j=0; j<m_pPhyVertexList[i].bBonesNum; j++)
//		{
//			TransformVector3_VPTR2(
//				&m_pPhyVertexList[i].pBoneList[j].v3Offset,
//				&pv3World[i],
//				&((CBaseObject*)m_pPhyVertexList[i].pBoneList[j].pBone)->GetTM()->mat4Inverse,1);
//		}
	}
	
	for (i=0; i<m_dwTotalBonesNum; i++)
	{
		m_pBoneList[i].pBone = (void*)((CBaseObject*)m_pBoneList[i].pBone)->GetIndex();
	}
	bResult = TRUE;

lb_return:
	return bResult;

}

/*
BOOL CPhysique::Build(VECTOR3* pv3World,VECTOR3* pv3NormalWorld)
{
	BOOL	bResult = FALSE;

	DWORD	i,j;
	if (!m_dwVertexNum)
		goto lb_return;

	MATRIX4		matInverse;

	for (i=0; i<m_dwVertexNum; i++)
	{
		for (j=0; j<m_pPhyVertexList[i].bBonesNum; j++)
		{
			
			char*	szObjName;
			szObjName = ((INode*)m_pPhyVertexList[i].pBoneList[j].pBone)->GetName();
			CBaseObject* pObj = g_pScene->SelectObject(szObjName);
			if (!pObj)
			{
				char	txt[512];
				wsprintf(txt,"오브젝트를 찾을 수 없습니다.해당 오브젝트가 더미오브젝트인지 확인해주세요.: %s",szObjName);
				MessageBox(NULL,txt,"Error",MB_OK);
				__asm int 3
			}
			m_pPhyVertexList[i].pBoneList[j].pBone = pObj;

			matInverse = pObj->GetTM()->mat4Inverse;
			matInverse._41 = 0.0f;
			matInverse._42 = 0.0f;
			matInverse._43 = 0.0f;

		
			TransformVector3_VPTR2(
				&m_pPhyVertexList[i].pBoneList[j].v3NormalOffset,
				&pv3NormalWorld[i],
				&matInverse,1);
		}
	}
	bResult = TRUE;

lb_return:
	return bResult;

}*/



DWORD CPhysique::WriteFile(FILE* fp,CoModel* pModel)
{
		

	// 익스텐디드 버텍스를 처리한다.
	// 본리스트를 그대로 파일에 출력
	// 블랜딩 여부에 상관없이 버텍스 순서로 본이 배열되어있다.
	// 피지크버텍스의 본 포인터에는 본 오브젝트 인덱스를 넣는다.
	DWORD	i;	

	DWORD	oldPos = ftell(fp);


	FILE_PHYSIQUE_HEADER* pHeader = (FILE_PHYSIQUE_HEADER*)this;
	fwrite(pHeader,sizeof(FILE_PHYSIQUE_HEADER),1,fp);

	PHYSIQUE_VERTEX		phyVer;
	for (i=0; i<m_dwVertexNum; i++)
	{
		phyVer.bBonesNum = m_pPhyVertexList[i].bBonesNum;
		phyVer.pBoneList = (BONE*)( m_pPhyVertexList[i].pBoneList - m_pBoneList );

		fwrite(&phyVer,sizeof(PHYSIQUE_VERTEX),1,fp);
	}
	fwrite(m_pBoneList,sizeof(BONE),m_dwTotalBonesNum,fp);

	return (ftell(fp)-oldPos);
}

DWORD CPhysique::ReadFile(void* pFP)
{
	// 본리스트를 그대로 파일에 출력
	// 블랜딩 여부에 상관없이 버텍스 순서로 본이 배열되어있다.
	// 피지크버텍스의 본 포인터에는 본 오브젝트 인덱스를 넣는다.

	DWORD	dwLen = 0;
	FILE_PHYSIQUE_HEADER	header;
	

	dwLen += g_pFileStorage->FSRead(pFP,&header,sizeof(FILE_PHYSIQUE_HEADER));
	if (header.dwVertexNum)
	{
		CreatePhysiqueVertex(header.dwVertexNum,header.dwTotalBonesNum);
		dwLen += g_pFileStorage->FSRead(pFP,m_pPhyVertexList,sizeof(PHYSIQUE_VERTEX)*header.dwVertexNum);
		dwLen += g_pFileStorage->FSRead(pFP,m_pBoneList,sizeof(BONE)*header.dwTotalBonesNum);
	}

	return dwLen;
	
	
}

DWORD CPhysique::ReadFileOld(void* pFP)
{
	// 본리스트를 그대로 파일에 출력
	// 블랜딩 여부에 상관없이 버텍스 순서로 본이 배열되어있다.
	// 피지크버텍스의 본 포인터에는 본 오브젝트 인덱스를 넣는다.

	DWORD	dwLen = 0;
	FILE_PHYSIQUE_HEADER	header;
	

	dwLen += g_pFileStorage->FSRead(pFP,&header,sizeof(FILE_PHYSIQUE_HEADER));
	if (header.dwVertexNum)
	{
		BONE_OLD*	pBoneOld = new BONE_OLD[header.dwTotalBonesNum];
		
		CreatePhysiqueVertex(header.dwVertexNum,header.dwTotalBonesNum);
		dwLen += g_pFileStorage->FSRead(pFP,m_pPhyVertexList,sizeof(PHYSIQUE_VERTEX)*header.dwVertexNum);
		dwLen += g_pFileStorage->FSRead(pFP,pBoneOld,sizeof(BONE_OLD)*header.dwTotalBonesNum);

		for (DWORD i=0; i<header.dwTotalBonesNum; i++)
		{
			*(BONE_OLD*)(m_pBoneList+i) = pBoneOld[i];
		}
		delete [] pBoneOld;
	}

	return dwLen;
	
	
}
CPhysique::~CPhysique()
{
	if (m_pBoneList)
	{
		delete [] m_pBoneList;
		m_pBoneList = NULL;
	}
	if (m_pPhyVertexList)
	{
		delete [] m_pPhyVertexList;
		m_pPhyVertexList = NULL;
	}
}

/*
void PhysiqueTransform(BYTE* pVertexResult,PHYSIQUE_VERTEX* pPhyVertex,DWORD dwVertexNum,DWORD dwSize,MATRIX4* pMatrixEntry)
{
	VECTOR3	v3,v3Result;

	DWORD	i,j;

	for (i=0; i<dwVertexNum; i++)
	{
		v3Result.x = 0.0f;
		v3Result.y = 0.0f;
		v3Result.z = 0.0f;

		for (j=0; j<pPhyVertex[i].bBonesNum; j++)
		{
			TransformVector3_VPTR2(
				&v3,
				&pPhyVertex[i].pBoneList[j].v3Offset,
				pMatrixEntry + (DWORD)pPhyVertex[i].pBoneList[j].pBone,1);

			v3.x *= pPhyVertex[i].pBoneList[j].fWeight;
			v3.y *= pPhyVertex[i].pBoneList[j].fWeight;
			v3.z *= pPhyVertex[i].pBoneList[j].fWeight;

			VECTOR3_ADDEQU_VECTOR3(&v3Result,&v3);
			
		}
		*(VECTOR3*)pVertexResult = v3Result;

		pVertexResult += dwSize;
	}
}*/