/*
	Myself97Common.h
	일반적으로 쓰이는 함수 모음.
*/

#ifndef	_Myself97Common_H_
#define	_Myself97Common_H_


#include "../4DyuchiGRX_common/typedef.h"
#include <limits.h>

// 충돌 처리 리턴값 정의.
// 리턴값의 형은 COLLISIONTYPE이고, 리턴되는 값은 아래에 디파인 되는 값.
#define		COLLISIONTYPE						int

#define		COLLISIONTYPE_Error					-1

#define		COLLISIONTYPE_None					0
#define		COLLISIONTYPE_Vertex				1
#define		COLLISIONTYPE_Line					2
#define		COLLISIONTYPE_LineSegment			3
#define		COLLISIONTYPE_Plane					4
#define		COLLISIONTYPE_Triangle				5


// aabb와 aabb가 만나는가.?
bool		IsAABBMeetAABB( MAABB& a, MAABB& b);

//	스피어와 스피어가 만나는가.?
bool		IsSphereMeetSphere( BOUNDING_SPHERE& bs1, BOUNDING_SPHERE& bs2);

// 스피어와 박스가 만나는가?
bool		IsSphereMeetAABB( BOUNDING_SPHERE& bs, MAABB& Box);

// 0: 걸친다. 1: 면의 앞에 있다. -1: 면의 뒤에 있다.
int			WhereIsAABBFromViewVolume( MAABB& b, VIEW_VOLUME& vv);
// 0: 걸친다. 1: 볼륨의 안에 완전히 들어간다. -1: 볼륨에서 완전히 벗어난다.
//int			WhereIsBBoxFrom
//	MAABB에서 BOUNDING_SPHERE를 만들어주는 함수.
BOUNDING_SPHERE		GetSphereFromAABB( MAABB& Box);
int			WhereIsSphereFromViewVolume( BOUNDING_SPHERE& Sphere, VIEW_VOLUME& vv);
// 0: 걸친다. 1: 볼륨 안에 완전히 들어간다. -1: 볼륨에서 완전히 벗어난다.
int		WhereIsAABBFromBoundingSphere( MAABB& b, BOUNDING_SPHERE& bs);
// 0: 걸친다. 1: 볼륨 안에 완전히 들어간다. -1: 볼륨에서 완전히 벗어난다.
int			WhereISBOUNDING_BOXFromViewVolume( BOUNDING_BOX& bb, VIEW_VOLUME& vv);
// 0: 걸친다. 1: 볼륨 안에 완전히 들어간다. -1: 볼륨에서 완전히 벗어난다.
int		BOUNDING_CAPSULE_Meet_BOUNDING_SPHERE( BOUNDING_CAPSULE& bc, BOUNDING_SPHERE& bs);
// true, false 만난다 안만난다.
// 스피어와 직선이 어디서 만나는가.? 리턴0이면 안만나고, 1이면 한점에서, 2면 두점에서 만난다.
DWORD WhereDidSphereMeetLine( VECTOR3* OUT pCandidate1, float* OUT pT1, VECTOR3* OUT pCandidate2, float* OUT pT2, VECTOR3* IN pLFrom, VECTOR3* IN pLTo, VECTOR3* IN pSPivot, float fSRadius);



//	라인 From-To 위에서 점 v와 가장 가까운 점을 찾는다.
//	LineFrom과 LineTo가 같지 않다고 가정한다.



BOOL		IsVECTOR3Same( VECTOR3* pV1, VECTOR3* pV2);

void		GetTriMiddle( VECTOR3* pMid, VECTOR3* pTri);

// 삼각형과, 그 평면위에 있을거라고 가정하는 버텍스가 만나는가.
// 리턴값은 만나면 TRUE이고, WhereMeet는 벡터01*x + 벡터02*y로 나타내는 텍스쳐좌표같은거.
BOOL		IsTriMeetVertex(  VECTOR3* pTri, VECTOR3* pVertex);



#ifdef	_DEBUG

inline	void	CheckVectorArray( VECTOR3* pArray, DWORD dwCount)
{
	float*	p	=	(float*)pArray;
	for( DWORD i = 0; i < dwCount*3; i++)
	{
		if( p[i] < -80000.0f || p[i] > 80000.0f)
		{
			_asm int 3;
			return;
		}
	}
	return;
}
#else

#define CheckVectorArray(x,count)
#endif


// 인라인화 한건, 디버그모드일 때 함수호출을 없애보려고... -_-;

#ifdef	_DEBUG
inline void	PrintfDebugString( char* szFormat, ...)
{
	va_list argptr;
	char cBuf[1024];


	va_start( argptr, szFormat );
	vsprintf( cBuf, szFormat, argptr );
	va_end( argptr );

	OutputDebugString( cBuf);

}

#define	PrintfDebugCodeLine() PrintfDebugString( "Code = %s, Line = %d\n", __FILE__, __LINE__)



#else

#define	PrintfDebugString /**/

#endif


//	 주어진 삼각형이 평면의 어디에 있는가.?
//	명백히 앞에 있으면 2, 명백히 뒤에 있으면 -2, 평면이 겹쳐있는데 노말이 같은경우 1, 노말이 반대인 경우 -1, 
//	평면과 삼각형이 교차하고 있으면 0 리턴.
int		WhereIsTriangleFromPLANE(VECTOR3 *pTri, PLANE *pPlane);

// pTri로 주어진 삼각형의 AABB를 구한다.
// 혹시 부피가 없다면 FALSE를 리턴.
BOOL	MakeAABBFromTri( MAABB* pOutBox, VECTOR3* pTri);
//BOOL ClipSphereWithViewVolume(VIEW_VOLUME* pVolume,BOUNDING_SPHERE* pSphere);

// 완전히 만나면 +1, 딱 접하면 0, 안만나면 -1
int	BoundingCapsuleMeetPlane( BOUNDING_CAPSULE* pBC, PLANE* pPlane);

#endif

// 여치가 주석처리했음...GFunc에 있는 함수로 대체
//void	MakePLANE( PLANE* pP, VECTOR3* pv1, VECTOR3* pv2, VECTOR3* pv3);
//void			FindNearestVertexOnLine( VECTOR3* pOut, float* pT, VECTOR3& LineFrom, VECTOR3& LineTo, VECTOR3& v);
