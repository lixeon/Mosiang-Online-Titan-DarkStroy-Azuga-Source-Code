#include "uv_plane.h"

CUVPlane::CUVPlane()
{
	memset(this,0,sizeof(CUVPlane));

}
DWORD CUVPlane::AddVertex(VECTOR3* pv3,TVERTEX* ptv)
{
	VERTEX_LINK*	pvl = new VERTEX_LINK;
	pvl->pNext = NULL;

	pvl->pv3In = pv3;
	pvl->ptv = ptv;

	if (!m_pVLCur)
	{
		m_pVLCur = pvl;
		m_pVLHead = pvl;
	}
	else 
	{
		m_pVLCur->pNext = pvl;
		m_pVLCur = pvl;
	}
	return m_dwVLNum++;
}

CUVPlane::~CUVPlane()
{
	VERTEX_LINK*	pCur = m_pVLHead;
	VERTEX_LINK*	pNext;
	
	while (pCur)
	{
		pNext = pCur->pNext;
		delete pCur;
		pCur = pNext;

	}
}
