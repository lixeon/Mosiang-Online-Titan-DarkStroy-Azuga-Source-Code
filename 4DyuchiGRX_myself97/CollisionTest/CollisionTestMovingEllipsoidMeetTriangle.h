
#ifndef	_CollisionTestMovingEllipsoidMeetTriangle_h_
#define _CollisionTestMovingEllipsoidMeetTriangle_h_

#include	"../myself97common.h"

/*
// 축방향 타원체.
struct	AAELLIPSOID
{
	VECTOR3		P;			// 피봇, 절대값.
	float		w;			// x, z쪽 지름.
	float		h;			// y축쪽 높이.
};
*/
struct		MOVING_ELLIPSOID
{
	VECTOR3		From;
	float		fWidth;
	float		fHeight;
	VECTOR3		Velocity;
};
/*
struct		ELLIPSOID_COLLISION_INFO
{
	VECTOR3		MeetPivot;		// 만났을때의 피봇.
	float		fMeetTime;		// 만났을때의 from-to간 시점.

	VECTOR3		WhereMeet;		// 만난 접점.
	PLANE		MeetPlane;		// 만난 접점의 평면.
};
*/
// 움직이는 타원과 삼각형이 주어졌을때 둘이 만나는 시점을 구한다.
// 만날 경우 TRUE, 안만날경우 FALSE를 리턴한다.
BOOL	CollisionTestMovingEllipsoidMeetTriangle( COLLISION_RESULT* pOut, MOVING_ELLIPSOID* pEllipsoid, VECTOR3* pTriangle);

// 타원과 타원위의 한 점이 주어졌을 때 접평면을 구한다.
// 타원과 주어진 점이 떨어져 있을 경우 FALSE 리턴 예정이다.
BOOL	CalculatePLANEOnEllipsoidWithVertex( PLANE* pOutPlane, AAELLIPSOID* pEllipsoid, VECTOR3* pVertex);

// 움직이는 타원과 대상 타원이 주어졌을 때 둘이 만나는 시점을 구한다.
// 만날 경우 TRUE, 안만날 경우 FALSE를 리턴한다.


// 이동하는 타원을 주어진 평면을 기준으로 민다.
BOOL	PushEllipsoidFromPLANE( VECTOR3* pOut, COLLISION_RESULT* pCollisionInfo, VECTOR3* pCurrentPivot, VECTOR3* pTo);


#endif
