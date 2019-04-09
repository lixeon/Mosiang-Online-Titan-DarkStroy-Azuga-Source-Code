#pragma once

#include "../4DyuchiGRX_Common/typedef.h"
#include "../4DyuchiGXGFunc/global.h"

class CoModel;

#pragma pack(push,4)

struct FILE_PHYSIQUE_HEADER
{
	DWORD				dwVertexNum;
	DWORD				dwTotalBonesNum;
	DWORD				dwAllocBonesNum;

};
#pragma pack(pop)

class CPhysique
{
	DWORD				m_dwVertexNum;
	DWORD				m_dwTotalBonesNum;
	DWORD				m_dwAllocBonesNum;

	PHYSIQUE_VERTEX*	m_pPhyVertexList;
	BONE*				m_pBoneList;
	BOOL				m_bHasTangentVectorList;
	
public:
	DWORD				GetVertexNum() {return m_dwVertexNum;}
	DWORD				ReadFile(void* pFP);
	DWORD				ReadFileOld(void* pFP);
	DWORD				WriteFile(FILE* fp,CoModel* pModel);
	BOOL				CreatePhysiqueVertex(DWORD dwVertexNum,DWORD dwTotalBonesNum);
	BOOL				Initialize(CoModel* pModel);
	void				PhysiqueTransform(BYTE* pVertex,DWORD dwSize,MATRIX4* pMatrixEntry)
	{
		::PhysiqueTransform(pVertex,m_pPhyVertexList,m_dwVertexNum,dwSize,pMatrixEntry);
	}
	void				PhysiqueTransformPosAndNormal(BYTE* pVertex,DWORD dwSize,MATRIX4* pMatrixEntry)
	{
		::PhysiqueTransformPosAndNormal(pVertex,m_pPhyVertexList,m_dwVertexNum,dwSize,pMatrixEntry);
	}
	void				PhysiqueTransformPosAndNormalAndUV(BYTE* pVertex,TVERTEX* ptv,DWORD dwSize,MATRIX4* pMatrixEntry)
	{
		::PhysiqueTransformPosAndNormalAndUV(pVertex,m_pPhyVertexList,ptv,m_dwVertexNum,dwSize,pMatrixEntry);
	}
	
	void				PhysiqueTransformPosAndWriteUV(BYTE* pVertex,TVERTEX* ptv,DWORD dwSize,MATRIX4* pMatrixEntry)
	{
		::PhysiqueTransformPosAndWriteUV(pVertex,m_pPhyVertexList,ptv,m_dwVertexNum,dwSize,pMatrixEntry);
	}
	
	CPhysique();
	~CPhysique();

};
