#ifndef VLFACEGROUP_H
#define VLFACEGROUP_H

#include "CTexture.h"
#include "indexbuffer.h"
#include "base_facegroup.h"

class CVertexBuffer;
class CoD3DDevice;


class CVLFaceGroup : public CBaseFaceGroup
{
	CVertexBuffer*			m_pVertexBuffer;
	CIndexBuffer			m_IB;
	DWORD					m_dwIndexNum;
	
	
public:
	DWORD					GetIBStartIndex() {return m_IB.GetStartIndex();}
	DWORD					GetIndexNum() {return m_dwIndexNum;}
	LPDIRECT3DINDEXBUFFER8	GetIndexBuffer() {return m_IB.GetIndexBuffer();}
	BOOL					CreateFaceGroup(CoD3DDevice* pRenderer,LPDIRECT3DDEVICE8 pDevice,VECTOR3* pv3,WORD* pIndex,DWORD* pVertexColor,DWORD dwFacesNum,DWORD dwFlag);

	CVLFaceGroup();
	~CVLFaceGroup();
};

#endif

