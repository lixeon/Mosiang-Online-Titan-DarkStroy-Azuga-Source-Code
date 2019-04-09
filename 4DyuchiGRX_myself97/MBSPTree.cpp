// MBSPTree.cpp: implementation of the MBSPTree class.
//
//////////////////////////////////////////////////////////////////////

#include "MBSPTree.h"
#include "myself97common.h"
#include "../4DyuchiGXGFunc/global.h"
#include "CheckClock.h"

BOOL	TempComparePlane( PLANE* pP1, PLANE* pP2);
float	TriangleSize( VECTOR3* pv1, VECTOR3* pv2, VECTOR3* pv3);

BOOL	TestEdge();
VECTOR3		*gpvE1, *gpvE2, *gpvMid, *gpvP;
float		gfMaxDist;
PLANE		*gpPlane;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int MBSPTree::siCount	=	0;

MBSPTree::MBSPTree()
{
	memset( this, 0, sizeof(MBSPTree));
}

MBSPTree::~MBSPTree()
{
	this->DeleteAll();
}

BOOL MBSPTree::Init()
{
	_asm nop;
	this->siCount++ ;

	if( mpNode != 0)
	{
		_asm int 3;
		return FALSE;
	}
	
	this->mpPreTriangle	=	new VECTOR3[10000*3];
	memset( this->mpPreTriangle, 0, sizeof(VECTOR3)*10000*3);
	
	mbBuilt	=	false;

	return TRUE;	
}

DWORD MBSPTree::AddTriangle(VECTOR3* IN pTri)
{
	if( mbBuilt == true)
		_asm int 3;
	if( this->mdwPreTriangleCount >= 10000-1)
	{
		// 임시 삼각형 갯수 초과.
		_asm int 3;
	}
	VECTOR3		vTemp;
	int			i;
	this->mpPreTriangle[this->mdwPreTriangleCount*3+0]	=	pTri[0];
	this->mpPreTriangle[this->mdwPreTriangleCount*3+1]	=	pTri[1];
	this->mpPreTriangle[this->mdwPreTriangleCount*3+2]	=	pTri[2];
	
	// 반올림.
	for( i = 0; i < 3; i++)
	{
//		vTemp.x	=	Round(pTri[i].x);
//		vTemp.y	=	Round(pTri[i].y);
//		vTemp.z	=	Round(pTri[i].z);
		vTemp	=	pTri[i];
		this->mpPreTriangle[this->mdwPreTriangleCount*3+i]	=	vTemp;
	}

	this->mdwPreTriangleCount++;
	
	return mdwPreTriangleCount-1;		// 면의 인덱스(?) 리턴.	
}

BOOL MBSPTree::BuildTree()
{
	mbBuilt	=	true;

	DWORD		dwBuildTreeClock	=	GetLowClock();
	VECTOR3		vTemp;
	float		fTemp;
	if( mdwPreTriangleCount == 0)
	{
		delete[]	this->mpPreTriangle;
		mpPreTriangle		=	0;
		return FALSE;
	}
	int		i, j;
	BOOL	bFound	=	FALSE;
	// 버퍼 정리하고,
	{
		// 삼각형 버퍼.
		mpTri	=	new VECTOR3[ this->mdwPreTriangleCount*3];					
		memcpy( mpTri, mpPreTriangle, sizeof(VECTOR3)*this->mdwPreTriangleCount*3);
		this->miTriCount	=	mdwPreTriangleCount;
		delete[]	mpPreTriangle;
		mpPreTriangle	=	0;
		this->mdwPreTriangleCount	=	0;

		mpTriSphere		=	new	BOUNDING_SPHERE[this->miTriCount];		// 삼각형의 바운딩 스피어.

		for( i = 0; i < this->miTriCount; i++)
		{
			GetTriMiddle( &vTemp, &(mpTri[i*3]));
			mpTriSphere[i].v3Point	=	vTemp;

			fTemp		=	VECTOR3Length(&(mpTriSphere[i].v3Point - this->mpTri[i*3]));
			mpTriSphere[i].fRs	=	fTemp;
		}
		
		// 평면 버퍼.
		PLANE*	pTempPlane	=	new PLANE[miTriCount];		// 숫자가 줄어들 것이니 임시버퍼를 일단, 
		memset( pTempPlane, 0, sizeof(PLANE)*miTriCount);
		miPlaneCount	=	0;
		float*	pTempPlaneSize		=	new	float[miTriCount];
		memset( pTempPlaneSize, 0, sizeof(float)*miTriCount);
		DWORD*	pTempPlaneIndex	=	new	DWORD[miTriCount];
		memset( pTempPlaneIndex, 0, sizeof(DWORD)*miTriCount);
		
		MakePLANE( &(pTempPlane[0]), &(mpTri[0]), &(mpTri[1]), &(mpTri[2]));	// 첫번째 평면.

		miPlaneCount = 1;
		pTempPlaneIndex[0]	=	0;
		
		// 삼각형을 돌아가면서, 평면을 찾아낸다. 겹치는건 인덱스만 기록.
		for( i = 1; i < miTriCount; i++)
		{
			PLANE	p;
			MakePLANE( &p, &(mpTri[i*3]), &(mpTri[i*3+1]), &(mpTri[i*3+2]));
			fTemp	=	VECTOR3Length( &(p.v3Up));
			if( fTemp < 0.1f)
				_asm nop;
			bFound	=	FALSE;
			for( j = 0; j < miPlaneCount; j++)
			{
				if( TempComparePlane( &p, &pTempPlane[j]))
				{
					pTempPlaneIndex[i]	=	j;
					float	fThisTriSize	=	TriangleSize( &(mpTri[i*3]), &(mpTri[i*3+1]), &(mpTri[i*3+2]));		
					pTempPlaneSize[j]	=	pTempPlaneSize[j] + fThisTriSize;			// 이 평면이 사용되는 면적.
					bFound	=	TRUE;
					_asm nop;
				}
			}
			if( bFound	==	FALSE)		// 새로운 평면.
			{
				pTempPlane[miPlaneCount]	=	p;
				pTempPlaneIndex[i]	=	miPlaneCount;
				pTempPlaneSize[miPlaneCount]	=	TriangleSize( &(mpTri[i*3]), &(mpTri[i*3+1]), &(mpTri[i*3+2]));
				miPlaneCount++;
			}
		}
		
		mpPlane	=	new PLANE[miPlaneCount];					// 평면 버퍼.
		memcpy( mpPlane, pTempPlane, sizeof(PLANE)*miPlaneCount);
		delete[]	pTempPlane;
		for( i = 0; i < this->miPlaneCount; i++)
		{
			mpPlane[i].fAreaSize	=	pTempPlaneSize[i];
		}
		delete[]	pTempPlaneSize;
		mpPlaneIndex	=	new	DWORD[miTriCount];					// 삼각형의 평면 인덱스 버퍼.
		memcpy( mpPlaneIndex, pTempPlaneIndex, sizeof(DWORD)*miTriCount);
		delete[]	pTempPlaneIndex;
		
	}
	
	// 트리 만든다.
	DWORD*	pTemp	=	new DWORD[this->miTriCount];
	for( i = 0; i < this->miTriCount; i++)
	{
		pTemp[i]	=	i;
	}
	DWORD*	pTempCheckedPlane	=	new DWORD[this->miPlaneCount];
	memset( pTempCheckedPlane, 0, sizeof(DWORD)*this->miPlaneCount);

	// 노드가 들어갈 버퍼.
	miNodeTemp	=	miPlaneCount*20;		// 임시 갯수, 이게 안맞으면 어쩌나.?
	mpNode		=	new BSPTreeNode[miNodeTemp];
	miNodeCount	=	0;

	// 트리 만들고, 
	CreateNodeReculsive( -1, 0, pTemp, this->miTriCount, pTempCheckedPlane, 0);

	delete[]	pTempCheckedPlane;
	pTempCheckedPlane	=	0;
	delete[]	pTemp;
	pTemp	=	0;

	// 노드 정리.
	BSPTreeNode*		pTempNode	=	0;
	if( miNodeCount != 0)
	{
		pTempNode	=	new	BSPTreeNode[miNodeCount];
		memcpy( pTempNode, mpNode, sizeof(BSPTreeNode)*miNodeCount);
	}
	delete[]	mpNode;
	mpNode	=	pTempNode;

	// 삼각형 인덱스를 선형버퍼에 밀어넣는다.
	int		iTemp	=	this->miNodeCount * this->miTriCount;
	this->miNodeTriIndexBuffUsedCount	=	0;
	this->mpNodeTriIndexBuff			=	new		int[iTemp];
	for( i = 0; i < this->miNodeCount; i++)
	{
		if( mpNode[i].iTriCount >= 0)
		{
			mpNode[i].iTriStartIndex	=	miNodeTriIndexBuffUsedCount;
			for( j = 0; j < mpNode[i].iTriCount; j++)
			{
				this->mpNodeTriIndexBuff[miNodeTriIndexBuffUsedCount]	=	mpNode[i].r->pTriIndex[j];
				miNodeTriIndexBuffUsedCount++;
			}
			if( mpNode[i].r->pChecked)
			{
				delete[]	mpNode[i].r->pChecked;
				mpNode[i].r->pChecked		=	0;
			}
			if( mpNode[i].r->pPlaneIndex)
			{
				delete[]	mpNode[i].r->pPlaneIndex;
				mpNode[i].r->pPlaneIndex	=	0;
			}
			if( mpNode[i].r->pTriIndex)
			{
				delete[]	mpNode[i].r->pTriIndex;
				mpNode[i].r->pTriIndex		=	0;
			}
		}
	}
	int*	piTemp	=	new	int[miNodeTriIndexBuffUsedCount];
	memcpy( piTemp, mpNodeTriIndexBuff, sizeof(int)*miNodeTriIndexBuffUsedCount);
	delete[]	mpNodeTriIndexBuff;
	mpNodeTriIndexBuff		=	piTemp;



	
	//	찾기용 버퍼 만들기.
	this->pOutputBuffer	=	new DWORD[this->miTriCount];
	this->mpTempOutput	=	new BYTE[this->miTriCount];	// 출력용 인덱스 버퍼.
	
	this->miNormalNodeCount	=	this->miNodeCount	-	this->miLeafCount	-	this->miNullLeafCount;

	if( this->miTriCount <= 0)
		_asm int 3;

	dwBuildTreeClock	=	GetLowClock()	-	dwBuildTreeClock;

	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;

	return TRUE;	
}

// bBack은 true이면 뒤, false이면 앞. -_-;
int MBSPTree::CreateNodeReculsive( int iParent, BOOL bIsBack, DWORD* pTriIndex, DWORD dwTriCount, DWORD* pCheckedPlane, int iLevel)
{
	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;


	if( iLevel > miMaxLevel)	miMaxLevel	=	iLevel;
	int	i;//, j;
	int		iResult;

	if( dwTriCount != 0 && pCheckedPlane == 0)
	{
		_asm nop;
	}

	// 일단 노드 만들고.
	if( this->miNodeCount >= this->miNodeTemp)
	{
		_asm int 3;
//		return -1;   //큰일났다.
	}

	BSPTreeNode*	pNode	=	&(mpNode[this->miNodeCount]);
	int				iNode	=	miNodeCount;
	memset( pNode, 0, sizeof(BSPTreeNode));
	pNode->iBack	=	-1;
	pNode->iFront	=	-1;
	this->miNodeCount++;
	pNode->r		=	new	BSPTreeNodeRuntime;
	memset( pNode->r, 0, sizeof(BSPTreeNodeRuntime));
	
	pNode->iParent		=	iParent;
	pNode->iLevel		=	iLevel;
	
	if( dwTriCount == 0)	// NULL leaf.
	{
		//		_asm int 3;
		pNode->bIsLeaf	=	TRUE;
		this->miNullLeafCount++;
		return iNode;
	}
	
	// 삼각형 버퍼, 만들어지고 난 다음엔 리프가 아니면 지워진다.
	pNode->r->pTriIndex	=	new DWORD[ dwTriCount];		
	memcpy( pNode->r->pTriIndex, pTriIndex, sizeof(DWORD)*dwTriCount);
	pNode->iTriCount	=	dwTriCount;
	// 평면 체크 버퍼.
	pNode->r->pChecked		=	new	DWORD[ miPlaneCount];
	memcpy( pNode->r->pChecked, pCheckedPlane, sizeof(DWORD)*miPlaneCount);
	
	// 부모에게 받은 삼각형 집합에서 가장 적합한(?)거 하나를 골라낸다.
	int		iTemp	=	FindDivideCandidate( pNode->r->pTriIndex, pNode->iTriCount, pNode->r->pChecked);
	
	// 리프 조건 찾기.
	if( iTemp == -1) // 더이상 평면이 남아있지 않으면, 
	{
		pNode->bIsLeaf	=	TRUE;
		this->miLeafCount++;
		return	iNode;
	}
	if( iTemp > this->miTriCount)
		_asm int 3;
	
	
	
	
	pNode->iDivPlane		=	this->mpPlaneIndex[pNode->r->pTriIndex[iTemp]];
	pNode->r->pChecked[pNode->iDivPlane]	=	1;	// 체크.
	
	
	// 삼각형을 평면에 대해 분류할때 쓰일 버퍼.
	DWORD*	pBack	=	new DWORD[dwTriCount];
	DWORD	dwBack	=	0;
	
	DWORD*	pFront	=	new DWORD[dwTriCount];
	DWORD	dwFront	=	0;
	
	// 그평면을 중심으로 노드의 삼각형을 앞, 뒤로 배분,
	{
		for( i = 0; i < pNode->iTriCount; i++)
		{
			iResult	=	WhereIsTriangleFromPlane( &(this->mpTri[(pNode->r->pTriIndex[i])*3]), &(this->mpPlane[pNode->iDivPlane]));
			switch( iResult)
			{
			case	-2:		// 명백하게 뒤에 있거나,
			case	-1:		// 평면은 겹치지만 노말이 반대인 경우.
				pBack[dwBack]	=	pNode->r->pTriIndex[i];
				dwBack++;
				
				break;
			case	0:		// 면을 중심으로 삼각형이 걸쳐져있는 경우.
				pBack[dwBack]	=	pNode->r->pTriIndex[i];
				dwBack++;
				pFront[dwFront]	=	pNode->r->pTriIndex[i];
				dwFront++;
				break;
			case	1:		// 평면이 겹치고 노말도 같은 방향인경우.
			case	2:		// 명백하게 앞에 있거나.
				pFront[dwFront]	=	pNode->r->pTriIndex[i];
				dwFront++;
				break;
			default:
				_asm int 3;
				break;
			}// switch
		}// for
	}
	
	
	// 자식 재귀.
	{
		// 앞 자식들 재귀.
		if( dwFront != 0)		// 주석을 풀면 널 노드는 날린다는 뜻이다.
		{
			pNode->iFront	=	this->CreateNodeReculsive( iNode, FALSE, pFront, dwFront, pNode->r->pChecked, pNode->iLevel+1);
		}
		
		// 뒷 자식들 재귀.
		if( dwBack != 0)		// 주석을 풀면 널 노드는 날린다는 뜻이다.
		{
			pNode->iBack	=	this->CreateNodeReculsive( iNode, TRUE, pBack, dwBack, pNode->r->pChecked, pNode->iLevel+1);
		}
	}
	
	// 버퍼 정리.
	delete[]	pBack;
	pBack	=	0;
	delete[]	pFront;
	pFront	=	0;
	
	delete[]	pNode->r->pTriIndex;		// 노드가 가지고 있던 삼각형 리스트는 날린다.
	pNode->r->pTriIndex	=	0;
	pNode->iTriCount	=	0;
	delete[]	pNode->r->pChecked;
	pNode->r->pChecked		=	0;
	
	return	iNode;
}


// 주어진 삼각형이 가리키는 평면중 bsp노드용으로 가장 적합한 평면을 찾는다.
//	리턴값은 주어진 범위내의 삼각형 인덱스.
int MBSPTree::FindDivideCandidate(DWORD* pTriIndex, DWORD dwTriCount, DWORD* pCheckedPlane)
{
	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;


	VECTOR3		vTemp;
	float		fTemp;
	if( dwTriCount == 0 || pTriIndex == 0 || pCheckedPlane == 0)		// 이런 상황이 오면 안된다.
	{
		_asm int 3;		
		return -1;
	}
	
	DWORD	i;// , j;
	DWORD	dwPlaneIndex;

	DWORD	dwCandidate	=	0xffffffff;		//  후보 삼각형.
	
	BOOL	bFound	=	FALSE;					// 몽땅 체크되어있던것이면 이 플랙이 for문을 빠져나와도 false이다.
	float		fTopSize	=	0;
	
	// x,z평면 얼라인된 평면 우선.
	for( i = 0; i < dwTriCount; i++)
	{
		dwPlaneIndex	=	this->mpPlaneIndex[pTriIndex[i]];
		if( pCheckedPlane[dwPlaneIndex] == 1)	continue;
		if( mpPlane[dwPlaneIndex].v3Up.y >=	0.999f)
		{
			bFound	=	TRUE;
			if( mpPlane[dwPlaneIndex].fAreaSize > fTopSize)
			{
				dwCandidate	=	i;
				fTopSize	=	mpPlane[dwPlaneIndex].fAreaSize;
			}
		}
	}
	if( bFound)
		goto	lbReturn;
	// y,z평면에 얼라인된 평면 우선.
	fTopSize	=	0;
	for( i = 0; i < dwTriCount; i++)
	{
		dwPlaneIndex	=	this->mpPlaneIndex[pTriIndex[i]];
		if( pCheckedPlane[dwPlaneIndex] == 1)	continue;
		if( mpPlane[dwPlaneIndex].v3Up.x >=	0.999f)
		{
			bFound	=	TRUE;
			if( mpPlane[dwPlaneIndex].fAreaSize > fTopSize)
			{
				dwCandidate	=	i;
				fTopSize	=	mpPlane[dwPlaneIndex].fAreaSize;
			}
		}
	}
	if( bFound)
		goto	lbReturn;

	// x, y평면에 얼라인된 면.
	fTopSize	=	0;
	for( i = 0; i < dwTriCount; i++)
	{
		dwPlaneIndex	=	this->mpPlaneIndex[pTriIndex[i]];
		if( pCheckedPlane[dwPlaneIndex] == 1)	continue;
		if( mpPlane[dwPlaneIndex].v3Up.z >=	0.999f)
		{
			bFound	=	TRUE;
			if( mpPlane[dwPlaneIndex].fAreaSize > fTopSize)
			{
				dwCandidate	=	i;
				fTopSize	=	mpPlane[dwPlaneIndex].fAreaSize;
			}
		}
	}
	if( bFound)
		goto	lbReturn;
	// 그 외 y축과 평행한 평면 3순위.
	fTopSize	=	0;
	for( i = 0; i < dwTriCount; i++)
	{
		dwPlaneIndex	=	this->mpPlaneIndex[pTriIndex[i]];
		if( pCheckedPlane[dwPlaneIndex] == 1)	continue;
		vTemp.x		=	0.0f;
		vTemp.y		=	1.0f;
		vTemp.z		=	0.0f;
		fTemp	=	vTemp * mpPlane[dwPlaneIndex].v3Up;
//		if( pPlane[dwPlaneIndex].v3Up.z >=	0.999f)
		if( fTemp >= -0.00001f && fTemp <= 0.00001f)
		{
			bFound	=	TRUE;
			if( mpPlane[dwPlaneIndex].fAreaSize > fTopSize)
			{
				dwCandidate	=	i;
				fTopSize	=	mpPlane[dwPlaneIndex].fAreaSize;
			}
		}
	}
	if( bFound)
		goto	lbReturn;

	// 그외 삼각형의 면적이 가장 넓은 평면을 후보로.
	bFound	=	FALSE;
	fTopSize	=	0;
	for( i = 0; i < dwTriCount; i++)
	{
		dwPlaneIndex	=	this->mpPlaneIndex[pTriIndex[i]];
		if( pCheckedPlane[dwPlaneIndex] == 1)	continue;
		bFound	=	TRUE;
//		if( this->pPlaneSize[dwPlaneIndex] > dwTopSize)
		if( mpPlane[dwPlaneIndex].fAreaSize > fTopSize)
		{
			dwCandidate	=	i;
			fTopSize	=	mpPlane[dwPlaneIndex].fAreaSize;
		}
	}


lbReturn:
	if( bFound == TRUE)		return		dwCandidate;

	// 이까지 오면 삼각형들이 다 체크되어있다 == 리프이다.
	return -1;
}



//	 주어진 삼각형이 평면의 어디에 있는가.?
//	명백히 앞에 있으면 2, 명백히 뒤에 있으면 -2, 평면이 겹쳐있는데 노말이 같은경우 1, 노말이 반대인 경우 -1, 
//	두개가 서로 만나고 있으면 0 리턴.
int MBSPTree::WhereIsTriangleFromPlane(VECTOR3 *pTri, PLANE *pPlane)
{
	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;

	DWORD	i;
	VECTOR3		Normal, vec1, vec2;
	
	DWORD	dwFront = 0;		// 평면의 앞에 잇는 점 갯수.
	DWORD	dwBack = 0;			// 뒤에 있는 점 갯수.
	DWORD	dwOn = 0;			// 평면 위에 있는 점 갯수.
	
	float	d;
	for( i = 0; i < 3; i++)
	{
		d	=	pPlane->v3Up.x*pTri[i].x + pPlane->v3Up.y*pTri[i].y + pPlane->v3Up.z*pTri[i].z + pPlane->D;
		if( d > 0.5f )			dwFront++;
		else if( d < -0.5f )	dwBack++;
		else					dwOn++;
	}
	
	if( dwOn == 3)	// 점 세개가 다 평면위에 있는경우, 노말을 판별해서 -1, 1을 리턴.
	{
		VECTOR3_SUB_VECTOR3( &vec1, &(pTri[1]), &(pTri[0]));
		VECTOR3_SUB_VECTOR3( &vec2, &(pTri[2]), &(pTri[0]));
		CrossProduct(	&Normal, &vec1, &vec2);
		// 평면 벡터와 삼각형 노말의 곱.
		float	dot	=	DotProduct( &Normal, &(pPlane->v3Up));
		if( dot > 0)	return 1;
		else if( dot < 0)	return -1;
		else	_asm int 3;
	}
	
	if( dwFront > 0)	// 점이 최소 하나가 앞에 있을경우.
	{
		if( dwBack == 0)	// 점들이 뒤에 하나도 없는경우. 
		{
			return 2;
		}
		// 점이 하나라도 뒤에 있으면, cross.
		return 0;
	}
	
	// 이까지 오는건 점들이 모두 뒤에 있다는 뜻.
	return	-2;
}

DWORD MBSPTree::FindNearTri(DWORD *pBuff, DWORD dwBuffSize, VECTOR3* pSPivot, float fSRadius)
{
	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;
	
	float	fTemp, fTemp2;
	if( this->miTriCount == 0)
	{
		return	0;
	}

	int		i;
	this->pSPivot	=	pSPivot;
	this->fSRadius	=	fSRadius;
	memset( this->mpTempOutput, 0, sizeof(BYTE)*this->miTriCount);
	DWORD	dwFound	=	0;
	DWORD	dwPreFound	=	0;
	DWORD	dwTempLoop = 0;

	// 트리 타고, 
	DWORD	a = GetLowClock();
	this->FindNearTriReculsive( 0);
	a = GetLowClock() - a;

	DWORD	b	=	GetLowClock();

	DWORD	c;
	// 버퍼 돌아가면서 체크.
	for( i = 0; i < this->miTriCount; i++)
	{
		if( this->mpTempOutput[i] == 1)
		{
			dwTempLoop++;
			// 실제론 삼각형이 스피어 안에 들어가는지 검사해야 한다.
 			c	=	GetLowClock();
//			if( FALSE	== this->IsTooFarBetweenVertexAndTri( fSRadius, i, pSPivot))    // <= 예가 제일 부하를 많이 먹는다. 구조를 바꿔야할까.?

//			vTemp	=	mpTriSphere[i].v3Point - *pSPivot;
//			fTemp	=	vTemp * vTemp;
//			fTemp2	=	(mpTriSphere[i].fRs+1.0f)*(mpTriSphere[i].fRs+1.0f)	+	(fSRadius+1.0f)*(fSRadius+1.0f);

			fTemp	=	(mpTriSphere[i].v3Point - *pSPivot)*(mpTriSphere[i].v3Point - *pSPivot);
			fTemp2	=	(mpTriSphere[i].fRs + fSRadius) * (mpTriSphere[i].fRs + fSRadius);

			if( fTemp <= fTemp2) 
			{
				dwPreFound++;
				if( FALSE	== this->IsTooFarBetweenVertexAndTri( fSRadius, i, pSPivot))    // <= 예가 제일 부하를 많이 먹는다. 구조를 바꿔야할까.?
				{
					this->pOutputBuffer[dwFound]		=	i;	// 삼각형의 인덱스를 복사. 외부에서 삼각형 버퍼에 직접 접근하게 하는게 메모리 좀 아낄듯.
					++dwFound;
				}
			}
			c = GetLowClock() - c;
			_asm nop;
		}
	}
	b	=	GetLowClock() - b;
	_asm nop;
	if( dwFound > dwBuffSize)
	{
		_asm nop;
		dwFound	=	dwBuffSize;
	}
	DWORD	d	=	GetLowClock();
	memcpy( pBuff, this->pOutputBuffer, sizeof(DWORD)*dwFound);
	d		=	GetLowClock()	-	d;
	return	dwFound;
}

void MBSPTree::FindNearTriReculsive(int iNode )
{
	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;

	mdwDBG_NodeTravelCount	=	0;
//	float	fTemp;
	int				iCurrentNode	=	iNode;
	BSPTreeNode*	pCurrentNode	=	&(mpNode[iNode]);
//	float	fWhere;
	PLANE*	pP;

lb_Start:
	mdwDBG_NodeTravelCount++;
	if( pCurrentNode->bIsLeaf == TRUE)		// 이 노드가 리프라면,
	{
		int i; 
//		for( i = 0; i < pCurrentNode->iTriCount; i++)
		{
			// 가지고 있는 삼각형 리스트를 다 벹어낸다.
//			this->pTempOutput[pCurrentNode->r->pTriIndex[i]]	=	1;
		}
		for( i = pCurrentNode->iTriStartIndex; i < pCurrentNode->iTriCount + pCurrentNode->iTriStartIndex; i++)
		{
			int		iIndex				=	this->mpNodeTriIndexBuff[i];
			this->mpTempOutput[iIndex]	=	1;
		}
		// 그냥 리턴.?
		goto lb_Return;
	}
	
	// 일반 노드, 일단 평면의 어디에 구의 중점이 있는지 판별.
	pP	=	&(mpPlane[pCurrentNode->iDivPlane]);
	pCurrentNode->r->fWhere	=	pP->v3Up.x*pSPivot->x + pP->v3Up.y*pSPivot->y + pP->v3Up.z*pSPivot->z + pP->D;	// 제일 빠르다.
//	pCurrentNode->fWhere	=	DotProduct( &(pP->v3Up), pSPivot) + pP->D;			// 2순위.
//	pCurrentNode->fWhere	=	(pP->v3Up)*(*pSPivot) + pP->D;			 // 제일 느리다.

	// 앞 노드 재귀.
	if( pCurrentNode->r->fWhere >= -fSRadius && pCurrentNode->iFront != -1)
	{

//		pCurrentNode	=	pCurrentNode->pFront;
		iCurrentNode	=	pCurrentNode->iFront;
		pCurrentNode	=	&(mpNode[iCurrentNode]);
		
		goto	lb_Start;
	}
lb_FrontNodeReculsived:
	// 뒷 노드 재귀.
	if( pCurrentNode->r->fWhere <= fSRadius && pCurrentNode->iBack != -1)
	{
		iCurrentNode	=	pCurrentNode->iBack;
		pCurrentNode	=	&(mpNode[iCurrentNode]);
//		pCurrentNode	=	pCurrentNode->pBack;
		goto	lb_Start;
	}
lb_BackNodeReculsived:

lb_Return:
	if( pCurrentNode	==	mpNode)
		return ;
//	if( pCurrentNode->pParent->pFront	==	pCurrentNode)
	if( mpNode[pCurrentNode->iParent].iFront == iCurrentNode)
	{
		iCurrentNode	=	pCurrentNode->iParent;
		pCurrentNode	=	&(mpNode[iCurrentNode]);
//		pCurrentNode	=	pCurrentNode->pParent;
		goto lb_FrontNodeReculsived;
	}
//	if( pCurrentNode->pParent->pBack	==	pCurrentNode)
	if( mpNode[pCurrentNode->iParent].iBack == iCurrentNode)
	{
		iCurrentNode	=	pCurrentNode->iParent;
		pCurrentNode	=	&(mpNode[iCurrentNode]);
//		pCurrentNode	=	pCurrentNode->pParent;
		goto	lb_BackNodeReculsived;
	}

_asm nop;
/*
	DWORD i; 
	if( pNode->bIsLeaf == TRUE)		// 이 노드가 리프라면,
	{
		for( i = 0; i < pNode->dwTriCount; i++)
		{
			// 가지고 있는 삼각형 리스트를 다 벹어낸다.
			this->pTempOutput[pNode->pTriIndex[i]]	=	1;
		}
		// 그냥 리턴.?
		return ;
	}
	
	// 일반 노드, 일단 평면의 어디에 구의 중점이 있는지 판별.
	const	PLANE&	p	=	pPlane[pNode->dwDivPlane];
	float	fWhere	=	p.v3Up.x*pSPivot->x + p.v3Up.y*pSPivot->y + p.v3Up.z*pSPivot->z + p.D;

	// 앞 노드 재귀.
	if( fWhere >= -fSRadius)
	{
		FindNearTriReculsive( pNode->pFront );
	}
	
	// 뒷 노드 재귀.
	if( fWhere <= fSRadius)
	{
		FindNearTriReculsive( pNode->pBack );
	}
*/
}


void MBSPTree::DeleteAll()
{
	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;
	
	// 평면 버퍼.
	if( mpPlane)		delete[]	 mpPlane;
	mpPlane			=	0;
	miPlaneCount	=	0;

	// 삼각형 버퍼.
	if( mpTri)		delete[]	mpTri;
	mpTri			=	0;
	miTriCount		=	0;
	if( mpPlaneIndex)	delete[]	mpPlaneIndex;
	mpPlaneIndex		=	0;
	mpPlaneIndex		=	0;
	
	// 바운딩 스피어 버퍼.
	if( mpTriSphere)	delete[]	mpTriSphere;
	mpTriSphere		=	0;

	// 출력용 버퍼.
	if( mpTempOutput)	delete[]	mpTempOutput;
	mpTempOutput		=	0;
	
	// 임시버퍼.
	if( mpPreTriangle)
	{
		_asm int 3;	// 예전에 지워졌다.
		delete[]	mpPreTriangle;
		mpPreTriangle	=	0;
	}
	mdwPreTriangleCount	=	0;
	
	if( mpNodeTriIndexBuff)			delete[]	mpNodeTriIndexBuff;

	// 트리.
	if( mpNode)
	{
		for( int i = 0; i < miNodeCount; i++)
		{
			if( mpNode[i].r->pChecked)			delete[]	mpNode[i].r->pChecked;
			if( mpNode[i].r->pPlaneIndex)		delete[]	mpNode[i].r->pPlaneIndex;
			if( mpNode[i].r->pTriIndex)		delete[]	mpNode[i].r->pTriIndex;
			delete		mpNode[i].r;			
		}
		delete[]	mpNode;
		mpNode	=	0;
	}

	

	if( pOutputBuffer)	delete[]	pOutputBuffer;
	pOutputBuffer	=	0;
	
	memset( this, 0, sizeof(MBSPTree));
}


BOOL	TempComparePlane( PLANE* pP1, PLANE* pP2)
{
	float	fTemp;

	// 노말 비교.
	VECTOR3		n1, n2, n;
	Normalize( &n1, &(pP1->v3Up));
	Normalize( &n2, &(pP2->v3Up));
	// 거의(?) 같은가.?
	VECTOR3_SUB_VECTOR3( &n, &n1, &n2);
	fTemp	=	VECTOR3Length( &n);
	if( fTemp	> 0.001f)
		return	FALSE;

	// d가 거의 같은가.?
	fTemp	=	pP1->D - pP2->D;
	if( fTemp > 0.5f || fTemp <= -0.5f)
		return	FALSE;

	//	여기까지 오면 같다.
	return	TRUE;
}

// 너무 멀면 TRUE, 거리 안에 있으면 FALSE
BOOL	MBSPTree::IsTooFarBetweenVertexAndTri( float fMaxDist, DWORD dwTriIndex, VECTOR3* pVertex)
{
	VECTOR3		vTemp;
	float	fTemp;
	PLANE&		Plane	=	this->mpPlane[this->mpPlaneIndex[dwTriIndex]];
	VECTOR3*	pTri	=	&(this->mpTri[dwTriIndex*3]);
	VECTOR3&	v		=	*pVertex;
	float		fDist;
	fDist	=	DotProduct( &(Plane.v3Up), &v) + Plane.D;


	if( fDist < 0 || fDist > fMaxDist)
	{
		goto lbReturnTRUE;
	}

	VECTOR3		Mid;
	GetTriMiddle( &Mid, pTri);
	VECTOR3		A, B;
	VECTOR3		cross;
	VECTOR3		OnLine;
	float		fOnLineT;
	// 첫번째 엣지 테스트.
//	A	=	pTri[1] - pTri[0];
	VECTOR3_SUB_VECTOR3( &A, &(pTri[1]), &(pTri[0]));
//	B	=	(*pVertex) - pTri[0];
	VECTOR3_SUB_VECTOR3( &B, pVertex, &(pTri[0]));
	CrossProduct( &cross, &A, &B);
	Normalize( &cross, &cross);
//	fTemp	=	cross*Plane.v3Up;
	fTemp	=	DotProduct( &cross, &(Plane.v3Up));
	if( fTemp <= 0)   // 밖으로 나갈경우.
	{
		// 엣지 위에서 가장 가까운점 테스트.
		FindNearestVertexOnLine( &OnLine, &fOnLineT, pTri[0], pTri[1], v);
		if( fOnLineT > 1)
		{
//			vTemp	=	pTri[1]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &(pTri[1]), &v);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > fMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT < 0)
		{
//			vTemp	=	pTri[0]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &(pTri[0]), &v);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > fMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT <= 1 && fOnLineT >= 0)
		{
//			vTemp	=	OnLine-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &OnLine, &v);
			fTemp	=	VECTOR3Length( &vTemp);
			if( fTemp > fMaxDist)	
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
	}
	// 두번째 엣지 테스트.
//	A	=	pTri[2] - pTri[1];
	VECTOR3_SUB_VECTOR3( &A, &(pTri[2]), &(pTri[1]));
//	B	=	(*pVertex) - pTri[1];
	VECTOR3_SUB_VECTOR3( &B, pVertex, &(pTri[1]));
	CrossProduct( &cross, &A, &B);
	Normalize( &cross, &cross);
//	fTemp	=	cross*Plane.v3Up;
	fTemp	=	DotProduct( &cross, &(Plane.v3Up));
	if( fTemp <= 0)   // 밖으로 나갈경우.
	{
		// 엣지 위에서 가장 가까운점 테스트.
		FindNearestVertexOnLine( &OnLine, &fOnLineT, pTri[1], pTri[2], v);
		if( fOnLineT > 1)
		{
//			vTemp	=	pTri[2]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &(pTri[2]), &v);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > fMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT < 0)
		{
//			vTemp	=	pTri[1]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &(pTri[1]), &v);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > fMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT <= 1 && fOnLineT >= 0)
		{
//			vTemp	=	OnLine-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &OnLine, &v);
			fTemp	=	VECTOR3Length( &vTemp);
			if( fTemp > fMaxDist)	
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
	}
	// 세번째 엣지 테스트.
//	A	=	pTri[0] - pTri[2];
	VECTOR3_SUB_VECTOR3( &A, &(pTri[0]), &(pTri[2]));
//	B	=	(*pVertex) - pTri[2];
	VECTOR3_SUB_VECTOR3( &B, pVertex, &(pTri[2]));
	CrossProduct( &cross, &A, &B);
	Normalize( &cross, &cross);
//	fTemp	=	cross*Plane.v3Up;
	fTemp	=	DotProduct( &cross, &(Plane.v3Up));
	if( fTemp <= 0)   // 밖으로 나갈경우.
	{
		// 엣지 위에서 가장 가까운점 테스트.
		FindNearestVertexOnLine( &OnLine, &fOnLineT, pTri[2], pTri[0], v);
		if( fOnLineT > 1)
		{
//			vTemp	=	pTri[0]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &(pTri[0]), &v);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > fMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT < 0)
		{
//			vTemp	=	pTri[2]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &(pTri[2]), &v);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > fMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT <= 1 && fOnLineT >= 0)
		{
//			vTemp	=	OnLine-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &OnLine, &v);
			fTemp	=	VECTOR3Length( &vTemp);
			if( fTemp > fMaxDist)	
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
	}





lbReturnFALSE:
	return		FALSE;
lbReturnTRUE:
	return		TRUE;

}

BOOL	TestEdge()
{
	VECTOR3		A, B, cross, OnLine, vTemp;
	float		fTemp, fOnLineT;
//	A	=	pTri[1] - pTri[0];
	VECTOR3_SUB_VECTOR3( &A, gpvE2, gpvE1);
//	B	=	(*pVertex) - pTri[0];
	VECTOR3_SUB_VECTOR3( &B, gpvP, gpvE1);
	CrossProduct( &cross, &A, &B);
	Normalize( &cross, &cross);
//	fTemp	=	cross*Plane.v3Up;
	fTemp	=	DotProduct( &cross, &(gpPlane->v3Up));
	if( fTemp <= 0)   // 밖으로 나갈경우.
	{
		// 엣지 위에서 가장 가까운점 테스트.
		FindNearestVertexOnLine( &OnLine, &fOnLineT, (*gpvE1), (*gpvE2), (*gpvP));
		if( fOnLineT > 1)
		{
//			vTemp	=	pTri[1]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, gpvE2, gpvP);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > gfMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT < 0)
		{
//			vTemp	=	pTri[0]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, gpvE1, gpvP);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > gfMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT <= 1 && fOnLineT >= 0)
		{
//			vTemp	=	OnLine-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &OnLine, gpvP);
			fTemp	=	VECTOR3Length( &vTemp);
			if( fTemp > gfMaxDist)	
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
	}
lbReturnFALSE:
	return		FALSE;
lbReturnTRUE:
	return		TRUE;

}



float	TriangleSize( VECTOR3* pv1, VECTOR3* pv2, VECTOR3* pv3)
{
	VECTOR3	p;
	float	t;
	FindNearestVertexOnLine( &p, &t, *pv2, *pv3, *pv1);
	float	fWidth		=	VECTOR3Length(&(*pv2 - *pv3));
	float	fHeight		=	VECTOR3Length(&(*pv1 - p));
	float	fSize		=	fWidth * fHeight * 0.5f;
	return	fSize;
}

/****************************************************************************
	트리 저장.
****************************************************************************/
bool MBSPTree::Save(HANDLE hFile)
{
	bool		bResult;
	DWORD		dwTemp	=	0;
	DWORD		dwSize	=	0;

	// 헤더 기록하기.
	MBSPTreeFileHeader		header;
	{
		// 버전.
		header.iVer			=	iBSPVer;
		// 삼각형 갯수.
		header.iTriCount	=	this->miTriCount;
		// 평면 갯수.
		header.iPlaneCount	=	this->miPlaneCount;
		// 노드 갯수.
		header.iNodeCount			=	this->miNodeCount;
		header.iNormalNodeCount		=	this->miNormalNodeCount;
		header.iLeafCount			=	this->miLeafCount;	// 리프 갯수.
		header.iNullLeafCount		=	this->miNullLeafCount;	// 삼각형을 아무것도 가지지 않은 리프 카운트.
		header.iMaxLevel			=	this->miMaxLevel;
	}
	bResult		=	WriteDataToFile( hFile, &header, sizeof(header));
	
	
	// 삼각형 기록.
	{
		// 삼각형,
		dwSize		=	sizeof(VECTOR3)*3*miTriCount;
		bResult		=	WriteDataToFile( hFile, mpTri, dwSize);
		// 바운딩 스피어,
		dwSize		=	sizeof( BOUNDING_SPHERE)*miTriCount;
		bResult		=	WriteDataToFile( hFile, mpTriSphere, dwSize);
		// 평면 인덱스.
		dwSize		=	sizeof( DWORD)*miTriCount;
		bResult		=	WriteDataToFile( hFile, mpPlaneIndex, dwSize);
	}

	// 평면 기록.
	{
		dwSize		=	sizeof(PLANE)*this->miPlaneCount;
		bResult		=	WriteDataToFile( hFile, mpPlane, dwSize);
	}

	// bsp 노드들.
	{
		dwSize		=	sizeof(BSPTreeNode)*this->miNodeCount;
		bResult		=	WriteDataToFile( hFile, mpNode, dwSize);
	}
	
	// 노드가 가지는 삼각형들 버퍼.
	{
		// 버퍼 크기.
		dwSize		=	sizeof(int);
		bResult		=	WriteDataToFile( hFile, &miNodeTriIndexBuffUsedCount, dwSize);

		// 버퍼.
		dwSize		=	sizeof(int)*miNodeTriIndexBuffUsedCount;
		bResult		=	WriteDataToFile( hFile, mpNodeTriIndexBuff, dwSize);
	}

	return		true;

}


bool	MBSPTree::Load(HANDLE hFile)
{
	bool		bResult	=	false;
	DWORD		dwTemp	=	0;
	DWORD		dwSize	=	0;

	// 헤더 읽어오기.
	MBSPTreeFileHeader		header;
	bResult		=	ReadDataFromFile( hFile, &header, sizeof(header));


	if( header.iVer != iBSPVer)
	{
		_asm int 3;
		return	false;
	}

	miTriCount				=	header.iTriCount;
	miPlaneCount			=	header.iPlaneCount;
	miNodeCount				=	header.iNodeCount;
	miNormalNodeCount		=	header.iNormalNodeCount;
	miLeafCount				=	header.iLeafCount;
	miNullLeafCount			=	header.iNullLeafCount;
	miMaxLevel				=	header.iMaxLevel;

	// 삼각형 읽기.
	{
		// 삼각형,
		mpTri		=	new VECTOR3[3*miTriCount];
		bResult		=	ReadDataFromFile( hFile, mpTri, sizeof(VECTOR3)*3*miTriCount);

		// 바운딩 스피어,
		mpTriSphere	=	new	BOUNDING_SPHERE[miTriCount];
		bResult		=	ReadDataFromFile( hFile, mpTriSphere, sizeof(BOUNDING_SPHERE)*miTriCount);

		// 평면 인덱스.
		mpPlaneIndex	=	new	DWORD[miTriCount];
		bResult		=	ReadDataFromFile( hFile, mpPlaneIndex, sizeof(DWORD)*miTriCount);
	}

	// 평면 읽기.
	{
		mpPlane		=	new	PLANE[miPlaneCount];
		bResult		=	ReadDataFromFile( hFile, mpPlane, sizeof(PLANE)*miPlaneCount);
	}

	// bsp 노드들.
	{
		mpNode		=	new	BSPTreeNode[miNodeCount];
		bResult		=	ReadDataFromFile( hFile, mpNode, sizeof(BSPTreeNode)*miNodeCount);
	}
	
	// 노드가 가지는 삼각형들 버퍼.
	{
		bResult		=	ReadDataFromFile( hFile, &miNodeTriIndexBuffUsedCount, sizeof(int));
		
		mpNodeTriIndexBuff		=	new	int[miNodeTriIndexBuffUsedCount];
		bResult		=	ReadDataFromFile( hFile, mpNodeTriIndexBuff, sizeof(int)*miNodeTriIndexBuffUsedCount);
	}

	miNodeTemp		=	0;

	//	찾기용 버퍼 만들기.
	this->pOutputBuffer	=	new DWORD[this->miTriCount];
	memset( pOutputBuffer, 0, sizeof(DWORD)*miTriCount);
	this->mpTempOutput	=	new BYTE[this->miTriCount];	// 출력용 인덱스 버퍼.
	memset( mpTempOutput, 0, sizeof(BYTE)*miTriCount);

	return	true;

}

bool	MBSPTree::ReadDataFromFile( HANDLE hFile, void* pData, DWORD dwCountOfByte)
{
		DWORD	dwTemp		=	0;
		BOOL	bResult		=	ReadFile( hFile, pData, dwCountOfByte, &dwTemp, 0);
		if( bResult == FALSE || dwCountOfByte != dwTemp)
		{
			_asm int 3;
			return	false;
		}
		return	true;
}

bool	MBSPTree::WriteDataToFile( HANDLE hFile, void* pData, DWORD dwCountOfByte)
{
	DWORD	dwTemp	=	0;
	BOOL	bResult		=	WriteFile( hFile, pData, dwCountOfByte, &dwTemp, 0);
	if( dwTemp != dwCountOfByte || bResult == FALSE)
	{
		_asm int 3;
		return	false;
	}
	return	true;
}
