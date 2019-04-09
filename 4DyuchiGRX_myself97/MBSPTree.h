/*
	충돌 처리를 위한 leaf based bsp tree.
	find 계열은 특정 지역 내에 들어오는 leaf를 찾는 방식으로 작동한다.
*/
#if !defined(AFX_MBSPTREE_H__9041AE2E_08D7_4BC4_B325_217816FAAF3D__INCLUDED_)
#define AFX_MBSPTREE_H__9041AE2E_08D7_4BC4_B325_217816FAAF3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4Dyuchigrx_common/typedef.h"


#define		iBSPVer		1

struct	MBSPTreeFileHeader
{
	// 헤더.
//	char	pHeader[4];
	// 버전.
	int		iVer;
	// 삼각형 갯수.
	int		iTriCount;
	// 평면 갯수.
	int		iPlaneCount;
	// 노드 정보
	int		iNodeCount;
	int		iNormalNodeCount;
	int		iLeafCount;	// 리프 갯수.
	int		iNullLeafCount;	// 삼각형을 아무것도 가지지 않은 리프 카운트.
	int		iMaxLevel;

};

struct	BSPTreeNodeRuntime
{
	DWORD*			pTriIndex;		//	이 노드가 가진 삼각형의 인덱스.
	DWORD*			pPlaneIndex;	//	이 노드가 가진 삼각형의 평면의 인덱스. -_-;
	DWORD			dwPlaneCount;	//	이 노드가 가진 삼각형의 평면의 갯수.
	DWORD*			pChecked;		//	이 노드가 가진 평면이 사용되었었나.?
	DWORD			dwCheckedPlaneCount;	//	사용된 평면 갯수.
	float			fWhere;			// 트리 재귀할 때 헬프용...

};

struct	BSPTreeNode
{
	int				iFront;
	int				iBack;
	int				iParent;

	BOOL			bIsLeaf;		// 이놈이 리프인가.?
	
	int				iDivPlane;		//	이 노드가 나누는 평면.

	int				iTriCount;		//	이 노드가 가진 삼각형의 갯수.
	int				iTriStartIndex;


	int				iLevel;		// 이 노드의 층.


	// 삼각형들, 트리를 만드는 과정이나 노드가 리프인 경우 외엔 필요가 없다. 
	BSPTreeNodeRuntime*		r;
};


class MBSPTree  
{
public:
	bool	Save( HANDLE hFile);			// 외부에서 준 핸들, 그냥 쭉 붙혀서 세이브.
	bool	Load( HANDLE hFile);

	BOOL	Init(void);		// 초기화 하고,
	DWORD	AddTriangle( VECTOR3* IN pTri);		// 삼각형들 푸쉬 푸쉬.
	BOOL	BuildTree(void);	// 트리 만든다.

	// pBuff로 리턴되는 값이 인덱스 값이고, 삼각형은 GetTriBuffer를 통해 얻는다. pTri가 수정되면 곤란...
	DWORD	FindNearTri( DWORD* pBuff, DWORD dwBuffSize, VECTOR3* pSPivot, float fSRadius);	// pSPivot과 fSRadius로 주어진 영역에 들어오는 삼각형을 찾는다.
	VECTOR3*	GetTriBuffer(void){	return this->mpTri;}
	DWORD	GetTriCount(){return miTriCount;}

	void DeleteAll(void);

	MBSPTree();
	~MBSPTree();

private:
	// Save;
	VECTOR3*				mpTri;				// 세개씩.
	BOUNDING_SPHERE*		mpTriSphere;		// 삼각형의 바운딩 스피어.
	int						miTriCount;			// 전체 삼각형의 갯수.
	DWORD*					mpPlaneIndex;		// 이 삼각형의 평면 인덱스.

	PLANE*					mpPlane;				// 평면, 갯수는 삼각형과 같거나 삼각형갯수보다 작겠지.
	int						miPlaneCount;

	// bsp 노드들.
	BSPTreeNode*			mpNode;
	int						miNodeCount;

	int						miNormalNodeCount;
	int						miLeafCount;	// 리프 갯수.
	int						miNullLeafCount;	// 삼각형을 아무것도 가지지 않은 리프 카운트.
	int						miMaxLevel;
	
	// 노드가 가지는 삼각형들 버퍼.
	int*					mpNodeTriIndexBuff;
	int						miNodeTriIndexBuffUsedCount;

	// do not Save
	int						miNodeTemp;

	BYTE*					mpTempOutput;	// 출력용 인덱스 버퍼.

	VECTOR3*				mpPreTriangle;		// AddTriangle때 사용될 임시 버퍼.
	DWORD					mdwPreTriangleCount;		// 갯수...

	DWORD*					pOutputBuffer;


	static	int				siCount;


	bool					mbBuilt;


	int		CreateNodeReculsive( int iParent, BOOL bIsBack, DWORD* pTriIndex, DWORD dwTriCount, DWORD* pCheckedPlane, int iLevel);// 재귀적으로 트리 만들기.
	int		WhereIsTriangleFromPlane( VECTOR3* pTri, PLANE* pPlane);	// 주어진 삼각형이 평면의 어디에 있는가.?

	int		FindDivideCandidate( DWORD* pTriIndex, DWORD dwTriCount, DWORD*	pCheckedPlane);	// 주어진 삼각형중, 노드 쓰일 가장 좋은 것을 골라낸다.
	void	FindNearTriReculsive( int iNode);	// FindNearObject의 내부 호출용.	
	VECTOR3*	pSPivot;
	float		fSRadius;
	
	DWORD					mdwDBG_NodeTravelCount;			// 트리를 뒤질때 노드를 돌아다닌 횟수.

	BOOL	IsTooFarBetweenVertexAndTri( float fMaxDist, DWORD dwTriIndex, VECTOR3* pVertex);		// 너무 멀면 TRUE, 거리 안에 있으면 FALSE

	bool	ReadDataFromFile( HANDLE hFile, void* pData, DWORD dwCountOfByte);
	bool	WriteDataToFile( HANDLE hFile, void* pData, DWORD dwCountOfByte);

};

#endif // !defined(AFX_MBSPTREE_H__9041AE2E_08D7_4BC4_B325_217816FAAF3D__INCLUDED_)
