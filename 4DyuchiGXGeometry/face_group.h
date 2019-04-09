#pragma once

#include "../4DyuchiGRX_common/typedef.h"
#include "material.h"

#pragma pack(push,4)
struct FILE_FACE_GROUP_HEADER
{
	DWORD					dwMtlIndex;
	DWORD					dwIndex;
	DWORD					dwFacesNum;
	DWORD					dwMaxFacesNum;
	DWORD					dwVertexIndexNum;
	DWORD					dwLightUVNum1;
	DWORD					dwLightUVNum2;
};	
#pragma pack(pop)

class CFaceGroup
{
	DWORD					m_dwMtlIndex;
	DWORD					m_dwIndex;
	DWORD					m_dwFacesNum;
	DWORD					m_dwMaxFacesNum;
	DWORD					m_dwVertexIndexNum;
	DWORD					m_dwLightUVNum1;
	DWORD					m_dwLightUVNum2;

	WORD*					m_pFace;
	CMaterial*				m_pMtl;		

	TVERTEX*				m_ptLightUV1;		// 면 수*3으로 순서까지 일치.DELETE필요
	TVERTEX*				m_ptLightUV2;		// 면 수*3으로 순서까지 일치.DELETE필요

	
	
public:
	BOOL					Duplicate(CFaceGroup* pFaceGroup);
	void					SetIndex(DWORD index) {m_dwIndex = index;}
	
	void					SetMaterial(DWORD* pdwVertexColor,CMaterial* pMtl);
	CMaterial*				GetMaterial() {return m_pMtl;}
	DWORD					GetFacesNum() {return m_dwFacesNum;}
	WORD*					GetFacesPtr() {return m_pFace;}
	TVERTEX*				GetLightUV1Ptr() {return m_ptLightUV1;}
	TVERTEX*				GetLightUV2Ptr() {return m_ptLightUV2;}
	BOOL					IsCollision(VECTOR3* pv3IntersectPoint,float* pfDist,WORD* pwIndex,VECTOR3* pv3RayPos,VECTOR3* pv3RayDir,VECTOR3* pv3Pos);
	DWORD					ReadFile(void* pFP);
	DWORD					WriteFile(FILE* fp);

	DWORD					GetUniqueIndex() {return m_dwIndex;}
	void					SetUniqueIndex(DWORD dwIndex) {m_dwIndex = dwIndex;}
	DWORD					GetMtlIndex() {return m_dwMtlIndex;}
	void					SetMtlIndex(DWORD dwMtlIndex) { m_dwMtlIndex = dwMtlIndex;}
	BOOL					InsertFace(WORD a,WORD b,WORD c);
	BOOL					SetFace(DWORD dwIndex,WORD a,WORD b,WORD c);
	WORD*					CreateFaceList(DWORD dwFacesNum);
	void					ReleaseFaceList();

	DWORD					CreateIVertexList(VECTOR3* pv3Point,TVERTEX* ptUV,IVERTEX** ppVertex);
	void					ReleaseIVertexList(IVERTEX* pv);

	CFaceGroup();
	~CFaceGroup();
};