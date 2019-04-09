
#ifndef	_CollisionTestMovingSphereMeetTriangle_h_
#define _CollisionTestMovingSphereMeetTriangle_h_

#include	"../myself97common.h"

struct	MOVING_SPHERE
{
	VECTOR3		From;
	float		fRadius;
	VECTOR3		Velocity;
};

BOOL	CollisionTestMovingSphereMeetTriangle( DWORD* pdwOutComponentType, VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pTriangle);




#endif
