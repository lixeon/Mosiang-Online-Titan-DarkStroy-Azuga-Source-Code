#pragma once

#include "../max_common/stdafx.h"
class CLightTextureGenerator;

#pragma pack(push,4)
struct FILE_FACE_GROUP_HEADER
{
	DWORD					dwMtlIndex;
	DWORD					dwIndex;
	DWORD					dwFacesNum;
	DWORD					dwMaxFacesNum;
	DWORD					dwVertexIndexNum;
	DWORD					dwLightUVNum0;
	DWORD					dwReserved;
};	
#pragma pack(pop)

class CFaceGroup
{
	DWORD					m_dwMtlIndex;
	DWORD					m_dwIndex;
	DWORD					m_dwFacesNum;
	DWORD					m_dwMaxFacesNum;
	DWORD					m_dwVertexIndexNum;
	DWORD					m_dwLightUVNum0;
	DWORD					m_dwReserved;
	/////////////////////////////////////////////////////////////////////

	TVERTEX*				m_ptLightUV0;
	
	

	WORD*					m_pFace;
	VECTOR3*				m_pdvFaceNormal;
	MATERIAL*				m_pMtl;			

public:
	CFaceGroup*				m_pNextFaceGroup;

	DWORD					GetFacesNum() {return m_dwFacesNum;}
	WORD*					GetFacesPtr() {return m_pFace;}

	DWORD					WriteFile(FILE* fp);

	DWORD					GetUniqueIndex() {return m_dwIndex;}
	void					SetUniqueIndex(DWORD dwIndex) {m_dwIndex = dwIndex;}
	DWORD					GetMtlIndex() {return m_dwMtlIndex;}
	void					SetMtlIndex(DWORD dwMtlIndex) { m_dwMtlIndex = dwMtlIndex;}
	BOOL					InsertFace(WORD a,WORD b,WORD c,TVERTEX* ptv1,TVERTEX* ptv2);
	BOOL					InsertFace(WORD a,WORD b,WORD c,TVERTEX* ptv);
	BOOL					InsertFace(WORD a,WORD b,WORD c);
	BOOL					SetFace(DWORD dwIndex,WORD a,WORD b,WORD c);
	WORD*					CreateFaceList(DWORD dwFacesNum);


	CFaceGroup();
	~CFaceGroup();
};