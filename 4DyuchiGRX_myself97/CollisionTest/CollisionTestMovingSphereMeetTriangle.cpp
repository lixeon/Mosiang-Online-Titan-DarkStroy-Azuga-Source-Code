
#include "CollisionTestMovingSphereMeetTriangle.h"
#include	"../../4DyuchiGXGFunc/global.h"

const	float	fTestValue	=	-0.001f;			// 일단 최적?

// 움직이는 스피어와 삼각형의 평면부분이 만나는 점.
BOOL	TestMovingSphereMeetPLANE2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pTriangle);

// 움직이는 스피어와 삼각형의 엣지가 만나는 점.
BOOL	TestMovingSphereMeetTriEdge2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pTriangle);

// 움직이는 스피어와 라인이 만나는 점.
BOOL	TestMovingSphereMeetLine2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pLFrom, VECTOR3* pLTo);
BOOL	TestMovingSphereMeetLine3( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pLFrom, VECTOR3* pLTo);

// 움직이는 스피어와 점이 만나는 점.
BOOL	TestMovingSphereMeetVertex2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pVertex);

BOOL	CollisionTestMovingSphereMeetTriangle( DWORD* pdwOutComponentType, VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pInSphere, VECTOR3* pInTriangle)
{
	// 일단, 속도 벡터와 주어진 평면의 노말이 수직이거나, 같은 방향일 경우 무조껀 false 리턴.
	PLANE	TestP;
	CalcPlaneEquation( &TestP, pInTriangle);
	
	float	fDot	=	DotProduct(&(TestP.v3Up), &(pInSphere->Velocity));
//	if( fDot >= -0.00001f)
	if( fDot >= 0.0f)
		return	FALSE;
	

	if( pInSphere->Velocity.y != 0.0f)
		_asm nop;

	
	//	100.0f로 반지름이 스케일 되어있나.?
	float		fOriginalRadius	=	pInSphere->fRadius;
	float		fUnitFactor		=	100.0f;
	float		fScaleFactor	=	fUnitFactor	/	fOriginalRadius;

	MOVING_SPHERE	Sphere	=	*pInSphere;
	VECTOR3		pTri[3];
	pTri[0]		=	pInTriangle[0];
	pTri[1]		=	pInTriangle[1];
	pTri[2]		=	pInTriangle[2];

	BOOL	bScaled	=	FALSE;

	if( fOriginalRadius != 100.0f)
	{
		// 스케일을 시킨다.
		bScaled		=	TRUE;
		
		Sphere.fRadius		*=	fScaleFactor;
		VECTOR3_MULEQU_FLOAT( &Sphere.From, fScaleFactor);
		VECTOR3_MULEQU_FLOAT( &Sphere.Velocity, fScaleFactor);

		VECTOR3_MULEQU_FLOAT( &(pTri[0]), fScaleFactor);
		VECTOR3_MULEQU_FLOAT( &(pTri[1]), fScaleFactor);
		VECTOR3_MULEQU_FLOAT( &(pTri[2]), fScaleFactor);
	}


	BOOL	bFound	=	FALSE;
	*pOutT	=	1.1f;

	DWORD	dwLastMeetComponent	=	0xffffffff;
	// 0 평면, 1, 라인, 2, 점.

	float		fResultTEdge;
	float		fResultTPlane;
	float		fResultTVertex;
	VECTOR3		WhereMeet;


	//	삼각형의 엣지와 테스트.
	pInSphere->fRadius -= -10.0f;
	BOOL	bEdgeMeet	=	TestMovingSphereMeetTriEdge2( &WhereMeet, &fResultTEdge, &Sphere, pTri);
	if( bEdgeMeet == TRUE)
	{
		if( *pOutT > fResultTEdge)
		{
			CheckVectorArray( &WhereMeet, 1);
			bFound	=	TRUE;
			*pOutT	=	fResultTEdge;
			*pOutWhereMeet	=	WhereMeet;

			dwLastMeetComponent	=	10;
		}
	}
	pInSphere->fRadius += 10.0f;

	//	삼각형의 평면과 테스트.
	BOOL	bPlaneMeet	=	TestMovingSphereMeetPLANE2( &WhereMeet, &fResultTPlane, &Sphere, pTri);
	if( bPlaneMeet == TRUE)
	{
		if( *pOutT > fResultTPlane)
		{
			CheckVectorArray( &WhereMeet, 1);
			bFound	=	TRUE;
			*pOutT	=	fResultTPlane;
			*pOutWhereMeet	=	WhereMeet;

			dwLastMeetComponent		=	0;
		}
	}

	if( dwLastMeetComponent == 10)
	{
		_asm nop;
//		return	FALSE;
	}

	//	삼각형의 모서리와 테스트.
	int i;
	BOOL		bMeet;
	for( i = 0; i < 3; i++)
	{
		bMeet	=	TestMovingSphereMeetVertex2( &WhereMeet, &fResultTVertex, &Sphere, &(pTri[i]));
		if( bMeet == TRUE)
		{
//			BreakIfTrue( fResultT < -0.01f || fResultT > 1.01f);
			if( *pOutT > fResultTVertex)
			{
				CheckVectorArray( &WhereMeet, 1);
				bFound	=	TRUE;
				*pOutT	=	fResultTVertex;
				*pOutWhereMeet	=	WhereMeet;

				dwLastMeetComponent	=	20+i;
			}
		}
	}



	
	if( bFound == FALSE)
	{
		return	FALSE;
	}

	if( dwLastMeetComponent == 10)
	{
		_asm nop;
	}


	// 속도 벡터와, 만나는 평면이 평행할 경우.? 어쩌라고.???
	VECTOR3			MeetPivot;
	MeetPivot		=	Sphere.From		+	Sphere.Velocity * (*pOutT);

	VECTOR3			PlaneNormal;
	PlaneNormal		=	MeetPivot	-	*pOutWhereMeet;
	float			fNormal	=	VECTOR3Length( &PlaneNormal);

	Normalize( &PlaneNormal, &PlaneNormal);

	float		fTest;
	fTest		=	DotProduct( &PlaneNormal, &Sphere.Velocity);
	if( fTest > fTestValue)
		return	FALSE;



	// 리스케일
	float	fRescale;
	if( bScaled)
	{
		fRescale	=		fOriginalRadius / fUnitFactor;
		VECTOR3_MULEQU_FLOAT( pOutWhereMeet, fRescale);
	}

	// 다시 체크.
	VECTOR3		MeetPivot2;
	MeetPivot2		=	pInSphere->From		+	pInSphere->Velocity * (*pOutT);

	VECTOR3		PlaneNormal2;
	PlaneNormal2	=	MeetPivot2	-	*pOutWhereMeet;
	float			fNormal2	=	VECTOR3Length( &PlaneNormal2);

	Normalize( &PlaneNormal2, &PlaneNormal2);

	float	fTest2;
	fTest2		=	DotProduct( &PlaneNormal2, &(pInSphere->Velocity));
//	if( fTest2 > fTestValue)
//		return	FALSE;
//	if( fTest2 >= 0.0f)
//		_asm int 3;



	CheckVectorArray( pOutWhereMeet, 1);
	*pdwOutComponentType	=	dwLastMeetComponent;
	return	TRUE;
}

BOOL	TestMovingSphereMeetPLANE2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pTri)
{
	BOOL		bResult;
	float		fTemp;

	float		fPlaneDotVelocity;

	float		t;			// 스피어가 면에 닿을때의 시점.
	VECTOR3		X;			// 스피어가 면에 닿는 위치.
	VECTOR3		MeetPos;	// 스피어와 면이 맞닿는 점.

	VECTOR3		Velocity	=	pSphere->Velocity;

	PLANE	p;
	CalcPlaneEquation( &p, pTri);

//	MakePLANE( &p, &(pTri[0]) , &(pTri[1]), &(pTri[2]));

	// 평행하거나, 스피어의 진행 방향과 면의 노말이 같은 방향이면 영향을 못 미친다.
	fPlaneDotVelocity	=	DotProduct( &(p.v3Up), &Velocity);
	if( fPlaneDotVelocity >= -0.0001f)
	{
		// 안만난다 리턴.
		return	FALSE;
	}

	// 스피어가 평면 뒷쪽에 있으면 당근 패스.
	float	fDet	=	DotProduct( &(p.v3Up), &(pSphere->From))	+	p.D;
	if( fDet < 0.0f)
	{
		return	FALSE;
	}

	// 스피어의 원점	: X = F + (T-F)*t
	// 평면에서 스피어 원점까지의 거리.		: X*N + d = r  (r은 스피어 반지름.)
	// t = -1*(N*F+d-r)/(N*(T-F))
	t	=	DotProduct( &(p.v3Up), &(pSphere->From));
	t	=	pSphere->fRadius - (t + p.D);
	t	=	t / fPlaneDotVelocity;
	// X = F + (T-F)*t
	VECTOR3_MUL_FLOAT( &X, &Velocity, t);
	VECTOR3_ADDEQU_VECTOR3( &X, &(pSphere->From));

	if( t > 1.0f)
	{
		return	FALSE;
	}

	if( t < -1.0f)	//	이런 경우는,
	{
		//	그냥 리턴 해도 될까.?
		//	계산이 정확하다면 당연하겠지.?
		return	FALSE;
	}
/*		//	'-1'이면 괜찮은건가.? 더 높게.?

		// 만약에 스피어의 중점이 면을 넘어가버렸다면 더이상 상대할 가치가 없다.
		// 오차가 쌓여서 이런 상황이 올 수는 있다.
		float	det	=	DotProduct( &(pSphere->From), &(p.v3Up))	+	p.D;
		if( det < 0.0f)
			return	FALSE;
*/
	// 여기서 0에 가까운 t값은 그냥 0으로 처리 하는게 좋지 않을까.?

	// 스피어와 평면이 만날때 서로 맞닿은 점은.?
	// X에서 평면노말의 반대방향으로 반지름만큼 간 점.
	// MeetPos	=	X + N*(-1*fRadius)
	MeetPos	=	p.v3Up;
	VECTOR3_MULEQU_FLOAT( &MeetPos, pSphere->fRadius * -1.0f);
	VECTOR3_ADDEQU_VECTOR3( &MeetPos, &X);

	// 그 만나는 점이 삼각형 안에 있는가.?
//	VECTOR2		v2Meet;
//	bResult	=	IsTriMeetVertex( &v2Meet, pTri, &MeetPos);
	bResult	=	IsTriMeetVertex( pTri, &MeetPos);

	if( bResult	== FALSE)
		return	FALSE;

	// 만약 float의 오차에 의해 이미 만나고 있는 점으로 판별될 경우를 가려서 처리한다.
	if( t < 0.0f)
	{
		// 만약 이미 만나고 있는 경우라면 스피어가 움직이기 전의 위치를 리턴한다.
		fTemp	=	DotProduct( &(pSphere->From), &(p.v3Up)) + p.D;
		// 스피어의 반지름은 정규화에 의해 100.0f이므로, 중점이 0.0f까지 들어갈 일은 없다고 보면 된다.
		if( fTemp <= 0.0f )	// 완전히 벗어났다면,  이 경우 반지름 1짜리가 왔다갔다 하면 문제가 될려나?
			return	FALSE;
		else
			_asm nop;
	}


	*pOutT			=	t;

	*pOutWhereMeet	=	MeetPos;

#ifdef	_DEBUG
	CheckVectorArray( pOutWhereMeet, 1);


	// 평면과 속도벡터가 평행하면 애초부터 말이 안된다. 그걸 잡아낸다.
	VECTOR3		MeetPivot;
	MeetPivot		=	pSphere->From		+	pSphere->Velocity * (*pOutT);

	VECTOR3		PlaneNormal;
	PlaneNormal		=	MeetPivot	-	*pOutWhereMeet;
	float			fNormal	=	VECTOR3Length( &PlaneNormal);

	Normalize( &PlaneNormal, &PlaneNormal);

	float	fTest;
	fTest		=	DotProduct( &PlaneNormal, &(pSphere->Velocity));
	if( fTest > fTestValue)
		return	FALSE;
//		_asm int 3;


#endif	


	// 리턴.
	return	TRUE;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	TestMovingSphereMeetTriEdge2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pTriangle)
{
	// 세개의 엣지를 돌아보다.
	BOOL	bResult[3];
	BOOL	bFound	=	FALSE;
	VECTOR3		WhereMeet[3];
	float		fMeetT[3];
	float		fLastMeetT	=	1.0f;
	VECTOR3		LastWhereMeet;

	DWORD		dwMeetTime	=	0;
	
	DWORD	i;
	for( i = 0; i < 3; i++)
	{
//		bResult[i]	=	TestMovingSphereMeetLine2( &WhereMeet[i], &fMeetT[i], pSphere, &pTriangle[i], &pTriangle[(i+1)%3]);
		bResult[i]	=	TestMovingSphereMeetLine3( &WhereMeet[i], &fMeetT[i], pSphere, &pTriangle[i], &pTriangle[(i+1)%3]);
		if( bResult[i] == TRUE)
		{
			CheckVectorArray( &WhereMeet[i], 0);
			if( fMeetT[i] > 1.0f)
				_asm int 3;

			if( fMeetT[i] < fLastMeetT)
			{
				bFound	=	TRUE;
				dwMeetTime++;
				fLastMeetT		=	fMeetT[i];
				LastWhereMeet		=	WhereMeet[i];
			}
		}
	}


	if( dwMeetTime == 2)
		_asm nop;

	if( bFound == FALSE)
	{
		return	FALSE;
	}

	CheckVectorArray( &LastWhereMeet, 0);

	*pOutT				=	fLastMeetT;
	*pOutWhereMeet		=	LastWhereMeet;

	return	TRUE;


}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	2003.03.19	t가 '-'일 경우 다시 만나고 있는지 체크하게 고치다. 이 방법밖엔 없나. -_-;
	2003.03.18	t가 '-'일 경우 리턴으로 고치다.
	움직이는 직선이 라인과 만나는가.?

	직선은		M = A + B*s;
	구의 궤적은 P = C + D*t;

	이때
	1. B*(M-P) = 0;
	2. (P-M)*(P-M) = r^2
	두개가 나온다.

	식1에 M, P를 대입하여 s = ~~  식으로 유도한 뒤,
	직선 M = A + B*s에 대입하여 M을 t에 관한 식으로 유도.

	M과 P를 식2에 대입해서, (E+F*t)*(E+F*t) = r^2 으로,

	F*F*t^2 + 2*E*F*t + E*E-r^2 = 0
	 = a        = b       = c

	판별식 det = b^2 - 4*a*c;

	근이 두개일때 t1 = (-b - root(det)) / 2a 하나만 사용 (t2는 의미가 없다.)

    이때 0 < s < 1 이 아니면 근은 의미가 없다.
*/

BOOL	TestMovingSphereMeetLine2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pLFrom, VECTOR3* pLTo)
{
	VECTOR3		A	=	*pLFrom;		// LineFrom
	VECTOR3		B;									// LineVelocity
	VECTOR3_SUB_VECTOR3( &B, pLTo, pLFrom);
	if( VECTOR3Length( &B) == 0.0f)
		return	FALSE;					// 

	VECTOR3		M;									// Line위의 한점.
	float		s;									// 그때의 시점.

	VECTOR3		C	=	pSphere->From;					// Sphere From.
	VECTOR3		D	=	pSphere->Velocity;					// Sphere Velocity.

	// 만약 라인의 방향과 스피어의 진행 방향이 같다면 계산이 의미가 없으므로 바로 빠져나간다.
	VECTOR3		NormalizedB;
	Normalize( &NormalizedB, &B);
	VECTOR3		NormalizedD;
	Normalize( &NormalizedD, &D);
	VECTOR3_SUBEQU_VECTOR3( &NormalizedB, &NormalizedD);
	float	fV	=	VECTOR3Length( &NormalizedB);
	if( fV <= 0.0001f || fV >= 1.9999f)
		return	FALSE;


	
	//	B와 D가 수직인 경우.
//	라인의 방향과 스피어가 움직이는 방향이 

	VECTOR3		P;									// Sphere위의 한점.
	float		t;									// 그때의 시점.

	float		dotBB	=	DotProduct( &B, &B);
	float		fTemp	=	(DotProduct( &B, &C) - DotProduct( &A, &B)) / dotBB;
	VECTOR3		E	=	B;
	VECTOR3_MULEQU_FLOAT( &E, fTemp);
	VECTOR3_SUBEQU_VECTOR3( &E, &C);
	VECTOR3_ADDEQU_VECTOR3( &E, &A);

	VECTOR3		F	=	B;
	VECTOR3_MULEQU_FLOAT( &F, DotProduct(&B, &D)/dotBB);
	VECTOR3_SUBEQU_VECTOR3( &F, &D);


	// a*t^2 + b*t + c = 0
	float	a	=	DotProduct( &F, &F);
	float	b	=	2.0f * DotProduct( &E, &F);
	float	c	=	DotProduct( &E, &E)	-	pSphere->fRadius * pSphere->fRadius;

	float	det	=	b*b - 4.0f*a*c;					// 판별식.

	// 판별식 < 0
	if( det < -0.001f)
	{
		// 만나지 않지.? 절대로.
		return	FALSE;
	}

	// 판별식 <= 0		// 의미 없다고 봐야지.?
	if( det <= 0.0f)
	{
		return	FALSE;
	}

	// 판별식 > 0
	t	=	(-b - (float)sqrt(det)) / (2.0f*a);

	// t가 너무 멀 경우. 
	if( t > 1.0f)
	{
		return	FALSE;
	}

	// 만날때 Sphere의 중점은 어디.?
	P	=	C + D*t;

	// 만날때 Line의 위치는 어디.?
	s	=	(B*C - A*B + B*D*t) /dotBB;
	if( s < 0.0f || s > 1.0f)		// line의 위치를 벗어나면,
	{
		return	FALSE;
	}
	M	=	A + B*s;		// 직선과 만나는 부분.

	// t가 '-'일 경우 스피어의 원래 위치에서 이미 만나고 있는가를 체크한다.
	if( t < 0.0f)
	{
//		return	FALSE;
		// 스피어의 중점보다 더 뒤에 있는 경우.
		VECTOR3			Temp1	=	M	-	pSphere->From;
		VECTOR3			Temp2	=	pSphere->Velocity;
		float			Temp3	=	DotProduct(&Temp1, &Temp2);
		if( Temp3 < 0.00001f)
			return	FALSE;

		// 여기로 패스하면 이미 만나고 있거나 오차상 살짝 파고 들어갔을 경우.
	}

	// 만나는 점이 이 구의 진행 방향과 수평인가? 그렇다면 무시.
	VECTOR3		WheneMeet	=	M	-	P;
	float		fDotWheneMeet	=	DotProduct( &WheneMeet, &pSphere->Velocity);
//	if( fDotWheneMeet <= 0.00001f)
	if( fDotWheneMeet <= 0.0f)
	{
		return	FALSE;
	}


	*pOutWhereMeet	=	M;
	*pOutT			=	t;

	return	TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	움직이는 스피어가 점과 만나는가.?
	
	점은 P, 
	스펴의 중점은 X = A + B*t;	로 정의.
	PX의 길이는 r.

*/
BOOL	TestMovingSphereMeetVertex2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pVertex)
{
	VECTOR3		A	=	pSphere->From;								// 스펴 프럼.
	VECTOR3		B	=	pSphere->Velocity;							// 스펴 속도벡터.
//	VECTOR3_SUB_VECTOR3( &B, &(pSphere->->To), &(pSphere->From));			
	VECTOR3		X;												// 스펴가 점과 만날때의 위치.
	float		t;												// 만나는 시점.
	// X = A + B*t

	VECTOR3		P	=	*pVertex;		// 타겟 점.

	VECTOR3		PA;
	VECTOR3_SUB_VECTOR3( &PA, &A, &P);

	float		a	=	DotProduct( &B, &B);
	float		b	=	2.0f * DotProduct( &B, &PA);
	float		c	=	DotProduct( &PA, &PA)	-	pSphere->fRadius * pSphere->fRadius;

	float		det	=	b*b		-	4*a*c;

	if( det <= 0.00000f)
	{
		return	FALSE;
	}
	
	t	=	(-b - (float)sqrt(det)) / (2*a);
	if( t > 1.0f)
	{
		return	FALSE;
	}

	//	X	=	A	+	B*t;
	VECTOR3_MUL_FLOAT( &X, &B, t);
	VECTOR3_ADDEQU_VECTOR3( &X, &A);

	// t가 '-'이더라도 이미 만나고 있고, 스피어의 중점이 아직 그 점을 안지났으면 TRUE 리턴.
	float	fTemp;
	if( t < 0.0f)
	{
		_asm nop;
		// 일단 스피어 내에 포함되는가?
		VECTOR3		vTemp;
		VECTOR3_SUB_VECTOR3( &vTemp, &(pSphere->From), &P);
		fTemp	=	VECTOR3Length( &vTemp);
		if( fTemp >= pSphere->fRadius)
			return	FALSE;

		// 스피어 From 위에 있고, 스피어의 속도벡터를 노말로 가지는 평면에 대해, '점'이 앞에 있나 뒤에 있나.?
		// 앞에 있다면 유효한 점이란 뜻이고, 뒤에 있다면 무효한 점이란 뜻이다.
		// 일단 스피어의 크기는 100.0f로 스케일 한 상태이니 믿어도 좋을듯.
		PLANE	p;
		p.v3Up		=	pSphere->Velocity;
		Normalize(&p.v3Up, &p.v3Up);
		p.D		=	-1.0f * DotProduct( &p.v3Up, &(pSphere->From));

		fTemp	=	DotProduct( &p.v3Up, pVertex)	+	p.D;
		if( fTemp <= 0.0f)
			return	FALSE;
		else
			_asm nop
/*
		VECTOR3		vTemp;
		VECTOR3_SUB_VECTOR3( &vTemp, &(pSphere->From), &P);
		float	fDist	=	VECTOR3Length( &vTemp);
		if( fDist >= pSphere->fRadius)
		{
			return	FALSE;
		}
*/
	}

	// pOut 정리.
	*pOutT			=	t;			// 만났을때 시점.
//	pOut->MeetPivot	=	X;			// 만났을때 스펴 피봇.
	*pOutWhereMeet	=	P;			// 당근 점이지.?

#ifdef	_DEBUG
	CheckVectorArray( pOutWhereMeet, 1);


	// 평면과 속도벡터가 평행하면 애초부터 말이 안된다. 그걸 잡아낸다.
	VECTOR3		MeetPivot;
	MeetPivot		=	pSphere->From		+	pSphere->Velocity * (*pOutT);

	VECTOR3		PlaneNormal;
	PlaneNormal		=	MeetPivot	-	*pOutWhereMeet;
	float			fNormal	=	VECTOR3Length( &PlaneNormal);

	Normalize( &PlaneNormal, &PlaneNormal);

	float	fTest;
	fTest		=	DotProduct( &PlaneNormal, &(pSphere->Velocity));
	if( fTest > fTestValue)
		return	FALSE;
//		_asm int 3;


#endif	

	return	TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	2003.03.19	t가 '-'일 경우 다시 만나고 있는지 체크하게 고치다. 이 방법밖엔 없나. -_-;
	2003.03.18	t가 '-'일 경우 리턴으로 고치다.
	움직이는 직선이 라인과 만나는가.?

	직선은		M = A + B*s;
	구의 궤적은 P = C + D*t;

	이때
	1. B*(M-P) = 0;
	2. (P-M)*(P-M) = r^2
	두개가 나온다.

	식1에 M, P를 대입하여 s = ~~  식으로 유도한 뒤,
	직선 M = A + B*s에 대입하여 M을 t에 관한 식으로 유도.

	M과 P를 식2에 대입해서, (E+F*t)*(E+F*t) = r^2 으로,

	F*F*t^2 + 2*E*F*t + E*E-r^2 = 0
	 = a        = b       = c

	판별식 det = b^2 - 4*a*c;

	근이 두개일때 t1 = (-b - root(det)) / 2a 하나만 사용 (t2는 의미가 없다.)

    이때 0 < s < 1 이 아니면 근은 의미가 없다.
*/

BOOL	TestMovingSphereMeetLine3( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pLFrom, VECTOR3* pLTo)
{
	VECTOR3		A	=	*pLFrom;		// LineFrom
	VECTOR3		B;									// LineVelocity
	VECTOR3_SUB_VECTOR3( &B, pLTo, pLFrom);
	if( VECTOR3Length( &B) == 0.0f)
		return	FALSE;					// 

	VECTOR3		M;									// Line위의 한점.
	float		s;									// 그때의 시점.

	VECTOR3		C	=	pSphere->From;					// Sphere From.
	VECTOR3		D	=	pSphere->Velocity;					// Sphere Velocity.

	// 만약 라인의 방향과 스피어의 진행 방향이 같다면 계산이 의미가 없으므로 바로 빠져나간다.
	VECTOR3		NormalizedB;
	Normalize( &NormalizedB, &B);
	VECTOR3		NormalizedD;
	Normalize( &NormalizedD, &D);
	VECTOR3_SUBEQU_VECTOR3( &NormalizedB, &NormalizedD);
	float	fV	=	VECTOR3Length( &NormalizedB);
	if( fV <= 0.0001f || fV >= 1.9999f)
		return	FALSE;


	
	//	B와 D가 수직인 경우.
//	라인의 방향과 스피어가 움직이는 방향이 

	VECTOR3		P;									// Sphere위의 한점.
	float		t;									// 그때의 시점.

	float		dotBB	=	DotProduct( &B, &B);
	float		fTemp	=	(DotProduct( &B, &C) - DotProduct( &A, &B)) / dotBB;
	VECTOR3		E	=	B;
	VECTOR3_MULEQU_FLOAT( &E, fTemp);
	VECTOR3_SUBEQU_VECTOR3( &E, &C);
	VECTOR3_ADDEQU_VECTOR3( &E, &A);

	VECTOR3		F	=	B;
	VECTOR3_MULEQU_FLOAT( &F, DotProduct(&B, &D)/dotBB);
	VECTOR3_SUBEQU_VECTOR3( &F, &D);


	// a*t^2 + b*t + c = 0
	float	a	=	DotProduct( &F, &F);
	float	b	=	2.0f * DotProduct( &E, &F);
	float	c	=	DotProduct( &E, &E)	-	pSphere->fRadius * pSphere->fRadius;

	float	det	=	b*b - 4.0f*a*c;					// 판별식.

	// 판별식 < 0
	if( det < -0.001f)
	{
		// 만나지 않지.? 절대로.
		return	FALSE;
	}

	// 판별식 <= 0		// 의미 없다고 봐야지.?
	if( det <= 0.0f)
	{
		return	FALSE;
	}

	// 판별식 > 0
	t	=	(-b - (float)sqrt(det)) / (2.0f*a);

	// t가 너무 멀 경우. 
	if( t > 1.0f)
	{
		return	FALSE;
	}

	// 만날때 Sphere의 중점은 어디.?
	P	=	C + D*t;

	// 만날때 Line의 위치는 어디.?
	s	=	(B*C - A*B + B*D*t) /dotBB;
	if( s < 0.0f || s > 1.0f)		// line의 위치를 벗어나면,
	{
		return	FALSE;
	}
	M	=	A + B*s;		// 직선과 만나는 부분.

	// t가 '-'일 경우 스피어의 원래 위치에서 이미 만나고 있는가를 체크한다.
	if( t < 0.0f)
	{
//		return	FALSE;
		// 스피어의 중점보다 더 뒤에 있는 경우.
		VECTOR3			Temp1	=	M	-	pSphere->From;
		VECTOR3			Temp2	=	pSphere->Velocity;
		float			Temp3	=	DotProduct(&Temp1, &Temp2);
		if( Temp3 < 0.00001f)
			return	FALSE;

		// 여기로 패스하면 이미 만나고 있거나 오차상 살짝 파고 들어갔을 경우.
	}

	// 만나는 점이 이 구의 진행 방향과 수평인가? 그렇다면 무시.
	VECTOR3		WheneMeet	=	M	-	P;
	float		fDotWheneMeet	=	DotProduct( &WheneMeet, &pSphere->Velocity);
//	if( fDotWheneMeet <= 0.00001f)
	if( fDotWheneMeet <= 0.0f)
	{
		return	FALSE;
	}


	*pOutWhereMeet	=	M;
	*pOutT			=	t;

	return	TRUE;
}
