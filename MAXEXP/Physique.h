#pragma once

#include "../max_common/stdafx.h"
#include "../4DyuchiGRX_Common/typedef.h"

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
	DWORD				m_dwOrigVertexNum;
	DWORD				m_dwExtVertexNum;
	DWORD*				m_pdwExtVertexIndexList;

	
public:

	BOOL				Build(VECTOR3* pv3World,VECTOR3* pv3NormalWorld,VECTOR3* pv3TangentU);
	BOOL				CreatePhysiqueVertex(DWORD dwVertexNum,DWORD dwTotalBonesNum,DWORD dwExtVertexNum,DWORD* pdwExtVertexIndex);
	BOOL				CreateBoneList(DWORD dwVertexIndex,BYTE bBonesNum);
	void				SetBone(DWORD dwVertexIndex,DWORD dwBoneIndex,void* pNode,float fWeight,VECTOR3* pv3Offset);
	
	DWORD				WriteFile(FILE* fp);
	CPhysique();
	~CPhysique();

};
