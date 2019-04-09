// CollisionMeshObjectTree.cpp: implementation of the CCollisionMeshObjectTree class.
//
//////////////////////////////////////////////////////////////////////

#include "CollisionMeshObjectTree.h"
#include "CollisionStaticModel.h"
#include "../4DyuchiGXGFunc/global.h"
#include	"../4DyuchiGRX_myself97/myself97common.h"
#include	"../4DyuchiGRX_myself97/CheckClock.h"
#include	"../4DyuchiGRX_myself97/CollisionTest/CollisionTestMovingEllipsoidMeetTriangle.h"





/*
struct	BSP_BUILD_INFO1
{
	// phase1
	DWORD						dwTriCount;
	DWORD						dwCurrentCount;
	VECTOR3**					ppTriBuff;					// 삼각형의 포인터 버퍼.
	PLANE**						ppPlaneBuff;				// 평면의 포인터 버퍼.
	CCollisionMeshObject**		ppObjPointer;
};
BSP_BUILD_INFO1		gBuildInfo;

struct	BSP_BUILD_INFO2
{
	DWORD						dwPlaneCount;
	PLANE**						ppResultPlane;				// 평면 모아둘 버퍼.
	DWORD*						pPlaneReffCount;			// 해당 인덱스의 평면이 몇번이나 참조됬나?
	DWORD*						pResultPlaneIndex;			// 해당 삼각형의 평면 인덱스.
};
BSP_BUILD_INFO2		gPhase2;
*/


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCollisionMeshObjectTree::CCollisionMeshObjectTree()
{
	m_Triangle.dwCount		=	0;
	m_Triangle.ppBuff		=	0;
	m_Triangle.pdwPlaneIndex	=	0;
	m_Triangle.pBox			=	0;

	m_Plane.pPlane			=	0;
	m_Plane.dwCount			=	0;

	m_dwMemoryUsage			=	0;

	m_pOutputBuffer			=	0;
	m_dwOutputBufferCount	=	0;
//	m_pOutputLink			=	0;
	

	memset( &m_Grid, 0, sizeof(m_Grid));
}

CCollisionMeshObjectTree::~CCollisionMeshObjectTree()
{
	// 메모리 삑사리 체크.
	if( m_Triangle.dwCount != 0)
		_asm int 3;
	if( m_Triangle.ppBuff != 0)
		_asm int 3;
	if( m_Triangle.pdwPlaneIndex != 0)
		_asm int 3;

	if( m_Plane.dwCount != 0)
		_asm int 3;
	if( m_Plane.pPlane != 0)
		_asm int 3;

}


BOOL CCollisionMeshObjectTree::CreateTree(CCollisionMeshObject *pObjBuff, DWORD dwObjCount)
{
	PrintfDebugString("CCollisionMeshObjectTree::CreateTree() ObjCount = %d\n", dwObjCount);
//	PrintfDebugCodeLine();
	DWORD	dwFuncClock	=	GetTickCount();
	
	DWORD	i, j;

	DWORD	dwTriPhaseClock	=	GetTickCount();

	// 오브젝트를 뒤져 삼각형의 갯수를 얻는다.
	CCollisionMeshObject*	pObj	=	0;
	m_Triangle.dwCount	=	0;
	for( i = 0;i < dwObjCount; i++)
	{
		pObj	=	&(pObjBuff[i]);
		m_Triangle.dwCount	+=	pObj->GetFaceCount();
	}

	// 돌면서 삼각형과 평면을 얻는다.
	m_Triangle.ppBuff			=	new	VECTOR3*[m_Triangle.dwCount];		// 삼각형 버퍼.
	m_Triangle.pdwPlaneIndex	=	new	DWORD[m_Triangle.dwCount];			// 삼각형의 평면 인덱스.
	m_Triangle.ppObjBuff		=	new	CCollisionMeshObject*[m_Triangle.dwCount];		// 오브젝트 버퍼.
	m_Triangle.pBox				=	new	MAABB[m_Triangle.dwCount];


	m_Plane.pPlane				=	new	PLANE[m_Triangle.dwCount];			// 평면이 들어갈 임시버퍼로 쓴다..
#ifdef	_DEBUG
	// 혹시나 모를 멤셋.
	memset( m_Triangle.ppBuff, 0, sizeof(VECTOR3*)*m_Triangle.dwCount);
	memset( m_Triangle.pdwPlaneIndex, 0, sizeof(DWORD)*m_Triangle.dwCount);
	memset( m_Triangle.ppObjBuff, 0, sizeof(CCollisionMeshObject*)*m_Triangle.dwCount);
	memset( m_Plane.pPlane, 0, sizeof(PLANE)*m_Triangle.dwCount);
#endif
	DWORD	dwTemp	=	0;
	for( i = 0; i < dwObjCount; i++)
	{
		pObj	=	&(pObjBuff[i]);
		DWORD			dwObjTriCount	=	pObj->GetFaceCount();
		VECTOR3*		pObjTriBuff		=	pObj->GetTriBuffer();
		for( j = 0; j < dwObjTriCount; j++)
		{
			// 삼각형 대입.
			m_Triangle.ppBuff[dwTemp]	=	&(pObjTriBuff[j*3]);			// 삼각형 버퍼.
			// 오브젝트 대입
			m_Triangle.ppObjBuff[dwTemp]	=	&(pObjBuff[i]);
			// 평면 만들어 넣는다.

			// 로딩 타임을 줄이기 위해 매쉬에서 직접 얻어올것...당연히 예전 맵데이타라면 다시 익스포트해야...-yuchi
//			m_Plane.ppPlane[dwTemp]		=	&(pObjPlane[j]);				// 평면 버퍼.
//			MakePLANE( &(m_Plane.pPlane[dwTemp]), m_Triangle.ppBuff[dwTemp], m_Triangle.ppBuff[dwTemp]+1, m_Triangle.ppBuff[dwTemp]+2);
			CalcPlaneEquation( &(m_Plane.pPlane[dwTemp]), m_Triangle.ppBuff[dwTemp]);
			
			float	fTemp	=	VECTOR3Length( &(m_Plane.pPlane[dwTemp].v3Up));
			if( fTemp <= 0.9999f)
				_asm int 3;
			if( fTemp > 1.1f)
				_asm int 3;
			// MAABB 만들어넣는다.
			BOOL	bMakeAABB	=	MakeAABBFromTri( &(m_Triangle.pBox[dwTemp]), m_Triangle.ppBuff[dwTemp]);
			if( bMakeAABB == FALSE)
			{
				// 부피가 없는 삼각형이 들어왔다.
				PrintfDebugString( "CCollisionMeshObjectTree::CreateTree() 부피가 없는 삼각형이 .col 파일에 섞여있다. (The Triangle has no Volume!)\n");
				_asm int 3;
			}

			
			dwTemp++;
		}
	}

#ifdef	_DEBUG
	if( dwTemp != m_Triangle.dwCount)
		_asm int 3;
#endif
	dwTriPhaseClock	=	GetTickCount()	-	dwTriPhaseClock;
	PrintfDebugString("CCollisionMeshObjectTree::CreateTree() Tri Phase Tick = %d\n", dwTriPhaseClock);
	PrintfDebugString("CCollisionMeshObjectTree::CreateTree() TriCount = %d\n", m_Triangle.dwCount);




	// 평면을 돌면서 같은 평면을 골라낸다.
	PLANE*		pPlane	=	new	PLANE[m_Triangle.dwCount];			// 타겟 버퍼.
#ifdef	_DEBUG
	memset( pPlane, 0, sizeof(PLANE)*m_Triangle.dwCount);
#endif
	// 삼각형 갯수가 하나거나 없거나 하면,
	if( m_Triangle.dwCount < 2)
		_asm int 3;



	DWORD	dwFindSamePlaneClock	=	GetTickCount();



	// 일단 하나를 같은것으로 두고,
	pPlane[0]	=	m_Plane.pPlane[0];
	m_Triangle.pdwPlaneIndex[0]	=	0;
	dwTemp		=	1;

	// 삼각형 전체 평면을 돌면서,
	for( i = 1; i < m_Triangle.dwCount; i++)
	{
		PLANE*		pSrc	=	&(m_Plane.pPlane[i]);
		// 타겟 버퍼의 평면들을 돌아보면서 같은것을 찾는다.
		for( j = 0; j < dwTemp; j++)		
		{
			PLANE*		pDest	=	&(pPlane[j]);

			// 일단 D
			float	fDist	=	pSrc->D		-	pDest->D;
			if( fDist > -0.01f && fDist < 0.01f)
			{
				// 노말 벡터.
				float	fDot	=	DotProduct( &(pDest->v3Up), &(pSrc->v3Up));
				if( fDot > 1.000001f)
					_asm int 3;
				if( fDot > 0.99f)
				{
					// 이넘이 같은 넘이다.
					m_Triangle.pdwPlaneIndex[i]	=	j;
					break;		// 루프 빠져나간다.
				}
			}
				_asm nop
		}

		// 만약 못찾았으면,
		if( j == dwTemp)
		{
			pPlane[dwTemp]	=	*pSrc;					// 버퍼에 하나 복사하고,
			m_Triangle.pdwPlaneIndex[i]	=	dwTemp;		// 인덱스를 새 평면을 준 다음,
			dwTemp++;									// 타겟 평면 하나 증가.
		}
	}

//	dwTemp;// 새 평면의 갯수.


	// 평면 버퍼 정리.
	m_Plane.dwCount		=	dwTemp;
	delete[]	m_Plane.pPlane;
	m_Plane.pPlane		=	new	PLANE[dwTemp];

	m_dwMemoryUsage	+=	sizeof(DWORD)*1;
	m_dwMemoryUsage	+=	sizeof(PLANE)*m_Plane.dwCount;


	memcpy( m_Plane.pPlane, pPlane, sizeof(PLANE)*dwTemp);

	delete[]	pPlane;
	pPlane	=	0;

	// ok
	dwFindSamePlaneClock	=	GetTickCount()	-	dwFindSamePlaneClock;
	PrintfDebugString("CCollisionMeshObjectTree::CreateTree() Plane Phase Ticks = %d\n", dwFindSamePlaneClock);





	// 그리드 만든다.
	CreateGrid();

	// 아웃풋 버퍼.
	CreateFindBuffer( 1);			// 이때 2는 그냥 테스트값이다.

	// 메모리 사용량들.
	// 삼각형.
	m_dwMemoryUsage	+=	sizeof(DWORD)*m_Triangle.dwCount;
	m_dwMemoryUsage	+=	sizeof(VECTOR3*)*m_Triangle.dwCount;
	m_dwMemoryUsage	+=	sizeof(CCollisionMeshObject*)*m_Triangle.dwCount;
	m_dwMemoryUsage	+=	sizeof(MAABB)*m_Triangle.dwCount;

	// 그리드.
	m_dwMemoryUsage	+=	sizeof(M_GRID_SECTOR)*m_Grid.dwGridXCount*m_Grid.dwGridYCount*m_Grid.dwGridZCount;
	m_dwMemoryUsage	+=	sizeof(DWORD)*m_Grid.dwTriIndexCount;

	// 아웃풋 버퍼.
	m_dwMemoryUsage	+=	sizeof(DWORD)*m_Triangle.dwCount;
	// 아웃풋 링크.
	m_dwMemoryUsage	+=	(sizeof(OUTPUT_LINK)*m_Triangle.dwCount+1);

	dwFuncClock	=	GetTickCount()	-	dwFuncClock;
	PrintfDebugString("CCollisionMeshObjectTree::CreateTree() Total Build Tree Ticks = %d, MemoryUsage = %d\n", dwFuncClock, m_dwMemoryUsage);
	return	TRUE;
}


void CCollisionMeshObjectTree::DeleteTree()
{
	// 출력 버퍼 지운다.
	DeleteFindBuffer();

	// 그리드 지운다.
	DeleteGrid();

	// 삼각형 지운다.
	if(m_Plane.dwCount)
	{
		delete[]	m_Plane.pPlane;
		m_Plane.pPlane		=	0;
		m_Plane.dwCount		=	0;
	}
	if( m_Triangle.dwCount)
	{
		delete[]	m_Triangle.pdwPlaneIndex;
		m_Triangle.pdwPlaneIndex	=	0;
		delete[]	m_Triangle.ppBuff;
		m_Triangle.ppBuff			=	0;
		delete[]	m_Triangle.ppObjBuff;
		m_Triangle.ppObjBuff		=	0;
		delete[]	m_Triangle.pBox;
		m_Triangle.pBox				=	0;
		m_Triangle.dwCount			=	0;
	}

	
}

void CCollisionMeshObjectTree::CreateGrid()
{
	DWORD	i, j, k;
	DWORD	a, b, c;
	M_GRID_SECTOR*	pGrid;
	// 1. 전체 월드를 아래에 bsp를 둘거라고 가정하고 나누는 경우.?
	// 어느정도로 월드를 나눠야 하는가.?
	// 기준은? 월드 크기? 삼각형 갯수? 뭘로.?
	// 월드 크기가 좋겠다.
	// 부하가 가벼운 곳은 가벼운 대로, 무거운 곳은 무거운 대로. 갯수가 많으면 어쩌라고.

	// 2. 그리드 자체만으로 삼각형을 가지고 있을 경우. 어차피 같긴 하지만, 테스트도 겸 해서 좀 잘게 자르고,
	// 아주 작은 갯수의 삼각형을 가지고 트리를 구축한다.

	//아래껀 임시 간격. 실제로 간격은 다시 계산 되고, 희망 간격, 혹은 그리드 갯수는 외부에서 입력 할 예정이다.
	VECTOR3		vGridStep;
	vGridStep.x					=	100.0f;
	vGridStep.y					=	100.0f;
	vGridStep.z					=	100.0f;

	// 일단 월드 박스를 구한다.
	MAABB	Box;
	Box.Min	=	Box.Max	=	*(m_Triangle.ppBuff[0]);
	for( i = 0; i < m_Triangle.dwCount; i++)
	{
		VECTOR3*		pTriangle	=	m_Triangle.ppBuff[i];
		for( j = 0; j < 3; j++)
		{
			VECTOR3*	pV	=	pTriangle+j;		// 포인터 더하기. 주의.
			if( Box.Max.x < pV->x)
					Box.Max.x = pV->x;
			if( Box.Max.y < pV->y)
					Box.Max.y = pV->y;
			if( Box.Max.z < pV->z)
					Box.Max.z = pV->z;

			if( Box.Min.x > pV->x)
					Box.Min.x = pV->x;
			if( Box.Min.y > pV->y)
					Box.Min.y = pV->y;
			if( Box.Min.z > pV->z)
					Box.Min.z = pV->z;
		}
	}

	// +10.0f 정도씩 마진을...	 필요가 있는지는 안정된 다음 빼보고 판단.
	Box.Max.x	+=	10.0f;
	Box.Max.y	+=	10.0f;
	Box.Max.z	+=	10.0f;
	Box.Min.x	-=	10.0f;
	Box.Min.y	-=	10.0f;
	Box.Min.z	-=	10.0f;

	m_Grid.WorldBox	=	Box;

	// 그리드 갯수 구하고,
	m_Grid.dwGridXCount	=	DWORD((Box.Max.x - Box.Min.x)	/	vGridStep.x);
	if( m_Grid.dwGridXCount == 0)
			m_Grid.dwGridXCount		=	1;			// 최소 한개는 있어야....
	m_Grid.fGridXStep	=	(Box.Max.x	-	Box.Min.x)	/	m_Grid.dwGridXCount;		// 간격은 다시 계산.

	m_Grid.dwGridYCount	=	DWORD((Box.Max.y - Box.Min.y)	/	vGridStep.y);
	if( m_Grid.dwGridYCount == 0)
			m_Grid.dwGridYCount		=	1;			// 최소 한개는 있어야....
	m_Grid.fGridYStep	=	(Box.Max.y	-	Box.Min.y)	/	m_Grid.dwGridYCount;		// 간격은 다시 계산.

	m_Grid.dwGridZCount	=	DWORD((Box.Max.z - Box.Min.z)	/	vGridStep.z);
	if( m_Grid.dwGridZCount == 0)
			m_Grid.dwGridZCount		=	1;			// 최소 한개는 있어야....
	m_Grid.fGridZStep	=	(Box.Max.z	-	Box.Min.z)	/	m_Grid.dwGridZCount;		// 간격은 다시 계산.

	// 각 그리드를 생성하고, 내부 구조를 채운다. bsp는 좀 나중에.
	DWORD	dwGridCount	=	m_Grid.dwGridXCount * m_Grid.dwGridYCount * m_Grid.dwGridZCount;
//	m_Grid.

	m_Grid.pGrid		=	new	M_GRID_SECTOR[dwGridCount];


	memset( m_Grid.pGrid, 0, sizeof(M_GRID_SECTOR)*dwGridCount);

	// 그리드를 돌면서 AABB를 채운다.
	for( k = 0; k < m_Grid.dwGridZCount; k++)
	{
		for( j = 0; j < m_Grid.dwGridYCount; j++)
		{
			for( i = 0; i < m_Grid.dwGridXCount; i++)
			{
				_asm nop;
				// 먼저 해당 그리드의 AABB 박스를 만든다.
				pGrid		=	m_Grid.GetSector( i, j, k);
				Box.Min.x	=	m_Grid.WorldBox.Min.x + ((m_Grid.WorldBox.Max.x - m_Grid.WorldBox.Min.x) / m_Grid.dwGridXCount) * i;
				Box.Max.x	=	m_Grid.WorldBox.Min.x + ((m_Grid.WorldBox.Max.x - m_Grid.WorldBox.Min.x) / m_Grid.dwGridXCount) * (i+1);

				Box.Min.y	=	m_Grid.WorldBox.Min.y + ((m_Grid.WorldBox.Max.y - m_Grid.WorldBox.Min.y) / m_Grid.dwGridYCount) * j;
				Box.Max.y	=	m_Grid.WorldBox.Min.y + ((m_Grid.WorldBox.Max.y - m_Grid.WorldBox.Min.y) / m_Grid.dwGridYCount) * (j+1);

				Box.Min.z	=	m_Grid.WorldBox.Min.z + ((m_Grid.WorldBox.Max.z - m_Grid.WorldBox.Min.z) / m_Grid.dwGridZCount) * k;
				Box.Max.z	=	m_Grid.WorldBox.Min.z + ((m_Grid.WorldBox.Max.z - m_Grid.WorldBox.Min.z) / m_Grid.dwGridZCount) * (k+1);

				pGrid->BB	=	Box;
			}
		}
	}



	////////////////////////////////////////////////////////////////
	// 삼각형 을 그리드 안에 넣는다.
	////////////////////////////////////////////////////////////////

	DWORD	dwIntegratedBuffSize	=	0;
	for( i = 0; i < m_Triangle.dwCount; i++)			// 삼각형을 돌아보면서,
	{
		VECTOR3*		pTri	=	m_Triangle.ppBuff[i];
		MAABB*			pBox	=	&(m_Triangle.pBox[i]);
		PLANE*			pPlane	=	&(m_Plane.pPlane[m_Triangle.pdwPlaneIndex[i]]);
		// 이 삼각형이 들어갈 그리드들을 구한다.
		DWORD	dwXStart	=	DWORD((pBox->Min.x - m_Grid.WorldBox.Min.x) / m_Grid.fGridXStep);
		DWORD	dwXEnd		=	DWORD((pBox->Max.x - m_Grid.WorldBox.Min.x) / m_Grid.fGridXStep);

		DWORD	dwYStart	=	DWORD((pBox->Min.y - m_Grid.WorldBox.Min.y) / m_Grid.fGridYStep);
		DWORD	dwYEnd		=	DWORD((pBox->Max.y - m_Grid.WorldBox.Min.y) / m_Grid.fGridYStep);

		DWORD	dwZStart	=	DWORD((pBox->Min.z - m_Grid.WorldBox.Min.z) / m_Grid.fGridZStep);
		DWORD	dwZEnd		=	DWORD((pBox->Max.z - m_Grid.WorldBox.Min.z) / m_Grid.fGridZStep);

#ifdef	_DEBUG
	// 월드박스보다 삼각형이 더 멀리 있는 경우. 삼각형을 기준으로 월드박스를 만들었는데 월드박스를 벗어나는 삼각형은 있을 수 없다.
	if( dwXStart > m_Grid.dwGridXCount || dwXEnd > m_Grid.dwGridXCount)
		_asm int 3;
	if( dwYStart > m_Grid.dwGridYCount || dwYEnd > m_Grid.dwGridYCount)
		_asm int 3;
	if( dwZStart > m_Grid.dwGridZCount || dwZEnd > m_Grid.dwGridZCount)
		_asm int 3;
#endif

		// 시작과 끝이 나왔으므로 해당 그리드에 삼각형들을 집어넣는다.



		for( c = dwZStart; c <= dwZEnd; c++)
		{
			for( b = dwYStart; b <= dwYEnd; b++)
			{
				for( a = dwXStart; a <= dwXEnd; a++)
				{
					// 전체 그리드에 쓰일 단일 버퍼를 만들기 전에 필요량을 센다.
					_asm nop;
					pGrid	=	m_Grid.GetSector(a, b, c);
					// 정말 이 그리드에 삼각형이 포함 되는가.?
					// 넣기 전에 테스트.
					BOOL	bGridMeetTri	=	TestPlaneWithGrid( pGrid, pPlane);
					if( bGridMeetTri)
					{
						// 일단 빌드타임때 임시로 링크리스트를 dwTriStartIndex에다 쑤셔넣고, 
						// 삼각형 인덱스 링크를 추가.
						M_TRIANGLE_LINK*		pLink	=	new	M_TRIANGLE_LINK;
						pLink->dwIndex		=	i;
						if( pGrid->pLink)
						{
							pLink->pNext	=	pGrid->pLink;			// 중간에 끼워넣는다.
							pGrid->pLink	=	pLink;
						}
						else	// pGrid->pLink == 0;
						{
							pGrid->pLink			=	pLink;
							pGrid->pLink->pNext		=	0;
						}


						pGrid->dwTriCount++;
						dwIntegratedBuffSize++;
					}
					else
						_asm nop;

				}
			}
		}
		_asm nop;
	}

	dwIntegratedBuffSize;

	DWORD*		pTriIndexBuffer	=	new	DWORD[dwIntegratedBuffSize];
	DWORD		dwTriBuffUsage	=	0;


	for( c = 0; c < m_Grid.dwGridZCount; c++)
	{
		for( b = 0; b < m_Grid.dwGridYCount; b++)
		{
			for( a = 0; a < m_Grid.dwGridXCount; a++)
			{
				pGrid	=	m_Grid.GetSector(a, b, c);
//				if( pGrid->bCheck)
//					_asm int 3;
//				pGrid->bCheck	=	TRUE;
				// 링크를 돌면서 버퍼에 인덱스를 집어넣는다.
				if( pGrid->dwTriCount)		// 삼각형이 하나라도 있으면,
				{
					DWORD	dwStartIndex	=	dwTriBuffUsage;	// pGrid->dwStart   여기다 바로 넣으면 공용체인 pLink가 깨져버린다.
					M_TRIANGLE_LINK*		pLink	=	pGrid->pLink;
					M_TRIANGLE_LINK*		pNext;
					while( pLink)
					{
						pNext	=	pLink->pNext;		// 지울거를 대비해서...
						pTriIndexBuffer[dwTriBuffUsage]	=	pLink->dwIndex;
						dwTriBuffUsage++;
						delete	pLink;
						pLink	=	pNext;
					}
//					if( pGrid->pLink)
//						_asm int 3;
					pGrid->pLink		=	0;
					pGrid->dwTriStartIndex	=	dwStartIndex;			// 전역 버퍼에서의 시작주소.
				}
				else						// 하나도 없으면 그냥 넘어가면 되지.?
				{
					pGrid->dwTriStartIndex	=	0xffffffff;		// 당연 없지.
					if( pGrid->dwTriCount != 0)
						_asm int 3;
				}
				
				_asm nop;
			}
		}
	}
	_asm nop;
	m_Grid.pTriIndexBuffer		=	pTriIndexBuffer;
	m_Grid.dwTriIndexCount		=	dwTriBuffUsage;

	// 그리드에 삼각형을 넣는 족족 링크 날리는걸 잊지 말자.
	_asm nop
}

void CCollisionMeshObjectTree::DeleteGrid()
{
	if( m_Grid.pGrid)
	{
		if( m_Grid.dwTriIndexCount)
		{
			delete[]	m_Grid.pTriIndexBuffer;
			m_Grid.pTriIndexBuffer		=	0;
		}

		delete[]	m_Grid.pGrid;
		m_Grid.pGrid	=	0;
	}
	_asm nop
}

// 그리드와 삼각형이 만나는가.?
BOOL CCollisionMeshObjectTree::TestPlaneWithGrid(M_GRID_SECTOR *pGrid, PLANE* pPlane)
{
	VECTOR3		v[8];
	// x > 0, y > 0, z > 0
	v[0]	=	pGrid->BB.Max;
	// x > 0, y > 0, z < 0
	v[1].x	=	pGrid->BB.Max.x;
	v[1].y	=	pGrid->BB.Max.y;
	v[1].z	=	pGrid->BB.Min.z;
	// x > 0, y < 0, z > 0
	v[2].x	=	pGrid->BB.Max.x;
	v[2].y	=	pGrid->BB.Min.y;
	v[2].z	=	pGrid->BB.Max.z;
	// x > 0, y < 0, z < 0
	v[3].x	=	pGrid->BB.Max.x;
	v[3].y	=	pGrid->BB.Min.y;
	v[3].z	=	pGrid->BB.Min.z;
	// x < 0, y > 0, z > 0
	v[4].x	=	pGrid->BB.Min.x;
	v[4].y	=	pGrid->BB.Max.y;
	v[4].z	=	pGrid->BB.Max.z;
	// x < 0, y > 0, z < 0
	v[5].x	=	pGrid->BB.Min.x;
	v[5].y	=	pGrid->BB.Max.y;
	v[5].z	=	pGrid->BB.Min.z;
	// x < 0, y < 0, z > 0
	v[6].x	=	pGrid->BB.Min.x;
	v[6].y	=	pGrid->BB.Min.y;
	v[6].z	=	pGrid->BB.Max.z;
	// x < 0, y < 0, z < 0
	v[7].x	=	pGrid->BB.Min.x;
	v[7].y	=	pGrid->BB.Min.y;
	v[7].z	=	pGrid->BB.Min.z;

	DWORD	i;
	DWORD	dwPositive	=	0;		// 평면 앞쪽.
	DWORD	dwNegative	=	0;		// 평면 뒷쪽.
	for( i = 0; i < 8; i++)
	{
		float	fDet	=	DotProduct( &(pPlane->v3Up), &(v[i]))	+	pPlane->D;
		if( fDet < 0.0f)
			dwNegative++;
//		else
//			dwPositive++;
	}

	if( dwNegative != 8)
		return	TRUE;

//aa

	// 일단 평면 테스트,
//	PLANE*	pPlane	=	m_Plane.pPlane[m_Triangle.pdwPlaneIndex[dwTriIndex]];

	return	FALSE;
}

/*
DWORD CCollisionMeshObjectTree::FindTriangleWithBS(BOUNDING_SPHERE *pBS)
{

	return	0;
}
*/

DWORD CCollisionMeshObjectTree::FindTriangleWithBC(BOUNDING_CAPSULE *pBC, BOOL bFaceFromVertex, DWORD dwOutputBufferIndex)
{
#ifdef	_DEBUG
	DBG_CheckOutputBufferIndex( dwOutputBufferIndex);
#endif
	// 시작 그리드와 끝 그리드는?
	MAABB	Box;
	// x
	if( pBC->v3From.x > pBC->v3To.x)
	{
		Box.Min.x	=	pBC->v3To.x;
		Box.Max.x	=	pBC->v3From.x;
	}
	else
	{
		Box.Min.x	=	pBC->v3From.x;
		Box.Max.x	=	pBC->v3To.x;
	}
	Box.Min.x	-=	pBC->fRadius;
	Box.Max.x	+=	pBC->fRadius;
	
	// y
	if( pBC->v3From.y > pBC->v3To.y)
	{
		Box.Min.y	=	pBC->v3To.y;
		Box.Max.y	=	pBC->v3From.y;
	}
	else
	{
		Box.Min.y	=	pBC->v3From.y;
		Box.Max.y	=	pBC->v3To.y;
	}
	Box.Min.y	-=	pBC->fRadius;
	Box.Max.y	+=	pBC->fRadius;

	// z
	if( pBC->v3From.z > pBC->v3To.z)
	{
		Box.Min.z	=	pBC->v3To.z;
		Box.Max.z	=	pBC->v3From.z;
	}
	else
	{
		Box.Min.z	=	pBC->v3From.z;
		Box.Max.z	=	pBC->v3To.z;
	}
	Box.Min.z	-=	pBC->fRadius;
	Box.Max.z	+=	pBC->fRadius;
	
	// 시작, 끝 인덱스.
	int		iXStart	=	int( (Box.Min.x - m_Grid.WorldBox.Min.x) / m_Grid.fGridXStep);
	int	iXEnd		=	int( (Box.Max.x - m_Grid.WorldBox.Min.x) / m_Grid.fGridXStep);
	
	int	iYStart	=	int( (Box.Min.y - m_Grid.WorldBox.Min.y) / m_Grid.fGridYStep);
	int	iYEnd		=	int( (Box.Max.y - m_Grid.WorldBox.Min.y) / m_Grid.fGridYStep);
	
	int	iZStart	=	int( (Box.Min.z - m_Grid.WorldBox.Min.z) / m_Grid.fGridZStep);
	int	iZEnd		=	int( (Box.Max.z - m_Grid.WorldBox.Min.z) / m_Grid.fGridZStep);


	if( iXStart < 0)			// '-' 값이 나올 수도 있다.
			iXStart	=	0;
	if( iXEnd >= (int)m_Grid.dwGridXCount)
		iXEnd	=	m_Grid.dwGridXCount-1;
	if( iYStart < 0)			// '-' 값이 나올 수도 있다.
			iYStart	=	0;
	if( iYEnd >= (int)m_Grid.dwGridYCount)
		iYEnd	=	m_Grid.dwGridYCount-1;
	if( iZStart < 0)			// '-' 값이 나올 수도 있다.
			iZStart	=	0;
	if( iZEnd >= (int)m_Grid.dwGridZCount)
		iZEnd	=	m_Grid.dwGridZCount-1;

	

	// 아웃풋 링크 클리어 하고.
	ClearOutputLink( dwOutputBufferIndex);
	OUTPUT_INDEX_BUFFER*	pBuffer		=	&(m_pOutputBuffer[dwOutputBufferIndex]);
	int	i, j, k;
	for( k = iZStart; k <= iZEnd; k++)
	{
		for( j = iYStart; j <= iYEnd; j++)
		{
			for( i = iXStart; i <= iXEnd; i++)
			{
				M_GRID_SECTOR*	pGrid	=	m_Grid.GetSector( i, j, k);
				DWORD			dwStart	=	pGrid->dwTriStartIndex;						// 시작 인덱스.
				DWORD			dwEnd	=	pGrid->dwTriCount	+	dwStart;			// 끝 인덱스 바로다음(?).
				// 이 그리드의 삼각형 인덱스들을 아웃풋 버퍼에 벹어낸다.
				DWORD	a;
				for( a = dwStart; a < dwEnd; a++)
				{
					int			iIndex	=	m_Grid.pTriIndexBuffer[a];		// 인덱스 두다리 거쳐서지.?

					// 일단 BC에 평면이 닫는가.?
					PLANE*		pP	=	&(m_Plane.pPlane[m_Triangle.pdwPlaneIndex[iIndex]]);
//					int		iMeet	=	BoundingCapsuleMeetPlane( pBC, pP);
//					if( i > 0)
					{
						// face 옵션 들어가있으면, 
						if( bFaceFromVertex)			// 평면이 pBC->v3From을 보고 있는지 체크 하고 
						{
							float	fDet	=	DotProduct( &(pP->v3Up), &(pBC->v3From))	+	pP->D;
							if( fDet >= 0)
							{
//								int		iMeet	=	BoundingCapsuleMeetPlane( pBC, pP);
//								if( iMeet >= 0)
									AddOutputLink( iIndex, dwOutputBufferIndex);	// Output 버퍼에 출력.
//								else
									_asm nop;
							}
								
						}
						else
						{
							int		iMeet	=	BoundingCapsuleMeetPlane( pBC, pP);
							if( iMeet > 0)
								AddOutputLink( iIndex, dwOutputBufferIndex);		// Output 버퍼에 그냥 출력.

						}
					}


//					여기
					_asm nop;
				}

			}
		}
	}
	
	// 출력 링크 정리.
//	m_dwOutputIndexCount		=	0;
	pBuffer->dwOutputIndexCount		=	0;
//	DWORD	dwCurrentLink	=	m_pOutputLink[0].dwNext;			// 0번이야 -1이니까.
	DWORD	dwCurrentLink	=	pBuffer->pOutputLink[0].dwNext;
	while(1)
	{
//		OUTPUT_LINK*	pCurrentLink	=	&(m_pOutputLink[dwCurrentLink]);
		OUTPUT_LINK*	pCurrentLink	=	&(pBuffer->pOutputLink[dwCurrentLink]);

		// 끝 노드라면..
		if( pCurrentLink->iIndex == INT_MAX)		
			break;

		// 끝노드가 아니라면 버퍼에 기록.
//		m_pdwOutputIndexBuffer[m_dwOutputIndexCount]	=	pCurrentLink->iIndex;
//		m_dwOutputIndexCount++;
		pBuffer->pdwOutputIndexBuffer[pBuffer->dwOutputIndexCount]		=	pCurrentLink->iIndex;
		pBuffer->dwOutputIndexCount++;
		dwCurrentLink	=	pCurrentLink->dwNext;
	}
	


//	return	m_dwOutputIndexCount;
	return	pBuffer->dwOutputIndexCount;
}

void CCollisionMeshObjectTree::AddOutputLink(int iIndex, DWORD dwBufferIndex)
{
#ifdef	_DEBUG
	DBG_CheckOutputBufferIndex( dwBufferIndex);
#endif
	
	DWORD		dwCurrent	=	0;			// 헤드 노드.

#ifdef	_DEBUG
	if( iIndex < 0)
		_asm int 3;
	if( iIndex >= INT_MAX)
		_asm int 3;
#endif

	OUTPUT_INDEX_BUFFER*		pBuffer		=	&(m_pOutputBuffer[dwBufferIndex]);

	while(1)
	{
		int		iCurrentIndex	=	pBuffer->pOutputLink[dwCurrent].iIndex;
		// 이미 갖고 있는 인덱스가 들어왔다면, 별 말 없이 끝낸다.
		if( iCurrentIndex == iIndex)
			break;		// return?
		// 지금 갖고 있는 노드보다 작다면 다음 노드로 넘어간다.
		if( iCurrentIndex < iIndex)
		{
			dwCurrent	=	pBuffer->pOutputLink[dwCurrent].dwNext;
			continue;
		}
#ifdef	_DEBUG
		if( iCurrentIndex <= iIndex)
			_asm int 3;
#endif
		// 지금 갖고 있는 노드가 iIndex보다 클 경우.
		// 앞의 노드와 지금 노드 사이에 노드를 생성하고 iIndex를 끼워넣는다.
		DWORD	dwNewNode	=	pBuffer->dwOutputLinkCurrent;//m_dwOutputLinkCurrent;
//		m_dwOutputLinkCurrent++;
		pBuffer->dwOutputLinkCurrent++;
//		m_pOutputLink[dwNewNode].dwNext	=	dwCurrent;
//		m_pOutputLink[dwNewNode].dwPrev	=	m_pOutputLink[dwCurrent].dwPrev;
//		m_pOutputLink[dwNewNode].iIndex	=	iIndex;
		pBuffer->pOutputLink[dwNewNode].dwNext	=	dwCurrent;
		pBuffer->pOutputLink[dwNewNode].dwPrev	=	pBuffer->pOutputLink[dwCurrent].dwPrev;
		pBuffer->pOutputLink[dwNewNode].iIndex	=	iIndex;

		DWORD	dwPrevNode	=	pBuffer->pOutputLink[dwCurrent].dwPrev;
		pBuffer->pOutputLink[dwPrevNode].dwNext	=	dwNewNode;

		pBuffer->pOutputLink[dwCurrent].dwPrev		=	dwNewNode;
		break;
	}




}

void CCollisionMeshObjectTree::ClearOutputLink( DWORD dwIndex)
{
#ifdef	_DEBUG
	DBG_CheckOutputBufferIndex( dwIndex);
#endif
	m_pOutputBuffer[dwIndex].dwOutputLinkCurrent	=	2;
//	m_dwOutputLinkCurrent	=	2;				// 할당을 할때의 인덱스.

//	m_pOutputLink[0].dwPrev		=	0;
//	m_pOutputLink[0].dwNext		=	1;
//	m_pOutputLink[0].iIndex		=	-1;	// 어쨌든 이놈은 제일 앞놈이다.
	m_pOutputBuffer[dwIndex].pOutputLink[0].dwPrev		=	0;
	m_pOutputBuffer[dwIndex].pOutputLink[0].dwNext		=	1;
	m_pOutputBuffer[dwIndex].pOutputLink[0].iIndex		=	-1;	// 어쨌든 이놈은 제일 앞놈이다.

//	m_pOutputLink[1].dwNext		=	0;
//	m_pOutputLink[1].dwPrev		=	0;
//	m_pOutputLink[1].iIndex		=	INT_MAX;		// 어쨌든 이놈은 제일 뒷놈이다.
	m_pOutputBuffer[dwIndex].pOutputLink[1].dwNext		=	0;
	m_pOutputBuffer[dwIndex].pOutputLink[1].dwPrev		=	0;
	m_pOutputBuffer[dwIndex].pOutputLink[1].iIndex		=	INT_MAX;		// 어쨌든 이놈은 제일 뒷놈이다.

	
}

BOOL CCollisionMeshObjectTree::CollisionTestLine(float *pOutT, VECTOR3 *pFrom, VECTOR3 *pTo, DWORD dwOutputBufferIndex)
{
#ifdef	_DEBUG
	DBG_CheckOutputBufferIndex( dwOutputBufferIndex);
#endif

	// 원래는 트리를 거쳐야 하지만 '임시'로 루프 돌면서 직접 찾는다.
	VECTOR3		vVelocity;//	=	*pTo	-	*pFrom;
	VECTOR3_SUB_VECTOR3( &vVelocity, pTo, pFrom);
#ifdef	_DEBUG
	if( *pTo == *pFrom)
	{
		PrintfDebugString( "CCollisionMeshObjectTree::CollisionTestLine() pFrom과 pTo가 같다.( pFrom and pTo are Same!)\n");
		_asm int 3;
		return	FALSE;
	}
#else
	if( *pTo == *pFrom)
		return	FALSE;


	if( vVelocity.x	!= 0.0f)
		_asm nop;
	if( vVelocity.z != 0.0f)
		_asm nop;

#endif



	// 트리를 돌려서 일단 삼각형 리스트를 얻는다.
	BOUNDING_CAPSULE		BC;
//	BC.fRadius	=	1.0f;
	BC.fRadius	=	0.0f;
	BC.v3From	=	*pFrom;
	BC.v3To		=	*pTo;
	

	DWORD	dwFindTreeClock	=	GetLowClock();

	DWORD	dwTriFound	=	FindTriangleWithBC(&BC, TRUE, dwOutputBufferIndex);
	if( dwTriFound == 0)		// 찾은거 없으면 바로 빠져나가고,
		return	FALSE;

	dwFindTreeClock	=	GetLowClock()	-	dwFindTreeClock;

	DWORD*	pdwFoundTri	=	this->GetResultIndex(dwOutputBufferIndex);

	float	fNearestT	=	1.1f;				// 그냥 1.0보다 큰수.
	DWORD	i;
	DWORD	dwIndex;
	for( i = 0; i < dwTriFound; i++)
	{
		dwIndex		=	pdwFoundTri[i];
		
		PLANE*	pPlane	=	&(m_Plane.pPlane[m_Triangle.pdwPlaneIndex[dwIndex]]);
		// 일단 그 삼각형의 평면과 주어진 선분이 만나는지를 판별한다.
		float	fDot1	=	DotProduct( pFrom, &(pPlane->v3Up))	+	pPlane->D;
//		if( fDot1 > -0.0001f && fDot1 < 0.0001f)
//			fDot1	=	0.0f;
		float	fDot2	=	DotProduct( pTo, &(pPlane->v3Up))		+	pPlane->D;
//		if( fDot2 > -0.0001f && fDot2 < 0.0001f)
//			fDot2	=	0.0f;
		if( fDot1 * fDot2 >= -0.00001f)
		{
			// 패스.
			continue;
		}

		float	t	=	-1.0f * (DotProduct( &(pPlane->v3Up), pFrom) + pPlane->D) / (DotProduct( &(pPlane->v3Up),  &vVelocity));
#ifdef	_DEBUG
		if( t > 1.0f || t < 0.0f)
//	_asm int 3;					// 여기서 걸리면 fDot 결과가 거짓말.
			_asm nop
#endif
		// 평면위의 점.
		VECTOR3		OnPlane	=	*pFrom + vVelocity * t;

		// 만나는가.?
		BOOL	bMeet	=	IsTriMeetVertex( m_Triangle.ppBuff[dwIndex], &OnPlane);
		if( bMeet)
		{
			// t 체크.
			if( fNearestT > t)
			{
				fNearestT	=	t;
			}
			else
				_asm nop
		}

	}

	// 만나지 않았다면 그냥 리턴.
	if( fNearestT	==	1.1f)
		return	FALSE;

	// 만났다면 리턴값 정리하고 리턴.
	*pOutT	=	fNearestT;

	return	TRUE;

}


BOOL CCollisionMeshObjectTree::CollisionTestAAELLIPSOID(VECTOR3 *pOut, VECTOR3 *pOutLastVelocity, BOOL *pbOutLanding, AAELLIPSOID *pEllipsoid, VECTOR3 *pTo, DWORD dwOutputBufferIndex)
{
#ifdef	_DEBUG
	DBG_CheckOutputBufferIndex( dwOutputBufferIndex);
#endif

	BOOL			bFuncResult	=	FALSE;					// 최종 결과.
	*pbOutLanding			=	FALSE;

	pOutLastVelocity->x		=	0.0f;
	pOutLastVelocity->y		=	0.0f;
	pOutLastVelocity->z		=	0.0f;

	*pOut	=	*pTo;

	if( pEllipsoid->P == *pTo)
	{
		// 움직이지도 안는구만.
		*pOut	=	*pTo;
		return	bFuncResult;
	}
	
	BOOL	bLanding	=	FALSE;		// 결과물이 바닥에 닿았나.?
	
	float		fRealTime	=	0.0f;		// 실제 속도벡터로 주어진 시간.
	// 예를 들면 익스큐티브 실행 프레임이 30프레임일 경우, 충돌 처리 함수는 1/30 초마다 한번 호출 되고,
	// fRealTime은 1/30 초에서 다음 1/30초까지 흐른다.
	// 이것은 속도 벡터를 추적해서 다시 리턴하기 위해 쓰인다.

	// 속도 벡터.?
	VECTOR3		vCurrentVelocity	=	*pTo	-	pEllipsoid->P;
	// 속도.
	float		fCurrentVelocity	=	VECTOR3Length( &vCurrentVelocity);
	if( fCurrentVelocity == 0.0f)
		_asm int 3;		// 위에서 걸러져야되는거 아닌가.?



	// 현재 위치.
	AAELLIPSOID			CurrentEllipsoid	=	*pEllipsoid;
	
	VECTOR3				vPrevVelocity;
	AAELLIPSOID			PrevEllipsoid;
	
	
	
	DWORD		dwLastMeetTriIndex	=	0xffffffff;
	DWORD	dwDebugLoopCount	=	0;
	while(1)
	{
		dwDebugLoopCount++;
		if( dwDebugLoopCount > 10)
			_asm nop;
		if( dwDebugLoopCount > 20)
		{
			// 그냥 멈췄다 하자.
			fCurrentVelocity	=	0.0f;
			vCurrentVelocity.x	=	0.0f;
			vCurrentVelocity.y	=	0.0f;
			vCurrentVelocity.z	=	0.0f;
		}

		// 만약 두번째 루프인데도 CurrentEllipsoid와 vCurrentVelocity가 바뀌지 않았다면 이거 문제 있는거지.
		if( fCurrentVelocity == 0.0f)
			break;

		// 밑에 내려가서 결과물이 바꼈는지 알아내기 위해 저장해둔다.
		vPrevVelocity		=	vCurrentVelocity;
		PrevEllipsoid		=	CurrentEllipsoid;


		// 트리로 관련 삼각형 긁어와서,
		BOUNDING_CAPSULE		BC;
		BC.fRadius		=	CurrentEllipsoid.h;
		if( CurrentEllipsoid.h < CurrentEllipsoid.w)
			BC.fRadius		=	CurrentEllipsoid.w+1000.0f;
		BC.v3From		=	CurrentEllipsoid.P;
		BC.v3To			=	CurrentEllipsoid.P + vCurrentVelocity;//*pTo;

		// 트리 타보고,
		DWORD	dwFindTreeClock	=	GetLowClock();
		DWORD	dwFoundTri	=	FindTriangleWithBC(&BC, TRUE, dwOutputBufferIndex);
		if( dwFoundTri > 50)
			_asm nop;			// 삼각형이 너무 많지 않은가.?
		dwFindTreeClock	=	GetLowClock()	-	dwFindTreeClock;

		// 트리 탄 버퍼.
		DWORD*	pFoundTriIndex	=	this->GetResultIndex( dwOutputBufferIndex);


		// 삼각형 각각에 대해 충돌 루프.
		DWORD	i;
		COLLISION_RESULT		CurrentOut;
		CurrentOut.fMeetTime	=	1.1f;
		VECTOR3*		pTargetTri;
		PLANE*			pTargetPlane;
		float			fDotPlaneVelocity;
		for( i = 0; i < dwFoundTri; i++)
		{
			
			// 인자용 무빙 타원체. 이거 타원체+속도로 바꿀까.?
			MOVING_ELLIPSOID			TestingEllipsoid;
			TestingEllipsoid.fHeight		=	CurrentEllipsoid.h;
			TestingEllipsoid.fWidth			=	CurrentEllipsoid.w;
			TestingEllipsoid.From			=	CurrentEllipsoid.P;
			TestingEllipsoid.Velocity		=	vCurrentVelocity;

			// 타겟 삼각형.
			pTargetTri		=	m_Triangle.ppBuff[pFoundTriIndex[i]];
			pTargetPlane	=	&(m_Plane.pPlane[m_Triangle.pdwPlaneIndex[pFoundTriIndex[i]]]);

			// 타겟 삼각형의 평면과 속도벡터가 평행하면 첨부터 패스.
			fDotPlaneVelocity	=	DotProduct( &(pTargetPlane->v3Up), &vCurrentVelocity);
			if( fDotPlaneVelocity >= 0.0f)
//			if( fDotPlaneVelocity >= -0.001f)
			{
				goto	lb_PassTest;
			}

			// 때려보고.
			COLLISION_RESULT		Out;		// 결과물 버퍼.
			BOOL	bFound;

			DWORD	dwCollTestClock;
			dwCollTestClock	=	GetLowClock();

			bFound	=	CollisionTestMovingEllipsoidMeetTriangle( &Out, &TestingEllipsoid, pTargetTri);
			dwCollTestClock	=	GetLowClock()	-	dwCollTestClock;

			if( bFound)
			{
				// 제일 가까운건가 테스트.
				if( CurrentOut.fMeetTime > Out.fMeetTime)	
				{
					// 만난 평면과 진행 방향이 평행하지 않나.?
					float	fMeetPlaneDotVelocity	=	DotProduct( &(Out.MeetPlane.v3Up), &(TestingEllipsoid.Velocity));
					if( fMeetPlaneDotVelocity < 0)
					{
						CurrentOut	=	Out;					// 이놈이 정답.
						dwLastMeetTriIndex		=	pFoundTriIndex[i];		// 이 삼각형의 인덱스.
						bFuncResult				=	TRUE;					// 일단 한번이라도 만났으니.
					}
					else
					{
						_asm nop;
					}
				}
			}
lb_PassTest:
			_asm nop;

		}
		

		// 가장 가까운 t? 속도 정리.
		if( CurrentOut.fMeetTime != 1.1f)
		{
			if( CurrentOut.dwComponentType == 10)
				_asm nop;
			// bLanding?
			CurrentOut.MeetPlane.v3Up;
			VECTOR3		vUp;
			vUp.x	=	0.0f;
			vUp.y	=	1.0f;
			vUp.z	=	0.0f;
			float	fDotLanding	=	DotProduct( &CurrentOut.MeetPlane.v3Up, &vUp);
			if( fDotLanding > 0.6f)
				bLanding		=	TRUE;
//			else
//				bLanding		=	FALSE;

			// 찾아진 평면으로 푸쉬~
			VECTOR3		NewTo;
			BOOL		bPush;
			bPush	= PushEllipsoidFromPLANE( &NewTo, &CurrentOut,  &(CurrentEllipsoid.P), &vCurrentVelocity);
			CheckVectorArray( &NewTo, 1);
			// 새로운 From과 To.

			CurrentEllipsoid.P		=	CurrentOut.vMeetPivot;
			vCurrentVelocity		=	NewTo	-	CurrentOut.vMeetPivot;
			fCurrentVelocity		=	VECTOR3Length( &vCurrentVelocity);

			fRealTime	=	fRealTime	+	((1.0f - fRealTime) * CurrentOut.fMeetTime);	// 시간의 경과 처리.

			if( fRealTime < 0.0f)
					fRealTime	=	0.0f;
			if( fRealTime > 1.0f)
					fRealTime	=	1.0f;
			if( fRealTime != 0.0f)
				_asm nop;

			_asm nop;
		}
		else
		{
			break;
		}

		// 충돌이라고 판정이 나더라도 변한게 없으면 아무것도 아니지 않는가.?
		// 저장해둔것들을 테스트 한다.
		if( PrevEllipsoid.P == CurrentEllipsoid.P)
		{
			if( vPrevVelocity == vCurrentVelocity)
			{
				// 그냥 멈췄다 하자.
				fCurrentVelocity	=	0.0f;
				vCurrentVelocity.x	=	0.0f;
				vCurrentVelocity.y	=	0.0f;
				vCurrentVelocity.z	=	0.0f;
				_asm nop;
			}
			else
			{
				_asm nop;
			}


		}

		
		_asm nop;
	}


//lbFinishCollision:
	// 매 충돌마다 정하던지, 어쨌든 선택 해야...
	// 마찰력 한번 적용.
	if( bLanding)
		VECTOR3_MULEQU_FLOAT( &vCurrentVelocity, 0.9f);//m_fFrictionalForce);
	*pOut	=	CurrentEllipsoid.P + vCurrentVelocity;

	float	fTimeScale;					// 실제 마지막 벡터에 적용되는 시간은.?

	fTimeScale	=	1.0f	-	fRealTime;

	if( fRealTime < 0.0f)
		_asm nop;							// 이경우는 꽤 골치아프지 않나.?
	if( fRealTime == 0.0f)
		_asm nop;							// 그냥 아무것도 안만난 경우.
	if( fRealTime == 1.0f)
		_asm nop;							// 주어진 시간을 꽉 채운 경우겠지.?
	if( fRealTime > 1.0f)
		_asm nop;							// 이건 뭐냐고... -_-;

	VECTOR3		vRealVelocity	=	vCurrentVelocity;			// 실시간은.?
	VECTOR3_MULEQU_FLOAT( &vRealVelocity, fTimeScale);

	*pOutLastVelocity	=	vRealVelocity;

	DWORD	pdwProperty[4];
	if( bFuncResult == TRUE)
	{
		m_Triangle.ppObjBuff[dwLastMeetTriIndex]->GetProperty(pdwProperty);				// 여기.
	}
	// 바닥에 닿았나 테스트.
//	이대로 안되. 만났으면 true 리턴하게...
	*pbOutLanding	=	bLanding;



	return	bFuncResult;
}

DWORD*	CCollisionMeshObjectTree::GetResultIndex(DWORD dwOutputBufferIndex)
{
#ifdef	_DEBUG
	DBG_CheckOutputBufferIndex( dwOutputBufferIndex);
#endif
	return	m_pOutputBuffer[dwOutputBufferIndex].pdwOutputIndexBuffer;
}


void CCollisionMeshObjectTree::CreateFindBuffer(DWORD dwCount)
{
//	m_dwMemoryUsage	+=	sizeof(DWORD)*m_Triangle.dwCount;

	if( dwCount == 0 )
	{
		PrintfDebugString( "CCollisionMeshObjectTree::CreateFindBuffer  인자가 0이다.\n");
		_asm int 3;
		dwCount = 1;
	}

	
	m_dwOutputBufferCount		=	dwCount;
	m_pOutputBuffer				=	new	OUTPUT_INDEX_BUFFER[m_dwOutputBufferCount];
	memset( m_pOutputBuffer, 0, sizeof(OUTPUT_INDEX_BUFFER)*m_dwOutputBufferCount);
	DWORD		i;
	for( i = 0; i < m_dwOutputBufferCount; i++)
	{
		m_pOutputBuffer[i].dwOutputIndexCount		=	0;
		m_pOutputBuffer[i].pdwOutputIndexBuffer		=	new	DWORD[m_Triangle.dwCount];
		memset( m_pOutputBuffer[i].pdwOutputIndexBuffer, 0, sizeof(DWORD)*m_Triangle.dwCount);
		m_pOutputBuffer[i].pOutputLink				=	new	OUTPUT_LINK[m_Triangle.dwCount+2];			// +2은 더미 링크를 위해.
		ClearOutputLink(i);
	}

	// 메모리 사용량.
	m_dwMemoryUsage	+=	sizeof(OUTPUT_INDEX_BUFFER)*m_dwOutputBufferCount;

}

void CCollisionMeshObjectTree::DeleteFindBuffer()
{
	if( m_pOutputBuffer)
	{
		DWORD	i;
		for( i = 0; i < m_dwOutputBufferCount; i++)
		{
			if(m_pOutputBuffer[i].pdwOutputIndexBuffer)
			{
				delete[]	m_pOutputBuffer[i].pdwOutputIndexBuffer;
				m_pOutputBuffer[i].pdwOutputIndexBuffer		=	0;
				m_pOutputBuffer[i].dwOutputIndexCount		=	0;
			}
			if( m_pOutputBuffer[i].pOutputLink)
			{
				delete[]	m_pOutputBuffer[i].pOutputLink;
				m_pOutputBuffer[i].pOutputLink				=	0;
				m_pOutputBuffer[i].dwOutputLinkCurrent		=	0;
			}
		}

		delete[]	m_pOutputBuffer;
		m_pOutputBuffer				=	0;
	}
}

void CCollisionMeshObjectTree::SetSimultaneousAccessCount(DWORD dwCount)
{
	if( dwCount == 0)
	{
		PrintfDebugString( "CCollisionMeshObjectTree::SetSimultaneousAccessCount	dwCount == 0 ?\n");
		_asm int 3;
		return;
	}

	if( m_dwOutputBufferCount != dwCount)
	{
		// 깨고 새로 만들어야지.
		DeleteFindBuffer();
//		m_dwOutputBufferCount		=	dwCount;
		CreateFindBuffer(dwCount);
	}
	else
	{
		_asm nop
	}
}

#ifdef	_DEBUG
void CCollisionMeshObjectTree::DBG_CheckOutputBufferIndex( DWORD dwBufferIndex)
{
	if( dwBufferIndex >= m_dwOutputBufferCount)
	{
		PrintfDebugString( "CCollisionMeshObjectTree::DBG_CheckOutputBufferIndex 주어진 버퍼 인덱스 %d 는 원래 버퍼의 갯수 %d를 넘었다.\n", dwBufferIndex, m_dwOutputBufferCount);
		_asm int 3;
	}
}
#endif
