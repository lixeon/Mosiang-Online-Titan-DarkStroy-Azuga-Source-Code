#include "../4DyuchiGRX_common/typedef.h"
#include "LMMeshObject.h"
#include "CoD3DDevice.h"
#include "LMFaceGroup.h"
#include "VBHeap.h"




CLMFaceGroup::CLMFaceGroup()
{
	memset(this,0,sizeof(CLMFaceGroup));
}

BOOL CLMFaceGroup::CreateFaceGroup(CoD3DDevice* pRenderer,LPDIRECT3DDEVICE8 pDevice,VECTOR3* pv3,TVERTEX* ptv,WORD* pIndex,TVERTEX* ptLightUV1,DWORD dwFacesNum,DWORD dwFlag)
{
	BOOL		bResult = FALSE;
	
	m_dwFacesNum = dwFacesNum;

	DWORD dwTotalVerticesNum = dwFacesNum*3;

	
	DWORD	dwSize;
	DWORD	dwErrorCode;
	if (!pRenderer->GetVBLMMeshHeap()->AllocVB(&dwErrorCode,&m_VB,dwTotalVerticesNum,&dwSize,&m_VB.m_bEnable,NULL))
		__asm int 3

	m_VB.SetVertexPosList(pIndex,pv3,dwTotalVerticesNum);
	if (ptv)
		m_VB.SetTexCoord(ptv,pIndex,TEX1_COORD_OFFSET_LMMESH,dwTotalVerticesNum);
	
	if (ptLightUV1)
		m_VB.SetTexCoord(ptLightUV1,TEX2_COORD_OFFSET_LMMESH,dwTotalVerticesNum);
	

	bResult = TRUE;


	if (dwFlag & CREATE_FACEGROUP_TYPE_STATIC)
	{
		m_pFaceList = new TRI_FACE[dwFacesNum];
		m_pwFaceIndexList = new WORD[dwFacesNum];

		BuildFaceList(m_pFaceList,pv3,pIndex,m_dwFacesNum);
		m_pPrjMeshDesc = new PRJ_MESH_DESC;
		memset(m_pPrjMeshDesc,0,sizeof(PRJ_MESH_DESC));
	}

lb_return:
	return bResult;
}


CLMFaceGroup::~CLMFaceGroup()
{

}
