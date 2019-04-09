#ifndef MESH_QUAD_TREE_H
#define MESH_QUAD_TREE_H

#include "../4DyuchiGRX_Common/typedef.h"

struct MESHNODE
{
	IVERTEX*	pv3Tri;
	MESHNODE*	pNext;
};

class CMeshQuadTree
{
	HFIELD_DESC			m_hfDesc;
	FLOAT_RECT			m_fRect;
	float				m_fWidth;
	float				m_fHeight;
	float				m_fGridLengthX;
	float				m_fGridLengthZ;

	MESHNODE*			m_pNodePool;
	DWORD				m_dwMaxNodeNum;
	DWORD				m_dwAllocNodeNum;
	MESHNODE**			m_ppNodeGrid;
	DWORD				m_dwNodeNumX;
	DWORD				m_dwNodeNumZ;
	IVERTEX*			m_pVertices;
	DWORD				m_dwFacesNum;
	float				m_fMinY;
	float				m_fMaxY;
	
	
	POINT				m_ptAddConst[9];

	MESHNODE*			AllocNode();
	void				InsertTri(IVERTEX* pTri);
	
public:
	BOOL				Initialize(IVERTEX* pVertices,DWORD dwFacesNum,float fMinY,float fMaxY);
	float				GetYFactor(float x,float z);
	
	CMeshQuadTree();
	~CMeshQuadTree();

};

#endif
