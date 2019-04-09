#pragma once

#include "object.h"
#include "../4DyuchiGRX_Common/mesh_flag.h"
#include "physique.h"

class CScene;

#pragma pack(push,4)

struct	FILE_MESH_HEADER
{
	DWORD				dwMaxVertexNum;
	DWORD				dwVertexNum;
	DWORD				dwOriginalVertexNum;
	DWORD				dwExtVertexNum;
	DWORD				dwTexVertexNum;
	DWORD				dwMtlIndex;
	DWORD				dwFaceGroupNum;
	CMeshFlag			meshFlag;
	DWORD				dwGridIndex;
	VECTOR3				v3Dir;

};
#pragma pack(pop)


class CMeshObject : public CBaseObject
{
	DWORD				m_dwMaxVertexNum;
	DWORD				m_dwVertexNum;
	DWORD				m_dwOriginalVertexNum;
	DWORD				m_dwExtVertexNum;
	DWORD				m_dwTexVertexNum;
	DWORD				m_dwMtlIndex;
	DWORD				m_dwFaceGroupNum;
	CMeshFlag			m_meshFlag;
	DWORD				m_dwGridIndex;
	VECTOR3				m_v3Dir;



	VECTOR3*			m_pv3Point;
	TVERTEX*			m_ptUVW;
	VECTOR3*			m_pv3Normal;
	VECTOR3*			m_pv3TangentU;
	CFaceGroup*			m_pBaseFaceGroup;
	
	DWORD				m_dwTotalFacesNum;
	DWORD*				m_pdwExtVertexIndex;
	CPhysique			m_Physique;

public:	
//	DWORD				SetVertexNormal(VECTOR3* pv3NormalResult,VECTOR3* pv3VertexList,DWORD dwVertexNum);
	DWORD				SetVertexTangentU(VECTOR3* pv3TangentUResult,VECTOR3* pv3VertexList,TVERTEX* ptvList,DWORD dwVertexNum);
	CPhysique*			GetPhysique()	{return &m_Physique;}
	BOOL				CreatePhysique(INode* pNode);
	void				SetDirectionVector(VECTOR3* pv3Dir) {m_v3Dir = *pv3Dir;}
	DWORD				CreateVertexList(VECTOR3** ppVertexList);
	void				ReleaseVertexList(VECTOR3* pVertexList);
	DWORD*				GetExtVertexNumPtr()	{return &m_dwExtVertexNum;}
	DWORD**				GetExtVertexPtrPtr()	{return &m_pdwExtVertexIndex;}
	void				SetOriginalVertexNum(DWORD dwNum) {m_dwOriginalVertexNum = dwNum;}
	
	void				SetTotalFacesNum(DWORD dwNum) {m_dwTotalFacesNum = dwNum;}
	DWORD				GetTotalFacesNum() {return m_dwTotalFacesNum;}
	CMeshFlag*			GetMeshFlagPtr() {return &m_meshFlag;}
	void				SetMeshFlag(CMeshFlag flag) {m_meshFlag = flag;}
	void				SetMtlIndex(DWORD index) { m_dwMtlIndex = index;}
	DWORD				GetMtlIndex() {return m_dwMtlIndex;}
	CFaceGroup*			AllocFaceGroup();
	CFaceGroup*			GetFaceGroup(DWORD dwMtlIndex);
	DWORD				GetVertexNum() {return m_dwVertexNum;}
	
	VECTOR3*			CreateVertexList(DWORD dwNum);
	TVERTEX*			CreateTexVertexList(DWORD dwNum);
	VECTOR3*			CreateNormalVertexList(VECTOR3* pv3Normal);
	VECTOR3*			CreateTangentULIst();

	BOOL				InsertVertex(VECTOR3* pv3);
	BOOL				InsertTexVertex(TVERTEX* ptUVW);
	
	void				ReleaseVertexList();
	void				ReleaseTexVertexList();
	void				ReleaseFaceGroups();
	

	DWORD				WriteFile(FILE* fp);

	CMeshObject();
	~CMeshObject();
};