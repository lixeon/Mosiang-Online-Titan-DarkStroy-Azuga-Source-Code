// MStaticObjectTree.cpp: implementation of the MStaticObjectTree class.
//
//////////////////////////////////////////////////////////////////////

#include "MStaticObjectTree.h"
#include	"myself97Common.h"
#include	"../4DyuchiGXGFunc/global.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MStaticObjectTree::MStaticObjectTree()
{
	mpTri			=	0;
	mpVertex		=	0;
	muiTriCount		=	0;
	muiDensity		=	0;
	muiGridCount	=	0;

	muiTemp			=	0;
	memset( &mBox, 0, sizeof(MAABB));
}

MStaticObjectTree::~MStaticObjectTree()
{
	DeleteAll();
}

// 초기화 하고,
BOOL MStaticObjectTree::Init(UINT uiTriCount)
{
	if( muiTriCount != 0)
	{
		DeleteAll();
	}
	if( uiTriCount == 0)
	{
		return	TRUE;
	}

	muiTriCount		=	uiTriCount;
	mpTri			=	new	STriangle[muiTriCount];
	mpVertex		=	new	VECTOR3[muiTriCount*3];

	mpCheck			=	new	BYTE[muiTriCount];

	muiTemp			=	0;

	return	TRUE;
}

// 삼각형들 푸쉬 푸쉬.
DWORD MStaticObjectTree::AddTriangle( VECTOR3* IN pTri)
{
	if( muiTemp >= muiTriCount)
	{
		_asm int 3;
		return	0;
	}
	if( muiTriCount == 0)
	{
		_asm int 3;
		return	0;
	}
//	memcpy( mpTri[muiTemp].pVertex, pTri, sizeof(VECTOR3)*3);
	memcpy( &(mpVertex[muiTemp*3]), pTri, sizeof(VECTOR3)*3);

//		BOUNDING_SPHERE		BS;
//		PLANE				Plane;
	GetTriMiddle(&(mpTri[muiTemp].BS.v3Point), pTri);
	mpTri[muiTemp].BS.fRs	=	CalcDistance(pTri, &(mpTri[muiTemp].BS.v3Point));

	CalcPlaneEquation(&(mpTri[muiTemp].Plane), pTri);
//	MakePLANE( &(mpTri[muiTemp].Plane), pTri, &(pTri[1]), &(pTri[2]));
	MAABB&	Box	=	mpTri[muiTemp].Box;
	Box.Max	=	Box.Min	=	mpTri[muiTemp].BS.v3Point;

	if( muiTemp == 9)
		_asm nop;

	int		i;
	for( i = 0; i < 3; i++)
	{
		// 삼각형 aabb
		if( pTri[i].x < Box.Min.x )		Box.Min.x	=	pTri[i].x;
		if( pTri[i].y < Box.Min.y )		Box.Min.y	=	pTri[i].y;
		if( pTri[i].z < Box.Min.z )		Box.Min.z	=	pTri[i].z;

		if( pTri[i].x > Box.Max.x )		Box.Max.x	=	pTri[i].x;
		if( pTri[i].y > Box.Max.y )		Box.Max.y	=	pTri[i].y;
		if( pTri[i].z > Box.Max.z )		Box.Max.z	=	pTri[i].z;

		// 월드.
		if( pTri[i].x < mBox.Min.x )	mBox.Min.x	=	pTri[i].x;
		if( pTri[i].y < mBox.Min.y )	mBox.Min.y	=	pTri[i].y;
		if( pTri[i].z < mBox.Min.z )	mBox.Min.z	=	pTri[i].z;

		if( pTri[i].x > mBox.Max.x )	mBox.Max.x	=	pTri[i].x;
		if( pTri[i].y > mBox.Max.y )	mBox.Max.y	=	pTri[i].y;
		if( pTri[i].z > mBox.Max.z )	mBox.Max.z	=	pTri[i].z;
	}

	muiTemp++;
	return	muiTemp-1;
}

// 트리 만든다.
// dwDensity 는 한 그리드 안에 들어갈 삼각형 갯수.
BOOL MStaticObjectTree::BuildTree(UINT dwDensity)
{
	VECTOR3		vTemp;
	if( muiTriCount == 0)
		return	FALSE;
	muiDensity	=	dwDensity;

	// 바운딩 박스 검증. 원래는 어떻게.?
	if( mBox.Max.x == mBox.Min.x)	mBox.Max.x = mBox.Min.x + 10.0f;
	if( mBox.Max.y == mBox.Min.y)	mBox.Max.y = mBox.Min.y + 10.0f;
	if( mBox.Max.z == mBox.Min.z)	mBox.Max.z = mBox.Min.z + 10.0f;

	// 바운딩 스피어.
	mBS	=	GetSphereFromAABB( mBox);


	muiGridCount	=	muiTriCount / dwDensity;

	if( muiGridCount == 0 && muiTriCount != 0)
		muiGridCount	=	1;
		
	// 승수 계산
	UINT	i;
	for( i = 0; i*i*i <= muiGridCount; i++)	;
	muiGridCount	=	i+1;			// mdwGridCount는 한 축당 그리드의 갯수.
	if( muiGridCount > 11)
		_asm int 3;		// 너무 크지 않은가...

//	muiGridCount	=	2;

	// 그리드 하나의 컴포넌트 크기.
	mfGridXStep		=	(mBox.Max.x-mBox.Min.x) /	muiGridCount;
	mfGridYStep		=	(mBox.Max.y-mBox.Min.y)	/	muiGridCount;
	mfGridZStep		=	(mBox.Max.z-mBox.Min.z)	/	muiGridCount;

	// 그리드 생성.
	mpGrid	=	new	SGrid[muiGridCount*muiGridCount*muiGridCount];
	memset( mpGrid, 0, sizeof(SGrid)*muiGridCount*muiGridCount*muiGridCount);

	// 그리드 돌아가면서 삼각형 채워넣고.
	// for 문 돌아가는 꼬라지땜에 -x, -y, -z방향이 인덱스 제일 앞에 올 듯.
	UINT	x, y, z;
	for( z = 0; z < muiGridCount; z++)			// z
	{
		for( y = 0; y < muiGridCount; y++)			// y
		{
			for( x = 0; x < muiGridCount; x++)			// x
			{
				int		iIndex	=	x	+	y*muiGridCount	+	z*muiGridCount*muiGridCount;

				// minbox
				mpGrid[iIndex].Box.Min.x	=	mBox.Min.x	+	x*mfGridXStep;
				mpGrid[iIndex].Box.Min.y	=	mBox.Min.y	+	y*mfGridYStep;
				mpGrid[iIndex].Box.Min.z	=	mBox.Min.z	+	z*mfGridZStep;
				// maxbox
				mpGrid[iIndex].Box.Max.x	=	mBox.Min.x	+	(x+1)*mfGridXStep;
				mpGrid[iIndex].Box.Max.y	=	mBox.Min.y	+	(y+1)*mfGridYStep;
				mpGrid[iIndex].Box.Max.z	=	mBox.Min.z	+	(z+1)*mfGridZStep;

				// bs
				VECTOR3_ADD_VECTOR3( &(mpGrid[iIndex].BS.v3Point), &(mpGrid[iIndex].Box.Min), &(mpGrid[iIndex].Box.Max));
				VECTOR3_DIVEQU_FLOAT( &(mpGrid[iIndex].BS.v3Point), 2.0f);
				VECTOR3_SUB_VECTOR3( &vTemp, &(mpGrid[iIndex].BS.v3Point), &(mpGrid[iIndex].Box.Min));
				mpGrid[iIndex].BS.fRs	=	VECTOR3Length( &vTemp);

				// 삼각형을 돌아가면서 자기한테 해당되는지 판별, add.
				mpGrid[iIndex].puiTri	=	new	UINT[muiTriCount];		// 일단 버퍼 최대크기 하고,
				memset( mpGrid[iIndex].puiTri, 0, sizeof(UINT)*muiTriCount);
				mpGrid[iIndex].uiTriCount	=	0;
				for( i = 0; i < muiTriCount; i++)
				{
					MAABB&	TriBox	=	mpTri[i].Box;
					bool	bMeet	=	IsAABBMeetAABB( TriBox, mpGrid[iIndex].Box);
					if( true == bMeet)
					{
						mpGrid[iIndex].puiTri[mpGrid[iIndex].uiTriCount]	=	i;
						mpGrid[iIndex].uiTriCount++;
					}
				}
				// 그리드의 삼각형 버퍼 정리.
				UINT*	puiTemp	=	new	UINT[mpGrid[iIndex].uiTriCount];
				memcpy( puiTemp, mpGrid[iIndex].puiTri, sizeof(UINT)*mpGrid[iIndex].uiTriCount);
				delete[]	mpGrid[iIndex].puiTri;
				mpGrid[iIndex].puiTri	=	puiTemp;

			}
		}
	}

	// 삼각형 루프를 돌면서 자기가 해당되는 그리드 내에 들어간다.
	


	return	FALSE;
}

void MStaticObjectTree::DeleteAll(void)
{
	if( muiGridCount)
	{
		UINT	i;
		for( i = 0; i < muiGridCount*muiGridCount*muiGridCount; i++)
		{
			delete[]	mpGrid[i].puiTri;
		}

		delete[]	mpVertex;
		mpVertex	=	0;

		delete[]		mpGrid;
		mpGrid			=	0;
		muiGridCount	=	0;

		delete[]	mpCheck;
		mpCheck			=	0;
	}
	mfGridXStep		=	0;		// x간격,
	mfGridYStep		=	0;		// y간격.
	mfGridZStep		=	0;		// z간격.
	muiDensity		=	0;

	if( muiTriCount)
	{
		muiTriCount	=	0;

		delete[]	mpTri;
		mpTri		=	0;
	}

	muiTemp		=	0;

	memset( &mBox, 0, sizeof(mBox));

}

DWORD MStaticObjectTree::FindTriWithSphere( DWORD* pBuff, DWORD dwBuffSize, BOUNDING_SPHERE* pBS, BOOL bLookAtPivotOnly)
{
	// 그리드가 완성되어있나.?
	if( muiTriCount == 0)
		return	0;

	bool	bResult;
	// 일단 이 스테이틱 오브젝트의 바운딩 스피어에 닫긴 닫나?
	bResult	=	IsSphereMeetSphere( *pBS, mBS);
	if( bResult == false)
		goto	lbReturnZero;
		
	// 일단 이 스테이틱 오브젝트의 바운딩박스에 닫긴 닫나.?
	bResult	=	IsSphereMeetAABB( *pBS, mBox);
	if( bResult == false)
		goto	lbReturnZero;

	// 이까지 오면 대충 그리드에 걸친다고 판별.
	int		iTemp;	

	// 해당되는 그리드를 찾은다음,
	// 계산식 어떻게?
	UINT	x1, x2, y1, y2, z1, z2;
	// x시작값.
	iTemp	=	(int)((pBS->v3Point.x - pBS->fRs - mBox.Min.x)/mfGridXStep);
	if( iTemp < 0)
		x1	=	0;
	else
		x1	=	(UINT)iTemp;
	// x끝값.
//	iTemp	=	(int)((pBS->v3Point.x + pBS->fRs - mBox.Min.x)/mfGridXStep+1);
	iTemp	=	(int)((pBS->v3Point.x + pBS->fRs - mBox.Min.x)/mfGridXStep);
	if( iTemp >= this->muiGridCount)
		x2	=	muiGridCount-1;
	else
		x2	=	(UINT)iTemp;

	// y
	iTemp	=	(int)((pBS->v3Point.y - pBS->fRs - mBox.Min.y)/mfGridYStep);
	if( iTemp < 0)
		y1	=	0;
	else
		y1	=	(UINT)iTemp;
	// y끝값.
//	iTemp	=	(int)((pBS->v3Point.y + pBS->fRs - mBox.Min.y)/mfGridYStep+1);
	iTemp	=	(int)((pBS->v3Point.y + pBS->fRs - mBox.Min.y)/mfGridYStep);
	if( iTemp >= this->muiGridCount)
		y2	=	muiGridCount-1;
	else
		y2	=	(UINT)iTemp;

	// z
	iTemp	=	(int)((pBS->v3Point.z - pBS->fRs - mBox.Min.z)/mfGridZStep);
	if( iTemp < 0)
		z1	=	0;
	else
		z1	=	(UINT)iTemp;
	// z끝값.
//	iTemp	=	(int)((pBS->v3Point.z + pBS->fRs - mBox.Min.z)/mfGridZStep+1);
	iTemp	=	(int)((pBS->v3Point.z + pBS->fRs - mBox.Min.z)/mfGridZStep);
	if( iTemp >= this->muiGridCount)
		z2	=	muiGridCount-1;
	else
		z2	=	(UINT)iTemp;


	memset( mpCheck, 0, sizeof(BYTE)*muiTriCount);
	// 루프 뺑뺑이.
	UINT	i, j, k;

	for( k = z1; k <= z2; k++)
	{
		for( j = y1; j <= y2; j++)
		{
			for( i = x1; i <= x2; i++)
			{
				// 이 그리드가 바운딩 스피어와 만나는가.? 
				SGrid&	grid	=	mpGrid[i + j*muiGridCount + k*muiGridCount*muiGridCount];
				bResult	=	IsSphereMeetAABB( *pBS, grid.Box);
				if( bResult == true)	// 만난다면, 
				{
					// 이 그리드의 삼각형을 몽땅 체크.
					// 이 그리드의 삼각형을 돌아가면서 체크.
					for( iTemp = 0; iTemp < grid.uiTriCount; iTemp++)
					{
//						mpCheck[grid.puiTri[iTemp]]	=	1;
//						bResult	=	IsSphereMeetSphere( *pBS, mpTri[grid.puiTri[iTemp]].BS);
//						if( bResult == true)
						{
							bResult	=	IsSphereMeetAABB( *pBS, mpTri[grid.puiTri[iTemp]].Box);
							if( bResult == true)
							{
								mpCheck[grid.puiTri[iTemp]]	=	1;
							}
						}
					}
				}
			}
		}
	}

	for( i = 0; i < muiTriCount; i++)
	{
		bResult	=	IsSphereMeetSphere( *pBS, mpTri[i].BS);
		if( bResult == true)
		{
			bResult	=	IsSphereMeetAABB( *pBS, mpTri[i].Box);
			if( bResult == true)
			{
				mpCheck[i]	=	1;
			}
		}
	}

	// 체크된놈의 인덱스를 버퍼에 출력.
	j	=	0;
	for( i = 0; i < muiTriCount; i++)
	{
#ifdef	_DEBUG
		if( j > dwBuffSize)
		{
			_asm int 3;			// 주어진 버퍼보다 결과물이 많다.
			return	j-1;
		}
#endif
		if( mpCheck[i])
		{
			// bLookAtPivotOnly 옵션 체크.
			if( bLookAtPivotOnly)
			{
				float	fDot	=	DotProduct(&(mpTri[i].Plane.v3Up), &(pBS->v3Point))	+	mpTri[i].Plane.D;
				if( fDot < -0.00001f)
					continue;				// 만약 피봇 뒤에 있으면 패스다.
			}

			{
				pBuff[j]	=	i;
				j++;
			}
		}
	}

	return	j;	// 갯수 리턴.


lbReturnZero:

	return	0;
}


DWORD MStaticObjectTree::FindTriWithBOUNDING_CAPSULE( DWORD* pIndexBuff, DWORD dwBuffSize, BOUNDING_CAPSULE* pBC, BOOL bLookAtPivotOnly)
{
	// 임시 구현 임시 구현 임시 구현.
	BOUNDING_SPHERE		BS, *pBS;
	BS.v3Point	=	pBC->v3From + pBC->v3To;
	VECTOR3_DIVEQU_FLOAT( &BS.v3Point, 2.0f);
	BS.fRs	=	CalcDistance( &BS.v3Point, &pBC->v3From)	+	pBC->fRadius;
	pBS	=	&BS;

	// 그리드가 완성되어있나.?
	if( muiTriCount == 0)
		return	0;

	bool	bResult;
	// 일단 이 스테이틱 오브젝트의 바운딩 스피어에 닫긴 닫나?
	bResult	=	IsSphereMeetSphere( *pBS, mBS);
	if( bResult == false)
		goto	lbReturnZero;
		
	// 일단 이 스테이틱 오브젝트의 바운딩박스에 닫긴 닫나.?
	bResult	=	IsSphereMeetAABB( *pBS, mBox);
	if( bResult == false)
		goto	lbReturnZero;

	// 이까지 오면 대충 그리드에 걸친다고 판별.
	int		iTemp;	

	// 해당되는 그리드를 찾은다음,
	// 계산식 어떻게?
	UINT	x1, x2, y1, y2, z1, z2;
	// x시작값.
	iTemp	=	(int)((pBS->v3Point.x - pBS->fRs - mBox.Min.x)/mfGridXStep);
	if( iTemp < 0)
		x1	=	0;
	else
		x1	=	(UINT)iTemp;
	// x끝값.
//	iTemp	=	(int)((pBS->v3Point.x + pBS->fRs - mBox.Min.x)/mfGridXStep+1);
	iTemp	=	(int)((pBS->v3Point.x + pBS->fRs - mBox.Min.x)/mfGridXStep);
	if( iTemp >= this->muiGridCount)
		x2	=	muiGridCount-1;
	else
		x2	=	(UINT)iTemp;

	// y
	iTemp	=	(int)((pBS->v3Point.y - pBS->fRs - mBox.Min.y)/mfGridYStep);
	if( iTemp < 0)
		y1	=	0;
	else
		y1	=	(UINT)iTemp;
	// y끝값.
//	iTemp	=	(int)((pBS->v3Point.y + pBS->fRs - mBox.Min.y)/mfGridYStep+1);
	iTemp	=	(int)((pBS->v3Point.y + pBS->fRs - mBox.Min.y)/mfGridYStep);
	if( iTemp >= this->muiGridCount)
		y2	=	muiGridCount-1;
	else
		y2	=	(UINT)iTemp;

	// z
	iTemp	=	(int)((pBS->v3Point.z - pBS->fRs - mBox.Min.z)/mfGridZStep);
	if( iTemp < 0)
		z1	=	0;
	else
		z1	=	(UINT)iTemp;
	// z끝값.
//	iTemp	=	(int)((pBS->v3Point.z + pBS->fRs - mBox.Min.z)/mfGridZStep+1);
	iTemp	=	(int)((pBS->v3Point.z + pBS->fRs - mBox.Min.z)/mfGridZStep);
	if( iTemp >= this->muiGridCount)
		z2	=	muiGridCount-1;
	else
		z2	=	(UINT)iTemp;


	memset( mpCheck, 0, sizeof(BYTE)*muiTriCount);
	// 루프 뺑뺑이.
	UINT	i, j, k;

	for( k = z1; k <= z2; k++)
	{
		for( j = y1; j <= y2; j++)
		{
			for( i = x1; i <= x2; i++)
			{
				// 이 그리드가 바운딩 스피어와 만나는가.? 
				SGrid&	grid	=	mpGrid[i + j*muiGridCount + k*muiGridCount*muiGridCount];
				bResult	=	IsSphereMeetAABB( *pBS, grid.Box);
				if( bResult == true)	// 만난다면, 
				{
					// 이 그리드의 삼각형을 몽땅 체크.
					// 이 그리드의 삼각형을 돌아가면서 체크.
					for( iTemp = 0; iTemp < grid.uiTriCount; iTemp++)
					{
//						mpCheck[grid.puiTri[iTemp]]	=	1;
//						bResult	=	IsSphereMeetSphere( *pBS, mpTri[grid.puiTri[iTemp]].BS);
//						if( bResult == true)
						{
							bResult	=	IsSphereMeetAABB( *pBS, mpTri[grid.puiTri[iTemp]].Box);
							if( bResult == true)
							{
								mpCheck[grid.puiTri[iTemp]]	=	1;
							}
						}
					}
				}
			}
		}
	}

	for( i = 0; i < muiTriCount; i++)
	{
		bResult	=	IsSphereMeetSphere( *pBS, mpTri[i].BS);
		if( bResult == true)
		{
			bResult	=	IsSphereMeetAABB( *pBS, mpTri[i].Box);
			if( bResult == true)
			{
				mpCheck[i]	=	1;
			}
		}
	}

	// 체크된놈의 인덱스를 버퍼에 출력.
	j	=	0;
	for( i = 0; i < muiTriCount; i++)
	{
#ifdef	_DEBUG
		if( j > dwBuffSize)
		{
			_asm int 3;			// 주어진 버퍼보다 결과물이 많다.
			return	j-1;
		}
#endif
//		if( mpCheck[i])
		{
			// bLookAtPivotOnly 옵션 체크.
			if( bLookAtPivotOnly)
			{
				float	fDot	=	DotProduct(&(mpTri[i].Plane.v3Up), &(pBS->v3Point))	+	mpTri[i].Plane.D;
				if( fDot < -0.00001f)
					continue;				// 만약 피봇 뒤에 있으면 패스다.
			}

			{
				pIndexBuff[j]	=	i;
				j++;
			}
		}
	}

	return	j;	// 갯수 리턴.


lbReturnZero:

	return	0;
}

