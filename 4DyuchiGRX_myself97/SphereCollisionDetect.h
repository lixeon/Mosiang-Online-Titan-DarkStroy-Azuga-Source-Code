/*
*/
#include "../4DyuchiGRX_common/typedef.h"

// 움직이는 스피어가 삼각형의 어디와 만나나.?
BOOL	WhereDidMovingSphereMeetTri(  VECTOR3* OUT pCandidate, float* pT, VECTOR3* pWhereMeet, VECTOR3* pSFrom, VECTOR3* pSTo, float fRadius, VECTOR3* pTri);
// 만나는 위치, 만날때의 t, 만날때의 슬라이딩 플레인, 원의 중점, 원의 to, 원 반지름, 삼각형.
// 만나는 위치의 원중심, t, 만나는 위치(평면구하기), 스피어프럼, 스피어투, 반지름, 삼각형)

// 움직이는 스피어가 스피어의 어디와 만나나.?
BOOL	WhereDidMovingSphereMeetSphere( VECTOR3* pCandidate, float* pT, VECTOR3* pWhereMeet, VECTOR3* pSFrom, VECTOR3* pSTo, float fSRadius, VECTOR3* pTSPivot, float fTSRadius);
// 만나는 위치, 만날때 t, 만나는 접점, 움직이는 스피어from, to, 반지름, 고정된 스피어pivot, 반지름.

// 푸쉬. 푸쉬.
void	SpherePushDir( VECTOR3* OUT pCandidate, VECTOR3* IN pFrom, VECTOR3* IN pTo, VECTOR3* IN pMeet, VECTOR3* IN pMeetPos, PLANE* IN pSlidingPlane, BOUNDING_VOLUME* pBV);
