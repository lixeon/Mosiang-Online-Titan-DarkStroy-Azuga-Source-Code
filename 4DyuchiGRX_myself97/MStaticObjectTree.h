// MStaticObjectTree.h: interface for the MStaticObjectTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSTATICOBJECTTREE_H__0E167021_FE94_4C6F_A9DC_0EE8E0D3BB84__INCLUDED_)
#define AFX_MSTATICOBJECTTREE_H__0E167021_FE94_4C6F_A9DC_0EE8E0D3BB84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4Dyuchigrx_common/typedef.h"

class MStaticObjectTree  
{
public:
	BOOL	Save( HANDLE hFile){return FALSE;}			// 외부에서 준 핸들, 그냥 쭉 붙혀서 세이브.
	BOOL	Load( HANDLE hFile){return FALSE;}

	BOOL	Init(UINT uiTriCount);						// 초기화 하고,
	DWORD	AddTriangle( VECTOR3* IN pTri);				// 삼각형들 푸쉬 푸쉬.
	BOOL	BuildTree(UINT iDensity);							// 트리 만든다.

	// pBuff로 리턴되는 값이 인덱스 값이고, 삼각형은 GetTriBuffer를 통해 얻는다. pTri가 수정되면 곤란...
	// pSPivot과 fSRadius로 주어진 영역에 들어오는 삼각형을 찾는다. bLookAtPivotOnly는 pBS의 중점을 향한놈만 걸러낸다.
	DWORD	FindTriWithSphere( DWORD* pIndexBuff, DWORD dwBuffSize, BOUNDING_SPHERE* pBS, BOOL bLookAtPivotOnly);
	DWORD	FindTriWithBOUNDING_CAPSULE( DWORD* pIndexBuff, DWORD dwBuffSize, BOUNDING_CAPSULE* pBC, BOOL bLookAtPivotOnly);
	VECTOR3*	GetTriBuffer(void){	return	mpVertex;}
//	DWORD	GetTriCount(){return 0;}
	void	GetTriBoundingSphere( BOUNDING_SPHERE* pOutBS, UINT uiTriNum){ *pOutBS = mpTri[uiTriNum].BS;}

	void	DeleteAll(void);

	MStaticObjectTree();
	~MStaticObjectTree();

protected:
	
	struct	STriangle
	{
		BOUNDING_SPHERE		BS;
		MAABB				Box;
		PLANE				Plane;
//		VECTOR3				pVertex[3];

	};

	STriangle*		mpTri;
	VECTOR3*		mpVertex;
	UINT			muiTriCount;

	UINT			muiDensity;				// 대충 하나의 그리드당 들어갈 삼각형 갯수.
	UINT			muiGridCount;			// 한차원당 그리드의 갯수.
	// 대충의 삼각형 갯수는 mdwGridCount^3 * mdwDensity
	float			mfGridXStep;		// x간격,
	float			mfGridYStep;		// y간격.
	float			mfGridZStep;		// z간격.
	struct	SGrid
	{
		BOUNDING_SPHERE		BS;
		MAABB				Box;
		UINT*				puiTri;
		UINT				uiTriCount;
	};
	SGrid*				mpGrid;

	UINT			muiTemp;

	MAABB			mBox;
	BOUNDING_SPHERE		mBS;

	BYTE*			mpCheck;

};

#endif // !defined(AFX_MSTATICOBJECTTREE_H__0E167021_FE94_4C6F_A9DC_0EE8E0D3BB84__INCLUDED_)
