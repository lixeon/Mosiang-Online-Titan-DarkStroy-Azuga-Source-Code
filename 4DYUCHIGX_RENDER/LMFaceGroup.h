#ifndef LMFACEGROUP_H
#define LMFACEGROUP_H

#include "IndexBuffer.h"
#include "CTexture.h"
#include "base_facegroup.h"

class CoD3DDevice;

class CLMFaceGroup : public CBaseFaceGroup
{
	CVertexBuffer			m_VB;
	
public:
	LPDIRECT3DVERTEXBUFFER8		GetVertexBuffer() {return m_VB.GetVertexBuffer();}
	DWORD						GetVertexSize() {return m_VB.GetVertexSize();}
	DWORD						GetVertexNum() {return m_VB.GetVerticesNum();}
	DWORD						GetVBStartIndex() {return m_VB.GetStartIndex();}
	
	BOOL						CreateFaceGroup(CoD3DDevice* pRenderer,LPDIRECT3DDEVICE8 pDevice,VECTOR3* pv3,TVERTEX* ptv,WORD* pIndex,TVERTEX* ptLightUV1,DWORD dwFacesNum,DWORD dwFlag);


	CLMFaceGroup();
	~CLMFaceGroup();
};

#endif
