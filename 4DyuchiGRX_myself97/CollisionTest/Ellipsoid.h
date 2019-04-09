/*
	AAEllipsoidCollisionTest.h
*/

#ifndef	_Ellipsoid_h_
#define	_Ellipsoid_h_

#include "../../4DyuchiGRX_common/typedef.h"



struct	ELLIPSOID
{
	// 여기는 외부에서 입력 받는것.
	VECTOR3		P;			// 피봇, 절대값.
	float		w;			// x, z쪽 지름.
	float		h;			// y축쪽 높이.

	// 여기서부터는 입력 받은걸로 만드는것.
	float		hperw;			// h/w 비.
	float		k;				// 중점과 위,아래중점과의 거리.
	VECTOR3		U;			// 타원의 윗쪽 중점, P에서의 상대 위치가 아니다.
	VECTOR3		L;			// 타원의 아랫쪽 중점, P에서의 상대 위치가 아니다.

	void	Create( const VECTOR3& Pivot, float fWidth, float fHeight);
	void	SetPosition( const VECTOR3& Pivot);			// 옮기기. U와 L도 갱신된다.
	
};

#endif