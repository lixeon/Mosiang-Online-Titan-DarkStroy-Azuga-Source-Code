#include "SphereCollisionDetect.h"
#include "../4DyuchiGRX_myself97/myself97common.h"
#include "../4DyuchiGXGFunc/global.h"

// 평면 앞쪽의 만나는점,t 평면 뒷쪽의 만나는점,t, 스피어의 원점, to, 반지름, 실제 평면.
// 리턴값은 안만나면 0, 만나면 1.
BOOL	MovingSphereMeetTriPlane( VECTOR3* OUT pCandidate, float* OUT pT, VECTOR3* pWhereMeet, VECTOR3* IN pSPivot, VECTOR3* IN pSTo, float fSRadius, VECTOR3* pTri);
BOOL	MovingSphereMeetTriEdge	( VECTOR3* OUT pCandidate, float* OUT pT, VECTOR3* pWhereMeet, VECTOR3* IN pSPivot, VECTOR3* IN pSTo, float fSRadius, VECTOR3* IN pTri);
BOOL	MovingSphereMeetLine( VECTOR3* pCandidate, float* pT1, VECTOR3* pWhereMeet, float* pT2, VECTOR3* pSPivot, VECTOR3* pSTo, float fSRadius, VECTOR3* pLFrom, VECTOR3* pLTo);
BOOL	MovingSphereMeetVertex( VECTOR3* pCandidate, float* pT, VECTOR3* pSPivot, VECTOR3* pSTo, float fSRadius, VECTOR3* pVertex);


// 움직이는 스피어가 삼각형의 어디와 만나나.?
//	평면, 엣지, 모서리, 각각에 대해 만나는 거리를 체크, 만나지 않는것은 빼고 만난다고 판정된 것중 가장 가까운것을 기준으로 리턴.
BOOL	WhereDidMovingSphereMeetTri( VECTOR3* OUT pCandidate, float* pT, VECTOR3* pWhereMeet, VECTOR3* pSFrom, VECTOR3* pSTo, float fSRadius, VECTOR3* pTri)
// 만나는 위치, 만날때의 t, 만날때의 슬라이딩 플레인, 원의 중점, 원의 to, 원 반지름, 삼각형.
{
	VECTOR3		vTemp;
	VECTOR3		vTemp2;
	float		fTemp;
	BOOL		bResult, bFound = FALSE;
	VECTOR3		Dir;
	VECTOR3_SUB_VECTOR3( &Dir, pSTo, pSFrom);

	VECTOR3	C1, W;
	float	t1;		// t는 만나는 경우에 한해서 '-'값을 가질 수 있다. 만나는건 호출되는 함수에서 미리 테스트되고, t가 -로 나오는 경우는 만난다는 의미다.
	//	삼각형이 이루는 평면과 만나나.?	평면의 '앞'만 검사.

	{
		// 삼각형의 평면과 만나는 점을 찾는다. 
		bResult		=	MovingSphereMeetTriPlane( &C1, &t1, &W, pSFrom, pSTo, fSRadius, pTri );
		
		if( bResult	== TRUE)
		{
			if( t1 < 1)
			{
				// 만날때의 점들.
				*pCandidate	=	C1;
				*pT			=	t1;
				*pWhereMeet	=	W;
				bFound		=	TRUE;
			}
		}

	}


	//	만나는 점이 삼각형 안에 없으면, 엣지 세개와 만나나.?
	{
		bResult	=	MovingSphereMeetTriEdge( &C1, &t1, &W, pSFrom, pSTo, fSRadius, pTri);
		if( bResult	== TRUE)
		{
			if( (bFound == TRUE && *pT > t1) || bFound == FALSE)
			{
				*pCandidate	=	C1;
				*pT			=	t1;
				*pWhereMeet	=	W;
				bFound		=	TRUE;
			}
		}

	}

	//	만나는 점이 삼각형의 모서리인가.?
	{
		DWORD	i;
		for( i = 0; i < 3; i++)

		{
			bResult	=	MovingSphereMeetVertex( &C1, &t1, pSFrom, pSTo, fSRadius, &(pTri[i]));
			if( bResult == TRUE)
			{
				if( (bFound == TRUE && *pT > t1) || bFound == FALSE)
				{
					*pCandidate	=	C1;
					*pT			=	t1;
					*pWhereMeet	=	pTri[i];
					bFound		=	TRUE;
				}
			}
		}
	}


	if( bFound == FALSE)		// 닫는게 하나도 없다.
	{
		return	FALSE;
	}
	// 슬라이딩 평면은 리턴 받는 입장에서 만들어 쓴다.

	// 실제로 면이 닫더라도, 진행방향 벡터와 상관이 없다면 만나지 않는다.
	// 벡터(만나는점-중점)과 진행방향 벡터의 내적이 0보다 작거나 같으면 무효.

	VECTOR3_SUB_VECTOR3( &vTemp, pSTo, pSFrom);
	VECTOR3_SUB_VECTOR3( &vTemp2, pWhereMeet, pSFrom);
	fTemp	=	DotProduct( &vTemp, &vTemp2);

//	if( (*pWhereMeet-*pSFrom)*(*pSTo-*pSFrom) <= 0)
	if( fTemp <= 0)
		return FALSE;

//	*pCandidate	=	*pSFrom + (*pSTo - *pSFrom) * *pT;
	VECTOR3_SUB_VECTOR3( &vTemp, pSTo, pSFrom);
	VECTOR3_MUL_FLOAT( &vTemp, &vTemp, (*pT));
	VECTOR3_ADD_VECTOR3( pCandidate, pSFrom, &vTemp);

	return TRUE;
}


// 삼각형중 평면과 건들리나.? 엣지나 뭐나 그런건 상관없이, 이동방향에 대해 삼각형이 반대로 보고 있는경우는 충돌처리 안된다..
BOOL	MovingSphereMeetTriPlane
	( VECTOR3* OUT pCandidate1, float* OUT pT1, VECTOR3* pWhereMeet, VECTOR3* IN pSPivot, VECTOR3* IN pSTo, float fSRadius, VECTOR3* pTri)

{
	VECTOR3	vTemp;
	float	fTemp;
	float	t;

	VECTOR3	LDir;
	VECTOR3_SUB_VECTOR3( &LDir, pSTo, pSPivot);
	
	PLANE	Plane;
	MakePLANE(  &Plane, &(pTri[0]), &(pTri[1]), &(pTri[2]));
	
	float	dot	=	DotProduct( &LDir, &(Plane.v3Up));
	if( dot == 0)	return FALSE;		// 평행하다. -_-;

	// 평면과 만날때 스피어의 위치.
//	float	fUnder	=	Plane.v3Up.x*(LDir.x) + Plane.v3Up.y*LDir.y + Plane.v3Up.z*LDir.z;
	float	fUnder	=	DotProduct(&(Plane.v3Up), &LDir);

//	float	fOver	=	-1*( Plane.v3Up.x*pSPivot->x + Plane.v3Up.y*pSPivot->y + Plane.v3Up.z*pSPivot->z + Plane.D  );
	fTemp	=	DotProduct( &(Plane.v3Up), pSPivot);
	float	fOver	=	-1*( fTemp + Plane.D  );
	*pT1	=	(fOver + fSRadius) / fUnder;
//	*pCandidate1	=	*pSPivot + (LDir*(*pT1));
	VECTOR3_MUL_FLOAT( &vTemp, &LDir, *pT1);
	VECTOR3_ADD_VECTOR3( pCandidate1, pSPivot, &vTemp);

	if( *pT1 > 1 )
		return	FALSE;
	if( *pT1 < 0)	// 평면을 뚫고 들어가있는 경우, 무작정 안만난다를 리턴하는게 아니고, 더이상 파고 들어가지 못하게 해야한다.
	// 특별한 요령이 있는건 아니고, 만나면 만나는걸 detect 해서 그렇다고 해주면 되겠지.
	{
		t	=	Plane.v3Up * (*pCandidate1) + Plane.D;
		if( -1 * t > fSRadius)	// 완전히 벗어난 경우다.
			return FALSE;
	}

	// 이때 평면과 만나는 점은.?
	fUnder		=	(Plane.v3Up.x)*(Plane.v3Up.x) + (Plane.v3Up.y)*(Plane.v3Up.y) + (Plane.v3Up.z)*(Plane.v3Up.z);
	fOver		=	(Plane.v3Up.x*(pCandidate1->x)) + (Plane.v3Up.y*(pCandidate1->y)) + (Plane.v3Up.z*(pCandidate1->z)) + Plane.D;
	t	=	fOver / fUnder;
	*pWhereMeet		=	*pCandidate1 - (Plane.v3Up*t);		// 평면위의 점.

	// 점이 삼각형 내에 들어오는가.?	각 엣지의 외적을 구해 평면을 만들고 내부에 들어가는가 테스트.
	VECTOR3	Mid;//		=	GetTriMiddle( &pTri[0], &pTri[1], &pTri[2]);
	GetTriMiddle( &Mid, pTri);
	VECTOR3	OnLine;
	VECTOR3	ToMid;
	PLANE	p;

	// 첫번째 엣지.
//	OnLine	=	FindNearestVertexOnLine( pTri[0], pTri[1], Mid);
	FindNearestVertexOnLine( &OnLine, &fTemp, pTri[0], pTri[1], Mid);
	VECTOR3_SUB_VECTOR3( &ToMid, &Mid, &OnLine);
	Normalize( &(p.v3Up), &ToMid);
	p.D			=	-1 * DotProduct( &(p.v3Up), &OnLine);		// A dot B = -d, 평면방정식.
	// 평면과 만나는 점이 첫번째 엣지 안에 있는가.?
	t	=	(p.v3Up) * (*pWhereMeet);
	if( t <= -1*p.D )
	{
		return FALSE;
	}

	// 두번째 엣지.
//	OnLine	=	FindNearestVertexOnLine( pTri[1], pTri[2], Mid);
	FindNearestVertexOnLine( &OnLine, &fTemp, pTri[1], pTri[2], Mid);
	VECTOR3_SUB_VECTOR3( &ToMid, &Mid, &OnLine);
	Normalize( &(p.v3Up), &ToMid);
	p.D			=	-1 * DotProduct( &(p.v3Up), &OnLine);
	t	=	(p.v3Up) * (*pWhereMeet);
	if( t <= -1*p.D )
	{
		return FALSE;
	}

	// 세번째 엣지.
//	OnLine	=	FindNearestVertexOnLine( pTri[2], pTri[0], Mid);
	FindNearestVertexOnLine( &OnLine, &fTemp, pTri[2], pTri[0], Mid);
	VECTOR3_SUB_VECTOR3( &ToMid, &Mid, &OnLine);
	Normalize( &(p.v3Up), &ToMid);
	p.D			=	-1 * DotProduct( &(p.v3Up), &OnLine);
	t	=	(p.v3Up) * (*pWhereMeet);
	if( t <= -1*p.D )
	{
		return FALSE;
	}

	// 이까지 오면 만난다.
	return	TRUE;

}

// 삼각형의 엣지와 건들리는... 만나는 시점과 만나는 점 출력.
BOOL	MovingSphereMeetTriEdge
	( VECTOR3* OUT pCandidate, float* OUT pT1, VECTOR3* pWhereMeet, VECTOR3* IN pSPivot, VECTOR3* IN pSTo, float fSRadius, VECTOR3* IN pTri)
	// 만나는 스피어의 위치, 시점, 엣지와 만나는 점, 엣지와 만날때의 방향벡터(평면벡터로 쓰면 된다.), 스피어 중점, 스피어to, 반지름, 삼각형 점1, 2, 3
{
	BOOL	bMeet	=	FALSE;
	VECTOR3	C, W;
	float	t1, t2, CandidateT	=	1.0f;
	// 첫번째 엣지.

	if( MovingSphereMeetLine( &C, &t1, &W, &t2, pSPivot, pSTo, fSRadius, &(pTri[0]), &(pTri[1])))
	{
		if( t1 <= 1 && t2 >= 0 && t2 <= 1)	
		{
			bMeet	=	TRUE;
			*pCandidate		=	C;
			CandidateT		=	*pT1	=	t1;
			*pWhereMeet		=	W;
		}
	}

	// 두번째 엣지.
	if( MovingSphereMeetLine( &C, &t1, &W, &t2, pSPivot, pSTo, fSRadius, &(pTri[1]), &(pTri[2])))
	{
		if( t1 <= 1 && t2 >= 0 && t2 <= 1)
		{
			if( bMeet == FALSE || CandidateT < t1)
			{
				bMeet	=	TRUE;
				*pCandidate		=	C;
				*pT1			=	t1;
				*pWhereMeet		=	W;
			}
		}
	}
	// 세번째 엣지.

	if( MovingSphereMeetLine( &C, &t1, &W, &t2, pSPivot, pSTo, fSRadius, &(pTri[2]), &(pTri[0])))
	{
		if( t1 <= 1 && t2 >= 0 && t2 <= 1)
		{
			if( bMeet == FALSE || CandidateT < t1)
			{
				bMeet	=	TRUE;
				*pCandidate		=	C;
				*pT1			=	t1;
				*pWhereMeet		=	W;
			}
		}
	}

	if( bMeet == TRUE)
	{
		return	TRUE;
	}


	return	FALSE;
}

// 선분과 움직이는 스피어가 어디서 만나는가.? 
BOOL	MovingSphereMeetLine( VECTOR3* pCandidate, float* pT1, VECTOR3* pWhereMeet, float* pT2, VECTOR3* pSPivot, VECTOR3* pSTo, float fSRadius, VECTOR3* pLFrom, VECTOR3* pLTo)
{
	VECTOR3		nDir;
	Normalize( &nDir, &(*pSTo - *pSPivot));
	VECTOR3&	A	=	*pLFrom;					// 라인 프럼.
	VECTOR3		B	=	*pLTo	-	*pLFrom;		// 라인 방향.
	VECTOR3&	C	=	*pSPivot;					// 스피어 프럼.
	VECTOR3		D	=	*pSTo	-	*pSPivot;		// 스피어 진행 방향.

	// 평행한가.?
	VECTOR3		tempvec1;
	VECTOR3		tempvec2;
	Normalize(	&tempvec1, &B);
	Normalize(	&tempvec2, &D);
	if( tempvec1 == tempvec2)
	{
//		_asm nop;
		// 유효숫자땜에 놓치는게 많겠군.
		return	FALSE;
	}


	float	dotBB	=	B*B;
	VECTOR3	AC	=	C-A;

//	VECTOR3		n	=	A + B*((B*(C-A))/(B*B)) - C;
	VECTOR3		n	=	A + B*((B*AC)/dotBB) - C;
//	VECTOR3		m	=	( B * ((B*D)/(B*B)) ) - D;
	VECTOR3		m	=	( B * ((B*D)/dotBB) ) - D;

	float	a	=	m * m;
	float	b	=	m * n * 2;
//	float	c	=	(n * n) - ((fSRadius+fBorder)*(fSRadius+fBorder));
	float	c	=	(n * n) - ((fSRadius)*(fSRadius));

	float	t	=	b*b - 4*a*c;
	if( t < -0.0001f)			// t는 어떻게 유효한지 아나.?
	{
		_asm nop;
		// 아무것도 안만난다는 뜻인가.
		return	FALSE;
	}
	else if( t >= -0.0001f && t < 0.0001f)	// 근이 하나인 경우. 유효숫자 문제가 좀 있는데..
	{
		_asm nop;
		*pT1	=	(-1 * b) / (2 * a);
	}
	else if( t >= 0.0001f)		// 근이 두개인 경우. 
	{
		// 결과값이 두개 나오지만 t1만 의미가 있고, t2는 의미가 없다.
		t	=	float(sqrt(double(t)));
		float	t1	=	(-b - t)/(2*a);
//		float	t2	=	(-b + t)/(2*a);		// 의미없는 t2
		*pT1	=	t1;
	}

	if( *pT1 > 1 )
		return	FALSE;

	*pCandidate		=	C + D*(*pT1);		

	*pT2			=	(B * (AC + D*(*pT1))) / dotBB;
	*pWhereMeet		=	A + B*(*pT2);

	float	fCW	=	VECTOR3Length( &(*pCandidate - *pSPivot));

//	if( *pT1 < 0 && fCW > fSRadius)	//  어쨌든 만난다면 -이더라도 t를 리턴.
	if( fCW > fSRadius+0.1f)	//  어쨌든 만난다면 -이더라도 t를 리턴.
	{
		return FALSE;
	}


	return		TRUE;

}

BOOL	MovingSphereMeetVertex( VECTOR3* pCandidate, float* pT, VECTOR3* pSPivot, VECTOR3* pSTo, float fSRadius, VECTOR3* pVertex)
{

//	VECTOR3&	A		=	*pSPivot;
	VECTOR3		B		=	*pSTo		-	*pSPivot;
	VECTOR3		VA		=	*pSPivot	-	*pVertex;
	float		a		=	B*B;
	float		b		=	2 * ( B * VA);
	float		c		=	VA * VA		-	fSRadius * fSRadius;

	float		t	=	b*b - 4*a*c;

	if( t < -0.0001f)
	{
		return	FALSE;
	}
	else if( t >=	-0.0001f && t < 0.0001f)
	{
		*pT		=	(-1*b) / (2*b);
	}
	else if( t >= 0.0001f)
	{
		t		=	float(sqrt(double(t)));
		*pT		=	(-1*b-t) / (2*a);
	}
	
	if( *pT	> 1)
	{
		return	FALSE;
	}
	*pCandidate		=	*pSPivot	+	B*(*pT);

	// 거리 체크, '-'더라도 만나면 '만났다' 체크.
	float	fCW	=	VECTOR3Length( &( *pCandidate - *pVertex ));
	if(	*pT < 0 && fCW > fSRadius)
	{
		return	0;
	}
	return	TRUE;

}


// 주어진 평면에서 from,to로 진행방향이 주어진 직선을 밀어준다. pMeet는 만나는 지점, pMeetPos는 만날때의 직선 위치, pSlidingPlane은 평면. pCandidate로 결과 출력.
// fMinimumFromPlane은 평면에서 최소한 떯어져있어야 하는 거리. 바운딩 스피어만 해당되는것일지도 모르겠는데, 이게 파고 들어가버리면 보정이 안되므로 이 값으로 보정을 한다.
void	SpherePushDir( VECTOR3* OUT pCandidate, VECTOR3* IN pFrom, VECTOR3* IN pTo, VECTOR3* IN pMeet, VECTOR3* IN pMeetPos, PLANE* IN pSlidingPlane, BOUNDING_VOLUME* pBV)
{
	const	float		fBorder	=	1.0f;
	VECTOR3				vPush	=	*pMeetPos - *pMeet;

	float			fPush	=	VECTOR3Length(&vPush);
	float			fTo		=	pSlidingPlane->v3Up.x*pTo->x + pSlidingPlane->v3Up.y*pTo->y + pSlidingPlane->v3Up.z*pTo->z + pSlidingPlane->D ;

	//	VECTOR3		vElasticity;
	if( pBV->fElasticity < 0.0f)
	{
		_asm int 3;		// 이런일이 있으면 안되지 않는가.?
	}

//	*pCandidate	=	*pTo	+	( pSlidingPlane->v3Up * ( fPush - fTo) *	(1.0f + pBV->fElasticity)  );
	*pCandidate	=	*pTo	+	( pSlidingPlane->v3Up * ( pBV->fRadius - fTo + fBorder) *	(1.0f + pBV->fElasticity)  );
}


// 움직이는 스피어가 스피어의 어디와 만나나.?
// 만나는 위치, 만날때 t, 만나는 접점, 움직이는 스피어from, to, 반지름, 고정된 스피어pivot, 반지름.
BOOL	WhereDidMovingSphereMeetSphere( VECTOR3* pCandidate, float* pT, VECTOR3* pWhereMeet, VECTOR3* pSFrom, VECTOR3* pSTo, float fSRadius, VECTOR3* pTSPivot, float fTSRadius)
{
	VECTOR3		vTemp;
	float		fTemp;
	VECTOR3&	A	=	*pSFrom;
	VECTOR3		B	=	*pSTo - A;
	VECTOR3&	C	=	*pCandidate;		// C = A + B*t;
	VECTOR3&	W	=	*pWhereMeet;
	float		r	=	fSRadius;
	float&		t	=	*pT;
	
	VECTOR3&	S	=	*pTSPivot;
	float		s	=	fTSRadius;

	// |CS| = (r+s)^2
	// C = A + B*t 이므로,
	// |(A-S) + B*t|^2 = (r+s)^2;
	// B*B*t^2 + 2*t*B*(A-S) + (A-S)*(A-S)-((r+s)^2)
	float	a	=	B*B;
	float	b	=	B*(A-S)*2.0f;
	float	c	=	(A-S)*(A-S)-(r+s)*(r+s);
	// 판별식 b^2-ac = 
	float	det	=	b*b - a*c*4.0f;

	if( det < -0.00001f)		// 만나지 않는다.
	{
		return	FALSE;
	}
	else if( det >= -0.00001f && det <= 0.00001f)		// 한점에서 만난다.
	{
		t	=	(-1*b) / (2*a);
	}
	else if( det >= 0.00001f)
	{
		t	=	((-1*b) - sqrt( det)) / (2*a);
	}

	if( t < 0) // 이미 만나고 있는 경우 체크.
	{
		vTemp	=	A-S;
		fTemp	=	vTemp*vTemp - (r+s)*(r+s);
		if( fTemp > 0.0f)		// 만나지 않는다.
		{
			return	FALSE;
		}
		_asm nop;
	}
	if( t >= 1.0f)		// 너무 멀어서 만나지 않는다.
	{
		return	FALSE;
	}
	
	C	=	A + B*t;
	W	=	C + (S-C)*(r / (r+s));

	fTemp	=	(*pWhereMeet - A) * (B);
	if( fTemp <= 0.0f)
		return	FALSE;

	return	TRUE;
}
