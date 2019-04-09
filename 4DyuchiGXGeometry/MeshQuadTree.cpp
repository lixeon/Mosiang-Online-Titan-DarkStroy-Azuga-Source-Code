#include "MeshQuadTree.h"
#include "../4DyuchiGXGFunc/global.h"
#include "global_variable.h"

CMeshQuadTree::CMeshQuadTree()
{
	memset(this,0,sizeof(CMeshQuadTree));
}

BOOL CMeshQuadTree::Initialize(IVERTEX* pVertices,DWORD dwFacesNum,float fMinY,float fMaxY)
{
	DWORD	i;
	float	fGridLengthX,fGridLengthZ;
	float	min_x,max_x,min_z,max_z,min_y,max_y;

	m_fMinY = fMinY;
	m_fMaxY = fMaxY;


	min_x = 900000.0f;
	min_z = 900000.0f;
	min_y = 900000.0f;

	max_x = -900000.0f;
	max_z = -900000.0f;
	max_y = -900000.0f;


	for (i=0; i<3; i++)
	{
		if (min_x > pVertices[i].x)
			min_x = pVertices[i].x;

		if (min_z > pVertices[i].z)
			min_z = pVertices[i].z;


		if (max_x < pVertices[i].x)
			max_x = pVertices[i].x;

		if (max_z < pVertices[i].z)
			max_z = pVertices[i].z;


	}
	fGridLengthX = max_x - min_x;
	fGridLengthZ = max_z - min_z;

//	fGridLengthX = 50.0f;
//	fGridLengthZ = 50.0f;


	min_x = 900000.0f;
	min_z = 900000.0f;
	max_x = -900000.0f;
	max_z = -900000.0f;


	for (i=0; i<dwFacesNum*3; i++)
	{
		if (min_x > pVertices[i].x)
			min_x = pVertices[i].x;

		if (min_z > pVertices[i].z)
			min_z = pVertices[i].z;

		if (min_y > pVertices[i].y)
			min_y = pVertices[i].y;


		if (max_x < pVertices[i].x)
			max_x = pVertices[i].x;

		if (max_z < pVertices[i].z)
			max_z = pVertices[i].z;


		if (max_y < pVertices[i].y)
			max_y = pVertices[i].y;

	}
	m_fRect.fLeft = min_x;
	m_fRect.fTop = min_z;
	m_fRect.fRight = max_x;
	m_fRect.fBottom = max_z;
	m_fWidth = max_x - min_x;
	m_fHeight = max_z - min_z;

	m_dwNodeNumX = (DWORD)(m_fWidth / fGridLengthX)+1;
	m_dwNodeNumZ = (DWORD)(m_fHeight / fGridLengthZ)+1;

	m_fGridLengthX	= fGridLengthX;
	m_fGridLengthZ	= fGridLengthZ;

	DWORD dwSize = 1024*1024*100;
	m_dwMaxNodeNum = dwSize / sizeof(MESHNODE);
	m_pNodePool = new MESHNODE[m_dwMaxNodeNum];
	memset(m_pNodePool,0,sizeof(MESHNODE*)*m_dwMaxNodeNum);
	
	
	m_ppNodeGrid = new MESHNODE*[m_dwNodeNumX*m_dwNodeNumZ];
	memset(m_ppNodeGrid,0,sizeof(MESHNODE*)*m_dwNodeNumX*m_dwNodeNumZ);

	
	for (i=0; i<dwFacesNum; i++)
	{
		InsertTri(&pVertices[i*3]);
	}
	m_pVertices = pVertices;
	m_dwFacesNum = dwFacesNum;
	return TRUE;
}

MESHNODE* CMeshQuadTree::AllocNode()
{
	MESHNODE*	pNode = NULL;

	if (m_dwAllocNodeNum >= m_dwMaxNodeNum)
	{
#ifdef _DEBUG
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"CMeshQuadTree::AllocNode(), if (m_dwAllocNodeNum >= m_dwMaxNodeNum), File:%s , Line:%d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
#endif
		goto lb_return;
	}

	pNode = m_pNodePool+m_dwAllocNodeNum;
	pNode->pNext = NULL;
	pNode->pv3Tri = NULL;

	m_dwAllocNodeNum++;
lb_return:
	return pNode;
}
void CMeshQuadTree::InsertTri(IVERTEX* pTri)
{
	DWORD	i;
	int max_x,max_z,min_x,min_z;
	
	min_x = 9000000;
	min_z = 9000000;
	max_x = -9000000;
	max_z = -9000000;

	int	x,z;

	for (i=0; i<3; i++)
	{
		x = (int)( (pTri[i].x - m_fRect.fLeft) / m_fGridLengthX );
		z = (int)( (pTri[i].z - m_fRect.fTop) / m_fGridLengthZ );
		
		if (min_x > x)
			min_x = x;

		if (min_z > z)
			min_z = z;

		if (max_x < x)
			max_x = x;

		if (max_z < z)
			max_z = z;

		
	}
	min_x--;
	min_z--;
	max_x++;
	max_z++;

	if (min_x < 0)
		min_x = 0;

	if (min_z < 0)
		min_z = 0;

	if (max_x >= (int)m_dwNodeNumX)
		max_x = (int)m_dwNodeNumX-1;

	if (max_z >= (int)m_dwNodeNumZ)
		max_z = (int)m_dwNodeNumZ-1;


	
	for (z=min_z; z<=max_z; z++)
	{
		for (x=min_x; x<=max_x; x++)
		{
			MESHNODE*	pNew = AllocNode();
			MESHNODE*	pCur = m_ppNodeGrid[x + m_dwNodeNumX*z];
			pNew->pNext = pCur;
			pNew->pv3Tri = pTri;
			m_ppNodeGrid[x + m_dwNodeNumX*z] = pNew;

		}
	}

	m_ptAddConst[0].x = 0;
	m_ptAddConst[0].y = 0;
	m_ptAddConst[1].x = -1;
	m_ptAddConst[1].y = 0;
	m_ptAddConst[2].x = 1;
	m_ptAddConst[2].y = 0;

	m_ptAddConst[3].x = -1;
	m_ptAddConst[3].y = -1;
	m_ptAddConst[4].x = 0;
	m_ptAddConst[4].y = -1;
	m_ptAddConst[5].x = 1;
	m_ptAddConst[5].y = -1;

	m_ptAddConst[6].x = -1;
	m_ptAddConst[6].y = 1;
	m_ptAddConst[7].x = 0;
	m_ptAddConst[7].y = 1;
	m_ptAddConst[8].x = 1;
	m_ptAddConst[8].y = 1;
}


float CMeshQuadTree::GetYFactor(float x,float z)
{
	float	y = 0.0f;
	int		iIndexX,iIndexZ;
	DWORD	i;
	MESHNODE*	pCur;
	VECTOR3		v3Orig;
	VECTOR3		v3Dir;
	VECTOR3		v3IntersectPoint;
	BOOL		bFind = FALSE;
	float	fDist,fBary1,fBary2;


	if (x < m_fRect.fLeft)
		goto lb_return;

	if (x > m_fRect.fRight)
		goto lb_return;

	if (z < m_fRect.fTop)
		goto lb_return;

	if (z > m_fRect.fBottom)
		goto lb_return;

	v3Orig.y = m_fMaxY;
	v3Dir.y = m_fMinY;

	v3Dir.x = 0.0f;
	v3Dir.z = 0.0f;

	iIndexX = (int)( (x - m_fRect.fLeft) / m_fGridLengthX );
	iIndexZ = (int)( (z - m_fRect.fTop) / m_fGridLengthZ );

	int iX,iZ;
	for (i=0; i<9; i++)
	{
		iX = iIndexX + m_ptAddConst[i].x;
		iZ = iIndexZ + m_ptAddConst[i].y;

		if (iX < 0)
			continue;

		if (iX >= (int)m_dwNodeNumX)
			continue;

		if (iZ < 0)
			continue;

		if (iZ >= (int)m_dwNodeNumZ)
			continue;

		pCur = m_ppNodeGrid[iX + (int)m_dwNodeNumX * iZ];
//		pCur = m_ppNodeGrid[dwIndexX + m_dwNodeNumX*dwIndexZ];
	//	pNode[dwNodeNum] = m_ppNodeGrid[dwIndexX + m_dwNodeNumX*dwIndexZ];
	//	dwNodeNum++;


		while(pCur)
		{
			v3Orig.x = x;
			v3Orig.z = z;
			

			if (IntersectTriangle(
				&v3IntersectPoint,
				&v3Orig,
				&v3Dir,
				(VECTOR3*)&pCur->pv3Tri[0],
				(VECTOR3*)&pCur->pv3Tri[1],
				(VECTOR3*)&pCur->pv3Tri[2],
				&fDist,
				&fBary1,
				&fBary2,
				TRUE))
			{


			//	if (y < v3IntersectPoint.y)
				y = v3IntersectPoint.y;

				bFind = TRUE;
				goto lb_return;
			}
		
			pCur = pCur->pNext;
		}
	}
lb_return:
	if (!bFind)
	{
		y = 0.0f;
	}
lb_exit:
//	if (y == 0.0f)
//		__asm nop
	
	return y;

}

CMeshQuadTree::~CMeshQuadTree()
{
	delete [] m_pNodePool;
	delete [] m_ppNodeGrid;

}

