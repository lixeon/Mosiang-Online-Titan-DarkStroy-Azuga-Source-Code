
#include "CollisionTestMovingEllipsoidMeetTriangle.h"
#include "CollisionTestMovingSphereMeetTriangle.h"
#include	"../../4DyuchiGXGFunc/global.h"



// 움직이는 타원과 삼각형이 주어졌을때 둘이 만나는 시점을 구한다.
// 만날 경우 TRUE, 안만날경우 FALSE를 리턴한다.
//BOOL	CollisionTestMovingEllipsoidMeetTriangle( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_ELLIPSOID* pEllipsoid, VECTOR3* pTriangle)
BOOL	CollisionTestMovingEllipsoidMeetTriangle( COLLISION_RESULT* pOut, MOVING_ELLIPSOID* pEllipsoid, VECTOR3* pTriangle)
{
	// 속도 벡터 체크, 움직이지 않는 타원체라면 바로 쫒아낸다.
	float	fVelocity	=	VECTOR3Length( &pEllipsoid->Velocity);
	if( fVelocity == 0.0f)
		return	FALSE;

	float		fFactorUnit	=	100.0f;		// 단위원의 크기. 1.0f으로 하면 계산중 1.0을 한계로 정한 것들과 문제가 생긴다.
//	float		fFactorUnit	=	pEllipsoid->fWidth;


	// x, z방향은 그대로 두고 y방향만 x, z와 같게 스케일 한다.

	BOOL	bResult;
	// 주어진 타원체를 단위 스피어(?)로 만들고, 주어진 삼각형도 같이 곱한다.
	float		fXFactor;			// 타원체의 x 스케일, 삼각형의 좌표도 이것에 따라 줄어든다.
	fXFactor	=	pEllipsoid->fWidth	/	fFactorUnit;
	float		fYFactor;			// 타원체의 높이 스케일.
	fYFactor	=	pEllipsoid->fHeight		/	fFactorUnit;
	float		fZFactor	=	fXFactor;	// x와 z는 같다.

	VECTOR3		pST[3];			// 스피어 공간으로 바꾼 pTri;
	pST[0].x	=	pTriangle[0].x	/	fXFactor;
	pST[0].y	=	pTriangle[0].y	/	fYFactor;
	pST[0].z	=	pTriangle[0].z	/	fZFactor;
	pST[1].x	=	pTriangle[1].x	/	fXFactor;
	pST[1].y	=	pTriangle[1].y	/	fYFactor;
	pST[1].z	=	pTriangle[1].z	/	fZFactor;
	pST[2].x	=	pTriangle[2].x	/	fXFactor;
	pST[2].y	=	pTriangle[2].y	/	fYFactor;
	pST[2].z	=	pTriangle[2].z	/	fZFactor;

	MOVING_SPHERE	Sphere;		// 스피어 로 바뀔 타원체.
	Sphere.fRadius	=	fFactorUnit;
	Sphere.From.x	=	pEllipsoid->From.x	/	fXFactor;
	Sphere.From.y	=	pEllipsoid->From.y	/	fYFactor;
	Sphere.From.z	=	pEllipsoid->From.z	/	fZFactor;
	Sphere.Velocity.x	=	pEllipsoid->Velocity.x / fXFactor;
	Sphere.Velocity.y	=	pEllipsoid->Velocity.y / fYFactor;
	Sphere.Velocity.z	=	pEllipsoid->Velocity.z / fZFactor;

	// 만들어진 스피어를 테스트.
	float		fMeetT;
	VECTOR3		WhereMeet;
	DWORD		dwLastComponentType;
	BOOL	bFound	=	CollisionTestMovingSphereMeetTriangle( &dwLastComponentType, &WhereMeet, &fMeetT, &Sphere, pST);

	// 만난게 있는가.?
	if( bFound == FALSE)
	{
		return	FALSE;
	}

	if( dwLastComponentType	== 10)
		_asm nop;

//	만나는 시점.
	pOut->fMeetTime		=	fMeetT;
//	만나는 시점.
	pOut->vMeetPivot		=	pEllipsoid->From	+	(pEllipsoid->Velocity * fMeetT);
//	만나는 점.
	pOut->vWhereMeet.x	=	WhereMeet.x	*	fXFactor;
	pOut->vWhereMeet.y	=	WhereMeet.y	*	fYFactor;
	pOut->vWhereMeet.z	=	WhereMeet.z	*	fZFactor;
//	만나는 평면.
	PLANE		OutPlane;
	AAELLIPSOID		EllipsoidOnMeetPos;
	EllipsoidOnMeetPos.P	=	pOut->vMeetPivot;
	EllipsoidOnMeetPos.h	=	pEllipsoid->fHeight;
	EllipsoidOnMeetPos.w	=	pEllipsoid->fWidth;
	bResult	=	CalculatePLANEOnEllipsoidWithVertex( &OutPlane, &EllipsoidOnMeetPos, &pOut->vWhereMeet);
	if( bResult == FALSE)
		_asm int 3;	// 이런일은 절대 없어야 한다.
	pOut->MeetPlane		=	OutPlane;

	// 만나는 평면과 속도벡터가 평행한가.?
	float	fMeetPlaneDotVelocity	=	DotProduct(&(pOut->MeetPlane.v3Up), &(pEllipsoid->Velocity));
	if( fMeetPlaneDotVelocity >= 0.0f)
	{
		_asm nop;		// 암만 오차가 있어도 왜 이런일이 생기는가.?
		return		FALSE;
	}

	// 왜??? 왜? 왜? 이런 일이 생기면 절대 안된다.
	float	fDotPlaneVelocity	=	DotProduct( &pEllipsoid->Velocity, &OutPlane.v3Up);
	if( fDotPlaneVelocity >= 0.001f)
		_asm int 3;

	// 처음 주어진 속도보다 더 멀리가서 만난다고 판정되는 경우?
	float	fFP	=	VECTOR3Length(&(pOut->vMeetPivot - pEllipsoid->From));
	if( fFP > fVelocity)
	{
		return	FALSE;
	}

	pOut->dwComponentType	=	dwLastComponentType;

	


	return	TRUE;
}

// 타원과 타원위의 한 점이 주어졌을 때 접평면을 구한다.
// 타원과 주어진 점이 떯어져 있을 경우 FALSE 리턴 예정이다.
BOOL	CalculatePLANEOnEllipsoidWithVertex( PLANE* pOutPlane, AAELLIPSOID* pEllipsoid, VECTOR3* pVertex)
{
#ifdef	_DEBUG
	// pVertex가 pEllipsoid 위에 있는가 일단 검사.
	float	fDetX	=	(pVertex->x - pEllipsoid->P.x);
	float	fDetZ	=	(pVertex->z - pEllipsoid->P.z);
	float	fDetY	=	(pVertex->y - pEllipsoid->P.y);

	float	fDet1	=	((fDetX * fDetX) + (fDetZ * fDetZ)) / (pEllipsoid->w * pEllipsoid->w);
	float	fDet2	=	(fDetY*fDetY)/(pEllipsoid->h*pEllipsoid->h);

	float	fDet	=	fDet1	+	fDet2;

	if( fDet > 1.00001f || fDet < -0.99999f)
		_asm nop;
#endif




/*
	타원위의 주어진 한 점(x1, y1, z1)
	타원의 중점 (xc, yx, zc) 라고 할 때

	(x1 - xc)(x - xc)   (y1 - yc)(y - yc)   (z1 - zc)(z - zc)
	----------------- + ----------------- + -----------------    =    1
	       a*a                 b*b                 b*b


	(x1 - xc)         (y1 - yc)         (z1 - zc)         (x1 - xc) * xc     (y1 - yc) * yc     (z1 - zc) * zc
	--------- * x  +  --------- * y  +  --------- * z  -  --------------  -  --------------  -  --------------  -  d   =   0
	   a*a               b*b               c*c                  a*a                 b*b               c*c



1) 타원에 접하는 평면은

			(x1 - xc)
	N.x =	---------
			   a*a

			(y1 - yc)
	N.y =	---------
			   b*b

			(z1 - zc)
	N.z =	---------
			   c*c

			   (x1 - xc) * xc     (y1 - yc) * yc     (z1 - zc) * zc
평면의 d = 	-  --------------  -  --------------  -  --------------  -  1
					a*a                 b*b               c*c


2) 이때 구해진 평면은 노말 방향이 타원의 바깥쪽이므로, 평면의 a, b, c, d 각각에 -를 곱해서 노말 방향을 뒤집어준다.

	N.x = -N.x;
	N.y = -N.y;
	N.z = -N.z;
	d = -d


3) 그다음 노말라이즈.

*/  

	float	x1xc	=	pVertex->x - pEllipsoid->P.x;
	float	y1yc	=	pVertex->y - pEllipsoid->P.y;
	float	z1zc	=	pVertex->z - pEllipsoid->P.z;

	float	aa		=	pEllipsoid->w * pEllipsoid->w;
	float	bb		=	pEllipsoid->h * pEllipsoid->h;
	float&	cc		=	aa;




	PLANE	OnEllipsoid;
// 1)	접평면.
	OnEllipsoid.v3Up.x	=	x1xc / aa;
	OnEllipsoid.v3Up.y	=	y1yc / bb;
	OnEllipsoid.v3Up.z	=	z1zc / cc;
	OnEllipsoid.D		=	-1.0f * (x1xc * pEllipsoid->P.x / aa) - (y1yc * pEllipsoid->P.y / bb) - (z1zc * pEllipsoid->P.z / cc) - 1;

// 2, 3) 면 뒤집고 노말라이즈.
	float	fPlaneLength	=	(float)sqrt( OnEllipsoid.v3Up.x * OnEllipsoid.v3Up.x  +  OnEllipsoid.v3Up.y * OnEllipsoid.v3Up.y  +  OnEllipsoid.v3Up.z * OnEllipsoid.v3Up.z );
	OnEllipsoid.v3Up	=	(OnEllipsoid.v3Up	/	fPlaneLength)	*	-1.0f;
	OnEllipsoid.D		=	OnEllipsoid.D		/	fPlaneLength	*	-1.0f;;

	*pOutPlane		=	OnEllipsoid;

	
#ifdef	_DEBUG	
	// 면에서 거리가 얼마나 떨어져있나? 테스트코드.
	float	fDebug	=	DotProduct( pVertex, &(pOutPlane->v3Up))	+	pOutPlane->D;
	if( fDebug > 0.1f || fDebug < -0.1f)
	{
		_asm int 3;
		return	FALSE;
	}
#endif



	return	TRUE;
}

// 이동하는 타원을 주어진 평면을 기준으로 민다.
BOOL	PushEllipsoidFromPLANE( VECTOR3* pOut, COLLISION_RESULT* pCollisionInfo, VECTOR3* pCurrentPivot, VECTOR3* pVelocity)
{
#ifdef	_DEBUG
	BOOL	bResult;
	bResult	=	IsValidFloat( pCollisionInfo->fMeetTime);
	if( bResult == FALSE)
		_asm int 3;
	bResult	=	IsValidVector3( &pCollisionInfo->vMeetPivot, 1);
	if( bResult == FALSE)
		_asm int 3;
	bResult	=	IsValidVector3( &pCollisionInfo->MeetPlane.v3Up, 1);
	if( bResult == FALSE)
		_asm int 3;
	bResult	=	IsValidVector3( &pCollisionInfo->vWhereMeet, 1);
	if( bResult == FALSE)
		_asm int 3;
	// 혹시 만난 평면과 진행 방향이 직각?이면.?
	float	fDotPlaneVelocity	=	DotProduct( pVelocity, &pCollisionInfo->MeetPlane.v3Up);
	if( fDotPlaneVelocity >= 0.0f)
		_asm int 3;								// 여기에 걸리면 밖에, 충돌처리직전 삼각형을 고를때 벡터랑 평면이랑 비교하는데서 임계치를 조절한다.
#endif

	VECTOR3		To	=	*pCurrentPivot + *pVelocity;

	//	접할때 타원의 중점과 접평면간의 거리.
	float	a	=	pCollisionInfo->vMeetPivot * pCollisionInfo->MeetPlane.v3Up	+	pCollisionInfo->MeetPlane.D;
	if( a <= 0.0f)
		_asm int 3;		// 이미 평면을 뜷고 들어갔다는 뜻인가..

	//	이 타원체의 목적지와 평면의 거리.
	float	b	=	To	*	pCollisionInfo->MeetPlane.v3Up		+		pCollisionInfo->MeetPlane.D;

	
	//	면의 방향벡터로 pTo를 밀어야 할 크기.
	float	c	=	a	-	b;//	+	1.0f;

	//	c가 적용된 방향벡터.
	VECTOR3		Push;
	Push	=	pCollisionInfo->MeetPlane.v3Up		*	c;

	// 결과 밀려있는 좌표는,
	*pOut	=	To	+	Push;

	// 리턴.
	return	TRUE;
}
