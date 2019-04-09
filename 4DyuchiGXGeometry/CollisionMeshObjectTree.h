// CollisionMeshObjectTree.h: interface for the CCollisionMeshObjectTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLLISIONMESHOBJECTTREE_H__E4BF3706_8152_4D5D_AB80_33BD7659275F__INCLUDED_)
#define AFX_COLLISIONMESHOBJECTTREE_H__E4BF3706_8152_4D5D_AB80_33BD7659275F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DYUCHIGRX_COMMON/typedef.h"

class	CCollisionMeshObject;

// 여기서 다룰 삼각형들.
struct	M_TRIANGLE_BUFFER
{
	DWORD						dwCount;			// 삼각형의 갯수.
	VECTOR3**					ppBuff;				// 삼각형의 포인터. 이 버퍼는 본체가 아니다. 참조할 땐 ppBuff[0]+n 식으로..
	DWORD*						pdwPlaneIndex;		// 삼각형의 평면 인덱스.
	CCollisionMeshObject**		ppObjBuff;			// 해당 삼각형의 오브젝트 포인터.
	MAABB*						pBox;				// 삼각형의 바운딩 박스.
};

// 위 삼각형의 평면 버퍼.
struct	M_PLANE_BUFFER
{
	DWORD			dwCount;			// 전체 평면의 갯수.
	PLANE*			pPlane;				// 평면 버퍼. 본체다.
};

// 아래 그리드에서 쓰일 임시 링크리스트.
// 자세한건 CreateGrid()에서 참고.
struct	M_TRIANGLE_LINK
{
	M_TRIANGLE_LINK*	pNext;
	DWORD				dwIndex;
};


// 하나의 그리드.
struct	M_GRID_SECTOR
{
	MAABB					BB;				// Bounding Box;
	DWORD					dwTriCount;		// 당근 삼각형의 갯수.
	union
	{
		DWORD				dwTriStartIndex;		// 통합버퍼(?)에서 삼각형의 시작 인덱스. 전체 그리드에서 new를 한번만 하기 위한 꽁수다.
		M_TRIANGLE_LINK*	pLink;					// 빌드 타임때 쓰일넘, 자세한건 크리에이트그리드에서...
	};
	//	BOOL					bCheck;
};

// 그리드들.
struct	M_GRID
{
	MAABB		WorldBox;
	
	DWORD		dwGridXCount;
	float		fGridXStep;
	
	DWORD		dwGridYCount;
	float		fGridYStep;
	
	DWORD		dwGridZCount;
	float		fGridZStep;
	
	M_GRID_SECTOR*			pGrid;				// 접근할 때는, x + (y * dwGridXCount) + (z * dwGridXCount * dwGridYCount);
	M_GRID_SECTOR*			GetSector( DWORD x, DWORD y, DWORD z)
	{
#ifdef	_DEBUG
		if( x >= this->dwGridXCount || y >= this->dwGridYCount || z >= this->dwGridZCount)
			_asm int 3;			// 범위를 벗어났다.
#endif
		return	&(pGrid[x + (y*dwGridXCount) + (z*dwGridXCount*dwGridYCount)]);
	}
	DWORD*		pTriIndexBuffer;				// 그리드 섹터가 가지고 있는 삼각형 인덱스의 배열 통합 버퍼.
	DWORD		dwTriIndexCount;				// 통합 배열의 갯수.
};

// 아웃풋 버퍼에 뿌리기 위한 링크.
struct	OUTPUT_LINK
{
	DWORD			dwPrev;
	DWORD			dwNext;

	DWORD			iIndex;
};

struct	OUTPUT_INDEX_BUFFER
{
	DWORD*						pdwOutputIndexBuffer;
	DWORD						dwOutputIndexCount;				// 버퍼에 씌여진것들 갯수.
	OUTPUT_LINK*				pOutputLink;
	DWORD						dwOutputLinkCurrent;				// 현재 링크의 사용량.
};

class CCollisionMeshObjectTree  
{
public:
	void SetSimultaneousAccessCount( DWORD dwCount);
	BOOL		CollisionTestAAELLIPSOID( VECTOR3* pOut, VECTOR3* pOutLastVelocity, BOOL* pbOutLanding, AAELLIPSOID* pEllipsoid, VECTOR3* pTo, DWORD dwOutputBufferIndex);
	
	BOOL		CreateTree( CCollisionMeshObject* pObjBuff, DWORD dwObjCount);
	
	BOOL		CollisionTestLine( float* pOutT, VECTOR3* pFrom, VECTOR3* pTo, DWORD dwOutputBufferIndex);
	
	DWORD		FindTriangleWithBC(BOUNDING_CAPSULE *pBC, BOOL bFaceFromVertex, DWORD dwOutputBufferIndex);
	// bFaceFromVertex는 pBC의 from이다. from이 삼각형의 앞에 있는걸로 판정된놈만 리턴.
	
	DWORD*		GetResultIndex(DWORD dwOutputBufferIndex);
	
	void		DeleteTree();
	
	CCollisionMeshObjectTree();
	virtual ~CCollisionMeshObjectTree();
	
protected:
	M_TRIANGLE_BUFFER			m_Triangle;
	M_PLANE_BUFFER				m_Plane;
	M_GRID						m_Grid;

	OUTPUT_INDEX_BUFFER*		m_pOutputBuffer;
	DWORD						m_dwOutputBufferCount;
//	DWORD*						m_pdwOutputIndexBuffer;
//	DWORD						m_dwOutputIndexCount;				// 버퍼에 씌여진것들 갯수.
//	OUTPUT_LINK*				m_pOutputLink;
//	DWORD						m_dwOutputLinkCurrent;				// 현재 링크의 사용량.
	
	// 디버그용인가.?
	DWORD						m_dwMemoryUsage;			// byte
	
	void		CreateGrid();
	void		DeleteGrid();

	void		DeleteFindBuffer(void);
	void		CreateFindBuffer( DWORD dwCount);

	// 찾기류의 함수 시작전에 출력링크 초기화.
	void		ClearOutputLink( DWORD dwIndex);
	// 출력 링크에 인덱스 애드.
	void		AddOutputLink( int iIndex, DWORD dwBufferIndex);

	BOOL		TestPlaneWithGrid( M_GRID_SECTOR* pGrid, PLANE* pPlane);

#ifdef	_DEBUG
	void		DBG_CheckOutputBufferIndex(DWORD dwIndex);
#endif
};

#endif // !defined(AFX_COLLISIONMESHOBJECTTREE_H__E4BF3706_8152_4D5D_AB80_33BD7659275F__INCLUDED_)
