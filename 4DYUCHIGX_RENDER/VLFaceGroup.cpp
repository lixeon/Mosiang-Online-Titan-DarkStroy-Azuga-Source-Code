#include "VLFaceGroup.h"
#include "CoD3DDevice.h"
#include "d3d_helper.h"
#include "vertexbuffer.h"
#include "IBHeap.h"

CVLFaceGroup::CVLFaceGroup()
{
	memset(this,0,sizeof(CVLFaceGroup));


}
BOOL CVLFaceGroup::CreateFaceGroup(CoD3DDevice* pRenderer,LPDIRECT3DDEVICE8 pDevice,VECTOR3* pv3,WORD* pIndex,DWORD* pVertexColor,DWORD dwFacesNum,DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	
//	m_pVertexColor = pVertexColor;
	m_dwFacesNum = dwFacesNum;
	m_dwIndexNum = dwFacesNum*3;
	DWORD	dwTotalVerticesNum = dwFacesNum*3;

	DWORD	dwErrorCode;
	if (!pRenderer->GetIBHeapManaged()->AllocIB(&dwErrorCode,&m_IB,dwTotalVerticesNum,NULL,&m_IB.m_bEnable,NULL))
		__asm int 3

	m_IB.CheckValid();
	m_IB.SetIndexList(pIndex,dwTotalVerticesNum);
	
	
	if (dwFlag & CREATE_FACEGROUP_TYPE_STATIC)
	{
		m_pFaceList = new TRI_FACE[dwFacesNum];
		m_pwFaceIndexList = new WORD[dwFacesNum];

		BuildFaceList(m_pFaceList,pv3,pIndex,m_dwFacesNum);
		m_pPrjMeshDesc = new PRJ_MESH_DESC;
		memset(m_pPrjMeshDesc,0,sizeof(PRJ_MESH_DESC));

	}

	bResult = TRUE;

lb_return:
	return bResult;
}


CVLFaceGroup::~CVLFaceGroup()
{

}